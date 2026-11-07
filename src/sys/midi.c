/*
 * The MIDI streams of WinMM for the port, played by FluidSynth.
 *
 * The game's player, Microsoft's MIDIPLYR sample, queues buffers of
 * events with midiStreamOut and refills each one that comes back to
 * its callback. Here a sequencer reads the queued events in the audio
 * thread of SDL's stream for music, at their times, and renders the
 * synthesizer between them. A buffer it finishes goes on a list that
 * the game's thread drains, in the message pump and in Sleep, calling
 * the game's callback there, so that the game's state changes in its
 * own thread. midiStreamStop and midiOutReset return every queued
 * buffer to the callback before they return, as the game's seqStop
 * waits for the count of its buffers to reach zero. The one lock is the
 * music stream's, which SDL holds while it renders, and while the layer
 * holds it, it runs its own code alone. A replay runs the sequencer
 * alone on its virtual clock, so that the callbacks come at the same
 * times on every platform.
 */

#include <string.h>

#include <SDL3/SDL.h>
#include <fluidsynth.h>
#include <windows.h>
#include <mmsystem.h>

#include "sys.h"

enum {
	Rate = 44100,
	Chunk = 256,	/* the frames rendered at once at most */
	Maxqueue = 64,
	Streammagic = 0x4d696469
};

typedef void (*Midiproc)(HMIDISTRM, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);

typedef struct Stream Stream;
struct Stream {
	uint32_t magic;
	int open;
	Midiproc proc;
	DWORD_PTR instance;
	int running;	/* playing: between midiStreamRestart and a pause or a stop */
	DWORD division;	/* ticks per quarter note */
	DWORD tempo;	/* microseconds per quarter note */
	LPMIDIHDR queue[Maxqueue];	/* the buffers midiStreamOut queued */
	int nqueue;
	DWORD off;	/* the offset of the next event in queue[0] */
	int havedelta;	/* whether wait holds the next event's delta */
	double wait;	/* frames before the next event */
	DWORD ticks;	/* the ticks the stream has played */
	LPMIDIHDR done[Maxqueue];	/* the finished buffers for the game's thread */
	int ndone;
};

static Stream stream;
static fluid_settings_t *settings;
static fluid_synth_t *synth;
static SDL_AudioStream *audio;
static float basegain = 0.4f;
static DWORD lastclock;	/* the virtual clock that a replay's sequencer reached */

static void
lock(void)
{
	if (audio != NULL)
		SDL_LockAudioStream(audio);
}

static void
unlock(void)
{
	if (audio != NULL)
		SDL_UnlockAudioStream(audio);
}

static uint32_t
word(const BYTE *p)
{
	return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

/* shortmsg sends a short MIDI message to the synthesizer. */
static void
shortmsg(uint32_t m)
{
	int status, chan, d1, d2;

	if (synth == NULL)
		return;
	status = m & 0xf0;
	chan = m & 0x0f;
	d1 = m >> 8 & 0x7f;
	d2 = m >> 16 & 0x7f;
	switch (status) {
	case 0x80:
		fluid_synth_noteoff(synth, chan, d1);
		break;
	case 0x90:
		if (d2 == 0)
			fluid_synth_noteoff(synth, chan, d1);
		else
			fluid_synth_noteon(synth, chan, d1, d2);
		break;
	case 0xa0:
		fluid_synth_key_pressure(synth, chan, d1, d2);
		break;
	case 0xb0:
		fluid_synth_cc(synth, chan, d1, d2);
		break;
	case 0xc0:
		fluid_synth_program_change(synth, chan, d1);
		break;
	case 0xd0:
		fluid_synth_channel_pressure(synth, chan, d1);
		break;
	case 0xe0:
		fluid_synth_pitch_bend(synth, chan, d1 | d2 << 7);
		break;
	default:
		break;
	}
}

/* silence turns every note off and leaves the controllers as they are. */
static void
silence(void)
{
	if (synth != NULL)
		fluid_synth_all_notes_off(synth, -1);
}

/* finish moves the first queued buffer to the finished ones. */
static void
finish(Stream *s)
{
	LPMIDIHDR h;

	h = s->queue[0];
	memmove(s->queue, s->queue + 1, (s->nqueue - 1) * sizeof s->queue[0]);
	s->nqueue--;
	h->dwFlags = (h->dwFlags & ~MHDR_INQUEUE) | MHDR_DONE;
	if (s->ndone < Maxqueue)
		s->done[s->ndone++] = h;
	s->off = 0;
	s->havedelta = 0;
}

/* framesof returns the frames that delta ticks last at the stream's tempo. */
static double
framesof(Stream *s, DWORD delta)
{
	if (s->division == 0)
		return 0;
	return (double)delta * s->tempo / s->division * Rate / 1e6;
}

/*
 * next readies the stream's next event, finishing the buffers it has
 * read to the end, and reports whether there is one.
 */
static int
next(Stream *s)
{
	LPMIDIHDR h;

	while (s->nqueue > 0) {
		h = s->queue[0];
		if (s->off + 12 <= h->dwBytesRecorded) {
			if (!s->havedelta) {
				s->wait += framesof(s, word((BYTE *)h->lpData + s->off));
				s->havedelta = 1;
			}
			return 1;
		}
		finish(s);
	}
	return 0;
}

/* dispatch plays the stream's next event, whose time has come. */
static void
dispatch(Stream *s)
{
	LPMIDIHDR h;
	const BYTE *p;
	uint32_t delta, ev, n;

	h = s->queue[0];
	p = (BYTE *)h->lpData + s->off;
	delta = word(p);
	ev = word(p + 8);
	s->ticks += delta;
	s->havedelta = 0;
	if (ev & MEVT_F_LONG) {
		n = ev & 0xffffff;
		if (s->off + 12 + n > h->dwBytesRecorded) {
			s->off = h->dwBytesRecorded;
			return;
		}
		if (MEVT_EVENTTYPE(ev & ~MEVT_F_CALLBACK) == MEVT_LONGMSG && synth != NULL &&
		    n >= 2 && p[12] == 0xf0 && p[12 + n - 1] == 0xf7)
			fluid_synth_sysex(synth, (const char *)p + 13, (int)n - 2, NULL, NULL, NULL, 0);
		s->off += 12 + ((n + 3) & ~(uint32_t)3);
		return;
	}
	s->off += 12;
	switch (MEVT_EVENTTYPE(ev & ~MEVT_F_CALLBACK)) {
	case MEVT_SHORTMSG:
		shortmsg(MEVT_EVENTPARM(ev));
		break;
	case MEVT_TEMPO:
		s->tempo = MEVT_EVENTPARM(ev);
		break;
	default:
		break;
	}
}

/*
 * sequence plays the stream for frames frames, rendering the
 * synthesizer into out, two channels interleaved, when out points to a
 * buffer.
 */
static void
sequence(int frames, float *out)
{
	Stream *s;
	int done, step;

	s = &stream;
	done = 0;
	while (done < frames) {
		step = frames - done;
		if (s->open && s->running && next(s)) {
			if (s->wait < 1) {
				s->wait = 0;
				dispatch(s);
				continue;
			}
			if (s->wait < step)
				step = (int)s->wait;
		}
		if (out != NULL) {
			if (synth != NULL)
				fluid_synth_write_float(synth, step, out, 2 * done, 2, out, 2 * done + 1, 2);
			else
				memset(out + 2 * done, 0, 2 * step * sizeof out[0]);
		}
		if (s->open && s->running && s->havedelta)
			s->wait -= step;
		done += step;
	}
}

static void SDLCALL
render(void *arg, SDL_AudioStream *as, int extra, int total)
{
	float buf[2 * Chunk];
	int frames, n;

	(void)arg, (void)total;
	frames = extra / (int)(2 * sizeof buf[0]);
	while (frames > 0) {
		n = frames < Chunk ? frames : Chunk;
		sequence(n, buf);
		SDL_PutAudioStreamData(as, buf, n * 2 * (int)sizeof buf[0]);
		frames -= n;
	}
}

/* soundfont finds the SoundFont: the registry's, else the one beside the game. */
static void
soundfont(char *path, size_t n)
{
	if (sysregstring("SoundFont", path, n) && path[0] != '\0')
		return;
	SDL_snprintf(path, n, "%sgeneraluser-gs.sf2", sysgamedir);
}

int
midiinit(void)
{
	SDL_AudioSpec spec;
	char path[Maxpath];

	if (replaying) {
		lastclock = sysclock();
		return 1;
	}
	fluid_set_log_function(FLUID_INFO, NULL, NULL);
	fluid_set_log_function(FLUID_DBG, NULL, NULL);
	settings = new_fluid_settings();
	if (settings == NULL)
		return 0;
	fluid_settings_setnum(settings, "synth.sample-rate", Rate);
	fluid_settings_setnum(settings, "synth.gain", basegain);
	synth = new_fluid_synth(settings);
	if (synth == NULL) {
		SDL_Log("teudogar: music: no synthesizer");
		return 0;
	}
	soundfont(path, sizeof path);
	if (fluid_synth_sfload(synth, path, 1) == FLUID_FAILED)
		SDL_Log("teudogar: music: %s: cannot load the SoundFont", path);
	spec.format = SDL_AUDIO_F32;
	spec.channels = 2;
	spec.freq = Rate;
	audio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, render, NULL);
	if (audio == NULL) {
		SDL_Log("teudogar: music: %s", SDL_GetError());
		return 0;
	}
	SDL_ResumeAudioStreamDevice(audio);
	return 1;
}

void
midiquit(void)
{
	if (audio != NULL)
		SDL_DestroyAudioStream(audio);
	audio = NULL;
	if (synth != NULL)
		delete_fluid_synth(synth);
	synth = NULL;
	if (settings != NULL)
		delete_fluid_settings(settings);
	settings = NULL;
}

/* deliver calls the game's callback with each buffer of list, in order. */
static void
deliver(LPMIDIHDR *list, int n)
{
	int i;

	for (i = 0; i < n; i++)
		if (stream.proc != NULL)
			stream.proc((HMIDISTRM)&stream, MOM_DONE, stream.instance, (DWORD_PTR)list[i], 0);
}

/* mididrain hands the finished buffers to the game's callback, in the game's thread. */
void
mididrain(void)
{
	LPMIDIHDR list[Maxqueue];
	DWORD now;
	int n;

	if (!stream.open && stream.ndone == 0)
		return;
	if (replaying && audio == NULL) {
		now = sysclock();
		if (now != lastclock)
			sequence((int)((now - lastclock) * (Rate / 1000)), NULL);
		lastclock = now;
	}
	lock();
	n = stream.ndone;
	memcpy(list, stream.done, n * sizeof list[0]);
	stream.ndone = 0;
	unlock();
	deliver(list, n);
}

static Stream *
streamof(HANDLE h)
{
	Stream *s;

	s = h;
	if (s != &stream || s->magic != Streammagic || !s->open)
		return NULL;
	return s;
}

MMRESULT
midiStreamOpen(LPHMIDISTRM out, UINT *dev, DWORD n, DWORD_PTR cb, DWORD_PTR inst, DWORD flags)
{
	(void)dev, (void)n;
	if (!replaying && audio == NULL)
		return MMSYSERR_NODRIVER;
	if (stream.open)
		return MMSYSERR_ALLOCATED;
	lock();
	memset(&stream, 0, sizeof stream);
	stream.magic = Streammagic;
	stream.open = 1;
	stream.proc = (flags & CALLBACK_TYPEMASK) == CALLBACK_FUNCTION ? (Midiproc)cb : NULL;
	stream.instance = inst;
	stream.division = 96;
	stream.tempo = 500000;
	unlock();
	lastclock = sysclock();
	*out = (HMIDISTRM)&stream;
	if (stream.proc != NULL)
		stream.proc(*out, MOM_OPEN, inst, 0, 0);
	return MMSYSERR_NOERROR;
}

MMRESULT
midiStreamClose(HMIDISTRM h)
{
	Stream *s;

	s = streamof(h);
	if (s == NULL)
		return MMSYSERR_INVALHANDLE;
	if (s->nqueue > 0)
		return MIDIERR_STILLPLAYING;
	lock();
	s->open = 0;
	s->running = 0;
	silence();
	unlock();
	if (s->proc != NULL)
		s->proc(h, MOM_CLOSE, s->instance, 0, 0);
	return MMSYSERR_NOERROR;
}

MMRESULT
midiStreamOut(HMIDISTRM h, LPMIDIHDR hdr, UINT size)
{
	Stream *s;

	(void)size;
	s = streamof(h);
	if (s == NULL)
		return MMSYSERR_INVALHANDLE;
	if (!(hdr->dwFlags & MHDR_PREPARED))
		return MIDIERR_UNPREPARED;
	lock();
	if (s->nqueue == Maxqueue) {
		unlock();
		return MMSYSERR_NOMEM;
	}
	hdr->dwFlags = (hdr->dwFlags & ~MHDR_DONE) | MHDR_INQUEUE | MHDR_ISSTRM;
	s->queue[s->nqueue++] = hdr;
	unlock();
	return MMSYSERR_NOERROR;
}

MMRESULT
midiStreamRestart(HMIDISTRM h)
{
	Stream *s;

	s = streamof(h);
	if (s == NULL)
		return MMSYSERR_INVALHANDLE;
	lock();
	s->running = 1;
	unlock();
	lastclock = sysclock();
	return MMSYSERR_NOERROR;
}

MMRESULT
midiStreamPause(HMIDISTRM h)
{
	Stream *s;

	s = streamof(h);
	if (s == NULL)
		return MMSYSERR_INVALHANDLE;
	lock();
	s->running = 0;
	silence();
	unlock();
	return MMSYSERR_NOERROR;
}

/* halt returns every queued buffer to the callback, and the stream to tick 0. */
static MMRESULT
halt(HANDLE h)
{
	LPMIDIHDR list[2 * Maxqueue];
	Stream *s;
	int i, n;

	s = streamof(h);
	if (s == NULL)
		return MMSYSERR_INVALHANDLE;
	lock();
	s->running = 0;
	n = s->ndone;
	memcpy(list, s->done, n * sizeof list[0]);
	s->ndone = 0;
	for (i = 0; i < s->nqueue; i++) {
		s->queue[i]->dwFlags = (s->queue[i]->dwFlags & ~MHDR_INQUEUE) | MHDR_DONE;
		list[n++] = s->queue[i];
	}
	s->nqueue = 0;
	s->off = 0;
	s->havedelta = 0;
	s->wait = 0;
	s->ticks = 0;
	silence();
	unlock();
	deliver(list, n);
	return MMSYSERR_NOERROR;
}

MMRESULT
midiStreamStop(HMIDISTRM h)
{
	return halt(h);
}

MMRESULT
midiOutReset(HMIDIOUT h)
{
	return halt(h);
}

MMRESULT
midiStreamPosition(HMIDISTRM h, LPMMTIME t, UINT size)
{
	Stream *s;

	(void)size;
	s = streamof(h);
	if (s == NULL)
		return MMSYSERR_INVALHANDLE;
	lock();
	if (t->wType == TIME_MS)
		t->u.ms = (DWORD)((double)s->ticks * s->tempo / (s->division ? s->division : 1) / 1000);
	else {
		t->wType = TIME_TICKS;
		t->u.ticks = s->ticks;
	}
	unlock();
	return MMSYSERR_NOERROR;
}

MMRESULT
midiStreamProperty(HMIDISTRM h, LPBYTE data, DWORD flags)
{
	LPMIDIPROPTIMEDIV td;
	LPMIDIPROPTEMPO tp;
	Stream *s;

	s = streamof(h);
	if (s == NULL)
		return MMSYSERR_INVALHANDLE;
	lock();
	if (flags & MIDIPROP_TIMEDIV) {
		td = (LPMIDIPROPTIMEDIV)data;
		if (flags & MIDIPROP_SET)
			s->division = td->dwTimeDiv;
		else
			td->dwTimeDiv = s->division;
	} else if (flags & MIDIPROP_TEMPO) {
		tp = (LPMIDIPROPTEMPO)data;
		if (flags & MIDIPROP_SET)
			s->tempo = tp->dwTempo;
		else
			tp->dwTempo = s->tempo;
	}
	unlock();
	return MMSYSERR_NOERROR;
}

MMRESULT
midiOutPrepareHeader(HMIDIOUT h, LPMIDIHDR hdr, UINT size)
{
	(void)h, (void)size;
	hdr->dwFlags |= MHDR_PREPARED;
	return MMSYSERR_NOERROR;
}

MMRESULT
midiOutUnprepareHeader(HMIDIOUT h, LPMIDIHDR hdr, UINT size)
{
	(void)h, (void)size;
	if (hdr->dwFlags & MHDR_INQUEUE)
		return MIDIERR_STILLPLAYING;
	hdr->dwFlags &= ~MHDR_PREPARED;
	return MMSYSERR_NOERROR;
}

/* midiOutSetVolume takes the left volume in the low word and the right in the high, each to 0xffff. */
MMRESULT
midiOutSetVolume(HMIDIOUT h, DWORD v)
{
	float g;

	(void)h;
	g = ((v & 0xffff) + (v >> 16 & 0xffff)) / 2.0f / 65535.0f;
	if (synth != NULL)
		fluid_synth_set_gain(synth, basegain * g);
	return MMSYSERR_NOERROR;
}

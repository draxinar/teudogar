/*
 * DirectSound for the port: the game's sound buffers, 8-bit mono PCM
 * at 11,025 Hz, mixed in software into one SDL audio stream. A buffer
 * plays once, or loops, from its position, at the volume that
 * SetVolume gives in hundredths of a decibel. In a replay the virtual
 * clock ends a buffer, since the game's logic tests whether it plays, and
 * the audio thread keeps real time.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <windows.h>
#include <dsound.h>

#include "sys.h"

enum {
	Rate = 11025,
	Maxbuffers = 128,
	Chunk = 512	/* the frames that one call of the callback mixes at most */
};

typedef struct Buffer Buffer;
struct Buffer {
	IDirectSoundBuffer iface;
	int primary;
	BYTE *data;
	DWORD size;
	DWORD pos;
	int playing;
	int looping;
	DWORD ends;	/* in a replay, the virtual time when it ends */
	float gain;
};

static SDL_AudioStream *stream;
static Buffer *buffers[Maxbuffers];
static int nbuffers;

static const IDirectSoundVtbl dsvtbl;
static const IDirectSoundBufferVtbl bufvtbl;
static IDirectSound ds = { &dsvtbl };

static void
lock(void)
{
	if (stream != NULL)
		SDL_LockAudioStream(stream);
}

static void
unlock(void)
{
	if (stream != NULL)
		SDL_UnlockAudioStream(stream);
}

/* mix adds the playing buffers into the stream, as the device asks for them. */
static void SDLCALL
mix(void *arg, SDL_AudioStream *s, int extra, int total)
{
	float out[Chunk];
	Buffer *b;
	int i, j, n;

	(void)arg, (void)total;
	extra /= (int)sizeof out[0];
	while (extra > 0) {
		n = extra < Chunk ? extra : Chunk;
		memset(out, 0, n * sizeof out[0]);
		for (i = 0; i < nbuffers; i++) {
			b = buffers[i];
			if (b == NULL || !b->playing || b->size == 0)
				continue;
			for (j = 0; j < n; j++) {
				if (b->pos >= b->size) {
					if (!b->looping) {
						if (!replaying)
							b->playing = 0;
						break;
					}
					b->pos = 0;
				}
				out[j] += b->gain * ((int)b->data[b->pos++] - 128) / 128.0f;
			}
		}
		for (j = 0; j < n; j++) {
			if (out[j] > 1.0f)
				out[j] = 1.0f;
			if (out[j] < -1.0f)
				out[j] = -1.0f;
		}
		SDL_PutAudioStreamData(s, out, n * (int)sizeof out[0]);
		extra -= n;
	}
}

int
soundinit(void)
{
	SDL_AudioSpec spec;

	spec.format = SDL_AUDIO_F32;
	spec.channels = 1;
	spec.freq = Rate;
	stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, mix, NULL);
	if (stream == NULL) {
		SDL_Log("teudogar: sound: %s", SDL_GetError());
		return 0;
	}
	SDL_ResumeAudioStreamDevice(stream);
	return 1;
}

void
soundquit(void)
{
	if (stream != NULL)
		SDL_DestroyAudioStream(stream);
	stream = NULL;
}

static ULONG
dsrelease(IDirectSound *p)
{
	(void)p;
	return 0;
}

static HRESULT
dscreatebuffer(IDirectSound *p, LPDSBUFFERDESC d, LPDIRECTSOUNDBUFFER *out, IUnknown *u)
{
	Buffer *b;
	int i;

	(void)p, (void)u;
	b = calloc(1, sizeof *b);
	if (b == NULL)
		return DSERR_OUTOFMEMORY;
	b->iface.lpVtbl = &bufvtbl;
	b->gain = 1.0f;
	if (d->dwFlags & DSBCAPS_PRIMARYBUFFER)
		b->primary = 1;
	else {
		if (d->dwBufferBytes == 0 || d->dwBufferBytes > 1 << 24) {
			free(b);
			return DSERR_INVALIDPARAM;
		}
		b->data = calloc(1, d->dwBufferBytes);
		if (b->data == NULL) {
			free(b);
			return DSERR_OUTOFMEMORY;
		}
		memset(b->data, 128, d->dwBufferBytes);
		b->size = d->dwBufferBytes;
		lock();
		for (i = 0; i < nbuffers && buffers[i] != NULL; i++)
			;
		if (i == Maxbuffers) {
			unlock();
			free(b->data);
			free(b);
			return DSERR_OUTOFMEMORY;
		}
		buffers[i] = b;
		if (i == nbuffers)
			nbuffers++;
		unlock();
	}
	*out = &b->iface;
	return DS_OK;
}

static HRESULT
dsgetcaps(IDirectSound *p, LPDSCAPS c)
{
	(void)p;
	c->dwFlags = DSCAPS_PRIMARYMONO | DSCAPS_PRIMARYSTEREO | DSCAPS_PRIMARY8BIT |
		DSCAPS_PRIMARY16BIT | DSCAPS_SECONDARYMONO | DSCAPS_SECONDARYSTEREO |
		DSCAPS_SECONDARY8BIT | DSCAPS_SECONDARY16BIT;
	c->dwMinSecondarySampleRate = 100;
	c->dwMaxSecondarySampleRate = 100000;
	c->dwPrimaryBuffers = 1;
	return DS_OK;
}

static HRESULT
dscooperate(IDirectSound *p, HWND w, DWORD level)
{
	(void)p, (void)w, (void)level;
	return DS_OK;
}

static const IDirectSoundVtbl dsvtbl = {
	dsrelease,
	dscreatebuffer,
	dsgetcaps,
	dscooperate,
};

HRESULT
DirectSoundCreate(GUID *g, LPDIRECTSOUND *out, IUnknown *u)
{
	(void)g, (void)u;
	if (stream == NULL)
		return DSERR_NODRIVER;
	*out = &ds;
	return DS_OK;
}

static Buffer *
bufof(IDirectSoundBuffer *p)
{
	return (Buffer *)p;
}

static ULONG
bufrelease(IDirectSoundBuffer *p)
{
	Buffer *b;
	int i;

	b = bufof(p);
	lock();
	for (i = 0; i < nbuffers; i++)
		if (buffers[i] == b)
			buffers[i] = NULL;
	unlock();
	free(b->data);
	free(b);
	return 0;
}

static HRESULT
bufgetcaps(IDirectSoundBuffer *p, LPDSBCAPS c)
{
	c->dwFlags = DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLDEFAULT;
	c->dwBufferBytes = bufof(p)->size;
	return DS_OK;
}

static HRESULT
bufgetstatus(IDirectSoundBuffer *p, LPDWORD st)
{
	Buffer *b;

	b = bufof(p);
	lock();
	if (replaying && b->playing && !b->looping && (Sint32)(sysclock() - b->ends) >= 0)
		b->playing = 0;
	*st = b->playing ? DSBSTATUS_PLAYING : 0;
	if (b->playing && b->looping)
		*st |= DSBSTATUS_LOOPING;
	unlock();
	return DS_OK;
}

static HRESULT
buflock(IDirectSoundBuffer *p, DWORD off, DWORD n, LPVOID *p1, LPDWORD n1, LPVOID *p2, LPDWORD n2, DWORD flags)
{
	Buffer *b;

	(void)flags;
	b = bufof(p);
	if (b->primary || off >= b->size)
		return DSERR_INVALIDPARAM;
	if (n > b->size - off)
		n = b->size - off;
	*p1 = b->data + off;
	*n1 = n;
	if (p2 != NULL)
		*p2 = NULL;
	if (n2 != NULL)
		*n2 = 0;
	return DS_OK;
}

static HRESULT
bufplay(IDirectSoundBuffer *p, DWORD r1, DWORD r2, DWORD flags)
{
	Buffer *b;

	(void)r1, (void)r2;
	b = bufof(p);
	if (b->primary)
		return DS_OK;
	lock();
	b->playing = 1;
	b->looping = (flags & DSBPLAY_LOOPING) != 0;
	b->ends = sysclock() + (DWORD)((Uint64)(b->size - b->pos) * 1000 / Rate);
	unlock();
	return DS_OK;
}

static HRESULT
bufrestore(IDirectSoundBuffer *p)
{
	(void)p;
	return DS_OK;
}

static HRESULT
bufsetpos(IDirectSoundBuffer *p, DWORD pos)
{
	Buffer *b;

	b = bufof(p);
	lock();
	b->pos = pos < b->size ? pos : b->size;
	unlock();
	return DS_OK;
}

static HRESULT
bufsetformat(IDirectSoundBuffer *p, LPCWAVEFORMATEX f)
{
	(void)p, (void)f;
	return DS_OK;
}

/* bufsetvolume takes hundredths of a decibel, from -10,000 to 0. */
static HRESULT
bufsetvolume(IDirectSoundBuffer *p, LONG v)
{
	Buffer *b;

	if (v > DSBVOLUME_MAX || v < DSBVOLUME_MIN)
		return DSERR_INVALIDPARAM;
	b = bufof(p);
	lock();
	b->gain = v <= DSBVOLUME_MIN ? 0.0f : (float)pow(10.0, v / 2000.0);
	unlock();
	return DS_OK;
}

static HRESULT
bufstop(IDirectSoundBuffer *p)
{
	Buffer *b;

	b = bufof(p);
	lock();
	b->playing = 0;
	unlock();
	return DS_OK;
}

static HRESULT
bufunlock(IDirectSoundBuffer *p, LPVOID p1, DWORD n1, LPVOID p2, DWORD n2)
{
	(void)p, (void)p1, (void)n1, (void)p2, (void)n2;
	return DS_OK;
}

static const IDirectSoundBufferVtbl bufvtbl = {
	bufrelease,
	bufgetcaps,
	bufgetstatus,
	buflock,
	bufplay,
	bufrestore,
	bufsetpos,
	bufsetformat,
	bufsetvolume,
	bufstop,
	bufunlock,
};

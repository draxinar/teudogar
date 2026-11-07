#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <dsound.h>

#include "base.h"

/*
 * The music player is Microsoft's MIDIPLYR sample (smf.c, smfread.c,
 * sequence.c) with the file read from memory. Its functions keep the
 * sample's names, which the log strings abbreviate.
 */

typedef DWORD TICKS;
typedef DWORD SMFRESULT;
typedef struct Tempo Tempo;
typedef struct Track Track;
typedef struct Smf Smf;
typedef struct Smfopen Smfopen;
typedef struct Smfinfo Smfinfo;

DECLARE_HANDLE(HSMF);

typedef struct Event Event;

/*
 * The sample's constants keep its types, which the code shows: Watcom
 * sizes an enum by its values.
 */
#define SMF_SUCCESS 0L
#define SMF_INVALID_FILE 1L
#define SMF_NO_MEMORY 2L
#define SMF_OPEN_FAILED 3L
#define SMF_INVALID_TRACK 4L
#define SMF_END_OF_FILE 11L
#define SMF_REACHED_TKMAX 12L

#define SMF_F_EOF 0x00000001L
#define SMF_F_INSERTSYSEX 0x00000002L

#define MIDI_SYSEX ((BYTE)0xf0)
#define MIDI_SYSEXEND ((BYTE)0xf7)
#define MIDI_META ((BYTE)0xff)
#define MIDI_META_EOT ((BYTE)0x2f)
#define MIDI_META_TEMPO ((BYTE)0x51)

/* what smfSeek gathers: 0xff where no event set it */
typedef struct Keyframe Keyframe;
struct Keyframe {
	BYTE rbTempo[3];
	BYTE rbProgram[16];
	BYTE rbControl[16 * 120];
};

#define KF_EMPTY ((BYTE)0xff)
#define MIDI_CONTROLCHANGE ((BYTE)0xb0)
#define MIDI_PROGRAMCHANGE ((BYTE)0xc0)

typedef struct Chunkhdr Chunkhdr;
typedef struct Filehdr Filehdr;

struct Chunkhdr {
	FOURCC fourccType;
	DWORD dwLength;
};

struct Filehdr {
	WORD wFormat;
	WORD wTracks;
	WORD wDivision;
};

#define FOURCC_MThd mmioFOURCC('M', 'T', 'h', 'd')
#define FOURCC_MTrk mmioFOURCC('M', 'T', 'r', 'k')
#define MAX_TICKS 0xFFFFFFFFL
#define MIDI_DEFAULT_TEMPO 500000L
#define C_TEMPO_MAP_CHK 16

#define SMF_TF_EOT 0x00000001L
#define MIDI_MSG ((BYTE)0x80)

#define WORDSWAP(w) ((((w) >> 8) & 0xFF) | (((w) << 8) & 0xFF00))
#define DWORDSWAP(dw) ((((dw) >> 24) & 0xFF) | (((dw) >> 8) & 0xFF00) | (((dw) << 8) & 0xFF0000) | (((dw) << 24) & 0xFF000000))

/* an event of a track, 15 bytes */
struct Event {
	TICKS tkDelta;
	BYTE abEvent[3];
	DWORD cbParm;
	BYTE *hpbParm;
};

#define EVENT_TYPE(e) ((e).abEvent[0])
#define EVENT_CH_B1(e) ((e).abEvent[1])
#define EVENT_CH_B2(e) ((e).abEvent[2])
#define EVENT_META_TYPE(e) ((e).abEvent[1])

/* a tempo change, 12 bytes */
struct Tempo {
	TICKS tkTempo;
	DWORD msBase;
	DWORD dwTempo;
};

/* a track of the file */
struct Track {
	Smf *psmf;
	DWORD idxTrack;
	TICKS tkPosition;
	DWORD cbLeft;
	BYTE *hpbImage;
	BYTE bRunningStatus;
	DWORD fdwTrack;
	TICKS tkLength;
	DWORD cbLength;
};

/* an open MIDI file, the tracks follow */
struct Smf {
	BYTE *hpbImage;
	DWORD cbImage;
	HTASK htask;
	TICKS tkPosition;
	TICKS tkLength;
	DWORD dwFormat;
	DWORD dwTracks;
	DWORD dwTimeDivision;
	DWORD fdwSMF;
	DWORD cTempoMap;
	DWORD cTempoMapAlloc;
	HLOCAL hTempoMap;
	Tempo *pTempoMap;
	DWORD dwPendingUserEvent;
	DWORD cbPendingUserEvent;
	BYTE *hpbPendingUserEvent;
	Track rTracks[];
};

/* what smfOpenFile reads: the file in memory */
struct Smfopen {
	BYTE *data;
	DWORD size;
	DWORD f8;
	HSMF hsmf;
};

struct Smfinfo {
	DWORD dwTracks;
	DWORD dwFormat;
	DWORD dwTimeDivision;
	TICKS tkLength;
};

typedef struct Seq Seq;

typedef struct Preroll Preroll;

#define SEQ_S_NOFILE 0
#define SEQ_S_OPENED 1
#define SEQ_S_PREROLLING 2
#define SEQ_S_PREROLLED 3
#define SEQ_S_PLAYING 4
#define SEQ_S_PAUSED 5
#define SEQ_S_STOPPING 6
#define SEQ_S_RESET 7

#define MMSG_DONE (WM_USER + 20)

#define SEQ_F_EOF 0x00000001L
#define SEQ_F_COLONIZED 0x00000002L
#define SEQ_F_WAITING 0x00000004L

struct Preroll {
	TICKS tkBase;
	TICKS tkEnd;
};

/* the sequencer: the sample's SEQ without pstrFile */
struct Seq {
	DWORD cBuffer;
	DWORD cbBuffer;
	UINT uDeviceID;
	UINT uMCIDeviceID;
	UINT uMCITimeFormat;
	UINT uMCITimeDiv;
	HWND hWnd;
	UINT uState;
	TICKS tkLength;
	DWORD cTrk;
	MMRESULT mmrcLastErr;
	Seq *pNext;
	HSMF hSmf;
	HMIDIOUT hmidi;
	DWORD dwTimeDivision;
	LPBYTE lpbAlloc;
	LPMIDIHDR lpmhFree;
	LPMIDIHDR lpmhPreroll;
	DWORD cbPreroll;
	UINT uBuffersInMMSYSTEM;
	TICKS tkBase;
	TICKS tkEnd;
	UINT fdwSeq;
};

extern char texts[];

#define T(off) (texts + (off))

int decodedlen(int id);
int seekentry(int id);
DWORD unpack(void *dst, DWORD dstlen, void *src, DWORD srclen);
int wrap(char *s, int width);
void watersound(void);
int __cdecl pollinput(void);
SMFRESULT WINAPI smfBuildFileIndex(Smf **ppsmf);
SMFRESULT WINAPI smfInsertParmData(Smf *psmf, TICKS tkDelta, LPMIDIHDR lpmh);
SMFRESULT WINAPI smfGetNextEvent(Smf *psmf, Event *pevent, TICKS tkMax);
DWORD WINAPI smfGetVDword(BYTE *hpbImage, DWORD dwLeft, DWORD *pdw);
MMRESULT WINAPI seqCloseFile(Seq *pSeq);
MMRESULT WINAPI seqStop(Seq *pSeq);
MMRESULT WINAPI seqPreroll(Seq *pSeq, Preroll *lpPreroll);
MMRESULT WINAPI seqStart(Seq *pSeq);
MMRESULT WINAPI seqTime(Seq *pSeq, TICKS *pTicks);
void CALLBACK seqMIDICallback(HMIDISTRM hms, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
static MMRESULT WINAPI XlatSMFErr(SMFRESULT smfrc);

extern HWND mainwnd;
extern int appactive;
extern short outalways;
extern char dserrtext[];
extern HANDLE datfh;
extern DWORD storedn;
extern unsigned char loadpack[];
extern char loadbuf[];
extern int sfxbytes;
extern int minmem;
extern int featuresoff;
extern char soundtext[];
extern char capstext[];
extern char musictext[];
extern int musicoff;
extern char savesdir[];
extern unsigned char soundson;
extern unsigned char passmode;
extern short censorship;
extern int sfxvolume;
extern unsigned char sfxswitch;
extern short sfxon;
extern int musicvolume;
extern Keyframe kf;

extern int dsi;
extern int dsj;
extern DSBUFFERDESC primdesc;
extern LPVOID lsp1;
extern LPVOID lsp2;
extern DWORD lsn1;
extern DWORD lsn2;
extern DWORD lslen;
extern DWORD lsi;
extern unsigned char *lsdata;
extern int lsquiet;
extern signed char lsc;
extern short lsk;
extern DWORD lsn;
extern int rsi;
extern char *infop;
extern int ssi;
extern int qi;
extern DWORD pstatus;
extern LONG vol;
extern int pqi;
extern char soundsok;
extern char dsready;
extern char dsmsg[304];
extern char dsshort[32];
extern DWORD swbytes;
extern DWORD hwbytes;
extern short queue[25];
extern short nqueue;
extern LPDIRECTSOUND ds;
extern DSCAPS dscaps;
extern LPDIRECTSOUNDBUFFER bufs[78];
extern LPDIRECTSOUNDBUFFER prim;
extern WAVEFORMATEX wfx;
/* the result of the last DirectSound call and its error (009cf8f4) */
struct Dss {
	HRESULT hr;
	HRESULT err;
};
extern HRESULT dsres[2];
extern int dslost;
extern DWORD musvol;
extern int cur;
extern int loop;
extern int lastsong;
extern int replaydue;
extern int replaywait;
extern int musplayed;
extern int musworks;
extern int musloaded;
extern DWORD musbytes;
extern char *minfop;
extern int introok;
extern int pos3;
extern int pos11;
extern int savepos3;
extern int savepos11;
extern int songsel;
extern Seq *songs[36];

extern char dbgbuf[2000];
extern char *dbgp;

void
initfmt(void)
{
	memset(&wfx, 0, sizeof wfx);
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 1;
	wfx.nSamplesPerSec = 11025;
	wfx.nAvgBytesPerSec = 11025;
	wfx.nBlockAlign = 1;
	wfx.wBitsPerSample = 8;
}

char *
dserrstr(HRESULT hr)
{
	return "";
}

int
dsfailed(char *where, HRESULT hr)
{
	(*(Dss *)dsres).hr = hr;
	dslost = 0;
	if (hr != DS_OK) {
		if (hr == DSERR_BUFFERLOST && appactive) {
			dslost = 1;
			return 2;
		}
		(*(Dss *)dsres).err = (*(Dss *)dsres).hr;
		wsprintf(dserrtext, T(0x8e4a), where, dserrstr((*(Dss *)dsres).err), (*(Dss *)dsres).err, HRESULT_SEVERITY((*(Dss *)dsres).err), HRESULT_FACILITY((*(Dss *)dsres).err), HRESULT_CODE((*(Dss *)dsres).err));
		return 1;
	}
	return 0;
}

short
muted(void)
{
	return 0;
}

void
clearbufs(void)
{
	int i;

	for (i = 0; i < 78; i++)
		bufs[i] = NULL;
}

int
createbuf(LPDIRECTSOUND d, LPDIRECTSOUNDBUFFER *buf, DWORD size)
{
	DSBUFFERDESC bd;

	memset(&bd, 0, sizeof bd);
	bd.dwSize = sizeof bd;
	bd.dwFlags = DSBCAPS_CTRLDEFAULT;
	bd.dwBufferBytes = size;
	bd.lpwfxFormat = &wfx;
	if (!dsfailed("crbuf.CrSBf", d->CreateSoundBuffer(&bd, buf, NULL)))
		return 1;
	*buf = NULL;
	return 0;
}

void
freesound(void)
{
	for (dsi = 0; dsi < 78; dsi++)
		if (bufs[dsi] != NULL) {
			bufs[dsi]->Release();
			bufs[dsi] = NULL;
		}
	swbytes = 0;
	hwbytes = 0;
	if (prim != NULL) {
		dsfailed("Prim.Rel", prim->Release());
		prim = NULL;
	}
	if (ds != NULL) {
		ds->Release();
		ds = NULL;
	}
	dsready = 0;
}

int
soundid(int n)
{
	if (n >= 0xff0 && n <= 0xff3)
		n -= 4;
	return n;
}

int
initsound(void)
{
	DSBCAPS caps;

	if (dsready)
		return 0;
	initfmt();
	dsmsg[0] = '\0';
	dsshort[0] = '\0';
	memset(&dscaps, 0, sizeof dscaps);
	dscaps.dwSize = sizeof dscaps;
	if (dsfailed("i_s.DSC", DirectSoundCreate(NULL, &ds, NULL))) {
		switch ((*(Dss *)dsres).err) {
		case DSERR_NOAGGREGATION:
			wsprintf(dsmsg, T(0x9229));
			wsprintf(dsshort, T(0x9267));
			return 0;
		case DSERR_ALLOCATED:
			wsprintf(dsmsg, T(0x918e));
			wsprintf(dsshort, T(0x920f));
			return 0;
		case DSERR_OUTOFMEMORY:
			wsprintf(dsmsg, T(0x9276));
			wsprintf(dsshort, T(0x92e2));
			return 0;
		case DSERR_NODRIVER:
			wsprintf(dsmsg, T(0x92f6));
			wsprintf(dsshort, T(0x930d));
			return 0;
		default:
			wsprintf(dsmsg, T(0x931d));
			wsprintf(dsshort, T(0x933f));
			return 0;
		}
	}
	dsfailed("i_s.sGtCaps", ds->GetCaps(&dscaps));
	if (ds == NULL) {
		wsprintf(dsmsg, T(0x931d));
		wsprintf(dsshort, T(0x933f));
		return 0;
	}
	if (dsfailed("i_s.sCoopLv", ds->SetCooperativeLevel(mainwnd, DSSCL_PRIORITY))) {
		freesound();
		wsprintf(dsmsg, T(0x934b));
		wsprintf(dsshort, T(0x936b));
		return 0;
	}
	memset(&primdesc, 0, sizeof primdesc);
	primdesc.dwSize = sizeof primdesc;
	primdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	if (dsfailed("i_s.CrSB.Prim", ds->CreateSoundBuffer(&primdesc, &prim, NULL)) || dsfailed("i_s.Prim.StFrm", prim->SetFormat(&wfx))) {
primfail:
		freesound();
		wsprintf(dsmsg, T(0x937b));
		wsprintf(dsshort, T(0x93a1));
		return 0;
	}
	if (outalways)
		if (dsfailed("i_s.Pl.Prim", prim->Play(0, 0, DSBPLAY_LOOPING)))
			goto primfail;
	swbytes = 0;
	hwbytes = 0;
	for (dsj = 0; dsj < 78; dsj++) {
		if (!createbuf(ds, &bufs[dsj], decodedlen(soundid(dsj + 0xfb5)) - 0x2c)) {
			freesound();
			wsprintf(dsmsg, T(0x93b8));
			wsprintf(dsshort, T(0x93d7));
			return 0;
		}
		caps.dwSize = sizeof caps;
		bufs[dsj]->GetCaps(&caps);
		if (caps.dwFlags & DSBCAPS_LOCHARDWARE)
			hwbytes += caps.dwBufferBytes;
		else
			swbytes += caps.dwBufferBytes;
	}
	dsready = 1;
	return 1;
}

/*
 * loadsound reads sound id into buffer n. The loop over the samples
 * starts one past the end of the data.
 */
DWORD
loadsound(int n, int id)
{
	if (ds == NULL || bufs[n] == NULL)
		return 0;
	lsquiet = 0;
	lsi = id;
	id = soundid(id);
	if (id != lsi)
		lsquiet = 1;
	if (!seekentry(id))
		return 0;
	lslen = fileread(loadpack, 1, storedn, datfh);
	lslen = unpack(loadbuf, 0x96000, loadpack, lslen);
	lsdata = (unsigned char *)loadbuf;
	if (lslen > 0x2c)
		lslen -= 0x2c;
	else
		lslen = 0;
	if (lslen == 0)
		return 0;
	if (lsquiet)
		for (lsi = lslen + 0x2c; lsi != 0; lsi--) {
			lsc = loadbuf[lsi] - 0x80;
			if (lsc < -1 || lsc > 1)
				lsc /= 2;
			loadbuf[lsi] = lsc + 0x80;
		}
	if (dsfailed("ls.lck", bufs[n]->Lock(0, lslen, &lsp1, &lsn1, &lsp2, &lsn2, 0)))
		return 0;
	memcpy(lsp1, loadbuf + 0x2c, lsn1);
	if (lsn2)
		memcpy(lsp2, loadbuf + 0x2c + lsn1, lsn2);
	if (dsfailed("ls.ulck", bufs[n]->Unlock(lsp1, lsn1, lsp2, lsn2)))
		return 0;
	dsfailed("ls.sCP0", bufs[n]->SetCurrentPosition(0));
	return lslen;
}

int
loadsounds(void)
{
	if (ds == NULL)
		return 0;
	soundsok = 0;
	sfxbytes = 0;
	for (lsk = 0; lsk < 78; lsk++)
		if (!muted()) {
			lsn = loadsound(lsk, lsk + 0xfb5);
			sfxbytes += lsn;
		}
	if (sfxbytes)
		soundsok = 1;
	return 1;
}

int
restoresounds(void)
{
	if (!dsready)
		return 0;
	for (rsi = 0; rsi < 78; rsi++)
		if (bufs[rsi] != NULL && dsfailed("rst&rl_sb", bufs[rsi]->Restore()))
			return 0;
	return loadsounds();
}

void
soundinfo(void)
{
	infop = soundtext;
	soundtext[0] = '\0';
	if (minmem || featuresoff)
		wsprintf(infop, T(0x93ed));
	else if (!dsready) {
		wrap(dsmsg, 0x1ac);
		wsprintf(infop, T(0x942e), dsmsg);
	}
	while (*infop)
		infop++;
	wsprintf(infop, T(0x9441), dscaps.dwTotalHwMemBytes >> 10, dscaps.dwFreeHwMemBytes >> 10, (dscaps.dwTotalHwMemBytes - dscaps.dwFreeHwMemBytes) >> 10);
	while (*infop)
		infop++;
	wsprintf(infop, T(0x948c), hwbytes >> 10);
	while (*infop)
		infop++;
	wsprintf(infop, T(0x94bb), swbytes >> 10);
	while (*infop)
		infop++;
	infop = capstext;
	capstext[0] = '\0';
	wsprintf(infop, T(0x94e6), dscaps.dwFlags & DSCAPS_EMULDRIVER ? T(0x6b9d) : dsready ? T(0x6ba4) : T(0x6ba1));
	while (*infop)
		infop++;
	wsprintf(infop, T(0x9506));
	while (*infop)
		infop++;
	wsprintf(infop, T(0x9517), dscaps.dwFlags & DSCAPS_PRIMARY8BIT ? T(0x6b87) : T(0x6b91));
	while (*infop)
		infop++;
	wsprintf(infop, T(0x9536), dscaps.dwFlags & DSCAPS_PRIMARYMONO ? T(0x6b87) : T(0x6b91));
	while (*infop)
		infop++;
	wsprintf(infop, T(0x954d));
	while (*infop)
		infop++;
	wsprintf(infop, T(0x9517), dscaps.dwFlags & DSCAPS_SECONDARY8BIT ? T(0x6b87) : T(0x6b91));
	while (*infop)
		infop++;
	wsprintf(infop, T(0x9536), dscaps.dwFlags & DSCAPS_SECONDARYMONO ? T(0x6b87) : T(0x6b91));
	while (*infop)
		infop++;
}

void
stopsounds(void)
{
	if (dsready)
		for (ssi = 0; ssi < 78; ssi++)
			if (bufs[ssi] != NULL)
				dsfailed("s_stp", bufs[ssi]->Stop());
}

void
clearqueue(void)
{
	nqueue = 0;
	memset(queue, 0, sizeof queue);
}

short
inqueue(short n)
{
	for (qi = 0; qi < nqueue; qi++)
		if (n == queue[qi])
			return 1;
	return 0;
}

short
playing(short n)
{
	if (!dsready)
		return 0;
	if (bufs[n] == NULL)
		return 0;
	while (dsfailed("swgg?", bufs[n]->GetStatus(&pstatus))) {
		if (dslost == 0 || appactive == 0)
			return 0;
		if (!restoresounds())
			return 0;
	}
	if (pstatus & DSBSTATUS_PLAYING)
		return 1;
	return 0;
}

void
queuesound(short n)
{
	if (soundson == 0 || dsready == 0 || dsready == 0xff || passmode || n == -1 || nqueue >= 25)
		return;
	if (censorship == 1 || censorship == 4)
		switch (n) {
		case 0x1b:
		case 0x1c:
		case 0x1d:
		case 0x1e:
		case 0x27:
		case 0x4b:
			n = 0x4d;
			break;
		}
	if (bufs[n] != NULL && !inqueue(n))
		queue[nqueue++] = n;
}

void
playsound(short n)
{
	if (n == -1 || bufs[n] == NULL)
		return;
	switch ((sfxvolume + 5) / 10) {
	case 0:
		vol = -10000;
		break;
	case 1:
		vol = -5000;
		break;
	case 2:
		vol = -3000;
		break;
	case 3:
		vol = -2000;
		break;
	case 4:
		vol = -1500;
		break;
	case 5:
		vol = -1000;
		break;
	case 6:
		vol = -750;
		break;
	case 7:
		vol = -500;
		break;
	case 8:
		vol = -250;
		break;
	case 9:
		vol = -100;
		break;
	case 10:
		vol = 0;
		break;
	}
	dsfailed("ssb.sv", bufs[n]->SetVolume(vol));
	while (dsfailed("ssb.sp", bufs[n]->SetCurrentPosition(0))) {
		if (dslost == 0)
			break;
		if (appactive == 0)
			return;
		if (!restoresounds())
			break;
	}
	while (dsfailed("ssb.pl", bufs[n]->Play(0, 0, 0))) {
		if (dslost == 0 || appactive == 0)
			return;
		if (!restoresounds())
			return;
	}
}

void
playnow(short n)
{
	if (soundson == 0 || dsready == 0 || dsready == 0xff || passmode)
		return;
	playsound(n);
}

void
playqueue(void)
{
	if (soundson == 0 || dsready == 0 || dsready == 0xff || passmode)
		return;
	if (nqueue)
		watersound();
	if (!nqueue)
		return;
	for (pqi = 0; pqi < nqueue; pqi++)
		if (!playing(queue[pqi]))
			playsound(queue[pqi]);
	clearqueue();
}

void
sfx(short n)
{
	if (sfxswitch == 0 || soundson == 0 || dsready == 0 || dsready == 0xff || passmode
	|| sfxswitch == 0 || sfxon == 0 || n == -1 || bufs[n] == NULL)
		return;
	queuesound(n);
}

void
sfxnow(short n)
{
	if (sfxswitch == 0 || sfxswitch == 0 || sfxon == 0)
		return;
	playnow(n);
}

void
dbgf(char *fmt, ...)
{
	va_list ap;

	if (dbgp >= dbgbuf + sizeof dbgbuf - 100)
		return;
	va_start(ap, fmt);
	wvsprintf(dbgp, fmt, ap);
	while (*dbgp)
		dbgp++;
	*dbgp++ = ' ';
	*dbgp = '\0';
}

SMFRESULT WINAPI
smfOpenFile(Smfopen *psofs)
{
	Smf *psmf;
	SMFRESULT smfrc;
	void *p;

	psmf = (Smf *)LocalAlloc(LPTR, sizeof(Smf));
	if (psmf == NULL) {
		dbgf(T(0xad67));
		return SMF_NO_MEMORY;
	}
	psmf->fdwSMF = 0;
	psmf->pTempoMap = NULL;
	psmf->cbImage = psofs->size;
	psmf->hpbImage = (BYTE *)GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE, psmf->cbImage + 0x20);
	if (psmf->hpbImage == NULL) {
		dbgf(T(0xad72), psmf->cbImage);
		LocalFree(psmf);
		return SMF_NO_MEMORY;
	}
	p = GlobalLock(psmf->hpbImage);
	memcpy(p, psofs->data, psofs->size);
	GlobalUnlock(psmf->hpbImage);
	smfrc = smfBuildFileIndex(&psmf);
	if (smfrc != SMF_SUCCESS) {
		dbgf(T(0xadac), smfrc);
		GlobalFreePtr(psmf->hpbImage);
		LocalFree(psmf);
		return smfrc;
	}
	psofs->hsmf = (HSMF)psmf;
	return smfrc;
}

SMFRESULT WINAPI
smfCloseFile(HSMF hsmf)
{
	Smf *psmf = (Smf *)hsmf;

	if (psmf != NULL) {
		if (psmf->hpbImage != NULL)
			GlobalFreePtr(psmf->hpbImage);
		LocalFree(psmf);
	}
	return SMF_SUCCESS;
}

SMFRESULT WINAPI
smfGetFileInfo(HSMF hsmf, Smfinfo *psfi)
{
	Smf *psmf = (Smf *)hsmf;

	if (psmf == NULL || psfi == NULL)
		return SMF_SUCCESS;
	psfi->dwTracks = psmf->dwTracks;
	psfi->dwFormat = psmf->dwFormat;
	psfi->dwTimeDivision = psmf->dwTimeDivision;
	psfi->tkLength = psmf->tkLength;
	return SMF_SUCCESS;
}

DWORD WINAPI
smfTicksToMillisecs(HSMF hsmf, TICKS tkOffset)
{
	Smf *psmf = (Smf *)hsmf;
	Tempo *pTempo;
	UINT idx;
	UINT uSMPTE;
	DWORD dwTicksPerSec;

	if (psmf == NULL)
		return 0;
	if (tkOffset > psmf->tkLength) {
		dbgf(T(0xadd7));
		tkOffset = psmf->tkLength;
	}
	if (psmf->dwTimeDivision & 0x8000) {
		uSMPTE = -(int)(char)((psmf->dwTimeDivision >> 8) & 0xFF);
		if (29 == uSMPTE)
			uSMPTE = 30;
		dwTicksPerSec = (DWORD)uSMPTE * (DWORD)(BYTE)(psmf->dwTimeDivision & 0xFF);
		return (DWORD)MulDiv(tkOffset, 1000L, dwTicksPerSec);
	}
	pTempo = psmf->pTempoMap;
	if (pTempo == NULL)
		return 0;
	for (idx = 0; idx < psmf->cTempoMap; idx++, pTempo++)
		if (tkOffset < pTempo->tkTempo)
			break;
	pTempo--;
	return pTempo->msBase + MulDiv(tkOffset - pTempo->tkTempo, pTempo->dwTempo, 1000L * psmf->dwTimeDivision);
}

TICKS WINAPI
smfMillisecsToTicks(HSMF hsmf, DWORD msOffset)
{
	Smf *psmf = (Smf *)hsmf;
	Tempo *pTempo;
	UINT idx;
	UINT uSMPTE;
	DWORD dwTicksPerSec;
	TICKS tkOffset;

	if (psmf == NULL)
		return 0;
	if (psmf->dwTimeDivision & 0x8000) {
		uSMPTE = -(int)(char)((psmf->dwTimeDivision >> 8) & 0xFF);
		if (29 == uSMPTE)
			uSMPTE = 30;
		dwTicksPerSec = (DWORD)uSMPTE * (DWORD)(BYTE)(psmf->dwTimeDivision & 0xFF);
		return (DWORD)MulDiv(msOffset, dwTicksPerSec, 1000L);
	}
	pTempo = psmf->pTempoMap;
	if (pTempo == NULL)
		return 0;
	for (idx = 0; idx < psmf->cTempoMap; idx++, pTempo++)
		if (msOffset < pTempo->msBase)
			break;
	pTempo--;
	tkOffset = pTempo->tkTempo + MulDiv(msOffset - pTempo->msBase, 1000L * psmf->dwTimeDivision, pTempo->dwTempo);
	if (tkOffset > psmf->tkLength) {
		dbgf(T(0xade9));
		tkOffset = psmf->tkLength;
	}
	return tkOffset;
}

SMFRESULT WINAPI
smfReadEvents(HSMF hsmf, LPMIDIHDR lpmh, TICKS tkMax)
{
	Smf *psmf = (Smf *)hsmf;
	SMFRESULT smfrc;
	Event event;
	DWORD *lpdw;
	DWORD dwTempo;

	if (psmf == NULL || lpmh == NULL)
		return SMF_SUCCESS;
	lpmh->dwBytesRecorded = 0;
	if (psmf->dwPendingUserEvent) {
		smfrc = smfInsertParmData(psmf, 0, lpmh);
		if (smfrc != SMF_SUCCESS) {
			dbgf(T(0xadfb), smfrc);
			return smfrc;
		}
	}
	lpdw = (DWORD *)(lpmh->lpData + lpmh->dwBytesRecorded);
	if (psmf->fdwSMF & SMF_F_EOF)
		return SMF_END_OF_FILE;
	for (;;) {
		if (lpmh->dwBytesRecorded > lpmh->dwBufferLength)
			return SMF_END_OF_FILE;
		if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4 * sizeof(DWORD))
			break;
		smfrc = smfGetNextEvent(psmf, &event, tkMax);
		if (smfrc != SMF_SUCCESS) {
			if (smfrc == SMF_REACHED_TKMAX)
				psmf->fdwSMF |= SMF_F_EOF;
			break;
		}
		if (EVENT_TYPE(event) < MIDI_SYSEX) {
			*lpdw++ = event.tkDelta;
			*lpdw++ = 0;
			*lpdw++ = ((DWORD)MEVT_SHORTMSG << 24) | (DWORD)EVENT_TYPE(event)
				| ((DWORD)EVENT_CH_B1(event) << 8) | ((DWORD)EVENT_CH_B2(event) << 16);
			lpmh->dwBytesRecorded += 3 * sizeof(DWORD);
		} else if (EVENT_TYPE(event) == MIDI_META && EVENT_META_TYPE(event) == MIDI_META_EOT) {
		} else if (EVENT_TYPE(event) == MIDI_META && EVENT_META_TYPE(event) == MIDI_META_TEMPO) {
			if (event.cbParm != 3) {
				dbgf(T(0xae0f));
				return SMF_INVALID_FILE;
			}
			dwTempo = ((DWORD)MEVT_TEMPO << 24) | ((DWORD)event.hpbParm[0] << 16)
				| ((DWORD)event.hpbParm[1] << 8) | (DWORD)event.hpbParm[2];
			*lpdw++ = event.tkDelta;
			*lpdw++ = 0;
			*lpdw++ = dwTempo;
			lpmh->dwBytesRecorded += 3 * sizeof(DWORD);
		} else if (EVENT_TYPE(event) != MIDI_META) {
			psmf->cbPendingUserEvent = event.cbParm;
			psmf->hpbPendingUserEvent = event.hpbParm;
			psmf->fdwSMF &= ~SMF_F_INSERTSYSEX;
			switch (EVENT_TYPE(event)) {
			case MIDI_SYSEX:
				psmf->fdwSMF |= SMF_F_INSERTSYSEX;
				++psmf->cbPendingUserEvent;
				/* fall through */
			case MIDI_SYSEXEND:
				psmf->dwPendingUserEvent = (DWORD)MEVT_LONGMSG << 24;
				break;
			}
			smfrc = smfInsertParmData(psmf, event.tkDelta, lpmh);
			if (smfrc != SMF_SUCCESS) {
				dbgf(T(0xae2b), smfrc);
				return smfrc;
			}
			lpdw = (DWORD *)(lpmh->lpData + lpmh->dwBytesRecorded);
		}
	}
	return (psmf->fdwSMF & SMF_F_EOF) ? SMF_END_OF_FILE : SMF_SUCCESS;
}

/*
 * The SysEx byte advances lpdw by a DWORD where the sample advanced it
 * by a byte, so the data lands 3 bytes late.
 */
SMFRESULT WINAPI
smfInsertParmData(Smf *psmf, TICKS tkDelta, LPMIDIHDR lpmh)
{
	DWORD dwLength;
	DWORD dwRounded;
	DWORD *lpdw;

	if (psmf == NULL || lpmh == NULL)
		return SMF_SUCCESS;
	if (lpmh->dwBufferLength < lpmh->dwBytesRecorded)
		return SMF_SUCCESS;
	if (4 * sizeof(DWORD) > lpmh->dwBufferLength - lpmh->dwBytesRecorded) {
		if (0 == tkDelta)
			return SMF_SUCCESS;
		dbgf(T(0xae40));
		return SMF_INVALID_FILE;
	}
	lpdw = (DWORD *)(lpmh->lpData + lpmh->dwBytesRecorded);
	dwLength = lpmh->dwBufferLength - lpmh->dwBytesRecorded - 3 * sizeof(DWORD);
	dwLength = min(dwLength, psmf->cbPendingUserEvent);
	*lpdw++ = (DWORD)tkDelta;
	*lpdw++ = 0L;
	*lpdw++ = (psmf->dwPendingUserEvent & 0xFF000000L) | (dwLength & 0x00FFFFFFL);
	dwRounded = (dwLength + 3) & ~3L;
	if (psmf->fdwSMF & SMF_F_INSERTSYSEX) {
		*(BYTE *)lpdw++ = MIDI_SYSEX;
		psmf->fdwSMF &= ~SMF_F_INSERTSYSEX;
		--dwLength;
		--psmf->cbPendingUserEvent;
	}
	memmove(lpdw, psmf->hpbPendingUserEvent, dwLength);
	if (0 == (psmf->cbPendingUserEvent -= dwLength))
		psmf->dwPendingUserEvent = 0;
	lpmh->dwBytesRecorded += 3 * sizeof(DWORD) + dwRounded;
	return SMF_SUCCESS;
}

SMFRESULT WINAPI
smfSeek(HSMF hsmf, TICKS tkPosition, LPMIDIHDR lpmh)
{
	/*
	 * unused: each local is a temporary of the code generator, and
	 * twenty-three of them give the controller loop's multiply by 120
	 * the original's shifts
	 */
	int u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11, u12, u13, u14, u15, u16, u17, u18, u19, u20, u21, u22;
	Smf *psmf = (Smf *)hsmf;
	Track *ptrk;
	DWORD idxTrack;
	SMFRESULT smfrc;
	Event event;
	DWORD *lpdw;
	BYTE bEvent;
	UINT idx;
	UINT idxChannel;
	UINT idxController;

	memset(&kf, 0xFF, sizeof(Keyframe));
	psmf->tkPosition = 0;
	psmf->fdwSMF &= ~SMF_F_EOF;
	for (ptrk = psmf->rTracks, idxTrack = psmf->dwTracks; idxTrack--; ptrk++) {
		ptrk->psmf = psmf;
		ptrk->tkPosition = 0;
		ptrk->cbLeft = ptrk->cbLength;
		ptrk->hpbImage = psmf->hpbImage + ptrk->idxTrack;
		ptrk->bRunningStatus = 0;
		ptrk->fdwTrack = 0;
	}
	while (SMF_SUCCESS == (smfrc = smfGetNextEvent(psmf, &event, tkPosition))) {
		if (MIDI_META == (bEvent = EVENT_TYPE(event))) {
			if (EVENT_META_TYPE(event) == MIDI_META_TEMPO) {
				if (event.cbParm != sizeof(kf.rbTempo))
					return SMF_INVALID_FILE;
				memmove(kf.rbTempo, event.hpbParm, event.cbParm);
			}
		} else switch (bEvent & 0xF0) {
		case MIDI_PROGRAMCHANGE:
			kf.rbProgram[bEvent & 0x0F] = EVENT_CH_B1(event);
			break;
		case MIDI_CONTROLCHANGE:
			kf.rbControl[(((WORD)bEvent & 0x0F) * 120) + EVENT_CH_B1(event)] = EVENT_CH_B2(event);
			break;
		}
	}
	if (SMF_REACHED_TKMAX != smfrc)
		return smfrc;
	lpmh->dwBytesRecorded = 0;
	lpdw = (DWORD *)lpmh->lpData;
	if (KF_EMPTY != kf.rbTempo[0] || KF_EMPTY != kf.rbTempo[1] || KF_EMPTY != kf.rbTempo[2]) {
		if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 3 * sizeof(DWORD))
			return SMF_NO_MEMORY;
		*lpdw++ = 0;
		*lpdw++ = 0;
		*lpdw++ = ((DWORD)kf.rbTempo[0] << 16) | ((DWORD)kf.rbTempo[1] << 8)
			| (DWORD)kf.rbTempo[2] | ((DWORD)MEVT_TEMPO << 24);
		lpmh->dwBytesRecorded += 3 * sizeof(DWORD);
	}
	for (idx = 0; idx < 16; idx++)
		if (KF_EMPTY != kf.rbProgram[idx]) {
			if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 3 * sizeof(DWORD))
				return SMF_NO_MEMORY;
			*lpdw++ = 0;
			*lpdw++ = 0;
			*lpdw++ = ((DWORD)MEVT_SHORTMSG << 24) | (DWORD)MIDI_PROGRAMCHANGE
				| (DWORD)idx | ((DWORD)kf.rbProgram[idx] << 8);
			lpmh->dwBytesRecorded += 3 * sizeof(DWORD);
		}
	idx = 0;
	for (idxChannel = 0; idxChannel < 16; idxChannel++)
		for (idxController = 0; idxController < 120; idxController++) {
			if (KF_EMPTY != kf.rbControl[idx]) {
				if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 3 * sizeof(DWORD))
					return SMF_NO_MEMORY;
				*lpdw++ = 0;
				*lpdw++ = 0;
				*lpdw++ = ((DWORD)MEVT_SHORTMSG << 24) | (DWORD)MIDI_CONTROLCHANGE
					| (DWORD)idxChannel | ((DWORD)idxController << 8)
					| ((DWORD)kf.rbControl[idx] << 16);
				lpmh->dwBytesRecorded += 3 * sizeof(DWORD);
			}
			idx++;
		}
	for (ptrk = psmf->rTracks, idxTrack = psmf->dwTracks; idxTrack--; ptrk++)
		ptrk->tkPosition = tkPosition;
	return SMF_SUCCESS;
}

DWORD WINAPI
smfGetStateMaxSize(void)
{
	return 3 * sizeof(DWORD) + 3 * 16 * sizeof(DWORD) + 3 * 16 * 120 * sizeof(DWORD) + 3 * sizeof(DWORD);
}

MMRESULT WINAPI
seqAllocBuffers(Seq *pSeq)
{
	DWORD dwEachBufferSize;
	DWORD dwAlloc;
	UINT i;
	LPBYTE lpbWork;

	if (pSeq == NULL)
		return MCIERR_UNSUPPORTED_FUNCTION;
	pSeq->uState = SEQ_S_NOFILE;
	pSeq->lpmhFree = NULL;
	pSeq->lpbAlloc = NULL;
	pSeq->hSmf = (HSMF)NULL;
	dwEachBufferSize = sizeof(MIDIHDR) + (DWORD)pSeq->cbBuffer;
	dwAlloc = dwEachBufferSize * (DWORD)pSeq->cBuffer;
	pSeq->lpbAlloc = (LPBYTE)GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE, dwAlloc);
	if (NULL == pSeq->lpbAlloc)
		return MCIERR_OUT_OF_MEMORY;
	pSeq->lpmhFree = NULL;
	lpbWork = pSeq->lpbAlloc;
	for (i = 0; i < pSeq->cBuffer; i++) {
		((LPMIDIHDR)lpbWork)->lpNext = pSeq->lpmhFree;
		((LPMIDIHDR)lpbWork)->lpData = (LPSTR)lpbWork + sizeof(MIDIHDR);
		((LPMIDIHDR)lpbWork)->dwBufferLength = pSeq->cbBuffer;
		((LPMIDIHDR)lpbWork)->dwBytesRecorded = 0;
		((LPMIDIHDR)lpbWork)->dwUser = (DWORD)(UINT)pSeq;
		((LPMIDIHDR)lpbWork)->dwFlags = 0;
		pSeq->lpmhFree = (LPMIDIHDR)lpbWork;
		lpbWork += dwEachBufferSize;
	}
	return MMSYSERR_NOERROR;
}

void WINAPI
seqFreeBuffers(Seq *pSeq)
{
	if (pSeq != NULL && NULL != pSeq->lpbAlloc)
		GlobalFreePtr(pSeq->lpbAlloc);
}

MMRESULT WINAPI
seqOpenFile(Seq *pSeq, BYTE *data, DWORD size)
{
	MMRESULT rc = MMSYSERR_NOERROR;
	Smfopen sofs;
	Smfinfo sfi;
	SMFRESULT smfrc;
	DWORD cbBuffer;

	if (pSeq == NULL)
		return MCIERR_UNSUPPORTED_FUNCTION;
	if (pSeq->uState != SEQ_S_NOFILE) {
		dbgf(T(0xac0d), pSeq->uState);
		return MCIERR_UNSUPPORTED_FUNCTION;
	}
	sofs.data = data;
	sofs.size = size;
	smfrc = smfOpenFile(&sofs);
	if (SMF_SUCCESS != smfrc) {
		dbgf(T(0xac1d), smfrc);
		rc = XlatSMFErr(smfrc);
		goto cleanup;
	}
	pSeq->hSmf = sofs.hsmf;
	smfGetFileInfo(pSeq->hSmf, &sfi);
	pSeq->dwTimeDivision = sfi.dwTimeDivision;
	pSeq->tkLength = sfi.tkLength;
	pSeq->cTrk = sfi.dwTracks;
	cbBuffer = min(pSeq->cbBuffer, smfGetStateMaxSize());
cleanup:
	if (MMSYSERR_NOERROR != rc)
		seqCloseFile(pSeq);
	else
		pSeq->uState = SEQ_S_OPENED;
	return rc;
}

MMRESULT WINAPI
seqCloseFile(Seq *pSeq)
{
	LPMIDIHDR lpmh;

	if (pSeq == NULL || SEQ_S_OPENED != pSeq->uState)
		return MCIERR_UNSUPPORTED_FUNCTION;
	if ((HSMF)NULL != pSeq->hSmf) {
		smfCloseFile(pSeq->hSmf);
		pSeq->hSmf = (HSMF)NULL;
	}
	for (lpmh = pSeq->lpmhFree; lpmh; lpmh = lpmh->lpNext)
		midiOutUnprepareHeader(pSeq->hmidi, lpmh, sizeof(*lpmh));
	if (pSeq->lpmhPreroll)
		midiOutUnprepareHeader(pSeq->hmidi, pSeq->lpmhPreroll, sizeof(*pSeq->lpmhPreroll));
	if (pSeq->hmidi != NULL) {
		midiStreamClose((HMIDISTRM)pSeq->hmidi);
		pSeq->hmidi = NULL;
	}
	pSeq->uState = SEQ_S_NOFILE;
	return MMSYSERR_NOERROR;
}

MMRESULT WINAPI
seqPreroll(Seq *pSeq, Preroll *lpPreroll)
{
	SMFRESULT smfrc;
	MMRESULT mmrc = MMSYSERR_NOERROR;
	MIDIPROPTIMEDIV mptd;
	LPMIDIHDR lpmh = NULL;
	LPMIDIHDR lpmhPreroll = NULL;
	DWORD cbPrerollBuffer;
	UINT uDeviceID;
	DWORD t;

	if (pSeq == NULL)
		return MCIERR_UNSUPPORTED_FUNCTION;
	pSeq->mmrcLastErr = MMSYSERR_NOERROR;
	if (pSeq->uState != SEQ_S_OPENED && pSeq->uState != SEQ_S_PREROLLED) {
		dbgf(T(0xac2f), pSeq->uState, SEQ_S_OPENED, SEQ_S_PREROLLED);
		return MCIERR_UNSUPPORTED_FUNCTION;
	}
	pSeq->tkBase = lpPreroll->tkBase;
	pSeq->tkEnd = lpPreroll->tkEnd;
	if (pSeq->hmidi) {
		pSeq->uState = SEQ_S_RESET;
		midiOutReset(pSeq->hmidi);
		t = ticks();
		while (pSeq->uBuffersInMMSYSTEM && since(t) < 5000)
			Sleep(0);
	}
	pSeq->uBuffersInMMSYSTEM = 0;
	pSeq->uState = SEQ_S_PREROLLING;
	if (NULL == pSeq->hmidi) {
		uDeviceID = pSeq->uDeviceID;
		if ((mmrc = midiStreamOpen((LPHMIDISTRM)&pSeq->hmidi, &uDeviceID, 1, (DWORD)seqMIDICallback, 0, CALLBACK_FUNCTION)) != MMSYSERR_NOERROR) {
			dbgf(T(0xaf1b), uDeviceID, (WORD)mmrc);
			pSeq->hmidi = NULL;
			goto seq_Preroll_Cleanup;
		}
		mptd.cbStruct = sizeof(mptd);
		mptd.dwTimeDiv = pSeq->dwTimeDivision;
		if ((mmrc = midiStreamProperty((HMIDISTRM)pSeq->hmidi, (LPBYTE)&mptd, MIDIPROP_SET | MIDIPROP_TIMEDIV)) != MMSYSERR_NOERROR) {
			dbgf(T(0xac44), (WORD)mmrc);
			midiStreamClose((HMIDISTRM)pSeq->hmidi);
			pSeq->hmidi = NULL;
			mmrc = MCIERR_DEVICE_NOT_READY;
			goto seq_Preroll_Cleanup;
		}
	}
	if (pSeq->hmidi) {
		musvol = musicvolume * 0xffff;
		musvol /= 100;
		musvol |= musvol << 16;
		if ((mmrc = midiOutSetVolume(pSeq->hmidi, musvol)) != MMSYSERR_NOERROR)
			dbgf(T(0xaf4e), (WORD)mmrc);
	}
	mmrc = MMSYSERR_NOERROR;
	if (!pSeq->lpmhPreroll) {
		cbPrerollBuffer = 4096;
		lpmhPreroll = (LPMIDIHDR)GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE, cbPrerollBuffer);
	} else {
		cbPrerollBuffer = pSeq->cbPreroll;
		lpmhPreroll = pSeq->lpmhPreroll;
	}
	lpmhPreroll->lpNext = pSeq->lpmhFree;
	lpmhPreroll->lpData = (LPSTR)lpmhPreroll + sizeof(MIDIHDR);
	lpmhPreroll->dwBufferLength = cbPrerollBuffer - sizeof(MIDIHDR);
	lpmhPreroll->dwBytesRecorded = 0;
	lpmhPreroll->dwUser = (DWORD)(UINT)pSeq;
	lpmhPreroll->dwFlags = 0;
	do {
		smfrc = smfSeek(pSeq->hSmf, pSeq->tkBase, lpmhPreroll);
		if (SMF_SUCCESS != smfrc) {
			if (SMF_NO_MEMORY != smfrc || cbPrerollBuffer >= 32768L) {
				dbgf(T(0xac58), smfrc);
				GlobalFreePtr(lpmhPreroll);
				pSeq->lpmhPreroll = NULL;
				mmrc = XlatSMFErr(smfrc);
				goto seq_Preroll_Cleanup;
			} else {
				cbPrerollBuffer *= 2;
				lpmh = (LPMIDIHDR)GlobalReAllocPtr(lpmhPreroll, cbPrerollBuffer, 0);
				if (NULL == lpmh) {
					dbgf(T(0xac66));
					mmrc = MCIERR_OUT_OF_MEMORY;
					goto seq_Preroll_Cleanup;
				}
				lpmhPreroll = lpmh;
				lpmhPreroll->lpData = (LPSTR)lpmhPreroll + sizeof(MIDIHDR);
				lpmhPreroll->dwBufferLength = cbPrerollBuffer - sizeof(MIDIHDR);
				pSeq->lpmhPreroll = lpmhPreroll;
				pSeq->cbPreroll = cbPrerollBuffer;
			}
		}
	} while (SMF_SUCCESS != smfrc);
	if (MMSYSERR_NOERROR != (mmrc = midiOutPrepareHeader(pSeq->hmidi, lpmhPreroll, sizeof(MIDIHDR)))) {
		dbgf(T(0xac74), mmrc);
		mmrc = MCIERR_DEVICE_NOT_READY;
		goto seq_Preroll_Cleanup;
	}
	++pSeq->uBuffersInMMSYSTEM;
	if (MMSYSERR_NOERROR != (mmrc = midiStreamOut((HMIDISTRM)pSeq->hmidi, lpmhPreroll, sizeof(MIDIHDR)))) {
		dbgf(T(0xac87), mmrc);
		mmrc = MCIERR_DEVICE_NOT_READY;
		--pSeq->uBuffersInMMSYSTEM;
		goto seq_Preroll_Cleanup;
	}
	pSeq->fdwSeq &= ~SEQ_F_EOF;
	while (pSeq->lpmhFree) {
		lpmh = pSeq->lpmhFree;
		pSeq->lpmhFree = lpmh->lpNext;
		smfrc = smfReadEvents(pSeq->hSmf, lpmh, pSeq->tkEnd);
		if (SMF_SUCCESS != smfrc && SMF_END_OF_FILE != smfrc) {
			dbgf(T(0xac9a), smfrc);
			mmrc = XlatSMFErr(smfrc);
			goto seq_Preroll_Cleanup;
		}
		if (MMSYSERR_NOERROR != (mmrc = midiOutPrepareHeader(pSeq->hmidi, lpmh, sizeof(*lpmh)))) {
			dbgf(T(0xacae));
			goto seq_Preroll_Cleanup;
		}
		if (MMSYSERR_NOERROR != (mmrc = midiStreamOut((HMIDISTRM)pSeq->hmidi, lpmh, sizeof(*lpmh)))) {
			dbgf(T(0xacc3));
			goto seq_Preroll_Cleanup;
		}
		++pSeq->uBuffersInMMSYSTEM;
		if (SMF_END_OF_FILE == smfrc) {
			pSeq->fdwSeq |= SEQ_F_EOF;
			break;
		}
	}
seq_Preroll_Cleanup:
	if (MMSYSERR_NOERROR != mmrc) {
		pSeq->uState = SEQ_S_OPENED;
		pSeq->fdwSeq &= ~SEQ_F_WAITING;
	} else
		pSeq->uState = SEQ_S_PREROLLED;
	return mmrc;
}

MMRESULT WINAPI
seqStart(Seq *pSeq)
{
	if (pSeq == NULL)
		return MCIERR_UNSUPPORTED_FUNCTION;
	if (SEQ_S_PREROLLED != pSeq->uState) {
		dbgf(T(0xacd7), pSeq->uState);
		return MCIERR_UNSUPPORTED_FUNCTION;
	}
	pSeq->uState = SEQ_S_PLAYING;
	return midiStreamRestart((HMIDISTRM)pSeq->hmidi);
}

MMRESULT WINAPI
seqPause(Seq *pSeq)
{
	if (pSeq == NULL || SEQ_S_PLAYING != pSeq->uState)
		return MCIERR_UNSUPPORTED_FUNCTION;
	pSeq->uState = SEQ_S_PAUSED;
	midiStreamPause((HMIDISTRM)pSeq->hmidi);
	return MMSYSERR_NOERROR;
}

MMRESULT WINAPI
seqRestart(Seq *pSeq)
{
	if (pSeq == NULL || SEQ_S_PAUSED != pSeq->uState)
		return MCIERR_UNSUPPORTED_FUNCTION;
	pSeq->uState = SEQ_S_PLAYING;
	midiStreamRestart((HMIDISTRM)pSeq->hmidi);
	return MMSYSERR_NOERROR;
}

MMRESULT WINAPI
seqStop(Seq *pSeq)
{
	DWORD t;

	if (pSeq == NULL)
		return MMSYSERR_NOERROR;
	if (SEQ_S_PLAYING != pSeq->uState && SEQ_S_PAUSED != pSeq->uState) {
		pSeq->fdwSeq &= ~SEQ_F_WAITING;
		return MMSYSERR_NOERROR;
	}
	pSeq->uState = SEQ_S_STOPPING;
	pSeq->fdwSeq |= SEQ_F_WAITING;
	if (MMSYSERR_NOERROR != (pSeq->mmrcLastErr = midiStreamStop((HMIDISTRM)pSeq->hmidi))) {
		dbgf(T(0xace6), pSeq->mmrcLastErr);
		pSeq->fdwSeq &= ~SEQ_F_WAITING;
		return MCIERR_DEVICE_NOT_READY;
	}
	t = ticks();
	while (pSeq->uBuffersInMMSYSTEM && since(t) < 5000)
		Sleep(0);
	return MMSYSERR_NOERROR;
}

MMRESULT WINAPI
seqTime(Seq *pSeq, TICKS *pTicks)
{
	MMRESULT mmr;
	MMTIME mmt;

	if (pSeq == NULL)
		return MCIERR_UNSUPPORTED_FUNCTION;
	if (SEQ_S_PLAYING != pSeq->uState && SEQ_S_PAUSED != pSeq->uState
	&& SEQ_S_PREROLLING != pSeq->uState && SEQ_S_PREROLLED != pSeq->uState
	&& SEQ_S_OPENED != pSeq->uState) {
		dbgf(T(0xacf8), pSeq->uState);
		return MCIERR_UNSUPPORTED_FUNCTION;
	}
	*pTicks = 0;
	if (SEQ_S_OPENED != pSeq->uState) {
		*pTicks = pSeq->tkBase;
		if (SEQ_S_PREROLLED != pSeq->uState) {
			mmt.wType = TIME_TICKS;
			mmr = midiStreamPosition((HMIDISTRM)pSeq->hmidi, &mmt, sizeof(mmt));
			if (MMSYSERR_NOERROR != mmr) {
				dbgf(T(0xad07), mmr);
				return MCIERR_DEVICE_NOT_READY;
			}
			*pTicks += mmt.u.ticks;
		}
	}
	return MMSYSERR_NOERROR;
}

TICKS WINAPI
seqMillisecsToTicks(Seq *pSeq, DWORD msOffset)
{
	return smfMillisecsToTicks(pSeq->hSmf, msOffset);
}

DWORD WINAPI
seqTicksToMillisecs(Seq *pSeq, TICKS tkOffset)
{
	return smfTicksToMillisecs(pSeq->hSmf, tkOffset);
}

void CALLBACK
seqMIDICallback(HMIDISTRM hms, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	LPMIDIHDR lpmh = (LPMIDIHDR)dw1;
	Seq *pSeq;
	MMRESULT mmrc;
	SMFRESULT smfrc;

	if (uMsg != MOM_DONE || lpmh == NULL)
		return;
	pSeq = (Seq *)lpmh->dwUser;
	if (pSeq == NULL)
		return;
	--pSeq->uBuffersInMMSYSTEM;
	if (SEQ_S_RESET == pSeq->uState) {
		if (lpmh != pSeq->lpmhPreroll) {
			lpmh->lpNext = pSeq->lpmhFree;
			pSeq->lpmhFree = lpmh;
		}
		return;
	}
	if (SEQ_S_STOPPING == pSeq->uState || (pSeq->fdwSeq & SEQ_F_EOF)) {
		if (lpmh != pSeq->lpmhPreroll) {
			lpmh->lpNext = pSeq->lpmhFree;
			pSeq->lpmhFree = lpmh;
		}
		if (MMSYSERR_NOERROR != (mmrc = midiOutUnprepareHeader(pSeq->hmidi, lpmh, sizeof(*lpmh))))
			dbgf(T(0xad19), mmrc);
		if (0 == pSeq->uBuffersInMMSYSTEM) {
			midiStreamClose((HMIDISTRM)pSeq->hmidi);
			pSeq->hmidi = NULL;
			pSeq->uState = SEQ_S_OPENED;
			pSeq->mmrcLastErr = MMSYSERR_NOERROR;
			pSeq->fdwSeq &= ~SEQ_F_WAITING;
			PostMessage(pSeq->hWnd, MMSG_DONE, (WPARAM)pSeq, pSeq->uState != SEQ_S_STOPPING ? 1 : 0);
			if (cur && loop) {
				replaydue = 1;
				replaywait = 0;
			} else {
				cur = 0;
				loop = 0;
				lastsong = 0;
				replaydue = 0;
				replaywait = 0;
			}
		}
	} else {
		smfrc = smfReadEvents(pSeq->hSmf, lpmh, pSeq->tkEnd);
		switch (smfrc) {
		case SMF_SUCCESS:
			break;
		case SMF_END_OF_FILE:
			pSeq->fdwSeq |= SEQ_F_EOF;
			smfrc = SMF_SUCCESS;
			break;
		default:
			dbgf(T(0xad38), smfrc);
			pSeq->uState = SEQ_S_STOPPING;
			break;
		}
		if (SMF_SUCCESS == smfrc) {
			++pSeq->uBuffersInMMSYSTEM;
			mmrc = midiStreamOut((HMIDISTRM)pSeq->hmidi, lpmh, sizeof(*lpmh));
			if (MMSYSERR_NOERROR != mmrc) {
				dbgf(T(0xad4b), mmrc);
				--pSeq->uBuffersInMMSYSTEM;
				pSeq->uState = SEQ_S_STOPPING;
			}
		}
	}
}

static MMRESULT WINAPI
XlatSMFErr(SMFRESULT smfrc)
{
	switch (smfrc) {
	case SMF_SUCCESS:
		return MMSYSERR_NOERROR;
	case SMF_NO_MEMORY:
		return MCIERR_OUT_OF_MEMORY;
	case SMF_INVALID_FILE:
	case SMF_OPEN_FAILED:
	case SMF_INVALID_TRACK:
		return MCIERR_INVALID_FILE;
	default:
		return MCIERR_UNSUPPORTED_FUNCTION;
	}
}

/* the length of a channel message by its status nibble */
extern int grbChanMsgLen[16];

SMFRESULT WINAPI
smfBuildFileIndex(Smf **ppsmf)
{
	SMFRESULT smfrc;
	Chunkhdr *pCh;
	Filehdr *pFh;
	DWORD idx;
	Smf *psmf, *psmfTemp;
	Track *ptrk;
	WORD wMemory;
	DWORD dwLeft;
	BYTE *hpbImage;
	DWORD idxTrack;
	Event event;
	BOOL fFirst;
	DWORD dwLength;
	HLOCAL hLocal;
	Tempo *pTempo;

	if (ppsmf == NULL)
		return SMF_SUCCESS;
	psmf = *ppsmf;
	if (psmf == NULL)
		return SMF_SUCCESS;
	dwLeft = psmf->cbImage;
	hpbImage = psmf->hpbImage;
	if (dwLeft < sizeof(Chunkhdr))
		return SMF_INVALID_FILE;
	pCh = (Chunkhdr *)hpbImage;
	dwLeft -= sizeof(Chunkhdr);
	hpbImage += sizeof(Chunkhdr);
	if (pCh->fourccType != FOURCC_MThd)
		return SMF_INVALID_FILE;
	dwLength = DWORDSWAP(pCh->dwLength);
	if (dwLength < sizeof(Filehdr) || dwLength > dwLeft)
		return SMF_INVALID_FILE;
	pFh = (Filehdr *)hpbImage;
	dwLeft -= dwLength;
	hpbImage += dwLength;
	psmf->dwFormat = (DWORD)WORDSWAP(pFh->wFormat);
	psmf->dwTracks = (DWORD)WORDSWAP(pFh->wTracks);
	psmf->dwTimeDivision = (DWORD)WORDSWAP(pFh->wDivision);
	wMemory = sizeof(Smf) + (WORD)(psmf->dwTracks * sizeof(Track));
	psmfTemp = (Smf *)LocalReAlloc((HLOCAL)psmf, wMemory, LMEM_MOVEABLE | LMEM_ZEROINIT);
	if (NULL == psmfTemp) {
		dbgf(T(0xae7a));
		return SMF_NO_MEMORY;
	}
	psmf = *ppsmf = psmfTemp;
	ptrk = psmf->rTracks;
	for (idx = 0; idx < psmf->dwTracks; idx++) {
		if (dwLeft < sizeof(Chunkhdr))
			return SMF_INVALID_FILE;
		pCh = (Chunkhdr *)hpbImage;
		dwLeft -= sizeof(Chunkhdr);
		hpbImage += sizeof(Chunkhdr);
		if (pCh->fourccType != FOURCC_MTrk)
			return SMF_INVALID_FILE;
		ptrk->idxTrack = (DWORD)(hpbImage - psmf->hpbImage);
		ptrk->cbLength = DWORDSWAP(pCh->dwLength);
		if (ptrk->cbLength > dwLeft)
			return SMF_INVALID_FILE;
		dwLeft -= ptrk->cbLength;
		hpbImage += ptrk->cbLength;
		ptrk++;
	}
	psmf->tkPosition = 0;
	psmf->fdwSMF &= ~SMF_F_EOF;
	for (ptrk = psmf->rTracks, idxTrack = psmf->dwTracks; idxTrack--; ptrk++) {
		ptrk->psmf = psmf;
		ptrk->tkPosition = 0;
		ptrk->cbLeft = ptrk->cbLength;
		ptrk->hpbImage = psmf->hpbImage + ptrk->idxTrack;
		ptrk->bRunningStatus = 0;
		ptrk->fdwTrack = 0;
	}
	while (SMF_SUCCESS == (smfrc = smfGetNextEvent(psmf, &event, MAX_TICKS))) {
		if (MIDI_META == event.abEvent[0] && MIDI_META_TEMPO == event.abEvent[1]) {
			if (3 != event.cbParm)
				return SMF_INVALID_FILE;
			if (psmf->cTempoMap == psmf->cTempoMapAlloc) {
				if (NULL != psmf->hTempoMap)
					LocalUnlock(psmf->hTempoMap);
				psmf->cTempoMapAlloc += C_TEMPO_MAP_CHK;
				fFirst = FALSE;
				if (0 == psmf->cTempoMap) {
					hLocal = LocalAlloc(LHND, (UINT)(psmf->cTempoMapAlloc * sizeof(Tempo)));
					fFirst = TRUE;
				} else
					hLocal = LocalReAlloc(psmf->hTempoMap, (UINT)(psmf->cTempoMapAlloc * sizeof(Tempo)), LHND);
				if (NULL == hLocal) {
					dbgf(T(0xaea2));
					return SMF_NO_MEMORY;
				}
				psmf->pTempoMap = (Tempo *)LocalLock(psmf->hTempoMap = hLocal);
			}
			if (fFirst && psmf->tkPosition != 0) {
				pTempo = &psmf->pTempoMap[psmf->cTempoMap++];
				pTempo->tkTempo = 0;
				pTempo->msBase = 0;
				pTempo->dwTempo = MIDI_DEFAULT_TEMPO;
				fFirst = FALSE;
			}
			pTempo = &psmf->pTempoMap[psmf->cTempoMap++];
			pTempo->tkTempo = psmf->tkPosition;
			if (fFirst)
				pTempo->msBase = 0;
			else
				pTempo->msBase = (pTempo - 1)->msBase + MulDiv(pTempo->tkTempo - (pTempo - 1)->tkTempo,
					(pTempo - 1)->dwTempo, 1000L * psmf->dwTimeDivision);
			pTempo->dwTempo = ((DWORD)event.hpbParm[0] << 16) | ((DWORD)event.hpbParm[1] << 8)
				| (DWORD)event.hpbParm[2];
		}
	}
	if (0 == psmf->cTempoMap) {
		hLocal = LocalAlloc(LHND, sizeof(Tempo));
		if (!hLocal) {
			dbgf(T(0xaed4));
			return SMF_NO_MEMORY;
		}
		psmf->pTempoMap = (Tempo *)LocalLock(psmf->hTempoMap = hLocal);
		psmf->cTempoMap = 1;
		psmf->cTempoMapAlloc = 1;
		psmf->pTempoMap->tkTempo = 0;
		psmf->pTempoMap->msBase = 0;
		psmf->pTempoMap->dwTempo = MIDI_DEFAULT_TEMPO;
	}
	if (SMF_END_OF_FILE == smfrc || SMF_SUCCESS == smfrc) {
		psmf->tkLength = psmf->tkPosition;
		smfrc = SMF_SUCCESS;
	}
	return smfrc;
}

SMFRESULT WINAPI
smfGetNextEvent(Smf *psmf, Event *pevent, TICKS tkMax)
{
	Track *ptrk;
	Track *ptrkFound;
	DWORD idxTrack;
	TICKS tkEventDelta;
	TICKS tkRelTime;
	TICKS tkMinRelTime;
	BYTE bEvent;
	DWORD dwGotTotal;
	DWORD dwGot;
	DWORD cbEvent;

	if (psmf == NULL || pevent == NULL)
		return SMF_SUCCESS;
	if (psmf->fdwSMF & SMF_F_EOF)
		return SMF_END_OF_FILE;
	ptrkFound = NULL;
	tkMinRelTime = MAX_TICKS;
	for (ptrk = psmf->rTracks, idxTrack = psmf->dwTracks; idxTrack--; ptrk++) {
		if (ptrk->fdwTrack & SMF_TF_EOT)
			continue;
		if (!smfGetVDword(ptrk->hpbImage, ptrk->cbLeft, &tkEventDelta)) {
			dbgf(T(0xaf04));
			return SMF_INVALID_FILE;
		}
		tkRelTime = ptrk->tkPosition + tkEventDelta - psmf->tkPosition;
		if (tkRelTime < tkMinRelTime) {
			tkMinRelTime = tkRelTime;
			ptrkFound = ptrk;
		}
	}
	if (!ptrkFound) {
		psmf->fdwSMF |= SMF_F_EOF;
		return SMF_END_OF_FILE;
	}
	ptrk = ptrkFound;
	if (psmf->tkPosition + tkMinRelTime >= tkMax)
		return SMF_REACHED_TKMAX;
	ptrk->hpbImage += (dwGot = smfGetVDword(ptrk->hpbImage, ptrk->cbLeft, &tkEventDelta));
	ptrk->cbLeft -= dwGot;
	if (ptrk->cbLeft < 3)
		return SMF_INVALID_FILE;
	ptrk->tkPosition += tkEventDelta;
	pevent->tkDelta = ptrk->tkPosition - psmf->tkPosition;
	psmf->tkPosition = ptrk->tkPosition;
	bEvent = *ptrk->hpbImage++;
	if (MIDI_MSG > bEvent) {
		if (0 == ptrk->bRunningStatus)
			return SMF_INVALID_FILE;
		dwGotTotal = 1;
		pevent->abEvent[0] = ptrk->bRunningStatus;
		pevent->abEvent[1] = bEvent;
		if (3 == grbChanMsgLen[(ptrk->bRunningStatus >> 4) & 0x0F]) {
			pevent->abEvent[2] = *ptrk->hpbImage++;
			dwGotTotal++;
		}
	} else if (MIDI_SYSEX > bEvent) {
		ptrk->bRunningStatus = bEvent;
		dwGotTotal = 2;
		pevent->abEvent[0] = bEvent;
		pevent->abEvent[1] = *ptrk->hpbImage++;
		if (3 == grbChanMsgLen[(bEvent >> 4) & 0x0F]) {
			pevent->abEvent[2] = *ptrk->hpbImage++;
			dwGotTotal++;
		}
	} else {
		ptrk->bRunningStatus = 0;
		if (MIDI_META == bEvent) {
			pevent->abEvent[0] = MIDI_META;
			if (MIDI_META_EOT == (pevent->abEvent[1] = *ptrk->hpbImage++))
				ptrk->fdwTrack |= SMF_TF_EOT;
			dwGotTotal = 2;
		} else if (MIDI_SYSEX == bEvent || MIDI_SYSEXEND == bEvent) {
			pevent->abEvent[0] = bEvent;
			dwGotTotal = 1;
		} else
			return SMF_INVALID_FILE;
		if (0 == (dwGot = smfGetVDword(ptrk->hpbImage, ptrk->cbLeft - 2, &cbEvent)))
			return SMF_INVALID_FILE;
		ptrk->hpbImage += dwGot;
		dwGotTotal += dwGot;
		if (dwGotTotal + cbEvent > ptrk->cbLeft)
			return SMF_INVALID_FILE;
		pevent->cbParm = cbEvent;
		pevent->hpbParm = ptrk->hpbImage;
		ptrk->hpbImage += cbEvent;
		dwGotTotal += cbEvent;
	}
	if (ptrk->cbLeft < dwGotTotal)
		return SMF_SUCCESS;
	ptrk->cbLeft -= dwGotTotal;
	return SMF_SUCCESS;
}

DWORD WINAPI
smfGetVDword(BYTE *hpbImage, DWORD dwLeft, DWORD *pdw)
{
	BYTE b;
	DWORD dwUsed = 0;

	if (hpbImage == NULL || pdw == NULL)
		return 0;
	*pdw = 0;
	do {
		if (!dwLeft)
			return 0;
		b = *hpbImage++;
		dwLeft--;
		dwUsed++;
		*pdw = (*pdw << 7) | (b & 0x7F);
	} while (b & 0x80);
	return dwUsed;
}

void
musicinfo(void)
{
	minfop = musictext;
	musictext[0] = '\0';
	wsprintf(minfop, T(0x9572));
	while (*minfop)
		minfop++;
	wsprintf(minfop, T(0x9594), musloaded ? T(0x6b9d) : T(0x6ba1), musbytes >> 10);
	while (*minfop)
		minfop++;
	wsprintf(minfop, T(0x95b6));
	while (*minfop)
		minfop++;
	if (musicoff)
		wsprintf(minfop, T(0x915a));
	else if (!musloaded)
		wsprintf(minfop, T(0x913f));
	else if (!musplayed)
		wsprintf(minfop, T(0x95c6));
	else if (!musworks)
		wsprintf(minfop, T(0x95dc));
	else
		wsprintf(minfop, T(0x95e9));
	while (*minfop)
		minfop++;
	if (dbgbuf[0]) {
		wsprintf(minfop, T(0x95ef), dbgbuf);
		while (*minfop)
			minfop++;
	}
}

Seq *
loadmidi(int id, HANDLE h, BYTE *data, DWORD size)
{
	Seq *seq;
	DWORD n;

	if (h == NULL || (n = fileread(loadbuf, 1, 0x96000, h)) == 0) {
		if (size == 0 || data == NULL) {
			if (!seekentry(id))
				return NULL;
			n = unpack(loadbuf, 0x96000, loadpack, fileread(loadpack, 1, storedn, datfh));
			if (n == 0)
				return NULL;
		} else {
			if (size > 0x96000)
				size = 0x96000;
			memcpy(loadbuf, data, size);
			n = size;
		}
	}
	seq = (Seq *)LocalAlloc(LPTR, sizeof(Seq));
	if (seq == NULL)
		return NULL;
	seq->cBuffer = 4;
	seq->cbBuffer = 0x400;
	if (seqAllocBuffers(seq) != MMSYSERR_NOERROR)
		return seq;
	seq->hWnd = mainwnd;
	if (seqOpenFile(seq, (BYTE *)loadbuf, n) != MMSYSERR_NOERROR)
		return seq;
	musbytes += n;
	return seq;
}

void
freemidi(Seq *seq)
{
	if (seq == NULL)
		return;
	if (musloaded)
		seqStop(seq);
	seqCloseFile(seq);
	seqFreeBuffers(seq);
	LocalFree(seq);
}

int
loadintro(void)
{
	HANDLE h;

	if (musicoff || introok)
		return 0;
	pathjoin((char *)loadbuf, savesdir, T(0x9cd7));
	if (!fopenr(&h, (char *)loadbuf))
		h = NULL;
	songs[0] = loadmidi(0x1003, h, NULL, 0);
	if (h)
		fileclose(&h);
	if (!songs[0])
		return 0;
	introok = 1;
	return 1;
}

int
unloadintro(void)
{
	if (introok)
		return 0;
	freemidi(songs[0]);
	songs[0] = NULL;
	introok = 0;
	return 1;
}

int
loadsongs(void)
{
	int i;
	HANDLE h;

	if (musicoff || musloaded)
		return 0;
	musloaded = 1;
	musbytes = 0;
	for (i = 0; i < 36; i++) {
		if (i == 0)
			continue;
		if (i == 2) {
			pathjoin((char *)loadbuf, savesdir, T(0x9ce1));
			if (!fopenr(&h, (char *)loadbuf))
				h = NULL;
			songs[i] = loadmidi(0x1003 + i, h, NULL, 0);
			if (h)
				fileclose(&h);
		} else
			songs[i] = loadmidi(0x1003 + i, NULL, NULL, 0);
		if (songs[i] == NULL)
			musloaded = 0;
	}
	if (!musloaded) {
		for (i = 0; i < 36; i++) {
			if (i == 0)
				continue;
			freemidi(songs[i]);
			songs[i] = NULL;
		}
		musbytes = 0;
		return 0;
	}
	return 1;
}

int
freesongs(void)
{
	int i;

	if (!musloaded)
		return 0;
	for (i = 0; i < 36; i++) {
		if (i == 0)
			continue;
		freemidi(songs[i]);
		songs[i] = NULL;
	}
	musworks = 0;
	musloaded = 0;
	musbytes = 0;
	return 1;
}

int
songdone(WPARAM wp, LPARAM lp)
{
	return 0;
}

int
stopmusic(void)
{
	int n;

	n = cur;
	loop = 0;
	lastsong = 0;
	replaydue = 0;
	replaywait = 0;
	pos3 = 0;
	pos11 = 0;
	if (n == 0)
		return 1;
	cur = 0;
	if (seqStop(songs[--n]) == MMSYSERR_NOERROR)
		return 1;
	return 0;
}

int
playsong(int n, TICKS pos)
{
	Preroll pr;

	pr.tkBase = pos;
	pr.tkEnd = songs[n - 1]->tkLength;
	songs[n - 1]->uDeviceID = MIDI_MAPPER;
	if (seqPreroll(songs[n - 1], &pr) != MMSYSERR_NOERROR) {
		musworks = 0;
		return 0;
	}
	if (seqStart(songs[n - 1]) != MMSYSERR_NOERROR) {
		musworks = 0;
		return 0;
	}
	cur = n;
	return 1;
}

TICKS
songtime(void)
{
	TICKS t;

	if (cur == 0)
		return 0;
	if (seqTime(songs[cur - 1], &t) != MMSYSERR_NOERROR)
		return 0;
	return t;
}

int
music(int n, int lp)
{
	if ((n == 1 && introok) || musloaded) {
		if (musicoff)
			return 0;
		pollinput();
		if (n == cur)
			return 1;
		if (cur == 3)
			pos3 = songtime();
		if (cur == 11 && n != 3)
			pos11 = songtime();
		savepos3 = pos3;
		savepos11 = pos11;
		stopmusic();
		pos3 = savepos3;
		pos11 = savepos11;
		if (n == 0 || n > 0x25)
			return 0;
		musplayed = 1;
		if (n == 3 && savepos3) {
			playsong(n, pos3);
			pos3 = 0;
		} else if (n == 11 && savepos11) {
			playsong(n, pos11);
			pos11 = 0;
		} else
			playsong(n, 0);
		cur = n;
		loop = lp;
		lastsong = n;
		musworks = 1;
		return 1;
	}
	return 0;
}

int
replay(void)
{
	replaydue = 0;
	replaywait = 0;
	if ((lastsong == 1 && introok) || musloaded) {
		if (lastsong == 0) {
			stopmusic();
			return 0;
		}
		if (!playsong(lastsong, 0)) {
			musworks = 0;
			return 0;
		}
		cur = lastsong;
		musworks = 1;
		return 1;
	}
	return 0;
}

UINT
songstate(void)
{
	if (cur)
		return songs[cur - 1]->uState;
	return 0;
}

int
songbychar(char c)
{
	if (c == '!')
		songsel = 0x14;
	else if (c == '?')
		songsel = 0x15;
	else if (c == '$')
		songsel = 0x16;
	else if (c == '*')
		songsel = 0x17;
	else if (c == '<')
		songsel = 0x18;
	else if (c == ')')
		songsel = 0x19;
	else if (c == '%')
		songsel = 0x1a;
	else if (c == '(')
		songsel = 0x1b;
	else if (c == 'S')
		songsel = 4;
	else if (c == 'K')
		songsel = 5;
	else if (c == 's')
		songsel = 0xa;
	else if (c == 'O')
		songsel = 0x22;
	else if (c == '+')
		songsel = 0x1f;
	else if (c == 'v')
		songsel = 0x1c;
	else if (c == 'C')
		songsel = 0xc;
	else if (c == 'R')
		songsel = 0x20;
	else
		songsel = 0;
	return songsel;
}

int
togglemusic(void)
{
	if (musloaded || musicoff) {
		if (musicoff) {
			musicoff = 0;
			loadintro();
			return loadsongs();
		}
		musicoff = 2;
		stopmusic();
		freesongs();
		return unloadintro();
	}
	loadintro();
	return loadsongs();
}

char landsounds = 1;
unsigned char sfxswitch = 1;
char menusounds = 1;
int sfxvolume = 100;
short blasterport = 0x220;
char blasterdma = 1;
char blasterirq = 5;
char soundtext[2000] = "";
char capstext[2000] = "";
char musictext[2000] = "";
unsigned char soundson = 1;
short outalways = 1;
char dserrtext[1504] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1
};
int musicvolume = 100;
char dbgbuf[2000] = "";
char *dbgp = dbgbuf;
int grbChanMsgLen[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	3, 3, 3, 3, 2, 2, 3, 0,
};
int dsi;
int dsj;
DSBUFFERDESC primdesc;
LPVOID lsp1;
LPVOID lsp2;
DWORD lsn1;
DWORD lsn2;
DWORD lslen;
DWORD lsi;
unsigned char *lsdata;
int lsquiet;
signed char lsc;
short lsk;
DWORD lsn;
int rsi;
char *infop;
int ssi;
int qi;
DWORD pstatus;
LONG vol;
int pqi;
Keyframe kf;
DWORD musvol;
char *minfop;
int savepos3;
int savepos11;
int songsel;
char soundsok;
char cfgbyte1;
char cfgbyte2;
char dsready;
char dsmsg[304];
char dsshort[32];
DWORD swbytes;
DWORD hwbytes;
short queue[25];
short nqueue;
LPDIRECTSOUND ds;
DSCAPS dscaps;
LPDIRECTSOUNDBUFFER bufs[78];
LPDIRECTSOUNDBUFFER prim;
WAVEFORMATEX wfx;
short dsunused;
HRESULT dsres[2];
int dslost;
int cur;
int loop;
int lastsong;
int replaydue;
int replaywait;
int musplayed;
int musworks;
int musloaded;
DWORD musbytes;
int introok;
int pos3;
int pos11;
Seq *songs[36];

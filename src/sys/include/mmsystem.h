/*
 * The part of WinMM that the game names: timeGetTime, and the MIDI
 * stream API that Microsoft's MIDIPLYR sample, in snd.cpp, drives.
 */

#ifndef SYS_MMSYSTEM_H
#define SYS_MMSYSTEM_H

#include <windows.h>

#pragma pack(push, 8)

typedef UINT MMRESULT;
typedef DWORD FOURCC;
typedef HANDLE HMIDIOUT;
typedef HANDLE HMIDISTRM;
typedef HMIDIOUT *LPHMIDIOUT;
typedef HMIDISTRM *LPHMIDISTRM;

#define MMSYSERR_BASE 0
#define MMSYSERR_NOERROR 0
#define MMSYSERR_ERROR (MMSYSERR_BASE + 1)
#define MMSYSERR_BADDEVICEID (MMSYSERR_BASE + 2)
#define MMSYSERR_NOTENABLED (MMSYSERR_BASE + 3)
#define MMSYSERR_ALLOCATED (MMSYSERR_BASE + 4)
#define MMSYSERR_INVALHANDLE (MMSYSERR_BASE + 5)
#define MMSYSERR_NODRIVER (MMSYSERR_BASE + 6)
#define MMSYSERR_NOMEM (MMSYSERR_BASE + 7)
#define MMSYSERR_NOTSUPPORTED (MMSYSERR_BASE + 8)
#define MMSYSERR_INVALFLAG (MMSYSERR_BASE + 10)
#define MMSYSERR_INVALPARAM (MMSYSERR_BASE + 11)
#define MIDIERR_BASE 64
#define MIDIERR_UNPREPARED (MIDIERR_BASE + 0)
#define MIDIERR_STILLPLAYING (MIDIERR_BASE + 1)
#define MCIERR_BASE 256
#define MCIERR_OUT_OF_MEMORY (MCIERR_BASE + 8)
#define MCIERR_UNSUPPORTED_FUNCTION (MCIERR_BASE + 18)
#define MCIERR_DEVICE_NOT_READY (MCIERR_BASE + 20)
#define MCIERR_INVALID_FILE (MCIERR_BASE + 40)

#define CALLBACK_TYPEMASK 0x00070000
#define CALLBACK_NULL 0x00000000
#define CALLBACK_WINDOW 0x00010000
#define CALLBACK_FUNCTION 0x00030000

#define MM_MOM_OPEN 0x3C7
#define MM_MOM_CLOSE 0x3C8
#define MM_MOM_DONE 0x3C9
#define MOM_OPEN MM_MOM_OPEN
#define MOM_CLOSE MM_MOM_CLOSE
#define MOM_DONE MM_MOM_DONE

#define MIDI_MAPPER ((UINT)-1)

/* a buffer of MIDI events, as midiStreamOut queues it */
typedef struct MIDIHDR MIDIHDR;
struct MIDIHDR {
	LPSTR lpData;
	DWORD dwBufferLength;
	DWORD dwBytesRecorded;
	DWORD_PTR dwUser;
	DWORD dwFlags;
	MIDIHDR *lpNext;
	DWORD_PTR reserved;
	DWORD dwOffset;
	DWORD_PTR dwReserved[4];
};
typedef MIDIHDR *LPMIDIHDR;
typedef MIDIHDR *PMIDIHDR;

#define MHDR_DONE 0x00000001
#define MHDR_PREPARED 0x00000002
#define MHDR_INQUEUE 0x00000004
#define MHDR_ISSTRM 0x00000008

/*
 * In a stream buffer each event is three DWORDs, the delta time, the
 * stream id, and the event, then for a long event its parameter bytes
 * padded to a DWORD.
 */
#define MEVT_F_SHORT 0x00000000
#define MEVT_F_LONG 0x80000000
#define MEVT_F_CALLBACK 0x40000000
#define MEVT_EVENTTYPE(x) ((BYTE)(((x) >> 24) & 0xFF))
#define MEVT_EVENTPARM(x) ((DWORD)((x) & 0x00FFFFFF))
#define MEVT_SHORTMSG ((BYTE)0x00)
#define MEVT_TEMPO ((BYTE)0x01)
#define MEVT_NOP ((BYTE)0x02)
#define MEVT_LONGMSG ((BYTE)0x80)
#define MEVT_COMMENT ((BYTE)0x82)
#define MEVT_VERSION ((BYTE)0x84)

#define MIDIPROP_SET 0x80000000
#define MIDIPROP_GET 0x40000000
#define MIDIPROP_TIMEDIV 0x00000001
#define MIDIPROP_TEMPO 0x00000002

typedef struct MIDIPROPTIMEDIV MIDIPROPTIMEDIV;
struct MIDIPROPTIMEDIV {
	DWORD cbStruct;
	DWORD dwTimeDiv;
};
typedef MIDIPROPTIMEDIV *LPMIDIPROPTIMEDIV;

typedef struct MIDIPROPTEMPO MIDIPROPTEMPO;
struct MIDIPROPTEMPO {
	DWORD cbStruct;
	DWORD dwTempo;
};
typedef MIDIPROPTEMPO *LPMIDIPROPTEMPO;

#define TIME_MS 0x0001
#define TIME_SAMPLES 0x0002
#define TIME_BYTES 0x0004
#define TIME_TICKS 0x0020

typedef struct MMTIME MMTIME;
struct MMTIME {
	UINT wType;
	union {
		DWORD ms;
		DWORD sample;
		DWORD cb;
		DWORD ticks;
	} u;
};
typedef MMTIME *LPMMTIME;

#define WAVE_FORMAT_PCM 1

typedef struct WAVEFORMATEX WAVEFORMATEX;
struct WAVEFORMATEX {
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;
	WORD wBitsPerSample;
	WORD cbSize;
};
typedef WAVEFORMATEX *LPWAVEFORMATEX;
typedef const WAVEFORMATEX *LPCWAVEFORMATEX;

#define mmioFOURCC(a, b, c, d) ((DWORD)(BYTE)(a) | ((DWORD)(BYTE)(b) << 8) | \
	((DWORD)(BYTE)(c) << 16) | ((DWORD)(BYTE)(d) << 24))

#define midiOutPrepareHeader sysmidiOutPrepareHeader
#define midiOutReset sysmidiOutReset
#define midiOutSetVolume sysmidiOutSetVolume
#define midiOutUnprepareHeader sysmidiOutUnprepareHeader
#define midiStreamClose sysmidiStreamClose
#define midiStreamOpen sysmidiStreamOpen
#define midiStreamOut sysmidiStreamOut
#define midiStreamPause sysmidiStreamPause
#define midiStreamPosition sysmidiStreamPosition
#define midiStreamProperty sysmidiStreamProperty
#define midiStreamRestart sysmidiStreamRestart
#define midiStreamStop sysmidiStreamStop
#define timeBeginPeriod systimeBeginPeriod
#define timeGetTime systimeGetTime

MMRESULT midiOutPrepareHeader(HMIDIOUT, LPMIDIHDR, UINT);
MMRESULT midiOutReset(HMIDIOUT);
MMRESULT midiOutSetVolume(HMIDIOUT, DWORD);
MMRESULT midiOutUnprepareHeader(HMIDIOUT, LPMIDIHDR, UINT);
MMRESULT midiStreamClose(HMIDISTRM);
MMRESULT midiStreamOpen(LPHMIDISTRM, UINT *, DWORD, DWORD_PTR, DWORD_PTR, DWORD);
MMRESULT midiStreamOut(HMIDISTRM, LPMIDIHDR, UINT);
MMRESULT midiStreamPause(HMIDISTRM);
MMRESULT midiStreamPosition(HMIDISTRM, LPMMTIME, UINT);
MMRESULT midiStreamProperty(HMIDISTRM, LPBYTE, DWORD);
MMRESULT midiStreamRestart(HMIDISTRM);
MMRESULT midiStreamStop(HMIDISTRM);
MMRESULT timeBeginPeriod(UINT);
DWORD timeGetTime(void);

#pragma pack(pop)

#endif

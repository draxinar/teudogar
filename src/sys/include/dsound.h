/*
 * The part of DirectSound that the game names, for the port: the
 * interfaces IDirectSound and IDirectSoundBuffer with the methods the
 * game calls, in the C binding of the DirectX headers.
 */

#ifndef SYS_DSOUND_H
#define SYS_DSOUND_H

#include <windows.h>

#pragma pack(push, 8)

typedef struct IDirectSound IDirectSound;
typedef struct IDirectSoundBuffer IDirectSoundBuffer;
typedef IDirectSound *LPDIRECTSOUND;
typedef IDirectSoundBuffer *LPDIRECTSOUNDBUFFER;

#define _FACDS 0x878
#define MAKE_DSHRESULT(code) MAKE_HRESULT(1, _FACDS, code)

#define DS_OK 0
#define DSERR_ALLOCATED MAKE_DSHRESULT(10)
#define DSERR_CONTROLUNAVAIL MAKE_DSHRESULT(30)
#define DSERR_INVALIDPARAM E_INVALIDARG
#define DSERR_INVALIDCALL MAKE_DSHRESULT(50)
#define DSERR_GENERIC E_FAIL
#define DSERR_PRIOLEVELNEEDED MAKE_DSHRESULT(70)
#define DSERR_OUTOFMEMORY E_OUTOFMEMORY
#define DSERR_BADFORMAT MAKE_DSHRESULT(100)
#define DSERR_UNSUPPORTED E_NOTIMPL
#define DSERR_NODRIVER MAKE_DSHRESULT(120)
#define DSERR_ALREADYINITIALIZED MAKE_DSHRESULT(130)
#define DSERR_NOAGGREGATION CLASS_E_NOAGGREGATION
#define DSERR_BUFFERLOST MAKE_DSHRESULT(150)
#define DSERR_OTHERAPPHASPRIO MAKE_DSHRESULT(160)
#define DSERR_UNINITIALIZED MAKE_DSHRESULT(170)

#define DSSCL_NORMAL 1
#define DSSCL_PRIORITY 2
#define DSSCL_EXCLUSIVE 3

#define DSCAPS_PRIMARYMONO 0x00000001
#define DSCAPS_PRIMARYSTEREO 0x00000002
#define DSCAPS_PRIMARY8BIT 0x00000004
#define DSCAPS_PRIMARY16BIT 0x00000008
#define DSCAPS_EMULDRIVER 0x00000020
#define DSCAPS_SECONDARYMONO 0x00000100
#define DSCAPS_SECONDARYSTEREO 0x00000200
#define DSCAPS_SECONDARY8BIT 0x00000400
#define DSCAPS_SECONDARY16BIT 0x00000800

#define DSBCAPS_PRIMARYBUFFER 0x00000001
#define DSBCAPS_STATIC 0x00000002
#define DSBCAPS_LOCHARDWARE 0x00000004
#define DSBCAPS_LOCSOFTWARE 0x00000008
#define DSBCAPS_CTRLFREQUENCY 0x00000020
#define DSBCAPS_CTRLPAN 0x00000040
#define DSBCAPS_CTRLVOLUME 0x00000080
#define DSBCAPS_CTRLDEFAULT 0x000000E0

#define DSBPLAY_LOOPING 0x00000001
#define DSBSTATUS_PLAYING 0x00000001
#define DSBSTATUS_BUFFERLOST 0x00000002
#define DSBSTATUS_LOOPING 0x00000004

#define DSBVOLUME_MIN -10000
#define DSBVOLUME_MAX 0

typedef struct DSCAPS DSCAPS;
struct DSCAPS {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwMinSecondarySampleRate;
	DWORD dwMaxSecondarySampleRate;
	DWORD dwPrimaryBuffers;
	DWORD dwMaxHwMixingAllBuffers;
	DWORD dwMaxHwMixingStaticBuffers;
	DWORD dwMaxHwMixingStreamingBuffers;
	DWORD dwFreeHwMixingAllBuffers;
	DWORD dwFreeHwMixingStaticBuffers;
	DWORD dwFreeHwMixingStreamingBuffers;
	DWORD dwMaxHw3DAllBuffers;
	DWORD dwMaxHw3DStaticBuffers;
	DWORD dwMaxHw3DStreamingBuffers;
	DWORD dwFreeHw3DAllBuffers;
	DWORD dwFreeHw3DStaticBuffers;
	DWORD dwFreeHw3DStreamingBuffers;
	DWORD dwTotalHwMemBytes;
	DWORD dwFreeHwMemBytes;
	DWORD dwMaxContigFreeHwMemBytes;
	DWORD dwUnlockTransferRateHwBuffers;
	DWORD dwPlayCpuOverheadSwBuffers;
	DWORD dwReserved1;
	DWORD dwReserved2;
};
typedef DSCAPS *LPDSCAPS;

typedef struct DSBCAPS DSBCAPS;
struct DSBCAPS {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwBufferBytes;
	DWORD dwUnlockTransferRate;
	DWORD dwPlayCpuOverhead;
};
typedef DSBCAPS *LPDSBCAPS;

typedef struct DSBUFFERDESC DSBUFFERDESC;
struct DSBUFFERDESC {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwBufferBytes;
	DWORD dwReserved;
	LPWAVEFORMATEX lpwfxFormat;
};
typedef DSBUFFERDESC *LPDSBUFFERDESC;

typedef struct IDirectSoundVtbl IDirectSoundVtbl;
struct IDirectSoundVtbl {
	ULONG (*Release)(IDirectSound *);
	HRESULT (*CreateSoundBuffer)(IDirectSound *, LPDSBUFFERDESC,
		LPDIRECTSOUNDBUFFER *, IUnknown *);
	HRESULT (*GetCaps)(IDirectSound *, LPDSCAPS);
	HRESULT (*SetCooperativeLevel)(IDirectSound *, HWND, DWORD);
};
struct IDirectSound {
	const IDirectSoundVtbl *lpVtbl;
};

typedef struct IDirectSoundBufferVtbl IDirectSoundBufferVtbl;
struct IDirectSoundBufferVtbl {
	ULONG (*Release)(IDirectSoundBuffer *);
	HRESULT (*GetCaps)(IDirectSoundBuffer *, LPDSBCAPS);
	HRESULT (*GetStatus)(IDirectSoundBuffer *, LPDWORD);
	HRESULT (*Lock)(IDirectSoundBuffer *, DWORD, DWORD, LPVOID *, LPDWORD,
		LPVOID *, LPDWORD, DWORD);
	HRESULT (*Play)(IDirectSoundBuffer *, DWORD, DWORD, DWORD);
	HRESULT (*Restore)(IDirectSoundBuffer *);
	HRESULT (*SetCurrentPosition)(IDirectSoundBuffer *, DWORD);
	HRESULT (*SetFormat)(IDirectSoundBuffer *, LPCWAVEFORMATEX);
	HRESULT (*SetVolume)(IDirectSoundBuffer *, LONG);
	HRESULT (*Stop)(IDirectSoundBuffer *);
	HRESULT (*Unlock)(IDirectSoundBuffer *, LPVOID, DWORD, LPVOID, DWORD);
};
struct IDirectSoundBuffer {
	const IDirectSoundBufferVtbl *lpVtbl;
};

#define IDirectSound_Release(p) (p)->lpVtbl->Release(p)
#define IDirectSound_CreateSoundBuffer(p, a, b, c) (p)->lpVtbl->CreateSoundBuffer(p, a, b, c)
#define IDirectSound_GetCaps(p, a) (p)->lpVtbl->GetCaps(p, a)
#define IDirectSound_SetCooperativeLevel(p, a, b) (p)->lpVtbl->SetCooperativeLevel(p, a, b)

#define IDirectSoundBuffer_Release(p) (p)->lpVtbl->Release(p)
#define IDirectSoundBuffer_GetCaps(p, a) (p)->lpVtbl->GetCaps(p, a)
#define IDirectSoundBuffer_GetStatus(p, a) (p)->lpVtbl->GetStatus(p, a)
#define IDirectSoundBuffer_Lock(p, a, b, c, d, e, f, g) (p)->lpVtbl->Lock(p, a, b, c, d, e, f, g)
#define IDirectSoundBuffer_Play(p, a, b, c) (p)->lpVtbl->Play(p, a, b, c)
#define IDirectSoundBuffer_Restore(p) (p)->lpVtbl->Restore(p)
#define IDirectSoundBuffer_SetCurrentPosition(p, a) (p)->lpVtbl->SetCurrentPosition(p, a)
#define IDirectSoundBuffer_SetFormat(p, a) (p)->lpVtbl->SetFormat(p, a)
#define IDirectSoundBuffer_SetVolume(p, a) (p)->lpVtbl->SetVolume(p, a)
#define IDirectSoundBuffer_Stop(p) (p)->lpVtbl->Stop(p)
#define IDirectSoundBuffer_Unlock(p, a, b, c, d) (p)->lpVtbl->Unlock(p, a, b, c, d)

#define DirectSoundCreate sysDirectSoundCreate

HRESULT DirectSoundCreate(GUID *, LPDIRECTSOUND *, IUnknown *);

#pragma pack(pop)

#endif

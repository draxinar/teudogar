/*
 * The part of DirectDraw that the game names, for the port: the
 * interfaces IDirectDraw, IDirectDrawSurface, and IDirectDrawPalette
 * with the methods the game calls, in the C binding of the DirectX
 * headers. The anonymous unions of the SDK's structs keep the one
 * member the game names.
 */

#ifndef SYS_DDRAW_H
#define SYS_DDRAW_H

#include <windows.h>

#pragma pack(push, 8)

typedef struct IDirectDraw IDirectDraw;
typedef struct IDirectDrawSurface IDirectDrawSurface;
typedef struct IDirectDrawPalette IDirectDrawPalette;
typedef struct IDirectDrawClipper IDirectDrawClipper;
typedef IDirectDraw *LPDIRECTDRAW;
typedef IDirectDrawSurface *LPDIRECTDRAWSURFACE;
typedef IDirectDrawPalette *LPDIRECTDRAWPALETTE;
typedef IDirectDrawClipper *LPDIRECTDRAWCLIPPER;

#define _FACDD 0x876
#define MAKE_DDHRESULT(code) MAKE_HRESULT(1, _FACDD, code)

#define DD_OK 0
#define DDERR_ALREADYINITIALIZED MAKE_DDHRESULT(5)
#define DDERR_CANNOTATTACHSURFACE MAKE_DDHRESULT(10)
#define DDERR_CANNOTDETACHSURFACE MAKE_DDHRESULT(20)
#define DDERR_CURRENTLYNOTAVAIL MAKE_DDHRESULT(40)
#define DDERR_EXCEPTION MAKE_DDHRESULT(55)
#define DDERR_GENERIC E_FAIL
#define DDERR_HEIGHTALIGN MAKE_DDHRESULT(90)
#define DDERR_INCOMPATIBLEPRIMARY MAKE_DDHRESULT(95)
#define DDERR_INVALIDCAPS MAKE_DDHRESULT(100)
#define DDERR_INVALIDCLIPLIST MAKE_DDHRESULT(110)
#define DDERR_INVALIDMODE MAKE_DDHRESULT(120)
#define DDERR_INVALIDOBJECT MAKE_DDHRESULT(130)
#define DDERR_INVALIDPARAMS E_INVALIDARG
#define DDERR_INVALIDPIXELFORMAT MAKE_DDHRESULT(145)
#define DDERR_INVALIDRECT MAKE_DDHRESULT(150)
#define DDERR_LOCKEDSURFACES MAKE_DDHRESULT(160)
#define DDERR_NO3D MAKE_DDHRESULT(170)
#define DDERR_NOALPHAHW MAKE_DDHRESULT(180)
#define DDERR_NOCLIPLIST MAKE_DDHRESULT(205)
#define DDERR_NOCOLORCONVHW MAKE_DDHRESULT(210)
#define DDERR_NOCOOPERATIVELEVELSET MAKE_DDHRESULT(212)
#define DDERR_NOCOLORKEY MAKE_DDHRESULT(215)
#define DDERR_NOCOLORKEYHW MAKE_DDHRESULT(220)
#define DDERR_NOEXCLUSIVEMODE MAKE_DDHRESULT(225)
#define DDERR_NOFLIPHW MAKE_DDHRESULT(230)
#define DDERR_NOGDI MAKE_DDHRESULT(240)
#define DDERR_NOMIRRORHW MAKE_DDHRESULT(250)
#define DDERR_NOTFOUND MAKE_DDHRESULT(255)
#define DDERR_NOOVERLAYHW MAKE_DDHRESULT(260)
#define DDERR_NORASTEROPHW MAKE_DDHRESULT(280)
#define DDERR_NOROTATIONHW MAKE_DDHRESULT(290)
#define DDERR_NOSTRETCHHW MAKE_DDHRESULT(310)
#define DDERR_NOT4BITCOLOR MAKE_DDHRESULT(316)
#define DDERR_NOT4BITCOLORINDEX MAKE_DDHRESULT(317)
#define DDERR_NOT8BITCOLOR MAKE_DDHRESULT(320)
#define DDERR_NOTEXTUREHW MAKE_DDHRESULT(330)
#define DDERR_NOVSYNCHW MAKE_DDHRESULT(335)
#define DDERR_NOZBUFFERHW MAKE_DDHRESULT(340)
#define DDERR_NOZOVERLAYHW MAKE_DDHRESULT(350)
#define DDERR_OUTOFCAPS MAKE_DDHRESULT(360)
#define DDERR_OUTOFMEMORY E_OUTOFMEMORY
#define DDERR_OUTOFVIDEOMEMORY MAKE_DDHRESULT(380)
#define DDERR_OVERLAYCANTCLIP MAKE_DDHRESULT(382)
#define DDERR_OVERLAYCOLORKEYONLYONEACTIVE MAKE_DDHRESULT(384)
#define DDERR_PALETTEBUSY MAKE_DDHRESULT(387)
#define DDERR_COLORKEYNOTSET MAKE_DDHRESULT(400)
#define DDERR_SURFACEALREADYATTACHED MAKE_DDHRESULT(410)
#define DDERR_SURFACEALREADYDEPENDENT MAKE_DDHRESULT(420)
#define DDERR_SURFACEBUSY MAKE_DDHRESULT(430)
#define DDERR_SURFACEISOBSCURED MAKE_DDHRESULT(440)
#define DDERR_SURFACELOST MAKE_DDHRESULT(450)
#define DDERR_SURFACENOTATTACHED MAKE_DDHRESULT(460)
#define DDERR_TOOBIGHEIGHT MAKE_DDHRESULT(470)
#define DDERR_TOOBIGSIZE MAKE_DDHRESULT(480)
#define DDERR_TOOBIGWIDTH MAKE_DDHRESULT(490)
#define DDERR_UNSUPPORTED E_NOTIMPL
#define DDERR_UNSUPPORTEDFORMAT MAKE_DDHRESULT(510)
#define DDERR_UNSUPPORTEDMASK MAKE_DDHRESULT(520)
#define DDERR_VERTICALBLANKINPROGRESS MAKE_DDHRESULT(537)
#define DDERR_WASSTILLDRAWING MAKE_DDHRESULT(540)
#define DDERR_XALIGN MAKE_DDHRESULT(560)
#define DDERR_INVALIDDIRECTDRAWGUID MAKE_DDHRESULT(561)
#define DDERR_DIRECTDRAWALREADYCREATED MAKE_DDHRESULT(562)
#define DDERR_NODIRECTDRAWHW MAKE_DDHRESULT(563)
#define DDERR_PRIMARYSURFACEALREADYEXISTS MAKE_DDHRESULT(564)
#define DDERR_NOEMULATION MAKE_DDHRESULT(565)
#define DDERR_REGIONTOOSMALL MAKE_DDHRESULT(566)
#define DDERR_CLIPPERISUSINGHWND MAKE_DDHRESULT(567)
#define DDERR_NOCLIPPERATTACHED MAKE_DDHRESULT(568)
#define DDERR_NOHWND MAKE_DDHRESULT(569)
#define DDERR_HWNDSUBCLASSED MAKE_DDHRESULT(570)
#define DDERR_HWNDALREADYSET MAKE_DDHRESULT(571)
#define DDERR_NOPALETTEATTACHED MAKE_DDHRESULT(572)
#define DDERR_NOPALETTEHW MAKE_DDHRESULT(573)
#define DDERR_BLTFASTCANTCLIP MAKE_DDHRESULT(574)
#define DDERR_NOBLTHW MAKE_DDHRESULT(575)
#define DDERR_NODDROPSHW MAKE_DDHRESULT(576)
#define DDERR_OVERLAYNOTVISIBLE MAKE_DDHRESULT(577)
#define DDERR_NOOVERLAYDEST MAKE_DDHRESULT(578)
#define DDERR_INVALIDPOSITION MAKE_DDHRESULT(579)
#define DDERR_NOTAOVERLAYSURFACE MAKE_DDHRESULT(580)
#define DDERR_EXCLUSIVEMODEALREADYSET MAKE_DDHRESULT(581)
#define DDERR_NOTFLIPPABLE MAKE_DDHRESULT(582)
#define DDERR_CANTDUPLICATE MAKE_DDHRESULT(583)
#define DDERR_NOTLOCKED MAKE_DDHRESULT(584)
#define DDERR_CANTCREATEDC MAKE_DDHRESULT(585)
#define DDERR_NODC MAKE_DDHRESULT(586)
#define DDERR_WRONGMODE MAKE_DDHRESULT(587)
#define DDERR_IMPLICITLYCREATED MAKE_DDHRESULT(588)
#define DDERR_NOTPALETTIZED MAKE_DDHRESULT(589)

#define DDSCL_FULLSCREEN 0x00000001
#define DDSCL_ALLOWREBOOT 0x00000002
#define DDSCL_NOWINDOWCHANGES 0x00000004
#define DDSCL_NORMAL 0x00000008
#define DDSCL_EXCLUSIVE 0x00000010

#define DDSD_CAPS 0x00000001
#define DDSD_HEIGHT 0x00000002
#define DDSD_WIDTH 0x00000004
#define DDSD_PITCH 0x00000008
#define DDSD_BACKBUFFERCOUNT 0x00000020
#define DDSD_PIXELFORMAT 0x00001000

#define DDSCAPS_BACKBUFFER 0x00000004
#define DDSCAPS_COMPLEX 0x00000008
#define DDSCAPS_FLIP 0x00000010
#define DDSCAPS_FRONTBUFFER 0x00000020
#define DDSCAPS_OFFSCREENPLAIN 0x00000040
#define DDSCAPS_PRIMARYSURFACE 0x00000200
#define DDSCAPS_SYSTEMMEMORY 0x00000800
#define DDSCAPS_VIDEOMEMORY 0x00004000

#define DDCAPS_BLT 0x00000040
#define DDCAPS_COLORKEY 0x00400000
#define DDCAPS_BLTCOLORFILL 0x04000000
#define DDCKEYCAPS_SRCBLT 0x00000200
#define DDCKEY_SRCBLT 0x00000008

#define DDPCAPS_8BIT 0x00000004
#define DDPCAPS_ALLOW256 0x00000040

#define DDBLT_COLORFILL 0x00000400
#define DDBLT_DDFX 0x00000800
#define DDBLT_WAIT 0x01000000
#define DDBLTFX_MIRRORUPDOWN 0x00000004
#define DDBLTFAST_NOCOLORKEY 0x00000000
#define DDBLTFAST_SRCCOLORKEY 0x00000001
#define DDBLTFAST_DESTCOLORKEY 0x00000002
#define DDBLTFAST_WAIT 0x00000010
#define DDFLIP_WAIT 0x00000001
#define DDLOCK_SURFACEMEMORYPTR 0x00000000
#define DDLOCK_WAIT 0x00000001
#define DDWAITVB_BLOCKBEGIN 0x00000001

typedef struct DDCOLORKEY DDCOLORKEY;
struct DDCOLORKEY {
	DWORD dwColorSpaceLowValue;
	DWORD dwColorSpaceHighValue;
};
typedef DDCOLORKEY *LPDDCOLORKEY;

typedef struct DDSCAPS DDSCAPS;
struct DDSCAPS {
	DWORD dwCaps;
};
typedef DDSCAPS *LPDDSCAPS;

typedef struct DDPIXELFORMAT DDPIXELFORMAT;
struct DDPIXELFORMAT {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwFourCC;
	DWORD dwRGBBitCount;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;
	DWORD dwRGBAlphaBitMask;
};

typedef struct DDSURFACEDESC DDSURFACEDESC;
struct DDSURFACEDESC {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwHeight;
	DWORD dwWidth;
	LONG lPitch;
	DWORD dwBackBufferCount;
	DWORD dwRefreshRate;
	DWORD dwAlphaBitDepth;
	DWORD dwReserved;
	LPVOID lpSurface;
	DDCOLORKEY ddckCKDestOverlay;
	DDCOLORKEY ddckCKDestBlt;
	DDCOLORKEY ddckCKSrcOverlay;
	DDCOLORKEY ddckCKSrcBlt;
	DDPIXELFORMAT ddpfPixelFormat;
	DDSCAPS ddsCaps;
};
typedef DDSURFACEDESC *LPDDSURFACEDESC;

typedef struct DDBLTFX DDBLTFX;
struct DDBLTFX {
	DWORD dwSize;
	DWORD dwDDFX;
	DWORD dwROP;
	DWORD dwDDROP;
	DWORD dwRotationAngle;
	DWORD dwFillColor;
	DDCOLORKEY ddckDestColorkey;
	DDCOLORKEY ddckSrcColorkey;
};
typedef DDBLTFX *LPDDBLTFX;

typedef struct DDCAPS DDCAPS;
struct DDCAPS {
	DWORD dwSize;
	DWORD dwCaps;
	DWORD dwCaps2;
	DWORD dwCKeyCaps;
	DWORD dwFXCaps;
	DWORD dwFXAlphaCaps;
	DWORD dwPalCaps;
	DWORD dwSVCaps;
	DWORD dwVidMemTotal;
	DWORD dwVidMemFree;
	DDSCAPS ddsCaps;
};
typedef DDCAPS *LPDDCAPS;

typedef struct IDirectDrawVtbl IDirectDrawVtbl;
struct IDirectDrawVtbl {
	ULONG (*Release)(IDirectDraw *);
	HRESULT (*CreatePalette)(IDirectDraw *, DWORD, LPPALETTEENTRY,
		LPDIRECTDRAWPALETTE *, IUnknown *);
	HRESULT (*CreateSurface)(IDirectDraw *, LPDDSURFACEDESC,
		LPDIRECTDRAWSURFACE *, IUnknown *);
	HRESULT (*GetCaps)(IDirectDraw *, LPDDCAPS, LPDDCAPS);
	HRESULT (*GetVerticalBlankStatus)(IDirectDraw *, LPBOOL);
	HRESULT (*SetCooperativeLevel)(IDirectDraw *, HWND, DWORD);
	HRESULT (*SetDisplayMode)(IDirectDraw *, DWORD, DWORD, DWORD);
	HRESULT (*WaitForVerticalBlank)(IDirectDraw *, DWORD, HANDLE);
};
struct IDirectDraw {
	const IDirectDrawVtbl *lpVtbl;
};

typedef struct IDirectDrawSurfaceVtbl IDirectDrawSurfaceVtbl;
struct IDirectDrawSurfaceVtbl {
	ULONG (*Release)(IDirectDrawSurface *);
	HRESULT (*Blt)(IDirectDrawSurface *, LPRECT, LPDIRECTDRAWSURFACE,
		LPRECT, DWORD, LPDDBLTFX);
	HRESULT (*BltFast)(IDirectDrawSurface *, DWORD, DWORD,
		LPDIRECTDRAWSURFACE, LPRECT, DWORD);
	HRESULT (*Flip)(IDirectDrawSurface *, LPDIRECTDRAWSURFACE, DWORD);
	HRESULT (*GetAttachedSurface)(IDirectDrawSurface *, LPDDSCAPS,
		LPDIRECTDRAWSURFACE *);
	HRESULT (*GetDC)(IDirectDrawSurface *, HDC *);
	HRESULT (*GetSurfaceDesc)(IDirectDrawSurface *, LPDDSURFACEDESC);
	HRESULT (*Lock)(IDirectDrawSurface *, LPRECT, LPDDSURFACEDESC, DWORD,
		HANDLE);
	HRESULT (*ReleaseDC)(IDirectDrawSurface *, HDC);
	HRESULT (*Restore)(IDirectDrawSurface *);
	HRESULT (*SetColorKey)(IDirectDrawSurface *, DWORD, LPDDCOLORKEY);
	HRESULT (*SetPalette)(IDirectDrawSurface *, LPDIRECTDRAWPALETTE);
	HRESULT (*Unlock)(IDirectDrawSurface *, LPVOID);
};
struct IDirectDrawSurface {
	const IDirectDrawSurfaceVtbl *lpVtbl;
};

typedef struct IDirectDrawPaletteVtbl IDirectDrawPaletteVtbl;
struct IDirectDrawPaletteVtbl {
	ULONG (*Release)(IDirectDrawPalette *);
	HRESULT (*GetEntries)(IDirectDrawPalette *, DWORD, DWORD, DWORD,
		LPPALETTEENTRY);
	HRESULT (*SetEntries)(IDirectDrawPalette *, DWORD, DWORD, DWORD,
		LPPALETTEENTRY);
};
struct IDirectDrawPalette {
	const IDirectDrawPaletteVtbl *lpVtbl;
};

#define IDirectDraw_Release(p) (p)->lpVtbl->Release(p)
#define IDirectDraw_CreatePalette(p, a, b, c, d) (p)->lpVtbl->CreatePalette(p, a, b, c, d)
#define IDirectDraw_CreateSurface(p, a, b, c) (p)->lpVtbl->CreateSurface(p, a, b, c)
#define IDirectDraw_GetCaps(p, a, b) (p)->lpVtbl->GetCaps(p, a, b)
#define IDirectDraw_GetVerticalBlankStatus(p, a) (p)->lpVtbl->GetVerticalBlankStatus(p, a)
#define IDirectDraw_SetCooperativeLevel(p, a, b) (p)->lpVtbl->SetCooperativeLevel(p, a, b)
#define IDirectDraw_SetDisplayMode(p, a, b, c) (p)->lpVtbl->SetDisplayMode(p, a, b, c)
#define IDirectDraw_WaitForVerticalBlank(p, a, b) (p)->lpVtbl->WaitForVerticalBlank(p, a, b)

#define IDirectDrawSurface_Release(p) (p)->lpVtbl->Release(p)
#define IDirectDrawSurface_Blt(p, a, b, c, d, e) (p)->lpVtbl->Blt(p, a, b, c, d, e)
#define IDirectDrawSurface_BltFast(p, a, b, c, d, e) (p)->lpVtbl->BltFast(p, a, b, c, d, e)
#define IDirectDrawSurface_Flip(p, a, b) (p)->lpVtbl->Flip(p, a, b)
#define IDirectDrawSurface_GetAttachedSurface(p, a, b) (p)->lpVtbl->GetAttachedSurface(p, a, b)
#define IDirectDrawSurface_GetDC(p, a) (p)->lpVtbl->GetDC(p, a)
#define IDirectDrawSurface_GetSurfaceDesc(p, a) (p)->lpVtbl->GetSurfaceDesc(p, a)
#define IDirectDrawSurface_Lock(p, a, b, c, d) (p)->lpVtbl->Lock(p, a, b, c, d)
#define IDirectDrawSurface_ReleaseDC(p, a) (p)->lpVtbl->ReleaseDC(p, a)
#define IDirectDrawSurface_Restore(p) (p)->lpVtbl->Restore(p)
#define IDirectDrawSurface_SetColorKey(p, a, b) (p)->lpVtbl->SetColorKey(p, a, b)
#define IDirectDrawSurface_SetPalette(p, a) (p)->lpVtbl->SetPalette(p, a)
#define IDirectDrawSurface_Unlock(p, a) (p)->lpVtbl->Unlock(p, a)

#define IDirectDrawPalette_Release(p) (p)->lpVtbl->Release(p)
#define IDirectDrawPalette_GetEntries(p, a, b, c, d) (p)->lpVtbl->GetEntries(p, a, b, c, d)
#define IDirectDrawPalette_SetEntries(p, a, b, c, d) (p)->lpVtbl->SetEntries(p, a, b, c, d)

#define DirectDrawCreate sysDirectDrawCreate

HRESULT DirectDrawCreate(GUID *, LPDIRECTDRAW *, IUnknown *);

#pragma pack(pop)

#endif

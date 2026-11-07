/*
 * The test of build/ddraw/ddraw.dll, which runs under Wine beside it with
 * WINEDLLOVERRIDES=ddraw=n,b, as make ddraw builds it. It opens the
 * primary surface and its back buffer in a window, as the game does,
 * then checks that BltFast refuses a source wider than its destination,
 * that the rows above and below the back buffer take the writes of the
 * game's text outline, and that a run of Flips keeps the display
 * thread's pace. It prints each check that fails, and exits 1 then.
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>
#include <ddraw.h>

enum {
	Flips = 30	/* the Flips of the run, half a second at 60 frames a second */
};

typedef HRESULT (WINAPI *Createfn)(GUID *, LPDIRECTDRAW *, IUnknown *);

static int failed;

static void
check(int ok, const char *what)
{
	if (!ok) {
		printf("ddraw: %s\n", what);
		failed = 1;
	}
}

/* offscreen makes a surface of w by h pixels in system memory. */
static LPDIRECTDRAWSURFACE
offscreen(LPDIRECTDRAW dd, int w, int h)
{
	LPDIRECTDRAWSURFACE s;
	DDSURFACEDESC sd;

	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	sd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	sd.dwWidth = w;
	sd.dwHeight = h;
	sd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	if (IDirectDraw_CreateSurface(dd, &sd, &s, NULL) != DD_OK)
		return NULL;
	return s;
}

int
main(void)
{
	LPDIRECTDRAWSURFACE prim, back, dst, src;
	LPDIRECTDRAW dd;
	DDSURFACEDESC sd;
	DDSCAPS caps;
	HMODULE dll;
	Createfn create;
	HWND w;
	char path[MAX_PATH];
	unsigned char *p;
	DWORD t;
	int i, y;

	dll = LoadLibraryA("ddraw.dll");
	if (dll == NULL || GetModuleFileNameA(dll, path, sizeof path) == 0 ||
	    strstr(path, "system32") != NULL || strstr(path, "syswow64") != NULL) {
		printf("ddraw: the DLL beside the test failed to load, set WINEDLLOVERRIDES=ddraw=n,b\n");
		return 1;
	}
	create = (Createfn)GetProcAddress(dll, "DirectDrawCreate");
	w = CreateWindowExA(0, "STATIC", "ddraw test", WS_POPUP | WS_VISIBLE, 0, 0, 640, 480,
		NULL, NULL, NULL, NULL);
	if (create == NULL || w == NULL || create(NULL, &dd, NULL) != DD_OK) {
		printf("ddraw: DirectDraw failed to open\n");
		return 1;
	}
	IDirectDraw_SetCooperativeLevel(dd, w, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	check(IDirectDraw_SetDisplayMode(dd, 640, 480, 8) == DD_OK, "640 by 480 by 8 is a mode");
	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	sd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	sd.dwBackBufferCount = 1;
	sd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	if (IDirectDraw_CreateSurface(dd, &sd, &prim, NULL) != DD_OK) {
		printf("ddraw: the primary surface failed to open\n");
		return 1;
	}
	memset(&caps, 0, sizeof caps);
	caps.dwCaps = DDSCAPS_BACKBUFFER;
	if (IDirectDrawSurface_GetAttachedSurface(prim, &caps, &back) != DD_OK) {
		printf("ddraw: the back buffer failed to attach\n");
		return 1;
	}

	dst = offscreen(dd, 32, 16);
	src = offscreen(dd, 64, 8);
	check(dst != NULL && src != NULL, "offscreen surfaces open");
	if (dst != NULL && src != NULL)
		check(IDirectDrawSurface_BltFast(dst, 0, 0, src, NULL, 0) == DDERR_INVALIDRECT,
			"BltFast refuses a source wider than its destination");

	/* the outline of a glyph on row 0 or 479, 3 rows out, as glyph3 draws it */
	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	if (IDirectDrawSurface_Lock(back, NULL, &sd, DDLOCK_WAIT, NULL) == DD_OK) {
		p = sd.lpSurface;
		for (y = 1; y <= 3; y++) {
			memset(p - y * sd.lPitch, 0xf4, 640);
			memset(p + (479 + y) * sd.lPitch, 0xf4, 640);
		}
		IDirectDrawSurface_Unlock(back, NULL);
	}
	check(HeapValidate((HANDLE)_get_heap_handle(), 0, NULL), "the outline past the rows leaves the heap whole");

	t = GetTickCount();
	for (i = 0; i < Flips; i++)
		if (IDirectDrawSurface_Flip(prim, NULL, DDFLIP_WAIT) != DD_OK)
			break;
	t = GetTickCount() - t;
	check(i == Flips, "every Flip succeeds");
	check(t >= Flips * 1000 / 60 * 3 / 4 && t < 5000, "the Flips keep the display thread's pace");
	if (t >= 5000)
		printf("ddraw: %d Flips took %lu ms\n", Flips, (unsigned long)t);
	return failed;
}

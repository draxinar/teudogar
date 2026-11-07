/*
 * The surfaces of src/sys/draw.c: Blt's color fill and BltFast's copy,
 * with and without the source's color key, the rectangles that
 * DirectDraw refuses with DDERR_INVALIDRECT, Flip, which swaps the
 * pixels of the primary surface and its back buffer, the rows above and
 * below a surface that the game's text outline writes, and on Linux the
 * texture and the renderer that a reset and a loss of the renderer's
 * device make again. The test prints each check that fails.
 */

#include <stdio.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <windows.h>
#include <ddraw.h>

#include "sys.h"
#include "test.h"

#ifndef _WIN32
/* checkdevice pushes a reset and a loss of the renderer's device, as SDL sends them. */
static void
checkdevice(void)
{
	SDL_Event e;
	SDL_PropertiesID props;
	SDL_RendererLogicalPresentation mode;
	Uint32 n;
	int w, h;

	if (!SDL_Init(SDL_INIT_VIDEO))
		sysfatal("draw: SDL: %s", SDL_GetError());
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, SDL_SOFTWARE_RENDERER);
	wininit();
	drawexposed();
	n = systextures;
	check(n == 1, "a frame makes the texture");
	props = SDL_GetRendererProperties(sysrenderer);

	memset(&e, 0, sizeof e);
	e.type = SDL_EVENT_RENDER_DEVICE_RESET;
	SDL_PushEvent(&e);
	winpump(0);
	drawexposed();
	check(systextures == n + 1, "a reset of the device makes the texture again");
	check(SDL_GetRendererProperties(sysrenderer) == props, "a reset of the device keeps the renderer");

	e.type = SDL_EVENT_RENDER_DEVICE_LOST;
	SDL_PushEvent(&e);
	winpump(0);
	drawexposed();
	check(SDL_GetRendererProperties(sysrenderer) != props, "a loss of the device makes the renderer again");
	check(systextures == n + 2, "a loss of the device makes the texture again");
	check(SDL_GetRenderLogicalPresentation(sysrenderer, &w, &h, &mode) && w == sysscreenw && h == Gameh,
		"the new renderer presents the screen's width by 480 rows");
}
#endif

int
main(void)
{
	LPDIRECTDRAW dd;
	LPDIRECTDRAWSURFACE dst, src, prim, back;
	DDSURFACEDESC sd;
	DDBLTFX fx;
	DDCOLORKEY key;
	DDSCAPS caps;
	RECT r;
	unsigned char *p;
	LONG pitch;
	int x, y, ok;

	if (DirectDrawCreate(NULL, &dd, NULL) != DD_OK)
		sysfatal("draw: no DirectDraw");
	dst = offscreen(dd, 32, 16);
	src = offscreen(dd, 8, 8);

	p = lock(dst, &pitch);
	check(pitch == 32, "an offscreen surface's pitch is its width");
	memset(p, 1, 32 * 16);
	IDirectDrawSurface_Unlock(dst, NULL);

	memset(&fx, 0, sizeof fx);
	fx.dwSize = sizeof fx;
	fx.dwFillColor = 7;
	setrect(&r, 2, 2, 6, 6);
	check(IDirectDrawSurface_Blt(dst, &r, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx) == DD_OK,
		"a color fill succeeds");
	p = lock(dst, &pitch);
	ok = 1;
	for (y = 0; y < 16; y++)
		for (x = 0; x < 32; x++)
			if (p[y * pitch + x] != (x >= 2 && x < 6 && y >= 2 && y < 6 ? 7 : 1))
				ok = 0;
	check(ok, "the fill covers the rectangle alone");
	IDirectDrawSurface_Unlock(dst, NULL);

	setrect(&r, 6, 2, 2, 6);
	check(IDirectDrawSurface_Blt(dst, &r, NULL, NULL, DDBLT_COLORFILL, &fx) == DDERR_INVALIDRECT,
		"an inverted rectangle is invalid");
	setrect(&r, 2, 2, 2, 6);
	check(IDirectDrawSurface_Blt(dst, &r, NULL, NULL, DDBLT_COLORFILL, &fx) == DDERR_INVALIDRECT,
		"an empty rectangle is invalid");
	setrect(&r, 30, 2, 34, 6);
	check(IDirectDrawSurface_Blt(dst, &r, NULL, NULL, DDBLT_COLORFILL, &fx) == DDERR_INVALIDRECT,
		"a rectangle past the edge is invalid");

	p = lock(src, &pitch);
	for (y = 0; y < 8; y++)
		for (x = 0; x < 8; x++)
			p[y * pitch + x] = (x + y) % 2 ? 0 : 9;
	IDirectDrawSurface_Unlock(src, NULL);
	key.dwColorSpaceLowValue = key.dwColorSpaceHighValue = 0;
	check(IDirectDrawSurface_SetColorKey(src, DDCKEY_SRCBLT, &key) == DD_OK, "the color key takes 0");
	setrect(&r, 0, 0, 8, 8);
	check(IDirectDrawSurface_BltFast(dst, 20, 4, src, &r, DDBLTFAST_SRCCOLORKEY) == DD_OK,
		"a keyed copy succeeds");
	check(IDirectDrawSurface_BltFast(dst, 28, 4, src, &r, DDBLTFAST_SRCCOLORKEY) == DDERR_INVALIDRECT,
		"a copy past the edge is invalid");
	p = lock(dst, &pitch);
	ok = 1;
	for (y = 0; y < 8; y++)
		for (x = 0; x < 8; x++)
			if (p[(4 + y) * pitch + 20 + x] != ((x + y) % 2 ? 1 : 9))
				ok = 0;
	check(ok, "the keyed copy leaves the key's pixels as they were");
	IDirectDrawSurface_Unlock(dst, NULL);
	check(IDirectDrawSurface_BltFast(dst, 20, 4, src, &r, DDBLTFAST_NOCOLORKEY) == DD_OK,
		"a plain copy succeeds");
	p = lock(dst, &pitch);
	check(p[4 * pitch + 21] == 0, "the plain copy copies the key's pixels too");
	IDirectDrawSurface_Unlock(dst, NULL);

	check(IDirectDraw_SetDisplayMode(dd, 640, 480, 8) == DD_OK, "640 by 480 by 8 is a mode");
	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	sd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	sd.dwBackBufferCount = 1;
	sd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	check(IDirectDraw_CreateSurface(dd, &sd, &prim, NULL) == DD_OK, "the primary surface opens");
	memset(&caps, 0, sizeof caps);
	caps.dwCaps = DDSCAPS_BACKBUFFER;
	check(IDirectDrawSurface_GetAttachedSurface(prim, &caps, &back) == DD_OK, "the back buffer attaches");
	p = lock(back, &pitch);
	p[0] = 42;
	IDirectDrawSurface_Unlock(back, NULL);
	p = lock(prim, &pitch);
	p[0] = 17;
	IDirectDrawSurface_Unlock(prim, NULL);
	check(IDirectDrawSurface_Flip(prim, NULL, DDFLIP_WAIT) == DD_OK, "Flip succeeds");
	p = lock(prim, &pitch);
	check(p[0] == 42, "Flip shows the back buffer's pixels");
	IDirectDrawSurface_Unlock(prim, NULL);
	p = lock(back, &pitch);
	check(p[0] == 17, "Flip moves the front's pixels to the back");
	/* the outline of a glyph on row 0 or 479, 3 rows out, as glyph3 draws it */
	for (y = 1; y <= 3; y++) {
		p[-y * pitch] = 0xf4;
		p[(479 + y) * pitch + 639] = 0xf4;
	}
	IDirectDrawSurface_Unlock(back, NULL);
	check(IDirectDrawSurface_Flip(prim, NULL, DDFLIP_WAIT) == DD_OK, "Flip succeeds after the outline");
	p = lock(prim, &pitch);
	check(p[0] == 17 && p[479 * pitch + 639] != 0xf4, "the outline past the rows stays out of the frame");
	IDirectDrawSurface_Unlock(prim, NULL);
	/* plain Wine gives SDL no display, and the Linux targets run the same code */
#ifndef _WIN32
	checkdevice();
#endif
	return failed;
}

/*
 * The wide screen of src/sys/draw.c: the tables of screenwidth, of
 * windowwidth, and of stripx, and on screens wider than the game's frame
 * Lock's origin and pitch, the numbers of GetSurfaceDesc, the rectangles at
 * the edges of sysscreen, the spans of fills and copies, a keyed copy, Flip,
 * which clears the new back buffer's wings, drawkeepwings, and what sysdrawx
 * shifts, and the window's first size. The test prints each check that
 * fails.
 */

#include <stdio.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <windows.h>
#include <ddraw.h>

#include "sys.h"
#include "test.h"

void screensurface(LPDIRECTDRAWSURFACE s);

enum {
	Wing = (Maxscreenw - Gamew) / 2	/* each wing's width in memory */
};

static const struct {
	int w, h, sharp;
	int want;
} widths[] = {
	{ 1920, 1080, 1, 852 },
	{ 1920, 1080, 0, 960 },
	{ 2560, 1440, 1, 852 },
	{ 2560, 1440, 0, 852 },
	{ 3440, 1440, 1, 1144 },
	{ 3840, 2160, 0, 960 },
	{ 1366, 768, 1, 852 },
	{ 1366, 768, 0, 1364 },
	{ 1280, 720, 1, 852 },
	{ 1280, 544, 1, 1128 },
	{ 1920, 1200, 1, 768 },
	{ 1280, 960, 1, 640 },
	{ 1280, 1024, 1, 640 },
	{ 640, 480, 0, 640 },
	{ 5120, 1440, 1, 1704 },
	{ 7680, 1440, 1, 1704 },
	{ 800, 400, 0, 800 },
	{ 100, 100, 1, 640 },
	{ 0, 0, 1, 640 },
};

static const struct {
	int w, h;
	int want;
} windows[] = {
	/* the display's shape */
	{ 1920, 1080, 852 },
	{ 2560, 1440, 852 },
	{ 1366, 768, 852 },
	{ 1920, 1200, 768 },
	{ 3440, 1440, 1144 },
	{ 5120, 1440, 1704 },
	{ 7680, 1440, 1704 },
	/* a display no wider than 4:3 takes 16:9 */
	{ 1280, 960, 852 },
	{ 1600, 1200, 852 },
	{ 1280, 1024, 852 },
	{ 852, 640, 852 },
	/* as far as it holds it */
	{ 800, 600, 800 },
	{ 802, 600, 800 },
	{ 640, 480, 640 },
	{ 320, 240, 640 },
	{ 0, 0, 640 },
};

static const struct {
	int w, x;
	int want;
} strips[] = {
	/* at the frame's width, the identity */
	{ 640, 0, 0 },
	{ 640, 28, 28 },
	{ 640, 29, 29 },
	{ 640, 599, 599 },
	{ 640, 600, 600 },
	{ 640, 639, 639 },
	/* two columns each side: the strips reach into the frame */
	{ 644, -2, 0 },
	{ 644, 26, 28 },
	{ 644, 27, 29 },
	{ 644, 601, 599 },
	{ 644, 602, 600 },
	{ 644, 641, 639 },
	/* the left strip, the map's clamp, and the right strip */
	{ 852, -106, 0 },
	{ 852, -78, 28 },
	{ 852, -77, 29 },
	{ 852, 0, 29 },
	{ 852, 8, 29 },
	{ 852, 29, 29 },
	{ 852, 30, 30 },
	{ 852, 599, 599 },
	{ 852, 600, 599 },
	{ 852, 631, 599 },
	{ 852, 705, 599 },
	{ 852, 706, 600 },
	{ 852, 745, 639 },
	/* the widest screen, and a stale x past its edges */
	{ 1704, -532, 0 },
	{ 1704, -504, 28 },
	{ 1704, -503, 29 },
	{ 1704, 1131, 599 },
	{ 1704, 1132, 600 },
	{ 1704, 1171, 639 },
	{ 852, -500, 0 },
	{ 852, 1100, 639 },
};

/* scripts, and whether the line # screen makes their points the screen's */
static const struct {
	const char *text;
	int want;
} scripts[] = {
	{ "# screen\n1 close\n", 1 },
	{ "# from session\n# screen\n1 close\n", 1 },
	{ "1 close\n# screen", 1 },
	{ "# from session\r\n# screen\r\n1 close\r\n", 1 },
	{ "", 0 },
	{ "1 close\n", 0 },
	{ "# screen shots\n1 close\n", 0 },
	{ " # screen\n1 close\n", 0 },
	{ "# screenshots\n", 0 },
};

/* screenpoints reports whether replayinit takes the script's points as the screen's. */
static int
screenpoints(const char *text)
{
	static const char path[] = "wide-script.tmp";
	SDL_IOStream *f;
	int r;

	f = SDL_IOFromFile(path, "w");
	if (f == NULL || SDL_WriteIO(f, text, strlen(text)) != strlen(text) || !SDL_CloseIO(f))
		sysfatal("wide: %s: %s", path, SDL_GetError());
	SDL_setenv_unsafe("TEUDOGAR_REPLAY", path, 1);
	replayinit();
	r = !sysframepoints;
	replayquit();
	replaying = 0;
	sysframepoints = 0;
	SDL_unsetenv_unsafe("TEUDOGAR_REPLAY");
	SDL_RemovePath(path);
	return r;
}

#ifndef _WIN32
/* firstsize opens the window at the scale that sharp names and returns its size in *w and *h. */
static void
firstsize(int sharp, int *w, int *h)
{
	syssharp = sharp;
	wininit();
	SDL_GetWindowSize(syswindow, w, h);
	SDL_DestroyRenderer(sysrenderer);
	SDL_DestroyWindow(syswindow);
	sysrenderer = NULL;
	syswindow = NULL;
}

/* checksizes checks the first size of a window on a screen of width sw. */
static void
checksizes(int sw)
{
	int w, h, w2, h2;

	drawsetwidth(sw);
	firstsize(0, &w, &h);
	firstsize(1, &w2, &h2);
	if (w != w2 || h != h2) {
		printf("wide: on a screen of %d, integer opens at %d by %d and sharp at %d by %d\n", sw, w, h, w2, h2);
		failed = 1;
	}
	if (w * Gameh != h * sw) {
		printf("wide: a window of %d by %d lacks the shape of a screen of %d\n", w, h, sw);
		failed = 1;
	}
}
#endif

/* pixel returns the pixel of s at x, y of the game's coordinates. */
static int
pixel(LPDIRECTDRAWSURFACE s, int x, int y)
{
	unsigned char *p;
	LONG pitch;
	int c;

	p = lock(s, &pitch);
	c = p[y * pitch + x];
	IDirectDrawSurface_Unlock(s, NULL);
	return c;
}

/* fill sets the rectangle of s to the color c and returns the result of Blt. */
static HRESULT
fill(LPDIRECTDRAWSURFACE s, int x0, int y0, int x1, int y1, int c)
{
	DDBLTFX fx;
	RECT r;

	memset(&fx, 0, sizeof fx);
	fx.dwSize = sizeof fx;
	fx.dwFillColor = c;
	setrect(&r, x0, y0, x1, y1);
	return IDirectDrawSurface_Blt(s, &r, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx);
}

/* clear sets the whole of s to 0, wings and all. */
static void
clear(LPDIRECTDRAWSURFACE s)
{
	DDBLTFX fx;

	memset(&fx, 0, sizeof fx);
	fx.dwSize = sizeof fx;
	check(IDirectDrawSurface_Blt(s, NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fx) == DD_OK,
		"a fill of the whole surface succeeds");
}

int
main(void)
{
	LPDIRECTDRAW dd;
	LPDIRECTDRAWSURFACE prim, back, surf2, tile, plain;
	DDSURFACEDESC sd;
	DDSCAPS caps;
	DDCOLORKEY key;
	RECT r;
	unsigned char *p;
	LONG pitch;
	size_t i;
#ifndef _WIN32
	int w, h;
#endif

	for (i = 0; i < sizeof widths / sizeof widths[0]; i++)
		if (screenwidth(widths[i].w, widths[i].h, widths[i].sharp) != widths[i].want) {
			printf("wide: screenwidth(%d, %d, %d) = %d, want %d\n", widths[i].w, widths[i].h,
				widths[i].sharp, screenwidth(widths[i].w, widths[i].h, widths[i].sharp), widths[i].want);
			failed = 1;
		}
	for (i = 0; i < sizeof windows / sizeof windows[0]; i++)
		if (windowwidth(windows[i].w, windows[i].h) != windows[i].want) {
			printf("wide: windowwidth(%d, %d) = %d, want %d\n", windows[i].w, windows[i].h,
				windowwidth(windows[i].w, windows[i].h), windows[i].want);
			failed = 1;
		}
	for (i = 0; i < sizeof scripts / sizeof scripts[0]; i++)
		if (screenpoints(scripts[i].text) != scripts[i].want) {
			printf("wide: script %d gives points of the %s\n", (int)i, scripts[i].want ? "frame" : "screen");
			failed = 1;
		}
	for (i = 0; i < sizeof strips / sizeof strips[0]; i++) {
		drawsetwidth(strips[i].w);
		if (stripx(strips[i].x) != strips[i].want) {
			printf("wide: stripx(%d) at width %d = %d, want %d\n", strips[i].x, strips[i].w,
				stripx(strips[i].x), strips[i].want);
			failed = 1;
		}
	}

	/* plain Wine gives SDL no display, and the Linux targets run the same code */
#ifndef _WIN32
	if (!SDL_Init(SDL_INIT_VIDEO))
		sysfatal("wide: SDL: %s", SDL_GetError());
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, SDL_SOFTWARE_RENDERER);
	checksizes(Gamew);
	checksizes(852);
	checksizes(1144);
	syssizew = 1000;
	syssizeh = 500;
	firstsize(1, &w, &h);
	check(w == 1000 && h == 500, "--size gives the sharp window's first size");
	firstsize(0, &w, &h);
	check(w == 1000 && h == 500, "--size gives the integer window's first size");
	syssizew = 0;
	syssizeh = 0;
#endif

	drawsetwidth(852);
	check(sysframex == 106, "the frame stands at column 106 of 852");
	check(sysscreen.left == -106 && sysscreen.right == 746, "the screen spans -106 to 746");
	if (DirectDrawCreate(NULL, &dd, NULL) != DD_OK)
		sysfatal("wide: no DirectDraw");
	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	sd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	sd.dwBackBufferCount = 1;
	sd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	check(IDirectDraw_CreateSurface(dd, &sd, &prim, NULL) == DD_OK, "the primary surface opens");
	memset(&caps, 0, sizeof caps);
	caps.dwCaps = DDSCAPS_BACKBUFFER;
	check(IDirectDrawSurface_GetAttachedSurface(prim, &caps, &back) == DD_OK, "the back buffer attaches");
	surf2 = offscreen(dd, Gamew, Gameh);
	screensurface(surf2);
	tile = offscreen(dd, 48, 48);

	p = lock(back, &pitch);
	check(pitch == Maxscreenw, "Lock gives the allocation's pitch");
	IDirectDrawSurface_Unlock(back, NULL);
	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	check(IDirectDrawSurface_GetSurfaceDesc(back, &sd) == DD_OK, "GetSurfaceDesc answers");
	check(sd.dwWidth == Gamew && sd.dwHeight == Gameh, "GetSurfaceDesc gives the frame's size");
	check(sd.lPitch == Gamew, "GetSurfaceDesc gives the frame's pitch");
	check(sd.lpSurface == p, "GetSurfaceDesc and Lock give one origin");

	check(fill(back, -106, 0, -100, 10, 3) == DD_OK, "a fill from the screen's left edge succeeds");
	check(pixel(back, -106, 0) == 3 && pixel(back, -100, 0) == 0, "the fill lands in the left wing");
	check(fill(back, -107, 0, -100, 10, 3) == DDERR_INVALIDRECT, "a fill past the left edge is invalid");
	check(fill(back, 740, 0, 746, 10, 4) == DD_OK, "a fill to the screen's right edge succeeds");
	check(pixel(back, 745, 0) == 4 && pixel(back, 739, 0) == 0, "the fill lands in the right wing");
	check(fill(back, 740, 0, 747, 10, 4) == DDERR_INVALIDRECT, "a fill past the right edge is invalid");
	setrect(&r, 0, 0, 48, 48);
	check(IDirectDrawSurface_BltFast(back, (DWORD)-106, 0, tile, &r, DDBLTFAST_WAIT) == DD_OK,
		"a copy to the screen's left edge succeeds");
	check(IDirectDrawSurface_BltFast(back, (DWORD)-107, 0, tile, &r, DDBLTFAST_WAIT) == DDERR_INVALIDRECT,
		"a copy past the left edge is invalid");
	check(IDirectDrawSurface_BltFast(back, 698, 0, tile, &r, DDBLTFAST_WAIT) == DD_OK,
		"a copy to the screen's right edge succeeds");
	check(IDirectDrawSurface_BltFast(back, 699, 0, tile, &r, DDBLTFAST_WAIT) == DDERR_INVALIDRECT,
		"a copy past the right edge is invalid");
	check(IDirectDrawSurface_BltFast(tile, (DWORD)-1, 0, tile, &r, DDBLTFAST_WAIT) == DDERR_INVALIDRECT,
		"a plain surface has no wings");
	check(IDirectDrawSurface_BltFast(back, 0x7fffffff, 0, tile, &r, DDBLTFAST_WAIT) == DDERR_INVALIDRECT,
		"a copy at the largest x is invalid");
	check(IDirectDrawSurface_BltFast(back, 0, 0x7fffffff, tile, &r, DDBLTFAST_WAIT) == DDERR_INVALIDRECT,
		"a copy at the largest y is invalid");

	clear(back);
	check(fill(back, 0, 10, Gamew, 20, 5) == DD_OK, "a fill across the frame succeeds");
	check(pixel(back, -Wing, 10) == 5 && pixel(back, Gamew + Wing - 1, 19) == 5,
		"a fill across the frame spans its rows to both edges");
	check(pixel(back, -Wing, 9) == 0 && pixel(back, -Wing, 20) == 0, "the span keeps to its rows");
	check(fill(back, 0, 30, Gamew - 1, 31, 6) == DD_OK, "a fill short of the frame's edge succeeds");
	check(pixel(back, -1, 30) == 0 && pixel(back, Gamew - 1, 30) == 0, "that fill keeps to its rectangle");

	clear(back);
	clear(surf2);
	check(fill(back, 400, 0, 402, Gameh, 7) == DD_OK, "a fill in the frame succeeds");
	check(fill(back, -50, 100, -40, 110, 8) == DD_OK, "a fill in the left wing succeeds");
	check(fill(back, 700, 200, 710, 210, 9) == DD_OK, "a fill in the right wing succeeds");
	setrect(&r, 0, 0, Gamew, Gameh);
	check(IDirectDrawSurface_BltFast(surf2, 0, 0, back, &r, DDBLTFAST_WAIT) == DD_OK, "a whole copy succeeds");
	check(pixel(surf2, 400, 0) == 7, "a whole copy copies the frame");
	check(pixel(surf2, -50, 100) == 8 && pixel(surf2, 700, 200) == 9, "a whole copy carries the wings");

	clear(back);
	setrect(&r, 0, 50, Gamew, 60);
	check(IDirectDrawSurface_BltFast(back, 0, 50, surf2, &r, DDBLTFAST_WAIT) == DD_OK, "a band's copy succeeds");
	check(pixel(back, 400, 50) == 7 && pixel(back, 400, 49) == 0 && pixel(back, 400, 60) == 0,
		"a band's copy copies the frame's rows alone");
	check(pixel(back, -50, 100) == 8 && pixel(back, 700, 200) == 9, "a band's copy carries the whole wings");

	clear(back);
	setrect(&r, 0, 50, Gamew - 1, 60);
	check(IDirectDrawSurface_BltFast(back, 0, 50, surf2, &r, DDBLTFAST_WAIT) == DD_OK, "a narrower copy succeeds");
	check(pixel(back, -50, 100) == 0, "a copy short of the frame's edge leaves the wings");
	plain = offscreen(dd, Gamew, Gameh);
	check(fill(plain, 0, 0, Gamew, Gameh, 0) == DD_OK, "a fill of a plain surface succeeds");
	check(fill(surf2, Gamew, 0, Gamew + 1, 1, 27) == DD_OK, "a fill of a screen's wing succeeds");
	setrect(&r, 0, 0, Gamew, 1);
	check(IDirectDrawSurface_BltFast(plain, 0, 0, surf2, &r, DDBLTFAST_WAIT) == DD_OK, "a copy to a plain surface succeeds");
	check(pixel(plain, 0, 1) == 0, "a copy to a plain surface leaves its next row, where the wings would land");

	key.dwColorSpaceLowValue = key.dwColorSpaceHighValue = 0;
	check(IDirectDrawSurface_SetColorKey(surf2, DDCKEY_SRCBLT, &key) == DD_OK, "the color key takes 0");
	clear(back);
	check(fill(back, -60, 100, -40, 110, 2) == DD_OK, "a fill under the keyed copy succeeds");
	setrect(&r, 0, 0, Gamew, Gameh);
	check(IDirectDrawSurface_BltFast(back, 0, 0, surf2, &r, DDBLTFAST_SRCCOLORKEY) == DD_OK, "a keyed copy succeeds");
	check(pixel(back, -50, 100) == 8, "a keyed copy carries the wings' other pixels");
	check(pixel(back, -60, 100) == 2, "a keyed copy leaves the pixels of the wings' key");

	clear(prim);
	clear(back);
	check(fill(back, 10, 0, 11, 1, 11) == DD_OK, "a fill of the frame succeeds");
	check(fill(back, -60, 0, -50, 1, 12) == DD_OK, "a fill of the left wing succeeds");
	check(fill(back, 700, 0, 710, 1, 13) == DD_OK, "a fill of the right wing succeeds");
	check(fill(prim, 20, 0, 21, 1, 14) == DD_OK, "a fill of the primary's frame succeeds");
	check(fill(prim, -30, 0, -20, 1, 15) == DD_OK, "a fill of the primary's wing succeeds");
	check(IDirectDrawSurface_Flip(prim, NULL, DDFLIP_WAIT) == DD_OK, "Flip succeeds");
	check(pixel(prim, 10, 0) == 11, "Flip shows the frame");
	check(pixel(prim, -60, 0) == 12 && pixel(prim, 700, 0) == 13, "Flip shows the wings");
	check(pixel(back, 20, 0) == 14, "Flip moves the front's frame to the back");
	check(pixel(back, -30, 0) == 0, "Flip clears the new back buffer's wings");

	clear(prim);
	clear(back);
	check(fill(prim, -60, 3, -50, 4, 16) == DD_OK, "a fill of the primary's left wing succeeds");
	check(fill(prim, 700, 3, 710, 4, 17) == DD_OK, "a fill of the primary's right wing succeeds");
	check(fill(prim, 100, 3, 110, 4, 18) == DD_OK, "a fill of the primary's frame succeeds");
	drawkeepwings();
	check(pixel(back, -60, 3) == 16 && pixel(back, 700, 3) == 17, "drawkeepwings copies the wings");
	check(pixel(back, 100, 3) == 0, "drawkeepwings leaves the frame");

	clear(back);
	clear(surf2);
	sysdrawx = -106;
	check(fill(back, 8, 5, 18, 6, 21) == DD_OK, "a fill at a left shift succeeds");
	setrect(&r, 0, 0, 48, 48);
	check(IDirectDrawSurface_BltFast(back, 8, 10, tile, &r, DDBLTFAST_WAIT) == DD_OK,
		"a copy at a left shift succeeds");
	p = lock(back, &pitch);
	p[20 * pitch + 8] = 22;
	IDirectDrawSurface_Unlock(back, NULL);
	check(IDirectDrawSurface_BltFast(back, (DWORD)-1, 0, tile, &r, DDBLTFAST_WAIT) == DDERR_INVALIDRECT,
		"a shift keeps a copy's bound at the screen's edge");
	check(fill(back, -1, 0, 0, 1, 1) == DDERR_INVALIDRECT, "a shift keeps a fill's bound at the screen's edge");
	setrect(&r, 8, 5, 12, 6);
	check(IDirectDrawSurface_BltFast(surf2, 30, 5, back, &r, DDBLTFAST_WAIT) == DD_OK,
		"a copy of a shifted rectangle succeeds");
	check(fill(back, 0, 30, Gamew, 31, 23) == DD_OK, "a fill across the frame under a shift succeeds");
	sysdrawx = 0;
	check(pixel(back, -98, 5) == 21 && pixel(back, 8, 5) == 0, "a fill lands at its shift");
	check(pixel(back, -98, 20) == 22 && pixel(back, 8, 20) == 0, "Lock gives the origin at its shift");
	check(pixel(surf2, -76, 5) == 21, "a copy takes its source and its destination at the shift");
	check(pixel(back, -Wing, 30) == 23 && pixel(back, Gamew + Wing - 1, 30) == 23,
		"a fill across the frame under a shift spans its rows");

	clear(back);
	clear(surf2);
	check(fill(surf2, -50, 40, -40, 41, 24) == DD_OK, "a fill of a wing succeeds");
	check(fill(surf2, 300, 40, 310, 41, 25) == DD_OK, "a fill of the frame succeeds");
	sysdrawx = 106;
	check(fill(back, 100, 7, 110, 8, 26) == DD_OK, "a fill at a right shift succeeds");
	check(fill(back, 640, 0, 641, 1, 1) == DDERR_INVALIDRECT, "a right shift keeps a fill's bound at the screen's edge");
	setrect(&r, 0, 40, Gamew, 41);
	check(IDirectDrawSurface_BltFast(back, 0, 40, surf2, &r, DDBLTFAST_WAIT) == DD_OK,
		"a band's copy under a shift succeeds");
	sysdrawx = 0;
	check(pixel(back, 206, 7) == 26 && pixel(back, 100, 7) == 0, "a right shift moves a fill");
	check(pixel(back, 300, 40) == 25 && pixel(back, -50, 40) == 24, "a band's copy stays put under a shift");

	sysdrawx = -106;
	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	check(IDirectDrawSurface_GetSurfaceDesc(back, &sd) == DD_OK, "GetSurfaceDesc answers under a shift");
	p = sd.lpSurface;
	sysdrawx = 0;
	check(lock(back, &pitch) == p, "GetSurfaceDesc keeps the frame's origin under a shift");
	IDirectDrawSurface_Unlock(back, NULL);
	sysdrawx = -106;
	drawsetwidth(1704);
	check(sysdrawx == -532, "a left shift follows the screen's left edge");
	drawsetwidth(852);
	check(sysdrawx == -106, "and back");
	sysdrawx = 0;

	p = lock(back, &pitch);
	drawsetwidth(1704);
	check(sysframex == 532 && sysscreen.left == -532 && sysscreen.right == 1172, "the width changes the screen");
	check(lock(back, &pitch) == p, "the frame keeps its place at another width");
	IDirectDrawSurface_Unlock(back, NULL);
	check(fill(back, -532, 0, -530, 1, 1) == DD_OK, "the widest screen's left edge holds a fill");
	check(fill(back, 1170, 0, 1172, 1, 1) == DD_OK, "the widest screen's right edge holds a fill");
	drawsetwidth(640);
	check(fill(back, -1, 0, 0, 1, 1) == DDERR_INVALIDRECT, "the frame's width leaves no wing to fill");
	check(fill(back, 640, 0, 641, 1, 1) == DDERR_INVALIDRECT, "nor on the frame's right");
	check(pixel(back, -532, 0) == 1 && pixel(back, 1171, 0) == 1, "the frame's width keeps the wings' pixels");
	return failed;
}

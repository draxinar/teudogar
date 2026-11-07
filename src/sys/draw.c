/*
 * DirectDraw for the port: 8-bit surfaces in memory, a primary with
 * one back buffer, a palette, and the presentation of the primary
 * through SDL3.
 *
 * Flip swaps the pixels of the primary and the back buffer, whose
 * objects keep their roles, as the game's surfs[0] and surfs[1]
 * expect. Blt and BltFast refuse an empty, inverted, or outside
 * rectangle with DDERR_INVALIDRECT, as DirectDraw does: the game clips
 * by hand. A palette's change shows at once, as on the hardware, so the
 * frame goes out whenever the primary or its palette changed, from the
 * message pump, at most once per tick of 60 Hz, with the pointer that
 * the game draws on the primary after each flip.
 *
 * The game draws a frame of 640 by 480. A screen surface (the primary,
 * its back buffer, and those that the game hands to screensurface) holds
 * the frame and the wings left and right of it, at negative x and at x
 * from 640 on, in either view. The frame keeps one column of every row, so
 * a new width changes only what shows. A rectangle of a screen
 * lies within sysscreen. A fill from x 0 to 640 spans the row to both
 * edges, and a copy between screens from x 0 to 640, to x 0, also copies
 * the source's whole wings, since a copy of a row band copies the picture
 * that the wings belong to. Flip clears the new back buffer's wings, so
 * the wings show what the frame's own drawing or copies put there.
 *
 * The game's text draws an outline 3 rows above and below its glyphs,
 * unclipped, which DirectDraw's memory around a surface took. Each
 * surface keeps Margin rows of its own above and below its pixels for
 * them, which no frame shows.
 *
 * sysdrawx shifts what the game draws on a screen, so that a part of the
 * classic screen shows elsewhere with its numbers as they are: Blt,
 * BltFast, and Lock add it to x. A rectangle as wide as the frame stands
 * for its rows and stays, so the copies that carry the wings hold under
 * any shift.
 */

#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <windows.h>
#include <ddraw.h>

#include "sys.h"

enum {
	Ticknumer = 1000,	/* a tick of 60 Hz in milliseconds, as a fraction */
	Tickdenom = 60,
	Wingw = (Maxscreenw - Gamew) / 2,	/* the frame's column in the rows of a wide screen */
	Margin = 24	/* the rows above and below a surface's pixels: a glyph's 20 and its outline's 3 */
};

typedef struct Surface Surface;
struct Surface {
	IDirectDrawSurface iface;
	int w, h;	/* the size that the game sees */
	int pitch;	/* the bytes of a row */
	int frame;	/* the column of the game's x 0 in a row */
	int screen;	/* whether it holds a whole screen */
	unsigned char *mem;	/* the allocation, from Margin rows above the pixels */
	unsigned char *pixels;	/* the first row */
	int keyed;
	BYTE key;
	int primary;
	int sysmem;
	Surface *back;
};

typedef struct Palette Palette;
struct Palette {
	IDirectDrawPalette iface;
	PALETTEENTRY e[256];
};

static Surface *primary;
static Palette *palette;
static SDL_Texture *texture;
static int texturew;	/* the width of texture */
static Uint32 argb[Maxscreenw * Gameh];
static int dirty;
static int flipped;	/* a flip waits to be shown */
static DWORD lastflip;	/* the clock at the last flip */
static Uint64 lastshow;	/* the tick of the last frame shown */
static int vblank;

static const IDirectDrawVtbl ddvtbl;
static const IDirectDrawSurfaceVtbl surfvtbl;
static const IDirectDrawPaletteVtbl palvtbl;
static IDirectDraw dd = { &ddvtbl };

int sysscreenw = Gamew;
int sysframex;
RECT sysscreen = { 0, 0, Gamew, Gameh };
int sysdrawx;
Uint32 sysflips;
Uint32 sysshows;
Uint32 systextures;

/* waitvblank waits for the next vertical blank of a display of sysvblank Hz. */
static void
waitvblank(void)
{
	Uint64 now, n;

	now = sysclock();
	n = now * sysvblank / Ticknumer + 1;
	sysdelay((DWORD)((n * Ticknumer + sysvblank - 1) / sysvblank - now));
}

/* show converts the primary through the palette and presents it. */
static void
show(void)
{
	const PALETTEENTRY *e;
	const unsigned char *src;
	Uint32 map[256], *dst;
	int i, y;

	if (sysrenderer == NULL || primary == NULL)
		return;
	if (texture != NULL && texturew != sysscreenw)
		drawreset();
	if (texture == NULL) {
		texture = SDL_CreateTexture(sysrenderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, sysscreenw, Gameh);
		if (texture == NULL)
			sysfatal("texture: %s", SDL_GetError());
		texturew = sysscreenw;
		systextures++;
		drawsetscale();
	}
	for (i = 0; i < 256; i++) {
		e = palette != NULL ? &palette->e[i] : NULL;
		map[i] = e == NULL ? 0xff000000 :
			0xff000000 | (Uint32)e->peRed << 16 | (Uint32)e->peGreen << 8 | e->peBlue;
	}
	dst = argb;
	for (y = 0; y < Gameh; y++) {
		src = primary->pixels + (size_t)y * primary->pitch + primary->frame - sysframex;
		for (i = 0; i < sysscreenw; i++)
			*dst++ = map[src[i]];
	}
	SDL_UpdateTexture(texture, NULL, argb, sysscreenw * sizeof argb[0]);
	SDL_SetRenderDrawColor(sysrenderer, 0, 0, 0, 255);
	SDL_RenderClear(sysrenderer);
	SDL_RenderTexture(sysrenderer, texture, NULL, NULL);
	SDL_RenderPresent(sysrenderer);
	dirty = 0;
	flipped = 0;
	sysshows++;
	if (sysvblank > 0)
		waitvblank();
}

/* tick returns the count of ticks of 60 Hz on the game's clock. */
static Uint64
tick(void)
{
	return (Uint64)sysclock() * Tickdenom / Ticknumer;
}

/*
 * holdms returns how long after a flip another change waits to be shown: a
 * frame and a half of the display, or of the game's 60 Hz on a faster one, and
 * 0 where a present waits for no refresh. A present can wait a whole refresh,
 * and while the game flips, its next flip falls due by then and shows the
 * change anyway.
 */
static DWORD
holdms(void)
{
	const SDL_DisplayMode *m;
	float hz;

	hz = sysvblank;
	if (sysvblank < 0 && !replaying) {
		m = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(syswindow));
		hz = m != NULL && m->refresh_rate > 0 ? m->refresh_rate : 60;
	}
	if (hz <= 0)
		return 0;
	return (DWORD)(1500 / SDL_min(hz, 60));
}

/* drawpresent shows the frame if it changed and a tick passed, or with force. */
void
drawpresent(int force)
{
	Uint64 t;

	if (!dirty && !force)
		return;
	t = tick();
	if (!force && t == lastshow)
		return;
	if (!force && !flipped && sysclock() - lastflip < holdms())
		return;
	lastshow = t;
	show();
}

/*
 * screenwidth returns the width of the screen for an output of w by h
 * pixels: the width that 480 rows leave at the output's shape when sharp,
 * and else what the whole multiple that fits the rows leaves, as a
 * multiple of 4, at least the frame's and at most Maxscreenw.
 */
int
screenwidth(int w, int h, int sharp)
{
	int n;

	if (w <= 0 || h <= 0)
		return Gamew;
	if (sharp)
		n = (int)((Sint64)Gameh * w / h);
	else
		n = w / SDL_max(1, h / Gameh);
	n -= n % 4;
	return SDL_clamp(n, Gamew, Maxscreenw);
}

/*
 * windowwidth returns the first width of a wide window on a display of w
 * by h pixels: the width of the display's shape, or on a display no wider
 * than 4:3, whose shape the frame already fills, 16:9 as far as the display
 * holds it.
 */
int
windowwidth(int w, int h)
{
	int n;

	n = screenwidth(w, h, 1);
	if (n > Gamew)
		return n;
	return SDL_clamp(w - w % 4, Gamew, Widewindow);
}

/* drawsetwidth sets the width of the screen, a multiple of 4. */
void
drawsetwidth(int w)
{
	sysscreenw = w;
	sysframex = (w - Gamew) / 2;
	sysscreen.left = -sysframex;
	sysscreen.right = Gamew + sysframex;
	if (sysdrawx < 0)
		sysdrawx = sysscreen.left;
}

/*
 * stripx returns the classic screen's x for a pointer at x of the wide
 * screen, on the map: the first columns of the screen stand for the
 * frame's first, and its last for the frame's last, where the gestures
 * and the hot corner lie. Between them x stops short of those bands, in
 * the same column of cells.
 */
int
stripx(int x)
{
	x = SDL_clamp(x, sysscreen.left, sysscreen.right - 1);
	if (x - sysscreen.left < Leftstrip)
		return x - sysscreen.left;
	if (sysscreen.right - x <= Rightstrip)
		return x - sysscreen.right + Gamew;
	return SDL_clamp(x, Leftstrip, Gamew - Rightstrip - 1);
}

/*
 * sharpscale is the scale mode of the sharp presentation. SDL 3.4 brought
 * SDL_SCALEMODE_PIXELART, and SDL 3.2 scales by the nearest pixel.
 */
#if SDL_VERSION_ATLEAST(3, 4, 0)
static const SDL_ScaleMode sharpscale = SDL_SCALEMODE_PIXELART;
#else
static const SDL_ScaleMode sharpscale = SDL_SCALEMODE_NEAREST;
#endif

/* drawsetscale gives the texture the scaling of the presentation. */
void
drawsetscale(void)
{
	if (texture != NULL)
		SDL_SetTextureScaleMode(texture, syssharp ? sharpscale : SDL_SCALEMODE_NEAREST);
}

/*
 * drawreset drops the texture, which the next frame makes again: after a
 * new width, and after a reset or a loss of the renderer's device, whose
 * textures SDL then holds empty.
 */
void
drawreset(void)
{
	if (texture != NULL)
		SDL_DestroyTexture(texture);
	texture = NULL;
	dirty = 1;
}

void
drawexposed(void)
{
	dirty = 1;
	drawpresent(1);
}

static Surface *
surfof(IDirectDrawSurface *p)
{
	return (Surface *)p;
}

static Surface *
newsurface(int w, int h, int sysmem)
{
	Surface *s;

	s = calloc(1, sizeof *s);
	if (s == NULL)
		return NULL;
	s->mem = calloc((size_t)w * (h + 2 * Margin), 1);
	if (s->mem == NULL) {
		free(s);
		return NULL;
	}
	s->pixels = s->mem + (size_t)Margin * w;
	s->iface.lpVtbl = &surfvtbl;
	s->w = w;
	s->h = h;
	s->pitch = w;
	s->sysmem = sysmem;
	return s;
}

/* makescreen makes s, of the frame's size, a screen, reporting whether its memory sufficed. */
static int
makescreen(Surface *s)
{
	unsigned char *p;

	s->screen = 1;
	p = calloc((size_t)Maxscreenw * (s->h + 2 * Margin), 1);
	if (p == NULL)
		return 0;
	free(s->mem);
	s->mem = p;
	s->pixels = p + (size_t)Margin * Maxscreenw;
	s->pitch = Maxscreenw;
	s->frame = Wingw;
	return 1;
}

/* screensurface makes s, a surface of the frame's size, a screen. */
void
screensurface(LPDIRECTDRAWSURFACE s)
{
	if (!makescreen((Surface *)s))
		sysfatal("the screen's surface: out of memory");
}

/* at returns the pixel of s at x, y of the game's coordinates. */
static unsigned char *
at(Surface *s, int x, int y)
{
	return s->pixels + (size_t)y * s->pitch + s->frame + x;
}

/* clearwings clears the wings of s, whatever the screen's width. */
static void
clearwings(Surface *s)
{
	int y;

	for (y = 0; y < s->h; y++) {
		memset(at(s, -s->frame, y), 0, s->frame);
		memset(at(s, s->w, y), 0, s->pitch - s->frame - s->w);
	}
}

/* drawclearback clears the wings of the primary's back buffer. */
void
drawclearback(void)
{
	clearwings(primary->back);
}

/* drawkeepwings copies the primary's wings to its back buffer, for a flip that keeps them. */
void
drawkeepwings(void)
{
	Surface *b;
	int y;

	b = primary->back;
	for (y = 0; y < b->h; y++) {
		memcpy(at(b, -b->frame, y), at(primary, -primary->frame, y), b->frame);
		memcpy(at(b, b->w, y), at(primary, primary->w, y), b->pitch - b->frame - b->w);
	}
}

static void
touched(Surface *s)
{
	if (s == primary)
		dirty = 1;
}

/* minx and maxx return the bounds of x on s, the last excluded. */
static int
minx(const Surface *s)
{
	return s->screen ? sysscreen.left : 0;
}

static int
maxx(const Surface *s)
{
	return s->screen ? sysscreen.right : s->w;
}

/* rectof sets r to the rectangle p as sysdrawx places it, or the whole of s, and reports whether it lies inside s and holds a pixel. */
static int
rectof(Surface *s, const RECT *p, RECT *r)
{
	if (p == NULL) {
		r->left = -s->frame;
		r->top = 0;
		r->right = s->pitch - s->frame;
		r->bottom = s->h;
		return 1;
	}
	*r = *p;
	if (s->screen && r->right - r->left != Gamew) {
		r->left += sysdrawx;
		r->right += sysdrawx;
	}
	return r->left >= minx(s) && r->top >= 0 && r->right <= maxx(s) && r->bottom <= s->h &&
		r->left < r->right && r->top < r->bottom;
}

/* copyrow copies n pixels from sp to dp, leaving those of the source's key. */
static void
copyrow(unsigned char *dp, const unsigned char *sp, int n, int keyed, BYTE key)
{
	int i;

	if (!keyed) {
		memmove(dp, sp, n);
		return;
	}
	for (i = 0; i < n; i++)
		if (sp[i] != key)
			dp[i] = sp[i];
}

/*
 * copyrect copies the rectangle b of s to x, y of d, and the source's
 * whole wings when both are screens and the rectangle crosses the frame
 * to x 0.
 */
static void
copyrect(Surface *d, int x, int y, Surface *s, const RECT *b, int keyed)
{
	int j, w, h;

	w = b->right - b->left;
	h = b->bottom - b->top;
	for (j = 0; j < h; j++)
		copyrow(at(d, x, y + j), at(s, b->left, b->top + j), w, keyed, s->key);
	if (!d->screen || !s->screen || d == s || x != 0 || b->left != 0 || b->right != Gamew)
		return;
	for (j = 0; j < s->h; j++) {
		copyrow(at(d, -d->frame, j), at(s, -s->frame, j), s->frame, keyed, s->key);
		copyrow(at(d, d->w, j), at(s, s->w, j), s->pitch - s->frame - s->w, keyed, s->key);
	}
}

static ULONG
ddrelease(IDirectDraw *p)
{
	(void)p;
	return 0;
}

static HRESULT
ddcreatepalette(IDirectDraw *p, DWORD flags, LPPALETTEENTRY e, LPDIRECTDRAWPALETTE *out, IUnknown *u)
{
	Palette *pal;

	(void)p, (void)flags, (void)u;
	pal = calloc(1, sizeof *pal);
	if (pal == NULL)
		return DDERR_OUTOFMEMORY;
	pal->iface.lpVtbl = &palvtbl;
	if (e != NULL)
		memcpy(pal->e, e, sizeof pal->e);
	*out = &pal->iface;
	return DD_OK;
}

static HRESULT
ddcreatesurface(IDirectDraw *p, LPDDSURFACEDESC sd, LPDIRECTDRAWSURFACE *out, IUnknown *u)
{
	Surface *s;

	(void)p, (void)u;
	if (sd->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE) {
		if (primary != NULL)
			return DDERR_PRIMARYSURFACEALREADYEXISTS;
		s = newsurface(Gamew, Gameh, 0);
		if (s == NULL || !makescreen(s))
			return DDERR_OUTOFMEMORY;
		s->primary = 1;
		if ((sd->dwFlags & DDSD_BACKBUFFERCOUNT) && sd->dwBackBufferCount > 0) {
			s->back = newsurface(Gamew, Gameh, 0);
			if (s->back == NULL || !makescreen(s->back))
				return DDERR_OUTOFMEMORY;
		}
		primary = s;
		*out = &s->iface;
		return DD_OK;
	}
	if (!(sd->dwFlags & DDSD_WIDTH) || !(sd->dwFlags & DDSD_HEIGHT) ||
	    sd->dwWidth == 0 || sd->dwHeight == 0 || sd->dwWidth > 4096 || sd->dwHeight > 4096)
		return DDERR_INVALIDPARAMS;
	s = newsurface((int)sd->dwWidth, (int)sd->dwHeight, (sd->ddsCaps.dwCaps & DDSCAPS_SYSTEMMEMORY) != 0);
	if (s == NULL)
		return DDERR_OUTOFVIDEOMEMORY;
	*out = &s->iface;
	return DD_OK;
}

static HRESULT
ddgetcaps(IDirectDraw *p, LPDDCAPS hw, LPDDCAPS hel)
{
	LPDDCAPS c[2];
	int i;

	(void)p;
	c[0] = hw;
	c[1] = hel;
	for (i = 0; i < 2; i++) {
		if (c[i] == NULL)
			continue;
		c[i]->dwCaps = DDCAPS_BLT | DDCAPS_BLTCOLORFILL | DDCAPS_COLORKEY;
		c[i]->dwCKeyCaps = DDCKEYCAPS_SRCBLT;
		c[i]->dwVidMemTotal = 64 << 20;
		c[i]->dwVidMemFree = 48 << 20;
	}
	return DD_OK;
}

/* ddvblank toggles, so that waitvbl's two loops each end. */
static HRESULT
ddvblank(IDirectDraw *p, LPBOOL b)
{
	(void)p;
	vblank = !vblank;
	*b = vblank;
	return DD_OK;
}

static HRESULT
ddcooperate(IDirectDraw *p, HWND w, DWORD flags)
{
	(void)p, (void)w, (void)flags;
	return DD_OK;
}

static HRESULT
ddsetmode(IDirectDraw *p, DWORD w, DWORD h, DWORD bpp)
{
	(void)p;
	if (w != Gamew || h != Gameh || bpp != 8)
		return DDERR_INVALIDMODE;
	return DD_OK;
}

/* ddwaitvb waits for the next tick of 60 Hz and shows the frame there. */
static HRESULT
ddwaitvb(IDirectDraw *p, DWORD flags, HANDLE ev)
{
	Uint64 now, next;

	(void)p, (void)flags, (void)ev;
	now = sysclock();
	next = (now * Tickdenom / Ticknumer + 1) * Ticknumer / Tickdenom;
	if (next > now)
		sysdelay((DWORD)(next - now));
	drawpresent(0);
	return DD_OK;
}

static const IDirectDrawVtbl ddvtbl = {
	ddrelease,
	ddcreatepalette,
	ddcreatesurface,
	ddgetcaps,
	ddvblank,
	ddcooperate,
	ddsetmode,
	ddwaitvb,
};

HRESULT
DirectDrawCreate(GUID *g, LPDIRECTDRAW *out, IUnknown *u)
{
	(void)g, (void)u;
	*out = &dd;
	return DD_OK;
}

static ULONG
surfrelease(IDirectDrawSurface *p)
{
	Surface *s;

	s = surfof(p);
	if (s == primary)
		primary = NULL;
	if (s->back != NULL) {
		free(s->back->mem);
		free(s->back);
	}
	free(s->mem);
	free(s);
	return 0;
}

static HRESULT
surfblt(IDirectDrawSurface *p, LPRECT dr, LPDIRECTDRAWSURFACE src, LPRECT sr, DWORD flags, LPDDBLTFX fx)
{
	Surface *d, *s;
	RECT a, b;
	int y, w, h;

	d = surfof(p);
	if (!rectof(d, dr, &a))
		return DDERR_INVALIDRECT;
	if (flags & DDBLT_COLORFILL) {
		if (fx == NULL)
			return DDERR_INVALIDPARAMS;
		if (d->screen && a.left == 0 && a.right == Gamew) {
			a.left = -d->frame;
			a.right = d->pitch - d->frame;
		}
		for (y = a.top; y < a.bottom; y++)
			memset(at(d, a.left, y), (int)(fx->dwFillColor & 0xff), a.right - a.left);
		touched(d);
		return DD_OK;
	}
	if (src == NULL)
		return DDERR_INVALIDPARAMS;
	s = surfof(src);
	if (!rectof(s, sr, &b))
		return DDERR_INVALIDRECT;
	w = a.right - a.left;
	h = a.bottom - a.top;
	if (w != b.right - b.left || h != b.bottom - b.top)
		return DDERR_NOSTRETCHHW;
	copyrect(d, a.left, a.top, s, &b, 0);
	touched(d);
	return DD_OK;
}

static HRESULT
surfbltfast(IDirectDrawSurface *p, DWORD x, DWORD y, LPDIRECTDRAWSURFACE src, LPRECT sr, DWORD flags)
{
	Surface *d, *s;
	RECT b;
	int dx, dy;

	d = surfof(p);
	s = surfof(src);
	if (!rectof(s, sr, &b))
		return DDERR_INVALIDRECT;
	dx = (int)x;
	dy = (int)y;
	if (dx < -Maxscreenw || dx > Maxscreenw)
		return DDERR_INVALIDRECT;
	if (d->screen && b.right - b.left != Gamew)
		dx += sysdrawx;
	if (dx < minx(d) || dy < 0 || dx > maxx(d) - (b.right - b.left) || dy > d->h - (b.bottom - b.top))
		return DDERR_INVALIDRECT;
	copyrect(d, dx, dy, s, &b, (flags & DDBLTFAST_SRCCOLORKEY) && s->keyed);
	touched(d);
	return DD_OK;
}

static HRESULT
surfflip(IDirectDrawSurface *p, LPDIRECTDRAWSURFACE over, DWORD flags)
{
	Surface *s;
	unsigned char *t;

	(void)over, (void)flags;
	s = surfof(p);
	if (!s->primary || s->back == NULL)
		return DDERR_NOTFLIPPABLE;
	t = s->mem;
	s->mem = s->back->mem;
	s->back->mem = t;
	t = s->pixels;
	s->pixels = s->back->pixels;
	s->back->pixels = t;
	clearwings(s->back);
	touched(s);
	flipped = 1;
	lastflip = sysclock();
	sysflips++;
	return DD_OK;
}

static HRESULT
surfattached(IDirectDrawSurface *p, LPDDSCAPS caps, LPDIRECTDRAWSURFACE *out)
{
	Surface *s;

	s = surfof(p);
	if (!(caps->dwCaps & DDSCAPS_BACKBUFFER) || s->back == NULL)
		return DDERR_NOTFOUND;
	*out = &s->back->iface;
	return DD_OK;
}

/* GDI on a surface serves only the game's dead textf */
static HRESULT
surfgetdc(IDirectDrawSurface *p, HDC *dc)
{
	(void)p;
	*dc = NULL;
	return DDERR_CANTCREATEDC;
}

static HRESULT
surfreleasedc(IDirectDrawSurface *p, HDC dc)
{
	(void)p, (void)dc;
	return DD_OK;
}

/*
 * describe gives the frame's numbers, which GetSurfaceDesc answers:
 * surfmem sums height by pitch into techinfo.txt, whatever the width.
 */
static void
describe(Surface *s, LPDDSURFACEDESC sd)
{
	sd->dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PITCH | DDSD_PIXELFORMAT;
	sd->dwWidth = (DWORD)s->w;
	sd->dwHeight = (DWORD)s->h;
	sd->lPitch = s->w;
	sd->lpSurface = s->pixels + s->frame;
	memset(&sd->ddpfPixelFormat, 0, sizeof sd->ddpfPixelFormat);
	sd->ddpfPixelFormat.dwSize = sizeof sd->ddpfPixelFormat;
	sd->ddpfPixelFormat.dwRGBBitCount = 8;
	sd->ddsCaps.dwCaps = s->sysmem ? DDSCAPS_SYSTEMMEMORY : DDSCAPS_VIDEOMEMORY;
	if (s->primary)
		sd->ddsCaps.dwCaps |= DDSCAPS_PRIMARYSURFACE;
}

static HRESULT
surfdesc(IDirectDrawSurface *p, LPDDSURFACEDESC sd)
{
	describe(surfof(p), sd);
	return DD_OK;
}

static HRESULT
surflock(IDirectDrawSurface *p, LPRECT r, LPDDSURFACEDESC sd, DWORD flags, HANDLE ev)
{
	(void)r, (void)flags, (void)ev;
	describe(surfof(p), sd);
	sd->lPitch = surfof(p)->pitch;
	if (surfof(p)->screen)
		sd->lpSurface = (unsigned char *)sd->lpSurface + sysdrawx;
	return DD_OK;
}

static HRESULT
surfrestore(IDirectDrawSurface *p)
{
	(void)p;
	return DD_OK;
}

static HRESULT
surfsetkey(IDirectDrawSurface *p, DWORD flags, LPDDCOLORKEY key)
{
	Surface *s;

	s = surfof(p);
	if (!(flags & DDCKEY_SRCBLT))
		return DDERR_INVALIDPARAMS;
	s->keyed = key != NULL;
	if (key != NULL)
		s->key = (BYTE)key->dwColorSpaceLowValue;
	return DD_OK;
}

static HRESULT
surfsetpalette(IDirectDrawSurface *p, LPDIRECTDRAWPALETTE pal)
{
	Surface *s;

	s = surfof(p);
	if (s->primary) {
		palette = (Palette *)pal;
		dirty = 1;
	}
	return DD_OK;
}

static HRESULT
surfunlock(IDirectDrawSurface *p, LPVOID bits)
{
	(void)bits;
	touched(surfof(p));
	return DD_OK;
}

static const IDirectDrawSurfaceVtbl surfvtbl = {
	surfrelease,
	surfblt,
	surfbltfast,
	surfflip,
	surfattached,
	surfgetdc,
	surfdesc,
	surflock,
	surfreleasedc,
	surfrestore,
	surfsetkey,
	surfsetpalette,
	surfunlock,
};

static ULONG
palrelease(IDirectDrawPalette *p)
{
	if ((Palette *)p == palette)
		palette = NULL;
	free(p);
	return 0;
}

static HRESULT
palgetentries(IDirectDrawPalette *p, DWORD flags, DWORD start, DWORD n, LPPALETTEENTRY e)
{
	(void)flags;
	if (start > 256 || n > 256 - start)
		return DDERR_INVALIDPARAMS;
	memcpy(e, &((Palette *)p)->e[start], n * sizeof *e);
	return DD_OK;
}

static HRESULT
palsetentries(IDirectDrawPalette *p, DWORD flags, DWORD start, DWORD n, LPPALETTEENTRY e)
{
	(void)flags;
	if (start > 256 || n > 256 - start)
		return DDERR_INVALIDPARAMS;
	memcpy(&((Palette *)p)->e[start], e, n * sizeof *e);
	if ((Palette *)p == palette)
		dirty = 1;
	return DD_OK;
}

static const IDirectDrawPaletteVtbl palvtbl = {
	palrelease,
	palgetentries,
	palsetentries,
};

/* GDI32: the game's printing and its text on surfaces are dead code */
HFONT
CreateFontA(int h, int w, int esc, int orient, int weight, DWORD italic, DWORD under,
	DWORD strike, DWORD cs, DWORD out, DWORD clip, DWORD q, DWORD pitch, LPCSTR face)
{
	static int font;

	(void)h, (void)w, (void)esc, (void)orient, (void)weight, (void)italic, (void)under;
	(void)strike, (void)cs, (void)out, (void)clip, (void)q, (void)pitch, (void)face;
	return &font;
}

BOOL
DeleteObject(HGDIOBJ o)
{
	(void)o;
	return TRUE;
}

HGDIOBJ
SelectObject(HDC dc, HGDIOBJ o)
{
	(void)dc;
	return o;
}

int
SetBkMode(HDC dc, int m)
{
	(void)dc, (void)m;
	return OPAQUE;
}

COLORREF
SetTextColor(HDC dc, COLORREF c)
{
	(void)dc, (void)c;
	return 0;
}

BOOL
TextOutA(HDC dc, int x, int y, LPCSTR s, int n)
{
	(void)dc, (void)x, (void)y, (void)s, (void)n;
	return FALSE;
}

BOOL
GetTextExtentPoint32A(HDC dc, LPCSTR s, int n, LPSIZE sz)
{
	(void)dc, (void)s;
	sz->cx = 8 * n;
	sz->cy = 16;
	return TRUE;
}

HDC
CreateDCA(LPCSTR driver, LPCSTR device, LPCSTR out, const void *mode)
{
	(void)driver, (void)device, (void)out, (void)mode;
	return NULL;
}

BOOL
DeleteDC(HDC dc)
{
	(void)dc;
	return TRUE;
}

int
GetDeviceCaps(HDC dc, int i)
{
	(void)dc, (void)i;
	return 0;
}

int
SetMapMode(HDC dc, int m)
{
	(void)dc, (void)m;
	return MM_TEXT;
}

int
StartDocA(HDC dc, const DOCINFOA *di)
{
	(void)dc, (void)di;
	return 0;
}

int
StartPage(HDC dc)
{
	(void)dc;
	return 0;
}

int
EndPage(HDC dc)
{
	(void)dc;
	return 0;
}

int
EndDoc(HDC dc)
{
	(void)dc;
	return 0;
}

BOOL
EnumPrintersA(DWORD flags, LPSTR name, DWORD level, LPBYTE buf, DWORD n, LPDWORD needed, LPDWORD count)
{
	(void)flags, (void)name, (void)level, (void)buf, (void)n;
	*needed = 0;
	*count = 0;
	return TRUE;
}

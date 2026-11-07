/*
 * ddraw.dll for the game on a Windows without DirectDraw's 8-bit modes,
 * Windows 8 and later. It stands beside teudogar.exe, the release's or a
 * rebuilt one, and needs nothing else.
 *
 * It holds the interfaces in plain memory, with the methods that the game
 * calls, and leaves the other entries of the vtables empty. A thread
 * shows the primary surface through GDI in the game's window, at 60
 * frames a second. The window covers the screen, so the frame stands in
 * the middle of it at 4:3, and the game's calls for the pointer go
 * through hooks that turn a point of the screen into one of the frame.
 *
 * Flip is what holds the release to its pace. As in DirectDraw, it
 * returns at once and the next Flip waits for the frame that shows this
 * one. Unlike DirectDraw, a blit never waits, so the pointer that the
 * game draws after Flip goes out in that same frame.
 *
 * The game's text draws an outline 3 rows above and below its glyphs,
 * unclipped, which DirectDraw's memory around a surface took. Each
 * surface keeps Margin rows of its own above and below its pixels for
 * them, which no frame shows.
 */

#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <ddraw.h>
#include <dwmapi.h>
#include <mmsystem.h>

enum {
	Gamew = 640,
	Gameh = 480,
	Maxsize = 4096,	/* the widest and the highest of other surfaces */
	Margin = 24,	/* the rows above and below a surface's pixels: a glyph's 20 and its outline's 3 */
	Stillms = 200,	/* the longest time between two frames, which repaints a window that was covered */
};

static const double Framems = 1000.0 / 60;
static const double Blankms = 1.5;	/* the part of a frame that GetVerticalBlankStatus calls a blank */

typedef struct Surface Surface;
struct Surface {
	IDirectDrawSurface iface;
	unsigned char *mem;	/* the allocation, from Margin rows above the pixels */
	unsigned char *pixels;	/* the first row */
	Surface *back;
	int w, h;
	int primary;
	int sysmem;
	int keyed;
	BYTE key;
};

typedef struct Palette Palette;
struct Palette {
	IDirectDrawPalette iface;
	PALETTEENTRY e[256];
};

static IDirectDrawVtbl ddvtbl;
static IDirectDrawSurfaceVtbl surfvtbl;
static IDirectDrawPaletteVtbl palvtbl;
static IDirectDraw dd = { &ddvtbl };

static ULONG WINAPI surfrelease(IDirectDrawSurface *p);

static CRITICAL_SECTION lock;	/* guards the pointers primary and palette and Flip's swap of the primary's pixels while a frame is made: the game writes the pixels and the colors outside it, and a frame that a write crosses shows the rest of it in the next */
static Surface *primary;
static Palette *palette;
static volatile LONG changed;	/* the primary changed since the last frame */
static volatile LONG flipping;	/* a flip waits for the next frame */
static volatile LONG frames;	/* the frames so far */
static volatile double framestart;	/* the time of the last frame */
static HANDLE framed;	/* set at each frame */

static HWND window;
static CRITICAL_SECTION arealock;	/* guards origin and area, which the display thread sets for the hooks */
static POINT origin;	/* the window's client area on the screen */
static RECT area = { 0, 0, Gamew, Gameh };	/* the frame in the client area */
static DWORD argb[Gamew * Gameh];	/* the frame that goes out */
static double perms;	/* the performance counter's counts per millisecond */

/* nowms returns the milliseconds of the performance counter. */
static double
nowms(void)
{
	LARGE_INTEGER n;

	QueryPerformanceCounter(&n);
	return (double)n.QuadPart / perms;
}

/*
 * placeframe sets area to the largest rectangle of 4:3 in the middle of
 * the window. The hooks read area and origin in the game's thread, so
 * both change under arealock, whole.
 */
static void
placeframe(void)
{
	RECT r, a;
	POINT o;
	int w, h;

	if (!GetClientRect(window, &r) || r.right < Gamew || r.bottom < Gameh)
		return;
	o.x = 0;
	o.y = 0;
	ClientToScreen(window, &o);
	w = r.right;
	h = r.bottom;
	if (w * Gameh > h * Gamew)
		w = h * Gamew / Gameh;
	else
		h = w * Gameh / Gamew;
	a.left = (r.right - w) / 2;
	a.top = (r.bottom - h) / 2;
	a.right = a.left + w;
	a.bottom = a.top + h;
	EnterCriticalSection(&arealock);
	origin = o;
	area = a;
	LeaveCriticalSection(&arealock);
}

/* framearea gives the frame on the screen: its top left corner and its size. */
static void
framearea(POINT *o, SIZE *n)
{
	EnterCriticalSection(&arealock);
	o->x = origin.x + area.left;
	o->y = origin.y + area.top;
	n->cx = area.right - area.left;
	n->cy = area.bottom - area.top;
	LeaveCriticalSection(&arealock);
}

/* toframe returns the frame's coordinate of v, on a side of n pixels of the screen that starts at o. */
static int
toframe(int v, int o, int n, int frame)
{
	v = v < o ? 0 : (v - o) * frame / n;
	return v < frame ? v : frame - 1;
}

/* toscreen returns the first pixel of the screen that toframe turns into v. */
static int
toscreen(int v, int o, int n, int frame)
{
	return o + (v * n + frame - 1) / frame;
}

static BOOL WINAPI
hookgetcursorpos(LPPOINT p)
{
	POINT o;
	SIZE n;

	if (!GetCursorPos(p))
		return FALSE;
	framearea(&o, &n);
	p->x = toframe(p->x, o.x, n.cx, Gamew);
	p->y = toframe(p->y, o.y, n.cy, Gameh);
	return TRUE;
}

static BOOL WINAPI
hooksetcursorpos(int x, int y)
{
	POINT o;
	SIZE n;

	framearea(&o, &n);
	return SetCursorPos(toscreen(x, o.x, n.cx, Gamew), toscreen(y, o.y, n.cy, Gameh));
}

static BOOL WINAPI
hookclipcursor(const RECT *r)
{
	RECT c;
	POINT o;
	SIZE n;

	if (r == NULL)
		return ClipCursor(NULL);
	framearea(&o, &n);
	c.left = toscreen(r->left, o.x, n.cx, Gamew);
	c.right = toscreen(r->right, o.x, n.cx, Gamew);
	c.top = toscreen(r->top, o.y, n.cy, Gameh);
	c.bottom = toscreen(r->bottom, o.y, n.cy, Gameh);
	return ClipCursor(&c);
}

/* hookimports points the game's imports of the calls for the pointer at the hooks. */
static void
hookimports(void)
{
	static const struct {
		const char *name;
		void *hook;
	} hooks[] = {
		{ "GetCursorPos", hookgetcursorpos },
		{ "SetCursorPos", hooksetcursorpos },
		{ "ClipCursor", hookclipcursor },
	};
	BYTE *base;
	IMAGE_NT_HEADERS *nt;
	IMAGE_IMPORT_DESCRIPTOR *d;
	IMAGE_THUNK_DATA *name, *slot;
	IMAGE_IMPORT_BY_NAME *by;
	DWORD old;
	size_t i;

	base = (BYTE *)GetModuleHandleA(NULL);
	nt = (IMAGE_NT_HEADERS *)(base + ((IMAGE_DOS_HEADER *)base)->e_lfanew);
	d = (IMAGE_IMPORT_DESCRIPTOR *)(base + nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	for (; d->Name != 0; d++) {
		if (lstrcmpiA((char *)(base + d->Name), "user32.dll") != 0)
			continue;
		name = (IMAGE_THUNK_DATA *)(base + d->OriginalFirstThunk);
		slot = (IMAGE_THUNK_DATA *)(base + d->FirstThunk);
		for (; name->u1.AddressOfData != 0; name++, slot++) {
			if (IMAGE_SNAP_BY_ORDINAL(name->u1.Ordinal))
				continue;
			by = (IMAGE_IMPORT_BY_NAME *)(base + name->u1.AddressOfData);
			for (i = 0; i < sizeof hooks / sizeof hooks[0]; i++) {
				if (strcmp((char *)by->Name, hooks[i].name) != 0)
					continue;
				VirtualProtect(slot, sizeof *slot, PAGE_READWRITE, &old);
				slot->u1.Function = (ULONG_PTR)hooks[i].hook;
				VirtualProtect(slot, sizeof *slot, old, &old);
			}
		}
	}
}

/*
 * nextframe waits for the next frame, from the time *last of the one before.
 * A display near 60 Hz sets the time through DWM, so that frames and refreshes
 * stay in step. DwmFlush returns at once while DWM has nothing to do, and the
 * clock takes over, as it does on any other display.
 */
static void
nextframe(double *last, int bydwm)
{
	double now;

	if (bydwm) {
		DwmFlush();
		now = nowms();
		if (now - *last >= Framems / 2) {
			*last = now;
			return;
		}
	}
	now = nowms();
	*last += Framems;
	if (*last < now - Framems)
		*last = now;
	while (nowms() < *last)
		Sleep(1);
}

/* makeframe writes the primary into argb, through its palette. */
static void
makeframe(void)
{
	DWORD map[256];
	const unsigned char *src;
	int i;

	for (i = 0; i < 256; i++)
		map[i] = (DWORD)palette->e[i].peRed << 16 | (DWORD)palette->e[i].peGreen << 8 | palette->e[i].peBlue;
	src = primary->pixels;
	for (i = 0; i < Gamew * Gameh; i++)
		argb[i] = map[src[i]];
}

/* showframe draws argb in the window. */
static void
showframe(void)
{
	static BITMAPINFO bmi;
	HDC dc;

	if (IsIconic(window))
		return;
	placeframe();
	dc = GetDC(window);
	if (dc == NULL)
		return;
	bmi.bmiHeader.biSize = sizeof bmi.bmiHeader;
	bmi.bmiHeader.biWidth = Gamew;
	bmi.bmiHeader.biHeight = -Gameh;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	SetStretchBltMode(dc, COLORONCOLOR);
	StretchDIBits(dc, area.left, area.top, area.right - area.left, area.bottom - area.top,
		0, 0, Gamew, Gameh, argb, &bmi, DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(window, dc);
}

/* display makes a frame 60 times a second, and draws those with something new. */
static DWORD WINAPI
display(void *arg)
{
	DEVMODEA m;
	double last, shown;
	int bydwm, show;

	(void)arg;
	memset(&m, 0, sizeof m);
	m.dmSize = sizeof m;
	bydwm = EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &m) && m.dmDisplayFrequency >= 59 && m.dmDisplayFrequency <= 61;
	last = nowms();
	shown = 0;
	for (;;) {
		nextframe(&last, bydwm);
		EnterCriticalSection(&lock);
		/* a change during makeframe sets changed again, for the next frame */
		show = primary != NULL && palette != NULL && (InterlockedExchange(&changed, 0) || last - shown >= Stillms);
		if (show)
			makeframe();
		flipping = 0;
		framestart = last;
		frames++;
		LeaveCriticalSection(&lock);
		SetEvent(framed);
		if (show) {
			showframe();
			shown = last;
		}
	}
	return 0;
}

/* waitframe waits for a frame after the frames counted n. */
static void
waitframe(LONG n)
{
	while (frames == n)
		WaitForSingleObject(framed, 100);
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
	s->sysmem = sysmem;
	return s;
}

static void
touched(Surface *s)
{
	if (s == primary)
		changed = 1;
}

/* rectof sets r to the rectangle p, or the whole of s, and reports whether it lies inside s and holds a pixel. */
static int
rectof(Surface *s, const RECT *p, RECT *r)
{
	if (p == NULL) {
		r->left = 0;
		r->top = 0;
		r->right = s->w;
		r->bottom = s->h;
		return 1;
	}
	*r = *p;
	return r->left >= 0 && r->top >= 0 && r->right <= s->w && r->bottom <= s->h &&
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

/* copyrect copies the rectangle b of s to x, y of d, from the bottom up when it moves down on one surface. */
static void
copyrect(Surface *d, int x, int y, Surface *s, const RECT *b, int keyed)
{
	const unsigned char *sp;
	unsigned char *dp;
	int j, w, h, step;

	w = b->right - b->left;
	h = b->bottom - b->top;
	step = d == s && y > b->top ? -1 : 1;
	sp = s->pixels + (size_t)(step > 0 ? b->top : b->bottom - 1) * s->w + b->left;
	dp = d->pixels + (size_t)(step > 0 ? y : y + h - 1) * d->w + x;
	for (j = 0; j < h; j++) {
		copyrow(dp, sp, w, keyed, s->key);
		sp += step * s->w;
		dp += step * d->w;
	}
}

static ULONG WINAPI
ddrelease(IDirectDraw *p)
{
	(void)p;
	return 0;
}

static HRESULT WINAPI
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

static HRESULT WINAPI
ddcreatesurface(IDirectDraw *p, LPDDSURFACEDESC sd, LPDIRECTDRAWSURFACE *out, IUnknown *u)
{
	static int started;
	Surface *s;
	HANDLE th;

	(void)p, (void)u;
	if (sd->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE) {
		if (primary != NULL)
			return DDERR_PRIMARYSURFACEALREADYEXISTS;
		s = newsurface(Gamew, Gameh, 0);
		if (s == NULL)
			return DDERR_OUTOFMEMORY;
		s->primary = 1;
		if ((sd->dwFlags & DDSD_BACKBUFFERCOUNT) && sd->dwBackBufferCount > 0) {
			s->back = newsurface(Gamew, Gameh, 0);
			if (s->back == NULL) {
				free(s->mem);
				free(s);
				return DDERR_OUTOFMEMORY;
			}
		}
		EnterCriticalSection(&lock);
		primary = s;
		LeaveCriticalSection(&lock);
		if (!started) {
			/* without the thread no frame would come, and Flip would wait for one forever */
			th = CreateThread(NULL, 0, display, NULL, 0, NULL);
			if (th == NULL) {
				surfrelease(&s->iface);
				return DDERR_GENERIC;
			}
			CloseHandle(th);
			started = 1;
		}
		*out = &s->iface;
		return DD_OK;
	}
	if (!(sd->dwFlags & DDSD_WIDTH) || !(sd->dwFlags & DDSD_HEIGHT) ||
	    sd->dwWidth == 0 || sd->dwHeight == 0 || sd->dwWidth > Maxsize || sd->dwHeight > Maxsize)
		return DDERR_INVALIDPARAMS;
	s = newsurface((int)sd->dwWidth, (int)sd->dwHeight, (sd->ddsCaps.dwCaps & DDSCAPS_SYSTEMMEMORY) != 0);
	if (s == NULL)
		return DDERR_OUTOFVIDEOMEMORY;
	*out = &s->iface;
	return DD_OK;
}

static HRESULT WINAPI
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

static HRESULT WINAPI
ddvblank(IDirectDraw *p, LPBOOL b)
{
	(void)p;
	*b = nowms() - framestart < Blankms;
	return DD_OK;
}

static HRESULT WINAPI
ddcooperate(IDirectDraw *p, HWND w, DWORD flags)
{
	(void)p, (void)flags;
	window = w;
	placeframe();
	hookimports();
	return DD_OK;
}

static HRESULT WINAPI
ddsetmode(IDirectDraw *p, DWORD w, DWORD h, DWORD bpp)
{
	(void)p;
	if (w != Gamew || h != Gameh || bpp != 8)
		return DDERR_INVALIDMODE;
	return DD_OK;
}

static HRESULT WINAPI
ddwaitvb(IDirectDraw *p, DWORD flags, HANDLE ev)
{
	(void)p, (void)flags, (void)ev;
	waitframe(frames);
	return DD_OK;
}

static IDirectDrawVtbl ddvtbl = {
	.Release = ddrelease,
	.CreatePalette = ddcreatepalette,
	.CreateSurface = ddcreatesurface,
	.GetCaps = ddgetcaps,
	.GetVerticalBlankStatus = ddvblank,
	.SetCooperativeLevel = ddcooperate,
	.SetDisplayMode = ddsetmode,
	.WaitForVerticalBlank = ddwaitvb,
};

__declspec(dllexport) HRESULT WINAPI
DirectDrawCreate(GUID *g, LPDIRECTDRAW *out, IUnknown *u)
{
	LARGE_INTEGER freq;

	(void)g, (void)u;
	if (framed == NULL) {
		QueryPerformanceFrequency(&freq);
		perms = (double)freq.QuadPart / 1000;
		InitializeCriticalSection(&lock);
		InitializeCriticalSection(&arealock);
		framed = CreateEventA(NULL, FALSE, FALSE, NULL);
		if (framed == NULL)
			return DDERR_GENERIC;
		timeBeginPeriod(1);
	}
	*out = &dd;
	return DD_OK;
}

static ULONG WINAPI
surfrelease(IDirectDrawSurface *p)
{
	Surface *s;

	s = surfof(p);
	EnterCriticalSection(&lock);
	if (s == primary)
		primary = NULL;
	LeaveCriticalSection(&lock);
	if (s->back != NULL) {
		free(s->back->mem);
		free(s->back);
	}
	free(s->mem);
	free(s);
	return 0;
}

static HRESULT WINAPI
surfblt(IDirectDrawSurface *p, LPRECT dr, LPDIRECTDRAWSURFACE src, LPRECT sr, DWORD flags, LPDDBLTFX fx)
{
	Surface *d, *s;
	RECT a, b;
	int y;

	d = surfof(p);
	if (!rectof(d, dr, &a))
		return DDERR_INVALIDRECT;
	if (flags & DDBLT_COLORFILL) {
		if (fx == NULL)
			return DDERR_INVALIDPARAMS;
		for (y = a.top; y < a.bottom; y++)
			memset(d->pixels + (size_t)y * d->w + a.left, (int)(fx->dwFillColor & 0xff), a.right - a.left);
		touched(d);
		return DD_OK;
	}
	if (src == NULL)
		return DDERR_INVALIDPARAMS;
	s = surfof(src);
	if (!rectof(s, sr, &b))
		return DDERR_INVALIDRECT;
	if (a.right - a.left != b.right - b.left || a.bottom - a.top != b.bottom - b.top)
		return DDERR_NOSTRETCHHW;
	copyrect(d, a.left, a.top, s, &b, 0);
	touched(d);
	return DD_OK;
}

static HRESULT WINAPI
surfbltfast(IDirectDrawSurface *p, DWORD x, DWORD y, LPDIRECTDRAWSURFACE src, LPRECT sr, DWORD flags)
{
	Surface *d, *s;
	RECT b;

	d = surfof(p);
	s = surfof(src);
	if (!rectof(s, sr, &b))
		return DDERR_INVALIDRECT;
	if ((long long)x + (b.right - b.left) > d->w || (long long)y + (b.bottom - b.top) > d->h)
		return DDERR_INVALIDRECT;
	copyrect(d, (int)x, (int)y, s, &b, (flags & DDBLTFAST_SRCCOLORKEY) && s->keyed);
	touched(d);
	return DD_OK;
}

static HRESULT WINAPI
surfflip(IDirectDrawSurface *p, LPDIRECTDRAWSURFACE over, DWORD flags)
{
	Surface *s;
	unsigned char *t;
	LONG n;

	(void)over, (void)flags;
	s = surfof(p);
	if (!s->primary || s->back == NULL)
		return DDERR_NOTFLIPPABLE;
	/* frames first: a frame that comes between the two reads then ends the wait at once */
	for (;;) {
		n = frames;
		if (!flipping)
			break;
		waitframe(n);
	}
	EnterCriticalSection(&lock);
	t = s->mem;
	s->mem = s->back->mem;
	s->back->mem = t;
	t = s->pixels;
	s->pixels = s->back->pixels;
	s->back->pixels = t;
	changed = 1;
	flipping = 1;
	LeaveCriticalSection(&lock);
	return DD_OK;
}

static HRESULT WINAPI
surfattached(IDirectDrawSurface *p, LPDDSCAPS caps, LPDIRECTDRAWSURFACE *out)
{
	Surface *s;

	s = surfof(p);
	if (!(caps->dwCaps & DDSCAPS_BACKBUFFER) || s->back == NULL)
		return DDERR_NOTFOUND;
	*out = &s->back->iface;
	return DD_OK;
}

/* GDI on a surface serves only the game's dead textf. */
static HRESULT WINAPI
surfgetdc(IDirectDrawSurface *p, HDC *dc)
{
	(void)p;
	*dc = NULL;
	return DDERR_CANTCREATEDC;
}

static HRESULT WINAPI
surfreleasedc(IDirectDrawSurface *p, HDC dc)
{
	(void)p, (void)dc;
	return DD_OK;
}

/* describe gives the numbers of s, for GetSurfaceDesc and Lock. */
static void
describe(Surface *s, LPDDSURFACEDESC sd)
{
	sd->dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PITCH | DDSD_PIXELFORMAT;
	sd->dwWidth = (DWORD)s->w;
	sd->dwHeight = (DWORD)s->h;
	sd->lPitch = s->w;
	sd->lpSurface = s->pixels;
	memset(&sd->ddpfPixelFormat, 0, sizeof sd->ddpfPixelFormat);
	sd->ddpfPixelFormat.dwSize = sizeof sd->ddpfPixelFormat;
	sd->ddpfPixelFormat.dwRGBBitCount = 8;
	sd->ddsCaps.dwCaps = s->sysmem ? DDSCAPS_SYSTEMMEMORY : DDSCAPS_VIDEOMEMORY;
	if (s->primary)
		sd->ddsCaps.dwCaps |= DDSCAPS_PRIMARYSURFACE;
}

static HRESULT WINAPI
surfdesc(IDirectDrawSurface *p, LPDDSURFACEDESC sd)
{
	describe(surfof(p), sd);
	return DD_OK;
}

static HRESULT WINAPI
surflock(IDirectDrawSurface *p, LPRECT r, LPDDSURFACEDESC sd, DWORD flags, HANDLE ev)
{
	(void)r, (void)flags, (void)ev;
	describe(surfof(p), sd);
	return DD_OK;
}

static HRESULT WINAPI
surfrestore(IDirectDrawSurface *p)
{
	(void)p;
	return DD_OK;
}

static HRESULT WINAPI
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

static HRESULT WINAPI
surfsetpalette(IDirectDrawSurface *p, LPDIRECTDRAWPALETTE pal)
{
	if (surfof(p)->primary) {
		EnterCriticalSection(&lock);
		palette = (Palette *)pal;
		changed = 1;
		LeaveCriticalSection(&lock);
	}
	return DD_OK;
}

static HRESULT WINAPI
surfunlock(IDirectDrawSurface *p, LPVOID bits)
{
	(void)bits;
	touched(surfof(p));
	return DD_OK;
}

static IDirectDrawSurfaceVtbl surfvtbl = {
	.Release = surfrelease,
	.Blt = surfblt,
	.BltFast = surfbltfast,
	.Flip = surfflip,
	.GetAttachedSurface = surfattached,
	.GetDC = surfgetdc,
	.GetSurfaceDesc = surfdesc,
	.Lock = surflock,
	.ReleaseDC = surfreleasedc,
	.Restore = surfrestore,
	.SetColorKey = surfsetkey,
	.SetPalette = surfsetpalette,
	.Unlock = surfunlock,
};

static ULONG WINAPI
palrelease(IDirectDrawPalette *p)
{
	EnterCriticalSection(&lock);
	if ((Palette *)p == palette)
		palette = NULL;
	LeaveCriticalSection(&lock);
	free(p);
	return 0;
}

static HRESULT WINAPI
palgetentries(IDirectDrawPalette *p, DWORD flags, DWORD start, DWORD n, LPPALETTEENTRY e)
{
	(void)flags;
	if (start > 256 || n > 256 - start)
		return DDERR_INVALIDPARAMS;
	memcpy(e, &((Palette *)p)->e[start], n * sizeof *e);
	return DD_OK;
}

static HRESULT WINAPI
palsetentries(IDirectDrawPalette *p, DWORD flags, DWORD start, DWORD n, LPPALETTEENTRY e)
{
	(void)flags;
	if (start > 256 || n > 256 - start)
		return DDERR_INVALIDPARAMS;
	memcpy(&((Palette *)p)->e[start], e, n * sizeof *e);
	if ((Palette *)p == palette)
		changed = 1;
	return DD_OK;
}

static IDirectDrawPaletteVtbl palvtbl = {
	.Release = palrelease,
	.GetEntries = palgetentries,
	.SetEntries = palsetentries,
};

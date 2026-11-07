/*
 * What the unit tests link in place of src/sys/main.c: the folders, the
 * display's settings, and sysfatal. It also holds setflag and
 * replaysetup, which replay.c calls, and the helpers of test.h.
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <windows.h>
#include <ddraw.h>

#include "sys.h"
#include "test.h"

char sysgamedir[Maxpath];
char sysuserdir[Maxpath];
int sysfullscreen;
int syssizew;
int syssizeh;
int syssharp;
int syswide;
int sysfixedwidth;
int sysedges;
int sysvblank = -1;
const int sysiconw = 1;
const int sysiconh = 1;
const unsigned int sysicon[1];
int failed;

static jmp_buf *catcher;	/* where sysfatal returns while fails runs */

/* setflag stands for the game's, plt.cpp, which replay.c calls. */
void
setflag(int n, int v)
{
	(void)n;
	(void)v;
}

/* the last call of replaysetup, which the test of replay.c reads */
int setupwhat, setupa, setupb;

/* replaysetup stands for the game's, npc.cpp, and keeps its arguments. */
void
replaysetup(int what, int a, int b)
{
	setupwhat = what;
	setupa = a;
	setupb = b;
}

void
sysfatal(const char *fmt, ...)
{
	va_list ap;

	if (catcher != NULL)
		longjmp(*catcher, 1);
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	exit(1);
}

/* fails reports whether f(arg) called sysfatal, which then ends f's run in place of the test's. */
int
fails(void (*f)(void *), void *arg)
{
	jmp_buf env;

	catcher = &env;
	if (setjmp(env) != 0) {
		catcher = NULL;
		return 1;
	}
	f(arg);
	catcher = NULL;
	return 0;
}

/* check prints what and marks the test failed, when ok is 0. */
void
check(int ok, const char *what)
{
	if (!ok) {
		printf("%s\n", what);
		failed = 1;
	}
}

/* setrect sets r to the rectangle from x0, y0 to x1, y1, the last excluded. */
void
setrect(RECT *r, int x0, int y0, int x1, int y1)
{
	r->left = x0;
	r->top = y0;
	r->right = x1;
	r->bottom = y1;
}

/* offscreen returns a new offscreen surface of w by h. */
LPDIRECTDRAWSURFACE
offscreen(LPDIRECTDRAW dd, int w, int h)
{
	DDSURFACEDESC sd;
	LPDIRECTDRAWSURFACE s;

	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	sd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	sd.dwWidth = w;
	sd.dwHeight = h;
	sd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	if (IDirectDraw_CreateSurface(dd, &sd, &s, NULL) != DD_OK)
		sysfatal("no offscreen surface of %d by %d", w, h);
	return s;
}

/* lock returns the pixel of s at the game's 0, 0 and its pitch in *pitch. */
unsigned char *
lock(LPDIRECTDRAWSURFACE s, LONG *pitch)
{
	DDSURFACEDESC sd;

	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	if (IDirectDrawSurface_Lock(s, NULL, &sd, DDLOCK_WAIT, NULL) != DD_OK)
		sysfatal("lock failed");
	*pitch = sd.lPitch;
	return sd.lpSurface;
}

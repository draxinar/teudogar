/*
 * The prelude of the game's files in the port. make compiles each
 * module of src/base, src/teudogar, and src/feature as C99 with
 * -include of this file, before the module's own includes.
 *
 * It includes every header the modules include, so that each later
 * #include finds its guard set, then gives the names of the game the
 * meaning Watcom gave them:
 * - The game's names that libc, libm, the mingw runtime, or the
 *   libraries of SDL3 and FluidSynth also define (readline's crlf), or
 *   that macOS's stdlib.h declares (setkey), take names of their own,
 *   so that each library keeps its own.
 * - WinMain becomes gamemain, which src/sys/main.c calls.
 * - __cdecl, __stdcall, and EXTERN_C expand to empty: C has one
 *   calling convention and one linkage.
 * - _lrotl and _lrotr rotate 32 bits, as Watcom's intrinsics do.
 * - On LP64, long is int, the 32 bits of Watcom's long, which the
 *   game's structs, saved games, checksums, and formats assume.
 */

#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <dsound.h>
#include <shlobj.h>

#define advance actadvance
#define clearerr baseclearerr
#define crlf basecrlf
#define ftime baseftime
#define logf baselogf
#define putch gfxputch
#define random baserandom
#define setkey gfxsetkey
#define sysinfo menusysinfo

#define WinMain gamemain

#undef __cdecl
#define __cdecl
#undef __stdcall
#define __stdcall
#undef EXTERN_C
#define EXTERN_C

#define _lrotl sysrotl
#define _lrotr sysrotr
DWORD _lrotl(DWORD, int);
DWORD _lrotr(DWORD, int);

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

/* fwdcopy copies n bytes from src to dst one at a time, upward, as
 * Watcom's memcpy does when they overlap */
void fwdcopy(void *dst, const void *src, size_t n);

/* systrace writes a tick of the game into TEUDOGAR_TRACE's file */
void systrace(int x, int y, int step, unsigned time, int pace, int scroll, int mode);

/* the screen in the game's coordinates: its frame from 0 to 640 and 480
 * rows, and in the wide view the wings past them */
extern RECT sysscreen;

/* what the game's drawing on a screen adds to x: a part of the classic
 * screen that shows elsewhere */
extern int sysdrawx;

#include "strip.h"

enum {
	Widehalf = 21	/* the columns of the wide view's sight each side of the hero */
};

/* stripx returns the classic screen's x for a pointer at x of the wide
 * screen, on the map */
int stripx(int x);

/* the wide screen's layout, src/sys and the PORT hooks of src/teudogar:
 * the game's numbers stay the classic screen's, a part of which shows
 * elsewhere at a shift, which drawing adds and the pointer subtracts */
enum {
	Wingkind = 0xe7	/* the hoverkind of a pointer on the map in a wing */
};
extern int sysfullscreen;	/* whether the window fills the display */
void wintogglefullscreen(void);
void wintogglewide(void);
extern int seenleft;	/* the left edge that the last poll saw, or 1 before the first */
extern int syswide;	/* the view the player chose: whether the screen takes the output's width */
extern int sysedges;	/* whether the HUD stands at the wide screen's edges */
extern int sysroam;	/* whether the pointer roams the screen, in the play view */
extern int sysframepoints;	/* whether a replay's points are the frame's */
void syssetroam(int on);
void sysholdpointer(int x);
void drawclearback(void);
extern int mousewx;	/* mousex's true place on the wide screen */
extern int hoverwx;	/* hoverx's */
extern int presswx;	/* presscx's point */
extern int widepanel;	/* the panel under the pointer while it roams, or 0xff */
extern int playflip;	/* whether the next flip shows a play frame */
extern int tipshift;	/* where a name's tip stands from its classic place */
extern int paneloff[16];	/* what a drag on the wide screen added to a panel's shift */
void widepoint(POINT *p);
int hudleft(void);
int hudright(void);
int panelshift(int id);
void placepanel(int id, int sx, int y);
void placeatpointer(int id);

/* screensurface makes s, a surface of 640 by 480, one of the screen's,
 * whose wings the copies between such surfaces carry */
void screensurface(LPDIRECTDRAWSURFACE s);

/* drawkeepwings copies the primary's wings to its back buffer, for a
 * flip that keeps them */
void drawkeepwings(void);

#if LONG_MAX > 0x7fffffffL
#define long int
#endif

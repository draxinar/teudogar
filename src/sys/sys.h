#include "strip.h"

/*
 * The interfaces between the files of src/sys, the layer of Win32,
 * DirectX, and WinMM on SDL3 that the port of the game runs on. Each
 * file of src/sys includes SDL3, then <windows.h>, then this header.
 */

enum {
	Gamew = 640,	/* the size of the game's screen */
	Gameh = 480,
	Maxscreenw = 1704,	/* the widest screen, 32:9 */
	Widewindow = 852,	/* a wide window's first width, 16:9, on a display no wider than 4:3 */
	Maxpath = 1024	/* the longest path of the host the layer builds */
};

/* main.c */
extern char sysgamedir[Maxpath];	/* the game's folder, with a final separator */
extern char sysuserdir[Maxpath];	/* the per-user folder, with a final separator */
extern int sysfullscreen;	/* whether the window fills the display */
extern int syssizew, syssizeh;	/* the window's first size that --size asks for, else 0 */
extern int syssharp;	/* whether the game scales sharp bilinear, else by whole numbers */
extern int syswide;	/* the view the player chose: whether the screen takes the output's width */
extern int sysfixedwidth;	/* whether TEUDOGAR_WIDTH or a recording fixes the screen's width */
extern int sysedges;	/* whether the HUD stands at the wide screen's edges, prelude.h */
extern const int sysiconw, sysiconh;	/* the size of the window's icon, build/gen/icon.c */
extern const unsigned int sysicon[];	/* its pixels, 0xAARRGGBB */
extern int sysvblank;	/* the refresh in Hz that TEUDOGAR_VBLANK simulates, 0 for none, or -1 without it */
void sysfatal(const char *fmt, ...) __attribute__((noreturn));	/* reports, and ends the run */

/* win.c */
extern SDL_Window *syswindow;
extern SDL_Renderer *sysrenderer;
int wininit(void);
void winpump(int wait);
void wintogglefullscreen(void);
void wintogglewide(void);
void wintogglescale(void);
int winviewwidth(void);
extern int sysroam;	/* whether the pointer roams the screen, prelude.h */
void syssetroam(int on);
void sysholdpointer(int x);
char *winutf8(const char *s);
int tocp1252(Uint32 c);

/* draw.c */
extern int sysscreenw;	/* the width of the screen, a multiple of 4 */
extern int sysframex;	/* the game's frame's column in the screen */
extern RECT sysscreen;	/* the screen in the game's coordinates, prelude.h */
extern int sysdrawx;	/* what the game's drawing on a screen adds to x, prelude.h */
extern Uint32 sysflips;	/* the flips so far, for the trace */
extern Uint32 sysshows;	/* the presents so far, for the trace */
extern Uint32 systextures;	/* the textures made so far, for the tests */
void drawpresent(int force);
void drawreset(void);
void drawexposed(void);
void drawsetscale(void);
void drawsetwidth(int w);
int screenwidth(int w, int h, int sharp);
int windowwidth(int w, int h);
int stripx(int x);
void drawkeepwings(void);
void drawclearback(void);

/* sound.c and midi.c */
int soundinit(void);
void soundquit(void);
int midiinit(void);
void midiquit(void);
void mididrain(void);

/* file.c */
int hostpath(const char *win, char *host, size_t n, int create);
void fileinit(void);
void sysfull(long n);
void sysinplace(int on);

/* reg.c */
void reginit(void);
DWORD sysregdword(const char *name, DWORD def);
void syssetregdword(const char *name, DWORD v);
int sysregstring(const char *name, char *buf, size_t n);

/* kernel.c */
DWORD sysclock(void);
void sysdelay(DWORD ms);
SDL_Time sysdatetime(void);
void sysnow(SYSTEMTIME *st);
void systotime(SDL_Time t, FILETIME *ft);
void syssetlasterror(DWORD err);
char *syslocale(char *country, size_t n);
int fileurl(char *url, size_t n, const char *path);

/* replay.c */
extern int replaying;	/* whether a script drives the game, or a recording runs */
extern int recording;	/* whether a recording runs */
extern int sysframepoints;	/* whether a replay's points are the frame's, prelude.h */
void replayinit(void);
void replaywidth(void);
DWORD replayclock(void);
void replayadvance(DWORD ms);
int replayevent(SDL_Event *e);
void replayrecord(const SDL_Event *e);
void replayquit(void);
void systrace(int x, int y, int step, unsigned time, int pace, int scroll, int mode);

/* sold.c, which make generates */
void soldinit(void);

/* the game */
int gamemain(HINSTANCE, HINSTANCE, LPSTR, int);
void initrng(void);

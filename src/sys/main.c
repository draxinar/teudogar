/*
 * The start of the port: SDL, the folders, the registry, the licence
 * bytes, the random generator, sound and music, then the game's
 * WinMain, which the prelude names gamemain.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <SDL3/SDL.h>
#include <windows.h>

#include "sys.h"

enum {
	Maxword = 255,	/* of a word for the game, whose & forms copy into buffers of 1000 */
	Maxsize = 16384,	/* of a side of the window that --size asks for */
	Maxvblank = 1000	/* of the refresh that TEUDOGAR_VBLANK simulates */
};

/*
 * The per-user folder: ~/.local/share/teudogar on Linux, as XDG wants,
 * ~/Library/Application Support/teudogar on macOS, and
 * %APPDATA%\Wolf Mittag\Teudogar on Windows, as each system wants.
 */
#ifdef _WIN32
static const char userorg[] = "Wolf Mittag";
static const char userapp[] = "Teudogar";
#else
static const char userorg[] = "";
static const char userapp[] = "teudogar";
#endif

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

/* the options that choose the display, which the registry file keeps */
static const struct {
	const char *opt;
	int *setting;
	const char *name;	/* the setting's value in the registry */
	int value;
} choices[] = {
	{ "--window", &sysfullscreen, "Fullscreen", 0 },
	{ "--fullscreen", &sysfullscreen, "Fullscreen", 1 },
	{ "--integer", &syssharp, "Scale", 0 },
	{ "--sharp", &syssharp, "Scale", 1 },
	{ "--wide", &syswide, "Wide", 1 },
	{ "--classic", &syswide, "Wide", 0 },
};

/* the options that stand for words of the game's command line (inisys) */
static const struct {
	const char *opt;
	const char *word;
} gameopts[] = {
	{ "--nosound", "nosound" },
	{ "--nomusic", "nomusic" },
	{ "--quickstart", "quickstart" },
	{ "--newgame", "newgame" },
};

/* the values of --lang, and the game's words for them */
static const struct {
	const char *code;
	const char *word;
} langs[] = {
	{ "de", "deut" },
	{ "en", "engl" },
	{ "fr", "fran" },
	{ "it", "ital" },
};

static const char usageline[] = "usage: teudogar [options] [-- word ...]\n";
static const char helptext[] =
	"\n"
	"--window, --fullscreen  a window, or the full screen\n"
	"--sharp, --integer      any size, or whole multiples\n"
	"--wide, --classic       the output's width up to 32:9, or 4:3\n"
	"--size WxH              the window's first size, for this run\n"
	"--nosound, --nomusic    no sound, no music\n"
	"--quickstart            skip the titles\n"
	"--newgame               start a new game\n"
	"--lang de|en|fr|it      the language\n"
	"-h, --help              this text\n"
	"--                      the rest is the game's own words\n"
	"\n"
	"The registry file keeps the window, the scale, and the view.\n";

static char cmdline[4096];	/* the game's command line, which inisys splits in place */
static int instance;	/* the address the instance handle points to */

void
sysfatal(const char *fmt, ...)
{
	va_list ap;
	char buf[1024];

	va_start(ap, fmt);
	vsnprintf(buf, sizeof buf, fmt, ap);
	va_end(ap);
	SDL_Log("teudogar: %s", buf);
	if (!replaying)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Teudogar", buf, NULL);
	exit(1);
}

/* setdir copies dir into buf with a final separator. */
static void
setdir(char *buf, const char *dir)
{
	size_t n;

	n = strlen(dir);
	if (n + 2 > Maxpath)
		sysfatal("folder %s: path too long", dir);
	memcpy(buf, dir, n + 1);
	if (n == 0 || (dir[n-1] != '/' && dir[n-1] != '\\'))
		strcat(buf, "/");
}

/* folders sets the game's folder and the per-user folder. */
static void
folders(void)
{
	const char *base, *home;
	char *pref;

	base = SDL_GetBasePath();
	if (base == NULL)
		sysfatal("the game's folder: %s", SDL_GetError());
	setdir(sysgamedir, base);
	home = SDL_getenv("TEUDOGAR_HOME");
	if (home != NULL && home[0] != '\0') {
		if (!SDL_CreateDirectory(home))
			sysfatal("%s: %s", home, SDL_GetError());
		setdir(sysuserdir, home);
		return;
	}
	pref = SDL_GetPrefPath(userorg, userapp);
	if (pref == NULL)
		sysfatal("the per-user folder: %s", SDL_GetError());
	setdir(sysuserdir, pref);
	SDL_free(pref);
}

/* say writes text on file descriptor fd, or in a box where Windows gave the program no such stream. */
static void
say(int fd, const char *text)
{
#ifdef _WIN32
	if (_get_osfhandle(fd) < 0) {
		SDL_ShowSimpleMessageBox(fd == 2 ? SDL_MESSAGEBOX_ERROR : SDL_MESSAGEBOX_INFORMATION,
			"Teudogar", text, NULL);
		return;
	}
	if (_write(fd, text, (unsigned)strlen(text)) < 0)
		exit(1);
#else
	if (write(fd, text, strlen(text)) < 0)
		exit(1);
#endif
}

/* usage reports a bad command line, as what with the argument arg, and exits. */
static void
usage(const char *what, const char *arg)
{
	char buf[Maxword + 256];

	snprintf(buf, sizeof buf, "teudogar: %s%.*s\n%s", what, Maxword, arg, usageline);
	say(2, buf);
	exit(2);
}

/* addword appends a word to the game's command line. */
static void
addword(const char *word)
{
	static size_t n;
	size_t len;

	len = strlen(word);
	if (len > Maxword)
		usage("a word over 255 bytes: ", word);
	if (n + len + 2 > sizeof cmdline)
		usage("the command line is too long", "");
	if (n > 0)
		cmdline[n++] = ' ';
	memcpy(cmdline + n, word, len + 1);
	n += len;
}

/* addlang appends the game's word for the language code. */
static void
addlang(const char *code)
{
	size_t i;

	for (i = 0; i < sizeof langs / sizeof langs[0]; i++)
		if (strcmp(code, langs[i].code) == 0) {
			addword(langs[i].word);
			return;
		}
	usage("--lang wants de, en, fr, or it, not ", code);
}

/* setsize takes the size of the window from s, WxH in decimal digits alone. */
static void
setsize(const char *s)
{
	char *end;
	long w, h;

	w = 0;
	h = 0;
	if (s[0] >= '0' && s[0] <= '9') {
		w = strtol(s, &end, 10);
		if (*end == 'x' && end[1] >= '0' && end[1] <= '9') {
			h = strtol(end + 1, &end, 10);
			if (*end != '\0')
				h = 0;
		}
	}
	if (w < Gamew || w > Maxsize || h < Gameh || h > Maxsize)
		usage("--size wants WxH from 640x480 to 16384x16384, not ", s);
	syssizew = (int)w;
	syssizeh = (int)h;
}

/* ischoice reports whether opt chooses a display setting. */
static int
ischoice(const char *opt)
{
	size_t i;

	for (i = 0; i < sizeof choices / sizeof choices[0]; i++)
		if (strcmp(opt, choices[i].opt) == 0)
			return 1;
	return 0;
}

/*
 * args checks the command line and joins what the game reads into
 * cmdline. It runs before SDL_Init, so that --help and a bad option stop
 * ahead of the display.
 */
static void
args(int argc, char *argv[])
{
	size_t i;
	int a;
	const char *s;

	for (a = 1; a < argc; a++) {
		s = argv[a];
		if (strcmp(s, "--") == 0) {
			for (a++; a < argc; a++)
				addword(argv[a]);
			return;
		}
		if (strcmp(s, "-h") == 0 || strcmp(s, "--help") == 0) {
			char text[sizeof usageline + sizeof helptext];

			snprintf(text, sizeof text, "%s%s", usageline, helptext);
			say(1, text);
			exit(0);
		}
		if (ischoice(s))
			continue;
		if (strcmp(s, "--lang") == 0) {
			if (++a == argc)
				usage("--lang wants de, en, fr, or it", "");
			addlang(argv[a]);
			continue;
		}
		if (strncmp(s, "--lang=", 7) == 0) {
			addlang(s + 7);
			continue;
		}
		if (strcmp(s, "--size") == 0) {
			if (++a == argc)
				usage("--size wants WxH", "");
			setsize(argv[a]);
			continue;
		}
		if (strncmp(s, "--size=", 7) == 0) {
			setsize(s + 7);
			continue;
		}
		for (i = 0; i < sizeof gameopts / sizeof gameopts[0]; i++)
			if (strcmp(s, gameopts[i].opt) == 0)
				break;
		if (i == sizeof gameopts / sizeof gameopts[0])
			usage("unknown option ", s);
		addword(gameopts[i].word);
	}
}

/* setchoices applies the options that choose the display, in argv's order, so a later one wins. */
static void
setchoices(int argc, char *argv[])
{
	size_t i;
	int a;

	for (a = 1; a < argc && strcmp(argv[a], "--") != 0; a++)
		for (i = 0; i < sizeof choices / sizeof choices[0]; i++)
			if (strcmp(argv[a], choices[i].opt) == 0) {
				*choices[i].setting = choices[i].value;
				syssetregdword(choices[i].name, choices[i].value);
			}
}

/*
 * setscreen gives the screen its first width: TEUDOGAR_WIDTH's, fixed
 * for the session, or in the wide view that of the size --size asks for,
 * or else a wide window's. The width then follows the window and the full
 * screen (win.c), unless a recording fixes it.
 */
static void
setscreen(void)
{
	const SDL_DisplayMode *m;
	const char *s;
	char *end;
	long w;

	s = SDL_getenv("TEUDOGAR_WIDTH");
	if (s != NULL && s[0] != '\0') {
		w = SDL_strtol(s, &end, 10);
		if (*end != '\0' || w < Gamew || w > Maxscreenw || w % 4 != 0)
			sysfatal("TEUDOGAR_WIDTH=%s: want %d to %d in steps of 4", s, Gamew, Maxscreenw);
		sysfixedwidth = 1;
		drawsetwidth((int)w);
		return;
	}
	sysfixedwidth = recording;
	if (!syswide)
		return;
	if (syssizew > 0)
		drawsetwidth(screenwidth(syssizew, syssizeh, syssharp));
	else if ((m = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay())) != NULL)
		drawsetwidth(windowwidth(m->w, m->h));
}

/*
 * setvblank reads TEUDOGAR_VBLANK, which simulates a display for the
 * measures of the game's pace: each present then waits for the next
 * vertical blank of a display of that many Hz, or with 0 for none, on
 * the game's clock.
 */
static void
setvblank(void)
{
	const char *s;
	char *end;
	long hz;

	s = SDL_getenv("TEUDOGAR_VBLANK");
	if (s == NULL || s[0] == '\0')
		return;
	hz = SDL_strtol(s, &end, 10);
	if (*end != '\0' || hz < 0 || hz > Maxvblank)
		sysfatal("TEUDOGAR_VBLANK=%s: want 0 to %d", s, Maxvblank);
	sysvblank = (int)hz;
}

int
main(int argc, char *argv[])
{
	int r;

	args(argc, argv);
	SDL_SetAppMetadata("Teudogar and the Alliance with Rome", VERSION, "io.github.draxinar.teudogar");
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		sysfatal("SDL: %s", SDL_GetError());
	folders();
	replayinit();
	fileinit();
	reginit();
	sysfullscreen = sysregdword("Fullscreen", 0);
	syssharp = sysregdword("Scale", !replaying);
	syswide = sysregdword("Wide", 0);
	setchoices(argc, argv);
	setscreen();
	setvblank();
	sysedges = SDL_getenv("TEUDOGAR_CENTERED") == NULL;
	replaywidth();
	soldinit();
	initrng();
	if (SDL_InitSubSystem(SDL_INIT_AUDIO)) {
		soundinit();
		midiinit();
	} else
		SDL_Log("teudogar: no audio: %s", SDL_GetError());
	r = gamemain((HINSTANCE)&instance, NULL, cmdline, SW_SHOW);
	midiquit();
	soundquit();
	replayquit();
	SDL_Quit();
	return r;
}

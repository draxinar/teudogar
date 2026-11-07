/*
 * Replays: with TEUDOGAR_REPLAY naming a script, the game runs on a
 * virtual clock and the script's events take the place of the keyboard
 * and the mouse, so that every build of the port plays the same game to
 * the same bytes. The clock starts far above 0, which the game takes for
 * unset, and moves 1 ms for each pump of messages that finds the queue
 * empty and by each Sleep, so the seed that inisys draws from
 * timeGetTime, the play time, and the times of the saved games are the
 * same on every run.
 *
 * A script holds one event to a line, after its time in milliseconds
 * since the start:
 *
 *	1500 key down Return	the key of SDL's scancode name, with _ for a blank
 *	1600 key up Return
 *	1700 key down Left_Alt
 *	2000 move 320 240	the mouse to a point of the game's screen
 *	2100 button left down 320 240
 *	2200 button left up 320 240
 *	9000 close	the window's close button
 *	9000 flag 500 1	game flag 500 takes the value 1, for a state that play reaches late
 *	9000 full 200000	the per-user folder takes 200000 more bytes, as a disk that fills, and -1 ends it
 *	9000 inplace 1	an open that empties a file empties the file itself, as on Windows, and 0 ends it
 *	9000 bless W 3	the blessing of the hero's item in the slot W of the scripts, 0 to 15, here a curse
 *	9000 health 30	the hero's health
 *	9000 hunger 5040	the hero's hunger, in game.hungercount: 1680 a day of rest
 *	9000 fear 40	the hero flees for 40 turns, as a Frighten leaves him
 *	9000 avenger 9971 2	2 avengers of scene 9971 come from beside the hero, as travel sends one, 1 to 150
 *	9000 summon 3800	the person of the map who speaks scene 3800 stands above the hero, or the nearest free cell
 *	9000 door 137 0	a closed door stands with its right half at cell 137,0 and opens, as a lever opens one
 *
 * A line that starts with # is a comment. A line holds the words of its
 * event alone, and each number lies in its event's range, the same on
 * every host: a time runs from 0 to 4274967295, where the virtual clock
 * ends, and a point's x and y from -32767 to 32767. Keys take the layout
 * of a US keyboard, whatever the host's, so that a script plays alike on
 * every host. A point is the game's own, at any width of the screen, or
 * with the line "# screen" in the script a true one of the wide screen,
 * which goes through the layout as real input does.
 *
 * With TEUDOGAR_RECORD naming a file, a session played by hand runs as
 * a replay does, on the virtual clock and with the keyboard and the
 * mouse, and writes its input into the file in this form, each event at
 * its virtual time, with the keys that SDL names as their own, every key
 * that the game reads among them. The clock keeps pace with the real
 * one: an advance that runs ahead of it waits for it. Replaying the file
 * then plays the same game. A recording plays no music, since the
 * synthesizer's thread would run on the real clock. On a wide screen it
 * fixes the width and writes it with "# screen".
 *
 * With TEUDOGAR_TRACE naming a file, each tick of the game writes a line
 * into it: the clock, the hero's x and y, steptick, gametime, ptrpace,
 * the scrolling option, the game's mode, and the flips and the presents
 * so far. It works in a replay and in a run on the real clock.
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <windows.h>

#include "sys.h"

enum {
	Clockbase = 20000000,
	Maxline = 256,
	Maxwords = 6,	/* the words of the longest line, a button's */
	Maxroom = 0x7fffffff,	/* the most bytes that full gives, as a long of 32 bits holds them */
	Maxpoint = 0x7fff,	/* the farthest x or y of a point, as the 16 bits of a mouse message's hold it */
	Nflags = 0xc8 * 8,	/* the bits of Game.flags, 0xc8 bytes */
	Nunits = 150,	/* the units of a map, units[150] in npc.cpp */
	Mapsize = 150	/* the cells of a map's row and of its column */
};

void setflag(int n, int v);	/* plt.cpp */
void replaysetup(int what, int a, int b);	/* npc.cpp */

/* a setup event but flag, by the name and the what of replaysetup */
typedef struct Setup Setup;
struct Setup {
	const char *name;
	int what;
	int lo, hi;	/* the range of its number: bless's after the slot, avenger's scene before the count, door's x before the y */
};

static const Setup setups[] = {
	{ "bless", 'b', 0, 15 },
	{ "health", 'h', 1, 999 },
	{ "hunger", 'u', 0, 30000 },
	{ "fear", 'f', 0, 255 },
	{ "avenger", 'a', 0, 0xffff },
	{ "summon", 's', 0, 0xffff },
	{ "door", 'd', 1, Mapsize - 1 },
};

int replaying;
int recording;
int sysframepoints;

static char *script;	/* the script's text */
static char *cursor;	/* the next line of the script */
static DWORD vclock = Clockbase;
static SDL_IOStream *record;
static SDL_IOStream *trace;
static Uint64 recordstart;
static SDL_Keymod mods;

/* hasline reports whether a line of text, whatever ends it, is line. */
static int
hasline(const char *text, const char *line)
{
	const char *p;
	size_t n;

	n = strlen(line);
	for (p = text; p != NULL; p = strchr(p, '\n'), p = p != NULL ? p + 1 : NULL)
		if (strncmp(p, line, n) == 0 && (p[n] == '\0' || p[n] == '\n' || p[n] == '\r'))
			return 1;
	return 0;
}

void
replayinit(void)
{
	const char *path;

	path = SDL_getenv("TEUDOGAR_TRACE");
	if (path != NULL && path[0] != '\0') {
		trace = SDL_IOFromFile(path, "w");
		if (trace == NULL)
			sysfatal("trace %s: %s", path, SDL_GetError());
	}
	path = SDL_getenv("TEUDOGAR_REPLAY");
	if (path != NULL && path[0] != '\0') {
		script = SDL_LoadFile(path, NULL);
		if (script == NULL)
			sysfatal("replay %s: %s", path, SDL_GetError());
		cursor = script;
		replaying = 1;
		sysframepoints = !hasline(script, "# screen");
		return;
	}
	path = SDL_getenv("TEUDOGAR_RECORD");
	if (path != NULL && path[0] != '\0') {
		record = SDL_IOFromFile(path, "w");
		if (record == NULL)
			sysfatal("record %s: %s", path, SDL_GetError());
		recordstart = SDL_GetTicks();
		replaying = 1;
		recording = 1;
	}
}

/* replaywidth writes the width of a recording's wide screen, once setscreen gave it. */
void
replaywidth(void)
{
	if (record != NULL && sysframex > 0)
		SDL_IOprintf(record, "# width %d\n# screen\n", sysscreenw);
}

void
replayquit(void)
{
	if (cursor != NULL && *cursor != '\0')
		SDL_Log("replay: the game ended before the rest of the script, at the virtual time %lu",
			(unsigned long)(vclock - Clockbase));
	if (record != NULL)
		SDL_CloseIO(record);
	record = NULL;
	if (trace != NULL)
		SDL_CloseIO(trace);
	trace = NULL;
	SDL_free(script);
	script = NULL;
	cursor = NULL;
}

/* systrace writes a tick of the game into the trace. */
void
systrace(int x, int y, int step, unsigned time, int pace, int scroll, int mode)
{
	if (trace != NULL)
		SDL_IOprintf(trace, "%lu %d %d %d %u %d %d %d %lu %lu\n",
			(unsigned long)sysclock(), x, y, step, time, pace, scroll, mode,
			(unsigned long)sysflips, (unsigned long)sysshows);
}

DWORD
replayclock(void)
{
	return vclock;
}

/* replayadvance moves the virtual clock by ms, at least 1, keeping pace with the real clock while recording. */
void
replayadvance(DWORD ms)
{
	Uint64 real, virt;

	vclock += ms > 0 ? ms : 1;
	if (!recording)
		return;
	real = SDL_GetTicks() - recordstart;
	virt = vclock - Clockbase;
	if (virt > real)
		SDL_Delay((Uint32)(virt - real));
}

/* modbit returns the modifier that the key sc holds down, or 0. */
static SDL_Keymod
modbit(SDL_Scancode sc)
{
	switch (sc) {
	case SDL_SCANCODE_LSHIFT:
		return SDL_KMOD_LSHIFT;
	case SDL_SCANCODE_RSHIFT:
		return SDL_KMOD_RSHIFT;
	case SDL_SCANCODE_LCTRL:
		return SDL_KMOD_LCTRL;
	case SDL_SCANCODE_RCTRL:
		return SDL_KMOD_RCTRL;
	case SDL_SCANCODE_LALT:
		return SDL_KMOD_LALT;
	case SDL_SCANCODE_RALT:
		return SDL_KMOD_RALT;
	default:
		return SDL_KMOD_NONE;
	}
}

/* windowpoint converts a point of the game's screen into the window's. */
static void
windowpoint(int x, int y, float *wx, float *wy)
{
	*wx = (float)x;
	*wy = (float)y;
	if (sysrenderer != NULL)
		SDL_RenderCoordinatesToWindow(sysrenderer, x + sysframex + 0.5f, y + 0.5f, wx, wy);
}

/*
 * split cuts s at its blanks into the words of word, at most max, and
 * returns their count, or -1 for a line of more.
 */
static int
split(char *s, char *word[], int max)
{
	int n;

	for (n = 0;; n++) {
		s += strspn(s, " \t\r");
		if (*s == '\0')
			return n;
		if (n == max)
			return -1;
		word[n] = s;
		s += strcspn(s, " \t\r");
		if (*s != '\0')
			*s++ = '\0';
	}
}

/* number reads the decimal number s, from lo to hi, into *v, and reports whether it could. */
static int
number(const char *s, long long lo, long long hi, long long *v)
{
	char *end;

	errno = 0;
	*v = strtoll(s, &end, 10);
	return errno == 0 && end != s && *end == '\0' && *v >= lo && *v <= hi;
}

/* point reads the point of the words x and y into *px and *py, and reports whether it could. */
static int
point(const char *x, const char *y, int *px, int *py)
{
	long long vx, vy;

	if (!number(x, -Maxpoint, Maxpoint, &vx) || !number(y, -Maxpoint, Maxpoint, &vy))
		return 0;
	*px = (int)vx;
	*py = (int)vy;
	return 1;
}

/* updown returns 1 for the word down, 0 for up, and -1 for any other. */
static int
updown(const char *s)
{
	if (strcmp(s, "down") == 0)
		return 1;
	if (strcmp(s, "up") == 0)
		return 0;
	return -1;
}

/*
 * parse fills e with the event of the n words of a line after its time,
 * and returns 1, or returns -1 for a malformed line.
 */
static int
parse(char *word[], int n, SDL_Event *e)
{
	char *s;
	int x, y, down, button;
	float wx, wy;
	SDL_Scancode sc;

	memset(e, 0, sizeof *e);
	if (strcmp(word[0], "key") == 0 && n == 3) {
		down = updown(word[1]);
		for (s = word[2]; *s != '\0'; s++)
			if (*s == '_')
				*s = ' ';
		sc = SDL_GetScancodeFromName(word[2]);
		if (down < 0 || sc == SDL_SCANCODE_UNKNOWN)
			return -1;
		e->type = down ? SDL_EVENT_KEY_DOWN : SDL_EVENT_KEY_UP;
		e->key.down = down;
		if (down)
			mods |= modbit(sc);
		else
			mods &= ~modbit(sc);
		e->key.windowID = SDL_GetWindowID(syswindow);
		e->key.scancode = sc;
		e->key.key = SDLK_UNKNOWN;
		e->key.mod = mods | SDL_KMOD_NUM;
		return 1;
	}
	if (strcmp(word[0], "move") == 0 && n == 3 && point(word[1], word[2], &x, &y)) {
		windowpoint(x, y, &wx, &wy);
		e->type = SDL_EVENT_MOUSE_MOTION;
		e->motion.windowID = SDL_GetWindowID(syswindow);
		e->motion.x = wx;
		e->motion.y = wy;
		return 1;
	}
	if (strcmp(word[0], "button") == 0 && n == 5 && point(word[3], word[4], &x, &y)) {
		if (strcmp(word[1], "left") == 0)
			button = SDL_BUTTON_LEFT;
		else if (strcmp(word[1], "right") == 0)
			button = SDL_BUTTON_RIGHT;
		else
			return -1;
		down = updown(word[2]);
		if (down < 0)
			return -1;
		windowpoint(x, y, &wx, &wy);
		e->type = down ? SDL_EVENT_MOUSE_BUTTON_DOWN : SDL_EVENT_MOUSE_BUTTON_UP;
		e->button.down = down;
		e->button.windowID = SDL_GetWindowID(syswindow);
		e->button.button = (Uint8)button;
		e->button.x = wx;
		e->button.y = wy;
		return 1;
	}
	if (strcmp(word[0], "close") == 0 && n == 1) {
		e->type = SDL_EVENT_WINDOW_CLOSE_REQUESTED;
		e->window.windowID = SDL_GetWindowID(syswindow);
		return 1;
	}
	return -1;
}

/*
 * setup hands the setup event s of line, whose words after its name are
 * the n of word, to replaysetup, or stops the replay on a malformed one.
 */
static void
setup(const Setup *s, char *word[], int n, const char *line)
{
	long long v, count, y;

	switch (s->what) {
	case 'b':
		if (n != 2 || strlen(word[0]) != 1 || !number(word[1], s->lo, s->hi, &v))
			sysfatal("replay: bless wants a slot's letter and a number from %d to %d: %s", s->lo, s->hi, line);
		replaysetup('b', (unsigned char)word[0][0], (int)v);
		break;
	case 'a':
		if (n != 2 || !number(word[0], s->lo, s->hi, &v))
			sysfatal("replay: avenger wants a number from %d to %d: %s", s->lo, s->hi, line);
		if (!number(word[1], 1, Nunits, &count))
			sysfatal("replay: avenger wants a count from 1 to %d: %s", Nunits, line);
		replaysetup('a', (int)v, (int)count);
		break;
	case 'd':
		if (n != 2 || !number(word[0], s->lo, s->hi, &v) || !number(word[1], 0, Mapsize - 1, &y))
			sysfatal("replay: door wants an x from %d to %d and a y from 0 to %d: %s", s->lo, s->hi, Mapsize - 1, line);
		replaysetup('d', (int)v, (int)y);
		break;
	default:
		if (n != 1 || !number(word[0], s->lo, s->hi, &v))
			sysfatal("replay: %s wants a number from %d to %d: %s", s->name, s->lo, s->hi, line);
		replaysetup(s->what, (int)v, 0);
		break;
	}
}

/* replayevent fills e with the script's next event whose time has come, and reports whether there was one. */
int
replayevent(SDL_Event *e)
{
	char line[Maxline], words[Maxline], *word[Maxwords], *end;
	long long t, v, w;
	size_t n, i;
	int nw;

	for (;;) {
		if (cursor == NULL || *cursor == '\0')
			return 0;
		end = strchr(cursor, '\n');
		n = end != NULL ? (size_t)(end - cursor) : strlen(cursor);
		if (cursor[0] == '#' || strspn(cursor, " \t\r") == n) {
			cursor += n + (end != NULL);
			continue;
		}
		if (n >= sizeof line)
			sysfatal("replay: line too long");
		memcpy(line, cursor, n);
		line[n] = '\0';
		memcpy(words, line, n + 1);
		nw = split(words, word, Maxwords);
		if (nw < 2 || !number(word[0], 0, (long long)UINT32_MAX - Clockbase, &t))
			sysfatal("replay: a line wants a time from 0 to %lu ms and an event: %s",
				(unsigned long)(UINT32_MAX - Clockbase), line);
		if (Clockbase + t > vclock)
			return 0;
		if (sysrenderer == NULL && strcmp(word[1], "close") != 0)
			return 0;
		cursor += n + (end != NULL);
		if (strcmp(word[1], "flag") == 0) {
			if (nw != 4 || !number(word[2], 0, Nflags - 1, &v) || !number(word[3], 0, 1, &w))
				sysfatal("replay: flag wants a flag from 0 to %d and 0 or 1: %s", Nflags - 1, line);
			setflag((int)v, (int)w);
			continue;
		}
		if (strcmp(word[1], "full") == 0) {
			if (nw != 3 || !number(word[2], -1, Maxroom, &v))
				sysfatal("replay: full wants a count of bytes up to %d, or -1: %s", Maxroom, line);
			sysfull((long)v);
			continue;
		}
		if (strcmp(word[1], "inplace") == 0) {
			if (nw != 3 || !number(word[2], 0, 1, &v))
				sysfatal("replay: inplace wants 1 or 0: %s", line);
			sysinplace((int)v);
			continue;
		}
		for (i = 0; i < sizeof setups / sizeof setups[0]; i++)
			if (strcmp(word[1], setups[i].name) == 0)
				break;
		if (i < sizeof setups / sizeof setups[0]) {
			setup(&setups[i], word + 2, nw - 2, line);
			continue;
		}
		if (parse(word + 1, nw - 1, e) < 0)
			sysfatal("replay: bad line: %s", line);
		return 1;
	}
}

/* replayrecord writes an event of the keyboard or the mouse into the record. */
void
replayrecord(const SDL_Event *e)
{
	unsigned long t;
	float x, y;
	char name[32], *s;
	const char *key;

	if (record == NULL)
		return;
	t = (unsigned long)(vclock - Clockbase);
	switch (e->type) {
	case SDL_EVENT_QUIT:
	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
		SDL_IOprintf(record, "%lu close\n", t);
		break;
	case SDL_EVENT_KEY_DOWN:
	case SDL_EVENT_KEY_UP:
		if (e->key.repeat)
			break;
		/* a key that SDL leaves unnamed, or names as another, would replay as no key or as the other */
		key = SDL_GetScancodeName(e->key.scancode);
		if (key[0] == '\0' || SDL_GetScancodeFromName(key) != e->key.scancode)
			break;
		SDL_strlcpy(name, key, sizeof name);
		for (s = name; *s != '\0'; s++)
			if (*s == ' ')
				*s = '_';
		SDL_IOprintf(record, "%lu key %s %s\n", t, e->key.down ? "down" : "up", name);
		break;
	case SDL_EVENT_MOUSE_MOTION:
		x = e->motion.x;
		y = e->motion.y;
		if (sysrenderer != NULL)
			SDL_RenderCoordinatesFromWindow(sysrenderer, x, y, &x, &y);
		SDL_IOprintf(record, "%lu move %d %d\n", t, (int)x - sysframex, (int)y);
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	case SDL_EVENT_MOUSE_BUTTON_UP:
		x = e->button.x;
		y = e->button.y;
		if (sysrenderer != NULL)
			SDL_RenderCoordinatesFromWindow(sysrenderer, x, y, &x, &y);
		SDL_IOprintf(record, "%lu button %s %s %d %d\n", t,
			e->button.button == SDL_BUTTON_RIGHT ? "right" : "left",
			e->button.down ? "down" : "up", (int)x - sysframex, (int)y);
		break;
	default:
		break;
	}
}

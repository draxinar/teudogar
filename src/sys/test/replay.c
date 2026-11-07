/*
 * The scripts of src/sys/replay.c: replayevent skips a comment or a blank
 * line of any length, takes a key by any of SDL's names, hands each
 * setup event its numbers through replaysetup, whose stub keeps them, and
 * reads a line full as the room of the disk, which cuts a write of the
 * per-user folder short, and a line inplace as writes into the files
 * themselves, without an event. It stops the replay at a number past its
 * event's range or a word past its event's, on every host alike, as
 * fails of stub.c sees. The chords of a script reach the queue
 * of src/sys/win.c as Windows posts them, a key with Alt as a system key
 * but one with Ctrl and Alt, as AltGr's, as a plain key, and accelerator
 * table 20 takes a key with Alt alone. Outside a replay, ToAscii gives
 * the characters of the numeric pad, whose keys SDL gives codes of their
 * own. A recording replays as its keys that SDL names as their own. The
 * test writes its scripts, and that folder, into the folder that its
 * argument names, and prints each check that fails.
 */

#include <stdio.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <windows.h>
#include <ddraw.h>

#include "sys.h"
#include "test.h"

enum {
	Maxskipped = 4096,
	Maxchord = 3	/* the keys of a chord */
};

/* the lines that replayevent skips */
static const struct {
	char first;	/* the line's first character */
	char rest;	/* the character of the rest of it */
	int n;	/* its length */
} skipped[] = {
	{ '#', 'x', 1 },
	{ '#', 'x', 255 },
	{ '#', 'x', 256 },
	{ '#', 'x', Maxskipped },
	{ ' ', ' ', 256 },
};

/* the names of keys, with SDL's longest */
static const struct {
	const char *name;
	SDL_Scancode sc;
} named[] = {
	{ "A", SDL_SCANCODE_A },
	{ "Left_Shift", SDL_SCANCODE_LSHIFT },
	{ "Keypad_Backspace", SDL_SCANCODE_KP_BACKSPACE },
	{ "MediaFastForward", SDL_SCANCODE_MEDIA_FAST_FORWARD },
	{ "MediaTrackPrevious", SDL_SCANCODE_MEDIA_PREVIOUS_TRACK },
};

/* the setup events, with what replaysetup takes from each */
static const struct {
	const char *line;
	int what, a, b;
} setups[] = {
	{ "0 bless W 3\n", 'b', 'W', 3 },
	{ "0 health 30\n", 'h', 30, 0 },
	{ "0 hunger 5040\n", 'u', 5040, 0 },
	{ "0 fear 40\n", 'f', 40, 0 },
	{ "0 avenger 9971 2\n", 'a', 9971, 2 },
	{ "0 summon 3800\n", 's', 3800, 0 },
	{ "0 door 137 0\n", 'd', 137, 0 },
	{ "0 door 1 149\n", 'd', 1, 149 },	/* the left half in the first column, on the last row */
};

/*
 * the chords of a script, the keys held and then the key pressed, with
 * the messages of the last key's down and up and whether accelerator
 * table 20 takes its down, as Windows sends and takes them
 */
static const struct {
	const char *keys[Maxchord];
	UINT down, up;
	int accel;
} chords[] = {
	{ { "Left_Alt", "Q" }, WM_SYSKEYDOWN, WM_SYSKEYUP, 1 },
	{ { "Right_Alt", "P" }, WM_SYSKEYDOWN, WM_SYSKEYUP, 1 },
	{ { "Left_Shift", "Left_Alt", "Q" }, WM_SYSKEYDOWN, WM_SYSKEYUP, 0 },
	{ { "Left_Ctrl", "Left_Alt", "Q" }, WM_KEYDOWN, WM_KEYUP, 0 },
	{ { "Left_Alt", "Left_Ctrl", "Q" }, WM_KEYDOWN, WM_KEYUP, 0 },
	{ { "Left_Ctrl", "Right_Alt", "Q" }, WM_KEYDOWN, WM_KEYUP, 0 },	/* AltGr, as SDL gives it on Windows */
	{ { "Left_Ctrl", "F10" }, WM_SYSKEYDOWN, WM_SYSKEYUP, 0 },
	{ { "Q" }, WM_KEYDOWN, WM_KEYUP, 0 },
};

/*
 * the keypad's keys, which SDL names apart from the characters, with the
 * virtual key that ToAscii takes and the character that it gives outside
 * a replay, or 0
 */
static const struct {
	const char *key;
	UINT vk;
	int c;
} pad[] = {
	{ "Keypad_1", VK_NUMPAD1, '1' },
	{ "Keypad_0", VK_NUMPAD0, '0' },
	{ "Keypad_.", VK_DECIMAL, '.' },
	{ "Keypad_/", VK_DIVIDE, '/' },
	{ "Keypad_1", VK_END, 0 },	/* with NumLock off */
};

/* the lines that stop a replay, on every host alike */
static const char *const refused[] = {
	"0 full 3000000000\n",	/* past the room of a long of 32 bits */
	"0 full 5 6\n",
	"0 flag 4294967797 1\n",	/* flag 501 in a long of 64 bits */
	"0 flag 500 2\n",
	"4274967296 close\n",	/* past the virtual clock's last */
	"-1 close\n",
	"0 close now\n",
	"0 inplace 1 0\n",
	"0 health 30 40\n",
	"0 avenger 9971 2 3\n",
	"0 door 0 0\n",	/* the left half off the map */
	"0 door 150 0\n",
	"0 door 137 150\n",
	"0 door 137\n",
	"0 key down Return now\n",
	"0 key dwn Return\n",
	"0 move 10 10 10\n",
	"0 move 10 32768\n",
	"0 button left dn 10 10\n",
	"0 button middle down 10 10\n",
	"0 button middle dn 10 10\n",
};

/* the keys of a recording: SDL leaves the first unnamed and names the second Return, the name of RETURN */
static const SDL_Scancode recorded[] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_RETURN2, SDL_SCANCODE_A };

/* the events that a replay gives */
typedef struct Taken Taken;
struct Taken {
	SDL_Event e[8];
	int n;
};

extern int setupwhat, setupa, setupb;	/* stub.c */

static char path[Maxpath];	/* the file of the script */

/* start begins a replay of the script text. */
static void
start(const char *text)
{
	SDL_IOStream *f;

	f = SDL_IOFromFile(path, "w");
	if (f == NULL || SDL_WriteIO(f, text, strlen(text)) != strlen(text) || !SDL_CloseIO(f))
		sysfatal("replay: %s: %s", path, SDL_GetError());
	SDL_setenv_unsafe("TEUDOGAR_REPLAY", path, 1);
	replayinit();
}

/* stop ends the replay that start began, whatever of the script is left. */
static void
stop(void)
{
	SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL);
	replayquit();
	SDL_ResetLogPriorities();
	replaying = 0;
	sysframepoints = 0;
	SDL_unsetenv_unsafe("TEUDOGAR_REPLAY");
	SDL_RemovePath(path);
}

/* drain keeps the events of the replay that start began in the Taken at arg, as many as it holds. */
static void
drain(void *arg)
{
	Taken *t;
	SDL_Event e;

	t = arg;
	while (replayevent(&e))
		if (t->n < (int)(sizeof t->e / sizeof t->e[0]))
			t->e[t->n++] = e;
}

/*
 * checkrecord records a down and then an up of each key of recorded into
 * a file of the folder dir, and wants the record to replay as the keys
 * that SDL names as their own: A down and up.
 */
static void
checkrecord(const char *dir)
{
	char rec[Maxpath], *text;
	SDL_Event e;
	Taken taken;
	size_t i;
	int down;

	SDL_snprintf(rec, sizeof rec, "%s/record.txt", dir);
	SDL_setenv_unsafe("TEUDOGAR_RECORD", rec, 1);
	replayinit();
	for (down = 1; down >= 0; down--)
		for (i = 0; i < sizeof recorded / sizeof recorded[0]; i++) {
			memset(&e, 0, sizeof e);
			e.type = down ? SDL_EVENT_KEY_DOWN : SDL_EVENT_KEY_UP;
			e.key.down = down;
			e.key.scancode = recorded[i];
			replayrecord(&e);
		}
	replayquit();
	recording = 0;
	replaying = 0;
	SDL_unsetenv_unsafe("TEUDOGAR_RECORD");
	text = SDL_LoadFile(rec, NULL);
	if (text == NULL)
		sysfatal("replay: %s: %s", rec, SDL_GetError());
	start(text);
	memset(&taken, 0, sizeof taken);
	check(!fails(drain, &taken) && taken.n == 2 &&
		taken.e[0].type == SDL_EVENT_KEY_DOWN && taken.e[0].key.scancode == SDL_SCANCODE_A &&
		taken.e[1].type == SDL_EVENT_KEY_UP && taken.e[1].key.scancode == SDL_SCANCODE_A,
		"a recording replays as the keys that SDL names as their own, A down and up");
	stop();
	SDL_free(text);
	SDL_RemovePath(rec);
}

/* checkchords presses each chord of chords through the message queue, and lets its keys go from the last. */
static void
checkchords(void)
{
	char script[256], line[64], what[256];
	HACCEL table;
	UINT down, up;
	MSG m;
	size_t i;
	int n, k, downs, accel;

	table = LoadAcceleratorsA(NULL, MAKEINTRESOURCE(20));
	for (i = 0; i < sizeof chords / sizeof chords[0]; i++) {
		script[0] = '\0';
		for (n = 0; n < Maxchord && chords[i].keys[n] != NULL; n++) {
			SDL_snprintf(line, sizeof line, "0 key down %s\n", chords[i].keys[n]);
			SDL_strlcat(script, line, sizeof script);
		}
		for (k = n - 1; k >= 0; k--) {
			SDL_snprintf(line, sizeof line, "0 key up %s\n", chords[i].keys[k]);
			SDL_strlcat(script, line, sizeof script);
		}
		start(script);
		downs = 0;
		down = up = 0;
		accel = -1;
		while (PeekMessageA(&m, NULL, 0, 0, PM_REMOVE)) {
			if (m.message == WM_KEYDOWN || m.message == WM_SYSKEYDOWN) {
				if (++downs == n) {
					down = m.message;
					accel = TranslateAcceleratorA(m.hwnd, table, &m);
				}
			} else if (up == 0)
				up = m.message;
		}
		stop();
		SDL_snprintf(what, sizeof what, "chord %d sends %#x, then %#x, and the accelerators take it: %d, want %#x, %#x, and %d",
			(int)i, down, up, accel, chords[i].down, chords[i].up, chords[i].accel);
		check(down == chords[i].down && up == chords[i].up && accel == chords[i].accel, what);
	}
}

/* checkpad presses each key of pad in a script, and asks ToAscii for its character outside a replay, as a player types it. */
static void
checkpad(void)
{
	char script[64], what[128];
	BYTE state[256];
	WORD c;
	MSG m;
	size_t i;
	int got;

	for (i = 0; i < sizeof pad / sizeof pad[0]; i++) {
		SDL_snprintf(script, sizeof script, "0 key down %s\n0 key up %s\n", pad[i].key, pad[i].key);
		start(script);
		got = -1;
		while (PeekMessageA(&m, NULL, 0, 0, PM_REMOVE))
			if (m.message == WM_KEYDOWN) {
				GetKeyboardState(state);
				replaying = 0;
				got = ToAscii(pad[i].vk, (UINT)(m.lParam >> 16 & 0xff), state, &c, 0) == 1 ? c : 0;
				replaying = 1;
			}
		stop();
		SDL_snprintf(what, sizeof what, "%s as the virtual key %#x types %d, want %d", pad[i].key, pad[i].vk, got, pad[i].c);
		check(got == pad[i].c, what);
	}
}

int
main(int argc, char *argv[])
{
	static char text[Maxskipped + 64];
	char what[128];
	SDL_Event e;
	Taken taken;
	HANDLE h;
	DWORD put;
	SDL_PathInfo info;
	SDL_IOStream *io;
	char fullpath[Maxpath];
	size_t i;
	int n;

	if (argc != 2) {
		fprintf(stderr, "usage: replay dir\n");
		return 2;
	}
	SDL_snprintf(path, sizeof path, "%s/replay.txt", argv[1]);
	for (i = 0; i < sizeof skipped / sizeof skipped[0]; i++) {
		n = skipped[i].n;
		memset(text, skipped[i].rest, n);
		text[0] = skipped[i].first;
		strcpy(text + n, "\n0 close\n");
		start(text);
		SDL_snprintf(what, sizeof what, "a line of %d bytes from '%c' hides the close after it", n, skipped[i].first);
		check(replayevent(&e) && e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED, what);
		stop();
	}
	/* a stand-in, since replayevent holds back all but a close until the window exists */
	sysrenderer = (SDL_Renderer *)text;
	for (i = 0; i < sizeof named / sizeof named[0]; i++) {
		SDL_snprintf(text, sizeof text, "0 key down %s\n", named[i].name);
		start(text);
		SDL_snprintf(what, sizeof what, "key %s gives no key down of scancode %d", named[i].name, (int)named[i].sc);
		check(replayevent(&e) && e.type == SDL_EVENT_KEY_DOWN && e.key.scancode == named[i].sc, what);
		stop();
	}
	for (i = 0; i < sizeof setups / sizeof setups[0]; i++) {
		setupwhat = setupa = setupb = -1;
		start(setups[i].line);
		SDL_snprintf(what, sizeof what, "%.*s takes %c %d %d", (int)strlen(setups[i].line) - 1, setups[i].line,
			setups[i].what, setups[i].a, setups[i].b);
		check(!replayevent(&e) && setupwhat == setups[i].what && setupa == setups[i].a && setupb == setups[i].b, what);
		stop();
	}
	checkchords();
	checkpad();
	for (i = 0; i < sizeof refused / sizeof refused[0]; i++) {
		memset(&taken, 0, sizeof taken);
		start(refused[i]);
		SDL_snprintf(what, sizeof what, "%.*s stops the replay", (int)strlen(refused[i]) - 1, refused[i]);
		check(fails(drain, &taken), what);
		stop();
	}
	sysfull(-1);
	sysinplace(0);
	memset(&taken, 0, sizeof taken);
	start("4274967295 close\n");
	check(!fails(drain, &taken) && taken.n == 0, "a close at the virtual clock's last waits for it");
	stop();
	checkrecord(argv[1]);
	SDL_snprintf(sysuserdir, sizeof sysuserdir, "%s/user/", argv[1]);
	if (!SDL_CreateDirectory(sysuserdir))
		sysfatal("replay: %s: %s", sysuserdir, SDL_GetError());
	SDL_snprintf(fullpath, sizeof fullpath, "%sfull.dat", sysuserdir);
	start("0 full 5\n0 close\n");
	check(replayevent(&e) && e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED, "full 5 gives no event, and the close after it comes");
	h = CreateFileA("C:\\teudogar.sav\\full.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	check(h != INVALID_HANDLE_VALUE && !WriteFile(h, "abcdef", 6, &put, NULL) && put == 5 && GetLastError() == ERROR_DISK_FULL,
		"full 5 lets a write of 6 bytes put 5 and fail with the disk full");
	if (h != INVALID_HANDLE_VALUE) {
		SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL);
		CloseHandle(h);
		SDL_ResetLogPriorities();
	}
	sysfull(-1);
	stop();
	start("0 full 0000000000000001000\n0 close\n");
	check(replayevent(&e) && e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED,
		"full 0000000000000001000 gives no event, and the close after it comes");
	h = CreateFileA("C:\\teudogar.sav\\full.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	check(h != INVALID_HANDLE_VALUE && WriteFile(h, "abcdef", 6, &put, NULL) && put == 6,
		"full 0000000000000001000 lets a write of 6 bytes put them all");
	if (h != INVALID_HANDLE_VALUE)
		CloseHandle(h);
	sysfull(-1);
	stop();
	start("0 inplace 1\n0 close\n");
	check(replayevent(&e) && e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED, "inplace 1 gives no event, and the close after it comes");
	io = SDL_IOFromFile(fullpath, "wb");
	if (io == NULL || SDL_WriteIO(io, "old", 3) != 3 || !SDL_CloseIO(io))
		sysfatal("replay: %s: %s", fullpath, SDL_GetError());
	h = CreateFileA("C:\\teudogar.sav\\full.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	check(h != INVALID_HANDLE_VALUE && SDL_GetPathInfo(fullpath, &info) && info.size == 0,
		"inplace 1 empties the file itself at its open");
	if (h != INVALID_HANDLE_VALUE)
		CloseHandle(h);
	SDL_RemovePath(fullpath);
	sysinplace(0);
	stop();
	sysrenderer = NULL;
	return failed;
}

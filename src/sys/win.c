/*
 * USER32 for the port: the game's window on SDL3, its message queue,
 * the keyboard and the mouse, accelerators, message boxes, and the
 * error box of DIALOG 40.
 *
 * The queue holds the window's messages and the thread's (posted with
 * a NULL window). GetAsyncKeyState, GetKeyboardState, and GetCursorPos
 * answer with the state as of the last message the game retrieved, as
 * pollinput expects when it tests whether the key of a message is still
 * down. Keys go out with their PC scan codes of set 1 in lParam, by
 * position, which the game's logic reads. Letters take their virtual
 * keys from the keyboard's layout, as on Windows, and the other keys
 * those of a German keyboard's positions, the author's.
 */

#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <windows.h>

#include "sys.h"

enum {
	Wndmagic = 0x57696e64,
	Maxmsgs = 1024,
	Htclient = 1,
	Bnclicked = 0,
	Maxtext = 2048,
	Borderw = 8,	/* what a window's borders may add to its size */
	Borderh = 40
};

/* the controls of DIALOG 40 that errproc sets, and their captions in the resource */
enum {
	Cabort = 3,
	Creadme = 0x32,
	Cwebsite = 0x33,
	Ctext = 0x3d,
	Ctitle = 0x3e,
	Ncontrols = 5
};

typedef struct Control Control;
struct Control {
	uint32_t magic;
	int id;
	int visible;
	char text[Maxtext];
	struct Window *dialog;
};

typedef struct Window Window;
struct Window {
	uint32_t magic;
	WNDPROC proc;
	DLGPROC dlgproc;
	int dialog;
	int ended;
	INT_PTR result;
	char title[256];
	Control controls[Ncontrols];
};

SDL_Window *syswindow;
SDL_Renderer *sysrenderer;
int sysroam;

static Window mainwin;
static WNDPROC classproc;
static int created;
static int destroyed;
static int iconic;
static int showcount;

static MSG queue[Maxmsgs];
static int qhead, qn;
static int quitposted, quitcode;

static BYTE keystate[256];	/* as of the last message retrieved */
static POINT cursor;	/* in the game's coordinates, as of the last message */
static POINT livecursor;	/* the latest position the events gave */
static int movequeued;	/* the index in queue of a pending WM_SETCURSOR, or -1 */
static SDL_Scancode lastsc[256];	/* the SDL scancode of each set-1 scan code */

static RECT cliprect = { 0, 0, Gamew, Gameh };	/* the rectangle of ClipCursor */
static int unclipped;	/* whether ClipCursor lifted it */

/*
 * the flags of the window: on macOS it takes the display's own pixels,
 * two to a point on a Retina display, which SDL's render conversions
 * count wherever the game's points meet the window's
 */
#ifdef __APPLE__
static const SDL_WindowFlags winflags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
#else
static const SDL_WindowFlags winflags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
#endif

static void makerenderer(void);
static void followoutput(void);
static void setmouserect(void);
static void setarrow(float x, float y);

/* The keys: SDL's scancode, the set-1 scan code, whether it is extended, and the virtual key of a German keyboard. */
static const struct {
	SDL_Scancode sc;
	BYTE scan;
	BYTE ext;
	BYTE vk;
} keys[] = {
	{ SDL_SCANCODE_ESCAPE, 0x01, 0, VK_ESCAPE },
	{ SDL_SCANCODE_1, 0x02, 0, '1' },
	{ SDL_SCANCODE_2, 0x03, 0, '2' },
	{ SDL_SCANCODE_3, 0x04, 0, '3' },
	{ SDL_SCANCODE_4, 0x05, 0, '4' },
	{ SDL_SCANCODE_5, 0x06, 0, '5' },
	{ SDL_SCANCODE_6, 0x07, 0, '6' },
	{ SDL_SCANCODE_7, 0x08, 0, '7' },
	{ SDL_SCANCODE_8, 0x09, 0, '8' },
	{ SDL_SCANCODE_9, 0x0a, 0, '9' },
	{ SDL_SCANCODE_0, 0x0b, 0, '0' },
	{ SDL_SCANCODE_MINUS, 0x0c, 0, VK_OEM_4 },
	{ SDL_SCANCODE_EQUALS, 0x0d, 0, VK_OEM_6 },
	{ SDL_SCANCODE_BACKSPACE, 0x0e, 0, VK_BACK },
	{ SDL_SCANCODE_TAB, 0x0f, 0, VK_TAB },
	{ SDL_SCANCODE_Q, 0x10, 0, 'Q' },
	{ SDL_SCANCODE_W, 0x11, 0, 'W' },
	{ SDL_SCANCODE_E, 0x12, 0, 'E' },
	{ SDL_SCANCODE_R, 0x13, 0, 'R' },
	{ SDL_SCANCODE_T, 0x14, 0, 'T' },
	{ SDL_SCANCODE_Y, 0x15, 0, 'Y' },
	{ SDL_SCANCODE_U, 0x16, 0, 'U' },
	{ SDL_SCANCODE_I, 0x17, 0, 'I' },
	{ SDL_SCANCODE_O, 0x18, 0, 'O' },
	{ SDL_SCANCODE_P, 0x19, 0, 'P' },
	{ SDL_SCANCODE_LEFTBRACKET, 0x1a, 0, VK_OEM_1 },
	{ SDL_SCANCODE_RIGHTBRACKET, 0x1b, 0, VK_OEM_PLUS },
	{ SDL_SCANCODE_RETURN, 0x1c, 0, VK_RETURN },
	{ SDL_SCANCODE_LCTRL, 0x1d, 0, VK_CONTROL },
	{ SDL_SCANCODE_A, 0x1e, 0, 'A' },
	{ SDL_SCANCODE_S, 0x1f, 0, 'S' },
	{ SDL_SCANCODE_D, 0x20, 0, 'D' },
	{ SDL_SCANCODE_F, 0x21, 0, 'F' },
	{ SDL_SCANCODE_G, 0x22, 0, 'G' },
	{ SDL_SCANCODE_H, 0x23, 0, 'H' },
	{ SDL_SCANCODE_J, 0x24, 0, 'J' },
	{ SDL_SCANCODE_K, 0x25, 0, 'K' },
	{ SDL_SCANCODE_L, 0x26, 0, 'L' },
	{ SDL_SCANCODE_SEMICOLON, 0x27, 0, VK_OEM_3 },
	{ SDL_SCANCODE_APOSTROPHE, 0x28, 0, VK_OEM_7 },
	{ SDL_SCANCODE_GRAVE, 0x29, 0, VK_OEM_5 },
	{ SDL_SCANCODE_LSHIFT, 0x2a, 0, VK_SHIFT },
	{ SDL_SCANCODE_BACKSLASH, 0x2b, 0, VK_OEM_2 },
	{ SDL_SCANCODE_NONUSHASH, 0x2b, 0, VK_OEM_2 },
	{ SDL_SCANCODE_Z, 0x2c, 0, 'Z' },
	{ SDL_SCANCODE_X, 0x2d, 0, 'X' },
	{ SDL_SCANCODE_C, 0x2e, 0, 'C' },
	{ SDL_SCANCODE_V, 0x2f, 0, 'V' },
	{ SDL_SCANCODE_B, 0x30, 0, 'B' },
	{ SDL_SCANCODE_N, 0x31, 0, 'N' },
	{ SDL_SCANCODE_M, 0x32, 0, 'M' },
	{ SDL_SCANCODE_COMMA, 0x33, 0, VK_OEM_COMMA },
	{ SDL_SCANCODE_PERIOD, 0x34, 0, VK_OEM_PERIOD },
	{ SDL_SCANCODE_SLASH, 0x35, 0, VK_OEM_MINUS },
	{ SDL_SCANCODE_RSHIFT, 0x36, 0, VK_SHIFT },
	{ SDL_SCANCODE_KP_MULTIPLY, 0x37, 0, VK_MULTIPLY },
	{ SDL_SCANCODE_LALT, 0x38, 0, VK_MENU },
	{ SDL_SCANCODE_SPACE, 0x39, 0, VK_SPACE },
	{ SDL_SCANCODE_CAPSLOCK, 0x3a, 0, VK_CAPITAL },
	{ SDL_SCANCODE_F1, 0x3b, 0, VK_F1 },
	{ SDL_SCANCODE_F2, 0x3c, 0, VK_F2 },
	{ SDL_SCANCODE_F3, 0x3d, 0, VK_F3 },
	{ SDL_SCANCODE_F4, 0x3e, 0, VK_F4 },
	{ SDL_SCANCODE_F5, 0x3f, 0, VK_F5 },
	{ SDL_SCANCODE_F6, 0x40, 0, VK_F6 },
	{ SDL_SCANCODE_F7, 0x41, 0, VK_F7 },
	{ SDL_SCANCODE_F8, 0x42, 0, VK_F8 },
	{ SDL_SCANCODE_F9, 0x43, 0, VK_F9 },
	{ SDL_SCANCODE_F10, 0x44, 0, VK_F10 },
	{ SDL_SCANCODE_NUMLOCKCLEAR, 0x45, 0, VK_NUMLOCK },
	{ SDL_SCANCODE_SCROLLLOCK, 0x46, 0, VK_SCROLL },
	{ SDL_SCANCODE_KP_7, 0x47, 0, VK_NUMPAD7 },
	{ SDL_SCANCODE_KP_8, 0x48, 0, VK_NUMPAD8 },
	{ SDL_SCANCODE_KP_9, 0x49, 0, VK_NUMPAD9 },
	{ SDL_SCANCODE_KP_MINUS, 0x4a, 0, VK_SUBTRACT },
	{ SDL_SCANCODE_KP_4, 0x4b, 0, VK_NUMPAD4 },
	{ SDL_SCANCODE_KP_5, 0x4c, 0, VK_NUMPAD5 },
	{ SDL_SCANCODE_KP_6, 0x4d, 0, VK_NUMPAD6 },
	{ SDL_SCANCODE_KP_PLUS, 0x4e, 0, VK_ADD },
	{ SDL_SCANCODE_KP_1, 0x4f, 0, VK_NUMPAD1 },
	{ SDL_SCANCODE_KP_2, 0x50, 0, VK_NUMPAD2 },
	{ SDL_SCANCODE_KP_3, 0x51, 0, VK_NUMPAD3 },
	{ SDL_SCANCODE_KP_0, 0x52, 0, VK_NUMPAD0 },
	{ SDL_SCANCODE_KP_PERIOD, 0x53, 0, VK_DECIMAL },
	{ SDL_SCANCODE_NONUSBACKSLASH, 0x56, 0, VK_OEM_102 },
	{ SDL_SCANCODE_F11, 0x57, 0, VK_F11 },
	{ SDL_SCANCODE_F12, 0x58, 0, VK_F12 },
	{ SDL_SCANCODE_KP_ENTER, 0x1c, 1, VK_RETURN },
	{ SDL_SCANCODE_RCTRL, 0x1d, 1, VK_CONTROL },
	{ SDL_SCANCODE_KP_DIVIDE, 0x35, 1, VK_DIVIDE },
	{ SDL_SCANCODE_PRINTSCREEN, 0x37, 1, VK_SNAPSHOT },
	{ SDL_SCANCODE_RALT, 0x38, 1, VK_MENU },
	{ SDL_SCANCODE_PAUSE, 0x45, 1, VK_PAUSE },
	{ SDL_SCANCODE_HOME, 0x47, 1, VK_HOME },
	{ SDL_SCANCODE_UP, 0x48, 1, VK_UP },
	{ SDL_SCANCODE_PAGEUP, 0x49, 1, VK_PRIOR },
	{ SDL_SCANCODE_LEFT, 0x4b, 1, VK_LEFT },
	{ SDL_SCANCODE_RIGHT, 0x4d, 1, VK_RIGHT },
	{ SDL_SCANCODE_END, 0x4f, 1, VK_END },
	{ SDL_SCANCODE_DOWN, 0x50, 1, VK_DOWN },
	{ SDL_SCANCODE_PAGEDOWN, 0x51, 1, VK_NEXT },
	{ SDL_SCANCODE_INSERT, 0x52, 1, VK_INSERT },
	{ SDL_SCANCODE_DELETE, 0x53, 1, VK_DELETE },
	{ SDL_SCANCODE_LGUI, 0x5b, 1, VK_LWIN },
	{ SDL_SCANCODE_RGUI, 0x5c, 1, VK_RWIN },
	{ SDL_SCANCODE_APPLICATION, 0x5d, 1, VK_APPS },
};

/* the keypad's keys with NumLock off, as on Windows */
static const struct {
	BYTE numvk;
	BYTE vk;
} padkeys[] = {
	{ VK_NUMPAD7, VK_HOME },
	{ VK_NUMPAD8, VK_UP },
	{ VK_NUMPAD9, VK_PRIOR },
	{ VK_NUMPAD4, VK_LEFT },
	{ VK_NUMPAD5, VK_CLEAR },
	{ VK_NUMPAD6, VK_RIGHT },
	{ VK_NUMPAD1, VK_END },
	{ VK_NUMPAD2, VK_DOWN },
	{ VK_NUMPAD3, VK_NEXT },
	{ VK_NUMPAD0, VK_INSERT },
	{ VK_DECIMAL, VK_DELETE },
};

/* accelerator table 20 of teudogar.rc: a key with Alt alone gives a command, as VIRTKEY, ALT asks */
static const struct {
	BYTE vk;
	BYTE mods;	/* the state of Shift, Ctrl, and Alt that the entry wants */
	WORD cmd;
} accels[] = {
	{ 'P', FALT, 32 },
	{ 'Q', FALT, 33 },
	{ 'X', FALT, 33 },
	{ 'Z', FALT, 30 },
	{ 'Y', FALT, 30 },
	{ 'M', FALT, 31 },
};
static int acceltable;	/* the address the accelerators' handle points to */
static int iconhandle;
static int cursorhandle;
static int stockobject;

/* the characters 0x80 to 0x9f of code page 1252, in Unicode */
static const Uint16 cp1252[32] = {
	0x20ac, 0x0081, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021,
	0x02c6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008d, 0x017d, 0x008f,
	0x0090, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
	0x02dc, 0x2122, 0x0161, 0x203a, 0x0153, 0x009d, 0x017e, 0x0178,
};

/*
 * winutf8 returns s, in code page 1252, as UTF-8, which the caller
 * frees with SDL_free. Its line ends, CR LF or the game's lone CR,
 * become LF.
 */
char *
winutf8(const char *s)
{
	size_t n;
	char *out, *p;
	Uint32 c;

	n = strlen(s);
	out = SDL_malloc(3 * n + 1);
	if (out == NULL)
		return SDL_strdup("");
	p = out;
	for (; *s != '\0'; s++) {
		c = (unsigned char)*s;
		if (c == '\r') {
			if (s[1] == '\n')
				continue;
			c = '\n';
		} else if (c >= 0x80 && c < 0xa0)
			c = cp1252[c - 0x80];
		p = SDL_UCS4ToUTF8(c, p);
	}
	*p = '\0';
	return out;
}

/* The characters of a US keyboard's keys, alone and with Shift, for replays. */
static const struct {
	SDL_Scancode sc;
	char c;
	char shifted;
} uskeys[] = {
	{ SDL_SCANCODE_1, '1', '!' }, { SDL_SCANCODE_2, '2', '@' },
	{ SDL_SCANCODE_3, '3', '#' }, { SDL_SCANCODE_4, '4', '$' },
	{ SDL_SCANCODE_5, '5', '%' }, { SDL_SCANCODE_6, '6', '^' },
	{ SDL_SCANCODE_7, '7', '&' }, { SDL_SCANCODE_8, '8', '*' },
	{ SDL_SCANCODE_9, '9', '(' }, { SDL_SCANCODE_0, '0', ')' },
	{ SDL_SCANCODE_SPACE, ' ', ' ' }, { SDL_SCANCODE_MINUS, '-', '_' },
	{ SDL_SCANCODE_EQUALS, '=', '+' }, { SDL_SCANCODE_LEFTBRACKET, '[', '{' },
	{ SDL_SCANCODE_RIGHTBRACKET, ']', '}' }, { SDL_SCANCODE_BACKSLASH, '\\', '|' },
	{ SDL_SCANCODE_NONUSBACKSLASH, '\\', '|' }, { SDL_SCANCODE_SEMICOLON, ';', ':' },
	{ SDL_SCANCODE_APOSTROPHE, '\'', '"' }, { SDL_SCANCODE_GRAVE, '`', '~' },
	{ SDL_SCANCODE_COMMA, ',', '<' }, { SDL_SCANCODE_PERIOD, '.', '>' },
	{ SDL_SCANCODE_SLASH, '/', '?' }, { SDL_SCANCODE_KP_DIVIDE, '/', '/' },
	{ SDL_SCANCODE_KP_MULTIPLY, '*', '*' }, { SDL_SCANCODE_KP_MINUS, '-', '-' },
	{ SDL_SCANCODE_KP_PLUS, '+', '+' }, { SDL_SCANCODE_KP_PERIOD, '.', '.' },
	{ SDL_SCANCODE_KP_1, '1', '1' }, { SDL_SCANCODE_KP_2, '2', '2' },
	{ SDL_SCANCODE_KP_3, '3', '3' }, { SDL_SCANCODE_KP_4, '4', '4' },
	{ SDL_SCANCODE_KP_5, '5', '5' }, { SDL_SCANCODE_KP_6, '6', '6' },
	{ SDL_SCANCODE_KP_7, '7', '7' }, { SDL_SCANCODE_KP_8, '8', '8' },
	{ SDL_SCANCODE_KP_9, '9', '9' }, { SDL_SCANCODE_KP_0, '0', '0' },
};

/* usascii returns the character of key sc under mod on a US keyboard, or SDLK_UNKNOWN. */
static SDL_Keycode
usascii(SDL_Scancode sc, SDL_Keymod mod)
{
	size_t i;
	int shift, caps;

	shift = (mod & SDL_KMOD_SHIFT) != 0;
	caps = (mod & SDL_KMOD_CAPS) != 0;
	if (sc >= SDL_SCANCODE_A && sc <= SDL_SCANCODE_Z)
		return (shift != caps ? 'A' : 'a') + (sc - SDL_SCANCODE_A);
	for (i = 0; i < sizeof uskeys / sizeof uskeys[0]; i++)
		if (uskeys[i].sc == sc)
			return shift ? uskeys[i].shifted : uskeys[i].c;
	return SDLK_UNKNOWN;
}

/* tocp1252 returns the character of code page 1252 for c, or 0. */
int
tocp1252(Uint32 c)
{
	int i;

	if (c < 0x80 || (c >= 0xa0 && c <= 0xff))
		return (int)c;
	for (i = 0; i < 32; i++)
		if (cp1252[i] == c)
			return 0x80 + i;
	return 0;
}

static int
keyindex(SDL_Scancode sc)
{
	size_t i;

	for (i = 0; i < sizeof keys / sizeof keys[0]; i++)
		if (keys[i].sc == sc)
			return (int)i;
	return -1;
}

/*
 * vkof returns the virtual key of key k, with the layout's letters (a US
 * keyboard's in a replay) and the keypad's NumLock.
 */
static int
vkof(int k, SDL_Keymod mod)
{
	SDL_Keycode code;
	size_t i;
	int vk;

	vk = keys[k].vk;
	if (vk >= 'A' && vk <= 'Z') {
		if (replaying)
			code = 'a' + (keys[k].sc - SDL_SCANCODE_A);
		else
			code = SDL_GetKeyFromScancode(keys[k].sc, SDL_KMOD_NONE, false);
		if (code >= 'a' && code <= 'z')
			vk = (int)code - 'a' + 'A';
	}
	if (!(mod & SDL_KMOD_NUM))
		for (i = 0; i < sizeof padkeys / sizeof padkeys[0]; i++)
			if (padkeys[i].numvk == vk)
				return padkeys[i].vk;
	return vk;
}

/* post appends a message to the queue. */
static int
post(HWND w, UINT msg, WPARAM wp, LPARAM lp, POINT pt)
{
	MSG *m;

	if (qn == Maxmsgs)
		return 0;
	m = &queue[(qhead + qn) % Maxmsgs];
	m->hwnd = w;
	m->message = msg;
	m->wParam = wp;
	m->lParam = lp;
	m->time = sysclock();
	m->pt = pt;
	qn++;
	movequeued = -1;
	return 1;
}

/* gamepoint converts the position of a mouse event into the game's coordinates, on the screen. */
static POINT
gamepoint(float x, float y)
{
	POINT p;

	p.x = (LONG)x - sysframex;
	p.y = (LONG)y;
	if (p.x < sysscreen.left)
		p.x = sysscreen.left;
	if (p.x >= sysscreen.right)
		p.x = sysscreen.right - 1;
	if (p.y < 0)
		p.y = 0;
	if (p.y >= Gameh)
		p.y = Gameh - 1;
	return p;
}

static void
keyevent(const SDL_KeyboardEvent *e)
{
	DWORD lp;
	UINT msg;
	int k, vk, alt, sys;

	k = keyindex(e->scancode);
	if (k < 0)
		return;
	vk = vkof(k, e->mod);
	alt = (e->mod & SDL_KMOD_ALT) != 0;
	if (e->down && alt && vk == VK_RETURN) {
		// FEATURE: fullscreen: Alt+Enter switches between the window and the full screen
		if (!e->repeat)
			wintogglefullscreen();
		return;
	}
	if (e->down && alt && vk == 'S') {
		// FEATURE: sharp: Alt+S switches between whole numbers and sharp bilinear
		if (!e->repeat)
			wintogglescale();
		return;
	}
	lastsc[keys[k].scan] = e->scancode;
	lp = 1 | (DWORD)keys[k].scan << 16 | (DWORD)keys[k].ext << 24;
	if (alt)
		lp |= 1u << 29;
	/* Windows sends a key with Alt as a system key, but one with Ctrl too as a plain key, as AltGr's characters */
	sys = (alt && !(e->mod & SDL_KMOD_CTRL)) || vk == VK_F10;
	if (e->down) {
		if (e->repeat)
			lp |= 1u << 30;
		msg = sys ? WM_SYSKEYDOWN : WM_KEYDOWN;
	} else {
		lp |= 3u << 30;
		msg = sys ? WM_SYSKEYUP : WM_KEYUP;
	}
	post(&mainwin, msg, (WPARAM)vk, (LPARAM)(LONG)lp, livecursor);
}

static void
mouseevent(SDL_Event *e)
{
	MSG *m;
	UINT msg;

	if (sysrenderer != NULL)
		SDL_ConvertEventToRenderCoordinates(sysrenderer, e);
	switch (e->type) {
	case SDL_EVENT_MOUSE_MOTION:
		setarrow(e->motion.x, e->motion.y);
		livecursor = gamepoint(e->motion.x, e->motion.y);
		if (movequeued >= 0) {
			m = &queue[movequeued];
			m->pt = livecursor;
			return;
		}
		if (post(&mainwin, WM_SETCURSOR, (WPARAM)&mainwin, MAKELPARAM(Htclient, WM_MOUSEMOVE), livecursor))
			movequeued = (qhead + qn - 1) % Maxmsgs;
		return;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	case SDL_EVENT_MOUSE_BUTTON_UP:
		livecursor = gamepoint(e->button.x, e->button.y);
		if (e->button.button == SDL_BUTTON_LEFT)
			msg = e->button.down ? WM_LBUTTONDOWN : WM_LBUTTONUP;
		else if (e->button.button == SDL_BUTTON_RIGHT)
			msg = e->button.down ? WM_RBUTTONDOWN : WM_RBUTTONUP;
		else
			return;
		post(&mainwin, msg, 0, MAKELPARAM(livecursor.x, livecursor.y), livecursor);
		return;
	default:
		return;
	}
}

/* translate turns an SDL event into the game's messages. */
static void
translate(SDL_Event *e)
{
	POINT none;

	none = livecursor;
	if (e->type != SDL_EVENT_QUIT && e->type != SDL_EVENT_WINDOW_CLOSE_REQUESTED)
		replayrecord(e);
	switch (e->type) {
	case SDL_EVENT_QUIT:
	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
		post(&mainwin, WM_CLOSE, 0, 0, none);
		break;
	case SDL_EVENT_WINDOW_MINIMIZED:
		if (!iconic && !replaying) {
			iconic = 1;
			post(&mainwin, WM_ACTIVATEAPP, FALSE, 0, none);
		}
		break;
	case SDL_EVENT_WINDOW_RESTORED:
		if (iconic) {
			iconic = 0;
			post(&mainwin, WM_ACTIVATEAPP, TRUE, 0, none);
		}
		break;
	case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
		followoutput();
		setmouserect();
		drawexposed();
		break;
	case SDL_EVENT_WINDOW_EXPOSED:
		drawexposed();
		break;
	case SDL_EVENT_RENDER_DEVICE_RESET:
		drawreset();
		break;
	case SDL_EVENT_RENDER_DEVICE_LOST:
		/* drop the texture before the renderer frees it, since a new one may take its address */
		drawreset();
		SDL_DestroyRenderer(sysrenderer);
		makerenderer();
		setmouserect();
		break;
	case SDL_EVENT_KEY_DOWN:
	case SDL_EVENT_KEY_UP:
		keyevent(&e->key);
		break;
	case SDL_EVENT_MOUSE_MOTION:
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	case SDL_EVENT_MOUSE_BUTTON_UP:
		mouseevent(e);
		break;
	default:
		break;
	}
}

/* isinput reports whether e comes from the keyboard or the mouse. */
static int
isinput(const SDL_Event *e)
{
	switch (e->type) {
	case SDL_EVENT_KEY_DOWN:
	case SDL_EVENT_KEY_UP:
	case SDL_EVENT_MOUSE_MOTION:
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	case SDL_EVENT_MOUSE_BUTTON_UP:
	case SDL_EVENT_MOUSE_WHEEL:
		return 1;
	default:
		return 0;
	}
}

/* isclose reports whether e asks to close the window. */
static int
isclose(const SDL_Event *e)
{
	return e->type == SDL_EVENT_QUIT || e->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED;
}

/*
 * winpump moves SDL's events into the queue, waiting up to wait
 * microseconds for one, delivers the music's finished buffers, and
 * shows the frame.
 */
void
winpump(int wait)
{
	SDL_Event e;
	int got;

	mididrain();
	got = 0;
	while (SDL_PollEvent(&e)) {
		if (replaying && !recording && isinput(&e))
			continue;
		translate(&e);
		/*
		 * In a replay or a recording the clock moves with the script's
		 * events alone, so the host's other events leave it alone.
		 */
		if (!replaying || isinput(&e) || isclose(&e))
			got = 1;
	}
	if (replaying)
		while (replayevent(&e))
			translate(&e), got = 1;
	if (!got && wait > 0 && qn == 0) {
		if (replaying)
			replayadvance(1);
		else
			SDL_DelayNS((Uint64)wait * 1000);
	}
	drawpresent(0);
}

/* take removes the queue's first message into m, applying the state it carries. */
static void
take(MSG *m)
{
	*m = queue[qhead];
	if (movequeued == qhead)
		movequeued = -1;
	qhead = (qhead + 1) % Maxmsgs;
	qn--;
	cursor = m->pt;
	switch (m->message) {
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(keystate[m->wParam & 0xff] & 0x80) && m->wParam == VK_CAPITAL)
			keystate[VK_CAPITAL] ^= 1;
		keystate[m->wParam & 0xff] |= 0x80;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		keystate[m->wParam & 0xff] &= ~0x80;
		break;
	case WM_LBUTTONDOWN:
		keystate[VK_LBUTTON] |= 0x80;
		break;
	case WM_LBUTTONUP:
		keystate[VK_LBUTTON] &= ~0x80;
		break;
	case WM_RBUTTONDOWN:
		keystate[VK_RBUTTON] |= 0x80;
		break;
	case WM_RBUTTONUP:
		keystate[VK_RBUTTON] &= ~0x80;
		break;
	default:
		break;
	}
}

static BOOL
inrange(const MSG *m, HWND w, UINT lo, UINT hi)
{
	if (w != NULL && m->hwnd != w)
		return FALSE;
	if (lo == 0 && hi == 0)
		return TRUE;
	return m->message >= lo && m->message <= hi;
}

BOOL
PeekMessageA(LPMSG m, HWND w, UINT lo, UINT hi, UINT flags)
{
	winpump(500);
	if (qn > 0 && inrange(&queue[qhead], w, lo, hi)) {
		if (flags & PM_REMOVE)
			take(m);
		else
			*m = queue[qhead];
		return TRUE;
	}
	if (qn == 0 && quitposted) {
		memset(m, 0, sizeof *m);
		m->message = WM_QUIT;
		m->wParam = (WPARAM)quitcode;
		if (flags & PM_REMOVE)
			quitposted = 0;
		return TRUE;
	}
	return FALSE;
}

BOOL
GetMessageA(LPMSG m, HWND w, UINT lo, UINT hi)
{
	for (;;) {
		if (PeekMessageA(m, w, lo, hi, PM_REMOVE))
			return m->message != WM_QUIT;
		winpump(2000);
	}
}

BOOL
WaitMessage(void)
{
	while (qn == 0 && !quitposted)
		winpump(2000);
	return TRUE;
}

BOOL
PostMessageA(HWND w, UINT msg, WPARAM wp, LPARAM lp)
{
	return post(w, msg, wp, lp, livecursor);
}

void
PostQuitMessage(int code)
{
	quitposted = 1;
	quitcode = code;
}

BOOL
TranslateMessage(const MSG *m)
{
	WORD c;

	if (m->message != WM_KEYDOWN)
		return FALSE;
	if (ToAscii((UINT)m->wParam, (UINT)(m->lParam >> 16 & 0xff), keystate, &c, 0) == 1)
		post(m->hwnd, WM_CHAR, c & 0xff, m->lParam, m->pt);
	return TRUE;
}

LRESULT
DispatchMessageA(const MSG *m)
{
	Window *w;

	w = m->hwnd;
	if (w == NULL || w->magic != Wndmagic || w->proc == NULL)
		return 0;
	return w->proc(m->hwnd, m->message, m->wParam, m->lParam);
}

LRESULT
SendMessageA(HWND h, UINT msg, WPARAM wp, LPARAM lp)
{
	Window *w;

	w = h;
	if (w == NULL || w->magic != Wndmagic || w->proc == NULL)
		return 0;
	return w->proc(h, msg, wp, lp);
}

LRESULT
DefWindowProcA(HWND w, UINT msg, WPARAM wp, LPARAM lp)
{
	(void)lp;
	switch (msg) {
	case WM_CLOSE:
		DestroyWindow(w);
		return 0;
	case WM_SYSCOMMAND:
		if ((wp & 0xfff0) == SC_CLOSE)
			PostMessageA(w, WM_CLOSE, 0, 0);
		return 0;
	case WM_SYSKEYDOWN:
		if (wp == VK_F4 && (lp & (LPARAM)1 << 29))
			PostMessageA(w, WM_SYSCOMMAND, SC_CLOSE, 0);
		return 0;
	default:
		return 0;
	}
}

ATOM
RegisterClassA(const WNDCLASSA *wc)
{
	classproc = wc->lpfnWndProc;
	return 1;
}

/* windowsize returns the scale of the window, for sharp and integer alike: 2 where twice the screen and the window's borders fit the usable area, else 1. */
static int
windowsize(void)
{
	SDL_Rect r;

	if (!SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &r))
		return 1;
	return r.w >= 2 * sysscreenw + Borderw && r.h >= 2 * Gameh + Borderh ? 2 : 1;
}

#ifdef __APPLE__
/*
 * seticon leaves macOS its icons: the app's, which its Info.plist names,
 * shows in the Dock, where SDL would put the window's of 48 pixels.
 */
static void
seticon(void)
{
}
#else
/* seticon gives the window the game's icon. */
static void
seticon(void)
{
	SDL_Surface *s;

	s = SDL_CreateSurfaceFrom(sysiconw, sysiconh, SDL_PIXELFORMAT_ARGB8888,
		(void *)sysicon, sysiconw * sizeof sysicon[0]);
	if (s == NULL)
		return;
	SDL_SetWindowIcon(syswindow, s);
	SDL_DestroySurface(s);
}
#endif

/* setpresentation shows the screen by whole numbers, or at any size when sharp. */
static void
setpresentation(void)
{
	SDL_SetRenderLogicalPresentation(sysrenderer, sysscreenw, Gameh,
		syssharp ? SDL_LOGICAL_PRESENTATION_LETTERBOX : SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
	drawsetscale();
}

/*
 * followoutput gives the wide screen the width of the output's shape. It
 * runs in the pump, between the frames, and the next frame draws the new
 * width.
 */
static void
followoutput(void)
{
	int w, h;

	if (!syswide || sysfixedwidth || sysrenderer == NULL)
		return;
	if (!SDL_GetWindowSizeInPixels(syswindow, &w, &h))
		return;
	w = screenwidth(w, h, syssharp);
	if (w == sysscreenw)
		return;
	drawsetwidth(w);
	setpresentation();
}

/* makerenderer gives the window its renderer, the software one if no other opens. */
static void
makerenderer(void)
{
	sysrenderer = SDL_CreateRenderer(syswindow, NULL);
	if (sysrenderer == NULL)
		sysrenderer = SDL_CreateRenderer(syswindow, SDL_SOFTWARE_RENDERER);
	if (sysrenderer == NULL)
		sysfatal("renderer: %s", SDL_GetError());
	/* a replay runs on its own clock, and the simulator holds the presents itself */
	SDL_SetRenderVSync(sysrenderer, replaying || sysvblank >= 0 ? 0 : 1);
	setpresentation();
}

/* wininit opens the game's window and its renderer. */
int
wininit(void)
{
	SDL_WindowFlags flags;
	int scale, w, h;

	if (syswindow != NULL)
		return 1;
	scale = windowsize();
	w = syssizew > 0 ? syssizew : scale * sysscreenw;
	h = syssizew > 0 ? syssizeh : scale * Gameh;
	flags = winflags;
	syswindow = SDL_CreateWindow("Teudogar", w, h, flags);
	if (syswindow == NULL)
		sysfatal("window: %s", SDL_GetError());
	SDL_SetWindowMinimumSize(syswindow, Gamew, Gameh);
	seticon();
	makerenderer();
	if (sysfullscreen)
		SDL_SetWindowFullscreen(syswindow, true);
	movequeued = -1;
	return 1;
}

void
wintogglefullscreen(void)
{
	sysfullscreen = !sysfullscreen;
	SDL_SetWindowFullscreen(syswindow, sysfullscreen);
	setmouserect();
	syssetregdword("Fullscreen", sysfullscreen);
	drawexposed();
}

void
wintogglescale(void)
{
	syssharp = !syssharp;
	syssetregdword("Scale", syssharp);
	followoutput();
	setpresentation();
	setmouserect();
	drawexposed();
}

HWND
CreateWindowExA(DWORD exstyle, LPCSTR cls, LPCSTR name, DWORD style, int x, int y, int w, int h,
	HWND parent, HMENU menu, HINSTANCE inst, LPVOID param)
{
	(void)exstyle, (void)cls, (void)style, (void)x, (void)y, (void)w, (void)h;
	(void)parent, (void)menu, (void)inst, (void)param;
	if (created)
		return NULL;
	wininit();
	mainwin.magic = Wndmagic;
	mainwin.proc = classproc;
	SDL_strlcpy(mainwin.title, name, sizeof mainwin.title);
	created = 1;
	return &mainwin;
}

BOOL
DestroyWindow(HWND h)
{
	Window *w;

	w = h;
	if (w == &mainwin && !destroyed) {
		destroyed = 1;
		SendMessageA(h, WM_DESTROY, 0, 0);
		return TRUE;
	}
	return FALSE;
}

BOOL
ShowWindow(HWND h, int show)
{
	Control *c;

	c = h;
	if (c != NULL && c->magic == Wndmagic + 1) {
		c->visible = show != SW_HIDE;
		return TRUE;
	}
	if (h != &mainwin || syswindow == NULL)
		return FALSE;
	if (show == SW_SHOWMINIMIZED || show == SW_MINIMIZE)
		SDL_MinimizeWindow(syswindow);
	else if (show != SW_HIDE) {
		SDL_ShowWindow(syswindow);
		SDL_RaiseWindow(syswindow);
		setmouserect();
	}
	return TRUE;
}

BOOL
UpdateWindow(HWND h)
{
	(void)h;
	drawexposed();
	return TRUE;
}

HWND
SetFocus(HWND h)
{
	return h;
}

HWND
GetForegroundWindow(void)
{
	return &mainwin;
}

BOOL
IsIconic(HWND h)
{
	(void)h;
	return iconic;
}

BOOL
SetWindowTextA(HWND h, LPCSTR s)
{
	Window *w;
	char *u;

	w = h;
	if (w == NULL || w->magic != Wndmagic)
		return FALSE;
	SDL_strlcpy(w->title, s, sizeof w->title);
	if (w == &mainwin && syswindow != NULL) {
		u = winutf8(s);
		SDL_SetWindowTitle(syswindow, u);
		SDL_free(u);
	}
	return TRUE;
}

BOOL
GetWindowRect(HWND h, LPRECT r)
{
	(void)h;
	r->left = 0;
	r->top = 0;
	r->right = Gamew;
	r->bottom = Gameh;
	return TRUE;
}

BOOL
SetWindowPos(HWND h, HWND after, int x, int y, int w, int hh, UINT flags)
{
	(void)h, (void)after, (void)x, (void)y, (void)w, (void)hh, (void)flags;
	return TRUE;
}

int
GetSystemMetrics(int i)
{
	switch (i) {
	case SM_CXSCREEN:
		return Gamew;
	case SM_CYSCREEN:
		return Gameh;
	default:
		return 0;
	}
}

HICON
LoadIconA(HINSTANCE inst, LPCSTR name)
{
	(void)inst, (void)name;
	return &iconhandle;
}

HCURSOR
LoadCursorA(HINSTANCE inst, LPCSTR name)
{
	(void)inst, (void)name;
	return &cursorhandle;
}

HGDIOBJ
GetStockObject(int i)
{
	(void)i;
	return &stockobject;
}

HACCEL
LoadAcceleratorsA(HINSTANCE inst, LPCSTR name)
{
	(void)inst;
	if (name != MAKEINTRESOURCE(20))
		return NULL;
	return &acceltable;
}

/*
 * TranslateAcceleratorA sends the command of a key's down whose entry
 * wants Shift, Ctrl, and Alt as they stand, as Windows matches a VIRTKEY
 * entry, with the keys' state as of the message.
 */
int
TranslateAcceleratorA(HWND w, HACCEL a, LPMSG m)
{
	size_t i;
	int mods;

	if (a != &acceltable || (m->message != WM_KEYDOWN && m->message != WM_SYSKEYDOWN))
		return 0;
	mods = 0;
	if (keystate[VK_SHIFT] & 0x80)
		mods |= FSHIFT;
	if (keystate[VK_CONTROL] & 0x80)
		mods |= FCONTROL;
	if (keystate[VK_MENU] & 0x80)
		mods |= FALT;
	for (i = 0; i < sizeof accels / sizeof accels[0]; i++)
		if (accels[i].vk == m->wParam && accels[i].mods == mods) {
			SendMessageA(w, WM_COMMAND, MAKEWPARAM(accels[i].cmd, 1), 0);
			return 1;
		}
	return 0;
}

SHORT
GetAsyncKeyState(int vk)
{
	return keystate[vk & 0xff] & 0x80 ? (SHORT)0x8000 : 0;
}

BOOL
GetKeyboardState(PBYTE b)
{
	memcpy(b, keystate, sizeof keystate);
	return TRUE;
}

UINT
MapVirtualKeyA(UINT code, UINT type)
{
	size_t i;

	for (i = 0; i < sizeof keys / sizeof keys[0]; i++) {
		if (keys[i].ext)
			continue;
		if (type == 0 && vkof((int)i, SDL_KMOD_NUM) == (int)code)
			return keys[i].scan;
		if (type == 1 && keys[i].scan == code)
			return (UINT)vkof((int)i, SDL_KMOD_NONE);
	}
	return 0;
}

/*
 * ToAscii gives the character of the key with the modifiers of state,
 * in code page 1252, as the keyboard's layout makes it, or the control
 * code of a letter under Control. The numeric pad's virtual keys type a
 * US keyboard's characters.
 */
int
ToAscii(UINT vk, UINT scan, const BYTE *state, LPWORD out, UINT flags)
{
	SDL_Scancode sc;
	SDL_Keycode code;
	SDL_Keymod mod;
	int c;

	(void)flags;
	if ((state[VK_CONTROL] & 0x80) && !(state[VK_MENU] & 0x80)) {
		if (vk >= 'A' && vk <= 'Z') {
			*out = (WORD)(vk - 'A' + 1);
			return 1;
		}
		return 0;
	}
	switch (vk) {
	case VK_RETURN:
		*out = 13;
		return 1;
	case VK_ESCAPE:
		*out = 27;
		return 1;
	case VK_BACK:
		*out = 8;
		return 1;
	case VK_TAB:
		*out = 9;
		return 1;
	default:
		break;
	}
	sc = lastsc[scan & 0xff];
	if (sc == SDL_SCANCODE_UNKNOWN)
		return 0;
	mod = SDL_KMOD_NONE;
	if (state[VK_SHIFT] & 0x80)
		mod |= SDL_KMOD_SHIFT;
	if (state[VK_CAPITAL] & 1)
		mod |= SDL_KMOD_CAPS;
	if (vk >= VK_NUMPAD0 && vk <= VK_DIVIDE)
		mod |= SDL_KMOD_NUM;
	if (replaying)
		code = usascii(sc, mod);
	else {
		code = SDL_GetKeyFromScancode(sc, mod, false);
		/* SDL gives the numeric pad's keys codes of their own, SDLK_KP_1 and kin */
		if ((code & SDLK_SCANCODE_MASK) && (mod & SDL_KMOD_NUM))
			code = usascii(sc, mod);
	}
	if (code == SDLK_UNKNOWN || (code & SDLK_SCANCODE_MASK))
		return 0;
	c = tocp1252((Uint32)code);
	if (c == 0)
		return 0;
	*out = (WORD)c;
	return 1;
}

BOOL
GetCursorPos(LPPOINT p)
{
	*p = cursor;
	return TRUE;
}

BOOL
SetCursorPos(int x, int y)
{
	float wx, wy;

	cursor.x = x;
	cursor.y = y;
	livecursor = cursor;
	if (replaying || sysrenderer == NULL)
		return TRUE;
	if (SDL_RenderCoordinatesToWindow(sysrenderer, x + sysframex + 0.5f, y + 0.5f, &wx, &wy))
		SDL_WarpMouseInWindow(syswindow, wx, wy);
	return TRUE;
}

/*
 * setmouserect confines the system's pointer in fullscreen to the
 * rectangle of ClipCursor, as Windows confined the original's, so that
 * the game's pointer follows it at once, where the wings and the bars
 * would leave a dead zone. While the pointer roams it confines to the
 * screen. A window leaves it free, to reach the desktop.
 */
static void
setmouserect(void)
{
	SDL_Rect r;
	RECT c;
	float x0, y0, x1, y1;

	if (syswindow == NULL || sysrenderer == NULL || (replaying && !recording))
		return;
	if (!sysfullscreen || unclipped) {
		SDL_SetWindowMouseRect(syswindow, NULL);
		SDL_SetWindowMouseGrab(syswindow, false);
		return;
	}
	if (showcount < 0)
		SDL_HideCursor();
	SDL_SetWindowMouseGrab(syswindow, true);
	c = cliprect;
	c.left += sysdrawx;
	c.right += sysdrawx;
	if (sysroam)
		c = sysscreen;
	if (!SDL_RenderCoordinatesToWindow(sysrenderer, c.left + sysframex, c.top, &x0, &y0) ||
	    !SDL_RenderCoordinatesToWindow(sysrenderer, c.right + sysframex, c.bottom, &x1, &y1))
		return;
	r.x = (int)SDL_ceilf(x0);
	r.y = (int)SDL_ceilf(y0);
	r.w = (int)x1 - r.x;
	r.h = (int)y1 - r.y;
	if (r.w > 0 && r.h > 0)
		SDL_SetWindowMouseRect(syswindow, &r);
}

/*
 * setarrow shows the system's arrow in a window at render's x, y, outside
 * what the game's own pointer reaches while the game hides the arrow: the
 * frame at sysdrawx, or the screen while the pointer roams.
 */
static void
setarrow(float x, float y)
{
	float left, right;

	if (showcount >= 0 || sysfullscreen || (replaying && !recording))
		return;
	left = sysroam ? 0 : sysframex + sysdrawx;
	right = sysroam ? sysscreenw : left + Gamew;
	if (x < left || x >= right || y < 0 || y >= Gameh)
		SDL_ShowCursor();
	else
		SDL_HideCursor();
}

/* syncpointer gives the system's pointer the rectangle and the arrow of the pointer's mode. */
static void
syncpointer(void)
{
	float x, y;

	setmouserect();
	if (sysrenderer == NULL || SDL_GetMouseFocus() != syswindow)
		return;
	SDL_GetMouseState(&x, &y);
	if (SDL_RenderCoordinatesFromWindow(sysrenderer, x, y, &x, &y))
		setarrow(x, y);
}

/* syssetroam lets the pointer roam the wide screen, or holds it to the frame. */
void
syssetroam(int on)
{
	on = on && sysedges && sysframex > 0 && !sysframepoints;
	if (on == sysroam)
		return;
	sysroam = on;
	syncpointer();
}

/* sysholdpointer holds the pointer to a modal screen that the game draws at sysdrawx x. */
void
sysholdpointer(int x)
{
	sysdrawx = x;
	sysroam = 0;
	syncpointer();
}

/* ClipCursor keeps the rectangle, and NULL lifts it, as the game does on its way out. */
BOOL
ClipCursor(const RECT *r)
{
	unclipped = r == NULL;
	if (r != NULL)
		cliprect = *r;
	setmouserect();
	return TRUE;
}

int
ShowCursor(BOOL show)
{
	showcount += show ? 1 : -1;
	if (showcount < 0)
		SDL_HideCursor();
	else
		SDL_ShowCursor();
	return showcount;
}

/* message boxes */
static int
messagebox(const char *text, const char *caption, UINT type)
{
	SDL_MessageBoxButtonData buttons[3];
	SDL_MessageBoxData box;
	char *t, *c;
	int n, id, def;

	switch (type & MB_TYPEMASK) {
	case MB_OKCANCEL:
		buttons[0].buttonID = IDOK, buttons[0].text = "OK";
		buttons[1].buttonID = IDCANCEL, buttons[1].text = "Cancel";
		n = 2;
		break;
	case MB_YESNOCANCEL:
		buttons[0].buttonID = IDYES, buttons[0].text = "Yes";
		buttons[1].buttonID = IDNO, buttons[1].text = "No";
		buttons[2].buttonID = IDCANCEL, buttons[2].text = "Cancel";
		n = 3;
		break;
	case MB_YESNO:
		buttons[0].buttonID = IDYES, buttons[0].text = "Yes";
		buttons[1].buttonID = IDNO, buttons[1].text = "No";
		n = 2;
		break;
	default:
		buttons[0].buttonID = IDOK, buttons[0].text = "OK";
		n = 1;
		break;
	}
	def = (type & MB_DEFMASK) == MB_DEFBUTTON2 && n > 1 ? 1 : 0;
	for (id = 0; id < n; id++)
		buttons[id].flags = id == def ? SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT : 0;
	if (replaying)
		return buttons[def].buttonID;
	t = winutf8(text);
	c = winutf8(caption != NULL ? caption : "");
	memset(&box, 0, sizeof box);
	box.flags = (type & MB_ICONMASK) == MB_ICONHAND ? SDL_MESSAGEBOX_ERROR : SDL_MESSAGEBOX_INFORMATION;
	box.window = syswindow;
	box.title = c;
	box.message = t;
	box.numbuttons = n;
	box.buttons = buttons;
	id = buttons[def].buttonID;
	if (!SDL_ShowMessageBox(&box, &id))
		SDL_Log("teudogar: message box: %s", SDL_GetError());
	SDL_free(t);
	SDL_free(c);
	return id < 0 ? IDCANCEL : id;
}

int
MessageBoxA(HWND w, LPCSTR text, LPCSTR caption, UINT type)
{
	(void)w;
	return messagebox(text, caption, type);
}

int
MessageBoxExA(HWND w, LPCSTR text, LPCSTR caption, UINT type, WORD lang)
{
	(void)w, (void)lang;
	return messagebox(text, caption, type);
}

/* dialogs: DIALOG 40 as a message box that errproc fills and answers */
static const struct {
	int id;
	const char *text;
} controls40[Ncontrols] = {
	{ Cwebsite, "Go to &Website" },
	{ Creadme, "&View ReadMe" },
	{ Cabort, "&Abort" },
	{ Ctitle, "Error" },
	{ Ctext, "Error" },
};

static Control *
control(Window *d, int id)
{
	int i;

	for (i = 0; i < Ncontrols; i++)
		if (d->controls[i].id == id)
			return &d->controls[i];
	return NULL;
}

/* noamp copies s, dropping the & that marks a key. */
static void
noamp(char *dst, const char *s, size_t n)
{
	size_t i;

	for (i = 0; *s != '\0' && i + 1 < n; s++)
		if (*s != '&')
			dst[i++] = *s;
	dst[i] = '\0';
}

INT_PTR
DialogBoxParamA(HINSTANCE inst, LPCSTR name, HWND parent, DLGPROC proc, LPARAM lp)
{
	SDL_MessageBoxButtonData buttons[3];
	SDL_MessageBoxData box;
	char caps[3][64], *msg, *title, text[2 * Maxtext + 4];
	Window *d;
	int i, n, id;

	(void)inst, (void)parent;
	if (name != MAKEINTRESOURCE(40))
		return -1;
	d = calloc(1, sizeof *d);
	if (d == NULL)
		return -1;
	d->magic = Wndmagic;
	d->dialog = 1;
	d->dlgproc = proc;
	SDL_strlcpy(d->title, "TEUDOGAR", sizeof d->title);
	for (i = 0; i < Ncontrols; i++) {
		d->controls[i].magic = Wndmagic + 1;
		d->controls[i].id = controls40[i].id;
		d->controls[i].visible = 1;
		d->controls[i].dialog = d;
		SDL_strlcpy(d->controls[i].text, controls40[i].text, sizeof d->controls[i].text);
	}
	proc(d, WM_INITDIALOG, 0, lp);
	while (!d->ended) {
		n = 0;
		for (i = 0; i < 3; i++) {
			if (!d->controls[i].visible)
				continue;
			noamp(caps[n], d->controls[i].text, sizeof caps[n]);
			buttons[n].buttonID = d->controls[i].id;
			buttons[n].text = caps[n];
			buttons[n].flags = d->controls[i].id == Cabort ?
				SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT | SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT : 0;
			n++;
		}
		SDL_snprintf(text, sizeof text, "%s\n\n%s", control(d, Ctitle)->text, control(d, Ctext)->text);
		id = Cabort;
		if (!replaying) {
			msg = winutf8(text);
			title = winutf8(d->title);
			memset(&box, 0, sizeof box);
			box.flags = SDL_MESSAGEBOX_ERROR;
			box.title = title;
			box.message = msg;
			box.numbuttons = n;
			box.buttons = buttons;
			if (!SDL_ShowMessageBox(&box, &id) || id < 0)
				id = Cabort;
			SDL_free(msg);
			SDL_free(title);
		}
		proc(d, WM_COMMAND, MAKEWPARAM(id, Bnclicked), 0);
	}
	id = (int)d->result;
	d->magic = 0;
	free(d);
	return id;
}

BOOL
EndDialog(HWND h, INT_PTR r)
{
	Window *d;

	d = h;
	if (d == NULL || d->magic != Wndmagic || !d->dialog)
		return FALSE;
	d->ended = 1;
	d->result = r;
	return TRUE;
}

HWND
GetDlgItem(HWND h, int id)
{
	Window *d;

	d = h;
	if (d == NULL || d->magic != Wndmagic || !d->dialog)
		return NULL;
	return control(d, id);
}

BOOL
SetDlgItemTextA(HWND h, int id, LPCSTR s)
{
	Control *c;

	c = GetDlgItem(h, id);
	if (c == NULL)
		return FALSE;
	SDL_strlcpy(c->text, s, sizeof c->text);
	return TRUE;
}

LRESULT
SendDlgItemMessageA(HWND h, int id, UINT msg, WPARAM wp, LPARAM lp)
{
	(void)h, (void)id, (void)msg, (void)wp, (void)lp;
	return 0;
}

int
CheckRadioButton(HWND h, int first, int last, int id)
{
	(void)h, (void)first, (void)last, (void)id;
	return TRUE;
}

DWORD
SetClassLongA(HWND h, int i, LONG v)
{
	(void)h, (void)i, (void)v;
	return 0;
}

BOOL
IsDialogMessageA(HWND h, LPMSG m)
{
	(void)h, (void)m;
	return FALSE;
}

/* the clipboard serves only dead code of the game */
BOOL
OpenClipboard(HWND h)
{
	(void)h;
	return FALSE;
}

BOOL
CloseClipboard(void)
{
	return TRUE;
}

BOOL
EmptyClipboard(void)
{
	return FALSE;
}

HANDLE
GetClipboardData(UINT f)
{
	(void)f;
	return NULL;
}

HANDLE
SetClipboardData(UINT f, HANDLE h)
{
	(void)f, (void)h;
	return NULL;
}

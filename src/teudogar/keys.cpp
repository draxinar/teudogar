#include <windows.h>

#include "base.h"

int __cdecl pollinput(void);
int __cdecl readinput(void);

extern char scantab[0x3a];

extern char waitcode;
extern char yesnochar;
extern BYTE keys[256];
extern char keycode;
extern char lastscan;
extern char lastchar;
extern unsigned char esckey;
extern WORD lastascii;
extern int lastvk;
extern char dirkeys;
extern char pausekey;
extern char tabkey;
extern int typing;

void
clearkey(void)
{
	if (keycode == lastscan)
		keycode = 0;
	lastscan = 0;
	lastvk = 0;
	lastchar = 0;
}

void
kbdinit(void)
{
}

int
keydown(int key, int lp)
{
	GetKeyboardState(keys);
	if (!typing || typing == 0xff)
		switch (key) {
		case 'W':
			if (typing == 0xff)
				break;
		case 0xbe:
		case 0xbf:
		case 0xc0:
		case 0xe2:
			return 1;
		case VK_HOME:
			dirkeys |= 1;
			break;
		case VK_UP:
			dirkeys |= 2;
			break;
		case VK_PRIOR:
			dirkeys |= 4;
			break;
		case VK_LEFT:
			dirkeys |= 8;
			break;
		case VK_RIGHT:
			dirkeys |= 0x10;
			break;
		case VK_END:
			dirkeys |= 0x20;
			break;
		case VK_DOWN:
			dirkeys |= 0x40;
			break;
		case VK_NEXT:
			dirkeys |= 0x80;
			break;
		}
	switch (key) {
	case VK_SHIFT:
	case VK_CONTROL:
	case VK_MENU:
		return 1;
	case VK_PAUSE:
		pausekey = 1;
		return 1;
	case VK_ESCAPE:
		if (esckey != 0xff && esckey != 0xfe)
			esckey = 1;
		break;
	case VK_TAB:
		tabkey = 1;
		break;
	}
	lastscan = keycode = (lp >> 16) & 0xff;
	lastvk = key;
	switch (ToAscii(key, lastscan, keys, &lastascii, 0)) {
	case 1:
	case 2:
		lastchar = lastascii;
		if (lastchar > 0x1f)
			return 0;
		if (lastchar == 13 && key != 'M')
			return 0;
		if (lastchar == 27)
			return 0;
		if (lastchar == 8 && key != 'H' || lastchar == 14 && key != 'N' || lastchar == 0)
			lastchar = 0;
		else
			lastchar += 0x60;
		return 0;
	default:
		lastchar = 0;
		if (lastscan == 0x4a)
			lastchar = '-';
		else if (lastscan == 0x4e)
			lastchar = '+';
		return 0;
	}
}

int
keyup(int key, int lp)
{
	GetKeyboardState(keys);
	keycode = 0;
	switch (key) {
	case VK_HOME:
		dirkeys &= ~1;
		break;
	case VK_UP:
		dirkeys &= ~2;
		break;
	case VK_PRIOR:
		dirkeys &= ~4;
		break;
	case VK_LEFT:
		dirkeys &= ~8;
		break;
	case VK_RIGHT:
		dirkeys &= ~0x10;
		break;
	case VK_END:
		dirkeys &= ~0x20;
		break;
	case VK_DOWN:
		dirkeys &= ~0x40;
		break;
	case VK_NEXT:
		dirkeys &= ~0x80;
		break;
	}
	return 0;
}

char
scanchar(char c)
{
	if (c == lastscan)
		return lastchar;
	if (c < sizeof scantab)
		return scantab[c];
	if (c == 0x4a)
		return '-';
	if (c == 0x4e)
		return '+';
	return 0;
}

char
waitkey(void)
{
	do
		pollinput();
	while (keycode);
	do {
		pollinput();
		waitcode = keycode;
	} while (waitcode == 0);
	return waitcode;
}

int
// PORT: a name of its own, which C wants for an overloaded function
#ifdef PORT
waitnokey(void)
#else
waitrelease(void)
#endif
{
	do {
		if (!readinput())
			return 0;
	} while (keycode && !esckey);
	return 1;
}

int
waitpress(void)
{
	do {
		if (!readinput())
			return 0;
	} while (!keycode && !esckey);
	return 1;
}

void
setkeymode(int v)
{
	if (v == 0xff)
		typing = 0xff;
	else if (v)
		typing = 1;
	else
		typing = 0;
}

char
askyesno(void)
{
	lastchar = 0;
	esckey = 0;
	for (;;) {
		pollinput();
		yesnochar = lastchar;
		if (esckey) {
			if (esckey != 0xff)
				esckey = 0;
			return 'n';
		}
		if (yesnochar == 'n' || yesnochar == 'N' || yesnochar == 27)
			return 'n';
		if (yesnochar == 'j' || yesnochar == 'J' || yesnochar == 'y' || yesnochar == 'Y'
		|| yesnochar == 'z' || yesnochar == 'Z' || yesnochar == 'O' || yesnochar == 'o'
		|| yesnochar == 'S' || yesnochar == 's')
			return 'j';
	}
}

char scantab[58] = {
	0, 1, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 61, '\xff', 9, 113,
	119, 101, 114, 116, 122, 117, 105, 111, 112, '\x81', 43, 13, 0, 97,
	115, 100, 102, 103, 104, 106, 107, 108, '\x94', '\x84', 1, 0, 1, 121,
	120, 99, 118, 98, 110, 109, 44, 46, 45, 0, 42, 0, 32
};
char waitcode;
char yesnochar;
BYTE keys[256];
char keycode;
char lastscan;
char lastchar;
unsigned char esckey;
WORD lastascii;
int lastvk;
int keyunused;
char shotkey;
char dirkeys;
char pausekey;
char musickey;
char sfxkey;
char tabkey;
int typing;

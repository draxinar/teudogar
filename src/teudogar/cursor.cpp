#include <windows.h>

void msrestore(void);
void iprestore(int s);
void ipsave(int s);
void mousedraw(void);

extern int playmode;
extern int mousex;
extern int mousey;
extern int mousemoved;
extern int lbutton;
extern int rbutton;
extern int presslatch;
extern int curkind;
extern int curimg;
extern int curshape;
extern int lastx;
extern int lasty;
extern int clipx1;
extern int clipy1;

extern int lostcontrol;
extern int spriteshape;
extern int spritex;
extern int spritey;
extern RECT cliprect;
extern int curchanged;
extern int shown;
extern short swapped;
extern int curkind1;
extern int curimg1;
extern int curkind2;
extern int curimg2;
extern int savedkind;
extern int savedimg;
extern int clipx0;
extern int clipy0;
extern int ptrstates;
extern int ptrdrawn;
extern unsigned char menumode;

int
mouseinit(void)
{
	swapped = GetSystemMetrics(SM_SWAPBUTTON);
	if (swapped) {
		lbutton = 2;
		rbutton = 1;
	} else {
		lbutton = 1;
		rbutton = 2;
	}
	shown = 0;
	return 1;
}

void
unclip(void)
{
	ClipCursor(NULL);
}

void
clampmouse(void)
{
	if (mousex < clipx0)
		mousex = clipx0;
	else if (mousex > clipx1)
		mousex = clipx1;
	if (mousey < clipy0)
		mousey = clipy0;
	else if (mousey > clipy1)
		mousey = clipy1;
}

void
setclip(short x0, short y0, short x1, short y1)
{
	clipx0 = x0;
	clipy0 = y0;
	clipx1 = x1;
	clipy1 = y1;
	cliprect.left = clipx0;
	cliprect.top = clipy0;
	cliprect.right = clipx1 + 1;
	cliprect.bottom = clipy1 + 1;
	ClipCursor(&cliprect);
	clampmouse();
}

void
defclip(void)
{
	setclip(8, 0, 631, 479);
}

void
setmouse(short x, short y)
{
	if (SetCursorPos(x, y)) {
		mousex = x;
		mousey = y;
	}
	clampmouse();
}

void
movesprite(int shape, int x, int y)
{
	x -= 16;
	y -= 16;
	if (x < 0)
		x = 0;
	else if (x > 592)
		x = 592;
	if (y < 0)
		y = 0;
	else if (y > 432)
		y = 432;
	spriteshape = shape;
	// PORT: the pointer roaming the wide screen stands at its true place, whose square stays on the screen
#ifdef PORT
	if (sysroam) {
		x = mousewx - 16;
		if (x < sysscreen.left)
			x = sysscreen.left;
		else if (x > sysscreen.right - 48)
			x = sysscreen.right - 48;
	}
#endif
	spritex = x;
	spritey = y;
}

void
hidecursor(void)
{
	if (shown)
		msrestore();
	if (ptrdrawn) {
		iprestore(curshape);
		ptrdrawn = 0;
	}
	shown = 0;
}

void
drawcursor(void)
{
	if (lostcontrol)
		return;
	if (shown && lastx == mousex && lasty == mousey)
		return;
	curshape = 1;
	if (shown)
		msrestore();
	if (ptrstates != ptrdrawn) {
		if (ptrdrawn && !menumode && playmode != 1)
			iprestore(curshape);
		ptrdrawn = 0;
	}
	if (ptrdrawn == 0) {
		if (playmode != 1)
			ipsave(curshape);
	}
	lastx = mousex;
	lasty = mousey;
	movesprite(curshape, lastx, lasty);
	mousedraw();
	shown = 1;
}

int
nearcursor(int x, int y)
{
	if (shown && x + 48 >= mousex && x < mousex + 48 && y + 48 >= mousey && y < mousey + 48)
		return 1;
	return 0;
}

void
showcursor(void)
{
	if (lostcontrol)
		return;
	curshape = 0;
	if (playmode != 1)
		ipsave(curshape);
	lastx = mousex;
	lasty = mousey;
	movesprite(curshape, lastx, lasty);
	ptrdrawn = 0;
	mousedraw();
	shown = 1;
}

/* An image of 0xffff keeps a layer and 0 clears it. */
int
setcursor(int kind, int img, int kind1, int img1, int kind2, int img2)
{
	curchanged = 0;
	if (img == 0xffff)
		;
	else if (kind != curkind || img != curimg) {
		curchanged = 1;
		curkind = kind;
		curimg = img;
	}
	if (img1 == 0xffff)
		;
	else if (img1 == 0) {
		if (curkind1 != 0 || curimg1 != 0) {
			curimg1 = curkind1 = 0;
			curchanged = 1;
		}
	} else if (curkind1 != kind1 || img1 != curimg1) {
		curchanged = 1;
		curkind1 = kind1;
		curimg1 = img1;
	}
	if (img2 == 0xffff)
		;
	else if (img2 == 0) {
		if (curkind2 != 0 || curimg2 != 0) {
			curimg2 = curkind2 = 0;
			curchanged = 1;
		}
	} else if (curkind2 != kind2 || img2 != curimg2) {
		curkind2 = kind2;
		curimg2 = img2;
		curchanged = 1;
	}
	if (curchanged) {
		lastx = 999;
		return 1;
	}
	return 0;
}

void
setcursor1(int kind, int img)
{
	setcursor(kind, img, 0, 0, 0, 0);
}

void
savecursor(void)
{
	savedkind = curkind;
	savedimg = curimg;
}

void
restorecursor(void)
{
	curkind = savedkind;
	curimg = savedimg;
}

void
redrawcursor(void)
{
	lastx = 999;
}

int mousex = 320;
int mousey = 240;
int mousemoved = 1;
int lbutton = 1;
int rbutton = 2;
int presslatch = 1;
int curkind = 3;
int curimg = 3;
int curshape = 1;
int lastx = 999;
int lasty = 999;
int clipx1 = 639;
int clipy1 = 479;
RECT cliprect;
int curchanged;
int clickdx;
int clickdy;
int mouseleft;
int mouseright;
int leftclick;
int rightclick;
int dblclick;
int shown;
short swapped;
int clickx;
int clicky;
int clicktime;
int curkind1;
int curimg1;
int curkind2;
int curimg2;
int curunused[6];
int savedkind;
int savedimg;
int clipx0;
int clipy0;
int ptrunit;
int ptrstates;
int ptrdrawn;

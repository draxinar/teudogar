#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

typedef struct Unit Unit;
typedef struct Node Node;
typedef struct Panel Panel;

/* 0x1b8 bytes each (009bc240) */
struct Unit {
	unsigned char x;
	unsigned char y;
	char f2[2];
	signed char dirx;		/* direction x */
	signed char diry;		/* direction y */
	char picset[2];
	unsigned char pose;
	char f9[0xf3 - 9];
	short health;
	char ff5[0x135 - 0xf5];
	unsigned char asleep;
	unsigned char bewitched;
	char f137[0x13a - 0x137];
	unsigned char fleeing;
	unsigned char confused;
	char bleeding;
	unsigned char berserk;
	char f13e[0x1b8 - 0x13e];
};

/* a node of a scene's dialog tree (as dialog defines it) */
struct Node {
	char *start;
	char *end;
	unsigned char parent;
	unsigned char nchild;
	unsigned char child[0x3c - 0xa];
	short width;
	unsigned char shown;
	char *text;
	char *src;
	short x;
	short y;
	unsigned char flags;
};

/* 0x28 bytes each (00a4ca78) */
struct Panel {
	unsigned short type;
	char f2[4];
	int x;
	int y;
	int w;
	int h;
	unsigned short id;
	unsigned short arg;
	char f1a[0x28 - 0x1a];
};

int __cdecl readinput(void);
void sqputm(unsigned n);
void sqputm48(unsigned n);
void sqputo(unsigned n);
void sqputo48(unsigned n);
void sqclear(void);
void sqclear48(void);
void savescr(void);
void restscr(void);
void drawspq(void);
char *capname(Unit *u);
int textwidth(char *s);
void drawtext2o(int n, unsigned off, char *s);
void text2n(int n, int x, int y, char *fmt, ...);
int text3n(int n, int x, int y, char *fmt, ...);
int text3(int n, int x, int y, char *s);
int text3m(int n, int x, int y, char *s);
void putface(int x, int y, char *src);
void faceshadow(int x, int y, char *src);
int tipshown(void);
void pgtoback(void);
void drawmap(int x, int y, short mode, short showunits);
void puthands(void);
void tradeframe(void);
void sqclip(int x, int y);
void cleardecos(void);
void drawdecos(void);
void drawfx(void);
int cansee(int x, int y);
void adddeco(void);
void calclight(void);
void darkcorners(void);
void clrframe(void);
void clrsides(void);
int text3o(int n, int x, int y, char *s);
int text3b(int n, int x, int y, char *s);
int pillrest(int x, int y, int w, int h);
unsigned long silvprice(unsigned long n);
int silvtext(char *buf, unsigned long n);
void drawtext2(int n, int x, int y, char *s);
int text3w(char *s);
void button(int x, int y, int w, int h, char *s, unsigned char lit);
int balance(int a, int b);
int wrap(char *s, int width);
int pillshade9(int x, int y, int w, int h);
void drawinventory(int id);
void drawstore(int id);
void winframe(int x, int y, int w, int h, int keyed);
void drawboxes(short x, short y);
int putwint(int x, int y, int idx);
int shade2(int x, int y, int w, int h);
int putgauge(int n, int x, int y, int v);
int putherostates(int surf, int x, int y, int flag, int mode);
void drawstats(void);
void drawmenubar(void);
void backto2(void);
void showcursor(void);
int flip(void);
void setlight(void);

extern Unit units[];
extern int turncount;
extern int playerx;
extern int playery;
extern unsigned short stepframe;
extern unsigned fireframe;
extern unsigned slowframe;
extern unsigned waterframe;
extern int showall;
extern int sqstate;
extern unsigned long map[22500];
extern unsigned long mapitems[];
extern unsigned long mapflags[];
extern unsigned char viewgrid[17][19];	/* the view, 19 by 17 cells around the map position */
extern short textbg;
extern int textmode;
extern unsigned char *drawbuf;
extern char packbuf[616000];
extern int nspq;
extern char speech[];
extern short speechpx;
extern short talky;
extern short nameoff;
extern char talkactive;
extern char trademode;
extern int playmode;
extern short nmsgs;
extern unsigned long gametime;
extern unsigned long msgtime[17];
extern short msgy[18];
extern char msgtext[17][200];
extern short actionturn;
extern int cheat;
extern unsigned char keys[256];
extern unsigned short tipx;
extern unsigned short tipy;
extern char tiptext[];
extern unsigned short nunits;		/* 004c3ba8 */
extern Node nodes[255];			/* 006559c0 */
extern Node choices[4];			/* 0065a578 */
extern unsigned short nnodes;		/* 0065a6a8 */
extern char *strs[];			/* 00a38f08 */
extern int surfslost;
extern int debugmap;
extern char texts[47942];
extern unsigned short curmap;
extern unsigned short steptick;
extern int litchoice;
extern int talkchoice;
extern char lastspeech;
extern short curblink;
extern short heroblink;
extern char curface2[];
extern char curface[];
extern char heroface2[];
extern char heroface[];
extern unsigned short talkstate;
extern unsigned short curunit;		/* 009ce8d6 */
extern int scrollx;
extern int scrolly;
extern char mapframed;
extern int decoframe;
extern int decophase;
extern Unit *decounit;
extern void (*viewhook)(void);
extern void (*screenhook)(void);
extern short tradekind;
extern unsigned long tradegoods[24];
extern unsigned long tradeoffer[24];
extern char picked[24];
extern int sqx;
extern int sqy;
extern unsigned char tradehints;
extern unsigned char showtotal;
extern unsigned long hinttime;
extern unsigned long goodsbase;
extern unsigned long offerbase;
extern char textbuf[];
extern DWORD nforsale;
extern unsigned long waresilv;
extern char namebuf[2500];
extern int text3width;
extern unsigned long goodsprice;
extern unsigned long offerprice;
extern int tradedone;
extern int dutalk;
extern short lastbutton;
extern short hotbutton;
extern unsigned char panelstack[16];
extern Panel panels[];
extern short condgauge;
extern int statshown;
extern int barshown;
extern int noflip;
extern int drawto2;
extern int shown;
extern int hoverx;
extern int hovery;

extern Panel *drawpanel;
extern int drawpn;
extern short drawi;
extern char *drawp;
extern int subscrollx;
extern int subscrolly;
extern short cellx;
extern short celly;
extern char numtext[24];
// PORT: the wide view's sight and the count of adddeco's pictures
#ifdef PORT
extern unsigned char wideview[17][2 * Widehalf + 1];
int widesee(int x, int y);
extern int ndecobufs;
#endif
extern unsigned char cellsight;
extern unsigned char topsight;
extern char *dnname;
extern short drawline;
extern unsigned long cellword;
extern unsigned long tile;
extern unsigned long *cellitem;
extern unsigned short cellidx;
extern short celldx;
extern short celldy;
extern short blankline;

/* whether the cell at x, y shows its next animation frame now */
short
animturn(short x, short y)
{
	int n;

	if (y & 1)
		n = (x + y * 150) & 7;
	else
		n = (149 - x + y * 150) & 7;
	if ((turncount & 7) == n)
		return 1;
	return 0;
}

// PORT: the wide view: its columns, the sight of its cells, and its units
#ifdef PORT
enum {
	Maxdecobufs = 120,	/* the pictures of 0x900 bytes in scrbuf from 0x7d00 */
	Stepscroll = 46	/* the farthest that a step scrolls the view */
};

/* viewcols returns the columns to draw each side of the hero: those that reach the wide screen's edge under a step's scroll, or the original's 7. */
static int
viewcols(void)
{
	if (sysscreen.left == 0)
		return 7;
	return (sysscreen.right - 296 + Stepscroll + 47) / 48 - 1;
}

/* inwing reports whether the current column lies past those that the original draws. */
static int
inwing(void)
{
	return celldx < -7 || celldx > 7;
}

/*
 * offmap reports whether the current cell stays black: one with no index in
 * map, or on the wide screen, which shows the original's hidden columns too,
 * a column past its 13 off the map.
 */
static int
offmap(void)
{
	int x;

	if (cellidx >= 22500)
		return 1;
	x = playerx + celldx;
	return sysscreen.left < 0 && (celldx < -6 || celldx > 6) && (x < 0 || x >= 150);
}

/* viewcell returns the sight of the current cell: viewgrid's in the original's columns, wideview's in the wings. */
static unsigned char
viewcell(void)
{
	if (offmap())
		return 0;
	if (inwing())
		return wideview[celldy + 8][celldx + Widehalf];
	return ((unsigned char *)viewgrid)[celldx + 9 + (celldy + 8) * 19];
}

/*
 * widedecos adds the units that the wings show. adddeco rewrites the
 * poses, seats, and trails of the units it draws, which the saved games
 * keep, so each unit here takes back its state. The units that the
 * original draws come first in the picture buffers, and these take the
 * room left.
 */
static void
widedecos(void)
{
	Unit saved, *lastunit, *u;
	int i, dx, lastframe, lastpos, n;

	if (sysscreen.left == 0)
		return;
	lastunit = decounit;
	lastframe = decoframe;
	lastpos = decophase;
	n = viewcols() + 1;
	for (i = 1; i < nunits && ndecobufs + 2 <= Maxdecobufs; i++) {
		u = &units[i];
		dx = abs(u->x - playerx);
		if (u->health <= 0 || dx <= 7 || dx > n || abs(u->y - playery) > 6)
			continue;
		if (!widesee(u->x, u->y) && !(u->pose == 1 && widesee(u->x + u->dirx, u->diry + u->y)))
			continue;
		decounit = u;
		decoframe = stepframe;
		decophase = decoframe * 4 + (steptick & 3);
		saved = *u;
		adddeco();
		*u = saved;
	}
	decounit = lastunit;
	decoframe = lastframe;
	decophase = lastpos;
}
#endif

void
animtile(void)
{
	if (tile >= 0x5b0 && tile < 0x5d7 || tile >= 0x888 && tile < 0x93d) {
		if (animturn(playerx + celldx, playery + celldy)) {
			tile++;
			tile += stepframe / 2;
		}
	} else if (tile >= 0x5d7 && tile < 0x618)
		tile += fireframe;
}

void
drawcell(void)
{
	// PORT: a cell off the map stays black, and the sight of the wings comes from wideview
#ifdef PORT
	if (!showall || offmap()) {
		cellsight = viewcell();
#else
	if (!showall) {
		cellsight = ((unsigned char *)viewgrid)[celldx + 9 + (celldy + 8) * 19];
#endif
		if (!cellsight) {
			if (sqstate)
				sqclear();
			else
				sqclear48();
			return;
		}
	} else
		cellsight = 1;
	if (cellsight == 8)
		return;
	cellword = map[cellidx];
	if (cellsight != 0x20) {
normal:
		tile = cellword & 0xfff;
		if (tile >= 0x1e1 && tile < 0x208)
			tile += slowframe;
		else if (tile >= 0x478 && tile < 0x5b0)
			tile += waterframe;
		if (sqstate)
			sqputm(tile);
		else
			sqputm48(tile);
	} else {
		if ((cellword & 0x1000) == 0 || (mapitems[cellword >> 16] & 0xfff) >= 0x9c)
			goto normal;
		if (sqstate)
			sqclear();
		else
			sqclear48();
	}
	if ((cellword & 0x1000) == 0)
		return;
	cellitem = &mapitems[cellword >> 16];
	switch (cellsight) {
	case 2:
	case 0x20:
		do {
			if (*cellitem & 0x4000)
				return;
			if ((*cellitem & 0xfff) < 0x9c) {
				tile = *cellitem & 0xfff;
				if (sqstate)
					sqputo(tile);
				else
					sqputo48(tile);
			}
		} while (*cellitem++ & 0x8000);
		break;
	case 0x40:
	default:
		do {
			if (*cellitem & 0x6000)
				return;
			tile = *cellitem & 0xfff;
			animtile();
			if (sqstate)
				sqputo(tile);
			else
				sqputo48(tile);
		} while (*cellitem++ & 0x8000);
		break;
	}
}

void
drawcelltop(void)
{
	// PORT: a cell off the map stays black
#ifdef PORT
	if (offmap())
		return;
#endif
	cellword = map[cellidx];
	if ((cellword & 0x1000) == 0)
		return;
	if ((cellword & 0x2000) == 0 && (mapflags[cellidx] & 0x80) == 0)
		return;
	if (!showall) {
		// PORT: the sight of the wings comes from wideview
#ifdef PORT
		topsight = viewcell();
#else
		topsight = ((unsigned char *)viewgrid)[celldx + 9 + (celldy + 8) * 19];
#endif
		if (!topsight)
			return;
	} else
		topsight = 1;
	cellitem = &mapitems[cellword >> 16];
	switch (topsight) {
	case 2:
	case 0x20:
		break;
	case 8:
		do {
			if (*cellitem & 0x4000) {
				tile = *cellitem & 0xfff;
				if (tile == 0x3b7 || tile == 0x792) {
					if (sqstate)
						sqclear();
					else
						sqclear48();
					return;
				}
				animtile();
				if (sqstate)
					sqputo(tile);
				else
					sqputo48(tile);
				return;
			}
		} while (*cellitem++ & 0x8000);
		break;
	case 0x40:
		do {
			if (*cellitem & 0x6000) {
				tile = *cellitem & 0xfff;
				animtile();
				if (sqstate)
					sqputo(tile);
				else
					sqputo48(tile);
			}
		} while (*cellitem++ & 0x8000);
		break;
	default:
		do {
			if (*cellitem & 0x4000)
				return;
			if (*cellitem & 0x2000) {
				tile = *cellitem & 0xfff;
				animtile();
				if (sqstate)
					sqputo(tile);
				else
					sqputo48(tile);
			}
		} while (*cellitem++ & 0x8000);
		break;
	}
}

void
drawmsgs(void)
{
	short j, first, i, n;
	char *p;

	if (!talkactive && !trademode) {
		if (playmode == 1 || units[0].health <= 0)
			first = nmsgs;
		else if (stepframe == 0) {
			if (!actionturn)
				first = nmsgs;
			else
				first = 0;
		} else if (stepframe == 1)
			first = 0;
		else
			first = nmsgs * (stepframe - 1) / 5;
		n = 0;
		for (i = 0; i < nmsgs; i++)
			if (msgtime[i] < gametime)
				n = i + 1;
		if (cheat && (keys[0xbf] & 0x80) && (keys[VK_CONTROL] & 0x80))
			text3n(0, 0, 0x50, "M:%d..%d/%d", n, first, nmsgs);
		if (n > first)
			first = n;
		if (first > nmsgs)
			first = nmsgs;
		for (j = 0; j < first; j++) {
			celldy = msgy[j];
			if (first < nmsgs)
				celldy += 478 - msgy[first];
			p = msgtext[j];
			for (;;) {
				text3(0, 0xffff, celldy, p);
				while (*p != 13) {
					if (*p == 0 || *p == 0x7b)
						goto next;
					p++;
				}
				p++;
				celldy += 22;
			}
		next:;
		}
	}
	if (tipshown() && (!talkactive || trademode)) {
		// PORT: a name's tip stands by the pointer on the wide screen
#ifdef PORT
		sysdrawx = tipshift;
#endif
		text3(0, tipx, tipy, tiptext);
		p = tiptext;
		celldy = tipy;
		for (;;) {
			if (*p == 13) {
				p++;
				celldy += 24;
				text3(0, 0xffff, celldy, p);
			} else if (*p == 0 || *p == 0x7b)
				break;
			else
				p++;
		}
		// PORT: as above
#ifdef PORT
		sysdrawx = 0;
#endif
	}
}

void
begintext(int on)
{
	int m;

	m = textbg;
	if (m == 1) {
		if (!on)
			textmode = 4;
		else {
			// PORT: the buffer as unsigned char, which C wants cast
#ifdef PORT
			drawbuf = (unsigned char *)(packbuf + 0x4b640);
#else
			drawbuf = packbuf + 0x4b640;
#endif
			savescr();
			textmode = 1;
			nspq = 0;
		}
	} else if (m == 2)
		textmode = 2;
	else if (m == 3)
		textmode = 3;
	else
		textmode = 1;
}

void
endtext(int on)
{
	if (on && textbg == 1) {
		restscr();
		if (nspq) {
			drawspq();
			nspq = 0;
		}
	}
	drawbuf = 0;
	textmode = 1;
}

void
// PORT: a name for the unused parameter, which C99 wants
#ifdef PORT
scenetext(int unused)
#else
scenetext(int)
#endif
{
	char *p;
	int c;

	p = speech;
	blankline = 1;
	talky = 70;
	drawline = 0;
	if (speechpx + 93 > 320) {
		talky = 320 - speechpx - 23;
		if (talky < 0)
			talky = 0;
	}
	while (drawline <= 15) {
		while (*p == ' ')
			p++;
		if (*p && *p != 13) {
			text3m(0, 152, talky, p);
			blankline = 0;
			talky += 23;
		} else {
			blankline = 1;
			talky += 15;
		}
		do {
			c = *p;
			if (c == 0)
				return;
			p++;
		} while (c != 13);
		drawline++;
	}
}

void
drawname(int x, int y, int n, int clip)
{
	dnname = capname(&units[n]);
	nameoff = 54 - textwidth(dnname) / 2;
	x += nameoff;
	if (x < 8) {
		if (!clip)
			return;
		x = 8;
	}
	drawtext2o(0, x + (y + 123) * 640, dnname);
}

void
drawface(int x, int y, char *pic, int n)
{
	putface(x, y, pic);
	faceshadow(x, y, pic);
	drawname(x, y, n, 0);
}

void
drawflatface(int x, int y, char *pic, int n)
{
	putface(x, y, pic);
	drawname(x, y, n, 0);
}

void
centertext(int i, int n, char *s)
{
	int y;

	y = (480 - n * 10) / 2;
	text2n(0, 308 - textwidth(s) / 2, y + i * 10, s);
}

void
drawscreen(void)
{
	readinput();
	surfslost = 0;
	if (trademode) {
		tradeframe();
		if (talkactive) {
			putface(16, 12, curblink ? curface2 : curface);
			if (curblink)
				faceshadow(16, 12, curface2);
			else
				faceshadow(16, 12, curface);
		}
	} else {
		if (debugmap && !(keys[VK_SHIFT] & 0x80) && !(keys['W'] & 0x80)
		|| cheat && (keys[VK_SHIFT] & 0x80) && (keys['W'] & 0x80)) {
			pgtoback();
			drawmap(100, 90, 0, 1);
			text3n(0, 0xffff, 0, texts + 0xb037, curmap, playerx, playery, gametime, steptick, stepframe);
			puthands();
		} else {
			subscrollx = 0;
			subscrolly = 0;
			if (units[0].pose == 1 && steptick <= 23) {
				if (units[0].dirx < 0) {
					scrollx = steptick;
					subscrollx = (steptick & 3);
				} else if (units[0].dirx <= 0)
					scrollx = 0;
				else {
					scrollx = steptick;
					scrollx = -scrollx;
					subscrollx = (steptick & 3);
					subscrollx = -subscrollx;
				}
				if (units[0].diry < 0) {
					scrolly = steptick;
					subscrolly = (steptick & 3);
				} else if (units[0].diry <= 0)
					scrolly = 0;
				else {
					scrolly = steptick;
					scrolly = -scrolly;
					subscrolly = (steptick & 3);
					subscrolly = -subscrolly;
				}
			} else {
				scrollx = 0;
				scrolly = 0;
			}
			scrollx *= 2;
			scrolly *= 2;
			subscrollx *= 2;
			subscrolly *= 2;
			for (celldy = -6; celldy <= 6; celldy++)
				// PORT: the columns of the wide screen
#ifdef PORT
				for (celldx = -viewcols(); celldx <= viewcols(); celldx++) {
#else
				for (celldx = -7; celldx <= 7; celldx++) {
#endif
					cellx = celldx * 48 + 296 + scrollx;
					celly = celldy * 48 + 216 + scrolly;
					sqclip(cellx, celly);
					if (sqstate != 2 && (!sqstate || !mapframed)) {
						cellidx = playerx + celldx + (playery + celldy) * 150;
						drawcell();
					}
				}
			cleardecos();
			decoframe = steptick + 3;
			if (decoframe >= 24)
				decoframe -= 24;
			decophase = decoframe;
			decoframe /= 4;
			for (drawi = 0; drawi < nunits; drawi++) {
				decounit = &units[drawi];
				if (drawi) {
					decoframe = stepframe;
					decophase = decoframe * 4 + (steptick & 3);
				}
				if (decounit->health > 0 && abs(decounit->x - playerx) <= 7
				&& abs(decounit->y - playery) <= 6)
					if (cansee(decounit->x, decounit->y)
					|| decounit->pose == 1 && decounit != &units[0]
					&& cansee(decounit->x + decounit->dirx, decounit->diry + decounit->y))
						adddeco();
			}
			// PORT: the units of the wide screen's wings
#ifdef PORT
			widedecos();
#endif
			drawdecos();
			for (celldy = -6; celldy <= 6; celldy++)
				// PORT: the columns of the wide screen
#ifdef PORT
				for (celldx = -viewcols(); celldx <= viewcols(); celldx++) {
#else
				for (celldx = -7; celldx <= 7; celldx++) {
#endif
					cellx = celldx * 48 + 296 + scrollx;
					celly = celldy * 48 + 216 + scrolly;
					sqclip(cellx, celly);
					if (sqstate != 2 && (!sqstate || !mapframed)) {
						cellidx = playerx + celldx + (playery + celldy) * 150;
						drawcelltop();
					}
				}
			drawfx();
			calclight();
			darkcorners();
			if (mapframed)
				clrframe();
			else
				clrsides();
			if (viewhook)
				viewhook();
		}
		litchoice = 0;
		if (talkactive || lastspeech) {
			begintext(1);
			scenetext(1);
			if (!lastspeech && talkstate != 2) {
				if (talky < 320)
					talky = 320;
				litchoice = talkchoice;
				if (talkstate == 1) {
					if (choices[1].shown) {
						if (talkchoice == 1)
							text3o(0, choices[1].x, talky, choices[1].start);
						text3m(0, choices[1].x, talky, choices[1].start);
					}
					if (choices[2].shown) {
						if (talkchoice == 2)
							text3o(0, choices[2].x, talky, choices[2].start);
						text3m(0, choices[2].x, talky, choices[2].start);
					}
					if (choices[3].shown) {
						if (talkchoice == 3)
							text3o(0, choices[3].x, talky, choices[3].start);
						text3m(0, choices[3].x, talky, choices[3].start);
					}
				} else {
					for (celldx = 0; celldx < nnodes; celldx++)
						if (nodes[celldx].shown && nodes[celldx].y + talky >= 460)
							talky -= 29;
					drawi = 152;
					celldx = 0;
					while (celldx < nnodes) {
						if (nodes[celldx].shown) {
							if (talkchoice && celldx == talkchoice)
								text3o(0, nodes[celldx].x, nodes[celldx].y + talky, nodes[celldx].text);
							if (nodes[celldx].flags & 1) {
								if (textmode == 2 || textmode == 3)
									text3o(0, nodes[celldx].x, talky + nodes[celldx].y, nodes[celldx].text);
								text3b(0, nodes[celldx].x, nodes[celldx].y + talky, nodes[celldx].text);
							} else
								text3m(0, nodes[celldx].x, talky + nodes[celldx].y, nodes[celldx].text);
						}
						celldx++;
					}
				}
			} else if (talky < 140)
				talky = 140;
			faceshadow(32, 30, curblink ? curface2 : curface);
			if (talkstate != 2 && lastspeech != 2) {
				celldx = talky - 40;
				if (celldx >= 340)
					celldx = 340;
				faceshadow(32, celldx, heroblink ? heroface2 : heroface);
			} else
				celldx = -1;
			endtext(1);
			putface(32, 30, curblink ? curface2 : curface);
			drawname(32, 30, curunit, 0);
			if (celldx != -1) {
				putface(32, celldx, heroblink ? heroface2 : heroface);
				drawname(32, celldx, 0, 1);
			}
		}
	}
	if (!trademode)
		goto wins;
	for (celldy = 0; celldy < 8; celldy++)
		for (celldx = 0; celldx < 3; celldx++) {
			drawi = celldy * 3 + celldx;
			cellx = celldx * 52 + 26;
			celly = celldy * 52 + 45;
			if (tradekind == 3) {
				if ((cellword = tradegoods[drawi]) != 0) {
					cellword &= 0xfff;
					sqx = cellx;
					sqy = celly;
					sqputo48(cellword);
				}
			} else if (trademode != 3 && trademode != 4
			&& (cellword = tradegoods[drawi]) != 0) {
				if (picked[drawi])
					pillrest(cellx + 1, celly + 1, 50, 50);
				sqx = cellx + 2;
				sqy = celly + 2;
				cellword &= 0xfff;
				sqputo48(cellword);
			}
			if ((cellword = tradeoffer[drawi]) != 0) {
				pillrest(cellx + 205, celly - 21, 50, 50);
				sqx = cellx + 206;
				sqy = celly - 20;
				cellword &= 0xfff;
				sqputo48(cellword);
			}
		}
	if (!((tradehints & 0x20) && (tradehints & 0x40) && hinttime != gametime)
	&& (!goodsbase || !offerbase)) {
		for (drawi = 0; drawi < 24; drawi++)
			if (tradeoffer[drawi])
				goto names;
		if (!goodsbase) {
			centertext(0, 3, strs[0x37a]);
			centertext(1, 3, strs[0x37b]);
			centertext(2, 3, strs[0x37c]);
		} else {
			if (offerbase)
				goto names;
			centertext(0, 3, strs[0x37d]);
			centertext(1, 3, strs[0x37e]);
			centertext(2, 3, strs[0x37f]);
		}
		if (!(tradehints & 0x20)) {
			if (hinttime != gametime) {
				tradehints |= 0x20;
				hinttime = gametime;
			}
		} else if (!(tradehints & 0x40) && hinttime != gametime) {
			tradehints |= 0x40;
			hinttime = gametime;
		}
	}
names:
	if (showtotal || cheat && (keys['W'] & 0x80)) {
		if (goodsbase && trademode != 4) {
			if ((drawi = silvprice(goodsbase))
			&& silvtext(textbuf, drawi)) {
				for (drawp = textbuf; *drawp && *drawp != ' '; drawp++)
					;
				if (*drawp++)
					drawtext2(0, 16, 470, drawp);
			}
		}
		if (offerbase) {
			if ((drawi = silvprice(offerbase))
			&& silvtext(textbuf, drawi)) {
				for (drawp = textbuf; *drawp && *drawp != ' '; drawp++)
					;
				if (*drawp++)
					drawtext2(0, 220, 470, drawp);
			}
		}
	}
	if ((trademode == 1 || trademode == 2) && nforsale == 1 && waresilv
	&& cheat && (keys[0xe2] & 0x80)) {
		wsprintf(namebuf, strs[0x484], waresilv);
		text3w(namebuf);
		celldx = 23;
		text3(0, 156 - text3width / 2, 405, namebuf);
	}
	if (trademode == 3 || trademode == 4 || nforsale == 1) {
		celldy = 0;
		drawi = 0;
		while (speech[drawi] && celldy < 316) {
			if (speech[drawi] == 13) {
				if (speech[drawi + 1] == 0)
					break;
				if (drawi > 0 && speech[drawi - 1] == 13)
					celldy += 10;
				else
					celldy += 20;
			}
			drawi++;
		}
		drawp = speech;
		/* the text's height measured above goes unused: it starts at 144 */
		celldy = 144;
		for (drawline = 0; drawline < 316; ) {
			text3w(drawp);
			text3(0, 104 - text3width / 2, drawline + celldy, drawp);
			for (;;) {
				if (*drawp == 0)
					goto buttons;
				if (*drawp == 13)
					break;
				drawp++;
			}
			if (drawp[-1] == 13)
				drawline += 10;
			else
				drawline += 20;
			drawp++;
		}
	}
buttons:
	if (trademode == 4) {
		button(230, 403, 156, 24, strs[goodsprice ? 0x45d : 0x45e], 0xfe);
		button(230, 438, 156, 24, strs[0x45c], 0xfe);
	} else {
		cellword = goodsprice <= offerprice;
		button(230, 403, 156, 24, strs[cellword && goodsprice && offerprice
			? offerprice == 0xffff ? 0x35b : 0x357
			: offerprice == 0xffff ? 0x35c : 0x35a], 0xfe);
		button(230, 438, 156, 24, strs[0x358], 0xfe);
		if (talkactive && !tradedone) {
			// PORT: the integer as a pointer through intptr_t, which 64 bits widen
#ifdef PORT
			drawp = (char *)(intptr_t)tradedone;
#else
			drawp = (char *)tradedone;
#endif
			if (goodsprice == 0 || offerprice == 0xffff)
				drawp = strs[0x35e + dutalk];
			else if (!offerprice) {
				if (trademode != 3 && trademode != 4 && nforsale != 1)
					drawp = strs[0x360 + dutalk];
			} else {
				switch (balance(offerprice, goodsprice)) {
				case 0x5b:
					drawp = strs[0x362 + dutalk];
					break;
				case 0x5c:
					drawp = strs[0x364 + dutalk];
					break;
				case 0x5d:
					drawp = strs[0x366 + dutalk];
					break;
				case 0x5e:
					drawp = strs[0x368 + dutalk];
					break;
				case 0x20:
				even:
					drawp = strs[0x36a + dutalk];
					break;
				case 0:
					if (!cellword)
						goto even;
					drawp = strs[0x36c + dutalk];
					break;
				case 0x21:
					if (!cellword)
						goto even;
					drawp = strs[0x36e + dutalk];
					break;
				case 0x5f:
					if (!cellword)
						goto even;
					drawp = strs[0x370 + dutalk];
					break;
				case 0x60:
					if (!cellword)
						goto even;
					drawp = strs[0x372 + dutalk];
					break;
				case 0x61:
					if (!cellword)
						goto even;
					drawp = strs[0x374 + dutalk];
					break;
				case 0x62:
					if (!cellword)
						goto even;
					drawp = strs[0x376 + dutalk];
					break;
				}
			}
			if (drawp) {
				wrap(drawp, 85);
				celldy = 0;
				for (; *drawp; celldy++) {
					for (; *drawp == 13; drawp++)
						celldy++;
					if (*drawp == 0)
						break;
					text3w(drawp);
					celldx = 190 - text3width;
					if (celldx < 8)
						celldx = 8;
					text3(0, celldx, celldy * 23 + 31, drawp);
					for (; *drawp && *drawp != 13; drawp++)
						;
					if (*drawp == 13)
						drawp++;
				}
			}
		}
	}
	switch (lastbutton = hotbutton) {
	case 1:
		celldy = 403;
		break;
	case 2:
		celldy = 438;
		break;
	default:
		goto wins;
	}
	pillshade9(230, celldy, 156, 24);
wins:
	for (drawi = 15; drawi >= 0; drawi--) {
		if (panelstack[drawi] == 0xff)
			continue;
		drawpn = panelstack[drawi];
		drawpanel = &panels[drawpn];
		// PORT: a panel stands where the wide screen's layout puts it
#ifdef PORT
		sysdrawx = panelshift(drawpn);
#endif
		switch (drawpanel->type) {
		case 1:
			drawinventory(drawpn);
			break;
		case 3:
			drawstore(drawpn);
			break;
		case 0xc:
			winframe(drawpanel->x, drawpanel->y, panels[12].w, panels[12].h, 0);
			drawboxes(drawpanel->x, drawpanel->y);
			break;
		case 0xe:
			putwint(drawpanel->x, drawpanel->y, 19);
			wsprintf(numtext, texts + 0xb324, drawpanel->id, drawpanel->arg);
			text3w(numtext);
			text3(0, drawpanel->x + 4 + (144 - text3width) / 2, drawpanel->y + 6, numtext);
			celldy = drawpanel->y + 28;
			celldx = drawpanel->arg;
			if (celldx)
				celldx = drawpanel->id * 104 / celldx;
			celldx += drawpanel->x + 23;
			if (drawpanel->arg)
				shade2(drawpanel->x + 23, celldy, celldx - (drawpanel->x + 23) + 1, 18);
			break;
		}
	}
	// PORT: as above
#ifdef PORT
	sysdrawx = 0;
#endif
	condgauge = 0;
	if (!talkactive && !statshown) {
		cellidx = 0;
		if (hoverx <= 80 && hovery >= 384)
			;
		else if (units[0].asleep)
			cellidx = units[0].asleep;
		else if (units[0].berserk)
			cellidx = units[0].berserk;
		else if (units[0].bewitched)
			cellidx = units[0].bewitched;
		else if (units[0].fleeing)
			cellidx = units[0].fleeing;
		else if (units[0].confused)
			cellidx = units[0].confused;
		if (cellidx) {
			if (cellidx > 128)
				cellidx = 128;
			// PORT: the gauge stands at the wide screen's left edge
#ifdef PORT
			sysdrawx = hudleft();
			putgauge(0, 8, 430, (128 - cellidx) * 100 / 128);
			sysdrawx = 0;
#else
			putgauge(0, 8, 430, (128 - cellidx) * 100 / 128);
#endif
			condgauge = 1;
		}
		// PORT: the hero's states stand at the wide screen's right edge
#ifdef PORT
		sysdrawx = hudright();
		putherostates(0, 608, 460, 0, 1);
		sysdrawx = 0;
#else
		putherostates(0, 608, 460, 0, 1);
#endif
	}
	if (tipshown() || nmsgs)
		drawmsgs();
	if (screenhook)
		screenhook();
	if (statshown == 1)
		drawstats();
	if (barshown)
		drawmenubar();
	if (drawto2)
		backto2();
	if (!noflip) {
		if (shown)
			showcursor();
		// PORT: the pointer roams a play frame of the wide screen
#ifdef PORT
		playflip = !talkactive && !trademode;
#endif
		flip();
		setlight();
		readinput();
	}
}

unsigned char caretcols[72] = {
	231, 231, 231, 231, 232, 232, 233, 233, 234, 234, 235, 235, 236, 236,
	237, 237, 237, 237, 238, 238, 238, 238, 237, 237, 236, 236, 235, 235,
	234, 234, 233, 233, 232, 232, 231, 231, 231, 231, 231, 231, 232, 232,
	233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 237, 237, 238, 238,
	238, 238, 237, 237, 236, 236, 235, 235, 234, 234, 233, 233, 232, 232,
	231, 231
};
int caretunused = -1;
short textbg = 1;
short pageturn = 1;
unsigned char cellsight;
unsigned char topsight;
char *dnname;
Panel *drawpanel;
int drawpn;
short drawi;
short drawline;
char *drawp;
Unit *decounit;
int decoframe;
int decophase;
int subscrollx;
int subscrolly;
unsigned long cellword;
unsigned long tile;	/* 00a532a0 */
unsigned long *cellitem;
unsigned short cellidx;
short cellx;
short celly;
short celldx;
short celldy;
short screenposx;
short screenposy;
short posunused;
short posunused2;
short blankline;
unsigned char showtotal;
unsigned char tradehints;
unsigned long hinttime;
int hintunused[3];
char numtext[24];

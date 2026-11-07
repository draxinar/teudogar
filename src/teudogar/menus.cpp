#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

#pragma pack(1)

typedef struct Rect Rect;
typedef struct Menu Menu;
typedef struct Unit Unit;
typedef struct Game Game;
typedef struct Action Action;
typedef struct Panel Panel;
typedef struct Zone Zone;

struct Rect {
	unsigned short x;
	unsigned short y;
	unsigned short w;
	unsigned short h;
};

/* 0x1f7 bytes: up to 40 items, numbered from 1 in sel */
struct Menu {
	unsigned short n;
	unsigned char enabled[40];
	unsigned char keys[40];
	unsigned char keys2[10];
	Rect rects[40];
	short ids[40];
	unsigned char color;
	void (*fn)(short, short);
	short sel;
	short keysel;
	short over;
};

/* 0x1b8 bytes each (009bc240) */
struct Unit {
	char f0[8];
	unsigned char pose;
	unsigned char mode;
	char fa[0xed - 0xa];
	short strength;
	short dexterity;
	short intellect;
	short health;
	short vigor;
	char ff7[0x104 - 0xf7];
	unsigned long head;
	unsigned long body;
	unsigned long hand;
	unsigned long offhand;
	unsigned long legs;
	char f118[0x13c - 0x118];
	char bleeding;
	char f13d[0x1b8 - 0x13d];
};

/* 0x5f8 bytes (009cc470) */
struct Game {
	char f0[0x16];
	char known[0x1e];
	char flags[0xc8];
	char ffc[0x176 - 0xfc];
	unsigned char tiredlevel;
	char hungercount[2];
	unsigned char hungerlevel;
	char f17a[0x5f8 - 0x17a];
};

/* 0x1a bytes each (004c3d68) */
struct Action {
	unsigned short name;
	unsigned short icon;
	unsigned char rowbreak;
	char f5;
	unsigned short cmd;
	unsigned short target;
	unsigned short cost;
	unsigned short x;
	unsigned short y;
	unsigned short turns;
	unsigned short state;
	unsigned short hostile;
	unsigned short where;
	char f18[2];
};

/* 0x28 bytes each (00a4ca78): the screen's panels */
struct Panel {
	char f0[6];
	int x;
	int y;
	int w;
	int h;
	unsigned short id;
	unsigned short arg;
	char f1a[0x28 - 0x1a];
};

/* 0x22 bytes each (004f9498): the zones of the map */
struct Zone {
	short id;
	short name;
	char f4[0x22 - 4];
};

#pragma pack()

void s2toback(void);
int flip(void);
int pillshade8(int x, int y, int w, int h);
int pillshade9(int x, int y, int w, int h);
short muted(void);
void playnow(short n);
int __cdecl pollinput(void);
int __cdecl readinput(void);
void stepcaret(void);
int animface(void);
int setcursor(int kind, int img, int kind1, int img1, int kind2, int img2);
int fadeslow(int in);
int fadefast(int in);
char *pick(char *de, char *en, char *fr, char *es, char *it, char *pl);
void buttonon(int x, int y, int w, int h, char *s);
void buttonoff(int x, int y, int w, int h, char *s);
int text3w(char *s);
DWORD savebmpf(char *name, int n, unsigned char *pal);
int drawtext(int n, int x, int y, char *s);
void setcursor1(int kind, int img);
void redrawcursor(void);
void pgtoback(void);
/* pgtoback under a second name, which keeps menupage2 apart from menupage */
void pgtoback2(void);
#pragma aux pgtoback2 "W?pgtoback$n()v"
void cleartexts(void);
void usepal(unsigned char n);
int text3(int n, int x, int y, char *s);
int text3grey(int n, int x, int y, char *s);
int text3grey2(int n, int x, int y, char *s);
void puthands(void);
void backto2(void);
void pagedone(void);
void drawcursor(void);
void turnpage(int a, int b, int c, int d);
int needredraw(int c);
void drawscreen(void);
void gametick(void);
void buttonarr(int x, int y, int w, int h, char *s);
void nop(void);
int putwint(int x, int y, int idx);
char *prevbutton(void);
char *cancelbutton(void);
char *nextid(short n);
void hidecursor(void);
void inkshade(void);
int put48t0(int x, int y, char *src);
char *daytime(unsigned char c);
int dayicon(char *s);
void putcs(int x, int y, unsigned n);
char *roman(DWORD n);
int textwidth(char *s);
void drawtext2o(int n, unsigned off, char *s);
short mapzone(short n);
void statustext(char *buf, Unit *u);
char *hungertext(unsigned char c);
char *tiredtext(unsigned char c);
int putsrc(int n, int x, int y, int kind, int idx);
int statbar(short v);
short roundto(short v, short step);
unsigned long baselook(unsigned long item);
unsigned char classof(int i);
int gearicon(unsigned long item);
void attackval(Unit *u, short *dmg, short *kind, short *a, short *b, short n);
void defenseval(Unit *u, short *a, short *b, short *c);
short protection(unsigned long item);
void putpixoff(unsigned off, unsigned char c);
void putpix2(int n, int x, int y, unsigned char c);
int lumadd(int x, int y, int w, int h, unsigned char add);
short nearfoe(int n);
int takeberserk(int a);
int drinkpotion(unsigned long item, int take);
void winframe(int x, int y, int w, int h, int keyed);
short unconscious(void);
void syncplayer(void);
int tipshown(void);
void dropheld(void);
void primtoback(void);
int wrap(char *s, int width);
int stopmusic(void);
void quit(void);
int playerout(void);
void mainmenu(void);
void charsheet(Unit *u, short mode);
void togglecombat(short n);
short denymsg(char *fmt, ...);
void markers(int param);
void journal(void);
void docmenu(void);
void travelmap(int mode, short a, short b);
int isopen(unsigned char n);
void closepanel(unsigned char n);
void openhero(void);
int doaction(short flag);
int keyhit(void);
void openpanel(unsigned char n);
void movepanel(unsigned char n, short x, short y);
void setclip(short x0, short y0, short x1, short y1);
void defclip(void);
void dragpanel(unsigned char n);
char *itemname(unsigned long item);
void denycombat(void);
void denyfoes(void);
void prependmsg(char *s);
int intoxicated(Unit *u);
int sanctioned(short m, int k, int x, int y, unsigned long item);
void notetarget(void);
void delmsgid(short id);
int canperform(Unit *u, short cmd, int n, int a, int c);
short msgf(short prio, char *fmt, ...);
int busyuntil(unsigned long t, short force);
int perform(Unit *u, short cmd, int n, int a, int c, short force);
short nosuccess(char *s);
void fillgauges(void);
void lookaround(void);
short askmsg(char *s);
void keeplast(void);
int asktarget(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, char cls10, char ctl, char cls15, char p9, char p10);
short asksteal(short m);
short sparring(Unit *u, Unit *v);
void denypractice(void);
short askattack(void);
unsigned short findclass(int cell, unsigned char kind);
int isfood(unsigned long item);
void waitmouseup(short wait);
void blitto(int b, int dx, int dy, int a, int x, int y, int w, int h);
void pgsides(void);
void inkwhite(void);
void text2o(int n, unsigned off, char *fmt, ...);
int fademed(int in);
int showscr(void);
void dissolvet(void);
int music(int n, int lp);
int loadscr(int id, unsigned char *dst, unsigned char *pal, void *buf, DWORD size);
void keepui(char *pal);
void clrback(void);
int getscreen(int n, char *dst);
void setkeymode(int v);
int save2(void);
int putrows(unsigned char *buf, int y, int h);
void text3e1(int n, int x, int y, char *s);
void setcaret(short x, short y);
void backto2rows(int y, int dy, int h);
void drawcaret(void);
unsigned char readkey(void);
void stopcaret(void);
long evalvar(char *s);
void textn(int n, int x, int y, char *fmt, ...);
int waitrelease(void);
int showscrt(void);
int save0(void);
void blitn(int a, int x, int y, int b, int dx, int dy, int w, int h);
char *titletail(void);
void text3e2(int n, int x, int y, char *s);
int putblockt(int n, int x, int y, unsigned char *buf, int bx, int by, int w, int h);
void text32c(int n, int x, int y, char *s);
void loadbigfont(void);

extern int mousex;
extern int mousey;
extern int quitting;
extern unsigned char esckey;
extern char quitprompt;
extern void (*menutick)(int);
extern void (*menudraw)(void);
extern DWORD ticknext;
extern DWORD hovertime;
extern int tickms;
extern char menusounds;
extern int mousemoved;
extern int mouseleft;
extern int mouseright;
extern char keycode;
extern char lastchar;
extern int surfslost;
extern int waitforup;
extern int leftclick;
extern int rightclick;
extern int hoverx;
extern int hovery;
extern unsigned char palblack;
extern char menuslow;
extern short menusnd;
extern short menuanim;
extern unsigned short pagearrows;
extern char texts[47942];
extern int savesready;
extern unsigned short shotno;
extern unsigned char menumode;
extern short pagefx;
extern short listw;
extern short listtop;
extern short listbright;
extern char *strs[2300];
extern char printbuf[];
extern int btnfull;
extern int clicktime;
extern int presslatch;
extern int dblclick;
extern char lastscan;
extern int tracesite;
extern int playmode;
extern Unit units[150];
extern short pagebright;
extern char *btntext[];
extern char empty[1];
extern int waitlost;
extern void (*waithook)(void);
extern Menu menu1;
extern Menu barmenu;
extern char namebuf[2500];
extern char textbuf[];
extern char tmpbuf[2500];
extern char progdir[];
extern int lang;
extern char langchar;
extern short nofoes;
extern int shown;
extern int cheat;
extern unsigned char keys[256];
extern unsigned char gamehour;
extern char mapstate[100];
extern short gameday;
extern unsigned short curmap;
extern Zone zones[];
extern char gauges[9][2304];
extern char romanbuf[];
extern Game game;
extern int noprinter;
extern char printpage[];
extern char msgline[1000];
extern short printchoice;
extern char scrbuf[309700];
extern unsigned char actsel;
extern Action actions[];
extern short foesnear;
extern unsigned char actcols;
extern unsigned char quickacts[];
extern int text3width;
extern unsigned short steptick;
extern unsigned short stepframe;
extern short nmsgs;
extern char packbuf[];
extern char soundsok;
extern char inedge;
extern unsigned long gametime;
extern int hoverkind;
extern unsigned char wascalm;
extern unsigned char wasable;
extern char talkactive;
extern unsigned char indoinput;
extern short popupdelay;
extern int statshown;
extern char trademode;
extern unsigned short talkstate;
extern unsigned char choicesh;
extern short talky;
extern unsigned short choicesr;
extern char combatcause;
extern int barshown;
extern short barsel;
extern char workbuf[];
extern unsigned char panelstack[16];
extern int hoverpx;
extern int hoverpy;
extern int hovercx;
extern int hovercy;
extern Panel panels[];
extern unsigned long targetitem;
extern int doactclear;
extern short lastfail;
extern unsigned long acttime;
extern short askslot;
extern short newmsgs;
extern short stealmsg;
extern short attackmsg;
extern short actionturn;
extern int targetx;
extern int targety;
extern int targetidx;
extern unsigned char targetkind;
extern unsigned long mapitems[];
extern char *textlines[];
extern short pageturn;
extern int turnsides;
extern short overpic;
extern short curtopic;
extern short ntextlines;
extern short pagex;
extern short pagemaxh;
extern short startpage;
extern short nofade;
extern short textsound;
extern short textsong;
extern int nstrs;
extern char chcut[200];
extern char gamepal[768];
extern int inkeep;
extern short (*inkeyfn)(char c);
extern void (*infieldfn)(short field);
extern void (*insetfn)(short field, char **s, short *max);
extern short fieldh;
extern unsigned char pals[10][768];
extern int intropage;
extern int turnunused;
extern unsigned char firststart;
extern int loop;
extern char picbuf[];
extern int text3x;
extern char tabkey;

extern short aliasi;
extern short stepi;
extern short menuoveri;
extern short hotkeyi;
extern short oldsel;
extern short menuk;
extern short clicked;
extern int keyx;
extern int keyy;
extern char key;
extern short savecur;
extern short lastcur;
extern DWORD esctime;
extern short savesel;
extern short savekeysel;
extern char *cornertext;
extern char shotname[16];
extern short listi;
extern short listy;
extern short listlines;
extern short listback;
extern char *listtitle;
extern short texth;
extern short pagek;
extern short pagen;
extern short rowwidth;
extern short pagey;
extern short pagelines;
extern short pagefirst;
extern short pagey0;
extern char *pagestart;
extern char *pagep;
extern char *pageq;
extern char pagekey;
extern unsigned char btnrow;
extern short pagebullet;
extern char msgesc;
extern short msgres;
extern char *urlp;
extern short printres;
extern short printbtn;
extern int printn;
extern HANDLE printfile;
extern char printerr[1000];
extern char printcur;
extern short paney;
extern short statatk;
extern short statv;
extern short statblk;
extern short statparry;
extern short statdmin;
extern short statdmax;
extern short statkind;
extern short cutword;
extern int staticon;
extern char *stattime;
extern short statmode;
extern char statshield;
extern short baritem;
extern short labelx;
extern short acttipx;
extern short acttipy;
extern int actcol;
extern int actrow;
extern int acti;
extern short actj;
extern unsigned short actk;
extern char edgetop;
extern char edgebot;
extern char edgeleft;
extern char edgeright;
extern DWORD topsince;
extern DWORD botsince;
extern DWORD leftsince;
extern DWORD rightsince;
extern int numok;
extern int numx;
extern int numy;
extern int numunused;
extern int numslide;
extern unsigned short *numval;
extern char numchanged;
extern unsigned char numctl;
extern DWORD numnext;
extern DWORD numdelay;
extern int numhkind;
extern int numhcx;
extern int numhcy;
extern int numhpx;
extern int numhpy;
extern unsigned char tgtopen;
extern unsigned char tgtperson;
extern unsigned char tgtcorpse;
extern unsigned char tgtpack;
extern unsigned char tgtself;
extern unsigned char tgtclass;
extern unsigned long acta;
extern int actb;
extern unsigned long actc;
extern int actres;
extern int actcmd;
extern unsigned int actturns;
extern char *actprompt;
extern char *acthint;
extern short actmsg;
extern short actcalm;
extern short promptmsg;
extern char *topicq;
extern DWORD turnwait;
extern int turnstep;
extern unsigned short textidx;
extern char *pagestarts[50];
extern short curpage;
extern short npages;
extern short pgkey;
extern short slide;
extern char *pagetext;
extern unsigned char pagemode;
extern unsigned char menukey;
extern DWORD waitstart;
extern int textresult;
extern int fnresult;
extern char atlast;
extern char slidein;
extern char pastfirst;
extern char firstshow;
extern char *picp;
extern short curpos;
extern short inputi;
extern short inj;
extern short inlen;
extern short infield;
extern short inxend;
extern short inx0;
extern short nkeys;
extern short undopos;
extern char inkey;
extern char inscan;
extern char inbuf[100];
extern char undobuf[100];
extern int inresult;
extern int inx;
extern int iny;
extern char varbuf[40];
extern int varres;
extern char numdigits[16];
extern char numfmt[10];
extern unsigned char numkey;
extern int numlen;
extern int numres;
extern short numfresh;
extern unsigned long titlei;
extern int titleesc;

void
quitnow(void)
{
	quitting = 1;
	esckey = 0xff;
	quitprompt += 2;
}

int
winclosed(void)
{
	if (quitting == 2) {
		quitnow();
		return 1;
	}
	return 0;
}

void
tickmenu(void)
{
	if (menutick == NULL)
		return;
	if (ticks() <= ticknext)
		return;
	menutick(1);
	ticknext = tickms + ticks();
}

short
aliased(Menu *m, short i)
{
	if (!m->enabled[i])
		return 0;
	for (aliasi = 0; aliasi < m->n; aliasi++)
		if (aliasi != i && m->enabled[aliasi] && m->ids[aliasi] == i + 1)
			return 1;
	return 0;
}

void
menubase(void)
{
	if (menudraw)
		menudraw();
	s2toback();
	if (menutick)
		menutick(0);
}

void
hilite(Menu *m, short i)
{
	menubase();
	if (i) {
		if (m->fn)
			m->fn(i - 1, 1);
		else if (m->color == 0 || m->color == 0xf4)
			pillshade8(m->rects[i - 1].x, m->rects[i - 1].y, m->rects[i - 1].w, m->rects[i - 1].h);
		else
			pillshade9(m->rects[i - 1].x, m->rects[i - 1].y, m->rects[i - 1].w, m->rects[i - 1].h);
	} else if (m->fn)
		m->fn(0xff, 0);
	flip();
}

short
stepitem(Menu *m, short i, short step, char wrap)
{
	for (stepi = 0; stepi < m->n * 2; stepi++) {
		i += step;
		if (i < 1) {
			if (wrap)
				i = m->n;
			else {
				i = 1;
				step = -step;
			}
		} else if (i > m->n) {
			if (wrap)
				i = 1;
			else {
				i = m->n;
				step = -step;
			}
		}
		if (m->enabled[i - 1])
			return i;
	}
	return 0;
}

void
menuhover(Menu *m)
{
	m->over = 0;
	for (menuoveri = 0; menuoveri < m->n; menuoveri++)
		if (m->enabled[menuoveri]
		&& mousex >= m->rects[menuoveri].x && mousex < m->rects[menuoveri].x + m->rects[menuoveri].w
		&& mousey >= m->rects[menuoveri].y && mousey < m->rects[menuoveri].y + m->rects[menuoveri].h) {
			m->over = menuoveri + 1;
			return;
		}
}

short
hotkey(Menu *m, char key)
{
	for (hotkeyi = 0; hotkeyi < m->n; hotkeyi++) {
		if (!m->enabled[hotkeyi])
			continue;
		if (m->keys[hotkeyi] == 0x1c && m->sel)
			continue;
		if (key == m->keys[hotkeyi] || m->keys[hotkeyi] == 0x15
		&& (key == 0x2c || key == 0x24 || key == 0x18 || key == 0x1f || key == 0x14)) {
			m->sel = hotkeyi + 1;
			hilite(m, m->sel);
			return 1;
		}
	}
	if (key != 0x1c)
		for (hotkeyi = 0; hotkeyi < m->n; hotkeyi++)
			if (m->enabled[hotkeyi] == 0 && key == m->keys[hotkeyi])
				return 2;
	return 0;
}

void
menusound(short sound)
{
	if (menusounds)
		if (!muted())
			playnow(sound);
}

void
clicksound(void)
{
	menusound(0x2e);
}

void
hoversound(void)
{
	if (ticks() < hovertime + 250)
		return;
	menusound(0x4c);
	hovertime = ticks();
}

void
pagesound(void)
{
	menusound(0x18);
}

short
runmenu(Menu *m, int (*idle)(void), short (*keyfn)(unsigned char, unsigned char), short cancel)
{
	oldsel = 0;
	clicked = 0;
	savecur = pagearrows;
	lastcur = pagearrows;
	keyx = 0;
	keyy = 0;
	ticknext = 0;
	hilite(m, m->sel);
	mousemoved = 1;
	waitforup = 1;
	pollinput();
	if (surfslost) {
	restored:
		leftclick = 0;
		rightclick = 0;
		waitforup = 0;
		pagearrows = savecur;
		return 0xfd;
	}
	if (esckey != 0xff && esckey != 0xfe)
		esckey = 0;
	while (mouseleft || mouseright || keycode) {
		if (idle && idle()) {
			leftclick = 0;
			rightclick = 0;
			waitforup = 0;
			pagearrows = savecur;
			return 0xfe;
		}
		readinput();
		stepcaret();
		tickmenu();
	}
	if (esckey == 0xfe) {
	again:
		esckey = 0;
		esctime = ticks();
		do {
			readinput();
			stepcaret();
			tickmenu();
			if (winclosed())
				goto done;
		} while (since(esctime) < 200);
		if (esckey == 0xff)
			goto again;
		esckey = 0;
	}
done:
	leftclick = 0;
	rightclick = 0;
	waitforup = 0;
	if (esckey != 0xff)
		esckey = 0;
	for (;;) {
		if (m->sel) {
			pagearrows = 0;
			setcursor(0, 0xffff, 0, 0, 0, 0);
		} else
			pagearrows = savecur;
		if (pagearrows != lastcur) {
			lastcur = pagearrows;
			mousemoved = 1;
		}
		readinput();
		if (surfslost)
			goto restored;
		stepcaret();
		tickmenu();
		menuhover(m);
		if (keyx == 0 && keyy == 0) {
			keyx = hoverx;
			keyy = hovery;
		} else if (hoverx != keyx || hovery != keyy)
			m->keysel = 0;
		oldsel = m->sel;
		if (m->keysel)
			m->sel = m->keysel;
		else
			m->sel = m->over;
		if (m->sel != oldsel || menuanim && animface()) {
			if (m->sel != oldsel && m->sel)
				hoversound();
			hilite(m, m->sel);
			oldsel = m->sel;
		}
		if (!clicked) {
			if (mouseleft) {
				if (m->keysel) {
					m->keysel = 0;
					m->sel = m->over;
					if (m->sel != oldsel) {
						hilite(m, m->sel);
						oldsel = m->sel;
					}
				}
				if (m->sel) {
					clicksound();
					clicked = 1;
				} else if (cancel)
					goto cancelled;
			}
		} else if (!mouseleft) {
			if (m->sel)
				goto chosen;
			clicked = 0;
		}
		if (palblack) {
			if (menuslow)
				fadeslow(1);
			else
				fadefast(1);
			if (menusnd)
				playnow(menusnd);
		}
		pollinput();
		if (surfslost)
			goto restored;
		if (keycode || esckey == 1) {
			key = esckey ? 1 : keycode;
			if (idle && idle()) {
				pagearrows = savecur;
				return 0xfe;
			}
			if (keyfn) {
				menuk = keyfn(key, lastchar);
				if (menuk) {
					if (menuk == 2) {
						hilite(m, m->sel);
						if (palblack) {
							if (menuslow)
								fadeslow(1);
							else
								fadefast(1);
							if (menusnd)
								playnow(menusnd);
						}
					} else if (menuk == 3) {
						pagearrows = savecur;
						return 0xff;
					}
					goto next;
				}
			}
			for (menuk = 0; menuk < sizeof m->keys2 && m->keys2[menuk]; menuk++)
				if (m->keys2[menuk] == key) {
				escape:
					pagearrows = savecur;
					clicksound();
					return 0;
				}
			switch (hotkey(m, key)) {
			case 1:
				goto select;
			case 2:
				goto next;
			}
			if (esckey)
				goto escape;
			switch (key) {
			case 0x1c:
			case 0x39:
				if (m->sel || cancel) {
				select:
					clicksound();
				chosen:
					pagearrows = savecur;
					if (m->ids[m->sel - 1])
						return m->ids[m->sel - 1];
					return m->sel;
				}
				break;
			case 0x48:
			case 0x4b:
				if (m->keysel == 0) {
					if (m->sel == 0)
						m->keysel = 2;
					else
						m->keysel = m->sel;
				}
				m->keysel = stepitem(m, m->keysel, -1, 1);
				break;
			case 0x4d:
			case 0x50:
				if (m->keysel == 0) {
					if (m->sel == 0)
						m->keysel = m->n - 1;
					else
						m->keysel = m->sel;
				}
				m->keysel = stepitem(m, m->keysel, 1, 1);
				break;
			case 0x47:
				m->keysel = stepitem(m, 2, -1, 0);
				break;
			case 0x4f:
				m->keysel = stepitem(m, m->n - 1, 1, 0);
				break;
			}
		next:
			if (m->sel != oldsel) {
				hilite(m, m->sel);
				oldsel = m->sel;
			}
			keyx = hoverx;
			keyy = hovery;
			pollinput();
			if (surfslost)
				goto restored;
			while (keycode) {
				readinput();
				stepcaret();
				tickmenu();
			}
		}
		if (idle && idle()) {
			pagearrows = savecur;
			return 0xfe;
		}
		if (winclosed() || esckey || mouseright)
			goto cancelled;
	}
cancelled:
	clicksound();
	pagearrows = savecur;
	return 0;
}

void
clearmenu(Menu *m)
{
	savesel = m->sel;
	savekeysel = m->keysel;
	memset(m, 0, sizeof *m);
	m->sel = savesel;
	m->keysel = savekeysel;
}

void
cornerrect(Menu *m, short i)
{
	m->rects[i].x = 404;
	m->rects[i].y = 439;
	m->rects[i].w = 130;
	m->rects[i].h = 24;
}

void
addcorner(Menu *m, short i, short kind)
{
	cornerrect(m, i);
	m->enabled[i] = 1;
	m->n++;
	if (kind == 1)
		cornertext = pick(texts + 0x6c8c, texts + 0x6c95, texts + 0x6ca0, texts + 0x6ca1, texts + 0x6ca2, texts + 0x6cad);
	else if (kind == 2)
		cornertext = pick(texts + 0x16c, texts + 0x173, texts + 0x178, texts + 0x179, texts + 0x17a, texts + 0x183);
	else
		cornertext = pick(texts + 0x6cae, texts + 0x6cb7, texts + 0x6cbe, texts + 0x6cbf, texts + 0x6cc0, texts + 0x6ccb);
	buttonon(m->rects[i].x, m->rects[i].y, m->rects[i].w, m->rects[i].h, cornertext);
}

void
fitbutton(Menu *m, short i, int x, int y, char *s, short right)
{
	short w;

	w = text3w(s) + 14;
	m->rects[i].w = w;
	m->rects[i].h = 24;
	if (right)
		m->rects[i].x = x - m->rects[i].w;
	else
		m->rects[i].x = x;
	m->rects[i].y = y;
}

void
addbutton(Menu *m, short i, int x, int y, char *s, short right)
{
	fitbutton(m, i, x, y, s, right);
	m->enabled[i] = 1;
	m->n++;
	buttonon(m->rects[i].x, m->rects[i].y, m->rects[i].w, m->rects[i].h, s);
}

void
screenshot(void)
{
	if (savesready == 0 || shotno > 999)
		return;
	wsprintf(shotname, texts + 0xa4b1, shotno);
	savebmpf(shotname, 1, 0);
	drawtext(1, 8, 0x1cc, shotname);
	shotno++;
}

void
entermenu(void)
{
	menumode = 1;
	setcursor1(3, 3);
	redrawcursor();
}

void
menupage(void)
{
	pgtoback();
}

void
menupage2(void)
{
	pgtoback2();
}

void
blankpage(void)
{
	cleartexts();
	if (pagefx != 2 && pagefx != 3) {
		if (pagefx == 0x20)
			fadeslow(0);
		else
			fadefast(0);
		usepal(0);
	}
	pgtoback();
}

void
blankpage2(void)
{
	cleartexts();
	if (pagefx != 2 && pagefx != 3) {
		if (pagefx == 0x20)
			fadeslow(0);
		else
			fadefast(0);
		usepal(0);
	}
	pgtoback();
}

void
itemtext(Menu *m, char **items, short i)
{
	if (listbright)
		text3(0, 0xffff, m->rects[i].y + 3, items[i + 1]);
	else
		text3grey(0, 0xffff, m->rects[i].y + 3, items[i + 1]);
}

void
listmenu(Menu *m, unsigned short n, char **items)
{
	memcpy(printbuf, m->enabled, sizeof m->enabled);
	memset(m, 0, sizeof *m);
	memcpy(m->enabled, printbuf, sizeof m->enabled);
	m->n = n - 1;
	m->color = 0xf4;
	if (items[n - 1] == strs[0x22a]) {
		listback = 1;
		m->n--;
	} else
		listback = 0;
	for (listi = 0; listi < m->n; listi++)
		if (m->enabled[listi] == 0xff)
			m->enabled[listi] = 0;
		else
			m->enabled[listi] = 1;
	if (listtop) {
		listy = listtop;
		listtop = 0;
	} else if (m->n >= 8) {
		m->n = 8;
		listy = 120;
	} else
		listy = (280 - m->n * 40) / 2 + 140;
	if (items[0]) {
		listi = 60;
		listtitle = items[0];
		if (listbright)
			text3(0, 0xffff, 60, listtitle);
		else
			text3grey2(0, 0xffff, 60, listtitle);
		listlines = 0;
		listtitle = items[0];
		do {
			while (*listtitle && *listtitle != 13)
				listtitle++;
			if (*listtitle == 13) {
				listtitle++;
				if (*listtitle == 10)
					listtitle++;
				listlines++;
				if (listbright)
					text3(0, 0xffff, listi + listlines * 20, listtitle);
				else
					text3grey(0, 0xffff, listi + listlines * 20, listtitle);
			}
		} while (*listtitle);
	}
	if (listbright)
		btnfull = 1;
	for (listi = 0; listi < m->n; listi++) {
		m->rects[listi].x = 320 - listw / 2;
		m->rects[listi].y = listy - 5;
		m->rects[listi].w = listw;
		m->rects[listi].h = 30;
		if (items[listi + 1] && *items[listi + 1] != 0) {
			if (m->enabled[listi])
				buttonon(m->rects[listi].x, m->rects[listi].y, m->rects[listi].w, m->rects[listi].h, items[listi + 1]);
			else
				buttonoff(m->rects[listi].x, m->rects[listi].y, m->rects[listi].w, m->rects[listi].h, items[listi + 1]);
		}
		listy += 40;
	}
	if (listback)
		addcorner(m, m->n, 0);
	btnfull = 0;
	if (!listbright)
		puthands();
	backto2();
	pagedone();
	entermenu();
	drawcursor();
	if (palblack) {
		flip();
		pagesound();
		if (pagefx == 0x20)
			fadeslow(1);
		else
			fadefast(1);
	} else if (pagefx == 0)
		flip();
	else
		turnpage(pagefx == 2 ? 1 : 0, 0, 480, 0);
}

void
resetinput(void)
{
	waitforup = 0;
	leftclick = 0;
	rightclick = 0;
	clicktime = 0;
	presslatch = 1;
	dblclick = 0;
	mouseleft = 0;
	mouseright = 0;
	keycode = 0;
	lastscan = 0;
	if (esckey != 0xff)
		esckey = 0;
	redrawcursor();
	mousemoved = 1;
}

void
waitrelease(short anim)
{
	if (pagearrows) {
		pagearrows = 0;
		setcursor(0, 0xffff, 0, 0, 0, 0);
	}
	menumode = 0;
	redrawcursor();
	mousemoved = 1;
	waitforup = 1;
	if (!quitting) {
		tracesite = 40;
		pollinput();
		while (mouseleft || mouseright || keycode) {
			if (anim && playmode == 1 && units[0].health > 0)
				gametick();
			else {
				readinput();
				if (needredraw(0xfd))
					drawscreen();
			}
		}
	}
	waitforup = 0;
	resetinput();
	readinput();
}

void
leavemenu(short anim)
{
	menumode = 0;
	mousemoved = 1;
	waitforup = 1;
	drawcursor();
	if (!quitting) {
		drawscreen();
		if (palblack)
			fadefast(1);
	}
	waitrelease(anim);
}

void
empty1(void)
{
}

short
drawpage(char **pp, unsigned short x, short mode, short top, short maxh, Menu *m)
{
	if (m)
		memset(m, 0, sizeof *m);
	pagestart = *pp;
	while (**pp == 13 || **pp == 10)
		(*pp)++;
	pagep = *pp;
	texth = 0;
	pagelines = 0;
	if (*pagep == '/' && pagep[1] == '/' && pagep[2] == '[' && m) {
		pagelines = 1;
		while (*pagep && *pagep++ != 13)
			;
	} else if (*pagep == '/' && pagep[1] == '[' && m) {
		pagelines++;
		while (*pagep && *pagep++ != 13)
			;
	}
	do {
		if (*pagep == 13) {
			if (pagep == pagestart)
				goto add20;
			if (pagep[1] == '/' && pagep[2] == '/' && pagep[3] == '[' && m)
				pagelines++;
			else if (pagep[1] == '/' && pagep[2] == '[' && m)
				pagelines++;
			else
				switch (pagep[-1]) {
				case 13:
					texth += 10;
					pagelines++;
					break;
				case '+':
					goto done;
				default:
				add20:
					texth += 20;
					pagelines++;
					break;
				}
			if (pagep[1] == '[' && m) {
				if (pagep[2] == '[') {
					pagelines++;
					goto done;
				}
				texth += 8;
			}
		}
		pagep++;
		if (*pagep == 0) {
			if (pagep[-1] != 13) {
				texth += 20;
				pagelines++;
			}
			goto done;
		}
	} while (maxh > texth);
	if (*pagep && *pagep != 13 && pagep[1] != 13) {
		for (pageq = pagep; *pageq != 0 && *pageq != 13; pageq++)
			;
		if (*pageq == 13) {
			if (pageq[1] == 0 || pageq[1] == 13 || pageq[1] == '+') {
				pagelines++;
				goto done;
			}
			for (pageq = pagep - 2; pageq > pagestart && *pageq != 0 && *pageq != 13; pageq--)
				;
			if (*pageq != 13 || pageq[-1] != 13)
				goto done;
			pagelines++;
			goto done;
		}
	}
done:
	if (mode == 0xfd)
		pagey = (480 - top - texth) / 2 + top;
	else if (mode > 0xf0)
		pagey = (480 - texth) / 2;
	else if (top)
		pagey = 479 - texth;
	else
		pagey = 30;
	pagey0 = pagey;
	pagebullet = 0;
	for (pagek = 0; pagek < pagelines; pagek++, pagey += 10) {
		if ((**pp == '.' || **pp == '}') && ((*pp)[1] == 13 || (*pp)[1] == 0) && (*pp == pagestart || (*pp)[-1] != 32))
			pagebullet = 0;
		else if (**pp == 13)
			pagebullet = 0;
		else if (**pp == '+' && ((*pp)[1] == 13 || (*pp)[1] == 0) && (*pp == pagestart || (*pp)[-1] != 32)) {
			pagebullet = 0;
			*pp += 2;
			break;
		} else {
			if (**pp == '<' && (*pp)[-1] == 13 && iswordc((*pp)[1])) {
				(*pp)++;
				pagebullet = 0;
			} else if (**pp == '[' || **pp == '/' && (*pp)[1] == '[' || **pp == '/' && (*pp)[1] == '/' && (*pp)[2] == '[') {
				pagebullet = 0;
				if (m) {
					btnrow = 0;
					if (**pp == '/' && (*pp)[1] == '/') {
						pagey = 22;
						*pp += 2;
						btnrow = 2;
					} else if (**pp == '/') {
						(*pp)++;
						if (pagey >= 20)
							pagey -= 20;
						else
							pagey = 0;
						btnrow = 1;
					} else if ((*pp)[1] == '[') {
						pagey = 439;
						(*pp)++;
					}
					pagefirst = m->n;
					pageq = *pp;
				item:
					pageq++;
					if (!isdec(*pageq))
						goto text;
					pagen = number(pageq);
					if (pagen == 72)
						pagen = 130;
					else {
						pagen *= 2;
						if (pagen > 428)
							pagen = 428;
					}
					if (pagen <= 0)
						goto text;
					m->rects[m->n].w = pagen;
					while (isdec(*pageq))
						pageq++;
					pagekey = 0;
					if (*pageq == '<') {
						pageq++;
						pagekey = 14;
					} else if (*pageq == '>') {
						pageq++;
						pagekey = 28;
					} else if (*pageq == 'x') {
						pageq++;
						pagekey = 1;
					}
					pageq++;
					if (*pageq == ']') {
						btntext[m->n] = empty;
						m->enabled[m->n] = 0;
					} else if (pageq[-1] == '.' && *pageq == 'R') {
						pageq++;
						if (!isdec(*pageq))
							goto text;
						pagen = number(pageq);
						if (pagen <= 0)
							goto text;
						while (isdec(*pageq))
							pageq++;
						pageq++;
						m->ids[m->n] = pagen;
						btntext[m->n] = pageq;
						m->enabled[m->n] = 1;
					} else if (pageq[-1] == '.' && *pageq == 'X') {
						pageq += 2;
						btntext[m->n] = pageq;
						m->enabled[m->n] = 0;
					} else {
						btntext[m->n] = pageq;
						m->enabled[m->n] = 1;
					}
					m->keys[m->n] = pagekey;
					m->rects[m->n].h = 24;
					m->rects[m->n].y = pagey;
					m->n++;
					while (*pageq != '[' && *pageq != ']' && *pageq != 13 && *pageq != '\0')
						pageq++;
					if (*pageq == ']' && pageq[1] == '[')
						pageq++;
					if (*pageq == '[' && isdec(pageq[1]))
						goto item;
					if (*pageq == ']')
						pageq++;
					for (pagen = pagefirst, rowwidth = 0; pagen < m->n; pagen++)
						rowwidth += m->rects[pagen].w;
					rowwidth += (m->n - pagefirst - 1) * 16;
					rowwidth = 428 - rowwidth;
					if (rowwidth < 0)
						rowwidth = 0;
					m->rects[pagefirst].x = rowwidth / 2 + 106;
					for (pagen = pagefirst + 1; pagen < m->n; pagen++)
						m->rects[pagen].x = m->rects[pagen - 1].x + m->rects[pagen - 1].w + 19;
					*pp = pageq;
					if (btnrow == 2) {
						m->rects[pagefirst].x = 0xffff;
						pagey = pagey0 + -20;
					} else if (btnrow)
						pagey += 28;
					else
						pagey += 18;
					goto skip;
				}
			}
		text:
			if (**pp == '-' && (*pp)[1] == '-' && (*pp)[2] == ' ') {
				pagebullet = 1;
				*pp += 3;
				if (pagebright || mode == 0)
					text3(0, x, pagey, texts + 0xb1d6);
				else
					text3grey(0, x, pagey, texts + 0xb1d6);
			}
			if (pagebullet)
				x += 16;
			if (**pp == 10 && *pp > pagestart && (*pp)[-1] == 13)
				(*pp)++;
			if (pagebright)
				text3(0, x, pagey, *pp);
			else if (mode == 1)
				text3grey(0, x, pagey, *pp);
			else if (mode == 0)
				text3(0, x, pagey, *pp);
			else if (mode == 0xfe || mode == 0xfd)
				text3grey(0, x, pagey, *pp);
			else if (mode == 0xff)
				text3grey(0, 0xffff, pagey, *pp);
			if (pagebullet)
				x -= 16;
			pagey += 10;
		}
	skip:
		for (;;) {
			if (**pp == 0)
				goto end;
			if (**pp == 13)
				break;
			(*pp)++;
		}
		(*pp)++;
	}
end:
	if (m) {
		if (m->rects[m->n - 1].y == 439 && m->rects[m->n - 1].w == 130 && (m->n <= 1 || m->rects[m->n - 2].y != 439))
			cornerrect(m, m->n - 1);
		for (pagen = 0; pagen < m->n; pagen++)
			if (*btntext[pagen] != 0) {
				if (m->rects[pagen].x == 0xffff)
					fitbutton(m, m->n - 1, 565, 22, btntext[pagen], 1);
				if (aliased(m, pagen))
					buttonarr(m->rects[pagen].x, m->rects[pagen].y, m->rects[pagen].w, m->rects[pagen].h, btntext[pagen]);
				else if (m->enabled[pagen])
					buttonon(m->rects[pagen].x, m->rects[pagen].y, m->rects[pagen].w, m->rects[pagen].h, btntext[pagen]);
				else
					buttonoff(m->rects[pagen].x, m->rects[pagen].y, m->rects[pagen].w, m->rects[pagen].h, btntext[pagen]);
			}
	}
	if (**pp == 0)
		return 0;
	if (**pp == '+' && (*pp)[1] == 13)
		*pp += 2;
	if (**pp == 0)
		return 0;
	pagek = 0;
	do {
		if ((*pp)[pagek] != 13)
			return 1;
		pagek++;
	} while ((*pp)[pagek] != 0);
	return 0;
}

void
waitinput(int ms)
{
	short timeout;
	DWORD t, t2;

	timeout = 0;
	waitlost = 0;
	t = ticks();
	if (ms == -1) {
		waitforup = 1;
		do {
			readinput();
			if (waithook)
				waithook();
		} while (!esckey && (mouseleft || keycode || mouseright));
		for (;;) {
			readinput();
			if (waithook)
				waithook();
			if (needredraw(0xfd)) {
				waitlost = 1;
				break;
			}
			if (mouseright && esckey != 0xff) {
				esckey = 1;
				break;
			}
			if (esckey)
				break;
			if (timeout == 0 && since(t) > 6000)
				timeout = 1;
			if (mouseleft || keycode)
				break;
		}
		waitforup = 0;
		leftclick = 0;
		rightclick = 0;
		return;
	}
	t2 = ticks();
	waitforup = 1;
	do {
		readinput();
		if (waithook)
			waithook();
	} while (!esckey && ticks() <= t2 + ms && (mouseleft || keycode || mouseright));
	for (;;) {
		readinput();
		if (waithook)
			waithook();
		if (needredraw(0xfd)) {
			waitlost = 1;
			break;
		}
		if (mouseright && esckey != 0xff) {
			esckey = 1;
			break;
		}
		if (esckey)
			break;
		if (ticks() > t2 + ms)
			break;
		if (timeout == 0 && since(t) > 6000)
			timeout = 1;
		if (mouseleft || keycode)
			break;
	}
	waitforup = 0;
	leftclick = 0;
	rightclick = 0;
}

void
putpage(char *s, char a, char b)
{
	if (a)
		blankpage2();
	else
		blankpage();
	drawpage(&s, (unsigned short)(a ? 106 : 106), b ? 0xff : 0xfe, 1, 400, &menu1);
	puthands();
	backto2();
	pagedone();
}

void
pagein(void)
{
	entermenu();
	if (palblack) {
		flip();
		if (pagefx == 0x20)
			fadeslow(1);
		else
			fadefast(1);
	} else if (pagefx == 0)
		flip();
	else
		turnpage(pagefx == 2 ? 1 : 0, 0, 480, 0);
}

short
runpage(int unused, int (*idle)(void))
{
	if (esckey != 0xff && esckey != 0xfe)
		esckey = 0;
	if (menu1.n)
		return runmenu(&menu1, idle, 0, 0);
	if (esckey != 0xff)
		esckey = 0;
	waitinput(-1);
	if (waitlost) {
		surfslost = 1;
		return 0xfd;
	}
	return 0;
}

void
pageout(void)
{
	if (pagefx != 2 && pagefx != 3) {
		if (pagefx == 0x20)
			fadeslow(0);
		else
			fadefast(0);
	}
	waitrelease(0);
}

short
msgbox(char *s, unsigned char c, char b)
{
	char a;

	nop();
	if (c == 0xff)
		a = 0;
	else
		a = 1;
	do {
		putpage(s, a, b);
		pagein();
	} while (needredraw(msgres = runpage(0, 0)));
	msgesc = esckey;
	pageout();
	esckey = msgesc;
	nop();
	return msgres;
}

int
hasurl(char *s)
{
	for (; *s; s++)
		if (s[0] == 'h' && s[1] == 't' && s[2] == 't' && s[3] == 'p' && s[4] == ':')
			return 1;
	return 0;
}

int
openurl(char *url)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	memset(&si, 0, sizeof si);
	si.cb = sizeof si;
	memset(&pi, 0, sizeof pi);
	if (GetWindowsDirectory(namebuf, 2500) && namebuf[0]) {
		for (urlp = namebuf + 1; *urlp; urlp++)
			;
		if (!(urlp[-1] == 92)) {
			*urlp++ = '\\';
			*urlp = 0;
		}
		copystr(urlp, texts + 0xa429);
	} else
		copystr(namebuf, texts + 0xa429);
	copystr(textbuf, namebuf);
	catstr(textbuf, texts + 0xb1f9);
	catstr(textbuf, url);
	if (!CreateProcess(namebuf, textbuf, NULL, NULL, FALSE, 0x30, NULL, NULL, &si, &pi))
		return 0;
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 1;
}

void
readmename(char *s)
{
	if (lang == 1 || lang == 0 || lang == 4)
		wsprintf(s, texts + 0x9c20, langchar);
	else
		wsprintf(s, texts + 0x9c20, 'E');
}

void
openreadme(void)
{
	readmename(textbuf);
	pathjoin(tmpbuf, progdir, textbuf);
	run(tmpbuf, 0, 0);
}

short
askprint(char *title, char *text, char *name, int unused)
{
	short k;

	k = 1;
	printcur = shown;
	if (noprinter)
		nprinters = 0;
	else
		getprinters();
	for (;;) {
		printres = 0;
		waitrelease(0);
		memset(printpage, 0, 10000);
		wsprintf(printpage, texts + 0xb1a4, title ? title : strs[0x40d], nprinters > 0 ? empty : (texts + 0xb2b6), strs[0x40e], strs[0x40f], name);
		if (k) {
			catstr(printpage, texts + 0xb1c5);
			catstr(printpage, strs[0x72c]);
			catstr(printpage, texts + 0xb1cb);
		} else {
			catstr(printpage, texts + 0xb2a2);
			catstr(printpage, texts + 0xb220);
		}
		catstr(printpage, texts + 0xb1cf);
		catstr(printpage, prevbutton());
		catstr(printpage, nextid(printchoice));
		catstr(printpage, cancelbutton());
		printbtn = msgbox(printpage, 1, 1);
		if (k == 0 && printbtn >= 3)
			printbtn++;
		if (printbtn == 0)
			printres = -1;
		else if (printbtn == 1) {
			printchoice = printbtn;
			if (noprinter)
				printn = 1;
			else
				printn = printtext(0, name, text, scrbuf);
			goto act;
		} else if (printbtn == 2) {
			printchoice = printbtn;
			goto act;
		} else if (printbtn == 3) {
			printchoice = printbtn;
			printres = 1;
		} else if (printbtn == 4)
			printres = -1;
		else
			printres = -2;
		break;
	act:
		if (pagefx == 0x20)
			fadeslow(0);
		else if (pagefx == 0 || pagefx == 1)
			fadefast(0);
		waitrelease(0);
		if (printbtn == 1) {
			if (printn == 1)
				printres = 3;
			else {
				wsprintf(printerr, strs[0x410]);
				catstr(printerr, msgline);
				catstr(printerr, texts + 0xb2af);
				catstr(printerr, prevbutton());
				catstr(printerr, texts + 0xb2a2);
				catstr(printerr, cancelbutton());
				if (msgbox(printerr, 0, 1) == 3)
					printres = -2;
				else if (esckey != 0xff)
					esckey = 0;
			}
		} else if (printbtn == 2) {
			for (printn = 0; text[printn]; printn++)
				;
			if (fopenw(&printfile, name)) {
				filewrite(text, printn, 1, printfile);
				fileclose(&printfile);
				printres = 2;
			} else {
				wsprintf(printerr, strs[0x417]);
				catstr(printerr, prevbutton());
				catstr(printerr, texts + 0xb2a2);
				catstr(printerr, cancelbutton());
				if (msgbox(printerr, 0, 1) != 3) {
					if (esckey != 0xff)
						esckey = 0;
					continue;
				}
			}
		}
		break;
	}
	if (!noprinter)
		freeprinters();
	if (printcur)
		drawcursor();
	else
		hidecursor();
	waitrelease(0);
	return printres;
}

int
offbar(void)
{
	if (hovery >= 46)
		return 1;
	return 0;
}

void
barhilite(short i, short flag)
{
	int n;

	if (i == 0xff)
		return;
	if (flag) {
		n = barmenu.rects[i].x + barmenu.rects[i].w - 88;
		pillshade9(n, 18, 88, 48);
	}
	putwint(8, 18, 12);
	if (playmode == 1)
		n = 0;
	else if (nofoes)
		n = 1;
	else
		n = 2;
	n += 16;
	putwint(272, 18, n);
}

int
dmgscale(short n)
{
	return (short)(n * 100) / 49;
}

int
protscale(short n)
{
	return (short)(n * 100) / 27;
}

void
drawstats(void)
{
	if (cheat && (keys['W'] & 0x80))
		statmode = 0;
	else
		statmode = 0xff;
	paney = 480;
	inkshade();
	putwint(8, paney - 100, 13);
	put48t0(10, paney - 48, gauges[6]);
	stattime = daytime(gamehour);
	if (((unsigned char *)mapstate)[0x22] <= 15)
		staticon = 150;
	else
		staticon = dayicon(stattime);
	putcs(10, paney - 96, staticon);
	statv = gameday;
	if (gamehour < 5)
		statv--;
	if (statv <= 0)
		statv = 1;
	roman(statv);
	wsprintf(textbuf, strs[546], stattime, romanbuf);
	drawtext2o(0, 230 - textwidth(textbuf) / 2 + (paney - 84) * 640, textbuf);
	statv = zones[mapzone(curmap)].name;
	wsprintf(textbuf, strs[1380], strs[statv]);
	drawtext2o(0, 230 - textwidth(textbuf) / 2 + (paney - 62) * 640, textbuf);
	statustext(textbuf, &units[0]);
	statv = textwidth(textbuf);
	cutword = 0;
	if (statv > 336) {
		for (statblk = 0; textbuf[statblk]; statblk++)
			;
		do {
			statblk--;
			if (textbuf[statblk] == ',')
				cutword = 0;
			else
				cutword = 1;
			textbuf[statblk] = 0;
			statv = textwidth(textbuf);
		} while (statblk && statv > 336);
	}
	if (cutword && statblk > 0) {
		do {
			statblk--;
			if (textbuf[statblk] == ',') {
				textbuf[statblk] = 0;
				break;
			}
		} while (statblk);
		statv = textwidth(textbuf);
	}
	drawtext2o(0, 230 - statv / 2 + (paney - 40) * 640, textbuf);
	wsprintf(textbuf, texts + 0xb072, tiredtext(game.tiredlevel), hungertext(game.hungerlevel));
	drawtext2o(0, 230 - textwidth(textbuf) / 2 + (paney - 18) * 640, textbuf);

	drawtext2o(0, (paney - 84) * 640 + 411, strs[813]);
	statv = units[0].health;
	if (statmode == 0xff)
		putsrc(0, 441, paney - 90, 5, statbar(statv));
	else {
		if (statmode)
			statv = roundto(statv, statmode);
		wsprintf(textbuf, texts + 0xb08d, statv);
		drawtext2o(0, (paney - 84) * 640 + 442, textbuf);
	}
	drawtext2o(0, (paney - 62) * 640 + 411, strs[810]);
	statv = units[0].strength;
	if (statmode == 0xff)
		putsrc(0, 441, paney - 68, 5, statbar(statv));
	else {
		if (statmode)
			statv = roundto(statv, statmode);
		wsprintf(textbuf, texts + 0xb08d, statv);
		drawtext2o(0, (paney - 62) * 640 + 442, textbuf);
	}
	drawtext2o(0, (paney - 40) * 640 + 411, strs[811]);
	statv = units[0].dexterity;
	if (statmode == 0xff)
		putsrc(0, 441, paney - 46, 5, statbar(statv));
	else {
		if (statmode)
			statv = roundto(statv, statmode);
		wsprintf(textbuf, texts + 0xb08d, statv);
		drawtext2o(0, (paney - 40) * 640 + 442, textbuf);
	}
	drawtext2o(0, (paney - 18) * 640 + 411, strs[812]);
	statv = units[0].intellect;
	if (statmode == 0xff)
		putsrc(0, 441, paney - 24, 5, statbar(statv));
	else {
		if (statmode)
			statv = roundto(statv, statmode);
		wsprintf(textbuf, texts + 0xb08d, statv);
		drawtext2o(0, (paney - 18) * 640 + 442, textbuf);
	}

	staticon = baselook(units[0].offhand & 0xfff);
	if (classof(staticon & 0xfff) == 85) {
		staticon = gearicon(staticon);
		if (staticon)
			putsrc(0, 608, paney - 43, 6, staticon);
		statshield = 1;
	} else
		statshield = 0;
	textbuf[0] = units[0].mode;
	units[0].mode = 20;
	attackval(&units[0], &statatk, &statkind, &statdmin, &statdmax, 0);
	defenseval(&units[0], &statparry, &statblk, &statv);
	units[0].mode = textbuf[0];
	if (statmode == 0xff) {
		putsrc(0, 520, paney - 90, 5, statbar(statatk));
		if (units[0].hand || statparry)
			putsrc(0, 520, paney - 68, 5, statbar(statparry));
		putsrc(0, 520, paney - 46, 5, statbar(statv));
		if (statshield)
			putsrc(0, 520, paney - 24, 5, statbar(statblk));
	} else {
		if (statmode) {
			statatk = roundto(statatk, statmode);
			statdmin = roundto(statdmin, statmode);
			statdmax = roundto(statdmax, statmode);
			statparry = roundto(statparry, statmode);
			statblk = roundto(statblk, statmode);
			statv = roundto(statv, statmode);
		}
		wsprintf(textbuf, texts + 0xb08d, statatk);
		drawtext2o(0, (paney - 86) * 640 + 520, textbuf);
		wsprintf(textbuf, texts + 0xb08d, statparry);
		drawtext2o(0, (paney - 64) * 640 + 520, textbuf);
		wsprintf(textbuf, texts + 0xb08d, statv);
		drawtext2o(0, (paney - 42) * 640 + 520, textbuf);
		if (statshield) {
			wsprintf(textbuf, texts + 0xb08d, statblk);
			drawtext2o(0, (paney - 20) * 640 + 520, textbuf);
		}
	}

	staticon = baselook(units[0].legs & 0xfff);
	if (staticon) {
		staticon = gearicon(staticon);
		if (staticon)
			putsrc(0, 560, paney - 22, 6, staticon);
		statv = protection(units[0].legs);
		if (statmode == 0xff) {
			statv = protscale(statv);
			putsrc(0, 584, paney - 24, 5, statbar(statv));
		} else {
			if (statmode)
				statv = roundto(statv, statmode);
			wsprintf(textbuf, texts + 0xb08d, statv);
			drawtext2o(0, (paney - 20) * 640 + 584, textbuf);
		}
	}
	staticon = baselook(units[0].body & 0xfff);
	if (staticon) {
		staticon = gearicon(staticon);
		if (staticon)
			putsrc(0, 560, paney - 44, 6, staticon);
		statv = protection(units[0].body);
		if (statmode == 0xff) {
			statv = protscale(statv);
			putsrc(0, 584, paney - 46, 5, statbar(statv));
		} else {
			if (statmode)
				statv = roundto(statv, statmode);
			wsprintf(textbuf, texts + 0xb08d, statv);
			drawtext2o(0, (paney - 42) * 640 + 584, textbuf);
		}
	}
	staticon = baselook(units[0].hand);
	if (staticon) {
		staticon = gearicon(staticon);
		if (staticon)
			putsrc(0, 560, paney - 88, 6, staticon);
	}
	if (statmode == 0xff) {
		statdmin = dmgscale(statdmin);
		statdmax = dmgscale(statdmax);
		wsprintf(textbuf, texts + 0xb320);
		drawtext2o(0, (paney - 84) * 640 + 598, textbuf);
		putsrc(0, 584, paney - 90, 5, statbar(statdmin));
		putsrc(0, 608, paney - 90, 5, statbar(statdmax));
	} else {
		if (statmode)
			statdmin = roundto(statdmin, statmode);
		if (statmode)
			statdmax = roundto(statdmax, statmode);
		wsprintf(textbuf, texts + 0xb0f8, statdmin, statdmax);
		drawtext2o(0, (paney - 86) * 640 + 584, textbuf);
	}
	staticon = baselook(units[0].head & 0xfff);
	if (staticon) {
		staticon = gearicon(staticon);
		if (staticon)
			putsrc(0, 560, paney - 66, 6, staticon);
		statv = protection(units[0].head);
		if (statmode == 0xff) {
			statv = protscale(statv);
			putsrc(0, 584, paney - 68, 5, statbar(statv));
		} else {
			if (statmode)
				statv = roundto(statv, statmode);
			wsprintf(textbuf, texts + 0xb08d, statv);
			drawtext2o(0, (paney - 66) * 640 + 584, textbuf);
		}
	}
	if (cheat) {
		putpixoff(0x4a871, 0xf5);
		putpixoff(0x4aaf0, 0xf5);
		putpixoff(0x4ad71, 0xf5);
		if ((keys['W'] & 0x80) && (keys[VK_SHIFT] & 0x80) && (keys[0xe2] & 0x80)) {
			staticon = 590;
			if (getbit(game.flags, staticon))
				putpix2(0, 8, 472, 73);
		}
	}
	inkshade();
}

void
barlabel(unsigned short i)
{
	labelx = textwidth(strs[i]);
	if (labelx > 88)
		labelx = 88;
	labelx = 88 - labelx;
	labelx /= 2;
	if (labelx < 12)
		labelx = 12;
	drawtext2o(0, barmenu.rects[baritem++].x + 3840 + labelx, strs[i]);
}

void
drawmenubar(void)
{
	putwint(8, 0, 11);
	memset(&barmenu, 0, sizeof barmenu);
	barmenu.fn = barhilite;
	for (baritem = 0; baritem < 7; baritem++) {
		barmenu.rects[baritem].x = baritem * 88 + 12;
		barmenu.rects[baritem].w = 88;
		barmenu.rects[baritem].y = 0;
		barmenu.rects[baritem].h = 70;
		barmenu.enabled[baritem] = 1;
	}
	barmenu.n = 7;
	barhilite(playmode, 0);
	inkshade();
	baritem = 0;
	barlabel(250);
	barlabel(251);
	barlabel(256);
	if (playmode == 1)
		barlabel(253);
	else
		barlabel(254);
	barlabel(252);
	barlabel(255);
	barlabel(257);
	inkshade();
	barmenu.rects[0].w += barmenu.rects[0].x;
	barmenu.rects[0].x = 0;
}

int
hasaction(unsigned char c)
{
	for (actsel = 0; actsel < 40; actsel++)
		if (actions[actsel].cmd == c)
			return 1;
	actsel = 0;
	return 0;
}

void
drawaction(short a, short mode)
{
	/* w0 to w2 only shape the code: with them the second loop of drawactions multiplies by 26 with imul */
	int w0, w1, w2;

	if (a == 0xff)
		return;
	if (mode == 1 && !(playmode != 1 && !actions[a].where) && !(actions[a].where == 2 && foesnear))
		pillshade9(actions[a].x, actions[a].y, 48, 48);
	if (!(cheat && (keys[VK_CONTROL] & 0x80)))
		putcs(actions[a].x, actions[a].y, actions[a].icon);
	if (playmode != 1 && !actions[a].where || actions[a].where == 2 && foesnear || !(mode != 2))
		lumadd(actions[a].x, actions[a].y, 48, 48, 80);
	if (mode != 1)
		return;
	namebuf[0] = 0;
	if (playmode != 1 && !actions[a].where) {
		wsprintf(printbuf, strs[actions[a].name]);
		wsprintf(namebuf, strs[307]);
	} else if (actions[a].where == 2 && foesnear) {
		wsprintf(printbuf, strs[actions[a].name]);
		wsprintf(namebuf, strs[308]);
	} else {
		if (actions[a].turns <= 1)
			wsprintf(printbuf, strs[actions[a].name]);
		else
			wsprintf(printbuf, strs[1453], strs[actions[a].name], actions[a].turns);
		if (actions[a].state == 2)
			wsprintf(namebuf, texts + 0xb134, strs[1417]);
	}
	acttipx = actcols * 48 + 32;
	acttipy = mousey;
	if (acttipy < 24)
		acttipy = 24;
	acttipy = (acttipy - 24) / 48;
	acttipy = acttipy * 48 + 39;
	if (namebuf[0] == 0)
		text3(0, acttipx, acttipy, printbuf);
	else {
		text3(0, acttipx, acttipy - 10, printbuf);
		text3(0, acttipx, acttipy + 10, namebuf);
	}
}

int
drawactions(void)
{
	foesnear = nearfoe(0);
	acti = 0;
	actcol = 0;
	actrow = 0;
	actcols = 0;
	do {
		if (actions[acti].rowbreak & 1) {
			actrow++;
			actcol = 0;
		}
		if (!actions[acti].icon) {
		hide:
			actions[acti].state = 0;
			actions[acti].x = 0;
			actions[acti].y = 0;
			continue;
		}
		if (cheat && (keys['W'] & 0x80 || keys[VK_SHIFT] & 0x80)
		&& actions[acti].cmd >= 29 && actions[acti].cmd <= 31)
			continue;
		if (actions[acti].cmd == 16 && takeberserk(0)
		|| actions[acti].cmd == 14 && drinkpotion(402, 0)
		|| actions[acti].cmd == 15 && drinkpotion(385, 0))
			actions[acti].state = 2;
		else if (getbit(game.known, actions[acti].cmd))
			actions[acti].state = 1;
		else if (cheat && (keys['W'] & 0x80 || keys[VK_SHIFT] & 0x80)
		&& !(actions[acti].cmd >= 29 && actions[acti].cmd <= 31))
			actions[acti].state = 3;
		else
			goto hide;
		actions[acti].x = actcol * 48 + 8;
		actions[acti].y = actrow * 48 + 24;
		actcol++;
		if (actcols < actcol)
			actcols = actcol;
	} while (++acti < 40);
	winframe(0, 0, actcols * 48 + 16, 480, 1);
	acti = 0;
	do {
		if (actions[acti].state)
			drawaction(acti, actions[acti].state == 3 ? 2 : 0);
	} while (++acti < 40);
	return 1;
}

int
offactions(void)
{
	if (actcols * 48 + 8 < hoverx)
		return 1;
	return 0;
}

short
actionidx(unsigned char c)
{
	/* u0 to u2 only shape the code: with them the loop multiplies by 26 with shifts */
	int u0, u1, u2;

	for (actj = 0; actj < 40; actj++)
		if (actions[actj].cmd == c)
			return actj;
	return 0xff;
}

short
bindaction(unsigned char scan, char key)
{
	if (menu1.sel == 0 || menu1.sel > 40 || key < '1' || key > '9')
		return 0;
	quickacts[key - '1'] = actions[menu1.sel - 1].cmd;
	wsprintf(printbuf, texts + 0xb103, key, strs[actions[menu1.sel - 1].name]);
	text3w(printbuf);
	text3(1, 608 - text3width, 460, printbuf);
	return 1;
}

void
actionmenu(short flag)
{
	if (unconscious())
		return;
	syncplayer();
	units[0].pose = 0;
	steptick = stepframe * 4;
	menumode = 1;
	if (tipshown() || nmsgs) {
		dropheld();
		cleartexts();
		drawscreen();
	}
	hidecursor();
	primtoback();
	setcursor1(3, 3);
	drawcursor();
	drawactions();
	backto2();
	pagedone();
	flip();
	memset(&menu1, 0, sizeof menu1);
	menu1.n = 40;
	for (actk = 0; actk < menu1.n; actk++) {
		if (actions[actk].state) {
			menu1.enabled[actk] = 1;
			menu1.rects[actk].x = actions[actk].x;
			menu1.rects[actk].y = actions[actk].y;
			menu1.rects[actk].w = 48;
			menu1.rects[actk].h = 48;
		}
	}
	menu1.fn = drawaction;
	menu1.keys2[0] = 0x52;
	hoversound();
	actsel = runmenu(&menu1, flag ? NULL : offactions,
		(short (*)(unsigned char, unsigned char))bindaction, 0);
	if (actsel >= 0xf0)
		actsel = 0;
	leavemenu(0);
}

short
confirm(char *s)
{
	short r;

	wsprintf(packbuf, s);
	wrap(packbuf, 428);
	catstr(packbuf, texts + 0xb2ab);
	catstr(packbuf, pick(texts + 0x6dd9, texts + 0x6de1, texts + 0x6dea, texts + 0x6deb, texts + 0x6dec, texts + 0x6df4));
	catstr(packbuf, pick(texts + 0x6df5, texts + 0x6dff, texts + 0x6e07, texts + 0x6e08, texts + 0x6e09, texts + 0x6e11));
	if (esckey == 0xfe)
		;
	else
		esckey = 0;
	do {
		putpage(packbuf, 0, 1);
		menu1.keys[0] = 0x15;
		menu1.keys[1] = 0x31;
		if (soundsok)
			playnow(45);
		pagein();
		drawcursor();
		r = runpage(0, NULL);
	} while (needredraw(r));
	pageout();
	if (esckey != 0xff)
		esckey = 0;
	if (r == 1)
		return 1;
	return 0;
}

short
mayquit(short unused)
{
	if (quitprompt >= 2)
		quitprompt -= 2;
	else if (!(keys['W'] & 0x80))
		esckey = 0xff;
	stopmusic();
	esckey = 0;
	return 1;
}

int
checkquit(void)
{
	if (quitting != 0 || esckey != 0xff)
		return 0;
	if (mayquit(1)) {
		quit();
		return 0;
	}
	waitrelease(0);
	esckey = 0;
	return 1;
}

int
edgemenus(short mode)
{
	if (quitting || inedge)
		return 0;
	if (gametime < 3) {
		edgetop = 1;
		edgebot = 1;
		edgeleft = 1;
		edgeright = 1;
		return 0;
	}
	if (!(units[0].pose == 0 || units[0].pose > 9 || talkactive)) {
		if (hovery <= 20)
			edgetop = 1;
		if (hovery >= 452)
			edgebot = 1;
		if (hoverx <= 28)
			edgeleft = 1;
		if (hoverx >= 610)
			edgeright = 1;
		return 0;
	}
	if (hoverkind == 230 && !esckey)
		return 0;
	inedge = 1;
	wascalm = playmode == 1 ? 1 : 0;
	wasable = playerout() ? 0 : 1;
	if (esckey) {
		if (!mode && !checkquit() && !quitting) {
			dropheld();
			mainmenu();
			esckey = 0;
		}
	out:
		inedge = 0;
		if (!mode) {
			if (checkquit())
				drawscreen();
			if (quitting)
				return 0;
		}
		if (units[0].health > 0) {
			if (palblack)
				drawscreen();
			fadefast(1);
		}
		return 0;
	}
	if (hovery > 20) {
		edgetop = 0;
		topsince = 0;
	}
	if (hovery < 452) {
		statshown = 0;
		edgebot = 0;
		botsince = 0;
	}
	if (hoverx > 28) {
		edgeleft = 0;
		leftsince = 0;
	}
	if (hoverx < 610) {
		edgeright = 0;
		rightsince = 0;
	}
	if (!(hovery > 16 && hovery < 456 && hoverx > 16 && hoverx < 614)
	&& !mouseleft && !mouseright && !(menumode && !mode)
	&& (mode || !indoinput && !talkactive)) {
		if (hovery <= 20 && edgetop == 0 && !mode) {
			if (popupdelay) {
				if (topsince == 0) {
					topsince = ticks();
					goto out;
				}
				if (ticks() < topsince + popupdelay)
					goto out;
				topsince = 0;
			}
			if (tipshown() || nmsgs) {
				dropheld();
				cleartexts();
			again:
				drawscreen();
			}
		menu:
			menumode = 1;
			edgetop = 1;
			hidecursor();
			primtoback();
			drawcursor();
			nearfoe(0);
			drawmenubar();
			backto2();
			pagedone();
			setcursor1(3, 3);
			flip();
			mousemoved = 1;
			barsel = runmenu(&barmenu, offbar, NULL, 0);
			if (needredraw(barsel))
				goto again;
			switch (barsel) {
			case 0:
				break;
			case 1:
				mainmenu();
				goto leave;
			case 2:
				charsheet(units, 0);
				goto leave;
			case 3:
				travelmap(0, curmap, curmap);
				goto leave;
			case 4:
				cleartexts();
				leftclick = 0;
				rightclick = 0;
				clicktime = 0;
				presslatch = 1;
				dblclick = 0;
				if (playmode == 3 && !nofoes) {
					denymsg(strs[316]);
					barshown = 1;
					drawscreen();
					barshown = 0;
				} else {
					if (playmode != 3)
						combatcause = 1;
					barshown = 1;
					togglecombat(1);
					barshown = 0;
				}
				goto menu;
			case 5:
				markers(0);
				goto leave;
			case 6:
				journal();
				break;
			case 7:
				docmenu();
				goto leave;
			case 0xff:
				break;
			}
			drawscreen();
			goto done;
		} else if (hovery >= 456 && edgebot == 0) {
			if (trademode) {
				if (hoverx < 404) {
					edgebot = 1;
					goto out;
				}
			} else if (talkactive && talkstate != 2 && choicesh + talky >= 370
			&& hoverx >= 140 && choicesr + 12 >= hoverx) {
				edgebot = 1;
				goto out;
			}
			if (popupdelay) {
				if (botsince == 0) {
					botsince = ticks();
					goto out;
				}
				if (ticks() < botsince + popupdelay)
					goto out;
				botsince = 0;
			}
			menumode = 1;
			edgebot = 1;
			statshown = 1;
			hoversound();
			drawscreen();
			hidecursor();
			tracesite = 50;
			while (mouseleft) {
				if (playmode == 1 && units[0].health > 0)
					gametick();
				else {
					readinput();
					if (needredraw(0xfd))
						drawscreen();
				}
			}
			leftclick = mouseleft;
			while (hovery >= 452 && !mouseright && !(wasable && playerout())
			&& !(wascalm && playmode != 1) && !esckey) {
				if (playmode == 1 && units[0].health > 0)
					gametick();
				else {
					readinput();
					if (needredraw(0xfd))
						drawscreen();
				}
			}
			statshown = 0;
			goto done;
		} else if (hoverx <= 16 && edgeleft == 0 && !mode) {
			if (popupdelay) {
				if (leftsince == 0) {
					leftsince = ticks();
					goto out;
				}
				if (ticks() < leftsince + popupdelay)
					goto out;
				leftsince = 0;
			}
			edgeleft = 1;
			actionmenu(0);
			inedge = 0;
			if (actsel) {
				actsel--;
				doaction(0);
			}
			inedge = 1;
			goto done;
		} else if (hoverx >= 614 && edgeright == 0 && mode != 2) {
			if (popupdelay) {
				if (rightsince == 0) {
					rightsince = ticks();
					goto out;
				}
				if (ticks() < rightsince + popupdelay)
					goto out;
				rightsince = 0;
			}
			menumode = 1;
			edgeright = 1;
			memcpy(workbuf, panelstack, 16);
			hidecursor();
			dropheld();
			cleartexts();
			if (isopen(1) && isopen(3)) {
				closepanel(1);
				closepanel(3);
			} else {
				openhero();
				hoversound();
			}
			drawscreen();
			mousemoved = 1;
			tracesite = 51;
			while (mouseleft) {
				if (playmode == 1 && units[0].health > 0)
					gametick();
				else {
					readinput();
					if (needredraw(0xfd))
						drawscreen();
				}
			}
			leftclick = mouseleft;
			do {
				if (!(hoverx >= 610) || mouseright || esckey
				|| wascalm && playmode != 1 || wasable && playerout()) {
					memcpy(panelstack, workbuf, 16);
					if (isopen(1) && isopen(3))
						hoversound();
					goto done;
				}
				if (playmode == 1 && units[0].health > 0)
					gametick();
				else {
					readinput();
					if (needredraw(0xfd))
						drawscreen();
				}
			} while (!mouseleft || mode);
			clicksound();
			goto done;
		}
	}
	goto out;
done:
	menumode = 0;
	drawscreen();
	mousemoved = 1;
	drawcursor();
	if (esckey != 0xff)
		esckey = 0;
	while (mouseleft)
		readinput();
	leftclick = mouseleft;
leave:
	inedge = 0;
	return 1;
}

/* lets the player pick *val from min to max in window 14 */
int
asknum(unsigned short *val, unsigned short min, unsigned short max)
{
	short y1, y0;

	numchanged = 0;
	numhkind = hoverkind;
	numhcx = hovercx;
	numhcy = hovercy;
	numok = 0;
	numhpx = hoverpx;
	numhpy = hoverpy;
	numctl = 0;
	openpanel(14);
	panels[14].id = *val;
	panels[14].arg = max;
	numval = &panels[14].id;
	movepanel(14, hoverx - 170, hovery - 38);
top:
	numx = panels[14].x;
	numy = panels[14].y;
	entermenu();
	mousemoved = 1;
	drawscreen();
	waitforup = 1;
	while (mouseleft || mouseright)
		readinput();
	leftclick = 0;
	if (esckey == 0xff)
		;
	else
		esckey = 0;
	waitforup = 0;
	numnext = 0;
	numdelay = 300;
	numunused = 0;
	for (;;) {
		readinput();
		if (mouseright) {
		cancel:
			*val = 0;
			playnow(17);
		finish:
			closepanel(14);
			defclip();
			leavemenu(1);
			hoverkind = numhkind;
			hovercx = numhcx;
			hovercy = numhcy;
			hoverpx = numhpx;
			hoverpy = numhpy;
			return numok;
		}
		while (ticks() <= numnext)
			readinput();
		if (!mouseleft)
			goto up;
		if (numctl != hovercx)
			hoversound();
		numctl = hovercx;
		numdelay = 300;
		y0 = numy + 27;
		y1 = numy + 49;
		switch (hovercx) {
		case 1:
			setclip(numx + 131, y0, numx + 150, y1);
			for (;;) {
				if (hoverkind != 0x36 || hovercx != 1)
					break;
				if (max > *numval) {
					numchanged = 1;
					(*numval)++;
				}
				drawscreen();
				numchanged = 0;
				numnext = ticks() + numdelay;
				if (numdelay >= 50)
					numdelay -= 50;
				else
					numdelay = 0;
				do {
					if (!mouseleft) {
						defclip();
						goto next;
					}
					readinput();
				} while (ticks() <= numnext);
			}
		release:
			defclip();
			while (mouseleft)
				readinput();
			break;
		case 2:
			setclip(numx + 1, y0, numx + 19, y1);
			for (;;) {
				if (hoverkind != 0x36 || hovercx != 2)
					break;
				if (min < *numval) {
					numchanged = 1;
					(*numval)--;
				}
				drawscreen();
				numchanged = 0;
				numnext = ticks() + numdelay;
				if (numdelay >= 50)
					numdelay -= 50;
				else
					numdelay = 0;
				for (;;) {
					if (!mouseleft) {
						defclip();
						goto next;
					}
					readinput();
					if (ticks() > numnext)
						break;
				}
			}
			goto release;
		case 3:
			setclip(numx + 21, y0, numx + 128, y1);
			for (;;) {
				if (hoverkind != 0x36 || hovercx != 3)
					break;
				*numval = max * hovercy / 104;
				if (min > *numval)
					*numval = min;
				if (max < *numval)
					*numval = max;
				numslide = hovercy;
				numchanged = 1;
				drawscreen();
				numchanged = 0;
				numnext = ticks() + numdelay;
				if (numdelay >= 50)
					numdelay -= 50;
				else
					numdelay = 0;
				do {
					if (!mouseleft) {
						defclip();
						goto next;
					}
					readinput();
				} while (hovercy == numslide);
			}
			goto release;
		case 4:
			if (hoverkind == 0x36) {
			ok:
				*val = *numval;
				numok = 1;
				goto finish;
			}
			break;
		case 999:
			dragpanel(hoverkind - 40);
			mousemoved = 1;
			goto top;
		}
		continue;
	up:
		numnext = 0;
		if (numctl == 3)
			hoversound();
		numctl = 0;
		pollinput();
		if (keycode == 0x1c)
			goto ok;
		if (esckey)
			goto cancel;
	next:;
	}
}

void
actfail(void)
{
	if (actions[actsel].target == 0)
		wsprintf(printbuf, strs[287]);
	else if (targetitem == 0)
		wsprintf(printbuf, strs[288]);
	else if (classof(targetitem & 0xfff) == 20 && targetitem >> 16 == 0)
		wsprintf(printbuf, strs[290]);
	else
		wsprintf(printbuf, strs[289], itemname(targetitem));
	denymsg(texts + 0xb079, strs[actions[actsel].name], printbuf);
}

/* carries out the action actsel, asking for its target first */
int
doaction(short flag)
{
	doactclear = 0;
	if (unconscious())
		return 1;
	if (!cheat || !(keys['W'] & 0x80)) {
		if (playmode != 1 && !actions[actsel].where) {
			denycombat();
			prependmsg(strs[actions[actsel].name]);
			return 0;
		}
		if (actions[actsel].where == 2 && foesnear) {
			denyfoes();
			prependmsg(strs[actions[actsel].name]);
			return 0;
		}
	}
	if (actions[actsel].cmd == 0x1c) {
		if (confirm(strs[564]))
			units[0].health = -31000;
		else {
			drawscreen();
			fadefast(1);
		}
		return 1;
	}
	if (actions[actsel].cost > units[0].vigor) {
		lastfail = actions[actsel].cmd;
		acttime = gametime;
		return 0;
	}
	acta = 0;
	actb = 0;
	actc = 0;
	if (!(actions[actsel].target == 0)) {
		if (actions[actsel].cmd == 0xe && intoxicated(units))
			goto exec;
		if (actions[actsel].cmd == 0xd && units[0].bleeding)
			goto exec;
		if (flag)
			goto target;
		switch (actions[actsel].cmd) {
		case 3:
			actprompt = strs[410];
			break;
		case 9:
			actprompt = strs[411];
			break;
		case 10:
			actprompt = strs[412];
			break;
		case 11:
			actprompt = strs[413];
			break;
		case 12:
			actprompt = strs[414];
			break;
		case 14:
			actprompt = strs[419];
			break;
		case 13:
			actprompt = strs[420];
			break;
		case 18:
			actprompt = strs[415];
			break;
		case 20:
			actprompt = strs[416];
			break;
		case 21:
		case 29:
			actprompt = strs[417];
			break;
		case 22:
		case 30:
			actprompt = strs[418];
			break;
		case 23:
		case 31:
			actprompt = strs[421];
			break;
		case 24:
			actprompt = strs[422];
			break;
		case 25:
			actprompt = strs[423];
			break;
		case 26:
			actprompt = strs[424];
			break;
		case 27:
			actprompt = strs[425];
			break;
		default:
			actprompt = strs[actions[actsel].name];
			break;
		}
		tgtopen = 0;
		tgtperson = 0;
		tgtcorpse = 0;
		tgtself = 0;
		switch (actions[actsel].target) {
		case 1:
			acthint = strs[399];
			tgtpack = 0;
			tgtself = 2;
			break;
		case 2:
			acthint = strs[400];
			tgtpack = 1;
			break;
		case 3:
			acthint = strs[401];
			tgtpack = 0;
			break;
		case 4:
			acthint = strs[402];
			tgtpack = 0;
			break;
		case 5:
			acthint = strs[403];
			tgtpack = 0;
			break;
		case 6:
			acthint = strs[404];
			tgtpack = 1;
			break;
		case 7:
		case 8:
			acthint = strs[405];
			tgtpack = 1;
			break;
		case 9:
			acthint = strs[406];
			tgtpack = 0;
			break;
		case 10:
			acthint = strs[407];
			tgtpack = 1;
			tgtself = 1;
			break;
		case 99:
		default:
			acthint = strs[408];
			tgtpack = 1;
			tgtself = 1;
			break;
		}
		attackmsg = 0;
		stealmsg = 0;
		wsprintf(printbuf, texts + 0xb23f, actprompt, acthint);
		promptmsg = askmsg(printbuf);
		keeplast();
	again:
		drawscreen();
		tgtopen = 1;
		tgtperson = 0;
		tgtcorpse = 1;
		switch (actions[actsel].target) {
		case 1:
			tgtopen = 0;
		case 10:
			tgtperson = 0;
			break;
		case 9:
			tgtcorpse = 0;
			break;
		case 3:
			tgtopen = 0;
			break;
		}
		newmsgs = 0;
		actcalm = playmode == 1 ? 1 : 0;
		actres = asktarget(0xff, 0xff, 0xff, 0xff, 0xff, tgtopen, tgtperson, tgtcorpse, tgtpack, tgtself);
		if (!(!(playerout() == 0) || actcalm && playmode != 1))
			goto cont;
	cancel:
		delmsgid(promptmsg);
		if (attackmsg)
			delmsgid(attackmsg);
		if (stealmsg)
			delmsgid(stealmsg);
		if (askslot)
			delmsgid(askslot);
		notetarget();
		drawscreen();
		drawcursor();
		menumode = 1;
		waitmouseup(0);
		lastfail = actions[actsel].cmd;
		menumode = 0;
		acttime = gametime;
		return 0;
	cont:
		if (actres == 0) {
			if (newmsgs) {
				prependmsg(strs[actions[actsel].name]);
				goto cancel;
			}
		fail:
			actfail();
			goto cancel;
		}
		if (actres == -1) {
			if (newmsgs)
				prependmsg(strs[actions[actsel].name]);
			goto cancel;
		}
	target:
		tgtclass = classof(targetitem & 0xfff);
		switch (actions[actsel].target) {
		case 1:
			if (tgtclass != 20)
				goto fail;
		unit:
			if (actions[actsel].hostile && sparring(units, &units[targetidx])) {
				denypractice();
				goto cancel;
			}
			if (actions[actsel].hostile && askattack()) {
				notetarget();
				goto again;
			}
			notetarget();
			acta = targetidx;
			actb = -1;
			break;
		case 9:
			if (tgtclass != 0x15 && tgtclass != 0xcd) {
				acta = findclass(targety * 150 + targetx, 0x15);
				if (!acta) {
					acta = findclass(targety * 150 + targetx, 0xcd);
					if (acta == 0) {
						actfail();
						goto cancel;
					}
				}
				targetidx = acta;
				targetitem = mapitems[acta];
			}
			acta = targetitem >> 16;
			break;
		case 4:
			acta = targetx;
			actb = targety;
			actc = acta + actb * 150;
			if (findclass(actc, 0x34))
				break;
			actfail();
			goto cancel;
		case 6:
			actc = baselook(targetitem & 0xfff);
			if (actc == 0x1b5)
				goto fail;
			if (tgtclass == 0x3c || tgtclass == 0x41 || tgtclass == 0x46 || tgtclass == 0x55
			|| tgtclass == 0x48 || tgtclass == 0x63 || tgtclass == 0x62 || tgtclass == 0xf
			|| tgtclass == 0x10 || isfood(targetitem) || actc == 0x180 || actc == 0x514
			|| actc == 0x50d || actc == 0x50e || actc == 0x50f) {
		case 99:
		ok:
				acta = targetkind;
				actb = targetidx;
				break;
			}
			actfail();
			goto cancel;
		case 10:
			if (tgtclass == 20)
				goto unit;
		case 2:
			if (tgtclass == 0x3c || tgtclass == 0x41 || tgtclass == 0x46 || tgtclass == 0x48
			|| tgtclass == 0x4b || tgtclass == 0x50 || tgtclass == 0x55 || tgtclass == 0x63
			|| tgtclass == 0x62)
				goto ok;
			actfail();
			goto cancel;
		case 3:
			if (tgtclass != 1 && tgtclass != 10 || targetkind != 1)
				goto fail;
			acta = targetidx;
			actb = targetitem;
			actc = tgtclass;
			break;
		}
	}
exec:
	if (actions[actsel].target) {
		if (sanctioned(actions[actsel].cmd, targetkind, targetx, targety, targetitem)
		|| !asksteal(0))
			;
		else {
			notetarget();
			goto again;
		}
		notetarget();
	}
	delmsgid(promptmsg);
	if (attackmsg)
		delmsgid(attackmsg);
	if (stealmsg)
		delmsgid(stealmsg);
	if (askslot)
		delmsgid(askslot);
	newmsgs = 0;
	actres = canperform(units, actions[actsel].cmd, acta, actb, actc);
	if (actres) {
		if (newmsgs == 0) {
			switch (actions[actsel].cmd) {
			case 4:
				actprompt = strs[1079];
				break;
			case 8:
				actprompt = strs[1080];
				break;
			case 16:
				actprompt = strs[1081];
				break;
			case 17:
				actprompt = strs[1082];
				break;
			default:
				actprompt = strs[actions[actsel].name];
				break;
			}
			actmsg = msgf(99, actprompt);
		} else
			actmsg = 0;
		actionturn = 1;
		drawscreen();
		actturns = actions[actsel].turns;
		if (actturns > 1)
			busyuntil(gametime + actturns - 1, 0);
		newmsgs = 0;
		if (playerout())
			actres = 0;
		else {
			actres = perform(units, actions[actsel].cmd, acta, actb, actc, 0);
			if (newmsgs) {
				actcmd = actions[actsel].cmd;
				if (actcmd == 2 || actcmd == 0xd || actcmd == 0xe) {
					if (!actres)
						goto call;
				} else {
				call:
					prependmsg(strs[actions[actsel].name]);
				}
				if (actmsg)
					delmsgid(actmsg);
			} else if (actres == 0) {
				if (actmsg)
					delmsgid(actmsg);
				nosuccess(strs[actions[actsel].name]);
			}
		}
	} else {
		if (newmsgs == 0)
			actfail();
		else
			prependmsg(strs[actions[actsel].name]);
	}
	fillgauges();
	lookaround();
	drawscreen();
	if (actres)
		lastfail = 0xff;
	else
		lastfail = actions[actsel].cmd;
	acttime = gametime;
	return 1;
}

short
topicok(short n)
{
	topicq = textlines[n];
	if (topicq[0] == 0)
		return 0;
	while (*--topicq == 0)
		;
	do
		topicq--;
	while (topicq[-1]);
	if (*topicq == '-')
		return 0;
	return 1;
}

int
clickout(void)
{
	if (menu1.over == 0 && (mouseleft || leftclick))
		return 1;
	return keyhit();
}

int
keyhit(void)
{
	if (!keycode)
		return 0;
	return 1;
}

short
passkey(unsigned char scan, unsigned char key)
{
	return 3;
}

/* turnwait holds both the saved shown and the frame deadline, so the final test always passes */
void
turnpage(int a, int b, int c, int d)
{
	/* i only shapes the code: with it the second loop multiplies by 35 with imul */
	int i;

	if (!d && pageturn == 2 || pageturn == 0) {
		flip();
		return;
	}
	if (pageturn == 3 && d != 2) {
		fadefast(0);
		flip();
		fadefast(1);
		return;
	}
	turnwait = shown;
	if (turnwait)
		hidecursor();
	if (b > 0) {
		blitto(0, 0, b, 1, 0, b, 0x6a, c);
		blitto(0, 0x216, b, 1, 0x216, b, 0x6a, c);
	}
	backto2();
	pagedone();
	if (!muted())
		playnow(0x18);
	if (a) {
		for (turnstep = 11; turnstep > 0; turnstep--) {
			turnwait = ticks() + 2;
			blitto(0, 0x6a, b, 1, 0x8d, b, turnstep * 35, c);
			blitto(0, turnstep * 35 + 0x6a, b, 2, 0x6a, b, (12 - turnstep) * 35, c);
			if (turnsides) {
				pgsides();
				puthands();
			}
			flip();
			while (ticks() < turnwait)
				pollinput();
		}
	} else {
		for (turnstep = 1; turnstep <= 11; turnstep++) {
			turnwait = ticks() + 2;
			blitto(0, turnstep * 35 + 0x6a, b, 1, (turnstep - 1) * 35 + 0x6a, b, (12 - turnstep) * 35, c);
			blitto(0, 0x6a, b, 2, (12 - turnstep) * 35 + 0x6a, b, turnstep * 35, c);
			if (turnsides) {
				pgsides();
				puthands();
			}
			flip();
			while (ticks() < turnwait)
				pollinput();
		}
	}
	s2toback();
	flip();
	mousemoved = 1;
	readinput();
	if (turnwait)
		drawcursor();
}

void
pagenum(int i, int n)
{
	if (n > 1) {
		if (!overpic)
			inkwhite();
		text2o(0, 0x48c86, texts + 0xb10c, i + 1, n);
		inkshade();
	}
}

int
textwin(char *s, char a, unsigned char b, unsigned char c, void (*pagefn)(short), Menu *m, int (*fn)(int), unsigned char mode)
{
	firstshow = 1;
	atlast = 0;
	slidein = 0;
	pastfirst = 0;
	slide = 0;
	if (mode == 0xfe) {
		slidein = 1;
		mode = 0xff;
	} else if (mode == 0xff)
		slidein = 2;
	if (curtopic != 0xfff && s != textlines[curtopic])
		curtopic = 0xfff;
	if (b == 0xfe || b == 0xff) {
		pagemode = b;
		b = 0;
	} else
		pagemode = 1;
	for (textidx = 0; (int)textidx < nstrs; textidx++)
		if (strs[textidx] == s)
			goto found;
	textidx = 0xffff;
found:
	if (!overpic)
		nop();
restart:
	curpage = 0;
relayout:
	if (curtopic != 0xfff) {
		if (chcut[curtopic])
			pagemode = 0xfe;
		else
			pagemode = 1;
	}
	npages = 0;
	pagetext = s;
	do
		pagestarts[npages++] = pagetext;
	while (drawpage(&pagetext, pagex, 2, 0, pagemaxh, &menu1));
	if (!overpic && !nofade && !a && !slidein) {
		if (mode == 1)
			fadeslow(0);
		else if (b)
			fademed(0);
		else
			fadefast(0);
	}
	if (!overpic)
		usepal(0);
	entermenu();
	drawcursor();
	if (atlast) {
		curpage = npages - 1;
		atlast = 0;
	}
	if (startpage) {
		if (startpage < npages)
			curpage = startpage;
		startpage = 0;
	}
	if (slidein)
		slide = slidein;
redraw:
	if (!overpic)
		pgtoback();
	else
		showscr();
	if (pagearrows) {
		pagearrows = 0;
		setcursor(0, 0xffff, 0, 0, 0, 0);
	}
	if (curpage > 0) {
		putcs(0x18, 0xd8, 0x38);
		pagearrows |= 1;
	} else if (curtopic != 0xfff && curtopic > 0) {
		putcs(0x18, 0xd8, 0x67);
		pagearrows |= 1;
	}
	if (npages > 1 && curpage < npages - 1) {
		putcs(0x246, 0xd8, 0x39);
		pagearrows |= 2;
	} else if (curtopic != 0xfff && curtopic < ntextlines - 1) {
		putcs(0x246, 0xd8, 0x3d);
		pagearrows |= 2;
	} else if (npages > 1) {
		if (curpage < npages - 1) {
			putcs(0x246, 0xd8, 0x39);
			pagearrows |= 2;
		} else
			putcs(0x246, 0xd8, 0x3c);
	}
	if (curpage > 0)
		pastfirst = 1;
	pagenum(curpage, npages);
	pagetext = pagestarts[curpage];
	if (overpic)
		pagebright = 1;
	drawpage(&pagetext, pagex, pagemode, 0, pagemaxh, &menu1);
	pagebright = 0;
	if (pagefn)
		pagefn(curpage + 1);
	if (!overpic)
		puthands();
	if (m) {
		backto2();
		if (menudraw || menutick)
			menubase();
	}
	pagedone();
	mousemoved = 1;
	readinput();
	if (!palblack) {
		if (slide || overpic) {
			if (!overpic)
				turnpage(slide - 1, 0, 480, 1);
			else
				dissolvet();
			goto shown;
		}
		if (slidein) {
			turnpage(slidein - 1, 0, 480, 1);
			goto shown;
		}
	}
	flip();
shown:
	if (palblack || firstshow) {
		if (textsong)
			music(textsong, 0);
		if (palblack) {
			if (mode == 1)
				fadeslow(1);
			else if (b)
				fademed(1);
			else
				fadefast(1);
		}
		if (textsound) {
			playnow(textsound);
			textsound = 0;
		} else if (!muted())
			playnow(0x18);
		firstshow = 0;
	}
	slidein = 0;
	if (a) {
		if (pagearrows) {
			pagearrows = 0;
			setcursor(0, 0xffff, 0, 0, 0, 0);
		}
		return 1;
	}
	if (esckey == 0xfe) {
		do {
			esckey = 0;
			waitstart = ticks();
			do {
				pollinput();
				if (winclosed())
					break;
			} while (since(waitstart) < 200);
		} while (esckey == 0xff);
	}
	if (esckey != 0xff)
		esckey = 0;
wait:
	waitforup = 1;
	while (mouseleft || mouseright || keycode) {
		pollinput();
		if (esckey || winclosed())
			break;
	}
	leftclick = 0;
	waitforup = 0;
	pollinput();
	if (m) {
		textresult = runmenu(m, clickout, passkey, 0);
		if (needredraw(textresult))
			goto relayout;
		if (textresult == 0) {
			textresult = 0;
			goto done;
		}
		if (textresult != 0xff && textresult != 0xfe) {
		chosen:
			textresult += (curpage + 1) * 100 + 10000;
			if (fn == NULL)
				goto done;
			fnresult = fn(textresult);
			if (fnresult == 0)
				goto wait;
			if (fnresult == 1)
				goto relayout;
			if (fnresult == 2)
				goto restart;
			if (fnresult == 3)
				goto done;
			if (fnresult >= 1001) {
				curpage = fnresult - 1001;
				if (curpage > npages - 1)
					curpage = npages - 1;
				goto relayout;
			}
			goto wait;
		}
		textresult = 1;
		goto key;
	}
	while (!mouseleft && !mouseright && !keycode && !esckey) {
		readinput();
		if (needredraw(0xfd))
			goto relayout;
	}
key:
	pollinput();
	pgkey = keycode;
	if (m && (pgkey == 0x1c || pgkey == 0xe || esckey)) {
		menukey = esckey ? 1 : pgkey;
		switch (hotkey(m, menukey)) {
		case 1:
			textresult = m->sel;
			goto chosen;
		case 2:
			pgkey = 0;
			break;
		}
	}
	if (cheat) {
		if (pgkey == 0x4a && textidx != 0xffff && textidx > 0) {
			textidx--;
			curpage = 0;
			s = strs[textidx];
			goto relayout;
		}
		if (pgkey == 0x4e && textidx != 0xffff && textidx < nstrs - 1) {
			textidx++;
			s = strs[textidx];
			curpage = 0;
			goto relayout;
		}
	}
	if (pgkey == 0x4b || mouseleft && mousex < 320) {
		if (keys[VK_CONTROL] & 0x80) {
			if (curtopic == 0xfff || curtopic <= 0)
				goto wait;
		} else {
			if (curpage > 0) {
				curpage--;
				slide = 1;
				goto redraw;
			}
			if (curtopic != 0xfff && curtopic > 0)
				atlast = 1;
			else
				goto first;
		}
		do
			curtopic--;
		while (curtopic && !topicok(curtopic));
		s = textlines[curtopic];
		slide = 1;
		curpage = 0;
		slidein = 1;
		goto relayout;
	first:
		if (npages > 1 && !pastfirst)
			goto next;
		goto last;
	}
	if (pgkey == 0x4d || mouseleft && mousex >= 320) {
		if (keys[VK_CONTROL] & 0x80) {
			if (curtopic == 0xfff || curtopic >= ntextlines - 1)
				goto wait;
		} else {
			if (curpage < npages - 1) {
			next:
				curpage++;
				slide = 2;
				goto redraw;
			}
			if (curtopic != 0xfff && curtopic < ntextlines - 1)
				goto nexttext;
			goto last;
		}
	nexttext:
		do
			curtopic++;
		while (curtopic < ntextlines - 1 && !topicok(curtopic));
		s = textlines[curtopic];
		slide = 2;
		curpage = 0;
		slidein = 2;
		goto relayout;
	}
	if (pgkey == 0x47) {
		if (npages <= 1)
			goto last;
	home:
		if (curpage <= 0)
			goto wait;
		curpage = 0;
		slide = 1;
		goto redraw;
	}
	if (pgkey == 0x4f) {
		if (npages <= 1)
			goto last;
	end:
		if (curpage >= npages - 1)
			goto wait;
		curpage = npages - 1;
		slide = 2;
		goto redraw;
	}
	if (pgkey == 0x49) {
		if (npages <= 1)
			goto last;
		if (curpage < 5)
			goto home;
		curpage -= 5;
		slide = 1;
		goto redraw;
	}
	if (pgkey == 0x51) {
		if (npages <= 1)
			goto last;
		if (curpage >= npages - 6)
			goto end;
		curpage += 5;
		slide = 2;
		goto redraw;
	}
	if (pgkey >= 2 && pgkey <= 10) {
		if (npages <= 1)
			goto last;
		if (pgkey - 2 >= npages || pgkey - 2 == curpage)
			goto wait;
		slide = pgkey - 2 < curpage ? 1 : 2;
		curpage = pgkey - 2;
		goto redraw;
	}
	if (npages > 1 && !esckey && !mouseright) {
		if (curpage < npages - 1)
			goto next;
		textresult = 1;
		goto done;
	}
last:
	textresult = 1;
done:
	clicksound();
	if (pagearrows) {
		pagearrows = 0;
		setcursor(0, 0xffff, 0, 0, 0, 0);
	}
	if (!nofade && mode != 0xff) {
		if (mode == 1)
			fadeslow(0);
		else if (b)
			fademed(0);
		else
			fadefast(0);
	}
	if (!overpic)
		nop();
	if (c != 0xff) {
		if (!c) {
			waitrelease(0);
			return textresult;
		}
		leavemenu(1);
	}
	return textresult;
}

void
picpage(short pic, short i)
{
	nop();
	fademed(0);
	do {
		if (!pic)
			clrback();
		else {
			loadscr(pic, (unsigned char *)scrbuf, (unsigned char *)gamepal, packbuf, 0x96640);
			showscr();
			keepui(gamepal);
		}
		picp = strs[i];
		drawpage(&picp, 0x6a, 0, 1, 400, &menu1);
		hidecursor();
		flip();
		fademed(1);
		waitinput(-1);
	} while (waitlost);
	fademed(0);
	nop();
	usepal(0);
	flip();
	drawcursor();
}

void
aboutdone(void)
{
}

void
drawenter(short x, short y)
{
	x += 7;
	if (x > 0x270)
		x = 0x270;
	text3(0, x, y, texts + 0xb575);
}

int
inputtext(short x, short y, char *s, short max, char mode, char flag, char toup, char filter, char cursor, short nfields, short maxw, short maxh, Menu *m)
{
	pagedone();
	getscreen(2, packbuf);
	s2toback();
	nkeys = 0;
	if (inkeep) {
		inkeep = 0;
		nkeys = 1;
	}
	infield = 0;
	if (flag)
		setkeymode(1);
refield:
	if (insetfn) {
		insetfn(infield, &s, &max);
		nkeys = 0;
	}
	if (infieldfn) {
		infieldfn(infield);
		save2();
		memcpy(packbuf, scrbuf, 0x4b000);
	}
	inx = x;
	iny = y + fieldh * infield;
	for (curpos = 0; s[curpos]; curpos++)
		;
	inlen = curpos;
	memset(inbuf, 0, 100);
	undopos = curpos;
	copystr(undobuf, s);
	inkey = 0;
redraw:
	putrows((unsigned char *)packbuf, iny, 0x14);
	if (mode == 1)
		text3grey(0, nfields ? inx : 0xffff, iny, s);
	else if (mode == 2)
		text3e1(0, 0xffff, iny, s);
	else
		text3(0, nfields ? inx : 0xffff, iny, s);
	inx0 = text3x;
	inxend = inx0 + text3width;
	if (inxend >= 0x268)
		max = inlen;
	if (cursor) {
		for (inputi = 0; s[inputi] && inputi < curpos; inputi++)
			inbuf[inputi] = s[inputi];
		inbuf[inputi] = 0;
		text3w(inbuf);
		((void (*)(unsigned short, unsigned short))setcaret)(inx0 + text3width, iny);
	}
	drawenter(inxend, iny);
	backto2rows(iny, iny, 0x14);
	drawcaret();
	flip();
	fadefast(1);
	while (mouseleft || mouseright)
		readinput();
	if (flag) {
		keycode = 0;
		lastchar = 0;
	}
	if (m) {
	menu:
		inj = runmenu(m, keyhit, passkey, 0);
		if (surfslost) {
			surfslost = 0;
			if (inj == 0xfd) {
				inresult = inj;
				goto finish;
			}
		}
		if (inj == 0) {
			inresult = inj;
			goto finish;
		}
		if (inj != 0xff && inj != 0xfe) {
			inresult = inj + 10000;
			goto finish;
		}
	}
	if (esckey == 0xff) {
	cancel:
		inresult = 2;
		goto finish;
	}
	if (flag) {
		inkey = 0;
	getkey:
		if (inkey != 0)
			goto gotkey;
		readinput();
		if (surfslost) {
			surfslost = 0;
			inresult = 0xfd;
			goto finish;
		}
		stepcaret();
		if (esckey == 0xff)
			goto cancel;
		inscan = keycode;
		inkey = lastchar;
		if (inscan && cursor) {
			if (inscan == 0x47) {
				nkeys++;
				if (keys[VK_CONTROL] & 0x80 && nfields && infield > 0) {
					infield = 0;
					goto refield;
				}
				curpos = 0;
				goto redraw;
			}
			if (inscan == 0x4f) {
				nkeys++;
				if (keys[VK_CONTROL] & 0x80 && nfields && infield < nfields - 1) {
					infield = nfields - 1;
					goto refield;
				}
				curpos = inlen;
				goto redraw;
			}
			if (inscan == 0x4b) {
				nkeys++;
				if (keys[VK_CONTROL] & 0x80) {
					curpos = wordstart(s, curpos, inlen);
					goto redraw;
				}
				if (curpos > 0) {
					curpos--;
					goto redraw;
				}
				goto other;
			}
			if (inscan == 0x4d) {
				nkeys++;
				if (keys[VK_CONTROL] & 0x80) {
					curpos = wordnext(s, curpos, inlen);
					goto redraw;
				}
				if (curpos < inlen) {
					curpos++;
					goto redraw;
				}
				goto other;
			}
			if (inscan == 0x48 || inscan == 0xf && keys[VK_SHIFT] & 0x80) {
				nkeys++;
				if (nfields && infield > 0)
					infield--;
				goto refield;
			}
			if (inscan == 0x50 || inscan == 0xf) {
				nkeys++;
				if (nfields && infield < nfields - 1)
					infield++;
				goto refield;
			}
			if (inscan == 0x53 && curpos < inlen) {
				nkeys++;
				if (keys[VK_CONTROL] & 0x80) {
					undopos = curpos;
					copystr(undobuf, s);
					inj = wordnext(s, curpos, inlen);
					while (inj > curpos) {
						inputi = curpos;
						inj--;
						inlen--;
						while (inputi <= inlen) {
							s[inputi] = s[inputi + 1];
							inputi++;
						}
						s[inlen] = 0;
					}
				edited:
					hoversound();
					goto redraw;
				}
				undopos = curpos;
				copystr(undobuf, s);
				goto delchar;
			}
			if (inscan == 0xe) {
				nkeys++;
				if (keys[VK_SHIFT] & 0x80) {
					inputi = curpos;
					copystr(inbuf, s);
					curpos = undopos;
					copystr(s, undobuf);
					undopos = inputi;
					copystr(undobuf, inbuf);
					for (inlen = 0; s[inlen]; inlen++)
						;
					goto edited;
				}
				if (!(keys[VK_CONTROL] & 0x80))
					goto backspace;
				goto delword;
			}
			if (inkeyfn) {
				nkeys++;
				inj = inkeyfn(inkey);
				if (inj == 1)
					goto nextfield;
				if (inj == 2)
					goto cancel;
				goto redraw;
			}
		}
	other:
		if (inkey) {
			nkeys++;
			goto getkey;
		}
		if (m)
			goto menu;
		if (mouseright && esckey != 0xff) {
			esckey = 1;
			inkey = 0x1b;
		} else if (mouseleft)
			inkey = 0xd;
		goto getkey;
	} else {
		inkey = readkey();
		if (surfslost) {
			surfslost = 0;
			if (inkey == 0xfd) {
				inresult = inkey;
				goto finish;
			}
		}
		nkeys++;
	}
gotkey:
	if (toup)
		inkey = upper(inkey);
	if (esckey || inkey == 0x1b)
		goto cancel;
	if (inkey == 8) {
	backspace:
		undopos = curpos;
		copystr(undobuf, s);
		if (curpos <= 0)
			goto checkenter;
		curpos--;
	delchar:
		inputi = curpos;
		inlen--;
		while (inputi <= inlen) {
			s[inputi] = s[inputi + 1];
			inputi++;
		}
		s[inlen] = 0;
		hoversound();
	} else {
		if (inkey == 0x7f) {
		delword:
			undopos = curpos;
			copystr(undobuf, s);
			inj = wordstart(s, curpos, inlen);
			while (curpos > inj) {
				curpos--;
				inputi = curpos;
				inlen--;
				while (inputi <= inlen) {
					s[inputi] = s[inputi + 1];
					inputi++;
				}
				s[inlen] = 0;
			}
			goto edited;
		}
		if (inkey == 0xd) {
		nextfield:
			if (nfields && infield < nfields - 1) {
				infield++;
				goto refield;
			}
			clicksound();
			goto enter;
		}
		if (nkeys <= 1 && iswordc(inkey))
			inkey = upper(inkey);
		if (inkey == ' ' && nkeys <= 1) {
			undopos = curpos;
			copystr(undobuf, s);
			inlen = curpos = 0;
			s[inlen] = 0;
			hoversound();
			goto checkenter;
		}
		if (inkey == ' ' && curpos <= 0)
			goto checkenter;
		if (filter && !(iswordc(inkey) || inkey == '-' || filter == 1 && inkey == ' '))
			goto checkenter;
		if (max > inlen) {
			if (nkeys > 1 && (maxw || maxh)) {
				copystr(namebuf, s);
				for (inputi = inlen; inputi >= curpos; inputi--)
					namebuf[inputi + 1] = namebuf[inputi];
				namebuf[curpos] = inkey;
				namebuf[inlen + 1] = 0;
				if (maxw) {
					text3w(namebuf);
					if (maxw < text3width)
						goto checkenter;
				}
				if (maxh && textwidth(namebuf) > maxh)
					goto checkenter;
			}
			undopos = curpos;
			copystr(undobuf, s);
			if (nkeys <= 1) {
				inlen = curpos = 0;
				s[0] = 0;
			}
			for (inputi = inlen; inputi >= curpos; inputi--)
				s[inputi + 1] = s[inputi];
			s[curpos++] = inkey;
			inlen++;
			s[inlen] = 0;
			hoversound();
		}
	}
checkenter:
	if (inkey == 0xd) {
	enter:
		if (!(curpos == 0 && !nfields)) {
			if (inkey == 0xd)
				clicksound();
			while (s[curpos])
				curpos++;
			while (curpos > 1 && s[curpos - 1] == ' ')
				curpos--;
			inresult = 1;
			s[curpos] = 0;
			goto finish;
		}
	}
	goto redraw;
finish:
	clicksound();
	inkeyfn = 0;
	infieldfn = 0;
	insetfn = 0;
	if (flag)
		setkeymode(0);
	tabkey = 0;
	stopcaret();
	pagedone();
	return inresult;
}

int
askvar(short x, short y)
{
	varbuf[0] = 0;
	varres = inputtext(x, y, varbuf, 40, 0, 1, 0, 0, 1, 0, 0, 0, 0);
	if (varres == 1)
		return evalvar(varbuf);
	return 0;
}

int
asknumber(short x, short y, unsigned short ndigits, int min, int max, int init)
{
	short py;

	py = y;
	numfresh = 1;
	numlen = 0;
	wsprintf(numfmt, "(%%5s)");
	numdigits[0] = 0;
	numfmt[2] = ndigits + '0';
	if (init) {
		/* numdigits is empty already: the store only shapes the byte registers */
		numdigits[0] = 0;
		if (init == -1)
			init = 0;
		wsprintf(numdigits, texts + 0xb08d, init);
		for (numlen = 0; numdigits[numlen]; numlen++)
			;
	}
	if (esckey != 0xff)
		esckey = 0;
	for (;;) {
		textn(1, x, py, numfmt, numdigits);
		while (pollinput() && keycode)
			if (esckey)
				return -1;
		while (pollinput() && !keycode)
			if (esckey)
				return -1;
		numkey = keycode;
		if (numkey >= 2 && numkey <= 11 && (numlen < ndigits || numfresh)) {
			if (numfresh) {
				numfresh = 0;
				numdigits[0] = 0;
				numlen = 0;
			}
			if (numkey == 11)
				numkey = '0';
			else
				numkey += 0x2f;
			numdigits[numlen++] = numkey;
			numdigits[numlen] = 0;
		} else if (numkey == 0xe && numlen) {
			numfresh = 0;
			numdigits[--numlen] = 0;
		} else {
			if (numkey == 1)
				return -1;
			if (numkey == 0x1c && numlen) {
				numres = evalvar(numdigits);
				lastscan = 0;
				waitrelease();
				if (numres == -1 || numres < min || numres > max)
					return -1;
				return numres;
			}
		}
		if (esckey)
			return -1;
	}
}

short
showpage(int n, unsigned short pic, unsigned short pic2, unsigned short pic3, unsigned short pic4, unsigned short pic5, char *text, short prev, short next, short sound)
{
	char *p;
	int r;
	unsigned char c;

top:
	p = text;
	clrback();
	loadscr(pic, (unsigned char *)scrbuf, (unsigned char *)((char *)pals[7]), workbuf, 0x96640);
	keepui(((char *)pals[7]));
	showscrt();
	if (pic2) {
		loadscr(pic2, (unsigned char *)scrbuf, 0, workbuf, 0x96640);
		showscrt();
	}
	if (pic3) {
		loadscr(pic3, (unsigned char *)scrbuf, 0, workbuf, 0x96640);
		showscrt();
	}
	if (pic4) {
		loadscr(pic4, (unsigned char *)scrbuf, 0, workbuf, 0x96640);
		showscrt();
	}
	if (pic5) {
		loadscr(pic5, (unsigned char *)scrbuf, 0, workbuf, 0x96640);
		showscrt();
	}
	drawpage(&p, 0xffff, 0xfd, 0x141, 0x95, 0);
	pagearrows = 0;
	if (prev) {
		putcs(0xe, 0x173, 0x38);
		pagearrows |= 1;
	}
	if (next) {
		putcs(0x250, 0x173, 0x39);
		pagearrows |= 2;
	} else if (prev) {
		putcs(0x250, 0x173, 0x3d);
		pagearrows |= 2;
	}
	hidecursor();
	entermenu();
	usepal(7);
	if (palblack) {
		flip();
		fadeslow(1);
	} else {
		if (n != intropage) {
			save0();
			blitn(1, 0, 0, 0, 0, 0, 0x280, 0x137);
			turnsides = 0;
			turnunused = 0;
			turnpage(n > intropage, 0x137, 0xa9, 2);
			turnsides = 1;
			turnunused = 1;
			showscr();
			hidecursor();
		}
		dissolvet();
	}
	setcursor1(3, 3);
	setcursor(0, 0xffff, 0, 0, 0, 0);
	mousemoved = 1;
	readinput();
	drawcursor();
	intropage = n;
	if (sound)
		playnow(sound);
	do
		readinput();
	while (mouseleft || mouseright || keycode);
	r = 0;
	for (;;) {
		readinput();
		if (needredraw(0xfd))
			goto top;
		if (mouseright || esckey)
			break;
		c = keycode;
		if (c) {
			if (c == 0x4b) {
				if (prev) {
					r = -1;
					break;
				}
			} else {
				r = next ? 1 : 0;
				break;
			}
		}
		if (mouseleft) {
			if (hoverx < 0x140 && prev)
				r = -1;
			else if (next)
				r = 1;
			else
				r = 0;
			break;
		}
	}
	hidecursor();
	pagearrows = 0;
	menumode = 0;
	return r;
}

void
intro(unsigned char mode)
{
	short skip;
	short r;

	fadeslow(0);
	skip = 0;
	if (esckey && firststart && esckey != 0xff)
		esckey = 0;
	else if (esckey)
		goto out;
	if (mode == 0x65) {
		skip = 1;
		mode = 1;
		goto show;
	}
	if (mode) {
	show:
		hidecursor();
		usepal(0);
		loadscr(0x2086, (unsigned char *)scrbuf, 0, packbuf, 0x96640);
		showscr();
		if (mode == 3) {
			text3e1(0, 0xffff, 0xe5, strs[0x236]);
			text3e1(0, 0xffff, 0xfb, strs[0x237]);
		} else
			text3e1(0, 0xffff, 0xe6, strs[0x235]);
		flip();
		if (mode < 3 && !skip)
			waitinput(800);
		fadeslow(1);
		starttimer3();
	}
	if (mode != 3)
		music(1, 1);
	if (esckey && firststart && esckey != 0xff || !esckey) {
		starttimer3();
		if (mode) {
			waitinput(0x251c);
			if (mode == 3)
				waitinput(10000);
			fadeslow(0);
			drawcursor();
			if (mode == 3) {
				starttimer3();
				goto out;
			}
			if (!skip)
				waitinput(1000);
		}
		if (esckey && firststart && esckey != 0xff)
			esckey = 0;
		else if (esckey)
			goto out;
		palblack = 1;
		music(1, 1);
		if (esckey && firststart && esckey != 0xff)
			esckey = 0;
		intropage = -1;
		while (!esckey) {
			r = showpage(1, 0xfa7, 0xfab, 0, 0, 0xfa6, strs[0x238], 0, 1, 0);
			if (r == 0 || esckey)
				break;
			for (;;) {
				r = showpage(2, 0xfa7, 0xfac, 0, 0, 0xfa6, strs[0x239], 1, 1, 0);
				if (r == 0 || esckey)
					goto out;
				if (r == -1)
					break;
				for (;;) {
					r = showpage(3, 0xfa7, 0xfa8, 0, 0, 0xfa6, strs[0x23a], 1, 1, 0);
					if (r == 0 || esckey)
						goto out;
					if (r == -1)
						break;
					for (;;) {
						r = showpage(4, 0xfa7, 0xfad, 0, 0, 0xfa6, strs[0x23b], 1, 1, 0);
						if (r == 0 || esckey)
							goto out;
						if (r == -1)
							break;
						for (;;) {
							r = showpage(5, 0xfa7, 0xfaa, 0xfae, 0, 0xfa6, strs[0x23c], 1, 1, 0);
							if (r == 0 || esckey)
								goto out;
							if (r == -1)
								break;
							r = showpage(6, 0xfa7, 0xfa9, 0xfaf, 0, 0xfa6, strs[0x23d], 1, 0, 0);
							if (r == 0 || esckey || r != -1)
								goto out;
						}
					}
				}
			}
		}
	}
out:
	fadeslow(0);
	usepal(0);
	drawcursor();
	loop = 0;
}

void
titlecolors(unsigned char *p, int flag)
{
	int i;

	if (flag) {
		for (i = 0; i < 0x4b000; i++, p++)
			if (*p >= 0x3c && *p <= 0x3f) {
				if (*p == 0x3c)
					*p = 0x74;
				else if (*p == 0x3d)
					*p = 0x76;
				else if (*p == 0x3e)
					*p = 0x77;
				else if (*p == 0x3f)
					*p = 0x78;
			}
	} else {
		for (i = 0; i < 0x4b000; i++, p++)
			if (*p >= 0x3c && *p <= 0x3f)
				*p = 0;
	}
}

void
drawsubtitle(int y, char flag)
{
	if (flag)
		text3e2(0, 0xffff, y, titletail() + 1);
	else
		text3e1(0, 0xffff, y, titletail() + 1);
}

void
maketitle(void)
{
	loadscr(0x2086, (unsigned char *)scrbuf, 0, packbuf, 0x96640);
	loadscr(0x2065, (unsigned char *)picbuf, 0, packbuf, 0x96640);
	titlecolors((unsigned char *)picbuf, 0);
	for (titlei = 0; titlei < 0x4b9c4; titlei++)
		if (picbuf[titlei])
			scrbuf[titlei] = picbuf[titlei];
}

void
menutitle(void)
{
	loadscr(0x2065, (unsigned char *)scrbuf, 0, packbuf, 0x96640);
	titlecolors((unsigned char *)scrbuf, 1);
	putblockt(0, 0x64, 0x28, (unsigned char *)scrbuf, 0x64, 0xaa, 0x1b8, 0xff);
	drawsubtitle(0x87, 1);
}

void
showtitle(void)
{
	titleesc = esckey;
	esckey = 0;
	showscr();
	drawsubtitle(0x109, 0);
	hidecursor();
	flip();
	primtoback();
	fadeslow(1);
	if (!esckey)
		esckey = titleesc;
}

int
presents(void)
{
	clrback();
	text32c(0, 0x140, 0xd0, strs[0xf0]);
	text3e1(0, 0xffff, 0xf0, strs[0xf1]);
	fadefast(0);
	if (esckey == 0xff)
		return 0;
	usepal(0);
	hidecursor();
	flip();
	fadeslow(1);
	return 1;
}

void
credits(void)
{
	loadbigfont();
	loadscr(0x2086, (unsigned char *)scrbuf, 0, packbuf, 0x96640);
	fadeslow(0);
	usepal(0);
	do {
		showscr();
		text3e1(0, 0xffff, 0xaa, strs[0x23f]);
		text3e1(0, 0xffff, 0xc8, strs[0x240]);
		text32c(0, 0x140, 0xfa, strs[0x243]);
		flip();
		fadeslow(1);
		if (esckey && firststart && esckey != 0xff)
			esckey = 0;
		waitinput((firststart ? 1500 : 0) + 9500);
	} while (waitlost);
	fadeslow(0);
	if (esckey && firststart && esckey != 0xff)
		esckey = 0;
	else if (esckey)
		return;
	do {
		showscr();
		text3e1(0, 0xffff, 0xc8, strs[0x245]);
		text32c(0, 0x140, 0xfa, strs[0x247]);
		waitinput(1000);
		flip();
		fadeslow(1);
		if (esckey && firststart && esckey != 0xff)
			esckey = 0;
		waitinput((firststart ? 1500 : 0) + 5000);
	} while (waitlost);
	if (esckey && firststart && esckey != 0xff)
		esckey = 0;
	fadeslow(0);
}

unsigned char wascalm = 1;
unsigned char wasable = 1;
short pagex = 106;
short pagemaxh = 0x190;
int turnsides = 1;
int turnunused = 1;
int turnunused2 = 255;
int tickms = 100;
short curtopic = 0xfff;
short pagefx = 1;
short listw = 0x168;
short lastfail = 255;
short fieldh = 20;
int intropage = -1;
short aliasi;
short stepi;
short menuoveri;
short hotkeyi;
short oldsel;
short menuk;
short clicked;
int keyx;
int keyy;
char key;
short savecur;
short lastcur;
DWORD esctime;
short savesel;
short savekeysel;
char *cornertext;
char shotname[16];
short listi;
short listy;
short listlines;
short listback;
char *listtitle;
short texth;
short pagek;
short pagen;
short rowwidth;
short pagey;
short pagelines;
short pagefirst;
short pagey0;
char *pagestart;
char *pagep;
char *pageq;
char pagekey;
unsigned char btnrow;
short pagebullet;
char msgesc;
short msgres;
char *urlp;
short printres;
short printbtn;
int printn;
HANDLE printfile;
char printerr[1000];
char printcur;
short paney;
short statatk;
short statv;
short statblk;
short statparry;
short statdmin;
short statdmax;
short statkind;
short cutword;
int staticon;
char *stattime;
short statmode;
char statshield;
short acttipx;
short acttipy;
int actcol;
int actrow;
int acti;
short actj;
unsigned short actk;
char edgetop;
char edgebot;
char edgeleft;
char edgeright;
DWORD topsince;
DWORD botsince;
DWORD leftsince;
DWORD rightsince;
int numok;
int numx;
int numy;
int numunused;
int numslide;
unsigned short *numval;
char numchanged;
unsigned char numctl;
DWORD numnext;
DWORD numdelay;
int numhkind;
int numhcx;
int numhcy;
int numhpx;
int numhpy;
unsigned char tgtopen;
unsigned char tgtperson;
unsigned char tgtcorpse;
unsigned char tgtpack;
unsigned char tgtself;
unsigned char tgtclass;
unsigned long acta;
int actb;
unsigned long actc;
int actres;
int actcmd;
unsigned int actturns;
char *actprompt;
char *acthint;
short actmsg;
short actcalm;
short promptmsg;
char *topicq;
DWORD turnwait;
int turnstep;
unsigned short textidx;
char *pagestarts[50];
short curpage;
short npages;
short pgkey;
short slide;
char *pagetext;
unsigned char pagemode;
unsigned char menukey;
DWORD waitstart;
int textresult;
int fnresult;
char atlast;
char slidein;
char pastfirst;
char firstshow;
char *picp;
short curpos;
short inputi;
short inj;
short inlen;
short infield;
short inxend;
short inx0;
short nkeys;
short undopos;
char inkey;
char inscan;
char inbuf[100];
char undobuf[100];
int inresult;
int inx;
int iny;
char varbuf[40];
int varres;
char numdigits[16];
char numfmt[10];
unsigned char numkey;
int numlen;
int numres;
short numfresh;
unsigned long titlei;
int titleesc;
unsigned short pagearrows;
short startpage;
short nofade;
short textsound;
short textsong;
short overpic;
char *btntext[40];
void (*menutick)(int);
DWORD ticknext;
void (*menudraw)(void);
DWORD hovertime;
char menuslow;
short menusnd;
short menuanim;
Menu menu0;
Menu menu1;
Menu menu2;
char *textlines[100];
char *texttitle;
short ntextlines;
unsigned short shotno;
int statshown;
int barshown;
int noflip;
short listtop;
short listbright;
short pagebright;
short initdone;
int waitlost;
void (*waithook)(void);
char printpage[10000];
short printchoice;
short baritem;
short labelx;
Menu barmenu;
short barsel;
unsigned long acttime;
unsigned char actsel;
short foesnear;
unsigned char actcols;
unsigned char quickacts[10];
char inedge;
int doactclear;
int doactspare;
short (*inkeyfn)(char c);
void (*infieldfn)(short field);
void (*insetfn)(short field, char **s, short *max);
int inkeep;

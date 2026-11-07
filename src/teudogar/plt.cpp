#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

typedef struct Unit Unit;
typedef struct Game Game;
typedef struct Region Region;
typedef struct Kind Kind;
typedef struct Realm Realm;
typedef struct Panel Panel;
typedef struct Labels Labels;
typedef struct Reg Reg;

/* 0x1b8 bytes each (009bc240) */
struct Unit {
	unsigned char x;
	unsigned char y;
	unsigned char nextx;
	unsigned char nexty;
	unsigned char dirx;
	unsigned char diry;
	char f6[8 - 6];
	unsigned char pose;
	unsigned char mode;
	char modea;
	unsigned char modeb;
	unsigned char modec;
	char moded;
	unsigned char schedmode[24];
	unsigned char scheda[24];
	unsigned char schedb[24];
	unsigned char postx[24];
	unsigned char posty[24];
	char f86[0xe9 - 0x86];
	unsigned short scene;
	short plea;
	char fed[0xf3 - 0xed];
	short health;
	char ff5[0xf7 - 0xf5];
	unsigned char leader;
	char ff8[0x100 - 0xf8];
	unsigned long belt;
	char f104[0x108 - 0x104];
	unsigned long body;
	unsigned long hand;
	unsigned long offhand;
	char f114[0x11c - 0x114];
	unsigned long items[3];
	char f128[0x12f - 0x128];
	unsigned char courage;
	char turnmarks;
	unsigned char clan;
	char f132[0x135 - 0x132];
	char asleep;
	char f136[0x138 - 0x136];
	unsigned char poison;
	unsigned char cursed;
	char fleeing;
	char confused;
	unsigned char bleeding;
	char berserk;
	unsigned char drunk;
	char f13f[0x141 - 0x13f];
	unsigned short home;
	unsigned char traits;
	short mercy;
	signed char sympathy;
	unsigned char type;
	char flags;
	unsigned char lord;
	char f14a[0x153 - 0x14a];
	unsigned char holdtime;
	char f154[0x159 - 0x154];
	unsigned char modee;
	unsigned char modef;
	char f15b[0x15d - 0x15b];
	unsigned char restturns;
	char f15e[0x161 - 0x15e];
	short face;
	unsigned short number;
	unsigned char spouse;
	char f166[0x16f - 0x166];
	unsigned char talkflags;
	char f170[0x1b8 - 0x170];
};

/* 0xd0 bytes each (005acf80) */
struct Region {
	char f0[0x69];
	unsigned char site;
	char f6a[0x6e - 0x6a];
	short id;
	char f70[0xd0 - 0x70];
};

/* what kindof returns */
struct Kind {
	char f0[0x18];
	short maxhealth;
};

/* 0x158 bytes each (005a1b78) */
struct Realm {
	unsigned long item;
	char f4[0x158 - 4];
};

/* 0x28 bytes each (00a4ca78): the screen's panels */
struct Panel {
	char f0[6];
	int x;
	int y;
	int w;
	int h;
	unsigned short id;
	char f18[0x28 - 0x18];
};

/* the map's labels at 005e68da: count, texts, places (setmark) */
struct Labels {
	short n;
	char f2[0x6 - 0x2];
	char text[23][20];
};

/* the registration as the registry holds it, 200 bytes (009a66d8) */
struct Reg {
	unsigned char oldmajor;
	unsigned short oldminor;
	char f3;
	unsigned short seen;
	unsigned char oldinfo;
	char f7[0x64 - 7];
	unsigned char key[100];
};

/* the state saved with the game, 0x5f8 bytes at 009cc470 */
struct Game {
	char f0[0x34];
	char flags[0xc8];
	char ffc[0x17e - 0xfc];
	short queuedtext;
	unsigned char queuedprio;
	unsigned short stolen;
	char f183[0x185 - 0x183];
	unsigned short sleepers;
	unsigned short ambushed;
	char f189[0x18b - 0x189];
	unsigned short defeated;
	char f18d[0x28a - 0x18d];
	char fate;
	short killer;
	char f28d[0x316 - 0x28d];
	unsigned short poisoned;
	char f318[0x340 - 0x318];
	char toldvision[30];
	char tolda[30];
	char toldb[30];
	char f39a[0x466 - 0x39a];
	unsigned char robberies;
	char f467[0x4bb - 0x467];
	short curseday;
	char f4bd[0x5f8 - 0x4bd];
};

void setareas(Unit *u);
int badunit(Unit *u);
void setunitmode(Unit *u, unsigned char mode);
short celldist(int x, int y, int x2, int y2);
unsigned long popitem(int pos, char any);
unsigned long additem(int pos, unsigned long item);
void markunits(void);
Kind *kindof(Unit *u);
void closenote(short id, short sound);
short addnote(short id);
void dropnote(short id);
void forgetplayer(Unit *u);
void copyhour(Unit *u, unsigned char k, unsigned char v);
void bruglowfeast(void);
unsigned short newunit(unsigned char kind, unsigned char type);
void flagunit(Unit *u);
short isherder(Unit *u);
unsigned short homeof(Unit *u);
short provat(short x, short y);
void calcstats(Unit *u);
void placeunit(Unit *u, short h);
void applyhour(Unit *u, short h);
int travel(unsigned short dest, int x, int y, unsigned long time, unsigned short depart, unsigned short a6, unsigned short mode, short a8);
short hasnote(short id);
void addscore(int n);
short noteopen(short id);
void clearavenge(void);
short isfemale(Unit *u);
short mapzone(short id);
unsigned long inpack(short *pk, short *pj, unsigned long item, int norm);
int wornslot(Unit *u, unsigned long item, int norm);
short isanimal(Unit *u);
unsigned long finditem(Unit *u, unsigned long item, int *realm, int *cell, int *slot, int mode);
unsigned long takeitem(int pos, unsigned long idx);
unsigned long takeout(unsigned short k, unsigned short i);
unsigned long valueof(unsigned long item);
unsigned char classof(int i);
unsigned long addcoins(unsigned long *p, unsigned long b);
short indexof(Unit *u);
unsigned long giveitem(short n, unsigned long item, short mode);
void cureunit(int n);
void nomercy(Unit *u);
short scheduled(Unit *u);
short yields(Unit *u, short k, unsigned long item, short m);
void killunit(Unit *u, short a, short b, short c, short d);
unsigned long sackregion(short x, short y, int mode);
unsigned long realmat(int x, int y);
int stow(int k, unsigned long item);
int emptyinto(int k, int a, int b, int c);
unsigned short wearof(unsigned long item);
void setwear(unsigned long *p, unsigned long v);
void dropitem(int pos, unsigned long item);
unsigned long newitem(unsigned long item);
unsigned short findclass(int pos, unsigned char kind);
void lightregion(int region, int n);
int addblood(int x, int y, int a, int b);
void fillunits(int g);
short regionat(short x, short y);
void provoke(Unit *a, Unit *b, signed char c, short d, short e);
short seesunit(Unit *u);
short myservant(Unit *u);
void deedbreakword(void);
unsigned short blessof(unsigned long item);
int music(int n, int lp);
int panelrealm(int n);
long wealth(int mode);
short fatevision(void);
int fadeslow(int in);
void waitinput(int ms);
void drawscreen(void);
short isrobber(Unit *u);
void setmark(short i, int x, int y);
int markposx(int i);
int markposy(int i);
int keydown(int key, int lp);
int keyup(int key, int lp);
int songdone(WPARAM wp, LPARAM lp);
int restoresurfs(void);
void hidecursor(void);
void drawcursor(void);
void resetinput(void);
int get48(unsigned char *dst, int n, int x, int y);
int putc1(int x, int y, int n);
int put481(int x, int y, unsigned char *src);
int replay(void);
short playermoving(void);
void setcursor1(int kind, int img);
int setcursor(int kind, int img, int kind1, int img1, int kind2, int img2);
void gearat(unsigned short u, unsigned long *pitem, int *pslot, int x, int y);
short slotat(unsigned short k, unsigned short x, unsigned short y);
short isthrowing(unsigned long item);
short clearsight(int x0, int y0, int x1, int y1);
int cansee(int x, int y);
unsigned char unitat(int x, int y);
int stateflags(Unit *u);
int topitem(int pos);
unsigned long panelgear(int n, int slot);
int needredraw(int c);
int putscreen(char *src, int n);
int flip(void);
int getscreen(int n, char *dst);
void stopsounds(void);
int togglemusic(void);
void screenshot(void);
int stopmusic(void);
char *gametitle(void);
int hasurl(char *s);
void openreadme(void);
char *pick(char *de, char *en, char *fr, char *es, char *it, char *pl);
int inisys(HINSTANCE inst, char *cmd, char *err);
int initwin(HINSTANCE inst, int show);
int ddstart(char *scr, unsigned char *pal);
int inisys2(char *err);
int exitsys(void);
void closemutex(void);
short entergame(unsigned short n);
void mainmenu(void);
void leavemenu(short anim);
void facetmpl(int alt);
int hasversion(void);
int noversion(void);
short msgf(short prio, char *fmt, ...);
void dropheld(void);
void holdmsg(void);
void playerturn(void);
int rununits(short mode);
void playqueue(void);
void runtalker(void);
void heroend(short n);
short endspars(void);
int edgemenus(short mode);
void inputhook(void);
void regaincontrol(void);
void doinput(void);
void gametick(void);
int displayed(Unit *u);
void sayscene(void);
void clearscenes(void);
int tipshown(void);
void cleartexts(void);
void showtip(int x, int y, char *fmt, ...);
short autocombat(void);
void offguard(Unit *u, short d, unsigned char c);
void talkto(unsigned short n);
void playnow(short n);
short skillof(Unit *u, short n, short b, short c, short d);
short prevails(short a, short b);
void hurt(Unit *a, Unit *u, short kind, short slot, short dmg);
void restfor(short n);
short exhausted(void);
void blackout(short n);
void handover(Unit *u, Unit *v);
unsigned long sackarea(int x0, int y0, int x1, int y1, int n, int mode);
void outcold(void);
void awaken(void);
short nearfoe(int nearby);

extern Unit units[];
extern Game game;
extern Region regions[];
extern unsigned short curmap;
extern unsigned long map[];
extern unsigned long mapitems[];
extern short gameday;
extern int playerx;
extern int playery;
extern int playmode;
extern unsigned char gamehour;
extern unsigned short nunits;
extern unsigned long gametime;
extern unsigned char passmode;
extern short avengers;
extern short victimfemale;
extern short victimname;
extern short victimzone;
extern unsigned char oddsswitch;
extern int nfound;
extern Realm realms[125];
extern char szvars[400];
extern short forcedfate;
extern char *strs[];
extern char texts[47942];
extern short nmarks;
// FIXED: the labels that the Labels view reaches past nmarks
#if defined(FEATURE_FIXES) || defined(PORT)
extern char marks[23][20];
#endif
extern unsigned long mapflags[150 * 150];
extern int appactive;
extern int wasactive;
extern int modeset;
extern int ddready;
extern unsigned char esckey;
extern int clipx0;
extern int clipy0;
extern int clipx1;
extern int clipy1;
extern int mousex;
extern int mousey;
extern int mousemoved;
extern int mouseleft;
extern int mouseright;
extern int leftclick;
extern int rightclick;
extern int dblclick;
extern int presslatch;
extern int clickdx;
extern int clickdy;
extern int clickx;
extern int clicky;
extern int clicktime;
extern unsigned short notedunit;
extern unsigned long notedtime;
extern unsigned char notedkind;
extern unsigned char lastkind;
extern char keycode;
extern char lastscan;
extern char lastchar;
extern char shotkey;
extern char musickey;
extern char sfxkey;
extern int shown;
extern char signback[];
extern int lastsong;
extern int replaydue;
extern int replaywait;
extern int lbutton;
extern int rbutton;
extern int lostcontrol;
extern char talkactive;
extern unsigned short busyturns;
extern char turnkeys;
extern char heldcmd;
extern char trademode;
extern unsigned char keys[256];
extern char dirkeys;
extern char pausekey;
extern unsigned char palblack;
extern char dsready;
extern unsigned char soundson;
extern int musicoff;
extern unsigned char menumode;
extern int hoverx;
extern int hoverpx;
extern int hoverpy;
extern int hoverkind;
extern int hovercx;
extern int hovercy;
extern int ptrdx;
extern int ptrdy;
extern int ptrdistx;
extern int ptrdisty;
extern int waitforup;
extern int movecursor;
extern int ptrpace;
extern unsigned char presskind;
extern unsigned long hovergear;
extern char ptrfar;
extern char ptrblocked;
extern int ptrimg;
extern int ptrmark1;
extern int ptrmark2;
extern unsigned short pagearrows;
extern int ptrunit;
extern int ptrstates;
extern int hovery;
extern int runradius;
extern int slowradius;
extern int presscx;
extern int presscy;
extern unsigned char panelstack[];
extern Panel panels[16];
extern char textbuf[];
extern char tmpbuf[2500];
extern char langcode[5];
extern int quickstart;
extern char scrbuf[309700];
extern unsigned char defpal[768];
extern char dderrmsg[];
extern DWORD ordercmd;
extern char progdir[1000];
extern char savesdir[1000];
extern int cheat;
extern char version[23];
extern Reg reg;
extern unsigned tipend;
extern int featuresoff;
extern unsigned char firststart;
extern unsigned short steptick;
extern short actionturn;
extern int restbreak;
extern short skiptick;
extern short maintick;
extern unsigned short lasttick;
extern short challenger;
extern short merciful;
extern short pleader;
extern Unit *sayunit;
extern short saynum;
extern short nmsgs;
extern int nstrs;
extern unsigned short playercell;

extern int setbedhour;
extern int movehour;
extern int posth;
extern unsigned long drapetype;
extern short osmi;
extern short brugi;
extern int cow;
extern int cowi;
extern int cowx;
extern int cowy;
extern short deathi;
extern short leavei;
extern char callsseen;
extern unsigned long keptitem;
extern int proceeds;
extern unsigned long enterv;
extern short feasti;
extern short feastreg;
extern char savedfate;
extern short visions[30];
extern short nvisions;
extern int manipv;
extern MSG pollmsg;
extern UINT keyvk;
extern int inputok;
extern short hoveri;
extern int hoverdx;
extern int hoverdy;
extern unsigned long hoveritem;
extern Panel *hoverpanel;
extern HGLOBAL pausescr;
extern int initres;
extern unsigned long mainv;
extern unsigned long mainw;
extern int robbed;
extern char combatcause;
extern int tracesite;
extern HWND mainwnd;
extern HINSTANCE appinst;
extern HACCEL accel;
extern int quitting;
extern int exitcode;
extern POINT cursorpos;
extern char initerr[3000];
extern MSG pumpmsg;
extern int signx;
extern int signy;
extern int signcur;

void
setbed(int n, int a, int b)
{
	for (setbedhour = 0; setbedhour < 24; setbedhour++)
		if (units[n].schedmode[setbedhour] == 90) {
			units[n].postx[setbedhour] = a;
			units[n].posty[setbedhour] = b;
		}
	setareas(&units[n]);
}

void
movepost(int n, int a, int b, int c, int d)
{
	for (movehour = 0; movehour < 24; movehour++)
		if (units[n].postx[movehour] == a && units[n].posty[movehour] == b) {
			units[n].postx[movehour] = c;
			units[n].posty[movehour] = d;
		}
}

void
postmode(int n, int a, int b, unsigned char c, unsigned char d, unsigned char e)
{
	for (posth = 0; posth < 24; posth++)
		if (units[n].postx[posth] == a && units[n].posty[posth] == b) {
			units[n].schedmode[posth] = c;
			units[n].scheda[posth] = d;
			units[n].schedb[posth] = e;
		}
}

void
setpost(int n, int i, unsigned char a, unsigned char b)
{
	units[n].postx[i] = a;
	units[n].posty[i] = b;
}

void
drape(int pos, unsigned long item)
{
	while (map[pos] & 0x1000) {
		if (mapitems[map[pos] >> 16] & 0x6000)
			break;
		drapetype = mapitems[map[pos] >> 16] & 0xfff;
		if (drapetype >= 0xa16 || drapetype >= 0x9de)
			break;
		popitem(pos, 0);
	}
	additem(pos, item);
}

void
draperow(int x, int y, int n)
{
	x += y * 150;
	drape(x, 0xa16);
	if (n < 2)
		return;
	x++;
	drape(x, 0xa17);
	if (n < 3)
		return;
	drape(x + 1, 0xa18);
}

void
drapecol(int x, int y, int n)
{
	x += y * 150;
	drape(x, 0x9de);
	if (n < 2)
		return;
	x += 150;
	drape(x, 0x9eb);
	if (n < 3)
		return;
	drape(x + 150, 0x9f8);
}

void
setflag(int n, int v)
{
	setbit(game.flags, n, v);
	if (n == 0x33) {
		if (v)
			game.curseday = gameday;
		else
			game.curseday = 0;
	}
	if (curmap == 0x20) {
		if (n == 0x234 && v) {
			if (getbit(game.flags, 0x235))
				return;
			if (units[10].health > 0 && units[18].health > 0) {
				setbed(18, 76, 53);
				units[18].spouse = 10;
				units[10].spouse = 18;
				units[4].spouse = 0;
			}
			setbit(game.flags, 0x235, v);
		}
	} else if (curmap == 15) {
		if (n == 3)
			callsseen |= 1;
		else if (n == 9)
			callsseen |= 2;
	}
}

void
callscene(Unit *u, unsigned long v)
{
	unsigned long t;

	if (badunit(u))
		return;
	v = (unsigned short)v;
	t = (v & 0xff00) >> 8;
	u->modee = t;
	t = v & 0xff;
	u->modef = t;
}

int
comeover(int n, unsigned short v, int dist)
{
	/*
	 * unused: each local is a temporary of the code generator, and
	 * eleven of them give osmundcall's first loop the original's shifts
	 */
	int u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10;

	if (units[0].health <= 0 || units[n].health <= 0 || units[n].mode == 20 || units[n].mode == 15)
		return 0;
	if (dist > 0 && celldist(units[2].x, units[2].y, playerx, playery) > dist)
		return 0;
	setunitmode(&units[n], 15);
	callscene(&units[n], v);
	return 1;
}

int
teudogarcall(void)
{
	if (curmap != 15)
		return 0;
	if (getbit(game.flags, 0x27) || getbit(game.flags, 5) || getbit(game.flags, 3) || getbit(game.flags, 15)) {
		if (!getbit(game.flags, 3) && units[19].mode == 15) {
			units[19].mode = 0;
			setflag(3, 1);
		}
		return 0;
	}
	if (units[19].health <= 0 || units[19].mode == 20 || units[2].health <= 0 || units[2].mode == 90 || units[19].mode == 15)
		return 0;
	units[2].number |= 0x2000;
	setunitmode(&units[19], 15);
	callscene(&units[19], 0x42d);
	if (gametime < 10) {
		units[19].x = 0x49;
		units[19].y = 0x63;
	}
	markunits();
	return 1;
}

int
ortwincall(void)
{
	if (curmap != 15)
		return 0;
	if (getbit(game.flags, 0x3cd) || getbit(game.flags, 0x27) || getbit(game.flags, 9) || !getbit(game.flags, 15)) {
		if (!getbit(game.flags, 9) && units[17].mode == 15) {
			units[17].mode = 0;
			setflag(9, 1);
		}
		return 0;
	}
	if (units[17].health <= 0 || units[17].mode == 20 || units[1].health <= 0 || units[1].mode == 90 || units[17].mode == 15)
		return 0;
	units[1].number |= 0x2000;
	setunitmode(&units[17], 15);
	callscene(&units[17], 0x434);
	if (gametime < 10) {
		units[17].x = 0x43;
		units[17].y = 0x51;
	}
	markunits();
	return 1;
}

int
osmundcall(void)
{
	if (curmap != 0x30)
		return 0;
	if (getbit(game.flags, 0x1ea))
		return 0;
	if (!getbit(game.flags, 0x1e8))
		return 0;
	if (units[0].health <= 0)
		return 0;
	for (osmi = 1; osmi < nunits; osmi++)
		if ((units[osmi].number & 0x1fff) == 0x1f) {
			if (comeover(osmi, 0xc20, -1) == 0)
				return 0;
			for (osmi = 1; osmi < nunits; osmi++)
				if (units[osmi].type == 0x1c) {
					if (units[osmi].health > 2)
						units[osmi].health = 1;
					units[osmi].bleeding = kindof(&units[osmi])->maxhealth;
				}
			return 1;
		}
	return 0;
}

int
bruglowplot(void)
{
	if (curmap != 0x2d)
		return 0;
	if (getbit(game.flags, 0x1d7) && !getbit(game.flags, 0x1dc)) {
		setflag(0x1dc, 1);
		for (brugi = 20; brugi < 24; brugi++) {
			units[4].postx[brugi] = 0x2e;
			units[4].posty[brugi] = 0x1c;
			units[4].schedmode[brugi] = 1;
			units[4].scheda[brugi] = 0;
			units[4].schedb[brugi] = 0;
		}
		units[4].holdtime = 0;
		units[4].restturns = 0xff;
		bruglowfeast();
	}
	if (getbit(game.flags, 0x1c3) && getbit(game.flags, 0x1d2) && getbit(game.flags, 0x1d5)
	&& (getbit(game.flags, 0x1d8) || units[7].health <= 0 || units[7].sympathy <= 0)
	&& !getbit(game.flags, 0x1cc)) {
		setflag(0x1cc, 1);
		setflag(0x1f9, 1);
		closenote(0x19, 1);
		closenote(0x17, 1);
		addnote(0x3d);
		addnote(15);
		addnote(0x3e);
		forgetplayer(&units[2]);
		forgetplayer(&units[7]);
		if (units[4].health > 0) {
			units[4].spouse = 0;
			units[4].face = 0x6f;
			for (brugi = 0; brugi < 24; brugi++) {
				units[4].postx[brugi] = 0x5e;
				units[4].posty[brugi] = 0x5d;
			}
			if (units[2].health > 0)
				units[4].home = units[2].home;
			else if (units[3].health > 0)
				units[4].home = units[3].home;
			setareas(&units[4]);
			copyhour(&units[4], 7, 15);
			copyhour(&units[4], 8, 15);
			copyhour(&units[4], 0x16, 15);
			copyhour(&units[4], 0x17, 3);
			if (getbit(game.flags, 0x1d6)) {
				if (units[4].sympathy > 0 && units[4].sympathy < 3)
					units[4].sympathy = 3;
			} else
				units[4].sympathy = 4;
			units[4].scene = 0x11c1;
			if (!getbit(game.flags, 0x1d6)) {
				if (units[2].sympathy > 0 && units[2].sympathy < 3)
					units[2].sympathy = 3;
				if (units[3].sympathy > 0 && units[3].sympathy < 3)
					units[3].sympathy = 3;
			}
		}
	}
	if (getbit(game.flags, 0x1c3) && !getbit(game.flags, 0x1d1) && !getbit(game.flags, 0x1d2)) {
		units[5].talkflags |= 8;
		units[5].scene = 0x11a3;
		return comeover(5, 0x11a3, -1);
	}
	if (getbit(game.flags, 0x1c3) && getbit(game.flags, 0x1d2) && !getbit(game.flags, 0x1d3)
	&& !getbit(game.flags, 0x1d5) && units[6].sympathy > 0) {
		units[6].talkflags |= 8;
		units[6].scene = 0x11a4;
		return comeover(6, 0x11a4, -1);
	}
	if (getbit(game.flags, 0x1c3) && getbit(game.flags, 0x1d2) && getbit(game.flags, 0x1d5)
	&& !getbit(game.flags, 0x1d8) && units[7].sympathy > 0) {
		units[7].talkflags |= 8;
		units[7].scene = 0x11a5;
		return comeover(7, 0x11a5, -1);
	}
	if (getbit(game.flags, 0x1c9))
		return 0;
	if (!getbit(game.flags, 0x1cc))
		return 0;
	if (getbit(game.flags, 0x1c4) || getbit(game.flags, 0x1c5) || getbit(game.flags, 0x1d6))
		return 0;
	if (!getbit(game.flags, 0x1d9)) {
		for (brugi = 5; brugi <= 12; brugi++)
			if (units[brugi].health > 0 && units[brugi].sympathy <= 0) {
				setflag(0x1d9, 1);
				break;
			}
	}
	if (getbit(game.flags, 0x1d9) && playery < 0x4b)
		return 0;
	if (playmode != 1)
		return 0;
	if (!comeover(4, 0x11c1, -1))
		return 0;
	return 1;
}

int
radomundcall(void)
{
	if (curmap != 0x18)
		return 0;
	if (getbit(game.flags, 0x91))
		return 0;
	if (!getbit(game.flags, 0x90))
		return 0;
	if (!comeover(2, units[2].scene, 30))
		return 0;
	return 1;
}

int
rejoin(int n, int m, int x, int y)
{
	if (curmap != m || units[0].health <= 0 || units[n].health <= 0 || units[n].mode == 20 || units[n].mode == 15)
		return 0;
	units[n].x = x;
	units[n].y = y;
	cureunit(n);
	setunitmode(&units[n], 15);
	callscene(&units[n], units[n].scene);
	markunits();
	return 1;
}

void
motherwait(void)
{
	if (curmap == 0x3a)
		cureunit(1);
}

void
motherback(void)
{
	rejoin(1, 0x3a, 0x45, 0x6b);
}

void
adalmarwait(void)
{
	if (curmap == 0x3d)
		cureunit(1);
}

void
adalmarback(void)
{
	rejoin(1, 0x3d, 0x49, 0x3d);
}

int
hrolfcall(void)
{
	if (curmap != 15 || units[15].health <= 0)
		return 0;
	if (!getbit(game.flags, 7)) {
		if (!getbit(game.flags, 8) || !getbit(game.flags, 5))
			return 0;
		setflag(7, 1);
	}
	if (units[15].mode == 90 || units[15].mode == 20 || units[15].mode == 15)
		return 0;
	setunitmode(&units[15], 15);
	callscene(&units[15], 0x44b);
	markunits();
	return 1;
}

void
deadret1(void)
{
}

void
bruglowlate(int force)
{
	if (curmap != 0x2d)
		return;
	if (getbit(game.flags, 0x1cc) || getbit(game.flags, 0x1d6) || !getbit(game.flags, 0x1c3))
		return;
	if (getbit(game.flags, 0x1d2) && getbit(game.flags, 0x1d5))
		return;
	if (force || gameday * 24 + gamehour >= ((short *)szvars)[0xa] + 5)
		setflag(0x1d6, 1);
}

void
addcow(int x, int y, unsigned char kind)
{
	cow = (unsigned short)newunit(10, 0x23);
	flagunit(&units[cow]);
	units[cow].traits &= ~4;
	units[cow].lord = 0;
	units[cow].leader = 0;
	for (cowi = 1; cowi < nunits; cowi++)
		if (cowi != cow && units[cowi].health > 0 && isherder(&units[cowi])
		&& regions[homeof(&units[cowi])].site == provat(0x49, 0x57)) {
			units[cow].leader = cowi;
			break;
		}
	cowx = randn(5) + 0x45;
	cowy = randn(5) + 0x7b;
	for (cowi = 0; cowi < 24; cowi++)
		if (cowi >= 6 && cowi <= 20 && units[cow].leader != 0) {
			units[cow].schedmode[cowi] = 7;
			units[cow].postx[cowi] = cowx;
			units[cow].posty[cowi] = cowy;
		} else {
			units[cow].schedmode[cowi] = 0x5a;
			units[cow].scheda[cowi] = kind;
			units[cow].postx[cowi] = x;
			units[cow].posty[cowi] = y;
		}
	setareas(&units[cow]);
	units[cow].x = units[cow].postx[gamehour];
	units[cow].y = units[cow].posty[gamehour];
	calcstats(&units[cow]);
	placeunit(&units[cow], gamehour);
	applyhour(&units[cow], gamehour);
}

void
penancecows(void)
{
	if (curmap != 15)
		return;
	if (!passmode)
		return;
	if (!getbit(game.flags, 0x2a))
		return;
	if (getbit(game.flags, 0x2c))
		return;
	addcow(0x4d, 0x55, 0);
	addcow(0x4d, 0x59, 2);
	addcow(0x4f, 0x59, 2);
	addcow(0x4c, 0x58, 2);
	addcow(0x50, 0x56, 3);
	if (getbit(game.flags, 0x2b))
		addcow(0x4c, 0x56, 1);
	markunits();
	setflag(0x2c, 1);
}

void
envoyangry(void)
{
	if (curmap != 15 || units[64].health <= 0)
		return;
	if (units[64].sympathy <= 0) {
		if (!getbit(game.flags, 0x3f)) {
			setflag(0x3f, 1);
			dropnote(0x26);
			dropnote(0x27);
		}
	} else if (getbit(game.flags, 0x3f))
		setflag(0x3f, 0);
}

void
teudogarwait(void)
{
	if (curmap == 15)
		cureunit(2);
}

void
cureunit(int n)
{
	units[n].bleeding = 0;
	units[n].poison = 0;
}

int
teudogarback(void)
{
	if (curmap != 15)
		return 0;
	travel(15, 0x56, 0x71, 0, 1, 1, 0, 1);
	units[0].dirx = 1;
	units[0].diry = 0;
	units[2].dirx = 0;
	units[2].diry = 1;
	return rejoin(2, 15, 0x57, 0x70);
}

void
plotcheck(void)
{
	teudogarcall();
	ortwincall();
	osmundcall();
	bruglowplot();
	radomundcall();
	bruglowlate(0);
	penancecows();
	envoyangry();
}

int
plotheir(short m, short k)
{
	if (m == 15 && k == 2)
		return 9;
	return 0;
}

short
fairgame(short m, short k)
{
	if (m == 0x20 && k == 4 && hasnote(0x13))
		return 1;
	return 0;
}

void
plottreasure(short m, short x, short y)
{
	if (m == 0x28) {
		if (x == 0x79 && y == 0x36) {
			setflag(0x1ec, 1);
			setflag(0x1ed, 1);
			closenote(0xd, 1);
		}
	} else if (m == 0x21) {
		if (x == 0x54 && y == 0x4e)
			closenote(0x15, 1);
	} else if (m == 0x25) {
		if (x == 0x7c && y == 0x76)
			closenote(0x16, 1);
	} else if (m == 0x10) {
		if (x == 0x3f && y == 0x2c)
			closenote(0x20, 1);
	} else if (m == 0x3b) {
	} else if (m == 0x36) {
		if (x == 0x82 && y == 0x2d) {
			setflag(0x41, 1);
			closenote(0x27, 1);
		}
	}
}

void
scorenote(int n)
{
	addscore(100);
}

void
closeshop(void)
{
	if (noteopen(11))
		closenote(11, 0);
}

int
deadspeaks(unsigned short n)
{
	switch (n) {
	case 0x29:
	case 0x7d:
		return 1;
	}
	return 0;
}

void
plotflee(int a, short n)
{
	if (units[n].scene >= 0x2702 && units[n].scene < 0x2707) {
		units[n].scene = 0x2707;
		units[n].traits |= 0x20;
	}
}

int
keyperson(short m, short k)
{
	if ((m == 15 && k == 2)
	|| (m == 15 && k == 9)
	|| (m == 15 && k == 1 && getbit(game.flags, 5))
	|| (m == 15 && k == 8 && getbit(game.flags, 5))
	|| (m == 0x18 && k == 3 && !getbit(game.flags, 0x28f))
	|| (m == 0x18 && k == 2 && !getbit(game.flags, 0x28f))
	|| (m == 0x18 && k == 1)
	|| (m == 0x18 && k == 9)
	|| (m == 0x20 && k == 1)
	|| (m == 0x20 && k == 6)
	|| (m == 0x20 && k == 15)
	|| (m == 0x1c && k == 1)
	|| (m == 0x2b && k == 1)
	|| (m == 0x2b && k == 2)
	|| (m == 0x3d && k == 1)
	|| (m == 0x3d && k == 2)
	|| (m == 0x41 && k == 1))
		return 1;
	return 0;
}

void
avenge(short m, short k)
{
	if (avengers && avengers != -1)
		return;
	if (!keyperson(m, k))
		return;
	clearavenge();
	avengers = 9;
	victimfemale = isfemale(&units[k]);
	victimname = units[k].number & ~0x2000;
	victimzone = mapzone(m);
}

void
losthostages(void)
{
	if (!getbit(game.flags, 0x281)) {
		setflag(0x281, 1);
		dropnote(0x1c);
		dropnote(0x1d);
	}
	setflag(0x28b, 0);
	setflag(0x28c, 0);
}

short
nearestof(short x, short y, short v, short busy)
{
	short i, d, best, n;

	n = 0xff;
	best = 300;
	for (i = 1; i < nunits; i++) {
		if (units[i].health <= 0 || units[i].clan != v)
			continue;
		if (busy && isfemale(&units[i]))
			continue;
		d = celldist(x, y, units[i].x, units[i].y);
		if (d < best) {
			n = i;
			best = d;
		}
	}
	return n;
}

void
plotdeath(short m, short k, short a, short b)
{
	short i, j, n, n2, n3;

	if (b == 0xfe)
		return;
	avenge(m, k);
	if (m == 0x1c) {
		setflag(0x33, 1);
		return;
	}
	if (m == 15) {
		if (k == 1) {
			setflag(4, 1);
			setflag(0x19, 0);
			closenote(1, 1);
			closenote(0x24, 1);
			dropnote(2);
			dropnote(3);
			dropnote(4);
			dropnote(0x25);
			setflag(2, 0);
			setflag(9, 1);
			dropnote(0x28);
			dropnote(0x29);
			dropnote(0x1b);
			dropnote(0x12);
			dropnote(0x10);
			dropnote(0x2d);
			dropnote(0x2e);
			dropnote(0x30);
			dropnote(0x31);
			dropnote(0x32);
			if (getbit(game.flags, 0x26))
				addnote(0x33);
		} else if (k == 2) {
			setflag(11, 1);
			setflag(3, 1);
			dropnote(6);
			dropnote(0x30);
			dropnote(0x31);
			dropnote(0x32);
		} else if (k == 9) {
			dropnote(0x32);
		} else if (k == 15) {
			dropnote(0x31);
		} else if (k == 8) {
			dropnote(0x10);
		} else if (k == 0x40) {
			setflag(0x40, 1);
			dropnote(0x26);
			dropnote(0x27);
			dropnote(0x2c);
		}
	} else if (m == 0x18) {
		if (k == 3)
			dropnote(4);
		else if (k == 2)
			dropnote(3);
		if (k == 3 || k == 2) {
			if (a == 0xff)
				return;
			if (k == 3)
				setflag(0x7f, 1);
			else if (k == 2)
				setflag(0x7e, 1);
			setflag(0x7d, 1);
		} else if (k == 1) {
			if (a != 0xff) {
				setflag(0x80, 1);
				setflag(0x7d, 1);
			}
			setflag(0x74, 0);
			dropnote(2);
			dropnote(7);
			dropnote(9);
			dropnote(10);
		} else if (k == 0x4f) {
			setflag(13, 1);
		} else if (k == 0x4d) {
			closenote(0x14, 0);
			if (a == 4 || a == 1)
				setflag(0x90, 1);
		}
	} else if (m == 0x20) {
		if (k == 14)
			setflag(0x1e3, 1);
		else if (k == 5)
			setflag(0x1e4, 1);
		else if (k == 13) {
			setflag(0x1e7, 1);
			dropnote(12);
		} else if (k == 2)
			dropnote(0x11);
		else if (k == 3) {
			setflag(0x23a, 1);
			setflag(0x22c, 1);
			if (units[1].sympathy > 1)
				units[1].sympathy = 1;
		} else if (k == 11 && b == 0) {
			setflag(0x23b, 1);
			setflag(0x1f4, 0);
			setflag(0x2ee, 0);
			setflag(0x320, 0);
			nomercy(&units[11]);
			dropnote(0x49);
			dropnote(0x4a);
		} else if (k == 1) {
			setflag(0x23c, 1);
			setflag(0x75, 0);
			setflag(0x1f4, 0);
			setflag(0x2ee, 0);
			setflag(0x320, 0);
			dropnote(0x49);
			dropnote(0x4a);
		} else if (k == 6 || k == 15) {
			dropnote(0x49);
			dropnote(0x4a);
		} else if (k == 4) {
			if (hasnote(0x13)) {
				closenote(0x13, 0);
				addnote(0x3b);
			}
		}
	} else if (m == 0x3d) {
		if (a != 0xff && k == 1) {
			setflag(0x7c, 1);
			setflag(0x2ee, 0);
			setflag(0x320, 0);
		}
	} else if (m == 0x2d) {
		if (k == 1) {
			setflag(0x1c3, 1);
			forgetplayer(&units[2]);
			closenote(0x18, 1);
			((short *)szvars)[0xa] = gameday * 24 + gamehour;
		} else if (k == 2 && b == 0) {
			setflag(0x1c4, 1);
			if (units[4].sympathy > 1)
				units[4].sympathy = 1;
		} else if (k == 3 && b == 0) {
			setflag(0x1c5, 1);
			if (units[4].sympathy > 1)
				units[4].sympathy = 1;
		} else if (k == 4 && b == 0) {
			setflag(0x1c6, 1);
			if (units[2].sympathy > 1)
				units[2].sympathy = 1;
			if (units[3].sympathy > 1)
				units[3].sympathy = 1;
		} else if (k >= 5 && k <= 12) {
			for (deathi = 5; deathi <= 12; deathi++) {
				if (deathi == k)
					continue;
				if (units[deathi].health <= 0)
					setflag(0x1c7, 1);
			}
			setflag(0x1c8, 1);
			units[1].talkflags |= 8;
			for (deathi = 5; deathi <= 12; deathi++) {
				units[deathi].talkflags |= 8;
				if (units[deathi].health > 0)
					setflag(0x1c8, 0);
			}
			if (k == 5)
				setflag(0x1d2, 1);
			else if (k == 6)
				setflag(0x1d5, 1);
		}
		if (getbit(game.flags, 0x1c3) && getbit(game.flags, 0x1c8) && !getbit(game.flags, 0x1ca))
			setflag(0x1ca, 1);
	} else if (m == 0x41) {
		if (units[k].clan == 2) {
			n = 0;
			for (i = 1; i < nunits; i++)
				if (units[i].health > 0 && units[i].clan == 2)
					n++;
			if (n != 0)
				return;
			n2 = 0;
			for (i = 1; i < nunits; i++)
				if (units[i].health > 0 && units[i].clan == 3)
					n2++;
			if (n2 == 0) {
			none:
				if (getbit(game.flags, 0x281))
					return;
				losthostages();
				units[1].scene = 0x41d;
				return;
			}
			if (getbit(game.flags, 0x285)) {
				setflag(0x280, 1);
				setunitmode(&units[1], 0);
				units[1].scene = 0x41c;
				comeover(1, 0x41c, -1);
				return;
			}
			losthostages();
			j = nearestof(playerx, playery, 3, 1);
			if (j == 0xff)
				j = nearestof(playerx, playery, 3, 0);
			if (j == 0xff)
				return;
			setunitmode(&units[j], 0);
			units[j].scene = 0x41e;
			comeover(j, 0x41e, -1);
		} else if (units[k].clan == 3) {
			n3 = 0;
			for (i = 1; i < nunits; i++)
				if (units[i].health > 0 && units[i].clan == 3)
					n3++;
			if (n3 == 0)
				goto none;
		}
	} else if (m == 0x19) {
		if (k == 2) {
			setflag(0x9a, 1);
			if (units[1].sympathy > 1)
				units[1].sympathy = 1;
		}
	}
}

int
plotsymp(short m, short k, int v)
{
	if (m == 0x20 && k == 1 && getbit(game.flags, 0x22c) && v >= 1)
		return 1;
	return v;
}

int
spared(short m, short k)
{
	if (m == 0x30 && (units[k].number & 0x1fff) == 0x1f)
		return 1;
	if (m == 0x41 && k == 1)
		return 1;
	return 0;
}

short
hampered(short m, short k, short v)
{
	if (k == 0 && !oddsswitch)
		return 1;
	if (m != 15 || k != 1 || v != 0 || !getbit(game.flags, 0x3b7))
		return 0;
	if (inpack(0, 0, 0x12901cf, 0) || wornslot(&units[0], 0x12901cf, 0))
		return 1;
	return 0;
}

short
favored(short m, short k, short v)
{
	if (v == 0 && !oddsswitch)
		return 1;
	if (m != 15 || k != 0 || v != 1 || !getbit(game.flags, 0x3b7))
		return 0;
	if (inpack(0, 0, 0x12901cf, 0) || wornslot(&units[0], 0x12901cf, 0))
		return 1;
	return 0;
}

int
keepbless(short m, unsigned long item, short k, short l)
{
	if (m != 15 || !getbit(game.flags, 0x3b7) || (item & 0xfff) != 0x1cf || k != 9
	|| getbit(game.flags, 4))
		return 0;
	return 1;
}

void
plotpeace(Unit *u, Unit *v)
{
	if (curmap == 0x30 && (u->number & 0x1fff) == 0x1f)
		osmundcall();
}

void
retarget(short n, short m)
{
	if (!units[n].clan)
		return;
	if (units[n].clan == units[m].clan)
		return;
	if (units[n].mode == 20 || units[n].mode == 21) {
		units[n].modec = units[n].modeb;
		units[n].modeb = m;
	}
}

void
plotrage(short n)
{
	if (curmap == 0x30) {
		retarget(1, 6);
		retarget(2, 6);
		retarget(3, 8);
	}
}

void
plotdoor(short a, short x, short y)
{
}

int
wary(short m, short k)
{
	if (keyperson(m, k))
		return 1;
	return 0;
}

int
plotsteal(short m, unsigned long item)
{
	if (m == 0x1c) {
		units[0].cursed = 0xff;
		calcstats(&units[0]);
	}
	return 0;
}

void
plotfight(short m, short k)
{
	if (m == 0x1c) {
		units[0].cursed = 0xff;
		calcstats(&units[0]);
		dropnote(7);
		dropnote(9);
		dropnote(10);
	} else if (m == 0x19 && k == 2) {
		if (!getbit(game.flags, 0x98))
			setflag(0x98, 1);
	}
}

void
plotleave(short m)
{
	bruglowlate(1);
	if (m == 15) {
		if (units[1].health > 0 && (units[1].sympathy <= 0 || getbit(game.flags, 0x29)))
			setflag(0x19, 1);
		else
			setflag(0x19, 0);
	} else if (m == 0x18) {
		if (units[1].health > 0 && units[1].sympathy <= 0)
			setflag(0x74, 1);
		else
			setflag(0x74, 0);
	} else if (m == 0x20) {
		if (units[1].health > 0 && units[1].sympathy <= 0)
			setflag(0x75, 1);
		else
			setflag(0x75, 0);
	} else if (m == 0x1c) {
		setflag(0x4f, 0);
		for (leavei = 1; leavei < nunits; leavei++)
			if (units[leavei].health > 0 && !isanimal(&units[leavei])
			&& units[leavei].sympathy <= 0) {
				setflag(0x4f, 1);
				return;
			}
	} else if (m == 0x30) {
		if (!getbit(game.flags, 0x1ea))
			setflag(0x1e9, 1);
	} else if (m == 0x41) {
		if (getbit(game.flags, 0x283) && !getbit(game.flags, 0x280))
			losthostages();
	} else if (m == 0x2b) {
		if (units[1].health > 0 && units[1].sympathy > 0 && units[2].health > 0 && units[2].sympathy > 0)
			setflag(0x25f, 0);
		else
			setflag(0x25f, 1);
	}
}

int
selloff(Unit *u, short b, int c)
{
	unsigned long x;
	int l0, l4, l8;

	keptitem = 0;
	do {
		l8 = 0;
		l4 = 0;
		l0 = 0;
		x = finditem(u, b, &l0, &l4, &l8, 3);
		if (c >= nfound)
			break;
		if (!l0)
			x = takeitem(l4, l8);
		else
			x = takeout(l0, l8);
		if (!x)
			break;
		proceeds += valueof(x) * 103 / 100;
		nfound--;
	} while (c < nfound);
	keptitem = x;
	return nfound;
}

int
buyfor(Unit *u, unsigned long item)
{
	Unit *p;
	unsigned long price, n;
	short kind;

	if (proceeds <= 0)
		return 0;
	price = valueof(item);
	p = u;
	kind = classof(item & 0xfff);
	if (price > proceeds) {
		if (kind == 0x10) {
			n = (unsigned long)(proceeds / 10) / 25;
			if (n == 0)
				return 0;
			item = n << 16 | 0x503;
		} else if (kind == 15) {
			n = proceeds / 10;
			if (n == 0)
				return 0;
			item = n << 16 | 0x4e9;
		} else
			return 0;
		addcoins(&item, 0);
		price = valueof(item);
	}
	if (giveitem(indexof(p), item, 2))
		return 0;
	proceeds -= price;
	if (proceeds < 0)
		proceeds = 0;
	return 1;
}

void
turnover(Unit *u)
{
	unsigned long v;
	short y, x;

	x = u->x;
	y = u->y;
	if (scheduled(u)) {
		u->x = u->postx[10];
		u->y = u->posty[10];
	}
	for (v = 0; v < 3; v++)
		if (yields(u, 2, u->items[v], 1) && giveitem(indexof(u), u->items[v], 2) == 0)
			u->items[v] = 0;
	proceeds = 0;
	selloff(u, 0x3c, 7);
	selloff(u, 0x46, 5);
	selloff(u, 0x55, 6);
	selloff(u, 0x48, 6);
	selloff(u, 0x49, 0);
	selloff(u, 0x89, 15);
	selloff(u, 0x37, 0);
	selloff(u, 0x21, 10);
	selloff(u, 0x39, 10);
	if (!selloff(u, 0x64, 8) || valueof(0x514) * 2 < proceeds) {
		for (v = 0; v < randn(3) + 2; v++)
			buyfor(u, 0x180);
		do {
			v = randn(100);
			if (v < 10)
				v = 0x514;
			else if (v < 20)
				v = 0x4f4;
			else if (v < 30)
				v = 0x4f7;
			else if (v < 45)
				v = 0x4f3;
			else if (v < 60)
				v = 0x4f6;
			else if (v < 80)
				v = 0x4f5;
			else
				v = 0x4f2;
			// FIXED: buyfor keeps the proceeds when the unit cannot stow the item, so the loop runs forever. Stop at the first purchase that fails.
#if defined(FEATURE_FIXES)
			if (!buyfor(u, v))
				break;
#else
			buyfor(u, v);
#endif
		} while (valueof(0x514) * 2 < proceeds);
	}
	if (selloff(u, 0x10, 10))
		v = keptitem >> 16;
	else
		v = 0;
	if (v < 5)
		for (;;) {
			v = 5 - v;
			v <<= 16;
			v |= 0x503;
			addcoins(&v, randn(4) << 16 | 0x503);
			// FIXED: as above, stop at the first purchase that fails.
#if defined(FEATURE_FIXES)
			if (!buyfor(u, v))
				break;
#else
			buyfor(u, v);
#endif
			if (valueof(0x514) * 2 / 3 >= proceeds)
				break;
			v = 0;
		}
	v = 0x3fff04e9;
	addcoins(&v, 0);
	buyfor(u, v);
	u->x = x;
	u->y = y;
}

void
plotenter(short m)
{
	setflag(0x46, 0);
	if (m == 15) {
		forgetplayer(&units[1]);
		if (units[3].health > 0)
			turnover(&units[3]);
		if (getbit(game.flags, 5) && units[2].health > 0) {
			killunit(&units[2], 0, 0, 0xfe, 9);
			setflag(10, 1);
		}
		if ((getbit(game.flags, 0x4b) || getbit(game.flags, 0x4c) || getbit(game.flags, 0x4d)) && !getbit(game.flags, 0x3cc)) {
			killunit(&units[64], 0, 0, 0xfe, 0);
			setflag(0x3cc, 1);
			dropnote(0x26);
			dropnote(0x27);
			dropnote(0x2c);
		}
		if ((getbit(game.flags, 5) || getbit(game.flags, 11)) && !getbit(game.flags, 14)) {
			killunit(&units[9], 0, 0, 0xfe, 0);
			killunit(&units[27], 0, 0, 0xfe, 0);
			killunit(&units[28], 0, 0, 0xfe, 0);
			if (units[19].health > 0) {
				enterv = sackregion(0x69, 0x72, 0xf201a7);
				if (!enterv)
					enterv = sackregion(0x69, 0x72, 0xf301a8);
				if (enterv != 0)
					giveitem(0x13, enterv, 0);
				enterv = sackregion(0x69, 0x72, 0xef01d5);
				if (enterv != 0)
					giveitem(0x13, enterv, 0);
				enterv = sackregion(0x5a, 0x71, 0xe901bd);
				if (enterv != 0)
					giveitem(0x13, enterv, 0);
			}
			sackregion(0x5a, 0x71, -1);
			sackregion(0x69, 0x72, -1);
			sackregion(0x56, 0x66, -1);
			sackregion(0x5e, 0x66, -1);
			sackregion(0x73, 0x6a, -1);
			setflag(14, 1);
		}
	} else if (m == 0x18) {
		if (getbit(game.flags, 0x3cb) && units[4].health > 0)
			turnover(&units[4]);
		setflag(0x3cb, 1);
		forgetplayer(&units[1]);
		units[79].x = 0;
		units[79].health = 0;
		units[79].y = 0;
		if (getbit(game.flags, 5) && !getbit(game.flags, 11) && !getbit(game.flags, 0x29a)) {
			setflag(0x29a, 1);
			enterv = realmat(0x84, 0x31);
			if (enterv != 0)
				stow(enterv, 0x91189);
		}
		if ((getbit(game.flags, 0x1f9) || getbit(game.flags, 0x28f)) && !getbit(game.flags, 0x1f5)) {
			killunit(&units[9], 0, 0, 0xfe, 0);
			killunit(&units[58], 0, 0, 0xfe, 0);
			killunit(&units[59], 0, 0, 0xfe, 0);
			killunit(&units[1], 0, 0, 0xfe, 2);
			sackregion(0x42, 0x2f, -4);
			sackregion(0x42, 0x2f, -3);
			draperow(0x34, 0x2c, 3);
			draperow(0x36, 0x2c, 3);
			draperow(0x41, 0x2c, 3);
			draperow(0x43, 0x2c, 3);
			draperow(0x45, 0x2c, 2);
			drapecol(0x34, 0x2c, 3);
			drapecol(0x34, 0x2e, 3);
			drapecol(0x34, 0x30, 2);
			additem(0x1d89, 0x1fc);
			additem(0x1d8c, 0x1fc);
			dropitem(0x1aa4, 0xf411a9);
			dropitem(0x1a05, 0xf411a9);
			additem(0x1cfc, newitem(0x566));
			additem(0x1a93, newitem(0x566));
			movepost(0x4d, 0x4e, 0x3d, 0x3f, 0x2d);
			movepost(0x4d, 0x4e, 0x3c, 0x3f, 0x2d);
			movepost(0x4d, 0x4e, 0x42, 0x45, 0x2d);
			movepost(0x4e, 0x52, 0x3f, 0x3f, 0x31);
			movepost(0x4e, 0x51, 0x44, 0x46, 0x30);
			movepost(2, 0x4e, 0x42, 0x35, 0x2f);
			movepost(2, 0x50, 0x42, 0x46, 0x2f);
			movepost(2, 0x53, 0x3c, 0x46, 0x2f);
			movepost(2, 0x4f, 0x44, 0x46, 0x2f);
			movepost(2, 0x4e, 0x3c, 0x46, 0x2f);
			units[2].scene = 0x7e5;
			forgetplayer(&units[2]);
			movepost(15, 0x41, 0x2e, 0x46, 0x3c);
			movepost(15, 0x4f, 0x40, 0x46, 0x3c);
			movepost(15, 0x4e, 0x43, 0x42, 0x2d);
			movepost(15, 0x45, 0x2d, 0x42, 0x2d);
			setpost(15, 12, 0x42, 0x2d);
			setpost(15, 20, 0x42, 0x2d);
			setpost(15, 21, 0x42, 0x2d);
			setpost(15, 22, 0x42, 0x2d);
			copyhour(&units[15], 11, 15);
			copyhour(&units[15], 13, 15);
			copyhour(&units[15], 14, 15);
			copyhour(&units[15], 18, 15);
			copyhour(&units[15], 19, 15);
			postmode(15, 0x46, 0x3c, 0, 3, 0);
			setareas(&units[2]);
			setareas(&units[77]);
			setareas(&units[78]);
			setareas(&units[15]);
			copyhour(&units[2], 8, 10);
			copyhour(&units[2], 9, 10);
			fillunits(regionat(0x4e, 0x3c));
			enterv = realmat(0x53, 0x3b);
			if (enterv != 0) {
				emptyinto(enterv, realmat(0x52, 0x44), realmat(0x53, 0x44), realmat(0x53, 0x41));
				stow(enterv, 0xc000017d);
				stow(enterv, 0x1972);
				stow(enterv, 0x1972);
				if (wearof(realms[enterv].item) >= 7)
					setwear(&realms[enterv].item, 0);
				realms[enterv].item |= 0x18000000;
			}
			setflag(0x1f5, 1);
			dropnote(0x23);
			dropnote(15);
			dropnote(0x3d);
			dropnote(0x3e);
			addnote(0x3f);
			addnote(0x40);
		}
		if ((getbit(game.flags, 0x28f) || getbit(game.flags, 0x2ee)) && !getbit(game.flags, 0x291)) {
			killunit(&units[2], 0, 0, 0xfe, 0);
			killunit(&units[77], 0, 0, 0xfe, 0);
			killunit(&units[78], 0, 0, 0xfe, 0);
			killunit(&units[75], 0, 0, 0xfe, 0);
			killunit(&units[76], 0, 0, 0xfe, 0);
			killunit(&units[14], 0, 0, 0xfe, 0);
			killunit(&units[15], 0, 0, 0xfe, 0);
			for (enterv = 22; enterv <= 31; enterv++)
				killunit(&units[enterv], 0, 0, 0xfe, 0);
			killunit(&units[69], 0, 0, 0xfe, 0);
			killunit(&units[70], 0, 0, 0xfe, 0);
			killunit(&units[71], 0, 0, 0xfe, 0);
			killunit(&units[72], 0, 0, 0xfe, 0);
			killunit(&units[74], 0, 0, 0xfe, 0);
			killunit(&units[3], 0, 0, 0xfe, 0);
			killunit(&units[62], 0, 0, 0xfe, 0);
			killunit(&units[63], 0, 0, 0xfe, 0);
			killunit(&units[64], 0, 0, 0xfe, 0);
			killunit(&units[65], 0, 0, 0xfe, 0);
			killunit(&units[66], 0, 0, 0xfe, 0);
			killunit(&units[67], 0, 0, 0xfe, 0);
			killunit(&units[68], 0, 0, 0xfe, 0);
			killunit(&units[32], 0, 0, 0xfe, 0);
			killunit(&units[33], 0, 0, 0xfe, 0);
			killunit(&units[34], 0, 0, 0xfe, 0);
			killunit(&units[36], 0, 0, 0xfe, 0);
			killunit(&units[37], 0, 0, 0xfe, 0);
			killunit(&units[38], 0, 0, 0xfe, 0);
			killunit(&units[39], 0, 0, 0xfe, 0);
			sackregion(0x51, 0x40, -2);
			fillunits(regionat(0x51, 0x40));
			sackregion(0x43, 0x42, -2);
			fillunits(regionat(0x43, 0x42));
			sackregion(0x3e, 0x2f, -2);
			fillunits(regionat(0x3e, 0x2f));
			sackregion(0x4e, 0x33, -2);
			fillunits(regionat(0x4e, 0x33));
			sackregion(0x41, 0x3a, -2);
			fillunits(regionat(0x41, 0x3a));
			sackregion(0x4f, 0x2b, -2);
			fillunits(regionat(0x4f, 0x2b));
			sackregion(0x44, 0x25, -2);
			fillunits(regionat(0x44, 0x25));
			sackregion(0x3b, 0x24, -2);
			fillunits(regionat(0x3b, 0x24));
			sackregion(0x87, 0x2b, -1);
			sackregion(0x69, 0x2a, -1);
			sackregion(0x73, 0x2b, -1);
			sackregion(0x78, 0x25, -1);
			sackregion(0x6e, 0x37, -1);
			sackregion(0x78, 0x35, -1);
			sackregion(0x85, 0x33, -1);
			addblood(0x4e, 0x3f, 1, 1);
			addblood(0x50, 0x3b, 1, 1);
			addblood(0x4f, 0x41, 1, 1);
			setflag(0x291, 1);
		}
	} else if (m == 0x20) {
		forgetplayer(&units[1]);
		if (units[8].health > 0)
			turnover(&units[8]);
		if (getbit(game.flags, 0x1f0) && !getbit(game.flags, 0x1e5)) {
			killunit(&units[13], 0, 0, 0xfe, 0);
			killunit(&units[14], 0, 0, 0xfe, 0);
			killunit(&units[5], 0, 0, 0xfe, 0);
			setflag(0x1e5, 1);
		}
		setflag(0x517, 1);
	} else if (m == 0x2d) {
		if (getbit(game.flags, 0x1cc) && !getbit(game.flags, 0x1cf))
			setflag(0x1cf, 1);
		forgetplayer(&units[2]);
		setflag(0x516, 1);
	} else if (m == 0x2b) {
		forgetplayer(&units[2]);
	} else if (m == 0x24) {
		forgetplayer(&units[1]);
	} else if (m == 0x3d) {
		forgetplayer(&units[1]);
		if (getbit(game.flags, 0x2ee) && !getbit(game.flags, 0x2ef)) {
			setflag(0x2ef, 1);
			sackregion(0x43, 0x37, -4);
			sackregion(0x43, 0x37, -3);
			draperow(0x3c, 0x34, 3);
			draperow(0x3e, 0x34, 3);
			draperow(0x40, 0x34, 3);
			draperow(0x48, 0x34, 3);
			draperow(0x4a, 0x34, 2);
			drapecol(0x3b, 0x34, 3);
			drapecol(0x3b, 0x39, 2);
			additem(0x210e, 0x4a1);
			additem(0x210f, 0x4a2);
			additem(0x2110, 0x4a1);
			additem(0x2111, 0x4a2);
			additem(0x1f4a, 0x9fc);
			additem(0x1f4a, newitem(0x566));
			for (enterv = 0x3e; enterv <= 0x41; enterv++) {
				additem(enterv + 0x203a, 0x4a4);
				additem(enterv + 0x2166, 0x4a3);
			}
			enterv = findclass(0x207d, 0x59);
			if (enterv != 0)
				takeitem(0x207d, enterv);
			additem(0x207d, 0x16a);
			lightregion(regionat(0x43, 0x37), 1);
			if (units[2].health > 0) {
				giveitem(2, 0x4f4, 0);
				giveitem(2, 0x4f7, 0);
			}
		}
	} else if (m == 0x28) {
		setflag(0x515, 1);
	} else if (m == 3) {
		setflag(0x5bf, 1);
	} else if (m == 0x41) {
		if (!getbit(game.flags, 0x283) && !getbit(game.flags, 0x281)) {
			for (enterv = 1; enterv < nunits; enterv++)
				if (units[enterv].clan == 2) {
					units[enterv].sympathy = 0;
					units[enterv].scene = 0;
				} else if (units[enterv].clan == 3) {
					units[enterv].sympathy = 2;
					units[enterv].scene = 0x41e;
				}
		} else {
			units[1].scene = 0x41d;
			comeover(1, 0x41d, -1);
			for (enterv = 1; enterv < nunits; enterv++)
				if (units[enterv].clan == 2 || units[enterv].clan == 3)
					killunit(&units[enterv], 0, 0, 0xfe, 0);
		}
		if (getbit(game.flags, 0x277) && !getbit(game.flags, 0x282)) {
			for (enterv = 1; enterv < nunits; enterv++)
				if (units[enterv].clan == 1) {
					units[enterv].clan = 0xff;
					if (enterv != 1)
						units[enterv].scene = 0x420;
				}
		} else {
			for (enterv = 1; enterv < nunits; enterv++)
				if (units[enterv].clan == 1)
					killunit(&units[enterv], 0, 0, 0xfe, 0);
		}
		if (!getbit(game.flags, 0x283) && !getbit(game.flags, 0x281) && getbit(game.flags, 0x277) && !getbit(game.flags, 0x282))
			setflag(0x285, 1);
		if (!getbit(game.flags, 0x283) && !getbit(game.flags, 0x281)) {
			setflag(0x283, 1);
			for (enterv = 1; enterv < nunits; enterv++)
				if (units[enterv].clan == 2)
					units[enterv].sympathy = 0;
				else if (units[enterv].clan == 0xff) {
					setunitmode(&units[enterv], 20);
					retarget((short)enterv, nearestof(units[enterv].x, units[enterv].y, 2, 1));
				}
		}
		if (getbit(game.flags, 0x277) && !getbit(game.flags, 0x282))
			setflag(0x282, 1);
		for (enterv = 1; enterv < nunits; enterv++)
			if (units[enterv].health > 0) {
				units[enterv].mercy = 0;
				units[enterv].plea = 0;
				units[enterv].courage = 3;
			}
	}
}

void
plotarrive(short m)
{
	if (m == 0x41) {
		units[0].x = playerx;
		units[0].y = playery;
		units[0].dirx = 1;
		units[0].nextx = units[0].x + 1;
		units[0].diry = 0;
		units[0].nexty = units[0].y;
	}
}

void
plotsight(short m, short k)
{
	if (m == 0x1c) {
		if (units[k].sympathy > 0 && !getbit(game.flags, 0x46) && k != 3 && k != 4 && !getbit(game.flags, 0x4e))
			provoke(&units[0], &units[k], 4, 7, 0);
	} else if (m == 0x19 && k == 2 && !getbit(game.flags, 0x96) && !getbit(game.flags, 0x97)) {
		if (seesunit(&units[1]))
			setflag(0x96, 1);
		else
			provoke(&units[0], &units[k], 4, 7, 0);
	}
}

void
plotdepart(short a, short b)
{
}

void
plotcombat(short a)
{
	if (a == 0 && curmap == 0x30 && !getbit(game.flags, 0x1ea) && !getbit(game.flags, 0x1e8))
		setflag(0x1e8, 1);
}

int
chaste(short m, short k)
{
	if (m == 0x18 && k == 0x42 && units[k].sympathy < 4 && !myservant(&units[k]))
		return 1;
	if (m == 0x2d && k == 4 && (!getbit(game.flags, 0x1c3) || !getbit(game.flags, 0x1cd)))
		return 1;
	return 0;
}

int
freegoods(short m, short k)
{
	if (m == 0x2d && (k == 2 || k == 3 || k == 4) && getbit(game.flags, 0x1cd))
		return 1;
	return 0;
}

int
sanctioned(short m, int k, int x, int y, unsigned long item)
{
	if (m == 0x18 && curmap == 0x18 && k == 1 && x == 0x53 && y == 0x3b
	&& classof(item & 0xfff) == 10 && getbit(game.flags, 0x1fc))
		return 1;
	return 0;
}

int
forgives(short m, short k, short a, signed char b, short c, short d)
{
	if (m == 0x2d) {
		if (k == 4 && !((getbit(game.flags, 0x1da) || getbit(game.flags, 0x1c9)) && playery >= 0x4b)
		&& (a != 0 || c == 3 && d != 2 && d != 3 || c == 5))
			return 1;
	} else if (m == 0x19) {
		if (k == 1 && c == 3 && !getbit(game.flags, 0x97) && !getbit(game.flags, 0x96))
			return 1;
	}
	return 0;
}

int
ceasefire(short m, short k)
{
	if (m != 0x19 || k != 2 || getbit(game.flags, 0x9d))
		return 0;
	if (!getbit(game.flags, 0x9c)) {
		if (!seesunit(&units[1]))
			return 0;
		setflag(0x96, 1);
	}
	setflag(0x9d, 1);
	return 1;
}

int
noheir(short m, short k, short x)
{
	if (m == 0x2d && k == 4 && !getbit(game.flags, 0x1cc))
		return 1;
	return 0;
}

short
plotmagic(short m, short k)
{
	if (m == 0x2d && k == 1)
		return 1;
	return 0;
}

short
canfrighten(short m, short k)
{
	if (m == 0x2d)
		return 0;
	return 1;
}

int
notravel(void)
{
	short i;

	if (getbit(game.flags, 0x28f) && !getbit(game.flags, 0x290))
		return 1;
	if (getbit(game.flags, 0x290) && !getbit(game.flags, 0x2ee))
		return 1;
	if (getbit(game.flags, 0x30) && !getbit(game.flags, 0x3c))
		return 1;
	if (curmap != 0x41 || !getbit(game.flags, 0x283) || getbit(game.flags, 0x280) || getbit(game.flags, 0x281))
		return 0;
	for (i = 1; i < nunits; i++)
		if (units[i].health > 0 && (units[i].clan == 0xff || units[i].clan == 1))
			return 1;
	return 0;
}

int
plotday(void)
{
	if (getbit(game.flags, 0x32) && gameday >= ((short *)szvars)[0]) {
		setflag(0x32, 0);
		dropnote(5);
		dropnote(0x35);
		deedbreakword();
		if (!getbit(game.flags, 0x33)) {
			setflag(0x33, 1);
			addnote(0x44);
		}
	}
	if (getbit(game.flags, 0x33) && gameday > game.curseday + 14)
		setflag(0x33, 0);
	if (getbit(game.flags, 0x25a) && !getbit(game.flags, 0x280) && !getbit(game.flags, 0x281) && gameday >= ((short *)szvars)[3] + 14) {
		setflag(0x284, 1);
		losthostages();
	}
	return 0;
}

void
bruglowfeast(void)
{
	if (curmap != 0x2d || !getbit(game.flags, 0x1dc) || getbit(game.flags, 0x1cc) || gamehour < 20 || units[4].health <= 0)
		return;
	// FIXED: the skip past units 2 to 4 comes after the test of life, so with unit 2 alive unit 5's life goes untested. Skip first.
#if defined(FEATURE_FIXES)
	for (feasti = 1; feasti <= 12; feasti++) {
		while (feasti > 1 && feasti < 5)
			feasti++;
		if (units[feasti].health <= 0)
			continue;
#else
	for (feasti = 1; feasti <= 12; feasti++) {
		if (units[feasti].health <= 0)
			continue;
		while (feasti > 1 && feasti < 5)
			feasti++;
#endif
		if (units[feasti].sympathy <= 0)
			return;
	}
	feastreg = regionat(units[4].x, units[4].y);
	if (!feastreg || regions[feastreg].id != 0x1195)
		return;
	// FIXED: as above, skip first.
#if defined(FEATURE_FIXES)
	for (feasti = 1; feasti <= 12; feasti++) {
		while (feasti > 1 && feasti < 5)
			feasti++;
		if (units[feasti].health <= 0)
			continue;
#else
	for (feasti = 1; feasti <= 12; feasti++) {
		if (units[feasti].health <= 0)
			continue;
		while (feasti > 1 && feasti < 5)
			feasti++;
#endif
		if (regionat(units[feasti].x, units[feasti].y) != feastreg)
			continue;
		if (units[feasti].drunk >= 0x78)
			continue;
		if (units[feasti].drunk >= 0x5a)
			units[feasti].drunk = 0x78;
		else {
			units[feasti].drunk = 0x78;
			calcstats(&units[feasti]);
		}
	}
}

void
plotfind(int k, int x, int y, unsigned long item)
{
	item &= 0xffff0fff;
	if (curmap == 0x2a) {
		if (item == 0x804f7 && !getbit(game.flags, 0x1de)) {
			setflag(0x1de, 1);
			closenote(0x1f, 1);
			closenote(0x3c, 1);
		}
	} else if (curmap == 0x3b) {
		if (k == 1 && x == 0x63 && y == 0x67 && !getbit(game.flags, 0x3b7)
		&& (item & 0xfff) == 0x1cf && blessof(item) == 9) {
			setflag(0x3b7, 1);
			closenote(0x28, 1);
			closenote(0x29, 1);
			music(12, 0);
		}
	} else if (curmap == 0x18) {
		if (item == 0xc000017d && k >= 0x2b && k <= 0x30 && !getbit(game.flags, 0x28e)
		&& panelrealm(k) == realmat(0x53, 0x3b)) {
			setflag(0x28e, 1);
			closenote(0x1e, 1);
			if (getbit(game.flags, 0x25a))
				addnote(0x45);
		}
	}
}

void
deadret2(void)
{
}

short
challenged(void)
{
	if (getbit(game.flags, 0x29))
		return 1;
	return 0;
}

short
antirome(void)
{
	if (getbit(game.flags, 5))
		return 1;
	return 0;
}

short
antiromelost(void)
{
	if (!antirome())
		return 0;
	if (getbit(game.flags, 0x7d) || getbit(game.flags, 0x23c))
		return 1;
	return 0;
}

int
fate(void)
{
	if (forcedfate != 0x63)
		game.fate = forcedfate;
	else if (getbit(game.flags, 0x23b) || getbit(game.flags, 0x23c) || getbit(game.flags, 0x75)
	|| (getbit(game.flags, 0x7d) || getbit(game.flags, 0x7f) || getbit(game.flags, 0x7e)
	|| getbit(game.flags, 0x80) && !getbit(game.flags, 0x1f9) || getbit(game.flags, 0x74) && !getbit(game.flags, 0x1f9))
	&& !getbit(game.flags, 0x29) || getbit(game.flags, 0x7c) || getbit(game.flags, 0x40))
		game.fate = 13;
	else if (getbit(game.flags, 0x320))
		game.fate = 0x1f;
	else if (getbit(game.flags, 0x2ee))
		game.fate = 0x1e;
	else if (getbit(game.flags, 0x1f4) && getbit(game.flags, 0x1f5))
		game.fate = 0x1a;
	else if (getbit(game.flags, 0x1f4))
		game.fate = 0x19;
	else if (getbit(game.flags, 0x1ec))
		game.fate = 0x12;
	else if (getbit(game.flags, 0x3c))
		game.fate = 12;
	else if (getbit(game.flags, 0x41) && !getbit(game.flags, 0x42) && !getbit(game.flags, 0x30))
		game.fate = 0x13;
	else if (wealth(3) > 0xdac)
		game.fate = 0x15;
	else if (getbit(game.flags, 0x29) || getbit(game.flags, 0x2f) && !getbit(game.flags, 0x30))
		game.fate = 11;
	else if (antirome() || antiromelost())
		game.fate = 0x14;
	else
		game.fate = 10;
	return game.fate;
}

short
tribefate(void)
{
	savedfate = game.fate;
	fate();
	switch (game.fate) {
	case 0xf:
	case 0x19:
	case 0x1e:
	case 0x1f:
		game.fate = savedfate;
		return 2;
	case 0x14:
	// FIXED: ending 0x1a, an alliance formed and then lost to the southern tribe's attacks, falls to the default, so the sheet can say "He died without having really lived." Count it with the failed alliance of 0x14.
#if defined(FEATURE_FIXES)
	case 0x1a:
#endif
		game.fate = savedfate;
		return 1;
	case 0x10:
	default:
		game.fate = savedfate;
		return 0;
	}
}

void
listvisions(void)
{
	short i;

	memset(visions, 0, sizeof visions);
	nvisions = 0;
	if (!getbit(game.flags, 5) && !getbit(game.flags, 4))
		visions[nvisions++] = 0x60a;
	if (game.defeated >= 5)
		visions[nvisions++] = 0x60b;
	if (game.stolen >= 3 || game.sleepers >= 2 || game.poisoned >= 2 || game.ambushed >= 2)
		visions[nvisions++] = 0x60c;
	if (antirome())
		visions[nvisions++] = 0x60d;
	if (getbit(game.flags, 5) && !getbit(game.flags, 0x1f5))
		visions[nvisions++] = 0x60e;
	if (getbit(game.flags, 0x1f4))
		visions[nvisions++] = 0x60f;
	fate();
	visions[nvisions++] = fatevision();
	if (antirome())
		visions[nvisions++] = 0x610;
	if (challenged())
		visions[nvisions++] = 0x611;
	if (antirome() || challenged()) {
		visions[nvisions++] = 0x612;
		visions[nvisions++] = 0x613;
		visions[nvisions++] = 0x615;
	}
	for (i = 0; i < nvisions; i++)
		visions[i] -= 0x5cf;
}

short
nextvision(char a, char b)
{
	short i, j;

	for (i = 0; i < nvisions; i++) {
		for (j = 0; j < 30; j++)
			if (game.toldvision[j] == visions[i] && game.tolda[j] == a && game.toldb[j] == b)
				goto next;
		return i;
	next:;
	}
	return -1;
}

int
foretell(char *buf, char a, char b)
{
	short k, j;
	int r;

	k = nextvision(a, b);
	if (k < 0) {
		if (a)
			for (j = 0; j < 30; j++)
				if (game.toldvision[j] && game.tolda[j] == a && game.toldb[j] == b)
					return wsprintf(buf, strs[0x617]);
		return wsprintf(buf, strs[0x616]);
	}
	wsprintf(buf, strs[visions[k] + 0x5cf]);
	r = k;
	if (a) {
		for (j = 0; j < 30; j++)
			if (game.toldvision[j] == 0) {
				game.toldb[j] = b;
				game.toldvision[j] = visions[k];
				game.tolda[j] = a;
				return j;
			}
		r = j;
	}
	return r;
}

void
tobattle(void)
{
}

int
sendback(int m, int n)
{
	if (curmap != m || units[n].health <= 0)
		return 0;
	setunitmode(&units[n], 15);
	units[n].modec = units[n].postx[gamehour];
	units[n].moded = units[n].posty[gamehour];
	return 1;
}

void
storymanip(int e)
{
	if (e == 1) {
		if (curmap != 0x3d || units[1].health <= 0)
			return;
		setunitmode(&units[1], 0x15);
		if (units[1].belt == 0) {
			manipv = sackregion(units[1].x, units[1].y, 0xef01d5);
			if (manipv) {
				units[1].belt = units[1].hand;
				units[1].hand = manipv;
				calcstats(&units[1]);
			}
		}
		setunitmode(&units[1], 0);
		drawscreen();
	} else if (e == 2) {
		if (curmap != 0x3d || units[1].health <= 0)
			return;
		units[1].body = 0xef01b0;
		giveitem(1, 0x4f4, 0);
		giveitem(1, 0x4f7, 0);
		units[1].offhand = 0;
		if (units[1].belt) {
			units[1].hand = units[1].belt;
			units[1].belt = 0;
		}
		calcstats(&units[1]);
	} else if (e == 3) {
		if (curmap == 15)
			killunit(&units[64], 0, 0, 0xfe, 0);
	} else if (e == 4) {
		if (curmap == 0x41) {
			fadeslow(0);
			for (manipv = 1; manipv < nunits; manipv++)
				killunit(&units[manipv], 0, 0, 0xfe, 0);
			waitinput(1000);
		}
	} else if (e == 5)
		sendback(15, 0x11);
	else if (e == 6)
		sendback(15, 0x13);
}

void
pickcall(void)
{
	if (!(callsseen & 2))
		setflag(15, 1);
	else if (callsseen & 1) {
		if (randn(100) < 50)
			setflag(15, 1);
	}
}

int
robbery(short m, short k)
{
	if (k == 0 || k == 0xff || badunit(&units[k]) || units[k].health <= 0)
		return 0;
	if ((m == 1 || m == 5) && game.robberies < 5 && isrobber(&units[k]))
		return 1;
	return 0;
}

void
addlabel(int a, int b, int c)
{
	short x, y;

	x = a;
	y = b;
	// FIXED: a new label takes slot nmarks, which holds a label after a deletion below it. Take the first free slot, as the automap does, and name nmarks and marks.
	// FIXED: the Labels view over nmarks reaches marks in WLINK's layout alone. Name nmarks and marks.
#if defined(FEATURE_FIXES)
	{
		int k;

		for (k = 0; k < 23 && marks[k][0]; k++)
			;
		if (k == 23)
			return;
		wsprintf(marks[k], texts + 0xb32c, c);
		setmark(k, x, y);
		nmarks++;
	}
#elif defined(PORT)
	wsprintf(marks[nmarks], texts + 0xb32c, c);
	setmark(nmarks, x, y);
	nmarks++;
#else
	wsprintf((*(Labels *)&nmarks).text[(*(Labels *)&nmarks).n], texts + 0xb32c, c);
	setmark((*(Labels *)&nmarks).n, x, y);
	(*(Labels *)&nmarks).n++;
#endif
}

void
labelseen(int x, int y, int c, int flag)
{
	int i, j, px, py, r, k;

	x += 4;
	y += 4;
	// FIXED: a label above a hole that the automap's delete leaves lies past nmarks, so the place gets a second label beside it. Walk every slot, as drawmarks does, and name nmarks.
	// FIXED: nmarks by its name, as in addlabel
#if defined(FEATURE_FIXES)
	if (nmarks >= 23 || getbit(game.flags, flag))
		return;
	for (i = 0; i < 23; i++) {
		if (!marks[i][0])
			continue;
#elif defined(PORT)
	if (nmarks >= 23 || getbit(game.flags, flag))
		return;
	for (i = 0; i < nmarks; i++) {
#else
	if ((*(Labels *)&nmarks).n >= 23 || getbit(game.flags, flag))
		return;
	for (i = 0; i < (*(Labels *)&nmarks).n; i++) {
#endif
		px = markposx(i);
		py = markposy(i);
		if (x >= px - 12 && x <= px + 24 && py - 10 <= y && py + 20 >= y)
			return;
	}
	for (j = y; j < y + 10; j++) {
		r = (j - 90) / 2;
		if (r >= 0 && r < 150)
			for (i = x; i < x + 12; i++) {
				k = (i - 100) / 2;
				if (k >= 0 && k < 150 && !(mapflags[r * 150 + k] & 0x200))
					return;
			}
	}
	addlabel(x, y, c);
	setflag(flag, 1);
}

void
plotlabels(short m)
{
	if (m == 0x18) {
		labelseen(0xd6, 0xc1, 0x1b, 0x5c8);
		labelseen(0xf5, 0xd3, 0x1c, 0x5c9);
		labelseen(0x157, 0xb1, 0x1d, 0x5ca);
		labelseen(0x8c, 0xb3, 0x1e, 0x5cb);
		labelseen(0xa1, 0xe8, 0x1f, 0x5cc);
		labelseen(0xdc, 0x102, 0x20, 0x5cd);
		labelseen(0x127, 0x10e, 0x21, 0x5ce);
	} else if (m == 0x20) {
		labelseen(0x112, 0x9d, 0x22, 0x5cf);
		labelseen(0x88, 0x137, 0x23, 0x5d0);
	} else if (m == 0x23)
		labelseen(0x93, 0xae, 0x24, 0x5d1);
	else if (m == 0x2d) {
		labelseen(0x127, 0x11d, 0x25, 0x5d2);
		labelseen(0xc5, 0x9f, 0x26, 0x5d3);
	}
}

LRESULT CALLBACK
wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_CLOSE:
		if (appactive) {
			esckey = 0xff;
			quitting = 2;
			return 0;
		}
		break;
	case WM_SETCURSOR:
		GetCursorPos(&cursorpos);
		if (cursorpos.x >= clipx0) {
			if (cursorpos.x > clipx1)
				cursorpos.x = clipx1;
		} else
			cursorpos.x = clipx0;
		if (cursorpos.y < clipy0)
			cursorpos.y = clipy0;
		else if (cursorpos.y > clipy1)
			cursorpos.y = clipy1;
		if (mousex != cursorpos.x || mousey != cursorpos.y)
			mousemoved = 1;
		mousex = cursorpos.x;
		mousey = cursorpos.y;
		if (appactive && modeset)
			return 1;
		break;
	case WM_LBUTTONDOWN:
		mouseleft = 1;
		leftclick = 1;
		mouseright = 0;
		rightclick = 0;
		presslatch = 0;
		GetCursorPos(&cursorpos);
		if (mousex != cursorpos.x || mousey != cursorpos.y)
			mousemoved = 1;
		mousex = cursorpos.x;
		mousey = cursorpos.y;
		if (clicktime == 0
		|| (clickdx = mousex - clickx) < -3 || clickdx > 3
		|| (clickdy = mousey - clicky) < -3 || clickdy > 3
		|| (clickdx = since(clicktime)) < -1000 || clickdx > 1000) {
			clickx = mousex;
			clicky = mousey;
			clicktime = ticks();
		} else {
			dblclick = 1;
			clicktime = 0;
			if (notedunit == 0xffff && notedkind == 0xff && notedtime + 2 >= gametime)
				notedkind = lastkind;
		}
		break;
	case WM_RBUTTONDOWN:
		mouseright = 1;
		rightclick = 1;
		mouseleft = 0;
		leftclick = 0;
		break;
	case WM_LBUTTONUP:
		mouseleft = 0;
		break;
	case WM_RBUTTONUP:
		mouseright = 0;
		break;
	case WM_KEYDOWN:
		if (keydown(wp, lp))
			return 1;
		break;
	case WM_KEYUP:
		if (keyup(wp, lp))
			return 1;
		break;
	case 0x414:
		if (songdone(wp, lp))
			return 1;
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case 0x20:
			shotkey = 2;
			return 1;
		case 0x1f:
			musickey = 1;
			return 1;
		case 0x1e:
			sfxkey = 1;
			return 1;
		case 0x21:
			if (esckey != 0xfe) {
				esckey = 0xff;
				keycode = 1;
				lastscan = 1;
			}
			lastchar = 0;
			return 1;
		}
		break;
	case WM_ACTIVATEAPP:
		wasactive = appactive;
		appactive = wp && GetForegroundWindow() == hwnd && !IsIconic(hwnd);
		if (appactive && !wasactive) {
			if (modeset && ddready) {
				restoresurfs();
				ShowCursor(0);
			}
		} else if (!appactive && wasactive && modeset && ddready)
			ShowCursor(1);
		break;
	case WM_SYSCOMMAND:
		if (appactive)
			switch (wp) {
			case SC_SIZE:
			case SC_MOVE:
			case SC_MAXIMIZE:
			case SC_KEYMENU:
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				return 1;
			}
		break;
	case WM_DESTROY:
		esckey = 0xff;
		quitting = 2;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

void
showsign(short a, short b, short c)
{
	int x, y;

	if (c) {
		signcur = shown;
		if (signcur) {
			hidecursor();
			x = mousex - 12;
			if (x < 4)
				x = 4;
			else if (x > 0x124)
				x = 0x124;
			y = mousey - 12;
			if (y > 0xaf)
				y = 0xaf;
		}
		x = 0x128;
		y = 0x73;
		signx = x;
		signy = y;
		get48((unsigned char *)signback, 1, x, y);
	}
	putc1(signx, signy, a);
	if (b)
		putc1(signx, signy, b);
}

void
hidesign(void)
{
	put481(signx, signy, (unsigned char *)signback);
	if (signcur)
		drawcursor();
	resetinput();
}

int __cdecl
pollinput(void)
{
	if (PeekMessage(&pollmsg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!GetMessage(&pollmsg, NULL, 0, 0)) {
			esckey = 0xff;
			quitting = 1;
			exitcode = pollmsg.wParam;
			return 0;
		}
		if (TranslateAccelerator(mainwnd, accel, &pollmsg))
			return 1;
		TranslateMessage(&pollmsg);
		DispatchMessage(&pollmsg);
	} else if (!appactive) {
		WaitMessage();
		return 1;
	}
	if (replaydue == 1) {
		if (lastsong == 11 || lastsong == 3)
			goto reset;
		if (!replaywait)
			replaywait = ticks();
		else if (replaywait + 5000 < ticks())
			goto reset;
	} else if (replaydue == 2) {
	reset:
		replay();
		replaydue = 0;
		replaywait = 0;
	}
	GetCursorPos(&cursorpos);
	if (!(GetAsyncKeyState(lbutton) & 0x8000))
		mouseleft = 0;
	if (!(GetAsyncKeyState(rbutton) & 0x8000))
		mouseright = 0;
	if (keycode) {
		keyvk = MapVirtualKey(keycode, 1);
		if (keyvk && !(GetAsyncKeyState(keyvk) & 0x8000))
			keycode = 0;
	}
	if (mousex != cursorpos.x || mousey != cursorpos.y)
		mousemoved = 1;
	mousex = cursorpos.x;
	mousey = cursorpos.y;
	return 1;
}

int __cdecl
readinput(void)
{
	int k;

	for (;;) {
		inputok = pollinput();
		if (!inputok)
			return 0;
		if (!appactive)
			continue;
		if (lostcontrol) {
			hidecursor();
			mousemoved = 0;
			mouseleft = 0;
			mouseright = 0;
			leftclick = 0;
			rightclick = 0;
			dblclick = 0;
			turnkeys = 0;
			ptrunit = 0;
			ptrstates = 0;
			presslatch = 1;
			ptrpace = 1;
			keycode = 0;
			esckey = 0;
			dirkeys = 0;
			lastscan = 0;
			return inputok;
		}
		if (mousemoved) {
		mouse:
			if (talkactive) {
				if (!trademode)
					setcursor1(3, 3);
				menumode = 1;
			}
			mousemoved = 0;
			hoverx = mousex;
			hoverkind = 0;
			movecursor = 0;
			hovergear = 0;
			ptrunit = 0;
			ptrstates = 0;
			ptrimg = 0;
			ptrmark1 = 0;
			ptrmark2 = 0;
			ptrfar = 2;
			hovery = mousey;
			ptrblocked = 2;
			hoverpx = hoverx;
			hovercx = hoverx;
			hoverpy = hovery;
			hovercy = hovery;
			if (hoverx >= 600 && hovery >= 0x180 && playmode == 3 && !menumode && !trademode
			&& (waitforup || !rightclick && !mouseright || leftclick || mouseleft)) {
				setcursor1(3, 0x2e);
				hoverkind = 0xe6;
			} else {
				if (!menumode || !pagearrows)
					for (hoveri = 0; hoveri < 16 && panelstack[hoveri] != 0xff; hoveri++) {
						hoverpanel = &panels[panelstack[hoveri]];
						if (hoverx >= hoverpanel->x && hoverx < hoverpanel->x + hoverpanel->w
						&& hovery >= hoverpanel->y && hovery < hoverpanel->y + hoverpanel->h) {
							hoverpx = hoverx - hoverpanel->x;
							hoverkind = panelstack[hoveri] + 0x28;
							hoverpy = hovery - hoverpanel->y;
							switch (panelstack[hoveri]) {
							case 1:
							case 2:
								gearat(hoverpanel->id, &hoveritem, &hovercx, hoverpx, hoverpy);
								hovercy = 0;
								break;
							case 3:
							case 4:
							case 5:
							case 6:
							case 7:
							case 8:
								hovercy = 0;
								hovercx = (unsigned short)slotat(hoverpanel->id, hoverpx, hoverpy);
								break;
							case 9:
								hovercx = 999;
								hovercy = 0;
								break;
							case 13:
								hovercx = 999;
								hovercy = 0;
								break;
							case 14:
								if (hoverpx >= 0x98)
									hovercx = 4;
								else if (hoverpy <= 0x1a)
									hovercx = 999;
								else if (hoverpx <= 0x13)
									hovercx = 2;
								else if (hoverpx >= 0x82)
									hovercx = 1;
								else if (hoverpx >= 0x15 && hoverpx <= 0x80) {
									hovercx = 3;
									hovercy = hoverpx - 0x17;
									if (hovercy < 0)
										hovercy = 0;
									else if (hovercy > 0x68)
										hovercy = 0x68;
								} else if (hoverpx < 0x17) {
									hovercx = 3;
									hovercy = 0;
								} else if (hoverpx > 0x7e) {
									hovercx = 3;
									hovercy = 0x67;
								} else
									hovercx = 999;
								break;
							default:
								hovercx = 999;
								hovercy = 0;
								break;
							}
							if (menumode || trademode)
								goto done;
							if (units[0].asleep || units[0].mode == 'Z') {
							busy:
								setcursor1(3, 0x2f);
								goto done;
							}
							if (playmode == 3)
								goto map;
						other:
							setcursor1(3, 3);
							goto done;
						}
					}
				if (trademode) {
					if (hoverx >= 0xe6 && hoverx <= 0x182 && hovery >= 0x193 && hovery < 0x1ab) {
						hoverkind = 0xdb;
						hovercy = 0;
						hovercx = 0;
					} else if (hoverx >= 0xe6 && hoverx <= 0x182 && hovery >= 0x1b6 && hovery < 0x1ce) {
						hoverkind = 0xdc;
						hovercy = 0;
						hovercx = 0;
					} else if (hoverx >= 0x1a && hoverx < 0xb6) {
						hoverkind = 0xd9;
						hovercy = 0;
						if (hovery >= 0x2d && hovery < 0x1cd) {
							hovercx = (hovery - 0x2d) / 0x34 * 3;
							hovercx += (hoverx - 0x1a) / 0x34;
						} else if (hovery < 0x2d)
							hovercx = (hoverx - 0x1a) / 0x34;
						else
							hovercx = 999;
					} else if (hoverx >= 0xe6 && hoverx < 0x182) {
						hoverkind = 0xda;
						hovercy = 0;
						if (hovery >= 0x17 && hovery < 0x1b7) {
							hovercx = (hovery - 0x17) / 0x34 * 3;
							hovercx += (hoverx - 0xe6) / 0x34;
						} else if (hovery < 0x17)
							hovercx = (hoverx - 0xe6) / 0x34;
						else
							hovercx = 999;
					} else
						hoverkind = 0;
				} else {
					hoverkind = 1;
					hovercx -= 8;
					hovercx /= 48;
					hovercx += playerx - 6;
					hovercy += 24;
					hovercy /= 48;
					hovercy += playery - 5;
					if (hoverx < 0x140) {
						ptrdistx = 0x13f - hoverx;
						ptrdx = -1;
					} else {
						ptrdx = 1;
						ptrdistx = hoverx - 0x140;
					}
					if (hovery < 0xf0) {
						ptrdisty = 0xef - hovery;
						ptrdy = -1;
					} else {
						ptrdy = 1;
						ptrdisty = hovery - 0xf0;
					}
					ptrpace = 1;
					if (!lostcontrol && !dirkeys) {
						if (ptrdistx <= slowradius && slowradius >= ptrdisty)
							ptrpace = 0;
						else if (ptrdistx >= runradius || runradius <= ptrdisty)
							ptrpace = 2;
						else
							ptrpace = 1;
					}
					if (ptrdx && ptrdy) {
						if (abs(ptrdistx) < abs(ptrdisty) / 2) {
							ptrdx = 0;
							ptrdistx = 0;
						} else if (abs(ptrdisty) < abs(ptrdistx) / 2) {
							ptrdy = 0;
							ptrdisty = 0;
						}
					}
					if (!menumode && !trademode) {
						if (units[0].asleep || units[0].mode == 'Z')
							goto busy;
						else if (busyturns || units[0].pose == 9)
							setcursor1(3, 0x2e);
						else if (playermoving()) {
							if (ptrpace >= 2) {
								if (ptrdy == -1) {
									if (ptrdx == -1)
										k = 0x10;
									else if (ptrdx == 1)
										k = 0x12;
									else
										k = 0x11;
								} else if (ptrdy == 1) {
									if (ptrdx != -1) {
										if (ptrdx == 1)
											k = 0x2c;
										else
											k = 0x2b;
									} else
										k = 0x2a;
								} else {
									if (ptrdx != -1) {
										if (ptrdx == 1)
											k = 0x1f;
										else
											goto map;
									} else
										k = 0x1d;
								}
							} else if (ptrpace >= 1) {
								if (ptrdy == -1) {
									if (ptrdx != -1) {
										if (ptrdx == 1)
											k = 0xf;
										else
											k = 0xe;
									} else
										k = 0xd;
								} else if (ptrdy == 1) {
									if (ptrdx != -1) {
										if (ptrdx == 1)
											k = 0x29;
										else
											k = 0x28;
									} else
										k = 0x27;
								} else {
									if (ptrdx != -1) {
										if (ptrdx == 1)
											k = 0x1c;
										else
											goto map;
									} else
										k = 0x1a;
								}
							} else {
								if (ptrdy == -1) {
									if (ptrdx != -1) {
										if (ptrdx == 1)
											k = 0xa8;
										else
											k = 0xa7;
									} else
										k = 0xa6;
								} else if (ptrdy == 1) {
									if (ptrdx != -1) {
										if (ptrdx == 1)
											k = 0xc2;
										else
											k = 0xc1;
									} else
										k = 0xc0;
								} else {
									if (ptrdx != -1) {
										if (ptrdx == 1)
											k = 0xb5;
										else
											goto map;
									} else
										k = 0xb3;
								}
							}
							setcursor1(3, k);
							movecursor = 1;
						} else {
						map:
							if (playmode == 3) {
								if (hoverkind == 1) {
									hoverdx = abs(playerx - hovercx);
									hoverdy = abs(playery - hovercy);
									if (isthrowing(units[0].hand) && (hoverdx > 1 || hoverdy > 1)) {
										if (hoverdx > 6 || hoverdy > 6)
											ptrmark1 = 0x3b;
										else {
											ptrfar = 0;
											if (clearsight(playerx, playery, hovercx, hovercy)) {
												ptrblocked = 0;
												ptrmark1 = 0x3a;
											} else
												ptrmark1 = 0x3b;
										}
									} else if (hoverdx <= 1 && hoverdy <= 1) {
										ptrfar = 0;
										ptrblocked = 0;
									} else
										ptrmark1 = 0x3b;
									if (cansee(hovercx, hovercy)) {
										hoverdx = unitat(hovercx, hovercy);
										if (hoverdx != 0xff && hoverdx != 0) {
											if (units[hoverdx].sympathy > 0) {
												if (units[hoverdx].mode == 0x16 && units[hoverdx].modeb == 0) {
													ptrmark2 = 0x8d;
													if (ptrmark1 == 0x3a)
														ptrmark1 = 0x3b;
													else
														ptrmark1 = 0;
												} else
													ptrmark2 = 8;
											}
											if (units[hoverdx].health < kindof(&units[hoverdx])->maxhealth / 3)
												ptrimg = 7;
											else if (units[hoverdx].health < kindof(&units[hoverdx])->maxhealth / 2)
												ptrimg = 6;
											else if (units[hoverdx].health < kindof(&units[hoverdx])->maxhealth * 2 / 3)
												ptrimg = 5;
											else
												ptrimg = 0x2d;
											ptrunit = hoverdx;
											if (hoverdx)
												ptrstates = stateflags(&units[hoverdx]);
											setcursor(3, ptrimg, 3, ptrmark1, 3, ptrmark2);
											goto done;
										}
										hoverdx = abs(playerx - hovercx);
										hoverdy = abs(playery - hovercy);
										if (hoverdx <= 1 && hoverdy <= 1) {
											if (playerx == hovercx && playery == hovercy)
												goto other;
											hoveritem = topitem(hovercy * 150 + hovercx);
											if (hoveritem) {
												hoveritem = mapitems[hoveritem];
												goto item;
											}
										}
									}
								} else if (hoverkind == 0x29 || hoverkind == 0x2a) {
									if (panels[hoverkind - 0x28].id == 0) {
										if (hovercx == 0x14) {
											setcursor(3, 3, 3, 8, 0, 0);
											goto done;
										}
										if (hovercx == 0x15 && units[0].belt) {
											setcursor1(3, 0xc);
											goto done;
										}
										if (hovercx == 10)
											goto walk;
									}
									hoveritem = panelgear(hoverkind, hovercx);
								item:
									if (hoveritem) {
										hoveri = classof(hoveritem & 0xfff);
										if (hoveri == 0x3c || hoveri == 0x55) {
											hovergear = hoveritem;
											setcursor1(3, 0xc);
											goto done;
										}
									}
								}
							walk:
								setcursor1(3, 2);
							} else
								goto other;
						}
					} else if (pagearrows) {
						if (pagearrows & 4) {
							if (pagearrows & 1 && hoverx <= 0x6a)
								setcursor(0, 0xffff, 3, 0x31, 0, 0xffff);
							else if (pagearrows & 2 && hoverx >= 0x216)
								setcursor(0, 0xffff, 3, 0x32, 0, 0xffff);
							else
								setcursor(0, 0xffff, 0, 0, 0, 0xffff);
						} else {
							if (pagearrows & 1 && hoverx <= 0x13f)
								setcursor(0, 0xffff, 3, 0x31, 0, 0xffff);
							else if (pagearrows & 2 && hoverx >= 0x140)
								setcursor(0, 0xffff, 3, 0x32, 0, 0xffff);
							else
								setcursor(0, 0xffff, 0, 0, 0, 0xffff);
						}
					}
				}
			}
		done:
			if (shown)
				drawcursor();
		} else if (movecursor && (waitforup || !playermoving()))
			goto mouse;
		if (presslatch == 0) {
			presskind = hoverkind;
			presscx = hovercx;
			presslatch = 1;
			presscy = hovercy;
		}
		if (pausekey) {
			pausekey = 0;
			if (!palblack) {
				showsign(0x19, 0, 1);
				pausescr = GlobalAlloc(0, 0x4b000);
				if (pausescr)
					getscreen(1, (char *)pausescr);
				showsign(0x19, 0, 0);
				do {
					pollinput();
					if (needredraw(0xfd)) {
						if (pausescr)
							putscreen((char *)pausescr, 0);
						flip();
						showsign(0x19, 0, 0);
					}
				} while (keycode || mouseleft || mouseright);
				do {
					pollinput();
					if (needredraw(0xfd)) {
						if (pausescr)
							putscreen((char *)pausescr, 0);
						flip();
						showsign(0x19, 0, 0);
					}
				} while (!keycode && !mouseleft && !mouseright && !pausekey);
				do {
					pollinput();
					if (needredraw(0xfd)) {
						if (pausescr)
							putscreen((char *)pausescr, 0);
						flip();
						showsign(0x19, 0, 0);
					}
				} while (keycode || mouseleft || mouseright);
				if (pausescr) {
					putscreen((char *)pausescr, 0);
					flip();
				}
				hidesign();
				if (pausescr) {
					GlobalFree(pausescr);
					pausescr = 0;
				}
				pausekey = 0;
				continue;
			}
		}
		if (musickey) {
			musickey = 0;
			togglemusic();
			if (palblack)
				resetinput();
			else {
				showsign(0x1b, musicoff ? 0x23 : 0, 1);
				hoveritem = ticks() + 1000;
				do
					pollinput();
				while (keycode || mouseleft || mouseright);
				do
					pollinput();
				while (ticks() < hoveritem);
				hidesign();
			}
			musickey = 0;
			continue;
		}
		if (sfxkey) {
			sfxkey = 0;
			if (dsready && dsready != 0xff) {
				if (soundson) {
					stopsounds();
					soundson = 0;
				} else
					soundson = 1;
			}
			if (palblack)
				resetinput();
			else {
				showsign(0x1e, dsready == 0 || dsready == 0xff || soundson == 0 ? 0x23 : 0, 1);
				hoveritem = ticks() + 1000;
				do
					pollinput();
				while (keycode || mouseleft || mouseright);
				do
					pollinput();
				while (ticks() < hoveritem);
				hidesign();
			}
			sfxkey = 0;
			continue;
		}
		pollinput();
		if (keycode == 0 && heldcmd) {
			if ((unsigned char)heldcmd == lastscan)
				lastscan = 0;
			heldcmd = 0;
		}
		if (shotkey) {
			if (!palblack) {
				screenshot();
				do
					pollinput();
				while (keycode || ((char *)keys)[0x50] & 0x80);
			}
			lastscan = 0;
			keycode = 0;
			shotkey = 0;
		}
		return inputok;
	}
}

void
quit(void)
{
	stopmusic();
	PostQuitMessage(0);
	esckey = 0xff;
	quitting = 1;
}

BOOL CALLBACK
errproc(HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
	RECT r;
	int h, w, x, y;

	switch (msg) {
	case WM_INITDIALOG:
		SetWindowText(dlg, gametitle());
		GetWindowRect(dlg, &r);
		w = r.right - r.left;
		h = r.bottom - r.top;
		x = GetSystemMetrics(SM_CXSCREEN) - w;
		if (x < 0)
			x = 0;
		x /= 2;
		y = GetSystemMetrics(SM_CYSCREEN) - h;
		if (y < 0)
			y = 0;
		y /= 2;
		SetWindowPos(dlg, NULL, x, y, w, h, SWP_NOSIZE);
		if (hasurl(initerr)) {
			ShowWindow(GetDlgItem(dlg, 0x33), SW_SHOW);
			SetDlgItemText(dlg, 0x33, pick(texts + 0x6e12, texts + 0x6e24, texts + 0x6e33, texts + 0x6e34, texts + 0x6e35, texts + 0x6e43));
		} else
			ShowWindow(GetDlgItem(dlg, 0x33), SW_HIDE);
		SetDlgItemText(dlg, 3, pick(texts + 0xd4, texts + 0xdf, texts + 0xe6, texts + 0xe7, texts + 0xe8, texts + 0xf1));
		SetDlgItemText(dlg, 0x32, pick(texts + 0x6e44, texts + 0x6e52, texts + 0x6e5f, texts + 0x6e60, texts + 0x6e61, texts + 0x6e74));
		SetDlgItemText(dlg, 0x3e, pick(texts + 0x155, texts + 0x15c, texts + 0x162, texts + 0x163, texts + 0x164, texts + 0x16b));
		SetDlgItemText(dlg, 0x3d, initerr);
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case 0x33:
			wsprintf(textbuf, texts + 0x990a, langcode);
			copystr(tmpbuf, texts + 0xa245);
			catstr(tmpbuf, textbuf);
			run(tmpbuf, NULL, NULL);
			EndDialog(dlg, 1);
			return 1;
		case 0x32:
			openreadme();
			EndDialog(dlg, 1);
			return 1;
		case 3:
			EndDialog(dlg, 0);
			return 0;
		}
	}
	return 0;
}

int PASCAL
WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
{
	int i;

	appinst = inst;
	mainwnd = NULL;
	setlogdir("c:\\dat\\p-surt\\facts");
	logf("Start");
	if (!inisys(inst, cmd, initerr)) {
		logf("!ini_sys");
	err:
		logf("outp err & xit");
		if (initerr[0]) {
			trimlines(initerr);
			ShowWindow(mainwnd, SW_SHOWMINIMIZED);
			DialogBoxParam(inst, MAKEINTRESOURCE(0x28), mainwnd, (DLGPROC)errproc, 0);
		}
		closemutex();
		PostQuitMessage(0);
		while (PeekMessage(&pumpmsg, NULL, 0, 0, PM_NOREMOVE) && GetMessage(&pumpmsg, NULL, 0, 0)) {
			TranslateMessage(&pumpmsg);
			DispatchMessage(&pumpmsg);
		}
		return 0;
	}
	logf("vor ini win");
	if (!initwin(inst, show)) {
		MessageBox(mainwnd, texts + 0x8e6b, texts + 9, MB_OK);
		return 0;
	}
	logf("vor ini gr");
	if (!ddstart(quickstart ? NULL : scrbuf, defpal)) {
		wsprintf(initerr, pick(texts + 0x794f, texts + 0x7980, texts + 0x79a3, texts + 0x79a4, texts + 0x79a5, texts + 0x79cb), dderrmsg);
		goto err;
	}
	logf("vor ini sys after igr");
	initres = inisys2(initerr);
	logf("nach ini sys after igr");
	if (!initres) {
		exitsys();
		goto err;
	}
	if (!quitting) {
		if (initres == 2)
			cheat = 1;
		else if (initres == 3)
			goto loaded;
		if (entergame(0) == 0 || units[0].health <= 0) {
			mainmenu();
			if (quitting)
				goto quit;
		}
		leavemenu(1);
	loaded:
		if (units[0].health > 0 && !exists(texts + 0x9bfd))
			facetmpl(0);
		if (hasversion()) {
			esckey = 0;
			msgf(0x63, strs[0x47f],
				reg.oldinfo & 2 ? pick(texts + 0x4d6, texts + 0x4e2, texts + 0x4ef, texts + 0x4f0, texts + 0x4f1, texts + 0x503)
				: pick(texts + 0x4ab, texts + 0x4b8, texts + 0x4c5, texts + 0x4c6, texts + 0x4c7, texts + 0x4d5),
				reg.oldmajor, reg.oldminor,
				pick(texts + 0x4d6, texts + 0x4e2, texts + 0x4ef, texts + 0x4f0, texts + 0x4f1, texts + 0x503),
				(version[8] - '0') * 10 + (version[9] - '0'),
				version[13] >= '0' && version[13] <= '9'
				? (version[11] - '0') * 100 + (version[12] - '0') * 10 + (version[13] - '0')
				: (version[11] - '0') * 10 + (version[12] - '0'));
			reg.oldmajor = 0;
			reg.oldminor = 0;
			tipend = gametime + 30;
		} else if (noversion()) {
			reg.oldmajor = 0;
			reg.oldminor = 0;
		}
		if (featuresoff) {
			dropheld();
			msgf(0x63, strs[0x231]);
			holdmsg();
			featuresoff = 0;
		}
		if (firststart)
			firststart = 0;
		resetinput();
		if (steptick == 0) {
		newmap:
			playerturn();
			if (quitting)
				goto quit;
		entermap:
			actionturn = 1;
			if (playmode != 1) {
				rununits(0);
				playqueue();
				markunits();
			}
			runtalker();
			goto check;
		}
		goto input;
	play:
		endspars();
		runtalker();
		for (;;) {
			if (quitting)
				goto quit;
		input:
			if (!readinput())
				goto quit;
			if (quitting)
				goto quit;
			if (!appactive)
				continue;
			if (!lostcontrol) {
				edgemenus(0);
				if (quitting)
					goto quit;
				if (units[0].health <= 0)
					goto dead;
			}
			inputhook();
			if (quitting)
				goto quit;
			if (lostcontrol && units[0].berserk == 0 && units[0].fleeing == 0)
				regaincontrol();
			if ((units[0].pose == 0 || units[0].pose > 9) && playmode != 3 && !lostcontrol) {
				doinput();
				if (units[0].health <= 0)
					goto dead;
				readinput();
				if (quitting)
					goto quit;
			}
			if (!skiptick) {
				tracesite = 0;
				maintick = 1;
				gametick();
				maintick = 0;
			} else
				skiptick = 0;
			if (units[0].health <= 0)
				goto dead;
			if (steptick != lasttick)
				break;
		}
		lasttick = steptick;
		if (steptick != 0)
			goto play;
		if (merciful) {
			sayunit = &units[merciful];
			if (sayunit->health > 0 && displayed(sayunit) && seesunit(sayunit)) {
				saynum = sayunit->mercy;
				sayunit->mercy = 0;
				sayscene();
			}
		}
		if (pleader) {
			sayunit = &units[pleader];
			if (sayunit->health > 0 && displayed(sayunit) && seesunit(sayunit)) {
				saynum = sayunit->plea;
				sayunit->plea = 0;
				sayscene();
			}
		}
		runtalker();
		clearscenes();
		if (game.queuedtext && !playermoving() && !tipshown() && nmsgs == 0) {
			cleartexts();
			if (game.queuedtext >= nstrs)
				game.queuedtext = nstrs - 1;
			showtip(0x128, 0xd7, strs[game.queuedtext]);
			drawscreen();
			leavemenu(1);
			game.queuedtext = 0;
			game.queuedprio = 0;
		}
		challenger = 0;
		for (mainv = 1; mainv < nunits; mainv++)
			if (units[mainv].mode == 4)
				if (units[mainv].modeb == 3) {
					if (units[mainv].x - 2 > playerx || units[mainv].x + 2 < playerx
					|| units[mainv].y - 2 > playery || units[mainv].y + 2 < playery)
					forget:
						units[mainv].modec = 0;
					else if (units[mainv].x - 1 <= playerx && units[mainv].x + 1 >= playerx
					&& units[mainv].y - 1 <= playery && units[mainv].y + 1 >= playery) {
					attack:
						if (units[mainv].modeb) {
							provoke(&units[0], &units[mainv], 3, 8, 0);
							units[mainv].mode = 0x14;
							autocombat();
							units[mainv].pose = 0;
						} else
							goto mark;
					} else if (units[mainv].x - 2 <= playerx && units[mainv].x + 2 >= playerx
					&& units[mainv].y - 2 <= playery && units[mainv].y + 2 >= playery
					&& !units[mainv].modec) {
					mark:
						if (challenger == 0) {
							challenger = mainv;
							units[mainv].modec = 1;
							if (units[mainv].modeb == 0)
								offguard(&units[mainv], 1, 3);
						}
					}
				} else {
					if (units[mainv].x - 3 > playerx || units[mainv].x + 3 < playerx
					|| units[mainv].y - 3 > playery || units[mainv].y + 3 < playery)
						goto forget;
					if (units[mainv].x - 2 <= playerx && units[mainv].x + 2 >= playerx
					&& units[mainv].y - 2 <= playery && units[mainv].y + 2 >= playery)
						goto attack;
					if (units[mainv].modec == 0)
						goto mark;
				}
		if (challenger) {
			if (units[challenger].scene)
				talkto(challenger);
			challenger = 0;
		}
		mainv = findclass(playercell, 0x34);
		mainw = findclass(playercell, 0x69);
		if (mainv && mainw) {
			if (mapitems[mainv] >> 16 != 0 || randn(100) < 10
			|| prevails(skillof(&units[0], 0x15, 0, 0, 0), 0x5a)) {
				msgf(0x63, strs[0x568]);
				playnow(0x26);
			} else {
				hurt(0, &units[0], 1, 2, randn(60) + 10);
				msgf(0x63, strs[0x569]);
			}
			takeitem(playercell, mainw);
			if (units[0].pose == 1)
				units[0].pose = 0;
			leavemenu(1);
		}
		if (units[0].health <= 0)
			goto dead;
		readinput();
		if (waitforup || lostcontrol)
			goto entermap;
		goto newmap;
	dead:
		playqueue();
	check:
		robbed = 0;
		if (units[0].health == -30000)
			heroend(game.fate);
		else if (units[0].health == -31000)
			heroend(fate());
		else {
			if (restbreak)
				restfor(0xfe);
			if (units[0].health == -32000)
				heroend(7);
			else {
				if (exhausted() && !restbreak) {
					blackout(0);
				again:
					outcold();
					if (units[0].health <= 0) {
						heroend(game.fate);
						goto next;
					}
					awaken();
					if (robbed == 1 && units[0].health > 0)
						msgf(0x63, strs[0x156]);
					robbed = 0;
				}
				while (units[0].health <= 0 && !restbreak) {
					if (game.fate == 0) {
						if (nearfoe(0))
							game.fate = 2;
						goto respawn;
					}
					if (!robbery(curmap, game.killer))
						goto respawn;
					mainv = 0;
					if ((mainw = findclass(playercell, 0x15)) != 0)
						do
							takeitem(playercell, mainw);
						while ((mainw = findclass(playercell, 0x15)) != 0 && mainv++ < 8);
					units[0].health = 1;
					blackout(0);
					units[0].bleeding = 0;
					units[0].health = 15;
					handover(&units[0], 0);
					sackarea(playerx - 30, playery - 30, playerx + 30, playery + 30, 0, -2);
					for (i = 1; i < nunits; i++)
						killunit(&units[i], 0, 0, 0xfe, 0);
					game.robberies++;
					robbed = 1;
					goto again;
				}
				goto play;
			respawn:
				blackout(1);
				heroend(game.fate);
			}
		}
	next:
		mainmenu();
		if (quitting)
			goto quit;
		if (steptick == 0)
			goto newmap;
		goto play;
	}
quit:
	exitsys();
	switch (ordercmd) {
	case 0:
	case 3:
	case 4:
	case 5:
		break;
	case 2:
		pathjoin(tmpbuf, progdir, texts + 0x9bc9);
		run(tmpbuf, NULL, NULL);
		break;
	case 6:
		pathjoin(tmpbuf, savesdir, texts + 0x9bbf);
		run(tmpbuf, NULL, NULL);
		break;
	case 1:
	case 7:
	case 8:
	default:
		wsprintf(textbuf, texts + 0x9934, langcode);
		copystr(tmpbuf, texts + 0xa245);
		catstr(tmpbuf, textbuf);
		run(tmpbuf, NULL, NULL);
		break;
	}
	return exitcode;
}

char homeurl[26] = "http://www.wolfmittag.com";
short nofoes = 1;
int appactive = 1;
int wasactive = 1;
int posth;
unsigned long drapetype;
short osmi;
short brugi;
int cow;
int cowi;
int cowx;
int cowy;
short deathi;
short leavei;
unsigned long enterv;
short feasti;
short feastreg;
char savedfate;
int manipv;
char callsseen;
unsigned long keptitem;
int proceeds;
short visions[30];
short nvisions;
MSG pollmsg;
UINT keyvk;
int inputok;
short hoveri;
int hoverdx;
int hoverdy;
unsigned long hoveritem;
Panel *hoverpanel;
HGLOBAL pausescr;
int initres;
unsigned long mainv;
unsigned long mainw;
int robbed;
char combatcause;
int tracesite;
HWND mainwnd;
HINSTANCE appinst;
HACCEL accel;
int quitting;
int exitcode;
POINT cursorpos;
char initerr[3000];
MSG pumpmsg;
int signx;
int signy;
int signcur;

#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

typedef struct Unit Unit;
typedef struct Recipe Recipe;
typedef struct Deco Deco;
typedef struct Pose Pose;
typedef struct Anim Anim;
typedef struct Prov Prov;
typedef struct Zone Zone;
typedef struct Part Part;
typedef struct Panel Panel;
typedef struct Kind Kind;
typedef struct Metal Metal;
typedef struct Gear Gear;
typedef struct Game Game;
typedef struct Icons Icons;
typedef struct Box Box;
typedef struct Realm Realm;

/* 0x1b8 bytes each (009bc240) */
struct Unit {
	unsigned char x;
	unsigned char y;
	unsigned char nextx;
	unsigned char nexty;
	signed char dirx;
	signed char diry;
	unsigned short picset;
	unsigned char pose;
	unsigned char mode;
	unsigned char modea;
	unsigned char modeb;
	unsigned char modec;
	char fd[0xe6 - 0xd];
	unsigned char part;
	char fe7;
	unsigned char frame;
	char fe9[0xf3 - 0xe9];
	short health;
	char ff5[0xf8 - 0xf5];
	unsigned long neck;
	unsigned long wrist;
	unsigned long belt;
	unsigned long head;
	unsigned long body;
	unsigned long hand;
	unsigned long offhand;
	unsigned long legs;
	unsigned long feet;
	unsigned long item1;
	unsigned long item2;
	unsigned long item3;
	unsigned long cloak;
	char f12c[0x135 - 0x12c];
	char asleep;
	unsigned char bewitched;
	unsigned char blessed;
	unsigned char poison;
	unsigned char cursed;
	unsigned char fleeing;
	char confused;
	unsigned char bleeding;
	unsigned char berserk;
	unsigned char drunk;
	unsigned char weakened;
	unsigned char warded;
	char f141[0x146 - 0x141];
	signed char sympathy;
	char f147[0x14a - 0x147];
	unsigned char handkind;
	unsigned char beltkind;
	char f14c[0x154 - 0x14c];
	unsigned char cellmate;
	char incoming;
	unsigned char idlestep;
	unsigned char idlepose;
	char f158[0x161 - 0x158];
	short face;
	char f163[0x1b8 - 0x163];
};

/* 0x1c bytes each (*004f9980) */
struct Deco {
	short unit;
	short x;
	short y;
	char f6[2];
	char *pics[4];
	unsigned long pic;
};

/* 0x20 bytes each (0055a078), by a unit's part */
struct Recipe {
	short parts[5];
	unsigned char colors[4];
	char fe[0x20 - 0xe];
};

/* 27 bytes: a move of a unit kind */
struct Pose {
	signed char dx;
	signed char dy;
	unsigned char chance;
	unsigned short pics[6];
	unsigned short marks[6];
};

/* 0x1b4 bytes each (004c4180), by the unit type's anim */
struct Anim {
	short chancea;
	short chanceb;
	Pose anims[2][8];
};

/* 0x164 bytes each (005d8510) */
struct Prov {
	short x0;
	short y0;
	short x1;
	short y1;
	short cx;
	short cy;
	short xs[16];
	short ys[16];
	short n;
	char f4e[0x6c - 0x4e];
	unsigned char kinds[16];
	char f7c[0x164 - 0x7c];
};

/* 0x22 bytes each (004f9498) */
struct Zone {
	short id;
	unsigned short name;
	char f4[0x12 - 4];
	short inx;
	short iny;
	char f16[0x1a - 0x16];
	short x;
	short y;
	char f1e[0x22 - 0x1e];
};

/* 0x22 bytes each: unittype.cpp's partdefs (004c6f48) and parts */
struct Part {
	char f0[4];
	unsigned char recolor;	/* color ranges that dyeset recolors */
	char cansit;
	unsigned char anim;
	char f7[0xe - 7];
	short text;		/* a text */
	char grassmask;
	unsigned long size;	/* of the cached picture set */
	// PORT: partpics holds the pointer, which 64 bits widen past these 4 bytes
#ifdef PORT
	char pics[4];
#else
	char *pics;		/* the cached picture set */
#endif
	char showhead;
	char showbody;
	char showcloak;
	unsigned char showjewels;
	char setflag;
	unsigned char hidering;
	char f1f[0x22 - 0x1f];
};

/* 0x28 bytes each (00a4ca78): the 16 panels (windows) */
struct Panel {
	short type;
	char f2[6 - 2];
	int x;
	int y;
	int w;
	int h;
	unsigned short id;
	unsigned short arg;
	char f1a[0x1c - 0x1a];
	short cellx;
	short celly;
	char f20[0x28 - 0x20];
};

/* 0x1b bytes each (004c08a8), by gearof of an item */
struct Metal {
	char f0[4];
	short attack;
	short parry;
	char f8[0x1b - 8];
};

/* the icons drawn beside a unit (00a3b7b8) */
struct Icons {
	int kinds[10];
	char marks[10];
};

/* the saved game, 0x5f8 bytes (009cc470) */
struct Game {
	char name[0x14];
	short look;
	char known[0x1e];
	char flags[0xc8];
	int skills[30];
	short tiredcount;
	unsigned char tiredlevel;
	short hungercount;
	unsigned char hungerlevel;
	unsigned char colors[4];
	short queuedtext;
	unsigned char queuedprio;
	unsigned short stolen;
	unsigned short killed;
	unsigned short sleepers;
	unsigned short ambushed;
	unsigned short broken;
	unsigned short defeated;
	unsigned long sacrificed;
	short invoked;
	int favor;
	unsigned char wodenblows;
	unsigned long sunk;
	short outfittext;
	unsigned short runkills;
	char f1a0[0x1d4 - 0x1a0];
	unsigned char oldlevels[30];
	unsigned char levels[30];
	unsigned long snaptime[30];
	short savespare;
	char fate;
	short killer;
	unsigned short mercies;
	unsigned short womenhit;
	unsigned short quests;
	unsigned long booty;
	int score;
	short id;
	int snappoints[30];
	unsigned char fightinvokes;
	unsigned short poisoned;
	short faces[20];
	char f340[0x39c - 0x340];
	unsigned char foes[30];
	char f3ba[0x3dc - 0x3ba];
	short absenttime;
	short backday;
	short lastregion;
	int regiontime;
	char f3e6[0x3ea - 0x3e6];
	unsigned long invoketime;
	int msgpoints[30];
	char robberies;
	unsigned char ncaves;
	unsigned char cavekind[10];
	unsigned char cavex[10];
	unsigned char cavey[10];
	unsigned char cavelayout[10];
	unsigned char cavemap[10];
	unsigned char cmd4fails;
	char f49b[0x4bd - 0x49b];
	int strpoints;
	int dexpoints;
	int intpoints;
	char f4c9[0x5f8 - 0x4c9];
};

/* 0x1a bytes each (004c0bd8), by gearof of an item */
struct Gear {
	char f0[0xe];
	short hindrance;
	char f10[0x1a - 0x10];
};

/* 0x68 bytes each (00a52a30): a box of text lines and pictures */
struct Box {
	char *lines[20];
	int icon;
	int window;		/* a picture */
	int worn;		/* a picture */
	// PORT: the unit as a pointer, which 64 bits widen past an int
#ifdef PORT
	Unit *unit;
#else
	int unit;
#endif
	short x;
	short y;
	short w;
	short h;
};

/* 0x158 bytes each (005a1b78) */
struct Realm {
	unsigned long item;
	unsigned long items[50];
	unsigned short pos[50];	/* y * 320 + x */
	char f130[0x158 - 0x130];
};

/* what kindof returns */
struct Kind {
	char f0[0x18];
	short maxhealth;
};

short indexof(Unit *u);
short maysit(Unit *u);
void resetpose(Unit *u, unsigned char v);
short isanimal(Unit *u);
short isdog(Unit *u);
int cellflag40(unsigned long cell);
void tilemask(unsigned char *dst, unsigned char *src);
void tintsilver(char *p, int n);
void tintbronze(char *p, int n);
void cleartrail(Unit *u);
void clearscr(void);
int showscrt(void);
void setscr2(int x, int y, unsigned char c);
void setscr2o(int off, unsigned char c);
void setscr4(int off, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
short mapzone(short n);
short isfemale(Unit *u);
Part *partof(Unit *u);
unsigned short findclass(int pos, unsigned char c);
short findtype(int pos, unsigned long type);
short isthrowing2(unsigned long item);
void sqclip(int x, int y);
void sqputt(char *src);
void sqput48t(char *src);
int playerdead(void);
int releaseheld(void);
void delmsg(short n);
int text3w(char *s);
void tipclear(void);
int seekentry(int id);
unsigned short idof(unsigned long item);
int raisepanel(unsigned char id);
Kind *kindof(Unit *u);
void set48x2(unsigned char *buf, int x, int y, unsigned char c);
void clear48(int x, int y);
void putcs(int x, int y, unsigned n);
void putic(int s, int x, int y, unsigned n);
int get48(unsigned char *dst, int n, int x, int y);
int badunit(Unit *u);
void putpix2i(int n, int x, int y, int c);
int overloaded(Unit *u);
int put48pt(int n, int x, int y, char *src, int sx, int sy, int w, int h);
unsigned long baselook(unsigned long item);
int putwint(int x, int y, int idx);
short istorch(unsigned long item);
int put48t0(int x, int y, char *src);
int puto(int x, int y, int n);
unsigned short itemweight(unsigned long item);
unsigned long valueof(unsigned long item);
unsigned long silvvalue(unsigned long item);
void damagerange(unsigned long item, short *a, short *b);
unsigned short gearof(unsigned long item);
char *ratetext(short n);
void dmgrange(char *buf, short a, short b);
short protection(unsigned long item);
char *armortext(short n);
int satiety(unsigned long x);
int textwidth(char *s);
int rectw(int idx);
int recth(int idx);
void inkshade(void);
void drawtext2o(int n, unsigned off, char *s);
int putwino(int x, int y, int idx);
unsigned char classof(int item);
void cointext(char *s, unsigned long item);
char *itemname(unsigned long item);
char *unitname(Unit *u);
void titlecase(char *s);
unsigned short blessof(unsigned long x);
short blessid(short n);
char *weartext(short n);
char *breaktext(short n);
void statustext(char *buf, Unit *u);
short freshid(unsigned long item);
unsigned long fishfood(unsigned long item);
void putcol(int n, int x, int y, int h, unsigned char *src);
int gearicon(unsigned long item);
int putsrc(int n, int x, int y, int kind, int idx);
char *daytime(unsigned char n);
char *carrytext(Unit *u, short n);
char *capname(Unit *u);
short myservant(Unit *u);
int __cdecl readinput(void);
int __cdecl pollinput(void);
DWORD unpack(void *dst, DWORD dstlen, void *src, DWORD srclen);

extern Unit units[];
extern unsigned short nunits;
extern char pics[][0x900];
extern short censorship;
extern unsigned char *tilecache;
extern int setpics;
extern int dir1frm0;
extern int dir1frm6;
extern int dir2frm0;
extern int dir2frm6;
extern int dir3frm0;
extern int dir3frm6;
extern unsigned fireframe;
extern char recipes[1488];
extern int hasgearbank;
extern int playerx;
extern int playery;
extern int scrollx;
extern int scrolly;
extern Deco *decos;
extern int sqstate;
extern char mapframed;
extern short nmsgs;
extern short msgkeep[17];
extern unsigned long gametime;
extern short tipclears;
extern int text3width;
extern int hovercx;
extern int tipanchorx;
extern int tipanchory;
extern int playmode;
extern char trademode;
extern char talkactive;
extern char lastspeech;
extern unsigned short curunit;
extern unsigned char faceflags[288];
extern short headanims[][12];
extern short handanims[][12];
extern short footanims[][12];
extern short heroblinkmin;
extern short heroblinkmax;
extern short curblinkmin;
extern short curblinkmax;
extern unsigned long map[150 * 150];
extern unsigned long mapflags[150 * 150];
extern Anim anims[13];
extern int dir0frm7;
extern int minmem;
extern int minsets;
extern int nopvari;
extern int noprun;
extern int dir0frm0;
extern unsigned char *gearbank;
extern int decoframe;
extern int decophase;
extern short heropace;
extern char scrbuf[309700];
extern char grassmask[];
extern char *grassbody;
extern char *gearbody;
extern unsigned char blocks[30][30];
extern unsigned char terrains[];
extern unsigned char provmap[150 * 150];
extern Prov provs[100];
extern short nsites;
extern unsigned char dots[5390];
extern unsigned short curmap;
extern Game game;
extern unsigned char caves[][2];
extern Zone zones[30];
extern int cheat;
extern unsigned char keys[256];
extern int debugmap;
extern unsigned char partkinds[][2];
extern Part partdefs[];
// PORT: the picture sets of partdefs, in unittype.cpp
#ifdef PORT
extern char *partpics[78];
#endif
extern char packbuf[];
extern char picbuf[];
extern HANDLE datfh;
extern DWORD storedn;
extern int dir1frm7;
extern int dir2frm7;
extern int dir3frm7;
extern unsigned char builtcolors[4];
extern int builtbody;
extern int builtlegs;
extern int builtarmor;
extern int builtcloak;
extern int builtbrace;
extern int builtring;
extern int builthelm;
extern int builtextra;
extern int builtfeet;
extern unsigned char panelstack[16];
extern int mousemoved;
extern int savedinvx;
extern int savedsackx;
extern int savedsacky;
extern unsigned char typenames[];
extern int ptrstates;
extern int curshape;
extern int ptrunit;
extern short sensesleft;
extern unsigned char gaugepeace;
extern unsigned char gaugecombat;
extern unsigned char tallcombat;
extern unsigned char mode2icon;
extern char tallpeace;
extern char mode3icon;
extern short dollday;
extern unsigned short hourticks;
extern unsigned short stepframe;
extern unsigned char headicons[8][2304];
extern char printbuf[];
extern char pounds;
extern char *strs[];
extern char texts[47942];
extern Metal metals[30];
extern Gear armors[];
extern short infow;
extern short infoh;
extern short lastlight;
extern char workbuf[];
extern Realm realms[125];
extern unsigned char caretcols[];
extern char mapstate[100];
extern unsigned char gamehour;
extern DWORD goodsprice;
extern DWORD offerprice;
extern DWORD goodsbase;
extern DWORD offerbase;
extern char keycode;
extern char lastchar;
extern unsigned char esckey;
extern int surfslost;
extern int mouseleft;
extern int mouseright;
extern DWORD walkdue;
extern DWORD standdue;
extern unsigned short walkdelay;
extern unsigned short standdelay;
extern short sunlight;
extern short lightlvl;

extern int bedi;
extern int bedself;
extern int lyingcell;
extern int lyingmate;
extern int bothwomen;
extern short iconbody;
extern short iconi;
extern short decosits;
extern int decooff;
extern int decox;
extern int decoy;
extern int decocx;
extern int decocy;
extern int decoi;
extern int decon;
extern int decocell;
extern int decowalls;
extern unsigned long decoflags;
extern unsigned long decomark;
extern unsigned char iconmark;
extern char *iconpic;
extern char *decodst;
extern short drawdi;
extern short fxi;
extern unsigned long fxpic;
extern unsigned char decomode;
extern char *decobase;
extern char *torchpose;
extern char *lay0;
extern char *lay1;
extern char *lay2;
extern char *lay3;
extern char *bodylay;
extern char *weaponlay;
extern char *shieldlay;
extern char *sheathlay;
extern Icons icons;
extern short nicons;
extern char iconbufs[30][2304];
extern short niconbufs;
extern Unit *decounit;
extern char boxtexts[20][40];
extern short nboxtexts;
extern Box boxes[20];
extern short nboxes;
extern short boxbrief;
extern unsigned short caretx;
extern unsigned short carety;
extern unsigned short caretstep;
extern DWORD carettime;
extern int menuframe;
extern short screenposx;
extern short screenposy;
extern int ndecobufs;
// PORT: whether light reaches the wide view's corners
#ifdef PORT
extern unsigned char widelit[4];
#endif
extern int ndecos;
extern int decoat;
extern short clearmsgi;
extern short fidgetr;
extern int fieldr;
extern int pathr;
extern int groundr;
extern int waterr;
extern int dmapx;
extern int dmapy;
extern int dmapx0;
extern int dmapy0;
extern int dmapx1;
extern int dmapy1;
extern int dmapcell;
extern int dmaptile;
extern int dmapterr;
extern int dmapoff;
extern unsigned char dmapcol;
extern int turnx;
extern int turny;
extern unsigned partpic;
extern unsigned long partlen;
extern int partdoff;
extern int partsoff;
extern unsigned partnpics;
extern char *partsrc;
extern char *partdst;
extern char *partp;
extern char partkind;
extern char partflag;
extern unsigned dyei;
extern unsigned layi;
extern unsigned short recipei;
extern unsigned short recipepart;
extern char *recipedst;
extern unsigned short recipen;
extern unsigned short slotloopi;
extern int dressbody;
extern int dressfeet;
extern unsigned long dresslegs;
extern unsigned long dressarmor;
extern unsigned long dresscloak;
extern unsigned long dresshelm;
extern int dressextra;
extern int dressbrace;
extern int dressring;
extern unsigned char alignpi;
extern unsigned char isopenpi;
extern unsigned char openpi;
extern short movemax;
extern unsigned char raisepi;
extern unsigned char closepi;
extern unsigned char itempi;
extern unsigned char newstore;
extern unsigned char storeslot;
extern int gaugeempty;
extern int gaugefull;
extern char *gaugesrc;
extern int rowsfull;
extern char *rowssrc;
extern int savedstates;
extern int stateset;
extern short dollhealth;
extern unsigned long dollweapon;
extern unsigned long weightv;
extern unsigned short weightint;
extern unsigned short weightfrac;
extern char weightch;
extern unsigned long valuev;
extern short weapmin;
extern short weapmax;
extern short weapgear;
extern short armprot;
extern short armgear;
extern short foodtexth;
extern short burnhours;
extern short fiti;
extern short fitln;
extern short fitdim;
extern short fith;
extern short fitw;
extern short fity;
extern short fitmaxw;
extern short dboxi;
extern short dboxln;
extern short dboxx;
extern short dboxy;
extern unsigned char desccls;
extern char *descp;
extern char *descline;
extern short descbless;
extern short descln;
extern short roundrem;
extern unsigned long *storeitem;
extern unsigned short *storepos;
extern Panel *storepan;
extern short storei;
extern int storex;
extern int storey;
extern char *invtext;
extern Unit *invunit;
extern Panel *invpan;
extern int invx;
extern int invy;
extern short invday;
extern unsigned char readk;
extern unsigned char pollk;
extern short savedlife;
extern short savedfaceno;
extern short oldblink;
extern char savedtalk;
extern char savedtrade;
extern unsigned short savedcur;
extern short litnw;
extern short litne;
extern short litsw;
extern short litse;
extern short handframe;
extern short headframe;
extern short footframe;
extern unsigned long blinkclock;
extern short heroblink;
extern short curblink;
extern unsigned long heroblinkat;
extern unsigned long curblinkat;
extern unsigned short tipx;
extern unsigned short tipy;
extern char tiptext[200];
extern unsigned tipend;
extern short headanim;
extern short headstep;
extern short handanim;
extern short handstep;
extern short footanim;
extern short footstep;
extern unsigned long worntype;
extern unsigned long wornwear;
extern int wornpic;
extern int wornpic2;
extern short wornx;
extern short worny;
extern short wornw;
extern short wornh;
extern int bloodtill;
extern Panel panels[16];
extern int savedinvy;
extern char gauges[9][2304];

void
cleartip(void)
{
	tipx = 0;
	tipy = 0;
}

int
tipshown(void)
{
	return tipx + tipy * 640;
}

void
cleartexts(void)
{
	cleartip();
	if (playerdead())
		return;
	releaseheld();
again:
	for (clearmsgi = 0; clearmsgi < nmsgs; clearmsgi++)
		if (!msgkeep[clearmsgi]) {
			delmsg(clearmsgi);
			goto again;
		}
}

void
placetip(int x, int y)
{
	// PORT: only a name, in showlabel, follows the pointer on the wide screen
#ifdef PORT
	tipshift = 0;
#endif
	tipx = x;
	tipy = y;
	tipend = 0;
	while (tiptext[tipend])
		tipend++;
	if (tipend < 4)
		tipend = 4;
	tipend += gametime;
}

void
showtip(int x, int y, char *fmt, ...)
{
	va_list ap;

	if (tipclears)
		tipclear();
	va_start(ap, fmt);
	wvsprintf(tiptext, fmt, ap);
	va_end(ap);
	text3w(tiptext);
	x -= text3width / 2;
	if (x < 8)
		x = 8;
	else if (x + text3width >= 640)
		x = 0x278 - text3width;
	y -= 0x1e;
	if (y < 0)
		y = 0;
	else if (y >= 0x1c2)
		y = 0x1c1;
	placetip(x, y);
}

void
holdtip(void)
{
	tipend = gametime + 0xff;
}

void
anchortip(unsigned char kind, int x, int y)
{
	if (kind == 1) {
		tipanchorx = (x - 8) / 48 * 48 + 0x1c;
		if (y > 0x18) {
			tipanchory = (y - 0x18) / 48;
			tipanchory *= 48;
			tipanchory += 0x1c;
		} else
			tipanchory = 0;
	} else if ((kind == 0xd9 || kind == 0xda) && hovercx >= 0 && hovercx <= 0x17) {
		if (kind == 0xd9) {
			tipanchorx = hovercx % 3 * 52 + 0x34;
			tipanchory = hovercx / 3 * 52 + 0x31;
		} else {
			tipanchorx = hovercx % 3 * 52 + 0x100;
			tipanchory = hovercx / 3 * 52 + 0x1b;
		}
	} else {
		tipanchorx = x;
		tipanchory = y - 4;
		if (tipanchory < 0)
			tipanchory = 0;
	}
}

void
resetanims(void)
{
	headframe = 0;
	headanim = 0;
	headstep = 0;
	handframe = 0;
	handanim = 0;
	handstep = 0;
	footframe = 0;
	footanim = 0;
	footstep = 0;
	heroblink = 0;
	heroblinkat = blinkclock + heroblinkmin + randn(heroblinkmax - heroblinkmin);
	curblink = 0;
	curblinkat = blinkclock + curblinkmin + randn(curblinkmax - curblinkmin);
}

void
stepanims(void)
{
	if (playmode != 1 && !trademode) {
		resetanims();
		return;
	}
	blinkclock++;
	if (talkactive || lastspeech) {
		if (heroblinkat < blinkclock) {
			if (units[0].health <= 0)
				heroblink = 0;
			else
				heroblink = heroblink ? 0 : 1;
			if (heroblink && faceflags[units[0].face] & 2 || !heroblink && faceflags[units[0].face] & 1)
				heroblinkat = blinkclock + heroblinkmin / 2 + randn(heroblinkmax / 2 - heroblinkmin / 2);
			else
				heroblinkat = blinkclock + heroblinkmin + randn(heroblinkmax - heroblinkmin);
		}
		if (curunit && curblinkat < blinkclock) {
			if (units[curunit].health <= 0)
				curblink = 0;
			else
				curblink = curblink ? 0 : 1;
			if (curblink && faceflags[units[curunit].face] & 2 || !curblink && faceflags[units[curunit].face] & 1)
				curblinkat = blinkclock + curblinkmin / 2 + randn(curblinkmax / 2 - curblinkmin / 2);
			else
				curblinkat = blinkclock + curblinkmin + randn(curblinkmax - curblinkmin);
		}
	}
	if (headanim) {
		headframe = headanims[headanim][headstep++];
		if (headframe == 0xff) {
			headframe = 0;
			headstep = 12;
		}
		if (headstep >= 12) {
			headanim = 0;
			headstep = 0;
		}
	} else
		headframe = 0;
	if (handanim) {
		handframe = handanims[handanim][handstep++];
		if (handframe == 0xff) {
			handframe = 0;
			handstep = 12;
		}
		if (handstep >= 12) {
			handanim = 0;
			handstep = 0;
		}
	} else
		handframe = 0;
	if (footanim) {
		footframe = footanims[footanim][footstep++];
		if (footframe == 0xff) {
			footframe = 0;
			footstep = 12;
		}
		if (footstep >= 12) {
			footanim = 0;
			footstep = 0;
		}
	} else
		footframe = 0;
}

void
animhead(short n)
{
	headstep = 0;
	headanim = n;
	headframe = headanims[n][0];
}

void
animhand(short n)
{
	handstep = 0;
	handanim = n;
	handframe = handanims[n][0];
}

void
animfoot(short n)
{
	footstep = 0;
	footanim = n;
	footframe = footanims[n][0];
}

void
fidget(void)
{
	if (units[0].health <= 0 || headanim || handanim || footanim)
		return;
	if (randn(100) > 15)
		return;
	fidgetr = randn(100);
	if (fidgetr < 33) {
		fidgetr = randn(100);
		if (fidgetr < 33)
			animhead(2);
		else if (fidgetr < 66)
			animhead(3);
		else
			animhead(4);
	} else if (fidgetr < 66) {
		fidgetr = randn(100);
		if (fidgetr < 25)
			animhand(1);
		else if (fidgetr < 50)
			animhand(2);
		else if (fidgetr < 75)
			animhand(3);
		else
			animhand(4);
	} else {
		fidgetr = randn(100);
		if (fidgetr < 50)
			animfoot(1);
		else
			animfoot(2);
	}
}

int
fieldcolor(void)
{
	fieldr = randn(99);
	if (fieldr < 33)
		return 0x48;
	if (fieldr < 66)
		return 0xc7;
	return 0xc8;
}

int
pathcolor(void)
{
	pathr = randn(99);
	if (pathr < 50)
		return 0xc4;
	return 0xc5;
}

int
groundcolor(void)
{
	groundr = randn(99);
	if (groundr < 33)
		return 0x45;
	if (groundr < 66)
		return 0x46;
	return 0x73;
}

int
watercolor(void)
{
	waterr = randn(99);
	if (waterr < 33)
		return 0x64;
	if (waterr < 66)
		return 0x65;
	return 0x74;
}

// PORT: C leaves the order of a call's arguments open. This draws the four colors from the last to the first, as Watcom compiled drawmap's calls
#ifdef PORT
static void
setscr4by(int off, int (*color)(void))
{
	int a, b, c, d;

	d = color();
	c = color();
	b = color();
	a = color();
	setscr4(off, a, b, c, d);
}
#endif

/* draws the minimap at x, y */
void
drawmap(int x, int y, short mode, short showunits)
{
	if (mode && mode != 0xff && mode != 0xfe) {
		dmapx0 = 6;
		dmapy0 = 5;
		dmapx1 = 0x8f;
		dmapy1 = 0x90;
	} else {
		dmapx0 = 0;
		dmapy0 = 0;
		dmapx1 = 0x95;
		dmapy1 = 0x95;
	}
	clearscr();
	if (mode == 0xff) {
		for (dmapy = dmapy0; dmapy <= dmapy1; dmapy++) {
			dmapoff = dmapx0 * 2 + x + (dmapy * 2 + y) * 640;
			for (dmapx = dmapx0; dmapx <= dmapx1; dmapx++, dmapoff += 2) {
				dmapcol = blocks[dmapy / 5][dmapx / 5];
				if (dmapcol & 0x40)
					goto red;
				else if (dmapcol & 0x80)
					setscr2o(dmapoff, 0x5b);
				else if (dmapcol & 0x20)
					setscr2o(dmapoff, 0x91);
				else {
					dmapcol &= 0x1f;
					switch (dmapcol) {
					case 1:
						setscr2o(dmapoff, 0x90);
						break;
					case 2:
					case 3:
						setscr2o(dmapoff, 7);
						break;
					case 4:
						setscr2o(dmapoff, 0xde);
						break;
					case 6:
						setscr2o(dmapoff, 0x79);
						break;
					case 5:
						setscr2o(dmapoff, 0xb1);
						break;
					case 7:
						break;
					case 8:
					red:
						setscr2o(dmapoff, 0x7f);
						break;
					}
				}
			}
		}
	} else {
		for (dmapy = dmapy0; dmapy <= dmapy1; dmapy++) {
			dmapoff = dmapx0 * 2 + x + (dmapy * 2 + y) * 640;
			for (dmapx = dmapx0; dmapx <= dmapx1; dmapoff += 2, dmapx++) {
				dmapcell = dmapy * 150 + dmapx;
				dmaptile = map[dmapcell];
				if (mode && (mapflags[dmapcell] & 0x200) == 0)
					continue;
				dmapterr = terrains[dmaptile & 0xfff];
				switch (dmapterr & 0x1f) {
				case 9:
					// PORT: C leaves the order of a call's arguments open. Draw them from the last to the first, as Watcom compiled them
#ifdef PORT
					setscr4by(dmapoff, watercolor);
#else
					setscr4(dmapoff, watercolor(), watercolor(), watercolor(), watercolor());
#endif
					continue;
				case 8:
					dmapterr = randn(99);
					if (dmapterr < 50)
						setscr2o(dmapoff, 0x76);
					else
						setscr2o(dmapoff, 0x77);
					continue;
				case 4:
					// PORT: as above
#ifdef PORT
					setscr4by(dmapoff, fieldcolor);
#else
					setscr4(dmapoff, fieldcolor(), fieldcolor(), fieldcolor(), fieldcolor());
#endif
					continue;
				}
				if (!(dmaptile & 0x4000)) {
					if (dmaptile & 0x8000)
						setscr2o(dmapoff, 0x4a);
					else
						setscr2o(dmapoff, 0x4b);
				} else if (mapflags[dmapcell] & 0x400)
					setscr2o(dmapoff, 0x49);
				else if (dmaptile & 0x1000) {
					if (!(map[dmapcell] & 0x2000) && mapflags[dmapcell] & 0x80) {
						switch (dmapterr & 0x1f) {
						case 0:
						case 1:
						case 2:
						case 3:
						case 7:
							dmapterr = randn(99);
							if (dmapterr < 50)
								setscr2o(dmapoff, 0x80);
							else
								setscr2o(dmapoff, 0x81);
							continue;
						}
					}
					setscr2o(dmapoff, 0x48);
				} else {
					dmapterr = terrains[dmaptile & 0xfff];
					switch (dmapterr & 0x1f) {
					case 2:
					case 3:
						// PORT: as above
#ifdef PORT
						setscr4by(dmapoff, pathcolor);
#else
						setscr4(dmapoff, pathcolor(), pathcolor(), pathcolor(), pathcolor());
#endif
						continue;
					default:
						if (debugmap || cheat && keys[VK_SHIFT] & 0x80 && keys['W'] & 0x80)
							setscr2o(dmapoff, 0x46);
						else
							// PORT: as above
#ifdef PORT
							setscr4by(dmapoff, groundcolor);
#else
							setscr4(dmapoff, groundcolor(), groundcolor(), groundcolor(), groundcolor());
#endif
						continue;
					}
				}
			}
		}
		for (dmapterr = 1; dmapterr < nsites; dmapterr++)
			for (dmaptile = 0; dmaptile < provs[dmapterr].n; dmaptile++) {
				dmapy = provs[dmapterr].ys[dmaptile];
				dmapx = provs[dmapterr].xs[dmaptile];
				if (!mode || mapflags[dmapy * 150 + dmapx] & 0x200) {
					dmapoff = dmapx * 2 + x + (dmapy * 2 + y) * 640;
					setscr2o(dmapoff, 0x47);
					if (provs[dmapterr].kinds[dmaptile] == 0xa1) {
						setscr2o(dmapoff - 0x502, 0x47);
						setscr2o(dmapoff - 0x500, 0x47);
						setscr2o(dmapoff + 2, 0x47);
						setscr2o(dmapoff - 0x4fe, 0x47);
					} else {
						setscr2o(dmapoff - 0x502, 0x47);
						setscr2o(dmapoff + 0x500, 0x47);
						setscr2o(dmapoff - 2, 0x47);
						setscr2o(dmapoff + 0x4fe, 0x47);
					}
				}
			}
		if (mode == 0xfe) {
			for (dmapy = dmapy0; dmapy <= dmapy1; dmapy++) {
				dmapoff = dmapx0 * 2 + x + (dmapy * 2 + y) * 640;
				for (dmapx = dmapx0; dmapx <= dmapx1; dmapx++, dmapoff += 2) {
					dmapcol = provmap[dmapy * 150 + dmapx];
					if (dmapcol)
						setscr2o(dmapoff, dmapcol);
				}
			}
			for (dmapy = 1; dmapy < nsites; dmapy++) {
				setscr2(provs[dmapy].x0 * 2 + x, provs[dmapy].y0 * 2 + y, 0x3a);
				setscr2(provs[dmapy].x0 * 2 + x, provs[dmapy].y1 * 2 + y, 0x3a);
				setscr2(provs[dmapy].x1 * 2 + x, provs[dmapy].y0 * 2 + y, 0x3a);
				setscr2(provs[dmapy].x1 * 2 + x, provs[dmapy].y1 * 2 + y, 0x3a);
				setscr2(provs[dmapy].cx * 2 + x, provs[dmapy].cy * 2 + y, 0x3a);
				for (dmapx = 0; dmapx < provs[dmapy].n; dmapx++)
					setscr2(provs[dmapy].xs[dmapx] * 2 + x, provs[dmapy].ys[dmapx] * 2 + y, 0x30);
			}
		}
		for (dmapx = 0; dmapx < 30; dmapx++)
			if (dots[dmapx]) {
				dmapcell = dots[0x1e + dmapx];
				dmapcell *= 150;
				dmapcell += dots[dmapx];
				if (!mode) {
					if (dots[0x3c + dmapx] & 1)
						dmapcol = 0x96;
					else
						dmapcol = 0x98;
					setscr2(dots[dmapx] * 2 + x, dots[0x1e + dmapx] * 2 + y, dmapcol);
				} else if (!((mapflags[dmapcell] & 0x200) == 0 || findclass(dmapcell, 0x69))) {
					if (dots[0x3c + dmapx] & 1)
						dmapcol = 0x70;
					else
						dmapcol = 0x51;
					setscr2(dots[dmapx] * 2 + x, dots[0x1e + dmapx] * 2 + y, dmapcol);
				}
			}
		if (curmap == 1 || curmap == 5) {
			dmapcol = 0x51;
			for (dmapx = 0; dmapx < game.ncaves; dmapx++) {
				dmapcell = game.cavex[dmapx] + game.cavey[dmapx] * 150;
				if (mapflags[dmapcell] & 0x200 || !mode)
					setscr2(game.cavex[dmapx] * 2 + x, game.cavey[dmapx] * 2 + y, dmapcol);
			}
		} else if (curmap == 3) {
			dmapcol = 0x70;
			for (dmapx = 0; dmapx < game.ncaves; dmapx++) {
				dmapcell = caves[game.cavekind[dmapx]][0] + caves[game.cavekind[dmapx]][1] * 150;
				if (mapflags[dmapcell] & 0x200 || !mode)
					setscr2(caves[game.cavekind[dmapx]][0] * 2 + x, caves[game.cavekind[dmapx]][1] * 2 + y, dmapcol);
			}
		}
		if (showunits) {
			for (dmapx = 1; dmapx < nunits; dmapx++) {
				if (units[dmapx].health <= 0)
					continue;
				dmapy = (units[dmapx].y * 2 + y) * 640 + x + units[dmapx].x * 2;
				if (mode || units[dmapx].sympathy > 0)
					setscr2o(dmapy, 0x3a);
				else
					setscr2o(dmapy, 0x38);
			}
		}
		if (cheat && keys['W'] & 0x80) {
			setscr2(zones[mapzone(curmap)].inx * 2 + x, y + zones[mapzone(curmap)].iny * 2, 0x14);
			setscr2(zones[mapzone(curmap)].x * 2 + x, zones[mapzone(curmap)].y * 2 + y, 0x16);
		}
	}
	dmapx = playerx * 2 + x + (y + playery * 2) * 640;
	if (debugmap == 0 && (!cheat || !(keys[VK_SHIFT] & 0x80) || !(keys['W'] & 0x80))) {
		setscr2o(dmapx - 0x500, 0x30);
		setscr2o(dmapx - 2, 0x30);
		setscr2o(dmapx, 0x30);
		setscr2o(dmapx + 2, 0x30);
		setscr2o(dmapx + 0x500, 0x30);
	} else
		setscr2o(dmapx, 0x30);
	showscrt();
}

/* turns a 48 by 48 picture over its diagonal */
void
turn48(char *dst, char *src)
{
	for (turny = 0; turny < 48; turny++)
		for (turnx = 0; turnx < 48; turnx++)
			*(dst + turnx * 48 + turny) = *(src + turnx + turny * 48);
}

/* loads picture set n into the pictures at dst, turning the mirrored ones */
void
loadpart(char *dst, unsigned n)
{
	partkind = partkinds[n][0];
	partflag = partkinds[n][1];
	if (partkind == 0x2f || partkind == 0x20) {
		memset(dst, 0, setpics * 0x900);
		return;
	}
	// PORT: partpics for Part's pics
#ifdef PORT
	if (partpics[n] == NULL) {
#else
	if (partdefs[n].pics == NULL) {
#endif
		if (!seekentry(n + 0x1398)) {
			memset(dst, 0, setpics * 0x900);
			return;
		}
		partlen = storedn;
		fileread(picbuf, partlen, 1, datfh);
		partlen = unpack(packbuf, 0x96640, picbuf, partlen);
	} else {
		partlen = partdefs[n].size;
		// PORT: partpics for Part's pics
#ifdef PORT
		memcpy(packbuf, partpics[n], partlen);
#else
		memcpy(packbuf, partdefs[n].pics, partlen);
#endif
	}
	partp = packbuf;
	if (minsets)
		partnpics = 1;
	else
		partnpics = 7;
	if (partkind != 0x78) {
		memcpy(dst + dir0frm0 * 0x900, partp, partnpics * 0x900);
		partp += 0x3f00;
		memcpy(dst + dir1frm0 * 0x900, partp, partnpics * 0x900);
		partp += 0x3f00;
		memcpy(dst + dir2frm0 * 0x900, partp, partnpics * 0x900);
		partp += 0x3f00;
		memcpy(dst + dir3frm0 * 0x900, partp, partnpics * 0x900);
		partp += 0x3f00;
	} else {
		memcpy(dst + dir0frm0 * 0x900, partp, partnpics * 0x900);
		partp += 0x3f00;
		memcpy(dst + dir2frm0 * 0x900, partp, partnpics * 0x900);
		partp += 0x3f00;
		for (partlen = 0; partlen < 2; partlen++)
			for (partpic = 0; partpic < partnpics; partpic++) {
				if (partlen != 0) {
					partdoff = dir3frm0;
					partsoff = dir2frm0;
				} else {
					partdoff = dir1frm0;
					partsoff = dir0frm0;
				}
				partdoff += partpic;
				partdoff *= 0x900;
				partsoff += partpic;
				partsoff *= 0x900;
				partdst = dst + partdoff;
				partsrc = dst + partsoff;
				turn48(partdst, partsrc);
			}
	}
	if (minsets == 0) {
		memcpy(dst + dir0frm7 * 0x900, partp, 0x900);
		partp += 0x900;
		memcpy(dst + dir1frm7 * 0x900, partp, 0x900);
		partp += 0x900;
		memcpy(dst + dir2frm7 * 0x900, partp, 0x900);
		partp += 0x900;
		memcpy(dst + dir3frm7 * 0x900, partp, 0x900);
		partp += 0x900;
		if (nopvari == 0) {
			if (partkind != 0x78) {
				memcpy(dst + 0x20 * 0x900, partp, 0x20 * 0x900);
				partp += 0x20 * 0x900;
			} else {
				memcpy(dst + 0x20 * 0x900, partp, 4 * 0x900);
				partp += 4 * 0x900;
				for (partpic = 0; partpic < 4; partpic++)
					turn48(dst + (partpic + 0x24) * 0x900, dst + (partpic + 0x20) * 0x900);
				memcpy(dst + 0x28 * 0x900, partp, 4 * 0x900);
				partp += 4 * 0x900;
				for (partpic = 0; partpic < 4; partpic++)
					turn48(dst + (partpic + 0x2c) * 0x900, dst + (partpic + 0x28) * 0x900);
				memcpy(dst + 0x30 * 0x900, partp, 8 * 0x900);
				partp += 8 * 0x900;
				memcpy(dst + 0x38 * 0x900, partp, 4 * 0x900);
				partp += 4 * 0x900;
				for (partpic = 0; partpic < 4; partpic++)
					turn48(dst + (partpic + 0x3c) * 0x900, dst + (partpic + 0x38) * 0x900);
			}
			if (noprun == 0) {
				memcpy(dst + 0x40 * 0x900, partp, 3 * 0x900);
				partp += 3 * 0x900;
				if (partkind != 0x78) {
					memcpy(dst + 0x43 * 0x900, partp, 3 * 0x900);
					partp += 3 * 0x900;
				} else {
					turn48(dst + 0x43 * 0x900, dst + 0x40 * 0x900);
					turn48(dst + 0x44 * 0x900, dst + 0x41 * 0x900);
					turn48(dst + 0x45 * 0x900, dst + 0x42 * 0x900);
				}
				memcpy(dst + 0x46 * 0x900, partp, 3 * 0x900);
				partp += 3 * 0x900;
				if (partkind == 0x78) {
					turn48(dst + 0x49 * 0x900, dst + 0x46 * 0x900);
					turn48(dst + 0x4a * 0x900, dst + 0x47 * 0x900);
					turn48(dst + 0x4b * 0x900, dst + 0x48 * 0x900);
				} else {
					memcpy(dst + 0x49 * 0x900, partp, 3 * 0x900);
					partp += 3 * 0x900;
				}
			}
		}
	}
}

unsigned char
rampcolor(unsigned char c)
{
	if (c == 0x1f || c == 0x2f)
		return 0xf;
	return c;
}

/* recolors the picture set at p: flags pick the ranges 0xf0 (to a), 0xe0 (to b), 0xc0 (to c) and 0xd0 (to d) */
void
dyeset(char *p, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char flags)
{
	if (flags == 0)
		return;
	for (dyei = 0; dyei < setpics * 0x900; dyei++, p++) {
		if (*p >= 0xf0) {
			if (flags & 1)
				*p = rampcolor(*p - 0xf0 + a);
		} else if (*p >= 0xe0 && *p <= 0xef) {
			if (flags & 2)
				*p = rampcolor(*p - 0xe0 + b);
		} else if (flags & 4 && flags & 8) {
			if (*p >= 0xc0 && *p <= 0xcf)
				*p = rampcolor(*p - 0xc0 + c);
			else if (*p >= 0xd0 && *p <= 0xdf)
				*p = rampcolor(*p - 0xd0 + d);
		} else if (*p >= 0xc0 && *p <= 0xdf) {
			if (flags & 4)
				*p = rampcolor(*p - 0xc0 + c);
			else if (flags & 8)
				*p = rampcolor(*p - 0xc0 + d);
		}
	}
}

/* copies the picture set at src over dst, but for its transparent bytes */
void
layset(char *dst, char *src)
{
	for (layi = 0; layi < setpics * 0x900; dst++, src++, layi++)
		if (*src != 0)
			*dst = *src;
}

void
tintsilver(char *p, int n)
{
	while (n) {
		n--;
		switch (*p) {
		case 0x15:
			*p = 0x54;
			break;
		case 0x16:
			*p = 0x56;
			break;
		case 0x17:
			*p = 0x57;
			break;
		}
		p++;
	}
}

void
tintbronze(char *p, int n)
{
	while (n) {
		n--;
		switch (*p) {
		case 0x15:
			*p = 0x46;
			break;
		case 0x16:
			*p = 0x48;
			break;
		case 0x17:
			*p = 0x49;
			break;
		}
		p++;
	}
}

/* loads the picture set of part n, recolors it, and lays it over dst */
void
addpart(unsigned char r, unsigned char slot, char *dst, unsigned short n, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	loadpart(scrbuf, n);
	dyeset(scrbuf, a, b, c, d, partdefs[n].recolor);
	layset(dst, scrbuf);
}

/* builds the pictures of recipe r at tilecache + off from its parts */
void
buildrecipe(unsigned r, int off)
{
	recipedst = ((char *)tilecache) + off;
	memset(recipedst, 0, setpics * 0x900);
	for (recipei = 0; recipei < 5; recipei++) {
		recipepart = ((Recipe *)(recipes + 0x20))[r].parts[recipei];
		if (recipepart == 0x1d || recipepart == 0x2a || recipepart == 0x2b
		|| recipepart == 0x2c || recipepart == 0x2d || recipepart == 0x38
		|| recipepart == 0x39 || recipepart == 0x3a || recipepart == 0x3b
		|| recipepart == 0x3c || recipepart == 0x27 || recipepart == 0x28
		|| recipepart == 0x29 || recipepart == 0x2e)
			continue;
		if (hasgearbank == 1) {
			if (recipepart == 0x1e || recipepart == 0x1f || recipepart == 0x20
			|| recipepart == 0x21 || recipepart == 0x22 || recipepart == 0x3f
			|| recipepart == 0x40) {
				recipepart = 0x35;
				goto draw;
			}
			if (recipepart == 0x1c || recipepart == 0x1a || recipepart == 0x46
			|| recipepart == 0x47 || recipepart == 0x37 || recipepart == 0x48
			|| recipepart == 0x2f || recipepart == 0x30 || recipepart == 0x31
			|| recipepart == 0x32 || recipepart == 0x3d)
				continue;
		}
		if (!(recipepart || recipei == 0))
			continue;
	draw:
		addpart(r, recipei, recipedst, recipepart, ((Recipe *)(recipes + 0x20))[r].colors[0],
			((Recipe *)(recipes + 0x20))[r].colors[1], ((Recipe *)(recipes + 0x20))[r].colors[2], ((Recipe *)(recipes + 0x20))[r].colors[3]);
	}
}

void
buildrecipes(void)
{
	for (recipen = 0; recipen < 22; recipen++)
		buildrecipe(recipen, setpics * (recipen + 15) * 0x900);
}

/* loads the picture sets of the parts into the banks tilecache and gearbank */
void
loadgear(void)
{
	loadpart(((char *)tilecache) + setpics * 0x900, 0x2a);
	loadpart(((char *)tilecache) + setpics * 0x1200, 0x2b);
	loadpart(((char *)tilecache) + setpics * 0x1b00, 0x2c);
	loadpart(((char *)tilecache) + setpics * 0x2400, 0x2d);
	loadpart(((char *)tilecache) + setpics * 0x2d00, 0x1d);
	loadpart(((char *)tilecache) + setpics * 0x3600, 0x27);
	loadpart(((char *)tilecache) + setpics * 0x3f00, 0x28);
	loadpart(((char *)tilecache) + setpics * 0x4800, 0x29);
	loadpart(((char *)tilecache) + setpics * 0x5100, 0x2e);
	loadpart(((char *)tilecache) + setpics * 0x5a00, 0x38);
	loadpart(((char *)tilecache) + setpics * 0x6300, 0x39);
	loadpart(((char *)tilecache) + setpics * 0x6c00, 0x3a);
	loadpart(((char *)tilecache) + setpics * 0x7500, 0x3b);
	loadpart(((char *)tilecache) + setpics * 0x7e00, 0x3c);
	loadpart(grassmask, 0x45);
	if (hasgearbank) {
		loadpart(((char *)gearbank), 0x1c);
		loadpart(((char *)gearbank) + setpics * 0x900, 0x1a);
		loadpart(((char *)gearbank) + setpics * 0x1b00, 0x1e);
		loadpart(((char *)gearbank) + setpics * 0x2400, 0x1f);
		loadpart(((char *)gearbank) + setpics * 0x2d00, 0x20);
		loadpart(((char *)gearbank) + setpics * 0x3600, 0x21);
		loadpart(((char *)gearbank) + setpics * 0x3f00, 0x22);
		loadpart(((char *)gearbank) + setpics * 0x4800, 0x3f);
		loadpart(((char *)gearbank) + setpics * 0x5100, 0x40);
		loadpart(((char *)gearbank) + setpics * 0x5a00, 0x2f);
		loadpart(((char *)gearbank) + setpics * 0x6300, 0x30);
		loadpart(((char *)gearbank) + setpics * 0x6c00, 0x31);
		loadpart(((char *)gearbank) + setpics * 0x7500, 0x32);
		loadpart(((char *)gearbank) + setpics * 0x7e00, 0x3d);
		loadpart(((char *)gearbank) + setpics * 0x8700, 0x46);
		loadpart(((char *)gearbank) + setpics * 0x9000, 0x47);
		loadpart(((char *)gearbank) + setpics * 0x1200, 0x37);
		loadpart(((char *)gearbank) + setpics * 0x9900, 0x48);
	}
}

void
slotloops(void)
{
	for (slotloopi = 15; slotloopi < 37; slotloopi++)
		;
	if (hasgearbank)
		for (slotloopi = 15; slotloopi < 37; slotloopi++)
			;
}

void
resetdress(void)
{
	builtcolors[0] = 0xff;
	builtarmor = 0xff;
}

/* builds the pictures of the hero from his outfit, unless it is the one built last */
void
dresshero(void)
{
	dressbody = 0;
	game.outfittext = 0x84d;
	if (units[0].feet == 0)
		dressfeet = 0x36;
	else
		dressfeet = 0;
	dresslegs = baselook(units[0].legs & 0xfff);
	switch (dresslegs) {
	case 0x1a1:
	case 0x1a2:
	case 0x1a3:
		dresslegs = 0x24;
		break;
	case 0:
		if (censorship == 2 || censorship == 4)
			dresslegs = 0x4d;
		else
			dresslegs = 0x25;
		if ((units[0].body & 0xfff) == 0)
			game.outfittext = 0x84e;
		break;
	default:
		dresslegs = 0;
		break;
	}
	dressarmor = baselook(units[0].body & 0xfff);
	switch (dressarmor) {
	case 0x1ad:
	case 0x1b5:
		dressarmor = 0x23;
		game.outfittext = 0x82e;
		break;
	case 0x1ae:
		dressarmor = 0x1e;
		game.outfittext = partdefs[0x1e].text;
		break;
	case 0x1af:
		dressarmor = 0x1f;
		game.outfittext = partdefs[0x1f].text;
		break;
	case 0x1b0:
		dressarmor = 0x20;
		game.outfittext = partdefs[0x20].text;
		break;
	case 0x1b1:
		dressarmor = 0x21;
		game.outfittext = partdefs[0x21].text;
		break;
	case 0x1b2:
		dressarmor = 0x22;
		game.outfittext = partdefs[0x22].text;
		break;
	case 0x1b3:
		dressarmor = 0x40;
		game.outfittext = partdefs[0x40].text;
		break;
	case 0x1b4:
		dressarmor = 0x3f;
		game.outfittext = partdefs[0x3f].text;
		break;
	default:
		dressarmor = 0;
		break;
	}
	dresscloak = baselook(units[0].cloak & 0xfff);
	switch (dresscloak) {
	case 0x1b9:
		dresscloak = 0x1c;
		break;
	case 0x1c6:
		dresscloak = 0x1a;
		break;
	default:
		dresscloak = 0;
		break;
	}
	if (units[0].wrist)
		dressbrace = 0x46;
	else
		dressbrace = 0;
	if (units[0].neck)
		dressring = 0x47;
	else
		dressring = 0;
	dresshelm = baselook(units[0].head & 0xfff);
	if (dresshelm)
		switch (dresshelm) {
		case 0x1ba:
			dresshelm = 0x2f;
			break;
		case 0x1bb:
		case 0x1bc:
			dresshelm = 0x30;
			break;
		case 0x1bd:
			dresshelm = 0x31;
			break;
		case 0x1bf:
			dresshelm = 0x3d;
			break;
		default:
			dresshelm = 0x32;
			break;
		}
	dressextra = 0;
	if (game.colors[0] == builtcolors[0] && game.colors[1] == builtcolors[1]
	&& game.colors[2] == builtcolors[2] && game.colors[3] == builtcolors[3]
	&& dressbody == builtbody && dressfeet == builtfeet
	&& dresslegs == builtlegs && dressarmor == builtarmor
	&& dresscloak == builtcloak && (units[0].wrist & 0xfff) == builtbrace
	&& (units[0].neck & 0xfff) == builtring && dresshelm == builthelm
	&& dressextra == builtextra)
		return;
	builtcolors[0] = game.colors[0];
	builtcolors[1] = game.colors[1];
	builtcolors[2] = game.colors[2];
	builtcolors[3] = game.colors[3];
	builtbody = dressbody;
	builtfeet = dressfeet;
	builtlegs = dresslegs;
	builtarmor = dressarmor;
	builtcloak = dresscloak;
	builtbrace = units[0].wrist & 0xfff;
	builtring = units[0].neck & 0xfff;
	builthelm = dresshelm;
	builtextra = dressextra;
	memset(((char *)tilecache), 0, setpics * 0x900);
	addpart(0x16, 0, ((char *)tilecache), dressbody, game.colors[0], game.colors[1], game.colors[2], game.colors[3]);
	if (dressfeet)
		addpart(0x16, 1, ((char *)tilecache), dressfeet, game.colors[0], game.colors[1], game.colors[2], game.colors[3]);
	if (dresslegs)
		addpart(0x16, 1, ((char *)tilecache), dresslegs, game.colors[0], game.colors[1], game.colors[2], game.colors[3]);
	if (dressarmor)
		addpart(0x16, 1, ((char *)tilecache), dressarmor, game.colors[0], game.colors[1], game.colors[2], game.colors[3]);
	if (dressbrace) {
		loadpart(scrbuf, dressbrace);
		dyeset(scrbuf, game.colors[0], game.colors[1], game.colors[2], game.colors[3], partdefs[dressbrace].recolor);
		if ((units[0].wrist & 0xfff) == 0x4f3) {
			tintsilver(scrbuf, setpics * 0x900);
			layset(((char *)tilecache), scrbuf);
		} else if ((units[0].wrist & 0xfff) == 0x4f2) {
			tintbronze(scrbuf, setpics * 0x900);
			layset(((char *)tilecache), scrbuf);
		} else
			layset(((char *)tilecache), scrbuf);
	}
	if (dresscloak)
		addpart(0x16, 1, ((char *)tilecache), dresscloak, game.colors[0], game.colors[1], game.colors[2], game.colors[3]);
	if (dressring) {
		loadpart(scrbuf, dressring);
		dyeset(scrbuf, game.colors[0], game.colors[1], game.colors[2], game.colors[3], partdefs[dressring].recolor);
		if ((units[0].neck & 0xfff) == 0x4f6)
			tintsilver(scrbuf, setpics * 0x900);
		else if ((units[0].neck & 0xfff) == 0x4f5)
			tintbronze(scrbuf, setpics * 0x900);
		layset(((char *)tilecache), scrbuf);
	}
	if ((game.look == 0 || game.look == 2) && hasgearbank)
		addpart(0x16, 0, ((char *)tilecache), 0x33, game.colors[0], game.colors[1], game.colors[2], game.colors[3]);
	if (dresshelm)
		addpart(0x16, 1, ((char *)tilecache), dresshelm, game.colors[0], game.colors[1], game.colors[2], game.colors[3]);
	if (dressextra)
		addpart(0x16, 1, ((char *)tilecache), dressextra, game.colors[0], game.colors[1], game.colors[2], game.colors[3]);
}

void
alignpanels(void)
{
	for (alignpi = 0; alignpi < 16; alignpi++)
		panels[alignpi].x &= ~3;
}

/* reports whether window id is open */
int
isopen(unsigned char id)
{
	for (isopenpi = 0; isopenpi < 16; isopenpi++)
		if (panelstack[isopenpi] == id)
			return 1;
	return 0;
}

int
noneopen(void)
{
	if (panelstack[0] == 0xff || panelstack[0] == 0 && panelstack[1] == 0xff)
		return 1;
	return 0;
}

/* opens window id on top of the others */
void
openpanel(unsigned char id)
{
	mousemoved = 1;
	if (id == 1)
		panels[1].arg = 1;
	if (isopen(id))
		raisepanel(id);
	else {
		for (openpi = 15; openpi >= 1; openpi--)
			panelstack[openpi] = panelstack[openpi - 1];
		panelstack[0] = id;
	}
}

// PORT: where the wide screen shows the panels and the HUD
#ifdef PORT
int paneloff[16];
int tipshift;
extern int hoverx;

/* hudon reports whether the HUD stands at the screen's edges now: talk and trade keep theirs in the frame. */
static int
hudon(void)
{
	return sysedges && !trademode && !talkactive;
}

int
hudleft(void)
{
	return hudon() ? sysscreen.left : 0;
}

int
hudright(void)
{
	return hudon() ? -sysscreen.left : 0;
}

/*
 * panelside returns the shift of the edge that a panel of classic x and
 * width w stands by. A panel whose classic x reaches the bands of the
 * gestures stands by that edge, so a pointer on it reaches them only there.
 */
static int
panelside(int id, int x, int w)
{
	if (x < Leftstrip)
		return sysscreen.left;
	if (x + w > 640 - Rightstrip)
		return -sysscreen.left;
	if (id == 1 || id == 3)
		return -sysscreen.left;
	if (id == 2 || id >= 4 && id <= 8)
		return sysscreen.left;
	return 0;
}

/* panelshift returns what the screen adds to panel id's classic x, which keeps it on the screen. */
int
panelshift(int id)
{
	int s, x, w;

	if (!hudon() || sysscreen.left == 0)
		return 0;
	x = panels[id].x;
	w = panels[id].w;
	s = panelside(id, x, w) + paneloff[id];
	if (x + s < sysscreen.left + 8)
		s = sysscreen.left + 8 - x;
	else if (x + w + s > sysscreen.right - 8)
		s = sysscreen.right - 8 - w - x;
	return s;
}

/*
 * placepanel puts panel id at sx, y of the screen, with a classic x that
 * reaches the bands of the gestures only at a true edge, and in the
 * multiples of 4 that movepanel leaves.
 */
void
placepanel(int id, int sx, int y)
{
	int x, w;

	w = panels[id].w;
	if (sx < sysscreen.left + 8)
		sx = sysscreen.left + 8;
	else if (sx + w > sysscreen.right - 8)
		sx = sysscreen.right - 8 - w;
	if (sx < sysscreen.left + Leftstrip)
		x = sx - sysscreen.left;
	else if (sx + w > sysscreen.right - Rightstrip)
		x = sx + sysscreen.left;
	else if (sx < 32)
		x = 32;
	else if (sx > 640 - Rightstrip - w)
		x = 640 - Rightstrip - w;
	else
		x = sx;
	x &= ~3;
	if (y < 0)
		y = 0;
	else if (y > 480 - panels[id].h)
		y = 480 - panels[id].h;
	panels[id].x = x;
	panels[id].y = y;
	paneloff[id] = sx - x - panelside(id, x, w);
}

/* placeatpointer keeps panel id, which movepanel placed by hoverx, by the pointer's true place. */
void
placeatpointer(int id)
{
	paneloff[id] = 0;
	if (sysroam)
		placepanel(id, panels[id].x + hoverwx - hoverx, panels[id].y);
}
#endif

/* moves window id to x, y inside the screen */
void
movepanel(unsigned char id, short x, short y)
{
	if (x < 8)
		x = 8;
	else {
		movemax = 632 - panels[id].w;
		if (x > movemax)
			x = movemax;
	}
	x /= 4;
	x *= 4;
	if (y < 0)
		y = 0;
	else {
		movemax = 480 - panels[id].h;
		if (y > movemax)
			y = movemax;
	}
	panels[id].x = x;
	panels[id].y = y;
	mousemoved = 1;
}

void
sizepanel(int id, int w, int h, short type)
{
	panels[id].w = w;
	panels[id].h = h;
	panels[id].type = type;
	movepanel(id, panels[id].x, panels[id].y);
}

void
initpanels(void)
{
	// PORT: a load or a new game forgets the drags on the wide screen
#ifdef PORT
	memset(paneloff, 0, sizeof paneloff);
#endif
	sizepanel(0, 28, 12, 0);
	sizepanel(1, 216, 200, 1);
	sizepanel(2, 216, 200, 1);
	sizepanel(3, 212, 280, 3);
	sizepanel(4, 280, 200, 3);
	sizepanel(5, 280, 200, 3);
	sizepanel(6, 280, 200, 3);
	sizepanel(7, 280, 200, 3);
	sizepanel(8, 280, 200, 3);
	sizepanel(9, 48, 160, 9);
	sizepanel(10, 200, 200, 10);
	sizepanel(11, 200, 200, 10);
	sizepanel(12, 216, 200, 12);
	sizepanel(13, 248, 84, 13);
	sizepanel(14, 192, 50, 14);
}

void
homepanels(void)
{
	movepanel(1, 416, 0);
	movepanel(3, 420, 200);
}

void
homepanel2(void)
{
	movepanel(2, 16, 8);
}

void
savepanels(void)
{
	savedinvx = panels[1].x;
	savedinvy = panels[1].y;
	savedsackx = panels[3].x;
	savedsacky = panels[3].y;
}

void
restorepanels(void)
{
	panels[1].x = savedinvx;
	panels[1].y = savedinvy;
	panels[3].x = savedsackx;
	panels[3].y = savedsacky;
}

/* centers window id on the screen */
void
centerpanel(unsigned char id)
{
	panels[id].x = 320 - panels[id].w / 2;
	panels[id].y = 240 - panels[id].h / 2;
	mousemoved = 1;
}

/* brings window id to the top, opening it if needed */
int
raisepanel(unsigned char id)
{
	for (raisepi = 0; raisepi < 16; raisepi++)
		if (id == panelstack[raisepi])
			goto found;
	openpanel(id);
	mousemoved = 1;
	return 1;
found:
	if (panelstack[0] == id)
		return 0;
	for (; raisepi > 0; raisepi--)
		panelstack[raisepi] = panelstack[raisepi - 1];
	panelstack[0] = id;
	mousemoved = 1;
	return 1;
}

/* closes window id */
void
closepanel(unsigned char id)
{
	for (closepi = 0; closepi < 16; closepi++)
		if (id == panelstack[closepi]) {
			for (; closepi < 15; closepi++)
				panelstack[closepi] = panelstack[closepi + 1];
			panelstack[15] = 0xff;
			mousemoved = 1;
			return;
		}
}

/* closes every window */
void
closepanels(void)
{
	memset(panelstack, 0xff, 16);
	mousemoved = 1;
}

/* returns the topmost item window that shows item, or 0xff */
unsigned char
itempanel(unsigned long item)
{
	for (itempi = 0; itempi < 16; itempi++)
		if (panelstack[itempi] != 0xff && panels[panelstack[itempi]].type == 3
		&& (int)panels[panelstack[itempi]].id == idof(item))
			return panelstack[itempi];
	return 0xff;
}

/* opens the window of item in a free slot from 4 to 8, or brings it to the top */
unsigned char
openstore(unsigned long item, int a, int b)
{
	int t;	/* a copy of a, for the original's registers */

	newstore = itempanel(item);
	if (newstore != 0xff) {
		raisepanel(newstore);
		return 0xfe;
	}
	newstore = 0xff;
	for (storeslot = 4; storeslot <= 8 && newstore == 0xff; storeslot++)
		if (!isopen(storeslot))
			newstore = storeslot;
	if (newstore == 0xff)
		return 0xff;
	openpanel(newstore);
	panels[newstore].type = 3;
	panels[newstore].id = idof(item);
	item &= 0xfff;
	panels[newstore].arg = item;
	t = a;
	panels[newstore].cellx = t;
	panels[newstore].celly = b;
	switch (typenames[item]) {
	case 0x1b:
		panels[newstore].arg = 0x197;
		panels[newstore].w = 268;
		panels[newstore].h = 202;
		break;
	case 0x1c:
		panels[newstore].arg = 0x198;
		panels[newstore].w = 244;
		panels[newstore].h = 186;
		break;
	case 0x1d:
		panels[newstore].arg = 0x199;
		panels[newstore].w = 280;
		panels[newstore].h = 146;
		break;
	default:
		panels[newstore].arg = 0x19a;
		panels[newstore].w = 212;
		panels[newstore].h = 200;
		break;
	}
	storeslot = newstore - 4;
	// PORT: a new chest opens at the wide screen's left edge
#ifdef PORT
	paneloff[newstore] = 0;
#endif
	panels[newstore].x = storeslot * 30 + 8;
	if (storeslot == 0)
		panels[newstore].y = 480 - panels[newstore].h;
	else
		panels[newstore].y = panels[newstore - 1].y + panels[newstore - 1].h - panels[newstore].h - 40;
	if (panels[newstore].x + panels[newstore].w > 632)
		panels[newstore].x = 632 - panels[newstore].w;
	else if (panels[newstore].x < 8)
		panels[newstore].x = 8;
	if (panels[newstore].y + panels[newstore].h > 480)
		panels[newstore].y = 480 - panels[newstore].h;
	else if (panels[newstore].y < 0)
		panels[newstore].y = 0;
	return newstore;
}

/* draws a gauge picture at dst, n of max full: the rows of full below those of empty */
void
fillgauge(char *dst, short n, short max, char *empty, char *full)
{
	if (n >= max) {
		memcpy(dst, full, 0x900);
		return;
	}
	gaugefull = n * 47 / max + 1;
	gaugeempty = 48 - gaugefull;
	gaugeempty *= 48;
	gaugefull *= 48;
	gaugesrc = empty;
	memcpy(dst, gaugesrc, gaugeempty);
	gaugesrc = full;
	memcpy(dst + gaugeempty, gaugesrc + gaugeempty, gaugefull);
}

/* the same for a picture of size bytes, w bytes a row, h rows of gauge below row top */
void
fillrows(char *dst, short n, short max, char *empty, char *full, short top, short h, short w, short size)
{
	if (max == 0 || (rowsfull = h * n / max) == 0) {
		memcpy(dst, empty, size);
		return;
	}
	memcpy(dst, full, size);
	if (n < max) {
		rowsfull = top + h - rowsfull;
		rowsfull *= w;
		rowssrc = empty;
		memcpy(dst, rowssrc, rowsfull);
	}
}

/* draws the hero's gauges, crossed while his life is under two thirds */
void
fillgauges(void)
{
	fillgauge(gauges[6], units[0].health, kindof(&units[0])->maxhealth, gauges[8], gauges[7]);
	fillrows(gauges[3], units[0].health, kindof(&units[0])->maxhealth, gauges[5], gauges[4], 2, 36, 48, 0x900);
	fillrows(gauges[0], units[0].health, kindof(&units[0])->maxhealth, gauges[2], gauges[1], 2, 16, 48, 0x900);
	if (units[0].health < kindof(&units[0])->maxhealth * 2 / 3) {
		set48x2((unsigned char *)gauges[6], 36, 2, 0xf5);
		set48x2((unsigned char *)gauges[6], 36, 4, 0xf5);
		set48x2((unsigned char *)gauges[6], 36, 6, 0xf5);
		set48x2((unsigned char *)gauges[6], 34, 4, 0xf5);
		set48x2((unsigned char *)gauges[6], 38, 4, 0xf5);
		set48x2((unsigned char *)gauges[3], 18, 4, 0xf5);
		set48x2((unsigned char *)gauges[0], 18, 4, 0xf5);
	}
}

/* loads the pictures of the gauges */
void
loadgauges(void)
{
	clear48(0, 0);
	putcs(0, 0, 0xf8);
	get48((unsigned char *)gauges[7], 0, 0, 0);
	clear48(0, 0);
	putcs(0, 0, 0xf9);
	get48((unsigned char *)gauges[8], 0, 0, 0);
	clear48(0, 0);
	putcs(0, 0, 0x105);
	get48((unsigned char *)gauges[4], 0, 0, 0);
	clear48(0, 0);
	putcs(0, 0, 0x106);
	get48((unsigned char *)gauges[5], 0, 0, 0);
	clear48(0, 0);
	putic(0, 0, 0, 2);
	get48((unsigned char *)gauges[1], 0, 0, 0);
	clear48(0, 0);
	putic(0, 0, 0, 4);
	get48((unsigned char *)gauges[2], 0, 0, 0);
}

/* draws the icons of u's states at x, y on surf, going left (mode 2) or up, and returns where it stopped */
int
putstates(int surf, int x, int y, Unit *u, int mode)
{
	int icon;

	if (badunit(u)) {
		if (mode == 2)
			return x;
		return y;
	}
	savedstates = ptrstates;
	ptrstates = 0;
	if (u->drunk) {
		if (mode)
			putic(surf, x, y, 0x18);
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
		ptrstates |= 0x1;
	}
	if (u->poison) {
		if (mode) {
			putic(surf, x, y, 8);
			if (u->health <= u->bleeding + u->poison)
				putpix2i(surf, x + 18, y + 4, 0xf5);
		}
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
		ptrstates |= 2;
	}
	if (u->bleeding) {
		if (mode) {
			putic(surf, x, y, 0xc);
			if (u->health <= u->bleeding + u->poison)
				putpix2i(surf, x + 18, y + 4, 0xf5);
		}
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
		ptrstates |= 4;
	}
	if (u->weakened) {
		if (mode)
			putic(surf, x, y, 0x21);
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
		ptrstates |= 0x8;
	}
	if (u->warded) {
		if (mode)
			putic(surf, x, y, 0xb);
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
		ptrstates |= 0x10;
	}
	if (u->blessed) {
		if (mode)
			putic(surf, x, y, 0x23);
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
		ptrstates |= 0x20;
	}
	if (u->cursed) {
		if (mode)
			putic(surf, x, y, 0x24);
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
		ptrstates |= 0x40;
	}
	if (u->berserk) {
		if (mode)
			putic(surf, x, y, 0x22);
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
		ptrstates |= 0x80;
	}
	if (u->fleeing || u->mode == 0x19) {
		if (mode)
			putic(surf, x, y, 0x17);
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
		ptrstates |= 0x100;
	}
	if (u->bewitched) {
		if (mode)
			putic(surf, x, y, 0xa);
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
		ptrstates |= 0x200;
	}
	if (u != &units[0]) {
		if (u->health < kindof(u)->maxhealth / 3)
			icon = 0x10;
		else if (u->health < kindof(u)->maxhealth / 2)
			icon = 0xf;
		else if (u->health < kindof(u)->maxhealth * 2 / 3)
			icon = 0xe;
		else
			icon = 0;
		if (icon) {
			if (mode)
				putic(surf, x, y, icon);
			if (mode == 2)
				x -= 24;
			else
				y -= 20;
			ptrstates |= 0x400;
		}
	}
	if (u == &units[0])
		ptrstates = savedstates;
	if (mode == 2)
		return x;
	return y;
}

void
msstates(int mode)
{
	// PORT: the hovered unit's states stand at the wide screen's left edge
#ifdef PORT
	int origin;

	origin = sysdrawx;
	sysdrawx = hudleft();
	putstates(curshape, 8, 460, &units[ptrunit], mode);
	sysdrawx = origin;
#else
	putstates(curshape, 8, 460, &units[ptrunit], mode);
#endif
}

/* returns the states of u as putstates flags them */
int
stateflags(Unit *u)
{
	stateset = 0;
	if (badunit(u))
		return 0;
	if (u->drunk)
		stateset |= 1;
	if (u->poison)
		stateset |= 2;
	if (u->bleeding)
		stateset |= 4;
	if (u->weakened)
		stateset |= 8;
	if (u->warded)
		stateset |= 0x10;
	if (u->blessed)
		stateset |= 0x20;
	if (u->cursed)
		stateset |= 0x40;
	if (u->berserk)
		stateset |= 0x80;
	if (u->fleeing || u->mode == 0x19)
		stateset |= 0x100;
	if (u->bewitched)
		stateset |= 0x200;
	if (u != &units[0] && u->health < kindof(u)->maxhealth * 2 / 3)
		stateset |= 0x400;
	return stateset;
}

/* draws the hero's status icons at x, y on surf, going left (mode 2) or up, and returns where it stopped */
int
putherostates(int surf, int x, int y, int flag, int mode)
{
	if (game.hungerlevel >= 5) {
		if (mode)
			putic(surf, x, y, game.hungerlevel < 7 ? 7 : 0x27);
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
	}
	if (game.tiredlevel >= 5) {
		if (mode)
			putic(surf, x, y, game.tiredlevel < 7 ? 6 : 0x26);
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
	}
	if (overloaded(&units[0])) {
		if (mode)
			putic(surf, x, y, 9);
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
	}
	if (mode == 2)
		x = putstates(surf, x, y, &units[0], mode);
	else
		y = putstates(surf, x, y, &units[0], mode);
	if (sensesleft) {
		if (mode)
			putic(surf, x, y, 0x20);
		if (mode == 2)
			x -= 24;
		else
			y -= 20;
	}
	if (playmode != 3 || flag) {
		if (!units[0].poison)
			switch (gaugepeace) {
			case 0:
				goto done;
			case 2:
				if (units[0].health >= kindof(&units[0])->maxhealth * 2 / 3)
					goto done;
				break;
			case 1:
				if (units[0].health >= kindof(&units[0])->maxhealth >> 1)
					goto done;
				break;
			case 3:
				break;
			}
		if (!tallpeace)
			goto flat;
		goto tall;
	} else {
		if (!units[0].poison)
			switch (gaugecombat) {
			case 0:
				goto done;
			case 2:
				if (units[0].health >= kindof(&units[0])->maxhealth * 2 / 3)
					goto done;
				break;
			case 1:
				if (units[0].health >= kindof(&units[0])->maxhealth >> 1)
					goto done;
				break;
			case 3:
				break;
			}
		if (tallcombat) {
tall:
			if (mode == 2)
				x -= 24;
			else
				y -= 20;
			if (mode)
				put48pt(surf, x, y, gauges[3], 0, 0, 24, 40);
			if (mode == 2)
				x -= 24;
			else
				y -= 20;
		} else {
		flat:
			if (mode)
				put48pt(surf, x, y, gauges[0], 0, 0, 24, 20);
			if (mode == 2)
				x -= 24;
			else
				y -= 20;
		}
	}
done:
	if (!flag)
		switch (playmode) {
		case 3:
			if (mode3icon)
				goto draw;
			break;
		case 2:
			if (mode2icon)
				goto draw;
			break;
		draw:
			if (mode)
				putic(surf, x, y, 0x16);
			if (mode == 2)
				x -= 24;
			else
				y -= 20;
			break;
		}
	if (mode == 2)
		return x;
	return y;
}

/* picks the picture of item and where it lies: worntype to wornh */
void
pickworn(unsigned long item, short flag)
{
	worntype = baselook(item & 0xfff);
	wornpic = 0;
	wornpic2 = 0;
	wornx = 0;
	worny = 0;
	if (worntype >= 0x1ad && worntype <= 0x1b5) {
		if (flag) {
			wornx = 0x50;
			worny = 0x32;
		}
		if (worntype == 0x1b5) {
			wornpic = 0x51;
			wornw = 0x50;
			wornh = 0x7a;
		} else {
			worntype -= 0x1ad;
			wornwear = item >> 30;
			worntype *= 3;
			if (wornwear > 1) {
				wornwear--;
				worntype += wornwear;
			}
			wornw = 0x50;
			wornh = 0x5a;
			wornpic = worntype + 0x2c;
		}
	} else if (worntype >= 0x4f2 && worntype <= 0x4f4) {
		if (flag) {
			wornx = 0x50;
			worny = 0x66;
		}
		wornpic = worntype - 0x42b;
		wornw = 0x58;
		wornpic2 = worntype - 0x428;
		wornh = 0x10;
		worntype -= 0x4f2;
	} else if (worntype >= 0x4f5 && worntype <= 0x4f7) {
		if (flag) {
			wornx = 0x68;
			worny = 0x34;
		}
		worntype -= 0x4f5;
		wornpic = worntype + 0xce;
		wornw = 0x28;
		wornh = 0xc;
	} else if (worntype >= 0x1a0 && worntype <= 0x1a3) {
		if (flag) {
			wornx = 0x60;
			worny = 0x66;
		}
		worntype -= 0x1a0;
		wornwear = item >> 30;
		worntype *= 3;
		if (wornwear > 1) {
			wornwear--;
			worntype += wornwear;
		}
		wornw = 0x38;
		wornh = 0x4e;
		wornpic = worntype + 0x45;
	} else if (worntype >= 0x1b7 && worntype <= 0x1b8) {
		if (flag) {
			wornx = 0x58;
			worny = 0xa4;
		}
		worntype = 0;
		wornpic = 0x53;
		wornw = 0x40;
		wornh = 0x1c;
	} else if (worntype >= 0x1c7 && worntype <= 0x1e0) {
		if (flag) {
			wornx = 0x48;
			worny = 0x68;
		}
		if (worntype >= 0x1c7 && worntype <= 0x1cf)
			worntype -= 0x1c7;
		else if (worntype == 0x1d6)
			worntype = 9;
		else if (worntype == 0x1d4)
			worntype = 0;
		else if (worntype == 0x1d5) {
			if (!flag) {
				wornpic = 0x9a;
				wornw = 0xb8;
				wornh = 0x16;
			} else {
				if (dollday > 0)
					wornpic = 0x98;
				else
					wornpic = 0x99;
				wornw = 0x18;
				wornh = 0xb2;
				wornx += 8;
				worny -= 0x5a;
			}
			return;
		} else
			worntype = 0;
		wornwear = item >> 30;
		worntype *= 3;
		if (wornwear > 1) {
			wornwear--;
			worntype += wornwear;
		}
		wornw = 0x48;
		wornh = 0x3c;
		wornpic = worntype + 0x5c;
		wornx += 8;
		worny += 4;
	} else if (worntype == 0x1b9 || worntype == 0x1c6) {
		if (flag) {
			wornx = 0x50;
			worny = 0x30;
		}
		wornwear = item >> 30;
		if (wornwear > 0)
			wornwear--;
		if (worntype == 0x1c6)
			wornpic = wornwear + 0xc4;
		else
			wornpic = wornwear + 0xc1;
		wornw = 0x58;
		if (flag)
			wornh = 0x3c;
		else
			wornh = 0x78;
	} else if (worntype >= 0x1a7 && worntype <= 0x1a9) {
		if (flag) {
			if (worntype == 0x1a9)
				wornx = 0x78;
			else
				wornx = 0x80;
			worny = 0x3a;
		}
		worntype -= 0x1a7;
		wornwear = item >> 30;
		worntype *= 3;
		if (wornwear > 1) {
			wornwear--;
			worntype += wornwear;
		}
		wornw = 0x40;
		wornh = 0x5e;
		wornpic = worntype + 0x9b;
	} else if (worntype == 0x221) {
		worntype = 9;
		goto stick;
	} else if (worntype == 0x604) {
		worntype = stepframe * 3 + 0x12;
	stick:
		if (flag) {
			wornx = 0x80;
			worny = 0x40;
		}
		wornwear = (item >> 16) * 10 / hourticks;
		if (wornwear > 20)
			wornwear = 2;
		else if (wornwear > 10)
			wornwear = 1;
		else
			wornwear = 0;
		wornx += 0x18;
		worntype += wornwear;
		worny -= 0x1c;
		wornpic = worntype + 0x9b;
		wornw = 0x40;
		wornh = 0x5e;
	} else if (worntype >= 0x1ba && worntype <= 0x1bf) {
		if (flag) {
			wornx = 0x68;
			worny = 0xa;
		}
		worntype -= 0x1ba;
		wornwear = item >> 30;
		worntype *= 3;
		if (wornwear > 1) {
			wornwear--;
			worntype += wornwear;
		}
		wornw = 0x28;
		wornh = 0x2a;
		wornpic = worntype + 0x16;
	}
}

void
picksheath(unsigned long item, short flag, short mode)
{
	worntype = baselook(item & 0xfff);
	wornpic = 0;
	wornx = 0;
	worny = 0;
	if (worntype >= 0x1c7 && worntype <= 0x1cf) {
		worntype -= 0x1c7;
	pick:
		if (flag) {
			wornx = 0x68;
			worny = 0x58;
		}
		if (mode == 0) {
			wornpic = worntype + 0x84;
			wornw = 0x40;
			wornh = 0x5a;
		} else {
			wornpic = worntype + 0x8e;
			wornw = 0x40;
			wornh = 0x5a;
		}
	} else if (worntype == 0x1d6) {
		worntype = 9;
		goto pick;
	}
}

/* draws unit u's outfit at x, y, with its belt items when items is set */
void
drawoutfit(int x, int y, Unit *u, short items, short flag)
{
	if (badunit(u))
		return;
	dollday = flag;
	if (u->cloak) {
		if ((u->cloak & 0xfff) == 0x1c6)
			putwint(x + 0x50, y + 0x30, 0xc0);
		else
			putwint(x + 0x50, y + 0x30, 0xbf);
	}
	if (u == &units[0]) {
		putwint(x + 0x50, y + 0x30, 0x44);
		if (!u->body && !u->legs && (censorship == 2 || censorship == 4))
			putwint(x + 0x50, y + 0x64, 0x52);
		if (units[0].offhand) {
			if (istorch(units[0].offhand))
				putcs(x + 0x88, y + 0x60, 0x77);
			else
				goto bare;
		} else
			switch (handframe) {
			case 1:
				putcs(x + 0x88, y + 0x60, 0x79);
				break;
			case 2:
				putcs(x + 0x88, y + 0x60, 0x7a);
				break;
			case 3:
				putcs(x + 0x88, y + 0x60, 0x7b);
				break;
			default:
			bare:
				putcs(x + 0x88, y + 0x60, 0x78);
				break;
			}
		dollhealth = u->health * 100 / kindof(u)->maxhealth;
		if (dollhealth <= 80)
			putcs(x + 0x50, y + 0x40, 0x70);
		if (dollhealth <= 65)
			putcs(x + 0x78, y + 0x38, 0x9a);
		if (dollhealth <= 50)
			putcs(x + 0x50, y + 0x56, 0x9b);
		if (dollhealth <= 35)
			putcs(x + 0x70, y + 0x48, 0x8e);
	}
	if (u->legs) {
		pickworn(u->legs, 1);
		if (wornpic)
			putwint(wornx + x, worny + y, wornpic);
	}
	if (u->feet) {
		pickworn(u->feet, 1);
		if (wornpic) {
			if (u == &units[0] && wornpic == 0x53 && footframe != 0)
				wornpic = 0x55;
			putwint(wornx + x, worny + y, wornpic);
		}
	}
	if (u->body) {
		pickworn(u->body, 1);
		if (wornpic)
			putwint(wornx + x, worny + y, wornpic);
	}
	if (u == &units[0]) {
		switch (headframe) {
		case 1:
			put48t0(x + 0x60, y + 0x16, ((char (*)[0x900])headicons)[1]);
			break;
		case 2:
			put48t0(x + 0x60, y + 0x16, ((char (*)[0x900])headicons)[2]);
			break;
		case 3:
			put48t0(x + 0x60, y + 0x16, ((char (*)[0x900])headicons)[3]);
			break;
		default:
			put48t0(x + 0x60, y + 0x16, ((char (*)[0x900])headicons)[0]);
			break;
		}
	}
	if (u->wrist) {
		pickworn(u->wrist, 1);
		if (wornpic)
			putwint(wornx + x, worny + y, wornpic);
	}
	if (u->belt) {
		if (u->body == 0 && u->legs == 0)
			putwint(x + 0x58, y + 0x5c, 0x57);
		picksheath(u->belt, 1, 0);
		if (wornpic)
			putwint(wornx + x, worny + y, wornpic);
	}
	if ((u->handkind == 2 || u->handkind == 1) && (u->body || u->legs)) {
		picksheath(u->hand, 1, 1);
		if (wornpic)
			putwint(wornx + x, worny + y, wornpic);
	}
	if (u->cloak) {
		pickworn(u->cloak, 1);
		if (wornpic)
			putwint(wornx + x, worny + y, wornpic);
	}
	if (u->neck) {
		pickworn(u->neck, 1);
		if (wornpic)
			putwint(wornx + x, worny + y, wornpic);
	}
	if (u->head) {
		pickworn(u->head, 1);
		if (wornpic)
			putwint(wornx + x, worny + y, wornpic);
	}
	if (u->offhand) {
		pickworn(u->offhand, 1);
		if (wornpic) {
			putwint(wornx + x, worny + y, wornpic);
			if (u == &units[0] && istorch(u->offhand))
				putwint(wornx + x - 8, worny + y + 0x48, 0x5b);
		}
	}
	if (!u->hand) {
		if (u == &units[0])
			putwint(x + 0x51, y + 0x72, 0x58);
	} else {
		pickworn(u->hand, 1);
		if (wornpic) {
			putwint(wornx + x, worny + y, wornpic);
			if (u == &units[0] && gametime < bloodtill && wornw == 0x48)
				putwint(wornx + x, worny + y, worntype / 3 + 0x7a);
			if (u == &units[0]) {
				dollweapon = baselook(u->hand & 0xfff);
				if (dollweapon == 0x1d5)
					putwint(wornx + x, worny + y + 0x5e, 0x5a);
				else if (dollweapon == 0x1d4)
					putwint(wornx + x, worny + y + 2, 0x59);
				else
					putwint(wornx + x, worny + y + 4, 0x59);
			}
		}
		if (baselook(u->hand & 0xfff) != 0x1d5) {
			if (u->wrist) {
				pickworn(u->wrist, 1);
				if (wornpic2) {
					putwint(wornx + x, worny + y, wornpic2);
					goto belt;
				}
				if (u == &units[0])
					goto bare2;
			} else if (u == &units[0]) {
			bare2:
				putwint(x + 0x50, y + 0x66, 0xcd);
			}
		}
	}
belt:
	if (items) {
		if (u->item1)
			puto(x + 8, y + 8, u->item1 & 0xfff);
		if (u->item2)
			puto(x + 8, y + 0x38, u->item2 & 0xfff);
		if (u->item3)
			puto(x + 8, y + 0x68, u->item3 & 0xfff);
	}
	dollday = 1;
}

/* formats the weight of item into printbuf, in kilograms or pounds */
short
weighttext(unsigned long item)
{
	weightv = itemweight(item);
	printbuf[0] = 0;
	if (weightv >= 65000)
		return 0;
	weightv *= 10;
	if (pounds) {
		weightv *= 1000;
		weightv /= 453;
	}
	weightfrac = weightv % 1000;
	weightint = weightv / 1000;
	if (cheat && (keys['W'] & 0x80))
		wsprintf(printbuf + 0x1f4, texts + 0xb1e3, weightint, weightfrac);
	else {
		if (weightint > 10) {
			if (weightfrac >= 500)
				weightint++;
			weightfrac = 0;
		}
		if (!weightint) {
			printbuf[0x1f4] = fracchar(weightfrac / 10);
			printbuf[0x1f5] = 0;
		} else {
			weightch = fracchar(weightfrac / 10);
			if (!weightfrac || weightch == '1' || weightch == '/')
				// FIXED: a fraction of 0.9 or more prints as a whole, and the whole printed is the part below it, so 2.95 kg shows 2 kg. Round it up.
#if defined(FEATURE_FIXES)
				wsprintf(printbuf + 0x1f4, texts + 0xb08d, weightint + (weightch == '1' ? 1 : 0));
#else
				wsprintf(printbuf + 0x1f4, texts + 0xb08d, weightint);
#endif
			else
				wsprintf(printbuf + 0x1f4, texts + 0xb1f1, weightint, weightch);
		}
	}
	wsprintf(printbuf, strs[1320], printbuf + 0x1f4, strs[pounds ? 816 : 815]);
	return 1;
}

int
silvtext(char *buf, unsigned long n)
{
	char c;

	if (n > 1) {
	common:
		wsprintf(buf, strs[1153], n);
		return 1;
	}
	c = fracchar(n * 10);
	if (c == 0x2f) {
		wsprintf(buf, strs[1160]);
		return 0;
	}
	if (c == 0x31) {
		n = 1;
		goto common;
	}
	wsprintf(buf, strs[1154], c);
	return 0;
}

int
valuetext(unsigned long item)
{
	if (cheat && (keys['W'] & 0x80)) {
		valuev = valueof(item);
		wsprintf(printbuf, strs[1155], valuev / 10, valuev % 10);
		return valuev ? 1 : 0;
	}
	valuev = silvvalue(item);
	return silvtext(printbuf, valuev);
}

void
weapontext(unsigned long item)
{
	damagerange(item, &weapmin, &weapmax);
	weapgear = gearof(item);
	if (cheat && (keys['W'] & 0x80))
		wsprintf(printbuf + 0x1f4, texts + 0xb0f8, weapmin, weapmax);
	else
		dmgrange(printbuf + 0x64, weapmin, weapmax);
	wsprintf(printbuf, strs[1318], printbuf + 0x64);
	wsprintf(printbuf + 0x1f4, strs[1315], ratetext(metals[weapgear].attack));
	wsprintf(printbuf + 0x3e8, strs[1316], ratetext(metals[weapgear].parry));
}

void
protecttext(unsigned long item)
{
	armgear = gearof(item);
	armprot = protection(item);
	if (armprot < 0)
		armprot = 0;
	if (cheat && (keys['W'] & 0x80))
		wsprintf(printbuf + 0x64, texts + 0xb08d, armprot);
	else
		wsprintf(printbuf + 0x64, armortext(armprot));
	wsprintf(printbuf, strs[1319], printbuf + 0x64);
	wsprintf(printbuf + 0x1f4, strs[1317], ratetext(120 - armors[armgear].hindrance * 7));
}

void
durtext(char *buf, int n)
{
	if (cheat && (keys['W'] & 0x80))
		wsprintf(buf, strs[1163], n);
	else {
		if (n > 36)
			wsprintf(buf, strs[1166], n / 24 + (n % 24 >= 12 ? 1 : 0));
		else if (n >= 21)
			wsprintf(buf, strs[1165]);
		else if (n < 4)
			wsprintf(buf, strs[1163], n);
		else
			/* defect: n * 100 % 24 stays below 24, so the 100 never applies */
			wsprintf(buf, strs[1164], fracchar(n * 100 / 24 + (n * 100 % 24 >= 1200 ? 100 : 0)));
	}
}

short
foodtext(unsigned long item)
{
	printbuf[0] = 0;
	foodtexth = satiety(item);
	if (foodtexth == 0)
		return 0;
	wsprintf(printbuf, strs[1162]);
	durtext(printbuf + 0x1f4, foodtexth);
	return 1;
}

short
burntext(unsigned long item)
{
	burnhours = (item >> 16) / hourticks;
	printbuf[0] = 0;
	if (burnhours == 0)
		return 0;
	wsprintf(printbuf, strs[1161]);
	wsprintf(printbuf + 0x1f4, strs[1163], burnhours);
	return 1;
}

void
clearboxes(void)
{
	// PORT: the size of boxes, whose pointers 64 bits widen
#ifdef PORT
	memset(boxes, 0, sizeof boxes);
#else
	memset(boxes, 0, 0x820);
#endif
	nboxes = 0;
	memset(boxtexts, 0, 800);
	nboxtexts = 0;
	infow = 0x130;
	infoh = 0x1e0;
	panels[12].w = 0x130;
	panels[12].h = 0x1e0;
}

/* sizes the boxes and lays them out: mode 2 in a column in panel 12 */
void
fitboxes(short mode)
{
	short h;

	fiti = 0;
	fitw = 0;
	fith = 0;
	fitmaxw = 0;
	for (; fiti < nboxes; fiti++) {
		boxes[fiti].w = 0;
		fitln = 0;
		boxes[fiti].h = 0;
		while (fitln < 20 && boxes[fiti].lines[fitln]) {
			fitdim = textwidth(boxes[fiti].lines[fitln]);
			if (fitdim > boxes[fiti].w)
				boxes[fiti].w = fitdim;
			fitln++;
		}
		boxes[fiti].h += fitln * 16 - 6;
		if (boxes[fiti].icon) {
			fitdim = 0x30;
			if (boxes[fiti].w < fitdim)
				boxes[fiti].w = fitdim;
			boxes[fiti].h += 0x30;
		}
		if (boxes[fiti].worn) {
			fitdim = rectw(boxes[fiti].worn);
			if (fitdim > boxes[fiti].w)
				boxes[fiti].w = fitdim;
			fitdim = recth(boxes[fiti].worn);
			if (boxes[fiti].window) {
				h = recth(boxes[fiti].window);
				/* defect: the store before the test leaves the larger height unchecked */
				fitdim = h;
				if (h > fitdim)
					fitdim = h;
			}
			boxes[fiti].h += fitdim;
		}
		if (boxes[fiti].unit) {
			fitdim = 0xb2;
			if (boxes[fiti].w < fitdim)
				boxes[fiti].w = fitdim;
			boxes[fiti].h += 0xb2;
		}
		if (boxes[fiti].w > fitmaxw)
			fitmaxw = boxes[fiti].w;
		fitw += boxes[fiti].w;
		fith += boxes[fiti].h;
	}
	if (mode == 2) {
		fitw = fitmaxw + 40;
		fith += (nboxes - 1) * 24;
		fith += 48;
		if (fitw % 4) {
			fitw /= 4;
			fitw *= 4;
			fitw += 4;
		}
		if (fitw > 0x130)
			fitw = 0x130;
		if (fith > 0x1e0)
			fith = 0x1e0;
		infow = fitw;
		panels[12].w = infow;
		infoh = fith;
		panels[12].h = infoh;
		for (fiti = 0; fiti < nboxes; fiti++) {
			boxes[fiti].x = (infow - boxes[fiti].w) / 2;
			boxes[fiti].y = fiti == 0 ? 24 : boxes[fiti - 1].y + boxes[fiti - 1].h + 24;
		}
	} else {
		fitw = infow - fitw;
		if (fitw < 0)
			fitw = 0;
		fitw /= nboxes + 1;
		fity = fith;
		fith = infoh - fith;
		if (fith < 0)
			fith = 0;
		fith /= nboxes + 1;
		if (fith > 24)
			fith = 24;
		fity += (nboxes - 1) * fith;
		fity = (infoh - fity) / 2;
		if (fity < 0)
			fity = 0;
		for (fiti = 0; fiti < nboxes; fiti++)
			if (mode) {
				boxes[fiti].x = fiti == 0 ? fitw : boxes[fiti - 1].x + boxes[fiti - 1].w + fitw;
				boxes[fiti].y = (infoh - boxes[fiti].h) / 2;
			} else {
				boxes[fiti].x = (infow - boxes[fiti].w) / 2;
				boxes[fiti].y = fiti == 0 ? fity : boxes[fiti - 1].y + boxes[fiti - 1].h + fith;
			}
	}
}

/* draws the boxes at x, y */
void
drawboxes(short x, short y)
{
	/* i only shapes the code: with it the loop multiplies by 104 with imul */
	int i;

	inkshade();
	for (dboxi = 0; dboxi < nboxes; dboxi++) {
		dboxln = 0;
		dboxx = boxes[dboxi].x + x;
		dboxy = boxes[dboxi].y + y;
		while (dboxln < 20 && boxes[dboxi].lines[dboxln]) {
			drawtext2o(0, (infow - textwidth(boxes[dboxi].lines[dboxln])) / 2 + x + (dboxy + dboxln * 16) * 640, boxes[dboxi].lines[dboxln]);
			dboxln++;
		}
		if (boxes[dboxi].icon)
			puto(dboxx, dboxy, boxes[dboxi].icon);
		if (boxes[dboxi].worn) {
			if (boxes[dboxi].window) {
				putwino(dboxx, dboxy, boxes[dboxi].window);
				putwint(dboxx, dboxy, boxes[dboxi].window);
			}
			putwino(dboxx, dboxy, boxes[dboxi].worn);
			putwint(dboxx, dboxy, boxes[dboxi].worn);
		}
		if (boxes[dboxi].unit) {
			if (((Unit *)boxes[dboxi].unit)->health <= 0) {
				drawoutfit(dboxx > 16 ? dboxx - 16 : 0, dboxy > 14 ? dboxy - 14 : 0, (Unit *)boxes[dboxi].unit, 1, 0);
			} else {
				drawoutfit(dboxx > 16 ? dboxx - 16 : 0, dboxy > 14 ? dboxy - 14 : 0, (Unit *)boxes[dboxi].unit, boxbrief ? 0 : 1, 0);
				if (!boxbrief) {
					dboxln = (dboxx > 16 ? dboxx - 16 : 0) + 0xb7;
					putstates(0, dboxln, (dboxy > 14 ? dboxy - 14 : 0) + 0xac, (Unit *)boxes[dboxi].unit, 1);
				}
			}
		}
	}
}

/* sets line i of box k to the text s */
void
boxline(short k, short i, char *s)
{
	wsprintf(boxtexts[nboxtexts], s);
	boxes[k].lines[i] = boxtexts[nboxtexts++];
}

/* fills the boxes that describe item, owned by unit owner, and lays them out */
void
describe(unsigned long item, short owner)
{
	if (lastlight <= 5)
		boxbrief = 1;
	else
		boxbrief = 0;
	clearboxes();
	desccls = classof(item & 0xfff);
	switch (desccls) {
	case 0xf:
	case 0x10:
		cointext(workbuf, item);
		break;
	case 0x14:
	case 0x15:
		wsprintf(workbuf, unitname(&units[item >> 16]));
		break;
	default:
		wsprintf(workbuf, itemname(item));
		break;
	}
	titlecase(workbuf);
	boxline(nboxes, 0, workbuf);
	nboxes++;
	switch (desccls) {
	case 0x3c:
	case 0x41:
	case 0x46:
	case 0x48:
	case 0x49:
	case 0x4a:
	case 0x4b:
	case 0x50:
	case 0x55:
		if (desccls == 0x49 || desccls == 0x4a)
			wsprintf(workbuf, strs[467]);
		else if (boxbrief)
			workbuf[0] = 0;
		else
			wsprintf(workbuf, weartext(item >> 29));
		boxline(nboxes, 0, workbuf);
		descbless = blessof(item);
		if (descbless != 7 && !boxbrief)
			boxline(nboxes, 1, strs[blessid(descbless)]);
		nboxes++;
		break;
	case 0x14:
		if (units[item >> 16].mode == 0x14 || units[item >> 16].mode == 0x15 || units[item >> 16].mode == 0x19) {
			if (units[item >> 16].modeb != 0xff) {
				wsprintf(printbuf, texts + 0x9dea, strs[units[item >> 16].mode == 0x19 ? 0x583 : 0x584], unitname(&units[units[item >> 16].modeb]));
				boxline(nboxes, 0, printbuf);
				nboxes++;
			}
		}
		break;
	}
	switch (desccls) {
	case 0x3c:
	case 0x41:
	case 0x46:
	case 0x48:
	case 0x4b:
	case 0x50:
	case 0x55:
	case 0x56:
	case 0x62:
	case 0x63:
		pickworn(item, 0);
		boxes[nboxes].worn = wornpic;
		if (desccls == 0x41)
			boxes[nboxes].window = 0x2b;
		else if (desccls == 0x48) {
			if (baselook(item & 0xfff) == 0x1c6)
				boxes[nboxes].window = 0xc0;
			else
				boxes[nboxes].window = 0xbf;
		}
		nboxes++;
		break;
	case 1:
	case 4:
		break;
	case 0x14:
	case 0x15:
		// PORT: the unit as a pointer, which 64 bits widen past an int
#ifdef PORT
		boxes[nboxes].unit = &units[item >> 16];
#else
		boxes[nboxes].unit = (int)&units[item >> 16];
#endif
		nboxes++;
		break;
	default:
		boxes[nboxes].icon = item & 0xfff;
		nboxes++;
		break;
	}
	switch (desccls) {
	case 0x3c:
		weapontext(item);
		boxline(nboxes, 0, printbuf);
		boxline(nboxes, 1, printbuf + 0x1f4);
		boxline(nboxes, 2, printbuf + 0x3e8);
		nboxes++;
		break;
	case 0x41:
	case 0x46:
	case 0x4b:
	case 0x50:
	case 0x55:
		protecttext(item);
		boxline(nboxes, 0, printbuf);
		boxline(nboxes, 1, printbuf + 0x1f4);
		nboxes++;
		break;
	case 0x62:
	case 0x63:
		if ((item >> 16) && !boxbrief) {
			descln = 0;
			if ((item & 0x80000000) == 0)
				boxline(nboxes, descln++, strs[1201]);
			else {
				if (item & 0x200000)
					boxline(nboxes, descln++, strs[1202]);
				if (item & 0x100000)
					boxline(nboxes, descln++, strs[1203]);
				if (item & 0x80000)
					boxline(nboxes, descln++, strs[1204]);
				if (item & 0x40000)
					boxline(nboxes, descln++, strs[1205]);
				if (item & 0x20000)
					boxline(nboxes, descln++, strs[1206]);
				if (item & 0x10000)
					boxline(nboxes, descln++, strs[1207]);
			}
			nboxes++;
		}
		break;
	case 0x37:
	case 0x38:
	case 0x39:
	case 0x82:
	case 0x83:
	case 0x84:
	case 0x85:
	case 0x86:
	case 0x87:
	case 0x88:
	case 0x89:
	case 0x8a:
	case 0x8b:
		if (foodtext(item)) {
			boxline(nboxes, 0, printbuf);
			boxline(nboxes, 1, printbuf + 0x1f4);
			nboxes++;
		}
		if (!boxbrief && desccls != 0x38 && desccls != 0x39 && (item & 0xee3f0000)) {
			descln = 0;
			if (item & 0xc0000000)
				boxline(nboxes, descln++, strs[1167]);
			if (item & 0x2e3f0000)
				boxline(nboxes, descln++, strs[1168]);
			nboxes++;
		}
		break;
	case 0x36:
		wsprintf(printbuf, strs[freshid(item)]);
		boxline(nboxes, 0, printbuf);
		nboxes++;
		if (foodtext(fishfood(item))) {
			boxline(nboxes, 0, printbuf);
			boxline(nboxes, 1, printbuf + 0x1f4);
			nboxes++;
		}
		break;
	case 0x56:
		if (burntext(item)) {
			boxline(nboxes, 0, printbuf);
			boxline(nboxes, 1, printbuf + 0x1f4);
			nboxes++;
		}
		break;
	case 1:
	case 4:
	case 0xa:
		if (typenames[item & 0xfff] == 0x1e)
			break;
		if (boxbrief)
			printbuf[0] = 0;
		else
			wsprintf(printbuf, breaktext(item >> 29));
		boxline(nboxes, 0, printbuf);
		if (desccls == 1 && (item & 0x8000000) || desccls == 0xa && (realms[idof(item)].item & 0x8000000))
			boxline(nboxes, 1, strs[270]);
		nboxes++;
		break;
	case 0x14:
	case 0x15:
		if (boxbrief)
			break;
		statustext(workbuf, &units[item >> 16]);
		descln = 0;
		descp = workbuf;
		descline = descp;
		for (;;) {
			if (!*descp)
				break;
			while (*descp && *descp != 0x2c)
				descp++;
			if (*descp == 0x2c) {
				*descp++ = 0;
				boxline(nboxes, descln++, descline);
				if (*descp == 0x20)
					descp++;
				descline = descp;
			} else {
				boxline(nboxes, descln++, descline);
				break;
			}
		}
		nboxes++;
		break;
	}
	if (!(desccls == 0x15 || desccls == 0x14) && !boxbrief && weighttext(item)) {
		boxline(nboxes, 0, printbuf);
		valuetext(item);
		boxline(nboxes, 1, printbuf);
		nboxes++;
	}
	if ((item & 0x1000) || owner) {
		if (owner == 0 || owner == 0xff)
			wsprintf(workbuf, strs[1314]);
		else
			wsprintf(workbuf, strs[1313], unitname(&units[owner]));
		boxline(nboxes, 0, workbuf);
		nboxes++;
	}
	fitboxes(2);
}

/* rounds a to the nearest multiple of b */
short
roundto(short a, short b)
{
	roundrem = a % b;
	a /= b;
	if (roundrem >= b / 2)
		a++;
	return a * b;
}

int
statbar(short v)
{
	if (v < 0)
		v = 0;
	else if (v > 210)
		v = 210;
	return v * 1000 / 125 / 100;
}

/* starts the glow at x, y */
void
setcaret(short x, short y)
{
	carety = y;
	caretx = x;
	if (caretstep)
		return;
	carettime = ticks();
	caretstep = 1;
}

void
stopcaret(void)
{
	caretstep = 0;
}

void
drawcaret(void)
{
	if (caretstep != 0)
		putcol(1, caretx, carety, 20, caretcols + caretstep - 1);
}

/* steps the glow every 100 ms */
void
stepcaret(void)
{
	if (caretstep == 0)
		return;
	if (ticks() >= carettime && ticks() <= carettime + 100)
		return;
	caretstep++;
	if (caretstep > 36)
		caretstep = 1;
	drawcaret();
	carettime = ticks();
}

/* draws the items of the store that panel id shows */
void
drawstore(int id)
{
	storepan = &panels[id];
	storex = storepan->x;
	storey = storepan->y;
	if (storepan->arg == 0x198)
		putwint(storex, storey, 7);
	else if (storepan->arg == 0x199)
		putwint(storex, storey, 8);
	else if (storepan->arg == 0x197)
		putwint(storex, storey, 9);
	else if (storepan->arg == 0x2e4)
		putwint(storex, storey, 10);
	else
		putwint(storex, storey, 6);
	storeitem = &realms[storepan->id].items[49];
	storepos = &realms[storepan->id].pos[49];
	for (storei = 49; storei >= 0; storei--) {
		if (*storeitem)
			puto(storex + *storepos % 320, storey + *storepos / 320, *storeitem & 0xfff);
		storepos--;
		storeitem--;
	}
}

void
putequ(int x, int y, unsigned long item)
{
	int n;

	n = gearicon(item);
	if (n == 0)
		return;
	putsrc(0, x, y, 6, n);
}

/* compares a with b: the text of how far a falls short of b, or b of a */
int
balance(int a, int b)
{
	if (a <= 50 && b <= 50)
		return 0;
	if (abs(b - a) <= 10)
		return 0;
	if (a < b * 50 / 100 && b > 150 && abs(b - a) > 100)
		return 0x5b;
	if (a < b * 66 / 100 && b > 150 && abs(b - a) > 50)
		return 0x5c;
	if (a < b * 75 / 100 && b > 150 && abs(b - a) > 50)
		return 0x5d;
	if (a < b * 90 / 100)
		return 0x5e;
	if (a < b * 95 / 100)
		return 0x20;
	if (a * 50 / 100 > b && abs(b - a) > 100)
		return 0x62;
	if (a * 66 / 100 > b && abs(b - a) > 50)
		return 0x61;
	if (a * 75 / 100 > b && abs(b - a) > 50)
		return 0x60;
	if (a * 90 / 100 > b)
		return 0x5f;
	if (a * 95 / 100 > b)
		return 0x21;
	return 0;
}

/* draws the unit panel id: frame, outfit, name, and the comparison with the hero */
void
drawinventory(int id)
{
	invpan = &panels[id];
	invx = invpan->x;
	invy = invpan->y;
	invday = 1;
	if (invpan->id == 0) {
		if (((unsigned char *)mapstate)[0x22] > 15) {
			invtext = daytime(gamehour);
			if (invtext == strs[492] || invtext == strs[498])
				putwint(invx, invy, 3);
			else if (invtext == strs[499]) {
				putwint(invx, invy, 4);
				invday = 0;
			} else
				putwint(invx, invy, 1);
		} else {
			putwint(invx, invy, 2);
			invday = 0;
		}
	} else {
		putwint(invx, invy, 5);
		invday = 0;
	}
	invunit = &units[invpan->id];
	drawoutfit(invx, invy, invunit, 1, invday);
	if (invunit->belt)
		putequ(invx + 8, invy + 0xb0, invunit->belt);
	inkshade();
	if (invpan->id == 0) {
		invtext = carrytext(invunit, 0);
		drawtext2o(0, invpan->x + 0xd6 - textwidth(invtext) + (invpan->y + 0xbc) * 640, invtext);
	} else if (invunit->health > 0)
		putstates(0, invx + 0xb8, invy + 0xac, invunit, 1);
	invtext = capname(invunit);
	drawtext2o(0, invpan->x + invpan->w / 2 - textwidth(invtext) / 2 + (invpan->y + 2) * 640, invtext);
	inkshade();
	if (invpan->id == 0 && trademode && trademode != 4 && goodsprice && offerprice) {
		if (talkactive && curunit && myservant(&units[curunit]))
			return;
		invx = balance(goodsbase, offerbase);
		if (invx)
			putcs(panels[1].x + 0x91, panels[1].y + 4, invx);
	}
}

/* waits for a key or a click: 0xfd when surfslost is set */
unsigned char
readkey(void)
{
	do
		readinput();
	while (keycode);
	lastchar = 0;
	esckey = 0;
	do {
		readinput();
		if (surfslost)
			return 0xfd;
		stepcaret();
		readk = lastchar;
		if (mouseleft && readk == 0)
			readk = 13;
		if (mouseright && esckey != 0xff)
			esckey = 1;
		if (esckey)
			return 1;
	} while (readk == 0);
	return readk;
}

unsigned char
pollkey(void)
{
	do
		pollinput();
	while (keycode);
	lastchar = 0;
	do {
		pollinput();
		if (surfslost)
			return 0xfd;
		pollk = lastchar;
		if (mouseleft && pollk == 0)
			pollk = 13;
		if (mouseright && esckey != 0xff)
			esckey = 1;
		if (esckey)
			return 1;
	} while (pollk == 0);
	return pollk;
}

/* runs one step of the hero's portraits as if unit 1 were on view */
int
animface(void)
{
	if (ticks() < standdue)
		return 0;
	oldblink = curblink;
	savedtalk = talkactive;
	talkactive = 1;
	savedtrade = trademode;
	trademode = 1;
	savedcur = curunit;
	curunit = 1;
	savedlife = units[1].health;
	savedfaceno = units[1].face;
	units[1].health = 100;
	units[1].face = units[0].face;
	walkdue = ticks() + walkdelay;
	standdue = ticks() + standdelay;
	if (++menuframe >= 24) {
		menuframe = 0;
		fidget();
	}
	if ((menuframe & 3) == 0)
		stepanims();
	units[curunit].face = savedfaceno;
	units[curunit].health = savedlife;
	curunit = savedcur;
	trademode = savedtrade;
	talkactive = savedtalk;
	if (oldblink == curblink)
		return 0;
	return 1;
}

/* draws the four corner pictures whose flags are clear */
void
darkcorners(void)
{
	if (lightlvl <= 11 || ((unsigned char *)mapstate)[0x22] <= 15 || sunlight < 11) {
		// PORT: the wide view's corners lie at the screen's, lit by what its own sight sees
#ifdef PORT
		if (sysscreen.left < 0 && !mapframed) {
			if (!widelit[0])
				putwint(sysscreen.left, 0, 0xd4);
			if (!widelit[1])
				putwint(sysscreen.right - 100, 0, 0xd5);
			if (!widelit[2])
				putwint(sysscreen.left, 0x140, 0xd6);
			if (!widelit[3])
				putwint(sysscreen.right - 100, 0x140, 0xd7);
			return;
		}
#endif
		if (!litnw)
			putwint(8, 0, 0xd4);
		if (!litne)
			putwint(0x214, 0, 0xd5);
		if (!litsw)
			putwint(8, 0x140, 0xd6);
		if (!litse)
			putwint(0x214, 0x140, 0xd7);
	}
}

short hoverunused = 0x3e7;
short hoverunused2 = 0x3e7;
int hovery = 199;
int runradius = 144;
int slowradius = 54;
int presscx = 0x3e7;
int presscy = 0x3e7;
unsigned short lasttick = 255;
unsigned char oddsswitch = 1;
int cfglong4 = 0x2710;
short dollday = 1;
short tipclears = 1;
short headanims[5][12] = {
	255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
	0, 2, 2, 2, 2, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 2, 2,
	2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 255
};
short handanims[5][12] = {
	255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 3, 3, 3, 1, 1, 1, 1, 1,
	1, 3, 3, 3, 3, 3, 255, 0, 0, 0, 0, 0, 0, 2, 2, 2, 1, 1, 1, 2, 2, 2,
	255, 0, 0, 3, 3, 1, 1, 1, 1, 1, 1, 255
};
short footanims[3][12] = {
	255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 255, 0, 0, 0, 0,
	0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 255
};
short heroblinkmin = 18;
short heroblinkmax = 45;
short curblinkmin = 18;
short curblinkmax = 45;
unsigned char panelstack[16] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255
};
unsigned char partkinds[78][2] = {
	46, 1, 46, 0, 47, 0, 47, 0, 47, 0, 47, 0, 47, 0, 47, 0, 47, 0, 47, 0,
	47, 0, 120, 0, 120, 0, 47, 0, 47, 0, 120, 0, 46, 0, 120, 0, 120, 0,
	47, 0, 47, 0, 47, 0, 47, 0, 47, 0, 47, 0, 120, 0, 46, 1, 120, 0, 46,
	1, 46, 1, 120, 1, 120, 1, 120, 1, 120, 1, 120, 1, 120, 1, 120, 1, 46,
	1, 120, 1, 46, 1, 46, 1, 46, 1, 46, 1, 46, 1, 46, 1, 46, 1, 46, 1,
	120, 1, 120, 1, 120, 1, 120, 1, 46, 1, 120, 0, 120, 1, 46, 1, 120, 0,
	46, 1, 46, 1, 46, 1, 46, 1, 46, 1, 120, 1, 120, 0, 120, 1, 120, 1,
	120, 0, 46, 0, 120, 0, 46, 0, 120, 1, 120, 1, 120, 1, 120, 0, 120, 0,
	47, 0, 120, 0, 120, 0, 46, 1
};
unsigned char soldmark2[16] = {
	101, 74, 202, 73, 13, 21, 104, 37, 0, 0, 252, 15, 194, 217, 215, 140
};
unsigned char builtcolors[4] = {
	255, 255, 255, 255
};
int builtbody = 255;
int builtlegs = 255;
int builtarmor = 255;
int builtcloak = 255;
int builtbrace = 255;
int builtring = 255;
int builthelm = 255;
int builtextra = 255;
int builtfeet = 255;
int savedinvx = 0x1a0;
int savedsackx = 0x1a4;
int savedsacky = 200;
short infow = 0x130;
short infoh = 0x1e0;
short clearmsgi;
short fidgetr;
int fieldr;
int pathr;
int groundr;
int waterr;
int dmapx;
int dmapy;
int dmapx0;
int dmapy0;
int dmapx1;
int dmapy1;
int dmapcell;
int dmaptile;
int dmapterr;
int dmapoff;
unsigned char dmapcol;
int turnx;
int turny;
unsigned partpic;
unsigned long partlen;
int partdoff;
int partsoff;
unsigned partnpics;
char *partsrc;
char *partdst;
char *partp;
char partkind;
char partflag;
unsigned dyei;
unsigned layi;
unsigned short recipei;
unsigned short recipepart;
char *recipedst;
unsigned short recipen;
unsigned short slotloopi;
int dressbody;
int dressfeet;
unsigned long dresslegs;
unsigned long dressarmor;
unsigned long dresscloak;
unsigned long dresshelm;
int dressextra;
int dressbrace;
int dressring;
unsigned char alignpi;
unsigned char isopenpi;
unsigned char openpi;
short movemax;
unsigned char raisepi;
unsigned char closepi;
unsigned char itempi;
unsigned char newstore;
unsigned char storeslot;
int gaugeempty;
int gaugefull;
char *gaugesrc;
int rowsfull;
char *rowssrc;
int savedstates;
int stateset;
short dollhealth;
unsigned long dollweapon;
unsigned long weightv;
unsigned short weightint;
unsigned short weightfrac;
char weightch;
unsigned long valuev;
short weapmin;
short weapmax;
short weapgear;
short armprot;
short armgear;
short foodtexth;
short burnhours;
short fiti;
short fitln;
short fitdim;
short fith;
short fitw;
short fity;
short fitmaxw;
short dboxi;
short dboxln;
short dboxx;
short dboxy;
unsigned char desccls;
char *descp;
char *descline;
short descbless;
short descln;
short roundrem;
unsigned long *storeitem;
unsigned short *storepos;
Panel *storepan;
short storei;
int storex;
int storey;
char *invtext;
Unit *invunit;
Panel *invpan;
int invx;
int invy;
short invday;
unsigned char readk;
unsigned char pollk;
short savedlife;
short savedfaceno;
short oldblink;
char savedtalk;
char savedtrade;
unsigned short savedcur;
unsigned char viewgrid[17][19];
short firedist;
short litnw;
short litne;
short litsw;
short litse;
short torchdist;
int lightunused;
unsigned char menumode;
int hoverx;
int hoverpx;
int hoverpy;
int hoverkind;
int hovercx;
int hovercy;
int ptrdx;
int ptrdy;
int ptrdistx;
int ptrdisty;
int waitforup;
int movecursor;
int ptrpace;
int savelong;
int debugmap;
unsigned char presskind;
char pressunused;
short pressunused2;
int pressunused3[2];
unsigned long hovergear;
char ptrfar;
char ptrblocked;
int ptrimg;
int ptrmark1;
int ptrmark2;
int ptrunused;
unsigned short steptick;
unsigned short stepframe;
unsigned fireframe;
unsigned slowframe;
unsigned waterframe;
unsigned char savebyte2;
int drawto2;
int showall;
void (*viewhook)(void);
void (*screenhook)(void);
int scrollx;
int scrolly;
int scrollunused;
int cfglong2;
int cfglong5;
int cfglong3;
short talky;
int talkchoice;
int litchoice;
unsigned char choicesh;
unsigned short choicesr;
short nameoff;
short nameunused;
short handframe;
short headframe;
short footframe;
unsigned long blinkclock;
short heroblink;
short curblink;
unsigned long heroblinkat;
unsigned long curblinkat;
unsigned short tipx;
unsigned short tipy;
char tiptext[200];
unsigned tipend;
short headanim;
short headstep;
short handanim;
short handstep;
short footanim;
short footstep;
unsigned long worntype;
unsigned long wornwear;
int wornpic;
int wornpic2;
short wornx;
short worny;
short wornw;
short wornh;
int bloodtill;
Panel panels[16];
int panelsunused;
int savedinvy;
char gauges[9][2304];
short condgauge;
char signback[2304];
char boxtexts[20][40];
short nboxtexts;
Box boxes[20];
short nboxes;
short boxbrief;
unsigned short caretx;
unsigned short carety;
unsigned short caretstep;
DWORD carettime;
int menuframe;

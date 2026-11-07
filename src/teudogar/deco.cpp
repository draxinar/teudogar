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

short
bedmate(void)
{
	bedself = indexof(decounit);
	for (bedi = 0; bedi < nunits; bedi++)
		if (bedi != bedself
		&& (units[bedi].mode == 90 || units[bedi].asleep)
		&& units[bedi].x == decounit->x && units[bedi].y == decounit->y)
			return bedi;
	return 0xff;
}

char *
lyingpic(void)
{
	lyingcell = decounit->x + decounit->y * 150;
	lyingmate = bedmate();
	bothwomen = 0;
	if (lyingmate != 0xff && censorship == 3 && isfemale(&units[lyingmate]))
		return pics[44];
	if (decounit->modea == 0xff || decounit->asleep) {
		if (!decounit->body) {
			if (!decounit->legs)
				return pics[9];
			return pics[8];
		}
		return pics[7];
	} else {
		if (lyingmate != 0xff) {
			if (isfemale(decounit)) {
				if (isfemale(&units[lyingmate]))
					bothwomen = 1;
			} else if (!isfemale(&units[lyingmate]))
				return pics[57];
		}
		if (findclass(lyingcell, 0x78)) {
			if (lyingmate != 0xff) {
				if (bothwomen)
					return pics[90];
				return pics[89];
			}
			return pics[isfemale(decounit) ? 0x58 : 0x57];
		}
		if (findclass(lyingcell, 0x79)) {
			if (lyingmate != 0xff) {
				if (bothwomen)
					return pics[77];
				return pics[76];
			}
			return pics[isfemale(decounit) ? 0x3f : 0x32];
		}
		if (findclass(lyingcell, 0x7b)) {
			if (lyingmate != 0xff) {
				if (bothwomen)
					return pics[85];
				return pics[84];
			}
			return pics[isfemale(decounit) ? 0x47 : 0x3a];
		}
		if (findclass(lyingcell, 0x7a)) {
			if (lyingmate != 0xff) {
				if (bothwomen)
					return pics[13];
				return pics[12];
			}
			return pics[isfemale(decounit) ? 0xb : 0xa];
		}
		if (findtype(lyingcell, 0x260)) {
			if (lyingmate != 0xff)
				return pics[79];
			return pics[isfemale(decounit) ? 0x53 : 0x51];
		}
		if (findtype(lyingcell, 0x26a)) {
			if (lyingmate != 0xff)
				return pics[78];
			return pics[isfemale(decounit) ? 0x52 : 0x50];
		}
		if (lyingmate != 0xff) {
			if (lyingcell & 1)
				return pics[65];
			return pics[66];
		}
		if (decounit == &units[0]) {
			if (decounit->cloak || decounit->modec == 2)
				return pics[6];
			if (!decounit->body) {
				if (!decounit->legs)
					return pics[9];
				return pics[8];
			}
			return pics[7];
		}
		if (isfemale(decounit)) {
			if (lyingcell & 1)
				return pics[69];
			return pics[70];
		}
		if (lyingcell & 1)
			return pics[67];
		return pics[68];
	}
}

char *
torchpic(char *p)
{
	if (fireframe) {
		if (p == ((char *)tilecache) + dir1frm0 * 0x900)
			return pics[fireframe + 0x5e];
		if (p == ((char *)tilecache) + dir1frm6 * 0x900)
			return pics[fireframe + 0x64];
		if (p == ((char *)tilecache) + dir3frm0 * 0x900)
			return pics[fireframe + 0x5c];
		if (p == ((char *)tilecache) + dir3frm6 * 0x900)
			return pics[fireframe + 0x62];
		if (p == ((char *)tilecache) + dir2frm0 * 0x900)
			return pics[fireframe + 0x5a];
		if (p == ((char *)tilecache) + dir2frm6 * 0x900)
			return pics[fireframe + 0x60];
	}
	return p + setpics * 0x5100;
}

void
pickshield(void)
{
	unsigned char k;

	if (decounit->pose == 0x18 || decounit->pose == 0x19) {
		k = partof(decounit)->anim;
		if (k == 6)
			goto none;
		k = partof(decounit)->anim;
		if (k == 2)
			goto none;
	}
	switch (decounit->offhand & 0xfff) {
	case 0x1a7:
		shieldlay = decobase + setpics * 0x3600;
		return;
	case 0x1a8:
		shieldlay = decobase + setpics * 0x3f00;
		return;
	case 0x1a9:
		shieldlay = decobase + setpics * 0x4800;
		return;
	case 0x604:
		if (torchpose)
			shieldlay = torchpic(torchpose);
		else
			shieldlay = torchpic(decobase);
		return;
	}
none:
	shieldlay = 0;
}

void
pickweapon(void)
{
	weaponlay = 0;
	sheathlay = 0;
	if (decounit->pose == 0x18 || decounit->pose == 0x19) {
		if (partof(decounit)->anim == 6)
			return;
		if (partof(decounit)->anim == 2)
			return;
	}
	if (decomode == 0x14 || decomode == 0x16 || isthrowing2(decounit->hand)) {
		switch (decounit->handkind) {
		case 2:
			weaponlay = decobase + setpics * 0x1200;
			if (decounit->body || decounit->legs)
				sheathlay = decobase + setpics * 0x7500;
			break;
		case 1:
			weaponlay = decobase + setpics * 0x1b00;
			if (decounit->body || decounit->legs)
				sheathlay = decobase + setpics * 0x6300;
			break;
		case 5:
			weaponlay = decobase + setpics * 0x2d00;
			break;
		case 4:
			weaponlay = decobase + setpics * 0x900;
			break;
		case 3:
			weaponlay = decobase + setpics * 0x2400;
			break;
		}
	} else {
		switch (decounit->handkind) {
		case 1:
			sheathlay = decobase + setpics * 0x5a00;
			break;
		case 2:
			sheathlay = decobase + setpics * 0x6c00;
			break;
		case 3:
			sheathlay = decobase + setpics * 0x7e00;
			break;
		}
	}
	if (sheathlay == 0) {
		switch (decounit->beltkind) {
		case 2:
			sheathlay = decobase + setpics * 0x6c00;
			break;
		case 1:
			sheathlay = decobase + setpics * 0x5a00;
			break;
		case 3:
			sheathlay = decobase + setpics * 0x7e00;
			break;
		}
	} else {
		switch (((Recipe *)(recipes + 0x20))[decounit->part].parts[0]) {
		case 0x16:
			sheathlay = 0;
			break;
		}
	}
}

void
pickicons(void)
{
	nicons = 0;
	if (hasgearbank != 1 || decounit == &units[0])
		return;
	iconbody = ((Recipe *)(recipes + 0x20))[decounit->part].parts[0];
	memset(icons.marks, 0xff, 10);
	switch (decounit->body & 0xfff) {
	case 0:
		break;
	case 0x1ad:
		break;
	case 0x1ae:
		if (partof(decounit)->showbody)
			icons.kinds[nicons++] = 3;
		break;
	case 0x1af:
		if (partof(decounit)->showbody)
			icons.kinds[nicons++] = 4;
		break;
	case 0x1b0:
		if (partof(decounit)->showbody)
			icons.kinds[nicons++] = 5;
		break;
	case 0x1b1:
		if (partof(decounit)->showbody)
			icons.kinds[nicons++] = 6;
		break;
	case 0x1b2:
		if (partof(decounit)->showbody)
			icons.kinds[nicons++] = 7;
		break;
	case 0x1b3:
		if (partof(decounit)->showbody)
			icons.kinds[nicons++] = 9;
		break;
	case 0x1b4:
		if (partof(decounit)->showbody)
			icons.kinds[nicons++] = 8;
		break;
	case 0x1b5:
		break;
	}
	if (decounit->wrist & 0xfff) {
		if (partof(decounit)->showjewels & 1) {
			if ((decounit->pose == 0x18 || decounit->pose == 0x19) && partof(decounit)->anim == 6)
				goto ring;
			icons.kinds[nicons] = 0xf;
		} else if (partof(decounit)->showjewels & 4)
			icons.kinds[nicons] = 2;
		else
			goto ring;
		if ((decounit->wrist & 0xfff) == 0x4f3)
			icons.marks[nicons] = 0xfe;
		else if ((decounit->wrist & 0xfff) == 0x4f2)
			icons.marks[nicons] = 0xfd;
		nicons++;
	}
ring:
	switch (decounit->cloak & 0xfff) {
	case 0x1b9:
		if (partof(decounit)->showcloak) {
			icons.kinds[nicons] = 0;
			for (iconi = 1; iconi < 5; iconi++)
				if (((Recipe *)(recipes + 0x20))[decounit->part].parts[iconi] == 0x26) {
					icons.marks[nicons] = 0x42;
					goto found;
				}
			icons.marks[nicons] = ((Recipe *)(recipes + 0x20))[decounit->part].colors[3];
		found:
			nicons++;
		}
		break;
	case 0x1c6:
		if (partof(decounit)->showcloak)
			icons.kinds[nicons++] = 1;
		break;
	}
	if (decounit->neck & 0xfff && !(partof(decounit)->hidering & 1)) {
		if (partof(decounit)->showjewels & 2)
			icons.kinds[nicons] = 0x10;
		else if (partof(decounit)->showjewels & 8)
			icons.kinds[nicons] = 0x11;
		else
			goto hand;
		if ((decounit->neck & 0xfff) == 0x4f6)
			icons.marks[nicons] = 0xfe;
		else if ((decounit->neck & 0xfff) == 0x4f5)
			icons.marks[nicons] = 0xfd;
		nicons++;
	}
hand:
	switch (decounit->head & 0xfff) {
	case 0:
		break;
	case 0x1ba:
		if (partof(decounit)->showhead)
			icons.kinds[nicons++] = 10;
		break;
	case 0x1bb:
	case 0x1bc:
		if (partof(decounit)->showhead)
			icons.kinds[nicons++] = 0xb;
		break;
	case 0x1bd:
		if (partof(decounit)->showhead)
			icons.kinds[nicons++] = 0xc;
		break;
	case 0x1bf:
		if (partof(decounit)->showhead)
			icons.kinds[nicons++] = 0xe;
		break;
	default:
		if (partof(decounit)->showhead)
			icons.kinds[nicons++] = 0xd;
		break;
	}
}

void
screenpos(Unit *u)
{
	screenposx = (u->x - playerx) * 48 + 0x128 + scrollx;
	screenposy = (u->y - playery) * 48 + 0xd8 + scrolly;
}

/* builds the deco of decounit and inserts it in *decos by y */
void
adddeco(void)
{
	decomode = decounit->mode;
	if (decounit == &units[0] && playmode == 3)
		decomode = 0x14;
	decosits = maysit(decounit);
	if (minsets || nopvari)
		resetpose(decounit, 2);
	decox = 0;
	decoy = 0;
	decocx = (decounit->x - playerx) * 48 + 0x128 + scrollx;
	decocy = (decounit->y - playery) * 48 + 0xd8 + scrolly;
	decocell = decounit->y * 150;
	decocell += decounit->x;
	decobase = ((char *)tilecache);
	lay0 = 0;
	lay1 = 0;
	lay2 = 0;
	lay3 = 0;
	decomark = 0;
	torchpose = 0;
	niconbufs = 0;
	if (decomode == 90 || decounit->asleep) {
		if (!isanimal(decounit)) {
			decobase = lay0 = lyingpic();
			decox += decocx;
			decoy += decocy;
			goto insert;
		}
		if (isdog(decounit)) {
			decobase = lay0 = pics[45];
			decox += decocx;
			decoy += decocy;
			goto insert;
		}
	}
	if (decounit->cellmate != 0xff) {
		decowalls = 0;
		if (mapflags[decocell] & 0x80) {
			if ((map[decocell - 1] & 0x4000) == 0)
				decowalls = 1;
			if ((map[decocell - 150] & 0x4000) == 0)
				decowalls |= 2;
		}
		if (decounit->cellmate > indexof(decounit)) {
			if ((decowalls & 1) == 0)
				decocx -= 4;
			if ((decowalls & 2) == 0)
				decocy -= 4;
		} else {
			if ((decowalls & 1) == 0)
				decocx += 4;
			if ((decowalls & 2) == 0)
				decocy += 4;
		}
	}
	switch (decounit->diry) {
	case -1:
		decobase = ((char *)tilecache) + dir0frm0 * 0x900;
		if (decophase) {
			decoy += -decophase * 2;
			if (decounit->dirx == -1)
				decox += -decophase * 2;
			else if (decounit->dirx == 1)
				decox += decophase * 2;
		}
		break;
	case 1:
		decobase = ((char *)tilecache) + dir2frm0 * 0x900;
		if (decophase) {
			decoy += decophase * 2;
			if (decounit->dirx == -1)
				decox += -decophase * 2;
			else if (decounit->dirx == 1)
				decox += decophase * 2;
		}
		break;
	default:
		if (decounit->dirx == -1) {
			decobase = ((char *)tilecache) + dir1frm0 * 0x900;
			if (decophase)
				decox += -decophase * 2;
		} else {
			decobase = ((char *)tilecache) + dir3frm0 * 0x900;
			if (decophase)
				decox += decophase * 2;
		}
		break;
	}
	decoflags = mapflags[decounit->x + decounit->y * 150];
	bodylay = pics[0];
	if (decounit->pose == 1) {
		resetpose(decounit, 3);
		if (decounit == &units[0]) {
			decox = 0;
			decoy = 0;
			decocx = 0x128;
			decocy = 0xd8;
		}
		if (!minsets)
			decobase += decoframe * 0x900;
		if (decounit == &units[0] && heropace >= 2 && !minsets && !nopvari
		&& !noprun && !((Recipe *)(recipes + 0x20))[decounit->part].parts[0]) {
			if (decounit->diry == 1) {
				switch (decoframe) {
				case 0:
					decobase = ((char *)tilecache) + 70 * 0x900;
					break;
				case 4:
					decobase = ((char *)tilecache) + 71 * 0x900;
					break;
				case 5:
					decobase = ((char *)tilecache) + 72 * 0x900;
					break;
				}
			} else if (decounit->diry == -1) {
				switch (decoframe) {
				case 0:
					decobase = ((char *)tilecache) + 64 * 0x900;
					break;
				case 4:
					decobase = ((char *)tilecache) + 65 * 0x900;
					break;
				case 5:
					decobase = ((char *)tilecache) + 66 * 0x900;
					break;
				}
			} else if (decounit->dirx == 1) {
				switch (decoframe) {
				case 0:
					decobase = ((char *)tilecache) + 73 * 0x900;
					break;
				case 4:
					decobase = ((char *)tilecache) + 74 * 0x900;
					break;
				case 5:
					decobase = ((char *)tilecache) + 75 * 0x900;
					break;
				}
			} else if (decounit->dirx == -1) {
				switch (decoframe) {
				case 0:
					decobase = ((char *)tilecache) + 67 * 0x900;
					break;
				case 4:
					decobase = ((char *)tilecache) + 68 * 0x900;
					break;
				case 5:
					decobase = ((char *)tilecache) + 69 * 0x900;
					break;
				}
			}
		}
		bodylay = decobase + setpics * decounit->picset * 0x900;
	} else if (decounit->pose == 3 && !minsets) {
		resetpose(decounit, 3);
		decox = partof(decounit)->anim;
		if (decox == 0xff && (decounit->hand & 0xfff) == 0) {
			bodylay = decobase + (decounit->picset * setpics + dir0frm7) * 0x900;
			decox = decocx;
			decoy = decocy;
			goto insert;
		}
		decox = 0;
		decoy = 0;
		switch (decoframe) {
		case 0:
		case 1:
			decobase += 0x900;
			break;
		default:
			decobase += 0x2d00;
			break;
		}
		bodylay = decobase + setpics * decounit->picset * 0x900;
	} else if (decounit->pose >= 11 && decounit->pose <= 18 && !minsets && !nopvari) {
		decox = anims[partof(decounit)->anim].anims[0][decounit->pose - 11].pics[decoframe];
		decomark = anims[partof(decounit)->anim].anims[1][decounit->pose - 11].marks[decoframe];
		resetpose(decounit, 1);
		if (decox < 1000)
			decobase = ((char *)tilecache) + decox * 0x900;
		bodylay = decobase + setpics * decounit->picset * 0x900;
		decox = 0;
		decoy = 0;
	} else if (decounit->pose >= 20 && decounit->pose <= 27 && !minsets && !nopvari
	&& !(decoflags & 0x40)) {
		decox = anims[partof(decounit)->anim].anims[1][decounit->pose - 20].pics[decoframe];
		decomark = anims[partof(decounit)->anim].anims[1][decounit->pose - 20].marks[decoframe];
		resetpose(decounit, 1);
		if (decox < 1000)
			decobase = ((char *)tilecache) + decox * 0x900;
		bodylay = decobase + setpics * decounit->picset * 0x900;
		decox = 0;
		decoy = 0;
	} else if (partof(decounit)->cansit && decosits && decoflags & 0x40) {
		switch (decoflags & 3) {
		case 3:
			decounit->dirx = 1;
			decounit->diry = 0;
			if (minsets) {
				torchpose = ((char *)tilecache) + dir3frm0 * 0x900;
				decounit->frame = dir3frm0;
			} else {
				torchpose = ((char *)tilecache) + 30 * 0x900;
				switch (decounit->idlepose) {
				case 1:
					decounit->frame = 0x2c;
					break;
				case 2:
					decounit->frame = 0x2d;
					break;
				case 3:
					decounit->frame = 0x2e;
					break;
				case 4:
					decounit->frame = 0x2f;
					break;
				default:
					decounit->frame = 0x1e;
					break;
				}
			}
			break;
		case 1:
			decounit->dirx = -1;
			decounit->diry = 0;
			if (minsets)
				decounit->frame = dir1frm0;
			else
				decounit->frame = 0xe;
			break;
		case 0:
			decounit->dirx = 0;
			decounit->diry = -1;
			if (minsets)
				decounit->frame = dir0frm0;
			else
				decounit->frame = 6;
			break;
		case 2:
		default:
			decounit->dirx = 0;
			decounit->diry = 1;
			if (minsets) {
				torchpose = ((char *)tilecache) + dir2frm0 * 0x900;
				decounit->frame = dir2frm0;
			} else {
				torchpose = ((char *)tilecache) + 22 * 0x900;
				switch (decounit->idlepose) {
				case 1:
					decounit->frame = 0x28;
					break;
				case 2:
					decounit->frame = 0x29;
					break;
				case 3:
					decounit->frame = 0x2a;
					break;
				case 4:
					decounit->frame = 0x2b;
					break;
				default:
					decounit->frame = 0x16;
					break;
				}
			}
			break;
		}
		cleartrail(decounit);
		decounit->pose = 10;
		goto pic;
	} else if (isdog(decounit) && !minmem && decosits) {
		if (decounit->dirx == 1)
			decounit->frame = 0x1e;
		else if (decounit->dirx == -1)
			decounit->frame = 0xe;
		else if (decounit->diry == -1)
			decounit->frame = 6;
		else
			decounit->frame = 0x16;
	pic:
		decobase = ((char *)tilecache) + decounit->frame * 0x900;
		bodylay = decobase + decounit->picset * setpics * 0x900;
		decox = 0;
		decoy = 0;
	} else {
		if (!minmem) {
			switch (decounit->diry) {
			case 1:
				torchpose = decobase;
				switch (decounit->idlestep) {
				case 1:
					decounit->frame = 0x20;
					goto turn;
				case 2:
					decounit->frame = 0x21;
					goto turn;
				case 3:
					decounit->frame = 0x22;
					goto turn;
				case 4:
					decounit->idlestep = 0;
					break;
				}
				break;
			case -1:
				break;
			default:
				if (decounit->dirx == -1)
					break;
				torchpose = decobase;
				switch (decounit->idlestep) {
				case 1:
					decounit->frame = 0x24;
					goto turn;
				case 2:
					decounit->frame = 0x25;
					goto turn;
				case 3:
					decounit->frame = 0x26;
					goto turn;
				case 4:
					decounit->idlestep = 0;
					break;
				}
				break;
			}
		}
		bodylay = decobase + decounit->picset * setpics * 0x900;
		decox = 0;
		decoy = 0;
		goto draw;
	turn:
		decobase = ((char *)tilecache) + decounit->frame * 0x900;
		bodylay = decobase + setpics * decounit->picset * 0x900;
		decox = 0;
		decoy = 0;
	}
draw:
	decooff = decobase - ((char *)tilecache);
	if (decooff < 0 || (unsigned)decooff > (setpics - 1) * 0x900)
		decooff = -1;
	// FIXED: the pictures of a unit's grass mask and gear come from scrbuf past the screen, which holds 120 of them, and 61 units on grass with gear take the 121st past scrbuf. Draw a unit without its mask once they run out.
#ifdef FEATURE_FAULTS
	if (cellflag40(map[decocell]) && partof(decounit)->grassmask && decooff != -1 && ndecobufs < 120) {
#else
	if (cellflag40(map[decocell]) && partof(decounit)->grassmask && decooff != -1) {
#endif
		grassbody = ((char (*)[0x900])(scrbuf + 0x7d00))[ndecobufs];
		ndecobufs++;
		memcpy(grassbody, bodylay, 0x900);
		tilemask((unsigned char *)grassbody, (unsigned char *)(grassmask + decooff));
		bodylay = grassbody;
	}
	pickicons();
	// FIXED: as for the grass mask, draw a unit without its gear once scrbuf's 120 pictures run out
#ifdef FEATURE_FAULTS
	if (nicons && hasgearbank == 1 && ndecobufs < 120) {
#else
	if (nicons && hasgearbank == 1) {
#endif
		gearbody = ((char (*)[0x900])(scrbuf + 0x7d00))[ndecobufs];
		ndecobufs++;
		memcpy(gearbody, bodylay, 0x900);
		for (decoi = 0; decoi < nicons; decoi++) {
			iconpic = icons.kinds[decoi] * setpics * 0x900 + ((char *)gearbank) + decooff;
			iconmark = icons.marks[decoi];
			if (niconbufs >= 30)
				;
			else {
				if (iconmark == 0xfe) {
					memcpy(iconbufs[niconbufs], iconpic, 0x900);
					tintsilver(iconbufs[niconbufs], 0x900);
					iconpic = iconbufs[niconbufs++];
				} else if (iconmark == 0xfd) {
					memcpy(iconbufs[niconbufs], iconpic, 0x900);
					tintbronze(iconbufs[niconbufs], 0x900);
					iconpic = iconbufs[niconbufs++];
				}
			}
			if (iconmark != 0xff) {
				memcpy(iconbufs[niconbufs], iconpic, 0x900);
				decodst = iconbufs[niconbufs];
				for (decon = 0x900; decon; decon--) {
					if (*decodst >= 0xc0 && *decodst <= 0xdf)
						*decodst = *decodst - 0xc0 + iconmark;
					decodst++;
				}
				iconpic = iconbufs[niconbufs];
				niconbufs++;
			}
			decodst = gearbody;
			for (decon = 0x900; decon; decon--) {
				if (*iconpic)
					*decodst++ = *iconpic++;
				else {
					decodst++;
					iconpic++;
				}
			}
		}
		bodylay = gearbody;
	}
	decox += decocx;
	decoy += decocy;
	pickweapon();
	pickshield();
	if (decounit->diry == -1) {
		lay0 = weaponlay;
		lay1 = shieldlay;
		lay2 = bodylay;
		lay3 = sheathlay;
	} else if (decounit->dirx == -1) {
		lay0 = weaponlay;
		lay1 = bodylay;
		lay2 = sheathlay;
		lay3 = shieldlay;
	} else {
		lay0 = bodylay;
		lay1 = sheathlay;
		lay2 = shieldlay;
		lay3 = weaponlay;
	}
insert:
	for (decoat = 0; decoat < ndecos; decoat++)
		if (decos[decoat].y > decoy) {
			for (decoi = ndecos; decoi > decoat; decoi--)
				memcpy(&decos[decoi], &decos[decoi - 1], sizeof(Deco));
			goto found;
		}
	decoat = ndecos;
found:
	ndecos++;
	decos[decoat].unit = indexof(decounit);
	decos[decoat].x = decox;
	decos[decoat].y = decoy;
	decos[decoat].pics[0] = lay0;
	decos[decoat].pics[1] = lay1;
	decos[decoat].pics[2] = lay2;
	decos[decoat].pics[3] = lay3;
	decos[decoat].pic = decomark;
}

void
cleardecos(void)
{
	ndecos = 0;
	ndecobufs = 0;
}

void
drawdecos(void)
{
	for (drawdi = 0; drawdi < ndecos; drawdi++) {
		sqclip(decos[drawdi].x, decos[drawdi].y);
		if (sqstate == 2)
			continue;
		if (sqstate == 1) {
			if (mapframed)
				continue;
			lay0 = decos[drawdi].pics[0];
			lay1 = decos[drawdi].pics[1];
			lay2 = decos[drawdi].pics[2];
			lay3 = decos[drawdi].pics[3];
			if (lay0)
				sqputt(lay0);
			if (lay1)
				sqputt(lay1);
			if (lay2)
				sqputt(lay2);
			if (lay3)
				sqputt(lay3);
		} else {
			lay0 = decos[drawdi].pics[0];
			lay1 = decos[drawdi].pics[1];
			lay2 = decos[drawdi].pics[2];
			lay3 = decos[drawdi].pics[3];
			if (lay0)
				sqput48t(lay0);
			if (lay1)
				sqput48t(lay1);
			if (lay2)
				sqput48t(lay2);
			if (lay3)
				sqput48t(lay3);
		}
	}
}

void
drawfx(void)
{
	for (fxi = 0; fxi < ndecos; fxi++) {
		fxpic = decos[fxi].pic;
		if (fxpic == 0)
			continue;
		if (fxpic >= 0x68)
			fxpic = 0;
		lay0 = pics[fxpic];
		sqclip(decos[fxi].x, decos[fxi].y);
		if (sqstate == 2)
			continue;
		if (sqstate == 1) {
			if (mapframed)
				continue;
			sqputt(lay0);
		} else
			sqput48t(lay0);
	}
}

Deco *decos = (Deco *)&scrbuf;
int bedi;
int bedself;
int lyingcell;
int lyingmate;
int bothwomen;
short iconbody;
short iconi;
short decosits;
int decooff;
int decox;
int decoy;
int decocx;
int decocy;
int decoi;
int decon;
int decocell;
int decowalls;
unsigned long decoflags;
unsigned long decomark;
unsigned char iconmark;
char *iconpic;
char *decodst;
short drawdi;
short fxi;
unsigned long fxpic;
unsigned char decomode;
char *decobase;
char *torchpose;
char *lay0;
char *lay1;
char *lay2;
char *lay3;
char *bodylay;
char *weaponlay;
char *shieldlay;
char *sheathlay;
Icons icons;
short nicons;
char iconbufs[30][2304];
short niconbufs;
int ndecobufs;
int ndecos;
int decoat;

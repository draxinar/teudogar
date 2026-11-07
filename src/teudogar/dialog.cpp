#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"

#pragma pack(1)

typedef struct Unit Unit;
typedef struct Region Region;
typedef struct Prov Prov;
typedef struct Kind Kind;
typedef struct Node Node;
typedef struct Metal Metal;
typedef struct Gear Gear;
typedef struct Zone Zone;
typedef struct Game Game;

struct Unit {
	unsigned char x;
	unsigned char y;
	char f2[4 - 2];
	unsigned char dirx;
	unsigned char diry;
	char f6[9 - 6];
	unsigned char mode;
	unsigned char modea;
	unsigned char modeb;
	unsigned char modec;
	unsigned char moded;
	char fe[0x56 - 0xe];
	unsigned char postx[24];
	unsigned char posty[24];
	char f86[0xe9 - 0x86];
	short scene;
	short plea;
	char fed[0xf3 - 0xed];
	short health;
	char ff5[0xf8 - 0xf5];
	unsigned long slot[13];
	char f12c[0x12f - 0x12c];
	unsigned char courage;
	char turnmarks;
	unsigned char clan;
	char f132[0x135 - 0x132];
	unsigned char asleep;
	char bewitched;
	unsigned char blessed;
	unsigned char poison;
	unsigned char cursed;
	char f13a[0x13c - 0x13a];
	unsigned char bleeding;
	char f13d[0x141 - 0x13d];
	short home;
	unsigned char traits;
	short mercy;
	signed char sympathy;
	char type;
	unsigned char flags;
	unsigned char lord;
	char f14a[0x159 - 0x14a];
	unsigned char modee;
	unsigned char modef;
	unsigned char modeg;
	char f15c[0x161 - 0x15c];
	short face;
	short number;
	unsigned char spouse;
	char f166[0x16a - 0x166];
	unsigned char sleepat;
	unsigned char workat1;
	unsigned char workat2;
	unsigned char eatat;
	unsigned char freeat;
	char talkflags;
	char scriptflags;
	short vars[3];
	char f177[0x1b8 - 0x177];
};

struct Region {
	char f0[0x10];
	unsigned char lord;
	char f11[0x33 - 0x11];
	short mealday;
	char f35[0x58 - 0x35];
	unsigned char placex[8];
	unsigned char placey[8];
	char nplaces;
	unsigned char site;
	char f6a[0x6e - 0x6a];
	short id;
	char f70[0xd0 - 0x70];
};

struct Prov {
	char f0[0x7c];
	short id;
	short mealday;
	short meals;
	char f82[0x164 - 0x82];
};

struct Kind {
	char f0[0x18];
	short maxhealth;
	char f1a[0x41 - 0x1a];
};

/* a node of a scene's dialog tree: a "...{ } block, a #...# choice */
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

/* the saved game, 0x5f8 bytes (009cc470) */
struct Game {
	char name[0x14];
	char f14[0x16 - 0x14];
	char known[0x1e];
	char flags[0xc8];
	int skills[30];
	short tiredcount;
	unsigned char tiredlevel;
	short hungercount;
	unsigned char hungerlevel;
	char f17a[0x17e - 0x17a];
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
	char outfittext[2];
	unsigned short runkills;
	char knownzones[52];
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
	char latinname[20];
	unsigned char combatbless;
	char reachedzones[13];
	unsigned short absenttime;
	unsigned short backday;
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

/* 0x1b bytes each (004c08a8): kinds of goods */
struct Metal {
	char f0[0x12];
	short days;
	short metal;
	char f16[0x1b - 0x16];
};

/* 0x1a bytes each (004c0bd8): kinds of food */
struct Gear {
	char f0[0x13];
	short days;
	short metal;
	char f17[0x1a - 0x17];
};

/* 0x22 bytes each (004f9498): the zones of the map */
struct Zone {
	short id;
	unsigned short name;
	char f4[0x22 - 4];
};

#pragma pack()

int badunit(Unit *u);
unsigned short blessof(unsigned long v);
unsigned long addcoins(unsigned long *p, unsigned long b);
unsigned long newitem(unsigned long item);
unsigned char classof(int item);
void setbless(unsigned long *item, unsigned short n);
unsigned long baselook(unsigned long item);
short scheduled(Unit *u);
unsigned short homeof(Unit *u);
short resthours(Unit *u);
Kind *kindof(Unit *u);
short isservant(Unit *u);
int counttypes(unsigned short k, unsigned long a, unsigned long b, unsigned long c);
long wealth(int mode);
unsigned long foodhours(void);
short provat(short x, short y);
short regionat(short x, short y);
char *youritem(unsigned long w, short a, short b);
char *itemname(unsigned long w);
unsigned short wearof(unsigned long x);
char *weartext(short n);
short blessid(short n);
char *personname(short n);
short favor(void);
void listvisions(void);
int foretell(char *buf, char a, char b);
void readytext(char *buf, unsigned long item, char *day);
void fmthealth(char *buf);
char *healthtext(Unit *u);
unsigned long charmprice(unsigned long v);
int runeletter(unsigned char n);
void castrunes(int n);
short myservant(Unit *u);
int wornslot(Unit *u, unsigned long item, int norm);
unsigned long *gearslot(Unit *u, int slot);
void calcstats(Unit *u);
unsigned long finditem(Unit *u, unsigned long w, int *a, int *b, int *c, int d);
unsigned long takeitem(int pos, unsigned long idx);
unsigned long takeout(unsigned short k, unsigned short i);
void addscore(int n);
void learnaction(void);
int fadeslow(int in);
int fademed(int in);
int fadefast(int in);
void flagunit(Unit *u);
void deeddefeat(Unit *u);
short msgf(short prio, char *fmt, ...);
short abdicate(Unit *u, short v);
void handover(Unit *u, Unit *v);
void makepeace(Unit *u, Unit *v);
int music(int n, int lp);
unsigned long jewelmagic(Unit *u, unsigned long mask);
int keepbless(short a, unsigned long w, short k, short n);
short isfemale(Unit *u);
void peaceall(Unit *u, Unit *v);
void peacehouse(Unit *u, Unit *v);
void nomercy(Unit *u);
void setlevels(void);
unsigned long giveitem(short u, unsigned long item, short how);
void train(unsigned char k, int n);
void hidecursor(void);
void primto2(void);
void s2toback(void);
int drawactions(void);
int flip(void);
void waitinput(int ms);
void dissolvet(void);
void s2toprim(void);
void drawcursor(void);
void waitrelease(short anim);
int wrap(char *s, int width);
void endtrade(void);
void knowpotion(unsigned long item);
void restfor(short n);
unsigned short healthof(Unit *u);
short learned(Unit *u);
void learnplace(void);
void travelmap(int mode, short a, short b);
void learnflag(void);
void setflag(int n, int v);
int songbychar(char c);
int paycoins(int type, unsigned long amount);
unsigned long inpack(short *pk, short *pj, unsigned long item, int norm);
void dresshero(void);
int taketypes(int k, unsigned long a, unsigned long b, unsigned long c, int n, int who);
void deedmercy(void);
void quitnow(void);
void closenote(short id, short sound);
short addnote(short id);
void dropnote(short id);
void setwear(unsigned long *p, unsigned long v);
void picpage(short a, short b);
void storymanip(int e);
void addsymp(Unit *u);
short gathergoods(Unit *u, short all, unsigned char kind);
void barter(Unit *u);
int opendoor(unsigned long pos, short force);
int closedoor(unsigned long pos, short mode);
void updatecell(int pos);
void lookaround(void);
void lookdone(void);
void offguard(Unit *u, short d, unsigned char c);
void deedbreakword(void);
void learnlore(void);
void setclock(short day, unsigned char hour);
char *unitname(Unit *u);
char *runesense(unsigned char n);
short informal(Unit *u);
int unlzw(unsigned char *dst, unsigned char *src, int n);
int text3w(char *s);
int langentry(int id);
int seekentry(int id);
unsigned short gearof(unsigned long item);
DWORD pack(void *dst, DWORD dstlen, void *src, DWORD srclen);
DWORD unpack(void *dst, DWORD dstlen, void *src, DWORD srclen);
short atwork(Unit *u);
short istrader(Unit *u);
short isherder(Unit *u);
int hasfought(Unit *u);
short neartools(short x, short y, short flag);
int knowsplayer(Unit *u);
short iscraftsman(Unit *u);
short issmith(Unit *u);
short repairable(unsigned long item);
short seesunit(Unit *u);
short hasweapon(Unit *u);
short isarmor(unsigned long item);
short hasnote(short id);
short noteclosed(short id);
int overladen(Unit *u);
short isfit(Unit *u);
short nearfoe(int n);
short underattack(short n);
short isguard(Unit *u);
short athome(Unit *u, short k);
char *daytime(unsigned char h);
short nextvision(char a, char b);
short freeplace(short r);
int travel(unsigned short dest, int x, int y, unsigned long time, unsigned short depart, unsigned short a6, unsigned short mode, short a8);
void cleartexts(void);
int trade(void);
unsigned long repaired(unsigned long item);
unsigned long passtime(unsigned long t, short ask, short b);
void drawscreen(void);
short itemplural(unsigned long item);
void sentcase(char *s);
void setunitmode(Unit *u, unsigned char mode);
short unitdist(Unit *u, Unit *v);
void armsound(Unit *u);
void duel(Unit *u, int n);
void learnfight(short n);
void provoke(Unit *a, Unit *b, signed char c, short d, short e);
void plotrage(short n);
void togglecombat(short n);

extern char *strs[];
extern char workbuf[];
extern char daytext[];
extern short gameday;
extern unsigned lockedslots;
extern unsigned short prices[];
extern short curvars[];
extern Unit units[];
extern unsigned short nunits;
extern unsigned short curunit;
extern char szvars[400];
extern Region regions[];
extern Prov provs[];
extern unsigned char gamehour;
extern unsigned short curmap;
extern Game game;
extern int playerx;
extern int playery;
extern long vara;
extern long varb;
extern long varc;
extern long vard;
extern long vare;
extern long varf;
extern char texts[47942];
extern char empty[1];
extern DWORD staylen;
extern unsigned char palblack;
extern char lastspeech;
extern short nregions;
extern short freetrade;
extern short telemapno;
extern short teleposx;
extern short teleposy;
extern short telehours;
extern short sendposx;
extern short sendposy;
extern short talknext;
extern short gobattle;
extern short talkfight;
extern int nobjtiles;
extern unsigned long tradeoffer[];
extern DWORD offerprice;
extern DWORD nforsale;
extern DWORD pricemarkup;
extern short repairmat;
extern char trademode;
extern Zone zones[];
extern short combatkind;
extern int playmode;
extern DWORD ordercmd;
extern int lang;
extern short avengers;
extern short victimname;
extern short victimzone;
extern unsigned char runepast;
extern unsigned char runenow;
extern unsigned char runenext;
extern char packbuf[];
extern HANDLE datfh;
extern DWORD storedn;
extern int text3width;
extern char scenevars[0x1388];
extern int scenevarlen;
extern int scriptlang;
extern short nscenes;
extern DWORD sceneidx;
extern int curscene;
extern char homenode;
extern short dealok;
extern DWORD scriptsize;
extern DWORD markoff[];
extern unsigned short marknum[];
extern char *scriptpos;
extern unsigned char currentnode;
extern Node nodes[255];
extern Node choices[4];
extern unsigned short nnodes;
extern char speech[];
extern char *resumeptr;
extern char nodetexts[2000];
extern char *nodetextp;
extern char talkflags[4];
extern char sceneflags[4];
extern unsigned long sceneitems[];
extern char scenerest[];
extern short tradekind;
extern char hasterms;
extern char reexpand;
extern int dutalk;
extern unsigned long tradegoods[];
extern DWORD goodsprice;
extern Metal metals[];
extern Gear armors[];
extern char condstring[];
extern char namebuf[2500];
extern short censorship;
extern char talkactive;
extern unsigned short hourticks;
extern char tmpbuf[2500];
extern short victimfemale;
extern unsigned short talkstate;
extern unsigned char talkanswer;
extern char nodecmds[];
extern unsigned char esckey;
extern short speechpx;
extern int spacestop;
extern unsigned char choicesh;
extern unsigned short choicesr;
extern char combatcause;

extern short lineofcount;
extern char *lineofptr;
extern unsigned long val;
extern unsigned long seglo;
extern unsigned long seghi;
extern unsigned long *segslot;
extern short unitid;
extern unsigned unitix;
extern int evn;
extern short evk;
extern short evm;
extern unsigned long evv;
extern unsigned long *evslot;
extern unsigned long evpct;
extern char *evp;
extern unsigned long exv;
extern unsigned long exw;
extern unsigned long *exslot;
extern char *exs;
extern char exc;
extern short nodei;
extern short nodebest;
extern short nodelen;
extern char *nodestart;
extern char *nodeend;
extern char *markp;
extern int markn;
extern DWORD loadi;
extern DWORD loadlen;
extern DWORD loadn;
extern HANDLE loadh;
extern int loadok;
extern unsigned short expi;
extern char *exps;
extern DWORD unpackn;
extern char *parsep;
extern char *parseq;
extern int parsei;
extern DWORD parseoff;
extern char *svp;
extern DWORD svn;
extern DWORD clrn;
extern short vj;
extern short vday;
extern short vi;
extern unsigned long vitem;
extern unsigned long litem;
extern short liti;
extern char *health;
extern char *healp;
extern short rdpick;
extern unsigned long tfslot;
extern int tfrealm;
extern int tfcell;
extern int tfidx;
extern unsigned long *tfgear;
extern short cmdret;
extern short *cmdvar;
extern unsigned short cmdrealm;
extern unsigned short cmdslotno;
extern unsigned long cmdw;
extern unsigned long cmdn;
extern unsigned long *cmdslot;
extern short cmdi;
extern short cmdk;
extern short *cmdp;
extern unsigned char lblk;
extern unsigned char lbln;
extern char *lblp;
extern short cmdj;
extern short cmdm;
extern unsigned short scrn;
extern short scrm;
extern short spendn;
extern short sympn;
extern short tuerx;
extern short tuery;
extern short tuerz;
extern char ce_val;
extern char ce_neg;
extern char ce_c;
extern char *ce_out;
extern char *ce_dst;
extern unsigned long cev;
extern unsigned long cea;
extern unsigned long ceb;
extern unsigned long *ceslot;
extern unsigned short cedays;
extern short blkdepth;
extern short svhour;
extern short svunit;
extern short wakereg;
extern short wakei;
extern short wakex;
extern short wakey;
extern char *rp;
extern short ret;
extern short cnt;
extern short lvi;
extern short lvj;
extern short lvn;
extern char *lvp;
extern unsigned long lvf;
extern char condv;
extern char *dlgp;
extern char *dlgq;
extern short dlgi;
extern short dlgj;
extern short dlgk;
extern short nchoice;
extern short nlong;
extern short dlgx;
extern short dlgy;
extern short dlgret;
extern DWORD scriptlen;
extern char script[450560];

/* number the line of the script text that p points into */
short
lineof(char *p)
{
	lineofcount = 1;
	for (lineofptr = workbuf; lineofptr < p; lineofptr++)
		if (*lineofptr == 13)
			lineofcount++;
	return lineofcount;
}

unsigned long *
unitslot(char c, Unit *u)
{
	if (badunit(u))
		return NULL;
	if (c >= 'a' && c <= 'z')
		c -= 0x20;
	if (c == 'P')
		return &u->slot[4];
	if (c == 'H')
		return &u->slot[3];
	if (c == 'W')
		return &u->slot[5];
	if (c == 'S')
		return &u->slot[6];
	if (c == 'G')
		return &u->slot[2];
	if (c == 'B')
		return &u->slot[7];
	if (c == 'M')
		return &u->slot[12];
	if (c == 'L')
		return &u->slot[8];
	if (c == 'R')
		return &u->slot[0];
	if (c == 'N')
		return &u->slot[1];
	if (c == '0')
		return &u->slot[9];
	if (c == '1')
		return &u->slot[10];
	if (c == '2')
		return &u->slot[11];
	return NULL;
}

void
setslotflag(char c)
{
	if (c == 'P')
		lockedslots |= 4;
	if (c == 'H')
		lockedslots |= 8;
	if (c == 'W')
		lockedslots |= 1;
	if (c == 'S')
		lockedslots |= 0x10;
	if (c == 'G')
		lockedslots |= 2;
	if (c == 'R')
		lockedslots |= 0x20;
	if (c == 'N')
		lockedslots |= 0x40;
}

void
fmtday(short d)
{
	if (d == 0) {
		wsprintf(daytext, strs[584]);
		return;
	}
	d -= gameday;
	if (d <= -14) {
		wsprintf(daytext, strs[585], -d / 7);
		return;
	}
	if (d > -14 && d <= -10) {
		wsprintf(daytext, strs[586]);
		return;
	}
	if (d > -10 && d <= -7) {
		wsprintf(daytext, strs[587]);
		return;
	}
	if (d > -7 && d < -2) {
		wsprintf(daytext, strs[588], -d);
		return;
	}
	if (d == -2)
		wsprintf(daytext, strs[589]);
	else if (d == -1)
		wsprintf(daytext, strs[590]);
	else if (d < 1)
		wsprintf(daytext, strs[591]);
	else if (d == 1)
		wsprintf(daytext, strs[592]);
	else if (d == 2)
		wsprintf(daytext, strs[593]);
	else if (d < 7)
		wsprintf(daytext, strs[594], d);
	else if (d < 10)
		wsprintf(daytext, strs[595]);
	else if (d < 14)
		wsprintf(daytext, strs[596]);
	else
		wsprintf(daytext, strs[597], d / 7);
}

void
fmtweekday(short d)
{
	if (d == 0)
		wsprintf(daytext, strs[1102]);
	else if (d == 1)
		wsprintf(daytext, strs[1103]);
	else if (d < 7)
		wsprintf(daytext, strs[1109], strs[1102 + d]);
	else if (d < 10)
		wsprintf(daytext, strs[1110]);
	else if (d < 14)
		wsprintf(daytext, strs[1111]);
	else
		wsprintf(daytext, strs[1112], strs[1102 + d / 7]);
}

unsigned long
getvar(char *s, short eval)
{
	val = 0;
	if (ideqi(s, texts + 0xa590)) {
		if (eval)
			val = prices[0x1cd];
		else
			val = 0xe701cd;
	} else if (ideqi(s, texts + 0xa59e)) {
		if (eval)
			val = prices[0x1c8];
		else
			val = 0xe201c8;
	} else if (ideqi(s, texts + 0xa5ab)) {
		if (eval)
			val = prices[0x1c9];
		else
			val = 0xe301c9;
	} else if (ideqi(s, texts + 0xa5b1)) {
		if (eval)
			val = prices[0x1ca];
		else
			val = 0xe401ca;
	} else if (ideqi(s, texts + 0xa5b7)) {
		if (eval)
			val = prices[0x1cb];
		else
			val = 0xe501cb;
	} else if (ideqi(s, texts + 0xa5c3)) {
		if (eval)
			val = prices[0x1cc];
		else
			val = 0xe601cc;
	} else if (ideqi(s, texts + 0xa5cd)) {
		if (eval)
			val = prices[0x1c7];
		else
			val = 0xe101c7;
	} else if (ideqi(s, texts + 0xa5e2)) {
		if (eval)
			val = prices[0x1ce];
		else
			val = 0xe801ce;
	} else if (ideqi(s, texts + 0xa5e6)) {
		if (eval)
			val = prices[0x1cf];
		else
			val = 0xe901cf;
	} else if (ideqi(s, texts + 0xa5ee)) {
		if (eval)
			val = prices[0x1d5];
		else
			val = 0xef01d5;
	} else if (ideqi(s, texts + 0xa5fd)) {
		if (eval)
			val = prices[0x528];
		else
			val = 0x528;
	} else if (ideqi(s, texts + 0xa631)) {
		if (eval)
			val = prices[0x1a0];
		else
			val = 0xe101a0;
	} else if (ideqi(s, texts + 0xa63c)) {
		if (eval)
			val = prices[0x1a1];
		else
			val = 0xe201a1;
	} else if (ideqi(s, texts + 0xa646)) {
		if (eval)
			val = prices[0x1a2];
		else
			val = 0xe301a2;
	} else if (ideqi(s, texts + 0xa653)) {
		if (eval)
			val = prices[0x1b7];
		else
			val = 0xe401b7;
	} else if (ideqi(s, texts + 0xa65b)) {
		if (eval)
			val = prices[0x1ba];
		else
			val = 0xe601ba;
	} else if (ideqi(s, texts + 0xa682)) {
		if (eval)
			val = prices[0x1bb];
		else
			val = 0xe701bb;
	} else if (ideqi(s, texts + 0xa665)) {
		if (eval)
			val = prices[0x1bc];
		else
			val = 0xe801bc;
	} else if (ideqi(s, texts + 0xa66c)) {
		if (eval)
			val = prices[0x1bd];
		else
			val = 0xe901bd;
	} else if (ideqi(s, texts + 0xa673)) {
		if (eval)
			val = prices[0x1be];
		else
			val = 0xea01be;
	} else if (ideqi(s, texts + 0xa67a)) {
		if (eval)
			val = prices[0x1bf];
		else
			val = 0xf901bf;
	} else if (ideqi(s, texts + 0xa689)) {
		if (eval)
			val = prices[0x1ad];
		else
			val = 0xec01ad;
	} else if (ideqi(s, texts + 0xa68e)) {
		if (eval)
			val = prices[0x1ae];
		else
			val = 0xed01ae;
	} else if (ideqi(s, texts + 0xa699)) {
		if (eval)
			val = prices[0x1af];
		else
			val = 0xee01af;
	} else if (ideqi(s, texts + 0xa6a5)) {
		if (eval)
			val = prices[0x1b0];
		else
			val = 0xef01b0;
	} else if (ideqi(s, texts + 0xa6b0)) {
		if (eval)
			val = prices[0x1b1];
		else
			val = 0xf001b1;
	} else if (ideqi(s, texts + 0xa6bf)) {
		if (eval)
			val = prices[0x1b2];
		else
			val = 0xf101b2;
	} else if (ideqi(s, texts + 0xa6ce)) {
		if (eval)
			val = prices[0x1b3];
		else
			val = 0xf701b3;
	} else if (ideqi(s, texts + 0xa6da)) {
		if (eval)
			val = prices[0x1b4];
		else
			val = 0xf801b4;
	} else if (ideqi(s, texts + 0xa6e7)) {
		if (eval)
			val = prices[0x1a7];
		else
			val = 0xf201a7;
	} else if (ideqi(s, texts + 0xa6f0)) {
		if (eval)
			val = prices[0x1a8];
		else
			val = 0xf301a8;
	} else if (ideqi(s, texts + 0xa6f9)) {
		if (eval)
			val = prices[0x1a9];
		else
			val = 0xf401a9;
	} else if (ideqi(s, texts + 0xa706)) {
		if (eval)
			val = prices[0x1b9];
		else
			val = 0xf601b9;
	} else if (ideqi(s, texts + 0xa70d)) {
		if (eval)
			val = prices[0x1c6];
		else
			val = 0xfa01c6;
	} else if (ideqi(s, texts + 0xa71d)) {
		if (eval)
			val = prices[0x4f2];
		else
			val = 0x4f2;
	} else if (ideqi(s, texts + 0xa72a)) {
		if (eval)
			val = prices[0x4f3];
		else
			val = 0x4f3;
	} else if (ideqi(s, texts + 0xa737)) {
		if (eval)
			val = prices[0x4f4];
		else
			val = 0x4f4;
	} else if (ideqi(s, texts + 0xa744)) {
		if (eval)
			val = prices[0x4f5];
		else
			val = 0x4f5;
	} else if (ideqi(s, texts + 0xa74f)) {
		if (eval)
			val = prices[0x4f6];
		else
			val = 0x4f6;
	} else if (ideqi(s, texts + 0xa75a)) {
		if (eval)
			val = prices[0x4f7];
		else
			val = 0x4f7;
	} else if (ideqi(s, texts + 0xa706)) {
		if (eval)
			val = prices[0x1b9];
		else
			val = 0xf601b9;
	} else if (ideqi(s, texts + 0xa765)) {
		if (eval)
			val = prices[0x514];
		else
			val = 0x514;
	} else if (ideqi(s, texts + 0xa78d)) {
		if (eval)
			val = prices[0x18e];
		else
			val = 0x40018e;
	} else if (ideqi(s, texts + 0xa795)) {
		if (eval)
			val = prices[0x18f];
		else
			val = 0x80018f;
	} else if (ideqi(s, texts + 0xa79a)) {
		if (eval)
			val = prices[0x533];
		else
			val = newitem(0x533);
	} else if (ideqi(s, texts + 0xa7a0)) {
		if (eval)
			val = prices[0x17f];
		else
			val = 0x40017f;
	} else if (ideqi(s, texts + 0xa7ae)) {
		if (eval)
			val = prices[0x190];
		else
			val = 0x1000190;
	} else if (ideqi(s, texts + 0xa7b4)) {
		if (eval)
			val = 10;
	} else if (ideqi(s, texts + 0xa7bf)) {
		if (eval)
			val = 20;
	} else if (ideqi(s, texts + 0xa7c6)) {
		if (eval)
			val = 10;
	} else if (ideqi(s, texts + 0xa7ca)) {
		if (eval)
			val = 250;
	} else if (ideqi(s, texts + 0xa7d2)) {
		if (eval)
			val = 100;
	} else if (ideqi(s, texts + 0xa7d6)) {
		if (eval)
			val = 150;
	} else if (ideqi(s, texts + 0xa7dd)) {
		if (eval)
			val = 250;
	} else if (ideqi(s, texts + 0xa7e5)) {
		if (eval)
			val = 200;
	} else if (ideqi(s, texts + 0xa7f5)) {
		if (eval)
			val = 0;
		else {
			val = newitem(0x1eb);
			val |= 0xc0020000;
		}
	} else if (ideqi(s, texts + 0xa865)) {
		if (eval)
			val = prices[0x144];
		else
			val = newitem(0x144);
	} else if (ideqi(s, texts + 0xa874)) {
		if (eval)
			val = prices[0x14e];
		else
			val = newitem(0x14e);
	} else if (ideqi(s, texts + 0xa880)) {
		if (eval)
			val = prices[0x151];
		else
			val = newitem(0x151);
	} else if (ideqi(s, texts + 0xa88f)) {
		if (eval)
			val = prices[0x175];
		else
			val = newitem(0x175);
	} else if (ideqi(s, texts + 0xa89c)) {
		if (eval)
			val = prices[0x176];
		else
			val = newitem(0x176);
	} else if (ideqi(s, texts + 0xa8a7)) {
		if (eval)
			val = prices[0x177];
		else
			val = newitem(0x177);
	} else if (ideqi(s, texts + 0xa8b3)) {
		if (eval)
			val = prices[0x17d];
		else
			val = newitem(0x17d);
	} else if (ideqi(s, texts + 0xa8be)) {
		if (eval)
			val = prices[0x182];
		else
			val = newitem(0x182);
	} else if (ideqi(s, texts + 0xa8c9)) {
		if (eval)
			val = prices[0x181];
		else
			val = newitem(0x181);
	} else if (ideqi(s, texts + 0xa8d6)) {
		if (eval)
			val = prices[0x185];
		else
			val = newitem(0x185);
	} else if (ideqi(s, texts + 0xa8e6)) {
		if (eval)
			val = prices[0x18c];
		else
			val = newitem(0x18c);
	} else if (ideqi(s, texts + 0xa8f3)) {
		if (eval)
			val = prices[0x18d];
		else
			val = newitem(0x18d);
	} else if (ideqi(s, texts + 0xa8ff)) {
		if (eval)
			val = prices[0x192];
		else
			val = newitem(0x192);
	} else if (ideqi(s, texts + 0xa90c)) {
		if (eval)
			val = prices[0x4a5];
		else
			val = newitem(0x4a5);
	} else if (ideqi(s, texts + 0xa916)) {
		if (eval)
			val = prices[0x178];
		else
			val = newitem(0x178);
	} else if (ideqi(s, texts + 0xa923)) {
		if (eval)
			val = prices[0x174];
		else
			val = newitem(0x174);
	} else if (ideqi(s, texts + 0xa92f)) {
		if (eval)
			val = prices[0x173];
		else
			val = newitem(0x173);
	} else if (ideqi(s, texts + 0xa7ee)) {
		if (eval)
			val = prices[0x221];
		else
			val = 0x3ff0221;
	} else if (ideqi(s, texts + 0xa803)) {
		val = 0x97f;
		if (eval)
			val = prices[val & 0xfff];
	} else if (ideqi(s, texts + 0xa831)) {
		val = 0x193;
		if (eval)
			val = prices[val & 0xfff];
	} else if (ideqi(s, texts + 0xa83e)) {
		val = 0x194;
		if (eval)
			val = prices[val & 0xfff];
	} else if (ideqi(s, texts + 0xa84b)) {
		val = 0x195;
		if (eval)
			val = prices[val & 0xfff];
	} else if (ideqi(s, texts + 0xa858)) {
		val = 0x196;
		if (eval)
			val = prices[val & 0xfff];
	} else if (ideqi(s, texts + 0xa80d)) {
		val = 0x16b;
		if (eval)
			val = prices[val & 0xfff];
	} else if (ideqi(s, texts + 0xa825)) {
		val = 0x532;
		if (eval)
			val = prices[val & 0xfff];
	} else if (ideqi(s, texts + 0xa815)) {
		val = 0x15e;
		if (eval)
			val = prices[val & 0xfff];
	} else if (ideqi(s, texts + 0xa81b)) {
		val = 0x965;
		if (eval)
			val = prices[val & 0xfff];
	} else if (s[0] == 'S' && s[1] == 'i' && s[2] == 'l' && s[3] == 'b' && s[4] >= '0' && s[4] <= '9') {
		val = number(s + 4);
		if (eval)
			val *= 10;
		else {
			val <<= 16;
			val |= 0x4e9;
			addcoins(&val, 0);
		}
	} else if (s[0] == 'G' && s[1] == 'o' && s[2] == 'l' && s[3] == 'd' && s[4] >= '0' && s[4] <= '9') {
		val = number(s + 4);
		if (eval)
			val *= 250;
		else {
			val <<= 16;
			val |= 0x503;
			addcoins(&val, 0);
		}
	} else if (s[0] == 'T' && s[1] == 'x' && s[2] == 't') {
		val = number(s + 3) << 16;
		val |= 0x189;
		if (eval)
			val = prices[val & 0xfff];
	} else if (s[0] == 's' && s[1] == 'e' && s[2] == 'g' && s[4] >= '0' && s[4] <= '9') {
		if (eval) {
			val = 0;
			seghi = number(s + 4);
			segslot = unitslot(s[3], units);
			if (segslot && *segslot) {
				seglo = blessof(*segslot);
				if (seglo >= seghi)
					val = 0;
				else {
					val = 0;
					if (seglo < 7 && seghi <= 7)
						val = (seghi - seglo) * 100;
					else if (seglo < 7) {
						val += (7 - seglo) * 100;
						seglo = 7;
						val += (seghi - seglo) * 250;
					} else
						val += (seghi - seglo) * 250;
				}
			}
		}
	} else if (s[0] == 'p' && s[1] >= '0' && s[1] <= '9')
		val = curvars[s[1] - '0'];
	else
		return 0;
	if (eval)
		val /= 10;
	while (istokc(*s))
		s++;
	if (eval) {
		if (*s == '/' && s[1] >= '1' && s[1] <= '9') {
			seglo = number(s + 1);
			if (seglo)
				val /= seglo;
		} else if (*s == '*' && s[1] >= '1' && s[1] <= '9') {
			seglo = number(s + 1);
			val *= seglo;
		}
	}
	return val;
}

/* find the unit whose number is s */
unsigned
findunit(char *s)
{
	unitid = number(s);
	if (!unitid)
		return 0;
	unitid |= 0x2000;
	for (unitix = 1; unitix < nunits; unitix++)
		if ((units[unitix].number | 0x2000) == unitid)
			return unitix;
	return 0;
}

long
evalvar(char *s)
{
	evp = s;
	if (*evp == '%') {
		evp++;
		if (evp[0] == '0' && evp[1] == 'N' && evp[2] >= '0' && evp[2] <= '2')
			return units[curunit].vars[evp[2] - '0'];
		if (evp[0] >= '0' && evp[0] <= '3' && evp[1] >= '0' && evp[1] <= '9')
			return curvars[evp[1] + 10 * (evp[0] - '0') - '0'];
		if (evp[0] >= '0' && evp[0] <= '9')
			return curvars[evp[0] - '0'];
		if ((evp[0] == 's' || evp[0] == 'S') && (evp[1] == 'z' || evp[1] == 'Z')) {
			evp += 2;
			evn = number(evp);
			if (evn >= 0 && evn < 200) {
				evn = ((short *)szvars)[evn];
				if (evn > 0)
					return evn;
			}
			return 0;
		}
		if (evp[0] == 'o' || evp[0] == 'O') {
			evp += 2;
			if (evp[0] == 'A' && evp[1] == '_') {
				evp += 2;
				return getvar(evp, 0) & 0xfff;
			}
			if (evp[0] == 'S' && evp[1] >= '0' && evp[1] <= '9' && evp[2] >= '0' && evp[2] <= '9' && evp[3] == '_') {
				evp++;
				evk = number(evp);
				evp += 3;
				evv = getvar(evp, 0);
				evn = classof(evv & 0xfff);
				if (evk >= 0 && evk <= 15)
					if (evn == 0x3c || evn == 0x41 || evn == 0x46 || evn == 0x4b || evn == 0x50 || evn == 0x55 || evn == 0x48)
						setbless(&evv, evk);
				return evv;
			}
			if (evp[0] == 'D' && evp[1] >= '0' && evp[1] <= '9') {
				evp++;
				evk = number(evp);
				while (*evp >= '0' && *evp <= '9')
					evp++;
				if (*evp != '_')
					return 0x702;
				evp++;
				evv = getvar(evp, 0);
				if (evv == 0)
					return 0x702;
				evv &= 0xffff;
				evv |= evk << 16;
				return evv;
			}
			return getvar(evp, 0);
		}
		if ((evp[0] == 'p' || evp[0] == 'P') && (evp[1] == 't' || evp[1] == 'T' || evp[1] == 'b' || evp[1] == 'B' || evp[1] == '_')) {
			evpct = 100;
			evp++;
			while (*evp == 't' || *evp == 'T') {
				evpct += 10;
				evp++;
			}
			while (*evp == 'b' || *evp == 'B') {
				evpct -= 10;
				evp++;
			}
			evp++;
			evpct *= getvar(evp, 1);
			evpct /= 100;
			if (evpct >= 300) {
				evpct = (evpct % 50 >= 25) + evpct / 50;
				evpct *= 50;
			}
			return evpct;
		}
		if ((evp[0] == 'n' || evp[0] == 'N') && evp[1] >= '0' && evp[1] <= '9')
			return findunit(evp + 1);
		if (evp[0] == 'N' && evp[1] == 'n' && evp[2] == 'r')
			return curunit;
		if ((evp[0] == 'n' || evp[0] == 'N') && (evp[1] == 'n' || evp[1] == 'N')) {
			if (curunit) {
				evp += 2;
				if (*evp == 'H' || *evp == 'h')
					evn = units[curunit].lord;
				else if (*evp == 'P' || *evp == 'p')
					evn = units[curunit].spouse;
				else
					return 0;
				return evn;
			}
			return 0;
		}
		if ((evp[0] == 'n' || evp[0] == 'N' || evp[0] == 's' || evp[0] == 'S') && evp[1] == '_') {
			if (evp[0] == 'n' || evp[0] == 'N') {
				if (curunit == 0 || curunit == 0xff)
					return 0;
				evk = curunit;
			} else
				evk = 0;
			evp += 2;
			if (evp[0] == 'A' && evp[1] == '_') {
				evp += 2;
				evslot = unitslot(*evp, &units[evk]);
				if (evslot == NULL)
					return 0;
				evv = baselook(*evslot);
				if (evp[1] == '_' && evp[2] == 'S') {
					if ((evv & 0xfff) == 0)
						evv = 7;
					else
						evv = blessof(evv);
				} else
					evv &= 0xfff;
				return evv;
			}
			if (evp[0] == 'F' && evp[1] == 'l' && evp[2] == 'u')
				return units[evk].cursed;
			if (*evp == 'A' || *evp == 'a') {
				if (evp[1] == 'k' && evp[2] == 't')
					return units[evk].mode;
				return units[evk].modea;
			}
			if (*evp == 'B' || *evp == 'b')
				return units[evk].modeb;
			if (*evp == 'C' || *evp == 'c')
				return units[evk].modec;
			if (*evp == 'D' || *evp == 'd')
				return units[evk].moded;
			if (*evp == 'E' || *evp == 'e')
				return units[evk].modee;
			if (*evp == 'F' || *evp == 'f')
				return units[evk].modef;
			if (*evp == 'G' || *evp == 'g')
				return units[evk].modeg;
		} else if ((evp[0] == 'v' || evp[0] == 'V') && evp[1] == '_') {
			evp += 2;
			if (*evp == 'A' || *evp == 'a')
				return vara;
			if (*evp == 'B' || *evp == 'b')
				return varb;
			if (*evp == 'C' || *evp == 'c')
				return varc;
			if (*evp == 'D' || *evp == 'd')
				return vard;
			if (*evp == 'E' || *evp == 'e')
				return vare;
			if (*evp == 'F' || *evp == 'f')
				return varf;
		} else if (ideqi(evp, texts + 0xa9fd))
			return wealth(0);
		else if (ideqi(evp, texts + 0xaa02))
			return wealth(1);
		else if (ideqi(evp, texts + 0xaa08))
			return wealth(2);
		else if (ideqi(evp, texts + 0xaa0d))
			return wealth(3);
		else if (evp[0] == 'K' && evp[1] == 'n' && evp[2] == 'r')
			return curmap;
		else if (evp[0] == 'T' && evp[1] == 'a' && evp[2] == 'b' && evp[3] == 's' && evp[4] == 'e' && evp[5] == 'n' && evp[6] == 't')
			return game.absenttime;
		else if (evp[0] == 'T' && evp[1] == 'w' && evp[2] == 'i' && evp[3] == 'e' && evp[4] == 'd' && evp[5] == 'e' && evp[6] == 'r' && evp[7] == 'd' && evp[8] == 'a') {
			if (evp[9] == 'T')
				return game.backday;
			if (gameday <= game.backday)
				return 0;
			return gameday - game.backday;
		} else if (evp[0] == 'k' && evp[1] == 'd' && evp[2] == 'i' && evp[3] == 'd') {
			if (!curunit)
				return 0;
			evn = regionat(units[curunit].x, units[curunit].y);
			if (evn != 0)
				return regions[evn].id;
			return 0;
		} else if (evp[0] == 'k' && evp[1] == 's' && evp[2] == 'd' && evp[3] == 'i' && evp[4] == 'd') {
			evn = regionat((*(short *)&playerx), (*(short *)&playery));
			if (evn != 0)
				return regions[evn].id;
			return 0;
		} else if (evp[0] == 'k' && evp[1] == 'z' && evp[2] == 'd' && evp[3] == 'i' && evp[4] == 'd') {
			if (evp[5] == 'N')
				evk = findunit(evp + 6);
			else if (evp[5] == 'n') {
				evm = number(evp + 5);
				if (evm < 0 || evm >= nunits)
					return 0;
				evk = evm;
			} else
				evk = curunit;
			if (!evk)
				return 0;
			if (scheduled(&units[evk]))
				evn = regionat(units[evk].postx[gamehour], units[evk].posty[gamehour]);
			else
				evn = regionat(units[evk].x, units[evk].y);
			if (evn != 0)
				return regions[evn].id;
			return 0;
		} else if (evp[0] == 'k' && evp[1] == 'd' && evp[2] == 's') {
			if (!curunit)
				return 0;
			if (evp[3] == 'n')
				return regions[units[curunit].sleepat].lord;
			return units[curunit].sleepat;
		} else if (evp[0] == 'k' && evp[1] == 'd' && evp[2] == 'a' && evp[3] == '1') {
			if (!curunit)
				return 0;
			if (evp[4] == 'n')
				return regions[units[curunit].workat1].lord;
			return units[curunit].workat1;
		} else if (evp[0] == 'k' && evp[1] == 'd' && evp[2] == 'a' && evp[3] == '2') {
			if (!curunit)
				return 0;
			if (evp[4] == 'n')
				return regions[units[curunit].workat2].lord;
			return units[curunit].workat2;
		} else if (evp[0] == 'k' && evp[1] == 'd' && evp[2] == 'e') {
			if (!curunit)
				return 0;
			if (evp[3] == 'n')
				return regions[units[curunit].eatat].lord;
			return units[curunit].eatat;
		} else if (evp[0] == 'k' && evp[1] == 'd' && evp[2] == 'f') {
			if (!curunit)
				return 0;
			if (evp[3] == 'n')
				return regions[units[curunit].freeat].lord;
			return units[curunit].freeat;
		} else if (evp[0] == 'k' && evp[1] == 'd') {
			if (!curunit)
				return 0;
			return regionat(units[curunit].x, units[curunit].y);
		} else if (evp[0] == 'k' && evp[1] == 'g' && evp[2] == 'i' && evp[3] == 'd') {
			if (!curunit)
				return 0;
			evn = provat(units[curunit].x, units[curunit].y);
			if (evn != 0)
				return provs[evn].id;
			return 0;
		} else if (evp[0] == 'k' && evp[1] == 'g' && evp[2] == '%' && evp[3] == 'k' && evp[4] == 'd') {
			evp += 5;
			if (!curunit)
				return 0;
			if (*evp == 's')
				return regions[units[curunit].sleepat].site;
			if (evp[0] == 'a' && evp[1] == '1')
				return regions[units[curunit].workat1].site;
			if (evp[0] == 'a' && evp[1] == '2')
				return regions[units[curunit].workat2].site;
			if (*evp == 'e')
				return regions[units[curunit].eatat].site;
			if (*evp == 'f')
				return regions[units[curunit].freeat].site;
			return 0;
		} else if (evp[0] == 'k' && evp[1] == 'g') {
			if (!curunit)
				return 0;
			return provat(units[curunit].x, units[curunit].y);
		} else if (ideqi(evp, texts + 0xab1a))
			return units[curunit].sympathy;
		else if (ideqi(evp, texts + 0xab1f)) {
			/* &units[curunit] as a sum whose first term is the product */
			// PORT: the unit by its index, as 64 bits truncate the address in a DWORD
#ifdef PORT
		if (isservant(&units[curunit]) && units[curunit].lord)
#else
		if (isservant((Unit *)(curunit * sizeof(Unit) + (DWORD)units)) && units[curunit].lord)
#endif
				return units[units[curunit].lord].sympathy;
			return units[curunit].sympathy;
		} else if (ideqi(evp, texts + 0xab25))
			return gameday;
		else if (ideqi(evp, texts + 0xab29))
			return gamehour;
		else if (ideqi(evp, texts + 0xab38)) {
			evm = homeof(&units[curunit]);
			if (!evm)
				return 0;
			if (regions[evm].site) {
				if (provs[regions[evm].site].mealday == gameday)
					return provs[regions[evm].site].meals;
				return 0;
			}
			if (regions[evm].mealday == gameday)
				return 9;
			return 0;
		} else if (ideqi(evp, texts + 0xab31)) {
			evm = homeof(&units[curunit]);
			if (!evm)
				return 0;
			if (regions[evm].site)
				return provs[regions[evm].site].mealday;
			return regions[evm].mealday;
		} else if (evp[0] == 'V' && evp[1] == 'e' && evp[2] == 'r' && evp[3] == 'w') {
			if (evp[4] == 't')
				return resthours(units);
			if (units[0].health < kindof(units)->maxhealth / 3)
				return 3;
			if (units[0].health < kindof(units)->maxhealth / 2)
				return 2;
			if (units[0].health < kindof(units)->maxhealth * 2 / 3)
				return 1;
			return 0;
		} else if (evp[0] == 'p' && evp[1] == 'r' && evp[2] == 'o' && evp[3] == 'v') {
			if (evp[4] == 't') {
				evn = foodhours();
				return evn / 24 + (evn % 24 >= 12);
			}
			return foodhours();
		} else if (evp[0] == 'S' && evp[1] == 'p' && evp[2] == 'W' && evp[3] == 'e' && evp[4] == 'i' && evp[5] == 'n' && evp[6] == 'A' && evp[7] == 'n' && evp[8] == 'z') {
			evm = 0;
			for (evn = 0; evn < 5; evn++)
				evm += counttypes(evn, 0x1e8, 0x1eb, 0);
			return evm;
		} else if (evp[0] == 'Z' && evp[1] == 'u' && evp[2] == 'f' && evp[3] >= '0' && evp[3] <= '9') {
			evm = number(evp + 3);
			evm = randn(evm);
			return evm;
		}
	} else if (*evp >= '0' && *evp <= '9')
		return number(evp);
	return 0;
}

/* expand the $ or % term at *src into *dst, advancing both */
void
expand(char **dst, char **src)
{
	if (**src == '$') {
		(*src)++;
		if ((*src)[0] == 'd' && (*src)[1] == 'o' && (*src)[2] == 'l' && (*src)[3] == 'r') {
			while (istokc(**src))
				(*src)++;
			*(*dst)++ = '$';
			**dst = 0;
			return;
		}
		if ((*src)[0] == 'w' && (*src)[1] == 'a' && (*src)[2] == 'n' && (*src)[3] == 'n') {
			exv = evalvar(*src + 4);
			while (istokc(**src))
				(*src)++;
			fmtday(exv);
			wsprintf(*dst, daytext);
			while (**dst)
				(*dst)++;
			return;
		}
		if ((*src)[0] == 'o' && (*src)[1] == 'b' && (*src)[2] == 'j') {
			exslot = unitslot((*src)[3], units);
			if (exslot && *exslot)
				exw = *exslot;
			else
				exw = 0;
			if ((*src)[4] == 'e') {
				wsprintf(*dst, youritem(exw, 0, dutalk));
				*src += 5;
			} else if ((*src)[4] == 'E') {
				wsprintf(*dst, youritem(exw, 1, dutalk));
				*src += 5;
			} else {
				wsprintf(*dst, texts + 0xb0f5, itemname(exw));
				*src += 4;
			}
			if (lang) {
				// FIXED: the case of an item's name that starts the speech, as at the answers of a priest's break spell in English, comes from 2 bytes before the speech. Take it for a sentence's start.
#ifdef FEATURE_FAULTS
				exc = *dst == speech || *dst == speech + 1 ? 0 : (*dst)[-2];
#else
				exc = (*dst)[-2];
#endif
				if (iswordc(exc) || exc == ',') {
					exc = **dst;
					**dst = lower(exc);
				}
				for ((*dst)++; **dst; (*dst)++) {
					exc = **dst;
					**dst = lower(exc);
				}
			}
			while (**dst)
				(*dst)++;
			return;
		}
		if ((*src)[0] == 'z' && (*src)[1] == 'u' && (*src)[2] == 's' && (*src)[3] == 't') {
			exslot = unitslot((*src)[4], units);
			if (exslot && *exslot) {
				wsprintf(*dst, weartext(wearof(*exslot)));
				while (**dst)
					(*dst)++;
			}
			*src += 5;
			return;
		}
		if ((*src)[0] == 's' && (*src)[1] == 'e' && (*src)[2] == 'g') {
			exslot = unitslot((*src)[3], units);
			if (exslot && *exslot) {
				wsprintf(*dst, strs[blessid(blessof(*exslot))]);
				while (**dst)
					(*dst)++;
			}
			*src += 4;
			return;
		}
		if (((*src)[0] == 'o' || (*src)[0] == 'O') && (*src)[1] == 'r' && (*src)[2] == 't') {
			*src += 3;
			exv = evalvar(*src);
			while (istokc(**src))
				(*src)++;
			if (exv >= 30)
				return;
			wsprintf(*dst, strs[zones[exv].name]);
			while (**dst)
				(*dst)++;
			return;
		}
		if (((*src)[0] == 's' || (*src)[0] == 'S') && (*src)[1] == 't' && (*src)[2] == 'a' && (*src)[3] == 'm' && (*src)[4] == 'm') {
			*src += 5;
			exv = evalvar(*src);
			while (istokc(**src))
				(*src)++;
			if (exv < 32) {
				if ((*src)[-1] == 'g')
					wsprintf(*dst, strs[1602 + exv * 4]);
				else if ((*src)[-1] == 'd')
					wsprintf(*dst, strs[1603 + exv * 4]);
				else if ((*src)[-1] == 'a')
					wsprintf(*dst, strs[1604 + exv * 4]);
				else
					wsprintf(*dst, strs[1601 + exv * 4]);
				while (**dst)
					(*dst)++;
			}
			return;
		}
		if ((*src)[0] == 'U' && (*src)[1] == 'e' && (*src)[2] == 'b' && (*src)[3] == 'R' && (*src)[4] == 'a' && (*src)[5] == 'c' && (*src)[6] == 'h' && (*src)[7] == 'N' && (*src)[8] == 'a' && (*src)[9] == 'm') {
			if (!avengers || victimname == 0)
				wsprintf(*dst, strs[1689]);
			else
				wsprintf(*dst, personname(victimname));
			while (**dst)
				(*dst)++;
			*src += 10;
			return;
		}
		if ((*src)[0] == 'U' && (*src)[1] == 'e' && (*src)[2] == 'b' && (*src)[3] == 'R' && (*src)[4] == 'a' && (*src)[5] == 'c' && (*src)[6] == 'h' && (*src)[7] == 'O' && (*src)[8] == 'r' && (*src)[9] == 't') {
			if (!avengers || victimzone == 0)
				wsprintf(*dst, strs[1690]);
			else
				wsprintf(*dst, strs[zones[victimzone].name]);
			while (**dst)
				(*dst)++;
			*src += 10;
			return;
		}
		if ((*src)[0] == 'w' && (*src)[1] == 'o' && (*src)[2] == 'd' && (*src)[3] == 'g' && (*src)[4] == 'u' && (*src)[5] == 'n') {
			wsprintf(*dst, strs[432 + favor()]);
			while (**dst)
				(*dst)++;
			*src += 6;
			return;
		}
		if ((*src)[0] == 'z' && (*src)[1] == 'u' && (*src)[2] == 'k' && (*src)[3] == 'u' && (*src)[4] == 'n' && (*src)[5] == 'f' && (*src)[6] == 't') {
			listvisions();
			foretell(*dst, curunit, curmap);
			while (**dst)
				(*dst)++;
			*src += 7;
			return;
		}
		if ((*src)[0] == 'w' && (*src)[1] == 'r' && (*src)[2] == 'k' && (*src)[3] == 'z') {
			wsprintf(*dst, strs[1137]);
			while (**dst)
				(*dst)++;
			*src += 4;
			return;
		}
		if ((*src)[0] == 'g' && (*src)[1] == 'e' && (*src)[2] == 's' && (*src)[3] == 'u' && (*src)[4] == 'n' && (*src)[5] == 'd') {
			fmthealth(*dst);
			while (**dst)
				(*dst)++;
			while ((*dst)[-1] == 13 || (*dst)[-1] == 10 || (*dst)[-1] == ' ')
				(*dst)--;
			**dst = 0;
			*src += 6;
			return;
		}
		if ((*src)[0] == 'r' && (*src)[1] == 'u' && (*src)[2] == 'n' && (*src)[3] == 'e' && (*src)[4] == '_') {
			if ((*src)[5] == 'a')
				exv = runepast;
			else if ((*src)[5] == 'b')
				exv = runenow;
			else if ((*src)[5] == 'c')
				exv = runenext;
			else
				exv = 0xffff;
			if (exv != 0xffff) {
				wsprintf(*dst, runesense(exv));
				while (**dst)
					(*dst)++;
				while ((*dst)[-1] == 13 || (*dst)[-1] == 10 || (*dst)[-1] == ' ')
					(*dst)--;
				**dst = 0;
			}
			*src += 6;
			return;
		}
		if (**src == 'N') {
			(*src)++;
			if (**src >= '0' && **src <= '9') {
				exw = evalvar(*src);
				while (**src >= '0' && **src <= '9')
					(*src)++;
				exs = personname(exw);
				while ((**dst = *exs++) != 0)
					(*dst)++;
				if (**src == 's' && lang != 4) {
					if (lang == 1) {
						*(*dst)++ = '\'';
						if ((*dst)[-2] == 's')
							(*src)++;
					} else if (lang == 0 && (*dst)[-1] == 's') {
						*(*dst)++ = '\'';
						(*src)++;
					}
				}
			} else {
				exs = unitname(&units[curunit]);
				while ((**dst = *exs++) != 0)
					(*dst)++;
			}
			**dst = 0;
			return;
		}
		if ((*src)[0] == 'S' && (*src)[1] == 'R') {
			exs = game.latinname;
			*src += 2;
			while ((**dst = *exs++) != 0)
				(*dst)++;
			return;
		}
		if (**src == 'S') {
			(*src)++;
			exs = ((char *)&game);
			while ((**dst = *exs++) != 0)
				(*dst)++;
			return;
		}
		if (**src == 'H') {
			(*src)++;
			if (!isservant(&units[curunit]))
				return;
			if (units[units[curunit].lord].number)
				units[units[curunit].lord].number |= 0x2000;
			exs = unitname(&units[units[curunit].lord]);
			while ((**dst = *exs++) != 0)
				(*dst)++;
			return;
		}
		if ((*src)[0] == 'w' && (*src)[1] == 'e' && (*src)[2] == 'r' && (*src)[3] == 'b') {
			*src += 4;
			if (**src < '0' || **src > '9')
				return;
			exw = evalvar(*src);
			while (**src >= '0' && **src <= '9')
				(*src)++;
			if (exw > 9)
				return;
			exs = strs[1825 + exw];
			while ((**dst = *exs++) != 0)
				(*dst)++;
			return;
		}
	} else if (**src == '%') {
		exv = evalvar(*src);
		(*src)++;
		if (((*src)[0] == 'P' || (*src)[0] == 'p') && ((*src)[1] == 't' || (*src)[1] == 'T' || (*src)[1] == 'b' || (*src)[1] == 'B' || (*src)[1] == '_'))
			wsprintf(*dst, texts + 0xb3b8, exv, strs[exv < 2 ? 842 : 843]);
		else if ((*src)[0] == 'O' || (*src)[0] == 'o')
			wsprintf(*dst, texts + 0xb3bf, exv);
		else
			wsprintf(*dst, texts + 0xb3c3, exv);
		while (**dst)
			(*dst)++;
		while (isvarc(**src))
			(*src)++;
	}
}

void
expandstr(char *dst, char *src)
{
	while (*src != 0) {
		if (*src == '%' || *src == '$')
			expand(&dst, &src);
		else
			*dst++ = *src++;
	}
	*dst = 0;
}

/* find the first { of the scene from p on */
int
findbrace(char *p)
{
	scriptpos = p;
	do {
		if (*scriptpos == '{')
			return 1;
	} while (++scriptpos < workbuf + scriptlen);
	return 0;
}

/* find the innermost node around scriptpos */
void
innernode(void)
{
	currentnode = 0;
	for (nodei = 0, nodebest = 32000; nodei < nnodes; nodei++) {
		nodestart = nodes[nodei].start;
		nodeend = nodes[nodei].end;
		if (nodestart <= scriptpos && nodeend >= scriptpos) {
			nodelen = nodeend - nodestart;
			if (nodelen <= nodebest) {
				currentnode = nodei;
				nodebest = nodelen;
			}
		}
	}
}

/* index the lines "// n" of the script text by n */
void
indexmarks(char *text, DWORD len)
{
	memset(markoff, 0, 0x578);
	nscenes = 0;
	markp = text;
	while (markp < text + len) {
		while (*markp != 13) {
			if (markp > text + len)
				goto out;
			markp++;
		}
		markp = skipspace(markp);
		if (markp[0] == '/' && markp[1] == '/' && markp[2] == ' ' && markp[3] >= '0' && markp[3] <= '9') {
			markp += 3;
			markoff[nscenes] = markp - text;
			markn = number(markp);
			marknum[nscenes] = markn;
			nscenes++;
		}
	}
out:
	markoff[nscenes] = markp - text;
}

int
loadscript(char *name)
{
	loadok = 1;
	if (name == NULL || *name == 0) {
		if (lang == scriptlang)
			return 0;
		scriptlang = lang;
	}
	memset(workbuf, 0, 0x96640);
	if (name != NULL && *name != '\0') {
		if (!fopenr(&loadh, name)) {
			loadok = 0;
			goto builtin;
		}
		loadlen = fileread(packbuf, 1, 0x96640, loadh);
		fileclose(&loadh);
		loadn = 0;
		for (loadi = 0; loadi < loadlen && loadi < 0x96640; workbuf[loadn++] = packbuf[loadi++])
			if (packbuf[loadi] == 13 || packbuf[loadi] == 10) {
				if (packbuf[loadi] == 13) {
					workbuf[loadn++] = 13;
					loadi++;
					workbuf[loadn++] = 10;
				}
				while (packbuf[loadi] == 10 || packbuf[loadi] == 32)
					loadi++;
			}
		loadlen = loadn;
		goto done;
	}
builtin:
	loadn = 0;
	if (!seekentry(langentry(lang + 0x200d)))
		return 0;
	loadlen = storedn;
	fileread(packbuf, loadlen, 1, datfh);
	loadn += unpack(workbuf, 0x96640, packbuf, loadlen);
	loadlen = loadn;
	for (loadi = 0; loadi < loadlen && loadi < 0x96640; loadi++)
		workbuf[loadi] ^= 0xfe;
done:
	indexmarks(workbuf, loadlen);
	for (loadi = 0; loadi < loadlen; loadi++)
		script[loadi] = workbuf[loadi] ^ 0xfe;
	scriptsize = loadlen;
	memset(choices, 0, sizeof choices);
	choices[1].start = strs[438];
	text3w(strs[438]);
	choices[1].width = text3width;
	choices[1].x = 0x98;
	choices[1].shown = 1;
	choices[2].start = strs[439];
	text3w(strs[439]);
	choices[2].width = text3width;
	choices[2].x = choices[1].x + choices[1].width + 24;
	choices[2].shown = 1;
	choices[3].start = strs[440];
	text3w(strs[440]);
	choices[3].width = text3width;
	choices[3].x = choices[1].x;
	choices[3].shown = 0;
	resumeptr = speech;
	return loadok;
}

void
settext(unsigned short i, char *s)
{
	if (*s == '!')
		s++;
	nodes[i].text = s;
	text3w(s);
	nodes[i].width = text3width;
}

/* expand the terms of the nodes that hold $ or % into nodetexts */
int
expandnodes(void)
{
	reexpand = 0;
	if (!hasterms)
		return 1;
	memset(nodetexts, 0, sizeof nodetexts);
	nodetextp = nodetexts + 4;
	for (expi = 1; expi <= nnodes; expi++) {
		if (nodes[expi].flags & 2) {
			if (nodetextp >= nodetexts + sizeof nodetexts - 50)
				return 0;
			exps = nodes[expi].src;
			nodes[expi].text = nodetextp;
			for (;;) {
				if (*exps == 0 || *exps == 13 || *exps == '{')
					break;
				if (*exps == '%' || *exps == '$')
					expand(&nodetextp, &exps);
				else
					*nodetextp++ = *exps++;
			}
			*nodetextp++ = *exps;
			*nodetextp++ = 0;
			settext(expi, nodes[expi].text);
		}
	}
	return 1;
}

DWORD
unpackvars(char *dst, DWORD n)
{
	memset(dst, 0, n);
	if (*(DWORD *)scenevars == 0xf4f9f4f9)
		unpackn = unpack(dst, n, scenevars + 4, scenevarlen - 4);
	else
		unpackn = unlzw((unsigned char *)dst, (unsigned char *)scenevars, scenevarlen);
	return unpackn;
}

void
packvars(char *src, DWORD n)
{
	*(DWORD *)scenevars = 0xf4f9f4f9;
	scenevarlen = pack(scenevars + 4, sizeof scenevars - 4, src, n) + 4;
}

/* parse the scene of the current mark into nodes */
int
parsescene(void)
{
	/*
	 * unused: a temporary of the code generator, which gives the
	 * parse loop the original's imul by 0x4c
	 */
	int u0;

	if (informal(&units[curunit]))
		dutalk = 1;
	else
		dutalk = 0;
	hasterms = 0;
	if ((DWORD)curscene <= 80)
		return 0;
	for (sceneidx = 0; sceneidx < nscenes; sceneidx++)
		if (marknum[sceneidx] == curscene)
			goto found;
	return 0;
found:
	memset(sceneflags, 0, 4);
	memset(curvars, 0, 0x46);
	memset(sceneitems, 0, 0xa0);
	memset(scenerest, 0, 0x42);
	parsei = unpackvars(workbuf, 0x96640);
	if (parsei <= sceneidx * 300)
		;
	else {
		parsep = workbuf + sceneidx * 300;
		memcpy(sceneflags, parsep, 4);
		parsep = parsep + 4;
		memcpy(curvars, parsep, 0x46);
		parsep = parsep + 0x46;
		memcpy(sceneitems, parsep, 0xa0);
		parsep = parsep + 0xa0;
		memcpy(scenerest, parsep, 0x42);
	}
	parseoff = markoff[sceneidx];
	scriptlen = markoff[sceneidx + 1] - parseoff;
	if (scriptlen >= 3)
		scriptlen -= 3;
	memset(workbuf, 0, 0x96640);
	memcpy(workbuf, script + parseoff, scriptlen);
	for (parsei = 0; parsei < scriptlen; parsei++)
		workbuf[parsei] ^= 0xfe;
	memset(nodes, 0, sizeof nodes);
	memset(talkflags, 0, 4);
	nnodes = 0;
	for (parsei = 0; parsei < 255; parsei++)
		nodes[parsei].src = nodes[parsei].text = texts + 0xa562;
	parsep = workbuf + 1;
	findbrace(workbuf);
	homenode = 0xff;
	dealok = 0xff;
	tradekind = 0;
	do {
		switch (*parsep) {
		case 0:
			return 0;
		case '"':
		quote:
			nodes[nnodes].start = ++parsep;
			while (*parsep != '{')
				parsep++;
			goto link;
		case '#':
			nodes[nnodes].start = ++parsep;
			while (*parsep != '#' && *parsep)
				parsep++;
			parsep++;
			if (*parsep != '{') {
				parseq = nodes[nnodes].src = nodes[nnodes].text = parsep;
				for (; *parseq != '{' && *parseq != 13 && *parseq; parseq++)
					if (*parseq == '$' || *parseq == '%') {
						nodes[nnodes].flags |= 2;
						hasterms = 1;
						break;
					}
				while (*parsep != '{' && *parsep != 13 && *parsep)
					parsep++;
				settext(nnodes, nodes[nnodes].text);
			}
		link:
			for (parsei = 1; parsei <= nnodes; parsei++)
				if (nodes[nnodes - parsei].end == 0) {
					nodes[nnodes].parent = nnodes - parsei;
					nodes[nnodes - parsei].child[nodes[nnodes - parsei].nchild++] = nnodes;
					break;
				}
			nnodes++;
			break;
		case '{':
			parsep--;
			goto quote;
		case '}':
			for (parsei = 1; parsei <= nnodes; parsei++)
				if (nodes[nnodes - parsei].end == 0) {
					nodes[nnodes - parsei].end = parsep;
					if (nnodes == parsei)
						return 1;
					break;
				}
			break;
		}
	} while (++parsep < workbuf + scriptlen);
	return 0;
}

/* save the scene's state into its 300 bytes of the packed variables */
void
savevars(void)
{
	memset(workbuf, 0, (sceneidx + 1) * 300);
	svn = unpackvars(workbuf, 0x96640);
	svp = workbuf + sceneidx * 300;
	memcpy(svp, sceneflags, 4);
	svp += 4;
	memcpy(svp, curvars, 0x46);
	svp += 0x46;
	memcpy(svp, sceneitems, 0xa0);
	svp += 0xa0;
	memcpy(svp, scenerest, 0x42);
	if ((sceneidx + 1) * 300 > svn)
		svn = (sceneidx + 1) * 300;
	packvars(workbuf, svn);
	vara = 0;
	varb = 0;
	varc = 0;
	vard = 0;
	vare = 0;
	varf = 0;
}

/* clear the state of every scene */
void
clearvars(char *buf)
{
	clrn = nscenes;
	clrn *= 300;
	memset(buf, 0, clrn);
	packvars(buf, clrn);
}

/* append the items of tradegoods to the dated list from slot 10 on */
void
addorders(void)
{
	for (vi = 10; sceneitems[vi] != 0; vi++)
		;
	if (vi > 10)
		vday = curvars[vi - 1];
	else
		vday = gameday;
	vj = 0;
	do {
		vitem = tradegoods[vj];
		switch (classof(vitem & 0xfff)) {
		case 0x3c:
			vday += metals[gearof(vitem)].days;
			break;
		case 0x41:
		case 0x46:
		case 0x48:
		case 0x4b:
		case 0x50:
		case 0x55:
			vday += armors[gearof(vitem)].days;
			break;
		default:
			vday++;
			break;
		}
		sceneitems[vi] = vitem;
		curvars[vi] = vday;
		vi++;
		vj++;
	} while (vj < goodsprice);
}

/* list the dated items from slot 10 on, with their days, into buf */
void
listitems(char *buf)
{
	for (liti = 10; (litem = sceneitems[liti]) != 0; liti++) {
		wsprintf(buf, youritem(litem, 0, dutalk));
		buf[0] = upper(buf[0]);
		if (lang) {
			if (liti == 10)
				buf++;
			lowerstr(buf);
		}
		while (*buf)
			buf++;
		fmtday(curvars[liti]);
		if (liti == 10)
			readytext(buf, litem, daytext);
		else {
			*buf++ = ' ';
			wsprintf(buf, daytext);
		}
		while (*buf)
			buf++;
		if (sceneitems[liti + 1] == 0) {
			wsprintf(buf, ".\r");
			return;
		}
		if (sceneitems[liti + 2] == 0)
			wsprintf(buf, strs[904]);
		else
			wsprintf(buf, ", ");
		while (*buf)
			buf++;
	}
	if (liti <= 10)
		wsprintf(buf, strs[905]);
}

/* the player's health as text into buf */
void
fmthealth(char *buf)
{
	char *s;

	health = healthtext(units);
	if (health == strs[542]) {
		if (units[0].poison) {
			wsprintf(buf, strs[dutalk ? 1088 : 1087], health);
			return;
		}
		if (game.hungercount >= 480) {
			wsprintf(buf, strs[dutalk ? 1090 : 1089], health, strs[1414]);
			return;
		}
		goto general;
	}
	if (health != strs[543]) {
general:
		if (units[0].poison)
			s = strs[1091];
		else if (game.hungercount >= 1680)
			s = strs[1414];
		else
			s = empty;
		wsprintf(buf, strs[dutalk ? 1090 : 1089], health, s);
		return;
	}
	if (units[0].poison) {
		if (game.hungercount >= 1680) {
			wsprintf(buf, strs[dutalk ? 1088 : 1087], strs[1413]);
			return;
		}
		wsprintf(buf, strs[dutalk ? 1093 : 1092]);
		return;
	}
	if (game.hungercount >= 480) {
		wsprintf(buf, strs[dutalk ? 1090 : 1089], strs[1413], empty);
		return;
	}
	wsprintf(buf, strs[dutalk ? 1095 : 1094]);
}

/* the player's health and the day of the cure, with its price when x is 0, into speech */
void
fmtcure(short x)
{
	fmthealth(speech);
	healp = speech;
	while (*healp)
		healp++;
	goodsprice = resthours(units);
	fmtday(gameday + goodsprice);
	staylen = goodsprice;
	if (x) {
		goodsprice = 0;
		daytext[0] = upper(daytext[0]);
		wsprintf(healp, strs[dutalk ? 1099 : 1098], daytext);
	} else {
		goodsprice *= 40;
		goodsprice = charmprice(goodsprice) / 50;
		if (!goodsprice)
			goodsprice++;
		goodsprice *= 50;
		wsprintf(healp, strs[dutalk ? 1097 : 1096], goodsprice / 10, daytext);
	}
}

/* strs[449], or a random one of strs[441] to strs[448], into speech */
void
replyno(int x)
{
	if (isservant(&units[curunit]))
		wsprintf(speech, texts + 0xb23b, strs[449]);
	else
		wsprintf(speech, texts + 0xb23b, strs[441 + (dutalk || x) + randn(4) * 2]);
}

/* three new random picks, with their letters, into speech */
void
runereading(void)
{
	castrunes(0);
	wsprintf(speech, strs[1209], runeletter(runepast), runeletter(runenow), runeletter(runenext),
		runesense(runepast), runesense(runenow), runesense(runenext));
}

/* strs[458], strs[861], or a random one of strs[450] to strs[459], into speech */
void
replydeal(void)
{
	if (myservant(&units[curunit])) {
		wsprintf(speech, texts + 0xb23b, strs[458]);
		return;
	}
	rdpick = randn(5);
	if (rdpick == 4)
		wsprintf(speech, texts + 0xb23b, strs[861]);
	else
		wsprintf(speech, texts + 0xb23b, strs[450 + (dutalk ? 1 : 0) + rdpick * 2]);
}

unsigned long
takefrom(Unit *u, unsigned long w)
{
	tfslot = wornslot(u, w, 0);
	if (tfslot) {
		tfgear = gearslot(u, tfslot);
		if (tfgear) {
			w = *tfgear;
			*tfgear = 0;
			calcstats(u);
		}
		return w;
	}
	tfslot = finditem(u, w, &tfrealm, &tfcell, &tfidx, 0);
	if (tfslot == 0)
		return 0;
	if (tfrealm == 0)
		tfslot = takeitem(tfcell, tfidx);
	else
		tfslot = takeout(tfrealm, tfidx);
	if (tfslot)
		w = tfslot;
	return w;
}

/* set flag n of the game, clearing its opposite, unless the opposite of 0x1d to 0x1f is set */
void
// PORT: a name of its own, which C wants for an overloaded function
#ifdef PORT
setknown(unsigned n)
#else
setflag(unsigned n)
#endif
{
	if (n >= 0xf0)
		return;
	// FIXED: the reward comes before the test that refuses 0x1d to 0x1f under their opposites, so a refused action pays on every call. Refuse first.
#if defined(FEATURE_FIXES)
	if (n == 0x1d && getbit(game.known, 0x15) || n == 0x1e && getbit(game.known, 0x16) || n == 0x1f && getbit(game.known, 0x17))
		return;
#endif
	if (!getbit(game.known, n)) {
		addscore(0x8c);
		learnaction();
	}
	switch (n) {
	case 0x15:
		setbit(game.known, 0x1d, 0);
		break;
	case 0x16:
		setbit(game.known, 0x1e, 0);
		break;
	case 0x17:
		setbit(game.known, 0x1f, 0);
		break;
	case 0x1d:
		if (getbit(game.known, 0x15))
			return;
		break;
	case 0x1e:
		if (getbit(game.known, 0x16))
			return;
		break;
	case 0x1f:
		if (getbit(game.known, 0x17))
			return;
		break;
	}
	setbit(game.known, n, 1);
}

/* run the commands of a line of the script and return what follows it */
short
runcmds(char *p)
{
	cmdret = 0;
	for (;;) {
		if (*p == 13 || *p == '\0')
			break;
		if (*p == '%') {
			cmdvar = NULL;
			p++;
			if ((*p == 's' || *p == 'S') && (p[1] == 'z' || p[1] == 'Z')) {
				p += 2;
				cmdn = number(p);
				if (cmdn < 200)
					cmdvar = &((short *)szvars)[cmdn];
			} else if (*p == '0' && p[1] == 'N' && p[2] >= '0' && p[2] <= '2') {
				cmdvar = &units[curunit].vars[p[2] - '0'];
				p += 2;
			} else if (*p >= '0' && *p <= '9') {
				cmdn = number(p);
				if (cmdn < 35)
					cmdvar = &curvars[cmdn];
			}
			if (cmdvar) {
				while (*p >= '0' && *p <= '9')
					p++;
				if (*p == '=') {
					cmdn = evalvar(p + 1);
					*cmdvar = cmdn;
				} else if (*p == '+') {
					cmdn = evalvar(p + 1);
					*cmdvar += cmdn;
				} else if (*p == '-') {
					cmdn = evalvar(p + 1);
					*cmdvar -= cmdn;
				} else if (*p == '*') {
					cmdn = evalvar(p + 1);
					*cmdvar *= cmdn;
				} else if (*p == ':' || *p == '/') {
					cmdn = evalvar(p + 1);
					*cmdvar /= (short)cmdn;
				}
			}
			p = skipword(p);
			continue;
		}
		if (*p >= 'A' && *p <= 'Z' && (p[1] == '0' || p[1] == '1')) {
			setbit(sceneflags, *p - 'A', p[1] - '0');
			p += 2;
			continue;
		}
		if (*p == 'N' && p[1] >= 'A' && p[1] <= 'H' && (p[2] == '0' || p[2] == '1')) {
			setbit(&units[curunit].scriptflags, p[1] - 'A', p[2] - '0');
			p += 3;
			continue;
		}
		if (*p >= 'a' && *p <= 'z' && (p[1] == '0' || p[1] == '1')) {
			setbit(talkflags, *p - 'a', p[1] - '0');
			p += 2;
			continue;
		}
		if (p[0] == 'a' && p[1] == 'b' && p[2] == 'l') {
			innernode();
			currentnode = nodes[currentnode].parent;
			scriptpos = nodes[currentnode].start;
			return 0xff0;
		}
		if (p[0] == 'a' && p[1] == 'r' && p[2] == 'b') {
			p += 3;
			continue;
		}
		/* defect: anz leaves p where it is, and runs again */
		if (p[0] == 'a' && p[1] == 'n' && p[2] == 'z') {
			if (palblack)
				fadeslow(1);
			continue;
		}
		if (p[0] == 'c' && p[1] == 'l' && p[2] == 's') {
			cmdret = 0xffb;
			p = skipword(p);
			continue;
		}
		if (p[0] == 'c' && p[1] == 'l' && p[2] == 'a') {
			lastspeech = 2;
			p = skipword(p);
			continue;
		}
		if (p[0] == 'd' && p[1] == 'a' && p[2] == 'n' && p[3] == 'n' && p[4] == 'i' && p[5] == 'c' && p[6] == 'h') {
			replyno(p[7] == 'd' ? 1 : 0);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'r' && p[1] == 'u' && p[2] == 'n' && p[3] == 'e' && p[4] == 'n') {
			runereading();
			p = skipword(p);
			continue;
		}
		if (p[0] == 'd' && p[1] == 'l' && p[2] == 'o' && p[3] == 'g') {
			if (p[4] >= '0' && p[4] <= '9') {
				cmdp = &units[curunit].scene;
				cmdn = evalvar(p + 4);
			} else if (p[4] == 'v') {
				if (units[curunit].health <= 0)
					goto dlogend;
				cmdp = &units[curunit].plea;
				cmdn = evalvar(p + 5);
			} else if (p[4] == 's') {
				cmdp = &units[curunit].mercy;
				cmdn = evalvar(p + 5);
			} else {
				cmdp = &units[curunit].scene;
				cmdn = evalvar(p + 5);
			}
			*cmdp = cmdn;
		dlogend:
			p = skipword(p);
			continue;
		}
		if (p[0] == 'e' && p[1] == 'i' && p[2] == 'g' && p[3] == 't') {
			cmdn = evalvar(p + 4);
			if (cmdn < nunits) {
				units[curunit].lord = cmdn;
				flagunit(&units[curunit]);
			}
			p = skipword(p);
			continue;
		}
		if (p[0] == 'e' && p[1] == 'n' && p[2] == 'd')
			return 0xfff;
		if (p[0] == 'e' && p[1] == 'r' && p[2] == 'g' && p[3] == 'e' && p[4] == 'b') {
			if (!(units[curunit].traits & 0x20))
				deeddefeat(&units[curunit]);
			msgf(0x63, strs[340], unitname(&units[curunit]));
			abdicate(&units[curunit], 0);
			handover(&units[curunit], units);
			units[curunit].traits |= 0x20;
			makepeace(&units[curunit], units);
			p += 5;
			music(0x23, 0);
			continue;
		}
		if (p[0] == 'e' && p[1] == 's' && p[2] == 's' && p[3] == 'e' && p[4] == 'n') {
			cmdk = homeof(&units[curunit]);
			if (cmdk) {
				regions[cmdk].mealday = gameday;
				if (regions[cmdk].lord)
					for (cmdi = 1; cmdi < nregions; cmdi++)
						if (cmdk != cmdi && regions[cmdi].lord == regions[cmdk].lord)
							regions[cmdi].mealday = gameday;
			}
			cmdk = regions[cmdk].site;
			if (!cmdk) {
				p += 5;
				continue;
			}
			if (gameday == provs[cmdk].mealday)
				provs[cmdk].meals++;
			else {
				provs[cmdk].mealday = gameday;
				provs[cmdk].meals = 1;
			}
			p += 5;
			continue;
		}
		if (p[0] == 'f' && p[1] == 'c' && p[2] == 'k') {
			p = skipword(p);
			return 0xfed;
		}
		if (p[0] == 'f' && p[1] == 'l' && p[2] == 'a' && p[3] == 'g') {
			cmdn = evalvar(p + 4);
			/* defect: a flag over 1600 leaves p where it is, and runs again */
			if (cmdn > 1600)
				continue;
			for (p += 5; *p != ','; p++)
				if (*p == 13)
					goto next;
			if (*++p == '1') {
				if (!getbit(game.flags, cmdn)) {
					learnflag();
					setflag(cmdn, 1);
				}
			} else
				setflag(cmdn, 0);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'f' && p[1] == 'l' && p[2] == 'u' || p[0] == 's' && p[1] == 'e' && p[2] == 'g') {
			if (p[3] == '_' && p[4] == 'b') {
				units[0].cursed = 0;
				units[0].blessed = 0;
				units[0].bleeding = 0;
			} else if (p[3] >= 'a' && p[3] <= 'z') {
				cmdslot = unitslot(p[3], units);
				if (cmdslot && *cmdslot) {
					cmdk = blessof(*cmdslot);
					cmdn = evalvar(p + 4);
					if (cmdk == 7 || cmdk < 7 && (cmdn < cmdk || cmdn >= 7) || cmdk > 7 && (cmdn > cmdk || cmdn <= 7))
						if (!keepbless(curmap, *cmdslot, cmdk, cmdn))
							if (cmdk >= cmdn && jewelmagic(units, 0x80000))
								;
							else
								setbless(cmdslot, cmdn);
				}
			} else if (p[0] == 'f') {
				if (!jewelmagic(units, 0x80000)) {
					units[0].blessed = 0;
					units[0].cursed = 0xff;
				}
			} else {
				units[0].cursed = 0;
				units[0].blessed = 0xff;
			}
			calcstats(units);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'f' && p[1] == 'o' && p[2] == 'r' && p[3] == 't') {
			gobattle = 0;
			if (p[4] == 'g') {
				p += 5;
				sendposx = evalvar(p);
				if (sendposx > 0 && sendposx < 150) {
					p = skipfield(p) + 1;
					sendposy = evalvar(p);
					if (sendposy > 0 && sendposy < 150)
						return 0xff1;
				}
				sendposx = 0;
				sendposy = 0;
				continue;
			}
			if (p[4] == '2')
				return 0xff4;
			units[curunit].home = 0;
			return 0xffe;
		}
		if (p[0] == 'f' && p[1] == 'r' && p[2] == 'e' && p[3] == 'i' && p[4] == 'l') {
			if (myservant(&units[curunit])) {
				units[curunit].clan = 0xff;
				units[curunit].lord = 0;
				units[curunit].flags &= 0xf5;
				units[curunit].flags |= 8;
				units[curunit].home = 0;
				units[curunit].traits &= 0xdf;
				units[curunit].sympathy = isfemale(&units[curunit]) ? 4 : 3;
				flagunit(&units[curunit]);
			}
			p += 5;
			continue;
		}
		if (p[0] == 'f' && p[1] == 'r' && p[2] == 'i' && p[3] == 'd') {
			if (p[4] == 'a') {
				p++;
				peaceall(&units[curunit], units);
			} else
				peacehouse(&units[curunit], units);
			p += 4;
			continue;
		}
		if (p[0] == 'f' && p[1] == 't' && p[2] == 'o' && p[3] == 'd' && p[4] == 'f') {
			p += 5;
			nomercy(&units[curunit]);
			continue;
		}
		if (p[0] == 'g' && (p[1] == '@' || p[1] == 'n' && p[2] == '@')) {
			if (p[1] == 'n') {
				lbln = 1;
				p++;
			} else
				lbln = 0;
			p += 2;
			for (lblp = nodes[0].start + 1; lblp < nodes[0].end; lblp++)
				if (*lblp == '@' && lblp[-1] != 'g' && lblp[-2] != 'g') {
					lblp++;
					for (lblk = 0; p[lblk] != ' ' && p[lblk] != 13; lblk++)
						if (p[lblk] != lblp[lblk])
							goto nolabel;
					while (*lblp != '\0' && *lblp != 13)
						lblp++;
					scriptpos = ++lblp;
					innernode();
					if (lbln)
						return 0xff0;
					return 0xffa;
				nolabel:
					;
				}
			p = skipword(p);
			continue;
		}
		if (p[0] == 'g' && p[1] == 'n' && p[2] == '0') {
			if (p[3] == 's' && p[4] == 'e' && p[5] == 't') {
				homenode = currentnode;
				p = skipword(p);
				continue;
			}
			if (homenode == 0xff)
				currentnode = 0;
			else
				currentnode = homenode;
			scriptpos = nodes[currentnode].start;
			return 0xff0;
		}
		if (p[0] == 'g' && p[1] == 'a' && p[2] == 's' && p[3] == 't') {
			game.hungercount -= 60;
			if (game.hungercount < 0)
				game.hungercount = 0;
			game.tiredcount -= 20;
			if (game.tiredcount < 0)
				game.tiredcount = 0;
			setlevels();
			calcstats(units);
			p += 4;
			continue;
		}
		if (p[0] == 'g' && p[1] == 'a') {
			cmdslot = unitslot(p[2], &units[curunit]);
			if (cmdslot) {
				cmdw = evalvar(p + 3);
				*cmdslot = cmdw;
				calcstats(&units[curunit]);
			}
			p = skipword(p);
			continue;
		}
		if (p[0] == 'g' && p[1] == 'e' && p[2] == 'i' && p[3] == 'g') {
			abdicate(&units[curunit], p[4] == 'w' ? 1 : 0);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'g' && p[1] == 'g') {
			cmdn = (evalvar(p + 2) << 16) + 0x4e9;
			addcoins(&cmdn, 0);
			giveitem(0, cmdn, 1);
			reexpand = 1;
			p = skipword(p);
			continue;
		}
		if (p[0] == 'g' && p[1] == 'o') {
			cmdw = evalvar(p + 2);
			if (cmdw && (cmdw & 0xfff) < nobjtiles) {
				cmdn = takefrom(&units[curunit], cmdw);
				if (cmdn)
					cmdw = cmdn;
				giveitem(0, cmdw, 1);
				reexpand = 1;
			}
			p = skipword(p);
			continue;
		}
		if (p[0] == 'g' && p[1] == 's') {
			cmdn = (evalvar(p + 2) << 16) + 0x193;
			giveitem(0, cmdn, 1);
			reexpand = 1;
			p = skipword(p);
			continue;
		}
		if (p[0] == 'g' && p[1] == 't' && p[2] == 'a' && p[3] == 's' && p[4] == 'c' && p[5] == 'h') {
			if (curunit)
				for (cmdj = 0; cmdj < 3; cmdj++)
					if (units[curunit].slot[cmdj + 9]) {
						giveitem(0, units[curunit].slot[cmdj + 9], 1);
						reexpand = 1;
						units[curunit].slot[cmdj + 9] = 0;
					}
			p = skipword(p);
			continue;
		}
		if (p[0] == 'g' && p[1] == 'w' && p[2] == 'r' && p[3] == 't') {
			handover(&units[curunit], units);
			abdicate(&units[curunit], 0);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'g' && p[1] == 'z') {
			if (p[2] == '_') {
				cmdn = evalvar(p + 3);
				// PORT: a name of its own, which C wants for an overloaded function
#ifdef PORT
				setknown(cmdn);
#else
				setflag(cmdn);
#endif
			} else {
				cmdn = evalvar(p + 2);
				if (cmdn < 0xf0 && !getbit(game.known, cmdn)) {
					hidecursor();
					primto2();
					s2toback();
					setbit(game.known, cmdn, 0);
					drawactions();
					flip();
					s2toback();
					// PORT: a name of its own, which C wants for an overloaded function
#ifdef PORT
					setknown(cmdn);
#else
					setflag(cmdn);
#endif
					drawactions();
					waitinput(1000);
					dissolvet();
					waitinput(1500);
					s2toprim();
					drawcursor();
					waitrelease(1);
				}
			}
			p = skipword(p);
			continue;
		}
		if (p[0] == 'g' && p[1] == 'e') {
			cmdn = evalvar(p + 2);
			if (cmdn < 30)
				train((unsigned char)cmdn, 0x23);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'g' && p[1] == 'r' && p[2] == 'p') {
			cmdn = evalvar(p + 3);
			if (cmdn < 0xff)
				units[curunit].clan = cmdn;
			p = skipword(p);
			continue;
		}
		if (p[0] == 'h' && p[1] == 'b' && p[2] == 'l' && p[3] == 'k') {
			setslotflag(p[4]);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'h' && p[1] == 'a' && p[2] == 'n' && p[3] == 'd' && p[4] == 'e' && p[5] == 'l') {
			tradekind = 0;
		trade:
			memset(tradegoods, 0, 0x60);
			memset(tradeoffer, 0, 0x60);
			dealok = 0xff;
			if (tradekind == 1) {
				fmtcure(0);
				wrap(speech, 0xa4);
				trademode = 3;
			} else {
				wrap(speech, 0xa4);
				cmdn = evalvar(p + 6);
				if (cmdn > 3) {
					endtrade();
					p = skipword(p);
					return 0;
				}
				trademode = cmdn;
				p = skipfield(p);
				if (*p++ != ',') {
					endtrade();
					p = skipword(p);
					return 0;
				}
				cmdn = evalvar(p);
				if (trademode == 3)
					goodsprice = cmdn;
				else {
					pricemarkup = cmdn;
					nforsale = 0;
					do {
						p = skipfield(p);
						if (*p++ != ',')
							break;
						cmdn = evalvar(p);
						tradeoffer[nforsale++] = cmdn;
					} while (cmdn && nforsale < 18);
					if (nforsale == 1)
						knowpotion(tradegoods[23] = tradeoffer[0]);
					else
						for (cmdn = 0; cmdn < nforsale; cmdn++)
							tradegoods[cmdn + (nforsale < 18 ? 6 : 3)] = tradeoffer[cmdn];
					memset(tradeoffer, 0, 0x60);
					p = skipword(p);
					return 0xff8;
				}
			}
			pricemarkup = 0;
			nforsale = 0;
			p = skipword(p);
			return 0xff8;
		}
		if (p[0] == 'h' && p[1] == 'e' && p[2] == 'i' && p[3] == 'l' && p[4] != 'm') {
			restfor(0xff);
			p = skipword(p);
			break;
		}
		if (p[0] == 'h' && p[1] == 'e' && p[2] == 'i' && p[3] == 'l' && p[4] == 'm' && p[5] == 'a' && p[6] == 'k') {
			dealok = 0xff;
			if (p[7] == '1') {
				p = skipword(p);
				return 0xff3;
			}
			goodsprice = resthours(units);
			if (!goodsprice) {
				endtrade();
				tradekind = 0;
				wsprintf(speech, strs[dutalk ? 1034 : 1033]);
				p = skipword(p);
				continue;
			}
			cmdk = healthof(units);
			if (cmdk == 0x21c || cmdk == 0x21b)
				if (!learned(&units[curunit])) {
					endtrade();
					tradekind = 0;
					wsprintf(speech, strs[dutalk ? 1036 : 1035]);
					p = skipword(p);
					continue;
				}
			if (p[7] == '0') {
				fmtcure(1);
				dealok = 1;
				p = skipword(p);
				continue;
			}
			tradekind = 1;
			goto trade;
		}
		if (p[0] == 'k' && p[1] == 's' && p[2] == 'e' && p[3] == 'g') {
			if (game.combatbless < 2)
				game.combatbless++;
			p = skipword(p);
			continue;
		}
		if (p[0] == 'h' && p[1] == 'n' && p[2] == 'd' && p[3] == 'l') {
			units[curunit].flags |= 0x20;
			p = skipword(p);
			continue;
		}
		if (p[0] == 'k' && p[1] == 't' && p[2] == 'x' && p[3] == 't') {
			cmdn = evalvar(p + 4);
			if (cmdn > 0 && cmdn < 80)
				wsprintf(speech, strs[619 + cmdn]);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'm' && p[1] == 'r' && p[2] == 'd' && p[3] == 'p' && p[4] == 't') {
			if (curunit) {
				for (cmdm = 0; cmdm < 20; cmdm++)
					if (game.faces[cmdm] == 0) {
						game.faces[cmdm] = units[curunit].face;
						goto mrdone;
					}
				for (cmdm = 0; cmdm < 10; cmdm++)
					game.faces[cmdm] = game.faces[cmdm + 10];
				game.faces[cmdm++] = units[curunit].face;
				// FIXED: the loop that clears the faces after the new one starts at 11 and stops below 10, so it never runs and the list stays full. Clear up to 20.
#if defined(FEATURE_FIXES)
				for (; cmdm < 20; cmdm++)
#else
				for (; cmdm < 10; cmdm++)
#endif
					game.faces[cmdm] = 0;
			}
		mrdone:
			p = skipword(p);
			continue;
		}
		if (p[0] == 'n' && p[1] == 'n' && p[2] == 'o') {
			cmdw = evalvar(p + 3);
			if (cmdw && (cmdw & 0xfff) < nobjtiles)
				takefrom(&units[curunit], cmdw);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'o' && p[1] == 'r' && p[2] == 't') {
			if (p[3] == '_' || p[3] == 'x')
				cmdn = evalvar(p + 4);
			else
				cmdn = evalvar(p + 3);
			if (cmdn < 30) {
				if (!getbit(game.knownzones, cmdn)) {
					addscore(0x8c);
					learnplace();
				} else if (p[3] != 'x')
					goto ortdone;
				setbit(game.knownzones, cmdn, 1);
				if (p[3] == 'x')
					travelmap(2, curmap, zones[cmdn].id);
				else if (p[3] != '_')
					travelmap(1, curmap, zones[cmdn].id);
			}
		ortdone:
			p = skipword(p);
			continue;
		}
		if (p[0] == 'p' && p[1] == 'a' && p[2] == 'l' && p[3] == 'b' && p[4] == 'l' && p[5] == '0') {
			if (p[6] == 'l')
				fadeslow(0);
			else
				fademed(0);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'r' && p[1] == 'e' && p[2] == 'p' && p[3] == 'm' && p[4] == 'a' && p[5] == 'k') {
			tradekind = 0;
			memset(tradegoods, 0, 0x60);
			memset(tradeoffer, 0, 0x60);
			freetrade = 0;
			trademode = 4;
			dealok = 0xff;
			if (p[6] == 'm')
				repairmat = 1;
			else if (p[6] == 'l')
				repairmat = 2;
			else
				repairmat = 0;
			if (p[6] == '0' || p[7] == '0')
				freetrade = 1;
			goodsprice = 10;
			offerprice = 10;
			staylen = 0;
			pricemarkup = 0;
			nforsale = 0;
			p = skipword(p);
			return 0xff8;
		}
		if (p[0] == 'l' && p[1] == 'i' && p[2] == 'e' && p[3] == 'f' && p[4] == 'e' && p[5] == 'r' && p[6] == 'n') {
			p += 7;
			if (sceneitems[10])
				return 0xff7;
			continue;
		}
		if (p[0] == 'm' && p[1] == 'u' && p[2] == 's') {
			if (isdec(p[3]))
				cmdn = evalvar(p + 3);
			else
				cmdn = songbychar(p[3]);
			if (cmdn)
				music(cmdn, 0);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'n' && p[1] == 'a' && p[2] == 'm') {
			if (p[3] == ':') {
				cmdn = evalvar(p + 4);
				units[curunit].number = cmdn;
			} else
				units[curunit].number |= 0x2000;
			p = skipword(p);
			continue;
		}
		if (p[0] == 'n' && p[1] == 'e' && p[2] == 'u' && p[3] == 'd' && p[4] == 'i' && p[5] == 'a') {
			cmdn = evalvar(p + 6);
			units[curunit].scene = cmdn;
			p = skipword(p);
			continue;
		}
		if (p[0] == 'n' && p[1] == 'g') {
			if (p[2] == 'g') {
				cmdn = evalvar(p + 3);
				paycoins(1, cmdn);
				cmdn = (cmdn << 16) + 0x503;
			} else {
				cmdn = evalvar(p + 2);
				paycoins(0, cmdn);
				cmdn = (cmdn << 16) + 0x4e9;
			}
			addcoins(&cmdn, 0);
			giveitem(curunit, cmdn, 1);
			reexpand = 1;
			p = skipword(p);
			continue;
		}
		if (p[0] == 'n' && (p[1] == 'o' || p[1] == 'O')) {
			cmdn = evalvar(p + 2);
			cmdw = inpack((short *)&cmdrealm, (short *)&cmdslotno, cmdn, p[1] == 'o' ? 0 : 1);
			if (cmdw) {
				cmdn = takeout(cmdrealm, cmdslotno);
				giveitem(curunit, cmdn, 1);
				reexpand = 1;
			} else {
				cmdw = wornslot(units, cmdn, p[1] == 'o' ? 0 : 1);
				if (cmdw) {
					cmdslot = gearslot(units, cmdw);
					if (cmdslot) {
						giveitem(curunit, *cmdslot, 1);
						reexpand = 1;
						*cmdslot = 0;
						calcstats(units);
						dresshero();
					}
				}
			}
			p = skipword(p);
			continue;
		}
		if (p[0] == 'n' && p[1] == 'w' && p[2] == 'e' && p[3] == 'i' && p[4] == 'n') {
			cmdn = evalvar(p + 5);
			cmdk = 0;
			if (cmdn) {
				cmdk = taketypes(0, 0x1e8, 0x1eb, 0, cmdn, curunit);
				if (cmdk)
					reexpand = 1;
			}
			p = skipword(p);
			continue;
		}
		if (p[0] == 'n' && p[1] == 'w' && p[2] == 'r' && p[3] == 't') {
			msgf(0x63, strs[341], unitname(&units[curunit]));
			deedmercy();
			if (p[4] == '0')
				;
			else
				handover(units, &units[curunit]);
			if (combatkind)
				combatkind = 2;
			if (playmode == 1)
				music(10, 0);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'o' && p[1] == 'r' && p[2] == 'd' && p[3] == 'e' && p[4] == 'r') {
			if (p[5] == '2')
				ordercmd = 2;
			else {
				ordercmd = 1;
				quitnow();
			}
			fadefast(0);
			return 0xff4;
		}
		if (p[0] == 'p' && p[1] == 'o' && p[2] == 'r' && p[3] == 't' && p[4] == 'r') {
			cmdn = evalvar(p + 5);
			units[curunit].face = cmdn;
			p = skipword(p);
			continue;
		}
		if (p[0] == 'q' && p[1] == 'u' && p[2] == 'e' && p[3] == 's' && p[4] == 't') {
			addnote(evalvar(p + 5));
			p = skipword(p);
			continue;
		}
		if (p[0] == 'q' && p[1] == 'l' && p[2] == 'o' && p[3] == 'e' && p[4] == 's' && p[5] == 'c' && p[6] == 'h') {
			dropnote(evalvar(p + 7));
			p = skipword(p);
			continue;
		}
		if (p[0] == 'q' && p[1] == 'g' && p[2] == 'e' && p[3] == 'l' && p[4] == 'o' && p[5] == 'e' && p[6] == 's' && p[7] == 't') {
			p += 8;
			goto qgel;
		}
		if (p[0] == 'q' && p[1] == 'g' && p[2] == 'e' && p[3] == 'l' && p[4] >= '0' && p[4] <= '9') {
			p += 4;
		qgel:
			closenote(evalvar(p), 1);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'r' && p[1] == 'e' && p[2] == 'l' && p[3] == 'o' && p[4] == 'a' && p[5] == 'd')
			return 0xff2;
		if (p[0] == 'r' && p[1] == 'e' && p[2] == 'p') {
			cmdslot = unitslot(p[3], units);
			if (cmdslot && *cmdslot)
				setwear(cmdslot, 0);
			p = skipword(p);
			continue;
		}
		if (p[0] == 'r' && p[1] == 'e' && p[2] == 't') {
			findbrace(nodes[0].start);
			p = skipword(p);
			continue;
		}
		if (p[0] == 's' && p[1] == 'c' && p[2] == 'r') {
			scrn = evalvar(p + 3);
			if (scrn == 999)
				scrn = 0;
			else
				scrn += 0x2065;
			p = skipfield(p);
			if (*p == ',') {
				scrm = evalvar(p + 1) + 0x69b;
				picpage(scrn, scrm);
			}
			cmdret = 0xffb;
			p = skipword(p);
			continue;
		}
		if (p[0] == 's' && p[1] == 'h' && p[2] == 'l' && p[3] == 'c' && p[4] == 'h' && p[5] == 't') {
			if (p[6] == '2')
				telemapno = 0xff;
			else if (p[6] == '3')
				telemapno = 0xfe;
			else if (p[6] == '4')
				telemapno = 0xfd;
			else if (p[6] == '5')
				telemapno = 0xfc;
			else if (p[6] == '6')
				telemapno = 0xfb;
			else if (p[6] == '7')
				telemapno = 0xfa;
			else {
				gobattle = 1;
				return 0xff4;
			}
			teleposx = 75;
			teleposy = 75;
			telehours = 0;
			p = skipword(p);
			fademed(0);
			return 0xff6;
		}
		if (p[0] == 's' && p[1] == 'n' && p[2] == 'd') {
			cmdn = 0;
			p = skipword(p);
			continue;
		}
		if (p[0] == 's' && p[1] == 'p' && p[2] == 'e' && p[3] == 'n' && p[4] == 'd') {
			spendn = evalvar(p + 5);
			if (spendn < 0 || spendn > 31) {
				p = skipword(p);
				continue;
			}
			game.fate = spendn;
			fadeslow(0);
			return 0xff5;
		}
		if (p[0] == 's' && p[1] == 't' && p[2] == 'o' && p[3] == 'r' && p[4] == 'y' && p[5] == 'm' && p[6] == 'a' && p[7] == 'n' && p[8] == 'i' && p[9] == 'p' && p[10] >= '0' && p[10] <= '9') {
			cmdn = evalvar(p + 10);
			storymanip(cmdn);
			p = skipword(p);
			continue;
		}
		if (p[0] == 's' && p[1] == 'y' && p[2] == 'm' && p[3] == 'p' && p[4] == 'p') {
			addsymp(&units[curunit]);
			p = skipword(p);
			continue;
		}
		if (p[0] == 's' && p[1] == 'y' && p[2] == 'm' && p[3] == 'p') {
			sympn = evalvar(p + 4);
			if (sympn >= 0 && sympn <= 4) {
				units[curunit].sympathy = sympn;
				if (units[curunit].sympathy == 4)
					units[curunit].traits &= 0xdf;
			}
			p = skipword(p);
			continue;
		}
		if (p[0] == 't' && p[1] == 'a' && p[2] == 'l' && p[3] == 'k' && p[4] == 't' && p[5] == 'o') {
			cmdk = evalvar(p + 6);
			if (cmdk > 0 && cmdk < nunits && units[cmdk].health > 0 && units[cmdk].scene != 0) {
				talknext = cmdk;
				return 0xff2;
			}
			p = skipword(p);
			continue;
		}
		if (p[0] == 't' && p[1] == 'a' && p[2] == 'p' && p[3] == 'f') {
			cmdk = evalvar(p + 4);
			if (cmdk >= 1 && cmdk <= 3) {
				units[curunit].courage = cmdk;
				if (cmdk == 3 && units[curunit].health > 0)
					units[curunit].plea = 0;
			}
			p = skipword(p);
			continue;
		}
		if (p[0] == 't' && p[1] == 'a' && p[2] == 'u' && p[3] == 's' && p[4] == 'c' && p[5] == 'h') {
			p += 6;
			if (*p >= 'a' && *p <= 'z')
				cmdk = *p++;
			else
				cmdk = 0;
			dealok = 0xff;
			if (gathergoods(&units[curunit], 1, cmdk))
				barter(&units[curunit]);
			if (dealok == 0xff)
				wsprintf(speech, strs[897]);
			else if (dealok)
				replydeal();
			else
				replyno(0);
			continue;
		}
		if (p[0] == 't' && p[1] == 'e' && p[2] == 'l' && p[3] == 'e') {
			p += 4;
			telemapno = evalvar(p);
			if (telemapno == 0)
				continue;
			p = skipfield(p) + 1;
			teleposx = evalvar(p);
			if (teleposx == 0)
				continue;
			p = skipfield(p) + 1;
			teleposy = evalvar(p);
			if (teleposy == 0)
				continue;
			p = skipfield(p) + 1;
			telehours = evalvar(p);
			p = skipword(p);
			fademed(0);
			return 0xff6;
		}
		if (p[0] == 't' && p[1] == 'o' && p[2] == 'd')
			return 0xffd;
		if (p[0] == 't' && p[1] == 'u' && p[2] == 'e' && p[3] == 'r') {
			p += 4;
			tuerx = evalvar(p);
			if (tuerx != 0) {
				p = skipfield(p) + 1;
				tuery = evalvar(p);
				if (tuery != 0) {
					p = skipfield(p) + 1;
					tuerx += tuery * 150;
					tuerz = evalvar(p);
					if (tuerz == 0)
						tuerz = opendoor(tuerx, 1);
					else
						tuerz = closedoor(tuerx, tuerz);
					updatecell(tuerx);
					if (tuerz == 1) {
						lookaround();
						lookdone();
					}
				}
			}
			p = skipword(p) + 1;
			continue;
		}
		if (p[0] == 'w' && p[1] == 'a' && p[2] == 'c' && p[3] == 'h' && p[4] == 'a' && p[5] == 'u' && p[6] == 's') {
			cmdn = curunit;
			if (p[7] >= '0' && p[7] <= '9')
				cmdn = number(p + 7);
			else if (p[8] >= '0' && p[8] <= '9')
				cmdn = number(p + 8);
			if (cmdn && cmdn < nunits) {
				if (p[7] == 'x')
					offguard(&units[cmdn], 1, 2);
				else
					offguard(&units[cmdn], 0, 0);
			}
			p = skipword(p) + 1;
			continue;
		}
		if (p[0] == 'w' && p[1] == 'b' && p[2] == 'r' && p[3] == 'u' && p[4] == 'c' && p[5] == 'h') {
			deedbreakword();
			p += 6;
			continue;
		}
		if (p[0] == 'w' && p[1] == 'u' && p[2] == 't' && p[3] == 'g' && p[4] == 'r') {
			talkfight = 5;
			p += 5;
			continue;
		}
		if (p[0] == 'w' && p[1] == 'u' && p[2] == 't') {
			talkfight = 1;
			p += 3;
			continue;
		}
		if (p[0] == 'w' && p[1] == 'i' && p[2] == 's') {
			learnlore();
			p += 3;
			continue;
		}
		if (p[0] == '2' && p[1] == 'k' && p[2] == 'm' && p[3] == 'p' && p[4] == 'f') {
			p += 5;
			if (talkfight == 1 || talkfight == 5)
				continue;
			if (*p == 'o') {
				talkfight = 4;
				p++;
				continue;
			}
			if (talkfight == 4)
				continue;
			talkfight = 2;
			continue;
		}
		if (p[0] == 'u' && p[1] == 'k' && p[2] == 'm' && p[3] == 'p' && p[4] == 'f') {
			p += 5;
			if (talkfight == 1 || talkfight == 5 || talkfight == 4 || talkfight == 2)
				continue;
			talkfight = 3;
			continue;
		}
		if (p[0] == 'z' && p[1] == 'e' && p[2] == 'i' && p[3] == 't') {
			p += 4;
			gameday += evalvar(p);
			p = skipfield(p) + 1;
			gamehour = evalvar(p);
			setclock(gameday, gamehour);
			p = skipword(p) + 1;
			continue;
		}
		if (p[0] == 'z' && p[1] == 'l' && p[2] == 'e' && p[3] == 'h' && p[4] == 'r') {
			p += 5;
			staylen = evalvar(p);
			dealok = 0xff;
			p = skipword(p);
			return 0xfef;
		}
		if (p[0] == 'z' && p[1] == 'v' && p[2] == 'e' && p[3] == 'r' && p[4] == 'g') {
			p += 5;
			staylen = evalvar(p);
			dealok = 0xff;
			p = skipword(p);
			return 0xfee;
		}
		p++;
	next:
		;
	}
	return cmdret;
}

/*
 * the truth of the condition at p, up to its { or #: terms joined by &
 * and |, each 1 or 0 in condstring, and & binds tighter. 0xff for
 * handel_ok while dealok is 0xff
 */
char
evalcond(char *p)
{
	/*
	 * unused: a temporary of the code generator, which gives the
	 * [ml]rep term the original's imul by 0x1b
	 */
	int us;
	short i;
	char *s;

	ce_val = 0;
	ce_neg = 0;
	ce_out = condstring;
	condstring[0] = 0;
	for (;;) {
		if (*p == '#' || *p == '{')
			break;
		ce_val = 0;
		if (*p == '!') {
			ce_neg = 1;
			p++;
		} else
			ce_neg = 0;
		if (*p >= 'A' && *p <= 'Z' && (p[1] < 'a' || p[1] > 'z')) {
			if (getbit(sceneflags, *p - 'A'))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (*p == 'N' && p[1] >= 'A' && p[1] <= 'H' && (p[2] == '&' || p[2] == '|' || p[2] == '{')) {
			if (getbit((char *)&units[curunit].scriptflags, p[1] - 'A'))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (*p >= 'a' && *p <= 'z' && (p[1] < 'a' || p[1] > 'z')) {
			if (getbit(talkflags, *p - 'a'))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (*p == '%') {
			cea = evalvar(p++);
			while (isvarc(*p))
				p++;
			if (*p == '{' || *p == '#' || *p == '|' || *p == '&') {
				if (cea)
					ce_val = 1;
				p--;
			} else {
				if (p[1] == '=')
					ceb = evalvar(p + 2);
				else
					ceb = evalvar(p + 1);
				if (*p == '=') {
					if (cea == ceb)
						ce_val = 1;
					else
						ce_val = 0;
				} else if (*p == '!') {
					if (cea != ceb)
						ce_val = 1;
					else
						ce_val = 0;
				} else if (*p == '<') {
					/* stores of 1 written apart, as in [ml]rep */
					if (cea < ceb)
						ce_val = 1;
					else if (p[1] == '=' && ceb == cea)
						ce_val = 1;
					else
						ce_val = 0;
				} else if (*p == '>') {
					if (cea > ceb)
						ce_val = 1;
					else if (p[1] == '=' && cea == ceb)
						ce_val = 1;
					else
						ce_val = 0;
				}
			}
		} else if (p[0] == 'a' && p[1] == 'r' && p[2] == 'b') {
			if (curunit && atwork(&units[curunit]))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'b' && p[1] == 'e' && p[2] == 's' && p[3] == 't' && p[4] == 'e' && p[5] == 'l' && p[6] == 'l' && p[7] == 't') {
			ce_val = sceneitems[10] ? 1 : 0;
		} else if (p[0] == 'f' && p[1] == 'l' && p[2] == 'a' && p[3] == 'g') {
			cea = evalvar(p + 4);
			if (getbit(game.flags, cea))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'h' && p[1] == 'a' && p[2] == 'n' && p[3] == 'd' && p[4] == 'e' && p[5] == 'l' && p[6] == '_' && p[7] == 'o' && p[8] == 'k') {
			if (dealok == 0xff)
				return 0xff;
			if (dealok)
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'k' && p[1] == 's' && p[2] == 'e' && p[3] == 'g') {
			if (game.combatbless)
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'h' && p[1] == 'n' && p[2] == 'd' && p[3] == 'l') {
			if (istrader(&units[curunit]))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'h' && p[1] == 'e' && p[2] == 'r' && p[3] == 'r' && p[4] == '_' && p[5] == 'f') {
			if (curunit && units[curunit].lord && isfemale(&units[units[curunit].lord]))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'h' && p[1] == 'i' && p[2] == 'r' && p[3] == 't') {
			if (isherder(&units[curunit]))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'h' && p[1] == 'f' && p[2] == 's' && p[3] == 'g' && p[4] == 'k') {
			if (hasfought(&units[curunit]))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'j' && p[1] == 'a') {
			ce_val = 1;
			choices[1].shown = 1;
			choices[2].shown = 1;
			choices[3].shown = 0;
			talkstate = 1;
		} else if (p[0] == 'w' && p[1] == 'e' && p[2] == 'i' && p[3] == 't' && p[4] == 'e' && p[5] == 'r') {
			ce_val = 1;
			choices[1].shown = 0;
			choices[3].shown = 1;
			choices[2].shown = 0;
			talkstate = 1;
		} else if (p[0] == 'w' && p[1] == 'z' && p[2] == 's' && p[3] == 'm' && p[4] == 'i') {
			ce_val = 0;
			if (curunit && neartools(units[curunit].x, units[curunit].y, 1))
				ce_val = 1;
		} else if (p[0] == 'w' && p[1] == 'z' && p[2] == 'l' && p[3] == 'b' && p[4] == 'a') {
			ce_val = 0;
			if (curunit && neartools(units[curunit].x, units[curunit].y, 0))
				ce_val = 1;
		} else if (p[0] == 'k' && p[1] == 'e' && p[2] == 'n') {
			if (curunit && knowsplayer(&units[curunit]))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'k' && p[1] == 'n' && p[2] == 'e' && p[3] == 'c' && p[4] == 'h' && p[5] == 't') {
			if (curunit && isservant(&units[curunit]))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 's' && p[1] == 'p' && p[2] == 'k' && p[3] == 'n' && p[4] == 'e' && p[5] == 'c' && p[6] == 'h' && p[7] == 't') {
			if (curunit && myservant(&units[curunit]))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'f' && p[1] == 'r' && p[2] == 'a' && p[3] == 'u') {
			if (curunit && isfemale(&units[curunit]))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'h' && p[1] == 'a' && p[2] == 'n' && p[3] == 'd' && p[4] == 'w') {
			if (curunit && iscraftsman(&units[curunit]))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 's' && p[1] == 'c' && p[2] == 'h' && p[3] == 'm' && p[4] == 'i' && p[5] == 'e' && p[6] == 'd') {
			if (curunit && issmith(&units[curunit]))
				ce_val = 1;
			else
				ce_val = 0;
		} else if ((p[0] == 'm' || p[0] == 'l') && p[1] == 'r' && p[2] == 'e' && p[3] == 'p') {
			ce_val = 0;
			ceslot = unitslot(p[4], units);
			if (ceslot && *ceslot) {
				cev = *ceslot;
				if (repairable(cev) && wearof(cev) >= 3) {
					cedays = classof(cev & 0xfff);
					if (cedays == 0x3c)
						cedays = metals[gearof(cev)].metal;
					else
						cedays = armors[gearof(cev)].metal;
					/*
					 * stores of 1 written apart merge, and the
					 * original's byte registers count each one
					 */
					if (p[0] == 'm') {
						if (cedays == 1)
							ce_val = 1;
					} else if (p[0] != 'l')
						ce_val = 1;
					else if (cedays == 0 || cedays == 2)
						ce_val = 1;
				}
			}
		} else if (p[0] == 'n' && p[1] == 's' && p[2] == 'i' && p[3] == 'c' && p[4] == 'h' && p[5] == 't' && p[6] == 'b') {
			ce_val = 0;
			cev = evalvar(p + 7);
			if (cev && cev < nunits && units[cev].health > 0 && !units[cev].asleep && units[cev].mode != 90 && seesunit(&units[cev]))
				ce_val = 1;
		} else if (p[0] == 'n' && p[1] == 't' && p[2] == 'o' && p[3] == 't') {
			ce_val = 0;
			cev = evalvar(p + 4);
			if (cev && cev < nunits && units[cev].health <= 0)
				ce_val = 1;
		} else if (p[0] == 'n' && p[1] == 'o' && p[2] == 's' && p[3] == 'e' && p[4] == 'x') {
			if (censorship == 2 || censorship == 4)
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'n' && p[1] == 'o' && p[2] == 'v' && p[3] == 'i' && p[4] == 'o') {
			if (censorship == 1 || censorship == 4)
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'n' && p[1] == 'o' && p[2] == 'b' && p[3] == 'j') {
			cev = evalvar(p + 4);
			if (cev == 0)
				ce_val = 0;
			else {
				ce_val = wornslot(units + curunit, cev, 0);
				if (ce_val == 0)
					ce_val = finditem(&units[curunit], cev, 0, 0, 0, 0);
			}
		} else if ((p[0] == 'o' || p[0] == 'O') && p[1] == 'b' && p[2] == 'j') {
			cev = evalvar(p + 3);
			if (cev == 0)
				ce_val = 0;
			else {
				ce_val = inpack(0, 0, cev, p[0] == 'o' ? 0 : 1);
				if (ce_val == 0)
					ce_val = wornslot(units, cev, p[0] == 'o' ? 0 : 1);
			}
		} else if (p[0] == 's' && p[1] == 'p' && p[2] == 'i' && p[3] == 'e' && p[4] == 'l') {
			ce_val = 0;
			ceslot = unitslot(p[5], units);
			if (ceslot && *ceslot) {
				ce_val = 1;
				if (ceslot == &units[0].slot[4]) {
					if (!isarmor(units[0].slot[4]))
						ce_val = 0;
				} else if (ceslot == &units[0].slot[6]) {
					if (units[0].slot[6] == 0 || classof(units[0].slot[6] & 0xfff) != 0x55)
						ce_val = 0;
				}
			}
		} else if ((p[0] == 's' || p[0] == 'n') && p[1] == '_' && p[2] == 'w' && p[3] == 'a' && p[4] == 'f' && p[5] == 'f') {
			/* a defect: n_waff without a unit asks units[0] all the same */
			if (p[0] == 'n' && !curunit)
				ce_val = 0;
			if (p[0] == 's')
				ce_val = hasweapon(units);
			else
				ce_val = hasweapon(&units[curunit]);
		} else if (p[0] == 's' && p[1] == 'p' && p[2] == 'z') {
			ce_val = 0;
			cev = evalvar(p + 3);
			if (cev && getbit(game.known, cev))
				ce_val = 1;
		} else if (p[0] == 's' && p[1] == 'o' && p[2] == 'r' && p[3] == 't' && p[4] == 'x') {
			ce_val = 0;
			cev = evalvar(p + 5);
			if (cev && getbit(game.knownzones, cev) && getbit(game.reachedzones, cev))
				ce_val = 1;
		} else if (p[0] == 's' && p[1] == 'o' && p[2] == 'r' && p[3] == 't') {
			ce_val = 0;
			cev = evalvar(p + 4);
			if (cev && getbit(game.knownzones, cev))
				ce_val = 1;
		} else if (p[0] == 't' && p[1] == 'a' && p[2] == 's' && p[3] == 'c' && p[4] == 'h') {
			if (curunit)
				for (i = 0; i < 3; i++)
					if (units[curunit].slot[9 + i])
						ce_val = 1;
		} else if (p[0] == 's' && p[1] == 'h' && p[2] == 'a' && p[3] == 'r' && p[4] == 'e' && p[5] == 'w') {
			ce_val = 0;
		} else if (p[0] == 's' && p[1] == 's' && p[2] == 'h' && p[3] == 'z' && p[4] == 'z' && p[5] == 'b') {
			if (jewelmagic(units, 0x80000))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'q' && p[1] == 'b' && p[2] == 'e' && p[3] == 'k') {
			ce_val = evalvar(p + 4);
			if (hasnote(ce_val))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'q' && p[1] == 'g' && p[2] == 'e' && p[3] == 'l') {
			ce_val = evalvar(p + 4);
			if (noteclosed(ce_val))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'u' && p[1] == 'e' && p[2] == 'b' && p[3] == 'l') {
			ce_val = overladen(units);
		} else if (p[0] == 'U' && p[1] == 'e' && p[2] == 'b' && p[3] == 'R' && p[4] == 'a' && p[5] == 'c' && p[6] == 'h') {
			ce_val = 0;
			if (p[7] == 'F') {
				if (avengers && victimfemale)
					ce_val = 1;
			} else if (avengers)
				ce_val = 1;
		} else if (p[0] == 'u' && p[1] == 'k' && p[2] == 'm' && p[3] == 'g' && p[4] == 'l') {
			if (isfit(units) && !nearfoe(0) && curunit && isfit(&units[curunit]) && !underattack(curunit))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'v' && p[1] == 'e' && p[2] == 'r' && p[3] == 'w') {
			ce_val = 0;
			if (p[4] == 'w') {
				if (resthours(units) && (units[0].health < kindof(units)->maxhealth * 2 / 3 || units[0].poison))
					ce_val = 1;
			} else if (resthours(units))
				ce_val = 1;
		} else if (p[0] == 'w' && p[1] == 'a' && p[2] == 'c' && p[3] == 'h' && p[4] == 'm' && p[5] == 'o' && p[6] == 'd') {
			if (curunit && isguard(&units[curunit]))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'z' && p[1] == 'h' && p[2] == 's') {
			if (athome(&units[curunit], 1))
				ce_val = 1;
			else
				ce_val = 0;
		} else if (p[0] == 'z' && p[1] == 't' && p[2] == '_') {
			if (p[3] == 'm') {
				s = daytime(gamehour);
				if (s == strs[492] || s == strs[493] || s == strs[494])
					ce_val = 1;
				else
					ce_val = 0;
			} else if (p[3] == 't') {
				s = daytime(gamehour);
				if (s == strs[495] || s == strs[496])
					ce_val = 1;
				else
					ce_val = 0;
			} else if (p[3] == 'a') {
				s = daytime(gamehour);
				if (s == strs[497] || s == strs[498])
					ce_val = 1;
				else
					ce_val = 0;
			} else if (p[3] == 'n') {
				if (daytime(gamehour) == strs[499])
					ce_val = 1;
				else
					ce_val = 0;
			}
		} else if (p[0] == 'z' && p[1] == 'u' && p[2] == 'k') {
			listvisions();
			if (nextvision(curunit, curmap) >= 0)
				ce_val = 1;
			else
				ce_val = 0;
		} else if (*p == '0')
			ce_val = 0;
		else if (*p == '1')
			ce_val = 1;
		if (ce_neg)
			ce_val = !ce_val;
		*ce_out++ = ce_val ? '1' : '0';
		*ce_out = 0;
		/*
		 * the & and | copies merge into one block, and the & copy,
		 * reached from the scan alone, reuses the scan's byte and
		 * p + 1 as the original does
		 */
		do {
			p++;
			if (*p == '&') {
				*ce_out++ = *p++;
				*ce_out = 0;
				goto next;
			}
			if (*p == '|') {
				*ce_out++ = *p++;
				*ce_out = 0;
				goto next;
			}
		} while (*p != '#' && *p != '{');
		break;
	next:
		;
	}
end:
	ce_out = condstring;
	if (condstring[0] == 0)
		return 1;
	if (condstring[1] == 0) {
		if (condstring[0] == '1')
			return 1;
		return 0;
	}
	if (condstring[3] == 0) {
		if (condstring[1] == '&') {
			if (condstring[0] == '1' && condstring[2] == '1')
				return 1;
			return 0;
		}
		if (condstring[1] == '|') {
			if (condstring[0] == '1' || condstring[2] == '1')
				return 1;
			return 0;
		}
	}
	while (haschar(condstring, '&')) {
		ce_dst = namebuf;
		ce_out = condstring;
		do {
			ce_c = *ce_out;
			if (ce_out[1] == '&') {
				*ce_dst++ = ce_out[0] == '1' && ce_out[2] == '1' ? '1' : '0';
				ce_out += 3;
			} else
				*ce_dst++ = *ce_out++;
		} while (ce_c);
		copystr(condstring, namebuf);
	}
	while (haschar(condstring, '|')) {
		ce_out = condstring;
		ce_dst = namebuf;
		do {
			ce_c = *ce_out;
			if (ce_out[1] == '|') {
				*ce_dst = ce_out[0] == '1' || ce_out[2] == '1' ? '1' : '0';
				ce_dst++;
				ce_out = ce_out + 3;
			} else
				*ce_dst++ = *ce_out++;
		} while (ce_c);
		copystr(condstring, namebuf);
	}
	if (condstring[0] == '1')
		return 1;
	return 0;
}

/* move *pp past the { } block at or after it */
void
skipblock(char **pp)
{
	while (**pp != '{')
		(*pp)++;
	blkdepth = 0;
	do {
		if (**pp == '{')
			blkdepth++;
		else if (**pp == '}')
			blkdepth--;
		(*pp)++;
	} while (blkdepth > 0);
}

void
staydays(short k)
{
	tradekind = 0;
	endtrade();
	fadeslow(0);
	svhour = talkactive;
	talkactive = 0;
	svunit = curunit;
	curunit = 0;
	wakex = (*(short *)&playerx);
	wakey = (*(short *)&playery);
	if (svunit) {
		wakereg = homeof(&units[svunit]);
		if (wakereg) {
			wakei = freeplace(wakereg);
			if (wakei != -1) {
				wakex = regions[wakereg].placex[wakei];
				wakey = regions[wakereg].placey[wakei];
			}
		}
	}
	restfor(0xff);
	if (k == 0) {
		if (staylen > 2)
			train(0x14, 0x11);
		train(0x19, 0x17);
	}
	if (k == 3)
		travel(curmap, wakex, wakey, hourticks, 0, 1, 0, 1);
	else
		travel(curmap, wakex, wakey, hourticks * (staylen * 24 + (gamehour >= 8 ? -(gamehour - 8) : 8 - gamehour)), 0, 1, 0, 1);
	restfor(0xff);
	if (k == 0 || k == 3)
		units[0].mode = 0x5a;
	else
		units[0].mode = 0;
	cleartexts();
	fmtweekday(staylen);
	if (k == 0)
		msgf(0x63, texts + 0xb24a, daytext, strs[1113]);
	else if (k == 1) {
		wsprintf(namebuf, strs[1114], svunit ? unitname(&units[svunit]) : strs[1432]);
		msgf(0x63, texts + 0xb24a, daytext, namebuf);
	} else if (k == 2)
		msgf(0x63, texts + 0xb24a, daytext, strs[1115]);
	else if (k == 3)
		msgf(0x63, strs[1101]);
	else if (k == 4) {
		char *s;

		wsprintf(tmpbuf, daytext);
		for (s = tmpbuf; *s; s++)
			;
		if (s > tmpbuf && s[-1] == ' ')
			s--;
		s[0] = '.';
		s[1] = '.';
		s[2] = '.';
		s[3] = 0;
		msgf(0x63, tmpbuf);
	}
	if (randn(2) == 0) {
		units[0].dirx = 0;
		units[0].diry = 1;
	} else {
		units[0].dirx = 1;
		units[0].diry = 0;
	}
	talkactive = svhour;
	curunit = svunit;
}

/*
 * run the text of node n into *pa: \ lines are commands (runcmds), whose
 * text goes to *pb, " lines conditions (evalcond), # and } end the text
 */
short
runnode(char **pa, char **pb, unsigned char n)
{
	char *q;	/* rp at the loop head, for the original's registers */

	if (talkstate == 1) {
		rp = resumeptr;
		talkstate = 0;
		resumeptr = speech;
		if (talkanswer == 0 || talkanswer == 2)
			;
		else {
		skipcond:
			skipblock(&rp);
		}
	skipline:
		while (*rp++ != 13)
			;
		rp++;
	} else {
		rp = nodes[n].start;
		while (*rp && *rp++ != 13)
			;
		rp++;
	}
	while ((q = rp) < nodes[n].end) {
		int k;

		for (;;) {
			if (*rp != ' ')
				break;
			rp++;
		}
		if (*rp == '\\') {
			rp++;
			if (*rp == '\\' || *rp == '*' || *rp == ';') {
				while (*rp && *rp++ != 13)
					;
				ret = 0;
				rp++;
				continue;
			}
			if (*pb != nodecmds)
				*(*pb)++ = ' ';
			ret = runcmds(rp);
			if (reexpand)
				expandnodes();
			*pa = speech;
			while (**pa)
				(*pa)++;
			// FIXED: the copy of a node's commands into nodecmds, which the game only writes, runs past its 180 bytes: 293 in the lesson of wizardry in script 3200, and at the fifth paid vision of script 2500 over the choice $N10 in nodetexts. Skip the line. *pb then stays at the array's start, and the test above skips the space too.
#ifdef FEATURE_FAULTS
			while (*rp++ != 13)
				;
#else
			while ((*(*pb)++ = *rp++) != 13)
				;
			*--*pb = 0;
#endif
			rp++;
			if (!ret)
				continue;
			if (ret == 0xffb) {
				*pa = speech;
				ret = 0;
				**pa = 0;
				continue;
			}
			if (ret == 0xff9) {
				ret = 0;
				continue;
			}
			if (ret == 0xffa) {
				n = currentnode;
				rp = scriptpos;
				ret = 0;
				continue;
			}
			if (ret == 0xff0) {
			done:
				*pa = speech;
				while (**pa)
					(*pa)++;
				return 0;
			}
			if (ret != 0xff3)
				;
			else {
				music(0x1e, 0);
				k = 0;
				goto wake;
			}
			if (ret == 0xfef) {
				k = 1;
			wake:
				/* the original passes k as an int */
				// FIXED: a call through another function type is undefined in C. Call it through its own, with the same value.
#ifdef PORT
				staydays(k);
#else
				((void (*)(int))staydays)(k);
#endif
				gobattle = 0;
				return 0xff4;
			}
			if (ret != 0xfee)
				;
			else {
				k = 2;
				goto wake;
			}
			if (ret == 0xff2)
				return 0xff2;
			if (ret == 0xff8) {
				if (trademode == 2) {
					for (ret = 10; sceneitems[ret]; ret++)
						;
					if (ret >= 13) {
						endtrade();
						wsprintf(speech, texts + 0xb23b, strs[896]);
						*pa = speech;
						while (**pa)
							(*pa)++;
						return 0;
					}
				}
				trade();
				if (esckey != 0xff)
					esckey = 0;
				if (dealok && dealok != 0xff) {
					if (tradekind == 1) {
						music(0x1e, 0);
						staydays(0);
						gobattle = 0;
						return 0xff4;
					}
					if (trademode == 4) {
						cnt = goodsprice / 10;
						if (goodsprice % 10)
							cnt++;
						if (cnt == 0)
							cnt++;
						if (myservant(&units[curunit]) || freetrade) {
							trademode = 3;
							dealok = 1;
							tradekind = 3;
						} else {
							wsprintf(speech, strs[1133], cnt);
							wrap(speech, 0xa4);
							tradekind = 3;
							trademode = 3;
							trade();
							if (esckey != 0xff)
								esckey = 0;
						}
						if (dealok)
							for (cnt = 0; cnt < 24; cnt++)
								if (tradegoods[cnt])
									tradegoods[cnt] = repaired(tradegoods[cnt]);
						for (cnt = 0; cnt < 24; cnt++)
							if (tradegoods[cnt])
								giveitem(0, tradegoods[cnt], 1);
						endtrade();
						tradekind = 0;
						expandnodes();
						if (!dealok)
							goto noshop;
						{
							fadeslow(0);
							passtime(hourticks, 0, 0);
							cleartexts();
							if (myservant(&units[curunit]))
								wsprintf(speech, strs[1136]);
							else
								wsprintf(speech, strs[1135]);
							drawscreen();
							fadeslow(1);
						}
					} else if (trademode == 1)
						replydeal();
					else if (trademode == 2) {
						addorders();
						listitems(speech);
					} else if (trademode == 3)
						speech[0] = 0;
				} else {
				noshop:
					replyno(0);
				}
				endtrade();
				tradekind = 0;
				expandnodes();
				*pa = speech;
				while (**pa)
					(*pa)++;
			} else if (ret == 0xfed) {
				music(5, 0);
				k = 3;
				goto wake;
			} else if (ret == 0xff7) {
				lvn = 0;
				lvp = *pa;
				lvi = 10;
				while (sceneitems[lvi]) {
					if (gameday >= curvars[lvi]) {
						if (lvn) {
							lvf = 0;
							for (lvj = lvi + 1; sceneitems[lvj]; lvj++) {
								short g;	/* for the original's registers */

								g = gameday;
								if (g >= curvars[lvj]) {
									lvf++;
									break;
								}
							}
							if (lvf == 0)
								wsprintf(lvp, strs[904]);
							else
								wsprintf(lvp, ", ");
							while (*lvp)
								lvp++;
						}
						lvf = sceneitems[lvi];
						giveitem(0, lvf, 1);
						reexpand = 1;
						wsprintf(lvp, youritem(lvf, 0, dutalk));
						if (lang) {
							if (lvn == 0)
								lvp++;
							// FIXED: *lvp++ = lower(*lvp) reads and moves lvp unsequenced, undefined in C. Read *lvp first, as Watcom compiled it.
#ifdef PORT
							for (; *lvp; lvp++)
								*lvp = lower(*lvp);
#else
							while (*lvp)
								*lvp++ = lower(*lvp);
#endif
						} else
							while (*lvp)
								lvp++;
						lvn++;
						for (lvj = lvi; sceneitems[lvj]; lvj++) {
							sceneitems[lvj] = sceneitems[lvj + 1];
							curvars[lvj] = curvars[lvj + 1];
						}
					} else
						lvi++;
				}
				if (lvn) {
					if (lvn == 1 && !itemplural(lvf))
						wsprintf(lvp, strs[906]);
					else
						wsprintf(lvp, strs[907]);
					while (*lvp)
						lvp++;
					*pa = lvp;
				}
				if (sceneitems[10])
					listitems(*pa);
				*pa = speech;
				while (**pa)
					(*pa)++;
				return 0;
			} else
				return ret;
			ret = 0;
			continue;
		}
		if (*rp == '"') {
			rp++;
			condv = evalcond(rp);
			if (!condv)
				goto skipcond;
			if (condv == 0xff) {
				skipblock(&rp);
				if (*rp == '-')
					skipblock(&rp);
			}
			if (talkstate == 1) {
				resumeptr = rp;
				**pa = 0;
				return 0xffc;
			}
			goto skipline;
		}
		if (*rp == '#') {
			scriptpos = rp - 1;
			innernode();
			goto done;
		}
		if (*rp == '}') {
			rp++;
			if (*rp == '-')
				skipblock(&rp);
			while (*rp++ != 13)
				;
			rp++;
			continue;
		}
		do {
			if (*rp == 10) {
				// FIXED: a line that ends at the speech's start reads before the buffer, and at its first character writes there too
#ifdef FEATURE_FAULTS
				switch (*pa - speech >= 2 ? (*pa)[-2] : 0) {
#else
				switch ((*pa)[-2]) {
#endif
				case 13:
				case '!':
				case '\'':
				case '.':
				case ':':
				case '?':
					break;
				default:
					// FIXED: as above, write inside the speech.
#ifdef FEATURE_FAULTS
					if (*pa > speech)
						(*pa)[-1] = ' ';
#else
					(*pa)[-1] = ' ';
#endif
					break;
				}
				rp++;
				while (*rp == ' ')
					rp++;
			} else if (*rp == '%' || *rp == '$')
				expand(pa, &rp);
			else
				*(*pa)++ = *rp++;
		} while (*rp != '"' && *rp != '#' && *rp != '}' && *rp != '\\');
		**pa = 0;
	}
	return 0;
}

/* whether node b lies in node a */
short
nodein(short a, short b)
{
	if (a == 0)
		return 1;
	for (;;) {
		if (b == a)
			return 1;
		if (b == 0)
			return 0;
		b = nodes[b].parent;
	}
}

/*
 * run node currentnode into speech: when its text ends, flag and lay
 * out the choices whose conditions hold, then break the text into lines
 */
short
shownode(void)
{
	if (talkstate == 1) {
		dlgp = speech;
		speech[0] = 0;
	} else {
		innernode();
		for (dlgi = 0; dlgi < nnodes; dlgi++)
			nodes[dlgi].shown = 0;
		dlgp = speech;
		speech[0] = 0;
		dlgq = nodecmds;
		nodecmds[0] = 0;
	}
	dlgret = runnode(&dlgp, &dlgq, currentnode);
	if (!dlgret) {
		nchoice = 0;
		dlgi = currentnode;
		dlgj = 0;
		while (dlgj < nodes[dlgi].nchild) {
			if (nodes[dlgk = nodes[dlgi].child[dlgj]].width) {
				if (!nodein(dlgk, currentnode)) {
					dlgx = evalcond(nodes[dlgk].start);
					if (dlgx == 0)
						nodes[dlgk].shown = 0;
					else {
						nodes[dlgk].shown = 1;
						nchoice++;
					}
				}
			}
			dlgj++;
		}
		if (currentnode != 0) {
			if (nodes[currentnode].text[-1] == '!') {
				dlgx = evalcond(nodes[currentnode].start);
				if (dlgx == 0)
					nodes[currentnode].shown = 0;
				else {
					nchoice++;
					nodes[currentnode].shown = 1;
				}
				dlgi = nodes[currentnode].parent;
				dlgj = 0;
				while (dlgj < nodes[dlgi].nchild) {
					if (nodes[dlgk = nodes[dlgi].child[dlgj]].width) {
						dlgx = evalcond(nodes[dlgk].start);
						if (dlgx == 0)
							nodes[dlgk].shown = 0;
						else {
							nchoice++;
							nodes[dlgk].shown = 1;
						}
					}
					dlgj++;
				}
			} else if (nchoice == 0) {
				if (homenode == 0xff)
					dlgi = 0;
				else
					dlgi = homenode;
				for (dlgj = 0; dlgj < nodes[dlgi].nchild; dlgj++) {
					if (nodes[dlgk = nodes[dlgi].child[dlgj]].width) {
						if (dlgk != currentnode) {
							dlgx = evalcond(nodes[dlgk].start);
							if (dlgx == 0)
								nodes[dlgk].shown = 0;
							else {
								nodes[dlgk].shown = 1;
								nchoice++;
							}
						}
					}
				}
			}
		}
		dlgi = 0;
		for (dlgj = 0; dlgj < nodes[dlgi].nchild; dlgj++) {
			if (nodes[dlgk = nodes[dlgi].child[dlgj]].width) {
				if (!nodein(dlgk, currentnode) && nodes[dlgk].text[-1] == '!') {
					dlgx = evalcond(nodes[dlgk].start);
					if (dlgx == 0)
						nodes[dlgk].shown = 0;
					else {
						nodes[dlgk].shown = 1;
						nchoice++;
					}
				}
			}
		}
		dlgx = 0x98;
		dlgy = 0;
		nchoice = 0;
		nlong = 0;
		dlgk = 0;
		while (dlgk < nnodes) {
			if (nodes[dlgk].shown) {
				nchoice++;
				if (countwords(nodes[dlgk].start) >= 4)
					nlong++;
				if (dlgx + nodes[dlgk].width >= 0x268) {
					dlgy += 29;
					dlgx = 0x98;
				}
				nodes[dlgk].x = dlgx;
				nodes[dlgk].y = dlgy;
				dlgx += nodes[dlgk].width;
				choicesr = dlgx;
				choicesh = dlgy;
				dlgx += 24;
			}
			dlgk++;
		}
		if (nchoice <= 5 && nlong && nlong >= nchoice / 2 && nchoice - nlong < nlong * 2) {
			dlgx = 0x98;
			dlgy = 0;
			choicesr = 0;
			for (dlgk = 0; dlgk < nnodes; dlgk++) {
				if (nodes[dlgk].shown) {
					nodes[dlgk].x = dlgx;
					nodes[dlgk].y = dlgy;
					if (nodes[dlgk].width > choicesr)
						choicesr = nodes[dlgk].width;
					choicesh = dlgy;
					dlgy += 29;
				}
			}
			choicesr += 0x98;
		}
		choicesh += 29;
		sentcase(speech);
	}
	dlgp = speech;
	spacestop = 1;
	dlgx = 0;
	speechpx = 0;
	for (;;) {
		text3w(dlgp);
		if (dlgx + 7 + text3width < 463) {
			dlgx += text3width + 7;
			while (*dlgp != ' ' && *dlgp != 13 && *dlgp)
				dlgp++;
			if (*dlgp == 0)
				break;
			if (*dlgp == 13) {
				if (dlgp <= speech || dlgp[-1] == 13)
					speechpx += 15;
				else
					speechpx += 23;
				dlgx = 0;
				while (dlgp[1] == 13) {
					dlgp++;
					speechpx += 15;
				}
			}
			dlgp++;
		} else if (dlgp > speech) {
			dlgx = 0;
			speechpx += 23;
			dlgp[-1] = 13;
		}
	}
	spacestop = 0;
	if (talkfight) {
		if (talkfight == 3) {
			setunitmode(&units[curunit], 0x16);
			units[curunit].modeb = 0;
			units[curunit].modec = 0xff;
			units[curunit].modee = 0;
			units[curunit].modef = 0;
			units[curunit].modeg = unitdist(&units[curunit], units);
			armsound(&units[curunit]);
		} else if (talkfight == 2) {
			duel(&units[curunit], 0);
			armsound(&units[curunit]);
			learnfight(curunit);
		} else if (talkfight == 4) {
			duel(&units[curunit], 1);
			armsound(&units[curunit]);
			learnfight(curunit);
		} else {
			provoke(units, &units[curunit], 4, 4, 0);
			setunitmode(&units[curunit], 0x14);
			units[curunit].modeb = 0;
			provoke(&units[curunit], units, 4, 4, 0);
			plotrage(curunit);
			learnfight(curunit);
			armsound(&units[curunit]);
			if (talkfight == 5) {
				dlgx = 1;
				while (dlgx < nunits) {
					if (units[dlgx].health > 0 && units[dlgx].mode == 0x15 && units[dlgx].modeb == 0)
						units[dlgx].mode = 0x14;
					dlgx++;
				}
			}
		}
		if (playmode == 1) {
			if (talkfight == 3)
				combatcause = 3;
			else
				combatcause = 4;
			togglecombat(0);
		}
		talkfight = 0;
	}
	if (homenode == 0xff) {
		innernode();
		homenode = currentnode;
	}
	return dlgret;
}

char marklabel[2] = "A";
int scriptlang = -1;
short nscenes = 1;
DWORD sceneidx = 1;
int curscene = 1;
char homenode = -1;
short lineofcount;
char *lineofptr;
unsigned long val;
unsigned long seglo;
unsigned long seghi;
unsigned long *segslot;
short unitid;
unsigned unitix;
int evn;
short evk;
short evm;
unsigned long evv;
unsigned long *evslot;
unsigned long evpct;
char *evp;
unsigned long exv;
unsigned long exw;
unsigned long *exslot;
char *exs;
char exc;
short nodei;
short nodebest;
short nodelen;
char *nodestart;
char *nodeend;
char *markp;
int markn;
DWORD loadi;
DWORD loadlen;
DWORD loadn;
HANDLE loadh;
int loadok;
unsigned short expi;
char *exps;
DWORD unpackn;
char *parsep;
char *parseq;
int parsei;
DWORD parseoff;
char *svp;
DWORD svn;
DWORD clrn;
short vj;
short vday;
short vi;
unsigned long vitem;
unsigned long litem;
short liti;
char *health;
char *healp;
short rdpick;
unsigned long tfslot;
int tfrealm;
int tfcell;
int tfidx;
unsigned long *tfgear;
short cmdret;
short *cmdvar;
unsigned short cmdrealm;
unsigned short cmdslotno;
unsigned long cmdw;
unsigned long cmdn;
unsigned long *cmdslot;
short cmdi;
short cmdk;
short *cmdp;
unsigned char lblk;
unsigned char lbln;
char *lblp;
short cmdj;
short cmdm;
unsigned short scrn;
short scrm;
short spendn;
short sympn;
short tuerx;
short tuery;
short tuerz;
char ce_val;
char ce_neg;
char ce_c;
char *ce_out;
char *ce_dst;
unsigned long cev;
unsigned long cea;
unsigned long ceb;
unsigned long *ceslot;
unsigned short cedays;
short blkdepth;
short svhour;
short svunit;
short wakereg;
short wakei;
short wakex;
short wakey;
char *rp;
short ret;
short cnt;
short lvi;
short lvj;
short lvn;
char *lvp;
unsigned long lvf;
char condv;
char *dlgp;
char *dlgq;
short dlgi;
short dlgj;
short dlgk;
short nchoice;
short nlong;
short dlgx;
short dlgy;
short dlgret;
DWORD scriptlen;
char script[450560];
DWORD scriptsize;
DWORD markoff[350];
unsigned short marknum[347];
char nodeunused1;
char nodeunused2;
short nodeunused3;
char nodeunused4;
char nodeunused5;
char *scriptpos;
unsigned char currentnode;
Node nodes[255];
Node choices[4];
unsigned short nnodes;
char speech[4000];
char nodecmds[180];
short speechpx;
char *resumeptr;
char nodetexts[2000];
char *nodetextp;
char talkflags[4];
char sceneflags[4];
short curvars[35];
unsigned long sceneitems[40];
char scenerest[66];
long vara;
long varb;
long varc;
long vard;
long vare;
long varf;
char condstring[80];
short tradekind;
char hasterms;
char reexpand;
short freetrade;
short telemapno;
short teleposx;
short teleposy;
short telehours;
int dutalk;
short sendposx;
short sendposy;
short talknext;
short gobattle;
DWORD staylen;
short talkfight;
// FIXED: the Italian "Una settimana e mezza più tardi " fills daytext's 32 bytes and its NUL writes past them. Give daytext room for every language's days.
#ifdef FEATURE_FAULTS
char daytext[64];
#else
char daytext[32];
#endif

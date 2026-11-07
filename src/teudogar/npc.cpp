#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

#pragma pack(1)

typedef struct Unit Unit;
typedef struct Step Step;
typedef struct Part Part;
typedef struct Gain Gain;
typedef struct Metal Metal;
typedef struct Gear Gear;
typedef struct Spot Spot;
typedef struct Seal Seal;
typedef struct Game Game;
typedef struct Kind Kind;
typedef struct Region Region;
typedef struct Prov Prov;
typedef struct Realm Realm;
typedef struct Points Points;
typedef struct Recipe Recipe;
typedef struct Outfit Outfit;
typedef struct Loot Loot;
typedef struct Fame Fame;
typedef struct Pose Pose;
typedef struct Anim Anim;
typedef struct Zone Zone;
typedef struct Panel Panel;
typedef struct Action Action;

/* a step a unit may take, with its score after each stage (10 bytes) */
struct Step {
	unsigned char dir;
	short score1;
	short score2;
	short score3;
	short score4;
	unsigned char kind;
};

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
	unsigned char moded;
	unsigned char sched[9][24];
	unsigned char part;
	unsigned char fe7;
	unsigned char frame;
	unsigned short scene;
	unsigned short plea;
	short strength;
	short dexterity;
	short intellect;
	short health;
	short vigor;
	unsigned char leader;
	unsigned long neck;
	unsigned long wrist;
	unsigned long belt;
	unsigned long head;
	unsigned long body;
	unsigned long hand;
	unsigned long offhand;
	unsigned long legs;
	unsigned long feet;
	unsigned long items[3];
	unsigned long cloak;
	unsigned short load;
	char spare;
	unsigned char courage;
	unsigned char turnmarks;
	unsigned char clan;
	char f132[0x134 - 0x132];
	unsigned char bewitcher;
	unsigned char asleep;
	unsigned char bewitched;
	unsigned char blessed;
	unsigned char poison;
	unsigned char cursed;
	unsigned char fleeing;
	unsigned char confused;
	unsigned char bleeding;
	unsigned char berserk;
	unsigned char drunk;
	unsigned char weakened;
	unsigned char warded;
	unsigned short home;
	unsigned char traits;
	unsigned short mercy;
	signed char sympathy;
	unsigned char type;
	unsigned char flags;
	unsigned char lord;
	char f14a[0x14c - 0x14a];
	unsigned char corpse;
	unsigned char trailx0;
	unsigned char traily0;
	unsigned char trailx1;
	unsigned char traily1;
	unsigned char trailx2;
	unsigned char traily2;
	unsigned char holdtime;
	unsigned char cellmate;
	unsigned char incoming;
	unsigned char idlestep;
	unsigned char idlepose;
	signed char idlewait;
	unsigned char modee;
	unsigned char modef;
	unsigned char modeg;
	unsigned char lasthit;
	unsigned char restturns;
	unsigned char awaketurns;
	char f15f[0x161 - 0x15f];
	short face;
	short number;
	unsigned char spouse;
	unsigned char trailx3;
	unsigned char traily3;
	unsigned char goalx;
	unsigned char goaly;
	char f16a[0x16f - 0x16a];
	unsigned char talkflags;
	char f170[0x177 - 0x170];
	Step steps[6];
	unsigned char nsteps;
	char f1b4[0x1b8 - 0x1b4];
};

/* 0x22 bytes each: partdefs (004c6f48) and parts (009cea88) */
struct Part {
	unsigned long f0;
	char recolor;
	unsigned char cansit;
	unsigned char anim;
	unsigned char type;
	char item[4];
	unsigned char outfit;
	char spare;
	short text;
	char f10[0x22 - 0x10];
};

/* a skill's minimum and the weights of the three stats in it, 8 bytes each (004c3c38) */
struct Gain {
	short min;
	short w[3];
};

/* 27 bytes each, indexed by bits 16 to 20 of an item (004c08a8) */
struct Metal {
	short skill;
	char f2[0x1b - 2];
};

/* the record by which mainmenu checks the program (004c5888): crc covers the
   n - 12 bytes from n on, crc1 the file up to off, crc2 the rest */
struct Seal {
	unsigned char magic[8];
	DWORD crc;
	DWORD n;
	DWORD off;
	DWORD crc1;
	DWORD crc2;
	DWORD f1c;
	DWORD f20;
};

/* 0x18 bytes each (005ac378) */
struct Spot {
	unsigned short tomap;
	unsigned char x;
	unsigned char y;
	unsigned char hours;
	char unused;
	char f6[0x18 - 6];
};

/* 26 bytes each, indexed like Metal (004c0bd8) */
struct Gear {
	char f0[0xe];
	short hindrance;
	char f10[0x17 - 0x10];
	unsigned char material;
	char f18[0x1a - 0x18];
};

/* 0x41 bytes each (004c6058): kindof(u) points at the kind of u->type */
struct Kind {
	char f0[0xe];
	unsigned char spare;
	unsigned char courage;
	unsigned char traits;
	unsigned char hostile;
	short strength;
	short dexterity;
	short intellect;
	short maxhealth;
	short maxvigor;
	unsigned char aptitude[0x3a - 0x1c];
	unsigned long wear;
	unsigned char loot;
	char f3f[0x41 - 0x3f];
};

/* 0xd0 bytes each (005acf80) */
struct Region {
	short x0;
	short y0;
	short x1;
	short y1;
	unsigned char realms[8];
	unsigned char lord;
	char f11;
	short exitx[8];
	short exity[8];
	unsigned char nexits;
	char f33[0x35 - 0x33];
	unsigned char tag;
	unsigned char lightx[8];
	unsigned char lighty[8];
	unsigned char n;
	unsigned char units[16];
	unsigned char nunits;
	unsigned char placex[8];
	unsigned char placey[8];
	unsigned char nplaces;
	unsigned char site;
	char f6a[0x70 - 0x6a];
	short store;
	char f72[0xd0 - 0x72];
};

/* 0x164 bytes each (005d8510) */
struct Prov {
	char f0[0xc];
	short a[16];
	short b[16];
	short n;
	char f4e[0x164 - 0x4e];
};

/* the state saved with the game, 0x5f8 bytes at 009cc470 */
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
	char f1a0[0x1ba - 0x1a0];
	char swordzones[0x1c7 - 0x1ba];
	char ambushzones[0x1d4 - 0x1c7];
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

/* 0x190 bytes each, one per region (005b9eb0) */
struct Points {
	char f0[0x15];
	unsigned char x[20];
	unsigned char y[20];
	signed char dx[20];
	signed char dy[20];
	unsigned char stay[20];
	char f79[0xb5 - 0x79];
	unsigned char lists[6][10];
	unsigned char counts[6];
	char ff7[0x190 - 0xf7];
};

/* 0x20 bytes each (0055a078): the parts of a unit by its part */
struct Recipe {
	short parts[7];
	char fe[0x20 - 0xe];
};

/* 28 bytes each (004c57c0): a part's starting items */
struct Outfit {
	unsigned long head;
	unsigned long body;
	unsigned long hand;
	unsigned long belt;
	unsigned long offhand;
	unsigned long legs;
	unsigned long feet;
};

/* 42 bytes each (004c58b0): a kind's random items and their chances */
struct Loot {
	unsigned long items[5];
	short chance[5];
	unsigned long weapons[2];
	short wchance[2];
};

/* 0x158 bytes each (005a1b78): a container and the items it holds */
struct Realm {
	unsigned long item;
	unsigned long items[50];
	unsigned short pos[50];
	char needs[0x158 - 0x130];
};

/* an entry of the hall of fame, 28 bytes each (009ccc00) */
struct Fame {
	unsigned short game;
	short unused;
	char name[0x14];
	unsigned long score;
};

/* a direction a unit may face in an idle animation, and its chance */
struct Pose {
	signed char dx;
	signed char dy;
	unsigned char chance;
	char f3[0x1b - 3];
};

/* 0x1b4 bytes each (004c4180), indexed by Part anim */
struct Anim {
	short chancea;
	short chanceb;
	Pose a[8];
	Pose b[8];
};

/* 0x22 bytes each (004f9498) */
struct Zone {
	short id;
	unsigned short name;
	char f4[0xe - 4];
	unsigned char x0;
	unsigned char y0;
	unsigned char x1;
	unsigned char y1;
	char f12[0x1a - 0x12];
	short x;
	short y;
	char f1e[0x22 - 0x1e];
};

/* 0x28 bytes each (00a4ca78) */
struct Panel {
	char f0[0x16];
	unsigned short unit;
	char f18[0x28 - 0x18];
};

/* 0x1a bytes each (004c3d68) */
struct Action {
	unsigned short name;
	char f2[6 - 2];
	short cmd;
	char f8[0x10 - 8];
	short turns;
	char f12[0x1a - 0x12];
};

#pragma pack()

unsigned short gearof(unsigned long item);
unsigned long baselook(unsigned long item);
unsigned char classof(int item);
unsigned long valueof(unsigned long item);
int isironsword(unsigned long item);
Part *partof(Unit *u);
short isservant(Unit *u);
short inregion(short x, short y, short g);
short provat(short x, short y);
short regionat(short x, short y);
Kind *kindof(Unit *u);
short isanimal(Unit *u);
short isfemale(Unit *u);
short staymode(unsigned char c);
short ispig(Unit *u);
short iscow(Unit *u);
int iswild(Unit *u);
short isarmor(unsigned long item);
short isweapon(unsigned long item);
void rearm(Unit *u);
void setweapkinds(Unit *u);
void setlevels(void);
void fillgauges(void);
short animalkind(Unit *u);
short femalekind(Unit *u);
short unfree(Unit *u);
short wearable(int item);
unsigned long puton(short n, unsigned long item);
unsigned long newitem(unsigned long item);
short isthrowing2(unsigned long item);
short ismelee(unsigned long item);
short randomtile(unsigned char k);
short isbear(Unit *u);
void setcry(Unit *u);
unsigned short wearof(unsigned long item);
void setwear(unsigned long *p, unsigned long v);
short findtype(int pos, unsigned long type);
unsigned long altlook(unsigned long item);
unsigned long coinitem(unsigned long n, int flag);
unsigned long randmeat(void);
int canput(int cell);
int addblood(int x, int y, int a, int b);
int topitem(int cell);
unsigned long popitem(int pos, char any);
int counttypes(unsigned short k, unsigned long a, unsigned long b, unsigned long c);
int stow(int k, unsigned long item);
unsigned long additem(int pos, unsigned long item);
unsigned short weightof(int code);
int corpseitem(Unit *u);
short checkposes(Unit *u);
short isrobber(Unit *u);
unsigned short findclass(int cell, unsigned char kind);
int onscreen(int x, int y);
int cansee(int x, int y);
short mapzone(short id);
int plotsteal(short a, unsigned long item);
int loadface(unsigned char *dst, unsigned char *dst2, unsigned n);
void cleartexts(void);
int armorpct(Unit *u);
short glutton(Unit *u);
short iscraftsman(Unit *u);
short issmith(Unit *u);
short myservant(Unit *u);
unsigned short itemweight(unsigned long item);
unsigned short idof(unsigned long item);
int noheir(short a, short b, short c);
int randpit(void);
unsigned long newmarker(int k, int x, int y);
int isfood(unsigned long item);
unsigned long fishfood(unsigned long item);
int satiety(unsigned long x);
int eatable(unsigned long item);
int isfirewood(unsigned long item);
void setbless(unsigned long *item, unsigned short n);
unsigned long randlook(unsigned long item);
void emptyvessel(unsigned long *item);
short canstack(unsigned long a, unsigned long item);
unsigned long addcoins(unsigned long *p, unsigned long item);
void clamptorch(unsigned long *p);
short islittorch(unsigned long item);
short isunlittorch(unsigned long item);
short setlit(unsigned long *p, short dir);
short istorch(unsigned long item);
int findcell(unsigned long n);
void delitem(int pos, unsigned long idx);
unsigned long takeitem(int pos, unsigned long idx);
char *hungertext(unsigned char n);
char *tiredtext(unsigned char n);
char *unitname(Unit *u);
void knowpotion(unsigned long item);
void syncplayer(void);
void togglecombat(short n);
short msgf(short prio, char *fmt, ...);
short denymsg(char *fmt, ...);
void delmsgid(short id);
short isthrowing(unsigned long item);
short isspear(unsigned long item);
short issword(unsigned long item);
short ofrank(Unit *u, int n);
short isherder(Unit *u);
short istrader(Unit *u);
short spellset(Unit *u);
short learned(Unit *u);
void queuesound(short n);
void playqueue(void);
int music(int n, int lp);
int opendoor(unsigned long pos, short force);
int inview(int x, int y);
unsigned long takeout(unsigned short k, unsigned short i);
short nofit(unsigned long item, short k);
void setcurregion(void);
void lookaround(void);
void tire(short n);
int autoeat(void);
short ambient(void);
void lookdone(void);
short fairgame(short a, short b);
void plotdeath(short m, short k, short a, short b);
void setflag(int n, int v);
int wary(short a, short b);
int forgives(short m, short k, short a, signed char b, short c, short d);
int ceasefire(short a, short b);
void plotfind(int a, int x, int y, unsigned long item);
void animhead(short n);
void dresshero(void);
void drawscreen(void);
void bruglowfeast(void);
int closedoor(unsigned long pos, short mode);
void linebegin(int x1, int y1, int x2, int y2);
void linestep(void);
short castat(Unit *u, short n, short cmd);
short protection(unsigned long item);
void attackval(Unit *u, short *dmg, short *kind, short *a, short *b, short n);
void defenseval(Unit *u, short *a, short *b, short *c);
int setsmoke(int cell, short n);
void updatecell(int pos);
int advance(int dx, int dy);
short askattack(void);
short hassecond(Unit *u);
void strike(Unit *a, Unit *u);
int castspell(Unit *u, unsigned char n);
short isdog(Unit *u);
short checksitanim(Unit *u);
short checkstandanim(Unit *u);
short muted(void);
short playing(short n);
int stopmusic(void);
void plotsight(short m, short k);
int loadscript(char *name);
void fixcoins(unsigned long *p);
void setlang(char c);
void loadstrs(void);
void initpanels(void);
short keepshelm(Unit *u);
void clearmsgs(void);
void stopsounds(void);
void clearqueue(void);
void markvisited(void);
int saveobjs(unsigned short map, char flag);
int autosave(void);
unsigned long packvalue(void);
short copymap(unsigned short dst, unsigned short src);
void plotleave(short map);
int loadcells(unsigned short map);
void buildparts(void);
void buildrecipes(void);
int isbasemap(int map);
unsigned long catchup(unsigned long t);
unsigned long passalone(unsigned long t);
char *atdaytime(unsigned char h);
void fmtweekday(short d);
void plotenter(short map);
void setdaylight(void);
void plotcheck(void);
void clrboth(void);
short autocombat(void);
void plotarrive(short map);
int __cdecl readinput(void);
void showtip(int a, int b, char *fmt, ...);
void screenpos(Unit *u);
void anchortip(unsigned char kind, int x, int y);
void animalcry(Unit *u);
int tipshown(void);
int plotsymp(short m, short k, int v);
unsigned long *itemptr(unsigned char where, int n);
unsigned short blessof(unsigned long item);
char *itemname(unsigned long item);
void assault(Unit *a, Unit *u);
short failmsg(char *fmt, ...);
unsigned long broken(unsigned long item);
void putcs(int x, int y, unsigned n);
void holdtip(void);
void closepanels(void);
void hidecursor(void);
void drawcursor(void);
void denypractice(void);
void waitinput(int ms);
void plotpeace(Unit *u, Unit *v);
int stowat(unsigned short k, unsigned short n, unsigned long item);
int stowspot(int k, int x, int y, unsigned long item);
void dropitem(int pos, unsigned long item);
short informal(Unit *u);
int trade(void);
void endtrade(void);
unsigned long groupkey(unsigned long item);
unsigned long typekey(unsigned long item);
unsigned long loosekey(unsigned long item);
unsigned long itemkey(unsigned long item);
int buried(int k);
void setid(unsigned long *p, unsigned long v);
int ishousepart(unsigned long item);
unsigned long breakdoor(unsigned long item);
int ispottery(unsigned long item);
short iswallns(unsigned long item);
short iswallew(unsigned long item);
int potsherd(void);
int randspeckle(void);
int itemburns(unsigned long item);
int potionknown(unsigned long item);
unsigned long inpack(short *pk, short *pj, unsigned long item, int norm);
short woulddie(unsigned long t);
short checklock(unsigned long item, unsigned char t, int i, short c);
void playnow(short n);
unsigned long passtime(unsigned long t, short ask, short b);
int gosleep(short ask);
void markers(int param);
int busyuntil(unsigned long t, short force);
short forcelock(unsigned long item, unsigned char t, int n, short k);
void talkto(unsigned short n);
void leavemenu(short anim);
void finishstep(short n);
int deadspeaks(unsigned short n);
DWORD pack(void *dst, DWORD dstlen, void *src, DWORD srclen);
DWORD unpack(void *dst, DWORD dstlen, void *src, DWORD srclen);
int unlzw(unsigned char *dst, unsigned char *src, int n);
void packrealm(unsigned short n);
void fillregions(void);
int visited(int map);
int basemap(int map);
int readunitrec(void *buf, int i);

void clearscenes(void);
int badunit(Unit *u);
void addfavor(int n);
void train(unsigned char k, int n);
void deedsteal(unsigned long item);
void deedpoison(void);
void deedcure(void);
void deedbooty(unsigned long item);
short isfit(Unit *u);
void trackregion(void);
void setowned(short n, short on);
int hasowned(short n);
void markunits(void);
short maysit(Unit *u);
short isidle(Unit *u);
short sitdown(Unit *u);
unsigned char unitat(int x, int y);
unsigned char otherat(int x, int y);
short indexof(Unit *u);
short scheduled(Unit *u);
unsigned short homeof(Unit *u);
unsigned short ownhome(Unit *u);
short samehouse(Unit *u, Unit *v);
short sameprov(Unit *u, Unit *v);
short allied(Unit *u, Unit *v);
short athome(Unit *u, short k);
short mustflee(Unit *u);
unsigned char foeof(Unit *u);
unsigned char foe2of(Unit *u);
short fightsplayer(Unit *u);
short sparring(Unit *u, Unit *v);
short seesunit(Unit *u);
int displayed(Unit *u);
void cleartrail(Unit *u);
short unitdist(Unit *u, Unit *v);
short stepdist(Unit *u, Unit *v);
short celldist(int x0, int y0, int x1, int y1);
int postdist(Unit *u, Unit *v);
int fightnear(Unit *u, short *d);
short route(Unit *u, short x, short y);
short nearfoe(int nearby);
short sparrer(void);
void setfacing(Unit *u);
void setstep(Unit *u);
void stepto(Unit *u, short x, short y);
void turnto(Unit *u, short x, short y);
void sharecells(void);
void setunitmode(Unit *u, unsigned char mode);
void duel(Unit *u, int right);
void applyhour(Unit *u, short h);
short isbusy(Unit *u);
short postblocked(Unit *u, short h);
int otherseat(Unit *u, short h);
short atpost(Unit *u, short h);
int ontask(Unit *u, short h);
void placeunits(short h);
void gopost(Unit *u);
short residentnear(short x, short y);
void talkface(short n);
void starttalk(short n);
void endtalk(void);
short ownerat(short x, short y);
int landowner(Unit *u);
int ownsland(Unit *u);
short nearregion(short x, short y);
short postregion(Unit *u);
short findheir(Unit *u, short flag);
short heirof(Unit *u);
short abdicate(Unit *u, short v);
unsigned long jewelmagic(Unit *u, unsigned long mask);
void calcload(unsigned short n);
unsigned long corpseof(unsigned long n);
void calcstats(Unit *u);
void varygear(Unit *u, unsigned long mask);
void baseitems(Unit *u);
void flagunit(Unit *u);
unsigned short newunit(unsigned char kind, unsigned char type);
short freenear(short x0, short y0, short *x, short *y);
unsigned short addpig(int n);
void addcamp(short x, short y, short flag, int n);
int addcave(void);
int fillcave(int x, int y, int type, int d, int e, int f);
int fillcaves(void);
short addrobber(short x, short y);
short addroman(short x, short y, unsigned char kind);
short addavenger(short x, short y, short v);
short addrefugee(short x, short y, short kind, short v);
void provoke(Unit *a, Unit *b, signed char dmg, short d, short e);
int packunit(short n, unsigned long item);
short haskey(Unit *u, unsigned long item);
int stowregion(Unit *u, short n, unsigned long item);
unsigned long giveitem(short n, unsigned long item, short mode);
short yields(Unit *u, short k, unsigned long item, short m);
int lordsitem(unsigned long item, int k);
int delflies(int pos, unsigned long k);
void reguard(void);
int consume(Unit *u, unsigned long item, short v);
short engaged(Unit *u);
void clearmind(Unit *u);
short baseskill(Unit *u, short n);
short skillof(Unit *u, short n, short b, short c, short d);
unsigned char skillbyte(Unit *u, short n, short b, short c, short d);
unsigned char halfskill(Unit *u, short n);
void addcap(unsigned char *p, unsigned char v);
void addpower(unsigned char *p, Unit *u, short n);
short prevails(short a, short b);
short prowess(Unit *u);
short winodds(Unit *u, Unit *v);
void addscore(int v);
void lightregion(int g, int open);
void addhealth(Unit *u, short n);
void stepaside(Unit *u);
int magicof(unsigned long item);
void regaincontrol(void);
int protect(Unit *u, Unit *v, short force);
int curepoison(Unit *u, Unit *v, short force);
void goberserk(Unit *u, short mode);
void makepeace(Unit *u, Unit *v);
int fetchitem(Unit *u, unsigned long item, int n);
int savemap(unsigned short map);
int loadmap(unsigned short map);

extern Unit units[];
extern Gain gains[];
extern Metal metals[];
extern Gear armors[];
extern int minsets;
extern int quitting;
extern Region regions[];
extern Prov provs[100];
extern unsigned char sqrttab[];
extern short nofoes;
extern char curface[];
extern char curface2[];
extern unsigned char lum[256];
extern unsigned long mapitems[];
extern Part partdefs[];
extern char recipes[1488];
extern Outfit outfits[];
extern Loot loot[];
extern unsigned char caves[][2];
extern Part parts[];
extern char *strs[];
extern char empty[1];
extern short part1;
extern short part8;
extern char lookstale;
extern int restbreak;
extern short newmsgs;
extern short playerrank;
extern short blocker;
extern Fame hiscores[];
extern short challenger;
extern long vara;
extern long varb;
extern int linestepx;
extern int linestepy;
extern int linesteps;
extern int waitforup;
extern int mouseright;
extern int rightclick;
extern int cur;
extern Anim anims[];
extern Zone zones[30];

extern unsigned short nunits;
extern unsigned short curmap;
extern unsigned long mapflags[22500];
extern unsigned long gametime;
extern short combatkind;
extern int lostcontrol;
extern Kind kinds[51];
extern short nregions;
extern unsigned char gamehour;
extern int playerx;
extern int playery;
extern unsigned long map[22500];
extern unsigned short playercell;
extern char talkactive;
extern unsigned short curunit;
extern unsigned char terrains[4096];
extern unsigned short nmapitems;
extern unsigned char typenames[4096];
extern unsigned char nrealms;
extern Realm realms[125];
extern Points points[255];
extern unsigned short hourticks;
extern char caveused[28];
extern int cheat;
extern unsigned char keys[256];
extern short merciful;
extern short pleader;
extern short talker;
extern short talkscene;
extern short avengers;
extern short victimfemale;
extern short victimname;
extern short victimzone;
extern Game game;
extern int skillpts;
extern int skstep;
extern int sksum;
extern int skleft;
extern int skworth;
extern short traink;
extern int stealval;
extern int daggerval;
extern int defeatpts;
extern short bootyval;
extern short trackreg;
extern short mucell;
extern unsigned long sitflags;
extern unsigned char uatidx;
extern Unit *uatunit;
extern unsigned char oatidx;
extern Unit *oatunit;
extern unsigned long ixoff;
extern unsigned short shhome;
extern unsigned short shhome2;
extern unsigned short sphome;
extern unsigned short sphome2;
extern short ahhome;
extern short ahreg;
extern int fuunit;
extern int fuhour;
extern short areareg;
extern short areahour;
extern short udistx;
extern short udisty;
extern short stepdx;
extern short stepdy;
extern int pdux;
extern int pduy;
extern int pdvx;
extern int pdvy;
extern short rebest;
extern short redist;
extern short rei;
extern short reexit;
extern short pebest;
extern short pedist;
extern short pei;
extern short peexit;
extern short fpi;
extern short fpunit;
extern short fpx;
extern short fpy;
extern int fpcell;
extern unsigned char fpfree[8];
extern short fpnfree;
extern unsigned char fpopen[8];
extern short fpnopen;
extern short fgunit;
extern short rtfrom;
extern short rtto;
extern short rtexit;
extern short afi;
extern short fdi;
extern short fdunit;
extern short fddist;
extern short fdmin;
extern short nfunit;
extern short nffoe;
extern short uaunit;
extern short sprunit;
extern signed char ssdx0;
extern signed char ssdy0;
extern int stdx;
extern int stdy;
extern signed char stdx0;
extern signed char stdy0;
extern short scunit;
extern short scself;
extern short scsunit;
extern unsigned char smold;
extern char smmoved;
extern int smarmor;
extern int smslot;
extern short smowner;
extern short pnx0;
extern short pny0;
extern short pnw;
extern short pnh;
extern short pnterr;
extern unsigned char pnprov;
extern short plreg;
extern short plplace;
extern unsigned char plx0;
extern unsigned char ply0;
extern int busycell;
extern short pbdx;
extern short pbdy;
extern short sfunit;
extern int oscell;
extern short osself;
extern short osplace;
extern short osreg;
extern short osx0;
extern short osy0;
extern short apreg;
extern short appost;
extern short plsunit;
extern short rnrad;
extern short rnunit;
extern short rnhour;
extern short rnreg;
extern short rnureg;
extern short tfface;
extern unsigned long vfval;
extern short vfpct;
extern short orunit;
extern short nmunit;
extern short nmfemale;
extern short nmseen;
extern short oareg;
extern short oawide;
extern short nrreg;
extern short nrwide;
extern int ocidx;
extern unsigned char occls;
extern short ocslot;
extern short ocrealm;
extern unsigned long daidx;
extern unsigned long daslot;
extern unsigned long darealm;
extern short orgx;
extern short orgy;
extern short olx;
extern short oly;
extern short fhunit;
extern short fhself;
extern Unit *fhcand;
extern unsigned char fhfoe;
extern unsigned char fhfoe2;
extern short hoheir;
extern short bqheir;
extern short bqi;
extern short bqself;
extern short abheir;
extern short abself;
extern short abi;
extern short abmaster;
extern short ntnear;
extern short ntreg;
extern short nti;
extern short ntpt;
extern short ntx;
extern short nty;
extern short ntkind;
extern int ntidx;
extern short immsum;
extern short immval;
extern unsigned long immitem;
extern short topimmob;
extern unsigned long toppiece;
extern short nextimmob;
extern unsigned long nextpiece;
extern unsigned long coidx;
extern unsigned long scoidx;
extern unsigned long scolook;
extern unsigned long scotype;
extern short snapk;
extern short csval;
extern short csload;
extern short rhours;
extern short rfgain;
extern short rfhours;
extern Unit *nuunit;
extern short nui;
extern short nuj;
extern short nupack;
extern short nuweapon;
extern short nurank;
extern short nuarmor;
extern unsigned long nuitem;
extern unsigned short fntry;
extern int fncell;
extern short fndy;
extern int ununit;
extern unsigned short pign;
extern short pigx;
extern short pigy;
extern int cloakroll;
extern unsigned long cloakitem;
extern int goodsroll;
extern int utensilroll;
extern int dropx;
extern int dropy;
extern int dropr;
extern int dropheavy;
extern int puttop;
extern unsigned long groundcell;
extern int ncdist;
extern int ncbest;
extern int ncmin;
extern int nci;
extern int caveroll;
extern int acx;
extern int acy;
extern int aci;
extern unsigned long accell;
extern int strewcell;
extern int strewroll;
extern int strewlast;
extern int fcx;
extern int fcy;
extern int fcn;
extern int fci;
extern int fccell;
extern unsigned long fcv;
extern int playmode;
extern short dirdx[8];
extern short dirdy[8];
extern unsigned short playernext;
extern short gameday;
extern char knockedout;
extern char mapstate[100];
extern unsigned char palblack;
extern char trademode;
extern char combatcause;
extern short sensesleft;
extern short freedirs[8];
extern short nfreedirs;
extern unsigned char passmode;
extern int hourtime;
extern int targetidx;
extern int targetx;
extern int targety;
extern int fcsi;
extern unsigned short arunit;
extern unsigned short ari;
extern short ark;
extern short arface;
extern unsigned short romunit;
extern unsigned short avunit;
extern unsigned short refunit;
extern short dui;
extern short standsum;
extern short standi;
extern short standself;
extern short tgplayer;
extern short tgdist;
extern short tgdist2;
extern short tgtmp;
extern unsigned char tgunit;
extern short angera;
extern short angerb;
extern short provi;
extern short prova;
extern short provb;
extern short wasdoomed;
extern int quaffing;
extern unsigned char puclass;
extern short puslot;
extern short keyslot;
extern short srgi;
extern short sedpart;
extern unsigned char giclass;
extern unsigned long giorig;
extern unsigned long gitmp;
extern unsigned long gival;
extern unsigned long gimin;
extern unsigned long gilook;
extern short gii;
extern short gislot;
extern short gihome;
extern short gij;
extern int gitop;
extern short handi;
extern short thhour;
extern unsigned char thx;
extern unsigned char thy;
extern short orphhour;
extern short subi;
extern short subself;
extern short subunit;
extern short subpass;
extern int killv;
extern unsigned short killcell;
extern unsigned short killi;
extern short killled;
extern short killfoe;
extern short killplayer;
extern short killfair;
extern unsigned short dcobj;
extern unsigned short dclook;
extern short dcbones;
extern unsigned short dccell;
extern unsigned short dcalt;
extern unsigned long dcword;
extern int dcx;
extern int dcy;
extern short ofhour;
extern short ighour;
extern short rgi;
extern short rghour;
extern short cnfill;
extern unsigned short cnv;
extern unsigned char cnclass;
extern short cndrink;
extern short cnmsg;
extern int cpfrom;
extern int cpto;
extern short fsdir;
extern short fsunit;
extern short fsdx;
extern short fsdy;
extern short fsx;
extern short fsy;
extern unsigned short fscell;
extern unsigned short stepcell;
extern unsigned short dircells[8];
extern short dirdoors[8];
extern short bsi;
extern short bsdir;
extern short bsv;
extern short bsx;
extern short bsy;
extern short bsdist;
extern short bsprov;
extern unsigned long bscell;
extern short tsdir;
extern short newhealth;
extern Unit *torchu;
extern short torchi;
extern short torchdist;
extern short bestdirs[8];
extern short nbestdirs;
extern short dirscores[8];
extern short turni;
extern Unit *mvu;
extern unsigned short mvi;
extern int mvcell;
extern int mvdoor;
extern unsigned short mvx;
extern unsigned short mvy;
extern short bkstat;
extern short bklevel;
extern short sostat;
extern short solevel;
extern short sopct;
extern short sbv;
extern unsigned char hsv;
extern short oddsv;
extern unsigned char pwn;
extern short pwatk;
extern short pwdef;
extern short pwprot;
extern short pwparry;
extern short pwlo;
extern short pwhi;
extern short pwkind;
extern short wou;
extern short wov;
extern short charmpct;
extern short hsrow;
extern short bai;
extern short bapass;
extern short spardist;
extern short spari;
extern short sparany;
extern int hli;
extern int lcobj;
extern int lccell;
extern int lri;
extern short pplist;
extern short ppi;
extern short pptry;
extern short ppx;
extern short ppy;
extern short ppj;
extern short ppself;
extern short ppfire;
extern int ppcell;
extern int wni;
extern int wnrank;
extern int wnbest;
extern int wnmin;
extern int wndist;
extern int hmi;
extern int lineokx;
extern int lineoky;
extern int lineendx;
extern int lineendy;
extern short sparending;
extern Unit *curu;
extern int curi;
extern unsigned long uttmp;
extern int utcell;
extern short uta;
extern short utb;
extern short utc;
extern short utdx;
extern short utdy;
extern short utblood;
extern int utdoor;
extern int uthere;
extern short utwalk;
extern short utmode;
extern unsigned char utanim;
extern short utcow;
extern short utdog;
extern short utpig;
extern short uthorse;
extern short utseen;
extern short utpdx;
extern short utpdy;
extern short utfx;
extern short utfy;
extern unsigned long barkat;
extern char packbuf[616000];
extern char texts[47942];
extern int afterupdate;
extern int afterinst;
extern int otherver;
extern unsigned short enteritems;
extern unsigned char enterflag;
extern int entersum;
extern int lang;
extern char scriptfile[1000];
extern unsigned char targetkind;
extern unsigned long targetitem;
extern unsigned short weights[4096];
extern int clocktime;
extern unsigned short clockhour;
extern short clockday;
extern int clocktime2;
extern unsigned short clockhour2;
extern short clockday2;
extern unsigned char notedkind;
extern int notedposx;
extern int notedposy;
extern unsigned long noteditem;
extern short waterpace;
extern short regiontag;
extern short sunlight;
extern short lastlight;
extern DWORD vermajor;
extern short lastfail;
extern int autolevel;
extern unsigned char skillsnow;
extern unsigned char combatnow;
extern unsigned char minmarks;
extern unsigned short lasttick;
extern unsigned char panelstack[16];
extern unsigned short notedunit;
extern short notedmap;
extern unsigned long notedtime;
extern short askedonce;
extern short askedunit;
extern int turncount;
extern unsigned short busyturns;
extern unsigned char savebyte1;
extern short skiptick;
extern short actionturn;
extern short walkedsum;
extern short walkeddx;
extern short walkeddy;
extern short saveword1;
extern short saveword2;
extern short saveword3;
extern short saveword4;
extern short saveword5;
extern short saveword6;
extern short saveword7;
extern short saveword8;
extern short saveword9;
extern char savebyte3;
extern char spotsong;
extern short spotday;
extern short curregion;
extern char printbuf[4800];
extern DWORD verminor;
extern DWORD verflags;
extern unsigned long acttime;
extern unsigned char actsel;
extern unsigned char quickacts[10];
extern char szvars[400];
extern int notes[500];
extern unsigned short nnotes;
extern char scenevars[5000];
extern int scenevarlen;
extern short trvfrom;
extern short trvto;
extern short trvleft;
extern int lefttime;
extern int shoetime;
extern unsigned char shoewarns;
extern int savelong;
extern unsigned short steptick;
extern unsigned short stepframe;
extern unsigned fireframe;
extern unsigned slowframe;
extern unsigned char savebyte2;
extern short handframe;
extern short headframe;
extern short footframe;
extern unsigned long blinkclock;
extern short heroblink;
extern short curblink;
extern unsigned long heroblinkat;
extern unsigned long curblinkat;
extern short headanim;
extern short headstep;
extern short handanim;
extern short handstep;
extern short footanim;
extern short footstep;
extern int bloodtill;
extern Panel panels[16];
extern unsigned char showtotal;
extern unsigned char tradehints;
extern int dutalk;
extern DWORD nforsale;
extern DWORD pricemarkup;
extern unsigned char esckey;
extern unsigned short prices[4096];
extern unsigned long sumi;
extern unsigned long sumj;
extern unsigned long sumv;
extern HANDLE savefh;
extern unsigned char *savep;
extern char *saveat;
extern unsigned long savesum;
extern int savex;
extern HANDLE loadfh;
extern unsigned char *loadp;
extern char *loadpos;
extern unsigned long loadsum;
extern unsigned long loadsize;
extern int loadtmp;
extern short saveditems;
extern int savedsum;
extern short loadlang;
extern int fixi;
extern int fixj;
extern int fixk;
extern int fixx;
extern int fixy;
extern int lootx;
extern int looty;
extern short lootarea;
extern short lootlord;
extern short looti;
extern short lootb;
extern short loothow;
extern short lootn;
extern unsigned long lootobj;
extern int lootcell;
extern int lootdest;
extern unsigned long lootprev;
extern unsigned long lootnext;
extern unsigned long lootunit;
extern Unit *lootu;
extern int scanx;
extern int scany;
extern unsigned short scanobj;
extern unsigned short scancell;
extern unsigned long scanv;
extern unsigned long scanw;
extern unsigned short scankind;
extern unsigned long backdiff;
extern unsigned long backhours;
extern unsigned long backdays;
extern short backhour;
extern short trvc;
extern short trvr;
extern short trvunit;
extern short trvn;
extern int trvhour;
extern int trvlead;
extern short trvmode;
extern short trvflag;
extern short trvmoved;
extern unsigned long trvtime;
extern unsigned long trvhours;
extern short trvhour0;
extern short trvday0;
extern short trvmsg;
extern int trvok;
extern int maploaded;
extern short censorship;
extern char trvfailmsg;
extern unsigned long mapsum;
// FIXED: as at its definition in dialog.cpp, daytext's room for every language's days
#ifdef FEATURE_FAULTS
extern char daytext[64];
#else
extern char daytext[32];
#endif
// FIXED: the whole writes of savegame and savemap, in mainmenu.cpp
#if defined(FEATURE_FIXES)
int openwhole(HANDLE *h);
int closewhole(HANDLE *h, char *name, int ok);
#endif
extern int shown;
extern short lastwarm;
extern unsigned char cmbautooff;
extern short movi;
extern short movx;
extern short movy;
extern int movcell;
extern int tipanchorx;
extern int tipanchory;
extern short screenposx;
extern short screenposy;
extern short nmsgs;
extern short tipclears;
extern unsigned long *cep;
extern int frfaith;
extern unsigned long *enp;
extern unsigned short enbless;
extern unsigned short endiff;
extern unsigned char enpower;
extern unsigned char sfxswitch;
extern unsigned long *sacp;
extern unsigned long sachi;
extern unsigned char sacclass;
extern unsigned char sacgear;
extern unsigned short sacval;
extern unsigned long *adjmap;
extern unsigned long *adjflags;
extern unsigned short vaccell;
extern short summi;
extern short summx;
extern short summy;
extern short dhx;
extern short dhy;
extern short dhk;
extern char workbuf[616000];
extern int scrollx;
extern int scrolly;
extern unsigned char runepast;
extern unsigned char runenow;
extern unsigned char runenext;
extern unsigned char menumode;
extern unsigned char protpot;
extern unsigned char protspell;
extern unsigned char rage;
extern int ragecell;
extern short ragesave;
extern short mphour;
extern short pmi;
extern short pmself;
extern short phi;
extern short phself;
extern short pai;
extern short paself;
extern unsigned char goodsclass;
extern unsigned long tradegoods[24];
extern unsigned short goodsrealm[24];
extern unsigned short goodsspot[24];
extern int tradei;
extern int ggn;
extern int ggi;
extern int ggj;
extern int gghome;
extern int ggk;
extern int ggrealm;
extern int ggx;
extern int ggy;
extern int ggpass;
extern int ggmaster;
extern short ggowner;
extern short ggspouse;
extern unsigned long ggitem;
extern int ggslot;
extern short barteri;
extern unsigned short bartercur;
extern int nfound;
extern short seekreg;
extern short seeki;
extern short seekrealm;
extern short seekslot;
extern short seekx;
extern short seeky;
extern short seekx0;
extern short seekx1;
extern short seeky0;
extern short seeky1;
extern short seekowner;
extern short seekself;
extern short seekfood;
extern short seekheavy;
extern unsigned long seekitem;
extern int seekcell;
extern int seekobj;
extern unsigned long seeklast;
extern unsigned long (*seekkey)(unsigned long item);
extern unsigned long plclass;
extern unsigned long plname;
extern int sackx;
extern int sacky;
extern int sackrealm;
extern int sackslot;
extern int sackcell;
extern int sackobj;
extern unsigned long sackitem;
extern short sackclass;
extern short srreg;
extern short sri;
extern short srrealm;
extern short srslot;
extern unsigned long sritem;
extern unsigned long dpfound;
extern unsigned long dpitem;
extern unsigned short dprealm;
extern unsigned short dpslot;
extern int ferealm;
extern int fecell;
extern int feslot;
extern unsigned long berserkitems[6];
extern unsigned long tbfound;
extern unsigned long tbitem;
extern int tbi;
extern unsigned short tbrealm;
extern unsigned short tbslot;
extern Unit *cptarget;
extern short cpdx;
extern short cpdy;
extern short cpcheat;
extern int pfres;
extern Unit *pftarget;
extern unsigned long pfobj;
extern short pfa;
extern short pfb;
extern short pfk;
extern unsigned char pfc;
extern unsigned char pffound;
extern Action actions[40];
extern int showall;
extern void (*viewhook)(void);
extern int hoverx;
extern int hovery;
extern unsigned short talkstate;
extern int syx;
extern int syy;
extern int syobj;
extern short syrealm;
extern HANDLE smapfh;
extern char smapname[16];
extern unsigned long smaplen;
extern char *smapp;
extern unsigned short nspots;
extern Spot spots[128];
extern char *umapp;
extern unsigned char umapused;
extern int umapi;
extern int umapj;
extern int umapk;
extern HANDLE lmapfh;
extern char lmapname[16];
extern unsigned long lmaplen;

void
clearscenes(void)
{
	merciful = 0;
	pleader = 0;
	talker = 0;
	talkscene = 0;
}

void
clearavenge(void)
{
	avengers = 0;
	victimfemale = 0;
	victimname = 0;
	victimzone = 0;
}

int
badunit(Unit *u)
{
	/* n moves wpp386's register choices in unitat and stowregion
	   to the original's, and gives rununits the size that places
	   yields' call to magicof */
	int n;

	if (u >= &units[nunits])
		return 1;
	return 0;
}

void
addfavor(int n)
{
	addscore(n);
	if (n >= 0) {
		if (game.favor > 320000)
			return;
	} else if (game.favor < -320000)
		return;
	game.favor += n;
}

void
addskill(unsigned char k, int n)
{
	addscore(n);
	skillpts = game.skills[k];
	skillpts += n;
	if (skillpts < 70)
		skillpts = 70;
	else if (skillpts > 70000)
		skillpts = 70000;
	game.skills[k] = skillpts;
	if (game.strpoints < 0x799e1460)
		game.strpoints += gains[k].w[0] * n;
	if (game.dexpoints < 0x799e1460)
		game.dexpoints += gains[k].w[1] * n;
	if (game.intpoints < 0x799e1460)
		game.intpoints += gains[k].w[2] * n;
	if (n > 1)
		game.snaptime[k] = gametime;
}

void
setskill(short k)
{
	sksum = 0;
	skleft = game.skills[k];
	for (skstep = 0; skleft > 0; skstep++) {
		skworth = 70;
		if (skleft < 70)
			skworth = skleft;
		skleft -= skworth;
		if (skstep > 100) {
			if (skstep <= 110)
				skworth -= (skstep - 100) * 10 / 10;
			else if (skstep <= 120)
				skworth -= (skstep - 100) * 12 / 10;
			else if (skstep <= 130)
				skworth -= (skstep - 100) * 15 / 10;
			else
				skworth -= (skstep - 100) * 20 / 10;
			if (skworth < 5)
				skworth = 5;
		}
		sksum += skworth;
	}
	kinds[19].aptitude[k] = sksum / 70;
	if (kinds[19].aptitude[k] > 160)
		kinds[19].aptitude[k] = 160;
	else if (kinds[19].aptitude[k] < 40)
		kinds[19].aptitude[k] = 40;
	game.levels[k] = kinds[19].aptitude[k];
}

void
train(unsigned char k, int n)
{
	addskill(k, n);
	if (n <= 0)
		return;
	if (k == 0x14 && n > 0) {
		addskill(0x15, 1);
		addskill(0x16, 1);
		addskill(0x17, 1);
	}
	switch (k) {
	case 0x17:
		addfavor(n * 3);
		break;
	case 0xb:
	case 0xc:
	case 0xd:
	case 0xe:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x18:
	case 0x19:
		addfavor(n);
		break;
	}
	for (traink = 0; traink < 30; traink++)
		setskill(traink);
}

unsigned char
weaponskill(void)
{
	return metals[gearof(units[0].hand)].skill;
}

void
deedsteal(unsigned long item)
{
	if (plotsteal(curmap, item))
		return;
	if (game.stolen < 0xffff)
		game.stolen++;
	if ((item & 0xfff) == 0x270)
		item = 0xe901cf;
	stealval = valueof(item);
	daggerval = valueof(0xe401ca);
	train(0x16, -(stealval * 20) / daggerval);
	addfavor(-(stealval * 250) / daggerval);
	setbit(game.swordzones, mapzone(curmap), 1);
}

void
deedkill(void)
{
	if (game.killed < 0xffff)
		game.killed++;
	train(0x14, 20);
	train(0x16, 5);
	train(0x17, 10);
	addfavor(50);
}

void
deedkillwoman(void)
{
	if (game.killed < 0xffff)
		game.killed++;
	train(0x14, -140);
	train(0x16, -350);
	train(0x17, -140);
	addfavor(-3000);
}

void
deedkillflee(void)
{
	if (game.runkills < 0xffff)
		game.runkills++;
	train(0x14, -15);
	train(0x16, -10);
	train(0x17, -10);
	addfavor(-40);
}

void
deedsleeper(void)
{
	if (game.sleepers < 0xffff)
		game.sleepers++;
	train(0x14, -350);
	train(0x16, -700);
	train(0x17, -233);
	addfavor(-100);
}

void
deedpoison(void)
{
	if (game.poisoned < 0xffff)
		game.poisoned++;
	train(0x14, -350);
	train(0x16, -700);
	train(0x17, -233);
	addfavor(-100);
}

void
deedcure(void)
{
	if (game.poisoned > 0)
		game.poisoned--;
	train(0x14, 350);
	train(0x16, 700);
	train(0x17, 233);
	addfavor(100);
}

void
deedambush(void)
{
	if (game.ambushed < 0xffff)
		game.ambushed++;
	train(0x14, -10);
	train(0x16, -30);
	train(0x17, -50);
	setbit(game.ambushzones, mapzone(curmap), 1);
	addfavor(5);
}

void
deedambushwoman(void)
{
	if (game.ambushed < 0xffff)
		game.ambushed++;
	train(0x14, -140);
	train(0x16, -210);
	train(0x17, -140);
	setbit(game.ambushzones, mapzone(curmap), 1);
	addfavor(-2000);
}

void
deedhitwoman(void)
{
	if (game.womenhit < 0xffff)
		game.womenhit++;
	train(0x14, -10);
	train(0x16, -10);
	train(0x17, -10);
}

void
deedbreakword(void)
{
	if (game.broken < 0xffff)
		game.broken++;
	train(0x14, -175);
	train(0x16, -350);
	train(0x17, -175);
	addfavor(-500);
}

void
deeddefeat(Unit *u)
{
	defeatpts = 100;
	switch (u->type) {
	case 0x18:
	case 0x1e:
	case 0x2d:
	case 0x2e:
		defeatpts += 50;
		break;
	case 0x32:
		defeatpts = 170;
		break;
	case 0x1a:
		defeatpts = 150;
		break;
	case 0x1f:
		defeatpts = 140;
		break;
	}
	switch (baselook(u->body & 0xfff)) {
	case 0x1af:
	case 0x1b3:
		defeatpts += 5;
		break;
	case 0x1b0:
	case 0x1b1:
	case 0x1b2:
	case 0x1b4:
		defeatpts += 15;
		break;
	}
	if (isironsword(u->hand))
		defeatpts += 25;
	if (classof(u->offhand & 0xfff) == 0x55)
		defeatpts += 5;
	if (u->head)
		defeatpts += 5;
	if (game.defeated < 0xffff)
		game.defeated++;
	train(0x14, defeatpts * 23 / 100);
	train(0x16, defeatpts * 35 / 100);
	train(0x17, defeatpts * 35 / 100);
	addfavor(defeatpts * 50 / 100);
	if (combatkind != 2)
		combatkind = 3;
}

void
deedmercy(void)
{
	if (game.mercies < 0xffff)
		game.mercies++;
	train(0x14, -175);
	train(0x16, -700);
	train(0x17, -700);
	addfavor(-1000);
}

void
deedsink(unsigned long item)
{
	game.sunk += valueof(item);
	if (game.sunk > 0xffff0000)
		game.sunk = 0xffff0000;
}

void
deedbooty(unsigned long item)
{
	// FIXED: bootyval keeps the value in a short, so an item worth 32,768 or more adds a negative booty. Add the value as valueof returns it.
#if defined(FEATURE_FIXES)
	addscore(valueof(item));
	game.booty += valueof(item);
#else
	bootyval = valueof(item);
	addscore(bootyval);
	game.booty += bootyval;
#endif
	if (game.booty > 0xffff0000)
		game.booty = 0xffff0000;
}

void
deedquest(int n)
{
	if (n <= 0)
		n = 1;
	else if (n > 5)
		n = 5;
	if (game.quests < 0xffff)
		game.quests++;
	train(0x14, n * 70 / 5);
	train(0x16, n * 70 / 6);
	train(0x17, n * 70 / 6);
}

void
learnquest(void)
{
	train(0x14, 14);
	train(0x16, 17);
	train(0x17, 14);
}

void
learnlore(void)
{
	train(0x14, 14);
	train(0x18, 35);
	train(0x15, 17);
}

void
learnaction(void)
{
	train(0x14, 14);
	train(0x18, 35);
	train(0x15, 35);
	train(0x16, 17);
	train(0x17, 14);
}

void
learnplace(void)
{
	train(0x14, 14);
	train(0x18, 35);
	train(0x15, 17);
}

void
learnflag(void)
{
	train(0x14, 1);
	train(0x18, 1);
	train(0x15, 1);
}

void
learntalk(void)
{
	train(0x14, 1);
}

void
learnfight(short n)
{
	train(0x14, 17);
	train(0x16, 14);
	train(0x17, 14);
}

short
favor(void)
{
	if (game.favor < -5000)
		return -2;
	if (game.favor < -2500)
		return -1;
	if (game.favor < 2500)
		return 0;
	if (game.favor < 5000)
		return 1;
	return 2;
}

short
exhausted(void)
{
	if (units[0].asleep > 0)
		return 0;
	if (units[0].health <= 0)
		return 0;
	if (game.tiredcount < 720 && units[0].dexterity >= 5 && units[0].strength >= 5)
		return 0;
	return 1;
}

short
weary(void)
{
	if (game.tiredcount >= 660 || units[0].dexterity < 15 || units[0].strength < 15)
		return 1;
	return 0;
}

short
isfit(Unit *u)
{
	if (badunit(u) || u->asleep || u->mode == 90 || u->health <= 15 || u->bewitched || u->fleeing || u->confused
	    || u->berserk || u->dexterity < 15 || u->strength < 15)
		return 0;
	if (u == &units[0] && (exhausted() || weary()))
		return 0;
	return 1;
}

int
playerout(void)
{
	if (units[0].health <= 0 || units[0].asleep || exhausted() || lostcontrol || quitting)
		return 1;
	return 0;
}

void
queuetext(short a, unsigned char b)
{
	if (!game.queuedtext || b >= game.queuedprio) {
		game.queuedtext = a;
		game.queuedprio = b;
	}
}

void
trackregion(void)
{
	trackreg = regionat(units[0].x, units[0].y);
	if (trackreg != game.lastregion) {
		game.lastregion = trackreg;
		game.regiontime = gametime;
	}
}

void
forgetplayer(Unit *u)
{
	if (!badunit(u) && u->health > 0)
		u->talkflags &= ~8;
}

void
setowned(short n, short on)
{
	if (on == 0) {
		units[n].head &= ~0x1000;
		units[n].body &= ~0x1000;
		units[n].hand &= ~0x1000;
		units[n].belt &= ~0x1000;
		units[n].offhand &= ~0x1000;
		units[n].legs &= ~0x1000;
		units[n].feet &= ~0x1000;
		units[n].items[0] &= ~0x1000;
		units[n].items[1] &= ~0x1000;
		units[n].items[2] &= ~0x1000;
		units[n].cloak &= ~0x1000;
		units[n].wrist &= ~0x1000;
		units[n].neck &= ~0x1000;
	} else {
		if (units[n].head)
			units[n].head |= 0x1000;
		if (units[n].body)
			units[n].body |= 0x1000;
		if (units[n].hand)
			units[n].hand |= 0x1000;
		if (units[n].belt)
			units[n].belt |= 0x1000;
		if (units[n].offhand)
			units[n].offhand |= 0x1000;
		if (units[n].legs)
			units[n].legs |= 0x1000;
		if (units[n].feet)
			units[n].feet |= 0x1000;
		if (units[n].items[0])
			units[n].items[0] |= 0x1000;
		if (units[n].items[1])
			units[n].items[1] |= 0x1000;
		if (units[n].items[2])
			units[n].items[2] |= 0x1000;
		if (units[n].cloak)
			units[n].cloak |= 0x1000;
		if (units[n].wrist)
			units[n].wrist |= 0x1000;
		if (units[n].neck)
			units[n].neck |= 0x1000;
	}
}

int
hasowned(short n)
{
	if (units[n].head && (units[n].head & 0x1000))
		return 1;
	if (units[n].body && (units[n].body & 0x1000))
		return 1;
	if (units[n].hand && (units[n].hand & 0x1000))
		return 1;
	if (units[n].belt && (units[n].belt & 0x1000))
		return 1;
	if (units[n].offhand && (units[n].offhand & 0x1000))
		return 1;
	if (units[n].legs && (units[n].legs & 0x1000))
		return 1;
	if (units[n].feet && (units[n].feet & 0x1000))
		return 1;
	if (units[n].items[0] && (units[n].items[0] & 0x1000))
		return 1;
	if (units[n].items[1] && (units[n].items[1] & 0x1000))
		return 1;
	if (units[n].items[2] && (units[n].items[2] & 0x1000))
		return 1;
	if (units[n].cloak && (units[n].cloak & 0x1000))
		return 1;
	if (units[n].wrist && (units[n].wrist & 0x1000))
		return 1;
	if (units[n].neck && (units[n].neck & 0x1000))
		return 1;
	return 0;
}

void
occupy(Unit *u)
{
	if (badunit(u))
		return;
	if (u->pose == 1)
		mapflags[u->x + u->dirx + (u->y + u->diry) * 150] |= 4;
	else
		mapflags[u->x + u->y * 150] |= 4;
}

void
vacate(Unit *u)
{
	if (badunit(u))
		return;
	if (u->pose == 1)
		mapflags[u->x + u->dirx + (u->y + u->diry) * 150] &= ~4;
	else
		mapflags[u->x + u->y * 150] &= ~4;
}

void
markunits(void)
{
	for (mucell = 0; mucell < 22500; mucell++)
		mapflags[mucell] &= ~4;
	for (mucell = 1; mucell < nunits; mucell++)
		occupy(&units[mucell]);
}

short
maysit(Unit *u)
{
	if (badunit(u))
		return 0;
	if (u->mode == 0 || u->mode == 2)
		return 1;
	if (isservant(u))
		return 0;
	if (u->mode == 1 || u->mode == 7 || u->mode == 3)
		return 1;
	return 0;
}

short
isidle(Unit *u)
{
	if (badunit(u))
		return 0;
	switch (u->mode) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 7:
	case 0x32:
		return 1;
	default:
		return 0;
	}
}

void
resetpose(Unit *u, unsigned char v)
{
	if (badunit(u))
		return;
	u->idlepose = 0;
	u->idlewait = v;
	u->idlestep = u->idlepose;
}

short
sitdown(Unit *u)
{
	if (badunit(u))
		return 0;
	if (u->pose != 0)
		return 0;
	sitflags = mapflags[u->x + u->y * 150];
	if (!(sitflags & 0x40))
		return 0;
	if (minsets != 0)
		return 0;
	if (!partof(u)->cansit)
		return 0;
	if (u->pose == 1 || u->pose == 3)
		return 0;
	if (!maysit(u))
		return 0;
	u->pose = 10;
	switch (sitflags & 3) {
	case 0:
		u->dirx = 0;
		u->diry = -1;
		break;
	case 3:
		u->dirx = 1;
		u->diry = 0;
		break;
	case 1:
		u->dirx = -1;
		u->diry = 0;
		break;
	case 2:
	default:
		u->dirx = 0;
		u->diry = 1;
		break;
	}
	return 1;
}

unsigned char
unitat(int x, int y)
{
	for (uatidx = 0; uatidx < nunits; uatidx++) {
		uatunit = &units[uatidx];
		if (uatunit->health > 0) {
			if (uatunit->x == x && uatunit->y == y)
				return uatidx;
			if (uatunit->pose == 1 && uatunit->x + uatunit->dirx == x
			    && uatunit->y + uatunit->diry == y)
				return uatidx;
		}
	}
	return 0xff;
}

unsigned char
otherat(int x, int y)
{
	for (oatidx = 1; oatidx < nunits; oatidx++) {
		oatunit = &units[oatidx];
		if (oatunit->health > 0) {
			if (oatunit->x == x && oatunit->y == y)
				return oatidx;
			if (oatunit->pose == 1 && oatunit->x + oatunit->dirx == x
			    && oatunit->y + oatunit->diry == y)
				return oatidx;
		}
	}
	return 0xff;
}

short
indexof(Unit *u)
{
	// PORT: the offset of u as a difference of pointers, which 64 bits keep whole
#ifdef PORT
	ixoff = (char *)u - (char *)units;
#else
	ixoff = (unsigned long)u - (unsigned long)units;
#endif
	ixoff /= sizeof(Unit);
	return ixoff;
}

int
unititem(Unit *u)
{
	return (indexof(u) << 16) + 0x270;
}

short
scheduled(Unit *u)
{
	if (badunit(u) || (u->traits & 4) || u->health <= 0)
		return 0;
	return 1;
}

void
setfought(Unit *u)
{
	u->talkflags |= 1;
}

int
hasfought(Unit *u)
{
	if (u->talkflags & 1)
		return 1;
	return 0;
}

unsigned short
homeof(Unit *u)
{
	if (badunit(u))
		return 0;
	return u->home & 0x7fff;
}

unsigned short
ownhome(Unit *u)
{
	if (badunit(u) || (u->home & 0x8000))
		return 0;
	return u->home & 0x7fff;
}

short
samehouse(Unit *u, Unit *v)
{
	if (badunit(u) || badunit(v) || isanimal(u) || isanimal(v))
		return 0;
	if ((shhome = homeof(u)) == 0 || (shhome2 = homeof(v)) == 0)
		return 0;
	if (shhome2 != shhome
	    && (!regions[shhome].lord || regions[shhome].lord != regions[shhome2].lord))
		return 0;
	return 1;
}

short
sameprov(Unit *u, Unit *v)
{
	if (badunit(u) || badunit(v) || isanimal(u) || isanimal(v))
		return 0;
	if ((sphome = homeof(u)) == 0 || (sphome2 = homeof(v)) == 0)
		return 0;
	if (!regions[sphome].site || regions[sphome].site != regions[sphome2].site)
		return 0;
	return 1;
}

short
allied(Unit *u, Unit *v)
{
	if (badunit(u) || badunit(v) || isanimal(u) || isanimal(v) || u->bewitched || v->bewitched)
		return 0;
	if (samehouse(u, v) || u->clan && u->clan == v->clan)
		return 1;
	if (u->spouse && u->spouse == indexof(v) || v->spouse && v->spouse == indexof(u))
		return 1;
	return 0;
}

short
athome(Unit *u, short k)
{
	if (badunit(u))
		return 0;
	ahhome = ownhome(u);
	ahreg = regionat(u->x, u->y);
	if (k && ahreg && isservant(u) && regions[ahreg].lord == u->lord)
		return ahreg;
	if (ahhome == 0) {
		if (k == 0)
			return 0;
		if (!isservant(u))
			return 0;
		ahhome = homeof(u);
		if (ahhome == 0)
			return 0;
		if (regions[ahhome].lord != u->lord)
			return 0;
	}
	if (ahreg == ahhome)
		return ahreg;
	if (ahreg && regions[ahreg].lord == regions[ahhome].lord)
		return ahreg;
	return 0;
}

short
mustflee(Unit *u)
{
	if (badunit(u) || u->bewitched)
		return 0;
	if (u->courage <= 1) {
		if (u->health < kindof(u)->maxhealth / 2)
			return 1;
		if ((isfemale(u) || ispig(u)) && u->health < kindof(u)->maxhealth * 2 / 3)
			return 1;
		if (iscow(u) && u->health < kindof(u)->maxhealth * 4 / 5)
			return 1;
	} else if (u->courage <= 2) {
		if (u->health < kindof(u)->maxhealth / 3)
			return 1;
	}
	return 0;
}

short
hastarget(Unit *u, unsigned char a, unsigned char b)
{
	if (badunit(u))
		return 0;
	if (u->mode == 0x14 || u->mode == 0x15) {
		// FIXED: 0xff, no foe, matches the 0xff of a unit with one foe, so findheir takes any fighter. Match a foe alone.
#if defined(FEATURE_FIXES)
		if (a != 0xff && (a == u->modeb || a == u->modec) || b != 0xff && (b == u->modeb || b == u->modec))
#else
		if (a == u->modeb || a == u->modec || b == u->modeb || b == u->modec)
#endif
			return 1;
	}
	return 0;
}

unsigned char
foeof(Unit *u)
{
	if (u->mode == 0x14 || u->mode == 0x15 || u->mode == 0x19)
		return u->modeb;
	return 0xff;
}

unsigned char
foe2of(Unit *u)
{
	if (u->mode == 0x14 || u->mode == 0x15)
		return u->modec;
	return 0xff;
}

short
fightsplayer(Unit *u)
{
	if (u->mode == 0x14 || u->mode == 0x15) {
		if (u->modeb == 0 || u->modec == 0)
			return 1;
	}
	return 0;
}

short
fleesplayer(Unit *u)
{
	if (u->mode == 0x19 && u->modeb == 0)
		return 1;
	return 0;
}

/* reads u->mode with u null when v is a live unit not in mode 0x16 */
short
sparring(Unit *u, Unit *v)
{
	if (u == NULL && badunit(v) == 0) {
		if (v->mode == 0x16)
			return 1;
	} else if (v == NULL && badunit(u) == 0) {
		if (u->mode == 0x16)
			return 1;
	} else {
		if (badunit(u))
			return 0;
		if (badunit(v))
			return 0;
	}
	if (u->mode == 0x16 && u->modeb == indexof(v))
		return 1;
	if (v->mode == 0x16 && v->modeb == indexof(u))
		return 1;
	return 0;
}

short
seesunit(Unit *u)
{
	if (badunit(u))
		return 0;
	return cansee(u->x, u->y);
}

int
displayed(Unit *u)
{
	if (badunit(u))
		return 0;
	return onscreen(u->x, u->y);
}

void
fillunits(int g)
{
	if (g == 0 || g >= nregions)
		return;
	memset(regions[g].units, 0, 16);
	regions[g].nunits = 0;
	for (fuunit = 1; fuunit < nunits && regions[g].nunits < 16; fuunit++) {
		if (units[fuunit].health <= 0 || isanimal(&units[fuunit]))
			continue;
		if (!scheduled(&units[fuunit])) {
			if (inregion(units[fuunit].x, units[fuunit].y, g))
				regions[g].units[regions[g].nunits++] = fuunit;
		} else {
			for (fuhour = 0; fuhour <= 23; fuhour++)
				if (inregion((units[fuunit].sched[7][fuhour] - units[fuunit].sched[5][fuhour]) / 2 + units[fuunit].sched[5][fuhour],
				    (units[fuunit].sched[8][fuhour] - units[fuunit].sched[6][fuhour]) / 2 + units[fuunit].sched[6][fuhour], g)) {
					regions[g].units[regions[g].nunits++] = fuunit;
					break;
				}
		}
	}
}

void
setarea(Unit *u, short j)
{
	if (staymode(u->sched[0][j])) {
		u->sched[5][j] = u->sched[7][j] = u->sched[3][j];
		u->sched[6][j] = u->sched[8][j] = u->sched[4][j];
		return;
	}
	if (isanimal(u) && u->leader)
		return;
	areareg = regionat(u->sched[3][j], u->sched[4][j]);
	if (areareg) {
		u->sched[5][j] = regions[areareg].x0;
		u->sched[6][j] = regions[areareg].y0;
		u->sched[7][j] = regions[areareg].x1;
		u->sched[8][j] = regions[areareg].y1;
	} else {
		// FIXED: the bytes of the area wrap for a post within 15 cells of the edge, so the area empties. Keep it on the map.
#if defined(FEATURE_FIXES)
		u->sched[5][j] = max(u->sched[3][j] - 15, 0);
		u->sched[6][j] = max(u->sched[4][j] - 15, 0);
		u->sched[7][j] = min(u->sched[3][j] + 15, 149);
		u->sched[8][j] = min(u->sched[4][j] + 15, 149);
#else
		u->sched[5][j] = u->sched[3][j] - 15;
		u->sched[6][j] = u->sched[4][j] - 15;
		u->sched[7][j] = u->sched[3][j] + 15;
		u->sched[8][j] = u->sched[4][j] + 15;
#endif
	}
}

void
setareas(Unit *u)
{
	for (areahour = 0; areahour < 24; areahour++)
		setarea(u, areahour);
}

void
cleartrail(Unit *u)
{
	if (badunit(u))
		return;
	u->trailx0 = u->x;
	u->trailx1 = u->x;
	u->trailx2 = u->x;
	u->trailx3 = u->x;
	u->traily0 = u->y;
	u->traily1 = u->y;
	u->traily2 = u->y;
	u->traily3 = u->y;
}

short
unitdist(Unit *u, Unit *v)
{
	if (badunit(u))
		return 150;
	if (badunit(v))
		return 150;
	udistx = abs(v->x - u->x);
	udisty = abs(v->y - u->y);
	if (udisty >= udistx)
		return udisty;
	return udistx;
}

short
stepdist(Unit *u, Unit *v)
{
	if (badunit(u) || badunit(v))
		return 150;
	if (u->pose == 1) {
		stepdx = u->x + u->dirx;
		stepdy = u->y + u->diry;
	} else {
		stepdx = u->x;
		stepdy = u->y;
	}
	if (v->pose == 1) {
		stepdx = abs(v->x + v->dirx - stepdx);
		stepdy = abs(v->y + v->diry - stepdy);
	} else {
		stepdx = abs(v->x - stepdx);
		stepdy = abs(v->y - stepdy);
	}
	if (stepdx > stepdy)
		return stepdx;
	return stepdy;
}

short
celldist(int x0, int y0, int x1, int y1)
{
	x0 = abs(x1 - x0);
	y0 = abs(y1 - y0);
	if (x0 == 0) {
		if (y0 > 0)
			y0--;
		return y0;
	}
	if (y0 == 0) {
		if (x0 > 0)
			x0--;
		return x0;
	}
	y1 = x0 * x0 + y0 * y0;
	if (y1 < 46225)
		return sqrttab[y1];
	if (x0 > y0)
		return x0;
	return y0;
}

int
postdist(Unit *u, Unit *v)
{
	if (badunit(u) || badunit(v))
		return 150;
	if (scheduled(u)) {
		pdux = u->sched[3][gamehour];
		pduy = u->sched[4][gamehour];
	} else {
		pdux = u->x;
		pduy = u->y;
	}
	if (scheduled(v)) {
		pdvx = v->sched[3][gamehour];
		pdvy = v->sched[4][gamehour];
	} else {
		pdvx = v->x;
		pdvy = v->y;
	}
	return celldist(pdux, pduy, pdvx, pdvy);
}

short
regionexit(short x, short y, short g)
{
	rebest = 300;
	reexit = -1;
	for (rei = 0; rei < regions[g].nexits; rei++) {
		redist = celldist(x, y, regions[g].exitx[rei], regions[g].exity[rei]);
		if (redist < rebest) {
			reexit = rei;
			rebest = redist;
		}
	}
	return reexit;
}

short
provexit(short x, short y, short k)
{
	pebest = 300;
	peexit = -1;
	for (pei = 0; pei < provs[k].n; pei++) {
		pedist = celldist(x, y, provs[k].a[pei], provs[k].b[pei]);
		if (pedist < pebest) {
			peexit = pei;
			pebest = pedist;
		}
	}
	return peexit;
}

short
freeplace(short g)
{
	if (regions[g].nplaces == 0)
		return -1;
	memset(fpfree, 0, 8);
	fpnfree = 0;
	if (regions[g].nplaces > 8)
		regions[g].nplaces = 8;
	for (fpi = 0; fpi < regions[g].nplaces; fpi++) {
		fpcell = regions[g].placey[fpi] * 150 + regions[g].placex[fpi];
		if (playercell == fpcell)
			continue;
		if (!(map[fpcell] & 0x4000))
			continue;
		if (mapflags[fpcell] & 4)
			continue;
		if (mapflags[fpcell] & 0x100)
			continue;
		if (mapflags[fpcell] & 0x400)
			continue;
		fpfree[fpnfree++] = fpi;
	}
	if (fpnfree == 0)
		return -1;
	if (fpnfree == 1)
		return fpfree[0];
	memset(fpopen, 0, 8);
	fpnopen = 0;
	for (fpi = 0; fpi < fpnfree; fpi++) {
		fpx = regions[g].placex[fpfree[fpi]];
		fpy = regions[g].placey[fpfree[fpi]];
		if (fpx == playerx && fpy == playery)
			continue;
		for (fpunit = 1; fpunit < nunits; fpunit++) {
			if (units[fpunit].nextx == fpx && units[fpunit].nexty == fpy)
				goto next;
			if (units[fpunit].x == fpx && units[fpunit].y == fpy)
				goto next;
		}
		fpopen[fpnopen++] = fpfree[fpi];
	next:;
	}
	if (fpnopen == 1)
		return fpopen[0];
	if (fpnopen == 0)
		return fpfree[randn(fpnfree)];
	return fpopen[randn(fpnopen)];
}

int
fightnear(Unit *u, short *d)
{
	for (fgunit = 1; fgunit < nunits; fgunit++)
		if ((units[fgunit].mode == 0x14 || units[fgunit].mode == 0x16)
		    && (*d = celldist(u->x, u->y, units[fgunit].x, units[fgunit].y)) < 8)
			return fgunit;
	return 0;
}

short
route(Unit *u, short x, short y)
{
	if (badunit(u))
		return 0;
	u->goalx = x;
	u->goaly = y;
	rtfrom = provat(u->x, u->y);
	rtto = provat(x, y);
	if (rtto != rtfrom) {
		if (rtto) {
			rtexit = provexit(u->x, u->y, rtto);
			if (rtexit == -1)
				return 0;
			x = provs[rtto].a[rtexit];
			y = provs[rtto].b[rtexit];
		}
		if (rtfrom) {
			rtexit = provexit(x, y, rtfrom);
			if (rtexit == -1)
				return 0;
			u->nextx = provs[rtfrom].a[rtexit];
			u->nexty = provs[rtfrom].b[rtexit];
			if (u->x != u->nextx || u->y != u->nexty) {
				x = u->nextx;
				y = u->nexty;
			} else
				rtfrom = 0;
		}
	}
	rtfrom = regionat(u->x, u->y);
	rtto = regionat(x, y);
	if (!rtfrom || rtto != rtfrom) {
		if (rtto) {
			rtexit = regionexit(u->x, u->y, rtto);
			if (rtexit == -1)
				return 0;
			x = regions[rtto].exitx[rtexit];
			y = regions[rtto].exity[rtexit];
		}
		if (rtfrom) {
			rtexit = regionexit(x, y, rtfrom);
			if (rtexit == -1)
				return 0;
			u->nextx = regions[rtfrom].exitx[rtexit];
			u->nexty = regions[rtfrom].exity[rtexit];
			if (u->nextx != u->x || u->y != u->nexty)
				return 1;
			rtfrom = 0;
		}
	}
	u->nextx = x;
	u->nexty = y;
	return 1;
}

void
addfoe(unsigned char c)
{
	for (afi = 0; afi < 30; afi++) {
		if (c == game.foes[afi])
			return;
		if (game.foes[afi] == 0) {
			game.foes[afi] = c;
			return;
		}
	}
	for (afi = 0; afi < 29; afi++)
		game.foes[afi] = game.foes[afi + 1];
	game.foes[afi] = c;
}

short
foedist(void)
{
	fdmin = 150;
	for (fdi = 0; fdi < 30; fdi++) {
		fdunit = game.foes[fdi];
		if (fdunit == 0)
			return fdmin;
		if ((units[fdunit].mode == 0x14 || units[fdunit].mode == 0x15) && units[fdunit].modeb == 0) {
			fddist = unitdist(&units[0], &units[fdunit]);
			if (fddist < fdmin)
				fdmin = fddist;
		}
	}
	if (fdmin <= 17)
		return fdmin;
	for (fdi = 0; fdi < 30; fdi++) {
		fdunit = game.foes[fdi];
		if (fdunit == 0)
			return fdmin;
		if (isanimal(&units[fdunit]) && units[fdunit].mode == 0x19 && units[fdunit].modeb == 0) {
			fddist = unitdist(&units[0], &units[fdunit]);
			if (fddist < fdmin)
				fdmin = fddist;
		}
	}
	return fdmin;
}

int
inrange(Unit *u, int nearby)
{
	if (nearby) {
		if (unitdist(&units[0], u) <= 13)
			return 1;
		return 0;
	}
	return cansee(u->x, u->y);
}

short
nearfoe(int nearby)
{
	nffoe = 0;
	for (nfunit = 1; nfunit < nunits; nfunit++) {
		if (units[nfunit].mode == 0x14 || units[nfunit].mode == 0x15) {
			if (units[nfunit].modeb == 0 || units[nfunit].modec == 0
			    || units[nfunit].bewitched && units[nfunit].bewitcher == 0
			    || units[nfunit].sympathy <= 0 && units[nfunit].fleeing) {
				if (inrange(&units[nfunit], nearby)) {
					if (cansee(units[nfunit].x, units[nfunit].y))
						addfoe(nfunit);
					if (nffoe == 0)
						nffoe = nfunit;
				}
			}
			if (talkactive && units[nfunit].modeb == curunit
			    && cansee(units[nfunit].x, units[nfunit].y) && nffoe == 0)
				nffoe = nfunit;
		}
	}
	if (nffoe == 0) {
		for (nfunit = 1; nfunit < nunits; nfunit++) {
			if ((units[nfunit].mode == 0x14 || units[nfunit].mode == 0x15)
			    && (units[nfunit].modeb != 0xff && units[units[nfunit].modeb].clan == units[0].clan
			    || units[nfunit].modec != 0xff && units[units[nfunit].modec].clan == units[0].clan)
			    && cansee(units[nfunit].x, units[nfunit].y) && nffoe == 0)
				nffoe = nfunit;
			if ((units[nfunit].sympathy <= 0 || iswild(&units[nfunit]))
			    && units[nfunit].mode != 0x19 && !mustflee(&units[nfunit])
			    && inrange(&units[nfunit], nearby) && nffoe == 0)
				nffoe = nfunit;
		}
	}
	if (nffoe) {
		nofoes = 0;
		return nffoe;
	}
	nofoes = 1;
	return 0;
}

short
underattack(short n)
{
	for (uaunit = 1; uaunit < nunits; uaunit++)
		if ((units[uaunit].mode == 0x14 || units[uaunit].mode == 0x15)
		    && units[uaunit].modeb == n && unitdist(&units[n], &units[uaunit]) < 8)
			return 1;
	return 0;
}

short
sparrer(void)
{
	for (sprunit = 1; sprunit < nunits; sprunit++)
		if (units[sprunit].mode == 0x16 && units[sprunit].modeb == 0)
			return sprunit;
	return 0;
}

int
stationary(Unit *u)
{
	if (badunit(u))
		return 0;
	switch (u->mode) {
	case 0:
	case 2:
	case 4:
	case 0x32:
		return 1;
	default:
		return 0;
	}
}

int
sitting(Unit *u)
{
	if (badunit(u))
		return 0;
	if (u->pose == 10 || u->pose >= 11 && u->pose <= 18)
		return 1;
	return 0;
}

void
setfacing(Unit *u)
{
	if (badunit(u))
		return;
	if (u->diry < 0)
		u->modea = 0;
	else if (u->diry > 0)
		u->modea = 2;
	else if (u->dirx < 0)
		u->modea = 1;
	else
		u->modea = 3;
}

void
setstep(Unit *u)
{
	if (badunit(u))
		return;
	if (mapflags[u->x + u->y * 150] & 0x40)
		return;
	ssdx0 = u->dirx;
	ssdy0 = u->diry;
	if (u->modea == 0) {
		u->dirx = 0;
		u->diry = -1;
	} else if (u->modea == 1) {
		u->dirx = -1;
		u->diry = 0;
	} else if (u->modea == 2) {
		u->dirx = 0;
		u->diry = 1;
	} else {
		u->dirx = 1;
		u->diry = 0;
	}
	if (u->dirx != ssdx0 || u->diry != ssdy0)
		u->idlestep = 0;
}

void
playerstep(int x, int y)
{
	signed char dy, dx;

	if (units[0].pose == 0 || units[0].pose > 9 || units[0].pose == 2) {
		if (units[0].pose == 10)
			return;
		dx = x - playerx;
		if (dx < 0)
			dx = -1;
		else if (dx > 0)
			dx = 1;
		dy = y - playery;
		if (dy < 0)
			dy = -1;
		else if (dy > 0)
			dy = 1;
		if (dx != units[0].dirx || dy != units[0].diry) {
			units[0].dirx = dx;
			units[0].diry = dy;
			units[0].pose = 0;
			units[0].idlestep = 0;
		}
	}
}

void
stepto(Unit *u, short x, short y)
{
	if (badunit(u))
		return;
	stdx0 = u->dirx;
	stdy0 = u->diry;
	if (u == &units[0]) {
		playerstep(x, y);
		return;
	}
	if (u->pose != 0)
		return;
	stdx = x - u->x;
	stdy = y - u->y;
	if (stdx == 1 && stdy == 1) {
		u->dirx = 1;
		u->diry = 0;
	} else if (stdx == -1 && stdy == -1) {
		u->dirx = -1;
		u->diry = 0;
	} else if (abs(stdy) >= abs(stdx)) {
		u->dirx = 0;
		u->diry = stdy < 0 ? -1 : 1;
	} else {
		u->dirx = stdx < 0 ? -1 : 1;
		u->diry = 0;
	}
	if (u->dirx != stdx0 || u->diry != stdy0)
		u->idlestep = 0;
}

void
turnto(Unit *u, short x, short y)
{
	if (badunit(u))
		return;
	if (u->pose != 0)
		return;
	stepto(u, x, y);
	switch (u->mode) {
	case 0:
	case 2:
	case 4:
	case 0x32:
		setfacing(u);
		break;
	}
}

int
sharecell(Unit *u)
{
	if (badunit(u))
		return 0;
	scself = indexof(u);
	u->cellmate = 0xff;
	if (u->incoming != 0)
		u->incoming = 0xff;
	for (scunit = 0; scunit < nunits; scunit++) {
		if (scunit == scself)
			continue;
		if (u->cellmate == 0xff && units[scunit].x == u->x && units[scunit].y == u->y
		    && units[scunit].pose != 1)
			u->cellmate = scunit;
		if (u->incoming == 0xff && units[scunit].mode == 8 && units[scunit].nextx == u->x
		    && units[scunit].nexty == u->y)
			u->incoming = scunit;
	}
	if (u->cellmate == 0xff)
		return 0;
	return 1;
}

void
sharecells(void)
{
	for (scsunit = 0; scsunit < nunits; scsunit++)
		sharecell(&units[scsunit]);
}

void
setunitmode(Unit *u, unsigned char mode)
{
	if (badunit(u))
		return;
	smold = u->mode;
	if (mode == smold)
		return;
	badunit(u);
	u->modee = 0;
	u->modef = 0;
	u->modeg = 0;
	u->mode = mode;
	if (mode == 0x14 || mode == 0x15) {
		smarmor = isarmor(u->body);
		smmoved = 0;
		if (!u->hand && !u->belt) {
			for (smslot = 0; smslot < 3; smslot++)
				if (isweapon(u->items[smslot])) {
					u->hand = u->items[smslot];
					u->items[smslot] = 0;
					smmoved = 1;
					// FIXED: each item of the kind in the pack takes the one slot in turn, and all but the last are lost. Take the first and leave the others.
#if defined(FEATURE_FIXES)
					break;
#endif
				}
		}
		if (u->offhand == 0) {
			for (smslot = 0; smslot < 3; smslot++)
				if (classof(u->items[smslot] & 0xfff) == 0x55) {
					u->offhand = u->items[smslot];
					smmoved = 1;
					u->items[smslot] = 0;
					// FIXED: as for the weapon, take the first and leave the others.
#if defined(FEATURE_FIXES)
					break;
#endif
				}
		}
		if (u->head == 0) {
			for (smslot = 0; smslot < 3; smslot++)
				if (classof(u->items[smslot] & 0xfff) == 0x41) {
					u->head = u->items[smslot];
					smmoved = 1;
					u->items[smslot] = 0;
					// FIXED: as for the weapon, take the first and leave the others.
#if defined(FEATURE_FIXES)
					break;
#endif
				}
		}
		if (smarmor == 0) {
			for (smslot = 0; smslot < 3; smslot++)
				if (isarmor(u->items[smslot])) {
					// FIXED: as for the weapon, take the first and leave the others. The armor also takes the slot of a doublet or a dress, which isarmor passes over, and that is lost. Leave it in the pack slot that the armor frees.
#if defined(FEATURE_FIXES)
					unsigned long worn = u->body;

					u->body = u->items[smslot];
					smmoved = 1;
					u->items[smslot] = worn;
					break;
#else
					u->body = u->items[smslot];
					smmoved = 1;
					u->items[smslot] = 0;
#endif
				}
		}
		if (smmoved)
			calcstats(u);
		if (!isservant(u) && !isfemale(u) && (u->mode == 0x15 || u->mode == 0x14 && smold != 0x15)) {
			if (!(classof(u->offhand & 0xfff) == 0x55)) {
				fetchitem(u, 0xf301a8, 1);
				if (classof(u->offhand & 0xfff) != 0x55)
					fetchitem(u, 0xf201a7, 1);
			}
			if (!u->head || !smarmor) {
				if (ownsland(u)) {
					if (!smarmor)
						fetchitem(u, 0xef01b0, 1);
					if (u->head == 0)
						fetchitem(u, 0xe901bd, 1);
				} else {
					smowner = landowner(u);
					if (smowner) {
						if (!smarmor && isarmor(units[smowner].body))
							fetchitem(u, 0xef01b0, 1);
						if (u->head == 0 && units[smowner].head)
							fetchitem(u, 0xe901bd, 1);
					}
				}
			}
		}
	}
	switch (u->mode) {
	case 1:
	one:
		u->mode = 1;
		u->modea = 0;
		u->modeb = 0;
		u->modec = 0;
		u->moded = 0;
		break;
	case 0x14:
	case 0x15:
	case 0x16:
		u->modea = 0;
		u->modeb = 0xff;
		u->modec = 0xff;
		cleartrail(u);
		if (u->pose == 0 || u->pose > 9)
			u->pose = 0;
		if (smold == 0x14 || smold == 0x15 || smold == 0x16) {
			checkposes(u);
			break;
		}
		u->idlestep = 0;
		break;
	case 0x19:
		if (smold == 0x14 || smold == 0x15 || smold == 0x19) {
			if (u->modeb == 0)
				u->traits |= 0x20;
		} else {
			u->modeb = 0xff;
			u->modec = 0xff;
		}
		u->modea = 0;
		if (u->modeb == 0xff) {
			u->modeb = u->modec;
			u->modec = 0xff;
		}
		cleartrail(u);
		if (u->sympathy <= 0 || u->modeb == 0 || u->modec == 0) {
			if (u->modeb != 0 && u->modec == 0xff)
				u->modec = 0;
			abdicate(u, 0);
		}
		if (u->modeb == 0xff) {
			u->modeb = u->modec;
			u->modec = 0xff;
		}
		if (isrobber(u))
			u->modee = 100;
		else
			u->modee = 50;
		u->modef = 1;
		break;
	case 3:
		u->mode = 3;
		u->modea = 0;
		u->modeb = 0;
		u->modec = 0;
		u->moded = 0;
		break;
	case 8:
		if (scheduled(u)) {
			cleartrail(u);
			u->modea = 0;
			u->modeb = 0;
			u->modec = 0;
			u->moded = 0;
			if (route(u, u->sched[3][gamehour], u->sched[4][gamehour])) {
				cleartrail(u);
				break;
			}
		}
		goto one;
	case 0:
	case 2:
	case 4:
	case 0x32:
		u->pose = 0;
		setstep(u);
		u->modec = 0;
		break;
	case 0x5a:
		u->pose = 0;
		cleartrail(u);
		u->modea = 0;
		u->modec = 1;
		break;
	case 0xf:
		cleartrail(u);
		u->modea = 0;
		u->modeb = 0;
		u->modee = 0;
		u->modef = 0;
		u->modec = 0xff;
		u->moded = 0xff;
		break;
	}
}

void
duel(Unit *u, int right)
{
	setunitmode(u, 0x14);
	if (right) {
		u->modea = 2;
		u->talkflags |= 4;
	} else {
		u->modea = 1;
		u->talkflags |= 2;
	}
	u->modeb = 0;
	u->sympathy = 0;
}

// FIXED: as in placenear
#ifdef FEATURE_FAULTS
extern unsigned short itemflags[4096];
extern unsigned char classes[4096];

/* terrainbyte returns byte i of terrains as the original reads it: past its 4,096, those of itemflags, weights, and classes, which WLINK places after it */
static unsigned char
terrainbyte(int i)
{
	if (i < 4096)
		return terrains[i];
	i -= 4096;
	if (i < 2 * 4096)
		return itemflags[i / 2] >> (i % 2 * 8);
	i -= 2 * 4096;
	if (i < 2 * 4096)
		return weights[i / 2] >> (i % 2 * 8);
	return classes[i - 2 * 4096];
}
#endif

void
placenear(Unit *u, short x, short y, int r)
{
	if (badunit(u))
		return;
	pnprov = provat(x, y);
	// FIXED: terrains holds the terrain of each of the 4,096 tiles, and the original indexes it by the cell, so the test of a field reads the terrain of tile x + y * 150, and past terrains other tables. Index it by the cell's tile, as the code does elsewhere.
	// FIXED: from cell 4,096 on, the original reads past terrains the bytes of itemflags, weights, and classes, which WLINK places after it. Read them there, through terrainbyte.
#if defined(FEATURE_FIXES)
	pnterr = terrains[map[x + y * 150] & 0xfff];
#elif defined(FEATURE_FAULTS)
	pnterr = terrainbyte(x + y * 150);
#else
	pnterr = terrains[x + y * 150];
#endif
	pnx0 = x - r;
	if (pnx0 < 5)
		pnx0 = 5;
	pny0 = y - r;
	if (pny0 < 5)
		pny0 = 5;
	pnw = x + r;
	if (pnw > 145)
		pnw = 145;
	pnh = y + r;
	if (pnh > 145)
		pnh = 145;
	pnw -= pnx0;
	pnh -= pny0;
	for (;;) {
		x = pnx0 + randn(pnw);
		y = pny0 + randn(pnh);
		if (!(map[y * 150 + x] & 0x4000))
			continue;
		if (map[y * 150 + x] & 0x2000)
			continue;
		if (unitat(x, y) != 0xff)
			continue;
		if (provat(x, y) != pnprov)
			continue;
		// FIXED: terrains by the cell's tile, as above
		// FIXED: the bytes past terrains through terrainbyte, as above
#if defined(FEATURE_FIXES)
		if (pnterr != 4 && terrains[map[y * 150 + x] & 0xfff] == 4)
#elif defined(FEATURE_FAULTS)
		if (pnterr != 4 && terrainbyte(y * 150 + x) == 4)
#else
		if (pnterr != 4 && terrains[y * 150 + x] == 4)
#endif
			continue;
		if (map[y * 150 + x - 1] & 0x4000)
			break;
		if (map[y * 150 + x + 1] & 0x4000)
			break;
		if (map[(y - 1) * 150 + x] & 0x4000)
			break;
		if (map[(y + 1) * 150 + x] & 0x4000)
			break;
	}
	u->pose = 0;
	u->x = x;
	u->y = y;
}

void
placeunit(Unit *u, short h)
{
	if (badunit(u))
		return;
	if (!scheduled(u))
		return;
	plx0 = u->x;
	ply0 = u->y;
	if (u->leader) {
		if (h >= 6 && h < 20) {
			placenear(u, units[u->leader].sched[3][h], units[u->leader].sched[4][h], 7);
			return;
		}
		if (h == 20)
			h = 22;
	}
	sharecell(u);
	if (u->cellmate == 0xff)
		vacate(u);
	u->x = u->sched[3][h];
	u->y = u->sched[4][h];
	u->pose = 0;
	occupy(u);
	if (u->x != playerx || u->y != playery)
		return;
	if (u->x == plx0 && u->y == ply0 && isfemale(u) && u->sympathy >= 4 && u->mode == 0x5a)
		return;
	if (mapflags[u->x + u->y * 150] & 0x40) {
		plreg = regionat(u->x, u->y);
		if (plreg) {
			plplace = freeplace(plreg);
			if (plplace != -1) {
				u->x = regions[plreg].placex[plplace];
				u->y = regions[plreg].placey[plplace];
				return;
			}
		}
	}
	plplace = 0;
	do {
		stepaside(u);
		if (u == &units[0])
			return;
		if (u->x != playerx || u->y != playery)
			return;
	} while (plplace++ < 50);
}

void
applyhour(Unit *u, short h)
{
	if (badunit(u))
		return;
	if (!scheduled(u))
		return;
	setunitmode(u, u->sched[0][h]);
	u->modea = u->sched[1][h];
	u->modeb = u->sched[2][h];
	if (stationary(u))
		setstep(u);
}

short
isbusy(Unit *u)
{
	if (badunit(u) || u == &units[0] || u->health <= 0 || u->asleep || u->fleeing || u->bewitched)
		return 1;
	if (!scheduled(u) || engaged(u))
		return 1;
	if (u->mode == 8)
		return 1;
	if (u->mode == 0xf) {
		if (u->modee != 0xff)
			return 1;
		if (u->modec == 0xff || u->moded == 0xff)
			return 0;
		busycell = u->modec + u->moded * 150;
		if (mapflags[busycell] & 0x100)
			return 0;
		if (findclass(busycell, 0x59))
			return 1;
		return 0;
	}
	if (u->mode == 0x16)
		return 1;
	return 0;
}

short
postblocked(Unit *u, short h)
{
	if (badunit(u))
		return 0;
	pbdx = u->sched[3][h];
	pbdy = u->sched[4][h];
	if (u->sched[3][h] != playerx || pbdy != playery)
		return 0;
	pbdx = abs(pbdx - u->x);
	pbdy = abs(pbdy - u->y);
	if (pbdx > 1 || pbdy > 1)
		return 0;
	return 1;
}

int
seatfree(int cell, short skip, unsigned char x, unsigned char y, short h)
{
	if (!(map[cell] & 0x4000))
		return 0;
	if (!(mapflags[cell] & 0x40))
		return 0;
	for (sfunit = 0; sfunit < nunits; sfunit++) {
		if (sfunit == skip)
			continue;
		if (x == units[sfunit].x && y == units[sfunit].y)
			return 0;
		if (x == units[sfunit].sched[3][h] && y == units[sfunit].sched[4][h])
			return 0;
	}
	return 1;
}

int
otherseat(Unit *u, short h)
{
	if (badunit(u))
		return 0;
	oscell = u->sched[4][h];
	oscell *= 150;
	oscell += u->sched[3][h];
	if (!(mapflags[oscell] & 0x40))
		return 0;
	osself = indexof(u);
	osx0 = u->nextx;
	osy0 = u->nexty;
	osreg = regionat(u->x, u->y);
	if (osreg) {
		osplace = freeplace(osreg);
		if (osplace != -1) {
			u->nextx = regions[osreg].placex[osplace];
			u->nexty = regions[osreg].placey[osplace];
			goto found;
		}
	}
	u->nextx = u->sched[3][h] - 1;
	u->nexty = u->sched[4][h];
	if (seatfree(oscell - 1, osself, u->nextx, u->nexty, h))
		goto found;
	u->nextx = u->sched[3][h] + 1;
	u->nexty = u->sched[4][h];
	if (seatfree(oscell + 1, osself, u->nextx, u->nexty, h))
		goto found;
	u->nextx = u->sched[3][h];
	u->nexty = u->sched[4][h] - 1;
	if (seatfree(oscell - 150, osself, u->nextx, u->nexty, h))
		goto found;
	u->nextx = u->sched[3][h];
	u->nexty = u->sched[4][h] + 1;
	if (seatfree(oscell + 150, osself, u->nextx, u->nexty, h))
		goto found;
	u->nextx = osx0;
	u->nexty = osy0;
	return 0;
found:
	if (u->x == u->nextx && u->y == u->nexty)
		return 0;
	if (u->mode == 8) {
		u->modea = 1;
		u->modeb = u->nextx;
		u->modec = u->nexty;
	}
	return 1;
}

short
atpost(Unit *u, short h)
{
	if (badunit(u))
		return 1;
	if (!scheduled(u))
		return 1;
	if (provat(u->x, u->y) != provat(u->sched[3][h], u->sched[4][h]))
		return 0;
	if (u->mode == 3) {
		if (scheduled(u) && !regionat(u->sched[3][h], u->sched[4][h])) {
			if (celldist(u->x, u->y, u->sched[3][h], u->sched[4][h]) > 4)
				return 0;
		} else if (u->modee == 0x11 || u->modee == 0x12) {
			apreg = regionat(u->x, u->y);
			appost = regionat(u->sched[3][h], u->sched[4][h]);
			if (apreg && !appost)
				;
			else if (apreg && apreg != appost)
				;
			else if (appost && apreg == appost)
				return 1;
			else if (!appost) {
				if (u->x < u->sched[5][h] || u->x > u->sched[7][h] || u->y < u->sched[6][h] || u->y > u->sched[8][h])
					return 0;
			} else if (!apreg && appost) {
				if (ownerat(u->sched[3][h], u->sched[4][h]) == ownerat(u->x, u->y))
					return 1;
			}
		}
	}
	if (u->x >= u->sched[5][h] && u->x <= u->sched[7][h] && u->y >= u->sched[6][h] && u->y <= u->sched[8][h])
		return 1;
	if (u->mode == u->sched[0][h] && abs(u->x - u->sched[3][h]) <= 15 && abs(u->y - u->sched[4][h]) <= 15
	    && regionat(u->x, u->y) == regionat(u->sched[3][h], u->sched[4][h])) {
		if (!(mapflags[u->sched[3][h] + u->sched[4][h] * 150] & 0x40))
			return 1;
		if (mapflags[u->x + u->y * 150] & 0x40)
			return 1;
		return 0;
	}
	if (u->mode == 8 && u->modea)
		return 0;
	if (postblocked(u, h) && !otherseat(u, h))
		return 1;
	return 0;
}

int
ontask(Unit *u, short h)
{
	if (badunit(u))
		return 1;
	if (!scheduled(u))
		return 1;
	if (u->mode == u->sched[0][h] && u->modea == u->sched[1][h] && u->modeb == u->sched[2][h])
		return 1;
	return 0;
}

void
placeunits(short h)
{
	markunits();
	for (plsunit = 1; plsunit < nunits; plsunit++) {
		if (scheduled(&units[plsunit])) {
			placeunit(&units[plsunit], h);
			applyhour(&units[plsunit], h);
		}
	}
	markunits();
	sharecells();
}

void
gopost(Unit *u)
{
	if (badunit(u))
		return;
	if (!scheduled(u))
		return;
	setunitmode(u, 8);
}

short
residentnear(short x, short y)
{
	rnreg = regionat(x, y);
	for (rnrad = 3; rnrad <= 15; rnrad += 2) {
		for (rnunit = 1; rnunit < nunits; rnunit++) {
			if (units[rnunit].health > 0 && !isanimal(&units[rnunit])) {
				if (!scheduled(&units[rnunit])) {
					// FIXED: a unit without a schedule counts within 15 cells on the first round, ahead of a nearer resident. Widen the square with rnrad, as for residents.
#if defined(FEATURE_FIXES)
					if (units[rnunit].x >= x - rnrad && units[rnunit].x <= x + rnrad
					    && units[rnunit].y >= y - rnrad && units[rnunit].y <= y + rnrad) {
#else
					if (units[rnunit].x >= x - 15 && units[rnunit].x <= x + 15
					    && units[rnunit].y >= y - 15 && units[rnunit].y <= y + 15) {
#endif
						if (rnreg) {
							rnureg = regionat(units[rnunit].x, units[rnunit].y);
							if (rnureg != rnreg)
								continue;
						}
						return rnunit;
					}
				} else {
					for (rnhour = 0; rnhour <= 23; rnhour++) {
						if (units[rnunit].sched[5][rnhour] >= x - rnrad
						    && units[rnunit].sched[7][rnhour] <= x + rnrad
						    && units[rnunit].sched[6][rnhour] >= y - rnrad
						    && units[rnunit].sched[8][rnhour] <= y + rnrad) {
							if (rnreg) {
								rnureg = regionat(units[rnunit].sched[5][rnhour]
								    + (units[rnunit].sched[7][rnhour] - units[rnunit].sched[5][rnhour]) / 2,
								    units[rnunit].sched[6][rnhour]
								    + (units[rnunit].sched[8][rnhour] - units[rnunit].sched[6][rnhour]) / 2);
								if (rnureg != rnreg)
									continue;
							}
							return rnunit;
						}
					}
				}
			}
		}
	}
	return 0;
}

void
talkface(short n)
{
	char c;

	tfface = units[n].face;
	if (!tfface) {
		memset(curface, 0, 0x32a0);
		return;
	}
	loadface((unsigned char *)curface, (unsigned char *)curface2, tfface);
	if (units[n].health > 0)
		return;
	for (tfface = 0; tfface < 0x32a0; tfface++) {
		c = curface[tfface];
		if (c)
			curface[tfface] = lum[c] + 0x50;
	}
	memcpy(curface2, curface, 0x32a0);
}

void
starttalk(short n)
{
	curunit = n;
	clearmind(&units[0]);
	clearmind(&units[curunit]);
	markunits();
	cleartexts();
	units[0].pose = 0;
	playerx = units[0].x;
	playery = units[0].y;
	trackregion();
	turnto(&units[0], units[n].x, units[n].y);
}

void
endtalk(void)
{
	if (curunit) {
		cleartrail(&units[curunit]);
		if (units[curunit].health && units[curunit].health < 3)
			units[curunit].health = 3;
		if (units[curunit].mode == 0x14 || units[curunit].mode == 0x15 || units[curunit].mode == 0x19)
			units[curunit].bleeding = 0;
	}
	markunits();
}

unsigned long
valuefor(unsigned long item, Unit *u)
{
	vfval = valueof(item);
	if (badunit(u))
		return vfval;
	vfpct = 100;
	switch (classof(item & 0xfff)) {
	case 0x1e:
		vfpct = skillof(u, 0x18, 0, 0, 0);
		break;
	case 0x38:
	case 0x39:
		vfpct = skillof(u, 0x19, 0, 0, 0);
		vfpct += skillof(u, 0x18, 0, 0, 0) - 100;
		break;
	case 0x3c:
		if (isservant(u))
			vfpct = 0x1e;
		else
			vfpct = baseskill(u, metals[gearof(item)].skill);
		break;
	case 0x41:
		vfpct = armorpct(u);
		break;
	case 0x46:
		// FIXED: the test of armor takes the hero's worn garment for the item priced. Test the item, as isarmor's other callers split class 0x46.
#if defined(FEATURE_FIXES)
		if (isarmor(item)) {
#else
		if (isarmor(units[0].body)) {
#endif
			vfpct = armorpct(u);
			break;
		}
		/* fall through */
	case 0x48:
	case 0x4b:
		if (isfemale(u)) {
			if ((item & 0xfff) == 0x1b5)
				vfpct = 0x82;
			else
				vfpct = 0x78;
		} else if (u->type == 0x1f)
			vfpct = 0x78;
		break;
	case 0xca:
		if (isfemale(u))
			vfpct = 0x78;
		break;
	case 0x62:
	case 0x63:
	case 0x64:
		if (isfemale(u))
			vfpct = 0x7d;
		else if (u->type == 0x1f)
			vfpct = 0x6e;
		break;
	case 0xcb:
	case 0xd1:
		if (iscraftsman(u) || issmith(u))
			vfpct = 0x7d;
		else if (isfemale(u))
			vfpct = 0x50;
		break;
	case 0x55:
		vfpct = baseskill(u, 0xe);
		break;
	case 0x36:
	case 0x37:
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
		if (glutton(u))
			vfpct = 0xaa;
		break;
	}
	if (vfpct != 100) {
		if (vfpct < 5)
			vfpct = 5;
		else if (vfpct > 0xc3)
			vfpct = 0xc3;
		vfval = vfpct * vfval / 100;
	}
	return vfval;
}

void
outrage(short flag)
{
	for (orunit = 1; orunit < nunits; orunit++) {
		if (cansee(units[orunit].x, units[orunit].y) && units[orunit].sympathy < 4
		    && !units[orunit].asleep && !isanimal(&units[orunit])) {
			if (!flag) {
				if (!(units[orunit].traits & 0x20))
					provoke(&units[0], &units[orunit], 2, 3, 0xff);
			} else if (!isfemale(&units[orunit]) || units[orunit].sympathy <= 0) {
				units[orunit].traits &= 0x5f;
				provoke(&units[0], &units[orunit], 3, 5, 0);
			}
		}
	}
}

void
nomercy(Unit *v)
{
	for (nmunit = 1; nmunit < nunits; nmunit++) {
		if (units[nmunit].health <= 0)
			continue;
		if (isanimal(&units[nmunit]))
			continue;
		if (myservant(&units[nmunit]))
			continue;
		nmseen = cansee(units[nmunit].x, units[nmunit].y);
		nmfemale = isfemale(&units[nmunit]);
		if ((units[nmunit].mode == 0x14 || units[nmunit].mode == 0x15) && nmseen) {
			if (units[nmunit].modeb == 0 || units[nmunit].modec == 0 || units[nmunit].sympathy <= 0) {
clear:
				units[nmunit].traits &= 0xdf;
calm:
				if (!nmfemale) {
					units[nmunit].courage = 3;
					if (units[nmunit].plea == 0x26d7)
						units[nmunit].plea = 0;
				}
				if (units[nmunit].mercy == 0x26d8 || units[nmunit].mercy == 0x26db)
					units[nmunit].mercy = 0;
			}
			continue;
		}
		if (units[nmunit].mode == 0x19 && units[nmunit].modeb == 0 && !nmfemale && nmseen) {
			units[nmunit].mode = 0x15;
			units[nmunit].modee = 0;
			units[nmunit].modec = 0xff;
			goto clear;
		}
		if (!v || &units[nmunit] == v || !allied(&units[nmunit], v))
			continue;
		goto calm;
	}
}

short
ownerat(short x, short y)
{
	if (x < 0 || y < 0 || x >= 150 || y >= 150)
		return 0;
	for (oawide = 0; oawide <= 13; oawide++)
		for (oareg = 1; oareg < nregions; oareg++)
			if (x >= regions[oareg].x0 - oawide && x <= regions[oareg].x1 + oawide
			    && y >= regions[oareg].y0 - oawide && y <= regions[oareg].y1 + oawide)
				return regions[oareg].lord;
	return 0;
}

int
landowner(Unit *u)
{
	return ownerat(u->x, u->y);
}

int
ownsland(Unit *u)
{
	if (ownerat(u->x, u->y) == indexof(u))
		return 1;
	return 0;
}

short
nearregion(short x, short y)
{
	if (x < 0 || y < 0 || x >= 150 || y >= 150)
		return 0;
	for (nrwide = 0; nrwide <= 13; nrwide++)
		for (nrreg = 1; nrreg < nregions; nrreg++)
			if (x >= regions[nrreg].x0 - nrwide && x <= regions[nrreg].x1 + nrwide
			    && y >= regions[nrreg].y0 - nrwide && y <= regions[nrreg].y1 + nrwide)
				return nrreg;
	return 0;
}

short
postregion(Unit *u)
{
	if (badunit(u))
		return 0;
	if (!scheduled(u))
		return 0;
	return nearregion(u->sched[3][gamehour], u->sched[4][gamehour]);
}

void
owncell(short x, short y, short on)
{
	/* n moves wpp386's multiply in findheir to the original's
	   imul */
	int n;
	unsigned long item;

	ocidx = x + y * 150;
	if (!(map[ocidx] & 0x1000))
		return;
	ocidx = map[ocidx] >> 16;
	for (;;) {
		occls = classof(mapitems[ocidx] & 0xfff);
		mapitems[ocidx] &= ~0x1000;
		if (on) {
			item = mapitems[ocidx];
			if (itemweight(item) >= 65000) {
				switch (occls) {
				case 0x69:
					if (!typenames[mapitems[ocidx] & 0xfff])
						break;
				case 1:
				case 4:
				case 10:
				case 0x32:
				case 0x37:
				case 0x59:
				case 0x5a:
					goto mark;
				}
			} else if (occls == 0xf0)
				;
			else {
mark:
				if (occls == 0x37 && !provat(x, y))
					;
				else
					mapitems[ocidx] |= 0x1000;
			}
		}
		if (occls == 10) {
			ocrealm = idof(mapitems[ocidx]);
			if (on)
				realms[ocrealm].item |= 0x1000;
			else
				realms[ocrealm].item &= ~0x1000;
			for (ocslot = 0; ocslot < 50; ocslot++)
				if (realms[ocrealm].items[ocslot]) {
					if (on)
						realms[ocrealm].items[ocslot] |= 0x1000;
					else
						realms[ocrealm].items[ocslot] &= ~0x1000;
				}
		} else if (occls == 0x15)
			if (on == 0) {
				ocrealm = mapitems[ocidx] >> 16;
				setowned(mapitems[ocidx] >> 16, 0);
			}
		if (!(mapitems[ocidx] & 0x8000))
			break;
		ocidx++;
	}
}

void
disownall(void)
{
	for (daidx = 1; daidx < nmapitems; daidx++)
		mapitems[daidx] &= ~0x1000;
	for (darealm = 5; darealm < nrealms + 5; darealm++) {
		realms[darealm].item &= ~0x1000;
		for (daslot = 0; daslot < 50; daslot++)
			realms[darealm].items[daslot] &= ~0x1000;
	}
}

void
ownregion(short g, short on)
{
	for (orgy = regions[g].y0; orgy <= regions[g].y1; orgy++)
		for (orgx = regions[g].x0; orgx <= regions[g].x1; orgx++)
			owncell(orgx, orgy, on);
}

void
ownland(short g, short on)
{
	for (oly = regions[g].y0 - 13; oly <= regions[g].y1 + 13; oly++)
		for (olx = regions[g].x0 - 13; olx <= regions[g].x1 + 13; olx++)
			if (ownerat(olx, oly) == regions[g].lord)
				owncell(olx, oly, on);
}

short
findheir(Unit *u, short flag)
{
	if (badunit(u))
		return 0;
	if (flag) {
		fhfoe = foeof(u);
		fhfoe2 = foe2of(u);
		if (fhfoe == 0xff && fhfoe2 == 0xff)
			flag = 0;
	}
	fhself = indexof(u);
	for (fhunit = 1; fhunit < nunits; fhunit++) {
		if (fhunit == fhself)
			continue;
		fhcand = &units[fhunit];
		if (!samehouse(u, fhcand))
			continue;
		if (isservant(fhcand))
			continue;
		if (isfemale(fhcand))
			continue;
		if (fhcand->traits & 0x80)
			continue;
		if (noheir(curmap, fhunit, fhself))
			continue;
		if (!flag) {
			if (!(fhcand->home & 0x8000))
				return fhunit;
		} else if (hastarget(fhcand, fhfoe, fhfoe2))
			return fhunit;
	}
	for (fhunit = 1; fhunit < nunits; fhunit++) {
		if (fhunit == fhself)
			continue;
		fhcand = &units[fhunit];
		if (!samehouse(u, fhcand))
			continue;
		if (isservant(fhcand))
			continue;
		if (fhcand->traits & 0x80)
			continue;
		if (noheir(curmap, fhunit, fhself))
			continue;
		if (!flag) {
			if (!(fhcand->home & 0x8000))
				return fhunit;
		} else if (hastarget(fhcand, fhfoe, fhfoe2))
			return fhunit;
	}
	return 0;
}

short
heirof(Unit *u)
{
	if (badunit(u))
		return 0;
	hoheir = findheir(u, 1);
	if (hoheir)
		return hoheir;
	return findheir(u, 0);
}

short
bequeath(Unit *u, short n)
{
	if (badunit(u))
		return 0;
	bqself = indexof(u);
	if (n < 0 && -n < nunits && units[-n].health > 0 && !isanimal(&units[-n])) {
		bqheir = n;
		bqheir = -bqheir;
	} else {
		if (n < 0)
			n = 0;
		bqheir = findheir(u, 1);
		if (bqheir == 0)
			bqheir = findheir(u, 0);
		if (!n && bqheir && isfemale(&units[bqheir]))
			bqheir = 0;
	}
	if (!bqheir)
		return 0;
	for (bqi = 1; bqi < nregions; bqi++)
		if (regions[bqi].lord == bqself)
			regions[bqi].lord = bqheir;
	for (bqi = 1; bqi < nunits; bqi++)
		if (units[bqi].health > 0 && units[bqi].lord == bqself) {
			units[bqi].lord = bqheir;
			flagunit(&units[bqi]);
		}
	if (u->health <= 0) {
		bqi = ownerat(u->x, u->y);
		if (bqi == bqheir || bqi && allied(&units[bqi], &units[bqheir])) {
			if (u->mode == 0x14 && u->modea && (u->modeb == 0 || u->modec == 0))
				setowned(indexof(u), 0);
			else
				setowned(indexof(u), 1);
		} else
			setowned(indexof(u), 0);
	}
	return bqheir;
}

short
abdicate(Unit *u, short v)
{
	if (badunit(u))
		return 0;
	abself = indexof(u);
	u->home |= 0x8000;
	u->traits |= 0x80;
	if (v < 0) {
		abheir = bequeath(u, v);
		if (abheir)
			return abheir;
	}
	abheir = bequeath(u, 0);
	if (abheir)
		return abheir;
	if (v) {
		abheir = bequeath(u, 1);
		if (abheir)
			return abheir;
	}
	for (abi = 1; abi < nunits; abi++) {
		if (abi == abself)
			continue;
		if (units[abi].health <= 0)
			continue;
		if (!samehouse(u, &units[abi]))
			continue;
		makepeace(&units[abi], &units[0]);
		units[abi].traits |= 0xa0;
		units[abi].home |= 0x8000;
		if (isservant(u))
			units[abi].sympathy = 2;
		else if (units[abi].sympathy != 4) {
			if (units[abi].sympathy == 3)
				units[abi].sympathy = 2;
			else
				units[abi].sympathy = 1;
		}
	}
	for (abi = 1; abi < nregions; abi++)
		if (regions[abi].lord && ownhome(&units[regions[abi].lord]) == 0) {
			regions[abi].lord = 0;
			ownland(abi, 0);
		}
	for (abi = 1; abi < nunits; abi++) {
		if (abi == abself)
			continue;
		if (units[abi].health <= 0)
			continue;
		abmaster = units[abi].lord;
		if (!abmaster)
			continue;
		if (ownhome(&units[abmaster]) != 0)
			continue;
		if (!(units[abmaster].traits & 0x80))
			continue;
		units[abi].clan = 0xff;
		units[abi].lord = 0;
		units[abi].traits &= 0xdf;
		makepeace(&units[abi], &units[0]);
		units[abi].sympathy = 2;
		flagunit(&units[abi]);
	}
	return 0;
}

void
copyhour(Unit *u, unsigned char dst, unsigned char src)
{
	u->sched[0][dst] = u->sched[0][src];
	u->sched[1][dst] = u->sched[1][src];
	u->sched[2][dst] = u->sched[2][src];
	u->sched[3][dst] = u->sched[3][src];
	u->sched[4][dst] = u->sched[4][src];
	u->sched[5][dst] = u->sched[5][src];
	u->sched[6][dst] = u->sched[6][src];
	u->sched[7][dst] = u->sched[7][src];
	u->sched[8][dst] = u->sched[8][src];
}

short
neartools(short x, short y, short flag)
{
	ntnear = nearregion(x, y);
	if (!ntnear)
		return 0;
	ntreg = regionat(x, y);
	if (flag)
		ntkind = 3;
	else
		ntkind = 4;
	for (nti = 0; nti < points[ntnear].counts[ntkind - 1]; nti++) {
		ntpt = points[ntnear].lists[ntkind - 1][nti];
		ntx = points[ntnear].x[ntpt];
		nty = points[ntnear].y[ntpt];
		if (!ntx || !nty)
			continue;
		if (flag) {
			ntidx = findclass(ntx + 1 + nty * 150, 0xd1);
			if (!ntidx)
				ntidx = findclass(ntx + (nty + 1) * 150, 0xd1);
			if (!ntidx)
				continue;
			if (typenames[mapitems[ntidx] & 0xfff] != 0x4d)
				continue;
found:
			if (regionat(ntx, nty) != ntreg)
				continue;
			if (ntreg)
				return 1;
			if (celldist(x, y, ntx, nty) < 10)
				return 1;
		} else {
			ntidx = findclass(ntx + nty * 150, 0xcb);
			if (!ntidx) {
				ntidx = findclass(ntx - 1 + nty * 150, 0xcb);
				if (!ntidx)
					ntidx = findclass(ntx + (nty - 1) * 150, 0xcb);
			}
			if (!ntidx)
				continue;
			if (typenames[mapitems[ntidx] & 0xfff] == 0x70)
				goto found;
		}
	}
	return 0;
}

short
atwork(Unit *u)
{
	if (badunit(u))
		return 0;
	switch (u->mode) {
	case 2:
	case 3:
	case 4:
	case 0x32:
		return 1;
	}
	return 0;
}

unsigned long
jewelmagic(Unit *u, unsigned long mask)
{
	if (badunit(u))
		return 0;
	return (u->wrist | u->neck) & mask;
}

void
calcload(unsigned short n)
{
	units[n].load = itemweight(units[n].head) + itemweight(units[n].body) + itemweight(units[n].cloak)
	    + itemweight(units[n].neck) + itemweight(units[n].wrist) + itemweight(units[n].hand)
	    + itemweight(units[n].belt) + itemweight(units[n].offhand) + itemweight(units[n].legs)
	    + itemweight(units[n].feet) + itemweight(units[n].items[0]) + itemweight(units[n].items[1])
	    + itemweight(units[n].items[2]);
	if (n == 0)
		units[0].load += itemweight(0x2e4) - weightof(0x2e4);
}

short
immobility(Unit *u)
{
	if (badunit(u))
		return 0;
	topimmob = 0;
	toppiece = 0;
	nextpiece = 0;
	nextimmob = 0;
	immsum = 0;
	immitem = u->feet;
	immval = armors[gearof(immitem)].hindrance;
	immsum += immval;
	if (immval && immval >= topimmob) {
		nextpiece = toppiece;
		toppiece = immitem;
		nextimmob = topimmob;
		topimmob = immval;
	}
	immitem = u->legs;
	immval = armors[gearof(immitem)].hindrance;
	immsum += immval;
	if (immval && immval >= topimmob) {
		nextpiece = toppiece;
		toppiece = immitem;
		nextimmob = topimmob;
		topimmob = immval;
	}
	if (classof(u->offhand & 0xfff) == 0x55) {
		immitem = u->offhand;
		immval = armors[gearof(immitem)].hindrance;
		immsum += immval;
		if (immval && immval >= topimmob) {
			nextpiece = toppiece;
			toppiece = immitem;
			nextimmob = topimmob;
			topimmob = immval;
		}
	}
	immitem = u->head;
	immval = armors[gearof(immitem)].hindrance;
	immsum += immval;
	if (immval && immval >= topimmob) {
		nextpiece = toppiece;
		toppiece = immitem;
		nextimmob = topimmob;
		topimmob = immval;
	}
	immitem = u->body;
	immval = armors[gearof(immitem)].hindrance;
	immsum += immval;
	if (immval && immval >= topimmob) {
		nextpiece = toppiece;
		toppiece = immitem;
		nextimmob = topimmob;
		topimmob = immval;
	}
	if (immsum > 100)
		immsum = 100;
	else if (immsum < 0)
		immsum = 0;
	return immsum;
}

unsigned long
corpseof(unsigned long n)
{
	if (!n)
		return 0;
	for (coidx = 1; coidx < nmapitems; coidx++)
		if (mapitems[coidx] >> 16 == n && classof(mapitems[coidx] & 0xfff) == 0x15)
			return coidx;
	return 0;
}

void
setcorpse(unsigned long n)
{
	if (!n)
		return;
	scoidx = corpseof(n);
	if (!scoidx)
		return;
	scolook = corpseitem(&units[n]);
	scotype = mapitems[scoidx] & 0xfff;
	if ((scolook & 0xfff) == scotype)
		return;
	if ((scolook & 0xfff) + 0x34 == scotype)
		return;
	if ((unsigned)random() < 0x3fff)
		scolook += 0x34;
	mapitems[scoidx] &= 0xfffff000;
	mapitems[scoidx] |= scolook;
}

void
snapskills(int mode)
{
	for (snapk = 0; snapk < 30; snapk++) {
		setskill(snapk);
		if (mode == 1) {
			game.oldlevels[snapk] = game.levels[snapk];
			game.levels[snapk] = kinds[19].aptitude[snapk];
			game.snaptime[snapk] = gametime;
			game.snappoints[snapk] = game.skills[snapk];
		} else if (gametime - game.snaptime[snapk] >= hourticks / 5) {
			game.oldlevels[snapk] = game.levels[snapk];
			game.snappoints[snapk] = game.skills[snapk];
		}
		if (mode == 2)
			game.msgpoints[snapk] = game.skills[snapk];
	}
}

short
statbonus(int a, int b)
{
	a = a / (a / 300 + 140) * b;
	a /= 100;
	a = a / 70 + 100;
	if (a < 40)
		a = 40;
	else if (a > 140)
		a = 140;
	a -= 100;
	return a;
}

void
calcstats(Unit *u)
{
	short v;

	if (badunit(u))
		return;
	if (u == &units[0]) {
		kindof(u)->strength = statbonus(game.strpoints, 200) + 100;
		kindof(u)->dexterity = statbonus(game.dexpoints, 100) + 100;
		kindof(u)->intellect = statbonus(game.intpoints, 50) + 100;
	}
	baseitems(u);
	if ((u->head & 0xfff) == 0)
		u->head = 0;
	if ((u->body & 0xfff) == 0)
		u->body = 0;
	if ((u->cloak & 0xfff) == 0)
		u->cloak = 0;
	if ((u->neck & 0xfff) == 0)
		u->neck = 0;
	else if (u->neck >> 16)
		u->neck |= 0x80000000;
	if ((u->wrist & 0xfff) == 0)
		u->wrist = 0;
	else if (u->wrist >> 16)
		u->wrist |= 0x80000000;
	if ((u->hand & 0xfff) == 0)
		u->hand = 0;
	if ((u->belt & 0xfff) == 0)
		u->belt = 0;
	if ((u->offhand & 0xfff) == 0)
		u->offhand = 0;
	if ((u->legs & 0xfff) == 0)
		u->legs = 0;
	if ((u->feet & 0xfff) == 0)
		u->feet = 0;
	if ((u->items[0] & 0xfff) == 0)
		u->items[0] = 0;
	if ((u->items[1] & 0xfff) == 0)
		u->items[1] = 0;
	if ((u->items[2] & 0xfff) == 0)
		u->items[2] = 0;
	if (u->hand == 0 && u->belt != 0)
		rearm(u);
	calcload(indexof(u));
	if (u == &units[0] && game.hungerlevel >= 9) {
		v = kindof(u)->maxhealth - (game.hungerlevel - 9) * 2;
		if (v < units[0].health)
			units[0].health = v;
	}
	u->strength = kindof(u)->strength;
	u->dexterity = kindof(u)->dexterity;
	u->intellect = kindof(u)->intellect;
	if (u->berserk)
		u->strength += kindof(u)->strength / 2;
	if (u->weakened)
		u->strength -= kindof(u)->strength / 5;
	if (u->poison) {
		u->strength *= 75;
		u->strength /= 100;
	}
	if (u->wrist & 0x200000) {
		u->strength -= kindof(u)->strength / 25;
		u->dexterity -= kindof(u)->dexterity / 25;
	}
	if (u->neck & 0x200000) {
		u->strength -= kindof(u)->strength / 25;
		u->dexterity -= kindof(u)->dexterity / 25;
	}
	if (u->wrist & 0x10000)
		u->strength += kindof(u)->strength / 50;
	if (u->neck & 0x10000)
		u->strength += kindof(u)->strength / 50;
	if (u->wrist & 0x20000)
		u->dexterity += kindof(u)->dexterity / 50;
	if (u->neck & 0x20000)
		u->dexterity += kindof(u)->dexterity / 50;
	if (u->drunk && u != &units[0]) {
		if (u->drunk >= 210)
			csval = 19;
		else if (u->drunk >= 150)
			csval = 46;
		else if (u->drunk >= 90)
			csval = 73;
		else
			csval = 97;
		u->dexterity *= csval;
		u->dexterity /= 100;
		u->intellect *= csval;
		u->intellect /= 100;
	}
	if (u->asleep) {
		u->dexterity *= 25;
		u->dexterity /= 100;
	}
	csval = u->health * 100 / kindof(u)->maxhealth;
	u->strength -= kindof(u)->strength * (100 - csval) / 100 / 2;
	if (u->berserk)
		u->dexterity += kindof(u)->dexterity / 3;
	if (u->warded)
		u->dexterity += kindof(u)->dexterity / 10;
	if (u->weakened)
		u->dexterity -= kindof(u)->dexterity / 10;
	if (u->poison) {
		u->dexterity *= 66;
		u->dexterity /= 100;
	}
	csval = u->health * 100 / kindof(u)->maxhealth;
	u->dexterity -= kindof(u)->dexterity * (100 - csval) / 100 / 3;
	if (u->berserk)
		u->intellect /= 2;
	if (u == &units[0]) {
		if (game.hungerlevel <= 5)
			;
		else if (game.hungerlevel == 6)
			u->strength -= kindof(u)->strength * 5 / 100;
		else if (game.hungerlevel == 7)
			u->strength -= kindof(u)->strength * 10 / 100;
		else if (game.hungerlevel >= 8) {
			u->strength -= kindof(u)->strength * 15 / 100;
			u->strength -= (game.hungerlevel - 8) * 2;
		}
		if (game.tiredcount >= 150) {
			csval = (game.tiredcount - 150) / 10;
			if (game.tiredcount >= 240)
				csval += (game.tiredcount - 230) / 20;
			u->dexterity -= csval;
			u->intellect -= csval;
		}
		snapskills(0);
	}
	csval = (u->strength * 25 - u->load) / 100;
	if (csval < 0) {
		csload = 0;
		if (csval < -10) {
			csload = (csval + 10) * 4;
			csval = -10;
		}
		if (csval < -5) {
			csload += (csval + 5) * 2;
			csval = -5;
		}
		if (csval < 0)
			csload += csval;
		csval = u->dexterity + csload;
		if (csval < 5 && u->dexterity >= 5)
			csval = 5;
		u->dexterity = csval;
	}
	csval = 100 - immobility(u);
	if (csval < 0)
		csval = 0;
	csval = csval * u->dexterity / 100;
	if (csval < 5 && u->dexterity >= 5)
		csval = 5;
	u->dexterity = csval;
	if (u->strength < 0)
		u->strength = 0;
	if (u->dexterity < 0)
		u->dexterity = 0;
	if (u->intellect < 0)
		u->intellect = 0;
	setweapkinds(u);
	if (u->health <= 0)
		setcorpse(indexof(u));
}

short
resthours(Unit *u)
{
	if (badunit(u))
		return 0;
	rhours = 0;
	if (u == &units[0] && game.hungercount >= 1680)
		rhours = game.hungercount / 1680;
	if (u->poison)
		rhours++;
	if (u->health < kindof(u)->maxhealth * 2 / 3) {
		rhours += (kindof(u)->maxhealth - u->health) / 10;
		rhours--;
		if (rhours <= 0)
			rhours = 1;
	}
	if (rhours == 0 && (u->health < kindof(u)->maxhealth * 95 / 100 || u->bleeding || u->weakened || u->bewitched || u == &units[0] && game.hungercount >= 480))
		rhours++;
	return rhours;
}

void
clearstatus(Unit *u)
{
	if (badunit(u))
		return;
	u->poison = 0;
	u->bleeding = 0;
	u->drunk = 0;
	u->weakened = 0;
	u->warded = 0;
	u->bewitched = 0;
	u->cursed = 0;
	u->asleep = 0;
}

void
restfor(short n)
{
	if (n != 0xfe)
		clearmind(&units[0]);
	rfhours = resthours(&units[0]);
	if (rfhours < 1)
		rfhours = 1;
	if (n > rfhours)
		n = rfhours;
	rfgain = kindof(&units[0])->maxhealth * n / rfhours;
	if (units[0].health < rfgain)
		units[0].health = rfgain;
	rfgain = kindof(&units[0])->maxvigor * n / rfhours;
	if (units[0].vigor < rfgain)
		units[0].vigor = rfgain;
	clearstatus(&units[0]);
	game.hungercount = 0;
	game.tiredcount = 0;
	setlevels();
	calcstats(&units[0]);
	fillgauges();
}

void
varywear(unsigned long *p, unsigned long mask)
{
	if (*p == 0)
		return;
	if (mask == 0) {
		if (randn(100) < 33)
			mask = 0x20000000;
	} else if (mask == 0xe0000000) {
		if (randn(100) < 33)
			mask -= 0x20000000;
	} else
		mask += (randn(3) - 1) << 29;
	*p &= 0x1fffffff;
	*p |= mask;
}

void
varygear(Unit *u, unsigned long mask)
{
	if (badunit(u))
		return;
	varywear(&u->head, mask);
	varywear(&u->body, mask);
	varywear(&u->cloak, mask);
	varywear(&u->legs, mask);
	varywear(&u->feet, mask);
	varywear(&u->hand, mask);
	varywear(&u->belt, mask);
	varywear(&u->offhand, mask);
}

void
baseitems(Unit *u)
{
	if (badunit(u))
		return;
	u->hand = baselook(u->hand);
	u->belt = baselook(u->belt);
	u->offhand = baselook(u->offhand);
	u->head = baselook(u->head);
	u->body = baselook(u->body);
	u->legs = baselook(u->legs);
	u->cloak = baselook(u->cloak);
	u->feet = baselook(u->feet);
	u->items[0] = baselook(u->items[0]);
	u->items[1] = baselook(u->items[1]);
	u->items[2] = baselook(u->items[2]);
}

void
flagunit(Unit *u)
{
	if (badunit(u))
		return;
	u->flags &= 0xfa;
	if (animalkind(u)) {
		u->flags |= 4;
		u->traits |= 0x10;
		if (u->home)
			u->home |= 0x8000;
	}
	if (femalekind(u))
		u->flags |= 1;
	if (unfree(u)) {
		u->flags |= 2;
		if (u->home)
			u->home |= 0x8000;
	} else
		u->flags &= 0xfd;
	if (myservant(u))
		u->traits &= 0xdf;
}

unsigned short
newunit(unsigned char kind, unsigned char type)
{
	short r;

	for (nui = 1; nui < nunits; nui++)
		if (units[nui].health <= 0 && units[nui].x == 0 && units[nui].y == 0 && units[nui].modeb == 0) {
			nuunit = &units[nui];
			goto found;
		}
	// FIXED: with 150 units the next one lands past units[150]. Give it the last unit's place.
#ifdef FEATURE_FAULTS
	if (nunits == 150)
		nunits--;
#endif
	nuunit = &units[nunits++];
found:
	memset(nuunit, 0, sizeof(Unit));
	nuunit->picset = (unsigned short)kind + 15;
	nuunit->part = kind;
	if (type)
		nuunit->type = type;
	else
		nuunit->type = partof(nuunit)->type;
	nuunit->dirx = 0;
	nuunit->diry = 1;
	nui = partof(nuunit)->outfit;
	nuunit->head = outfits[nui].head;
	nuunit->body = outfits[nui].body;
	nuunit->hand = outfits[nui].hand;
	nuunit->belt = outfits[nui].belt;
	nuunit->offhand = outfits[nui].offhand;
	nuunit->legs = outfits[nui].legs;
	nuunit->feet = outfits[nui].feet;
	nuunit->health = kindof(nuunit)->maxhealth;
	nuunit->vigor = kindof(nuunit)->maxvigor;
	nuunit->spare = kindof(nuunit)->spare;
	nuunit->courage = kindof(nuunit)->courage;
	nuunit->traits = kindof(nuunit)->traits;
	nuunit->flags = 0;
	flagunit(nuunit);
	if (kindof(nuunit)->hostile) {
		nuunit->sympathy = 0;
		setunitmode(nuunit, 0x14);
		nuunit->modea = 0;
		nuunit->modeb = 0;
		nuunit->modec = 0xff;
	} else {
		if (!isfemale(nuunit) || isservant(nuunit))
			nuunit->sympathy = 2;
		else
			nuunit->sympathy = 3;
		if (isanimal(nuunit))
			setunitmode(nuunit, 7);
		else
			setunitmode(nuunit, 3);
	}
	nuweapon = 0;
	nuarmor = 0;
	nupack = 0;
	for (nui = 1; nui < 5; nui++) {
		nuitem = *(unsigned long *)partdefs[((Recipe *)(recipes + 0x20))[nuunit->part].parts[nui]].item;
		if (nuitem == 0)
			continue;
		if (isweapon(nuitem)) {
			nuunit->hand = nuitem;
			nuweapon = 1;
			nuunit->belt = 0;
		} else if (wearable(nuitem))
			puton(indexof(nuunit), nuitem);
		else if (nupack < 3)
			nuunit->items[nupack++] = nuitem;
		if (classof(nuitem & 0xfff) == 0x46)
			nuarmor = 1;
	}
	nui = kindof(nuunit)->loot;
	if (!nuweapon)
		for (nuj = 0; nuj < 2; nuj++) {
			nuitem = loot[nui].weapons[nuj];
			if (nuitem && randn(100) < loot[nui].wchance[nuj]) {
				nuitem = newitem(nuitem);
				nuunit->hand = nuitem;
			}
		}
	nurank = 0;
	if (nuunit->head && (nuunit->head & 0xfff) != 0x1ba)
		nurank = 1;
	if (nuunit->body) {
		if (valueof(nuunit->body) > valueof(0xee01af))
			nurank = 1;
		if (valueof(nuunit->body) >= valueof(0xf001b1) * 2 / 3)
			nurank = 2;
	}
	if (!nurank && classof(nuunit->offhand & 0xfff) != 0x55 && nuunit->head == 0 && (nuunit->body & 0xfff) == 0x1ad && nuunit->cloak == 0)
		nurank = -1;
	for (nuj = 0; nuj < 5 && nupack < 3; nuj++) {
		nuitem = loot[nui].items[nuj];
		if (nuitem && randn(100) < loot[nui].chance[nuj]) {
			nuitem = newitem(nuitem);
			nuunit->items[nupack] = nuitem;
			nupack++;
		}
	}
	if (isthrowing2(nuunit->hand) && !isrobber(nuunit)) {
		nuitem = 0;
		if (nuunit->type == 0x2d)
			nuitem = 0xe501cb;
		else if (nuunit->type == 0x1a || nuunit->type == 0x1b || nuunit->type == 0x18 || nuunit->type == 0x1e || nuunit->type == 0x1d) {
			if (nurank > 1) {
				if (randn(100) < 66)
					nuitem = 0xe901cf;
				else
					nuitem = 0xe501cb;
			} else if (nurank > 0) {
				if (randn(100) < 10)
					nuitem = 0xe901cf;
				else if (randn(100) < 25)
					nuitem = 0xe801ce;
				else
					nuitem = 0xe401ca;
			} else
				nuitem = 0xe401ca;
		} else {
			r = randn(100);
			if (r < 15)
				nuitem = 0xe801ce;
			else if (r < 50)
				nuitem = 0xe401ca;
			else
				nuitem = 0xe301c9;
		}
		nuunit->belt = nuitem;
	}
	baseitems(nuunit);
	if (nuunit->belt) {
		if (ismelee(nuunit->hand)) {
			packunit(indexof(nuunit), nuunit->belt);
			nuunit->belt = 0;
		} else
			for (nuj = 0; nuj < 3; nuj++)
				if (ismelee(nuunit->items[nuj]) && valueof(nuunit->items[nuj]) > valueof(nuunit->belt)) {
					nuitem = nuunit->belt;
					nuunit->belt = nuunit->items[nuj];
					nuunit->items[nuj] = nuitem;
				}
	}
	if (ismelee(nuunit->hand))
		for (nuj = 0; nuj < 3; nuj++)
			if (ismelee(nuunit->items[nuj]) && valueof(nuunit->items[nuj]) > valueof(nuunit->hand)) {
				nuitem = nuunit->hand;
				nuunit->hand = nuunit->items[nuj];
				nuunit->items[nuj] = nuitem;
			}
	if (nuunit->belt == 0 && isthrowing2(nuunit->hand))
		for (nuj = 0; nuj < 3; nuj++)
			if (ismelee(nuunit->items[nuj])) {
				nuunit->belt = nuunit->items[nuj];
				nuunit->items[nuj] = 0x80018f;
			}
	for (nuj = 0; nuj < 3; nuj++)
		if (nuunit->items[nuj] && ((nuunit->items[nuj] & 0xfff) == (nuunit->belt & 0xfff) || (nuunit->items[nuj] & 0xfff) == (nuunit->hand & 0xfff)))
			nuunit->items[nuj] = 0x80018f;
	varygear(nuunit, kindof(nuunit)->wear);
	if (isservant(nuunit))
		nuunit->scene = 0x26e1;
	if (nuunit == &units[0])
		;
	else if (isrobber(nuunit)) {
		if (getbit(game.flags, 0x4bb))
			nuunit->scene = randn(5) + 0x2702;
		else
			nuunit->scene = 0x2702;
		nuunit->plea = 0;
		nuunit->mercy = 0x26d8;
	} else if (nuunit->sympathy > 0 && !isanimal(nuunit)) {
		if (isservant(nuunit)) {
			nuunit->mercy = 0x26d8;
			nuunit->plea = 0x26dc;
		} else if (isfemale(nuunit)) {
			nuunit->mercy = 0x26db;
			nuunit->plea = 0x26da;
		} else {
			nuunit->mercy = 0x26d8;
			if (nuunit->courage < 3)
				nuunit->plea = 0x26d7;
		}
	} else if (isanimal(nuunit))
		setcry(nuunit);
	nuunit->face = randomtile(nuunit->type);
	if (isbear(nuunit))
		nuunit->clan = 0x62;
	else if (isanimal(nuunit))
		nuunit->number = 1;
	if (isservant(nuunit)) {
		nuunit->hand = 0;
		nuunit->belt = 0;
	}
	if (((Recipe *)(recipes + 0x20))[nuunit->part].parts[0] == 0 && !nuarmor)
		nuunit->body = 0;
	calcstats(nuunit);
	return indexof(nuunit);
}

int
replaceitem(unsigned long *p, unsigned long item, unsigned long x)
{
	if (item && (*p & 0xfff) != (item & 0xfff))
		return 0;
	setwear(&x, wearof(*p));
	*p = x;
	return 1;
}

short
freenear(short x0, short y0, short *x, short *y)
{
	if (*x < 8)
		*x = 8;
	else if (*x > 142)
		*x = 142;
	if (*y < 8)
		*y = 8;
	else if (*y > 142)
		*y = 142;
	fntry = 0;
	for (;;) {
		if (++fntry > 300)
			return 0;
		*x = randn(4);
		fndy = *x < 2 ? 2 : randn(3);
		if (randn(100) < 50)
			*x = -*x;
		*x += x0;
		*y = fndy + randn(2);
		if (randn(100) < 50)
			*y = -*y;
		*y += y0;
		fncell = *x + *y * 150;
		if (mapflags[fncell] & 0x80 && fntry < 200)
			continue;
		if (!(map[fncell] & 0x4000))
			continue;
		if (!(map[fncell] & 0x8000))
			continue;
		if (findtype(fncell, 0x133))
			continue;
		if (unitat(*x, *y) != 0xff)
			continue;
		return 1;
	}
}

int
unitnear(int x, int y)
{
	for (ununit = 1; ununit < nunits; ununit++)
		if (celldist(x, y, units[ununit].x, units[ununit].y) < 18)
			return ununit;
	return 0;
}

unsigned short
addpig(int n)
{
	pign = 0;
	do {
		pigx = randn(130) + 10;
		pigy = randn(130) + 10;
		if (pign++ > 100)
			return 0;
	} while (unitnear(pigx, pigy));
	if (!freenear(pigx, pigy, &pigx, &pigy))
		return 0;
	pign = newunit(n, 0x24);
	if (pign && pign != 0xff) {
		units[pign].x = pigx;
		units[pign].y = pigy;
		units[pign].number = 0;
		units[pign].traits |= 4;
		calcstats(&units[pign]);
		setunitmode(&units[pign], 7);
	}
	return pign;
}

int
cellfree(int x, int y)
{
	x += y * 150;
	if (mapflags[x] & 0x84)
		return 0;
	if (!(map[x] & 0x4000))
		return 0;
	if (!(map[x] & 0x8000))
		return 0;
	return 1;
}

unsigned long
randcloak(void)
{
	cloakroll = randn(100);
	if (cloakroll < 40)
		cloakitem = altlook(0xe0f601b9);
	else if (cloakroll < 80)
		cloakitem = 0xe0f601b9;
	else
		cloakitem = 0xe0fa01c6;
	return cloakitem;
}

unsigned long
randgoods(int n)
{
again:
	goodsroll = randn(100);
	if (goodsroll < 1 && game.defeated > 4)
		return 0x4f4;
	if (goodsroll < 2 && game.defeated > 4)
		return 0x4f7;
	if (goodsroll < 3 && game.defeated > 5)
		return 0x514;
	if (goodsroll < 6 && game.defeated > 3)
		return 0x4f3;
	if (goodsroll < 9 && game.defeated > 3)
		return 0x4f6;
	if (goodsroll < 14)
		return 0x4f2;
	if (goodsroll < 20)
		// FIXED: the pairs of jewelry take 0x4f4 and 0x4f7, then 0x4f3 and 0x4f6, then 0x4f2 twice. Take 0x4f5 second, as plt.cpp's ladder buys it.
#if defined(FEATURE_FIXES)
		return 0x4f5;
#else
		return 0x4f2;
#endif
	if (goodsroll < 30) {
		if (n)
			goto again;
		return 0x2c3;
	}
	if (goodsroll < 40) {
		if (n)
			goto again;
		return randn(3) + 0x50d;
	}
	if (goodsroll < 45)
		return newitem(0x1eb);
	if (goodsroll < 46)
		return newitem(0x176);
	if (goodsroll < 48)
		return newitem(0x177);
	if (goodsroll < 51)
		return newitem(0x17d);
	if (goodsroll < 52)
		return newitem(0x18d);
	if (goodsroll < 53)
		return newitem(0x192);
	if (goodsroll < 55)
		return newitem(0x175);
	if (goodsroll < 56)
		return newitem(0x151);
	if (goodsroll < 57)
		return newitem(0x18c);
	if (goodsroll < 67)
		return 0x180;
	if (goodsroll < 70)
		return 0x965;
	if (goodsroll < 80 && game.defeated > 4)
		return coinitem(1, 1);
	return coinitem(randn(15) + 3, 0);
}

unsigned long
randutensil(void)
{
	utensilroll = randn(100);
	if (utensilroll < 10)
		return 0x972;
	if (utensilroll < 20)
		return 0x9a6;
	if (utensilroll < 30)
		return 0x97f;
	if (utensilroll < 40)
		return 0xe0e801ce;
	if (utensilroll < 55)
		return 0xe0e401ca;
	if (utensilroll < 70)
		return 0x532;
	return 0x3ff0221;
}

unsigned long
randloot(int k)
{
	int r;

	if (k == 0) {
		r = randn(100);
		if (r < 40)
			return randloot(1);
		if (r < 70)
			return randloot(2);
		return randloot(3);
	}
	if (k == 1) {
		if (randn(100) < 20)
			return newitem(0x533);
		return randmeat();
	}
	if (k == 2)
		return randgoods(0);
	if (k == 3)
		return randutensil();
	return 0x40018e;
}

int
dropspot(int x, int y, int flag)
{
	for (dropr = 0; dropr < 7; dropr++)
		for (dropy = y - dropr; dropy <= y + dropr; dropy++)
			for (dropx = x - dropr; dropx <= x + dropr; dropx++)
				if (canput(dropx + dropy * 150) && (!flag || addblood(dropx, dropy, -1, 0)))
					return dropx + dropy * 150;
	return 0;
}

int
dropnear(int x, int y, unsigned long item)
{
	if (weightof(item & 0xfff) >= 65000)
		dropheavy = 1;
	else
		dropheavy = 0;
	x = dropspot(x, y, dropheavy);
	if (!x)
		return 0;
	if (dropheavy && topitem(x))
		return 0;
	additem(x, item);
	return x;
}

void
putclear(int x, int y, unsigned long item)
{
	x += y * 150;
	for (;;) {
		puttop = topitem(x);
		if (!puttop || weightof(mapitems[puttop] & 0xfff) >= 65000)
			break;
		popitem(x, 0);
	}
	additem(x, item);
}

void
stash(int x, int y, unsigned long item, int k, int n)
{
	unsigned long id, it;
	int j;

	id = newitem(item);
	if (id == 0xffffffff)
		return;
	putclear(x, y, id);
	for (x = 0; x < n; x++) {
		j = 0;
		do {
			it = randloot(k);
			if (!counttypes(idof(id), it, 0, 0))
				break;
			switch (classof(it & 0xfff)) {
			case 0xf:
			case 0x10:
			case 0x36:
			case 0x37:
			case 0x56:
				goto add;
			}
			if ((it & 0xfff) == 0x180)
				break;
		} while (j++ < 10);
	add:
		stow(idof(id), it);
	}
}

/* reads i and j unset when flag is set (its one caller passes 0) */
void
addcamp(short x, short y, short flag, int n)
{
	int r, i, j;

	if (!flag) {
		for (r = 8; r < 30; r++) {
			if (y < playery)
				j = y - r;
			else
				j = y + r;
			for (;;) {
				for (i = x - r; i <= x + r; i++) {
					if (i < 7 || j < 7 || i > 143 || j > 143)
						continue;
					if (i >= x - 7 && i <= x + 7 && j >= y - 7 && j <= y + 7)
						continue;
					if (!cellfree(i, j) || terrains[map[i + j * 150] & 0xfff] == 3)
						continue;
					if (!cellfree(i + 1, j) || terrains[map[i + 1 + j * 150] & 0xfff] == 3)
						continue;
					if (cellfree(i + 1, j + 1) && terrains[map[i + 1 + (j + 1) * 150] & 0xfff] != 3
					|| cellfree(i, j + 1) && terrains[map[i + (j + 1) * 150] & 0xfff] != 3) {
						x = i;
						y = j;
						goto place;
					}
				}
				if (y < playery) {
					if (j++ < y + r)
						continue;
				} else if (y - r < j--)
					continue;
				break;
			}
		}
		return;
	}
place:
	if (randn(100) < 50) {
		putclear(i, j, randcloak());
		putclear(i + 1, j, 0x133);
		additem(i + 1 + j * 150, 0x131);
	} else {
		putclear(i + 1, j, randcloak());
		putclear(i, j, 0x133);
		additem(i + j * 150, 0x131);
	}
	if (cellfree(i + 1, j + 1) && terrains[map[i + 1 + (j + 1) * 150] & 0xfff] != 3)
		stash(i + 1, j + 1, 0x19a, 0, randn(8) + 3);
	else if (cellfree(i, j + 1))
		stash(i, j + 1, 0x19a, 0, randn(8) + 3);
	if (n >= 3) {
		if (n > 2 && randn(100) < 50 || randn(100) < 15) {
			if (!freenear(i, j, &x, &y))
				return;
			stash(x, y, 0x19a, 1, randn(5) + 3);
		}
		if (n > 3 && randn(100) < 40 || randn(100) < 10) {
			if (!freenear(i, j, &x, &y))
				return;
			stash(x, y, 0x19a, 3, randn(4) + 3);
		}
		if (n > 4 && randn(100) < 30 || randn(100) < 5) {
			if (!freenear(i, j, &x, &y))
				return;
			stash(x, y, 0x19a, 2, randn(4) + 3);
		}
	}
	{
		int k;

		for (k = 0; k < n; k++) {
			if (!freenear(i, j, &x, &y))
				return;
			if (randn(100) < 60)
				// FIXED: each sleeper's cloak lands on the camp's own cell, where freenear has found a free one. Lay it there.
#if defined(FEATURE_FIXES)
				putclear(x, y, randcloak());
#else
				putclear(i, j, randcloak());
#endif
		}
	}
}

int
openground(int cell)
{
	groundcell = map[cell];
	if (!(groundcell & 0x4000) || terrains[groundcell & 0xfff] > 1 || mapflags[cell] & 4)
		return 0;
	return 1;
}

int
nearcave(int x, int y)
{
	ncmin = 300;
	ncbest = -1;
	for (nci = 0; nci < 28; nci++)
		if (!caveused[nci]) {
			ncdist = celldist(x, y, caves[nci][0], caves[nci][1]);
			if (ncdist <= ncmin) {
				ncmin = ncdist;
				ncbest = nci;
			}
		}
	return ncbest;
}

int
randcave(void)
{
again:
	caveroll = randn(100);
	if (caveroll < 10)
		return 1;
	if (caveroll < 20)
		return 2;
	if (caveroll < 30)
		return 3;
	if (caveroll < 40)
		return 4;
	if (caveroll < 50)
		return 5;
	if (caveroll < 60)
		return 6;
	if (!getbit(game.flags, 0x5bf))
		goto again;
	if (cheat && keys['W'] & 0x80)
		goto again;
	return 0x63;
}

int
addcave(void)
{
	if (game.ncaves >= 10)
		return 0;
	aci = 0;
	do {
	again:
		acx = randn(120) + 15;
		acy = randn(120) + 15;
		accell = acx + acy * 150;
		if (openground(accell) && openground(accell + 1)
		&& openground(accell + 2) && openground(accell + 150)
		&& openground(accell + 151) && openground(accell + 152)
		&& openground(accell + 300) && openground(accell + 301)
		&& !(mapflags[accell + 301] & 0x80)
		&& openground(accell + 302) && openground(accell + 451)) {
			// FIXED: the count of tries doubles as the index of the caves, and a spot near a cave starts over uncounted, so the search can run forever. Count every try.
#if defined(FEATURE_FIXES)
			{
				int k;

				for (k = 0; k < game.ncaves; k++)
					if (celldist(game.cavex[k], game.cavey[k], accell % 150, accell / 150) < 20)
						break;
				if (k < game.ncaves)
					continue;
			}
#else
			for (aci = 0; aci < game.ncaves; aci++)
				if (celldist(game.cavex[aci], game.cavey[aci], accell % 150, accell / 150) < 20)
					goto again;
#endif
			aci = nearcave(acx, acy);
			caveused[aci] = 1;
			game.cavemap[game.ncaves] = curmap;
			game.cavex[game.ncaves] = accell % 150 + 1;
			game.cavey[game.ncaves] = accell / 150 + 1;
			game.cavekind[game.ncaves] = aci;
			game.cavelayout[game.ncaves] = randcave();
			game.ncaves++;
			putclear(acx, acy, 0x236);
			putclear(acx + 1, acy, 0x237);
			putclear(acx + 2, acy, 0x238);
			putclear(acx, acy + 1, 0x243);
			putclear(acx + 1, acy + 1, 0xa3);
			additem(accell + 151, newmarker(3, caves[aci][0], caves[aci][1] - 1));
			additem(accell + 151, 0x244);
			putclear(acx + 2, acy + 1, 0x245);
			putclear(acx, acy + 2, 0x250);
			putclear(acx + 1, acy + 2, 0xb0);
			additem(accell + 301, 0x251);
			putclear(acx + 2, acy + 2, 0x252);
			return 1;
		}
	} while (aci++ <= 2500);
	return 0;
}

int
strewcave(int x, int y, int type)
{
	strewcell = x + y * 150;
	if (strewcell == strewlast)
		return 0;
	if (type == 3) {
	drop:
		dropnear(x, y, randpit() + 13);
	done:
		strewlast = strewcell;
		return 1;
	}
	if (type == 1 || type == 2) {
		strewroll = randn(100);
		if (strewroll < 30)
			goto drop;
		if (strewroll < 60) {
			strewroll = dropspot(x, y, 1);
			if (strewroll) {
				addblood(strewroll % 150, strewroll / 150, randn(100) < 60 ? 0 : 1, 2);
				if (randn(100) < 30) {
					additem(strewroll, randmeat());
					goto done;
				}
			}
		}
		strewroll = randn(100);
		if (strewroll < 15)
			dropnear(x, y, randn(100) < 50 ? 0x30b : 0x2fe);
		else if (strewroll < 25) {
			unsigned long k = (randn(100) < 50 ? 0x34 : 0) + 0xa2a;

			dropnear(x, y, k);
		}
		else if (strewroll < 30) {
		chest:
			dropnear(x, y, newitem(0x375));
		}
		goto done;
	}
	if (type == 4 && randn(100) < 30)
		dropnear(x, y, randpit() + 13);
	if (type == 5 || type == 6) {
		strewroll = randn(100);
		if (strewroll < 20)
			goto drop;
		if (strewroll < 24)
			goto chest;
		if (strewroll < 28)
			dropnear(x, y, 0x3ff0221);
		goto done;
	}
	return 0;
}

// PORT: C leaves the order of a call's arguments open. These draw y's offset, then x's, as Watcom compiled fillcave's calls
#ifdef PORT
static int
strewnear(int x, int y, int n, int type)
{
	int ry;

	ry = randn(n);
	return strewcave(x + randn(n), y + ry, type);
}

static int
spotnear(int x, int y, int n, int flag)
{
	int ry;

	ry = randn(n);
	return dropspot(x + randn(n), y + ry, flag);
}
#endif

int
fillcave(int x, int y, int type, int d, int e, int f)
{
	additem(x + y * 150, newmarker(d, e, f));
	if (cheat && keys['W'] & 0x80)
		type = randcave();
	if (type == 0x63)
		return 0;
	strewlast = 0;
	fcx = x;
	fcy = y;
	for (fci = 0; fci < 7; fci++) {
		for (fcn = 0; fcy > 10 && map[(fcy - 1) * 150 + fcx] & 0x4000; fcy--, fcn++)
			if (fci == 0 && !(map[fcx - 1 + fcy * 150] & 0x4000) && (map[fcx + 1 + fcy * 150] & 0x4000) == 0)
				mapflags[fcy * 150 + fcx] |= 8;
		if (fcn > 6)
			fcy -= 3;
		// PORT: C leaves the order of a call's arguments open. Draw them from the last to the first, as Watcom compiled them
#ifdef PORT
		strewnear(fcx - 2, fcy - 2, 5, type);
#else
		strewcave(fcx - 2 + randn(5), fcy - 2 + randn(5), type);
#endif
		if (randn(100) < 50) {
			for (fcn = 0; fcx > 10 && map[fcx - 1 + fcy * 150] & 0x4000; fcx--)
				fcn++;
			if (fcn < 5)
				goto right;
		} else {
		right:
			for (fcn = 0; fcx < 140 && map[fcx + 1 + fcy * 150] & 0x4000; fcx++)
				fcn++;
		}
		// PORT: as above
#ifdef PORT
		strewnear(fcx - 2, fcy - 2, 5, type);
#else
		strewcave(fcx - 2 + randn(5), fcy - 2 + randn(5), type);
#endif
	}
	fccell = dropspot(fcx, fcy, 1);
	if (fccell == 0)
		return 0;
	fcx = fccell % 150;
	fcy = fccell / 150;
	if (type == 6) {
		// PORT: as above
#ifdef PORT
		fccell = spotnear(fcx - 2, fcy - 2, 5, 1);
#else
		fccell = dropspot(fcx - 2 + randn(5), fcy - 2 + randn(5), 1);
#endif
		if (fccell)
			stash(fccell % 150, fccell / 150, 0x19a, 0, randn(8) + 3);
		fci = 1;
		if (randn(100) < 20) {
			// PORT: as above
#ifdef PORT
			fccell = spotnear(fcx - 2, fcy - 2, 5, 1);
#else
			fccell = dropspot(fcx - 2 + randn(5), fcy - 2 + randn(5), 1);
#endif
			if (fccell) {
				stash(fccell % 150, fccell / 150, 0x19a, 1, randn(5) + 3);
				fci++;
			}
		}
		if (randn(100) < 15) {
			// PORT: as above
#ifdef PORT
			fccell = spotnear(fcx - 2, fcy - 2, 5, 1);
#else
			fccell = dropspot(fcx - 2 + randn(5), fcy - 2 + randn(5), 1);
#endif
			if (fccell) {
				stash(fccell % 150, fccell / 150, 0x19a, 3, randn(4) + 3);
				fci++;
			}
		}
		if (randn(100) < 10) {
			// PORT: as above
#ifdef PORT
			fccell = spotnear(fcx - 2, fcy - 2, 5, 1);
#else
			fccell = dropspot(fcx - 2 + randn(5), fcy - 2 + randn(5), 1);
#endif
			if (fccell) {
				stash(fccell % 150, fccell / 150, 0x19a, 2, randn(4) + 3);
				fci++;
			}
		}
		fcn = fci;
		for (fci = 0; fci < fcn - 1 + randn(3); fci++) {
			// PORT: as above
#ifdef PORT
			fccell = spotnear(fcx - 2, fcy - 2, 5, 0);
#else
			fccell = dropspot(fcx - 2 + randn(5), fcy - 2 + randn(5), 0);
#endif
			if (fccell)
				dropnear(fccell % 150, fccell / 150, randcloak());
		}
	}
	if (type == 4) {
		stash(fcx, fcy, 0x198, 2, randn(8) + 3);
		if (randn(100) < 30)
			for (fci = 0; fci < randn(3); fci++) {
				// PORT: as above
#ifdef PORT
				fccell = spotnear(fcx - 2, fcy - 2, 5, 1);
#else
				fccell = dropspot(fcx - 2 + randn(5), fcy - 2 + randn(5), 1);
#endif
				if (fccell)
					// PORT: as above
#ifdef PORT
				{
					int sn, sk;

					sn = randn(5) + 3;
					sk = randn(100) < 50 ? 2 : 3;
					stash(fccell % 150, fccell / 150, randn(100) < 50 ? 0x2d5 : 0x288, sk, sn);
				}
#else
					stash(fccell % 150, fccell / 150, randn(100) < 50 ? 0x2d5 : 0x288, randn(100) < 50 ? 2 : 3, randn(5) + 3);
#endif
			}
	}
	if (type == 5 || type == 1 && randn(100) < 40 || type == 2 && randn(100) < 40) {
		fcn = randn(100);
		if (fcn < 33)
			fcv = 0xe0000000;
		else if (fcn < 66)
			fcv = 0xc0000000;
		else
			fcv = 0xa0000000;
		additem(fccell, randgoods(1));
		additem(fccell, (randn(100) < 50 ? 0x34 : 0) + 0x825);
		fcn = randn(100);
		if (fcn < 10)
			additem(fccell, fcv | 0xe901cf);
		else if (fcn < 40)
			additem(fccell, fcv | 0xef01d5);
		else if (fcn < 70)
			additem(fccell, fcv | 0xe801ce);
		else
			additem(fccell, fcv | 0xe401ca);
		// PORT: as above
#ifdef PORT
		fccell = spotnear(fcx - 2, fcy - 2, 5, 0);
#else
		fccell = dropspot(fcx - 2 + randn(5), fcy - 2 + randn(5), 0);
#endif
		if (fccell)
			dropnear(fccell % 150, fccell / 150, newitem(randn(100) < 50 ? 0x347 : 0x348));
	}
	if (type == 1) {
		fcv = newunit(0x15, 0);
		if (fcv && fcv != 0xff) {
			units[fcv].x = fcx;
			units[fcv].y = fcy;
			markunits();
		}
	} else if (type == 2 || type == 3) {
		fcn = randn(3) + 1 + (type == 2 ? randn(3) : 0);
		for (fci = 0; fci < fcn; fci++) {
			// PORT: as above
#ifdef PORT
			fccell = spotnear(fcx - 5, fcy - 5, 11, 0);
#else
			fccell = dropspot(fcx - 5 + randn(11), fcy - 5 + randn(11), 0);
#endif
			if (fccell) {
				if (type == 2) {
					fcv = newunit(0x14, 0);
					if (fcv && fcv != 0xff) {
						units[fcv].clan = 0x62;
						units[fcv].number = 0;
						units[fcv].x = fccell % 150;
						units[fcv].y = fccell / 150;
					}
				} else if (type == 3) {
					fcv = newunit(9, 0);
					if (fcv && fcv != 0xff) {
						units[fcv].number = 0;
						units[fcv].x = fccell % 150;
						units[fcv].y = fccell / 150;
					}
				}
				markunits();
			}
		}
	}
	return 1;
}

int
fillcaves(void)
{
	if (curmap != 3 || getbit(game.flags, 0x5be))
		return 0;
	for (fcsi = 0; fcsi < game.ncaves; fcsi++)
		fillcave(caves[game.cavekind[fcsi]][0], caves[game.cavekind[fcsi]][1],
			game.cavelayout[fcsi], game.cavemap[fcsi], game.cavex[fcsi], game.cavey[fcsi]);
	setflag(0x5be, 1);
	return 1;
}

short
addrobber(short x, short y)
{
	// FIXED: the robber stands near the hero wherever travel scatters it. Search near x and y, which travel keeps on the map.
#if defined(FEATURE_FIXES)
	if (!freenear(x, y, &x, &y))
#else
	if (!freenear(playerx, playery, &x, &y))
#endif
		return 0;
	if (game.defeated < 4) {
		ari = 0;
		do {
			arunit = randn(21);
			ark = parts[arunit].type;
		} while ((ark == 0x18 || ark == 0x1a || ark == 0x1d || ark == 0x1e
			|| ark == 0x1f || ark == 0x29) && ari++ < 0x21);
	} else
		arunit = randn(21);
	arunit = newunit(arunit, 0);
	units[arunit].courage = randn(2) + 1;
	units[arunit].sympathy = 1;
	units[arunit].scene = randn(5) + 0x2702;
	units[arunit].plea = 0;
	units[arunit].mercy = 0x26d8;
	units[arunit].lord = 0;
	if (units[arunit].type == 0x20)
		units[arunit].flags |= 8;
	flagunit(&units[arunit]);
	units[arunit].number = 0x3fff;
	ark = 0;
again:
	arface = randomtile(0x22);
	if (ark++ < 100)
		for (ari = 0; ari < 20; ari++)
			if (game.faces[ari] == arface)
				goto again;
	units[arunit].clan = 1;
	units[arunit].face = arface;
	units[arunit].x = x;
	units[arunit].y = y;
	for (ari = 0; ari < 3; ari++) {
		ark = classof(units[arunit].items[ari] & 0xfff);
		if (ark == 0xcb || ark == 0xd1 || ark == 0x21)
			units[arunit].items[ari] = 0x40018e;
	}
	if (!issword(units[0].belt) && !issword(units[0].hand)) {
		if (issword(units[arunit].belt))
			units[arunit].belt = randn(100) < 75 ? 0xe401ca : 0xe801ce;
		if (issword(units[arunit].hand))
			units[arunit].hand = randn(100) < 50 ? 0xe401ca : randn(100) < 75 ? 0xef01d5 : 0xe801ce;
	}
	if (game.defeated >= 3 && getbit(game.flags, 0x4bb)) {
		if (randn(100) < 30)
			units[arunit].offhand = 0xf201a7;
		else if (randn(100) < 20)
			units[arunit].offhand = 0xf301a8;
	}
	if (game.defeated < 2)
		units[arunit].courage = 1;
	varygear(&units[arunit], 0xe0000000);
	calcstats(&units[arunit]);
	setunitmode(&units[arunit], 2);
	turnto(&units[arunit], playerx, playery);
	return arunit;
}

short
addroman(short x, short y, unsigned char kind)
{
	romunit = newunit(kind, 0);
	units[romunit].courage = 3;
	units[romunit].sympathy = 1;
	units[romunit].scene = 0x13aa;
	units[romunit].plea = 0;
	units[romunit].mercy = 0;
	units[romunit].x = x;
	units[romunit].clan = 1;
	units[romunit].y = y;
	varygear(&units[romunit], 0x60000000);
	calcstats(&units[romunit]);
	setunitmode(&units[romunit], 2);
	turnto(&units[romunit], playerx, playery);
	return romunit;
}

short
addavenger(short x, short y, short v)
{
	// FIXED: the avenger stands near the hero wherever travel scatters it. Search near x and y, which travel keeps on the map.
#if defined(FEATURE_FIXES)
	if (!freenear(x, y, &x, &y))
#else
	if (!freenear(playerx, playery, &x, &y))
#endif
		return 0;
	avunit = newunit(randn(21), 0);
	units[avunit].courage = 3;
	units[avunit].sympathy = 1;
	units[avunit].scene = v;
	units[avunit].plea = 0;
	units[avunit].mercy = 0;
	units[avunit].x = x;
	units[avunit].clan = 1;
	units[avunit].y = y;
	units[avunit].type = 0x1d;
	units[avunit].hand = 0xef01d5;
	setbless(&units[avunit].hand, 0xe);
	units[avunit].belt = 0xe901cf;
	setbless(&units[avunit].belt, 10);
	units[avunit].offhand = randn(100) < 50 ? 0xf201a7 : 0xf301a8;
	varygear(&units[avunit], 0x60000000);
	calcstats(&units[avunit]);
	setunitmode(&units[avunit], 2);
	turnto(&units[avunit], playerx, playery);
	return avunit;
}

// PORT: the setup events of a replay (src/sys/replay.c), which put the hero in states that play reaches rarely: an item cursed or blessed, wounds, hunger, fear, avengers beside him, the speaker of a scene brought to him, and a door that the replay puts on a cell and opens as a lever does
#ifdef PORT
unsigned long *unitslot(char c, Unit *u);
extern int doors[2][16];	/* world.cpp: two rows of four doors, each its closed1, open1, closed2, and open2 */

/* replaysetup takes a replay's setup event: what names it, and a and b hold its arguments */
void
replaysetup(int what, int a, int b)
{
	unsigned long *slot;
	short x, y;
	int k, pos;

	switch (what) {
	case 'b':
		slot = unitslot((char)a, units);
		if (slot != NULL && *slot != 0)
			setbless(slot, b);
		break;
	case 'h':
		units[0].health = a;
		break;
	case 'u':
		game.hungercount = a;
		break;
	case 'f':
		units[0].fleeing = a;
		break;
	case 'a':
		for (k = 0; k < b; k++)
			addavenger(playerx, playery, a);
		break;
	case 's':
		for (k = 1; k < nunits; k++)
			if (units[k].scene == a && freenear(playerx, playery - 1, &x, &y)) {
				units[k].x = x;
				units[k].y = y;
				units[k].nextx = x;
				units[k].nexty = y;
				setunitmode(&units[k], 1);
				turnto(&units[k], playerx, playery);
				break;
			}
		break;
	case 'd':
		/* the first door of the row whose halves lie side by side, closed1 left of closed2 */
		pos = b * 150 + a;
		additem(pos - 1, doors[0][0]);
		additem(pos, doors[0][2]);
		updatecell(pos - 1);
		updatecell(pos);
		opendoor(pos, 1);
		break;
	}
}
#endif

short
addrefugee(short x, short y, short kind, short v)
{
	refunit = newunit(kind, 0);
	units[refunit].courage = 3;
	units[refunit].scene = v;
	units[refunit].sympathy = 3;
	units[refunit].clan = 0xff;
	units[refunit].plea = 0;
	units[refunit].mercy = 0;
	units[refunit].x = x;
	units[refunit].y = y;
	units[refunit].offhand = kind == 8 ? 0xf201a7 : 0xf301a8;
	if (kind == 7)
		units[refunit].hand = 0xe901cf;
	else {
		units[refunit].hand = 0xef01d5;
		units[refunit].belt = 0xe901cf;
	}
	varygear(&units[refunit], kind == 7 ? 0x60000000 : 0x80000000);
	calcstats(&units[refunit]);
	setunitmode(&units[refunit], 0);
	turnto(&units[refunit], 0x49, 0x4f);
	return refunit;
}

void
delunit(short n)
{
	if (n != 0) {
		for (dui = 1; dui < nunits; dui++)
			if (units[dui].leader == n)
				units[dui].leader = 0;
		memset(&units[n], 0, sizeof(Unit));
		if (n == nunits - 1) {
			nunits--;
			for (dui = nunits - 1; dui > 0; dui--) {
				if (units[dui].health > 0 || units[dui].x != 0 || units[dui].y != 0 || units[dui].modeb != 0)
					break;
				nunits--;
			}
		}
		markunits();
	}
}

short
flanked(Unit *u)
{
	if (badunit(u))
		return 0;
	if ((u->mode == 0x14 || u->mode == 0x15 || u->mode == 0x19) && u->modeb != 0xff && u->modec != 0xff
	    && abs(units[u->modeb].x - u->x) <= 1 && abs(units[u->modeb].y - u->y) <= 1
	    && abs(units[u->modec].x - u->x) <= 1 && abs(units[u->modec].y - u->y) <= 1)
		return 1;
	return 0;
}

short
standing(Unit *u)
{
	if (badunit(u))
		return 0;
	standsum = 0;
	if (u->type == 0x2a || u->type == 0x2b)
		standsum--;
	if (isfemale(u))
		standsum--;
	if (isservant(u))
		standsum--;
	standself = indexof(u);
	if (standself)
		for (standi = 1; standi < nunits; standi++)
			if (units[standi].health > 0 && units[standi].lord == standself && !isanimal(&units[standi]))
				standsum++;
	return standsum;
}

void
settarget(Unit *u, unsigned char t)
{
	if (badunit(u))
		return;
	if (u->mode == 0x19 || u->mode == 0x14 || u->mode == 0x15)
		if (t != u->modeb) {
			u->modec = u->modeb;
			u->modeb = t;
		}
}

void
sorttargets(Unit *u)
{
	if (badunit(u))
		return;
	if (u->mode == 0x19 || u->mode == 0x14 || u->mode == 0x15) {
		if (u->modeb == 0xff)
			tgdist = 0xfff;
		else
			tgdist = celldist(u->x, u->y, units[u->modeb].x, units[u->modeb].y);
		if (u->modec == 0xff)
			tgdist2 = 0xfff;
		else
			tgdist2 = celldist(u->x, u->y, units[u->modec].x, units[u->modec].y);
		if (tgdist2 < tgdist) {
			tgunit = u->modeb;
			u->modeb = u->modec;
			tgtmp = tgdist;
			tgdist = tgdist2;
			tgdist2 = tgtmp;
			u->modec = tgunit;
		}
		if (u->sympathy <= 0 && u->modeb != 0 && u->modec != 0 && (!u->bewitched || u->bewitcher != 0)) {
			tgplayer = celldist(u->x, u->y, units[0].x, units[0].y);
			if (tgplayer < tgdist) {
				u->modec = u->modeb;
				u->modeb = 0;
			} else if (tgplayer < tgdist2)
				u->modec = 0;
		}
	}
}

void
armsound(Unit *u)
{
	if (badunit(u))
		return;
	if (!inview(u->x, u->y))
		return;
	if (issword(u->hand))
		queuesound(10);
	else
		queuesound(12);
}

short
induel(Unit *a, Unit *b)
{
	if (badunit(a) || badunit(b))
		return 0;
	if (b->mode == 0x14 && b->modea == 1 && b->modeb == indexof(a))
		return 1;
	if (a->mode == 0x14 && a->modea == 1 && a->modeb == indexof(b))
		return 1;
	return 0;
}

short
anger(Unit *a, Unit *b, signed char dmg, short d, short e)
{
	if (badunit(a))
		return 0;
	if (badunit(b) || b->health <= 0 || b->asleep)
		return 0;
	if (a == b)
		return 0;
	if (isservant(b) && &units[b->lord] == a)
		return 0;
	if (b == &units[0] && playmode != 3 && !lostcontrol && !knockedout) {
		combatcause = 2;
		togglecombat(0);
	}
	angera = indexof(a);
	angerb = indexof(b);
	if (b->bewitched && b->bewitcher == angera)
		return 0;
	if (forgives(curmap, angerb, angera, dmg, d, e))
		return 0;
	if (angera == 0) {
		if (b->sympathy <= 0)
			b->talkflags &= 0xf9;
		else if (b->turnmarks & 2)
			return 0;
		b->turnmarks |= 2;
		if (b->sympathy > 0) {
			b->sympathy -= dmg;
			if (b->sympathy <= 0) {
				b->sympathy = 0;
				b->talkflags &= 0xf9;
			}
			if (!talkactive && !trademode && cansee(b->x, b->y) && !isanimal(b)) {
				if (b->sympathy > 0) {
					msgf(standing(b) + 20, strs[1409], unitname(b));
					return 0;
				}
				msgf(standing(b) + 30, strs[1407], unitname(b));
			}
		}
	}
	if (b->mode == 0x19 || b->mode == 0x14 || b->mode == 0x15) {
		if (b->modeb != (unsigned char)angera && b->modec != (unsigned char)angera) {
			if (b->modeb != 0xff) {
				if (b->modec == 0xff)
					b->modec = angera;
				else if (flanked(b)) {
					if (abs(a->x - b->x) <= 1 && abs(a->y - b->y) <= 1 && !isfemale(a)) {
						if (isfemale(&units[b->modeb])) {
							b->modeb = angera;
							return 1;
						}
						if (isfemale(&units[b->modec])) {
							b->modec = angera;
							return 1;
						}
					}
				} else {
					if (unitdist(b, &units[b->modec]) < unitdist(b, &units[b->modeb]))
						b->modeb = b->modec;
					if (unitdist(b, a) < unitdist(b, &units[b->modeb])) {
						b->modeb = angera;
						return 1;
					}
					if (unitdist(b, a) < unitdist(b, &units[b->modec])) {
						b->modec = angera;
						return 1;
					}
				}
			} else
				b->modeb = angera;
		}
		return 1;
	}
	if (a->sympathy <= 0 && !talkactive && !trademode && cansee(b->x, b->y) && !isanimal(b))
		msgf(standing(b) + 25, strs[1408], unitname(b), unitname(a));
	setunitmode(b, 0x15);
	b->modea = 0;
	b->modeb = angera;
	b->modec = 0xff;
	armsound(b);
	return 1;
}

void
provoke(Unit *a, Unit *b, signed char dmg, short d, short e)
{
	if (badunit(a) || badunit(b))
		return;
	prova = indexof(a);
	provb = indexof(b);
	if (sparring(a, b))
		return;
	if (induel(a, b)) {
		if (b == &units[0])
			a->sympathy = 0;
		else if (a == &units[0])
			b->sympathy = 0;
		return;
	}
	if (!anger(a, b, dmg, d, e))
		return;
	if ((b->mode == 0x14 || b->mode == 0x15) && b->modea == 2)
		return;
	prova = indexof(a);
	provb = indexof(b);
	for (provi = 1; provi < nunits; provi++) {
		if (units[provi].health <= 0 || prova == provi || provi == provb)
			continue;
		if (provb == 0 && (units[provi].sympathy >= 3 || units[provi].clan == 0xff) && !isanimal(&units[provi])
		    && (!isfemale(&units[provi]) || sameprov(&units[provi], &units[0])
		    || units[provi].sympathy > 3 && cansee(units[provi].x, units[provi].y))
		    && celldist(b->x, b->y, units[provi].x, units[provi].y) <= 25) {
			if (allied(&units[provi], a)) {
				if (!isservant(&units[provi]) && units[provi].sympathy < 4)
					continue;
				units[provi].clan = 0;
				units[provi].home = 0;
				if (units[provi].lord && units[units[provi].lord].sympathy <= 0)
					units[provi].lord = 0;
			}
		} else {
			if (allied(&units[provi], a) || sameprov(&units[provi], a))
				continue;
			if (b->lord && b->lord != 0xff && (b->lord == provi || allied(&units[b->lord], &units[provi]))
			    || allied(b, &units[provi])) {
				if (celldist(b->x, b->y, units[provi].x, units[provi].y) > 25 && b->spouse != provi)
					continue;
				if (prova == 0 || units[prova].sympathy >= 3) {
					if (units[provi].sympathy >= 4 || units[provi].traits & 0x20)
						continue;
					if (isservant(&units[provi]) && units[provi].sympathy >= 3)
						continue;
				}
			} else {
				if (prova != 0)
					continue;
				if ((units[provi].sympathy >= 3 || myservant(&units[provi])) && !isanimal(&units[provi]) && !isfemale(&units[provi])
				    && celldist(a->x, a->y, units[provi].x, units[provi].y) <= 25)
					anger(b, &units[provi], dmg, 3, prova);
				continue;
			}
		}
		anger(a, &units[provi], dmg, 3, provb);
	}
}

short
notepoison(short n)
{
	if (units[n].poison && units[n].poison + units[n].bleeding >= units[n].health || units[n].sympathy < 2)
		wasdoomed = 1;
	else
		wasdoomed = 0;
	return wasdoomed;
}

int
checkpoison(short n)
{
	if (n != 0 && wasdoomed == 0 && units[n].poison && units[n].poison + units[n].bleeding >= units[n].health) {
		deedpoison();
		return 1;
	}
	return 0;
}

int
packordrop(int item)
{
	item &= ~0x1000;
	if (istorch(item))
		clamptorch((unsigned long *)&item);
	if (stow(0, item))
		return 1;
	additem(playery * 150 + playerx, item);
	return 1;
}

int
packitem(int item)
{
	item &= ~0x1000;
	if (nofit(item, 0))
		return 0;
	if (istorch(item))
		clamptorch((unsigned long *)&item);
	if (stow(0, item))
		return 1;
	return 0;
}

int
packunit(short n, unsigned long item)
{
	if (n == 0)
		return 0;
	item = baselook(item);
	if (istorch(item))
		clamptorch(&item);
	puclass = classof(item & 0xfff);
	if ((puclass == 15 || puclass == 16) && item >> 16 < 15000)
		for (puslot = 0; puslot < 3; puslot++)
			if (canstack(units[n].items[puslot], item)) {
				addcoins(&units[n].items[puslot], item);
				return 1;
			}
	for (puslot = 0; puslot < 3; puslot++)
		if (units[n].items[puslot] == 0) {
			if (islittorch(item))
				setlit(&item, 0);
			units[n].items[puslot] = item;
			return 1;
		}
	return 0;
}

short
haskey(Unit *u, unsigned long item)
{
	if (badunit(u))
		return 0;
	for (keyslot = 0; keyslot < 3; keyslot++)
		if (u->items[keyslot] && classof(u->items[keyslot] & 0xfff) == 5
		    && idof(u->items[keyslot]) == idof(item))
			return 1;
	return 0;
}

int
stowregion(Unit *u, short n, unsigned long item)
{
	if (u && badunit(u))
		return 0;
	if (istorch(item))
		clamptorch(&item);
	for (srgi = 0; srgi < 8; srgi++) {
		if (regions[n].realms[srgi] == 0)
			continue;
		if (realms[regions[n].realms[srgi]].item & 0x8000000 && u && !haskey(u, realms[regions[n].realms[srgi]].item))
			continue;
		if (realms[regions[n].realms[srgi]].item & 0x10000000)
			continue;
		if (stow(regions[n].realms[srgi], item))
			return 1;
	}
	return 0;
}

int
islovepotion(int item)
{
	if ((item & 0xfff) == 0x18d)
		return 1;
	return 0;
}

int
seducible(Unit *u)
{
	if (badunit(u))
		return 0;
	if (!isfemale(u) || u->sympathy < 3 || spellset(u) == 1)
		return 0;
	if (u->spouse && units[u->spouse].health > 0) {
		sedpart = ((Recipe *)(recipes + 0x20))[units[u->spouse].part].parts[0];
		if (!(units[u->spouse].traits & 0x20) && sedpart != 11 && sedpart != 14 && sedpart != 18
		    && sedpart != 16 && units[u->spouse].type != 0x2a && units[u->spouse].health > 0)
			return 0;
	}
	return 1;
}

unsigned long
giveitem(short n, unsigned long item, short mode)
{
	unsigned long look;

	giorig = item;
again:
	item &= ~0x1000;
	item = randlook(item);
	if (istorch(item))
		clamptorch(&item);
	gival = valueof(item);
	giclass = classof(item & 0xfff);
	if (n == 0)
		plotfind(1, playerx, playery, item);
	if (mode == 2)
		goto done;
	if (isfood(item) && (eatable(item) || islovepotion(item) && seducible(&units[n]))
	    && giclass != 0x82 && giclass != 0x83 && giclass != 0x84 && giclass != 0x85
	    && giclass != 0x86 && n) {
		if (item & 0x10000000 && (units[n].mode == 0x14 || units[n].mode == 0x15 || units[n].sympathy <= 0
		    || units[n].drunk >= 0x59))
			goto slot;
		if (item & 0xc0050000) {
			if (!(item & 0x20000) || learned(&units[n]))
				// PORT: the unit by its index, as 64 bits truncate the address in a DWORD
#ifdef PORT
				if (skillof(&units[n], 0x19, 0, 0, 0) >= 0x5f || skillof(&units[n], 0x15, 0, 0, 0) > 100
				    || skillof(&units[n], 0x18, 0, 0, 0) > 100)
#else
				if (skillof((Unit *)(n * sizeof(Unit) + (DWORD)units), 0x19, 0, 0, 0) >= 0x5f || skillof((Unit *)(n * sizeof(Unit) + (DWORD)units), 0x15, 0, 0, 0) > 100
				    || skillof((Unit *)(n * sizeof(Unit) + (DWORD)units), 0x18, 0, 0, 0) > 100)
#endif
					goto slot;
			if (wary(curmap, n))
				goto slot;
		}
		if (!isfood(giorig) || palblack || trademode || !inview(units[n].x, units[n].y))
			gij = 0;
		else
			gij = 2;
		consume(&units[n], item, gij);
		if (giclass != 0x87 && giclass != 0x88 && giclass != 0x89 && giclass != 0x8a
		    && giclass != 0x8b && giclass != 0x38)
			goto out;
		emptyvessel(&item);
		goto again;
	}
slot:
	if (giclass == 0x3c) {
		if (!isservant(&units[n]) || units[n].mode == 0x14 || units[n].mode == 0x15) {
			if (isthrowing2(item)) {
				if (n == 0 || mode == 3 || !isspear(item)) {
					if (!isthrowing(item) && isthrowing(units[n].hand))
						goto fail;
					if (ismelee(units[n].hand))
						goto ok;
					if (valueof(units[n].hand) < gival)
						goto ok;
				} else {
					if (!isspear(units[n].hand))
						goto fail;
					if (valueof(units[n].hand) < gival)
						goto ok;
				}
			} else {
				if (units[n].hand == 0)
					goto ok;
				if (ismelee(units[n].hand) && valueof(units[n].hand) < gival)
					goto ok;
				if (!isthrowing2(units[n].hand))
					goto fail;
				if (units[n].belt == 0)
					goto ok;
				if (valueof(units[n].belt) < gival)
					goto ok;
			}
		}
	} else if (giclass == 0x41) {
		if (n == 0 || mode == 3 || units[n].head || isspear(units[n].hand)
		    || classof(units[n].offhand & 0xfff) == 0x55)
			if (valueof(units[n].head) < gival)
				goto ok;
	} else if (giclass == 0x46) {
		if ((n == 0 || mode == 3 || isspear(units[n].hand) || classof(units[n].offhand & 0xfff) == 0x55
		    || units[n].head) && (baselook(item) & 0xfff) != 0x1b5
		    || isfemale(&units[n]) && (baselook(item) & 0xfff) == 0x1b5
		    || !isfemale(&units[n]) && (baselook(item) & 0xfff) == 0x1ad) {
			look = units[n].body;
			if (valueof(look) >= gival)
				goto fail;
			gilook = baselook(units[n].body) & 0xfff;
			if ((baselook(item) & 0xfff) == gilook)
				goto ok;
			if (gilook == 0x1b0 || gilook == 0x1b1 || gilook == 0x1b2 || gilook == 0x1b4)
				goto fail;
			goto ok;
		}
	} else if (giclass == 0x48) {
		if (issmith(&units[n]) || istrader(&units[n]) && units[n].cloak == 0)
			goto fail;
		if (units[n].cloak || isservant(&units[n])) {
			if (valueof(units[n].cloak) >= gival)
				goto fail;
			gilook = baselook(units[n].cloak) & 0xfff;
			if ((baselook(item) & 0xfff) != gilook)
				goto fail;
		}
		goto ok;
	} else if (giclass == 0x63) {
		if (valueof(units[n].wrist) < gival)
			goto ok;
	} else if (giclass == 0x62) {
		if (valueof(units[n].neck) < gival)
			goto ok;
	} else if (giclass == 0x4b) {
		if (!isfemale(&units[n]) && valueof(units[n].legs) < gival)
			goto ok;
	} else if (giclass == 0x50) {
		if (((Recipe *)(recipes + 0x20))[units[n].part].parts[0] != 0x44 && valueof(units[n].feet) < gival)
			goto ok;
	} else if (giclass == 0x55) {
		if ((n == 0 && !islittorch(units[0].offhand) || units[n].offhand && classof(units[n].offhand & 0xfff) == 0x55
		    || mode == 3) && valueof(units[n].offhand) < gival)
			goto ok;
	} else if (giclass == 0x56) {
		if (!istorch(units[n].offhand) && valueof(units[n].offhand) < gival)
			goto ok;
	}
	goto fail;
ok:
	gitmp = puton(n, item);
	if (gitmp == 0)
		goto out;
	if (gitmp == 999)
		goto fail;
	item = gitmp;
	goto again;
fail:
	if (!n)
		goto player;
	if (packunit(n, item))
		goto out;
	for (gii = 0; gii < 4; gii++) {
	swap:
		gitmp = baselook(item);
		gival = valueof(item);
		for (gij = 0; gij < 3; gij++)
			if (valueof(units[n].items[gij]) < gival
			    && classof(units[n].items[gij] & 0xfff) != 5) {
				item = units[n].items[gij];
				units[n].items[gij] = gitmp;
				goto swap;
			}
	}
done:
	if (item == 0)
		goto out;
	if (!mode) {
		calcstats(&units[n]);
		return item;
	}
	item = randlook(item);
	if (n) {
		gihome = athome(&units[n], 1);
		gij = ownerat(units[n].x, units[n].y);
		if (!myservant(&units[n]) && (gihome || n == gij
		    || gij && allied(&units[n], &units[gij])))
			item |= 0x1000;
		if (!gihome) {
			gihome = homeof(&units[n]);
			item |= 0x1000;
		}
		if (gihome) {
			if (isservant(&units[n]))
				gihome = regions[gihome].store;
			if (stowregion(&units[n], gihome, item))
				goto out;
		}
	}
	if (mode == 2)
		return item;
	giclass = classof(item & 0xfff);
	if ((giclass == 15 || giclass == 16) && item >> 16 < 15000
	    && (gitop = topitem(units[n].y * 150 + units[n].x)) != 0
	    && canstack(mapitems[gitop], item)
	    && !((item & 0x1000) != 0 && (mapitems[gitop] & 0x1000) == 0)
	    && !((item & 0x1000) == 0 && (mapitems[gitop] & 0x1000) != 0)) {
		addcoins(&mapitems[gitop], item);
		mapitems[gitop] &= ~0x1000;
		mapitems[gitop] |= item & 0x1000;
		goto out;
	}
	if (units[n].health > 0) {
		if (units[n].mode == 0x14 || units[n].mode == 0x19)
			item &= ~0x1000;
		additem(units[n].y * 150 + units[n].x, item);
	} else {
		gitmp = corpseof(n);
		if (gitmp == 0)
			goto out;
		gitmp = findcell(gitmp);
		if (gitmp == 0)
			goto out;
		additem(gitmp, item & ~0x1000);
	}
out:
	calcstats(&units[n]);
	return 0;
player:
	if (packitem(item))
		goto out;
	gimin = valueof(item);
	gislot = 0xff;
	for (gii = 0; gii < 50; gii++) {
		gival = valueof(realms[0].items[gii]);
		if (gival < gimin) {
			gimin = gival;
			gislot = gii;
		}
	}
	if (gislot != 0xff) {
		gitmp = item;
		item = takeout(0, gislot);
		if (!packitem(gitmp)) {
			if (packitem(item))
				item = gitmp;
			else {
				additem(playery * 150 + playerx, item);
				item = gitmp;
			}
		}
	}
	goto done;
}

short
yields(Unit *u, short k, unsigned long item, short m)
{
	if (badunit(u) || !item)
		return 0;
	switch (classof(item & 0xfff)) {
	case 0:
	case 0xf:
	case 0x10:
	case 0x3c:
	case 0x41:
	case 0x48:
	case 0x55:
	case 0x64:
		return 1;
	case 0x62:
	case 0x63:
		if (k == 2) {
			if (magicof(item) > 0 && u->type == 0x19)
				return 0;
			if (item & 0x100000 && spellset(u) == 1)
				return 0;
			if (item & 0x70000 && !isfemale(u))
				return 0;
		}
		return 1;
	case 0x4b:
	case 0x50:
		if (k == 1)
			return 0;
		return 1;
	case 0x46:
		if (k == 1 && !isarmor(item))
			return 0;
	case 0x38:
	case 0x39:
		return 1;
	case 0x36:
	case 0x37:
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
		if (m)
			return 1;
		if (u == units || k == 2)
			return 0;
		return 1;
	case 5:
		if (u == units)
			return 0;
		if (k == 2 && !myservant(u))
			return 0;
		return 1;
	default:
		if (u == units)
			return 0;
		return 1;
	}
}

int
lordsitem(unsigned long item, int k)
{
	switch (classof(item & 0xfff)) {
	case 5:
	case 0x3c:
	case 0x41:
	case 0x46:
	case 0x4b:
	case 0x50:
	case 0x55:
		return 1;
	case 0xf:
	case 0x10:
	case 0x38:
	case 0x39:
	case 0x62:
	case 0x63:
	case 0x64:
		if (!k)
			return 1;
	}
	return 0;
}

void
handover(Unit *u, Unit *v)
{
	short i;

	if (badunit(u) || v && badunit(v))
		return;
	if (yields(u, 1, u->body, 1)) {
		if (v) {
			giveitem(indexof(v), u->body, 1);
			if (v == units)
				deedbooty(u->body);
		}
		u->body = 0;
	}
	if (yields(u, 1, u->hand, 1)) {
		if (v) {
			giveitem(indexof(v), u->hand, 1);
			if (v == units)
				deedbooty(u->hand);
		}
		u->hand = 0;
	}
	if (yields(u, 1, u->belt, 1)) {
		if (v) {
			giveitem(indexof(v), u->belt, 1);
			if (v == units)
				deedbooty(u->belt);
		}
		u->belt = 0;
	}
	if (yields(u, 1, u->offhand, 1)) {
		if (v) {
			giveitem(indexof(v), u->offhand, 1);
			if (v == units)
				deedbooty(u->offhand);
		}
		u->offhand = 0;
	}
	for (handi = 0; handi < 3; handi++)
		if (yields(u, 0, u->items[handi], 1)) {
			if (v) {
				giveitem(indexof(v), u->items[handi], 1);
				if (v == units)
					deedbooty(u->items[handi]);
			}
			u->items[handi] = 0;
		}
	if (yields(u, 1, u->head, 1)) {
		if (v) {
			giveitem(indexof(v), u->head, 1);
			if (v == units)
				deedbooty(u->head);
		}
		u->head = 0;
	}
	if (yields(u, 1, u->cloak, 1)) {
		if (v) {
			giveitem(indexof(v), u->cloak, 1);
			if (v == units)
				deedbooty(u->cloak);
		}
		u->cloak = 0;
	}
	if (yields(u, 1, u->neck, 1)) {
		if (v) {
			giveitem(indexof(v), u->neck, 1);
			if (v == units)
				deedbooty(u->neck);
		}
		u->neck = 0;
	}
	if (yields(u, 1, u->wrist, 1)) {
		if (v) {
			giveitem(indexof(v), u->wrist, 1);
			if (v == units)
				deedbooty(u->wrist);
		}
		u->wrist = 0;
	}
	if (u == units)
		for (i = 0; i < 50; i++)
			if (yields(u, 0, realms[0].items[i], v == 0 ? 1 : 0)) {
				if (v)
					giveitem(indexof(v), realms[0].items[i], 1);
				takeout(0, i);
				i--;
			}
	if (v) {
		calcload(indexof(v));
		calcstats(v);
	}
	calcload(indexof(u));
	calcstats(u);
	if (u == units)
		dresshero();
}

void
takehours(Unit *u, Unit *v)
{
	if (badunit(u) || badunit(v))
		return;
	u->traits &= ~4;
	thx = u->sched[3][22];
	thy = u->sched[4][22];
	for (thhour = 0; thhour < 24; thhour++) {
		u->sched[0][thhour] = v->sched[0][thhour];
		u->sched[1][thhour] = v->sched[1][thhour];
		u->sched[2][thhour] = v->sched[2][thhour];
		if (v->sched[0][thhour] == 0x5a) {
			u->sched[5][thhour] = u->sched[7][thhour] = u->sched[3][thhour] = thx;
			u->sched[6][thhour] = u->sched[8][thhour] = u->sched[4][thhour] = thy;
		} else {
			u->sched[3][thhour] = v->sched[3][thhour];
			u->sched[4][thhour] = v->sched[4][thhour];
			u->sched[5][thhour] = v->sched[5][thhour];
			u->sched[6][thhour] = v->sched[6][thhour];
			u->sched[7][thhour] = v->sched[7][thhour];
			u->sched[8][thhour] = v->sched[8][thhour];
		}
	}
}

void
orphan(Unit *u)
{
	if (badunit(u))
		return;
	u->leader = 0;
	if (u->traits & 4)
		return;
	if (regionat(u->x, u->y) == regionat(u->sched[3][22], u->sched[4][22])) {
		for (orphhour = 6; orphhour <= 20; orphhour++) {
			u->sched[0][orphhour] = 2;
			u->sched[1][orphhour] = u->sched[1][22];
			u->sched[2][orphhour] = u->sched[2][22];
			u->sched[5][orphhour] = u->sched[7][orphhour] = u->sched[3][orphhour] = u->sched[3][22];
			u->sched[6][orphhour] = u->sched[8][orphhour] = u->sched[4][orphhour] = u->sched[4][22];
		}
		u->sched[0][20] = u->sched[0][22];
		return;
	}
	for (orphhour = 0; orphhour < 24; orphhour++)
		if (u->sched[0][orphhour] == 0x5a) {
			u->sched[5][orphhour] = u->sched[7][orphhour] = u->sched[3][orphhour] = u->x;
			u->sched[6][orphhour] = u->sched[8][orphhour] = u->sched[4][orphhour] = u->y;
		} else {
			u->sched[5][orphhour] = 0;
			u->sched[6][orphhour] = 0;
			u->sched[7][orphhour] = 0x95;
			u->sched[8][orphhour] = 0x95;
		}
	u->lord = 0;
}

short
substitute(Unit *u)
{
	if (badunit(u))
		return 0;
	subself = indexof(u);
	if (subself == 0)
		return 0;
	for (subpass = 0; subpass < 2; subpass++)
		for (subi = 1; subi < nunits; subi++)
			if (subi != subself && units[subi].health > 0 && samehouse(u, &units[subi])
			    && isservant(&units[subi])) {
				if (isfemale(&units[subi]) && subpass < 1)
					continue;
				goto found;
			}
	for (subi = 1; subi < nunits; subi++)
		if (units[subi].leader == subself)
			orphan(&units[subi]);
	return 0;
found:
	subunit = subi;
	takehours(&units[subunit], u);
	units[subunit].holdtime = 0;
	units[subunit].flags |= 0x10;
	for (subi = 1; subi < nunits; subi++)
		if (units[subi].leader == subself)
			units[subi].leader = subunit;
	flagunit(&units[subunit]);
	return subunit;
}

void
killunit(Unit *u, short a, short b, short c, short d)
{
	if (badunit(u))
		return;
	if (u->health <= 0 && u->corpse == 0xff)
		return;
	if (a)
		addblood(u->x, u->y, 1, 0);
	if (c == 0)
		killplayer = 1;
	else
		killplayer = 0;
	killv = indexof(u);
	if (a == 0)
		plotdeath(curmap, killv, 0xff, c);
	else
		plotdeath(curmap, killv, b, c);
	if (u->sympathy < 2)
		killfoe = 1;
	else
		killfoe = 0;
	u->leader = 0;
	killled = 0;
	killfair = fairgame(curmap, indexof(u)) ? 1 : 0;
	for (killi = 1; killi < nunits; killi++) {
		if (killplayer && allied(u, &units[killi]) && !isservant(&units[killi]) && !killfair
		    && units[killi].sympathy > 0 && units[killi].sympathy != 4)
			units[killi].sympathy = 1;
		if (units[killi].leader == killv)
			killled = 1;
	}
	if (killled)
		substitute(u);
	u->health = 0;
	if (u != units)
		abdicate(u, d ? -d : 0);
	u->mercy = u->home;
	u->home = 0;
	setunitmode(u, 0);
	u->pose = 0;
	u->asleep = 0;
	u->traits |= 4;
	if (u != units && islittorch(u->offhand))
		setlit(&u->offhand, 0);
	clearmind(u);
	u->plea = gameday;
	u->modeb = 0xfe;
	u->modec = 0xfe;
	u->moded = 0xfe;
	killcell = u->x + u->y * 150;
	u->corpse = 0xff;
	if (u == units) {
		playerx = u->x;
		playery = u->y;
		playercell = playerx + playery * 150;
		trackregion();
	} else {
		u->x = 0;
		u->y = 0;
	}
	if (u != units && u->traits & 8)
		u->traits = 0xff;
	else if (a && partof(u)->text) {
		killv = corpseitem(u);
		if (killv & 0xfff) {
			if (classof(u->offhand & 0xfff) == 0x55) {
				if (killfoe)
					additem(killcell, randlook(u->offhand) & ~0x1000);
				else
					additem(killcell, randlook(u->offhand));
				if (u != units)
					u->offhand = 0;
			}
			if ((DWORD)random() < 0x3fff)
				killv += 0x34;
			additem(killcell, killv);
			if (u->hand) {
				if (killfoe)
					additem(killcell, randlook(u->hand) & ~0x1000);
				else
					additem(killcell, randlook(u->hand));
				if (u != units)
					u->hand = 0;
			}
			lookstale = 1;
		}
	}
	markunits();
	if (u == units) {
		syncplayer();
		setcurregion();
		lookaround();
		lookdone();
		fillgauges();
	}
}

int
playerdead(void)
{
	if (units[0].health > 0 || units[0].corpse != 0xff)
		return 0;
	return 1;
}

short
decayed(int k)
{
	switch (k) {
	case 0x825:
	case 0x827:
	case 0x82c:
	case 0x840:
	case 0x841:
	case 0x843:
	case 0x846:
	case 0x84b:
	case 0xa28:
	case 0xa29:
	case 0xa2a:
		return 1;
	}
	return 0;
}

int
bonesof(unsigned long k)
{
	if (k == 0x843 || k == 0x840 || k == 0x846)
		return 0xa28;
	if (k == 0x827)
		return 0xa29;
	if (k == 0x841)
		return 0xa2a;
	return 0;
}

int
delflies(int pos, unsigned long k)
{
	do {
		if ((mapitems[k] & 0xfff) == 0x5e7) {
			if (pos == -1)
				pos = findcell(k);
			delitem(pos, k);
			return 1;
		}
	} while (mapitems[k++] & 0x8000);
	return 0;
}

void
decaycorpses(void)
{
	if (((unsigned char *)recipes)[0x2de] && part1 != 0xff && part8 != 0xff) {
		for (dcy = 5; dcy <= 144; dcy++)
			for (dcx = 6; dcx <= 143; dcx++) {
				dccell = dcx + dcy * 150;
				dcobj = findclass(dccell, 0x15);
				if (!dcobj)
					continue;
				dcword = mapitems[dcobj];
				dclook = dcword & 0xfff;
				if (!(dclook >= 0x820 && dclook <= 0x853)) {
					if (dclook < 0x854 || dclook > 0x887)
						continue;
					dclook -= 0x34;
				}
				if (decayed(dclook))
					continue;
				dcword >>= 16;
				if (dcword == 0 || dcword >= nunits)
					continue;
				if (dclook == 0x84c) {
					if (gameday - units[dcword].plea < 14)
						continue;
					takeitem(dccell, dcobj);
					units[dcword].picset = part1 + 15;
					units[dcword].part = part1;
					units[dcword].type = 1;
				} else {
					if (gameday - units[dcword].plea < 3)
						continue;
					takeitem(dccell, dcobj);
					units[dcword].picset = part8 + 15;
					units[dcword].part = part8;
					units[dcword].type = 8;
				}
				units[dcword].health = kindof(&units[dcword])->maxhealth;
				units[dcword].plea = 0;
				units[dcword].mercy = 0;
				units[dcword].sympathy = 0;
				units[dcword].x = dcx;
				units[dcword].y = dcy;
				setunitmode(&units[dcword], 0x14);
				units[dcword].corpse = 0;
			}
		markunits();
	}
	for (dcobj = 0; dcobj < nmapitems; dcobj++) {
		dcword = mapitems[dcobj];
		dclook = dcword & 0xfff;
		if (classof(dclook) != 0x15)
			continue;
		dcword >>= 16;
		if (dcword >= nunits)
			continue;
		if (dcword == 0 && ((*(unsigned long *)(mapstate + 0x2b)) == 0 || (*(unsigned long *)(mapstate + 0x2b)) >= gametime))
			continue;
		if (dclook >= 0x820 && dclook <= 0x853)
			dcalt = 0;
		else if (dclook >= 0x854 && dclook <= 0x887) {
			dclook -= 0x34;
			dcalt = 0x34;
		} else
			continue;
		if (dclook == 0x825 || dclook == 0xa28 || dclook == 0xa29 || dclook == 0xa2a
		    || dclook == 0x82c || dclook == 0x84b)
			continue;
		if (dcword == 0) {
			unsigned long t = gametime - (*(unsigned long *)(mapstate + 0x2b));
			if (t >= hourticks * 0x180)
				dclook = 0x825;
			else if (t >= hourticks * 0x78) {
				dcbones = bonesof(dclook);
				if (dcbones)
					dclook = dcbones;
				else
					dclook = 0x84c;
			} else
				continue;
		} else if (gameday - units[dcword].plea >= 14) {
			if (units[dcword].head && armors[gearof(units[dcword].head)].material == 0)
				setwear(&units[dcword].head, 7);
			if (units[dcword].cloak && armors[gearof(units[dcword].cloak)].material == 0)
				setwear(&units[dcword].cloak, 7);
			if (units[dcword].body && armors[gearof(units[dcword].body)].material == 0)
				setwear(&units[dcword].body, 7);
			if (units[dcword].legs && armors[gearof(units[dcword].legs)].material == 0)
				setwear(&units[dcword].legs, 7);
			if (units[dcword].offhand && classof(units[dcword].offhand & 0xfff) == 0x55
			    && armors[gearof(units[dcword].offhand)].material != 2)
				setwear(&units[dcword].offhand, 7);
			if (units[dcword].feet && armors[gearof(units[dcword].feet)].material == 0)
				setwear(&units[dcword].feet, 7);
			dclook = corpseitem(&units[dcword]);
		} else if (gameday - units[dcword].plea < 3)
			continue;
		else {
			dcbones = bonesof(dclook);
			if (dcbones)
				dclook = dcbones;
			else
				dclook = corpseitem(&units[dcword]);
		}
		mapitems[dcobj] &= 0xfffff000;
		mapitems[dcobj] |= dclook + dcalt;
		delflies(-1, dcobj);
	}
}

void
offguard(Unit *u, short d, unsigned char c)
{
	if (badunit(u))
		return;
	if (!d)
		c = 0x32;
	for (ofhour = 0; ofhour < 24; ofhour++)
		if (u->sched[0][ofhour] == 4 || u->sched[0][ofhour] == 0x32)
			u->sched[0][ofhour] = c;
	if (u->mode == 4 || u->mode == 0x32) {
		u->modec = 0;
		setunitmode(u, c);
	}
}

short
isguard(Unit *u)
{
	if (badunit(u))
		return 0;
	if (u->mode == 4)
		return 1;
	for (ighour = 0; ighour < 24; ighour++)
		if (u->sched[0][ighour] == 4)
			return 1;
	return 0;
}

void
reguard(void)
{
	for (rgi = 1; rgi < nunits; rgi++) {
		if (units[rgi].mode == 0x32)
			units[rgi].mode = 4;
		for (rghour = 0; rghour < 24; rghour++)
			if (units[rgi].sched[0][rghour] == 0x32)
				units[rgi].sched[0][rghour] = 4;
	}
}

int
consume(Unit *u, unsigned long item, short v)
{
	if (badunit(u))
		return 0;
	cnfill = 0;
	cnmsg = 0;
	cnclass = classof(item & 0xfff);
	if (cnclass == 0x36) {
		item = fishfood(item);
		cnclass = classof(item & 0xfff);
	}
	switch (cnclass) {
	case 0x82:
	case 0x83:
	case 0x84:
	case 0x86:
		cndrink = 1;
		break;
	case 0x38:
	case 0x87:
	case 0x88:
	case 0x89:
	case 0x8b:
		cndrink = 2;
		break;
	case 0x85:
	case 0x8a:
	default:
		cndrink = 0;
		break;
	}
	if (u == units)
		newmsgs = 0;
	cnfill = satiety(item) * 10;
	if (!cndrink && !(item & 0x10000000) && u == units && game.hungerlevel == 0 && cnfill) {
		switch (v) {
		case 0:
		case 3:
			return 0;
		case 1:
			break;
		}
		if (!cheat || !(keys['W'] & 0x80)) {
			denymsg(strs[318]);
			return 0;
		}
	}
	if (v && !trademode && !palblack) {
		if (cndrink)
			queuesound(6);
		else
			queuesound(5);
		if (u == units)
			animhead(1);
	}
	if (u == units) {
		game.hungercount -= cnfill;
		if (item & 0x10000000) {
			if (cndrink)
				tire(0x2d);
			else
				tire(0xb4);
		}
		setlevels();
		if (item & 0x10000000) {
			if (v && v != 3) {
				if (game.tiredlevel < 3)
					cnmsg = msgf(0x63, strs[1458]);
				else {
					cnmsg = msgf(0x63, strs[1415], tiredtext(game.tiredlevel));
					knowpotion(item);
				}
			}
		} else if (cnfill) {
			switch (v) {
			case 0:
			case 3:
				break;
			case 1:
				cnmsg = msgf(0x63, strs[242], game.hungerlevel >= 3 ? strs[243] : empty, hungertext(game.hungerlevel));
				break;
			default:
				cnmsg = msgf(0x63, strs[242], game.hungerlevel >= 3 ? strs[243] : empty, hungertext(game.hungerlevel));
				break;
			}
		} else if (cndrink && v && v != 3)
			cnmsg = msgf(0x63, strs[1458]);
	} else if (item & 0x10000000) {
		cnv = u->drunk + 60;
		if (cnv > 0xff)
			cnv = 0xff;
		u->drunk = cnv;
		if (u->drunk >= 0xd2)
			u->asleep = 0xff;
	}
	if (u == units && (item & 0x20000000)) {
		if (v && v != 3 && game.tiredcount) {
			delmsgid(cnmsg);
			cnmsg = msgf(2, strs[389]);
			knowpotion(item);
		}
		game.tiredcount = 0;
	}
	cnv = 0;
	if ((item & 0x80000000) && (item & 0x40000000))
		cnv = randn(40) + 150;
	else {
		if (item & 0x80000000)
			cnv += randn(20) + 10;
		if (item & 0x40000000)
			cnv += randn(40) + 30;
	}
	if (cnv) {
		if (u == units && v && v != 3) {
			delmsgid(cnmsg);
			cnmsg = msgf(11, strs[387]);
			knowpotion(item);
		}
		cnv += u->poison;
		if (cnv > 0xff)
			cnv = 0xff;
		u->poison = cnv;
	}
	cnv = 0;
	if (item & 0x4000000)
		cnv += kindof(u)->maxhealth / 5;
	if (item & 0x2000000)
		cnv += kindof(u)->maxhealth / 4;
	if (cnv) {
		if (u == units && v && v != 3 && u->health < kindof(u)->maxhealth) {
			delmsgid(cnmsg);
			cnmsg = msgf(10, strs[384]);
			knowpotion(item);
		}
		addhealth(u, cnv);
	}
	if (item & 0x200000) {
		if (u == units && v && v != 3)
			delmsgid(cnmsg);
		if (!quaffing)
			curepoison(u, u, 3);
		if (u == units)
			knowpotion(item);
	}
	if (item & 0x8000000) {
		if (u == units && v && v != 3)
			delmsgid(cnmsg);
		if (!quaffing)
			protect(u, u, 3);
		if (u == units)
			knowpotion(item);
	}
	if (item & 0x100000) {
		if (!u->berserk) {
			delmsgid(cnmsg);
			if (!quaffing) {
				drawscreen();
				goberserk(u, cnclass == 0x38 ? 2 : 3);
			}
			if (u == units)
				knowpotion(item);
		}
		if (u == units && cnclass == 0x39)
			setflag(0x4b1, 1);
	}
	if (item & 0x40000) {
		if (u == units && v && v != 3) {
			delmsgid(cnmsg);
			cnmsg = msgf(14, strs[393]);
			knowpotion(item);
		}
		u->asleep = 75;
	}
	if (item & 0x80000) {
		cnv = kindof(u)->maxvigor / 3;
		if (u->vigor + cnv >= kindof(u)->maxvigor)
			u->vigor = kindof(u)->maxvigor;
		else
			u->vigor += cnv;
		if (u == units)
			knowpotion(item);
	}
	if ((item & 0x20000) && u == units)
		knowpotion(item);
	if (item & 0x10000) {
		if (u == units && v && v != 3) {
			delmsgid(cnmsg);
			cnmsg = msgf(11, strs[394]);
			knowpotion(item);
		}
		u->weakened = 75;
		u->blessed = 0;
		u->warded = 0;
	}
	if (islovepotion(item)) {
		if (u == units) {
			units[0].sympathy = 4;
			if (v && v != 3) {
				delmsgid(cnmsg);
				cnmsg = msgf(0x63, strs[396]);
			}
		} else if (u->sympathy < 2) {
			if (v && v != 3) {
				delmsgid(cnmsg);
				cnmsg = msgf(0x63, strs[378], unitname(u));
			}
		} else if (u->sympathy == 2 || !isfemale(u)) {
			u->sympathy = 3;
			if (v && v != 3) {
				delmsgid(cnmsg);
				cnmsg = msgf(0x63, strs[377], unitname(u));
				knowpotion(item);
			}
		} else {
			u->sympathy = 4;
			u->traits &= ~0x20;
			if (v && v != 3) {
				delmsgid(cnmsg);
				cnmsg = msgf(0x63, strs[376], unitname(u));
				knowpotion(item);
				music(0x17, 0);
			}
		}
	}
	if (u == units && v && v != 3) {
		if ((item & 0xfff) == 0x173) {
			delmsgid(cnmsg);
			cnmsg = msgf(0x63, strs[390]);
			knowpotion(item);
		} else if ((item & 0xfff) == 0x174) {
			delmsgid(cnmsg);
			cnmsg = msgf(0x63, strs[392]);
			knowpotion(item);
		} else if ((item & 0xfff) == 0x18c) {
			delmsgid(cnmsg);
			cnmsg = msgf(0x63, strs[391]);
			knowpotion(item);
		} else if (newmsgs == 0) {
			if (cnclass == 0x38) {
				delmsgid(cnmsg);
				cnmsg = msgf(1, strs[397]);
			} else if (cnclass == 0x39) {
				delmsgid(cnmsg);
				cnmsg = msgf(1, strs[398]);
			}
		}
	}
	if (u == units)
		setlevels();
	calcstats(u);
	if (u == units)
		fillgauges();
	return 1;
}

short
engaged(Unit *u)
{
	unsigned char t;

	if (badunit(u))
		return 0;
	if (u->mode == 0x19) {
		if (u->modee > 0)
			return 1;
		if (u->modeb != 0xff && units[u->modeb].mode != 0x14)
			u->modeb = 0xff;
		if (u->modec != 0xff && units[u->modec].mode != 0x14)
			u->modec = 0xff;
		if (u->modeb == 0xff)
			u->modeb = u->modec;
		if (u->modeb == 0xff) {
		release:
			if (u->sympathy > 0)
				makepeace(u, NULL);
			else
				makepeace(u, units);
			return 0;
		}
	}
	if (u->mode == 0x14 || u->mode == 0x15 || u->mode == 0x19) {
		if (ceasefire(curmap, indexof(u)))
			goto release;
		if (u->modec != 0xff && allied(u, &units[u->modec]))
			u->modec = 0xff;
		if (u->modeb != 0xff && allied(u, &units[u->modeb])) {
			u->modeb = u->modec;
			u->modec = 0xff;
		}
		if (u->modeb != 0xff) {
			if (iscow(u)) {
				if (abs(units[u->modeb].x - u->x) < (u->mode == 0x19 ? 6 : 3)
				    && abs(units[u->modeb].y - u->y) < (u->mode == 0x19 ? 6 : 3))
					return 1;
			} else if (iswild(u)) {
				if (abs(units[u->modeb].x - u->x) < 37 && abs(units[u->modeb].y - u->y) < 37)
					return 1;
			} else if (isanimal(u)) {
				if (abs(units[u->modeb].x - u->x) < 15 && abs(units[u->modeb].y - u->y) < 15)
					return 1;
			} else if (abs(units[u->modeb].x - u->x) < 37 && abs(units[u->modeb].y - u->y) < 37)
				return 1;
		}
		if (flanked(u) && u->modeb != 0xff && u->modec != 0xff && isfemale(&units[u->modeb]) && !isfemale(&units[u->modec])) {
			t = u->modeb;
			u->modeb = u->modec;
			u->modec = t;
			return 0;
		}
	}
	return 0;
}

void
roam(Unit *u)
{
	short a, b, i;

	if (badunit(u))
		return;
	a = provat(u->x, u->y);
	i = 0;
	b = regionat(u->x, u->y);
	while (i < 15) {
		u->nextx = randn(7) + u->x - 3;
		if (u->nextx < 3)
			u->nextx = 3;
		else if (u->nextx > 0x93)
			u->nextx = 0x93;
		u->nexty = randn(7) + u->y - 3;
		if (u->nexty < 3)
			u->nexty = 3;
		else if (u->nexty > 0x93)
			u->nexty = 0x93;
		if (provat(u->nextx, u->nexty) == a && regionat(u->nextx, u->nexty) == b)
			return;
		cleartrail(u);
		i++;
	}
}

short
diagfree(short x, short y, short dx, short dy)
{
	if (dx && dy) {
		if (dy < 0) {
			if (dx < 0) {
				if ((map[(y - 1) * 150 + x] & 0x4000) == 0)
					return 0;
				if ((map[y * 150 + x - 1] & 0x4000) == 0)
					return 0;
			} else if (dx > 0) {
				if ((map[(y - 1) * 150 + x] & 0x4000) == 0)
					return 0;
				if ((map[y * 150 + x + 1] & 0x4000) == 0)
					return 0;
				if ((mapflags[(y - 1) * 150 + x] & 0x40000) != 0)
					return 0;
			}
		} else if (dy > 0) {
			if (dx < 0) {
				if ((map[(y + 1) * 150 + x] & 0x4000) == 0)
					return 0;
				if ((map[y * 150 + x - 1] & 0x4000) == 0)
					return 0;
				if ((mapflags[y * 150 + x - 1] & 0x40000) != 0)
					return 0;
			} else if (dx > 0) {
				if ((map[(y + 1) * 150 + x] & 0x4000) == 0)
					return 0;
				if ((map[y * 150 + x + 1] & 0x4000) == 0)
					return 0;
			}
		}
	}
	return 1;
}

short
canpass(int x0, int y0, int x1, int y1)
{
	cpfrom = x0 + y0 * 150;
	cpto = x1 + y1 * 150;
	if (x1 < x0) {
		if (y1 == y0) {
			if (mapflags[cpfrom] & 0x4000 || mapflags[cpto] & 0x1000)
				return 0;
		} else if (y1 < y0) {
			if (mapflags[cpfrom] & 0x8000 || mapflags[cpto] & 0x40000)
				return 0;
		} else {
			if (mapflags[cpfrom] & 0x20000 || mapflags[cpto] & 0x10000)
				return 0;
		}
	} else if (x1 > x0) {
		if (y1 == y0) {
			if (mapflags[cpfrom] & 0x1000 || mapflags[cpto] & 0x4000)
				return 0;
		} else if (y1 < y0) {
			if (mapflags[cpfrom] & 0x10000 || mapflags[cpto] & 0x20000)
				return 0;
		} else {
			if (mapflags[cpfrom] & 0x40000 || mapflags[cpto] & 0x8000)
				return 0;
		}
	} else if (y1 < y0) {
		if (mapflags[cpfrom] & 0x800 || mapflags[cpto] & 0x2000)
			return 0;
	} else if (y1 > y0) {
		if (mapflags[cpfrom] & 0x2000 || mapflags[cpto] & 0x800)
			return 0;
	}
	return 1;
}

void
freesteps(Unit *u)
{
	nfreedirs = 0;
	if (badunit(u))
		return;
	stepcell = u->y;
	stepcell *= 150;
	stepcell += u->x;
	for (fsdir = 0; fsdir < 8; fsdir += 2) {
		fsdx = dirdx[fsdir];
		fsx = u->x + fsdx;
		fsdy = dirdy[fsdir];
		fsy = u->y + fsdy;
		fscell = fsx + fsy * 150;
		dircells[fsdir] = fscell;
		dirdoors[fsdir] = 0;
		if ((map[fscell] & 0x4000) == 0) {
			if ((mapflags[fscell] & 1) == 0 || (mapflags[fscell] & 0x40) != 0)
				continue;
			dirdoors[fsdir] = 1;
		}
		if (!canpass(u->x, u->y, fsx, fsy))
			continue;
		if ((u->mode == 1 || u->mode == 3 && u->modee != 0x11 || u->mode == 7) && (mapflags[fscell] & 0x10) != 0)
			continue;
		if (fscell == playercell || fscell == playernext) {
			if (playmode != 1 && u->sympathy <= 0)
				continue;
		} else if (!(mapflags[fscell] & 4))
			goto add;
		if ((mapflags[fscell] & 8) != 0
		    || gamehour >= 6 && gamehour <= 20 && (isherder(u) || u->leader)) {
		add:
			freedirs[nfreedirs++] = fsdir;
			continue;
		}
		if ((u->traits & 4) != 0 || u->mode != 8)
			continue;
		if (fsx == u->sched[3][gamehour] && fsy == u->sched[4][gamehour]) {
			fsunit = unitat(fsx, u->sched[4][gamehour]);
			if (fsunit == 0xff)
				goto add;
			if ((units[fsunit].traits & 4) == 0 && fsx == units[fsunit].sched[3][gamehour]
			    && fsy == units[fsunit].sched[4][gamehour])
				goto add;
			continue;
		}
		if (abs(fsx - u->sched[3][gamehour]) <= 1 && abs(fsy - u->sched[4][gamehour]) <= 1)
			goto add;
		if (!u->modea || abs(fsx - u->modeb) > 1)
			continue;
		if (abs(fsy - u->modec) <= 1)
			goto add;
	}
	if (nfreedirs == 0) {
		if (u->mode == 0 || u->mode == 2 || u->mode == 0x5a || u->mode == 4)
			return;
		// FIXED: the tests of the neighbors read around fscell, the last cell the loop tried, where unitat looks around u. Test around stepcell, u's cell.
#if defined(FEATURE_FIXES)
		fscell = stepcell;
#endif
		if (mapflags[fscell + 1] & 4) {
			fsunit = unitat(u->x + 1, u->y);
			if (fsunit != 0xff && fsunit != 0) {
				units[fsunit].incoming = indexof(u);
				return;
			}
		}
		if (mapflags[fscell + 150] & 4) {
			fsunit = unitat(u->x, u->y + 1);
			if (fsunit != 0xff && fsunit != 0) {
				units[fsunit].incoming = indexof(u);
				return;
			}
		}
		if (mapflags[fscell - 1] & 4) {
			fsunit = unitat(u->x - 1, u->y);
			if (fsunit != 0xff && fsunit != 0) {
				units[fsunit].incoming = indexof(u);
				return;
			}
		}
		if (mapflags[fscell - 150] & 4) {
			fsunit = unitat(u->x, u->y - 1);
			if (fsunit != 0xff && fsunit != 0) {
				units[fsunit].incoming = indexof(u);
				return;
			}
		}
		return;
	}
	for (fsdir = 0; fsdir < nfreedirs - 1; fsdir++)
		if (u->x + dirdx[freedirs[fsdir]] == u->trailx0 && u->y + dirdy[freedirs[fsdir]] == u->traily0) {
			fsunit = freedirs[fsdir];
			freedirs[fsdir] = freedirs[nfreedirs - 1];
			freedirs[nfreedirs - 1] = fsunit;
			return;
		}
}

short
beststep(Unit *u, short mode)
{
	if (badunit(u))
		return 0;
	for (bsi = 0; bsi < 8; bsi++)
		dirscores[bsi] = 0x57e4;
	u->nsteps = nfreedirs;
	for (bsi = 0; bsi < nfreedirs; bsi++) {
			bsdir = freedirs[bsi];
			u->steps[bsi].dir = bsdir;
			u->steps[bsi].kind = 9;
			bscell = map[dircells[bsdir]];
			bsx = u->x + dirdx[bsdir];
			bsy = u->y + dirdy[bsdir];
			bsprov = provat(u->x, u->y);
			bsdist = celldist(bsx, bsy, u->nextx, u->nexty);
			bsv = bsdist;
			if (mode)
				bsv = 0x32 - bsdist;
			dirscores[bsdir] = bsv * 2;
			u->steps[bsi].score1 = dirscores[bsdir];
			if (u->mode == 0x19 && u->modeb != 0xff) {
				bsv = celldist(bsx, bsy, units[u->modeb].x, units[u->modeb].y);
				dirscores[bsdir] -= bsv;
			} else if (!mode && !(bsdist <= 1 && abs(bsx - u->nextx) <= 1 && abs(bsy - u->nexty) <= 1)) {
				if (bsx == u->trailx0 && bsy == u->traily0)
					dirscores[bsdir] += 0x96;
				else if (bsx == u->trailx1 && bsy == u->traily1)
					dirscores[bsdir] += 100;
				else if (!isanimal(u)) {
					if (bsx == u->trailx2 && bsy == u->traily2)
						dirscores[bsdir] += 0x4d;
					else if (bsx == u->trailx3 && bsy == u->traily3)
						dirscores[bsdir] += 0x3c;
					else {
						if (dirdx[bsdir] && dirdx[bsdir] == u->trailx0 - u->x)
							dirscores[bsdir] += 0x4b;
						if (dirdy[bsdir] && dirdy[bsdir] == u->traily0 - u->y)
							dirscores[bsdir] += 0x4b;
					}
				}
			}
			u->steps[bsi].score2 = dirscores[bsdir];
			if (mapflags[dircells[bsdir]] & 4)
				dirscores[bsdir] += 3;
			if (!(u->mode == 0x14 || u->mode == 0x19)) {
				if (isanimal(u)) {
					// FIXED: an animal's test reads the terrain of its goal, the same for every step, so it changes no choice. Read the step's cell, as the other tests do.
#if defined(FEATURE_FIXES)
					bsv = terrains[bscell & 0xfff];
#else
					bsv = terrains[map[u->nexty * 150 + u->nextx] & 0xfff];
#endif
					if (bsv == 5)
						dirscores[bsdir] += 0x4b;
				} else if (!isherder(u) || bsprov) {
					bsv = terrains[map[u->sched[4][gamehour] * 150 + u->sched[3][gamehour]] & 0xfff];
					if ((bsv == 4 || bsv == 1 || bsv == 0) && !bsprov && bsdist < 0x1e) {
						if (u->mode == 3 || u->mode == 7) {
							bsv = terrains[map[u->y * 150 + u->x] & 0xfff];
							if (bsv == 4 || bsv == 1 || bsv == 0) {
								bsv = terrains[bscell & 0xfff];
								if (bsv == 3 || bsv == 2 || bsv == 5)
									dirscores[bsdir] += 0x9b;
							} else {
								bsv = terrains[bscell & 0xfff];
								if (bsv == 4 || bsv == 1 || bsv == 0)
									dirscores[bsdir] -= 3;
							}
						}
					} else if (u->mode != 7) {
						if (bsdist < 5) {
							bsv = terrains[map[u->nexty * 150 + u->nextx] & 0xfff];
							if (!(bsv == 3 || bsv == 5)) {
								dirscores[bsdir] -= 3;
								goto next;
							}
						}
						bsv = terrains[bscell & 0xfff];
						u->steps[bsi].kind = bsv;
						if (bsv == 3 || bsv == 5)
							dirscores[bsdir] -= 3;
						else {
							bsv = terrains[map[u->y * 150 + u->x] & 0xfff];
							if (bsv == 3 || bsv == 5)
								dirscores[bsdir] += 3;
						}
					}
				}
			}
		next:
			u->steps[bsi].score3 = dirscores[bsdir];
			bsv = regionat(u->x + dirdx[bsdir], u->y + dirdy[bsdir]);
			if (regionat(u->nextx, u->nexty) != bsv)
				dirscores[bsdir] += 0x2ee;
			bsv = provat(u->x + dirdx[bsdir], u->y + dirdy[bsdir]);
			if (provat(u->nextx, u->nexty) != bsv)
				dirscores[bsdir] += 0x2ee;
			if ((mapflags[dircells[bsdir]] & 0x40) && !(bsx == u->nextx && bsy == u->nexty)
			    && (isservant(u) || (mapflags[stepcell] & 0x40) || u->awaketurns < 0x2c || isanimal(u)
			    || u->mode == 0x14 || u->mode == 0x16 || u->mode == 8 || u->mode == 3 && u->modee == 0x11))
				dirscores[bsdir] += 0xbb;
			if ((mapflags[dircells[bsdir]] & 0x400) && !(mapflags[dircells[bsdir]] & 8)
			    && u->mode != 0x14 && u->mode != 0x15 && u->mode != 0x16) {
				if (u->mode == 1 || u->mode == 3 && u->modee != 0x11 || u->mode == 7)
					dirscores[bsdir] += 0xc9;
				else if (mapflags[dircells[bsdir]] & 8)
					dirscores[bsdir]++;
				else
					dirscores[bsdir] += 0xc9;
			}
			if (u->mode == 8 && u->nextx == u->sched[3][gamehour] && u->nexty == u->sched[4][gamehour]
			    && bsx == u->sched[3][gamehour] && bsy == u->sched[4][gamehour])
				dirscores[bsdir] = -150;
			u->steps[bsi].score4 = dirscores[bsdir];
	}
	bsv = 0x57e4;
	for (bsi = 0; bsi < 8; bsi++)
		if (dirscores[bsi] < bsv)
			bsv = dirscores[bsi];
	nbestdirs = 0;
	for (bsi = 0; bsi < 8; bsi++)
		if (bsv == dirscores[bsi])
			bestdirs[nbestdirs++] = bsi;
	if (nbestdirs == 1)
		return bestdirs[0];
	if (nbestdirs < 1)
		return -1;
	bsdir = randn(nbestdirs);
	return bestdirs[bsdir];
}

int
takestep(Unit *u, short v)
{
	if (badunit(u))
		return 0;
	freesteps(u);
	{
		short n = nfreedirs;

		if (!n)
			return 0;
	}
	if (u->mode != 1 && u->mode != 7 && v != 0x25)
		tsdir = beststep(u, v);
	else {
		roam(u);
		tsdir = beststep(u, 0);
	}
	if (tsdir == -1)
		return 0;
	u->dirx = dirdx[tsdir];
	u->diry = dirdy[tsdir];
	u->pose = 1;
	mapflags[stepcell] &= ~4;
	mapflags[dircells[tsdir]] |= 4;
	if (!dirdoors[tsdir])
		return 1;
	opendoor(dircells[tsdir], 0);
	lookstale = 1;
	return 2;
}

void
clearmind(Unit *u)
{
	if (badunit(u))
		return;
	if (u->fleeing || u->berserk || u->bewitched) {
		if (u == units && units[0].health > 0) {
			msgf(0x63, strs[1084]);
			if (units[0].berserk) {
				game.tiredcount += 200;
				game.hungercount += 0x1e0;
			}
			setlevels();
		}
		u->berserk = 0;
		u->fleeing = 0;
		u->bewitched = 0;
		u->bewitcher = 0;
		calcstats(u);
	}
	if (u == units && lostcontrol)
		regaincontrol();
}

void
tickstatus(Unit *u)
{
	unsigned char i, close;

	if (badunit(u))
		return;
	if (u == units) {
		autoeat();
		if (sensesleft && !--sensesleft) {
			game.tiredcount += 0x32;
			setlevels();
			lookdone();
			msgf(0x14, strs[1085]);
		}
	}
	if (u->asleep)
		u->asleep--;
	if (u->bewitched && --u->bewitched == 0) {
		u->bewitched = 1;
		clearmind(u);
		provoke(&units[u->bewitcher], u, 3, 6, 0);
	}
	if (u->cursed)
		u->cursed--;
	if (u->blessed)
		u->blessed--;
	if (u->fleeing && --u->fleeing == 0) {
		u->fleeing = 1;
		clearmind(u);
	}
	if (u->confused && u->confused != 0xff)
		u->confused--;
	if (u->berserk && --u->berserk == 0) {
		u->berserk = 1;
		clearmind(u);
	}
	if (u->weakened) {
		u->weakened--;
		if (u->weakened == 0)
			calcstats(u);
	}
	if (u->warded && --u->warded == 0)
		calcstats(u);
	if (u->mode == 0x14 || u->mode == 0x15) {
		if (u->modeb != 0xff && u->modeb && (u->modeb == indexof(u) || units[u->modeb].health <= 0
		    || units[u->modeb].mode != 0x14 && units[u->modeb].mode != 0x15))
			u->modeb = 0xff;
		if (u->modec != 0xff && u->modec && (u->modec == indexof(u) || units[u->modec].health <= 0
		    || units[u->modec].mode != 0x14 && units[u->modec].mode != 0x15))
			u->modec = 0xff;
		if (u->modeb == 0xff) {
			if ((u->traits & 8) && u != units)
				u->traits = 0xff;
			else if (u->modec != 0xff) {
				u->modeb = u->modec;
				u->modec = 0xff;
			} else if (u->berserk) {
				for (i = 0, close = 0xff; i < nunits; i++) {
					if (i == indexof(u))
						continue;
					if (isfemale(&units[i]))
						continue;
					if (units[i].x < u->x - 7 || units[i].x > u->x + 7)
						continue;
					if (units[i].y < u->y - 7 || units[i].y > u->y + 7)
						continue;
					if (allied(u, &units[i]))
						continue;
					close = i;
					if (units[i].x >= u->x - 2 && units[i].x <= u->x + 2 && units[i].y >= u->y - 2 && units[i].y <= u->y + 2) {
						if (u->modeb != 0xff) {
							u->modec = i;
							break;
						}
						u->modeb = i;
						if (u->modec != 0xff)
							break;
					} else {
						u->modec = i;
						if (u->modeb != 0xff)
							break;
					}
				}
				if (u->modeb == 0xff)
					u->modeb = close;
			} else if (u->lord && (units[u->lord].mode == 0x14 || units[u->lord].mode == 0x15 || units[u->lord].mode == 0x19)) {
				u->modeb = units[u->lord].modeb;
				u->modec = units[u->lord].modec;
			} else if (u->sympathy <= 0 && (!u->bewitched || u->bewitcher != 0))
				u->modeb = 0;
			else if (u == units || u->bewitched) {
				u->modeb = 0xff;
				u->modec = 0xff;
			} else
				makepeace(u, NULL);
		}
		if (u->mode == 0x14 || u->mode == 0x15) {
			if (u->modec == indexof(u))
				u->modec = 0xff;
			if (u->modeb == indexof(u))
				u->modeb = u->modec;
			if (u->modeb == 0 || u->modec == 0)
				u->sympathy = 0;
		}
	} else if (u->mode == 0x19) {
		if (u->modee > 0 && u->modee != 0xff)
			u->modee--;
		if (u->modeb != 0xff && units[u->modeb].health <= 0 && u->modeb)
			u->modeb = 0xff;
		if (u->modeb == 0xff && u->modee <= 0)
			makepeace(u, NULL);
	}
}

void
tickhealth(Unit *u)
{
	short n;

	if (badunit(u) || talkactive)
		return;
	newhealth = u->health;
	if (u->vigor < kindof(u)->maxvigor) {
		u->vigor++;
		if (u == units)
			fillgauges();
	}
	if (u->poison) {
		u->poison--;
		newhealth--;
		if (u->poison == 0) {
			calcstats(u);
			if (u == units)
				fillgauges();
		}
	}
	if (u->drunk && --u->drunk == 0)
		calcstats(u);
	if (u->bleeding) {
		newhealth--;
		u->bleeding--;
		if (u != units && u->mode != 0x19 && u->mode != 0x14 && u->mode != 0x15 && u->mode != 0x16
		    && skillof(u, 0x19, 0, 0, 0) >= 50)
			u->bleeding = 0;
	}
	if (newhealth == u->health && newhealth >= kindof(u)->maxhealth * 2 / 3) {
		newhealth++;
		if (u->asleep || u->mode == 0x5a)
			newhealth++;
	}
	if (u == units && game.hungerlevel >= 9) {
		n = kindof(u)->maxhealth - (game.hungerlevel - 9) * 2;
		if (n < newhealth)
			newhealth = n;
	}
	if (newhealth < 0)
		newhealth = 0;
	else if (newhealth > kindof(u)->maxhealth)
		newhealth = kindof(u)->maxhealth;
	if (newhealth != u->health) {
		u->health = newhealth;
		calcstats(u);
		if (u == units)
			fillgauges();
		if (u->health <= 0) {
			if (u == units && restbreak)
				u->health = 1;
			else {
				if (cansee(u->x, u->y)) {
					msgf(u->sympathy > 0 ? 0x19 : 0x1e, strs[1410], unitname(u));
					if (u == units) {
						music(0x11, 0);
						queuesound(0x1e);
						playqueue();
					} else if (!isanimal(u)) {
						if (isfemale(u))
							queuesound(0x4b);
						else
							queuesound(0x1e);
						if (u != units && u->sympathy <= 0)
							music(0x23, 0);
					}
				}
				killunit(u, 1, 4, 0xff, 0);
				if (u == units) {
					game.fate = 0;
					game.killer = 0;
				}
			}
		}
	}
	if (u == units)
		return;
	if (islittorch(u->offhand)) {
		if (u->health <= 0 || u->mode == 0x5a || ambient() > 6)
			setlit(&u->offhand, 0);
	} else if (isunlittorch(u->offhand) && u->health > 0 && u->mode != 0x5a && ambient() <= 6)
		setlit(&u->offhand, 1);
}

void
calctorch(void)
{
	short dx, dy, d;

	torchdist = 0xff;
	for (torchi = 1; torchi < nunits; torchi++) {
		torchu = &units[torchi];
		if (islittorch(torchu->offhand) && cansee(torchu->x, torchu->y)) {
			d = dx = abs(torchu->x - playerx);
			dy = abs(torchu->y - playery);
			if (dx < dy)
				d = dy;
			if (d < torchdist)
				torchdist = d;
		}
	}
}

void
clearturn(void)
{
	for (turni = 0; turni < nunits; turni++)
		units[turni].turnmarks = 0;
}

int
moveunits(short mode)
{
	mvdoor = 0;
	if (mode != 2 && (gametime & 7) == 0)
		bruglowfeast();
	if (mode) {
		mvi = 0;
		mvu = units;
		goto turn;
	}
	for (mvi = 1; mvi < nunits; mvi++) {
		mvu = &units[mvi];
		if (mvu->x == 0 && mvu->y == 0)
			continue;
		mapflags[mvu->y * 150 + mvu->x] &= ~4;
		if (mvu->health > 0) {
			if (mvu->pose == 1) {
				mvu->trailx3 = mvu->trailx2;
				mvu->traily3 = mvu->traily2;
				mvu->trailx2 = mvu->trailx1;
				mvu->traily2 = mvu->traily1;
				mvu->trailx1 = mvu->trailx0;
				mvu->traily1 = mvu->traily0;
				mvu->trailx0 = mvu->x;
				mvu->traily0 = mvu->y;
				mvx = mvu->x;
				mvy = mvu->y;
				mvu->x += mvu->dirx;
				mvu->y += mvu->diry;
				mvcell = mvx + mvy * 150;
				if ((mapflags[mvcell] & 1) && !(mapflags[mvcell] & 0x40) && gamehour >= 21
				    && playercell != mvcell && playernext != mvcell
				    && mvu->mode != 0x14 && mvu->mode != 0x15 && mvu->mode != 0x16
				    && !isanimal(mvu)
				    && !(isherder(mvu) && gamehour >= 6 && gamehour <= 20)) {
					mvu->lasthit = 0xfe;
					mvu->pose = 0;
					goto turn;
				}
			} else {
				if (mvu->pose != 10)
					mvu->pose = 0;
				goto turn;
			}
		}
		mvu->pose = 0;
	turn:
		if (talkactive == 0) {
			tickstatus(mvu);
			if ((gametime & 7) == 0)
				tickhealth(mvu);
		}
		if (mode == 2)
			break;
	}
	for (mvi = 1; mvi < nunits; ) {
		mvu = &units[mvi];
		if (mvu->traits == 0xff)
			delunit(mvi);
		else {
			mapflags[mvu->y * 150 + mvu->x] |= 4;
			mvi++;
		}
	}
	for (mvi = 1; mvi < nunits; mvi++) {
		mvu = &units[mvi];
		if (mvu->lasthit == 0xfe) {
			mvcell = mvu->traily0;
			mvcell *= 150;
			mvcell += mvu->trailx0;
			if (!(mapflags[mvcell] & 4)) {
				closedoor(mvcell, 1);
				mvdoor = 1;
				lookstale = 1;
			}
		}
		mvu->lasthit = 0;
	}
	return mvdoor;
}

short
baseskill(Unit *u, short n)
{
	if (badunit(u))
		return 100;
	bkstat = kindof(u)->strength * gains[n].w[0] / 100 + kindof(u)->dexterity * gains[n].w[1] / 100
		+ kindof(u)->intellect * gains[n].w[2] / 100;
	bklevel = bkstat - gains[n].min;
	if (bklevel < 0)
		bklevel = 0;
	bklevel *= kindof(u)->aptitude[n];
	bklevel /= 100;
	return bklevel;
}

short
skillof(Unit *u, short n, short b, short c, short d)
{
	if (badunit(u))
		return 100;
	sostat = (u->strength + b) * gains[n].w[0] / 100 + (u->dexterity + c) * gains[n].w[1] / 100
		+ (u->intellect + d) * gains[n].w[2] / 100;
	solevel = sostat - gains[n].min;
	if (solevel < 0)
		solevel = 0;
	solevel *= kindof(u)->aptitude[n];
	solevel /= 100;
	sopct = 100;
	switch (n) {
	case 0xb:
	case 0xc:
	case 0xd:
	case 0xe:
	case 0xf:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
		if (u->wrist & 0x40000)
			sopct += 2;
		if (u->neck & 0x40000)
			sopct += 2;
		break;
	case 0x16:
		if ((u->wrist & 0xfff) == 0x4f3)
			sopct = 101;
		else if ((u->wrist & 0xfff) == 0x4f4)
			sopct = 102;
		if ((u->neck & 0xfff) == 0x4f6)
			sopct++;
		else if ((u->neck & 0xfff) == 0x4f7)
			sopct += 2;
		if (u->cloak && wearof(u->cloak) <= 3)
			sopct++;
		if (u->body && valueof(u->body) > valueof(0xee01af) * 2)
			sopct += 2;
		if (u->head && valueof(u->head) > valueof(0xe601ba) * 2)
			sopct++;
		if (u->belt && valueof(u->belt) > valueof(0xe801ce) * 2)
			sopct += 2;
		if (u->hand && valueof(u->hand) > valueof(0xe801ce) * 2)
			sopct += 2;
	case 0x14:
	case 0x15:
	case 0x18:
	case 0x19:
	worn:
		if (u->wrist & 0x100000)
			sopct += 4;
		if (u->neck & 0x100000)
			sopct += 4;
		break;
	case 0x17:
		if (u->wrist)
			sopct = 101;
		if (u->neck)
			sopct++;
		goto worn;
	}
	if (sopct != 100) {
		solevel *= sopct;
		solevel /= 100;
	}
	return solevel;
}

unsigned char
skillbyte(Unit *u, short n, short b, short c, short d)
{
	if (badunit(u))
		return 100;
	sbv = skillof(u, n, b, c, d);
	if (sbv < 0)
		sbv = 0;
	else if (sbv > 0xff)
		sbv = 0xff;
	return sbv;
}

unsigned char
halfskill(Unit *u, short n)
{
	if (badunit(u))
		return 0;
	hsv = skillbyte(u, n, 0, 0, 0) / 2;
	return hsv;
}

void
addcap(unsigned char *p, unsigned char v)
{
	if (v + *p > 0xff)
		*p = 0xff;
	else
		*p += v;
}

void
addpower(unsigned char *p, Unit *u, short n)
{
	if (badunit(u))
		return;
	addcap(p, halfskill(u, n));
}

short
odds(short a, short b)
{
	oddsv = (a - b) * 15 / 10;
	if (oddsv < -50)
		return 0;
	if (oddsv > 50)
		return 100;
	return oddsv + 50;
}

short
prevails(short a, short b)
{
	if (cheat && (keys['W'] & 0x80))
		return 1;
	if (randn(100) < odds(a, b))
		return 1;
	return 0;
}

short
prowess(Unit *u)
{
	short s;

	if (badunit(u))
		return 0;
	pwn = u->mode;
	u->mode = 0x14;
	attackval(u, &pwatk, &pwkind, &pwlo, &pwhi, 0);
	defenseval(u, &pwparry, &pwprot, &pwdef);
	u->mode = pwn;
	pwn = 0;
	if (pwdef)
		pwn = 1;
	if (pwprot) {
		pwdef += pwprot;
		pwn++;
	}
	if (pwparry) {
		pwdef += pwparry;
		pwn++;
	}
	if (pwn)
		pwdef /= pwn;
	else
		pwdef = 0;
	pwprot = protection(u->head) + protection(u->body) + protection(u->legs);
	s = u->health + (pwatk * 2 - 200) + pwlo + pwhi / 2 + (pwdef * 2 - 200) + pwprot;
	pwatk = s * 100 / 150;
	if (pwatk < 0)
		pwatk = 0;
	return pwatk;
}

short
winodds(Unit *u, Unit *v)
{
	if (badunit(u))
		return 0;
	if (badunit(v))
		return 0;
	wou = prowess(u);
	wov = prowess(v);
	if (!wou && !wov)
		return 50;
	// FIXED: the product takes a short, so above a prowess of 327 the odds turn negative. Multiply in an int.
#if defined(FEATURE_FIXES)
	return (int)wou * 100 / (wou + wov);
#else
	return (short)(wou * 100) / (wou + wov);
#endif
}

unsigned long
charmprice(unsigned long v)
{
	charmpct = (100 - skillof(units, 0x16, 0, 0, 0)) / 3 + 100;
	if (charmpct < 50)
		charmpct = 50;
	else if (charmpct > 150)
		charmpct = 150;
	v *= charmpct;
	v /= 100;
	return v;
}

void
addscore(int v)
{
	game.score += v;
	if (game.score < 0)
		game.score = 0;
	else if (game.score > 1000000)
		game.score = 1000000;
}

void
addhiscore(void)
{
	for (playerrank = 0; playerrank < 13; playerrank++)
		if (hiscores[playerrank].game == game.id) {
			for (hsrow = playerrank; hsrow < 12; hsrow++)
				memcpy(&hiscores[hsrow], &hiscores[hsrow + 1], 0x1c);
			memset(&hiscores[hsrow], 0, 0x1c);
		}
	for (playerrank = 0; playerrank < 12; playerrank++)
		if (hiscores[playerrank].score <= game.score || hiscores[playerrank].name[0] == 0) {
			for (hsrow = 12; hsrow > playerrank; hsrow--)
				memcpy(&hiscores[hsrow], &hiscores[hsrow - 1], 0x1c);
			break;
		}
	memcpy(hiscores[playerrank].name, game.name, 0x14);
	hiscores[playerrank].score = game.score;
	hiscores[playerrank].game = game.id;
	hiscores[playerrank].unused = 0;
}

short
clearsight(int x0, int y0, int x1, int y1)
{
	lineokx = lineendx = x0;
	lineoky = lineendy = y0;
	blocker = 0xff;
	linebegin((lineendx - playerx + 6) * 48 + 32, (lineendy - playery + 5) * 48,
		(x1 - playerx + 6) * 48 + 32, (y1 - playery + 5) * 48);
	while (linesteps) {
		lineendx = linestepx - 8;
		lineendx /= 48;
		lineendx += playerx - 6;
		lineendy = linestepy + 24;
		lineendy /= 48;
		lineendy += playery - 5;
		if (!(map[lineendy * 150 + lineendx] & 0x8000))
			return 0;
		if (!(lineendx == x0 && lineendy == y0) && !(lineendx == x1 && lineendy == y1)
		    && (mapflags[lineendy * 150 + lineendx] & 4)) {
			blocker = unitat(lineendx, lineendy);
			// FIXED: a cell flagged as taken may hold a dead unit alone, and unitat's 0xff then reads units[255], past the 150 units. Take such a cell as clear.
#ifdef FEATURE_FAULTS
			if (blocker != 0xff && !isfemale(&units[blocker]) && units[blocker].sympathy <= 0)
#else
			if (!isfemale(&units[blocker]) && units[blocker].sympathy <= 0)
#endif
				return 0;
			blocker = 0xff;
		}
		lineokx = lineendx;
		lineoky = lineendy;
		linestep();
	}
	return 1;
}

short
clearpath(int x0, int y0, int x1, int y1)
{
	lineokx = lineendx = x0;
	lineoky = lineendy = y0;
	blocker = 0xff;
	linebegin((lineendx - playerx + 6) * 48 + 32, (lineendy - playery + 5) * 48,
		(x1 - playerx + 6) * 48 + 32, (y1 - playery + 5) * 48);
	while (linesteps) {
		lineendx = linestepx - 8;
		lineendx /= 48;
		lineendx += playerx - 6;
		lineendy = linestepy + 24;
		lineendy /= 48;
		lineendy += playery - 5;
		if (!(map[lineendy * 150 + lineendx] & 0x4000))
			return 0;
		if (!(lineendx == x0 && lineendy == y0) && !(lineendx == x1 && lineendy == y1)
		    && (mapflags[lineendy * 150 + lineendx] & 4)) {
			blocker = unitat(lineendx, lineendy);
			return 0;
		}
		lineokx = lineendx;
		lineoky = lineendy;
		linestep();
	}
	return 1;
}

short
blessally(Unit *u)
{
	if (badunit(u) || u->modeb == 0xff)
		return 0;
	for (bapass = 0; bapass <= 3; bapass++)
		for (bai = 0; bai < nunits; bai++) {
			if (indexof(u) == bai)
				continue;
			if (!(units[bai].mode == 0x14 || units[bai].mode == 0x15))
				continue;
			if (units[bai].modeb != u->modeb || units[bai].blessed)
				continue;
			if (!(samehouse(u, &units[bai]) || bapass >= 3))
				continue;
			if (isservant(&units[bai]) && bapass < 2)
				continue;
			if (isfemale(&units[bai]) && bapass < 1)
				continue;
			if (units[bai].cursed || units[bai].weakened)
				continue;
			castat(u, bai, 0x15);
			return 1;
		}
	return 0;
}

short
endspar(Unit *u)
{
	if (badunit(u) || u->mode != 0x16 || u->modeb == 0 && challenger)
		return 0;
	if (u->modeb == 0xff || u->health <= 0 || units[u->modeb].health <= 0 || !isfit(u) || !isfit(&units[u->modeb])
	    || u->modeb == 0 && (!inview(u->x, u->y) || nearfoe(0)))
		goto release;
	spardist = stepdist(u, &units[u->modeb]);
	if (spardist > u->modeg || spardist > 10 || u->modee >= 50 || u->modef >= 50 || u->modeb == 0 && playmode == 1) {
		if (u->modeb == 0) {
			vara = u->modee;
			varb = u->modef;
			talker = indexof(u);
			talkscene = 0x26ef;
		}
		goto release;
	}
	if (spardist < u->modeg)
		u->modeg = spardist;
	return 0;
release:
	if (u->modeb == 0 && playmode != 1 && !nearfoe(0)) {
		sparending = 1;
		togglecombat(0);
		sparending = 0;
	}
	setfacing(u);
	setunitmode(u, 0);
	return 1;
}

short
endspars(void)
{
	sparany = 0;
	for (spari = 1; spari < nunits; spari++)
		if (endspar(&units[spari]))
			sparany = 1;
	return sparany;
}

int
haslight(int g, unsigned char a, unsigned char b)
{
	for (hli = 0; hli < regions[g].n; hli++)
		if (regions[g].lightx[hli] == a && regions[g].lighty[hli] == b)
			return 1;
	return 0;
}

short
lightcell(int x, int y, int open)
{
	lccell = y * 150;
	lccell += x;
	lcobj = findclass(lccell, open ? 0x59 : 0x5a);
	if (lcobj) {
		setlit(&mapitems[lcobj], open);
		setsmoke(lccell - 151, open);
		updatecell(lccell);
		return 1;
	}
	if (!open) {
		lcobj = findclass(lccell, 0x56);
		if (lcobj) {
			clamptorch(&mapitems[lcobj]);
			return 1;
		}
	}
	return 0;
}

void
lightregion(int g, int open)
{
	if (g == 0)
		return;
	for (lri = 0; lri < regions[g].n; lri++)
		lightcell(regions[g].lightx[lri], regions[g].lighty[lri], open);
}

unsigned char
idlepoint(Unit *u, short g, int flag)
{
	if (badunit(u))
		return 0;
	switch (u->type) {
	case 0x29:
		if (regionat(u->x, u->y) && randn(100) < 33)
			return 0xff;
		pplist = 3;
		break;
	case 0x16:
	case 0x17:
		if (regionat(u->x, u->y) && randn(100) < 50)
			return 0xff;
		if (randn(100) < (u->type == 0x17 ? 30 : 0) + 30) {
			if (isfemale(u))
				pplist = 2;
			else
				pplist = 1;
		} else
			pplist = 4;
		break;
	case 0x1a:
	case 0x1b:
		if (regionat(u->x, u->y) && randn(100) < 75)
			return 0xff;
		pplist = 5;
		break;
	default:
		if (isservant(u) && regionat(u->x, u->y) && randn(100) < 66)
			return 0xff;
		if (isfemale(u))
			pplist = 2;
		else
			pplist = 1;
		break;
	case 0x18:
	case 0x1c:
	case 0x1d:
	case 0x1e:
	case 0x1f:
	case 0x2d:
	case 0x2e:
	case 0x31:
		if (regionat(u->x, u->y) && randn(100) < 80)
			return 0xff;
		pplist = 6;
		break;
	}
	ppself = indexof(u);
	pptry = 0;
	ppfire = 0;
	if (!flag && g) {
		for (ppi = 0; ppi < 20; ppi++) {
			ppx = points[g].x[ppi] + points[g].dx[ppi];
			ppy = points[g].y[ppi] + points[g].dy[ppi];
			ppcell = ppx + ppy * 150;
			if (!haslight(g, ppx, ppy) || (mapflags[ppcell] & 0x100))
				continue;
			if ((u->mode == 1 || u->mode == 3) && u->goalx == 0xff && !ownerat(ppx, ppy))
				continue;
			ppcell = topitem(ppcell);
			if (ppcell && classof(mapitems[ppcell] & 0xfff) == 0x59 && !isfirewood(mapitems[ppcell])) {
				ppfire = 1;
				goto found;
			}
		}
	}
	while (pptry < 15) {
		ppi = randn(points[g].counts[pplist - 1]);
		ppi = points[g].lists[pplist - 1][ppi];
	found:
		ppx = points[g].x[ppi];
		ppy = points[g].y[ppi];
		if (flag && regionat(ppx, ppy))
			goto reject;
		if (ppfire) {
			for (ppj = 0; ppj < nunits; ppj++)
				if (ppj != ppself && units[ppj].x == ppx && units[ppj].y == ppy)
					goto reject;
		} else {
			for (ppj = 0; ppj < nunits; ppj++)
				if (ppj != ppself
				    && celldist(units[ppj].x, units[ppj].y, ppx, ppy) < 2)
					goto reject;
		}
		if (!(mapflags[ppx + ppy * 150] & 0x400))
			return ppi;
	reject:
		ppfire = 0;
		pptry++;
	}
	return 0xff;
}

short
playermoving(void)
{
	if (waitforup == 0 && (units[0].pose == 1 || rightclick || mouseright))
		return 1;
	return 0;
}

int
womannear(Unit *u, int n, short area)
{
	for (wnrank = 1; wnrank <= n; wnrank++) {
		wnbest = 0xff;
		wnmin = 150;
		for (wni = 1; wni < nunits; wni++)
			if (isfemale(&units[wni]) && ofrank(&units[wni], wnrank)) {
				wndist = celldist(u->x, u->y, units[wni].x, units[wni].y);
				if (wndist <= 10 && regionat(units[wni].x, units[wni].y) == area
				    && wndist < wnmin) {
					wnbest = wni;
					wnmin = wndist;
				}
			}
		if (wnbest != 0xff)
			return wnbest;
	}
	return 0xff;
}

int
hasman(short area)
{
	for (hmi = 1; hmi < nunits; hmi++)
		if (units[hmi].health > 0 && regionat(units[hmi].x, units[hmi].y) == area
		    && !isfemale(&units[hmi]))
			return 1;
	return 0;
}

/* the turn of every unit: mode 0 skips the player, mode 2 moves the player alone */
int
rununits(short mode)
{
	short i;

	utdoor = 0;
	utwalk = 0;
	utmode = 0;
	utcow = 0;
	utdog = 0;
	utpig = 0;
	uthorse = 0;
	if (units[0].mode == 0x14 && units[0].bleeding > 5 && !lostcontrol && ((units[0].modef & 2) || randn(100) < 20)) {
		if (mapflags[units[0].y * 150 + units[0].x] & 0x80)
			units[0].modef |= 2;
		else if (addblood(units[0].x, units[0].y, 0, 0))
			units[0].modef &= ~2;
	}
	if (lostcontrol && units[0].berserk && randn(100) < 20 && !playing(0x1b) && !playing(0x1c)) {
		uttmp = randn(100);
		if (uttmp < 50)
			queuesound(0x1b);
		else
			queuesound(0x1c);
	}
	if (mode)
		curi = 0;
	else
		curi = 1;
	if (mode != 2)
		challenger = 0;
	for (; curi < nunits && units[0].health > 0; curi++) {
		curu = &units[curi];
		if (curu->x == 0 && curu->y == 0)
			continue;
		uthere = curu->x + curu->y * 150;
		utpdx = playerx - curu->x;
		utpdy = playery - curu->y;
		utseen = inview(curu->x, curu->y);
		if (utseen)
			plotsight(curmap, curi);
		utwalk = 0;
		utmode = 0;
		if (curu->idlewait-- <= 0 && displayed(curu)) {
			uttmp = randn(100);
			curu->idlestep = curu->idlepose = uttmp / 20;
			curu->idlewait = randn(15) + 4;
			checkposes(curu);
		}
		if (curunit == curi && talkactive) {
		special:
			if (curu->pose == 10)
				;
			else {
				curu->pose = 0;
				if (partof(curu)->anim == 8 && (cur == 0xe || cur == 0xf || cur == 0x10))
					stopmusic();
				turnto(curu, playerx, playery);
			}
			curu->frame = 0;
			continue;
		}
		if (curi && (merciful == curi || pleader == curi || talker == curi))
			goto special;
		if (curu->health <= 0 || curu->asleep)
			continue;
		if (iswild(curu)) {
			curu->sympathy = 0;
			if (curu->mode == 0x14 || curu->mode == 0x15 || curu->mode == 0x19)
				goto react;
			if (!mustflee(curu) && abs(units[curu->modeb].x - curu->x) < 37 && abs(units[curu->modeb].y - curu->y) < 37) {
				setunitmode(curu, 0x14);
				curu->modec = 0xff;
				curu->modeb = 0;
				goto react;
			}
		} else if (isdog(curu)) {
			if (curu->lord) {
				curu->bewitcher = curu->lord;
				curu->clan = 0;
				if (units[curu->lord].mode == 0x14) {
					setunitmode(curu, 0x14);
					curu->modeb = units[curu->lord].modeb;
					curu->modec = units[curu->lord].modec;
					if (curu->modeb == 0 || curu->modec == 0)
						curu->sympathy = 0;
					goto retaliate;
				}
			}
			curu->sympathy = 2;
		} else
			goto group;
		if (curu->mode == 8 || curu->mode == 0x5a)
			;
		else if (gamehour > 22 || gamehour < 5)
			setunitmode(curu, 0x5a);
		else
			setunitmode(curu, 7);
	group:
		if (curu->leader) {
			if (units[curu->leader].health <= 0) {
				substitute(&units[curu->leader]);
				if (curu->leader == 0)
					goto state;
			}
			if (units[curu->leader].mode == 0x14 || curu->leader == curunit)
				;
			else {
				if (gamehour == 20) {
					if (curu->x == curu->sched[3][22] && curu->y == curu->sched[4][22]) {
						curu->mode = 0x5a;
						curu->modea = curu->sched[1][22];
						curu->modeb = 0;
						setstep(curu);
						goto mark;
					}
					uttmp = regionat(curu->x, curu->y);
					if (uttmp && regionat(curu->sched[3][22], curu->sched[4][22]) == uttmp) {
						if (route(curu, curu->sched[3][22], curu->sched[4][22]))
							goto follow;
						goto stuck;
					}
				} else if (gamehour < 6 || gamehour > 20)
					goto state;
				if ((curu->mode == 7 || curu->mode == 8)
				    && (units[curu->leader].mode == 3 || units[curu->leader].mode == 8)) {
					if (provat(curu->x, curu->y) == provat(curu->sched[3][gamehour], curu->sched[4][gamehour])) {
						uta = abs(units[curu->leader].x - curu->x);
						utb = abs(units[curu->leader].y - curu->y);
						if (units[curu->leader].mode == 8) {
							if (uta < 2 && utb < 2) {
								utwalk = 0;
								goto mark;
							}
						} else {
							uttmp = regionat(units[curu->leader].x, units[curu->leader].y);
							if (uta <= 7 && utb <= 7
							    && (!uttmp || regionat(curu->x, curu->y) == uttmp)) {
								curu->mode = 7;
								goto state;
							}
						}
					}
					if (route(curu, units[curu->leader].x, units[curu->leader].y)) {
				follow:
						curu->mode = 8;
						if (curu->type == 0x23 && utseen)
							utcow = 2;
						utwalk = 1;
						goto move;
					}
					goto stuck;
				}
			}
		}
	state:
		if (curu->fleeing)
			goto flee;
		if (curu->bewitched) {
		retaliate:
			if (curu->modeb == curi)
				curu->modeb = 0xff;
			if (curu->modec == curi)
				curu->modec = 0xff;
			if (curu->modeb == 0xff) {
				curu->modeb = curu->modec;
				curu->modec = 0xff;
			}
			if (curu->modeb == 0xff && units[curu->bewitcher].mode == 0x14) {
				uta = units[curu->bewitcher].modeb;
				utb = units[curu->bewitcher].modec;
				if (uta == curi)
					uta = 0xff;
				if (utb == curi)
					utb = 0xff;
				if (uta == 0xff) {
					uta = utb;
					utb = 0xff;
				}
				curu->modeb = uta;
				curu->modec = utb;
			}
			if (curu->modeb == 0xff && curu->modec == 0xff) {
				curu->nextx = units[curu->bewitcher].x;
				curu->nexty = units[curu->bewitcher].y;
				utwalk = 1;
				goto move;
			}
			goto attack;
		}
	react:
		if (curu->sympathy <= 0 && (utseen || cansee(curu->x, curu->y)) && !curu->asleep
		    && curu->mode != 0x14 && curu->mode != 0x15 && curu->mode != 0x19) {
			if (curu->asleep || curu->mode == 0x5a) {
				if (abs(utpdx) >= 3 || abs(utpdy) >= 3)
					goto sounds;
			} else if (mustflee(curu) && !curu->plea) {
				setunitmode(curu, 0x19);
				goto flee;
			}
			if (curu->talkflags & 6) {
				if (curu->talkflags & 4)
					duel(curu, 1);
				else
					duel(curu, 0);
			} else
				provoke(units, curu, 1, 7, 0);
		}
	sounds:
		if (curu->mode != 0x5a) {
			if (curu->type == 0x23 && utseen) {
				if (utcow == 2)
					;
				else if (curu->mode == 8)
					utcow = 2;
				else
					utcow = 1;
			} else if (curu->type == 0x24 && utseen)
				utpig = 1;
			else if (curu->type == 0x25 && utseen && curu->mode != 0x14 && curu->mode != 0x19) {
				utdog = 1;
				if (curu->x + curu->dirx == playerx && curu->y + curu->diry == playery
				    || curu->dirx == 0 && curu->x - 1 == playerx && curu->y + curu->diry == playery
				    || curu->dirx == 0 && curu->x + 1 == playerx && curu->y + curu->diry == playery
				    || curu->diry == 0 && curu->x + curu->dirx == playerx && curu->y - 1 == playery
				    || curu->diry == 0 && curu->x + curu->dirx == playerx && curu->y + 1 == playery)
					utdog = 2;
			} else if (curu->type == 0x30 && utseen && curu->mode != 0x19)
				uthorse = 1;
		}
		if (curu == units && !lostcontrol)
			;
		else {
			if (curu->mode == 0x19 || curu->mode == 0x14 || curu->mode == 0x15) {
				sorttargets(curu);
				if (!engaged(curu))
					goto resched;
			} else if (curu->mode == 0x16 && endspar(curu))
				goto resched;
		}
		if (isanimal(curu) && curu->mode != 0x19 && curu->mode != 0x14 && curu->mode != 0x15 && !iswild(curu)) {
			if (curu->sympathy < 1)
				curu->sympathy = 1;
			if (utseen && (curu->sympathy < 2 || (curu->traits & 0x20) || curu->lord == 0 && curu->type != 0x23)
			    && abs(utpdx) < 4 && abs(utpdy) < 4) {
				setunitmode(curu, 0x19);
				curu->modeb = 0;
				curu->modee = 10;
				utwalk = 1;
				curu->nextx = playerx;
				utmode = 1;
				curu->nexty = playery;
				goto move;
			}
		}
		if (curu->holdtime + hourtime > gametime || isbusy(curu))
			;
		else {
		resched:
			if (atpost(curu, gamehour)) {
				if (!ontask(curu, gamehour))
					applyhour(curu, gamehour);
			} else if (curu->sched[3][gamehour] != playerx || curu->sched[4][gamehour] != playery
			    || curu->mode != 8 || curu->modea == 0)
				gopost(curu);
		}
		if (curi == 0 && !(lostcontrol && (units[0].pose == 0 || units[0].pose > 9)))
			goto done;
		switch (curu->mode) {
		case 1:
		case 3:
		case 7:
			if ((cur == 0xe || cur == 0xf || cur == 0x10) && partof(curu)->anim == 8) {
				utwalk = 0;
				break;
			}
			if (utseen && (uttmp = fightnear(curu, &uta)) != 0) {
				curu->nextx = units[uttmp].x;
				curu->nexty = units[uttmp].y;
				curu->modee = 0;
				if (uta <= 2) {
					utwalk = 1;
					utmode = 1;
					break;
				}
				utwalk = 0;
				utmode = 0;
				stepto(curu, curu->nextx, curu->nexty);
				break;
			}
			if (curu->mode == 3) {
				if (isherder(curu) && gamehour >= 6 && gamehour < 20) {
					curu->modee = 0;
					if (randn(100) < 4) {
						roam(curu);
						utwalk = 1;
					}
					break;
				}
				if (!scheduled(curu) || regionat(curu->sched[3][gamehour], curu->sched[4][gamehour])) {
					if (curu->modee == 0x11) {
						curu->nextx = points[curu->modeg].x[curu->modec];
						curu->nexty = points[curu->modeg].y[curu->modec];
						if (curu->nextx == 0 || curu->nexty == 0) {
						reset:
							curu->modee = 0;
							goto idle;
						}
						uttmp = curu->nexty * 150 + curu->nextx;
						uta = abs(curu->nextx - curu->x);
						utb = abs(curu->nexty - curu->y);
						utdx = points[curu->modeg].dx[curu->modec];
						utdy = points[curu->modeg].dy[curu->modec];
						if (mapflags[uttmp] & 0x400)
							goto newplace;
						if ((utdx || utdy) && (uta >= 1 || utb >= 1)
						    && (map[uttmp] & 0x4000)
						    && (!(mapflags[uttmp] & 0x40) || maysit(curu))
						    && !(mapflags[uttmp] & 4))
							goto walk;
						if (uta <= 1 && utb <= 1) {
							curu->modee = 0x12;
							cleartrail(curu);
							if (utdx || utdy) {
								curu->dirx = utdx;
								curu->diry = utdy;
							}
							break;
						}
					walk:
						if (route(curu, curu->nextx, curu->nexty)) {
							utwalk = 1;
							break;
						}
						if (curu->nextx == 0 || curu->nexty == 0)
							goto reset;
						goto stuck;
					}
					if ((curu->modee == 0x12 || curu->modee == 0x13)
					    && (curu->cellmate != 0xff || (mapflags[uthere] & 0x400) || curu->incoming != 0xff)) {
						curu->modee = 0;
						goto idle;
					}
					if (curu->modee == 0x12) {
						utdx = points[curu->modeg].dx[curu->modec];
						utdy = points[curu->modeg].dy[curu->modec];
						if (utdx || utdy) {
							curu->dirx = utdx;
							curu->diry = utdy;
						} else {
							curu->nextx = points[curu->modeg].x[curu->modec];
							curu->nexty = points[curu->modeg].y[curu->modec];
							stepto(curu, curu->nextx, curu->nexty);
						}
						utfx = curu->nextx + utdx;
						utfy = utdy + curu->nexty;
					open:
						utcell = utfy * 150 + utfx;
						uttmp = topitem(utcell);
						if (uttmp && classof(mapitems[uttmp] & 0xfff) == 0x59
						    && !isfirewood(mapitems[uttmp])) {
							if ((curu->mode == 1 || curu->mode == 3) && curu->goalx == 0xff && !ownerat(utfx, utfy))
								curu->modef = 0;
							else if (setlit(&mapitems[uttmp], 1)) {
								setsmoke(utcell - 151, 1);
								updatecell(utcell);
								if (utseen)
									calctorch();
								curu->goalx = 0xff;
							}
						}
						if (curu->mode != 1 && curu->mode != 3)
							break;
						if (curu->modef) {
							curu->modef--;
							break;
						}
					}
					if (curu->modee == 0x13 && regionat(curu->x, curu->y) && curu->modef)
						curu->modef--;
					else {
					newplace:
						curu->modee = 0;
						curu->modeg = postregion(curu);
						curu->goalx = 0;
						if (!curu->modeg)
							curu->modeg = nearregion(curu->x, curu->y);
						if (!curu->modeg) {
							curu->modee = 0x13;
							curu->modef = 0xf;
							goto idle;
						}
						curu->modec = idlepoint(curu, curu->modeg, !regionat(curu->sched[3][gamehour], curu->sched[4][gamehour]));
						if (curu->modec == 0xff) {
							curu->modee = 0x13;
							if (isservant(curu))
								curu->modef = 2;
							else if (issmith(curu))
								curu->modef = randn(10) + 2;
							else
								curu->modef = randn(25) + 2;
							curu->modec = 0;
						} else {
							curu->modee = 0x11;
							curu->modef = points[curu->modeg].stay[curu->modec];
							curu->modef += randn(curu->modef);
							cleartrail(curu);
						}
						break;
					}
				}
			}
		idle:
			if (curu->restturns && curu->restturns < 0x2c)
				break;
			if (curu->cellmate != 0xff || (mapflags[uthere] & 0x400))
				utwalk = 1;
			else if (curu->incoming != 0xff) {
				cleartrail(curu);
				utwalk = 1;
			}
			if (curu->mode == 7 && isanimal(curu) && !isdog(curu) && terrains[map[uthere] & 0xfff]) {
				utwalk = 1;
				utmode = 0;
				break;
			}
			if (curu->pose == 10 || curu->mode == 7) {
				if (randn(100) < 5)
					utwalk = 1;
			} else if (randn(100) < 16)
				utwalk = 1;
			if (isanimal(curu))
				goto wander;
			uta = regionat(curu->x, curu->y);
			utb = 0xff;
			utc = 0;
			if (isservant(curu) && (utb = curu->lord) != 0xff)
				utc = 1;
			else if (!isfemale(curu) && randn(100) < 33 && (utb = womannear(curu, 2, uta)) != 0xff)
				;
			// FIXED: ownerat gives 0 off a site as on the hero's land, and the test takes both for unit 0, so idle people walk up to the hero in the wild. Take 0 for him on a site alone, and leave 0xff when no one is found.
#if defined(FEATURE_FIXES)
			else if (uta != 0 && (utb = regions[uta].lord) != 0xff)
				;
			else if (uta == 0 && (utb = ownerat(curu->x, curu->y)) != 0xff
			    && (utb != 0 || provat(curu->x, curu->y) != 0)
			    && postdist(curu, &units[utb]) <= 10)
				;
			else if (!isfemale(curu) && (utb = womannear(curu, 3, uta)) != 0xff)
				utc = 2;
			else
				utb = 0xff;
#else
			else if (uta != 0 && (utb = regions[uta].lord) != 0xff)
				;
			else if (uta == 0 && (utb = ownerat(curu->x, curu->y)) != 0xff
			    && postdist(curu, &units[utb]) <= 10)
				;
			else if (!isfemale(curu) && (utb = womannear(curu, 3, uta)) != 0xff)
				utc = 2;
#endif
			if (utb != 0xff && utb != curi
			    && regionat(units[utb].x, units[utb].y) == uta) {
				curu->nextx = units[utb].x + units[utb].dirx;
				curu->nexty = units[utb].y + units[utb].diry;
				utmode = 0;
				if (!isservant(curu)) {
					uta = celldist(curu->x, curu->y, curu->nextx, curu->nexty);
					if (uta < 3 && randn(100) < 80) {
						utwalk = 0;
						stepto(curu, curu->nextx, curu->nexty);
						break;
					}
					if (units[utb].mode == 3 && units[utb].modee == 0x11)
						;
					else if (uta >= 3 && curu->pose != 10
					    && postdist(curu, &units[utb]) <= 6 && randn(100) < 66) {
						stepto(curu, curu->nextx, curu->nexty);
						utwalk = 1;
						break;
					}
				}
			} else {
			wander:
				roam(curu);
				utmode = 0;
				break;
			}
			if (utc == 1 || utc == 2 && randn(100) < 50 || randn(100) < 33) {
				uta = curu->dirx;
				utb = curu->diry;
				stepto(curu, curu->nextx, curu->nexty);
				if (curu->dirx == -uta || curu->diry == -utb) {
					curu->dirx = uta;
					curu->diry = utb;
				}
			}
			break;
		case 0x19:
		flee:
			// FIXED: a unit that flees from no one, as a hero whom Frighten leaves without a foe once tickstatus sets modeb to 0xff, reads units[255], 46 KB past the 150 units. Take it for a foe out of reach.
#ifdef FEATURE_FAULTS
			if (curu->modeb == 0xff) {
				makepeace(curu, units);
				break;
			}
#endif
			uta = abs(units[curu->modeb].x - curu->x);
			utb = abs(units[curu->modeb].y - curu->y);
			if (uta > 50 || utb > 50) {
				makepeace(curu, units);
				break;
			}
			if (curu->mode == 0x19 && curu->health <= kindof(curu)->maxhealth * 2 / 3 && playmode != 1
			    && (uta > 1 || utb > 1) && (gametime & 3) == 0) {
				utwalk = 0;
				utmode = 0;
				break;
			}
			uta = curu->x - units[curu->modeb].x;
			uta *= 25;
			utwalk = 1;
			uta += curu->x;
			utmode = 0;
			if (uta < 0)
				uta = 0;
			else if (uta >= 150)
				uta = 149;
			utb = curu->y - units[curu->modeb].y;
			utb *= 25;
			utb += curu->y;
			if (utb < 0)
				utb = 0;
			else if (utb >= 150)
				utb = 149;
			if (!route(curu, uta, utb)) {
				utwalk = 1;
				utmode = 1;
				curu->nextx = units[curu->modeb].x;
				curu->nexty = units[curu->modeb].y;
			}
			if (curu->bleeding && ((curu->modef & 2) || randn(100) < 35)) {
				if (mapflags[uthere] & 0x80)
					curu->modef |= 2;
				else if (addblood(curu->x, curu->y, 0, 0))
					curu->modef &= ~2;
			}
			if ((curu->modef & 1) || randn(100) < 15) {
				if (mapflags[uthere] & 0x80) {
					curu->modef |= 1;
					break;
				}
				utblood = findclass(uthere, 0x67);
				if (curu->offhand) {
					additem(uthere, randlook(curu->offhand));
					curu->offhand = 0;
				} else if (curu->items[0] && (!utblood || !isfood(curu->items[0]))) {
					additem(uthere, randlook(curu->items[0]));
					curu->items[0] = 0;
				} else if (curu->items[1] && (!utblood || !isfood(curu->items[1]))) {
					additem(uthere, randlook(curu->items[1]));
					curu->items[1] = 0;
				} else if (curu->items[2] && (!utblood || !isfood(curu->items[2]))) {
					additem(uthere, randlook(curu->items[2]));
					curu->items[2] = 0;
				} else if (curu->hand) {
					additem(uthere, randlook(curu->hand));
					if (curu->belt) {
						curu->hand = curu->belt;
						curu->belt = 0;
					} else
						curu->hand = 0;
				} else if (curu->belt) {
					additem(uthere, randlook(curu->belt));
					curu->belt = 0;
				} else
					break;
				curu->modef &= ~1;
				calcstats(curu);
			}
			break;
		case 0x15:
			curu->mode = 0x14;
			if (curu->modeb != 0xff) {
				curu->nextx = units[curu->modeb].x;
				curu->nexty = units[curu->modeb].y;
				provoke(&units[curu->modeb], curu, 3, 7, 0);
			} else {
				curu->nextx = curu->x;
				curu->nexty = curu->y + 1;
			}
			uta = abs(curu->nextx - curu->x);
			utb = abs(curu->nexty - curu->y);
			turnto(curu, curu->nextx, curu->nexty);
			break;
		case 0x14:
		attack:
			if (curu->mode == 0x14 && curu->bleeding > 5 && ((curu->modef & 2) || randn(100) < 20)) {
				if (mapflags[uthere] & 0x80)
					curu->modef |= 2;
				else if (addblood(curu->x, curu->y, 0, 0))
					curu->modef &= ~2;
			}
			if (curu->hand == 0) {
				for (i = 0; i < 3 && curu->hand == 0; i++)
					if (isweapon(curu->items[i])) {
						curu->hand = curu->items[i];
						curu->items[i] = 0;
					}
				calcstats(curu);
			}
			if (curu->modeb == 0xff) {
			noenemy:
				curu->nextx = curu->x;
				curu->nexty = curu->y;
				utwalk = 1;
				utmode = 0x25;
				break;
			}
			if (units[curu->modeb].health <= 0) {
				curu->modeb = 0xff;
				goto noenemy;
			}
			if (allied(curu, &units[curu->modeb])) {
				curu->modeb = 0xff;
				if (allied(curu, &units[curu->modec]))
					curu->modec = 0xff;
				goto noenemy;
			}
			if (!curu->bewitched)
				provoke(&units[curu->modeb], curu, 4, 7, 0xff);
			uta = abs(units[curu->modeb].x - curu->x);
			utb = abs(units[curu->modeb].y - curu->y);
			if (!curu->bewitched && !curu->berserk && (uta > 50 || utb > 50)) {
				makepeace(curu, 0);
				break;
			}
			if (!(uta <= 1 && utb <= 1) && uta <= 2 && utb <= 2
			    && isthrowing(curu->hand) && hassecond(curu)) {
				curu->nextx = units[curu->modeb].x;
				curu->nexty = units[curu->modeb].y;
				if (clearsight(curu->x, curu->y, curu->nextx, curu->nexty))
					goto shoot;
			}
			if (!route(curu, units[curu->modeb].x, units[curu->modeb].y))
				goto stuck;
			if (curu->nextx != units[curu->modeb].x || curu->nexty != units[curu->modeb].y) {
				utwalk = 1;
				break;
			}
			if (curu->cellmate != 0xff) {
				utwalk = 1;
				break;
			}
			if (curu->incoming != 0xff) {
				cleartrail(curu);
				// FIXED: a unit asked to step aside with no free step loses its blow, on every pass that the unit behind it stays stuck. Step aside when a step is free, and strike otherwise.
#if defined(FEATURE_FIXES)
				freesteps(curu);
				if (nfreedirs) {
					utwalk = 1;
					break;
				}
#else
				utwalk = 1;
				break;
#endif
			}
			uta = abs(curu->nextx - curu->x);
			utb = abs(curu->nexty - curu->y);
			if (uta <= 1 && utb <= 1) {
				if (isthrowing2(curu->hand) && curu->belt) {
					additem(uthere, curu->hand);
					curu->hand = curu->belt;
					curu->belt = 0;
					calcstats(curu);
					break;
				}
				if (spellset(curu) == 1 && !isanimal(&units[curu->modeb])) {
					turnto(curu, curu->nextx, curu->nexty);
					if (castspell(curu, curu->modeb) != -1)
						break;
				}
				goto shoot;
			}
			if (spellset(curu)) {
				if (uta > 4 || utb > 3) {
					utwalk = 1;
					break;
				}
				if (!isanimal(&units[curu->modeb])) {
					turnto(curu, curu->nextx, curu->nexty);
					if (castspell(curu, curu->modeb) != -1)
						break;
				}
			}
			if (isfemale(curu)) {
				if (uta > 4 || utb > 3) {
					utwalk = 1;
					break;
				}
				if (spellset(curu) != 1 && blessally(curu))
					break;
			}
			if (isthrowing(curu->hand) && hassecond(curu)) {
				if (uta > 4 || utb > 3) {
					utwalk = 1;
					break;
				}
				if (units[curu->modeb].pose == 1
				    && abs(units[curu->modeb].x + units[curu->modeb].dirx - curu->x) <= abs(units[curu->modeb].x - curu->x)
				    && abs(units[curu->modeb].y + units[curu->modeb].diry - curu->y) <= abs(units[curu->modeb].y - curu->y)) {
					if (clearsight(curu->x, curu->y, units[curu->modeb].x + units[curu->modeb].dirx, units[curu->modeb].y + units[curu->modeb].diry))
						goto shoot;
					utwalk = 1;
					break;
				}
				if (clearsight(curu->x, curu->y, curu->nextx, curu->nexty))
					goto shoot;
				utwalk = 1;
				break;
			}
			if (uta > 1 || utb > 1) {
				utwalk = 1;
				utmode = 0;
				break;
			}
		shoot:
			turnto(curu, curu->nextx, curu->nexty);
			if (talkactive && curu->modeb == curunit)
				break;
			if (curu == units) {
				targetx = units[curu->modeb].x;
				targety = units[curu->modeb].y;
				targetidx = curu->modeb;
				askattack();
			}
			strike(curu, &units[curu->modeb]);
			break;
		case 0x16:
			if (curu->modeb == 0xff || curu->bewitched || curu->berserk)
				break;
			uta = abs(units[curu->modeb].x - curu->x);
			utb = abs(units[curu->modeb].y - curu->y);
			if (!route(curu, units[curu->modeb].x, units[curu->modeb].y) || uta > 10 || utb > 10)
				break;
			if (uta > 1 || utb > 1) {
				utwalk = 1;
				break;
			}
			if (curu->cellmate != 0xff) {
				utwalk = 1;
				break;
			}
			if (curu->incoming == 0xff)
				goto shoot;
			cleartrail(curu);
			// FIXED: as above, shoot when no step is free.
#if defined(FEATURE_FIXES)
			freesteps(curu);
			if (!nfreedirs)
				goto shoot;
#endif
			utwalk = 1;
			break;
		case 8:
			if (!curu->modea) {
				if (postblocked(curu, gamehour) && !otherseat(curu, gamehour)) {
					applyhour(curu, gamehour);
					break;
				}
			}
			if (curu->modea) {
				if (curu->sched[3][gamehour] == playerx && curu->sched[4][gamehour] == playery) {
					if (curu->x == curu->modeb && curu->y == curu->modec) {
						applyhour(curu, gamehour);
						break;
					}
					if (!route(curu, curu->modeb, curu->modec))
						goto stuck;
					utwalk = 1;
					break;
				}
				curu->modea = 0;
				curu->modeb = 0;
				curu->modec = 0;
			}
			if (route(curu, curu->sched[3][gamehour], curu->sched[4][gamehour])) {
				if (curu->x == curu->sched[3][gamehour] && curu->y == curu->sched[4][gamehour]) {
					applyhour(curu, gamehour);
					break;
				}
				utwalk = 1;
				break;
			}
		stuck:
			uta = unitat(curu->sched[3][gamehour], curu->sched[4][gamehour]);
			/* a defect: the original tests == 0xff where it means != 0xff */
			// FIXED: with uta 0xff the test reaches units[255], past the 150 units, and it holds for no other unit. Test != 0xff, as the code means.
			// FIXED: with uta 0xff the test reaches units[255], past the 150 units, and it holds for no other unit. Leave it false, as for every other unit.
#if defined(FEATURE_FIXES)
			if (uta != 0xff && uta && units[uta].incoming == 0xff)
#elif defined(FEATURE_FAULTS)
			if (0)
#else
			if (uta == 0xff && uta && units[uta].incoming == 0xff)
#endif
				units[uta].incoming = curi;
			roam(curu);
			if (curu->cellmate != 0xff)
				utwalk = 1;
			else if (curu->incoming != 0xff) {
				cleartrail(curu);
				utwalk = 1;
			} else if (randn(100) < 50)
				utwalk = 1;
			break;
		case 0x5a:
			curu->pose = 0;
			if (utseen && (uttmp = fightnear(curu, &uta)) != 0) {
				curu->mode = 0;
				curu->nextx = units[uttmp].x;
				curu->nexty = units[uttmp].y;
				utwalk = 0;
				utmode = 0;
				stepto(curu, curu->nextx, curu->nexty);
				setfacing(curu);
				break;
			}
			if (curu->modea != 0xff)
				setstep(curu);
			cleartrail(curu);
			break;
		case 0:
		case 2:
		case 0x32:
			setstep(curu);
			break;
		case 4:
			if (utseen && curu->modeb == 2)
				provoke(units, curu, 4, 8, 0);
			else
				setstep(curu);
			break;
		case 0xf:
			if (curu->modec != 0xff && curu->moded != 0xff) {
				uta = abs(curu->modec - curu->x);
				utb = abs(curu->moded - curu->y);
				if (uta < 3 && utb < 3) {
					if (curu->modee == 0xff) {
						if (uta < 2 && utb < 2) {
							utfx = curu->modec;
							utfy = curu->moded;
							setunitmode(curu, 0);
							turnto(curu, utfx, utfy);
							goto open;
						}
					} else {
						setunitmode(curu, 0);
						turnto(curu, (short)curu->modec, (short)curu->moded);
						break;
					}
				}
				if (route(curu, curu->modec, curu->moded)) {
					utwalk = 1;
					break;
				}
				utmode = 1;
				utwalk = 1;
				break;
			}
			if (curu->modeb == 0xff || curu->bewitched || curu->berserk)
				break;
			if (units[0].mode == 0x5a || units[0].asleep) {
				curu->modea = 0;
				curu->modeb = 0;
				setunitmode(curu, 0);
				break;
			}
			uta = abs(utpdx);
			utb = abs(utpdy);
			if (uta < 3 && utb < 3 && seesunit(curu)) {
				if (talker || talkscene || talkactive)
					break;
				talker = indexof(curu);
				talkscene = curu->modee;
				talkscene <<= 8;
				talkscene |= curu->modef;
				break;
			}
			if (route(curu, playerx, playery)) {
				utwalk = 1;
				break;
			}
			utmode = 1;
			utwalk = 1;
			break;
		}
	move:
		if (utwalk) {
			uttmp = takestep(curu, utmode);
			if (uttmp == 2)
				utdoor = 1;
			else if (uttmp == 0)
				goto mark;
			curu->incoming = 0xff;
			if (curu == units) {
				cleartexts();
				mapflags[units[0].x + units[0].dirx + (units[0].y + units[0].diry) * 150] &= ~4;
				advance(units[0].dirx, units[0].diry);
			}
			goto done;
		}
	mark:
		if (curu != units)
			mapflags[curu->x + curu->y * 150] |= 4;
	done:
		if (curu->pose >= 0xb && curu->pose <= 0x12 || curu->pose >= 0x14 && curu->pose <= 0x1b)
			curu->pose = 0;
		sitdown(curu);
		if (!passmode && !(curu == units && playermoving()) && displayed(curu) && isidle(curu)
		    && (utanim = partof(curu)->anim) != 0 && utanim != 0xff && curu->pose != 1) {
			if (curu->pose == 10) {
				utc = randn(100);
				if (curu->mode == 0 && utanim == 1 && utc < 7) {
					if (curu->dirx == anims[utanim].a[0].dx && curu->diry == anims[utanim].a[0].dy)
						curu->pose = 0xb;
					else if (curu->dirx == anims[utanim].a[1].dx && curu->diry == anims[utanim].a[1].dy)
						curu->pose = 0xc;
				} else {
					if (curu != units && curu->mode == 0) {
						utc *= 2;
						utc /= 3;
					}
					if (utc > anims[utanim].chancea)
						goto still;
					for (uta = 0; uta < 8; uta++)
						if (curu->dirx == anims[utanim].a[uta].dx && curu->diry == anims[utanim].a[uta].dy
						    && randn(100) < anims[utanim].a[uta].chance)
							curu->pose = uta + 0xb;
				}
				if (curu == units && (curu->pose == 0xb || curu->pose == 0xc) && !(gamehour >= 12 && gamehour <= 13)
				    && gamehour < 19)
					curu->pose = 0;
				checksitanim(curu);
			} else {
				if (partof(curu)->anim == 6 && (curu->mode == 2 || curu->mode == 3 && curu->modee == 0x12)) {
					if (findtype((curu->y + 1) * 150 + curu->x, 0x96c)) {
						if (randn(100) < 60) {
							curu->dirx = 0;
							curu->diry = 1;
							curu->pose = 0x18;
							goto anim;
						}
					} else if (findtype(curu->x + 1 + curu->y * 150, 0x96e) && randn(100) < 60) {
						curu->dirx = 1;
						curu->diry = 0;
						curu->pose = 0x19;
						goto anim;
					}
				} else if (partof(curu)->anim == 2 && (curu->mode == 2 || curu->mode == 3 && curu->modee == 0x12)) {
					if (findtype(curu->y * 150 + curu->x, 0x994)) {
						if (randn(100) < 80) {
							curu->dirx = 0;
							curu->diry = -1;
							curu->pose = 0x18;
							goto anim;
						}
					} else if (findtype(curu->y * 150 + curu->x, 0x98a) && randn(100) < 80) {
						curu->dirx = -1;
						curu->diry = 0;
						curu->pose = 0x19;
						goto anim;
					}
				}
				if ((cur == 0xe || cur == 0xf || cur == 0x10) && partof(curu)->anim == 8) {
					if (curu->dirx == anims[utanim].b[3].dx && curu->diry == anims[utanim].b[3].dy)
						curu->pose = 0x17;
					else if (curu->dirx == anims[utanim].b[2].dx && curu->diry == anims[utanim].b[2].dy)
						curu->pose = 0x16;
					else if (randn(100) < 50) {
						curu->dirx = anims[utanim].b[2].dx;
						curu->diry = anims[utanim].b[2].dy;
						curu->pose = 0x16;
					} else {
						curu->dirx = anims[utanim].b[3].dx;
						curu->diry = anims[utanim].b[3].dy;
						curu->pose = 0x17;
					}
				} else {
					utc = randn(100);
					if (utc <= anims[utanim].chanceb) {
						for (uta = 0; uta < 8; uta++)
							if (curu->dirx == anims[utanim].b[uta].dx && curu->diry == anims[utanim].b[uta].dy
							    && randn(100) < anims[utanim].b[uta].chance)
								curu->pose = uta + 0x14;
					anim:
						checkstandanim(curu);
						if (utseen && !palblack && !passmode) {
							if (partof(curu)->anim == 8 && (curu->pose == 0x16 || curu->pose == 0x17)) {
								if (cur == 0)
									music(randn(3) + 0xe, 0);
							} else if (partof(curu)->anim == 6 && (curu->pose == 0x18 || curu->pose == 0x19))
								queuesound(0x2b);
						}
					}
				}
			}
		}
	still:
		if (curu->pose == 10 || curu->pose >= 0xb && curu->pose <= 0x12) {
			curu->awaketurns = 0;
			if (curu->restturns < 0xff)
				curu->restturns++;
		} else {
			curu->restturns = 0;
			if (curu->awaketurns < 0xff)
				curu->awaketurns++;
		}
		if (mode == 2)
			break;
	}
	if (gamehour >= 6 && gamehour <= 0x15 && !muted()) {
		if (utcow) {
			if (utcow == 2) {
				if (randn(100) < 25)
					queuesound(0x2f);
			} else if (randn(100) < 4)
				queuesound(0x2f);
		}
		if (utpig && randn(100) < 4)
			queuesound(0x31);
		if (utdog && barkat + 20 < gametime) {
			if (utdog == 2) {
				if (randn(100) < 70) {
					queuesound(0x30);
					barkat = gametime;
				}
			} else if (randn(100) < 2) {
				queuesound(0x30);
				barkat = gametime;
			}
		}
		if (uthorse && randn(100) < 3)
			queuesound(0x32);
	}
	return utdoor;
}

unsigned long
statesum(void)
{
	int k;

	sumv = 0;
	for (k = 0; k < 150 * 150; k++) {
		sumj = map[k];
		if (sumj & 0x1000) {
			for (sumj >>= 16;; sumj++) {
				sumv += mapitems[sumj] & 0xffff0fff;
				if ((mapitems[sumj] & 0x8000) == 0)
					break;
			}
		}
	}
	for (sumi = 5; sumi < nrealms + 5; sumi++)
		for (sumj = 0; sumj < 50; sumj++)
			sumv += realms[sumi].items[sumj] & 0xffff0fff;
	return sumv;
}

int
savegame(void)
{
	// FIXED: savegame writes D0000000.DAT in place, which CREATE_ALWAYS empties at its open, so a write that fails, as on a full disk, leaves a part of the saved game, which the next start reads. Write it whole (openwhole, mainmenu.cpp).
#if defined(FEATURE_FIXES)
	if (!openwhole(&savefh))
		return 0;
#else
	if (!fopenw(&savefh, texts + 0xa435))
		return 0;
#endif
	memset(packbuf, 0, sizeof packbuf);
	saveat = packbuf;
	savex = 0;
	game.savespare = 0;
	memwrite(&nnotes, 2, 1, &saveat);
	memwrite(&notes, 4, nnotes, &saveat);
	memwrite(units, 0x1b8, 1, &saveat);
	memwrite(&game, 0x5f8, 1, &saveat);
	memwrite(realms, 0x158, 5, &saveat);
	memwrite(&gametime, 4, 1, &saveat);
	memwrite(&turncount, 4, 1, &saveat);
	memwrite(&gamehour, 1, 1, &saveat);
	memwrite(&gameday, 2, 1, &saveat);
	memwrite(&regiontag, 2, 1, &saveat);
	memwrite(&sunlight, 2, 1, &saveat);
	memwrite(&lastlight, 2, 1, &saveat);
	memwrite(&actsel, 1, 1, &saveat);
	memwrite(&curmap, 2, 1, &saveat);
	memwrite(&playerx, 4, 1, &saveat);
	memwrite(&playery, 4, 1, &saveat);
	memwrite(&savebyte1, 1, 1, &saveat);
	memwrite(&steptick, 2, 1, &saveat);
	memwrite(&fireframe, 4, 1, &saveat);
	memwrite(&slowframe, 4, 1, &saveat);
	memwrite(&walkedsum, 2, 1, &saveat);
	memwrite(&walkeddx, 2, 1, &saveat);
	memwrite(&walkeddy, 2, 1, &saveat);
	memwrite(&playercell, 2, 1, &saveat);
	/* the original passes savebyte2 twice */
	memwrite(&savebyte2, 1, 1, &saveat);
	memwrite(&savebyte2, 1, 1, &saveat);
	memwrite(&playmode, 4, 1, &saveat);
	memwrite(&panels, 0x280, 1, &saveat);
	memwrite(&panelstack, 0x10, 1, &saveat);
	memwrite(&busyturns, 2, 1, &saveat);
	/* the original passes printbuf twice, with two sizes */
	memwrite(&printbuf, 0x50, 1, &saveat);
	memwrite(&printbuf, 0x64, 1, &saveat);
	memwrite(&sensesleft, 2, 1, &saveat);
	memwrite(&curregion, 2, 1, &saveat);
	memwrite(&notedkind, 1, 1, &saveat);
	memwrite(&notedposx, 4, 1, &saveat);
	memwrite(&notedposy, 4, 1, &saveat);
	memwrite(&noteditem, 4, 1, &saveat);
	memwrite(&notedunit, 2, 1, &saveat);
	memwrite(&notedmap, 2, 1, &saveat);
	memwrite(&playernext, 2, 1, &saveat);
	memwrite(&minmarks, 1, 1, &saveat);
	memwrite(&quickacts, 0xa, 1, &saveat);
	memwrite(&saveword1, 2, 1, &saveat);
	memwrite(&saveword2, 2, 1, &saveat);
	memwrite(&saveword3, 2, 1, &saveat);
	memwrite(&saveword4, 2, 1, &saveat);
	memwrite(&saveword5, 2, 1, &saveat);
	memwrite(&saveword6, 2, 1, &saveat);
	memwrite(&saveword7, 2, 1, &saveat);
	memwrite(&saveword8, 2, 1, &saveat);
	memwrite(&saveword9, 2, 1, &saveat);
	memwrite(&savex, 1, 1, &saveat);
	memwrite(&savebyte3, 1, 1, &saveat);
	memwrite(&stepframe, 2, 1, &saveat);
	memwrite(&knockedout, 1, 1, &saveat);
	memwrite(&bloodtill, 4, 1, &saveat);
	memwrite(&notedtime, 4, 1, &saveat);
	memwrite(&trvfrom, 2, 1, &saveat);
	memwrite(&trvto, 2, 1, &saveat);
	memwrite(&trvleft, 2, 1, &saveat);
	savesum = statesum();
	memwrite(&nmapitems, 2, 1, &saveat);
	memwrite(&savesum, 4, 1, &saveat);
	memwrite(&scenevarlen, 4, 1, &saveat);
	memwrite(&scenevars, scenevarlen, 1, &saveat);
	memwrite(&kinds[19].aptitude, 0x1e, 1, &saveat);
	memwrite(&lefttime, 4, 1, &saveat);
	memwrite(&merciful, 2, 1, &saveat);
	memwrite(&pleader, 2, 1, &saveat);
	memwrite(&skiptick, 2, 1, &saveat);
	memwrite(&hourtime, 4, 1, &saveat);
	memwrite(&lastfail, 2, 1, &saveat);
	memwrite(&acttime, 4, 1, &saveat);
	memwrite(&headframe, 2, 1, &saveat);
	memwrite(&headanim, 2, 1, &saveat);
	memwrite(&headstep, 2, 1, &saveat);
	memwrite(&handframe, 2, 1, &saveat);
	memwrite(&handanim, 2, 1, &saveat);
	memwrite(&handstep, 2, 1, &saveat);
	memwrite(&footframe, 2, 1, &saveat);
	memwrite(&footanim, 2, 1, &saveat);
	memwrite(&footstep, 2, 1, &saveat);
	memwrite(&talker, 2, 1, &saveat);
	memwrite(&talkscene, 2, 1, &saveat);
	memwrite(&combatcause, 1, 1, &saveat);
	memwrite(&lasttick, 2, 1, &saveat);
	memwrite(&blinkclock, 4, 1, &saveat);
	memwrite(&heroblink, 2, 1, &saveat);
	memwrite(&curblink, 2, 1, &saveat);
	memwrite(&heroblinkat, 4, 1, &saveat);
	memwrite(&curblinkat, 4, 1, &saveat);
	memwrite(&actionturn, 2, 1, &saveat);
	memwrite(&shoetime, 4, 1, &saveat);
	memwrite(&shoewarns, 1, 1, &saveat);
	memwrite(&szvars, 0x190, 1, &saveat);
	memwrite(&waterpace, 2, 1, &saveat);
	memwrite(&skillsnow, 1, 1, &saveat);
	memwrite(&combatnow, 1, 1, &saveat);
	memwrite(&savelong, 4, 1, &saveat);
	memwrite(&askedonce, 2, 1, &saveat);
	memwrite(&askedunit, 2, 1, &saveat);
	memwrite(&combatkind, 2, 1, &saveat);
	memwrite(&spotsong, 1, 1, &saveat);
	memwrite(&spotday, 2, 1, &saveat);
	memwrite(&avengers, 2, 1, &saveat);
	memwrite(&victimfemale, 2, 1, &saveat);
	memwrite(&victimname, 2, 1, &saveat);
	memwrite(&victimzone, 2, 1, &saveat);
	memwrite(&vermajor, 4, 1, &saveat);
	memwrite(&verminor, 4, 1, &saveat);
	memwrite(&verflags, 4, 1, &saveat);
	memwrite(&autolevel, 4, 1, &saveat);
	memwrite(&savesum, 4, 1, &saveat);
	memwrite(&showtotal, 1, 1, &saveat);
	memwrite(&tradehints, 1, 1, &saveat);
	savesum = 0;
	for (savep = (unsigned char *)packbuf; savep < (unsigned char *)saveat; savep++)
		savesum += *savep;
	memwrite(&savesum, 4, 1, &saveat);
	for (savep = (unsigned char *)packbuf; savep < (unsigned char *)saveat; savep++)
		*savep ^= 0xff;
	// FIXED: savegame reports success whatever its write and its close did, so a slot's save takes a saved game that the disk refused. Report them, and put the file in place once whole.
#if defined(FEATURE_FIXES)
	{
		int ok;

		ok = filewrite(packbuf, saveat - packbuf, 1, savefh) != 0;
		ok = closewhole(&savefh, texts + 0xa435, ok);
		otherver = 0;
		deletefile(texts + 0xa583);
		return ok;
	}
#else
	filewrite(packbuf, saveat - packbuf, 1, savefh);
	fileclose(&savefh);
	otherver = 0;
	deletefile(texts + 0xa583);
	return 1;
#endif
}

int
loadgame(int n)
{
	if (!fopenr(&loadfh, texts + 0xa435)) {
		enterflag = 0;
		enteritems = 0xff;
		entersum = 1;
		return 0;
	}
	memset(packbuf, 0, sizeof packbuf);
	loadpos = packbuf;
	loadsize = fileread(packbuf, 1, sizeof packbuf, loadfh);
	fileclose(&loadfh);
	if (loadsize == 0)
		return 0;
	for (loadsum = 0; loadsum < loadsize; loadsum++)
		packbuf[loadsum] ^= 0xff;
	loadlang = lang;
	loadtmp = 0;
	memread(&nnotes, 2, 1, &loadpos);
	memset(notes, 0, sizeof notes);
	memread(&notes, 4, nnotes, &loadpos);
	memread(units, 0x1b8, 1, &loadpos);
	memread(&game, 0x5f8, 1, &loadpos);
	memread(realms, 0x158, 5, &loadpos);
	memread(&gametime, 4, 1, &loadpos);
	memread(&turncount, 4, 1, &loadpos);
	memread(&gamehour, 1, 1, &loadpos);
	memread(&gameday, 2, 1, &loadpos);
	memread(&regiontag, 2, 1, &loadpos);
	memread(&sunlight, 2, 1, &loadpos);
	memread(&lastlight, 2, 1, &loadpos);
	memread(&actsel, 1, 1, &loadpos);
	memread(&curmap, 2, 1, &loadpos);
	memread(&playerx, 4, 1, &loadpos);
	memread(&playery, 4, 1, &loadpos);
	memread(&savebyte1, 1, 1, &loadpos);
	memread(&steptick, 2, 1, &loadpos);
	memread(&fireframe, 4, 1, &loadpos);
	memread(&slowframe, 4, 1, &loadpos);
	memread(&walkedsum, 2, 1, &loadpos);
	memread(&walkeddx, 2, 1, &loadpos);
	memread(&walkeddy, 2, 1, &loadpos);
	memread(&playercell, 2, 1, &loadpos);
	/* the original passes savebyte2 twice */
	memread(&savebyte2, 1, 1, &loadpos);
	memread(&savebyte2, 1, 1, &loadpos);
	memread(&playmode, 4, 1, &loadpos);
	memread(&panels, 0x280, 1, &loadpos);
	memread(&panelstack, 0x10, 1, &loadpos);
	memread(&busyturns, 2, 1, &loadpos);
	/* the original passes printbuf twice, with two sizes */
	memread(&printbuf, 0x50, 1, &loadpos);
	memread(&printbuf, 0x64, 1, &loadpos);
	memread(&sensesleft, 2, 1, &loadpos);
	memread(&curregion, 2, 1, &loadpos);
	memread(&notedkind, 1, 1, &loadpos);
	memread(&notedposx, 4, 1, &loadpos);
	memread(&notedposy, 4, 1, &loadpos);
	memread(&noteditem, 4, 1, &loadpos);
	memread(&notedunit, 2, 1, &loadpos);
	memread(&notedmap, 2, 1, &loadpos);
	memread(&playernext, 2, 1, &loadpos);
	memread(&minmarks, 1, 1, &loadpos);
	memread(&quickacts, 0xa, 1, &loadpos);
	memread(&saveword1, 2, 1, &loadpos);
	memread(&saveword2, 2, 1, &loadpos);
	memread(&saveword3, 2, 1, &loadpos);
	memread(&saveword4, 2, 1, &loadpos);
	memread(&saveword5, 2, 1, &loadpos);
	memread(&saveword6, 2, 1, &loadpos);
	memread(&saveword7, 2, 1, &loadpos);
	memread(&saveword8, 2, 1, &loadpos);
	memread(&saveword9, 2, 1, &loadpos);
	memread(&loadtmp, 1, 1, &loadpos);
	memread(&savebyte3, 1, 1, &loadpos);
	memread(&stepframe, 2, 1, &loadpos);
	memread(&knockedout, 1, 1, &loadpos);
	memread(&bloodtill, 4, 1, &loadpos);
	memread(&notedtime, 4, 1, &loadpos);
	memread(&trvfrom, 2, 1, &loadpos);
	memread(&trvto, 2, 1, &loadpos);
	memread(&trvleft, 2, 1, &loadpos);
	memread(&saveditems, 2, 1, &loadpos);
	memread(&savedsum, 4, 1, &loadpos);
	memread(&scenevarlen, 4, 1, &loadpos);
	memread(&scenevars, scenevarlen, 1, &loadpos);
	memread(&kinds[19].aptitude, 0x1e, 1, &loadpos);
	memread(&lefttime, 4, 1, &loadpos);
	memread(&merciful, 2, 1, &loadpos);
	memread(&pleader, 2, 1, &loadpos);
	memread(&skiptick, 2, 1, &loadpos);
	memread(&hourtime, 4, 1, &loadpos);
	memread(&lastfail, 2, 1, &loadpos);
	memread(&acttime, 4, 1, &loadpos);
	memread(&headframe, 2, 1, &loadpos);
	memread(&headanim, 2, 1, &loadpos);
	memread(&headstep, 2, 1, &loadpos);
	memread(&handframe, 2, 1, &loadpos);
	memread(&handanim, 2, 1, &loadpos);
	memread(&handstep, 2, 1, &loadpos);
	memread(&footframe, 2, 1, &loadpos);
	memread(&footanim, 2, 1, &loadpos);
	memread(&footstep, 2, 1, &loadpos);
	memread(&talker, 2, 1, &loadpos);
	memread(&talkscene, 2, 1, &loadpos);
	memread(&combatcause, 1, 1, &loadpos);
	memread(&lasttick, 2, 1, &loadpos);
	memread(&blinkclock, 4, 1, &loadpos);
	memread(&heroblink, 2, 1, &loadpos);
	memread(&curblink, 2, 1, &loadpos);
	memread(&heroblinkat, 4, 1, &loadpos);
	memread(&curblinkat, 4, 1, &loadpos);
	memread(&actionturn, 2, 1, &loadpos);
	memread(&shoetime, 4, 1, &loadpos);
	memread(&shoewarns, 1, 1, &loadpos);
	memread(&szvars, 0x190, 1, &loadpos);
	memread(&waterpace, 2, 1, &loadpos);
	memread(&skillsnow, 1, 1, &loadpos);
	memread(&combatnow, 1, 1, &loadpos);
	memread(&savelong, 4, 1, &loadpos);
	memread(&askedonce, 2, 1, &loadpos);
	memread(&askedunit, 2, 1, &loadpos);
	memread(&combatkind, 2, 1, &loadpos);
	memread(&spotsong, 1, 1, &loadpos);
	memread(&spotday, 2, 1, &loadpos);
	memread(&avengers, 2, 1, &loadpos);
	memread(&victimfemale, 2, 1, &loadpos);
	memread(&victimname, 2, 1, &loadpos);
	memread(&victimzone, 2, 1, &loadpos);
	otherver = 0;
	memread(&loadtmp, 4, 1, &loadpos);
	if (loadtmp != vermajor)
		otherver = 1;
	memread(&loadtmp, 4, 1, &loadpos);
	if (loadtmp != verminor)
		otherver = 1;
	memread(&loadtmp, 4, 1, &loadpos);
	if (loadtmp != verflags)
		otherver = 1;
	memread(&autolevel, 4, 1, &loadpos);
	memread(&loadsize, 4, 1, &loadpos);
	memread(&showtotal, 1, 1, &loadpos);
	memread(&tradehints, 1, 1, &loadpos);
	for (loadp = (unsigned char *)packbuf, loadsum = 0; loadp < (unsigned char *)loadpos; loadp++)
		loadsum += *loadp;
	memread(&loadsize, 4, 1, &loadpos);
	if (afterupdate || afterinst || otherver) {
		enteritems = 0xff;
		entersum = 1;
	} else if (!(enteritems == 0xff && entersum == 1)) {
		enteritems = saveditems;
		entersum = savedsum;
	}
	loadsize = loadsum;
	enterflag = 0;
	setlang(lang);
	if (loadlang != lang) {
		loadstrs();
		loadscript(scriptfile);
	}
	initpanels();
	units[0].part = 0x16;
	calcstats(&units[0]);
	targetkind = 0;
	targetx = 999;
	targety = 999;
	targetitem = 0;
	targetidx = 0;
	return 1;
}

void
addhealth(Unit *u, short n)
{
	if (badunit(u))
		return;
	if (u->health + n >= kindof(u)->maxhealth)
		u->health = kindof(u)->maxhealth;
	else
		u->health += n;
	u->bleeding = 0;
	calcstats(u);
	if (u == units)
		fillgauges();
}

void
purge(Unit *u)
{
	if (badunit(u))
		return;
	u->poison = 0;
	u->drunk = 0;
	if (u == units)
		fillgauges();
}

void
fixitems(void)
{
	Unit *u;
	Realm *r;

	for (fixi = 1; fixi < nmapitems; fixi++)
		fixcoins(&mapitems[fixi]);
	u = units;
	for (fixi = 0; fixi < nunits; fixi++)
		for (fixk = 0; fixk < 3; fixk++)
			fixcoins(&u[fixi].items[fixk]);
	r = realms;
	for (fixi = 0; fixi < nrealms + 5; fixi++)
		for (fixj = 0; fixj < 50; fixj++)
			fixcoins(&r[fixi].items[fixj]);
	for (fixi = 0; fixi < 5; fixi++)
		for (fixj = 0; fixj < 50; fixj++) {
			fixy = realms[fixi].pos[fixj];
			fixx = fixy;
			fixx %= 320;
			fixy /= 320;
			if (fixx < 0)
				fixx = 0;
			if (fixx > 152)
				fixx = 152;
			if (fixy < 0)
				fixy = 0;
			if (fixy > 207)
				fixy = 207;
			fixx += fixy * 320;
			realms[fixi].pos[fixj] = fixx;
		}
}

// FIXED: the first corpse of a cell past item after, for the search of scavenge below
#if defined(FEATURE_FIXES)
static short
nextcorpse(int cell, short after)
{
	short k;

	if (!(map[cell] & 0x1000))
		return 0;
	for (k = map[cell] >> 16;; k++) {
		if (k > after && classof(mapitems[k] & 0xfff) == 0x15)
			return k;
		if (!(mapitems[k] & 0x8000))
			return 0;
	}
}
#endif

void
scavenge(short all)
{
	for (looti = 1; looti < nregions; looti++) {
		if (regions[looti].lord && units[regions[looti].lord].health > 0)
			continue;
		for (lootx = 0; lootx < regions[looti].nunits; lootx++) {
			looty = regions[looti].units[lootx];
			if (looty != 0 && units[looty].health > 0)
				goto alive;
		}
		lightregion(looti, 0);
	alive:
		;
	}
	if (((unsigned char *)recipes)[0x2de])
		return;
	lootarea = mapzone(curmap);
	if (lootarea == 0)
		return;
	for (looty = zones[lootarea].y0; looty <= zones[lootarea].y1; looty++)
		for (lootx = zones[lootarea].x0; lootx <= zones[lootarea].x1; lootx++) {
			if (all == 0 && !passmode && cansee(lootx, looty))
				continue;
			lootcell = looty * 150 + lootx;
			lootn = 0;
			// FIXED: findclass finds a cell's first corpse each time, so a corpse passed over comes back 30 times and those behind it stay. Search past the last corpse passed, and from the start after a take.
#if defined(FEATURE_FIXES)
			lootobj = 0;
			for (;;) {
				lootobj = nextcorpse(lootcell, lootobj);
#else
			for (;;) {
				lootobj = findclass(lootcell, 0x15);
#endif
				if (lootobj == 0 || lootn++ >= 30)
					break;
				lootunit = mapitems[lootobj] >> 16;
				if (lootunit == 0)
					continue;
				if (gameday - units[lootunit].plea < 1)
					continue;
				if ((mapitems[lootobj] & 0x8000) && classof(mapitems[lootobj + 1] & 0xfff) == 0x69)
					continue;
				lootlord = ownerat(lootx, looty);
				lootb = residentnear(lootx, looty);
				if ((!lootlord || units[lootlord].health <= 0) && !lootb)
					continue;
				lootprev = 0;
				if (lootobj > 0 && (mapitems[lootobj - 1] & 0x8000) && classof(mapitems[lootobj - 1] & 0xfff) == 0x55) {
					lootprev = takeitem(lootcell, lootobj - 1);
					lootobj--;
				}
				lootnext = 0;
				if ((mapitems[lootobj] & 0x8000) && classof(mapitems[lootobj + 1] & 0xfff) == 0x3c)
					lootnext = takeitem(lootcell, lootobj + 1);
				takeitem(lootcell, lootobj);
				// FIXED: as above, search from the start after a take.
#if defined(FEATURE_FIXES)
				lootobj = 0;
#endif
				loothow = 1;
				if (hasowned(lootunit)) {
					units[lootunit].home = units[lootunit].mercy;
					lootdest = heirof(&units[lootunit]);
					units[lootunit].home = 0;
				} else if (lootlord)
					lootdest = lootlord;
				else if (lootb) {
					lootdest = lootb;
					if (myservant(&units[lootdest]) || isfemale(&units[lootdest]))
						loothow = 2;
				} else
					lootdest = 0;
				if (lootdest != 0) {
					if (units[lootunit].belt)
						giveitem(lootdest, randlook(units[lootunit].belt), loothow);
					if (units[lootunit].hand)
						giveitem(lootdest, randlook(units[lootunit].hand), loothow);
					if (units[lootunit].offhand)
						giveitem(lootdest, randlook(units[lootunit].offhand), loothow);
					if (units[lootunit].cloak)
						giveitem(lootdest, randlook(units[lootunit].cloak), loothow);
					if (units[lootunit].wrist)
						giveitem(lootdest, randlook(units[lootunit].wrist), loothow);
					if (units[lootunit].neck)
						giveitem(lootdest, randlook(units[lootunit].neck), loothow);
					if (units[lootunit].items[0])
						giveitem(lootdest, randlook(units[lootunit].items[0]), loothow);
					if (units[lootunit].items[1])
						giveitem(lootdest, randlook(units[lootunit].items[1]), loothow);
					if (units[lootunit].items[2])
						giveitem(lootdest, randlook(units[lootunit].items[2]), loothow);
					if (units[lootunit].body)
						giveitem(lootdest, randlook(units[lootunit].body), loothow);
					if (units[lootunit].head)
						giveitem(lootdest, randlook(units[lootunit].head), loothow);
					if (units[lootunit].feet)
						giveitem(lootdest, randlook(units[lootunit].feet), loothow);
					if (units[lootunit].legs)
						giveitem(lootdest, randlook(units[lootunit].legs), loothow);
					if (lootprev)
						giveitem(lootdest, randlook(lootprev), loothow);
					if (lootnext)
						giveitem(lootdest, randlook(lootnext), loothow);
				} else {
					if (lootprev)
						additem(lootcell, lootprev);
					if (units[lootunit].feet)
						additem(lootcell, randlook(units[lootunit].feet));
					if (units[lootunit].legs)
						additem(lootcell, randlook(units[lootunit].legs));
					if (units[lootunit].cloak)
						additem(lootcell, randlook(units[lootunit].cloak));
					if (units[lootunit].wrist)
						additem(lootcell, randlook(units[lootunit].wrist));
					if (units[lootunit].neck)
						additem(lootcell, randlook(units[lootunit].neck));
					if (units[lootunit].items[0])
						additem(lootcell, randlook(units[lootunit].items[0]));
					if (units[lootunit].items[1])
						additem(lootcell, randlook(units[lootunit].items[1]));
					if (units[lootunit].items[2])
						additem(lootcell, randlook(units[lootunit].items[2]));
					if (units[lootunit].body)
						additem(lootcell, randlook(units[lootunit].body));
					if (units[lootunit].head)
						additem(lootcell, randlook(units[lootunit].head));
					if (units[lootunit].offhand)
						additem(lootcell, randlook(units[lootunit].offhand));
					if (units[lootunit].belt)
						additem(lootcell, randlook(units[lootunit].belt));
					if (units[lootunit].hand)
						additem(lootcell, randlook(units[lootunit].hand));
					if (lootnext)
						additem(lootcell, lootnext);
				}
			}
		}
	for (lootb = 1; lootb < nunits; lootb++) {
		lootu = &units[lootb];
		if (lootu->head != 0 && (isfemale(lootu) || ownsland(lootu)) && !giveitem(lootb, units[lootb].head, 2))
			units[lootb].head = 0;
		if (lootu->type == 0x19 && !lootu->wrist && !lootu->neck && athome(lootu, 0)
		&& !fetchitem(lootu, 0x4f4, 0) && !fetchitem(lootu, 0x4f7, 0) && !fetchitem(lootu, 0x4f3, 0))
			fetchitem(lootu, 0x4f6, 0);
		if (lootu->type == 0x18 && !lootu->cloak && ownsland(lootu) && !fetchitem(lootu, 0xf601b9, 1))
			fetchitem(lootu, 0xfa01c6, 1);
		if (lootu->type == 0x1a && !lootu->cloak && ownsland(lootu) && !fetchitem(lootu, 0xfa01c6, 1))
			fetchitem(lootu, 0xf601b9, 1);
		if (!isfemale(lootu) && !isservant(lootu) && lootu->hand == 0 && ownsland(lootu)
		&& !fetchitem(lootu, 0xe901cf, 1) && !fetchitem(lootu, 0xe401ca, 1) && !fetchitem(lootu, 0xef01d5, 1))
			fetchitem(lootu, 0xe801ce, 1);
		if (lootu->head != 0 && !keepshelm(lootu) && !giveitem(lootb, lootu->head, 2)) {
			lootu->head = 0;
			calcstats(lootu);
		}
	}
}

void
sweepmap(short all)
{
	for (scany = 0; scany < 150; scany++)
		for (scanx = 0; scanx < 150; scanx++) {
			if (!all && !passmode && cansee(scanx, scany))
				continue;
			scancell = scanx + scany * 150;
		again:
			scanv = map[scancell];
			if (scanv & 0x1000) {
				scanobj = scanv >> 16;
				if (scanobj) {
					for (;;) {
						scanw = mapitems[scanobj];
						scankind = classof(scanw & 0xfff);
						switch (scankind) {
						case 0x36:
						case 0x37:
							if (scanv & 0x2000)
								break;
							if (typenames[scanw & 0xfff] == 0x14)
								break;
							if (weights[scanw & 0xfff] >= 65000)
								break;
						remove:
							takeitem(scancell, scanobj);
							if ((scanw & 0x8000) == 0)
								goto next;
							delflies(scancell, scanobj);
							goto again;
						case 0x67:
						case 0x68:
							if (scanw >> 16 == 2)
								break;
							if (scanw >> 16 == 1) {
								if ((*(unsigned long *)(mapstate + 0x2b)) == 0)
									break;
								if (gametime <= (*(unsigned long *)(mapstate + 0x2b)))
									break;
								if (gametime - (*(unsigned long *)(mapstate + 0x2b)) <= hourticks * 0x60)
									break;
								if ((mapflags[scancell] & 0x200) == 0)
									break;
							}
							if (scankind != 0x68)
								goto remove;
							mapitems[scanobj] += 0xd;
							break;
						}
						if ((scanw & 0x8000) == 0)
							break;
						scanobj++;
					}
				}
			}
		next:
			;
		}
}

void
saveclock(void)
{
	clocktime = gametime;
	clockhour = gamehour;
	clockday = gameday;
}

void
loadclock(void)
{
	gametime = clocktime;
	gamehour = clockhour;
	gameday = clockday;
}

void
saveclock2(void)
{
	clocktime2 = gametime;
	clockhour2 = gamehour;
	clockday2 = gameday;
}

void
loadclock2(void)
{
	gametime = clocktime2;
	gamehour = clockhour2;
	gameday = clockday2;
}

void
settimeback(unsigned long t)
{
	if (t < gametime) {
		backdiff = gametime - t;
		backhours = backdiff / hourticks;
		backdays = backhours / 24;
		backhours -= backdays * 24;
		if (gameday >= backdays)
			gameday -= backdays;
		else
			gameday = 0;
		backhour = gamehour;
		backhour -= backhours;
		// FIXED: an hour that wraps past midnight leaves the day as it was. Go back a day with it.
#if defined(FEATURE_FIXES)
		if (backhour < 0 && gameday > 0)
			gameday--;
#endif
		if (backhour < 0)
			backhour += 24;
		gametime = t;
		gamehour = backhour;
	}
}

int
travel(unsigned short dest, int x, int y, unsigned long time, unsigned short depart, unsigned short a6, unsigned short mode, short a8)
{
	trvok = 1;
	trvmsg = 0;
	starttimer3();
	clearmsgs();
	cleartexts();
	stopsounds();
	clearqueue();
	trvmode = 0;
	trvmoved = 0;
	trvflag = 0;
	trvhour0 = gamehour;
	trvday0 = gameday;
	if (dest == curmap)
		goto same;
	if (depart) {
		markvisited();
		if (maploaded) {
			saveobjs(curmap, 0);
			savemap(curmap);
		}
		if (maploaded && autolevel >= 1)
			autosave();
	}
	if (mode && curmap != 1 && curmap != 5 && curmap != 0x30 && time) {
		if (mode == 0xff)
			;
		else if (avengers && avengers != -1) {
			trvflag = 1;
			mode = 0xfe;
		} else if (getbit(game.flags, 0x292)) {
			trvflag = 1;
			mode = 0xfd;
		} else if (!cheat || !(keys[0xbe] & 0x80) && !(keys[0xe2] & 0x80)) {
			trvc = 25;
			if (valueof(units[0].body) > valueof(0xf001b1))
				trvc += 20;
			if (valueof(units[0].body) > valueof(0xee01af))
				trvc += 10;
			else if (valueof(units[0].body) >= valueof(0xed01ae))
				trvc += 5;
			if (valueof(units[0].belt) >= valueof(0xe501cb) * 3 / 4
			|| valueof(units[0].hand) >= valueof(0xe501cb) * 3 / 4)
				trvc += 10;
			else if (valueof(units[0].belt) >= valueof(0xe101c7))
				trvc += 5;
			else if (valueof(units[0].hand) >= valueof(0xe101c7))
				trvc += 5;
			if (units[0].head)
				trvc += 10;
			if (units[0].cloak)
				trvc += 10;
			if (units[0].wrist) {
				short a = trvc + 10;

				trvc += 5;
				if ((units[0].wrist & 0xfff) == 0x4f3 || (units[0].wrist & 0xfff) == 0x4f4)
					trvc = a;
			}
			if (units[0].neck) {
				short a = trvc + 10;

				trvc += 5;
				if ((units[0].neck & 0xfff) == 0x4f6 || (units[0].neck & 0xfff) == 0x4f7)
					trvc = a;
			}
			if (itemweight(0x2e4) >= kindof(&units[0])->strength * 25 / 3)
				trvc += 10;
			trvc *= time / hourticks;
			trvc /= 72;
			if (trvc > 80)
				trvc = 80;
			if ((censorship == 1 || censorship == 4) && trvc > 25)
				trvc = 25;
			if (!units[0].hand && !isarmor(units[0].body) && !units[0].head && !units[0].wrist && !units[0].neck
			&& classof(units[0].offhand & 0xfff) != 0x55 && (!units[0].cloak || wearof(units[0].cloak) >= 5)
			&& (int)packvalue() < 400)
				trvc = 0;
			else if (!game.defeated && (!units[0].hand || units[0].health <= kindof(&units[0])->maxhealth * 3 / 4))
				trvc = 0;
			if (cheat && (keys['W'] & 0x80) && !(keys[VK_CONTROL] & 0x80)
			|| randn(100) >= trvc && getbit(game.flags, 0x4bb))
				goto arrive;
		}
		if (mode == 0xff)
			trvmode = 0xff;
		else {
			if (mode == 0 || mode == 1)
				trvmode = 1;
			else
				trvmode = mode;
			copymap(1, 2);
			copymap(5, 6);
			copymap(3, 4);
			setflag(0x5be, 0);
		}
		trvfrom = mapzone(curmap);
		trvto = mapzone(dest);
		trvtime = time;
		if (trvmode != 0xff)
			time >>= 1;
		trvhour = (time / hourticks + gamehour) % 24;
		if (trvhour < 8)
			time += hourticks * (8 - trvhour);
		else if (mode == 0xff && trvhour > 12 || trvhour > 21)
			time += hourticks * (24 - trvhour + 7 + randn(3));
		trvhour = (time / hourticks + gamehour) % 24;
		if (trvtime <= time)
			trvtime = 1;
		else
			trvtime -= time;
		if (trvtime < hourticks)
			trvtime = hourticks;
		trvleft = trvtime / hourticks;
		if (trvmode == 0xff)
			dest = 0x30;
		else
			dest = randn(100) < 50 ? 1 : 5;
	}
arrive:
	plotleave(curmap);
	if (!loadcells(dest)) {
		if (trvfailmsg) {
			trvfailmsg = 0;
			trvmsg = 1;
		}
		loadcells(15);
		dest = 15;
		trvok = 0;
		x = zones[1].x;
		playerx = x;
		y = zones[1].y;
		playery = y;
		mode = 0;
		trvmode = 0;
	}
	if (mode == 0xff)
		nunits = 1;
	lefttime = 0;
	shoetime = 0;
	buildparts();
	loadmap(dest);
	mapsum = statesum();
	fixitems();
	buildrecipes();
	if (dest != curmap) {
		curmap = dest;
		trvmoved = 1;
	}
	if (trvmode) {
		game.ncaves = 0;
		trvhour = 0;
	again:
		x = randn(110) + 20;
		y = randn(110) + 20;
		if (trvhour < 1000 && terrains[map[y * 150 + x] & 0xfff] != 3 && terrains[map[y * 150 + x] & 0xfff] != 2) {
			trvhour++;
			goto again;
		}
		for (trvr = y - 2; trvr <= y + 2; trvr++)
			for (trvc = x - 2; trvc <= x + 2; trvc++)
				if (!(map[trvc + trvr * 150] & 0x4000) || !(map[trvc + trvr * 150] & 0x8000)) {
					trvhour++;
					goto again;
				}
		playerx = x;
		playery = y;
		if (trvmode == 0xff) {
			x = 0x4b;
			y = 0x4c;
			playerx = x;
			playery = y;
			units[0].dirx = 0;
			units[0].diry = 1;
			trvunit = addrefugee(playerx - 1, playery, 7, 0xc20);
			units[trvunit].face = 0x9b;
			units[trvunit].number = 0x201f;
			if (getbit(game.flags, 0x1e3)) {
				trvunit = newunit(8, 0);
				units[trvunit].health = 0;
				units[trvunit].x = 0;
				units[trvunit].y = 0;
			} else {
				trvunit = addrefugee(playerx, playery - 1, 8, 0xc21);
				units[trvunit].face = 0x22;
			}
			if (getbit(game.flags, 0x1e4)) {
				trvunit = newunit(9, 0);
				units[trvunit].health = 0;
				units[trvunit].x = 0;
				units[trvunit].y = 0;
			} else {
				trvunit = addrefugee(playerx + 1, playery, 9, 0xc21);
				units[trvunit].face = 0x27;
			}
			markunits();
			trvunit = addroman(0x49, 0x4f, 0xe);
			units[trvunit].scene = 0x13ce;
			units[trvunit].face = 0x11b;
			trvlead = trvunit;
		} else if (trvmode == 0xfe || mode == 0xfd) {
			trvunit = addavenger(x, y, trvmode == 0xfe ? 0x26f2 : 0x26f3);
			units[trvunit].face = 0xa3;
			trvlead = trvunit;
		} else {
			trvunit = addrobber(x, y);
			if (!trvflag && !getbit(game.flags, 0x4bb) && !game.defeated) {
				units[trvunit].hand = 0xa0e301c9;
				units[trvunit].belt = 0;
				calcstats(&units[trvunit]);
			}
			trvlead = trvunit;
		}
		markunits();
		setunitmode(&units[trvunit], 4);
		turnto(&units[trvunit], playerx, playery);
		if (trvmode == 0xff) {
			addroman(0x48, 0x4e, 0x13);
			addroman(0x47, 0x4c, 0xb);
			addroman(0x47, 0x4d, 0xb);
			addroman(0x4a, 0x4f, 0xb);
			markunits();
			for (trvr = randn(7) + 4; trvr > 0; trvr--)
				addpig(0);
			markunits();
		} else if (trvmode == 0xfe || trvmode == 0xfd) {
			if (trvmode == 0xfd)
				trvn = 9;
			else
				trvn = avengers;
			avengers = -1;
			for (trvunit = 0; trvunit < trvn; trvunit++) {
				if (trvunit <= 5)
					addavenger(x, y, trvmode == 0xfe ? 0x26f2 : 0x26f3);
				else
					// PORT: C leaves the order of a call's arguments open. Draw them from the last to the first, as Watcom compiled them
#ifdef PORT
				{
					int ry;

					ry = randn(30);
					addavenger(x + randn(30) - 15, y + ry - 15, trvmode == 0xfe ? 0x26f2 : 0x26f3);
				}
#else
					addavenger(x + randn(30) - 15, y + randn(30) - 15, trvmode == 0xfe ? 0x26f2 : 0x26f3);
#endif
				markunits();
			}
		} else {
			trvn = 0;
			if (game.defeated >= 3 && getbit(game.flags, 0x4bb)) {
				trvn = randn(prowess(&units[0]) / 20);
				if (trvn < 3) {
					if (game.defeated > 10)
						trvn++;
					if (game.defeated > 20 && randn(100) < 50)
						trvn++;
					if (game.defeated > 30 && randn(100) < 50)
						trvn++;
				}
				if (game.defeated < 5 && trvn > 2)
					trvn = 2;
				else if (game.defeated < 8 && trvn > 3)
					trvn = 3;
				if (trvn > 10)
					trvn = 10;
				if (trvn > 5 && randn(100) < 20)
					trvn = randn(4);
				for (trvunit = 0; trvunit < trvn; trvunit++) {
					if (trvunit <= 5) {
						trvr = addrobber(x, y);
						if (trvr && game.defeated < trvunit + 3)
							units[trvr].courage = 1;
					} else
						// PORT: as above
#ifdef PORT
					{
						int ry;

						ry = randn(30);
						trvr = addrobber(x + randn(30) - 15, y + ry - 15);
					}
#else
						trvr = addrobber(x + randn(30) - 15, y + randn(30) - 15);
#endif
					if (game.defeated < 10 && spellset(&units[trvr]))
						units[trvr].type = 0x22;
					markunits();
				}
			}
			game.ncaves = 0;
			memset(caveused, 0, sizeof caveused);
			for (trvr = 0; trvr < randn(3) + 2; trvr++)
				addcave();
			addcamp(units[trvlead].x, units[trvlead].y, 0, trvn);
			for (trvr = randn(7) + 4; trvr > 0; trvr--)
				addpig(0x15);
		}
		markunits();
		setflag(0x4bb, 1);
	}
	if ((*(unsigned long *)(mapstate + 0x27)) > gametime)
		(*(unsigned long *)(mapstate + 0x27)) = gametime - 1;
	if ((*(unsigned long *)(mapstate + 0x2b)) == 0) {
		train(0x14, 5);
		train(0x18, isbasemap(dest) ? 0x32 : 15);
	}
same:
	playerx = x;
	playery = y;
	units[0].x = x;
	units[0].y = y;
	trackregion();
	if (time || dest != curmap) {
		units[0].mode = 0;
		units[0].pose = 0;
		syncplayer();
		steptick = 0x18;
		stepframe = 6;
	}
	markvisited();
	if (trvmoved) {
		if ((*(unsigned long *)(mapstate + 0x2b)))
			trvtime = gametime - (*(unsigned long *)(mapstate + 0x27));
		else
			trvtime = hourticks * 3 < gametime ? hourticks * 3 : gametime;
		saveclock();
		settimeback((*(unsigned long *)(mapstate + 0x2b)) ? (*(unsigned long *)(mapstate + 0x27)) : gametime - trvtime);
		catchup(trvtime);
		loadclock();
	}
	if (time) {
		playmode = 1;
		setunitmode(&units[0], 1);
		trvtime = gametime;
		saveclock2();
		passalone(time);
		trvtime = gametime - trvtime;
		saveclock();
		loadclock2();
		catchup(trvtime);
		loadclock();
		units[0].pose = 0;
		trvhours = trvtime / hourticks;
		clearmsgs();
		cleartexts();
		if (gameday <= trvday0)
			msgf(0x63, strs[508], atdaytime(gamehour), strs[zones[mapzone(curmap)].name]);
		if (gameday >= trvday0 + 1) {
			fmtweekday(gameday - trvday0);
			msgf(0x63, strs[508], daytext, strs[zones[mapzone(curmap)].name]);
		}
	}
	if (trvmoved && gametime != (*(unsigned long *)(mapstate + 0x27))) {
		trvtime = gametime - (*(unsigned long *)(mapstate + 0x27));
		if (trvtime % hourticks >= hourticks / 2) {
			trvtime /= hourticks;
			trvtime++;
		} else
			trvtime /= hourticks;
		if (trvtime % 24 >= 12)
			trvtime = trvtime / 24 + 1;
		else
			trvtime = trvtime / 24;
		game.absenttime = trvtime;
		game.backday = gameday;
		if ((*(unsigned long *)(mapstate + 0x2b)) == 0)
			game.absenttime = 0;
		fillcaves();
		plotenter(curmap);
		reguard();
	}
	if (!(*(unsigned long *)(mapstate + 0x2b)))
		(*(unsigned long *)(mapstate + 0x2b)) = gametime;
	if (a6)
		setdaylight();
	if (a6)
		placeunits(gamehour);
	plotcheck();
	if (a8 && otherat(playerx, playery) != 0xff)
		stepaside(&units[0]);
	sharecells();
	clearscenes();
	setcurregion();
	if (time && !units[0].feet) {
		trvc = kindof(&units[0])->maxhealth * 2 / 3 + 1;
		if (units[0].health > trvc)
			units[0].health = trvc;
	}
	if (hourticks * 12 <= time && gamehour >= 5 || hourticks * 24 <= time) {
		if (gamehour >= 6)
			game.tiredcount = (gamehour - 6) * 12;
		else
			game.tiredcount = gamehour * 12 + 0xd8;
	}
	setlevels();
	calcstats(&units[0]);
	if (trvmode)
		cleartexts();
	if (palblack)
		clrboth();
	lookaround();
	shown = 1;
	markunits();
	lastlight = 0xff;
	lastwarm = 0xff;
	lookdone();
	clearqueue();
	nearfoe(0);
	if (cmbautooff)
		autocombat();
	if (trvmoved && gametime != (*(unsigned long *)(mapstate + 0x27)))
		plotarrive(curmap);
	if (time)
		while (elapsed3() < 1000)
			readinput();
	if (trvmsg)
		msgf(0x63, strs[267]);
	return trvok;
}

void
stepaside(Unit *u)
{
	if (badunit(u))
		return;
	nfreedirs = 0;
	for (movi = 0; movi < 8; movi += 2) {
		movx = u->x + dirdx[movi];
		movy = u->y + dirdy[movi];
		movcell = movx + movy * 150;
		if (map[movcell] & 0x4000) {
			if ((u->mode == 1 || u->mode == 3 || u->mode == 7) && (mapflags[movcell] & 0x10))
				continue;
			if (mapflags[movcell] & 4)
				continue;
			if (u != units && movx == playerx && movy == playery)
				continue;
			freedirs[nfreedirs++] = movi;
		}
	}
	if (nfreedirs == 0)
		return;
	if (nfreedirs == 1)
		movi = 0;
	else
		movi = randn(nfreedirs);
	u->x += dirdx[freedirs[movi]];
	u->y += dirdy[freedirs[movi]];
	if (u == units) {
		playerx = u->x;
		playery = u->y;
		playercell = playerx + playery * 150;
		trackregion();
	}
}

void
sayline(Unit *u, int msg)
{
	if (msg == 0x575 && isservant(u) && u->sympathy >= 3)
		showtip(tipanchorx, tipanchory, strs[0x577]);
	else if (msg == 0x575 && u->sympathy >= 4)
		showtip(tipanchorx, tipanchory, strs[0x576], game.name);
	else
		showtip(tipanchorx, tipanchory, strs[msg]);
}

void
say(Unit *u, int now, int msg)
{
	if (u->asleep || u->mode == 0x5a)
		return;
	screenpos(u);
	anchortip(1, screenposx, screenposy);
	turnto(u, units[0].x, units[0].y);
	if (isanimal(u)) {
		animalcry(u);
		return;
	}
	if (!now) {
		if (tipshown())
			return;
		if (nmsgs) {
			tipclears = 0;
			sayline(u, msg);
			tipclears = 1;
			return;
		}
	}
	sayline(u, msg);
}

void
addsymp(Unit *u)
{
	if (u->sympathy < 1 || u->sympathy >= 3)
		return;
	u->sympathy = plotsymp(curmap, indexof(u), u->sympathy + 1);
}

int
magicof(unsigned long item)
{
	if (item & 0x200000)
		return -1;
	if (item & 0x1f0000)
		return 1;
	return 0;
}

int
canenchant(int n, int a, unsigned long *p, Unit *u, unsigned short mode, int unused)
{
	if (badunit(u))
		return 0;
	if (a == -1) {
		if (!isanimal(&units[n]))
			return 1;
		if (mode == 2)
			return 1;
		return 0;
	}
	if (p)
		cep = p;
	else
		cep = itemptr(n, a);
	if (cep == 0)
		return 0;
	switch (classof(*cep & 0xfff)) {
	case 0x62:
	case 0x63:
		if (mode == 1 && !magicof(*cep))
			return 0;
		return 1;
	}
	if (mode == 1 && blessof(*cep) == 7)
		return 0;
	return 1;
}

void
spellmsg(Unit *u, int msg, int n, int good)
{
	if (u != units)
		return;
	msgf(0x63, strs[msg], itemname(n));
	if (good)
		music(0x22, 0);
	else
		music(0x1c, 0);
}

int
faithroll(Unit *u, int n)
{
	if (curunit && u == &units[curunit])
		return 1;
	frfaith = skillof(u, 0x17, 0, 0, 0);
	n += 100;
	if (n >= frfaith * 2)
		return 0;
	if (randn(100) < 10 || prevails(frfaith, n))
		return 1;
	return 0;
}

void
enchant(int n, int a, unsigned long *p, Unit *u, unsigned short mode, short force)
{
	if (badunit(u))
		return;
	if (a == -1 && isanimal(&units[n]) && mode != 2)
		return;
	if (u == units) {
		tire(3);
		train(0x14, 1);
		train(0x17, 5);
	}
	if (a == -1) {
		switch (mode) {
		case 0:
			assault(u, &units[n]);
			if (units[n].blessed || units[n].warded || units[n].berserk)
				goto fail;
			if (jewelmagic(&units[n], 0x80000)
			|| !(randn(100) < 10
			|| prevails((skillof(u, 0x16, 0, 0, 0) * 2 + skillof(u, 0x14, 0, 0, 0) + skillof(u, 0x17, 0, 0, 0)) / 4,
			    skillof(&units[n], 0x14, 0, 0, 0) + skillof(&units[n], 0x18, 0, 0, 0) - skillof(&units[n], 0x17, 0, 0, 0))
			|| force || curunit && u == &units[curunit])) {
				if (u == units)
					failmsg(strs[1371], unitname(&units[n]));
				break;
			}
			if (u == units) {
				msgf(0x63, strs[361], unitname(&units[n]));
				music(0x1c, 0);
			} else if (n == 0) {
				msgf(0x12, strs[362], unitname(u));
				music(0x1c, 0);
			}
			if (units[n].blessed)
				units[n].blessed = 0;
			else
				addpower(&units[n].cursed, u, 0x16);
			break;
		case 1:
			if (!units[n].cursed && !units[n].blessed && !units[n].weakened && !units[n].warded && !units[n].berserk)
				goto fail;
			if (!(randn(100) < 10 || prevails(skillof(u, 0x17, 0, 0, 0), 0x5a)
			|| force || curunit && u == &units[curunit]))
				goto fail;
			if (u == units) {
				msgf(0x63, strs[363], unitname(&units[n]));
				if (sfxswitch) {
					if (units[n].cursed || units[n].weakened || units[n].berserk) {
						music(0x22, 0);
						if (&units[n] != units) {
							if (units[n].mode == 0x14 || units[n].mode == 0x15)
								addsymp(&units[n]);
							say(&units[n], 0, 0x575);
						}
					} else
						music(0x1c, 0);
				}
			}
			units[n].cursed = 0;
			units[n].blessed = 0;
			units[n].weakened = 0;
			units[n].warded = 0;
			units[n].berserk = 0;
			break;
		case 2:
			if (units[n].cursed || units[n].weakened) {
				if (force != 0xff)
					goto fail;
				units[n].cursed = 0;
				units[n].weakened = 0;
			}
			if (!(randn(100) < 10 || prevails(skillof(u, u == &units[n] ? 0x17 : 0x16, 0, 0, 0), 0x69)
			|| force || curunit && u == &units[curunit]))
				goto fail;
			if (u == units) {
				if (force == 0xff)
					;
				else {
					msgf(0x63, strs[368], unitname(&units[n]));
					music(0x22, 0);
					if (&units[n] != u) {
						if (!units[n].blessed && (units[n].mode == 0x14 || units[n].mode == 0x15))
							addsymp(&units[n]);
						say(&units[n], 0, 0x575);
					}
				}
			} else if (n == 0) {
				msgf(0x12, strs[369], unitname(u));
				music(0x22, 0);
			}
			if (force == 0xff)
				enpower = 0x21;
			else
				enpower = halfskill(u, u == &units[n] ? 0x17 : 0x16);
			if (units[n].cursed) {
				if (units[n].cursed > enpower)
					units[n].cursed -= enpower;
				else
					units[n].cursed = 0;
			} else if (enpower + units[n].blessed > 0xff)
				units[n].blessed = 0xff;
			else
				units[n].blessed += enpower;
			break;
		}
		goto end;
	fail:
		if (u == units)
			failmsg(strs[436]);
	end:
		calcstats(&units[n]);
		return;
	}
	if (p)
		enp = p;
	else
		enp = itemptr(n, a);
	if (enp == 0)
		return;
	enbless = classof(*enp & 0xfff);
	if (enbless == 0x63 || enbless == 0x62) {
		if (mode == 1) {
			if (!magicof(*enp) || (*enp & 0x80000000) == 0)
				goto fail2;
			if (!faithroll(u, 100) && !force)
				goto fail2;
			((unsigned short *)enp)[1] = 0;
			spellmsg(u, 0x16b, *enp, 0);
			goto done;
		}
		if (mode == 0) {
			if (magicof(*enp))
				goto fail2;
			if (!faithroll(u, 100) && !force)
				goto fail2;
			((unsigned short *)enp)[1] = 0;
			((unsigned short *)enp)[1] |= 0x8020;
			spellmsg(u, 0x168, *enp, 0);
			goto done;
		}
		if (mode == 2) {
			if (magicof(*enp))
				goto fail2;
			if (!faithroll(u, 0x78) && !force)
				goto fail2;
			((unsigned short *)enp)[1] = 0;
			((unsigned short *)enp)[1] |= 0x8004;
			spellmsg(u, 0x16f, *enp, 1);
			goto done;
		}
	}
	enbless = blessof(*enp);
	switch (mode) {
	case 0:
		if (enbless <= 1 || enbless > 7)
			goto fail2;
		endiff = (7 - enbless) * 15;
		if (!faithroll(u, endiff + 0x14) && !force)
			goto fail2;
		spellmsg(u, 0x168, *enp, 0);
		enbless--;
		break;
	case 1:
		if (enbless == 7 || enbless == 0 || enbless >= 15)
			goto fail2;
		endiff = classof(*enp & 0xfff);
		if ((endiff == 0x49 || endiff == 0x4a) && enbless == 0)
			goto fail2;
		if (enbless < 7)
			endiff = (7 - enbless) * 15;
		else
			endiff = (enbless - 7) * 15;
		if (!faithroll(u, endiff + 0x28) && !force)
			goto fail2;
		spellmsg(u, 0x16b, *enp, enbless < 7);
		enbless = 7;
		break;
	case 2:
		if (wearof(*enp) == 7 && enbless == 0 || enbless >= 14 || enbless < 7)
			goto fail2;
		endiff = (enbless - 7) * 15;
		if (!faithroll(u, endiff + 0x1e) && !force)
			goto fail2;
		spellmsg(u, 0x16f, *enp, 1);
		enbless++;
		break;
	}
	setbless(enp, enbless);
done:
	train(0x14, 2);
	train(0x17, 5);
	if (n != 0x29 && n != 0x2a)
		return;
	a = panels[n - 0x28].unit;
	calcstats(&units[a]);
	return;
fail2:
	if (u == units)
		failmsg(strs[436]);
}

void
blessitem(unsigned long *p, Unit *u, short b, short force)
{
	if (badunit(u))
		return;
	if (p == 0 || *p == 0)
		return;
	if (!b && blessof(*p) > 7 || b && blessof(*p) < 7)
		enchant(0, 0, p, u, 1, force);
	enchant(0, 0, p, u, b ? 2 : 0, force);
}

short
sacrifice(int n, unsigned a)
{
	sacp = itemptr(n, a);
	if (sacp == 0)
		return 0;
	sachi = *sacp & 0xf000;
	sacclass = classof(*sacp & 0xfff);
	if (sacclass == 0x3c || sacclass == 0x41 || sacclass == 0x46 || sacclass == 0x48 || sacclass == 0x55)
		sacgear = 1;
	else
		sacgear = 0;
	if ((sacclass == 0x49 || sacclass == 0x4a) && blessof(*sacp) == 0)
		return 0;
	msgf(0x63, strs[364], itemname(*sacp));
	// FIXED: sacval keeps the value in an unsigned short, so an offering worth 65,536 or more counts its value modulo 65,536. Count the value as valueof returns it.
#if defined(FEATURE_FIXES)
	game.sacrificed += valueof(*sacp);
	if (game.sacrificed > 0xffff0000)
		game.sacrificed = 0xffff0000;
	addfavor(valueof(*sacp) / 10);
#else
	sacval = valueof(*sacp);
	game.sacrificed += sacval;
	if (game.sacrificed > 0xffff0000)
		game.sacrificed = 0xffff0000;
	addfavor(sacval / 10);
#endif
	if (sacgear) {
		*sacp = broken(*sacp);
		if (*sacp == 0)
			*sacp = sachi | 0x2d1;
		else {
			setwear(sacp, 7);
			setbless(sacp, 0);
		}
	} else
		*sacp = sachi | 0x2d1;
	if ((n == 0x29 || n == 0x2a) && !(a >= 0x28 && a <= 0x2a)) {
		sacval = panels[n - 0x28].unit;
		giveitem(sacval, *sacp, 1);
		*sacp = 0;
		calcstats(&units[sacval]);
		if (sacval == 0)
			dresshero();
	}
	music(0x21, 0);
	return 1;
}

void
markadj(int cell)
{
	int k;

	if ((map[cell] & 0x4000) == 0)
		return;
	k = cell - 151;
	adjmap = &map[k];
	adjflags = &mapflags[k];
	adjflags++;
	*adjflags |= 0x200;
	adjmap += 302;
	adjflags += 149;
	*adjflags |= 0x200;
	adjflags += 2;
	*adjflags |= 0x200;
	adjflags += 149;
	*adjflags |= 0x200;
	adjflags++;
}

void
flood(int x, int y, int r)
{
	int x0, x1, y1, y0, i, j, k, c;

	x0 = x - r;
	if (x0 < 1)
		x0 = 1;
	x1 = x + r;
	if (x1 > 0x94)
		x1 = 0x94;
	y0 = y - r;
	if (y0 < 1)
		y0 = 1;
	y1 = y + r;
	if (y1 > 0x94)
		y1 = 0x94;
	markadj(x + y * 150);
	for (k = 0; k < 6; k++) {
		for (j = y0; j <= y1; j++) {
			for (i = x0; i <= x1; i++)
				if (mapflags[c = i + j * 150] & 0x200)
					markadj(c);
			for (i = x1; i >= x0; i--)
				if (mapflags[c = i + j * 150] & 0x200)
					markadj(c);
		}
		for (j = y1; j >= y0; j--) {
			for (i = x1; i >= x0; i--)
				if (mapflags[c = i + j * 150] & 0x200)
					markadj(c);
			for (i = x0; i <= x1; i++)
				if (mapflags[c = i + j * 150] & 0x200)
					markadj(c);
		}
	}
}

short
vacant(short x, short y)
{
	vaccell = x + y * 150;
	if ((map[vaccell] & 0x4000) == 0 || mapflags[vaccell] & 4 || vaccell == playercell || vaccell == playernext)
		return 0;
	return 1;
}

short
summon(unsigned char kind, short u)
{
	/* spare moves wpp386's choices in sackarea to the original's */
	int spare;

	summx = units[u].x;
	summy = units[u].y;
	if (vacant(summx - 1, units[u].y))
		summx--;
	else if (vacant(summx + 1, summy))
		summx++;
	else if (vacant(summx, summy - 1))
		summy--;
	else if (vacant(summx, summy + 1))
		summy++;
	else if (vacant(summx - 1, summy - 1)) {
		summx--;
		summy--;
	} else if (vacant(summx + 1, summy - 1)) {
		summx++;
		summy--;
	} else if (vacant(summx - 1, summy + 1)) {
		summx--;
		summy++;
	} else if (vacant(summx + 1, summy + 1)) {
		summx++;
		summy++;
	} else {
		for (summi = -2; ; summi++) {
			if (summi > 2)
				return 0;
			if (vacant(summx + summi, summy - 2)) {
				summx += summi;
				summy -= 2;
				break;
			}
			if (vacant(summx + summi, summy + 2)) {
				summx += summi;
				summy += 2;
				break;
			}
			if (vacant(summx - 2, summy + summi)) {
				summy += summi;
				summx -= 2;
				break;
			}
			if (vacant(summx + 2, summy + summi)) {
				summy += summi;
				summx += 2;
				break;
			}
		}
	}
	summi = newunit(kind, 0);
	units[summi].modec = 0xff;
	units[summi].modeb = u;
	if (u != 0)
		units[summi].sympathy = 2;
	units[summi].x = summx;
	units[summi].traits |= 8;
	units[summi].y = summy;
	return 1;
}

void
drawhidden(void)
{
	dhy = -3;
	dhk = 0;
	for (; dhy <= 3; dhy++)
		for (dhx = -5; dhx <= 5; dhx++)
			if (workbuf[0x1388 + dhk++])
				putcs(dhx * 0x30 + 0x128 + scrollx, dhy * 0x30 + 0xd8 + scrolly, 4);
}

char *
runegloss(unsigned char n)
{
	return strs[1211 + n * 2];
}

char *
runesense(unsigned char n)
{
	char *s, *p;

	s = runegloss(n);
	for (p = s; *p; p++)
		if (*p == ',') {
			for (p++; *p == ' '; p++)
				;
			return p;
		}
	return s;
}

char *
runename(unsigned char n)
{
	return strs[1210 + n * 2];
}

int
runeletter(unsigned char n)
{
	return strs[1210 + n * 2][0] - 'A';
}

void
castrunes(int show)
{
	runepast = randn(17);
	do
		runenow = randn(17);
	while (runenow == runepast);
	do
		runenext = randn(17);
	while (runenext == runepast || runenext == runenow);
	if (!show)
		return;
	wsprintf(printbuf, strs[1208], runeletter(runepast), runename(runepast), runegloss(runepast),
		runeletter(runenow), runename(runenow), runegloss(runenow),
		runeletter(runenext), runename(runenext), runegloss(runenext));
	showtip(0x140, 0xbe, printbuf);
	holdtip();
}

void
losecontrol(void)
{
	steptick = stepframe;
	playmode = 1;
	closepanels();
	menumode = 1;
	hidecursor();
	lostcontrol = 1;
}

void
regaincontrol(void)
{
	units[0].berserk = 0;
	units[0].fleeing = 0;
	lostcontrol = 0;
	calcstats(units);
	menumode = 0;
	if (units[0].mode != 0x5a) {
		playmode = 3;
		setunitmode(units, 0x14);
	}
	readinput();
	drawcursor();
}

int
protect(Unit *u, Unit *v, short force)
{
	if (u == units && force < 2) {
		tire(3);
		train(0x14, 1);
		train(0x17, 5);
	}
	if (v->berserk)
		goto fail;
	if (v->cursed || v->weakened) {
		if (force < 2)
			goto fail;
		v->cursed = 0;
		v->weakened = 0;
	}
	if (!(randn(100) < 10 || prevails(skillof(u, 0x17, 0, 0, 0), 100) || force))
		goto fail;
	if (force < 2)
		addpower(&v->warded, u, 0x17);
	else {
		protpot = protspell = v->warded;
		addcap(&protpot, 0x4b);
		addpower(&protspell, u, 0x17);
		if (protspell > protpot)
			v->warded = protspell;
		else
			v->warded = protpot;
		if (v == units) {
			game.tiredcount = 0;
			setlevels();
		}
	}
	calcstats(v);
	if (u == units) {
		msgf(0x63, strs[386]);
		music(0x1f, 0);
	}
	if (u == units && v != u)
		say(v, 0, 0x575);
	return 1;
fail:
	if (u == units)
		failmsg(strs[436]);
	return 0;
}

int
curepoison(Unit *u, Unit *v, short force)
{
	if (u == units && force < 2) {
		tire(1);
		train(0x14, 1);
		train(0x19, 5);
		train(0x17, 2);
	}
	if (!(randn(100) < 13
	|| prevails(skillof(u, 0x19, 0, 0, 0), v->poison + v->poison / 2 - v->blessed / 2 + v->weakened / 2 + v->cursed / 2)
	|| force)) {
		if (u == units) {
			if (v == u)
				failmsg(strs[1375]);
			else
				failmsg(strs[1376], unitname(v));
		}
		return 0;
	}
	music(0x1e, 0);
	purge(v);
	calcstats(v);
	if (u == units) {
		train(0x14, 1);
		train(0x19, 5);
		train(0x17, 2);
	}
	if (u == units) {
		if (v == u)
			msgf(0x63, strs[384]);
		else
			msgf(0x63, strs[385], unitname(v));
	}
	if (u == units && v != u) {
		addsymp(v);
		say(v, 0, 0x575);
		deedcure();
	}
	return 1;
}

void
goberserk(Unit *u, short mode)
{
	if (badunit(u))
		return;
	if (isanimal(u))
		return;
	if (u == units) {
		if (sparrer()) {
			denypractice();
			return;
		}
		tire(0x14);
		train(0x14, 1);
		train(0x17, 5);
	}
	if (!(randn(100) < 10 || prevails(skillof(u, 0x17, 0, 0, 0), 100) || mode)) {
		if (u == units)
			failmsg(strs[436]);
		return;
	}
	if (u->bewitched || u->fleeing)
		clearmind(u);
	if (u == units) {
		game.hungercount = 0;
		game.tiredcount = 0;
		setlevels();
		if (playmode != 3) {
			togglecombat(0);
			combatkind = 1;
		}
		hidecursor();
		ragesave = actionturn;
		actionturn = 0;
		cleartexts();
		msgf(0x32, strs[395]);
		drawscreen();
		actionturn = ragesave;
		music(0xb, 0);
		waitinput(2000);
	}
	ragecell = u->x + u->y * 150;
	if (u->body) {
		additem(ragecell, u->body);
		u->body = 0;
	}
	if (u->cloak) {
		if (canput(ragecell - 1))
			additem(ragecell - 1, u->cloak);
		else
			additem(ragecell, u->cloak);
		u->cloak = 0;
	}
	if (u->legs) {
		if (canput(ragecell + 1))
			additem(ragecell + 1, u->legs);
		else
			additem(ragecell, u->legs);
		u->legs = 0;
	}
	if (u->head) {
		if (canput(ragecell - 150))
			additem(ragecell - 150, u->head);
		else
			additem(ragecell, u->head);
		u->head = 0;
	}
	if (u->feet) {
		if (canput(ragecell + 150))
			additem(ragecell + 150, u->feet);
		else
			additem(ragecell, u->feet);
		u->feet = 0;
	}
	if (u == units)
		dresshero();
	rage = halfskill(u, 0x17);
	if (mode == 2) {
		if (rage < 0x4b)
			rage = 0x4b;
	} else if (mode == 3 && rage < 0x25)
		rage = 0x25;
	if (u->berserk + rage > 0xff)
		u->berserk = 0xff;
	else
		u->berserk += rage;
	u->warded = 0;
	u->fleeing = 0;
	u->bewitched = 0;
	u->weakened = 0;
	setunitmode(u, 0x14);
	calcstats(u);
	u->modeb = 0xff;
	u->modec = 0xff;
	if (u == units)
		losecontrol();
}

void
makepeace(Unit *u, Unit *v)
{
	if (badunit(u) || badunit(v))
		return;
	if (v == units) {
		for (mphour = 0; mphour < 0x18; mphour++)
			if (u->sched[0][mphour] == 4 || u->sched[0][mphour] == 0x32)
				u->sched[0][mphour] = 3;
		if (u->sympathy < 2)
			u->sympathy = 2;
	}
	if (u->mode == 0x14 || u->mode == 0x15) {
		if (v) {
			if (u->modeb == indexof(v)) {
				if (u->modec != 0xff && u->modec != indexof(v)) {
					u->modeb = u->modec;
					u->modec = 0xff;
					return;
				}
			} else {
				if (u->modec != indexof(v))
					return;
				u->modec = 0xff;
				if (u->modeb != 0xff && u->modeb != indexof(v))
					return;
			}
		}
	} else if (u->mode == 0x19) {
		if (v && u->modeb != indexof(v))
			return;
	} else if (u->mode == 4 || u->mode == 0x32) {
		u->modeb = 0;
		setunitmode(u, 3);
		return;
	} else if (scheduled(u) && !atpost(u, gamehour))
		gopost(u);
	if (u->traits & 8)
		u->traits = 0xff;
	setunitmode(u, 1);
	u->modea = u->modeb = u->modec = u->moded = 0;
	if (scheduled(u))
		gopost(u);
	plotpeace(u, v);
}

void
peacemen(Unit *u, Unit *v)
{
	if (badunit(v) || badunit(u))
		return;
	pmself = indexof(u);
	for (pmi = 1; pmi < nunits; pmi++)
		if (pmi != pmself && units[pmi].lord == pmself)
			makepeace(&units[pmi], v);
}

void
peacehouse(Unit *u, Unit *v)
{
	if (badunit(v) || badunit(u))
		return;
	makepeace(u, v);
	peacemen(u, v);
	if (isservant(u))
		return;
	phself = indexof(u);
	for (phi = 1; phi < nunits; phi++)
		if (phi != phself && isfemale(&units[phi]) && samehouse(u, &units[phi])) {
			peacemen(&units[phi], v);
			makepeace(&units[phi], v);
		}
}

void
peaceall(Unit *u, Unit *v)
{
	if (badunit(v) || badunit(u))
		return;
	makepeace(u, v);
	paself = indexof(u);
	for (pai = 1; pai < nunits; pai++)
		if (pai != paself
		&& (allied(u, &units[pai]) || v == units && (fightsplayer(&units[pai]) || units[pai].sympathy <= 0)))
			makepeace(&units[pai], v);
}

short
isgoods(unsigned long item, unsigned char kind)
{
	if (item == 0)
		return 0;
	if (kind == 0)
		return 1;
	goodsclass = classof(item & 0xfff);
	switch (kind) {
	case 'w':
		if (goodsclass == 0x3c)
			return 1;
		break;
	case 'r':
		if (goodsclass == 0x55 || goodsclass == 0x41 || isarmor(item))
			return 1;
		break;
	case 'z':
		if (goodsclass == 0x21 || goodsclass == 0x20 || goodsclass == 0x3a || baselook(item & 0xfff) == 0x1ce)
			return 1;
		break;
	case 'f':
		if (goodsclass == 0x56)
			return 1;
		break;
	case 'k':
		if (goodsclass == 0x4b || goodsclass == 0x50 || goodsclass == 0x48 || goodsclass == 0x46 && !isarmor(item)
		|| typenames[item & 0xfff] == 7 || typenames[item & 0xfff] == 0x83)
			return 1;
		break;
	case 's':
		if (goodsclass == 0x62 || goodsclass == 0x63 || goodsclass == 0x64 || typenames[item & 0xfff] == 0x9e)
			return 1;
		break;
	case 'g':
		if (goodsclass == 0xf || goodsclass == 0x10)
			return 1;
		break;
	case 'e':
		if (goodsclass == 0x36 || goodsclass == 0x37 || goodsclass == 0x87 || goodsclass == 0x88
		|| goodsclass == 0x89 || goodsclass == 0x8a || goodsclass == 0x8b)
			return 1;
		break;
	case 'b':
		if (goodsclass == 0x38 || goodsclass == 0x39 || goodsclass == 0x2d)
			return 1;
		break;
	}
	return 0;
}

int
ungather(Unit *u, int slot)
{
	if (tradegoods[slot] != 0) {
		if (goodsrealm[slot]) {
			if (!stowat(goodsrealm[slot], goodsspot[slot] ? goodsspot[slot] : stowspot(goodsrealm[slot], 0xff, 0xff, tradegoods[slot] & 0xfff), tradegoods[slot]))
				goto drop;
		} else if (goodsspot[slot])
			dropitem(goodsspot[slot], tradegoods[slot]);
		else {
	drop:
			giveitem(indexof(u), tradegoods[slot], 1);
		}
		return 1;
	}
	return 0;
}

int
tradeslot(Unit *u, int start)
{
	for (tradei = start; tradei < 0x18; tradei++)
		if (tradegoods[tradei] == 0)
			return tradei;
	if (!isservant(u)) {
		for (tradei = start; tradei < 0x18; tradei++)
			if (valueof(tradegoods[tradei]) < 10 && ungather(u, tradei))
				return tradei;
		for (tradei = start; tradei < 0x18; tradei++)
			if (valueof(tradegoods[tradei]) < 50 && ungather(u, tradei))
				return tradei;
		for (tradei = start; tradei < 0x18; tradei++)
			if (valueof(tradegoods[tradei]) < 100 && ungather(u, tradei))
				return tradei;
	}
	return -1;
}

short
gathergoods(Unit *u, short all, unsigned char kind)
{
	if (badunit(u))
		return 0;
	memset(tradegoods, 0, sizeof tradegoods);
	memset(goodsrealm, 0, sizeof goodsrealm);
	memset(goodsspot, 0, sizeof goodsspot);
	ggn = 6;
	if (yields(u, 2, u->head, 1) && isgoods(u->head, kind)) {
		tradegoods[ggn++] = u->head;
		u->head = 0;
	}
	if (myservant(u) || all) {
		if (u->hand && isgoods(u->hand, kind)) {
			tradegoods[ggn++] = u->hand;
			u->hand = 0;
		}
		if (u->belt && isgoods(u->belt, kind)) {
			tradegoods[ggn++] = u->belt;
			u->belt = 0;
		}
		if (u->body && isgoods(u->body, kind) && yields(u, 1, u->body, 1)) {
			tradegoods[ggn++] = u->body;
			u->body = 0;
		}
	}
	if (yields(u, 2, u->offhand, 1) && isgoods(u->offhand, kind)) {
		tradegoods[ggn++] = u->offhand;
		u->offhand = 0;
	}
	if (yields(u, 2, u->cloak, 1) && isgoods(u->cloak, kind)) {
		tradegoods[ggn++] = u->cloak;
		u->cloak = 0;
	}
	if (yields(u, 2, u->wrist, 1) && isgoods(u->wrist, kind)) {
		tradegoods[ggn++] = u->wrist;
		u->wrist = 0;
	}
	if (yields(u, 2, u->neck, 1) && isgoods(u->neck, kind)) {
		tradegoods[ggn++] = u->neck;
		u->neck = 0;
	}
	for (ggpass = 0; ggpass < 2; ggpass++) {
		for (ggi = 0; ggi < 3 && ggn < 0x18; ggi++)
			if (yields(u, 2, u->items[ggi], ggpass) && isgoods(u->items[ggi], kind)) {
				tradegoods[ggn++] = u->items[ggi];
				u->items[ggi] = 0;
			}
		gghome = athome(u, 0);
		ggmaster = 0;
		if (!gghome) {
			gghome = athome(u, 1);
			if (gghome != 0)
				ggmaster = 1;
		}
		ggowner = ownsland(u);
		ggspouse = 0;
		if (u->spouse && u->spouse < 0xff && ownsland(&units[u->spouse]))
			ggspouse = 1;
		if (gghome) {
			if (ggmaster == 0)
				for (ggy = regions[gghome].y0; ggy <= regions[gghome].y1 && ggn < 0x18; ggy++)
					for (ggx = regions[gghome].x0; ggx <= regions[gghome].x1 && ggn < 0x18; ggx++) {
						ggitem = topitem(ggy * 150 + ggx);
						if (ggitem && mapitems[ggitem] & 0x1000
						&& weightof(mapitems[ggitem] & 0xfff) < 15000
						&& valueof(mapitems[ggitem]) > 0
						&& yields(u, 2, mapitems[ggitem], ggpass)
						&& isgoods(mapitems[ggitem], kind)
						&& (ggowner || !lordsitem(mapitems[ggitem], ggspouse))
						&& !islittorch(mapitems[ggitem])) {
							tradegoods[ggn] = mapitems[ggitem];
							goodsspot[ggn] = ggx + ggy * 150;
							ggn++;
							popitem(ggy * 150 + ggx, 0);
						}
					}
			for (ggj = 0; ggj < 8 && ggn < 0x18; ggj++) {
				ggrealm = regions[gghome].realms[ggj];
				if (ggrealm != 0 && !(realms[ggrealm].item & 0x10000000)
				&& (!(realms[ggrealm].item & 0x8000000) || haskey(u, realms[ggrealm].item)))
				for (ggk = 0; ggk < 50 && ggn < 0x18; ggk++) {
					ggitem = realms[ggrealm].items[ggk];
					if (weightof(ggitem & 0xfff) < 15000 && valueof(ggitem) > 0
					&& (!ggmaster && yields(u, 2, ggitem, ggpass) && isgoods(ggitem, kind)
					|| ggmaster != 0 && (isunlittorch(ggitem) || classof(ggitem & 0xfff) == 0x3a))
					&& (ggowner || !lordsitem(ggitem, ggspouse))
					&& !islittorch(ggitem)) {
						tradegoods[ggn] = ggitem;
						goodsrealm[ggn] = ggrealm;
						goodsspot[ggn] = realms[ggrealm].pos[ggk];
						ggn++;
						takeout(ggrealm, ggk);
						ggk--;
					}
				}
			}
		}
	}
	memset(printbuf, 0, 0x18);
again:
	for (ggk = 0; ggk < 2; ggk++)
		for (ggx = 0x17; ggx >= 6; ggx--)
			if (tradegoods[ggx] != 0 && !printbuf[ggx]) {
				ggy = classof(tradegoods[ggx] & 0xfff);
				if (!(ggk == 0 && ggy == 0x10 || ggk == 1 && ggy == 0xf))
					continue;
				ggitem = tradegoods[ggx] >> 16;
				if (!(ggk == 0 && ggitem >= 2 || ggk == 1 && ggitem >= 10))
					continue;
				ggslot = tradeslot(u, 6);
				if (ggslot == -1)
					goto done;
				printbuf[ggx] = 1;
				ggj = ggitem >> 1;
				ggitem -= ggj;
				ggitem <<= 16;
				ggitem |= tradegoods[ggx] & 0xffff;
				addcoins(&ggitem, 0);
				tradegoods[ggx] = ggitem;
				addcoins(&tradegoods[ggx], 0);
				ggitem = ggj;
				ggitem <<= 16;
				ggitem |= tradegoods[ggx] & 0xffff;
				addcoins(&ggitem, 0);
				tradegoods[ggslot] = ggitem;
				printbuf[ggslot] = 0;
				goodsrealm[ggslot] = goodsrealm[ggx];
				goodsspot[ggslot] = goodsspot[ggx];
				goto again;
			}
done:
	// FIXED: ggn starts at 6 and only grows, so the test for no goods never holds and barter opens empty. Test 6, so that strs[897] says the goods are gone.
#if defined(FEATURE_FIXES)
	if (ggn == 6)
#else
	if (ggn == 3)
#endif
		return 0;
	return 1;
}

void
barter(Unit *u)
{
	if (badunit(u))
		return;
	bartercur = curunit;
	curunit = indexof(u);
	if (informal(u))
		dutalk = 1;
	else
		dutalk = 0;
	trademode = 1;
	nforsale = 0;
	pricemarkup = 0;
	if (!myservant(u) && istrader(u))
		pricemarkup = 10;
	trade();
	if (esckey != 0xff)
		esckey = 0;
	for (barteri = 0; barteri < 0x18; barteri++)
		ungather(u, barteri);
	memset(tradegoods, 0, sizeof tradegoods);
	memset(goodsrealm, 0, sizeof goodsrealm);
	endtrade();
	curunit = bartercur;
}

unsigned long
finditem(Unit *u, unsigned long item, int *realm, int *cell, int *slot, int mode)
{
	nfound = 0;
	seeklast = 0;
	if (badunit(u))
		return 0;
	if (mode == 3)
		seekkey = groupkey;
	else if (mode == 2)
		seekkey = typekey;
	else if (mode == 1)
		seekkey = loosekey;
	else
		seekkey = itemkey;
	if (mode == 3) {
		seekfood = 0;
		seekheavy = 0;
	} else {
		seekfood = isfood(item);
		item = seekkey(item);
		seekheavy = weightof(item & 0xfff) >= 65000 ? 1 : 0;
	}
	seekself = indexof(u);
	if (seekheavy)
		seekreg = regionat(u->x, u->y);
	else
		seekreg = athome(u, 1);
	if (!seekreg) {
		if (regionat(u->x, u->y))
			return 0;
		{
			seekx0 = u->x - 10;
			if (seekx0 < 0)
				seekx0 = 0;
			seekx1 = u->x + 10;
			if (seekx1 > 0x95)
				seekx1 = 0x95;
			seeky0 = u->y - 10;
			if (seeky0 < 0)
				seeky0 = 0;
			seeky1 = u->y + 10;
			if (seeky1 > 0x95)
				seeky1 = 0x95;
			for (seeky = seeky0; seeky <= seeky1; seeky++)
				for (seekx = seekx0; seekx <= seekx1; seekx++) {
					seekowner = ownerat(seekx, seeky);
					if (seekowner == 0 || seekowner == seekself || allied(u, &units[seekowner])) {
						seekcell = seeky * 150 + seekx;
						if (map[seekcell] & 0x1000)
							for (seekobj = map[seekcell] >> 16;; seekobj++) {
								seekitem = mapitems[seekobj];
								if ((seekitem & 0x1000 || myservant(u) || seekfood && isfood(seekitem))
								&& seekkey(seekitem) == item)
									goto found;
								if ((seekitem & 0x8000) == 0)
									break;
							}
					}
				}
		}
		return 0;
	} else {
		if (!seekheavy)
			for (seeki = 0; seeki < 8; seeki++) {
				seekrealm = regions[seekreg].realms[seeki];
				if (seekrealm != 0 && (!(realms[seekrealm].item & 0x8000000) || haskey(u, realms[seekrealm].item)))
					for (seekslot = 0; seekslot < 50; seekslot++) {
						seekitem = realms[seekrealm].items[seekslot];
						if ((seekitem & 0x1000 || myservant(u) || seekfood && isfood(seekitem))
						&& seekkey(seekitem) == item) {
							if (realm)
								*realm = seekrealm;
							if (cell)
								*cell = 0;
							if (slot)
								*slot = seekslot;
							if (mode < 2)
								return seekitem;
							nfound++;
							seeklast = seekitem;
						}
					}
			}
		for (seeky = regions[seekreg].y0; seeky <= regions[seekreg].y1; seeky++)
			for (seekx = regions[seekreg].x0; seekx <= regions[seekreg].x1; seekx++) {
				seekcell = seekx + seeky * 150;
				if (map[seekcell] & 0x1000)
					for (seekobj = map[seekcell] >> 16;; seekobj++) {
						seekitem = mapitems[seekobj];
						if ((seekitem & 0x1000 || seekheavy || u == units || myservant(u)
						|| seekfood && isfood(seekitem)) && seekkey(seekitem) == item) {
						found:
							if (realm)
								*realm = 0;
							if (cell)
								*cell = seekcell;
							if (slot)
								*slot = seekobj;
							if (mode < 2)
								return seekitem;
							nfound++;
							seeklast = seekitem;
						}
						if (!(seekitem & 0x8000))
							break;
					}
			}
		if (mode >= 2)
			return seeklast;
	}
	return 0;
}

int
plunderable(unsigned long item, int all)
{
	unsigned long n;

	if (weightof(item & 0xfff) >= 15000) {
		plname = typenames[item & 0xfff];
		if (plname == 0x58)
			return 1;
		if (all)
			switch (plname) {
			case 9:
			case 0x1d:
			case 0x34:
			case 0x35:
			case 0x70:
			case 0xb2:
			case 0xbe:
				return 1;
			}
		return 0;
	}
	n = prices[0x1e3];
	if (valueof(item) >= n)
		return 1;
	plclass = classof(item & 0xfff);
	switch (plclass) {
	case 5:
	case 0xf:
	case 0x10:
	case 0x37:
	case 0x64:
		return 1;
	}
	if (all)
		switch (plclass) {
		case 0x39:
		case 0x3c:
		case 0x46:
		case 0x48:
		case 0x4b:
		case 0x55:
		case 0x87:
		case 0x88:
		case 0x89:
		case 0x8a:
		case 0x8b:
			return 1;
		}
	return 0;
}

int
isfurniture(unsigned long code)
{
	code &= 0xfff;
	switch (classof(code)) {
	case 0x37:
	case 0x39:
	case 0x49:
	case 0x4a:
	case 0x84:
	case 0x89:
		return 1;
	case 0:
		if (code >= 0x119 && code <= 0x11d || code == 0x133)
			return 1;
		break;
	}
	switch (typenames[code]) {
	case 4:
	case 0x33:
	case 0x37:
	case 0x58:
	case 0x5a:
	case 0x63:
	case 0x79:
	case 0x8b:
	case 0x98:
	case 0xad:
	case 0xb2:
	case 0xc5:
		return 1;
	}
	if (code == 0x6f0 || code == 0x6fd || code == 0x6fe || code == 0x293 || code == 0x294 || code == 0x219 || code == 0x22e)
		return 1;
	return 0;
}

unsigned long
sackarea(int x0, int y0, int x1, int y1, int n, int mode)
{
	if (x0 < 0)
		x0 = 0;
	if (x1 > 0x95)
		x1 = 0x95;
	if (y0 < 0)
		y0 = 0;
	if (y1 > 0x95)
		y1 = 0x95;
	for (sacky = y0; sacky <= y1; sacky++)
		for (sackx = x0; sackx <= x1; sackx++) {
			sackcell = sackx + sacky * 150;
		again:
			if (map[sackcell] & 0x1000) {
				sackobj = map[sackcell] >> 16;
				sackclass = classof(mapitems[sackobj] & 0xfff);
				if (sackclass != 0xff) {
					for (;; sackobj++) {
						sackitem = mapitems[sackobj];
						// FIXED: the test for a container takes the class of the cell's first item for every item, so the id of a later item can reach past the 125 realms. Take each item's class.
						// FIXED: the test for a container takes the class of the cell's first item for every item, so the id of a later item can reach past the 125 realms. Keep to the realms.
#if defined(FEATURE_FIXES)
						sackclass = classof(sackitem & 0xfff);
						if ((mode == -1 || mode == -2) && sackclass == 10) {
#elif defined(FEATURE_FAULTS)
						if ((mode == -1 || mode == -2) && sackclass == 10 && idof(sackitem) < 125) {
#else
						if ((mode == -1 || mode == -2) && sackclass == 10) {
#endif
							if (!(mode == -2 && buried(sackobj))) {
								sackrealm = idof(mapitems[sackobj]);
								if (sackrealm) {
									realms[sackrealm].item &= ~0x8000000;
									if (mode == -2)
										setid(&realms[sackrealm].item, 0);
								restart:
									for (sackslot = 0; sackslot < 50; sackslot++)
										if (plunderable(realms[sackrealm].items[sackslot], 1) && takeout(sackrealm, sackslot))
											goto restart;
								}
							}
							if (idof(mapitems[sackobj]))
								memset(realms[idof(mapitems[sackobj])].needs, 0, 0x18);
						}
						if (mode == -1) {
							if (plunderable(sackitem, 0) && takeitem(sackcell, sackobj))
								goto again;
						} else if (mode == -2) {
							sackclass = classof(mapitems[sackobj] & 0xfff);
							if (plunderable(sackitem, 1)) {
								if (takeitem(sackcell, sackobj))
									goto again;
							} else if (ishousepart(sackitem))
								mapitems[sackobj] = breakdoor(mapitems[sackobj]);
							else if (ispottery(sackitem) && !iswallns(map[sackcell] & 0xfff) && !iswallew(map[sackcell] & 0xfff))
								mapitems[sackobj] = mapitems[sackobj] & 0xffff000 | potsherd();
							else if (itemburns(sackitem)) {
								setlit(&mapitems[sackobj], 0);
								setsmoke(sackcell - 151, 0);
							} else if (sackclass == 0x86 || sackclass == 0x84 || sackclass == 0x82 || sackclass == 0x83 || sackclass == 0x85
							|| (sackitem & 0xfff) >= 0x271 && (sackitem & 0xfff) <= 0x276
							|| (sackitem & 0xfff) >= 0x27a && (sackitem & 0xfff) <= 0x27c
							|| (sackitem & 0xfff) == 0x293 || (sackitem & 0xfff) == 0x294
							|| (sackitem & 0xfff) == 0x219 || (sackitem & 0xfff) == 0x22e) {
								if (takeitem(sackcell, sackobj))
									goto again;
							}
						} else if (mode == -3) {
							if (isfurniture(sackitem) && takeitem(sackcell, sackobj))
								goto again;
						} else if (mode == -4) {
							if (n && weightof(mapitems[sackobj] & 0xfff) < 3000 && !(mapitems[sackobj] & 0x1000)) {
								sackitem = takeitem(sackcell, sackobj);
								stowregion(0, n, baselook(sackitem));
								goto again;
							}
						} else if (loosekey(sackitem) == mode) {
							sackitem = takeitem(sackcell, sackobj);
							if (sackitem)
								return sackitem;
						}
						if (!(sackitem & 0x8000))
							break;
					}
					continue;
				}
			}
			if (mode == -2 && n && sacky > y0 && sacky < y1 && x0 < sackx && x1 > sackx)
				additem(sackcell, randspeckle());
		}
	return 0;
}

unsigned long
sackregion(short x, short y, int mode)
{
	if (mode == -1 || mode == -2 || mode == -3 || mode == -4)
		;
	else
		mode = loosekey(mode);
	srreg = regionat(x, y);
	if (srreg == 0)
		return 0;
	if (mode == -3 || mode == -4)
		;
	else
		for (sri = 0; sri < 8; sri++) {
			srrealm = regions[srreg].realms[sri];
			if (srrealm != 0) {
			restart:
				for (srslot = 0; srslot < 50; srslot++) {
					sritem = realms[srrealm].items[srslot];
					if (mode == -1) {
						if (plunderable(sritem, 0) && takeout(srrealm, srslot))
							goto restart;
					} else if (mode == -2) {
						if (plunderable(sritem, 1) && takeout(srrealm, srslot))
							goto restart;
					} else if (loosekey(sritem) == mode) {
						sritem = takeout(srrealm, srslot);
						if (sritem)
							return sritem;
					}
				}
			}
		}
	sritem = sackarea(regions[srreg].x0, regions[srreg].y0, regions[srreg].x1, regions[srreg].y1, srreg, mode);
	if (mode == -1 || mode == -2)
		lightregion(srreg, 0);
	return sritem;
}

int
drinkpotion(unsigned long item, int take)
{
	if (!potionknown(item))
		return 0;
	dpitem = newitem(item);
	dpfound = inpack((short *)&dprealm, (short *)&dpslot, dpitem, 0);
	if (dpfound) {
		if (!take)
			return 2;
		dpitem = takeout(dprealm, dpslot);
		if (dpitem) {
			consume(units, dpitem, 3);
			if (classof(dpitem & 0xfff) != 0x38)
				return 3;
			emptyvessel(&dpitem);
			giveitem(0, dpitem, 0);
			return 2;
		}
	}
	return 0;
}

int
fetchitem(Unit *u, unsigned long item, int n)
{
	if (!finditem(u, item, &ferealm, &fecell, &feslot, n))
		return 0;
	if (!ferealm)
		item = takeitem(fecell, feslot);
	else
		item = takeout(ferealm, feslot);
	if (item == 0)
		return 0;
	giveitem(indexof(u), item, 3);
	return 1;
}

int
intoxicated(Unit *u)
{
	if (u->poison || u->drunk)
		return 1;
	return 0;
}

int
takeberserk(int take)
{
	for (tbi = 0; berserkitems[tbi] != 0; tbi++) {
		if (tbi == 0) {
			if (!getbit(game.flags, 0x57d))
				continue;
		} else if (!getbit(game.flags, 0x4b0) && !getbit(game.flags, 0x4b1))
			return 0;
		tbitem = newitem(berserkitems[tbi]);
		tbfound = inpack((short *)&tbrealm, (short *)&tbslot, tbitem, 0);
		if (!tbfound)
			continue;
		if (!take)
			return 1;
		tbitem = takeout(tbrealm, tbslot);
		if (!tbitem)
			continue;
		consume(units, tbitem, 3);
		if (classof(tbitem & 0xfff) != 0x38)
			return 3;
		emptyvessel(&tbitem);
		additem(playerx + playery * 150, tbitem);
		return 2;
	}
	return 0;
}

int
canperform(Unit *u, short cmd, int n, int a, int c)
{
	if (badunit(u))
		return 0;
	if (cheat && keys['W'] & 0x80 && u == units)
		cpcheat = 1;
	else
		cpcheat = 0;
	switch (cmd) {
	case 1:
		if (woulddie(hourticks))
			return 0;
		return 1;
	case 2:
		if (woulddie(hourticks))
			return 0;
		return 1;
	case 6:
		if (u == units) {
			if (game.tiredlevel >= 5 && !cpcheat) {
				denymsg(strs[1373]);
				return 0;
			}
			music(0x22, 0);
		}
		return 1;
	case 8:
		if (lastlight <= 5) {
			denymsg(strs[427]);
			return 0;
		}
		music(0x1d, 0);
		return 1;
	case 4:
	case 7:
		if (u != units)
			return 0;
		if (lastlight <= 5) {
			denymsg(strs[427]);
			return 0;
		}
		music(0x1d, 0);
		return 1;
	case 5:
		if (u != units)
			return 0;
		music(0x1d, 0);
		return 1;
	case 24:
		if (!checklock(a, c, n, 0))
			return 0;
		playnow(0x14);
		return 1;
	case 25:
		return 1;
	case 26:
		cptarget = &units[n];
		if (cptarget == units || isanimal(cptarget))
			return 0;
		cpdx = abs(cptarget->x - u->x);
		cpdy = abs(cptarget->y - u->y);
		if (cpdx > 1 || cpdy > 1) {
			if (u == units)
				denymsg(strs[286]);
			return 0;
		}
		return 1;
	case 27:
		if (u != units)
			return 0;
		cptarget = &units[n];
		if (cptarget == units || isanimal(cptarget) || cptarget->asleep || cptarget->mode == 0x5a || cptarget->sympathy <= 0)
			return 0;
		cpdx = abs(cptarget->x - u->x);
		cpdy = abs(cptarget->y - u->y);
		if (cpdx <= 2 && cpdy <= 2)
			return 1;
		denymsg(strs[286]);
		return 0;
	case 20:
		if (u != units)
			return 0;
		if ((unsigned)n >= nunits)
			return 0;
		if (n != 0) {
			if (isanimal(&units[n]))
				return 0;
			if (units[n].health > 0)
				return 0;
		}
		music(0x20, 0);
		return 1;
	case 19:
		if (u != units)
			return 0;
		music(0x21, 0);
		return 1;
	case 3:
		if (u != units)
			return 0;
		return 1;
	case 10:
		if (u == units && n == 0)
			return 0;
		return 1;
	case 9:
		if (n == 0)
			return 0;
		return 1;
	case 12:
		if (n == 0)
			return 0;
		return 1;
	case 13:
		if (u == units)
			if (n == 0) {
				if (!units[0].bleeding) {
					if (units[0].health >= kindof(units)->maxhealth * 4 / 5) {
						denymsg(strs[1429]);
						return 0;
					}
					denymsg(texts + 0xb23f, strs[381], units[0].health < kindof(units)->maxhealth * 2 / 3 ? strs[380] : strs[379]);
					return 0;
				}
			} else if (!units[n].bleeding) {
				denymsg(strs[1429]);
				return 0;
			}
		return 1;
	case 14:
		if (u == units && !intoxicated(&units[n])) {
			denymsg(strs[1429]);
			return 0;
		}
		return 1;
	case 16:
		if (u == units && sparrer()) {
			denypractice();
			return 0;
		}
		if (u == units)
			music(0x21, 0);
		return 1;
	case 21:
	case 29:
		if (!canenchant(n, a, 0, u, 2, cpcheat))
			return 0;
		return 1;
	case 22:
	case 30:
		if (!canenchant(n, a, 0, u, 0, cpcheat))
			return 0;
		return 1;
	case 23:
	case 31:
		if (!canenchant(n, a, 0, u, 1, cpcheat))
			return 0;
		return 1;
	case 17:
		if (u == units)
			music(0x20, 0);
		return 1;
	case 18:
		if (u == units)
			music(0x21, 0);
		return 1;
	}
	return 1;
}

int
perform(Unit *u, short cmd, int n, int a, int c, short force)
{
	int k;

	if (badunit(u))
		return 0;
	pfres = 0;
	if (u == units && cheat && keys['W'] & 0x80)
		force = 1;
	if (actions[actsel].turns)
		u->pose = 9;
	switch (cmd) {
	case 1:
		if ((units[0].pose == 0 || units[0].pose > 9) && units[0].pose != 10)
			units[0].pose = 0;
		setunitmode(units, 0);
		passtime(hourticks - 2, 0, 1);
		tire(-3);
		pfres = 1;
		break;
	case 2:
		units[0].pose = 0;
		targetkind = 1;
		targetidx = topitem(playercell);
		targetx = playerx;
		targety = playery;
		targetitem = targetidx == 0 ? 0 : mapitems[targetidx];
		pfres = gosleep(1);
		break;
	case 6:
		if (u == units) {
			tire(1);
			train(0x14, 1);
			train(0x15, 5);
		}
		if (!prevails(skillof(u, 0x15, 0, 0, 0), 0x5f) && !force) {
			if (u == units)
				failmsg(strs[436]);
			break;
		}
		sensesleft += halfskill(u, 0x15);
		lookdone();
		pfres = 1;
		if (u == units)
			msgf(0x63, strs[355]);
		break;
	case 8:
		pfc = skillbyte(u, 0x15, 0, 0, 0);
		if (pfc < 0x3a)
			pfc = 0;
		else
			pfc -= 0x3a;
		pfres = pfc;
		if (pfres <= 10)
			pfres = 10;
		flood(u->x, u->y, pfres);
		markers(0);
		pfres = 1;
		tire(1);
		train(0x14, 1);
		train(0x15, 6);
		break;
	case 4:
	case 7:
		if (u != units)
			break;
		tire(1);
		train(0x14, 1);
		train(0x15, 8);
		if (!(randn(100) < 10 || prevails(skillof(u, 0x15, 0, 0, 0), 0x5f) || force)) {
			if (cmd == 4) {
				if (game.cmd4fails >= 2)
					failmsg(strs[428]);
				else {
					game.cmd4fails++;
					failmsg(strs[429]);
				}
			} else
				failmsg(strs[436]);
			break;
		}
		if (cmd == 7) {
			showall = 1;
			if (u == units)
				msgf(0x63, strs[356]);
		} else if (cmd == 4) {
			pffound = 0;
			pfb = -3;
			pfk = 0;
			for (; pfb <= 3; pfb++)
				for (pfa = -5; pfa <= 5; pfa++, pfk++) {
					workbuf[0x1388 + pfk] = 0;
					pfobj = map[pfa + playerx + (pfb + playery) * 150];
					if ((pfobj & 0x1000) == 0)
						continue;
					pfobj >>= 16;
					do {
						pfres = mapitems[pfobj] & 0xfff;
						pfc = classof(pfres);
						if (pfc == 0x69) {
							if (pfc == 0x69 && classof(mapitems[pfobj - 1] & 0xfff) == 0x34)
								mapitems[pfobj - 1] |= 0x10000;
							pffound = 1;
							workbuf[0x1388 + pfk] = 1;
							break;
						}
					} while (mapitems[pfobj++] & 0x8000);
				}
			if (pffound == 0) {
				failmsg(strs[544]);
				break;
			}
			train(0x14, 1);
			train(0x15, 10);
			if (u == units)
				msgf(0x63, strs[1386]);
			viewhook = drawhidden;
		}
		hidecursor();
		memcpy(workbuf, panelstack, 0x10);
		memset(panelstack, 0xff, 0x10);
		drawscreen();
		busyuntil(gametime + 5, 0);
		showall = 0;
		viewhook = 0;
		memcpy(panelstack, workbuf, 0x10);
		drawscreen();
		drawcursor();
		pfres = 1;
		break;
	case 5:
		if (u != units)
			break;
		train(0x14, 1);
		train(0x15, 5);
		if (randn(100) < 10 || prevails(skillof(u, 0x15, 0, 0, 0), 0x5f) || force) {
			pfres = 1;
			break;
		}
		failmsg(strs[544]);
		break;
	case 24:
		pfres = forcelock(a, c, n, 0);
		break;
	case 25:
		k = findclass(c, 0x69);
		if (k)
			takeitem(c, k);
		msgf(0x63, strs[1384]);
		playnow(0x26);
		music(0x16, 0);
		drawscreen();
		pfres = 1;
		break;
	case 26:
		pftarget = &units[n];
		pfa = abs(pftarget->x - u->x);
		pfb = abs(pftarget->y - u->y);
		if (u == units) {
			train(0x14, 1);
			train(0x1a, 5);
			train(0x16, -10);
		}
		if (force || randn(100) < 10 || pftarget->asleep || pftarget->mode == 0x5a && randn(100) < 50
		|| prevails((skillof(u, 0x1a, 0, 0, 0) * 2 + skillof(u, 0x14, 0, 0, 0)) / 3, skillof(pftarget, 0x15, 0, 0, -20))) {
			for (pfk = 0; pfk < 3; pfk++)
				if (pftarget->items[pfk] & 0xfff && valueof(pftarget->items[pfk]) <= valueof(0x514)) {
					targetitem = pftarget->items[pfk];
					pftarget->items[pfk] = 0;
					if (u == units) {
						deedsteal(targetitem);
						msgf(0x63, strs[1086], itemname(targetitem));
						playnow(4);
					}
					giveitem(0, targetitem, 1);
					drawscreen();
					pfres = 1;
					goto done;
				}
			if (u == units) {
				anchortip(targetkind, hoverx, hovery);
				denymsg(strs[544]);
			}
			break;
		}
		provoke(u, pftarget, 2, 1, indexof(u));
		if (u == units)
			msgf(0x63, strs[1370], unitname(pftarget));
		break;
	case 27:
		if (u != units)
			break;
		pftarget = &units[n];
		if (istrader(pftarget) && !myservant(pftarget)) {
			talkto(n);
		take:
			pfres = 1;
			if (cmbautooff)
				autocombat();
			leavemenu(1);
			if (cmbautooff)
				autocombat();
			break;
		}
		if (!gathergoods(pftarget, 0, 0))
			break;
		finishstep(0);
		starttalk(n);
		talkface(n);
		turnto(pftarget, playerx, playery);
		drawscreen();
		talkstate = 2;
		talkactive = 2;
		barter(pftarget);
		talkactive = 0;
		endtalk();
		curunit = 0;
		units[0].pose = 9;
		goto take;
	case 20:
		if (u != units)
			break;
		/* the original passes scene to deadspeaks as a short and tests a
		   short result */
		if (n == 0 || units[n].scene == 0 || u == units && units[n].sympathy <= 0
		    // FIXED: a call through another function type is undefined in C. Call it through its own, and test the low 16 bits as the original did.
#ifdef PORT
		    || !(short)deadspeaks(units[n].scene)) {
#else
		    || !((short (*)(short))deadspeaks)(units[n].scene)) {
#endif
			anchortip(1, hoverx, hovery);
			failmsg(strs[303]);
			break;
		}
		tire(10);
		train(0x14, 1);
		train(0x17, 5);
		if (prevails(skillof(u, 0x14, 0, 0, 0) / 2 + skillof(u, 0x17, 0, 0, 0) + skillof(u, 0x15, 0, 0, 0), 0xdc) || force) {
			talkto(n);
			pfres = 1;
			break;
		}
		failmsg(strs[436]);
		break;
	case 21:
	case 29:
		enchant(n, a, 0, u, 2, force);
		pfres = 1;
		break;
	case 22:
	case 30:
		enchant(n, a, 0, u, 0, force);
		pfres = 1;
		break;
	case 23:
	case 31:
		enchant(n, a, 0, u, 1, force);
		pfres = 1;
		break;
	case 18:
		pfres = sacrifice(n, a);
		calcstats(u);
		if (u != units)
			break;
		if (!pfres) {
			denymsg(strs[1378]);
			break;
		}
		tire(1);
		train(0x14, 1);
		train(0x17, 5);
		break;
	case 19:
		if (u != units)
			break;
		if (game.favor >= 2500 || force)
			game.wodenblows = 1;
		else if (game.invoketime && game.invoketime + 20 >= gametime) {
			msgf(0x63, strs[366]);
			goto tired;
		}
		msgf(0x63, strs[365]);
	tired:
		game.invoked++;
		game.fightinvokes++;
		if (!force && game.favor > -2500)
			if (game.favor - 2500 < -2500) {
				pfres = -2500 - game.favor;
				addfavor(pfres);
				addscore(-pfres);
			} else {
				addfavor(-2500);
				addscore(2500);
			}
		game.invoketime = gametime;
		pfres = 1;
		tire(1);
		train(0x14, 1);
		train(0x17, 15);
		break;
	case 3:
		if (units[n].sympathy < 0)
			units[n].sympathy = 0;
		else if (units[n].sympathy > 4)
			units[n].sympathy = 4;
		if (units[n].sympathy >= 4)
			units[n].traits &= ~0x20;
		if (n == 0)
			msgf(0x63, strs[1445]);
		else if (units[n].sympathy > 0 && units[n].traits & 0x20)
			msgf(0x63, strs[1444], unitname(&units[n]));
		else
			msgf(0x63, strs[1439 + units[n].sympathy], unitname(&units[n]));
		pfres = 1;
		tire(1);
		train(0x14, 1);
		train(0x15, 5);
		break;
	case 10:
		pftarget = &units[n];
		assault(u, pftarget);
		if (u == units) {
			tire(2);
			train(0x14, 1);
			train(0x16, 5);
		}
		if (pftarget->warded || pftarget->blessed || pftarget->courage >= 3 || jewelmagic(pftarget, 0x80000)
		|| !(randn(100) < 10
		|| prevails((skillof(u, 0x16, 0, 0, 0) * 2 + skillof(u, 0x14, 0, 0, 0) + skillof(u, 0x17, 0, 0, 0)) / 4,
		    (skillof(pftarget, 0x14, 0, 0, 0) + skillof(pftarget, 0x18, 0, 0, 0) - skillof(pftarget, 0x17, 0, 0, 0)) * (winodds(pftarget, u) + 50) / 100)
		|| force)) {
			if (u == units)
				failmsg(strs[1371], unitname(pftarget));
			break;
		}
		if (pftarget->berserk || pftarget->bewitched)
			clearmind(pftarget);
		if (pftarget == units) {
			hidecursor();
			playmode = 3;
			pfa = actionturn;
			actionturn = 0;
			cleartexts();
			msgf(0x63, strs[1083]);
			drawscreen();
			actionturn = pfa;
			waitinput(2500);
		}
		addpower(&pftarget->fleeing, u, 0x16);
		pftarget->dirx = 0;
		pftarget->diry = 0;
		calcstats(pftarget);
		if (pftarget != units && inview(pftarget->x, pftarget->y)) {
			msgf(u == units ? 0x63 : 0xd, strs[338], unitname(pftarget));
			if (pftarget->sympathy <= 0)
				music(0x23, 0);
		}
		pfres = 1;
		if (pftarget == units) {
			losecontrol();
			cleartexts();
		}
		break;
	case 9:
		pftarget = &units[n];
		if (foeof(pftarget) != indexof(u) && foe2of(pftarget) != indexof(u) && !(u == units && pftarget->sympathy <= 0)) {
			if (u == units)
				denymsg(strs[370], unitname(pftarget));
			break;
		}
		if (u == units) {
			tire(1);
			train(0x14, 1);
			train(0x16, 5);
		}
		if (pftarget->mode == 0x19)
			goto charm;
		if (pftarget->berserk || pftarget->bewitched)
			goto resist;
		pfa = findheir(pftarget, 1);
		if (pfa && !isfemale(&units[pfa])) {
			if (u == units)
				failmsg(strs[isfemale(pftarget) ? 373 : 372], unitname(pftarget));
			break;
		}
		if (force)
			goto charm;
		if (pftarget->courage >= 3)
			goto resist;
		if (winodds(pftarget, u) < 0x21)
			goto charm;
		if (winodds(pftarget, u) > 0x42)
			goto resist;
		if (pftarget->courage <= 1
		|| pftarget->courage <= 2 && pftarget->health < kindof(pftarget)->maxhealth * 2 / 3
		|| pftarget->health < kindof(pftarget)->maxhealth / 4
		|| prevails((skillof(u, 0x16, 0, 0, 0) * 2 + skillof(u, 0x14, 0, 0, 0) + skillof(u, 0x17, 0, 0, 0)) / 4,
		    (skillof(pftarget, 0x14, 0, 0, 0) + skillof(pftarget, 0x17, 0, 0, 0) - skillof(pftarget, 0x18, 0, 0, 0)) * (winodds(pftarget, u) + 50) / 100)
		|| randn(100) < 10) {
		charm:
			peacehouse(pftarget, u);
			pfres = 1;
			if (u != units)
				break;
			music(0x19, 0);
			pftarget->sympathy = 2;
			if (!isanimal(u))
				msgf(0x63, strs[371], unitname(pftarget));
			break;
		}
	resist:
		if (u == units)
			failmsg(strs[1372], unitname(pftarget));
		break;
	case 11:
		pftarget = &units[n];
		assault(u, pftarget);
		if (isanimal(pftarget))
			break;
		if (u == units) {
			tire(2);
			train(0x14, 1);
			train(0x16, 5);
		}
		if (pftarget->warded || pftarget->blessed || jewelmagic(pftarget, 0x80000)
		|| !(randn(100) < 10
		|| prevails((skillof(u, 0x16, 0, 0, 0) * 2 + skillof(u, 0x14, 0, 0, 0) + skillof(u, 0x17, 0, 0, 0)) / 4,
		    skillof(pftarget, 0x14, 0, 0, 0) + skillof(pftarget, 0x18, 0, 0, 0) - skillof(pftarget, 0x17, 0, 0, 0))
		|| force)) {
			if (u == units)
				failmsg(strs[1371], unitname(pftarget));
			break;
		}
		addpower(&pftarget->weakened, u, 0x16);
		calcstats(pftarget);
		pfres = 1;
		if (pftarget == units) {
			msgf(0x63, strs[394]);
			music(0x1c, 0);
			break;
		}
		msgf(0x63, strs[374], unitname(pftarget));
		if (u == units && inview(pftarget->x, pftarget->y))
			music(0x24, 0);
		break;
	case 12:
		pftarget = &units[n];
		if (isanimal(pftarget)) {
			assault(u, pftarget);
			break;
		}
		if (u == units) {
			tire(5);
			train(0x14, 1);
			train(0x16, 10);
		}
		if (pftarget->warded || pftarget->blessed || ownsland(pftarget) || jewelmagic(pftarget, 0x80000)
		|| !(randn(100) < 10
		|| prevails((skillof(u, 0x16, 0, 0, 0) * 2 + skillof(u, 0x14, 0, 0, 0) + skillof(u, 0x17, 0, 0, 0)) / 4,
		    (skillof(pftarget, 0x14, 0, 0, 0) + skillof(pftarget, 0x18, 0, 0, 0) - skillof(pftarget, 0x17, 0, 0, 0)) * (winodds(pftarget, u) + 50) / 100)
		|| force)) {
			assault(u, pftarget);
			if (u == units)
				failmsg(strs[1371], unitname(pftarget));
			break;
		}
		if (pftarget->berserk || pftarget->fleeing)
			clearmind(pftarget);
		assault(u, pftarget);
		if (u->modeb == n)
			u->modeb = 0xff;
		if (u->modec == n)
			u->modec = 0xff;
		addpower((unsigned char *)&pftarget->bewitched, u, 0x16);
		pftarget->bewitcher = indexof(u);
		calcstats(pftarget);
		setunitmode(pftarget, 0x14);
		if (u->mode == 0x14 || u->mode == 0x15) {
			pftarget->modeb = u->modeb;
			pftarget->modec = u->modec;
		} else {
			pftarget->modeb = 0xff;
			pftarget->modec = 0xff;
		}
		pfres = 1;
		if (u == units) {
			msgf(0x63, strs[375], unitname(pftarget));
			if (inview(pftarget->x, pftarget->y))
				music(0x24, 0);
		}
		break;
	case 13:
		pftarget = &units[n];
		if (u == units) {
			tire(1);
			train(0x14, 1);
			train(0x19, 5);
			train(0x17, 2);
		}
		if (!(randn(100) < 15
		|| prevails(skillof(pftarget, 0x17, 0, 0, 0) / 3 + skillof(u, 0x19, 0, 0, 0),
		    pftarget->bleeding + pftarget->bleeding / 2 - pftarget->blessed / 2 - pftarget->warded / 2 + pftarget->weakened / 3 + pftarget->cursed / 3)
		|| force)) {
			if (u == units)
				failmsg(strs[1374]);
			break;
		}
		music(0x1e, 0);
		if (u == units) {
			train(0x14, 1);
			train(0x19, 5);
			train(0x17, 2);
		}
		pftarget->bleeding = 0;
		if (pftarget->health < kindof(pftarget)->maxhealth * 2 / 3) {
			if (pftarget->health + 5 < kindof(pftarget)->maxhealth * 2 / 3)
				pftarget->health += 5;
			else
				pftarget->health = kindof(pftarget)->maxhealth * 2 / 3 - 1;
		} else if (pftarget->health < kindof(pftarget)->maxhealth - 5)
			pftarget->health += 5;
		else
			pftarget->health = kindof(pftarget)->maxhealth;
		calcstats(pftarget);
		pfres = 1;
		if (u == units) {
			if (n == 0)
				msgf(0x63, texts + 0xb23f, strs[382], units[0].health < kindof(units)->maxhealth * 2 / 3 ? strs[380] : strs[379]);
			else
				msgf(0x63, strs[383], unitname(&units[n]));
		}
		if (u == units && pftarget != u) {
			addsymp(pftarget);
			say(pftarget, 0, 0x575);
		}
		break;
	case 14:
		if (u == units) {
			quaffing = 1;
			pfres = drinkpotion(0x192, 1);
			quaffing = 0;
			if (pfres) {
				force = pfres;
				pfres = 0;
			}
		}
		if (!force && u == units && !getbit(game.known, 0xe))
			break;
		pfres = curepoison(u, &units[n], force);
		break;
	case 15:
		if (u == units) {
			quaffing = 1;
			pfres = drinkpotion(0x181, 1);
			quaffing = 0;
			if (pfres) {
				force = pfres;
				pfres = 0;
			}
		}
		if (!force && u == units && !getbit(game.known, 0xf))
			break;
		pfres = protect(u, &units[n], force);
		break;
	case 16:
		if (u == units) {
			quaffing = 1;
			pfres = takeberserk(1);
			quaffing = 0;
			if (pfres) {
				force = pfres;
				pfres = 0;
			}
		}
		if (!force && u == units && !getbit(game.known, 0x10))
			break;
		goberserk(u, force);
		pfres = 1;
		break;
	case 17:
		castrunes(1);
		pfres = 1;
		tire(2);
		train(0x14, 1);
		train(0x17, 5);
		break;
	}
done:
	actionturn = 1;
	return pfres;
}

void
syncrealms(void)
{
	for (syy = 0; syy < 150; syy++)
		for (syx = 0; syx < 150; syx++) {
			syobj = findclass(syy * 150 + syx, 10);
			if (syobj != 0) {
				syrealm = idof(mapitems[syobj]);
				if (syrealm) {
					realms[syrealm].item &= 0xfffff000;
					realms[syrealm].item |= mapitems[syobj] & 0xfff;
				}
			}
		}
}

int
savemap(unsigned short map)
{
	syncrealms();
	wsprintf(smapname, texts + 0xa44f, map);
	// FIXED: as savegame does, savemap writes the map's record in place. Write it whole.
#if defined(FEATURE_FIXES)
	openwhole(&smapfh);
#else
	fopenw(&smapfh, smapname);
#endif
	if (!smapfh)
		return 0;
	memset(workbuf, 0, sizeof workbuf);
	smapp = memopen(workbuf);
	memwrite(&nunits, 2, 1, &smapp);
	memwrite(&units[1], 0x1b8, nunits - 1, &smapp);
	memwrite(&nrealms, 1, 1, &smapp);
	memwrite(&realms[5], 0x158, nrealms, &smapp);
	memwrite(&nspots, 2, 1, &smapp);
	if (nspots < 1)
		nspots = 1;
	memwrite(((char *)&spots[1]), 0x18, nspots - 1, &smapp);
	memwrite(&nregions, 2, 1, &smapp);
	memwrite(regions, 0xd0, nregions, &smapp);
	memwrite(points, 0x190, nregions, &smapp);
	*(unsigned long *)packbuf = 0xf4f9f4f9;
	smaplen = smapp - workbuf;
	smaplen = pack(packbuf + 4, sizeof packbuf - 4, workbuf, smaplen) + 4;
	// FIXED: savemap reports success whatever its write and its close did, so a slot's save takes a record that the disk refused. Report them, and put the file in place once whole.
#if defined(FEATURE_FIXES)
	smaplen = filewrite(packbuf, smaplen, 1, smapfh);
	return closewhole(&smapfh, smapname, smaplen != 0);
#else
	filewrite(packbuf, smaplen, 1, smapfh);
	fileclose(&smapfh);
	return 1;
#endif
}

// FIXED: the records hold the areas that setarea leaves for a post within 15 cells of the edge, whose low bound wraps past 150 and whose high bound runs past 149, as for 25 units of the release. atpost, fillunits, and residentnear read them as they stand, so unit 18 of map 15, with 252,71,26,101 for its post at 11,86, is never inside its area. Keep them on the map, as setarea's fix does.
#if defined(FEATURE_FIXES)
static void
keeparea(Unit *u)
{
	int h;

	for (h = 0; h < 24; h++) {
		if (u->sched[5][h] > u->sched[7][h])
			u->sched[5][h] = 0;
		if (u->sched[6][h] > u->sched[8][h])
			u->sched[6][h] = 0;
		if (u->sched[7][h] > 149)
			u->sched[7][h] = 149;
		if (u->sched[8][h] > 149)
			u->sched[8][h] = 149;
	}
}
#endif

int
unpackmap(char *buf, int size, char *src, int n, int keep)
{
	nunits = 1;
	nrealms = 0;
	if (n == 0)
		return 0;
	memset(buf, 0, size);
	if (*(unsigned long *)src == 0xf4f9f4f9)
		n = unpack(buf, size, src + 4, n - 4);
	else
		n = unlzw((unsigned char *)buf, (unsigned char *)src, n);
	if (n <= 0)
		return 0;
	umapp = memopen(buf);
	memread(&nunits, 2, 1, &umapp);
	memread(&units[1], 0x1b8, nunits - 1, &umapp);
	for (umapi = 1; umapi < nunits; umapi++) {
		// FIXED: as above, keep the record's areas on the map.
#if defined(FEATURE_FIXES)
		keeparea(&units[umapi]);
#endif
		units[umapi].picset = units[umapi].part + 15;
		if (units[umapi].x >= 150)
			units[umapi].x = 75;
		if (units[umapi].y >= 150)
			units[umapi].y = 75;
		if (units[umapi].health > 0)
			setowned(umapi, 0);
		calcstats(&units[umapi]);
		flagunit(&units[umapi]);
		if (units[umapi].picset & 0x8000) {
			units[umapi].picset &= 0x7fff;
			if (units[umapi].picset > 2)
				units[umapi].picset -= 2;
			else
				units[umapi].picset = 0;
			units[umapi].picset += 15;
		}
		if (units[umapi].health > 0) {
			if (units[umapi].x >= 150)
				units[umapi].x = 75;
			if (units[umapi].y >= 150)
				units[umapi].y = 75;
		}
	}
	memread(&nrealms, 1, 1, &umapp);
	for (umapk = 0, umapi = 0; umapi < nrealms; umapi++) {
		memread(&realms[umapk + 5], 0x158, 1, &umapp);
		if (keep) {
			umapk++;
			continue;
		}
		umapj = 0;
		umapused = 0;
		for (; umapj < nmapitems; umapj++)
			if (classof(mapitems[umapj] & 0xfff) == 10 && idof(mapitems[umapj]) == (unsigned short)(umapi + 5)) {
				umapused = 1;
				setid(&mapitems[umapj], umapk + 5);
			}
		if (umapused) {
			packrealm(umapk + 5);
			umapk++;
		}
	}
	nrealms = umapk;
	memread(&nspots, 2, 1, &umapp);
	if (nspots < 1)
		nspots = 1;
	memread(((char *)&spots[1]), 0x18, nspots - 1, &umapp);
	memread(&nregions, 2, 1, &umapp);
	memread(regions, 0xd0, nregions, &umapp);
	memread(points, 0x190, nregions, &umapp);
	regions[0].tag = 0x63;
	return 1;
}

int
loadmap(unsigned short map)
{
	nunits = 1;
	memset(&units[1], 0, 0x10018);
	nrealms = 0;
	memset(&realms[5], 0, 0xa140);
	nspots = 1;
	memset(spots, 0, sizeof spots);
	if (!visited(map)) {
		enteritems = 0xff;
		entersum = 1;
		lmaplen = readunitrec(packbuf, basemap(map));
		if (lmaplen)
			goto unpack;
	}
	wsprintf(lmapname, texts + 0xa44f, map);
	fopenr(&lmapfh, lmapname);
	if (lmapfh == 0)
		return 0;
	lmaplen = fileread(packbuf, 1, sizeof packbuf, lmapfh);
	fileclose(&lmapfh);
unpack:
	if (!unpackmap(workbuf, sizeof workbuf, packbuf, lmaplen, 0))
		return 0;
	fillregions();
	syncrealms();
	markunits();
	sharecells();
	return 1;
}

unsigned long berserkitems[6] = {
	0x177, 0x178, 0x4be, 0x4bd, 0x4ac
};
unsigned short nunits = 1;
short dirdx[8] = {
	0, 1, 1, 1, 0, -1, -1, -1
};
short dirdy[8] = {
	-1, -1, 0, 1, 1, 1, 0, -1
};
unsigned char licblock[100] = {
	215, 188, 81, 0, 159, 219, 246, 255
};
short playerrank = 255;
unsigned char minmarks = 1;
unsigned char talkanswer = 255;
Gain gains[30] = {
	5, 20, 60, 20, 5, 30, 50, 20, 5, 30, 50, 20, 10, 0, 80, 20, 5, 30, 50,
	20, 0, 20, 50, 30, 10, 0, 70, 30, 5, 0, 50, 50, 5, 0, 0, 100, 5, 0, 0,
	100, 5, 20, 70, 10, 5, 20, 60, 20, 5, 20, 50, 30, 10, 10, 60, 30, 0,
	20, 50, 30, 0, 20, 60, 20, 0, 20, 60, 20, 0, 30, 50, 20, 0, 30, 50,
	20, 0, 20, 60, 20, 5, 15, 15, 70, 5, 10, 20, 70, 5, 25, 20, 55, 5, 30,
	30, 40, 5, 10, 10, 80, 5, 10, 20, 70, 5, 20, 40, 40, 5, 40, 40, 20
};
unsigned char caves[28][2] = {
	95, 18, 15, 19, 117, 21, 62, 24, 40, 30, 134, 34, 84, 38, 100, 41, 48,
	53, 74, 57, 115, 59, 25, 64, 129, 66, 68, 75, 89, 81, 37, 83, 124, 85,
	28, 102, 86, 104, 134, 104, 43, 114, 75, 124, 116, 124, 22, 126, 97,
	133, 39, 135, 136, 135, 59, 137
};
short blocker = 255;
Action actions[40] = {
	0x41b, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -1,
	0, 0x419, 65, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	-1, 0, 0x41a, 66, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, -1, 0, 0x421, 72, 0, 1, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0,
	0, 0, 1, 0, -1, 0, 0x41e, 69, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 1, 0, -1, 0, 0x41c, 67, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 0, 0, 0, 1, 0, -1, 0, 0x41d, 68, 0, 0, 0, 4, 0, 0, 0, 0, 0,
	0, 0, 0, 3, 0, 0, 0, 0, 1, 0, -1, 0, 0x422, 73, 0, 1, 0, 9, 1, 0, 0,
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0, 0x423, 74, 0, 0, 0, 10, 1,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, -1, 0, 0x425, 76, 0, 0, 0,
	12, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 1, 0, -1, 0, 0x426, 77, 0,
	1, 0, 13, 1, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 2, 0, -1, 0, 0x427,
	78, 0, 0, 0, 14, 1, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 2, 0, -1, 0,
	0x42a, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -1,
	0, 0x429, 80, 0, 1, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1,
	0, -1, 0, 0x424, 75, 0, 0, 0, 11, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
	0, 1, 0, -1, 0, 0x428, 79, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,
	0, 0, 0, 1, 0, -1, 0, 0x435, 63, 0, 1, 0, 27, 1, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, -1, 0, 0x434, 90, 0, 0, 0, 26, 1, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, 0x432, 88, 0, 0, 0, 24, 3, 0, 0, 0,
	0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, -1, 0, 0x433, 89, 0, 0, 0, 25, 4, 0,
	0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, -1, 0, 0x42f, 99, 0, 1, 0, 29,
	1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0, 0x42f, 85, 0, 0,
	0, 21, 10, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0, 0x430,
	100, 0, 0, 0, 30, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, -1, 0,
	0x430, 86, 0, 0, 0, 22, 10, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0,
	-1, 0, 0x431, 101, 0, 0, 0, 31, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0,
	1, 0, -1, 0, 0x431, 87, 0, 0, 0, 23, 10, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0,
	0, 0, 1, 0, -1, 0, 0x42c, 82, 0, 1, 0, 18, 6, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, -1, 0, 0x42d, 83, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 0, 0, 0, 1, 0, -1, 0, 0x42e, 84, 0, 0, 0, 20, 9, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0, 0x42b, 81, 0, 1, 0, 17, 0, 0, 0,
	0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, -1, 0, 0x41f, 70, 0, 0, 0, 6, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0, 0x420, 71, 0, 0, 0,
	7, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0, 0x436, 102, 0,
	0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, -1
};
char actunused[8] = "";
Anim anims[13] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 25, 0,
	1, 10, 58, 0, 59, 0, 59, 0, 59, 0, 59, 0, 58, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 0, 10, 45, 0, 62, 0, 63, 0, 63, 0, 63, 0, 62, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 15, 56, 0, 56, 0, 56, 0, 57, 0,
	57, 0, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 15, 60, 0, 60,
	0, 60, 0, 61, 0, 61, 0, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 20, 23, 0, 23, 0,
	23, 0, 31, 0, 31, 0, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	20, 7, 0, 7, 0, 7, 0, 15, 0, 15, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 30, 30, 0, 1, 8, 56, 0, 56, 0, 56, 0, 56, 0, 56, 0, 56, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8, 57, 0, 57, 0, 57, 0, 57,
	0, 57, 0, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 8, 60, 0,
	60, 0, 60, 0, 60, 0, 60, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 8, 61, 0, 61, 0, 61, 0, 61, 0, 61, 0, 61, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 25, 48, 0, 49, 0, 50, 0, 51, 0, 49, 0, 48, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 0, 25, 52, 0, 53, 0, 54, 0, 55, 0, 53, 0, 52, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 23,
	0, 31, 0, 23, 0, 31, 0, 23, 0, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, -1, 0, 0, 7, 0, 15, 0, 7, 0, 15, 0, 7, 0, 15, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 15, 48, 0, 49, 0, 50, 0, 50, 0, 51, 0, 51, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 15, 52, 0, 53, 0, 54, 0, 54, 0, 55, 0,
	55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 10, 23, 0, 23, 0, 16, 0, 16, 0, 23, 0, 23, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 15, 16, 0, 23, 0, 31, 0, 23, 0, 31, 0, 16, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 15, 24, 0, 7, 0, 15, 0, 7, 0, 15, 0, 24,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 50, 0, 1, 10, 58, 0, 59,
	0, 59, 0, 59, 0, 59, 0, 58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 10, 45, 0, 62, 0, 63, 0, 63, 0, 63, 0, 62, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 15, 56, 0, 56, 0, 56, 0, 57, 0, 57, 0, 57, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 15, 60, 0, 60, 0, 60, 0, 61, 0,
	61, 0, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 20, 23, 0, 23, 0, 23, 0, 31, 0, 31,
	0, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 20, 7, 0, 7, 0, 7,
	0, 15, 0, 15, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 10,
	16, 0, 48, 0, 49, 0, 49, 0, 48, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 10, 24, 0, 52, 0, 53, 0, 53, 0, 52, 0, 24, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 51, 0, 50, 0, 51, 0, 50, 0, 51, 0,
	50, 0, 27, 0, 26, 0, 27, 0, 26, 0, 27, 0, 26, 0, 1, 0, 0, 55, 0, 54,
	0, 55, 0, 54, 0, 55, 0, 54, 0, 31, 0, 30, 0, 31, 0, 30, 0, 31, 0, 30,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 20,
	23, 0, 23, 0, 23, 0, 31, 0, 31, 0, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 20, 7, 0, 7, 0, 7, 0, 15, 0, 15, 0, 15, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 50, 48, 0, 48, 0, 49, 0, 49, 0, 48, 0,
	48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 50, 52, 0, 52, 0, 53,
	0, 53, 0, 52, 0, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 20, 23, 0, 23, 0, 23, 0, 31, 0, 31, 0, 31, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 20, 7, 0, 7, 0, 7, 0, 15, 0, 15, 0, 15, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 10, 48, 0, 49, 0, 50, 0, 51,
	0, 50, 0, 49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 10, 52, 0,
	53, 0, 54, 0, 55, 0, 54, 0, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 50, 0, 1, 10, 58,
	0, 59, 0, 59, 0, 59, 0, 59, 0, 58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 10, 45, 0, 62, 0, 63, 0, 63, 0, 63, 0, 62, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 15, 56, 0, 56, 0, 56, 0, 57, 0, 57, 0, 57,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 15, 60, 0, 60, 0, 60, 0,
	61, 0, 61, 0, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 20, 23, 0, 23, 0, 23, 0, 31,
	0, 31, 0, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 20, 7, 0,
	7, 0, 7, 0, 15, 0, 15, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 20, 48, 0, 50, 0, 51, 0, 50, 0, 51, 0, 49, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 20, 52, 0, 54, 0, 55, 0, 54, 0, 55, 0, 53, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 50, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 20, 48, 0, 48,
	0, 49, 0, 49, 0, 50, 0, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 20, 52, 0, 52, 0, 53, 0, 53, 0, 54, 0, 54, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 75, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 75, 23, 0,
	23, 0, 23, 0, 23, 0, 23, 0, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 75, 31, 0, 31, 0, 31, 0, 31, 0, 31, 0, 31, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, -1, 75, 7, 0, 7, 0, 7, 0, 7, 0, 7, 0, 7, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 75, 15, 0, 15, 0, 15, 0, 15, 0,
	15, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 75, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 40,
	23, 0, 23, 0, 31, 0, 31, 0, 23, 0, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 40, 7, 0, 7, 0, 15, 0, 15, 0, 7, 0, 7
};
char signature[17] = "tt*8 A^^--------";
Outfit outfits[7] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0xec01ad, 0xe401ca, 0, 0, 0xe101a0, 0xe401b7,
	0, 0xec01b5, 0, 0, 0, 0, 0xe401b7, 0xe601ba, 0xee01af, 0xe601cc, 0, 0,
	0xe101a0, 0xe401b7, 0xe801bc, 0xef01b0, 0xef01d5, 0xe501cb, 0xf401a9,
	0xe101a0, 0xe401b7, 0, 0, 0xe301c9, 0, 0, 0xe101a0, 0, 0, 0xf701b3, 0,
	0xe401ca, 0, 0, 0xe401b7
};
Seal seal = {
	58, 36, 63, 47, 167, 45, 42, 59, 0, 36, 0xc3a88, 0, 0, 0, 0x12340000
};
Loot loot[34] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x40018e, 0x80018f, 0x972,
	0, 0, 50, 50, 33, 0, 0, 0xe801ce, 0, 20, 0, 0x548, 249, 0x972, 0x57e,
	0x57f, 10, 30, 20, 20, 20, 0, 0, 0, 0, 0x1e2, 0x1e3, 0x4e9, 0x50d,
	0x50e, 50, 50, 20, 20, 20, 0xe401ca, 0, 50, 0, 0x80018f, 0x40018e,
	0x9a6, 0x972, 0, 50, 50, 50, 50, 0, 0, 0, 0, 0, 0x4e9, 0x173, 0x178,
	0x6f1, 0x701, 30, 30, 30, 30, 30, 0, 0, 0, 0, 0x80018f, 0x528, 0, 0,
	0, 70, 50, 0, 0, 0, 0xe301c9, 0xe401ca, 40, 30, 0x40018e, 0x965,
	0x4e9, 0x503, 0, 50, 50, 50, 20, 0, 0xef01d5, 0xe901cf, 50, 50,
	0x40018e, 0x965, 0x4e9, 0, 0, 50, 50, 50, 0, 0, 0xef01d5, 0xe901cf,
	50, 50, 0x80018f, 0x972, 0x528, 0, 0, 30, 10, 33, 0, 0, 0xe401ca,
	0xe301c9, 25, 30, 249, 0x57e, 0x57f, 0x80018f, 0x548, 40, 15, 15, 40,
	15, 0, 0, 0, 0, 0x1e2, 0x1e3, 0x80018f, 0x972, 0x4e9, 35, 35, 25, 10,
	20, 0xe801ce, 0, 15, 0, 249, 0x80018f, 0x2c3, 0x57c, 0x57d, 30, 30,
	30, 10, 10, 0, 0, 0, 0, 0xe401ca, 0x503, 0x4e9, 0x40018e, 0x965, 20,
	40, 30, 40, 40, 0xe901cf, 0xe501cb, 60, 40, 0x503, 0x57d, 0x180,
	0x40018e, 0x965, 30, 30, 30, 30, 30, 0, 0, 0, 0, 0xe401ca, 0x503,
	0x6f1, 0x965, 0x701, 20, 30, 25, 25, 25, 0xe501cb, 0xe901cf, 40, 40,
	0x503, 0x6f1, 0x965, 0x701, 0x180, 30, 25, 25, 25, 25, 0xe401ca,
	0xe501cb, 70, 70, 0x965, 0x40018e, 0x4e9, 0x80018f, 0xe401ca, 30, 30,
	30, 30, 30, 0xef01d5, 0xe901cf, 70, 10, 0x965, 0x40018e, 0x4e9,
	0x80018f, 0xe401ca, 30, 30, 30, 30, 30, 0xef01d5, 0xe901cf, 70, 20,
	0xe401ca, 0x503, 0x4e9, 0x40018e, 0x965, 20, 30, 20, 40, 40, 0xe901cf,
	0xe501cb, 60, 40, 0x965, 0x40018e, 0x503, 0x80018f, 0xe401ca, 30, 30,
	30, 30, 30, 0xe901cf, 0xe501cb, 45, 30, 0x9a6, 0x548, 0x80018f, 0x97f,
	0x221, 10, 20, 20, 20, 20, 0, 0, 0, 0, 0x548, 0x57e, 0x9a6, 0x57f,
	0x97f, 30, 20, 40, 20, 40, 0, 0, 0, 0, 0x80018f, 0x972, 0x184, 0, 0,
	30, 20, 20, 0, 0, 0, 0, 0, 0, 0x1e2, 0x1e3, 0x1000190, 0x972, 0x503,
	35, 35, 25, 10, 20, 0xe801ce, 0, 25, 0, 0x1e2, 0x1e3, 0x80018f, 0x972,
	0x4e9, 35, 15, 25, 40, 20, 0xe801ce, 0, 15, 0, 0x80018f, 0x972,
	0x1000190, 0x40018e, 0x965, 20, 20, 20, 20, 20, 0xe801ce, 0, 15, 0,
	0x1e2, 0x1e3, 0x503, 0x50d, 0x50e, 70, 70, 30, 35, 35, 0xe901cf,
	0xe501cb, 40, 30, 0x80018f, 0x972, 0, 0, 0, 30, 30, 0, 0, 0, 0xe301c9,
	0, 35, 0, 249, 0x57e, 0x57f, 0x80018f, 0x972, 40, 15, 15, 40, 15, 0,
	0, 0, 0, 249, 0x57e, 0x57f, 0x80018f, 0x972, 40, 15, 15, 40, 15, 0, 0,
	0, 0, 0x972, 0x40018e, 0x4e9, 0x80018f, 0xe401ca, 30, 30, 30, 30, 30,
	0xef01d5, 0xe501cb, 70, 70, 0x1eb, 0x40018e, 0x503, 0x80018f,
	0xe401ca, 30, 30, 30, 30, 30, 0xe501cb, 0xe401ca, 70, 70, 0x1eb,
	0x40018e, 0x503, 0x80018f, 0xe401ca, 30, 30, 30, 30, 30, 0xe501cb,
	0xe401ca, 70, 70
};
unsigned char lic[72] = {
	84, 186, 226, 64, 119, 23, 225, 72, 222, 242, 153, 195, 190, 93, 82,
	167, 220, 19, 240, 167, 0, 0, 0, 0, 52, 53, 54, 55, 56, 57, 65, 66,
	67, 68, 69, 70, 71, 72, 73, 74, 156, 57, 191
};
int skillpts;
int skstep;
int sksum;
int skleft;
int skworth;
short traink;
int stealval;
int daggerval;
int defeatpts;
short bootyval;
short trackreg;
short mucell;
unsigned long sitflags;
unsigned char uatidx;
Unit *uatunit;
unsigned char oatidx;
Unit *oatunit;
unsigned long ixoff;
unsigned short shhome;
unsigned short shhome2;
unsigned short sphome;
unsigned short sphome2;
short ahhome;
short ahreg;
int fuunit;
int fuhour;
short areareg;
short areahour;
short udistx;
short udisty;
short stepdx;
short stepdy;
int pdux;
int pduy;
int pdvx;
int pdvy;
short rebest;
short redist;
short rei;
short reexit;
short pebest;
short pedist;
short pei;
short peexit;
short fpi;
short fpunit;
short fpx;
short fpy;
int fpcell;
unsigned char fpfree[8];
short fpnfree;
unsigned char fpopen[8];
short fpnopen;
short fgunit;
short rtfrom;
short rtto;
short rtexit;
short afi;
short fdi;
short fdunit;
short fddist;
short fdmin;
short nfunit;
short nffoe;
short uaunit;
short sprunit;
signed char ssdx0;
signed char ssdy0;
int stdx;
int stdy;
signed char stdx0;
signed char stdy0;
short scunit;
short scself;
short scsunit;
unsigned char smold;
char smmoved;
int smarmor;
int smslot;
short smowner;
short pnx0;
short pny0;
short pnw;
short pnh;
short pnterr;
unsigned char pnprov;
short plreg;
short plplace;
unsigned char plx0;
unsigned char ply0;
int busycell;
short pbdx;
short pbdy;
short sfunit;
int oscell;
short osself;
short osplace;
short osreg;
short osx0;
short osy0;
short apreg;
short appost;
short plsunit;
short rnrad;
short rnunit;
short rnhour;
short rnreg;
short rnureg;
short tfface;
unsigned long vfval;
short vfpct;
short orunit;
short nmunit;
short nmfemale;
short nmseen;
short oareg;
short oawide;
short nrreg;
short nrwide;
int ocidx;
unsigned char occls;
short ocslot;
short ocrealm;
unsigned long daidx;
unsigned long daslot;
unsigned long darealm;
short orgx;
short orgy;
short olx;
short oly;
short fhunit;
short fhself;
Unit *fhcand;
unsigned char fhfoe;
unsigned char fhfoe2;
short hoheir;
short bqheir;
short bqi;
short bqself;
short abheir;
short abself;
short abi;
short abmaster;
short ntnear;
short ntreg;
short nti;
short ntpt;
short ntx;
short nty;
short ntkind;
int ntidx;
short immsum;
short immval;
unsigned long immitem;
unsigned long coidx;
unsigned long scoidx;
unsigned long scolook;
unsigned long scotype;
short snapk;
short csval;
short csload;
short rhours;
short rfgain;
short rfhours;
Unit *nuunit;
short nui;
short nuj;
short nupack;
short nuweapon;
short nurank;
short nuarmor;
unsigned long nuitem;
unsigned short fntry;
int fncell;
short fndy;
int ununit;
unsigned short pign;
short pigx;
short pigy;
int cloakroll;
unsigned long cloakitem;
int goodsroll;
int utensilroll;
int dropx;
int dropy;
int dropr;
int dropheavy;
int puttop;
unsigned long groundcell;
int ncdist;
int ncbest;
int ncmin;
int nci;
int caveroll;
int acx;
int acy;
int aci;
unsigned long accell;
int strewcell;
int strewroll;
int fcx;
int fcy;
int fcn;
int fci;
int fccell;
unsigned long fcv;
int fcsi;
unsigned short arunit;
unsigned short ari;
short ark;
short arface;
unsigned short romunit;
unsigned short avunit;
unsigned short refunit;
short dui;
short standsum;
short standi;
short standself;
short tgplayer;
short tgdist;
short tgdist2;
short tgtmp;
unsigned char tgunit;
short angera;
short angerb;
short provi;
short prova;
short provb;
unsigned char puclass;
short puslot;
short keyslot;
short srgi;
short sedpart;
unsigned char giclass;
unsigned long giorig;
unsigned long gitmp;
unsigned long gival;
unsigned long gimin;
unsigned long gilook;
short gii;
short gislot;
short gihome;
short gij;
int gitop;
short handi;
short thhour;
unsigned char thx;
unsigned char thy;
short orphhour;
short subi;
short subself;
short subunit;
short subpass;
int killv;
unsigned short killcell;
unsigned short killi;
short killled;
short killfoe;
short killplayer;
short killfair;
unsigned short dcobj;
unsigned short dclook;
short dcbones;
unsigned short dccell;
unsigned short dcalt;
unsigned long dcword;
int dcx;
int dcy;
short ofhour;
short ighour;
short rgi;
short rghour;
short cnfill;
unsigned short cnv;
unsigned char cnclass;
short cndrink;
short cnmsg;
int cpfrom;
int cpto;
short fsdir;
short fsunit;
short fsdx;
short fsdy;
short fsx;
short fsy;
unsigned short fscell;
short bsi;
short bsdir;
short bsv;
short bsx;
short bsy;
short bsdist;
short bsprov;
unsigned long bscell;
short tsdir;
short newhealth;
Unit *torchu;
short torchi;
short turni;
Unit *mvu;
unsigned short mvi;
int mvcell;
int mvdoor;
unsigned short mvx;
unsigned short mvy;
short bkstat;
short bklevel;
short sostat;
short solevel;
short sopct;
short sbv;
unsigned char hsv;
short oddsv;
unsigned char pwn;
short pwatk;
short pwdef;
short pwprot;
short pwparry;
short pwlo;
short pwhi;
short pwkind;
short wou;
short wov;
short charmpct;
short hsrow;
short bai;
short bapass;
short spardist;
short spari;
short sparany;
int hli;
int lcobj;
int lccell;
int lri;
short pplist;
short ppi;
short pptry;
short ppx;
short ppy;
short ppj;
short ppself;
short ppfire;
int ppcell;
int wni;
int wnrank;
int wnbest;
int wnmin;
int wndist;
int hmi;
Unit *curu;
int curi;
unsigned long uttmp;
int utcell;
short uta;
short utb;
short utc;
short utdx;
short utdy;
short utblood;
int utdoor;
int uthere;
short utwalk;
short utmode;
unsigned char utanim;
short utcow;
short utdog;
short utpig;
short uthorse;
short utseen;
short utpdx;
short utpdy;
short utfx;
short utfy;
unsigned long sumi;
unsigned long sumj;
unsigned long sumv;
HANDLE savefh;
unsigned char *savep;
char *saveat;
unsigned long savesum;
int savex;
HANDLE loadfh;
unsigned char *loadp;
char *loadpos;
unsigned long loadsum;
unsigned long loadsize;
int loadtmp;
short saveditems;
int savedsum;
short loadlang;
int fixi;
int fixj;
int fixk;
int fixx;
int fixy;
int lootx;
int looty;
short lootarea;
short lootlord;
short looti;
short lootb;
short loothow;
short lootn;
unsigned long lootobj;
int lootcell;
int lootdest;
unsigned long lootprev;
unsigned long lootnext;
unsigned long lootunit;
Unit *lootu;
int scanx;
int scany;
unsigned short scanobj;
unsigned short scancell;
unsigned long scanv;
unsigned long scanw;
unsigned short scankind;
unsigned long backdiff;
unsigned long backhours;
unsigned long backdays;
short backhour;
short trvc;
short trvr;
short trvunit;
short trvn;
int trvhour;
int trvlead;
short trvmode;
short trvflag;
short trvmoved;
unsigned long trvtime;
unsigned long trvhours;
short trvhour0;
short trvday0;
short trvmsg;
int trvok;
short movi;
short movx;
short movy;
int movcell;
unsigned long *cep;
int frfaith;
unsigned long *enp;
unsigned short enbless;
unsigned short endiff;
unsigned char enpower;
unsigned long *sacp;
unsigned long sachi;
unsigned char sacclass;
unsigned char sacgear;
unsigned short sacval;
unsigned long *adjmap;
unsigned long *adjflags;
unsigned short vaccell;
short summi;
short summx;
short summy;
short dhx;
short dhy;
short dhk;
unsigned char protpot;
unsigned char protspell;
unsigned char rage;
int ragecell;
short ragesave;
short mphour;
short pmi;
short pmself;
short phi;
short phself;
short pai;
short paself;
unsigned char goodsclass;
int tradei;
int ggn;
int ggi;
int ggj;
int gghome;
int ggk;
int ggrealm;
int ggx;
int ggy;
int ggpass;
int ggmaster;
short ggowner;
short ggspouse;
unsigned long ggitem;
int ggslot;
short barteri;
unsigned short bartercur;
short seekreg;
short seeki;
short seekrealm;
short seekslot;
short seekx;
short seeky;
short seekx0;
short seekx1;
short seeky0;
short seeky1;
short seekowner;
short seekself;
short seekfood;
short seekheavy;
unsigned long seekitem;
int seekcell;
int seekobj;
unsigned long seeklast;
unsigned long (*seekkey)(unsigned long item);
unsigned long plclass;
unsigned long plname;
int sackx;
int sacky;
int sackrealm;
int sackslot;
int sackcell;
int sackobj;
unsigned long sackitem;
short sackclass;
short srreg;
short sri;
short srrealm;
short srslot;
unsigned long sritem;
unsigned long dpfound;
unsigned long dpitem;
unsigned short dprealm;
unsigned short dpslot;
int ferealm;
int fecell;
int feslot;
unsigned long tbfound;
unsigned long tbitem;
int tbi;
unsigned short tbrealm;
unsigned short tbslot;
Unit *cptarget;
short cpdx;
short cpdy;
short cpcheat;
int pfres;
Unit *pftarget;
unsigned long pfobj;
short pfa;
short pfb;
short pfk;
unsigned char pfc;
unsigned char pffound;
int syx;
int syy;
int syobj;
short syrealm;
HANDLE smapfh;
char smapname[16];
unsigned long smaplen;
char *smapp;
char *umapp;
unsigned char umapused;
int umapi;
int umapj;
int umapk;
HANDLE lmapfh;
char lmapname[16];
unsigned long lmaplen;
Unit units[150];
short freedirs[8];
short nfreedirs;
short bestdirs[8];
short nbestdirs;
unsigned short stepcell;
unsigned short dircells[8];
short dirdoors[8];
short dirscores[8];
short challenger;
short merciful;
short pleader;
short talker;
short talkscene;
Game game;
char szvars[400];
short gamesbegun;
Fame hiscores[13];
int scoreunused;
int notes[500];
unsigned short nnotes;
char scenevars[5000];
int scenevarlen;
unsigned short talkstate;
unsigned short curunit;
short trvfrom;
short trvto;
short trvleft;
short avengers;
short victimfemale;
short victimname;
short victimzone;
short topimmob;
unsigned long toppiece;
short nextimmob;
unsigned long nextpiece;
char caveused[28];
int strewlast;
short wasdoomed;
int quaffing;
int lineokx;
int lineoky;
int lineendx;
int lineendy;
short sparending;
unsigned long barkat;
int clocktime;
unsigned short clockhour;
short clockday;
int clocktime2;
unsigned short clockhour2;
short clockday2;
unsigned long mapsum;
unsigned char runepast;
unsigned char runenow;
unsigned char runenext;
int nfound;

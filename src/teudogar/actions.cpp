#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

typedef struct Unit Unit;
typedef struct Panel Panel;
typedef struct Spot Spot;
typedef struct Realm Realm;
typedef struct Menu Menu;
typedef struct Node Node;
typedef struct Metal Metal;
typedef struct Gear Gear;
typedef struct Zone Zone;
typedef struct Game Game;

#define T(off) (texts + (off))

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
	char fa[0xc - 0xa];
	unsigned char modec;
	unsigned char moded;
	char fe[0xe9 - 0xe];
	unsigned short scene;
	char feb[0xed - 0xeb];
	short strength;
	short dexterity;
	char ff1[0xf3 - 0xf1];
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
	unsigned long items[3];
	unsigned long cloak;
	unsigned short load;
	char f12e[0x135 - 0x12e];
	unsigned char asleep;
	char f136[0x139 - 0x136];
	unsigned char cursed;
	char f13a[0x13b - 0x13a];
	unsigned char confused;
	char f13c[0x143 - 0x13c];
	unsigned char traits;
	char f144[0x146 - 0x144];
	signed char sympathy;
	char f147[0x149 - 0x147];
	unsigned char lord;
	char f14a[0x155 - 0x14a];
	unsigned char incoming;
	char f156[0x159 - 0x156];
	unsigned char modee;
	char f15a[0x161 - 0x15a];
	short face;
	char f163[0x165 - 0x163];
	unsigned char spouse;
	char f166[0x16f - 0x166];
	unsigned char talkflags;
	char f170[0x1b8 - 0x170];
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
	char f1a[0x20 - 0x1a];
	unsigned long item;
	char f24[0x28 - 0x24];
};

/* 0x18 bytes each (005ac378) */
struct Spot {
	unsigned short tomap;
	unsigned char x;
	unsigned char y;
	unsigned char hours;
	char unused;
	char song;
	char f7[0x18 - 7];
};

/* 0x4c bytes each: a node of the dialogue script (006559c0) */
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

/* 0x1b bytes each (004c08a8): kinds of goods */
struct Metal {
	char f0[0x12];
	short days;
	short metal;
	unsigned char kind;
	unsigned char thrown;
	unsigned char material;
	char f19[0x1b - 0x19];
};

/* 0x1a bytes each (004c0bd8): kinds of food */
struct Gear {
	char f0[0x13];
	short days;
	short metal;
	unsigned char material;
	char f18[0x1a - 0x18];
};

/* 0x158 bytes each (005a1b78) */
struct Realm {
	unsigned long item;
	char f4[0x158 - 4];
};

/* 0x22 bytes each (004f9498) */
struct Zone {
	short id;
	unsigned short name;
	char f4[0x12 - 4];
	short inx;
	short iny;
	char f16[0x22 - 0x16];
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
	char wodenblows;
	unsigned long sunk;
	char outfittext[2];
	unsigned short runkills;
	char f1a0[0x1d4 - 0x1a0];
	unsigned char oldlevels[30];
	unsigned char levels[30];
	unsigned long snaptime[30];
	short savespare;
	unsigned char fate;
	short killer;
	unsigned short mercies;
	unsigned short womenhit;
	unsigned short quests;
	unsigned long booty;
	int score;
	short id;
	int snappoints[30];
	char fightinvokes;
	unsigned short poisoned;
	short faces[20];
	char f340[0x39a - 0x340];
	unsigned char fishcount;
	unsigned char tries;
	char foes[30];
	char f3ba[0x3ce - 0x3ba];
	unsigned char combatbless;
	char f3cf[0x3dc - 0x3cf];
	short absenttime;
	short backday;
	short lastregion;
	int regiontime;
	int talkendtime;
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

unsigned char classof(int item);
int topitem(int pos);
short iswallns(unsigned long k);
short iswallew(unsigned long k);
unsigned long baselook(unsigned long item);
unsigned long eastlook(unsigned long item);
unsigned long southlook(unsigned long item);
unsigned long randlook(unsigned long item);
short canstack(unsigned long a, unsigned long item);
unsigned long addcoins(unsigned long *p, unsigned long b);
void clamptorch(unsigned long *item);
short islittorch(unsigned long item);
short setlit(unsigned long *item, short n);
short istorch(unsigned long item);
void listtorches(void);
void updatecell(int pos);
unsigned long additem(int pos, unsigned long item);
unsigned short findclass(int pos, unsigned char c);
unsigned long takeitem(int pos, unsigned long idx);
unsigned long popitem(int pos, char any);
int sackout(unsigned long *item);
int sackin(unsigned long *item);
void syncplayer(void);
void gametick(void);
int fademed(int in);
short msgf(short prio, char *fmt, ...);
short denymsg(char *fmt, ...);
short asksteal(short m);
short woulddie(unsigned long t);
short issword(unsigned long item);
void skillmsg(void);
void hidecursor(void);
void redrawcursor(void);
void hoversound(void);
void resetinput(void);
void leavemenu(short anim);
void waitrelease(short anim);
int autosave(void);
void resetpose(Unit *u, unsigned char v);
void markunits(void);
short foedist(void);
short nearfoe(int n);
short sparrer(void);
void turnto(Unit *u, short x, short y);
void setunitmode(Unit *u, unsigned char mode);
void calcload(unsigned short n);
int playerout(void);
void snapskills(int mode);
void calcstats(Unit *u);
short diagfree(short x, short y, short dx, short dy);
short canpass(int x0, int y0, int x1, int y1);
void addscore(int n);
void enchant(int n, int a, unsigned long *p, Unit *u, unsigned short mode, short force);
int travel(unsigned short dest, int x, int y, unsigned long time, unsigned short depart, unsigned short a6, unsigned short mode, short a8);
short isanimal(Unit *u);
void queuesound(short n);
void playqueue(void);
int stopmusic(void);
int music(int n, int lp);
int songbychar(char c);
short unconscious(void);
void lookaround(void);
void lookdone(void);
void plotcheck(void);
void plottreasure(short m, short x, short y);
void plotcombat(short a);
int __cdecl readinput(void);
void cleartip(void);
void cleartexts(void);
void showtip(int a, int b, char *fmt, ...);
void anchortip(unsigned char kind, int x, int y);
void resetanims(void);
void openpanel(unsigned char n);
void movepanel(unsigned char n, short x, short y);
void closepanel(unsigned char n);
void closepanels(void);
void describe(unsigned long item, short n);
void drawscreen(void);
int stowat(unsigned short k, unsigned short n, unsigned long item);
int stowspot(int k, int x, int y, unsigned long item);
unsigned long takeout(unsigned short k, unsigned short n);
unsigned long takeoff(int n, int slot);
unsigned long puton(short u, unsigned long item);
unsigned short itemweight(unsigned long item);
void setcurregion(void);
unsigned short idof(unsigned long item);
unsigned short wearof(unsigned long x);
char *itemname(unsigned long item);
short askmsg(char *s);
void keeplast(void);
int asktarget(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e,
	char f, char g, char h, char i, char j);
void delmsgid(short id);
short failmsg(char *fmt, ...);
unsigned char itempanel(unsigned long item);
int pulllever(unsigned long *p);
void plotdoor(short a, short x, short y);
void expandstr(char *dst, char *src);
int wrap(char *s, int width);
int textwin(char *s, char a, unsigned char b, unsigned char c, void (*pagefn)(short), Menu *m, int (*fn)(int), unsigned char mode);
void trackregion(void);
void dropheld(void);
short provat(short x, short y);
short regionat(short x, short y);
short celldist(int x, int y, int x2, int y2);
int itemburns(unsigned long item);
int setsmoke(int pos, short dir);
void dresshero(void);
void calctorch(void);
void scavenge(short all);
void decaycorpses(void);
void setdaylight(void);
void placeunits(short h);
void clrboth(void);
void setlevels(void);
void closeborder(void);
void clearturn(void);
void clearscenes(void);
short myservant(Unit *u);
int inview(int x, int y);
void killunit(Unit *u, short a, short b, short c, short d);
void nexthour(void);
int rununits(short mode);
int moveunits(short mode);
void clearqueue(void);
void tickhealth(Unit *u);
void sharecells(void);
void agetorches(unsigned long n, int all);
int fadefast(int in);
char *daytime(unsigned char h);
void tickstatus(Unit *u);
unsigned long inpack(short *pk, short *pj, unsigned long item, int norm);
int wornslot(Unit *u, unsigned long item, int norm);
void playnow(short n);
short isweapon(unsigned long item);
short isthrowing2(unsigned long item);
unsigned long giveitem(short n, unsigned long item, short mode);
int packitem(int item);
int packordrop(int item);
unsigned short gearof(unsigned long item);
void setwear(unsigned long *p, unsigned long v);
unsigned long *itemptr(unsigned char where, int n);
void tire(short n);
void train(unsigned char k, int n);
unsigned char weaponskill(void);
int randpit(void);
void addfavor(int n);
char *pick(char *de, char *en, char *fr, char *es, char *it, char *pl);
int outofreach(int a, int x, int y, unsigned long item, int b, int c);
int counttypes(unsigned short k, unsigned long a, unsigned long b, unsigned long c);
int taketypes(int k, unsigned long a, unsigned long b, unsigned long c, int n, int who);
unsigned long newitem(unsigned long item);
short usefail(char *s);
unsigned long finditem(Unit *u, unsigned long w, int *a, int *b, int *c, int d);
short repairable(unsigned long item);
void savetarget(void);
void restoretarget(void);
short skillof(Unit *u, short n, short b, short c, short d);
short prevails(short a, short b);
unsigned long broken(unsigned long item);
void forgettarget(void);
void notetarget(void);
char *typetext(unsigned long item);
int needredraw(int c);
short unmanly(unsigned long item, short msg);
int opencontainer(void);
int consume(Unit *u, unsigned long item, short v);
int pickcrop(unsigned long *item);
void emptyvessel(unsigned long *item);
int waterat(int x, int y);
short isfemale(Unit *u);
int chaste(short a, short b);
int cansee(int x, int y);
short isservant(Unit *u);
char *unitname(Unit *u);
int usecorpse(short use);
void denycombat(void);
short isunlittorch(unsigned long item);
int hasaction(unsigned char c);
int doaction(short flag);
int diggable(int x, int y);
int forceopen(short k);
unsigned char unitat(int x, int y);
int opendoor(unsigned long pos, short force);
void travelmap(int mode, short a, short b);
int findbrace(char *p);
int loadscript(char *name);
int expandnodes(void);
int parsescene(void);
void savevars(void);
void staydays(short k);
short shownode(void);
int fadeslow(int in);
int flip(void);
void backto2(void);
void s2toback(void);
void s2tobackrows(int y, int dy, int h);
void primto2(void);
void s2toprim(void);
void dissolvet(void);
void animalcry(Unit *u);
void drawcursor(void);
void setcursor1(int kind, int img);
void clicksound(void);
void entermenu(void);
void waitinput(int ms);
int drawactions(void);
int edgemenus(short mode);
short aboutfull(int a, int b);
void learntalk(void);
short seesunit(Unit *u);
int displayed(Unit *u);
void talkface(short n);
void starttalk(short n);
void endtalk(void);
short mapzone(short id);
unsigned long travelhours(short a, short b);
unsigned long traveltime(unsigned long n);
void motherwait(void);
void motherback(void);
void adalmarwait(void);
void adalmarback(void);
void teudogarwait(void);
int teudogarback(void);
int plotheir(short m, short k);
void tobattle(void);
int __cdecl pollinput(void);
void screenpos(Unit *u);
void drawface(int x, int y, char *buf, int n);

extern unsigned char targetkind;
extern unsigned long targetitem;
extern int playerx;
extern int playery;
extern int targetx;
extern int targety;
extern int playmode;
extern int hovery;
extern unsigned long map[150 * 150];
extern unsigned long mapitems[];
extern unsigned long tradeoffer[];
extern unsigned long gametime;
extern unsigned short busyturns;
extern char unitsrun;
extern Unit units[];
extern char *strs[];
extern int lefttime;
extern int tracesite;
extern int hoverx;
extern int hoverpx;
extern int hoverpy;
extern Panel panels[];
extern int ptrdx;
extern int ptrdy;
extern int ptrdistx;
extern int ptrdisty;
extern int waitforup;
extern unsigned short steptick;
extern unsigned short stepframe;
extern int drawto2;
extern int bloodtill;
extern char trademode;
extern char combatcause;
extern unsigned short nunits;
extern unsigned short playercell;
extern int targetidx;
extern short targetowner;
extern unsigned short notedunit;
extern short askedonce;
extern short askedunit;
extern unsigned char cmbautooff;
extern char empty[1];
extern unsigned char infopanel;
extern int mousemoved;
extern int autolevel;
extern unsigned long mapflags[150 * 150];
extern char talkactive;
extern char lastspeech;
extern char knockedout;
extern char lbltext[];
extern short lblx;
extern short lbly;
extern short skiptick;
extern short toggling;
extern short inautocombat;
extern short foeunit;
extern char texts[47942];
extern char tmpbuf[2500];
extern unsigned char typenames[];
extern Gear armors[];
extern short freetake;
extern int mouseleft;
extern int leftclick;
extern Game game;
extern int tipanchorx;
extern int tipanchory;
extern int cheat;
extern unsigned char keys[256];
extern short censorship;
extern char mapstate[100];
extern short curregion;
extern unsigned char panelstack[16];
extern char picbuf[308200];
extern int ptrpace;
extern short heropace;
/* the first of three shorts (005590c8 to 005590cd) that advance reads as an array */
extern short walkedsum;
extern unsigned short playernext;
extern unsigned char notedkind;
extern int notedposx;
extern int notedposy;
extern unsigned long noteditem;
extern unsigned long notedtime;
extern short lastfail;
extern unsigned long acttime;
extern Metal metals[];
extern char printbuf[];
extern unsigned char targetcls;
extern short combatkind;
extern short actionturn;
extern int lostcontrol;
extern int maploaded;
extern Spot spots[];
extern char spotsong;
extern short spotday;
extern short gameday;
extern unsigned short hourticks;
extern unsigned short curmap;
extern unsigned char palblack;
extern short sparending;
extern Realm realms[125];
extern short nmsgs;
extern short lastlight;
extern int nstrs;
extern char packbuf[];
extern Menu menu1;
extern Node nodes[255];
extern Node choices[4];
extern unsigned short nnodes;
extern unsigned short talkstate;
extern short talky;
extern unsigned char gamehour;
extern unsigned char passmode;
extern int curscene;
extern int chkpending;
extern int startpending;
extern int licpending;
extern int talkok;
extern unsigned char talkanswer;
extern Zone zones[];
extern char scriptfile[];
extern char speech[];
extern short telemapno;
extern short teleposx;
extern short teleposy;
extern short telehours;
extern short sendposx;
extern short sendposy;
extern short talknext;
extern short gobattle;
extern unsigned long staylen;
extern char curface[];
extern char curface2[];
extern char talkpic[];
extern int wrongcopy;
extern int licseal;
extern int licenseok;
extern DWORD ordercmd;
extern int mouseright;
extern int rightclick;
extern int dblclick;
extern int statshown;
extern int noflip;
extern unsigned short curunit;
extern int quitting;
extern char keycode;
extern char lastscan;
extern unsigned char esckey;
extern unsigned char menumode;
extern int talkchoice;
extern int litchoice;
extern short curblink;
extern short screenposx;
extern short screenposy;

extern unsigned short liftn;
extern int liftres;
extern unsigned long dropw;
extern int placek;
extern int placepos;
extern int placex;
extern int placey;
extern int busysave;
extern int busyres;
extern short combati;
extern short autofoe;
extern unsigned char infocls;
extern short infounit;
extern int otherpos;
extern int trywt[2];
extern int tryx;
extern int tryy;
extern char trycount;
extern unsigned short trycell;
extern char tryalt;
extern int advwx;
extern int advwy;
extern int advunit;
extern char advtry;
extern unsigned short advcell;
extern char advalt;
extern int spotn;
extern unsigned long *keylock;
extern short keyres;
extern short keyask;
extern char *readtxt;
extern char *readscan;
extern unsigned short readno;
extern int placedir;
extern short underi;
extern short underxy;
extern short underw;
extern short undern;
extern Node *undertab;
extern short byei;
extern short byefound;
extern char *byep;
extern char *byeq;
extern short byen;
extern Node *byetab;
extern short nodecnti;
extern short nodecnt;
extern short talkplain;
extern int talkcode;
extern short talknode;
extern short talkstep;
extern short talkmx;
extern short talkmy;
extern short talki;
extern short talknew;
extern unsigned char talktop;
extern unsigned char talkbottom;
extern unsigned char talkleft;
extern unsigned char talkright;
extern short talkbegun;
extern unsigned long talkwait;
extern char *talkp;
extern short talkslide;
extern short talkprev;
extern char talkknown[30];
extern int talkunit;
extern short idleunit;
extern unsigned long *firep;
extern short firelit;
extern short tohours;
extern int toticks;
extern short ambi;
extern unsigned long passdone;
extern short passi;
extern unsigned long aloneticks;
extern unsigned long awayticks;
extern unsigned long awaytime;
extern short awayi;
extern short awayhour;
extern short awayday;
extern int awayx;
extern int awayy;
extern unsigned short sleepslot;
extern unsigned char bedkind;
extern unsigned char bedcls;
extern unsigned long sleepwait;
extern int sleepx;
extern int sleepy;
extern short grindwear;
extern unsigned long *mixp;
extern unsigned char mixcls;
extern int mixres;
extern short mixask;
extern short digtrap;
extern char rephammer;
extern char reppliers;
extern char repfire;
extern char repanvil;
extern int repval;
extern short repask;
extern short repmetal;
extern short repsoft;
extern int useres;
extern int usedx;
extern int usedy;
extern unsigned char usecls;
extern short usewater;
extern unsigned long useother;
extern int usebusy;
extern short useask;
extern short skilldue;

int
liftitem(short ask)
{
	switch (targetkind) {
	case 0:
	case 0x28:
		liftres = 0;
		goto out;
	case 0x29:
	case 0x2a:
		if (targetx == 999) {
			liftres = 0;
			goto out;
		}
		if (ask) {
			liftn = panels[targetkind - 0x28].arg;
			if (liftn == 0 || liftn == 2 && targetx != 0x28 && targetx != 0x29 && targetx != 0x2a) {
				liftres = 0;
				goto out;
			}
		}
		liftn = panels[targetkind - 0x28].id;
		targetitem = takeoff(liftn, targetx);
		calcstats(&units[liftn]);
		liftres = 1;
		goto out;
	case 0x2b:
	case 0x2c:
	case 0x2d:
	case 0x2e:
	case 0x2f:
	case 0x30:
		if (targetx == 999) {
			liftres = 0;
			goto out;
		}
		if (ask && asksteal(0)) {
			liftres = 0;
			goto out;
		}
		liftn = panels[targetkind - 0x28].id;
		takeout(liftn, targetx);
		if (liftn < 5) {
			calcstats(units);
			if (classof(targetitem & 0xfff) == 10)
				sackout(&targetitem);
		}
		liftres = 1;
		goto out;
	case 1:
		if (targetx == playerx && targety == playery)
			liftres = 0;
		else if (ask && asksteal(0))
			liftres = 0;
		else {
			liftn = targety * 150 + targetx;
			popitem(liftn, 0);
			updatecell(liftn);
			if (islittorch(targetitem))
				listtorches();
			liftres = 1;
			if (iswallns(map[liftn]) || iswallew(map[liftn]))
				targetitem = baselook(targetitem);
			if (istorch(targetitem))
				clamptorch(&targetitem);
		}
		goto out;
	case 0xda:
		if (targetx == 999 || tradeoffer[targetx] == 0) {
			liftres = 0;
			goto out;
		}
		tradeoffer[targetx] = 0;
		liftres = 1;
		goto out;
	}
	liftres = 0;
out:
	return liftres;
}

void
dropitem(int pos, unsigned long item)
{
	if (item & 0xfff) {
		dropw = map[pos];
		if (iswallns(dropw)) {
			dropw = eastlook(item);
			if (dropw)
				item = dropw;
		} else if (iswallew(dropw)) {
			dropw = southlook(item);
			if (dropw)
				item = dropw;
		} else
			item = randlook(item);
		dropw = topitem(pos);
		if (!dropw || !canstack(mapitems[dropw], item) || !addcoins(&mapitems[dropw], item))
			additem(pos, item);
	}
	updatecell(pos);
	if (islittorch(item))
		listtorches();
}

int
placeitem(void)
{
	switch (targetkind) {
	case 1:
		if (classof(targetitem & 0xfff) == 0x66)
			lefttime = 0;
		dropitem(targety * 150 + targetx, targetitem);
		return 1;
	case 0x2b:
	case 0x2c:
	case 0x2d:
	case 0x2e:
	case 0x2f:
	case 0x30:
		targetitem = randlook(targetitem);
		placek = panels[targetkind - 0x28].id;
		if (targetx == 0xff && targety == 0xff
		|| hoverx < panels[targetkind - 0x28].x || hoverx >= panels[targetkind - 0x28].x + panels[targetkind - 0x28].w
		|| hovery < panels[targetkind - 0x28].y || hovery >= panels[targetkind - 0x28].y + panels[targetkind - 0x28].h) {
			placex = 0xff;
			placey = 0xff;
		} else {
			placex = hoverpx > 0x10 ? hoverpx - 0x10 : 0;
			placey = hoverpy > 0x10 ? hoverpy - 0x10 : 0;
		}
		placepos = stowspot(placek, placex, placey, targetitem & 0xfff);
		if (stowat(placek, placepos, targetitem)) {
			if (placek < 5) {
				if (classof(targetitem & 0xfff) == 10)
					sackin(&targetitem);
				calcstats(units);
			}
			return 1;
		}
		denymsg(strs[545]);
		return 0;
	case 0x29:
	case 0x2a:
		targetitem = baselook(targetitem);
		placek = panels[targetkind - 0x28].id;
		if (targetx == 0x28) {
			if (islittorch(targetitem))
				setlit(&targetitem, 0);
			units[placek].items[0] = targetitem;
		} else if (targetx == 0x29) {
			if (islittorch(targetitem))
				setlit(&targetitem, 0);
			units[placek].items[1] = targetitem;
		} else if (targetx == 0x2a) {
			if (islittorch(targetitem))
				setlit(&targetitem, 0);
			units[placek].items[2] = targetitem;
		} else if (puton(placek, targetitem) == 999)
			return 0;
		calcstats(&units[placek]);
		return 1;
	case 0xda:
		targetitem = baselook(targetitem);
		tradeoffer[targetx] = targetitem;
		return 1;
	}
	return 0;
}

int
busyuntil(unsigned long t, short force)
{
	busyres = 1;
	if (playerout() || playmode != 1 && force)
		return 0;
	unitsrun = 1;
	busysave = busyturns;
	busyturns = 0xffff;
	tracesite = 1;
	while (t > gametime) {
		units[0].pose = 9;
		gametick();
		if (playerout() || playmode != 1 && force) {
			busyres = 0;
			break;
		}
	}
	busyturns = busysave;
	resetinput();
	unitsrun = 0;
	return busyres;
}

void
setcombat(short n)
{
	bloodtill = 0;
	units[0].pose = 0;
	steptick = 0;
	notedunit = 0xffff;
	askedonce = 0;
	game.talkendtime = gametime;
	askedunit = 0xff;
	plotcombat(n);
	if (!n) {
		playmode = 1;
		setunitmode(units, 1);
		if (units[0].health > 0) {
			stopmusic();
			if (combatkind == 3)
				music(4, 0);
			else if (combatkind == 2)
				music(10, 0);
			else if (combatkind)
				music(5, 0);
			if (issword(units[0].hand))
				queuesound(11);
			else
				queuesound(13);
			playqueue();
		}
		combatcause = 0;
		combatkind = 0;
		memset(game.foes, 0, 30);
		game.wodenblows = 0;
	} else {
		snapskills(2);
		playmode = 3;
		game.fightinvokes = 0;
		setunitmode(units, 20);
		resetanims();
		resetpose(units, 2);
		if (units[0].health > 0) {
			music(3, 0);
			if (issword(units[0].hand))
				queuesound(10);
			else
				queuesound(12);
			playqueue();
			if (game.combatbless) {
				if (game.combatbless > 0)
					game.combatbless--;
				if (units[0].cursed < 0xfa)
					enchant(0, -1, 0, units, 2, 0xff);
			}
		}
		for (combati = 0; combati < nunits; combati++)
			units[combati].pose = 0;
		actionturn = 0;
	}
	markunits();
	mousemoved = 1;
	redrawcursor();
	readinput();
	if (!sparending && !sparrer()) {
		if (!talkactive && !lastspeech && units[0].health > 0) {
			if (playmode == 3)
				msgf(15, strs[312], inautocombat ? strs[315] : empty, strs[247]);
			if (playmode == 1)
				skillmsg();
		} else if (units[0].health > 0 && (talkactive || lastspeech) && playmode == 1)
			skilldue = 1;
	}
	plotcheck();
}

void
finishstep(short n)
{
	if (trademode || playmode != 1) {
		syncplayer();
		readinput();
		units[0].pose = 0;
		steptick = 0;
		stepframe = 0;
		return;
	}
	tracesite = 60;
	waitforup = 1;
	if (units[0].pose == 1)
		while (steptick != 0)
			gametick();
	steptick = 0;
	units[0].pose = 0;
	if (n) {
		playmode = 3;
		steptick = stepframe * 4;
	}
	while (stepframe != 0)
		gametick();
	if (n)
		playmode = 1;
	waitforup = 0;
	units[0].pose = 0;
	steptick = 0;
}

void
togglecombat(short n)
{
	if (toggling || units[0].health <= 0 || unconscious())
		return;
	toggling = 1;
	finishstep(1);
	setcombat(playmode == 1 ? 1 : 0);
	stepframe = 0;
	if (!talkactive) {
		if (n)
			drawto2 = 1;
		drawscreen();
		drawto2 = 0;
	}
	if (playmode != 1) {
		skiptick = 1;
		if (!talkactive && maploaded && autolevel >= 3)
			autosave();
	}
	toggling = 0;
}

short
autocombat(void)
{
	if (inautocombat || units[0].health <= 0 || lostcontrol || trademode)
		return 0;
	if (knockedout)
		return 0;
	inautocombat = 1;
	autofoe = nearfoe(0);
	foeunit = autofoe;
	if (autofoe && combatkind == 0)
		combatkind = 1;
	if (playmode != 3) {
		if (autofoe) {
			syncplayer();
			turnto(units, units[autofoe].x, units[autofoe].y);
			setcurregion();
			closepanels();
			lookaround();
			lookdone();
			combatcause = 2;
			togglecombat(0);
			waitrelease(0);
			inautocombat = 0;
			return 1;
		}
	} else if (autofoe == 0 && combatkind && cmbautooff == 2 && foedist() > 17 && !nearfoe(1)) {
		togglecombat(0);
		waitrelease(0);
		inautocombat = 0;
		return 1;
	}
	inautocombat = 0;
	return 0;
}

short
disturbed(void)
{
	foeunit = nearfoe(0);
	if (!foeunit)
		return 0;
	units[0].pose = 0;
	playmode = 3;
	steptick = stepframe * 4;
	return 1;
}

short
showlabel(void)
{
	if (!lbltext[0])
		return 0;
	showtip(lblx, lbly, lbltext);
	lbltext[0] = 0;
	return 1;
}

short
useinfo(void)
{
	return 1;
}

void
showinfo(unsigned char show)
{
	if (infopanel != 0xff) {
		closepanel(infopanel);
		infopanel = 0xff;
		if (show == 0) {
			lbltext[0] = 0;
			cleartip();
			drawscreen();
		}
	}
	if (show == 0 || targetitem == 0 || unconscious())
		return;
	infocls = classof(targetitem & 0xfff);
	anchortip(targetkind, hoverx, hovery);
	addscore(1);
	if (infocls == 0x14) {
		if (isanimal(&units[targetidx]))
			goto text;
	} else if (infocls == 0x15) {
		calcload(infounit = targetitem >> 16);
		if (infounit == 0 || units[infounit].load == 0) {
			msgf(0x63, strs[544]);
			goto draw;
		}
	} else if ((infocls == 1 || infocls == 4) && targetitem & 0x8000000)
		;
	else if (infocls != 10 && itemweight(targetitem) >= 65000) {
	text:
		showlabel();
	draw:
		drawscreen();
		return;
	}
	openpanel(12);
	describe(targetitem, targetowner);
	movepanel(12, hoverx - panels[12].w / 2, hovery - panels[12].h / 2);
	panels[12].item = targetitem;
	infopanel = 12;
	cleartexts();
	hidecursor();
	mousemoved = 1;
	hoversound();
	drawscreen();
}

short
otherexit(unsigned long pos)
{
	otherpos = playercell - 1;
	if (otherpos != pos && (map[otherpos] & 0x4000) != 0 && (mapflags[otherpos] & 4) == 0)
		return 1;
	otherpos = playercell + 1;
	if (otherpos != pos && (map[otherpos] & 0x4000) != 0 && (mapflags[otherpos] & 4) == 0)
		return 1;
	otherpos = playercell - 150;
	if (otherpos != pos && (map[otherpos] & 0x4000) != 0 && (mapflags[otherpos] & 4) == 0)
		return 1;
	otherpos = playercell + 150;
	if (otherpos != pos && (map[otherpos] & 0x4000) != 0 && (mapflags[otherpos] & 4) == 0)
		return 1;
	return 0;
}

int
canstep(int dx, int dy)
{
	int c;

	trycount = 0;
	tryalt = 0;
	if (!dx && !dy) {
		readinput();
		trywt[0] = ptrdistx;
		dx = ptrdx;
		trywt[1] = ptrdisty;
		dy = ptrdy;
		tryx = playerx + dx;
		trycount = 0;
		tryy = playery + dy;
		tryalt = 0;
		if (tryx <= 5 || tryx >= 0x90 || tryy <= 4 || tryy >= 0x91)
			return 0;
		goto again;
	}
	trywt[0] = dx ? 100 : 0;
	trywt[1] = dy ? 100 : 0;
	tryx = playerx + dx;
	trycount = 0;
	tryy = playery + dy;
	tryalt = 1;
	if (tryx <= 5 || tryx >= 0x90 || tryy <= 4 || tryy >= 0x91)
		return 0;
loop:
	trycell = tryy * 150 + tryx;
	if ((map[trycell] & 0x4000 || mapflags[trycell] & 1 && (mapflags[trycell] & 0x40) == 0)
	    && canpass(playerx, playery, tryx, tryy)
	    && !(playmode != 3 && mapflags[playercell] & 0x40 && mapflags[trycell] & 0x40 && otherexit(trycell))
	    && (!(mapflags[trycell] & 4) || mapflags[trycell] & 8)) {
		if (dx == 0 || dy == 0) {
		ok:
			return 1;
		}
		goto diag;
	}
fail:
	if (dx == 0 || dy == 0) {
		if (tryalt)
			return 0;
		tryalt = 1;
		if (hoverx < 0x140) {
			dx = -1;
			trywt[0] = 0x13f - hoverx;
		} else {
			dx = 1;
			trywt[0] = hoverx - 0x140;
		}
		trywt[0] *= 2;
		trywt[0] /= 3;
		if (hovery < 0xf0) {
			trywt[1] = 0xf0 - hovery;
			dy = -1;
		} else {
			trywt[1] = hovery - 0xef;
			dy = 1;
		}
	again:
		tryx = playerx + dx;
		tryy = playery + dy;
		trycount = 0;
		goto loop;
	}
	readinput();
	c = ++trycount;
	if (c == 1) {
		if (trywt[0] > trywt[1]) {
			tryx = playerx + dx;
			tryy = playery;
		} else {
			tryy = playery + dy;
			tryx = playerx;
		}
	} else if (c == 2) {
		if (trywt[0] > trywt[1]) {
			tryy = playery + dy;
			tryx = playerx;
		} else {
			tryx = playerx + dx;
			tryy = playery;
		}
	} else
		return 0;
	goto loop;
diag:
	if (diagfree(playerx, playery, dx, dy))
		return 1;
	if (trywt[0] > trywt[1]) {
		tryx = playerx + dx;
		tryy = playery;
	} else {
		tryx = playerx;
		tryy = playery + dy;
	}
	trycell = tryy * 150 + tryx;
	if ((!((map[trycell] & 0x4000) == 0) || mapflags[trycell] & 1 && (mapflags[trycell] & 0x40) == 0)
	    && !(mapflags[trycell] & 4))
		return 1;
	if (trywt[0] > trywt[1]) {
		tryx = playerx;
		tryy = playery + dy;
		trycell = tryy * 150 + tryx;
	} else {
		tryx = playerx + dx;
		tryy = playery;
		trycell = tryy * 150 + tryx;
	}
	if ((map[trycell] & 0x4000) == 0 && (!(mapflags[trycell] & 1) || mapflags[trycell] & 0x40))
		return 0;
	if (mapflags[trycell] & 4)
		return 0;
	if (!canpass(playerx, playery, tryx, tryy))
		goto fail;
	if (playmode == 3 || !(mapflags[playercell] & 0x40) || !(mapflags[trycell] & 0x40) || !otherexit(trycell))
		goto ok;
	return 0;
}

short
canstepkey(short k)
{
	if (k == 0)
		return 0;
	if (k & 2)
		return canstep(0, -1);
	if (k & 0x40)
		return canstep(0, 1);
	if (k & 8)
		return canstep(-1, 0);
	if (k & 0x10)
		return canstep(1, 0);
	if (k & 1)
		return canstep(-1, -1);
	if (k & 4)
		return canstep(1, -1);
	if (k & 0x20)
		return canstep(-1, 1);
	if (k & 0x80)
		return canstep(1, 1);
	return 0;
}

int
advance(int dx, int dy)
{
	advtry = 0;
	advalt = 0;
	if (dx || dy) {
		advwx = dx ? 100 : 0;
		advwy = dy ? 100 : 0;
		units[0].nextx = playerx + dx;
		heropace = 0;
		units[0].nexty = playery + dy;
		advtry = 0;
		advalt = 1;
		if (units[0].nextx <= 5 || units[0].nextx >= 0x90 || units[0].nexty <= 4 || units[0].nexty >= 0x91) {
		out:
			travelmap(0, curmap, curmap);
			return 0;
		}
	loop:
		advcell = units[0].nexty * 150 + units[0].nextx;
		if (mapflags[advcell] & 4) {
			advunit = unitat(units[0].nextx, units[0].nexty);
			if (advunit && advunit != 0xff)
				units[advunit].incoming = 0;
		}
		if ((map[advcell] & 0x4000 || mapflags[advcell] & 1 && (mapflags[advcell] & 0x40) == 0)
		    && canpass(playerx, playery, units[0].nextx, units[0].nexty)
		    && !(playmode != 3 && mapflags[playercell] & 0x40 && mapflags[advcell] & 0x40 && otherexit(advcell))
		    && (!(mapflags[advcell] & 4) || mapflags[advcell] & 8)) {
			if (dx && dy && !diagfree(playerx, playery, dx, dy))
				goto diag;
		move:
			opendoor(advcell, 0);
			plotdoor(curmap, advcell % 150, advcell / 150);
			units[0].dirx = units[0].nextx - playerx;
			units[0].diry = units[0].nexty - playery;
			playernext = units[0].nextx + units[0].nexty * 150;
			if (units[0].diry) {
				(&walkedsum)[0] += (signed char)units[0].diry * 0x5a;
				(&walkedsum)[2] += (signed char)units[0].diry;
			}
			if (units[0].dirx) {
				(&walkedsum)[0] += (signed char)units[0].dirx;
				(&walkedsum)[1] += (signed char)units[0].dirx;
			}
			notedkind = 0xff;
			units[0].pose = 1;
			return 1;
		}
	fail:
		if (dx == 0 || dy == 0) {
			if (advalt)
				goto stop;
			advalt = 1;
			if (hoverx < 0x140) {
				dx = -1;
				advwx = 0x13f - hoverx;
			} else {
				dx = 1;
				advwx = hoverx - 0x140;
			}
			advwx *= 2;
			advwx /= 3;
			if (hovery < 0xf0) {
				dy = -1;
				advwy = 0xf0 - hovery;
			} else {
				dy = 1;
				advwy = hovery - 0xef;
			}
		again:
			units[0].nextx = playerx + dx;
			units[0].nexty = playery + dy;
			advtry = 0;
			goto loop;
		}
		readinput();
		if (++advtry == 1) {
			if (advwx > advwy) {
				units[0].nextx = playerx + dx;
				units[0].nexty = playery;
			} else {
				units[0].nextx = playerx;
				units[0].nexty = playery + dy;
			}
		} else if (advtry == 2) {
			if (advwx > advwy) {
				units[0].nextx = playerx;
				units[0].nexty = playery + dy;
			} else {
				units[0].nextx = playerx + dx;
				units[0].nexty = playery;
			}
		} else {
		stop:
			units[0].pose = 0;
			return 0;
		}
		goto loop;
	diag:
		if (advwx > advwy && canpass(playerx, playery, playerx + dx, playery)) {
			units[0].nextx = playerx + dx;
			units[0].nexty = playery;
		} else {
			if (!canpass(playerx, playery, playerx, playery + dy))
				goto move;
			units[0].nextx = playerx;
			units[0].nexty = playery + dy;
		}
		advcell = units[0].nexty * 150 + units[0].nextx;
		if (!((map[advcell] & 0x4000) == 0) || mapflags[advcell] & 1 && (mapflags[advcell] & 0x40) == 0)
			if (!(mapflags[advcell] & 4))
				goto move;
		if (advwx > advwy) {
			units[0].nextx = playerx;
			units[0].nexty = playery + dy;
		} else {
			units[0].nextx = playerx + dx;
			units[0].nexty = playery;
		}
		advcell = units[0].nexty * 150 + units[0].nextx;
		if ((map[advcell] & 0x4000) == 0 && (!(mapflags[advcell] & 1) || mapflags[advcell] & 0x40))
			goto stop;
		if (mapflags[advcell] & 4)
			goto stop;
		if (!canpass(playerx, playery, units[0].nextx, units[0].nexty))
			goto fail;
		if (playmode == 3 || !(mapflags[playercell] & 0x40) || !(mapflags[advcell] & 0x40) || !otherexit(advcell))
			goto move;
		goto stop;
	} else {
		readinput();
		dx = ptrdx;
		dy = ptrdy;
		advwx = ptrdistx;
		advwy = ptrdisty;
		heropace = ptrpace;
		advtry = 0;
		advalt = 0;
		units[0].nextx = playerx + dx;
		units[0].nexty = playery + dy;
		if (units[0].nextx <= 5 || units[0].nextx >= 0x90 || units[0].nexty <= 4 || units[0].nexty >= 0x91)
			goto out;
		goto again;
	}
}

void
enterspot(int n)
{
	if (spots[n].song && !(spots[n].song == spotsong && gameday == spotday)) {
		spotn = songbychar(spots[n].song);
		if (spotn) {
			music(spotn, 0);
			spotsong = spots[n].song;
			spotday = gameday;
		}
	}
	if (!spots[n].tomap)
		return;
	if (spots[n].hours && woulddie(spots[n].hours * hourticks)) {
		drawscreen();
		return;
	}
	fademed(0);
	travel(spots[n].tomap, spots[n].x, spots[n].y, spots[n].hours * hourticks, 1, 1, 0, 1);
	spotn = findclass(playery * 150 + playerx, 0x69);
	if (spotn) {
		plottreasure(curmap, playerx, playery);
		takeitem(playery * 150 + playerx, spotn);
	}
	if (!palblack)
		fademed(0);
	leavemenu(1);
}

int
usekey(unsigned long key)
{
	if (!key) {
		key = targetitem;
		keyask = askmsg(strs[285]);
		keeplast();
		keyres = asktarget(1, 10, 0xff, 0xff, 0xff, 0, 0, 0, 0, 0);
		delmsgid(keyask);
		if (playerout() || playmode != 1)
			return 0;
		if (keyres == 0) {
			if (!nmsgs)
				denymsg(strs[273], itemname(key));
			return 0;
		}
	}
	if (classof(targetitem & 0xfff) == 10) {
		if (itempanel(targetitem) != 0xff) {
			denymsg(strs[293], itemname(targetitem));
			return 0;
		}
		keylock = &realms[idof(targetitem)].item;
		if (idof(*keylock) == 0)
			goto nokey;
	} else {
		keylock = &mapitems[targetidx];
		if (*keylock == 0)
			goto wrong;
		if (idof(*keylock) == 0) {
		nokey:
			denymsg(strs[275], itemname(targetitem));
			return 0;
		}
		if (*keylock & 0x10000000) {
			denymsg(strs[293], itemname(targetitem));
			return 0;
		}
	}
	if (idof(key) != idof(*keylock)) {
	wrong:
		failmsg(strs[292]);
		return 0;
	}
	if (*keylock & 0x8000000) {
		*keylock &= ~0x8000000;
		if (targetkind == 1)
			updatecell(targety * 150 + targetx);
		msgf(0x63, texts + 0x9dea, itemname(targetitem), strs[295]);
		queuesound(0x15);
		return 1;
	}
	if (wearof(*keylock) >= 7)
		denymsg(strs[1395]);
	else {
		*keylock |= 0x8000000;
		if (targetkind == 1)
			updatecell(targety * 150 + targetx);
		msgf(0x63, texts + 0x9dea, itemname(targetitem), strs[294]);
		queuesound(0x15);
	}
	return 1;
}

int
uselever(void)
{
	if (targetkind != 1)
		return 0;
	switch (pulllever(&mapitems[targetidx])) {
	case 0:
	case 2:
	case 3:
		failmsg(strs[305]);
		break;
	case 1:
		queuesound(0x16);
		plotdoor(curmap, targetx, targety);
		break;
	}
	return 1;
}

short
issign(unsigned long item)
{
	item &= 0xfff;
	if (item == 0x1f5 || item == 0x1f6 || item == 0x211 || item == 0x212)
		return 1;
	return 0;
}

short
readitem(short show)
{
	if (!show && !issign(targetitem))
		return 0;
	if (lastlight <= 5) {
		if (show) {
			denymsg(strs[1451]);
			return 1;
		}
		return 0;
	}
	readno = (targetitem >> 16) + 1691;
	if (readno >= nstrs)
		readno = nstrs - 1;
	readtxt = strs[readno];
	expandstr(packbuf, readtxt);
	readtxt = packbuf;
	if (packbuf[0] == '!' && packbuf[1] == '!') {
		if (packbuf[2] == 'U')
			upperstr(readtxt);
		readtxt += 4;
	}
	for (readscan = readtxt; *readscan; readscan++)
		if (*readscan == 13) {
			if (!show)
				return 0;
			wrap(readtxt, 0x1ac);
			textwin(readtxt, 0, 0xfe, 1, 0, &menu1, 0, 0);
			return 1;
		}
	msgf(0x63, readtxt);
	return 1;
}

void
placeplayer(short face, int x, int y)
{
	setunitmode(units, 0);
	if (face == 1) {
		placedir = x - playerx;
		if (placedir < -1)
			placedir = -1;
		else if (placedir > 1)
			placedir = 1;
		units[0].dirx = placedir;
		placedir = y - playery;
		if (placedir < -1)
			placedir = -1;
		else if (placedir > 1)
			placedir = 1;
		units[0].diry = placedir;
	}
	playery = y;
	playerx = x;
	playercell = playerx + playery * 150;
	units[0].x = playerx;
	units[0].y = playery;
	trackregion();
	markunits();
	lookaround();
}

short
nodeat(int x, int y)
{
	if (talkstate == 1) {
		undertab = choices;
		undern = 4;
	} else {
		undertab = nodes;
		undern = nnodes;
	}
	for (underi = 0; underi < undern; underi++) {
		if (!undertab[underi].shown)
			continue;
		underw = undertab[underi].width;
		if (!underw)
			continue;
		underxy = talky + undertab[underi].y;
		if (y < underxy - 3 || y >= underxy + 23)
			continue;
		underxy = undertab[underi].x;
		if (x >= underxy - 6 && x < underxy + underw + 6)
			return underi;
	}
	return 0;
}

short
byenode(void)
{
	if (talkstate == 1)
		return 0;
	byetab = nodes;
	byen = nnodes;
	byefound = 0;
	for (byei = 0; byei < byen; byei++) {
		if (!byetab[byei].shown || !byetab[byei].width)
			continue;
		for (byep = byetab[byei].start; *byep && *byep != 13; byep++)
			;
		if (!*byep)
			continue;
		for (; byep < byetab[byei].end && *byep != '#'; byep++) {
			if (*byep == '"') {
				for (byeq = byep + 1; *byeq != 13 && *byeq != '{'; byeq++)
					if (*byeq == 'j' && byeq[1] == 'a' && !isletter(byeq[2])) {
						for (byeq += 3; *byeq != '}' && byeq < byetab[byei].end; byeq++)
							;
						if (*byeq == '}' && byeq[1] == '-' && byeq[2] == '{')
							for (byeq += 3; *byeq != '}' && byeq < byetab[byei].end; byeq++)
								;
						byep = byeq;
						break;
					}
			}
			if (*byep == 0x5c) {
				for (; *byep != 13 && *byep && byep < byetab[byei].end - 2; byep++)
					if (*byep == 'e' && byep[1] == 'n' && byep[2] == 'd') {
						if (byefound)
							goto byname;
						byefound = byei;
						break;
					}
				break;
			}
		}
	}
	if (byefound)
		return byefound;
byname:
	for (byei = 0; byei < byen; byei++)
		if (byetab[byei].shown && byetab[byei].width
		&& wordeqi(byetab[byei].text, pick(T(0x6bb9), T(0x6bc5), T(0x6bc9), T(0x6bca), T(0x6bcb), T(0x6bd7))))
			return byei;
	return 0;
}

short
nodecount(void)
{
	if (talkstate == 1)
		return 2;
	for (nodecnti = 0, nodecnt = 0; nodecnti < nnodes; nodecnti++)
		if (nodes[nodecnti].shown && nodes[nodecnti].width)
			nodecnt++;
	return nodecnt;
}

/* the dialogue with unit n */
void
talkto(unsigned short n)
{
	int old;
	int t;

	dropheld();
	if (units[n].scene && maploaded && autolevel >= 2) {
		if (curmap == 1 || curmap == 5) {
			if (units[n].traits & 0x20)
				autosave();
		} else
			autosave();
	}
	skilldue = 0;
	talkslide = 0;
	talkprev = 0;
	talkunit = 0;
	talknext = 0;
	memcpy(talkknown, game.known, 30);
again:
	talkchoice = 0;
	talkbegun = 0;
	finishstep(0);
	curunit = n;
	turnto(&units[n], playerx, playery);
	syncplayer();
	setcurregion();
	lookaround();
	lookdone();
	if (chkpending || startpending || licseal || licpending) {
	nodlg:
		tmpbuf[0] = 0;
	simple:
		screenpos(&units[n]);
		anchortip(1, screenposx, screenposy);
		xorstr(tmpbuf, pick(T(0x719f), T(0x71dc), T(0x7208), T(0x7209), T(0x720a), T(0x723d)));
		showtip(tipanchorx, tipanchory, tmpbuf);
		talkplain = 1;
	} else {
	loop:
		if (!talkok || wrongcopy)
			goto nodlg;
		talkplain = 0;
		curscene = units[n].scene;
		if (units[n].scene <= 0x50 && !units[n].face) {
			screenpos(&units[n]);
			anchortip(1, screenposx, screenposy);
			showtip(tipanchorx, tipanchory, strs[0x269 + units[n].scene]);
			talkplain = 1;
			goto shown;
		}
		if (!licenseok || wrongcopy)
			goto simple;
		if (!talkbegun) {
			if (scriptfile[0])
				loadscript(scriptfile);
			starttalk(n);
			entermenu();
			drawcursor();
		}
		if (units[n].scene <= 0x50)
			wsprintf(speech, strs[0x269 + units[n].scene]);
		else if (!parsescene())
			talkplain = 1;
		expandnodes();
		if (!talkbegun) {
			train(0x18, 5);
			talkface(n);
			talkbegun = 1;
		}
		cleartip();
		closepanels();
		entermenu();
		if (talkslide) {
			talkslide = 0;
			talkactive = 0;
			hidecursor();
			noflip = 1;
			drawscreen();
			noflip = 0;
			backto2();
			drawface(0x20, 0x1e, talkpic, talkprev);
			flip();
			s2toback();
			drawface(0x20, 0x1e, curface, curunit);
			drawface(0x20, 0x1e, talkpic, talkprev);
			dissolvet();
			for (talkstep = 0; talkstep <= 0x6c; talkstep += 4) {
				s2tobackrows(0x1e, 0x1e, 0x8c);
				drawface(talkstep + 0x20, 0x1e, curface, curunit);
				drawface(0x20, 0x1e, talkpic, talkprev);
				flip();
				waitinput(1);
			}
			waitinput(10);
			for (talkstep = 0x6c; talkstep >= 0; talkstep -= 4) {
				s2tobackrows(0x1e, 0x1e, 0x8c);
				drawface(0x20 - (0x6c - talkstep), 0x1e, talkpic, talkprev);
				drawface(talkstep + 0x20, 0x1e, curface, curunit);
				flip();
				waitinput(1);
			}
			s2toback();
			drawface(0x20, 0x1e, curface, curunit);
			flip();
			drawcursor();
		}
		talkactive = 1;
		talkstate = 0;
		rightclick = 0;
		if (units[n].scene <= 0x50 || talkplain)
			goto end;
	next:
		talkcode = shownode();
		playqueue();
		if (talkcode == 0xff2) {
			savevars();
			if (!talknext)
				goto loop;
			units[curunit].talkflags |= 8;
			talkprev = curunit;
			memcpy(talkpic, curblink ? curface2 : curface, 0x32a0);
			endtalk();
			n = talknext;
			talkbegun = 0;
			talknext = 0;
			talkslide = 1;
			goto again;
		}
		if (talkcode == 0xfff) {
		end:
			endtalk();
			talkactive = 0;
			lastspeech = 1;
			goto shown;
		}
		if (talkcode == 0xffe || talkcode == 0xff4 || talkcode == 0xff6 || talkcode == 0xff1) {
		close:
			endtalk();
			talkactive = 0;
			lastspeech = 0;
			if (talkcode == 0xff1) {
				setunitmode(&units[curunit], 15);
				old = busyturns;
				busyturns = 0xffff;
				unitsrun = 1;
				units[curunit].modec = sendposx;
				units[curunit].moded = sendposy;
				tracesite = 0x5a;
				talkwait = gametime;
				hidecursor();
				do
					gametick();
				while (displayed(&units[curunit]) && seesunit(&units[curunit])
				&& talkwait + 50 > gametime && !playerout());
				unitsrun = 0;
				busyturns = old;
				mousemoved = 1;
				readinput();
				drawcursor();
			}
			if (talkcode == 0xffe || talkcode == 0xff1)
				killunit(&units[curunit], 0, 0, 0xfe, plotheir(curmap, curunit));
			goto talked;
		}
		if (talkcode == 0xffd) {
			endtalk();
			killunit(&units[curunit], talkcode == 0xffd ? 1 : 0, 0, 0xff, plotheir(curmap, curunit));
			talkactive = 0;
			lastspeech = 1;
			goto talked;
		}
		if (talkcode == 0xff5) {
			fadeslow(0);
			units[0].health = -30000;
			goto close;
		}
		if (nodecount() == 0)
			goto end;
		drawscreen();
		if (!quitting && esckey != 0xff && !ordercmd)
			fademed(1);
		tracesite = 4;
		while (mouseright)
			if (playmode == 1 && units[0].health > 0)
				gametick();
			else
				readinput();
		rightclick = 0;
		do {
			tracesite = 5;
			while (mouseleft)
				if (playmode == 1 && units[0].health > 0)
					gametick();
				else {
					readinput();
					if (needredraw(0xfd))
						drawscreen();
				}
			leftclick = 0;
			while (!mouseleft && !leftclick) {
				if (quitting) {
					endtalk();
					goto shown;
				}
				if (mouseright || rightclick || esckey) {
					talknode = byenode();
					rightclick = 0;
					if (talknode)
						goto chosen;
					waitforup = 1;
					tracesite = 6;
					do
						if (playmode == 1 && units[0].health > 0)
							gametick();
						else {
							readinput();
							if (needredraw(0xfd))
								drawscreen();
						}
					while (mouseright);
					waitforup = 0;
					if (!cheat)
						esckey = 0;
				} else {
					talkchoice = nodeat(hoverx, hovery);
					if (talkchoice && talkchoice != litchoice) {
						hoversound();
						drawscreen();
					}
					if (playmode == 1 && units[0].health > 0)
						gametick();
					else {
						readinput();
						if (needredraw(0xfd))
							drawscreen();
					}
				}
				edgemenus(1);
				menumode = 1;
				talkactive = 1;
			}
			leftclick = 0;
			talknode = nodeat(hoverx, hovery);
		chosen:
			;
		} while (!talknode);
		learntalk();
		if (talkstate == 1) {
			talkanswer = talknode - 1;
			clicksound();
		} else {
			findbrace(nodes[talknode].start);
			nodes[talknode].flags |= 1;
			clicksound();
		}
		goto next;
	}
shown:
	markunits();
	if (quitting)
		goto leave;
talked:
	talkstate = 2;
	setcursor1(3, 3);
	talkmx = hoverx;
	talkmy = hovery;
	markunits();
	if (cmbautooff)
		autocombat();
	leftclick = 0;
	rightclick = 0;
	dblclick = 0;
	lastscan = 0;
	if (esckey != 0xff)
		esckey = 0;
	talkactive = 0;
	savevars();
	units[0].pose = 0;
	menumode = 0;
	mousemoved = 1;
	waitforup = 1;
	drawscreen();
	waitforup = 0;
	if (!(talkcode == 0xff6 || talkcode == 0xff5 || talkcode == 0xff4 && gobattle
	|| quitting || esckey == 0xff || ordercmd))
		fademed(1);
	talki = 0;
	talknew = 0;
	for (; talki < 30; talki++)
		if (talkknown[talki] != game.known[talki]) {
			talknew = 1;
			break;
		}
	if (talknew && getbit(game.flags, 0x44c)) {
		waitinput(500);
		hidecursor();
		primto2();
		s2toback();
		memcpy(tmpbuf, game.known, 30);
		memcpy(game.known, talkknown, 30);
		drawactions();
		flip();
		s2toback();
		memcpy(game.known, tmpbuf, 30);
		drawactions();
		waitinput(1000);
		dissolvet();
		waitinput(1500);
		s2toprim();
		drawcursor();
	}
	if (talkcode == 0xff6 || talkcode == 0xff5 || talkcode == 0xff4)
		goto leave;
	if (!quitting) {
		waitforup = 1;
		tracesite = 7;
		pollinput();
		while ((mouseleft || mouseright || keycode) && !playerout())
			if (playmode == 1 && units[0].health > 0)
				gametick();
			else {
				readinput();
				if (needredraw(0xfd))
					drawscreen();
			}
		waitforup = 0;
		leftclick = 0;
	}
	if (speech[0] == 0 || quitting)
		goto leave;
	talktop = 0;
	talkbottom = 0;
	talkleft = 0;
	talkright = 0;
	if (hovery <= 20)
		talktop = 1;
	if (hovery >= 452)
		talkbottom = 1;
	if (hoverx <= 28)
		talkleft = 1;
	if (hoverx >= 610)
		talkright = 1;
	if (isanimal(&units[n]))
		animalcry(&units[n]);
	tracesite = 8;
	talkwait = 0;
	talkp = speech;
	while (*talkp) {
		if (*talkp == 13)
			talkwait++;
		talkp++;
	}
	talkwait *= 3;
	talkstep = stepframe;
	talkwait += 7;
	talkplain = 0;
	pollinput();
	for (;;) {
		if (mouseleft || mouseright || leftclick || rightclick || keycode || esckey
		|| keys[VK_CONTROL] & 0x80 || playerout()
		|| abs(hoverx - talkmx) >= 90 || abs(hovery - talkmy) >= 90
		|| talkcode != 0xffd && talkcode != 0xffe && talkcode != 0xff5
		&& !cansee(units[n].x, units[n].y))
			break;
		if (hovery > 20)
			talktop = 0;
		if (hovery < 452)
			statshown = 0;
		if (hoverx > 28)
			talkleft = 0;
		if (hoverx < 610)
			talkright = 0;
		if (hovery <= 20 && !talktop || hovery >= 456 && !talkbottom
		|| hoverx <= 16 && !talkleft || hoverx >= 614 && !talkright)
			break;
		if (playmode == 1 && units[0].health > 0)
			gametick();
		else {
			readinput();
			if (needredraw(0xfd))
				drawscreen();
		}
		t = stepframe;
		if (talkstep != t) {
			talkplain++;
			talkstep = t;
			if (talkplain / 6 > talkwait)
				break;
		}
	}
	if (skilldue)
		skillmsg();
leave:
	talkactive = 0;
	lastspeech = 0;
	units[curunit].talkflags |= 8;
	talkunit = curunit;
	curunit = 0;
	game.talkendtime = gametime;
	markunits();
	if (cmbautooff)
		autocombat();
	if (!quitting) {
		if (talkcode == 0xff4) {
			if (gobattle == 1)
				tobattle();
		} else if (talkcode == 0xff6) {
			if (telemapno == 0xff)
				travel(zones[8].id, zones[8].inx, zones[8].iny,
					traveltime(travelhours(mapzone(curmap), 8)), 1, 1, 0xff, 1);
			else if (telemapno == 0xfe)
				travel(zones[17].id, zones[17].inx, zones[17].iny,
					traveltime(travelhours(mapzone(curmap), 17)), 1, 0, 0, 0);
			else if (telemapno == 0xfd || telemapno == 0xfc) {
				adalmarwait();
				if (telemapno == 0xfd)
					staylen = travelhours(16, 2) * 2 / 24;
				else
					staylen = travelhours(16, 2) * 3 / 24;
				curunit = talkunit;
				staydays(4);
				curunit = 0;
				adalmarback();
			} else if (telemapno == 0xfb) {
				teudogarwait();
				staylen = 7;
				if (curmap == 15)
					curunit = 2;
				else
					curunit = talkunit;
				staydays(4);
				curunit = 0;
				teudogarback();
			} else if (telemapno == 0xfa) {
				motherwait();
				staylen = travelhours(18, 1) * 2 / 24;
				curunit = talkunit;
				staydays(4);
				curunit = 0;
				motherback();
			} else
				travel(telemapno, teleposx, teleposy, telehours * hourticks,
					telemapno != curmap, telehours || telemapno != curmap ? 1 : 0, 0, 1);
		} else if (talkcode != 0xff5)
			drawscreen();
		if (units[0].mode == 0x5a) {
			if (playmode == 1)
				busyuntil(gametime + 3, 1);
			placeplayer(1, playerx, playery);
			drawscreen();
		}
	}
	plotcheck();
	talkchoice = 0;
	if (ordercmd == 2) {
		ordercmd = 0;
		aboutfull(0, 0);
	}
}

short
nearidle(short x, short y, short a, short b)
{
	short min, best, pass, i, d;

	best = 0xff;
	min = 300;
	for (pass = 0; pass <= 1; pass++) {
		for (i = 1; i < nunits; i++) {
			if (units[i].health <= 0 || isanimal(&units[i]) || units[i].sympathy <= 0)
				continue;
			if (!(!pass && regionat(units[i].x, units[i].y) == a || pass && provat(units[i].x, units[i].y) == b))
				continue;
			if (units[i].mode == 0 || units[i].mode == 2 && !pass || units[i].mode == 1 || units[i].mode == 3 || units[i].mode == 7
			|| units[i].mode == 15 && units[i].modee == 0xff) {
				d = celldist(x, y, units[i].x, units[i].y);
				if (d < min) {
					best = i;
					min = d;
				}
			}
		}
		if (best != 0xff || !b)
			break;
	}
	return best;
}

short
sendidle(short x, short y)
{
	idleunit = nearidle(x, y, regionat(x, y), provat(x, y));
	if (idleunit == 0xff)
		return 0;
	setunitmode(&units[idleunit], 15);
	units[idleunit].modec = x;
	units[idleunit].modee = 0xff;
	units[idleunit].moded = y;
	return idleunit;
}

short
togglefire(void)
{
	switch (targetkind) {
	case 1:
		firep = &mapitems[targetidx];
		break;
	case 0x29:
	case 0x2a:
		firep = &units[panels[targetkind - 0x28].id].offhand;
		if (*firep == targetitem)
			break;
	default:
		goto fail;
	}
	firelit = itemburns(*firep) ? 0 : 1;
	if (!setlit(firep, firelit))
		goto fail;
	if (firelit)
		queuesound(7);
	else
		queuesound(2);
	targetitem = *firep;
	if (targetkind == 1) {
		setsmoke(targetx - 1 + (targety - 1) * 150, firelit);
		updatecell(targety * 150 + targetx);
		if (firelit == 0)
			sendidle(targetx, targety);
	}
	dresshero();
	lookaround();
	calctorch();
	lookdone();
	drawscreen();
	return 1;
fail:
	denymsg(strs[272], itemname(targetitem));
	return 0;
}

void
setclock(short day, unsigned char hour)
{
	gameday = day;
	gamehour = hour;
	scavenge(1);
	decaycorpses();
	setdaylight();
	lookaround();
	lookdone();
	placeunits(gamehour);
	markunits();
}

void
afterskip(void)
{
	markunits();
	setcurregion();
	lookaround();
	clrboth();
	scavenge(1);
	decaycorpses();
	setdaylight();
	lastlight = 0xff;
	lookaround();
	calctorch();
	lookdone();
	setlevels();
	calcstats(units);
	listtorches();
	markunits();
	closeborder();
	clearturn();
	clearscenes();
}

int
ticksto(short hour)
{
	if (gamehour >= hour)
		tohours = hour + 24 - gamehour;
	else
		tohours = hour - gamehour;
	toticks = gametime / hourticks * hourticks;
	toticks += tohours * hourticks;
	toticks++;
	toticks -= gametime;
	return toticks;
}

short
ambushed(void)
{
	if (!units[0].asleep && units[0].mode != 0x5a)
		return 0;
	for (ambi = 1; ambi < nunits; ambi++) {
		if (units[ambi].health <= 0)
			continue;
		if (units[ambi].traits & 0x20 && (units[ambi].sympathy >= 4 || myservant(&units[ambi])))
			units[ambi].traits &= ~0x20;
		if (units[ambi].traits & 0x20 && inview(units[ambi].x, units[ambi].y)) {
			units[ambi].sympathy = 0;
			setunitmode(&units[ambi], 20);
			game.fate = 6;
			game.killer = ambi;
			killunit(units, 1, 2, ambi ? ambi : 0xff, 0);
			if (playmode != 1)
				setcombat(0);
			return ambi;
		}
	}
	return 0;
}

int
resttick(void)
{
	clearturn();
	if (gametime % hourticks == 0) {
		if (ambushed())
			return 1;
		nexthour();
	}
	rununits(1);
	moveunits(1);
	disturbed();
	if (playmode != 1)
		return 1;
	if (units[0].health <= 0)
		return 1;
	return 0;
}

unsigned long
passtime(unsigned long t, short ask, short b)
{
	if (!t)
		return 0;
	clearqueue();
	passmode = 1;
	fademed(0);
	cleartexts();
	starttimer2();
	scavenge(1);
	decaycorpses();
	units[0].pose = 0;
	steptick = 0;
	stepframe = 0;
	markunits();
	passdone = 0;
	if (ambushed())
		t = passdone;
	for (; t > passdone; gametime++, passdone++) {
		if (hourticks <= passdone)
			break;
		if (resttick()) {
			t = passdone;
			break;
		}
	}
	for (; passdone + 20 < t; gametime++, passdone++) {
		if (!(gametime & 7))
			for (passi = 0; passi < nunits; passi++)
				tickhealth(&units[passi]);
		if (gametime % hourticks == 0) {
			if (ambushed()) {
				t = passdone;
				break;
			}
			nexthour();
			markunits();
			placeunits(gamehour);
			markunits();
			disturbed();
			if (playmode != 1) {
				t = passdone;
				break;
			}
		}
		if (units[0].health <= 0) {
			t = passdone;
			break;
		}
	}
	plotcheck();
	sharecells();
	while (t > passdone && !resttick()) {
		gametime++;
		passdone++;
	}
	plotcheck();
	sharecells();
	agetorches(passdone, 1);
	fadefast(0);
	afterskip();
	passmode = 0;
	clearqueue();
	if (ask && playmode != 1) {
		killunit(units, 1, 2, foeunit ? foeunit : 0xff, 0);
		game.fate = 4;
		game.killer = foeunit;
		while (elapsed2() < 500)
			readinput();
	} else
		goto other;
done:
	clearturn();
	return passdone;
other:
	if (units[0].health <= 0) {
		while (elapsed2() < 500)
			readinput();
		goto done;
	}
	if (units[0].dexterity < 5 || units[0].strength < 5) {
		killunit(units, 1, 3, 0xff, 0);
		game.killer = 0;
		game.fate = 0;
		while (elapsed2() < 500)
			readinput();
		goto done;
	}
	if (playmode != 1) {
		while (elapsed2() < 500)
			readinput();
		if (playmode != 1) {
			playmode = 1;
			togglecombat(0);
		}
		msgf(0x63, strs[245], daytime(gamehour));
		drawscreen();
		if (b)
			fadefast(1);
	} else {
		while (elapsed2() < 1000)
			readinput();
		msgf(0x63, strs[244], daytime(gamehour));
		drawscreen();
		if (b)
			fademed(1);
	}
	clearturn();
	return passdone;
}

unsigned long
passalone(unsigned long t)
{
	if (!t)
		return 0;
	clearqueue();
	passmode = 2;
	units[0].pose = 0;
	steptick = 0;
	stepframe = 0;
	for (aloneticks = 0; aloneticks < t; aloneticks++, gametime++) {
		tickstatus(units);
		if ((gametime & 7) == 0)
			tickhealth(units);
		if (gametime % hourticks == 0)
			nexthour();
		if (units[0].health <= 0) {
			t = aloneticks;
			break;
		}
	}
	agetorches(aloneticks, 1);
	afterskip();
	passmode = 0;
	clearqueue();
	clearturn();
	if (units[0].health > 0 && units[0].dexterity >= 5 && units[0].strength >= 5) {
		msgf(0x63, strs[244], daytime(gamehour));
		return aloneticks;
	}
	return aloneticks;
}

unsigned long
catchup(unsigned long t)
{
	if (!t)
		return 0;
	clearqueue();
	passmode = 3;
	scavenge(1);
	decaycorpses();
	units[0].pose = 0;
	steptick = 0;
	stepframe = 0;
	markunits();
	placeunits(gamehour);
	markunits();
	awayx = playerx;
	units[0].x = 0xff;
	awayy = playery;
	playery = 0xff;
	playerx = 0xff;
	playercell = playerx + playery * 150;
	units[0].y = 0xff;
	awayhour = gamehour;
	awayday = gameday;
	awaytime = gametime;
	for (awayticks = 0; awayticks < t; awayticks++, gametime++) {
		for (awayi = 1; awayi < nunits; awayi++) {
			tickstatus(&units[awayi]);
			if (!(gametime & 7))
				tickhealth(&units[awayi]);
		}
		if (gametime % hourticks == 0) {
			nexthour();
			markunits();
			placeunits(gamehour);
			markunits();
		}
	}
	gametime = awaytime;
	gameday = awayday;
	gamehour = awayhour;
	playerx = awayx;
	playery = awayy;
	playercell = playerx + playery * 150;
	units[0].x = awayx;
	units[0].y = awayy;
	trackregion();
	agetorches(awayticks, 0);
	afterskip();
	passmode = 0;
	clearqueue();
	clearturn();
	return awayticks;
}

int
gosleep(short ask)
{
	bedkind = 0;
	sleepx = playerx;
	sleepy = playery;
	bedcls = classof(targetitem & 0xfff);
	if (bedcls == 0x19 || bedcls >= 0x78 && bedcls <= 0x7b) {
		if (targetkind == 1)
			bedkind = 1;
		else
			bedkind = 2;
	}
	if (woulddie(hourticks * 3)) {
		drawscreen();
		return 0;
	}
	if (game.tiredlevel == 0 && !(gamehour >= 12) && gamehour >= 6
	&& !(bedkind && notedkind == targetkind && notedposx == targetx && notedposy == targety
	&& noteditem == targetitem && notedtime + 10 > gametime)
	&& (lastfail != 2 || gametime - acttime >= 5)) {
		denymsg(strs[319]);
		return 0;
	}
	closepanels();
	if (bedkind == 1) {
		units[0].x = targetx;
		playerx = units[0].x;
		units[0].y = targety;
		playery = units[0].y;
		playercell = playerx + playery * 150;
		trackregion();
		markunits();
		lookaround();
		if (targetkind == 1 && (targetitem & 0xfff) == 0x19f)
			bedkind = 0;
	}
	setunitmode(units, 0x5a);
	if (bedkind == 1)
		units[0].modec = 0;
	else if (units[0].cloak || (targetitem & 0xfff) == 0x19f || inpack((short *)&sleepslot, (short *)&sleepslot, 0x19f, 1)
	|| wornslot(units, 0x19f, 1))
		units[0].modec = 2;
	else
		units[0].modec = 1;
	drawscreen();
	if (ask)
		playnow(0x19);
	else
		playnow(0x1a);
	sleepwait = gametime + 3;
	busyuntil(sleepwait, 1);
	if (playmode == 1 && !playerout()) {
		passtime(ticksto(7), 0, 1);
		sleepwait = gametime + 3;
		busyuntil(sleepwait, 1);
	}
	placeplayer(bedkind, sleepx, sleepy);
	drawscreen();
	return 1;
}

void
wield(int mode, int x, int y, unsigned long item)
{
	item &= ~0x1000;
	item = baselook(item);
	if (!isweapon(item)) {
		giveitem(0, item, 1);
		return;
	}
	if (isthrowing2(item)) {
		if (units[0].hand) {
			if (isthrowing2(units[0].hand)) {
			check:
				if (!packitem(units[0].hand)) {
					if (mode == 1)
						additem(y * 150 + x, units[0].hand);
					else
						additem(playery * 150 + playerx, units[0].hand);
					units[0].hand = 0;
					goto wear;
				}
			} else if (units[0].belt) {
				packordrop(units[0].belt);
				units[0].belt = 0;
				packordrop(units[0].hand);
				units[0].hand = 0;
				goto wear;
			} else {
				packordrop(units[0].belt);
				units[0].belt = units[0].hand;
				units[0].hand = 0;
				goto wear;
			}
			units[0].hand = 0;
		}
	} else {
		if (units[0].belt) {
			packordrop(units[0].belt);
			units[0].belt = 0;
		}
		if (units[0].hand) {
			if (isthrowing2(units[0].hand))
				goto check;
			packordrop(units[0].hand);
			units[0].hand = 0;
		}
	}
wear:
	units[0].hand = item;
	bloodtill = 0;
	queuesound(2);
	calcstats(units);
}

int
fillvessel(unsigned long *p, short k)
{
	*p = baselook(*p);
	if (k) {
	one:
		*p += 1;
	} else
		switch (classof(targetitem & 0xfff)) {
		case 0x82:
			goto one;
		case 0x83:
			*p += 2;
			break;
		case 0x84:
			*p += 3;
			break;
		case 0x85:
			*p += 4;
			break;
		case 0x86:
			*p += 5;
			break;
		default:
			denymsg(strs[273], itemname(*p));
			return 0;
		}
	queuesound(9);
	if (k == 0) {
		*p &= 0xffff;
		*p |= targetitem & 0xffff0000;
	} else if (k == 1)
		*p &= 0xffff;
	else if (k == 2) {
		*p &= 0xffff;
		*p |= 0x80000000;
	}
	return 1;
}

int
grind(void)
{
	if (classof(targetitem & 0xfff) != 0x3c || metals[gearof(targetitem)].material != 2) {
		wsprintf(printbuf, strs[1564], itemname(targetitem));
		denymsg(printbuf);
		return 0;
	}
	grindwear = wearof(targetitem);
	if (grindwear >= 4) {
		wsprintf(printbuf, strs[1563], itemname(targetitem));
		denymsg(printbuf);
		return 0;
	}
	if (grindwear == 0) {
		wsprintf(printbuf, strs[1565], itemname(targetitem));
		failmsg(printbuf);
		return 0;
	}
	setwear(&targetitem, grindwear - 1);
	msgf(0x63, strs[1566], itemname(targetitem));
	queuesound(10);
	return 1;
}

int
mixpotion(unsigned long key)
{
	if (!key) {
		key = targetitem;
		mixask = askmsg(strs[285]);
		keeplast();
		mixres = asktarget(0xff, 0xff, 0xff, 0xff, 0xff, 1, 0, 1, 1, 2);
		delmsgid(mixask);
		if (playerout() || playmode != 1)
			return 0;
		if (mixres == 0)
			goto fail;
		if (mixres == 2)
			return 2;
		if (asksteal(2))
			return 0;
	}
	mixcls = classof(targetitem & 0xfff);
	if (mixcls == 0x14 && targetidx == 0)
		return 2;
	if (mixcls == 0x37 || mixcls == 0x87 || mixcls == 0x88 || mixcls == 0x89
	|| mixcls == 0x8a || mixcls == 0x8b) {
		mixp = itemptr(targetkind, targetidx);
		if (mixp == 0)
			goto fail;
		key &= 0xffff0000;
		*mixp |= key;
		msgf(0x63, strs[357], itemname(*mixp));
		queuesound(2);
		return 1;
	}
fail:
	if (!nmsgs)
		denymsg(strs[273], itemname(key));
	return 0;
}

int
dig(short ask)
{
	digtrap = 0;
	if (targetkind != 1)
		return 0;
	if (abs(targetx - playerx) > 1)
		return 0;
	if (abs(targety - playery) > 1)
		return 0;
	if (targetcls != 0x69) {
		additem(targety * 150 + targetx, randpit());
		msgf(0x63, strs[1387]);
		return 0;
	}
	digtrap = findclass(targety * 150 + targetx, 0x34);
	if (!ask && !digtrap)
		return 0;
	if (asksteal(0))
		return 0;
	tire(1);
	train(11, 1);
	train(weaponskill(), 1);
	plottreasure(curmap, targetx, targety);
	liftitem(0);
	targetitem = 0;
	lookaround();
	if (digtrap) {
		msgf(0x63, strs[1384]);
		playnow(0x26);
		music(0x16, 0);
	} else {
		msgf(0x63, strs[1386]);
		playnow(0x24);
		music(0x16, 0);
	}
	drawscreen();
	train(0x14, 5);
	train(0x16, 5);
	train(0x17, 5);
	addfavor(10);
	return 1;
}

int
workfor(int t)
{
	if (woulddie(t))
		return 0;
	if (units[0].pose == 0 || units[0].pose > 9) {
		if (units[0].pose != 10)
			units[0].pose = 0;
	}
	setunitmode(units, 0);
	passtime(t - 2, 0, 0);
	tire(-1);
	return 1;
}

int
weave(int n)
{
	if (!n) {
		if (outofreach(targetkind, targetx, targety, targetitem, 1, 0)) {
			msgf(0x63, texts + 0xb079, itemname(targetitem), strs[1569]);
			return 0;
		}
		if (counttypes(0, 0xf9, 0x57c, 0x57d) < 6) {
			wsprintf(tmpbuf, strs[1183], 6);
			usefail(tmpbuf);
			return 0;
		}
		return 1;
	}
	taketypes(0, 0xf9, 0x57c, 0x57d, 6, -1);
	giveitem(0, randlook(newitem(0x2c3)), 1);
	train(0x1a, 0x14);
	msgf(0x63, strs[1184]);
	return 1;
}

int
repair(void)
{
	unsigned long item;
	short r;

	repmetal = 0;
	switch (typenames[targetitem & 0xfff]) {
	case 0x4d:
		repmetal = 1;
		break;
	case 0x70:
		break;
	default:
		switch (baselook(targetitem & 0xfff)) {
		case 0x1e2:
		case 0x1e3:
			repmetal = 1;
			goto done;
		}
		return 0;
	}
done:
	item = inpack(0, 0, 0x1e3, 0);
	rephammer = item;
	reppliers = inpack(0, 0, 0x1e2, 0);
	repanvil = finditem(units, 0x96e, 0, 0, 0, 2) || finditem(units, 0x96c, 0, 0, 0, 2) ? 1 : 0;
	repfire = finditem(units, 0x5fe, 0, 0, 0, 2);
	if (repmetal) {
		if (!repfire || !repanvil) {
			if (rephammer || reppliers)
				msgf(0x63, strs[1185]);
			return 1;
		}
		if (!reppliers) {
			msgf(0x63, strs[1186]);
			return 1;
		}
		if (!rephammer) {
			msgf(0x63, strs[1187]);
			return 1;
		}
		savetarget();
		repask = askmsg(strs[1188]);
		keeplast();
	} else {
		savetarget();
		repask = askmsg(strs[1188]);
		keeplast();
	}
	do {
		if (repmetal)
			repval = asktarget(0x3c, 0x46, 0x41, 0xff, 0xff, 1, 0, 1, 1, 0);
		else
			repval = asktarget(0x3c, 0x46, 0x41, 0x55, 0x4b, 1, 0, 1, 1, 0);
	} while (repval != -1 && asksteal(0));
	delmsgid(repask);
	if (repval == -1 || playerout() || playmode != 1) {
		restoretarget();
		if (!nmsgs)
			denymsg(strs[273], itemname(targetitem));
		return 1;
	}
	if (!repairable(targetitem)) {
	empty:
		msgf(0x63, strs[1189], itemname(targetitem));
		return 1;
	}
	if (targetcls == 0x49 || targetcls == 0x4a) {
	cannot:
		msgf(0x63, strs[1190], itemname(targetitem));
		return 1;
	}
	repsoft = 0;
	if (targetcls == 0x3c)
		repval = metals[gearof(targetitem)].metal;
	else {
		repval = armors[gearof(targetitem)].metal;
		if (armors[gearof(targetitem)].material == 0)
			repsoft = 1;
	}
	if (repmetal) {
		if (repval != 1) {
			msgf(0x63, strs[1191], itemname(targetitem));
			return 1;
		}
	} else if (repval == 1) {
		if (!repfire || !repanvil || !reppliers || !rephammer) {
			msgf(0x63, strs[1192], itemname(targetitem));
			return 1;
		}
		repmetal = 1;
	}
	repval = wearof(targetitem);
	if (repval == 0)
		goto empty;
	if (repval >= 6)
		goto cannot;
	train(0x1a, repval * 2 + 0x14);
	if (workfor(hourticks * 5)) {
		if (repval != 1)
			goto other;
		if (prevails(skillof(units, 0x1a, 0, 0, 0), 0x50)) {
		ok:
			train(0x1a, repval * 10 + 0x14);
			liftitem(0);
			repval--;
			setwear(&targetitem, repval);
			placeitem();
			msgf(0x63, strs[1193], itemname(targetitem));
			return 2;
		}
		goto fail;
	other:
		if (repval == 2)
			r = prevails(skillof(units, 0x1a, 0, 0, 0), 0x5a);
		else if (repval == 3)
			r = prevails(skillof(units, 0x1a, 0, 0, 0), 0x64);
		else if (repval == 4)
			r = prevails(skillof(units, 0x1a, 0, 0, 0), 0x6e);
		else
			r = prevails(skillof(units, 0x1a, 0, 0, 0), 0x78);
		if (r)
			goto ok;
	fail:
		if (randn(100) < 50)
			msgf(0x63, strs[1194]);
		else {
			liftitem(0);
			repval++;
			if (!prevails(skillof(units, 0x1a, 0, 0, 0), 0x5a))
				repval += randn(4);
			if (repval > 7) {
				targetitem = broken(targetitem);
				giveitem(0, targetitem, 1);
			} else {
				setwear(&targetitem, repval);
				placeitem();
			}
			if (repval > 7)
				msgf(0x63, strs[1195], itemname(targetitem), repsoft ? strs[1196] : strs[1197]);
			else
				msgf(0x63, strs[1198], itemname(targetitem));
		}
	}
	return 2;
}

int
useitem(short ask)
{
	Unit *u;
	int n;

	useres = 0xff;
	usebusy = busyturns;
	if (unconscious())
		return 0;
	if (targetkind == 0x29 || targetkind == 0x2a) {
		usecls = panels[targetkind - 0x28].arg;
		if (usecls == 0 || usecls == 2 && targetx != 0x28 && targetx != 0x29 && targetx != 0x2a)
			return 0;
	}
	usecls = classof(targetitem & 0xfff);
	if (!(usecls == 0x14 || issign(targetitem) || usecls == 0xc8 || usecls == 0xc9
	|| usecls == 0xca || usecls == 0xcb || usecls == 0xd1 || usecls == 0xcc
	|| usecls == 0xcd || usecls == 0xce || usecls == 0xcf || usecls == 0xd0
	|| usecls == 0x98 || usecls == 0x64
	|| usecls == 0x46 && unmanly(targetitem, 0)
	|| usecls == 0x69 && typenames[targetitem & 0xfff]
	|| usecls == 0x6a && typenames[targetitem & 0xfff] == 0x4b
	|| usecls == 0
	|| !outofreach(targetkind, targetx, targety, targetitem, 2, 1)))
		return 0;
	if (asksteal(1)) {
		useres = 0;
		goto done;
	}
	addscore(1);
	switch (usecls) {
	case 5:
		units[0].pose = 0;
		usekey(0);
		useres = 1;
		break;
	case 0x32:
		units[0].pose = 0;
		uselever();
		useres = 1;
		break;
	case 0xa:
		useres = opencontainer();
		break;
	case 0xf:
	case 0x10:
		msgf(0x63, strs[(unsigned)random() < 0x3fff ? 301 : 302]);
		playnow(2);
		useres = 1;
		break;
	case 0x1e:
		if (readitem(1)) {
			if (playmode != 1)
				busyturns += 3;
			useres = 1;
		} else
			useres = 0;
		break;
	case 1:
		units[0].pose = 0;
		return 999;
	case 4:
		denymsg(strs[1396]);
		useres = 1;
		break;
	case 0x36:
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
		if (typenames[targetitem & 0xfff] == 0x99 && game.hungerlevel < 6) {
			anchortip(1, 0x140, 100);
			showtip(tipanchorx, tipanchory, strs[1199]);
			useres = 1;
			break;
		}
		if ((usecls == 0x38 || usecls == 0x39) && (targetitem & 0xc0050000) && ask
		&& !(usecls != 0x38 && !getbit(game.flags, 0x4b0))) {
			savetarget();
			useres = mixpotion(0);
			restoretarget();
			if (useres == 2)
				;
			else {
				if (useres) {
					liftitem(0);
					if (usecls == 0x38) {
						emptyvessel(&targetitem);
						goto put;
					}
				}
				goto shown;
			}
		}
		usecls = classof(targetitem & 0xfff);
		if (consume(units, targetitem, 1)) {
			if (usecls == 0x37 || usecls == 0x36 || usecls == 0x39) {
				liftitem(0);
				if (pickcrop(&targetitem))
					goto put;
			} else if (usecls == 0x87 || usecls == 0x88 || usecls == 0x89
			|| usecls == 0x8a || usecls == 0x8b || usecls == 0x38) {
				liftitem(0);
				emptyvessel(&targetitem);
			put:
				placeitem();
			}
		}
	shown:
		calcstats(units);
		useres = 1;
		break;
	case 0x3a:
		units[0].pose = 0;
		memcpy(picbuf, panelstack, 16);
		useother = targetitem;
		savetarget();
		useask = askmsg(strs[1459]);
		keeplast();
		useres = asktarget(0x82, 0x83, 0x84, 0x85, 0x86, 0, 0, 0, 0, 0);
		delmsgid(useask);
		if (playerout() || playmode != 1) {
			restoretarget();
			useres = 0;
			break;
		}
		if (targetkind == 1) {
			if (outofreach(targetkind, targetx, targety, targetitem, 2, 1))
				usewater = 0;
			else
				usewater = waterat(targetx, targety);
		} else
			usewater = 0;
		if (!useres && usewater == 0) {
			restoretarget();
			if (!nmsgs)
				denymsg(strs[273], itemname(targetitem));
			useres = 0;
			break;
		}
		fillvessel(&useother, usewater);
		restoretarget();
		liftitem(0);
		targetitem = useother;
		placeitem();
		memcpy(panelstack, picbuf, 16);
		useres = 1;
		break;
	case 0x14:
		useres = 1;
		if (units[targetidx].mode == 0x5a) {
			u = &units[targetidx];
			if (isfemale(u) && censorship != 2 && censorship != 4
			&& !chaste(curmap, (short)targetidx)
			&& (!u->spouse || units[u->spouse].health <= 0)) {
				if (u->sympathy < 4 && !myservant(u)) {
					if (u->lord) {
						if (units[u->lord].sympathy >= 3)
							goto ambush;
						if (units[u->lord].sympathy < 2 && cansee(units[u->lord].x, units[u->lord].y))
							goto talk;
					}
					if (!isservant(u) || u->sympathy < 3)
						goto talk;
				}
			ambush:
				if (!outofreach(targetkind, targetx, targety, targetitem, 2, 0)) {
					if (!woulddie(hourticks * 3)) {
						targetitem = 0x260;
						gosleep(0);
						useres = 1;
						break;
					}
					cleartexts();
				}
			}
		} else if (units[targetidx].scene && units[targetidx].sympathy > 0 && units[targetidx].confused == 0
		&& units[targetidx].asleep == 0 && units[targetidx].mode != 0x5a && !units[targetidx].asleep) {
			dropheld();
			talkto(targetidx);
			calcstats(units);
			drawscreen();
			return 0x378;
		}
	talk:
		if (units[targetidx].mode == 0x19)
			msgf(0x63, strs[304], unitname(&units[targetidx]));
		else
			msgf(0x63, strs[303]);
		calcstats(units);
		break;
	case 0x15:
		queuesound(4);
		useres = usecorpse(1);
		break;
	case 0x16:
		msgf(0x63, strs[544]);
		useres = 0;
		break;
	case 0x28:
		units[0].pose = 0;
		useres = (unsigned short)(targetitem >> 16 & 0x7f);
		if (useres) {
			queuesound(2);
			curregion = targetitem >> 23 & 0xff;
			enterspot(useres);
		} else
			failmsg(strs[317]);
		useres = 1;
		calcstats(units);
		break;
	case 0x19:
	case 0x78:
	case 0x79:
	case 0x7a:
	case 0x7b:
		units[0].pose = 0;
		if (playmode != 1) {
			denycombat();
			useres = 1;
		} else {
			gosleep(1);
			useres = 1;
		}
		break;
	case 0x46:
		if (unmanly(targetitem, 1)) {
			useres = 0;
			break;
		}
		if (playmode != 1) {
			if (units[0].body)
				busyturns += 2;
			busyturns += 2;
		}
		goto use;
	case 0x48:
		if (playmode != 1) {
			if (units[0].cloak)
				busyturns++;
			busyturns++;
		}
		goto use;
	case 0x4b:
		if (playmode != 1) {
			if (units[0].legs)
				busyturns += 2;
			busyturns += 2;
		}
		goto use;
	case 0x50:
		if (playmode != 1 && units[0].feet)
			busyturns++;
		goto use;
	case 0x56:
		units[0].pose = 0;
		if (istorch(units[0].offhand)) {
			if (targetkind == 0x2b) {
				if (isunlittorch(targetitem)) {
					setlit(&targetitem, 1);
					queuesound(7);
				}
				useother = units[0].offhand;
				if (islittorch(useother))
					setlit(&useother, 0);
				units[0].offhand = baselook(targetitem);
				liftitem(0);
				targetitem = useother;
				placeitem();
				useres = 1;
				break;
			}
		} else {
		drop:
			liftitem(0);
			targetitem = baselook(targetitem);
			if (isunlittorch(targetitem)) {
				setlit(&targetitem, 1);
				queuesound(7);
			}
			if (units[0].offhand) {
				giveitem(0, units[0].offhand, 1);
				units[0].offhand = 0;
			}
			giveitem(0, targetitem, 1);
			useres = 1;
			break;
		}
		if (targetkind >= 0x2b && targetkind <= 0x30)
			goto drop;
		if (targetkind == 0x29 || targetkind == 0x2a) {
			useother = panels[targetkind - 0x28].id;
			if (useother && units[useother].health <= 0)
				goto drop;
		}
		/* fall through */
	case 0x59:
	case 0x5a:
		units[0].pose = 0;
		togglefire();
		useres = 1;
		calcstats(units);
		break;
	case 0x55:
		if (playmode != 1 && units[0].offhand)
			busyturns++;
		goto use;
	case 0x3c:
		if (playmode != 1) {
			if (units[0].hand)
				busyturns++;
			break;
		}
		if ((targetkind == 0x29 || targetkind == 0x2a) && panels[targetkind - 0x28].id == 0
		&& targetx == 0x14) {
			combatcause = 1;
			togglecombat(0);
			useres = 1;
			break;
		}
		goto use;
	case 0x63:
		if (playmode != 1 && units[0].wrist)
			busyturns++;
		goto use;
	case 0x62:
		if (playmode != 1 && units[0].neck)
			busyturns++;
		goto use;
	case 0x41:
		if (playmode != 1 && units[0].head)
			busyturns++;
	use:
		if ((targetkind == 0x29 || targetkind == 0x2a) && panels[targetkind - 0x28].id == 0
		&& targetx != 0x28 && targetx != 0x29 && targetx != 0x2a && targetx != 0x15
		&& usecls != 0x56) {
			busyturns = usebusy;
			useres = 0;
			break;
		}
		units[0].pose = 0;
		useres = 1;
		liftitem(0);
		drawscreen();
		if (usecls == 0x3c)
			wield(targetkind, targetx, targety, targetitem);
		else {
			useother = puton(0, targetitem);
			if (useother == 0x3e7) {
				placeitem();
				denymsg(strs[274], itemname(targetitem));
			} else {
				if (usecls == 0x56)
					;
				else if (usecls == 0x3c || usecls == 0x55)
					queuesound(2);
				else
					queuesound(3);
				if (useother && !packitem(useother)) {
					targetitem = puton(0, useother);
					if (targetitem != 0x3e7 && targetitem)
						placeitem();
					denymsg(strs[545]);
				}
			}
		}
		calcstats(units);
		lookaround();
		calctorch();
		drawscreen();
		if (usecls == 0x56)
			lookdone();
		break;
	case 0x2e:
		hasaction(0x12);
		doaction(0);
		useres = 1;
		break;
	case 0x69:
		if (cheat && (keys['W'] & 0x80)) {
			if (dig(1))
				useres = 1;
		} else if (typenames[targetitem & 0xfff])
			usefail(strs[1182]);
		else if (dig(0))
			useres = 1;
		break;
	case 0x6a:
		if (typenames[targetitem & 0xfff] != 0x4b)
			break;
		/* fall through */
	case 0xcb:
	case 0xd1:
		n = repair();
		if (!n)
			goto nofood;
		if (n != 2) {
			useres = 0;
			break;
		}
		useres = 1;
		break;
	case 0x97:
		if (targetkind == 1 && (mapflags[targety * 150 + targetx - 1] & 0x100)) {
			queuesound(7);
			useres = 1;
		} else {
			usefail(strs[1561]);
			useres = 1;
		}
		break;
	case 0x96:
		useother = targetitem;
		useask = askmsg(strs[409]);
		keeplast();
		useres = asktarget(0x3c, 0xff, 0xff, 0xff, 0xff, 1, 0, 1, 1, 0);
		delmsgid(useask);
		if (playerout() || playmode != 1) {
			useres = 0;
			break;
		}
		if (useres == 0) {
			if (!nmsgs)
				denymsg(strs[273], itemname(useother));
			break;
		}
		if (asksteal(2)) {
			useres = 0;
			break;
		}
		liftitem(0);
		useres = (short)grind();
		placeitem();
		if (useres)
			units[0].pose = 0;
		break;
	case 0x20:
		useres = 1;
		if (game.fishcount >= 15) {
			anchortip(1, 0x140, 100);
			showtip(tipanchorx, tipanchory, strs[1181], game.fishcount);
			break;
		}
		for (usedy = -1; usedy <= 1; usedy++)
			for (usedx = -1; usedx <= 1; usedx++)
				if (waterat(playerx + usedx, playery + usedy) == 1) {
					if (((unsigned char *)mapstate)[0x22] <= 15) {
						usefail(strs[1179]);
						goto done;
					}
					if (!workfor(hourticks / 2))
						goto done;
					if (randn(100) < 30) {
						failmsg(strs[1178]);
						goto done;
					}
					useother = newitem(0x533);
					useother = randlook(useother);
					giveitem(0, useother, 1);
					if (game.fishcount < 250)
						game.fishcount++;
					msgf(0x63, strs[1177]);
					queuesound(8);
					useres = 1;
					goto done;
				}
		usefail(strs[1180]);
		break;
	case 0x21:
		useres = 1;
		if (woulddie(hourticks / 3))
			break;
		if (game.tries >= 15) {
			anchortip(1, 0x140, 100);
			showtip(tipanchorx, tipanchory, strs[1388], game.tries);
			break;
		}
		usewater = playmode;
		useask = askmsg(strs[426]);
		keeplast();
		do
			usedx = asktarget(0xff, 0xff, 0xff, 0xff, 0xff, 0, 0, 0, 0, 0);
		while (usedx != -1 && asksteal(0));
		delmsgid(useask);
		if (usedx == -1 || playerout() || usewater != playmode)
			break;
		if (targetkind != 1)
			denymsg(strs[287]);
		if ((targetcls == 1 || targetcls == 4) && (targetitem & 0x10000000) == 0 && (targetitem & 0x8000000) != 0
		|| targetcls == 10 && (realms[idof(targetitem)].item & 0x8000000)) {
			if (!outofreach(targetkind, targetx, targety, targetitem, 1, 1)) {
				passtime(hourticks / 3 - 2, 0, 0);
				forceopen(2);
			}
			break;
		}
		if (findclass(targety * 150 + targetx, 0x68) || findclass(targety * 150 + targetx, 0x67)) {
			denymsg(strs[1389]);
			break;
		}
		if (!findclass(targety * 150 + targetx, 0x69) && !diggable(targetx, targety)) {
		nofire:
			denymsg(strs[1390]);
			break;
		}
		if (targetitem && classof(targetitem & 0xfff) != 0x69) {
			if (itemweight(targetitem) < 65000) {
				denymsg(strs[1391]);
				break;
			}
			if (typenames[targetitem & 0xfff] == 0)
				;
			else
				goto nofire;
		}
		if (!outofreach(targetkind, targetx, targety, targetitem, 1, 1)) {
			if (units[0].pose == 0 || units[0].pose > 9) {
				if (units[0].pose != 10)
					units[0].pose = 0;
			}
			setunitmode(units, 0);
			passtime(hourticks / 3 - 2, 0, 0);
			tire(9);
			dig(1);
			if (game.tries < 250)
				game.tries++;
		}
		break;
	case 0xc8:
		msgf(0x63, texts + 0xb079, itemname(targetitem), strs[1567]);
		useres = 0;
		break;
	case 0xc9:
		msgf(0x63, texts + 0xb079, itemname(targetitem), strs[1568]);
		useres = 0;
		break;
	case 0xca:
		if (typenames[targetitem & 0xfff] == 0x67) {
			useres = 1;
			if (weave(0) && workfor(hourticks * 7))
				weave(1);
		} else {
			msgf(0x63, texts + 0xb079, itemname(targetitem), strs[1569]);
			useres = 0;
		}
		break;
	nofood:
		useres = 0;
		if (usecls == 0xd1)
			msgf(0x63, texts + 0xb079, itemname(targetitem), strs[1576]);
		else
			msgf(0x63, texts + 0xb079, itemname(targetitem), strs[1570]);
		break;
	case 0xcc:
		msgf(0x63, texts + 0xb079, itemname(targetitem), strs[1571]);
		useres = 0;
		break;
	case 0xcd:
		msgf(0x63, texts + 0xb079, itemname(targetitem), strs[1572]);
		useres = 0;
		break;
	case 0xce:
		msgf(0x63, texts + 0xb079, itemname(targetitem), strs[1573]);
		useres = 0;
		break;
	case 0xcf:
		msgf(0x63, texts + 0xb079, itemname(targetitem), strs[1574]);
		useres = 0;
		break;
	case 0xd0:
		msgf(0x63, texts + 0xb079, itemname(targetitem), strs[1575]);
		useres = 0;
		break;
	case 0x98:
		msgf(0x63, texts + 0xb079, itemname(targetitem), strs[1562]);
		useres = 1;
		break;
	case 0:
		if (unmanly(targetitem, 1))
			useres = 0;
		break;
	}
done:
	if (freetake)
		forgettarget();
	else
		notetarget();
	playqueue();
	if (useres == 0xff && !(usecls == 0xf0 || usecls == 0xff || usecls == 0x16
	|| usecls == 0x69 || usecls == 0x68 || usecls == 0x67) && *typetext(targetitem) != 0)
		denymsg(strs[272], itemname(targetitem));
	drawscreen();
	if (units[0].health > 0)
		fadefast(1);
	tracesite = 9;
	while (mouseleft) {
		if (playerout())
			break;
		if (playmode == 1 && units[0].health > 0)
			gametick();
		else {
			readinput();
			if (needredraw(0xfd))
				drawscreen();
		}
	}
	leftclick = 0;
	return useres;
}

unsigned char infopanel = 255;
unsigned short liftn;
int liftres;
unsigned long dropw;
int placek;
int placepos;
int placex;
int placey;
int busysave;
int busyres;
short combati;
short autofoe;
unsigned char infocls;
short infounit;
int otherpos;
int trywt[2];
int tryx;
int tryy;
char trycount;
unsigned short trycell;
char tryalt;
int advwx;
int advwy;
int advunit;
char advtry;
unsigned short advcell;
char advalt;
int spotn;
unsigned long *keylock;
short keyres;
short keyask;
char *readtxt;
char *readscan;
unsigned short readno;
int placedir;
short underi;
short underxy;
short underw;
short undern;
Node *undertab;
short byei;
short byefound;
char *byep;
char *byeq;
short byen;
Node *byetab;
short nodecnti;
short nodecnt;
short talkplain;
int talkcode;
short talknode;
short talkstep;
short talkmx;
short talkmy;
short talki;
short talknew;
unsigned char talktop;
unsigned char talkbottom;
unsigned char talkleft;
unsigned char talkright;
short talkbegun;
unsigned long talkwait;
char *talkp;
short talkslide;
short talkprev;
char talkknown[30];
int talkunit;
short idleunit;
unsigned long *firep;
short firelit;
short tohours;
int toticks;
short ambi;
unsigned long passdone;
short passi;
unsigned long aloneticks;
unsigned long awayticks;
unsigned long awaytime;
short awayi;
short awayhour;
short awayday;
int awayx;
int awayy;
unsigned short sleepslot;
unsigned char bedkind;
unsigned char bedcls;
unsigned long sleepwait;
int sleepx;
int sleepy;
short grindwear;
unsigned long *mixp;
unsigned char mixcls;
int mixres;
short mixask;
short digtrap;
char rephammer;
char reppliers;
char repfire;
char repanvil;
int repval;
short repask;
short repmetal;
short repsoft;
int useres;
int usedx;
int usedy;
unsigned char usecls;
short usewater;
unsigned long useother;
int usebusy;
short useask;
short skilldue;

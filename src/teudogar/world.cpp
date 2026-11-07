#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

typedef struct Door Door;
typedef struct Realm Realm;
typedef struct Need Need;
typedef struct Unit Unit;

struct Door {
	int closed1;
	int open1;
	int closed2;
	int open2;
};

struct Need {
	unsigned long item;
	unsigned char n;
	unsigned char maker;
	unsigned char period;
	unsigned char elapsed;
};

struct Realm {
	unsigned long item;
	unsigned long items[50];
	unsigned short pos[50];
	Need needs[3];
	unsigned char nneeds;
	unsigned char region;
	char f14a[0x158 - 0x14a];
};

struct Unit {
	unsigned char x;
	unsigned char y;
	char f2[4 - 2];
	unsigned char dirx;
	unsigned char diry;
	char f6[8 - 6];
	char pose;
	unsigned char mode;
	char fa[0xeb - 0xa];
	unsigned short plea;
	short strength;
	short dexterity;
	char intellect[2];
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
	char asleep;
	char bewitched;
	char blessed;
	unsigned char poison;
	char cursed;
	char f13a[0x13c - 0x13a];
	char bleeding;
	char f13d[0x15f - 0x13d];
	unsigned char jobcount;
	unsigned char job;
	char f161[0x1b8 - 0x161];
};

typedef struct Metal Metal;
typedef struct Region Region;
typedef struct Panel Panel;
typedef struct Job Job;
typedef struct Game Game;
typedef struct Kind Kind;
typedef struct Zone Zone;
typedef struct Rect Rect;
typedef struct Menu Menu;

/* 0xd0 bytes each (005acf80) */
struct Region {
	short x0;
	short y0;
	short x1;
	short y1;
	char f8[0x10 - 8];
	unsigned char lord;
	char f11;
	short exitx[8];
	short exity[8];
	unsigned char nexits;
	char f33[0x35 - 0x33];
	unsigned char tag;
	char f36[0x47 - 0x36];
	unsigned char units[0x10];
	unsigned char nunits;
	char f58[0xd0 - 0x58];
};

/* 0x28 bytes each (00a4ca78): the screen's panels */
struct Panel {
	char f0[6];
	int x;
	int y;
	int w;
	int h;
	unsigned short id;
	short arg;
	char f1a[0x1c - 0x1a];
	unsigned short cellx;
	unsigned short celly;
	char f20[0x28 - 0x20];
};

/* 6 bytes each (004f9458) */
struct Job {
	unsigned long item;
	unsigned char type;
	unsigned char n;
};

/* the state saved with the game, 0x5f8 bytes at 009cc470 */
struct Game {
	char f0[0x34];
	char flags[0xc8];
	char ffc[0x174 - 0xfc];
	short tiredcount;
	unsigned char tiredlevel;
	short hungercount;
	unsigned char hungerlevel;
	char f17a[0x1a0 - 0x17a];
	char knownzones[13];
	char seenzones[13];
	char f1ba[0x39a - 0x1ba];
	unsigned char fishcount;
	unsigned char tries;
	char f39c[0x3cf - 0x39c];
	char reachedzones[0x49b - 0x3cf];
	char visited[0x4bd - 0x49b];
	char f4bd[0x5f8 - 0x4bd];
};

/* 0x41 bytes each (unittype.cpp's kinds) */
struct Kind {
	char f0[0x18];
	short maxhealth;
	char f1a[0x41 - 0x1a];
};

struct Rect {
	unsigned short x;
	unsigned short y;
	unsigned short w;
	unsigned short h;
};

/* menus's menu, 0x1f7 bytes */
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

/* 0x22 bytes each (004f9498): a rectangle x0, y0 to x1, y1 */
struct Zone {
	short id;
	unsigned short name;
	short f4;
	short tmx0;
	short tmy0;
	short tmx1;
	short tmy1;
	char fe[0x12 - 0xe];
	short inx;
	short iny;
	short indx;
	short indy;
	short x;
	short y;
	short dx;
	short dy;
};

typedef struct Piece Piece;

/* 0x14 bytes each (004f9898): a piece of the travel map, drawn once its game flag is set */
struct Piece {
	int x0;
	int y0;
	int x1;
	int y1;
	int flag;
};

/* 0x1b bytes each (004c08a8) */
struct Metal {
	unsigned char f0;
	char f1;
	short minstr;
	char f4[0x1b - 4];
};

unsigned short weightof(int i);
unsigned char classof(int item);
unsigned long ageof(unsigned long item);
int satiety(unsigned long item);
int topitem(int pos);
unsigned short wearof(unsigned long item);
unsigned long baselook(unsigned long item);
short canstack(unsigned long a, unsigned long b);
unsigned long addcoins(unsigned long *p, unsigned long b);
short islittorch(unsigned long item);
unsigned short idof(unsigned long item);
short setlit(unsigned long *p, short dir);
void updatecell(int pos);
unsigned long randlook(unsigned long item);
unsigned long valueof(unsigned long item);
unsigned long additem(int pos, unsigned long item);
unsigned long popitem(int pos, char any);
void immobtext(char *buf, Unit *u);
char *itemname(unsigned long item);
unsigned char tilepix(int x, int y);
void delmsgid(short id);
short msgf(short prio, char *fmt, ...);
short isarmor(unsigned long item);
short isspear(unsigned long item);
void tilebox(int *x0, int *y0, int *x1, int *y1);
void gettileo(int n);
int badunit(Unit *u);
void calcstats(Unit *u);
void queuesound(short n);
unsigned long giveitem(short n, unsigned long item, short mode);
void dresshero(void);
short denymsg(char *fmt, ...);
short isthrowing2(unsigned long item);
unsigned short gearof(unsigned long item);
short isunlittorch(unsigned long item);
void picksheath(unsigned long item, short flag, short mode);
void pickworn(unsigned long item, short flag);
unsigned char getpixr(int idx, int x, int y);
short isdagger(unsigned long item);
short issword(unsigned long item);
int usabletorch(unsigned long item);
short burntorch(unsigned long *p);
int itemcell(int n);
void listtorches(void);
void resetinput(void);
void waitrelease(short anim);
void drawscreen(void);
short findclass2(int pos, unsigned char a, unsigned char b);
void calctorch(void);
int __cdecl pollinput(void);
int __cdecl readinput(void);
void gametick(void);
int needredraw(int c);
void hidecursor(void);
void setclip(short x0, short y0, short x1, short y1);
void hoversound(void);
void defclip(void);
void drawcursor(void);
void closepanels(void);
void cleartexts(void);
void playnow(short n);
void closepanel(unsigned char n);
int raisepanel(unsigned char n);
void openpanel(unsigned char n);
void movepanel(unsigned char n, short x, short y);
void dropheld(void);
int isopen(unsigned char n);
void playerstep(int x, int y);
unsigned char unitat(int x, int y);
short ownerat(short x, short y);
unsigned long corpseof(unsigned long n);
int findcell(unsigned long n);
unsigned long *gearslot(Unit *u, int slot);
short clearpath(int x0, int y0, int x1, int y1);
int liftitem(short ask);
short failmsg(char *fmt, ...);
void calcload(unsigned short n);
void cleartip(void);
unsigned char openstore(unsigned long item, int x, int y);
void entermenu(void);
void setcursor1(int kind, int img);
int playerout(void);
void leavemenu(short anim);
short issmith(Unit *u);
short iscraftsman(Unit *u);
short isfemale(Unit *u);
short isservant(Unit *u);
short myservant(Unit *u);
short atwork(Unit *u);
short athome(Unit *u, short k);
unsigned char itempanel(unsigned long item);
int consume(Unit *u, unsigned long item, short v);
Kind *kindof(Unit *u);
unsigned long jewelmagic(Unit *u, unsigned long mask);
void fillgauges(void);
void markunits(void);
int plotday(void);
void scavenge(short all);
void sweepmap(short all);
void decaycorpses(void);
void plotcheck(void);
int lightpal(int n, int which);
unsigned char getpixel(unsigned char *buf, int x, int y);
int nearcursor(int x, int y);
void putm1b(int n, int x, int y, int idx);
void grabtile(int k, int s, int x, int y);
void puttile1(int k, int idx);
void puttile3(int k, int idx);
int pillshade8(int x, int y, int w, int h);
int text3w(char *s);
int text3cn(int n, int cx, int y, char *fmt, ...);
char *daystext(short n);
short woulddie(unsigned long t);
int text3(int n, int x, int y, char *s);
int putwh(unsigned char *src, int n, int x, int y, int w, int h);
void putfacen(int n, int x, int y, char *src);
int isbasemap(int cell);
int notravel(void);
void nop(void);
int fadefast(int in);
void usepal(unsigned char n);
void pgtoback(void);
int loadscr(int id, unsigned char *dst, unsigned char *pal, void *buf, DWORD size);
int unpackscr(unsigned char *dst, unsigned char *pal, void *src, DWORD n);
int showscrt(void);
int putblockt(int n, int x, int y, unsigned char *buf, int bx, int by, int w, int h);
void line(int x1, int y1, int x2, int y2, unsigned char color);
short mayquit(short unused);
void quit(void);
short nearfoe(int n);
void addcorner(Menu *m, short n, short kind);
void faceshadow(int x, int y, char *src);
int getwh(int n, int x, int y, unsigned char *dst, int w, int h);
void putface(int x, int y, char *src);
void drawname(int x, int y, int n, int clip);
void begintext(int on);
int text3m(int n, int x, int y, char *s);
void endtext(int on);
void puthands(void);
void backto2(void);
void pagesound(void);
int flip(void);
void turnpage(int a, int b, int c, int d);
int music(int n, int lp);
short runmenu(Menu *m, int (*idle)(void), short (*keyfn)(unsigned char, unsigned char), short cancel);
short celldist(int x, int y, int x2, int y2);
void train(unsigned char k, int n);
void setcombat(short n);
void plotdepart(short a, short b);
int autosave(void);
int travel(unsigned short dest, int x, int y, unsigned long time, unsigned short depart, unsigned short a6, unsigned short mode, short a8);
// FIXED: the option of the AutoSave, which travelmap reads below
#if defined(FEATURE_FIXES)
extern int autolevel;
#endif

extern int playerx;
extern int playery;
extern unsigned long mapitems[];
extern int showall;
extern unsigned char viewgrid[17][19];
extern int doors[2][16];
extern unsigned short playercell;
extern unsigned long mapflags[150 * 150];
extern Realm realms[125];
extern unsigned char typenames[];
extern unsigned short volumes[];
extern Unit units[150];
extern char texts[47942];
extern char *strs[];
extern char talkactive;
extern char namebuf[2500];
extern char textbuf[];
extern int bloodtill;
extern Metal metals[];
extern short lastlight;
extern char empty[1];
extern int wornpic;
extern short wornx;
extern short worny;
extern short wornw;
extern short wornh;
extern int hoverpx;
extern int hoverpy;
extern char knockedout;
extern unsigned char passmode;
extern short torches[100];
extern unsigned short ntorches;
extern unsigned short nmapitems;
extern unsigned char palblack;
extern Region regions[];
extern short nregions;
extern unsigned char regionmap[150 * 150];
extern unsigned char provmap[150 * 150];
extern unsigned long map[150 * 150];
extern short curregion;
extern short regiontag;
extern unsigned char blocks[30][30];
extern char lookstale;
extern unsigned char gamehour;
extern short firedist;
extern short litnw;
extern short litne;
extern short litsw;
extern short litse;
extern int tracesite;
extern int waitforup;

extern int mouseleft;
extern int mouseright;
extern int leftclick;
extern int rightclick;
extern int dblclick;
extern int playmode;
extern char keycode;
extern char lastscan;
extern unsigned char esckey;
extern int quitting;
extern int mousex;
extern int mousey;
extern int mousemoved;
extern int presslatch;
extern Panel panels[];
extern unsigned char panelstack[];
extern unsigned char targetkind;
extern unsigned long targetitem;
extern int targetidx;
extern int targetx;
extern int targety;
extern int savtx;
extern int savty;
extern int hoverkind;
extern int hovercx;
extern int hovercy;
extern int savhx;
extern int savhy;
extern unsigned char presskind;
extern int presscx;
extern int presscy;
extern unsigned char targetcls;
extern short targetowner;
extern unsigned char menumode;
extern unsigned long tradegoods[];
extern unsigned long tradeoffer[];
extern short tradekind;
extern unsigned short curunit;
extern int cheat;
extern unsigned char keys[256];
extern int lineendx;
extern int lineendy;
extern short gameday;
extern short sunlight;
extern char mapstate[100];
extern short lastambient;
extern short sensesleft;
extern short lightwarm;
extern short lightlvl;
extern short torchdist;
extern short lastwarm;
extern int lineokx;
extern int lineoky;
extern Zone zones[];
extern char scrbuf[309700];
extern Menu menu1;
extern char printbuf[];
extern int text3width;
extern char msgtext[17][200];
extern short nmsgs;
extern unsigned short hourticks;
extern short curblink;
extern char faceback[];
extern char curface2[];
extern char curface[];
extern unsigned short curmap;
extern int lefttime;
extern int shoetime;
extern unsigned char shoewarns;
extern unsigned char indoinput;
extern short actionturn;
extern int statshown;
extern int hoverx;
extern unsigned char notedkind;
extern Job jobs[];
extern unsigned short nunits;
extern unsigned char nrealms;
extern Game game;
extern int hourtime;
extern unsigned long gametime;
extern unsigned char *drawbuf;
extern int memguard6;
extern int talkok;
extern short pagefx;
extern HGLOBAL travel1;
extern int travel1len;
extern HGLOBAL travel2;
extern int travel2len;
extern HGLOBAL travel3;
extern int travel3len;
extern char packbuf[];
extern Piece pieces[5];
extern char lastspeech;
extern short trvfrom;
extern short trvto;
extern char speech[];
extern int tickms;
extern short menuanim;
extern void (*menudraw)(void);
extern void (*menutick)(int);
extern int cur;

extern int sacol;
extern int sarow;
extern unsigned long csitem;
extern unsigned char cscls;
extern unsigned long oditem;
extern unsigned long odk;
extern int odidx;
extern unsigned long cditem;
extern int cdk;
extern int cdidx;
extern int bdidx;
extern unsigned char bdcls;
extern unsigned long plcell;
extern int plidx;
extern unsigned long plword;
extern unsigned char plcls;
extern short gkcls;
extern short swslot;
extern unsigned char swcls;
extern int rbx;
extern int rby;
extern int rbx0;
extern int rby0;
extern int rbx1;
extern int rby1;
extern int rbslot;
extern unsigned short bfslot;
extern unsigned long alcls;
extern unsigned long alname;
extern int spx;
extern int spy;
extern int spw;
extern int sph;
extern int spbx;
extern int spby;
extern int spbw;
extern int spbh;
extern unsigned short sptype;
extern int spcls;
extern int spname;
extern int spslot;
extern int spk;
extern int sppass;
extern int spx0;
extern int spy0;
extern int spx1;
extern int spy1;
extern unsigned short toslot;
extern unsigned long toitem;
extern int ctslot;
extern int ctn;
extern unsigned long cttype;
extern int tkn;
extern int tkslot;
extern unsigned long tkitem;
extern short prslot;
extern short prnext;
extern short raslot;
extern unsigned long raitem;
extern unsigned short rapos;
extern int slslot;
extern int slx;
extern int sly;
extern int slpad;
extern int slfirst;
extern int slx0;
extern int slx1;
extern int sly0;
extern int sly1;
extern unsigned short iaslot;
/* itemweight's type (then realm), weight, and coins: one array, whose parts stay in memory */
extern unsigned short iw[3];
extern unsigned short iwslot;
extern unsigned char iwcls;
extern unsigned short ivtype;
extern short rvslot;
extern unsigned long rvsum;
extern int eislot;
extern unsigned long nfvol;
extern int weslot;
extern int werealm;
extern unsigned long wesum;
extern unsigned char wecls;
/* paycoins's slot, coins of the item, and realm: one array, whose parts stay in memory */
extern int pc[3];
extern short iprealm;
extern short ipslot;
extern short cprealm;
extern short cpslot;
extern int pvslot;
extern int pvrealm;
extern unsigned long pvsum;
extern unsigned short pfcls;
extern short fhrealm;
extern short fhslot;
extern unsigned long fhitem;
extern unsigned long fhsum;
extern unsigned long wsitem;
extern unsigned long *pgp;
extern unsigned long tfitem;
extern unsigned long *tfp;
extern unsigned char tfcls;
extern unsigned long poold;
extern short pomob;
extern short posecond;
extern unsigned long gaitem;
extern unsigned short ttslot;
extern unsigned long tttorch;
extern unsigned long *ttp;
extern short btk;
extern unsigned short btidx;
extern unsigned long btshield;
extern short btrealm;
extern unsigned long bnleft;
extern unsigned long agidx;
extern short frk;
extern short frx;
extern short fry;
extern short fdk;
extern int looktype;
extern int dpoffx;
extern int dpoffy;
extern int dpx;
extern int dpy;
extern int dplastx;
extern int dplasty;
extern int ordx;
extern int ordy;
extern unsigned char orcls;
extern short rmroll;
extern unsigned long buk;
extern unsigned long corpseunit;
extern unsigned char ocpanel;
extern unsigned char akres;
extern char akedge;
extern char aksave;
extern int akret;
extern short akmode;
extern unsigned char pjcraft;
extern short pjjob;
extern short pdk;
extern short pdi;
extern short pdj;
extern short pdneed;
extern short pdregion;
extern short pdcount;
extern unsigned long pditem;
extern unsigned short aeslot;
extern short aemode;
extern unsigned long aeitem;
extern int aeate;
extern short hlunit;
extern unsigned char nhhunger;
extern unsigned char nhtired;
extern unsigned char cursestruck;
extern short cllevel;
extern short clamb;
extern short clwarm;
extern short cladd;
extern short clnear;
extern short clboth;
extern short clcap;
extern short exitk;
extern short exitx;
extern short exity;
extern int exitcell;
extern short nestep;
extern int necell;
extern short idregion;
extern short idstep;
extern short zbk;
extern short mzk;
extern unsigned long mcn;
extern short mck;
extern unsigned char *mcp;
extern unsigned long szhours;
extern int szx;
extern int szy;
extern unsigned short lbidx;
extern unsigned short lbcell;
extern unsigned long tvtime;
extern unsigned long tvk;
extern short tmzone;
extern short tmk;
extern short tmsel;
extern short tmx;
extern short tmy;
extern short tmdx;
extern short tmdy;
extern unsigned long tmtime;
extern unsigned char *tmdraw;
extern short mvk;
extern char trademode;
extern short gearmsg;
extern int memguard4;
extern int memguard5;
extern unsigned char boxx0[50];
extern unsigned char boxy0[50];
extern unsigned char boxx1[50];
extern unsigned char boxy1[50];
extern unsigned char blockmax;
extern unsigned char blockn;
extern unsigned char spreadk;
extern unsigned char spreadrow;
extern unsigned char *spreadp;
extern unsigned long *spreadmap;
extern unsigned long *lookflags;
extern short lookcol;
extern short lookrow;
extern short lookx;
extern short looky;
extern unsigned char *lookp;
extern short lookdist;
extern int lookcell;
extern unsigned char savtkind;
extern int savtitem;
extern int savtidx;
extern unsigned char savhkind;
extern short tmmode;
extern unsigned char tmzones[30];
extern short tmmap;
extern short tmmarkx;
extern short tmmarky;
extern unsigned char tmframe;
extern unsigned char tmface;
extern unsigned char tmhere;

int
onscreen(int x, int y)
{
	if (x < playerx - 7 || x > playerx + 7 || y < playery - 5 || y > playery + 5)
		return 0;
	return 1;
}

unsigned char
sightat(int x, int y)
{
	sacol = x - playerx;
	sarow = y - playery;
	sacol += 9;
	sarow += 8;
	if (sacol < 0 || sacol >= 19 || sarow < 0 || sarow >= 17)
		return 0;
	if (showall)
		return 1;
	return viewgrid[sarow][sacol];
}

int
cansee(int x, int y)
{
	switch (sightat(x, y)) {
	case 1:
	case 4:
	case 0x40:
		return 1;
	case 2:
		if ((csitem = topitem(y * 150 + x)) == 0)
			return 0;
		csitem = mapitems[csitem];
		cscls = classof(csitem & 0xfff);
		if (cscls == 1 || cscls == 2 || cscls == 3 || cscls == 4)
			return 1;
		return 0;
	default:
		return 0;
	}
}

int
inview(int x, int y)
{
	if (abs(x - playerx) > 6 || abs(y - playery) > 5)
		return 0;
	return cansee(x, y);
}

int
ismassive(int item)
{
	switch (item & 0xfff) {
	case 0x2c:
	case 0x30:
	case 0x39:
	case 0x3a:
	case 0x3b:
	case 0x3d:
	case 0x3e:
	case 0x3f:
	case 0x93:
		return 1;
	}
	return 0;
}

short
isportcullis(int item)
{
	if (classof(item & 0xfff) == 4)
		return 1;
	return 0;
}

int
opendoor(unsigned long pos, short force)
{
	odidx = topitem(pos);
	oditem = mapitems[odidx];
	if (oditem & 0x10000000)
		return 2;
	odk = classof(oditem & 0xfff);
	if (odk != 1 && odk != 4)
		goto fail;
	if (oditem & 0x8000000) {
		if (wearof(oditem) >= 7 || force)
			oditem &= ~0x8000000;
		else
			goto fail;
	}
	if ((oditem & 0xfff) == 0x93) {
		if (!(mapitems[odidx] & 0x8000) || !(mapitems[odidx + 1] & 0x2000))
			goto fail;
		mapitems[odidx] |= 0x10000000;
		if ((mapitems[odidx + 1] & 0xfff) == 0x90) {
			mapitems[odidx + 1] &= 0xfffff000;
			mapitems[odidx + 1] |= 0x92;
			popitem(pos - 1, 0);
			additem(pos - 1, 0x91);
		} else {
			mapitems[odidx + 1] &= 0xfffff000;
			mapitems[odidx + 1] |= 0x97;
			popitem(pos - 150, 0);
			additem(pos - 150, 0x96);
		}
	} else {
		for (odk = 0; odk < 4; odk++)
			if ((unsigned short)(oditem & 0xfff) == ((Door (*)[4])doors)[0][odk].closed2) {
				oditem &= 0xffff7000;
				oditem |= ((Door (*)[4])doors)[0][odk].open2 + 0x10000000;
				popitem(pos - 1, 0);
				popitem(pos, 0);
				additem(pos - 1, ((Door (*)[4])doors)[0][odk].open1);
				additem(pos, oditem);
				goto done;
			}
		for (odk = 0; odk < 4; odk++)
			if ((unsigned short)(oditem & 0xfff) == ((Door (*)[4])doors)[1][odk].closed2) {
				oditem &= 0xffff7000;
				oditem |= ((Door (*)[4])doors)[1][odk].open2 + 0x10000000;
				popitem(pos - 150, 0);
				popitem(pos, 0);
				additem(pos - 150, ((Door (*)[4])doors)[1][odk].open1);
				additem(pos, oditem);
				goto done;
			}
		goto fail;
	}
done:
	updatecell(pos);
	if (inview(pos % 150, pos / 150)) {
		if (isportcullis(oditem))
			queuesound(0x17);
		// FIXED: the sound comes from the door's column times 10 over its row, and a door on the map's top row divides by 0. Take the sound of an even quotient there.
#ifdef FEATURE_FAULTS
		else if (pos / 150 != 0 && (pos % 150 * 10 / (pos / 150) & 1))
#else
		else if (pos % 150 * 10 / (pos / 150) & 1)
#endif
			queuesound(0);
		else
			queuesound(0x4a);
	}
	return 1;
fail:
	updatecell(pos);
	return 0;
}

int
closedoor(unsigned long pos, short mode)
{
	cdidx = topitem(pos);
	cditem = mapitems[cdidx];
	if (cditem & 0x10000000) {
		if ((cditem & 0xfff) == 0x93) {
			if (!(mapitems[cdidx] & 0x8000) || !(mapitems[cdidx + 1] & 0x2000))
				goto fail;
			mapitems[cdidx] &= ~0x10000000;
			if ((mapitems[cdidx + 1] & 0xfff) == 0x92) {
				mapitems[cdidx + 1] &= 0xfffff000;
				mapitems[cdidx + 1] |= 0x90;
				popitem(pos - 1, 0);
				additem(pos - 1, 0x8f);
			} else {
				mapitems[cdidx + 1] &= 0xfffff000;
				mapitems[cdidx + 1] |= 0x95;
				popitem(pos - 150, 0);
				additem(pos - 150, 0x94);
			}
			goto done;
		}
		for (cdk = 0; cdk < 4; cdk++)
			if (((unsigned short)cditem & 0xfff) == ((Door (*)[4])doors)[0][cdk].open2) {
				cditem &= 0xefff7000;
				cditem |= ((Door (*)[4])doors)[0][cdk].closed2;
				if (mode == 2 && !(wearof(cditem) >= 7))
					cditem |= 0x8000000;
				popitem(pos - 1, 0);
				popitem(pos, 0);
				additem(pos - 1, ((Door (*)[4])doors)[0][cdk].closed1);
				additem(pos, cditem);
				goto done;
			}
		for (cdk = 0; cdk < 4; cdk++)
			if (((unsigned short)cditem & 0xfff) == ((Door (*)[4])doors)[1][cdk].open2) {
				cditem &= 0xefff7000;
				cditem |= ((Door (*)[4])doors)[1][cdk].closed2;
				if (mode == 2 && !(wearof(cditem) >= 7))
					cditem |= 0x8000000;
				popitem(pos - 150, 0);
				popitem(pos, 0);
				additem(pos - 150, ((Door (*)[4])doors)[1][cdk].closed1);
				additem(pos, cditem);
				goto done;
			}
	}
	if (mode == 2) {
		if (wearof(cditem) < 7)
			mapitems[topitem(pos)] |= 0x8000000;
	} else
		mapitems[topitem(pos)] &= ~0x8000000;
fail:
	updatecell(pos);
	return 0;
done:
	updatecell(pos);
	if (inview(pos % 150, pos / 150)) {
		if (isportcullis(cditem))
			queuesound(0x17);
		else
			queuesound(1);
	}
	return 1;
}

int
baddoor(int pos)
{
	if (pos == playercell || mapflags[pos] & 4)
		return 3;
	if ((bdidx = topitem(pos)) == 0)
		return 4;
	bdcls = classof(mapitems[bdidx] & 0xfff);
	if (bdcls != 1 && bdcls != 4)
		return 2;
	return 0;
}

int
pulllever(unsigned long *item)
{
	plcell = *item >> 16;
	if (plcell == playercell || mapflags[plcell] & 4)
		return 3;
	if ((plidx = topitem(plcell)) == 0)
		return 0;
	plcls = classof(mapitems[plidx] & 0xfff);
	if (plcls != 1 && plcls != 4)
		return 2;
	if ((mapitems[plidx] & 0x8000000) == 0)
		plword = closedoor(plcell, 2);
	else
		plword = opendoor(plcell, 1);
	if (plword) {
		plword = *item & 0xfff;
		*item &= 0xfffff000;
		switch (plword) {
		case 0x179:
			plword = 0x17a;
			break;
		case 0x17a:
			plword = 0x179;
			break;
		case 0x17b:
			plword = 0x17c;
			break;
		case 0x17c:
			plword = 0x17b;
			break;
		}
		*item |= plword;
	}
	return 1;
}

unsigned long
itemkey(unsigned long item)
{
	item = baselook(item);
	item &= 0xffff0fff;
	switch (classof(item & 0xfff)) {
	case 0x3c:
	case 0x41:
	case 0x46:
	case 0x48:
	case 0x4b:
	case 0x50:
	case 0x55:
		item &= 0x1ffffff;
		break;
	case 0x62:
	case 0x63:
		item &= 0x7fffffff;
		break;
	}
	return item;
}

unsigned long
loosekey(unsigned long item)
{
	item = baselook(item);
	item &= 0xffff0fff;
	switch (classof(item & 0xfff)) {
	case 0x3c:
	case 0x41:
	case 0x46:
	case 0x48:
	case 0x4b:
	case 0x50:
	case 0x55:
		item &= 0x1fffff;
		item |= 0xe00000;
		break;
	case 0x62:
	case 0x63:
		item &= 0xffff;
		break;
	}
	return item;
}

unsigned long
typekey(unsigned long item)
{
	return baselook(item) & 0xfff;
}

unsigned long
groupkey(unsigned long item)
{
	if ((gkcls = classof(item & 0xfff)) >= 0x87 && gkcls <= 0x8b)
		return 0x89;
	switch (gkcls) {
	case 0x36:
		return 0x37;
	case 0x38:
		return 0x39;
	case 0x41:
		return 0x55;
	case 0x20:
	case 0x3a:
	case 0x56:
		return 0x21;
	case 0x46:
		if (isarmor(item))
			return gkcls;
	case 0x4b:
	case 0x50:
		return 0x48;
	case 0x4a:
	case 0x67:
	case 0x68:
		return 0x49;
	case 0x62:
	case 0x63:
		return 0x64;
	}
	return gkcls;
}

int
panelrealm(int n)
{
	if (n >= 0x2b && n <= 0x30)
		return panels[n - 0x28].id;
	return 0;
}

int
realmfull(unsigned short n)
{
	if (realms[n].items[49])
		return 1;
	return 0;
}

int
stowat(unsigned short k, unsigned short n, unsigned long item)
{
	if (item & 0xfff) {
		if (!(realms[k].item & 0x10000000)) {
			swcls = classof((unsigned short)(item & 0xfff));
			if ((swcls == 0xf || swcls == 0x10) && item >> 16 < 15000) {
				for (swslot = 0; swslot < 50; swslot++)
					if (canstack(item, realms[k].items[swslot])
					    && addcoins(&item, realms[k].items[swslot])) {
						n = realms[k].pos[swslot];
						goto shift;
					}
			}
		}
		if (realmfull(k))
			return 0;
		swslot = 49;
shift:
		for (; swslot; swslot--) {
			realms[k].items[swslot] = realms[k].items[swslot - 1];
			realms[k].pos[swslot] = realms[k].pos[swslot - 1];
		}
		/*
		 * swslot is 0 already: the compiler drops this store, but it
		 * keeps swslot in di and n on the stack, as in the original
		 */
		swslot = 0;
		if (islittorch(item))
			setlit(&item, 0);
		realms[k].items[swslot] = item;
		realms[k].pos[swslot] = n;
	}
	return 1;
}

void
realmboxes(short k)
{
	for (rbslot = 0; realms[k].items[rbslot] != 0 && rbslot < 50; rbslot++) {
		gettileo(realms[k].items[rbslot] & 0xfff);
		tilebox(&rbx0, &rby0, &rbx1, &rby1);
		rbx = realms[k].pos[rbslot] % 320;
		rbx0 += rbx;
		if (rbx0)
			rbx0--;
		if (rbx0)
			rbx0--;
		rbx1 += rbx;
		rbx1 += 2;
		rby = realms[k].pos[rbslot] / 320;
		rby0 += rby;
		if (rby0)
			rby0--;
		if (rby0)
			rby0--;
		rby1 += rby;
		boxx0[rbslot] = rbx0;
		boxy0[rbslot] = rby0;
		boxx1[rbslot] = rbx1;
		rby1 += 2;
		boxy1[rbslot] = rby1;
	}
	for (; rbslot < 50; rbslot++) {
		boxx0[rbslot] = 0;
		boxy0[rbslot] = 0;
		boxx1[rbslot] = 0;
		boxy1[rbslot] = 0;
	}
}

short
boxfree(short k, unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1)
{
	for (bfslot = 0; realms[k].items[bfslot] != 0 && bfslot < 50; bfslot++)
		if ((x0 >= boxx0[bfslot] && x0 <= boxx1[bfslot]
		    || x1 >= boxx0[bfslot] && x1 <= boxx1[bfslot]
		    || x0 <= boxx0[bfslot] && x1 >= boxx1[bfslot])
		    && (y0 >= boxy0[bfslot] && y0 <= boxy1[bfslot]
		    || y1 >= boxy0[bfslot] && y1 <= boxy1[bfslot]
		    || y0 <= boxy0[bfslot] && y1 >= boxy1[bfslot]))
			return 0;
	return 1;
}

int
alike(unsigned long item, unsigned long cls, unsigned long sub)
{
	alcls = classof(item & 0xfff);
	alname = typenames[item & 0xfff];
	if (cls == alcls && sub && sub == alname)
		return 2;
	if (cls == alcls)
		return 1;
	switch (alcls) {
	case 0x36:
	case 0x37:
		switch (cls) {
		case 0x36:
		case 0x37:
			if (sub && sub == alname)
				return 2;
			return 1;
		}
		return 0;
	case 0x3a:
	case 0x87:
	case 0x88:
	case 0x89:
	case 0x8a:
	case 0x8b:
		switch (cls) {
		case 0x3a:
		case 0x87:
		case 0x88:
		case 0x89:
		case 0x8a:
		case 0x8b:
			return 1;
		}
		return 0;
	case 0x41:
	case 0x46:
	case 0x48:
	case 0x4b:
	case 0x50:
		switch (cls) {
		case 0x41:
		case 0x46:
		case 0x48:
		case 0x4b:
		case 0x50:
			return 1;
		}
		return 0;
	case 0x56:
	case 0x59:
	case 0x5a:
		switch (cls) {
		case 0x56:
		case 0x59:
		case 0x5a:
			return 1;
		}
		return 0;
	case 0xf:
	case 0x10:
	case 0x62:
	case 0x63:
	case 0x64:
		switch (cls) {
		case 0xf:
		case 0x10:
		case 0x62:
		case 0x63:
		case 0x64:
			return 1;
		}
		return 0;
	case 0x38:
	case 0x39:
	case 0xcc:
		switch (cls) {
		case 0x38:
		case 0x39:
		case 0xcc:
			return 1;
		}
		return 0;
	case 0xc8:
	case 0xc9:
	case 0xcb:
	case 0xd0:
	case 0xd1:
		switch (cls) {
		case 0xc8:
		case 0xc9:
		case 0xcb:
		case 0xd0:
		case 0xd1:
			return 1;
		}
		return 0;
	}
	return 0;
}

int
stowspot(int k, int x, int y, unsigned long item)
{
	/*
	 * unused: a temporary of the code generator, which gives
	 * packrealm the original's registers
	 */
	int u0;

	sptype = realms[k].item & 0xfff;
	if (item == 0) {
		spbx = 0;
		spby = 0;
		spbw = 0x30;
		spbh = 0x30;
	} else {
		gettileo(item);
		tilebox(&spbx, &spby, &spbw, &spbh);
		spbw -= spbx;
		spbh -= spby;
	}
	if ((sptype & 0xfff) == 0x2e4) {
		spx = 0x1e;
		spy = 0x1e;
		spw = 0x8c - spbw;
		sph = 0xc3 - spbh;
	} else {
		switch (typenames[sptype]) {
		case 0x1c:
			spx = 0x1e;
			spy = 0x32;
			spw = 0xb0 - spbw;
			sph = 0x64 - spbh;
			break;
		case 0x1d:
			spx = 0x30;
			spy = 0x20;
			spw = 0xb8 - spbw;
			sph = 0x4c - spbh;
			break;
		case 0x1b:
			spx = 0x18;
			spy = 0x16;
			spw = 0x6c - spbw;
			sph = 0x5e - spbh;
			break;
		default:
			spx = 0x28;
			spy = 0x20;
			spw = 0x74 - spbw;
			sph = 0x70 - spbh;
			break;
		}
	}
	if (x == 0xff && y == 0xff) {
		if (k != 0xff) {
			realmboxes(k);
			if (k == 0) {
				spcls = classof(item & 0xfff);
				spname = typenames[item & 0xfff];
				if (spcls) {
					sppass = 0;
					if (spname == 0)
						sppass = 1;
					for (; sppass < 2; sppass++) {
						spslot = realms[k].items[1] ? 1 : 0;
						for (;;) {
							spk = alike(realms[k].items[spslot],
							    spcls, spname);
							if (spk == 2) {
								x = boxx0[spslot] - 10 + randn(20) - spbx;
								y = boxy0[spslot] - 10 + randn(20) - spby;
								if (x < spx - spbx)
									x = spx - spbx;
								else if (x > spx - spbx + spw)
									x = spx - spbx + spw;
								if (y < spy - spby)
									y = spy - spby;
								else if (y > spy - spby + sph)
									y = spy - spby + sph;
								return x + y * 320;
							}
							if (spk == 1 && sppass) {
								spk = 16;
								x = (boxx1[spslot] - boxx0[spslot]) / 2
								    + boxx0[spslot] - spbx;
								y = (boxy1[spslot] - boxy0[spslot]) / 2
								    + boxy0[spslot] - spby;
								spx0 = x - spk;
								if (spx0 < spx - spbx)
									spx0 = spx - spbx;
								spy0 = y - spk;
								if (spy0 < spy - spby)
									spy0 = spy - spby;
								spx1 = x + spk;
								if (spx1 > spx - spbx + spw)
									spx1 = spx - spbx + spw;
								spy1 = y + spk;
								if (spy1 > spy - spby + sph)
									spy1 = spy - spby + sph;
								for (spk = 0; spk < 20; spk++) {
									x = spx0 + randn(spx1 + 1 - spx0);
									y = spy0 + randn(spy1 + 1 - spy0);
									if (boxfree(k, x + spbx, y + spby,
									    x + spbx + spbw - 1, y + spby + spbh - 1))
										return x + y * 320;
								}
								for (spk = 1; spk < 72; spk++) {
									x = (boxx1[spslot] - boxx0[spslot]) / 2
									    + boxx0[spslot] - spbx;
									y = (boxy1[spslot] - boxy0[spslot]) / 2
									    + boxy0[spslot] - spby;
									spx0 = x - spk;
									if (spx0 < spx - spbx)
										spx0 = spx - spbx;
									spy0 = y - spk;
									if (spy0 < spy - spby)
										spy0 = spy - spby;
									spx1 = x + spk;
									if (spx1 > spx - spbx + spw)
										spx1 = spx - spbx + spw;
									spy1 = y + spk;
									if (spy1 > spy - spby + sph)
										spy1 = spy - spby + sph;
									for (y = spy0; y <= spy1; y++)
										for (x = spx0; x <= spx1; x++)
											if (boxfree(k, x + spbx, y + spby,
											    x + spbx + spbw - 1, y + spby + spbh - 1))
												return x + y * 320;
								}
							}
							if (spslot == 0 && realms[k].items[1])
								break;
							spslot++;
							if (realms[k].items[spslot]) {
								if (spslot >= 50)
									spslot = 0;
							} else {
								if (spslot == 1)
									break;
								spslot = 0;
							}
						}
					}
				}
				if (randn(100) < 50) {
					// FIXED: the bound of y subtracts spbx, the tile's x offset, where its y offset spby is meant, as in each other bound. Subtract spby.
#if defined(FEATURE_FIXES)
					for (y = 0; y < sph / 2 - spbh - spby; y++)
#else
					for (y = 0; y < sph / 2 - spbh - spbx; y++)
#endif
						for (x = 0; x < spw / 2 - spbw - spbx; x++) {
							if (boxfree(k, spw / 2 + spx + x + spbx,
							    sph / 2 + spy + y + spby,
							    spw / 2 + spx + x + spbx + spbw - 1,
							    sph / 2 + spy + y + spby + spbh - 1))
								return spw / 2 + spx + x
								    + (sph / 2 + spy + y) * 320;
							if (boxfree(k, spw / 2 + spx - x + spbx,
							    sph / 2 + spy + y + spby,
							    spw / 2 + spx - x + spbx + spbw - 1,
							    sph / 2 + spy + y + spby + spbh - 1))
								return spw / 2 + spx - x
								    + (sph / 2 + spy + y) * 320;
							if (boxfree(k, spw / 2 + spx + x + spbx,
							    sph / 2 + spy - y + spby,
							    spw / 2 + spx + x + spbx + spbw - 1,
							    sph / 2 + spy - y + spby + spbh - 1))
								return spw / 2 + spx + x
								    + (sph / 2 + spy - y) * 320;
							if (boxfree(k, spw / 2 + spx - x + spbx,
							    sph / 2 + spy - y + spby,
							    spw / 2 + spx - x + spbx + spbw - 1,
							    sph / 2 + spy - y + spby + spbh - 1))
								return spw / 2 + spx - x
								    + (sph / 2 + spy - y) * 320;
						}
				} else {
					for (x = 0; x < spw / 2 - spbw - spbx; x++)
						// FIXED: as above, subtract spby.
#if defined(FEATURE_FIXES)
						for (y = 0; y < sph / 2 - spbh - spby; y++) {
#else
						for (y = 0; y < sph / 2 - spbh - spbx; y++) {
#endif
							if (boxfree(k, spw / 2 + spx + x + spbx,
							    sph / 2 + spy - y + spby,
							    spw / 2 + spx + x + spbx + spbw - 1,
							    sph / 2 + spy - y + spby + spbh - 1))
								return spw / 2 + spx + x
								    + (sph / 2 + spy - y) * 320;
							if (boxfree(k, spw / 2 + spx - x + spbx,
							    sph / 2 + spy - y + spby,
							    spw / 2 + spx - x + spbx + spbw - 1,
							    sph / 2 + spy - y + spby + spbh - 1))
								return spw / 2 + spx - x
								    + (sph / 2 + spy - y) * 320;
							if (boxfree(k, spw / 2 + spx + x + spbx,
							    sph / 2 + spy + y + spby,
							    spw / 2 + spx + x + spbx + spbw - 1,
							    sph / 2 + spy + y + spby + spbh - 1))
								return spw / 2 + spx + x
								    + (sph / 2 + spy + y) * 320;
							if (boxfree(k, spw / 2 + spx - x + spbx,
							    sph / 2 + spy + y + spby,
							    spw / 2 + spx - x + spbx + spbw - 1,
							    sph / 2 + spy + y + spby + spbh - 1))
								return spw / 2 + spx - x
								    + (sph / 2 + spy + y) * 320;
						}
				}
				for (y = spy - spby; y <= spy - spby + sph; y++)
					for (x = spx - spbx; x <= spx - spbx + spw; x++)
						if (boxfree(k, x + spbx, y + spby,
						    x + spbx + spbw - 1, y + spby + spbh - 1))
							return x + y * 320;
			} else {
				if (randn(100) < 50) {
					// FIXED: the bound of y subtracts spbx, the tile's x offset, where its y offset spby is meant, as in each other bound. Subtract spby.
#if defined(FEATURE_FIXES)
					for (y = 0; y < sph / 2 - spbh - spby; y++)
#else
					for (y = 0; y < sph / 2 - spbh - spbx; y++)
#endif
						for (x = 0; x < spw / 2 - spbw - spbx; x++) {
							if (boxfree(k, spw / 2 + spx + x + spbx,
							    sph / 2 + spy + y + spby,
							    spw / 2 + spx + x + spbx + spbw - 1,
							    sph / 2 + spy + y + spby + spbh - 1))
								return spw / 2 + spx + x
								    + (sph / 2 + spy + y) * 320;
							if (boxfree(k, spw / 2 + spx - x + spbx,
							    sph / 2 + spy + y + spby,
							    spw / 2 + spx - x + spbx + spbw - 1,
							    sph / 2 + spy + y + spby + spbh - 1))
								return spw / 2 + spx - x
								    + (sph / 2 + spy + y) * 320;
							if (boxfree(k, spw / 2 + spx + x + spbx,
							    sph / 2 + spy - y + spby,
							    spw / 2 + spx + x + spbx + spbw - 1,
							    sph / 2 + spy - y + spby + spbh - 1))
								return spw / 2 + spx + x
								    + (sph / 2 + spy - y) * 320;
							if (boxfree(k, spw / 2 + spx - x + spbx,
							    sph / 2 + spy - y + spby,
							    spw / 2 + spx - x + spbx + spbw - 1,
							    sph / 2 + spy - y + spby + spbh - 1))
								return spw / 2 + spx - x
								    + (sph / 2 + spy - y) * 320;
						}
				} else {
					for (x = 0; x < spw / 2 - spbw - spbx; x++)
						// FIXED: as above, subtract spby.
#if defined(FEATURE_FIXES)
						for (y = 0; y < sph / 2 - spbh - spby; y++) {
#else
						for (y = 0; y < sph / 2 - spbh - spbx; y++) {
#endif
							if (boxfree(k, spw / 2 + spx + x + spbx,
							    sph / 2 + spy - y + spby,
							    spw / 2 + spx + x + spbx + spbw - 1,
							    sph / 2 + spy - y + spby + spbh - 1))
								return spw / 2 + spx + x
								    + (sph / 2 + spy - y) * 320;
							if (boxfree(k, spw / 2 + spx - x + spbx,
							    sph / 2 + spy - y + spby,
							    spw / 2 + spx - x + spbx + spbw - 1,
							    sph / 2 + spy - y + spby + spbh - 1))
								return spw / 2 + spx - x
								    + (sph / 2 + spy - y) * 320;
							if (boxfree(k, spw / 2 + spx + x + spbx,
							    sph / 2 + spy + y + spby,
							    spw / 2 + spx + x + spbx + spbw - 1,
							    sph / 2 + spy + y + spby + spbh - 1))
								return spw / 2 + spx + x
								    + (sph / 2 + spy + y) * 320;
							if (boxfree(k, spw / 2 + spx - x + spbx,
							    sph / 2 + spy + y + spby,
							    spw / 2 + spx - x + spbx + spbw - 1,
							    sph / 2 + spy + y + spby + spbh - 1))
								return spw / 2 + spx - x
								    + (sph / 2 + spy + y) * 320;
						}
				}
			}
			for (y = spy - spby + sph; y >= spy - spby; y--)
				for (x = spx - spbx + spw; x >= spx - spbx; x--)
					if (boxfree(k, x + spbx, y + spby,
					    x + spbx + spbw - 1, y + spby + spbh - 1))
						return x + y * 320;
		}
		return spx + randn(spw) - spbx
		    + (spy + randn(sph) - spby) * 320;
	}
	if (spbx + x < spx)
		x = spx - spbx;
	else if (spbx + x > spx + spw)
		x = spx - spbx + spw;
	if (spby + y < spy)
		y = spy - spby;
	else if (spby + y > spy + sph)
		y = spy - spby + sph;
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	return x + y * 320;
}

int
stow(int k, unsigned long item)
{
	return stowat(k, stowspot(k, 0xff, 0xff, item & 0xfff), item);
}

unsigned long
takeout(unsigned short k, unsigned short n)
{
	toitem = realms[k].items[n];
	for (toslot = n; toslot < 49; toslot++) {
		realms[k].items[toslot] = realms[k].items[toslot + 1];
		realms[k].pos[toslot] = realms[k].pos[toslot + 1];
	}
	realms[k].items[49] = 0;
	realms[k].pos[49] = 0;
	return toitem;
}

int
counttypes(unsigned short k, unsigned long a, unsigned long b, unsigned long c)
{
	a &= 0xfff;
	b &= 0xfff;
	c &= 0xfff;
	ctn = 0;
	for (ctslot = 0; ctslot < 50; ctslot++) {
		cttype = realms[k].items[ctslot] & 0xfff;
		if (a && cttype == a)
			ctn++;
		else if (b && cttype == b)
			ctn++;
		else if (c && cttype == c)
			ctn++;
	}
	return ctn;
}

int
taketypes(int k, unsigned long a, unsigned long b, unsigned long c, int n, int who)
{
	a &= 0xfff;
	b &= 0xfff;
	c &= 0xfff;
	tkn = 0;
	for (;;) {
		for (tkslot = 0; tkslot < 50; tkslot++) {
			tkitem = loosekey(realms[k].items[tkslot]) & 0xfff;
			if (tkitem == a || tkitem == b || tkitem == c)
				goto found;
		}
		// FIXED: the search runs on into realm 5, the first container's, past the party's realms 0 to 4. Stop at 4, as counttypes and wealth do.
#if defined(FEATURE_FIXES)
		if (k >= 4)
#else
		if (k >= 5)
#endif
			break;
		k++;
		continue;
	found:
		tkitem = takeout(k, tkslot);
		if (who >= 0)
			giveitem(who, tkitem, 1);
		if (++tkn >= n)
			return tkn;
	}
	return tkn;
}

void
packrealm(unsigned short k)
{
	for (prslot = 0; prslot < 49; prslot++)
		if ((realms[k].items[prslot] & 0xfff) == 0)
			for (prnext = prslot + 1; prnext < 50; prnext++)
				if (realms[k].items[prnext] & 0xfff) {
					do
						takeout(k, prslot);
					while ((realms[k].items[prslot] & 0xfff) == 0);
					break;
				}
}

void
raiseitem(unsigned short k, unsigned short n)
{
	raitem = realms[k].items[n];
	rapos = realms[k].pos[n];
	for (raslot = n; raslot >= 1; raslot--) {
		realms[k].items[raslot] = realms[k].items[raslot - 1];
		realms[k].pos[raslot] = realms[k].pos[raslot - 1];
	}
	realms[k].items[0] = raitem;
	realms[k].pos[0] = rapos;
}

short
slotat(unsigned short k, unsigned short x, unsigned short y)
{
	slfirst = 999;
	for (slpad = 0; slfirst == 999 && slpad < 4; slpad++)
		for (slslot = 0; slslot < 50; slslot++) {
			if (realms[k].items[slslot] == 0)
				break;
			slx = realms[k].pos[slslot];
			sly = slx / 320;
			slx -= sly * 320;
			gettileo(realms[k].items[slslot] & 0xfff);
			tilebox(&slx0, &sly0, &slx1, &sly1);
			if (y >= sly + sly0 - slpad && y <= sly + sly1 + slpad
			    && x >= slx + slx0 - slpad && x <= slx + slx1 + slpad) {
				slx = x - slx;
				sly = y - sly;
				if (tilepix(slx, sly) != 0)
					return slslot;
				if (slfirst == 999)
					slfirst = slslot;
			}
		}
	return slfirst;
}

unsigned long
itemat(unsigned short k, unsigned short x, unsigned short y)
{
	if ((iaslot = slotat(k, x, y)) == 999)
		return 0;
	return realms[k].items[iaslot];
}

unsigned short
itemweight(unsigned long item)
{
	if (item == 0)
		return 0;
	iw[0] = item & 0xfff;
	switch (classof(iw[0])) {
	case 15:
		iw[1] = (unsigned short)(item >> 16) * 4 / 10;
		break;
	case 16:
		iw[1] = (unsigned short)(item >> 16) * 8 / 10;
		break;
	case 10:
		iw[1] = weightof(iw[0]);
		if (iw[1] > 65000)
			break;
		iw[0] = idof(item);
		// FIXED: the sum of a container's items runs in an unsigned short, so a load past 65,535 wraps to a light one. Sum in a long, up to 65,000, the weight that nothing moves.
#if defined(FEATURE_FIXES)
		{
			unsigned long w = iw[1];

			for (iwslot = 0; iwslot < 50; iwslot++) {
				item = realms[iw[0]].items[iwslot];
				if (item == 0)
					continue;
				iwcls = classof(item & 0xfff);
				if (iwcls == 15)
					w += (item >> 16) * 4 / 10;
				else if (iwcls == 16)
					w += (item >> 16) * 8 / 10;
				else
					w += weightof(item & 0xfff);
			}
			iw[1] = min(w, 65000);
		}
#else
		for (iwslot = 0; iwslot < 50; iwslot++) {
			item = realms[iw[0]].items[iwslot];
			if (item == 0)
				continue;
			iwcls = classof(item & 0xfff);
			if (iwcls == 15)
				iw[1] += (iw[2] = item >> 16) * 4 / 10;
			else if (iwcls == 16)
				iw[1] += (iw[2] = item >> 16) * 8 / 10;
			else
				iw[1] += weightof(item & 0xfff);
		}
#endif
		break;
	default:
		iw[1] = weightof(iw[0]);
		break;
	}
	return iw[1];
}

unsigned long
itemvolume(unsigned long item)
{
	if (item == 0)
		return 0;
	ivtype = item & 0xfff;
	switch (classof(ivtype)) {
	case 15:
		return (item >> 16) / 20;
	case 16:
		return (item >> 16) / 10;
	}
	return volumes[ivtype];
}

unsigned long
realmvolume(short k)
{
	rvsum = 0;
	for (rvslot = 0; rvslot < 50; rvslot++)
		rvsum += itemvolume(realms[k].items[rvslot]);
	return rvsum;
}

int
capacity(short k)
{
	switch (typenames[realms[k].item & 0xfff]) {
	case 0x1b:
		return 12150;
	case 0x1c:
		return 21600;
	case 0x1d:
		return 18000;
	default:
		return 8000;
	}
}

int
emptyinto(int k, int a, int b, int c)
{
	if (a == 0 && b == 0 && c == 0)
		return 0;
	for (eislot = 49; eislot >= 0; eislot--)
		if (realms[k].items[eislot] != 0) {
			if (a && stow(a, realms[k].items[eislot])
			    || b && stow(b, realms[k].items[eislot])
			    || c && stow(c, realms[k].items[eislot]))
				realms[k].items[eislot] = 0;
			else
				return 0;
		}
	return 1;
}

short
nofit(unsigned long item, short k)
{
	nfvol = realmvolume(k) + itemvolume(item);
	if (capacity(k) < nfvol)
		return 1;
	if (itemweight(item) > 65000)
		return 1;
	if (isspear(item))
		return 1;
	return 0;
}

void
fmtgoods(char *buf, short k)
{
	short i;

	*buf = 0;
	if (!realms[k].nneeds)
		return;
	*buf++ = 'P';
	*buf++ = ':';
	for (i = 0; i < realms[k].nneeds; i++) {
		if (i)
			*buf++ = ',';
		if (realms[k].needs[i].n > 1) {
			wsprintf(buf, texts + 0xb08d, realms[k].needs[i].n);
			while (*buf)
				buf++;
		}
		wsprintf(buf, itemname(realms[k].needs[i].item & 0xfff));
		while (*buf)
			buf++;
	}
}

long
wealth(int mode)
{
	wesum = 0;
	for (werealm = 0; werealm < 5; werealm++)
		for (weslot = 0; weslot < 50; weslot++) {
			wecls = classof((unsigned short)(realms[werealm].items[weslot] & 0xfff));
			if (wecls == 15) {
				if (mode == 0 || mode == 2 || mode == 3)
					wesum += realms[werealm].items[weslot] >> 16;
			} else if (wecls == 16) {
				if (mode == 0 || mode == 3)
					wesum += (realms[werealm].items[weslot] >> 16) * 25;
				else if (mode == 1)
					wesum += realms[werealm].items[weslot] >> 16;
			} else if (mode == 3) {
				if (wecls == 0x3c || wecls == 0x46 || wecls == 0x41
				    || wecls == 0x62 || wecls == 0x63 || wecls == 0x64)
					wesum += valueof(realms[werealm].items[weslot]) / 10;
			}
		}
	if (mode == 3) {
		if (units[0].head)
			wesum += valueof(units[0].head) / 10;
		if (units[0].body)
			wesum += valueof(units[0].body) / 10;
		if (units[0].hand)
			wesum += valueof(units[0].hand) / 10;
		if (units[0].wrist)
			wesum += valueof(units[0].wrist) / 10;
		if (units[0].neck)
			wesum += valueof(units[0].neck) / 10;
	}
	return wesum;
}

int
paycoins(int type, unsigned long amount)
{
	for (pc[2] = 0; pc[2] < 5; pc[2]++) {
	again:
		for (pc[0] = 0; pc[0] < 50; pc[0]++)
			if (classof((unsigned short)(realms[pc[2]].items[pc[0]] & 0xfff)) == (type ? 16 : 15)) {
				pc[1] = realms[pc[2]].items[pc[0]] >> 16;
				if (amount >= pc[1]) {
					units[0].load -= itemweight(realms[pc[2]].items[pc[0]]);
					amount -= pc[1];
					takeout(pc[2], pc[0]);
					if (amount == 0)
						return 1;
					goto again;
				}
				units[0].load -= itemweight(realms[pc[2]].items[pc[0]]);
				/*
				 * the element's byte offset with the slot before the
				 * realm: the compiler then gives the realm's row eax and
				 * the slot edx, as in the original
				 */
				*(unsigned long *)((char *)realms[0].items + pc[0] * 4 + pc[2] * sizeof(Realm)) -= amount << 16;
				units[0].load += itemweight(*(unsigned long *)((char *)realms[0].items + pc[0] * 4 + pc[2] * sizeof(Realm)));
				addcoins(&realms[pc[2]].items[pc[0]], 0);
				return 1;
			}
	}
	return 0;
}

unsigned long
inpack(short *pk, short *pj, unsigned long item, int norm)
{
	item = norm ? loosekey(item) : itemkey(item);
	for (iprealm = 0; iprealm < 5; iprealm++)
		for (ipslot = 0; ipslot < 50; ipslot++)
			if (norm && loosekey(realms[iprealm].items[ipslot]) == item
			    || !norm && itemkey(realms[iprealm].items[ipslot]) == item) {
				if (pk)
					*pk = iprealm;
				if (pj)
					*pj = ipslot;
				return realms[iprealm].items[ipslot];
			}
	return 0;
}

unsigned long
classinpack(short *pk, short *pj, short cls)
{
	for (cprealm = 0; cprealm < 5; cprealm++)
		for (cpslot = 0; cpslot < 50; cpslot++)
			if (classof(realms[cprealm].items[cpslot] & 0xfff) == cls) {
				if (cls == 0x46 && baselook(realms[cprealm].items[cpslot] & 0xfff) == 0x1b5)
					continue;
				if (pk)
					*pk = cprealm;
				if (pj)
					*pj = cpslot;
				return realms[cprealm].items[cpslot];
			}
	return 0;
}

unsigned long
packvalue(void)
{
	pvsum = 0;
	for (pvrealm = 0; pvrealm < 5; pvrealm++)
		for (pvslot = 0; pvslot < 50; pvslot++)
			if (realms[pvrealm].items[pvslot] != 0)
				pvsum += valueof(realms[pvrealm].items[pvslot]);
	return pvsum;
}

int
plainfood(unsigned long item)
{
	pfcls = classof(item & 0xfff);
	if (pfcls == 0x37 && (item & 0xfe3f0000) == 0)
		return 1;
	if (pfcls == 0x36 && ageof(item) < 4)
		return 1;
	return 0;
}

unsigned long
foodhours(void)
{
	fhsum = 0;
	for (fhrealm = 0; fhrealm < 5; fhrealm++)
		for (fhslot = 0; fhslot < 50; fhslot++) {
			fhitem = itemkey(realms[fhrealm].items[fhslot]);
			if (plainfood(fhitem))
				fhsum += satiety(fhitem);
		}
	return fhsum;
}

int
wornslot(Unit *u, unsigned long item, int norm)
{
	if (badunit(u))
		return 0;
	item = norm ? loosekey(item) : itemkey(item);
	wsitem = u->items[0];
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 0x28;
	wsitem = u->items[1];
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 0x29;
	wsitem = u->items[2];
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 0x2a;
	wsitem = u->body;
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 0xf;
	wsitem = u->cloak;
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 3;
	wsitem = u->wrist;
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 6;
	wsitem = u->neck;
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 7;
	wsitem = u->belt;
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 0x15;
	wsitem = u->hand;
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 0x14;
	wsitem = u->offhand;
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 0xa;
	wsitem = u->head;
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 1;
	wsitem = u->legs;
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 0x19;
	wsitem = u->feet;
	if (norm && item == loosekey(wsitem) || !norm && item == itemkey(wsitem))
		return 0x1e;
	return 0;
}

unsigned long *
gearslot(Unit *u, int slot)
{
	if (badunit(u))
		u = &units[0];
	switch (slot) {
	case 3:
		return &u->cloak;
	case 1:
		return &u->head;
	case 0x14:
		return &u->hand;
	case 0x15:
		return &u->belt;
	case 0xf:
		return &u->body;
	case 6:
		return &u->wrist;
	case 7:
		return &u->neck;
	case 0xa:
		return &u->offhand;
	case 0x19:
		return &u->legs;
	case 0x1e:
		return &u->feet;
	case 0x28:
		return &u->items[0];
	case 0x29:
		return &u->items[1];
	case 0x2a:
		return &u->items[2];
	}
	return 0;
}

unsigned long
panelgear(int n, int slot)
{
	if ((pgp = gearslot(&units[panels[n - 0x28].id], slot)) == 0)
		return 0;
	return *pgp;
}

unsigned long
takeoff(int n, int slot)
{
	tfitem = 0;
	if ((tfp = gearslot(&units[n], slot)) == 0)
		return 0;
	tfitem = *tfp;
	*tfp = 0;
	if (n == 0) {
		if (!talkactive && !trademode && !(slot >= 0x28 && slot <= 0x2a)) {
			tfcls = classof(tfitem & 0xfff);
			if (tfcls == 0x41 || tfcls == 0x46 || tfcls == 0x4b
			    || tfcls == 0x50 || tfcls == 0x48 || tfcls == 0x55) {
				calcstats(&units[0]);
				immobtext(namebuf, &units[0]);
				if (namebuf[0]) {
					delmsgid(gearmsg);
					wsprintf(textbuf, strs[1303], itemname(tfitem), strs[1305], strs[243], namebuf);
					gearmsg = msgf(5, textbuf);
				}
			}
		}
		if (slot == 0x14)
			bloodtill = 0;
		dresshero();
	}
	if (slot < 0x28 || slot > 0x2a)
		tfitem = randlook(tfitem);
	return tfitem;
}

short
wearable(int item)
{
	switch (classof(item & 0xfff)) {
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
		return 1;
	default:
		return 0;
	}
}

short
unmanly(unsigned long item, short msg)
{
	item = baselook(item) & 0xfff;
	if (item == 0x1b5 || item == 0x514) {
		if (msg)
			denymsg(strs[1465]);
		return 1;
	}
	return 0;
}

unsigned long
puton(short u, unsigned long item)
{
	poold = 999;
	if (u == 0)
		item &= ~0x1000;
	if (u == 0 && unmanly(item, 0))
		return 999;
	pomob = units[u].dexterity;
	item = baselook(item);
	switch (classof(item & 0xfff)) {
	case 0x3c:
		if (isthrowing2(item)) {
			posecond = 0;
			if (!isthrowing2(units[u].hand) && units[u].belt == 0) {
				units[u].belt = units[u].hand;
				units[u].hand = 0;
			}
		} else
			posecond = isthrowing2(units[u].hand);
		if (posecond) {
			poold = takeoff(u, 0x15);
			units[u].belt = item;
		} else {
			poold = takeoff(u, 0x14);
			units[u].hand = item;
		}
		if (u == 0)
			bloodtill = 0;
		if (u == 0 && !talkactive && !trademode && units[0].strength < metals[gearof(item)].minstr)
			msgf(0x63, strs[435]);
		break;
	case 0x41:
		poold = takeoff(u, 1);
		units[u].head = item;
	msg:
		if (u == 0 && !talkactive && !trademode) {
			calcstats(&units[0]);
			immobtext(namebuf, &units[0]);
			if (namebuf[0] != 0) {
				delmsgid(gearmsg);
				wsprintf(textbuf, strs[1303], itemname(item), strs[1304],
				    poold && pomob <= units[u].dexterity ? strs[243] : empty, namebuf);
				gearmsg = msgf(5, textbuf);
			}
		}
		break;
	case 0x46:
		poold = takeoff(u, 0xf);
		units[u].body = item;
		goto msg;
	case 0x48:
		poold = takeoff(u, 3);
		units[u].cloak = item;
		goto msg;
	case 0x63:
		poold = takeoff(u, 6);
		units[u].wrist = item;
		break;
	case 0x62:
		poold = takeoff(u, 7);
		units[u].neck = item;
		break;
	case 0x4b:
		poold = takeoff(u, 0x19);
		units[u].legs = item;
		goto msg;
	case 0x50:
		poold = takeoff(u, 0x1e);
		units[u].feet = item;
		break;
	case 0x55:
		poold = takeoff(u, 0xa);
		units[u].offhand = item;
		goto msg;
	case 0x56:
		poold = takeoff(u, 0xa);
		if (u) {
			if (units[u].health > 0) {
				if (isunlittorch(item) && lastlight < 15)
					setlit(&item, 1);
				if (inview(units[u].x, units[u].y))
					queuesound(7);
			} else if (islittorch(item))
				setlit(&item, 0);
		} else if (isunlittorch(item) && lastlight < 15) {
			setlit(&item, 1);
			queuesound(7);
		}
		units[u].offhand = item;
		break;
	}
	if (u == 0)
		dresshero();
	return poold;
}

int
putonfree(unsigned short u, unsigned long item)
{
	if (u == 0)
		item &= ~0x1000;
	item = baselook(item);
	if (u == 0 && unmanly(item, 0))
		return 999;
	switch (classof(item & 0xfff)) {
	case 0x3c:
		if (units[u].hand)
			return 999;
		units[u].hand = item;
		if (u == 0) {
			bloodtill = 0;
			dresshero();
		}
		return 1;
	case 0x41:
		if (units[u].head)
			return 999;
		units[u].head = item;
		if (u == 0)
			dresshero();
		return 1;
	case 0x46:
		if (units[u].body)
			return 999;
		units[u].body = item;
		if (u == 0)
			dresshero();
		return 1;
	case 0x48:
		if (units[u].cloak)
			return 999;
		units[u].cloak = item;
		if (u == 0)
			dresshero();
		return 1;
	case 0x63:
		if (units[u].wrist == 0) {
			units[u].wrist = item;
			return 1;
		}
		return 999;
	case 0x62:
		if (units[u].neck == 0) {
			units[u].neck = item;
			return 1;
		}
		return 999;
	case 0x4b:
		if (units[u].legs)
			return 999;
		units[u].legs = item;
		if (u == 0)
			dresshero();
		return 1;
	case 0x50:
		if (units[u].feet)
			return 999;
		units[u].feet = item;
		if (u == 0)
			dresshero();
		return 1;
	case 0x55:
	case 0x56:
		if (units[u].offhand)
			return 999;
		units[u].offhand = item;
		if (u == 0)
			dresshero();
		return 1;
	}
	return 999;
}

short
gearhit(unsigned long item, short mode)
{
	if (item == 0)
		return 0;
	if (mode == 3)
		picksheath(item, 1, 1);
	else if (!mode)
		pickworn(item, 1);
	else
		picksheath(item, 1, 0);
	if (wornpic == 0 || wornx > hoverpx || wornx + wornw < hoverpx
	    || worny > hoverpy || worny + wornh < hoverpy)
		return 0;
	if (getpixr(wornpic, hoverpx - wornx, hoverpy - worny))
		return 1;
	return 0;
}

short
onhead(short x, short y)
{
	if (x >= 0x66 && x <= 0x86 && y >= 0xc && y <= 0x30)
		return 1;
	return 0;
}

void
gearat(unsigned short u, unsigned long *pitem, int *pslot, int x, int y)
{
	*pslot = 999;
	*pitem = 0;
	if (x >= 8 && x < 0x18 && y >= 0xb0 && y < 0xc0 && units[u].belt) {
		*pslot = 0x15;
		*pitem = units[u].belt;
		return;
	}
	if (u && x >= 6 && x < 0x3a) {
		if (y >= 8 && y < 0x38) {
			*pslot = 0x28;
			*pitem = units[u].items[0];
			return;
		}
		if (y >= 0x38 && y < 0x68) {
			*pslot = 0x29;
			*pitem = units[u].items[1];
			return;
		}
		if (y >= 0x68 && y < 0x98) {
			*pslot = 0x2a;
			*pitem = units[u].items[2];
			return;
		}
	}
	gaitem = units[u].hand;
	if (gaitem && gearhit(gaitem, 0)) {
		*pslot = 0x14;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].offhand;
	if (gearhit(gaitem, 0)) {
		*pslot = 10;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].belt;
	if (gearhit(gaitem, 1)) {
		*pslot = 0x15;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].wrist;
	if (gearhit(gaitem, 0)) {
		*pslot = 6;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].hand;
	if (gaitem && (isdagger(gaitem) || issword(gaitem)) && gearhit(gaitem, 3)) {
		*pslot = 0x14;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].neck;
	if (gearhit(gaitem, 0)) {
		*pslot = 7;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].head;
	if (gearhit(gaitem, 0)) {
		*pslot = 1;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].cloak;
	if (gearhit(gaitem, 0)) {
		*pslot = 3;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].body;
	if (gearhit(gaitem, 0)) {
		*pslot = 0xf;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].feet;
	if (gearhit(gaitem, 0)) {
		*pslot = 0x1e;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].legs;
	if (gearhit(gaitem, 0)) {
		*pslot = 0x19;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].wrist;
	if (gaitem && x >= 0x50 && y >= 0x68 && x <= 0x68 && y <= 0x74) {
		*pslot = 6;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].neck;
	if (gaitem && x >= 0x6c && y >= 0x34 && x <= 0x8c && y <= 0x40) {
		*pslot = 7;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].head;
	if (gaitem && x >= 0x5e && y >= 0xe && x <= 0x92 && y <= 0x38) {
		*pslot = 1;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].cloak;
	if (gaitem && x >= 0x52 && y >= 0x30 && x <= 0xa2 && y <= 0x4e) {
		*pslot = 3;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].feet;
	if (gaitem && x >= 0x58 && y >= 0xa2 && x <= 0x96 && y <= 0xbe) {
		*pslot = 0x1e;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].legs;
	if (gaitem && x >= 0x62 && y >= 0x68 && x <= 0x96 && y <= 0xbe) {
		*pslot = 0x19;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].hand;
	if (gaitem && x >= 0x3a && y >= 0xe && x <= 0x66 && y <= 0xbe) {
		*pslot = 0x14;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].body;
	if (gaitem && x >= 0x52 && y >= 0x30 && x <= 0xa2 && y <= 0x92) {
		*pslot = 0xf;
		*pitem = gaitem;
		return;
	}
	gaitem = units[u].offhand;
	if (gaitem && x >= 0x8a && y >= 0xe && x <= 0xcc && y <= 0xbe) {
		*pslot = 10;
		*pitem = gaitem;
		return;
	}
}

int
taketorch(void)
{
	if (units[0].offhand == 0 && !knockedout && units[0].mode != 0x5a && !units[0].asleep) {
		ttp = 0;
		if (usabletorch(units[0].items[0]))
			ttp = &units[0].items[0];
		else if (usabletorch(units[0].items[1]))
			ttp = &units[0].items[1];
		else if (usabletorch(units[0].items[2]))
			ttp = &units[0].items[2];
		if (ttp) {
			units[0].offhand = *ttp;
			*ttp = 0;
		ok:
			setlit(&units[0].offhand, 1);
			queuesound(7);
			return 1;
		}
		for (ttslot = 0; ttslot < 50; ttslot++)
			if (usabletorch(realms[0].items[ttslot])) {
				units[0].offhand = tttorch = takeout(0, ttslot);
				goto ok;
			}
	}
	return 0;
}

void
burntorches(void)
{
	if (burntorch(&units[0].offhand)) {
		units[0].offhand = 0;
		units[0].load -= itemweight(0x604);
		if (passmode || !taketorch()) {
			if (passmode && (btshield = classinpack(&btrealm, &btk, 0x55)) != 0
			    && (btshield = takeout(btrealm, btk)) != 0)
				giveitem(0, btshield, 3);
			dresshero();
		}
	}
	for (btk = 0; btk < ntorches; btk++) {
		btidx = torches[btk];
		if (mapitems[btidx] & 0x8000 && !(mapitems[btidx + 1] & 0x6000))
			setlit(&mapitems[btidx], 0);
		else if (burntorch(&mapitems[btidx])) {
			mapitems[btidx] &= 0xfffff000;
			mapitems[btidx] |= 0x2d7;
		} else
			continue;
		updatecell(itemcell(btidx));
		ntorches--;
		for (btidx = btk; btidx < ntorches; btidx++)
			memcpy(&torches[btidx], &torches[btidx + 1], 2);
		btk--;
	}
}

void
burndown(unsigned long *p, unsigned long n)
{
	if (islittorch(*p)) {
		bnleft = *p >> 16;
		if (bnleft < 0x400) {
			if (n >= bnleft)
				bnleft = 0;
			else
				bnleft -= n;
			*p &= 0xffff;
			*p |= bnleft << 16;
		}
	}
}

void
agetorches(unsigned long n, int all)
{
	if (n)
		n--;
	if (n)
		n--;
	listtorches();
	burntorches();
	for (agidx = 1; agidx < nmapitems; agidx++)
		burndown(&mapitems[agidx], n);
	if (all)
		burndown(&units[0].offhand, n);
	burntorches();
	listtorches();
}

short
unconscious(void)
{
	if (units[0].asleep) {
		if (!palblack) {
			if (!talkactive) {
				denymsg(strs[306]);
				drawscreen();
			}
			waitrelease(0);
		} else
			resetinput();
		return 1;
	}
	return 0;
}

short
always(void)
{
	return 1;
}

short
inregion(short x, short y, short n)
{
	if (x >= regions[n].x0 && x <= regions[n].x1 && y >= regions[n].y0 && y <= regions[n].y1)
		return 1;
	return 0;
}

void
fillregions(void)
{
	memset(regionmap, 0, sizeof regionmap);
	for (frk = 0; frk < nregions; frk++)
		for (fry = regions[frk].y0; fry <= regions[frk].y1; fry++)
			for (frx = regions[frk].x0; frx <= regions[frk].x1; frx++)
				regionmap[frx + fry * 150] = frk;
}

short
provat(short x, short y)
{
	if (x < 0 || y < 0 || x >= 150 || y >= 150)
		return 0;
	return provmap[y * 150 + x];
}

short
cellprov(unsigned short pos)
{
	return provmap[pos];
}

short
regionat(short x, short y)
{
	if (x < 0 || y < 0 || x >= 150 || y >= 150)
		return 0;
	return regionmap[y * 150 + x];
}

short
cellregion(unsigned short pos)
{
	return regionmap[pos];
}

short
findregion(short x, short y)
{
	if (x < 0 || y < 0 || x >= 150 || y >= 150)
		return 0;
	for (fdk = 1; fdk < nregions; fdk++)
		if (x >= regions[fdk].x0 && x <= regions[fdk].x1
		    && y >= regions[fdk].y0 && y <= regions[fdk].y1)
			return fdk;
	return 0;
}

void
setcurregion(void)
{
	curregion = 0;
	regiontag = 99;
	if (map[playery * 150 + playerx] & 0x2000) {
		curregion = regionat(playerx, playery);
		if (curregion)
			regiontag = regions[curregion].tag;
	}
}

void
scanblocks(void)
{
	unsigned char c;
	int i, y, cx, found, j, x, cy;

	found = 0;
	blockmax = 0;
	blockn = 0;
	cx = (playerx - 2) / 5;
	cy = (playery - 2) / 5;
	for (j = -2; j <= 2; j++) {
		y = cy + j;
		if (y < 0 || y >= 30)
			continue;
		for (i = -2; i <= 2; i++) {
			x = cx + i;
			if (x < 0 || x >= 30)
				continue;
			c = blocks[y][x];
			if (c & 0x20) {
				blockn++;
				if (j >= 1 && blocks[y - 1][x] & 0x20 || i < 1 && blocks[y][x + 1] & 0x20
				    || j < 1 && blocks[y + 1][x] & 0x20 || i >= 1 && blocks[y][x - 1] & 0x20)
					found = 1;
			}
			if (i >= -1 && i <= 1 && j >= -1 && j <= 1 && c > blockmax)
				blockmax = c;
		}
	}
	if (!found) {
		blockn = 0;
		if (!(blocks[cy][cx] & 0x20))
			blockmax &= ~0x20;
	}
}

void
seeall(void)
{
	memset(viewgrid, 1, sizeof viewgrid);
}

void
seeroofs(void)
{
	memset(viewgrid, 0x40, sizeof viewgrid);
}

// FIXED: near the map's top and bottom the walks of the sight leave map, which the original's memory fills with its neighbors. They read zeros off the map here
#ifdef FEATURE_FAULTS
enum {
	Sightrows = 10	/* the rows off the map that a walk can reach, and those from an end where it can */
};

static unsigned long sightbuf[22500 + 2 * Sightrows * 150];
static unsigned long *sight = map;

/* sightinit gives the walks the map, or near its ends a copy between rows of zeros. */
void
sightinit(void)
{
	sight = map;
	if (playery >= Sightrows && playery < 150 - Sightrows)
		return;
	memcpy(sightbuf + Sightrows * 150, map, sizeof map);
	sight = sightbuf + Sightrows * 150;
}

#define map sight
#endif

void
spreadup(void)
{
	spreadp = &viewgrid[0][9];
	spreadp += spreadrow * 19;
	spreadmap = &map[(spreadrow + playery - 8) * 150 + playerx];
	for (spreadk = 0; spreadk <= 7; spreadk++) {
		spreadp -= spreadk;
		if (spreadp[-1] == 1 || spreadp[1] == 1 || spreadp[19] == 1)
			if (spreadmap[-spreadk] & 0x8000)
				*spreadp = 1;
			else
				*spreadp = 2;
		spreadp += 2 * spreadk;
		if (spreadp[-1] == 1 || spreadp[1] == 1 || spreadp[19] == 1)
			if (spreadmap[spreadk] & 0x8000)
				*spreadp = 1;
			else
				*spreadp = 2;
		spreadp -= spreadk;
	}
}

void
spreaddown(void)
{
	spreadp = &viewgrid[0][9];
	spreadp += spreadrow * 19;
	spreadmap = &map[(spreadrow + playery - 8) * 150 + playerx];
	for (spreadk = 0; spreadk <= 7; spreadk++) {
		spreadp -= spreadk;
		if (spreadp[-1] == 1 || spreadp[1] == 1 || spreadp[-19] == 1)
			if (spreadmap[-spreadk] & 0x8000)
				*spreadp = 1;
			else
				*spreadp = 2;
		spreadp += 2 * spreadk;
		if (spreadp[-1] == 1 || spreadp[1] == 1 || spreadp[-19] == 1)
			if (spreadmap[spreadk] & 0x8000)
				*spreadp = 1;
			else
				*spreadp = 2;
		spreadp -= spreadk;
	}
}

void
spreadeast(void)
{
	spreadp = &viewgrid[0][0];
	spreadp += spreadrow * 19;
	spreadp++;
	spreadmap = &map[(spreadrow + playery - 8) * 150 + 1 + playerx - 9];
	for (spreadk = 1; spreadk <= 17; spreadmap++, spreadp++, spreadk++)
		if (spreadp[-1] == 1 || spreadp[1] == 1 || spreadp[19] == 1 || spreadp[-19] == 1
		    || spreadp[-20] == 1 || spreadp[-18] == 1 || spreadp[18] == 1 || spreadp[20] == 1)
			if (*spreadmap & 0x8000)
				*spreadp = 1;
			else
				*spreadp = 2;
}

void
spreadwest(void)
{
	spreadp = &viewgrid[spreadrow][17];
	spreadmap = &map[(spreadrow + playery - 8) * 150 + 17 + playerx - 9];
	for (spreadk = 17; spreadk >= 1; spreadmap--, spreadp--, spreadk--)
		if (spreadp[-1] == 1 || spreadp[1] == 1 || spreadp[19] == 1 || spreadp[-19] == 1
		    || spreadp[-20] == 1 || spreadp[-18] == 1 || spreadp[18] == 1 || spreadp[20] == 1)
			if (*spreadmap & 0x8000)
				*spreadp = 1;
			else
				*spreadp = 2;
}

void
roofeast(void)
{
	spreadp = &viewgrid[0][0];
	spreadp += spreadrow * 19;
	spreadp++;
	spreadmap = &map[(spreadrow + playery - 8) * 150 + 1 + playerx - 9];
	for (spreadk = 1; spreadk <= 17; spreadmap++, spreadp++, spreadk++)
		if (*spreadp == 0 && *spreadmap & 0x2000
		    && (spreadp[-1] == 8 || spreadp[-1] == 0x40 || spreadp[1] == 8 || spreadp[1] == 0x40
		    || spreadp[-19] == 8 || spreadp[-19] == 0x40 || spreadp[19] == 8 || spreadp[19] == 0x40))
			*spreadp = 8;
}

void
roofwest(void)
{
	spreadp = &viewgrid[spreadrow][17];
	spreadmap = &map[(spreadrow + playery - 8) * 150 + 17 + playerx - 9];
	for (spreadk = 17; spreadk >= 1; spreadmap--, spreadp--, spreadk--)
		if (*spreadp == 0 && *spreadmap & 0x2000
		    && (spreadp[-1] == 8 || spreadp[-1] == 0x40 || spreadp[1] == 8 || spreadp[1] == 0x40
		    || spreadp[-19] == 8 || spreadp[-19] == 0x40 || spreadp[19] == 8 || spreadp[19] == 0x40))
			*spreadp = 8;
}

void
lookaround(void)
{
	int k;

	// FIXED: sightinit gives the walks of the sight the zeros that they read off the map
#ifdef FEATURE_FAULTS
	sightinit();
#endif
	lookstale = 0;
	memset(viewgrid, 0, sizeof viewgrid);
	viewgrid[8][9] = 1;
	for (lookrow = 0; lookrow < 7; lookrow++) {
		spreadrow = 8 - lookrow;
		spreadup();
		spreadrow = lookrow + 8;
		spreaddown();
	}
	for (lookrow = 1; lookrow <= 15; lookrow++) {
		lookp = &viewgrid[0][lookrow * 19 + 1];
		for (lookcol = 1; lookcol <= 17; lookp++, lookcol++)
			if (*lookp == 2 && lookp[1] != 1 && lookp[19] != 1)
				*lookp = 0;
	}
	for (spreadrow = 1; spreadrow <= 15; spreadrow++)
		spreadeast();
	for (spreadrow = 15; spreadrow >= 1; spreadrow--)
		spreadwest();
	for (spreadrow = 1; spreadrow <= 15; spreadrow++)
		spreadeast();
	for (spreadrow = 15; spreadrow >= 1; spreadrow--)
		// FIXED: the passes go east, west, east, and east again, the last over the rows of a west pass. Go west, as roofeast and roofwest pair.
#if defined(FEATURE_FIXES)
		spreadwest();
#else
		spreadeast();
#endif
	for (lookrow = 1; lookrow <= 15; lookrow++) {
		lookp = &viewgrid[0][lookrow * 19 + 1];
		for (lookcol = 1; lookcol <= 17; lookp++, lookcol++)
			if (*lookp == 2)
				if (lookp[1] == 1)
					*lookp = 4;
				else if (lookp[19] == 1)
					*lookp = 4;
	}
	for (lookrow = 1; lookrow <= 15; lookrow++) {
		lookp = &viewgrid[0][lookrow * 19 + 1];
		for (lookcol = 1; lookcol <= 17; lookp++, lookcol++)
			if (*lookp == 2)
				if (lookp[19] == 0 && lookp[-1] == 0)
					*lookp = 0x20;
				else if (lookp[-19] == 0 && lookp[1] == 0)
					*lookp = 0x20;
				else if (lookp[20] != 0)
					if (lookp[20] == 1)
						*lookp = 4;
					else if (lookp[19] == 1 && lookp[-1] == 1)
						*lookp = 4;
					else if (lookp[-19] == 1 && lookp[1] == 1)
						*lookp = 4;
					else if (lookp[19] == 4 && lookp[1] == 4)
						*lookp = 4;
					else if (lookp[-19] == 1 && lookp[-1] == 1)
						*lookp = 4;
					else if (lookp[-19] == 4 && lookp[-1] == 4)
						*lookp = 4;
					else if (lookp[19] == 1 && lookp[1] == 1)
						*lookp = 4;
	}
	for (lookrow = 1; lookrow <= 15; lookrow++) {
		looky = lookrow + playery - 8;
		lookx = playerx - 8;
		k = lookrow * 19 + 1;
		lookp = &viewgrid[0][k];
		spreadmap = &map[looky * 150 + lookx];
		lookflags = &mapflags[lookx + looky * 150];
		for (lookcol = 1; lookcol <= 17; lookx++, lookcol++, lookp++, spreadmap++, lookflags++) {
			// FIXED: a mark off the map's top or bottom goes, where the original's lands before or past mapflags on its neighbors
#if defined(FEATURE_FIXES)
			if (*lookp && lookx + looky * 150 >= 0 && lookx + looky * 150 < 22500)
				*lookflags |= 0x200;
#else
			if (*lookp)
				*lookflags |= 0x200;
#endif
			if (!(lookx >= regions[curregion].x0 && looky >= regions[curregion].y0
			    && lookx <= regions[curregion].x1 && looky <= regions[curregion].y1)
			    && *spreadmap & 0x2000)
				switch (*lookp) {
				case 0:
					break;
				case 2:
					*lookp = 8;
					break;
				case 1:
					*lookp = 0x40;
					break;
				default:
					*lookp = 0x40;
					break;
				}
		}
	}
	for (spreadrow = 1; spreadrow <= 15; spreadrow++)
		roofeast();
	for (spreadrow = 15; spreadrow >= 1; spreadrow--)
		roofwest();
	for (lookrow = 1; lookrow <= 15; lookrow++) {
		spreadmap = &map[(playery + lookrow - 8) * 150 + (playerx - 8)];
		spreadp = &viewgrid[0][lookrow * 19 + 1];
		for (lookcol = 1; lookcol <= 17; spreadp++, spreadmap++, lookcol++)
			if (*spreadp == 0x40 && spreadp[1] & 0x48 && spreadp[19] & 0x48 && spreadp[20] & 0x48)
				*spreadp = 8;
	}
	firedist = 25;
	litnw = 0;
	litne = 0;
	litsw = 0;
	litse = 0;
	for (lookrow = 1; lookrow <= 15; lookrow++) {
		lookp = &viewgrid[0][lookrow * 19 + 1];
		for (lookcol = 1; lookcol <= 17; lookp++, lookcol++) {
			lookx = lookcol - 9;
			looky = lookrow - 8;
			lookcell = (looky + playery) * 150 + (lookx + playerx);
			// FIXED: a cell off the map goes, where the original reads mapflags and map past their ends
#ifdef FEATURE_FAULTS
			if (lookcell < 0 || lookcell >= 22500)
				continue;
#endif
			if (mapflags[lookcell] & 0x100 && (*lookp == 1 || *lookp == 4 || *lookp == 0x40)) {
				looktype = map[lookcell] & 0xfff;
				if ((looktype == 0x289 || looktype == 0x296) && (gamehour <= 4 || gamehour >= 23)
				    && !findclass2(lookcell, 0x56, 0x5a))
					continue;
				/*
				 * dead: the code generator looks 0x100 up after 0x296,
				 * so the loop keeps 0x100 in ebp as in the original
				 */
				k = 0x100;
				if (lookcol < 8) {
					if (lookrow < 7)
						litnw = 1;
					else if (lookrow > 9)
						litsw = 1;
					else {
						litnw = 1;
						litsw = 1;
					}
				} else if (lookcol > 10) {
					if (lookrow < 7)
						litne = 1;
					else if (lookrow > 9)
						litse = 1;
					else {
						litne = 1;
						litse = 1;
					}
				} else {
					if (lookrow < 7) {
						litnw = 1;
						litne = 1;
					} else if (lookrow > 9) {
						litsw = 1;
						litse = 1;
					}
				}
				lookx = abs(lookx);
				looky = abs(looky);
				lookdist = lookx > looky ? lookx : looky;
				if (lookdist < firedist)
					firedist = lookdist;
			}
		}
	}
	calctorch();
	scanblocks();
}

// FIXED: map names the map again past the walks of the sight
#ifdef FEATURE_FAULTS
#undef map
#endif

void
waitmouseup(short wait)
{
	tracesite = 0x46;
	if (wait)
		waitforup = 1;
	pollinput();
	while (mouseleft != 0 || wait && mouseleft != mouseright)
		if (playmode == 1 && units[0].health > 0)
			gametick();
		else {
			readinput();
			if (needredraw(0xfd))
				drawscreen();
		}
	if (wait)
		waitforup = 0;
	leftclick = 0;
}

void
waitkeyup(void)
{
	tracesite = 0x4b;
	pollinput();
	while (keycode)
		if (playmode == 1 && units[0].health > 0)
			gametick();
		else {
			readinput();
			if (needredraw(0xfd))
				drawscreen();
		}
	lastscan = 0;
}

char
nextkey(void)
{
	while (keycode)
		readinput();
	while (!keycode)
		readinput();
	return keycode;
}

void
dragpanel(unsigned char n)
{
	dplastx = 999;
	dplasty = 999;
	dpoffx = hoverpx;
	dpoffy = hoverpy;
	hidecursor();
	setclip(dpoffx + 8, dpoffy, dpoffx + 632 - panels[n].w, dpoffy + 480 - panels[n].h);
	while (mouseleft) {
		if (esckey || quitting)
			break;
		pollinput();
		dpx = mousex - dpoffx;
		dpy = mousey - dpoffy;
		panels[n].x = dpx;
		panels[n].y = dpy;
		if (dpx != dplastx || dpy != dplasty) {
			dplastx = dpx;
			dplasty = dpy;
			drawscreen();
		}
	}
	hoversound();
	defclip();
	leftclick = 0;
	rightclick = 0;
	dblclick = 0;
	waitrelease(0);
	drawcursor();
}

int
closehover(unsigned char c)
{
	if (panelstack[0] == 0xff)
		return 0;
	switch (c) {
	case 1:
	case 0xe6:
		closepanels();
		break;
	case 0x31:
		return 0;
	default:
		closepanel(c - 0x28);
		break;
	}
	cleartexts();
	if (!palblack)
		playnow(0x11);
	mousemoved = 1;
	rightclick = 0;
	return 1;
}

int
grabpanel(unsigned char c)
{
	if (mouseleft == 0)
		return 0;
	mousemoved = 1;
	readinput();
	cleartexts();
	if (raisepanel(c))
		drawscreen();
	if (hovercx != 999)
		return 0;
	dragpanel(c);
	mousemoved = 1;
	return 1;
}

void
openhero(void)
{
	openpanel(1);
	movepanel(1, panels[1].x, panels[1].y);
	openpanel(3);
	panels[3].id = 0;
	panels[3].arg = 740;
	panels[3].cellx = 0;
	panels[3].celly = 0;
	panels[3].w = 212;
	panels[3].h = 280;
	movepanel(3, panels[3].x, panels[3].y);
	mousemoved = 1;
}

void
togglehero(void)
{
	dropheld();
	cleartexts();
	if (isopen(1) && isopen(3)) {
		closepanel(1);
		closepanel(3);
		if (!palblack)
			playnow(0x11);
	} else {
		openhero();
		hoversound();
	}
	drawscreen();
}

void
savetarget(void)
{
	savtkind = targetkind;
	savtx = targetx;
	savty = targety;
	savtitem = targetitem;
	savtidx = targetidx;
}

void
restoretarget(void)
{
	targetkind = savtkind;
	targetx = savtx;
	targety = savty;
	targetitem = savtitem;
	targetidx = savtidx;
}

void
savehover(void)
{
	savhkind = hoverkind;
	savhx = hovercx;
	savhy = hovercy;
}

void
restorehover(void)
{
	hoverkind = savhkind;
	hovercx = savhx;
	hovercy = savhy;
}

void
clickhover(void)
{
	if (!presslatch)
		readinput();
	hoverkind = presskind;
	hovercx = presscx;
	hovercy = presscy;
}

int
picktarget(void)
{
	short n, m;

	targetidx = 0;
	targetcls = 0;
	targetowner = 0;
	switch (hoverkind) {
	case 1:
		if (hovercx == playerx && hovercy == playery) {
			if (menumode) {
				targetidx = 0;
				targetitem = 0x270;
				return 2;
			}
	setplace:
			targetkind = hoverkind;
			targetx = hovercx;
			targety = hovercy;
			return 0;
		}
		for (;;) {
			targetkind = hoverkind;
			targetx = hovercx;
			targety = hovercy;
			playerstep(hovercx, hovercy);
			if (!cansee(hovercx, hovercy))
				return 0;
			targetidx = unitat(hovercx, hovercy);
			if (targetidx != 0xff) {
				targetitem = (targetidx << 16) + 0x270;
				targetcls = 0x14;
				return 2;
			}
			targetitem = 0;
			targetidx = topitem(hovercy * 150 + hovercx);
			if (!targetidx)
				return 0;
			targetitem = mapitems[targetidx];
			targetcls = classof(targetitem & 0xfff);
			if (targetitem & 0x1000) {
				targetowner = ownerat(hovercx, hovercy);
				if (!targetowner)
					targetowner = 0xff;
			}
			if (targetcls == 2) {
				if (baddoor(hovercy * 150 + (hovercx + 1)))
					return 1;
				hovercx++;
			} else if (targetcls == 3) {
				if (baddoor((hovercy + 1) * 150 + hovercx))
					return 1;
				hovercy++;
			} else
				return 1;
		}
	case 0x2b:
	case 0x2c:
	case 0x2d:
	case 0x2e:
	case 0x2f:
	case 0x30:
		targetkind = hoverkind;
		targetx = hovercx;
		targety = hovercy;
		raisepanel(hoverkind - 0x28);
		if (hovercx == 999)
			;
		else {
			targetidx = hovercx;
			/*
			 * the item's byte offset with the slot before the realm:
			 * the compiler then gives the realm's row edx and the slot
			 * eax, as in the original
			 */
			targetitem = *(unsigned long *)((char *)realms[0].items + hovercx * 4 + panels[hoverkind - 0x28].id * sizeof(Realm));
			targetcls = classof(targetitem & 0xfff);
			if (targetitem & 0x1000 && targetkind != 0x2b) {
				if (panels[hoverkind - 0x28].cellx == 0 || panels[hoverkind - 0x28].celly == 0)
					targetowner = 0xff;
				else {
					targetowner = ownerat(panels[hoverkind - 0x28].cellx, panels[hoverkind - 0x28].celly);
					if (!targetowner)
						targetowner = 0xff;
				}
			}
			return 1;
		}
		targetitem = 0;
		targetidx = 0;
		if (!trademode && grabpanel(hoverkind - 0x28))
			return 3;
		return 0;
	case 0x29:
	case 0x2a:
		raisepanel(hoverkind - 0x28);
		targetidx = hovercx;
		targetkind = hoverkind;
		targetx = hovercx;
		targety = hovercy;
		targetitem = panelgear(targetkind, targetx);
		if (!targetitem) {
			if (hoverkind == 0x29 && onhead(hoverpx, hoverpy) || trademode || !grabpanel(hoverkind - 0x28))
				return 0;
			return 3;
		}
		targetcls = classof(targetitem & 0xfff);
		if (targetitem & 0x1000 && panels[hoverkind - 0x28].id != 0) {
			n = panels[hoverkind - 0x28].id;
			if (units[n].health > 0)
				targetowner = n;
			else {
				targetowner = 0xff;
				m = corpseof(n);
				if (m) {
					m = findcell(m);
					if (m) {
						m = ownerat(m % 150, m / 150);
						if (m) {
							targetowner = m;
							return 1;
						}
					}
				}
			}
		}
		return 1;
	case 0x28:
		if (grabpanel(0))
			return 3;
		goto setplace;
	case 0x31:
	case 0x32:
	case 0x33:
	case 0x35:
	case 0x36:
		if (grabpanel(hoverkind - 0x28))
			return 3;
		goto setplace;
	case 0xd9:
	case 0xda:
		targetkind = hoverkind;
		targetx = hovercx;
		targety = hovercy;
		if (targetx != 999) {
			targetitem = hoverkind == 0xd9 ? tradegoods[targetx] : tradeoffer[targetx];
			targetcls = classof(targetitem & 0xfff);
			if (hoverkind == 0xd9 && tradekind != 3)
				if (curunit)
					targetowner = curunit;
				else
					targetowner = 0xff;
		} else
			targetitem = 0;
		return 1;
	case 0xe6:
		goto setplace;
	}
	return 0;
}

unsigned long *
itemptr(unsigned char where, int n)
{
	switch (where) {
	case 1:
		return &mapitems[n];
	case 0x29:
	case 0x2a:
		return gearslot(&units[panels[where - 0x28].id], n);
	case 0x2b:
	case 0x2c:
	case 0x2d:
	case 0x2e:
	case 0x2f:
	case 0x30:
		return &realms[panels[where - 0x28].id].items[n];
	}
	return 0;
}

int
outofreach(int mode, int x, int y, unsigned long item, int range, int msg)
{
	unsigned char c;

	if (cheat && keys['W'] & 0x80 || mode != 1)
		return 0;
	ordx = abs(x - playerx);
	ordy = abs(y - playery);
	if (ordx > range || ordy > range) {
		if (msg)
			denymsg(strs[286]);
		return 1;
	}
	orcls = classof(item & 0xfff);
	if (orcls != 1 && orcls != 4) {
		c = sightat(x, y);
		if ((c == 4 || c == 0x40)
		    && (x > playerx && sightat(x, y + 1) != 1 || y > playery && sightat(x + 1, y) != 1)) {
	blocked:
			if (msg)
				denymsg(strs[854]);
			return 1;
		}
	}
	if (ordx <= 1 && ordy <= 1)
		return 0;
	if (clearpath(playerx, playery, x, y))
		return 0;
	if (x != lineendx || y != lineendy)
		goto blocked;
	return 0;
}

unsigned long
randmeat(void)
{
	rmroll = randn(100);
	if (rmroll < 40)
		return 0x40018e;
	if (rmroll < 70)
		return 0x400141;
	return 0x400143;
}

int
randpit(void)
{
	return randn(5) + 0x7f1;
}

short
butcher(int a, unsigned long n, int c)
{
	buk = targetitem & 0xfff;
	if (a != buk && a + 0x34 != buk)
		return 0;
	liftitem(0);
	targetitem = c + (randn(100) < 50 ? 0x34 : 0);
	additem(targety * 150 + targetx, targetitem);
	for (buk = 0; buk < n; buk++)
		additem(targety * 150 + targetx, randmeat());
	updatecell(targety * 150 + targetx);
	return 1;
}

int
usecorpse(short use)
{
	if (use) {
		switch (targetitem & 0xfff) {
		case 0x827:
		case 0x840:
		case 0x841:
		case 0x843:
		case 0x846:
		case 0x85b:
		case 0x874:
		case 0x875:
		case 0x877:
		case 0x87a:
			break;
		default:
			goto other;
		}
		if (targetitem >> 16 == 0 || gameday - units[targetitem >> 16].plea >= 3) {
			failmsg(strs[1200]);
			return 0;
		}
		if (butcher(0x840, 0xe, 0xa28))
			return 1;
		if (butcher(0x827, 9, 0xa29))
			return 1;
		if (butcher(0x841, 3, 0xa2a))
			return 1;
		if (butcher(0x843, 0xb, 0xa28))
			return 1;
		if (butcher(0x846, 0xe, 0xa28))
			return 1;
	}
other:
	units[0].pose = 0;
	if (targetitem >> 16) {
		dropheld();
		corpseunit = targetitem >> 16;
		calcload(corpseunit);
		if (!(units[corpseunit].load == 0)) {
			openpanel(2);
			panels[2].arg = 1;
			panels[2].id = corpseunit;
			mousemoved = 1;
			playnow(0x10);
			drawscreen();
			return 1;
		}
	}
	failmsg(strs[544]);
	return 0;
}

int
opencontainer(void)
{
	units[0].pose = 0;
	if ((!cheat || !(keys['W'] & 0x80)) && realms[idof(targetitem)].item & 0x8000000) {
		cleartip();
		denymsg(strs[271], itemname(targetitem));
		return 0;
	}
	if (targetkind == 1)
		ocpanel = openstore(targetitem, targetx, targety);
	else
		ocpanel = openstore(targetitem, 0, 0);
	if (ocpanel == 0xff)
		return 0;
	playnow(0xe);
	drawscreen();
	return 1;
}

int
asktarget(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e,
	char cls10, char ctl, char cls15, char p9, char p10)
{
	aksave = indoinput;
	akret = 0;
	actionturn = 0;
	indoinput = 1;
	if (!p9)
		closepanels();
	statshown = 0xff;
	akmode = playmode == 1 ? 1 : 0;
	akedge = 0;
again:
	drawscreen();
	waitrelease(1);
	entermenu();
	setcursor1(3, 9);
	drawcursor();
	leftclick = 0;
	tracesite = 0x50;
	for (;;) {
		if (playmode == 1 && units[0].health > 0)
			gametick();
		else {
			readinput();
			if (needredraw(0xfd))
				drawscreen();
		}
		if (playerout() || akmode && playmode != 1)
			goto cancel;
		if (mouseright != 0 || rightclick != 0) {
			rightclick = 0;
			if (!closehover(hoverkind))
				goto cancel;
			waitforup = 1;
			mousemoved = 1;
			drawscreen();
			tracesite = 0x51;
			while (mouseright)
				if (playmode == 1 && units[0].health > 0)
					gametick();
				else {
					readinput();
					if (needredraw(0xfd))
						drawscreen();
				}
			waitforup = 0;
			mousemoved = 1;
			goto again;
		}
		if (esckey) {
	cancel:
			akret = -1;
			goto end;
		}
		if (p9)
			if (hoverx < 0x262)
				akedge = 0;
			else if (hoverx >= 0x266 && akedge == 0) {
				akedge = 1;
				togglehero();
				mousemoved = 1;
				goto again;
			}
		if (mouseleft != 0 || leftclick != 0)
			break;
	}
	akres = picktarget();
	if (p10 && targetkind == 0x29 && onhead(hoverpx, hoverpy))
		if (akres == 1 && targetx == 1 && p10 < 2)
			;
		else {
			targetkind = 1;
			targetx = playerx;
			targetidx = 0;
			targetitem = 0x270;
			akret = 2;
			targety = playery;
			goto end;
		}
	switch (akres) {
	case 0:
		akret = 0;
		goto end;
	case 3:
		goto again;
	}
	akres = classof(targetitem & 0xfff);
	if (akres != 0x14 && outofreach(targetkind, targetx, targety, targetitem, 2, 1)) {
		akret = 0;
		goto end;
	}
	if (cls10 && akres == 10) {
		notedkind = 0xff;
		if (opencontainer())
			goto again;
		akret = 0;
		goto end;
	}
	if (cls15 && akres == 0x15) {
		if (usecorpse(0))
			goto again;
		akret = 0;
		goto end;
	}
	if (akres == 0x14 && p9 && ctl && targetidx == 0) {
		togglehero();
		mousemoved = 1;
		goto again;
	}
	if (akres == 0x14 && ctl) {
		usecorpse(0);
		goto again;
	}
	if (a == 0xff)
		akret = 1;
	else if (akres == a || akres == b || akres == c || akres == d || akres == e)
		akret = 1;
	else
		akret = 0;
end:
	leavemenu(1);
	indoinput = aksave;
	return akret;
}

int
craftof(Unit *u)
{
	if (badunit(u))
		return 1;
	if (issmith(u))
		return 3;
	if (iscraftsman(u))
		return 4;
	if (isfemale(u))
		return 2;
	return 1;
}

void
pickjob(Unit *u)
{
	if (badunit(u))
		return;
	u->job = 0;
	u->jobcount = 0;
	pjcraft = craftof(u);
	if (!isfemale(u))
		goto pick;
	if (!units[0].legs && randn(100) < 75)
		pjjob = 9;
	else if (!units[0].body && randn(100) < 50)
		pjjob = 1;
	else if (!units[0].cloak && randn(100) < 30)
		pjjob = 8;
	else
pick:
		pjjob = randn(9) + 1;
	while (jobs[pjjob].type != pjcraft) {
		pjjob++;
		if (pjjob >= 10)
			pjjob = 1;
	}
	u->job = pjjob;
}

void
produce(void)
{
	for (pdk = 1; pdk < nunits; pdk++) {
		if (units[pdk].health <= 0)
			continue;
		if (isservant(&units[pdk]))
			for (pdi = 0; pdi < 3; pdi++)
				if (units[pdk].items[pdi] == 0)
					units[pdk].items[pdi] = 0x80018f;
		if (!myservant(&units[pdk]) || !atwork(&units[pdk]))
			continue;
		pdi = athome(&units[pdk], 1);
		if (!pdi)
			continue;
		if (units[pdk].job == 0)
			pickjob(&units[pdk]);
		if (!units[pdk].job)
			continue;
		units[pdk].jobcount++;
		if (units[pdk].jobcount <= jobs[units[pdk].job].n)
			continue;
		if (jobs[units[pdk].job].item)
			giveitem(pdk, jobs[units[pdk].job].item, 2);
		units[pdk].job = 0;
		units[pdk].jobcount = 0;
		pickjob(&units[pdk]);
	}
	for (pdk = 5; pdk < nrealms + 5; pdk++) {
		if (realmfull(pdk))
			continue;
		if (itempanel(pdk << 16) != 0xff)
			continue;
		for (pdneed = 0; pdneed < realms[pdk].nneeds; pdneed++) {
			pdi = realms[pdk].needs[pdneed].maker;
			if (pdi) {
				if (pdi < nunits) {
					if (units[pdi].health > 0)
						goto found;
					continue;
				} else {
					pdregion = realms[pdk].region;
					if (!pdregion)
						continue;
					if (pdi == 0xff) {
						for (pdi = 0; pdi < regions[pdregion].nunits; pdi++) {
							pdj = regions[pdregion].units[pdi];
							if (units[pdj].health > 0)
								goto found;
						}
						continue;
					}
					if (pdi != 0xfe)
						continue;
					for (pdi = 0; pdi < regions[pdregion].nunits; pdi++) {
						pdj = regions[pdregion].units[pdi];
						if (units[pdj].health > 0 && isservant(&units[pdj]))
							goto found;
					}
					continue;
				}
			}
		found:
			// FIXED: pdregion comes from the last need that a region's units met, so a need met another way takes the owner of a stale region. Take the realm's own region.
#if defined(FEATURE_FIXES)
			pdregion = realms[pdk].region;
#endif
			pdj = typenames[realms[pdk].needs[pdneed].item & 0xfff];
			pdcount = 0;
			for (pdi = 0; pdi < 50; pdi++)
				if (typenames[realms[pdk].items[pdi] & 0xfff] == pdj)
					pdcount++;
			if (pdcount >= 5)
				continue;
			if (++realms[pdk].needs[pdneed].elapsed < realms[pdk].needs[pdneed].period)
				continue;
			pditem = realms[pdk].needs[pdneed].item;
			if (regions[pdregion].lord)
				pditem |= 0x1000;
			for (pdi = 0; pdi < realms[pdk].needs[pdneed].n; pdi++)
				stowat(pdk, stowspot(pdk, 0xff, 0xff, pditem & 0xfff), pditem);
			realms[pdk].needs[pdneed].elapsed = 0;
		}
	}
}

void
setlevels(void)
{
	if (game.tiredcount < 70)
		game.tiredlevel = 0;
	else if (game.tiredcount < 100)
		game.tiredlevel = 1;
	else if (game.tiredcount < 130)
		game.tiredlevel = 2;
	else if (game.tiredcount < 150)
		game.tiredlevel = 3;
	else if (game.tiredcount < 160)
		game.tiredlevel = 4;
	else if (game.tiredcount < 190)
		game.tiredlevel = 5;
	else if (game.tiredcount < 240)
		game.tiredlevel = 6;
	else if (game.tiredcount < 360)
		game.tiredlevel = 7;
	else
		game.tiredlevel = (game.tiredcount - 360) / 10 + 8;
	if (game.hungercount < 10)
		game.hungerlevel = 0;
	else if (game.hungercount < 30)
		game.hungerlevel = 1;
	else if (game.hungercount < 40)
		game.hungerlevel = 2;
	else if (game.hungercount < 50)
		game.hungerlevel = 3;
	else if (game.hungercount < 60)
		game.hungerlevel = 4;
	else if (game.hungercount < 120)
		game.hungerlevel = 5;
	else if (game.hungercount < 240)
		game.hungerlevel = 6;
	else if (game.hungercount < 720)
		game.hungerlevel = 7;
	else
		game.hungerlevel = (game.hungercount - 720) / 240 + 8;
}

void
tire(short n)
{
	game.tiredcount += n;
	if (game.tiredcount < 0)
		game.tiredcount = 0;
	else if (game.tiredcount > 30000)
		game.tiredcount = 30000;
	setlevels();
}

int
autoeat(void)
{
	aeate = 0;
	if (game.hungerlevel < 5 || trademode || playmode != 1)
		return 0;
again:
	for (aeslot = 0; aeslot < 50; aeslot++) {
		aeitem = realms[0].items[aeslot];
		if (plainfood(aeitem)) {
			aeitem = takeout(0, aeslot);
			units[0].load -= itemweight(aeitem);
			if (!palblack && isopen(3))
				aemode = 3;
			else
				aemode = 0;
			consume(&units[0], aeitem, aemode);
			aeate = 1;
			if (game.hungerlevel < 5)
				return aeate;
			goto again;
		}
	}
	return aeate;
}

void
healunits(void)
{
	for (hlunit = 1; hlunit < nunits; hlunit++)
		if (units[hlunit].health > 0 && !(gamehour & 1)
		    && units[hlunit].health < kindof(&units[hlunit])->maxhealth * 2 / 3
		    && !units[hlunit].poison && !units[hlunit].bleeding)
			units[hlunit].health++;
}

void
nexthour(void)
{
	nhhunger = 0;
	nhtired = 0;
	cursestruck = 0;
	if (passmode != 2)
		healunits();
	gamehour++;
	if (gamehour == 11) {
		if (passmode == 2)
			;
		else
			produce();
	} else if (gamehour > 23) {
		gamehour = 0;
		game.fishcount = 0;
		game.tries = 0;
		gameday++;
		if (passmode != 3)
			plotday();
	} else if (gamehour == 3) {
		if (passmode != 2) {
			scavenge(0);
			sweepmap(0);
			decaycorpses();
		}
	}
	if (passmode == 2)
		;
	else
		plotcheck();
	if (passmode != 3) {
		hourtime = gametime;
		nhtired = game.tiredlevel;
		if (units[0].asleep || units[0].mode == 0x5a) {
			if (game.tiredcount >= 10)
				game.tiredcount -= 10;
			if (game.tiredcount >= 10)
				game.tiredcount -= 10;
		} else if (game.tiredcount < 32000)
			game.tiredcount += 10;
		nhhunger = game.hungerlevel;
		if (game.hungercount < 32000) {
			if (gamehour > 18 || gamehour <= 6)
				game.hungercount += 5;
			else
				game.hungercount += 10;
		}
		setlevels();
		if (passmode == 2)
			autoeat();
		if (getbit(game.flags, 0x33) && !jewelmagic(&units[0], 0x80000)) {
			if (!units[0].cursed)
				cursestruck = 1;
			units[0].blessed = 0;
			units[0].cursed = 0xff;
			if (!units[0].asleep && !passmode && units[0].health > kindof(&units[0])->maxhealth * 2 / 3) {
				if (!units[0].bleeding)
					cursestruck = 1;
				units[0].bleeding = units[0].health / 3;
				msgf(0x63, strs[388]);
			}
		}
		calcstats(&units[0]);
		fillgauges();
	}
	markunits();
}

void
setdaylight(void)
{
	if (gamehour <= 3)
		sunlight = 4;
	else if (gamehour == 4 || gamehour == 23)
		sunlight = 7;
	else if (gamehour == 5 || gamehour == 22)
		sunlight = 11;
	else
		sunlight = 15;
}

short
ambient(void)
{
	if (mapstate[0x22] <= 15)
		return lastambient = 5;
	return lastambient = sunlight;
}

void
calclight(void)
{
	clwarm = 0;
	if (sensesleft)
		cllevel = 15;
	else {
		clamb = cllevel = ambient();
		if (cllevel < 15) {
			cladd = 0;
			clboth = 0;
			clnear = 0;
			if (firedist < torchdist) {
				if (firedist < 8) {
					cladd = 8 - firedist;
					if (cladd > 8)
						cladd = 8;
				}
			} else if (torchdist < 8) {
				cladd = 8 - torchdist;
				if (cladd > 8)
					cladd = 8;
			}
			if (islittorch(units[0].offhand)) {
				clnear = cladd;
				cladd = 8;
				if (clnear > cladd) {
					cladd = clnear;
					clboth = 1;
				} else if (clnear >= 7) {
					clboth = 1;
					cladd = 9;
				}
			}
			if (cladd) {
				if (mapstate[0x22] <= 15)
					;
				else {
					clcap = 12;
					if (clboth)
						clcap = 13;
					if (cllevel >= clcap)
						cladd = 0;
					else if (cllevel + cladd > clcap)
						cladd = clcap - cllevel;
				}
				if (cladd) {
					cllevel += cladd;
					clwarm = 1;
				}
			}
		}
	}
	if (cllevel > 15)
		cllevel = 15;
	else if (cllevel < 5)
		cllevel = 5;
	lightlvl = cllevel;
	lightwarm = clwarm;
}

void
setlight(void)
{
	calclight();
	if ((lightlvl != lastlight || lightwarm != lastwarm) && !trademode)
		lightpal(lightlvl, lightwarm);
	lastlight = lightlvl;
	lastwarm = lightwarm;
}

void
lookdone(void)
{
}

short
seesexit(short r, short x, short y)
{
	int d;

	/*
	 * d is dead: its three stores move the code generator's
	 * temporaries so that the multiply by 150 below compiles to
	 * shifts and r and x take esi and edi, as in the original
	 */
	d = x + 1;
	d = x + 2;
	d = x + 3;
	exitk = 0;
	while (exitk < regions[r].nexits) {
		exitx = regions[r].exitx[exitk];
		exity = regions[r].exity[exitk];
		exitcell = exitx + exity * 150;
		if (map[exitcell] & 0x4000 || mapflags[exitcell] & 1 && !(mapflags[exitcell] & 0x40)) {
			if (clearpath(x, y, exitx, exity))
				return 1;
			if (abs(lineokx - exitx) <= 1 && abs(lineoky - exity) <= 1)
				return 1;
		}
		exitk++;
	}
	return 0;
}

short
nearexit(short a, short x, short y)
{
	necell = x + y * 150;
	for (nestep = 1; nestep < 25; nestep++) {
		if (x + nestep <= 0 || x + nestep >= 150 || !(map[necell + nestep] & 0x4000))
			break;
		if (seesexit(a, x + nestep, y))
			return 1;
	}
	for (nestep = -1; nestep > -25; nestep--) {
		if (x + nestep <= 0 || x + nestep >= 150 || !(map[necell + nestep] & 0x4000))
			break;
		if (seesexit(a, x + nestep, y))
			return 1;
	}
	for (nestep = 1; nestep < 25; nestep++) {
		if (y + nestep <= 0 || y + nestep >= 150 || !(map[necell + nestep * 150] & 0x4000))
			break;
		if (seesexit(a, x, y + nestep))
			return 1;
	}
	for (nestep = -1; nestep > -25; nestep--) {
		if (y + nestep <= 0 || y + nestep >= 150 || !(map[necell + nestep * 150] & 0x4000))
			break;
		if (seesexit(a, x, y + nestep))
			return 1;
	}
	return 0;
}

short
indoors(void)
{
	if (!(map[playerx + playery * 150] & 0x2000))
		return 0;
	idregion = regionat(playerx, playery);
	if (!idregion)
		return 0;
	if (regions[idregion].nexits) {
		if (seesexit(idregion, playerx, playery))
			return 0;
		for (idstep = 1; idstep < 25; idstep++) {
			if (playerx + idstep <= 0 || playerx + idstep >= 150 || !(map[playercell + idstep] & 0x4000))
				break;
			if (seesexit(idregion, playerx + idstep, playery))
				return 0;
			if (nearexit(idregion, playerx + idstep, playery))
				return 0;
		}
		for (idstep = -1; idstep > -25; idstep--) {
			if (playerx + idstep <= 0 || playerx + idstep >= 150 || !(map[playercell + idstep] & 0x4000))
				break;
			if (seesexit(idregion, playerx + idstep, playery))
				return 0;
			if (nearexit(idregion, playerx + idstep, playery))
				return 0;
		}
		for (idstep = 1; idstep < 25; idstep++) {
			if (playery + idstep <= 0 || playery + idstep >= 150 || !(map[playercell + idstep * 150] & 0x4000))
				break;
			if (seesexit(idregion, playerx, playery + idstep))
				return 0;
			if (nearexit(idregion, playerx, playery + idstep))
				return 0;
		}
		for (idstep = -1; idstep > -25; idstep--) {
			if (playery + idstep <= 0 || playery + idstep >= 150 || !(map[playercell + idstep * 150] & 0x4000))
				break;
			if (seesexit(idregion, playerx, playery + idstep))
				return 0;
			if (nearexit(idregion, playerx, playery + idstep))
				return 0;
		}
	}
	return 1;
}

short
zonebyname(unsigned short id)
{
	for (zbk = 0; zbk < 30; zbk++)
		if (zones[zbk].name == id)
			return zbk;
	return 0;
}

short
mapzone(short id)
{
	for (mzk = 0; mzk < 30; mzk++)
		if (zones[mzk].id == id)
			return mzk;
	return 0;
}

short
zonecolors(unsigned char *buf, short n)
{
	short x, count, y, k;
	unsigned char c;

	count = 0;
	for (x = zones[n].tmx0; x <= zones[n].tmx1; x++)
		for (y = zones[n].tmy0; y <= zones[n].tmy1; y++) {
			c = getpixel(((unsigned char *)scrbuf), x, y);
			if (c) {
				for (k = 0; k < count; k++)
					if (c == buf[k])
						goto next;
				buf[count++] = c;
			}
		next:
			;
		}
	return count;
}

void
commoncolors(unsigned char *out, short *nout, unsigned char *a, short na, unsigned char *b, short nb)
{
	short i, j, k;

	for (i = 0; i < nb; i++)
		for (j = 0; j < na; j++)
			if (b[i] == a[j]) {
				for (k = 0; k < *nout; k++)
					if (out[k] == b[i])
						goto next;
				out[*nout] = b[i];
				(*nout)++;
			next:
				;
			}
}

void
maskcolors(unsigned char *p, unsigned char *set, short n)
{
	for (mcn = 0, mcp = p; mcn < 640 * 480; mcn++, mcp++) {
		if (*mcp) {
			for (mck = 0; mck < n; mck++)
				if (set[mck] == *mcp) {
					*mcp = 0x19;
					goto next;
				}
			*mcp = 0;
		}
	next:
		;
	}
}

void
blinkmark(int on)
{
	if (!tmmarkx)
		return;
	if (on) {
		if (++tmframe == 2)
			tmframe = 0;
		if (nearcursor(tmmarkx, tmmarky)) {
			hidecursor();
			putm1b(1, tmmarkx, tmmarky, tmframe);
			drawcursor();
		} else
			putm1b(1, tmmarkx, tmmarky, tmframe);
	} else
		putm1b(0, tmmarkx, tmmarky, tmframe);
}

void
makemark(short on)
{
	if (!tmmarkx)
		return;
	if (on == 0)
		grabtile(5, 0, tmmarkx, tmmarky);
	puttile1(3, 5);
	puttile1(4, 5);
	puttile3(3, 0x98);
	puttile3(4, 0x99);
	puttile1(0, 3);
	puttile1(1, 4);
	if (on == 0)
		blinkmark(0);
}

unsigned long
travelhours(short a, short b)
{
	unsigned long dx, dy, d, n, r;

	if (a == b)
		return 0;
	dx = abs(zones[b].tmx0 + (zones[b].tmx1 - zones[b].tmx0) / 2 - (zones[a].tmx0 + (zones[a].tmx1 - zones[a].tmx0) / 2));
	d = dx;
	dy = abs(zones[b].tmy0 + (zones[b].tmy1 - zones[b].tmy0) / 2 - (zones[a].tmy0 + (zones[a].tmy1 - zones[a].tmy0) / 2));
	if (dy == 0) {
		if (dx == 0)
			return 0;
	} else if (dx == 0) {
		if (dy == 0)
			return 0;
		d = dy;
	} else if (dx > dy)
		d = dx + dy / 2;
	else
		d = dy + dx / 2;
	n = 0;
	while (d > 40) {
		d -= 40;
		n += 24;
	}
	if (d % 7 > 3)
		r = 1;
	else
		r = 0;
	return d / 7 + r + n;
}

void
showzone(short n, short on)
{
	if (n == 0xff) {
		makemark(1);
		blinkmark(0);
		return;
	}
	if (n == menu1.n - 1) {
		if (on) {
			makemark(1);
			pillshade8(menu1.rects[n].x, menu1.rects[n].y, menu1.rects[n].w, menu1.rects[n].h);
		}
		return;
	}
	if (!on)
		return;
	if (zones[tmzones[n]].id != tmmap) {
		szhours = travelhours(tmzones[n], mapzone(tmmap));
		wsprintf(printbuf, texts + 0xb2b9, strs[zones[tmzones[n]].name], daystext(szhours));
		if (!tmmode && woulddie(hourticks * szhours))
			text3(0, 0xffff, 30, msgtext[nmsgs - 1]);
	} else
		wsprintf(printbuf, strs[zones[tmzones[n]].name]);
	szx = menu1.rects[n].x + menu1.rects[n].w / 2;
	szy = menu1.rects[n].y - 20;
	if (tmhere && n + 1 == tmhere)
		szy = tmmarky - 3;
	if (tmface && szy < 0x8e) {
		text3w(printbuf);
		if (szx - text3width / 2 < 0xbb)
			szx = text3width / 2 + 0xbb;
	}
	if (!tmmarkx) {
		text3cn(0, szx, szy, printbuf);
		return;
	}
	makemark(1);
	putm1b(0, tmmarkx, tmmarky, 0);
	text3cn(0, szx, szy, printbuf);
	grabtile(0, 0, tmmarkx, tmmarky);
	putm1b(0, tmmarkx, tmmarky, 1);
	text3cn(0, szx, szy, printbuf);
	grabtile(1, 0, tmmarkx, tmmarky);
	blinkmark(0);
}

void
travelface(void)
{
	if (!tmface)
		return;
	putwh((unsigned char *)faceback, 2, 0x4b, 0x16, 0x6c, 0x78);
	putfacen(2, 0x4b, 0x16, curblink ? curface2 : curface);
}

int
leftbehind(void)
{
	if (isbasemap(curmap) && curmap != 0x30)
		return 0;
	if (lefttime == 0 || gametime - lefttime > 20)
		;
	else
		return 0;
	for (lbidx = 1; lbidx < nmapitems; lbidx++)
		if (classof(mapitems[lbidx] & 0xfff) == 0x66) {
			lbcell = findcell(lbidx);
			if (mapflags[lbcell] & 0x200) {
				lefttime = gametime;
				return 2;
			}
		}
	for (lbidx = 1; lbidx < nmapitems; lbidx++)
		if (valueof(mapitems[lbidx]) > 2000) {
			lbcell = findcell(lbidx);
			if (mapflags[lbcell] & 0x200) {
				lefttime = gametime;
				return 1;
			}
		}
	return 0;
}

short
noshoes(void)
{
	if (units[0].feet)
		return 0;
	if (shoetime == 0 || gametime - shoetime > 20)
		;
	else
		return 0;
	if (shoewarns >= 3)
		return 0;
	shoetime = gametime;
	shoewarns++;
	return 1;
}

/* n * 100 / 15 exceeds n, so the random spread never applies: n * 15 / 100 was likely meant */
unsigned long
traveltime(unsigned long n)
{
	tvtime = n;
	// FIXED: n * 100 / 15 makes the spread more than n, so the test n >= tvk / 2 never holds and every journey takes its time to the hour. Spread by 15% of n.
#if defined(FEATURE_FIXES)
	tvk = n * 15 / 100;
#else
	tvk = n * 100 / 15;
#endif
	if (tvk >= 2 && n >= tvk / 2)
		tvtime = n - tvk / 2 + randn(tvk);
	if (!tvtime)
		tvtime++;
	tvk = (gamehour + tvtime) % 24;
	if (tvk > 22 || tvk == 0)
		tvtime += 9;
	else if (tvk < 10)
		tvtime += 10 - tvk;
	tvk = (gamehour + tvtime) % 24;
	tvtime *= hourticks;
	return tvtime;
}

/* the travel map: in mode 0 the player picks a zone to travel to from map b, other modes only show the map */
void
travelmap(int mode, short a, short b)
{
	int x0, y0, x1, y1;

	if (notravel())
		goto leave;
	tmdraw = drawbuf;
	drawbuf = 0;
	if (memguard6 != memguard4 && memguard6 != memguard5)
		talkok = 0;
	tmmode = mode;
	cleartexts();
	nop();
	tmmap = b;
	tmdx = 0;
	tmdy = 1;
	pagefx = 1;
again:
	tmzone = mapzone(tmmap);
	if (cheat && (keys['W'] & 0x80) || tmmode)
		goto draw;
	if (tmzone == 0) {
		denymsg(strs[1402]);
		drawscreen();
		goto out;
	}
	if (cheat && (keys['W'] & 0x80))
		goto draw;
	if (unconscious())
		goto out;
	if (indoors()) {
		denymsg(strs[1404]);
		drawscreen();
		goto out;
	}
	if (nearfoe(0)) {
		denymsg(strs[1403]);
		drawscreen();
		goto out;
	}
	tmy = leftbehind();
	if (tmy) {
		cleartexts();
		msgf(0x63, tmy == 1 ? strs[1140] : strs[1141]);
	} else if (noshoes()) {
		cleartexts();
		msgf(0x63, strs[1142]);
	} else
		goto draw;
	playnow(0x2d);
	drawscreen();
	goto out;
draw:
	cleartexts();
	if (pagefx < 2)
		fadefast(0);
	usepal(0);
	pgtoback();
	if (travel1)
		unpackscr(((unsigned char *)scrbuf), 0, (void *)travel1, travel1len);
	else
		loadscr(0x206b, ((unsigned char *)scrbuf), 0, packbuf, 0x96640);
	showscrt();
	if (travel2)
		unpackscr(((unsigned char *)scrbuf), 0, (void *)travel2, travel2len);
	else
		loadscr(0x206d, ((unsigned char *)scrbuf), 0, packbuf, 0x96640);
	for (tmk = 0; tmk < 5; tmk++) {
		if (pieces[tmk].flag == 0)
			break;
		if (getbit(game.flags, pieces[tmk].flag) || cheat && (keys['W'] & 0x80))
			putblockt(0, pieces[tmk].x0, pieces[tmk].y0, ((unsigned char *)scrbuf),
			    pieces[tmk].x0, pieces[tmk].y0,
			    pieces[tmk].x1 - pieces[tmk].x0,
			    pieces[tmk].y1 - pieces[tmk].y0);
	}
	if (travel3)
		unpackscr(((unsigned char *)scrbuf), 0, (void *)travel3, travel3len);
	else
		loadscr(0x206f, ((unsigned char *)scrbuf), 0, packbuf, 0x96640);
	memset(&menu1, 0, sizeof menu1);
	for (tmk = 1, menu1.n = 0; tmk < 30; tmk++) {
		if (!zones[tmk].id || tmk == 5 || tmk == 0x13 || tmk == 8)
			continue;
		if (!getbit(game.knownzones, tmk) && !(cheat && (keys['W'] & 0x80)))
			continue;
		putblockt(0, zones[tmk].tmx0, zones[tmk].tmy0, ((unsigned char *)scrbuf),
		    zones[tmk].tmx0, zones[tmk].tmy0,
		    zones[tmk].tmx1 - zones[tmk].tmx0,
		    zones[tmk].tmy1 - zones[tmk].tmy0);
		menu1.rects[menu1.n].x = zones[tmk].tmx0;
		menu1.rects[menu1.n].y = zones[tmk].tmy0;
		menu1.rects[menu1.n].w = zones[tmk].tmx1 - zones[tmk].tmx0;
		menu1.rects[menu1.n].h = zones[tmk].tmy1 - zones[tmk].tmy0;
		tmzones[menu1.n] = tmk;
		menu1.n++;
		if (tmmode == 2 && mapzone(a) != tmk)
			continue;
		if (cheat && (keys['W'] & 0x80) && zones[tmzone].id && zones[tmk].id && (keys[VK_SHIFT] & 0x80))
			line(zones[tmzone].tmx0 + (zones[tmzone].tmx1 - zones[tmzone].tmx0) / 2,
			    zones[tmzone].tmy0 + (zones[tmzone].tmy1 - zones[tmzone].tmy0) / 2,
			    zones[tmk].tmx0 + (zones[tmk].tmx1 - zones[tmk].tmx0) / 2,
			    zones[tmk].tmy0 + (zones[tmk].tmy1 - zones[tmk].tmy0) / 2, 0x3a);
	}
	tmzones[menu1.n] = 0xff;
	addcorner(&menu1, menu1.n, tmmode ? 1 : 0);
	memset(menu1.enabled, 1, menu1.n);
	tmmarkx = 0;
	tmmarky = 0;
	tmhere = 0;
	tmface = 0;
	if (tmmode) {
		if (talkactive || lastspeech) {
			tmface = 1;
			faceshadow(0x4b, 0x16, curblink ? curface2 : curface);
			getwh(0, 0x4b, 0x16, (unsigned char *)faceback, 0x6c, 0x78);
			putface(0x4b, 0x16, curblink ? curface2 : curface);
			drawname(0x4b, 0x16, curunit, 1);
			begintext(0);
			text3m(0, 0xbe, 0x1e, speech);
			endtext(0);
		}
		tmmarkx = zones[tmzone].tmx1 - 18;
		tmmarky = zones[tmzone].tmy0 + (zones[tmzone].tmy1 - zones[tmzone].tmy0) / 2 - 36;
		makemark(0);
		for (tmsel = 0; tmsel < menu1.n; tmsel++)
			if (zones[tmzones[tmsel]].id == tmmap) {
				tmhere = tmsel + 1;
				showzone(tmsel, 1);
				break;
			}
		makemark(0);
	} else if (tmmap != 1 && tmmap != 5) {
		tmmarkx = zones[tmzone].tmx1 - 18;
		tmmarky = zones[tmzone].tmy0 + (zones[tmzone].tmy1 - zones[tmzone].tmy0) / 2 - 36;
		makemark(0);
		for (tmsel = 0; tmsel < menu1.n; tmsel++)
			if (zones[tmzones[tmsel]].id == tmmap) {
				tmhere = tmsel + 1;
				break;
			}
	} else {
		x0 = zones[trvfrom].tmx0 + (zones[trvfrom].tmx1 - zones[trvfrom].tmx0) / 2;
		y0 = zones[trvfrom].tmy0 + (zones[trvfrom].tmy1 - zones[trvfrom].tmy0) / 2;
		x1 = zones[trvto].tmx0 + (zones[trvto].tmx1 - zones[trvto].tmx0) / 2;
		y1 = zones[trvto].tmy0 + (zones[trvto].tmy1 - zones[trvto].tmy0) / 2;
		x0 += (x1 - x0) / 2;
		y0 += (y1 - y0) / 2;
		tmmarkx = x0 - 18;
		tmmarky = y0 - 36;
		makemark(0);
	}
	zones[0].tmx0 = zones[0].tmx1 = tmmarkx + 18;
	zones[0].tmy0 = zones[0].tmy1 = tmmarky + 36;
	zones[5].tmx0 = zones[5].tmx1 = zones[0x13].tmx0 = zones[0x13].tmx1 = zones[0].tmx1;
	zones[5].tmy0 = zones[5].tmy1 = zones[0x13].tmy0 = zones[0x13].tmy1 = zones[0].tmy1;
	puthands();
	backto2();
	entermenu();
	drawcursor();
	if (pagefx)
		pagesound();
	if (pagefx < 2 || palblack) {
		flip();
		fadefast(1);
	} else
		turnpage(pagefx == 2 ? 1 : 0, 0, 0x1e0, 0);
	pagefx = 0;
	if (!tmmode && cur != 6 && cur != 7 && cur != 8)
		music(randn(3) + 6, 0);
	menu1.keys2[0] = 0xe;
	menu1.keys2[1] = 0x42;
	tickms = 300;
	tmtime = 0;
	menuanim = 1;
	menu1.fn = showzone;
	menudraw = travelface;
	menutick = blinkmark;
	tmsel = runmenu(&menu1, 0, 0, 0);
	menutick = 0;
	menuanim = 0;
	menudraw = 0;
	if (needredraw(tmsel))
		goto draw;
	if (esckey == 0xff)
		goto out;
	if (tmmode)
		tmsel = 0;
	if (tmsel == 0)
		;
	else if (tmsel == menu1.n)
		tmsel = 0;
	else if (tmzones[tmsel - 1] == tmzone) {
		if (getbit(game.seenzones, tmzones[tmsel - 1])) {
			tmx = zones[tmzones[tmsel - 1]].x;
			tmy = zones[tmzones[tmsel - 1]].y;
		} else {
			tmx = zones[tmzones[tmsel - 1]].inx;
			tmy = zones[tmzones[tmsel - 1]].iny;
			if (celldist(playerx, playery, zones[tmzones[tmsel - 1]].x, zones[tmzones[tmsel - 1]].y) <= 25)
				tmsel = 0;
		}
		tmtime = celldist(playerx, playery, tmx, tmy);
		if (tmtime <= 25)
			tmsel = 0;
	} else
		tmtime = traveltime(travelhours(tmzones[tmsel - 1], tmzone));
	fadefast(0);
	cleartexts();
	if (!tmsel)
		goto out;
	tmsel = tmzones[tmsel - 1];
	if (tmsel == 0xff)
		goto out;
	if (getbit(game.seenzones, tmsel)) {
		tmx = zones[tmsel].x;
		tmy = zones[tmsel].y;
		tmdx = zones[tmsel].dx;
		tmdy = zones[tmsel].dy;
	} else {
		tmx = zones[tmsel].inx;
		tmy = zones[tmsel].iny;
		tmdx = zones[tmsel].indx;
		tmdy = zones[tmsel].indy;
	}
	closepanels();
	train(0x18, 5);
	if ((!cheat || !(keys['W'] & 0x80)) && woulddie(tmtime)) {
		units[0].health = -32000;
		if (playmode != 1)
			setcombat(0);
		starttimer3();
		while (elapsed3() < 1500)
			pollinput();
		fadefast(0);
		goto out;
	}
	cleartexts();
	nop();
	plotdepart(tmmap, zones[tmsel].id);
	units[0].dirx = tmdx;
	units[0].diry = tmdy;
	tmk = zones[tmsel].id;
	// FIXED: the travel map saves the AutoSave whatever the option, whose off means none, and with the option on travel saves it again at the departure from another map. Leave that to travel, and save it here for a journey inside the map, whose departure travel passes by, when the option asks.
#if defined(FEATURE_FIXES)
	if (autolevel >= 1 && tmk == curmap)
		autosave();
#else
	autosave();
#endif
	travel(tmk, tmx, tmy, cheat && (keys['W'] & 0x80) ? 1 : tmtime, 1, 1, 1, 1);
	nop();
out:
	drawbuf = tmdraw;
	if (esckey == 0xff) {
		pagefx = 2;
		if (mayquit(1)) {
			pagefx = 1;
			quit();
			return;
		}
		pagefx = 3;
		goto again;
	}
	nop();
leave:
	if (!talkactive && !lastspeech && units[0].health > 0)
		leavemenu(1);
	else
		waitrelease(0);
	pagefx = 1;
}

int
visited(int n)
{
	if (n < 0 || n > 80)
		return 0;
	return getbit(game.visited, n);
}

void
markvisited(void)
{
	// FIXED: the clock's test leaves a new game's first map unmarked until a later travel, so the first save re-enters it from the release's records, which hang up what the hero took and undo what he did there. Mark it on its first arrival, where the saves that follow write its records.
#if defined(FEATURE_FIXES)
	if (curmap < 0x100u)
		setbit(game.visited, curmap, 1);
#endif
	if (gametime < 2)
		return;
	if (isbasemap(curmap))
		for (mvk = 0; mvk < 7; mvk++)
			if (!isbasemap(mvk))
				setbit(game.visited, mvk, 0);
	if (curmap < 0x100u)
		setbit(game.visited, curmap, 1);
	mvk = mapzone(curmap);
	if (!mvk)
		return;
	setbit(game.knownzones, mvk, 1);
	setbit(game.reachedzones, mvk, 1);
	if (mapflags[zones[mvk].x + zones[mvk].y * 150] & 0x200)
		setbit(game.seenzones, mvk, 1);
}

short lastambient = 15;
int doors[2][16] = {
	39, 52, 40, 53, 43, 56, 44, 57, 47, 60, 48, 61, 0, 0, 0, 0, 41, 42,
	54, 55, 45, 46, 58, 59, 49, 50, 62, 63
};
int memguard6 = 1;
int savtx = 0x3e7;
int savty = 0x3e7;
int savhx = 0x3e7;
int savhy = 0x3e7;
Job jobs[10] = {
	0, 0, 0, 0xec01ad, 2, 7, 0x3ff0221, 1, 1, 0xee01af, 4, 12, 0xe401ca,
	3, 8, 0xf301a8, 4, 6, 0x80018f, 2, 1, 0x50d, 3, 7, 0xf601b9, 2, 10,
	0xe101a0, 2, 6
};
Zone zones[30] = {
	0, 0x62a, 0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 74, 0, 1, 74, 74, 0, 1, 15,
	0x62b, 1, 0x174, 148, 0x1a1, 192, 1, 1, -108, -108, 70, 87, 0, 1, 73,
	89, 0, -1, 24, 0x62c, 1, 0x13b, 0x11f, 0x166, 0x14a, 1, 1, -108, -108,
	50, 15, 0, 1, 63, 52, 1, 0, 32, 0x62d, 1, 0x194, 0x17c, 0x1c3, 0x1a6,
	1, 1, -108, -108, 29, 45, 1, 0, 67, 43, 0, 1, 40, 0x62e, 0, 181, 158,
	211, 186, 0, 0, 0, 0, 90, 145, 0, -1, 121, 53, 0, 1, 1, 0x62a, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 43, 71, 0, 1, 43, 71, 0, 1, 28, 0x62f, 1, 0x189,
	0x129, 0x1a9, 0x14c, 1, 1, -108, -108, 54, 107, 0, -1, 50, 92, 0, -1,
	50, 0x630, 0, 165, 216, 196, 244, 1, 1, -108, -108, 49, 116, 0, -1,
	51, 81, 0, -1, 48, 0x63b, 0, 220, 188, 220, 188, 0, 0, 0, 0, 75, 76,
	0, 1, 75, 76, 0, 1, 35, 0x631, 0, 0x1cf, 0x169, 0x1ea, 0x17f, 15, 35,
	45, 55, 57, 141, 0, -1, 28, 45, 0, -1, 45, 0x632, 1, 0x10c, 183,
	0x132, 223, 1, 1, -108, -108, 89, 130, 0, -1, 95, 99, 0, -1, 37,
	0x633, 0, 0x166, 0x193, 0x177, 0x1a6, 0, 0, 0, 0, 88, 34, 0, 1, 76,
	56, 0, 1, 25, 0x634, 0, 0x11a, 0x150, 0x131, 0x166, 70, 75, -116,
	-126, 139, 102, 0, 1, 90, 93, -1, 0, 41, 0x635, 0, 0x150, 220, 0x168,
	244, 1, 1, -108, -108, 105, 87, -1, 0, 86, 83, -1, 0, 43, 0x636, 0,
	0x1c1, 103, 0x1df, 126, 1, 1, -108, -108, 84, 74, -1, 0, 47, 71, -1,
	0, 52, 0x637, 0, 0x1a9, 163, 0x1ba, 181, 0, 0, 0, 0, 8, 140, 0, -1,
	103, 38, 0, -1, 61, 0x638, 0, 0x169, 0x10b, 0x17d, 0x120, 1, 1, -108,
	-108, 69, 105, 0, -1, 68, 62, 0, -1, 65, 0x639, 0, 0x123, 0x11e,
	0x139, 0x132, 0, 0, 0, 0, 100, 96, 1, 0, 100, 96, 1, 0, 58, 0x63a, 0,
	0x153, 144, 0x169, 162, 10, 10, 100, 90, 64, 121, 0, -1, 69, 107, 0,
	-1, 5, 0x62a, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, 71, 0, 1, 43, 71, 0, 1
};
Piece pieces[5] = {
	136, 50, 196, 98, 0x514, 149, 149, 209, 205, 0x515, 0x101, 154, 0x13d,
	219, 0x516, 0x198, 127, 0x20a, 0x197, 0x517
};
short forcedfate = 99;
int sacol;
int sarow;
unsigned long csitem;
unsigned char cscls;
unsigned long oditem;
unsigned long odk;
int odidx;
unsigned long cditem;
int cdk;
int cdidx;
int bdidx;
unsigned char bdcls;
unsigned long plcell;
int plidx;
unsigned long plword;
unsigned char plcls;
short gkcls;
short swslot;
unsigned char swcls;
int rbx;
int rby;
int rbx0;
int rby0;
int rbx1;
int rby1;
int rbslot;
unsigned short bfslot;
unsigned long alcls;
unsigned long alname;
int spx;
int spy;
int spw;
int sph;
int spbx;
int spby;
int spbw;
int spbh;
unsigned short sptype;
int spcls;
int spname;
int spslot;
int spk;
int sppass;
int spx0;
int spy0;
int spx1;
int spy1;
unsigned short toslot;
unsigned long toitem;
int ctslot;
int ctn;
unsigned long cttype;
int tkn;
int tkslot;
unsigned long tkitem;
short prslot;
short prnext;
short raslot;
unsigned long raitem;
unsigned short rapos;
int slslot;
int slx;
int sly;
int slpad;
int slfirst;
int slx0;
int slx1;
int sly0;
int sly1;
unsigned short iaslot;
unsigned short iw[3];
unsigned short iwslot;
unsigned char iwcls;
unsigned short ivtype;
short rvslot;
unsigned long rvsum;
int eislot;
unsigned long nfvol;
int weslot;
int werealm;
unsigned long wesum;
unsigned char wecls;
int pc[3];
short iprealm;
short ipslot;
short cprealm;
short cpslot;
int pvslot;
int pvrealm;
unsigned long pvsum;
unsigned short pfcls;
short fhrealm;
short fhslot;
unsigned long fhitem;
unsigned long fhsum;
unsigned long wsitem;
unsigned long *pgp;
unsigned long tfitem;
unsigned long *tfp;
unsigned char tfcls;
unsigned long poold;
short pomob;
short posecond;
unsigned long gaitem;
unsigned short ttslot;
unsigned long tttorch;
unsigned long *ttp;
short btk;
unsigned short btidx;
unsigned long btshield;
short btrealm;
unsigned long bnleft;
unsigned long agidx;
short frk;
short frx;
short fry;
short fdk;
int looktype;
int dpoffx;
int dpoffy;
int dpx;
int dpy;
int dplastx;
int dplasty;
int ordx;
int ordy;
unsigned char orcls;
short rmroll;
unsigned long buk;
unsigned long corpseunit;
unsigned char ocpanel;
unsigned char akres;
char akedge;
char aksave;
int akret;
short akmode;
unsigned char pjcraft;
short pjjob;
short pdk;
short pdi;
short pdj;
short pdneed;
short pdregion;
short pdcount;
unsigned long pditem;
unsigned short aeslot;
short aemode;
unsigned long aeitem;
int aeate;
short hlunit;
unsigned char nhhunger;
unsigned char nhtired;
unsigned char cursestruck;
short cllevel;
short clamb;
short clwarm;
short cladd;
short clnear;
short clboth;
short clcap;
short exitk;
short exitx;
short exity;
int exitcell;
short nestep;
int necell;
short idregion;
short idstep;
short zbk;
short mzk;
unsigned long mcn;
short mck;
unsigned char *mcp;
unsigned long szhours;
int szx;
int szy;
unsigned short lbidx;
unsigned short lbcell;
unsigned long tvtime;
unsigned long tvk;
short tmzone;
short tmk;
short tmsel;
short tmx;
short tmy;
short tmdx;
short tmdy;
unsigned long tmtime;
unsigned char *tmdraw;
short mvk;
unsigned short lastslot;
char trademode;
short gearmsg;
unsigned long crcword;
int memguard4;
int memguard5;
unsigned char boxx0[50];
unsigned char boxy0[50];
unsigned char boxx1[50];
unsigned char boxy1[50];
unsigned char blockmax;
unsigned char blockn;
unsigned char spreadk;
unsigned char spreadrow;
unsigned char *spreadp;
unsigned long *spreadmap;
unsigned long *lookflags;
short lookcol;
short lookrow;
short lookx;
short looky;
unsigned char *lookp;
int lookunused;
short lookdist;
int lookcell;
unsigned char savtkind;
int savtitem;
int savtidx;
unsigned char savhkind;
char savhunused;
short savhunused2;
int savhunused3[2];
int lefttime;
int shoetime;
unsigned char shoewarns;
short tmmode;
unsigned char tmzones[30];
short tmmap;
short tmmarkx;
short tmmarky;
unsigned char tmframe;
unsigned char tmface;
unsigned char tmhere;
int setbedhour;
int movehour;

#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

typedef struct Realm Realm;
typedef struct Unit Unit;
typedef struct Panel Panel;
typedef struct Zone Zone;
typedef struct Reg Reg;
typedef struct Action Action;
typedef struct Game Game;
typedef struct Menu Menu;

struct Realm {
	unsigned long item;
	unsigned long items[50];
	unsigned short pos[50];
	char f130[0x158 - 0x130];
};

/* the saved game (009cc470) */
struct Game {
	char name[0x14];
	char look[2];
	char known[0x1e];	/* bits of the actions known, by Action.bit */
	char f34[0x5f8 - 0x34];
};

/* 0x1a bytes each (004c3d68) */
struct Action {
	unsigned short name;
	char f2[4];
	unsigned short bit;	/* bit in game.known */
	char f8[0x1a - 8];
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

/* 0x22 bytes each (004f9498): the zones of the map */
struct Zone {
	short id;
	unsigned short name;
	char f4[0x22 - 4];
};

/* 0x28 bytes each (00a4ca78): the screen's panels */
struct Panel {
	char f0[0x16];
	unsigned short id;
	unsigned short arg;
	char f1a[0x28 - 0x1a];
};

struct Unit {
	unsigned char x;
	unsigned char y;
	unsigned char nextx;
	unsigned char nexty;
	signed char dirx;
	signed char diry;
	short picset;
	unsigned char pose;
	unsigned char mode;
	unsigned char modea;
	unsigned char modeb;
	unsigned char modec;
	unsigned char moded;
	unsigned char schedmode[24];
	unsigned char scheda[24];
	unsigned char schedb[24];
	unsigned char postx[24];
	unsigned char posty[24];
	unsigned char areax0[24];
	unsigned char areay0[24];
	unsigned char areax1[24];
	unsigned char areay1[24];
	unsigned char part;
	char fe7[2];
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
	char turnmarks;
	unsigned char clan;
	char f132[0x134 - 0x132];
	unsigned char bewitcher;
	unsigned char asleep;
	unsigned char bewitched;
	char blessed;
	unsigned char poison;
	char cursed;
	unsigned char fleeing;
	unsigned char confused;
	char bleeding;
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
	char f14a[0x14d - 0x14a];
	unsigned char trailx0;
	unsigned char traily0;
	unsigned char trailx1;
	unsigned char traily1;
	unsigned char trailx2;
	unsigned char traily2;
	char holdtime;
	unsigned char cellmate;
	unsigned char incoming;
	unsigned char idlestep;
	unsigned char idlepose;
	unsigned char idlewait;
	unsigned char modee;
	unsigned char modef;
	unsigned char modeg;
	char f15c[0x161 - 0x15c];
	short face;
	short number;
	unsigned char spouse;
	unsigned char trailx3;
	unsigned char traily3;
	unsigned char goalx;
	unsigned char goaly;
	char f16a[0x16f - 0x16a];
	unsigned char talkflags;
	char f170[0x1b8 - 0x170];
};

unsigned short idof(unsigned long item);
int canput(int cell);
char *itemname(unsigned long item);
unsigned char classof(int i);
unsigned long breakdoor(unsigned long item);
void setwear(unsigned long *p, unsigned long v);
unsigned short wearof(unsigned long x);
unsigned short findclass(int pos, unsigned char c);
void updatecell(int cell);
char *breaktext(short n);
short forcelock(unsigned long item, unsigned char t, int n, short k);
void weargear(int a, Unit *u, unsigned long *item, short n, short e);
void attackval(Unit *u, short *dmg, short *kind, short *a, short *b, short n);
short msgf(short prio, char *fmt, ...);
void witness(unsigned long item, unsigned char t, int n);
void train(unsigned char k, int n);
unsigned char weaponskill(void);
short ownerat(short x, short y);
short skillof(Unit *u, short n, short b, short c, short d);
short prevails(short a, short b);
void queuesound(short n);
int music(int n, int lp);
int ismassive(int item);
void tire(short n);
int sanctioned(short m, int k, int x, int y, unsigned long item);
int liftitem(short ask);
void togglecombat(short n);
void wield(int a, int x, int y, unsigned long item);
int dig(short ask);
short asksteal(short m);
short askattack(void);
short isthrowing(unsigned long item);
void strike(Unit *a, Unit *u);
void throwat(short x, short y);
void denycombat(void);
void denypractice(void);
short usefail(char *s);
void notetarget(void);
void setcursor1(int kind, int img);
short sparring(Unit *u, Unit *v);
short nearfoe(int nearby);
void calcstats(Unit *u);
unsigned long giveitem(short u, unsigned long item, short how);
void calctorch(void);
void playnow(short n);
int putonfree(unsigned short u, unsigned long item);
short unconscious(void);
int picktarget(void);
int outofreach(int a, int x, int y, unsigned long item, int b, int c);
void waitmouseup(short wait);
int __cdecl readinput(void);
void cleartexts(void);
int opendoor(unsigned long pos, short force);
void lookaround(void);
void lookdone(void);
void plotdoor(short a, short x, short y);
void drawscreen(void);
short denymsg(char *fmt, ...);
void raiseitem(unsigned short k, unsigned short n);
void anchortip(unsigned char kind, int x, int y);
void plotfind(int a, int x, int y, unsigned long item);
int pickup(void);
int putheld(unsigned char a, int b);
int placeitem(void);
int isopen(unsigned char n);
char *typetext(unsigned long item);
unsigned long silvvalue(unsigned long item);
void titlecase(char *s);
int closedoor(unsigned long pos, short mode);
void playqueue(void);
void cleartip(void);
void cointext(char *s, unsigned long item);
short readitem(short n);
char *unitname(Unit *u);
unsigned long ageof(unsigned long item);
unsigned long valueof(unsigned long item);
void setid(unsigned long *p, unsigned long v);
unsigned short itemweight(unsigned long item);
int asknum(unsigned short *val, unsigned short min, unsigned short max);
unsigned long addcoins(unsigned long *p, unsigned long b);
int playerout(void);
short muted(void);
short yields(Unit *u, short k, unsigned long item, short m);
unsigned long classinpack(short *pk, short *pj, short cls);
void addfavor(int n);
void addscore(int n);
int isfood(unsigned long item);
int ispond(unsigned long x);
int waterat(int x, int y);
int deepwater(int x, int y);
int topitem(int cell);
short iswallns(unsigned long item);
short iswallew(unsigned long item);
unsigned long baselook(unsigned long item);
unsigned long eastlook(unsigned long item);
unsigned long southlook(unsigned long item);
unsigned long randlook(unsigned long item);
void emptyvessel(unsigned long *item);
short canstack(unsigned long a, unsigned long b);
short islittorch(unsigned long item);
short setlit(unsigned long *p, short dir);
void listtorches(void);
unsigned long additem(int pos, unsigned long item);
int usekey(unsigned long key);
int fillvessel(unsigned long *p, short k);
int grind(void);
short isthrowing2(unsigned long item);
int hasaction(unsigned char c);
int doaction(short flag);
void deedsink(unsigned long item);
unsigned char unitat(int x, int y);
void playerstep(int x, int y);
unsigned long valuefor(unsigned long item, Unit *u);
short notepoison(short n);
int checkpoison(short n);
int consume(Unit *u, unsigned long item, short v);
short clearsight(int x0, int y0, int x1, int y1);
void say(Unit *u, int now, int msg);
short isanimal(Unit *u);
short isfemale(Unit *u);
short isservant(Unit *u);
void clearqueue(void);
int cansee(int x, int y);
int stowat(unsigned short k, unsigned short n, unsigned long item);
int stowspot(int k, int x, int y, unsigned long item);
short nofit(unsigned long item, short k);
unsigned long *gearslot(Unit *u, int slot);
unsigned long panelgear(int n, int slot);
short unmanly(unsigned long item, short msg);
unsigned long puton(short u, unsigned long item);
short onhead(short x, short y);
void savetarget(void);
void restoretarget(void);
void gametick(void);
int needredraw(int c);
short showlabel(void);
short useinfo(void);
void showinfo(unsigned char show);
void forgettarget(void);
void drawcursor(void);
void hoversound(void);
void togglehero(void);
void savecursor(void);
void restorecursor(void);
void drawtext2(int n, int x, int y, char *s);
int text3(int n, int x, int y, char *s);
void hidecursor(void);
void waitrelease(short anim);
short actionidx(unsigned char c);
int waitrelease(void);
int __cdecl pollinput(void);
char *daytime(unsigned char h);
char *titlehead(void);
char *roman(DWORD n);
int fadefast(int in);
int textwin(char *s, char a, unsigned char b, unsigned char c, void (*pagefn)(short), Menu *m, int (*fn)(int), unsigned char mode);
void notelist(unsigned char marks);
void dropheld(void);
void restfor(short n);
void waitkeyup(void);
void markers(int param);
void textn(int n, int x, int y, char *fmt, ...);
int asknumber(short x, short y, unsigned short ndigits, int min, int max, int init);
int travel(unsigned short dest, int x, int y, unsigned long time, unsigned short depart, unsigned short a6, unsigned short mode, short a8);
void travelmap(int mode, short a, short b);
void actionmenu(short flag);
int loadslot(unsigned short slot, unsigned short mode);
short entergame(unsigned short n);
short loadmenu(int b);
short savemenu(int b);
void aftersave(void);
void leavemenu(short anim);
void showreport(int flag);
void charsheet(Unit *u, short mode);
void journal(void);
void options(int ingame);
int controls(short a, int b);
void docmenu(void);
void verstr(char *buf, short n);
short mapzone(short id);
short inqueue(short n);
short playing(short n);
short animturn(short x, short y);
short findtype(int pos, unsigned long type);
int stopmusic(void);
void trackregion(void);
void textat(unsigned pos, char *fmt, ...);
void entermenu(void);
int checkquit(void);
void clickhover(void);
int closehover(unsigned char k);
int useitem(short ask);
int tipshown(void);
int canstep(int dx, int dy);
short canstepkey(short k);
unsigned long markerat(int cell);
void enterspot(int n);
int sharecell(Unit *u);
void sharecells(void);
int moveunits(short mode);
void stepanims(void);
short autocombat(void);
int rununits(short mode);
void fidget(void);
void clearturn(void);
int releaseheld(void);
void delmsg(short n);
void layoutmsgs(void);
void nexthour(void);
void setdaylight(void);
void burntorches(void);
void markunits(void);
int cellflag40(unsigned long x);
int isswamp(unsigned long x);
void setcurregion(void);
void fillgauges(void);
void clearmind(Unit *u);
short indexof(Unit *u);
void talkto(unsigned short n);
int edgemenus(short mode);
int advance(int dx, int dy);
void killunit(Unit *u, short a, short b, short c, short d);
void setcombat(short n);
void endgame(short why);
int displayed(Unit *u);
short seesunit(Unit *u);
void setunitmode(Unit *u, unsigned char mode);

extern char *strs[];
extern int playerx;
extern int playery;
extern int targetx;
extern int targety;
extern int playmode;
extern int mousemoved;
extern short nofoes;
extern unsigned short curmap;
extern unsigned char targetkind;
extern unsigned long targetitem;
extern int targetidx;
extern unsigned char targetcls;
extern unsigned long mapitems[];
extern unsigned short weights[0x1000];
extern int cheat;
extern Realm realms[];
extern Unit units[];
extern char texts[47942];
extern unsigned char keys[256];
extern unsigned char menumode;
extern unsigned long hovergear;
extern char ptrfar;
extern char ptrblocked;
extern Panel panels[];
extern int hoverkind;
extern int hovercx;
extern int hovercy;
extern int hoverx;
extern int hovery;
extern char trademode;
extern char talkactive;
extern unsigned long tradeoffer[];
extern short lblx;
extern short lbly;
extern int tipanchorx;
extern int tipanchory;
extern int leftclick;
extern int dblclick;
extern int clicktime;
extern unsigned char nrealms;
extern unsigned char typenames[];
extern unsigned lockedslots;
extern unsigned long map[];
extern int savtitem;
extern int lefttime;
extern short nqueue;
extern int hoverpx;
extern int hoverpy;
extern unsigned char actsel;
extern int tracesite;
extern int shown;
extern int mouseleft;
extern int mousex;
extern int mousey;
extern unsigned char esckey;
extern int statshown;
extern unsigned short busyturns;
extern Action actions[];
extern unsigned char quickacts[];
extern char scrbuf[309700];
extern int mouseright;
extern int rightclick;
extern char keycode;
extern unsigned char gamehour;
extern short gameday;
extern char romanbuf[];
extern char workbuf[616000];
extern Game game;
extern char tabkey;
extern char heldcmd;
extern char lastchar;
extern char turnkeys;
extern char dirkeys;
extern char combatcause;
extern unsigned short lastslot;
extern int entersum;
extern unsigned short enteritems;
extern Zone zones[];
extern char printbuf[];
extern unsigned long gametime;
extern unsigned short playercell;
extern unsigned long mapflags[];
extern short curregion;
extern char landsounds;
extern unsigned char blockmax;
extern unsigned char blockn;
extern char mapstate[100];
extern short sunlight;
extern short scrolling;
extern int ptrpace;
extern int cur;
extern unsigned char indoinput;
extern int quitting;
extern int waitforup;
extern short nmsgs;
extern short lastlight;
extern char lastscan;
extern int lostcontrol;
extern unsigned short steptick;
extern unsigned short stepframe;
extern unsigned short lasttick;
extern unsigned short standdelay;
extern unsigned short walkdelay;
extern DWORD walkdue;
extern DWORD standdue;
extern unsigned walkturns;
extern unsigned char gtdepth;
extern short gtunused;
extern int stepinc;
extern unsigned fireframe;
extern unsigned slowframe;
extern unsigned waterframe;
extern short waterpace;
extern unsigned char cmbautooff;
extern short actionturn;
extern unsigned tipend;
extern unsigned long msgtime[17];
extern short msgkeep[17];
extern unsigned short hourticks;
extern int turncount;
extern char unitsrun;
extern char lookstale;
extern int movecursor;
extern short condgauge;
extern short talker;
extern short talkscene;

extern char lbltext[60];
extern unsigned short chbit;
extern int chstr;
extern unsigned long ctres;
extern unsigned char ctcls;
extern char ctbusy;
extern int ctkind;
extern int ctx;
extern int cty;
extern unsigned long *clword;
extern unsigned long *flword;
extern int fltype;
extern unsigned long flidx;
extern short flhard;
extern short flhit;
extern short flkind;
extern short fldmin;
extern short fldmax;
extern short flsnd;
extern short tobi;
extern int fores;
extern int atpick;
extern short atdx;
extern short atdy;
extern unsigned char atcls;
extern unsigned long atthrow;
extern int soold;
extern unsigned long sonew;
extern int siold;
extern unsigned long sinew;
extern unsigned short spcn;
extern unsigned long spcpart;
extern unsigned char spccls;
extern int puunit;
extern int puwcls;
extern unsigned char pucls;
extern unsigned char dicls;
extern short diunit;
extern int didone;
extern int digift;
extern unsigned long diword;
extern unsigned long dilook;
extern int dicell;
extern int ditop;
extern int dgdrop;
extern int dgpick;
extern int dgx;
extern int dgy;
extern short dcmenu;
extern int dcres;
extern int dcret;
extern short sqi;
extern short sqact;
extern int pkpick;
extern int pkx;
extern int pky;
extern short pkcalm;
extern short telemap;
extern short telex;
extern short teley;
extern int inpick;
extern int inres;
extern int lastindex;
extern int firstindex;
extern short wsr;
extern short ambr;
extern unsigned long gtmark;
extern int gtspot;
extern short gtmsg;
extern int gtwait;
extern int gtpct;
extern unsigned long tumark;
extern Unit *sayunit;
extern short saynum;
extern short sayold;
extern char dropsnd;
extern char cheatbuf[10];
extern char cheatc;
extern unsigned char cheati;
extern unsigned char lastkey;
extern unsigned char prevkey;
extern Reg reg;

/* the length of the cheat typed so far: 0xff until ',' starts one */
extern unsigned char cheatlen;
extern char cheats[10][10];

void
clickhint(int cell, unsigned char t)
{
	char *p;

	if (t == 20) {
		chstr = 0x115;
		chbit = 1;
	} else if (t == 10) {
		chstr = 0x117;
		chbit = 2;
	} else if (weights[cell & 0xfff] >= 65000) {
		if (t == 0x19 || t == 0x28 || t == 0x2e || t == 0x32 || t == 0x78
		|| t == 0x79 || t == 0x7a || t == 0x7b || t == 0x5a || t == 0x59
		|| t == 0x1e) {
			chstr = 0x116;
			chbit = 4;
		} else if (t == 0x37 || t == 0x85) {
			chstr = 0x118;
			chbit = 8;
		} else if (t == 0x82 || t == 0x83 || t == 0x84 || t == 0x86) {
			chstr = 0x119;
			chbit = 0x10;
		} else
			return;
	} else {
		if (t == 5 || t == 0x1e || t == 0x20 || t == 0x21 || t == 0x2d
		|| t == 0x3a || t == 0x56 || t == 0x59 || t == 0x5a) {
			chstr = 0x114;
			chbit = 0x20;
		} else if (t == 0x3c || t == 0x41 || t == 0x46 || t == 0x48
		|| t == 0x4b || t == 0x50 || t == 0x55 || t == 0x62 || t == 0x63) {
			chstr = 0x11a;
			chbit = 0x40;
		} else if (t == 0x36 || t == 0x37 || t == 0x8a || t == 0x39) {
			chstr = 0x11b;
			chbit = 0x80;
		} else if (t == 0x38 || t == 0x87 || t == 0x88 || t == 0x89
		|| t == 0x8b) {
			chstr = 0x11c;
			chbit = 0x100;
		} else
			return;
	}
	if ((reg.seen & chbit) == 0) {
		reg.seen |= chbit;
		for (p = lbltext; *p; p++)
			;
		wsprintf(p, strs[chstr]);
	}
}

int
dropcell(int *x, int *y)
{
	switch (randn(9)) {
	case 0:
	case 1:
		if (canput(playerx - 1 + (playery + 1) * 150)) {
			*x = playerx - 1;
			*y = playery + 1;
			return 1;
		}
		break;
	case 2:
	case 3:
		if (canput(playerx + 1 + (playery + 1) * 150)) {
			*x = playerx + 1;
			*y = playery + 1;
			return 1;
		}
		break;
	case 4:
		if (canput(playerx - 1 + playery * 150)) {
			*x = playerx - 1;
			*y = playery;
			return 1;
		}
		break;
	case 5:
		if (canput(playerx + 1 + playery * 150)) {
			*x = playerx + 1;
			*y = playery;
			return 1;
		}
		break;
	default:
		if (canput(playerx + (playery + 1) * 150)) {
			*x = playerx;
			*y = playery + 1;
			return 1;
		}
		break;
	}
	if (canput(playerx + (playery + 1) * 150)) {
		*x = playerx;
		*y = playery + 1;
		return 1;
	}
	if (canput(playerx - 1 + (playery + 1) * 150)) {
		*x = playerx - 1;
		*y = playery + 1;
		return 1;
	}
	if (canput(playerx + 1 + (playery + 1) * 150)) {
		*x = playerx + 1;
		*y = playery + 1;
		return 1;
	}
	if (canput(playerx - 1 + playery * 150)) {
		*x = playerx - 1;
		*y = playery;
		return 1;
	}
	if (canput(playerx + 1 + playery * 150)) {
		*x = playerx + 1;
		*y = playery;
		return 1;
	}
	if (canput(playerx - 1 + (playery - 1) * 150)) {
		*x = playerx - 1;
		*y = playery - 1;
		return 1;
	}
	if (canput(playerx + (playery - 1) * 150)) {
		*x = playerx;
		*y = playery - 1;
		return 1;
	}
	if (canput(playerx + 1 + (playery - 1) * 150)) {
		*x = playerx + 1;
		*y = playery - 1;
		return 1;
	}
	*x = playerx;
	*y = playery + 1;
	return 1;
}

int
clicktarget(short m)
{
	char *p;

	lbltext[0] = 0;
	if (ctbusy)
		return 0;
	if (unconscious())
		return 0;
	ctbusy = 1;
	if (hoverkind >= 0x2b && hoverkind <= 0x30 && hovercx != 0 && hovercx != 999) {
		raiseitem(panels[hoverkind - 0x28].id, hovercx);
		hovercx = 0;
		picktarget();
	}
	if (!menumode)
		cleartexts();
	if ((targetitem & 0xfff) == 0) {
	cancel:
		ctbusy = 0;
		return 0;
	}
	anchortip(targetkind, hoverx, hovery);
	if (m == 4 || (keys[VK_SHIFT] & 0x80) && m == 0 && playmode == 1 && !unconscious()) {
		plotfind(targetkind, targetx, targety, targetitem);
		ctkind = targetkind;
		ctx = targetx;
		cty = targety;
		dropsnd = 0;
		clicktime = 0;
		leftclick = 0;
		dblclick = 0;
		if (ctkind == 0x29 || ctkind == 0x2b) {
			hovercx = 0xff;
			hovercy = 0xff;
			if (trademode) {
				hoverkind = 0xda;
				for (hovercx = 0; tradeoffer[hovercx]; )
					if (++hovercx >= 24)
						goto fail;
				hovercy = 0;
			} else if (isopen(4))
				hoverkind = 0x2c;
			else if (isopen(5))
				hoverkind = 0x2d;
			else if (isopen(6))
				hoverkind = 0x2e;
			else if (isopen(7))
				hoverkind = 0x2f;
			else if (isopen(8))
				hoverkind = 0x30;
			else {
				hoverkind = 1;
				dropcell(&hovercx, &hovercy);
			}
		} else if (ctkind == 1 || ctkind == 0x2a
		|| ctkind >= 0x2c && ctkind <= 0x30 || targetkind == 0xda) {
			hovercx = playerx;
			hovercy = playery;
			hoverkind = 1;
		} else {
			ctbusy = 0;
			return -124;
		}
		if (targetkind != 1)
			;
		else if (classof(targetitem & 0xfff) == 0x15) {
		busy:
			ctbusy = 0;
			return -123;
		}
		ctres = pickup();
		mousemoved = 1;
		if (!ctres)
			goto fail;
		if (ctres == 4)
			goto busy;
		if (ctres == 3)
			;
		else {
			ctres = putheld(0, 1);
			mousemoved = 1;
			if (ctres == 1)
				goto fail;
			targetkind = ctkind;
			targetx = ctx;
			targety = cty;
		}
		placeitem();
	fail:
		ctbusy = 0;
		return -124;
	}
	ctcls = classof(targetitem & 0xfff);
	switch (ctcls) {
	case 1:
		if (!m) {
			if (targetkind != 1)
				break;
			if (!outofreach(targetkind, targetx, targety, targetitem, 2, 0)) {
				if (targetitem & 0x10000000)
					closedoor(targety * 150 + targetx, 1);
				else {
					if (cheat && (keys['W'] & 0x80))
						;
					else if (targetitem & 0x8000000) {
						cleartip();
						denymsg(strs[271], itemname(targetitem));
						ctbusy = 0;
						return 0;
					}
					opendoor(targety * 150 + targetx, cheat && (keys['W'] & 0x80) ? 1 : 0);
					plotdoor(curmap, targetx, targety);
				}
				playqueue();
				cleartexts();
				lookaround();
				calctorch();
				ctbusy = 0;
				return 0;
			}
		}
		goto name;
	case 0x69:
		if (!m && dig(0))
			goto cancel;
		goto name;
	case 0xf:
	case 0x10:
		lblx = tipanchorx;
		lbly = tipanchory;
		cointext(lbltext, targetitem);
		break;
	case 0x1e:
		if (!m && readitem(0))
			break;
		goto name;
	case 0x14:
		lblx = tipanchorx;
		lbly = tipanchory;
		wsprintf(lbltext, unitname(&units[targetidx]));
		break;
	case 0x36:
		if (ageof(targetitem) >= 4) {
			lblx = tipanchorx;
			lbly = tipanchory;
			wsprintf(lbltext, strs[1176]);
			break;
		}
	default:
	name:
		lblx = tipanchorx;
		lbly = tipanchory;
		if (*typetext(targetitem) == 0)
			lbltext[0] = 0;
		else
			wsprintf(lbltext, itemname(targetitem));
		break;
	}
	if (trademode && lbltext[0] && targetcls != 15) {
		for (p = lbltext; *p; p++)
			;
		ctres = silvvalue(targetitem);
		if (ctres > 1)
			;
		else {
			ctres = fracchar(valueof(targetitem) * 10);
			if (ctres == '/')
				goto show;
			if (ctres != '1') {
				wsprintf(p, strs[1158], ctres);
				goto show;
			}
			ctres = 1;
		}
		wsprintf(p, strs[1157], ctres);
	} else if (!trademode && !talkactive && !m && ctcls)
		clickhint(targetitem, ctcls);
show:
	titlecase(lbltext);
	ctbusy = 0;
	return 0;
}

short
checklock(unsigned long item, unsigned char t, int i, short c)
{
	if (t == 10)
		clword = &realms[idof(item)].item;
	else
		clword = &mapitems[i];
	if (!c && idof(*clword) == 0) {
		denymsg(strs[275], itemname(item));
		return 0;
	}
	if (t == 10)
		;
	else if (*clword & 0x10000000) {
		denymsg(strs[293], itemname(item));
		return 0;
	}
	if (*clword & 0x8000000)
		return 1;
	denymsg(strs[358], itemname(item));
	return 0;
}

short
forcelock(unsigned long item, unsigned char t, int n, short k)
{
	flsnd = 0;
	if (!checklock(item, t, n, k))
		return 0;
	if (t == 10)
		flword = &realms[idof(item)].item;
	else
		flword = &mapitems[n];
	*flword &= ~0x1000;
	if (ownerat(targetx, targety)
	&& !(k == 0 && sanctioned(0x18, targetkind, targetx, targety, item))) {
		if (k)
			witness(item, t, 90);
		else
			witness(item, t, 15);
	}
	if (k) {
		if (k != 2) {
			tire(1);
			train(11, 1);
			train(weaponskill(), 1);
		} else {
			tire(15);
			train(11, 15);
		}
		flsnd = 18;
	} else {
		tire(1);
		train(20, 1);
		train(26, 5);
		flsnd = 0;
	}
	if ((*flword & 0x4000000) && (*flword & 0x2000000)) {
		if (flsnd)
			queuesound(flsnd);
		if (k == 1)
			weargear(0, units, &units[0].hand, 150, 1);
		msgf(99, strs[359], itemname(*flword));
		return 1;
	}
	flhard = 0;
	if (*flword & 0x4000000)
		flhard = 30;
	if (*flword & 0x2000000)
		flhard += 60;
	if (k == 2) {
		if (randn(units[0].strength + units[0].dexterity) < flhard) {
			flhard = 0;
		fail:
			msgf(99, strs[1393], itemname(item));
			goto award;
		}
		flhard = 7;
		goto open;
	}
	if (!k)
		goto force;
	attackval(units, &flhit, &flkind, &fldmin, &fldmax, 1);
	flhit = randn(fldmax - fldmin) + fldmin;
	if ((short)ismassive(item) == 1) {
		msgf(99, strs[1392], itemname(item));
		flhit *= 15;
		flhit /= 10;
		goto award;
	}
	if (randn(100) >= 7 && flhit < flhard + 20) {
		flhard = 0;
		goto fail;
	}
	train(11, 2);
	train(weaponskill(), 2);
	train(20, 1);
	flhard = wearof(*flword);
	if (flhard < 7)
		flhard++;
open:
	setwear(flword, flhard);
	if (flhard == 7) {
		if (t == 1) {
			fltype = *flword & 0xfff;
			*flword &= 0xfffff000;
			*flword |= breakdoor(fltype);
			if ((flidx = findclass(targetx - 1 + targety * 150, 2)) != 0) {
				fltype = mapitems[flidx] & 0xfff;
				mapitems[flidx] &= 0xfffff000;
				mapitems[flidx] |= breakdoor(fltype);
			} else if ((flidx = findclass(targetx + (targety - 1) * 150, 3)) != 0) {
				fltype = mapitems[flidx] & 0xfff;
				mapitems[flidx] &= 0xfffff000;
				mapitems[flidx] |= breakdoor(fltype);
			}
		}
		flsnd = 19;
		msgf(99, strs[1394], itemname(item));
		music(22, 0);
	} else
		msgf(99, texts + 0xb318, itemname(item), breaktext(flhard));
award:
	if ((units[0].hand & 0xfff) == 0x1ce)
		weargear(0, units, &units[0].hand, flhit, 1);
	else
		for (flidx = 0; flidx < 4; flidx++)
			weargear(0, units, &units[0].hand, flhit * 100, 1);
	if (flhard == 7) {
	unlocked:
		*flword &= ~0x8000000;
		if (targetkind == 1)
			updatecell(targetx + targety * 150);
		if (flsnd)
			queuesound(flsnd);
		return 2;
	}
	goto done;
force:
	if (randn(100) < 7
	|| prevails(skillof(units, 26, 0, 0, 0), flhard + 80)
	|| cheat && (keys['W'] & 0x80)) {
		msgf(99, texts + 0x9dea, itemname(item), strs[295]);
		flsnd = 21;
		music(22, 0);
		goto unlocked;
	}
	msgf(99, strs[436]);
done:
	if (flsnd)
		queuesound(flsnd);
	return 1;
}

int
tobelt(int v)
{
	for (tobi = 0; tobi < 3; tobi++)
		if (units[0].items[tobi] == 0) {
			units[0].items[tobi] = v;
			return 1;
		}
	return 0;
}

int
forceopen(short k)
{
	if (targetcls == 1) {
		if (!(targetitem & 0x10000000)) {
			if (targetitem & 0x8000000) {
			locked:
				fores = forcelock(targetitem, targetcls, targetidx, k);
				if (!(fores == 2)) {
					if (fores)
						return 1;
					return 0;
				}
			}
			opendoor(targety * 150 + targetx, 0);
			plotdoor(curmap, targetx, targety);
			lookaround();
			calctorch();
			lookdone();
			drawscreen();
			return 1;
		}
	} else if (targetcls == 4) {
		if (!(targetitem & 0x10000000) && (targetitem & 0x8000000))
			goto locked;
	} else if (targetcls == 10) {
		if (forcelock(targetitem, targetcls, targetidx, k))
			return 1;
	}
	return 0;
}

int
attacktarget(void)
{
	if (!menumode)
		cleartexts();
	if (unconscious()) {
		notetarget();
		goto done;
	}
	atpick = picktarget();
	if (atpick == 3)
		goto done;
	if (atpick == 0)
		goto cancel;
	if ((targetkind == 0x29 || targetkind == 0x2a) && panels[targetkind - 0x28].id == 0) {
		if (targetx == 20) {
			nearfoe(0);
			if (playmode == 3 && !nofoes) {
				denymsg(strs[316]);
				drawscreen();
				notetarget();
				drawscreen();
			} else {
				togglecombat(0);
				notetarget();
				drawscreen();
			}
			goto done;
		}
		if (targetx == 21 && units[0].belt != 0) {
			atthrow = units[0].belt;
			units[0].belt = 0;
			wield(-1, 0, 0, atthrow);
		found:
			calcstats(units);
			return 1;
		}
	}
	if (targetkind != 1 && hovergear == 0) {
		denycombat();
		goto cancel;
	}
	if (atpick == 2) {
		atdx = units[0].x - units[targetidx].x;
		atdy = units[0].y - units[targetidx].y;
		atdx = abs(atdx);
		atdy = abs(atdy);
		if (isthrowing(units[0].hand) && (atdx > 1 || atdy > 1)) {
			if (sparring(units, &units[targetidx])) {
				denypractice();
				goto cancel;
			}
			if (ptrfar)
				goto toofar;
			if (!ptrblocked)
				goto use;
		busy:
			denymsg(strs[437]);
			notetarget();
			goto fail;
		}
		if (atdx > 1 || atdy > 1) {
		toofar:
			denymsg(strs[854]);
			goto cancel;
		}
	use:
		if (askattack())
			return 2;
		strike(units, &units[targetidx]);
		return 1;
	}
	atdx = units[0].x - targetx;
	atdy = units[0].y - targety;
	atdx = abs(atdx);
	atdy = abs(atdy);
	atcls = classof(targetitem & 0xfff);
	if (!hovergear) {
		if (atcls == 1 || atcls == 4) {
			if (atcls == 1
			&& outofreach(targetkind, targetx, targety, targetitem, 2, 0))
				goto toofar;
			if (!(targetitem & 0x10000000) && (targetitem & 0x8000000)
			&& (atdx > 1 || atdy > 1))
				goto toofar;
		} else if (atcls == 0x69) {
			if (atdx > 1 || atdy > 1)
				goto fail;
		} else if (atcls == 0x98) {
			if (isthrowing(units[0].hand) && (atdx > 1 || atdy > 1)) {
				if (ptrfar)
					goto toofar;
				if (ptrblocked)
					goto busy;
				throwat(targetx, targety);
				return 1;
			}
			usefail(strs[1562]);
			goto fail;
		} else if (atdx > 1 || atdy > 1)
			goto fail;
	}
	if ((hovergear != 0
	|| (atcls == 1 || atcls == 4) && (targetitem & 0x10000000) == 0 && (targetitem & 0x8000000)
	|| atcls == 10 && (realms[idof(targetitem)].item & 0x8000000))
	&& asksteal(0))
		return 2;
	notetarget();
	if (hovergear != 0) {
		liftitem(0);
		hovergear &= ~0x1000;
		if (atcls == 0x55) {
			if (units[0].offhand != 0) {
				giveitem(0, units[0].offhand, 1);
				units[0].offhand = 0;
			}
			putonfree(0, hovergear);
		} else if (atcls == 0x3c)
			wield(targetkind, targetx, targety, hovergear);
		goto found;
	}
	switch (atcls) {
	case 1:
	case 4:
	case 10:
		if (forceopen(1))
			return 1;
		break;
	case 0x69:
		if (dig(0))
			return 1;
		break;
	}
	goto fail;
cancel:
	notetarget();
fail:
	playnow(0x2c);
	setcursor1(3, 4);
	mousemoved = 1;
	menumode = 1;
	drawscreen();
done:
	notetarget();
	waitmouseup(0);
	menumode = 0;
	mousemoved = 1;
	readinput();
	return 0;
}

int
sackout(unsigned long *item)
{
	soold = idof(*item);
	for (sonew = 5; sonew < nrealms + 5; sonew++)
		if (realms[sonew].item == 0)
			goto found;
	// FIXED: with 120 containers the new realm lands past realms[125]. Leave the item in its realm.
#ifdef FEATURE_FAULTS
	if (nrealms + 5 >= 125)
		return 0;
#endif
	sonew = nrealms + 5;
	nrealms++;
found:
	memcpy(&realms[sonew], &realms[soold], sizeof(Realm));
	realms[soold].item = 0;
	setid(item, sonew);
	return 1;
}

int
sackin(unsigned long *item)
{
	siold = idof(*item);
	for (sinew = 1; realms[sinew].item != 0; )
		if (++sinew >= 5)
			return 0;
	memcpy(&realms[sinew], &realms[siold], sizeof(Realm));
	realms[siold].item = 0;
	setid(item, sinew);
	return 1;
}

short
overload(void)
{
	if (trademode)
		return 0;
	if (units[0].load + itemweight(targetitem) <= units[0].strength * 50)
		return 0;
	return 1;
}

short
splitcoins(unsigned short n)
{
	spccls = classof(targetitem & 0xfff);
	if (spccls != 0xf && spccls != 0x10)
		return 0;
	spcn = targetitem >> 16;
	if (n >= spcn)
		return 0;
	asknum(&spcn, n, spcn);
	if (spcn == 0)
		return 1;
	if (spcn != targetitem >> 16) {
		spcpart = spcn << 16;
		targetitem -= spcpart;
		addcoins(&targetitem, 0);
		placeitem();
		targetitem &= 0xffff;
		targetitem |= spcpart;
		addcoins(&targetitem, 0);
	}
	return 0;
}

short
attireslot(unsigned long t)
{
	switch (t) {
	case 1:
	case 3:
	case 6:
	case 7:
	case 0xf:
	case 0x19:
	case 0x1e:
		return 1;
	}
	return 0;
}

int
pickup(void)
{
	if (targetitem == 0)
		return 0;
	cleartexts();
	lbltext[0] = 0;
	pucls = classof(targetitem & 0xfff);
	if (itemweight(targetitem) >= 65000) {
		if (pucls == 1 || pucls == 4 || pucls == 10
		|| pucls == 20 || pucls == 25 || pucls == 0x28
		|| pucls == 0x2e || pucls == 0x32 || pucls == 0x78
		|| pucls == 0x79 || pucls == 0x7a || pucls == 0x7b
		|| pucls == 0x82 || pucls == 0x83 || pucls == 0x84
		|| pucls == 0x85 || pucls == 0x86 || pucls == 0x69
		|| pucls == 0x5a || pucls == 0x59 || pucls == 0x1e)
			return 4;
		if (pucls == 0x97)
			return 4;
		if (pucls == 0x96)
			return 4;
		if (pucls == 0xc8)
			return 4;
		if (pucls == 0xc9)
			return 4;
		if (pucls == 0xca)
			return 4;
		if (pucls == 0xcb)
			return 4;
		if (pucls == 0xd1)
			return 4;
		if (pucls == 0xcc)
			return 4;
		if (pucls == 0xcd)
			return 4;
		if (pucls == 0xce)
			return 4;
		if (pucls == 0xcf)
			return 4;
		if (pucls == 0xd0)
			return 4;
		if (pucls == 0x69 && typenames[targetitem & 0xfff])
			return 4;
		if (pucls == 0x6a && typenames[targetitem & 0xfff] == 0x4b)
			return 4;
		if (pucls == 0x37 || pucls == 0x98)
			return 4;
		return 0;
	}
	if (pucls == 10)
		return 0;
	if (trademode && lockedslots && targetkind == 0x29) {
		if (targetx == 1 && (lockedslots & 8))
			return 0;
		if (targetx == 0xf && (lockedslots & 4))
			return 0;
		if (targetx == 10 && (lockedslots & 0x10))
			return 0;
		if (targetx == 20 && (lockedslots & 1))
			return 0;
		if ((targetx == 0x15 || targetx == 20) && (lockedslots & 2))
			return 0;
		if (targetx == 6 && (lockedslots & 0x40))
			return 0;
		if (targetx == 7 && (lockedslots & 0x20))
			return 0;
	}
	if (outofreach(targetkind, targetx, targety, targetitem, 2, 1))
		return 0;
	if ((targetitem & 0xfff) == 0)
		return 0;
	if (pucls == 20)
		return 0;
	if (playerout() || playmode != 1)
		return 0;
	if (asksteal(0))
		return 0;
	if (!liftitem(0))
		return 0;
	targetitem &= ~0x1000;
	if (targetkind == 0x29 || targetkind == 0x2a) {
		if (attireslot(targetx)) {
			if (!muted())
				playnow(4);
			puunit = panels[targetkind - 0x28].id;
			if (puunit && units[puunit].health <= 0
			&& !yields(&units[puunit], 1, targetitem, 0)) {
				puwcls = classof(targetitem & 0xfff);
				if (classinpack(0, 0, puwcls)
				|| !(puwcls == 0x46 && !units[0].body
				|| puwcls == 0x4b && !units[0].legs
				|| puwcls == 0x50 && !units[0].feet)) {
					msgf(99, strs[1286]);
					train(20, -20);
					train(22, -20);
					train(23, -20);
					addfavor(-50);
				}
			}
		}
	}
	addscore(1);
	return 1;
}

int
putheld(unsigned char a, int b)
{
	unsigned short n, k;
	unsigned char t;
	short s;
	unsigned long pt0;

	didone = 0;
	digift = 0;
	diunit = 0xff;
	if ((targetitem & 0xfff) == 0)
		return 3;
	dicls = classof(targetitem & 0xfff);
	switch (hoverkind) {
	case 1:
		targetitem = randlook(targetitem);
		if (hovercx == playerx && hovercy == playery) {
			if (dicls == 10 || dicls == 0x15) {
				denymsg(strs[296]);
				if (a == 0)
					return 999;
				return 3;
			}
			if (overload()) {
				denymsg(strs[817]);
				if (a == 0)
					return 999;
				return 3;
			}
			diword = giveitem(0, targetitem, 0);
			if (diword & 0xfff) {
				targetitem = diword;
				denymsg(strs[545]);
				if (a == 0)
					return 999;
				return 3;
			}
			didone = 1;
			lookaround();
		} else {
			playerstep(hovercx, hovercy);
			diunit = unitat(hovercx, hovercy);
			if (diunit == 0xff && deepwater(hovercx, hovercy)) {
				if (!clearsight(playerx, playery, hovercx, hovercy)) {
					anchortip(1, hoverx, hovery);
					denymsg(strs[854]);
					if (a == 0)
						return 999;
					return 3;
				}
				deedsink(targetitem);
				didone = 0;
			} else {
				if (cansee(hovercx, hovercy) == 0) {
					if (a == 0)
						return 999;
					return 3;
				}
				ditop = topitem(hovercy * 150 + hovercx);
				if (ditop)
					ditop = mapitems[ditop];
				if (outofreach(hoverkind, hovercx, hovercy, ditop, 2, 1)) {
					if (a == 0)
						return 999;
					return 3;
				}
				if (b) {
					diunit = 0xff;
					dicls = classof(targetitem & 0xfff);
				} else {
					if (diunit != 0xff && dicls != 0x15) {
						if (units[diunit].mode == 0x5a || units[diunit].asleep
						|| isanimal(&units[diunit])) {
							if (a == 0)
								return 999;
							return 3;
						}
						dicls = classof(targetitem & 0xfff);
						if (dicls == 0xf || dicls == 0x10) {
							k = targetitem >> 16;
							if (k > 10) {
								asknum(&k, 10, k);
								if (k == 0) {
									if (a == 0)
										return 999;
									return 3;
								}
								if (targetitem >> 16 != k) {
									diword = k << 16;
									targetitem -= diword;
									addcoins(&targetitem, 0);
									placeitem();
									targetitem &= 0xffff;
									targetitem |= diword;
									addcoins(&targetitem, 0);
								}
							}
						}
						cleartexts();
						if (units[diunit].sympathy <= 1
						&& !(isservant(&units[diunit])
						&& units[units[diunit].lord].health > 0
						&& units[units[diunit].lord].sympathy > 1)
						|| dicls == 0x36 && ageof(targetitem) >= 4) {
							if (units[diunit].sympathy <= 1)
								say(&units[diunit], 1, 0x579);
							else
								say(&units[diunit], 1, 0x578);
							drawscreen();
							if (a == 0)
								return 999;
							return 3;
						}
						notepoison(diunit);
						clearqueue();
						giveitem(diunit, targetitem, 1);
						if (nqueue) {
							dropsnd = 1;
							if (!muted())
								playqueue();
						}
						checkpoison(diunit);
						ditop = valuefor(targetitem, &units[diunit]) / 10;
						if (isservant(&units[diunit])) {
							if (ditop >= 50) {
								if (isfemale(&units[diunit])) {
									if (units[diunit].sympathy >= 1) {
										units[diunit].sympathy = 3;
										units[diunit].traits &= 0xdf;
									}
								} else if (units[diunit].sympathy >= 1) {
									units[diunit].sympathy = 3;
									units[diunit].traits &= 0xdf;
								}
							} else if (ditop > 5) {
								if (units[diunit].sympathy == 2)
									units[diunit].sympathy = 3;
								else if (units[diunit].sympathy == 1)
									units[diunit].sympathy = 2;
							}
						} else if (ditop >= 50) {
							if (units[diunit].sympathy == 2)
								units[diunit].sympathy = 3;
							else if (units[diunit].sympathy == 1)
								units[diunit].sympathy = 2;
						}
						say(&units[diunit], 0, 0x575);
						drawscreen();
						if (!dropsnd && !muted())
							playnow(2);
						didone = 0;
						digift = 1;
						goto put;
					}
					dicls = classof(targetitem & 0xfff);
					if (dicls == 5) {
						savetarget();
						if (picktarget() == 1) {
							t = classof(targetitem & 0xfff);
							if (t == 1 || t == 10) {
								usekey(savtitem);
								restoretarget();
								if (a == 0)
									return 999;
								return 3;
							}
						}
						restoretarget();
					}
					if (dicls == 0x3a) {
						dilook = targetitem;
						savetarget();
						if (picktarget() == 1) {
							t = classof(targetitem & 0xfff);
							if (t == 0x82 || t == 0x83 || t == 0x84 || t == 0x85 || t == 0x86) {
								fillvessel(&dilook, 0);
								restoretarget();
								targetitem = dilook;
								return 2;
							}
							if (targetkind == 1 && ispond(targetitem & 0xfff)
							&& (s = waterat(targetx, targety))) {
								fillvessel(&dilook, s);
								restoretarget();
								targetitem = dilook;
								return 2;
							}
						}
						restoretarget();
					}
					if (findclass(hovercy * 150 + hovercx, 0x96)) {
						grind();
						dropsnd = 1;
						return 999;
					}
				}
				ditop = 0;
				dicell = hovercy * 150 + hovercx;
				if (canput(dicell) == 0) {
					if (iswallns(map[dicell])
					&& (playerx > hovercx || ditop)) {
						dilook = eastlook(targetitem);
						if (dilook) {
						found:
							targetitem = dilook;
							didone = 1;
							goto put;
						}
					} else if (iswallew(map[dicell])
					&& (playery > hovercy || ditop)) {
						dilook = southlook(targetitem);
						if (dilook)
							goto found;
					}
					if (ditop == 0) {
						if (a == 0)
							return 999;
						return 3;
					}
				}
				didone = 1;
			}
		put:
			dicls = classof(targetitem & 0xfff);
			if (dicls == 0x66)
				lefttime = 0;
			if (diunit == 0xff && (dicls == 0xf || dicls == 0x10)) {
				n = targetitem >> 16;
				if (n > 10) {
					asknum(&n, 10, n);
					if (n == 0) {
						if (a == 0)
							return 999;
						return 3;
					}
					if (n != targetitem >> 16) {
						diword = n << 16;
						targetitem -= diword;
						addcoins(&targetitem, 0);
						placeitem();
						targetitem &= 0xffff;
						targetitem |= diword;
						addcoins(&targetitem, 0);
					}
				}
				if (didone == 0) {
					if (dicls == 0x66) {
						if (a == 0)
							return 999;
						return 3;
					}
					didone = 1;
					if (digift == 0) {
						if (!muted())
							playnow(8);
						dropsnd = 1;
					}
					goto out;
				}
				diword = topitem(hovercy * 150 + hovercx);
				if (diword && canstack(mapitems[diword], targetitem)
				&& addcoins(&mapitems[diword], targetitem))
					goto out;
			}
			if (didone == 0) {
				didone = 1;
				if (digift == 0) {
					if (!muted())
						playnow(8);
					dropsnd = 1;
				}
			} else {
				additem(hovercy * 150 + hovercx, targetitem);
				updatecell(hovercy * 150 + hovercx);
			}
			if (b != 0)
				;
			else if (findclass(hovercy * 150 + hovercx, 0x2e)) {
				hasaction(0x12);
				picktarget();
				doaction(1);
			}
			if (islittorch(targetitem)) {
				listtorches();
				lookaround();
				drawscreen();
			}
			lookdone();
			goto out;
		}
		break;
	case 0x2b:
	case 0x2c:
	case 0x2d:
	case 0x2e:
	case 0x2f:
	case 0x30:
		dicell = panels[hoverkind - 0x28].id;
		if (!trademode && (classof(targetitem & 0xfff) == 10
		|| classof(targetitem & 0xfff) == 0x15)) {
			denymsg(strs[296]);
			if (a == 0)
				return 999;
			return 3;
		}
		if (targetkind != hoverkind && !(dicell < 5 && targetkind == 0x29)
		&& splitcoins(targetkind == 0xda ? 1 : 10)) {
			if (a == 0)
				return 999;
			return 3;
		}
		if (!trademode && nofit(targetitem, dicell)) {
			denymsg(strs[545]);
			if (a == 0)
				return 999;
			return 3;
		}
		if (!trademode && dicell < 5 && overload()) {
			denymsg(strs[817]);
			if (a == 0)
				return 999;
			return 3;
		}
		if (b)
			diword = stowspot(dicell, 0xff, 0xff, targetitem & 0xfff);
		else
			diword = stowspot(dicell,
				hoverpx > 16 ? hoverpx - 16 : 0,
				hoverpy > 16 ? hoverpy - 16 : 0,
				targetitem & 0xfff);
		didone = stowat(dicell, diword, targetitem);
		if (didone == 0) {
			denymsg(strs[545]);
			if (a == 0)
				return 999;
			return 3;
		}
		if (dicell < 5) {
			if (classof(targetitem & 0xfff) == 10 && !sackin(&targetitem)) {
				if (a == 0)
					return 999;
				return 3;
			}
			calcstats(units);
		}
		goto out;
	case 0x29:
	case 0x2a:
		dicls = classof(targetitem & 0xfff);
		if (dicls == 0x15) {
		fail:
			if (a == 0)
				return 999;
			return 3;
		}
		dicell = panels[hoverkind - 0x28].arg;
		if (dicell == 0 || dicell == 2) {
			if (a == 0)
				return 999;
			return 3;
		}
		dicell = panels[hoverkind - 0x28].id;
		ditop = classof(targetitem & 0xfff);
		if (dicell == 0 && trademode && lockedslots && hoverkind == 0x29) {
			if (ditop == 0x41 && (lockedslots & 8))
				goto fail;
			if (ditop == 0x46 && (lockedslots & 4))
				goto fail;
			if ((ditop == 0x55 || ditop == 0x56) && (lockedslots & 0x10))
				goto fail;
			if (ditop == 0x3c && (lockedslots & 1)
			&& (!units[0].hand || !isthrowing2(units[0].hand) || isthrowing2(targetitem)))
				goto fail;
			if (ditop == 0x3c && (lockedslots & 2) && units[0].hand
			&& !isthrowing2(targetitem) && isthrowing2(units[0].hand))
				goto fail;
			if (ditop == 0x63 && (lockedslots & 0x40))
				goto fail;
			if (ditop == 0x62 && (lockedslots & 0x20))
				goto fail;
		}
		if (dicell == 0 && isfood(targetitem)
		&& onhead(hoverpx, hoverpy) && consume(units, targetitem, 2)) {
			if (dicls == 0x87 || dicls == 0x88 || dicls == 0x89
			|| dicls == 0x8a || dicls == 0x8b || dicls == 0x38) {
				emptyvessel(&targetitem);
				return 2;
			}
			return 1;
		}
		if (dicell == 0 && overload()) {
			denymsg(strs[817]);
			if (a == 0)
				return 999;
			return 3;
		}
		targetitem = baselook(targetitem);
		didone = panelgear(hoverkind, hovercx);
		if (hovercx >= 0x28 && hovercx <= 0x2a) {
			if (targetkind != hoverkind && !(dicell == 0 && targetkind == 0x2b)
			&& splitcoins(targetkind == 0xda ? 1 : 10)) {
				if (a == 0)
					return 999;
				return 3;
			}
			dicls = classof(targetitem & 0xfff);
			if (canstack(targetitem, didone)) {
				addcoins(&targetitem, didone);
				didone = 0;
			}
			if (islittorch(targetitem))
				setlit(&targetitem, 0);
			*gearslot(&units[dicell], hovercx) = targetitem;
		} else if (targetkind == hoverkind && targetx == 0x15
		&& units[dicell].hand && !trademode) {
			didone = units[dicell].hand;
			units[dicell].hand = baselook(targetitem);
		} else {
			if (dicell == 0 && unmanly(targetitem, 1))
				didone = 999;
			else
				didone = puton(dicell, targetitem);
			if (didone == 999) {
				if (a == 0)
					return 999;
				return 3;
			}
			dicls = classof(targetitem & 0xfff);
			if (dicls == 0x46 || dicls == 0x48 || dicls == 0x63
			|| dicls == 0x62 || dicls == 0x41 || dicls == 0x4b
			|| dicls == 0x50) {
				if (!muted())
					playnow(3);
				dropsnd = 1;
			}
		}
		calcstats(&units[dicell]);
		if (didone == 0)
			didone = 1;
		else {
			targetx = hovercx;
			targety = hovercy;
			targetkind = hoverkind;
			targetitem = didone;
			didone = 2;
			setcursor1(2, targetitem & 0xfff);
		}
		lookaround();
		break;
	case 0xda:
		if (hovercx == 999) {
			if (a == 0)
				return 999;
			return 3;
		}
		if (hoverkind != targetkind && splitcoins(1)) {
			if (a == 0)
				return 999;
			return 3;
		}
		targetitem = baselook(targetitem);
		pt0 = tradeoffer[hovercx];
		dicell = pt0;
		dicls = classof(targetitem & 0xfff);
		if (canstack(targetitem, dicell)) {
			addcoins(&targetitem, dicell);
			dicell = 0;
		}
		tradeoffer[hovercx] = targetitem;
		if (dicell == 0) {
			tradeoffer[hovercx] = targetitem;
			didone = 1;
		} else {
			targetx = hovercx;
			targety = hovercy;
			targetkind = hoverkind;
			targetitem = dicell;
			setcursor1(2, targetitem & 0xfff);
			didone = 2;
		}
		goto out;
	default:
		if (a == 0)
			return 999;
		return 3;
	}
	lookdone();
out:
	mousemoved = 1;
	if (didone && !dropsnd && !muted()) {
		queuesound(2);
		playqueue();
	}
	calcstats(units);
	return didone;
}

int
dragitem(unsigned char a, unsigned char b)
{
	dropsnd = 0;
	dgdrop = 0;
	dgpick = 0;
	if (unconscious())
		return 0;
	actsel = 0;
	if (!a) {
		readinput();
		starttimer3();
		dgx = hoverx;
		tracesite = 10;
		dgy = hovery;
		do {
			if (!trademode && playmode == 1 && units[0].health > 0)
				gametick();
			else {
				readinput();
				if (needredraw(0xfd))
					drawscreen();
			}
			if (!mouseleft || playerout() || playmode != 1) {
				showlabel();
				forgettarget();
				return 0;
			}
			if (abs(mousex - dgx) >= 6 || abs(mousey - dgy) >= 6)
				goto moved;
		} while (elapsed3() < 250);
		plotfind(targetkind, targetx, targety, targetitem);
		if (useinfo())
			showinfo(1);
		else {
			clicktarget(1);
			cleartexts();
			showlabel();
		}
		drawscreen();
		for (;;) {
			if (!trademode && playmode == 1 && units[0].health > 0)
				gametick();
			else {
				readinput();
				if (needredraw(0xfd))
					drawscreen();
			}
			if (mouseleft == 0 || playerout() || playmode != 1) {
				showinfo(0);
				drawcursor();
				forgettarget();
				return 0;
			}
			if (abs(mousex - dgx) >= 16 || abs(mousey - dgy) >= 16)
				break;
		}
		showinfo(0);
	moved:
		clicktime = 0;
		drawcursor();
		dgpick = pickup();
		if (dgpick != 1)
			return dgpick;
	}
	for (;;) {
		dropsnd = 0;
		units[0].pose = 2;
		dgpick = 0;
		setcursor1(2, targetitem & 0xfff);
		shown = 1;
		menumode = 1;
		mousemoved = 1;
		lookaround();
		drawscreen();
		lookdone();
		if ((targetitem & 0xfff) == 0)
			return 3;
		if (!a) {
			while (mouseleft == 0) {
				if (!trademode && playmode == 1 && units[0].health > 0)
					gametick();
				else {
					readinput();
					if (needredraw(0xfd))
						drawscreen();
				}
				if (esckey) {
					if (b)
						return 3;
					if (esckey != 0xff)
						esckey = 0;
				}
				if (playerout())
					return 3;
				if (playmode != 1)
					return 3;
			}
		}
		leftclick = 0;
		dgx = 0;
		dgy = 0;
		do {
			if (!trademode) {
				if (playmode != 1 || units[0].health <= 0) {
					readinput();
					if (needredraw(0xfd))
						drawscreen();
				} else
					gametick();
				if (hovery <= 0x1c8)
					dgy = 0;
				else if (hovery > 0x1c8 && dgy == 0) {
					dgy = 1;
					if (statshown == 0) {
						statshown = 1;
						hoversound();
					} else
						statshown = 0;
					drawscreen();
				}
				if (hoverx < 0x262)
					dgx = 0;
				else if (hoverx >= 0x266 && dgx == 0) {
					dgx = 1;
					togglehero();
					mousemoved = 1;
				}
			} else
				readinput();
			if (esckey) {
				if (b)
					return 3;
				if (esckey != 0xff)
					esckey = 0;
			}
			if (playerout())
				return 3;
			if (playmode != 1)
				return 3;
		} while (a ? !mouseleft : mouseleft);
		leftclick = 0;
		dgdrop = putheld(b, 0);
		if (!(dgdrop == 999))
			return dgdrop;
		drawscreen();
		while (mouseleft) {
			if (!trademode && playmode == 1 && units[0].health > 0)
				gametick();
			else {
				readinput();
				if (needredraw(0xfd))
					drawscreen();
			}
		}
		leftclick = 0;
	}
}

int
contpanel(int t)
{
	if (t >= 0x2b && t <= 0x30)
		return 1;
	return 0;
}

int
dragclick(void)
{
	dcres = 0;
	dcret = 1;
	dcmenu = menumode;
	savecursor();
	switch (dragitem(0, 1)) {
	case 0:
		units[0].pose = 0;
		break;
	case 1:
		if (playmode != 1)
			units[0].pose = 9;
		else
			units[0].pose = 0;
		break;
	case 2:
		if (playerout() || playmode != 1) {
		home:
			targetx = playerx;
			targetkind = 1;
			targety = playery;
			placeitem();
		clear:
			clicktime = 0;
			units[0].pose = 0;
			break;
		}
		do {
			if (playmode != 1)
				busyturns++;
			drawscreen();
			tracesite = 11;
			while (mouseleft) {
				if (!trademode && playmode == 1 && units[0].health > 0)
					gametick();
				else {
					readinput();
					if (needredraw(0xfd))
						drawscreen();
				}
			}
			leftclick = 0;
			clicktime = 0;
			dcres = dragitem(1, 0);
		} while (dcres == 2);
		if (dcres == 3)
			goto home;
		goto clear;
	case 3:
		mousemoved = 1;
		if (contpanel(targetkind)) {
			targetx = 0xff;
			targety = 0xff;
		}
		placeitem();
		lookaround();
		lookdone();
		units[0].pose = 0;
		break;
	case 4:
		dcret = 0;
		break;
	default:
		units[0].pose = 0;
		break;
	}
	leftclick = 0;
	dblclick = 0;
	menumode = dcmenu;
	restorecursor();
	mousemoved = 1;
	statshown = 0;
	if (dcret == 0)
		return dcres;
	drawscreen();
	return dcret;
}

void
showquick(void)
{
	hidecursor();
	cleartexts();
	drawscreen();
	text3(1, 0xf0, 0x73, strs[1143]);
	for (sqi = 0; sqi < 9; sqi++) {
		if (quickacts[sqi])
			sqact = actionidx(quickacts[sqi]);
		else
			sqact = 0xff;
		if (sqact != 0xff)
			wsprintf(scrbuf, texts + 0xb103, sqi + 0x31,
				strs[actions[sqact].name]);
		else
			wsprintf(scrbuf, texts + 0xb103, sqi + 0x31, texts + 0xb1f7);
		text3(1, 0xf0, sqi * 21 + 0x92, scrbuf);
	}
	sqact = sqi * 21 + 0x9c;
	drawtext2(1, 0xf0, sqact, strs[1144]);
	drawtext2(1, 0xf0, sqact + 10, strs[1145]);
	waitrelease();
	waitrelease(0);
	drawcursor();
	mousemoved = 1;
	drawscreen();
}

void
peektarget(void)
{
	pkpick = picktarget();
	if (targetkind == 1 && targetx == playerx && targety == playery) {
		targetitem = 0x270;
		targetidx = 0;
	} else if (!(pkpick == 1 || pkpick == 2) || (targetitem & 0xfff) == 0)
		return;
	plotfind(targetkind, targetx, targety, targetitem);
	clicktarget(1);
	cleartexts();
	showlabel();
	if (useinfo()) {
		pkx = hoverx;
		pky = hovery;
		pkcalm = playmode == 1 ? 1 : 0;
		showinfo(1);
		tracesite = 15;
		for (;;) {
			if (!trademode && playmode == 1 && units[0].health > 0)
				gametick();
			else {
				readinput();
				if (needredraw(0xfd))
					drawscreen();
			}
			pollinput();
			if (mouseleft || mouseright || leftclick || rightclick || playerout()
			|| pkcalm && playmode != 1 || esckey
			|| (keys[VK_CONTROL] & 0x80) && ((keys[0xe2] & 0x80) || (keys[VK_SHIFT] & 0x80)
			|| (keys[0xbe] & 0x80) || keycode)
			|| keycode != 0x10 && !(keys[VK_CONTROL] & 0x80))
				break;
		}
		showinfo(0);
		drawcursor();
	} else
		drawscreen();
	if (pkcalm && playmode != 1) {
		do
			readinput();
		while ((keys[VK_CONTROL] & 0x80) && !(keys[0xe2] & 0x80) && !(keys[VK_SHIFT] & 0x80)
		&& !(keys[0xbe] & 0x80) && !keycode);
	}
}

void
showquests(int)
{
	roman(gamehour >= 5 ? gameday : gameday - 1);
	wsprintf(workbuf + 0xd6d8, strs[546], daytime(gamehour), romanbuf);
	wsprintf(workbuf, strs[1435], titlehead(), workbuf + 0xd6d8, game.name);
	notelist(3);
	catstr(workbuf, strs[1436]);
	notelist(2);
	catstr(workbuf, strs[1437]);
	notelist(1);
	catstr(workbuf, strs[1438]);
	notelist(0);
	fadefast(0);
	textwin(workbuf, 0, 0xfe, 0, 0, 0, 0, 0xff);
}

int
keycmd(unsigned char key)
{
	tabkey = 0;
	if (key == heldcmd)
		return 0;
	heldcmd = key;
	dblclick = 0;
	dropheld();
	cleartexts();
	if (key == lastkey && key == prevkey) {
		if (key == 0x19)
			msgf(0x63, texts + 0xb0e7, curmap, playerx, playery);
		else if (key == 0x23)
			restfor(0xff);
	}
	prevkey = lastkey;
	lastkey = key;
	cheatc = lower(lastchar);
	if (cheatc == 0)
		;
	else {
		if (cheatc == ',') {
			cheatlen = 0;
		swallow:
			waitkeyup();
			return 0;
		}
		// FIXED: the tenth letter writes the terminator past cheatbuf. Keep nine, room for every cheat word.
#ifdef FEATURE_FAULTS
		if (cheatlen < sizeof cheatbuf - 1) {
#else
		if (cheatlen < sizeof cheatbuf) {
#endif
			cheatbuf[cheatlen++] = cheatc;
			cheatbuf[cheatlen] = 0;
			for (cheatc = 0; cheatc < 10; cheatc++) {
				for (cheati = 0; cheati <= cheatlen; cheati++)
					if (cheatbuf[cheati] != cheats[cheatc][cheati]) {
						if (cheatbuf[cheati] == 0)
							goto swallow;
						goto next;
					}
				cheatlen = 0;
				switch (cheatc) {
				case 0:
					if (cheat == 0)
						cheat = 1;
					else
						cheat = 0;
					msgf(0x63, strs[246], cheat ? strs[247] : strs[248]);
					playnow(0x15);
					drawscreen();
					break;
				case 1:
					if (cheat) {
						cheatc = cheat;
						cheat = 1;
						markers(1);
						cheat = cheatc;
						drawscreen();
					}
					break;
				case 2:
					if (cheat) {
						restfor(0xff);
						drawscreen();
					}
					break;
				case 3:
					if (cheat) {
						showquests(cheat);
						drawscreen();
					}
					break;
				case 4:
					if (cheat)
						msgf(0x63, "current location: %d,%d,%d.", curmap,
							playerx, playery);
					break;
				case 5:
					if (cheat == 0)
						break;
					textn(1, 8, 0x1b8, "location");
					telemap = asknumber(8, 0x1cc, 3, 0, 0x50, curmap);
					if (telemap == -1)
						break;
					drawscreen();
					textn(1, 8, 0x1b8, "X");
					telex = asknumber(8, 0x1cc, 3, 0, 0x96, playerx);
					if (telex == -1)
						break;
					drawscreen();
					textn(1, 8, 0x1b8, "Y");
					teley = asknumber(8, 0x1cc, 3, 0, 0x96, playery);
					if (teley == -1)
						break;
					drawscreen();
					if (telemap == curmap)
						;
					else if (telemap < 7)
						telemap = 7;
					else if (telemap >= 0x50)
						telemap = 0x4f;
					if (telex < 5)
						telex = 5;
					else if (telex > 0x90)
						telex = 0x90;
					if (teley < 4)
						teley = 4;
					else if (teley > 0x91)
						teley = 0x91;
					if (telemap != curmap || telex != playerx
					|| teley != playery)
						travel(telemap, telex, teley, 0, 1, 1, 0, 1);
					break;
				}
				return 0;
			next:
				;
			}
		}
	}
	switch (key) {
	case 0xb:
		showquick();
		break;
	case 0x47:
	case 0x48:
	case 0x49:
	case 0x4b:
	case 0x4d:
	case 0x4f:
	case 0x50:
	case 0x51:
		turnkeys = dirkeys;
		hidecursor();
		break;
	case 0x1c:
	case 0x43:
		nearfoe(0);
		if (playmode == 3) {
			if (!nofoes) {
				denymsg(strs[316]);
				drawscreen();
				break;
			}
		} else
			combatcause = 1;
		togglecombat(0);
		break;
	case 0xf:
		if ((keys[0xe2] & 0x80) || (keys[VK_SHIFT] & 0x80) || (keys[VK_CONTROL] & 0x80)
		|| (keys[0xbe] & 0x80)) {
			waitkeyup();
			break;
		}
		/* fall through */
	case 0x40:
		if (!(keys[0xe2] & 0x80))
			markers(0);
		break;
	case 0x10:
	case 0x1d:
		if (key != 0x1d || !(keys[0xe2] & 0x80) && !(keys[VK_SHIFT] & 0x80)
		&& !(keys[0xbe] & 0x80))
			peektarget();
		break;
	case 0xe:
	case 0x42:
		travelmap(0, curmap, curmap);
		break;
	case 0x52:
		hoverx = 12;
		actionmenu(1);
		if (actsel) {
			actsel--;
			doaction(0);
		}
		mousemoved = 1;
		readinput();
		break;
	case 0x26:
	case 0x3d:
		if ((key == 0x26 && ((keys[VK_SHIFT] & 0x80) || (keys[0xbe] & 0x80))
		|| key == 0x3d && ((keys[VK_SHIFT] & 0x80) || (keys[0xbe] & 0x80)))
		&& lastslot) {
			cleartexts();
			drawscreen();
			hidecursor();
			if (loadslot(lastslot, 0)) {
				entersum = 1;
				enteritems = 0xff;
				fadefast(0);
				if (entergame(0)) {
				msg:
					msgf(0x63, strs[1148], (short)lastslot + 0x40);
				}
				drawscreen();
				leavemenu(1);
				drawcursor();
				break;
			}
			drawcursor();
		}
		if (loadmenu(0))
			goto msg;
		leavemenu(1);
		break;
	case 0x1f:
	case 0x3c:
		if (savemenu(0))
			aftersave();
		leavemenu(1);
		if (lastslot) {
			msgf(0x63, strs[1147], (short)lastslot + 0x40);
			drawscreen();
			drawcursor();
		}
		break;
	case 0x17:
		if (keys[VK_CONTROL] & 0x80) {
			showreport(0);
			break;
		}
		/* fall through */
	case 0x3f:
		charsheet(units, 0);
		break;
	case 0x31:
	case 0x41:
		journal();
		break;
	case 0x18:
	case 0x3e:
		options(0);
		leavemenu(1);
		break;
	case 0x3b:
		if (keys[0xe2] & 0x80) {
			controls(0, 0);
			drawscreen();
			fadefast(1);
			waitrelease(0);
		} else
			docmenu();
		break;
	case 0x4e:
		if (playmode != 1) {
			units[0].pose = 9;
			break;
		}
		/* fall through */
	case 0x44:
		waitkeyup();
		break;
	case 0x39:
	case 0x4c:
		togglehero();
		waitkeyup();
		break;
	case 0x2f:
		verstr(printbuf, 2);
		msgf(0x63, printbuf);
		music(playmode == 1 ? 1 : 0x1f, 0);
		drawscreen();
		waitkeyup();
		break;
	case 0x19:
		if (cheat && (keys[VK_SHIFT] & 0x80)) {
			msgf(0x63, strs[1452], strs[zones[mapzone(curmap)].name],
				curmap, playerx, playery, gameday, gamehour, gametime);
			drawscreen();
			waitkeyup();
		}
		break;
	default:
		if (key >= 2 && key <= 10 && !(keys[0xe2] & 0x80) && quickacts[key - 2] != 0) {
			actsel = actionidx(quickacts[key - 2]);
			if (actsel != 0xff
			&& (cheat || getbit(game.known, actions[actsel].bit)))
				doaction(0);
		}
		break;
	}
	return 0;
}

void
doinput(void)
{
	if (playerout())
		return;
	indoinput = 1;
	lbltext[0] = 0;
	readinput();
	if (esckey == 0xff) {
		entermenu();
		if (checkquit()) {
			menumode = 0;
			mousemoved = 1;
			drawscreen();
			lastlight = 0xff;
			lookdone();
			fadefast(1);
		}
		if (quitting)
			return;
		leavemenu(1);
	}
	if (keycode) {
		if (keycmd(keycode))
			goto out;
	} else if (tabkey) {
		tabkey = 0;
		if (keycmd(0xf))
			goto out;
	} else if (lastscan) {
		if (lastscan == 0xf)
			lastscan = 0;
		else if (keycmd(lastscan))
			goto out;
	} else if ((keys[VK_CONTROL] & 0x80) && (keys[0xe2] & 0x80) == 0 && (keys[VK_SHIFT] & 0x80) == 0 && (keys[0xbe] & 0x80) == 0) {
		if (keycmd(0x1d))
			goto out;
	}
	if (quitting)
		goto out;
	if (dblclick) {
		dblclick = 0;
		leftclick = 0;
		clickhover();
		if (hoverkind == 1 && hovercx == playerx && hovercy == playery)
			goto here;
		inpick = picktarget();
		if (!(playmode != 1 && playmode != 2)) {
			cleartexts();
			if ((targetitem & 0xfff) == 0)
				goto out;
			if (inpick != 0 && inpick != 3) {
			menu:
				plotfind(targetkind, targetx, targety, targetitem);
				inpick = useitem(1);
				if (inpick == 0x378)
					goto out;
				if (inpick == 999)
					goto look;
				if (inpick != 0 && playmode != 1)
					units[0].pose = 9;
				else
					units[0].pose = 0;
			}
			tracesite = 0x14;
			while (mouseleft && !playerout())
				if (playmode == 1 && units[0].health > 0)
					gametick();
				else {
					readinput();
					if (needredraw(0xfd))
						drawscreen();
				}
			leftclick = 0;
			goto out;
		}
	}
	if (mouseright || rightclick || turnkeys) {
		forgettarget();
		leftclick = 0;
		if (closehover(hoverkind)) {
			waitforup = 1;
			mousemoved = 1;
			drawscreen();
			tracesite = 0x15;
			while (mouseright && !playerout())
				if (playmode == 1 && units[0].health > 0)
					gametick();
				else {
					readinput();
					if (needredraw(0xfd))
						drawscreen();
				}
			waitforup = 0;
			mousemoved = 1;
			goto out;
		}
	}
	if (!mouseleft && !leftclick)
		goto out;
	leftclick = 0;
	if (hoverkind != 0xe6)
		clickhover();
	if (hoverkind == 1 && hovercx == playerx && hovercy == playery) {
	here:
		forgettarget();
		togglehero();
		tracesite = 0x16;
		while (mouseleft && !playerout())
			if (playmode == 1 && units[0].health > 0)
				gametick();
			else {
				readinput();
				if (needredraw(0xfd))
					drawscreen();
			}
		leftclick = 0;
		goto out;
	}
	inpick = picktarget();
	if (inpick == 3) {
	none:
		forgettarget();
		goto other;
	}
	if (unconscious()) {
		forgettarget();
		goto out;
	}
	if (playmode == 3) {
		if (targetkind == 0xe6) {
			cleartexts();
			units[0].pose = 9;
			goto out;
		}
		if ((targetitem & 0xfff) == 0)
			goto out;
		plotfind(targetkind, targetx, targety, targetitem);
		switch (attacktarget()) {
		case 0:
			break;
		case 1:
			units[0].pose = 3;
			break;
		case 2:
			drawscreen();
			while (mouseleft)
				readinput();
			leftclick = 0;
			break;
		}
		goto out;
	}
	if (inpick == 0 || inpick == 3 || (targetitem & 0xfff) == 0)
		goto none;
look:
	plotfind(targetkind, targetx, targety, targetitem);
	inres = clicktarget(0);
	if (inres == -123)
		goto menu;
	if (inres == -124) {
		drawscreen();
		goto busy;
	}
	if (inres) {
	other:
		forgettarget();
		if (tipshown() || nmsgs)
			if (!menumode) {
				cleartexts();
				drawscreen();
			}
		goto out;
	}
	if (mouseleft == 0) {
		if (showlabel())
			drawscreen();
		forgettarget();
		goto out;
	}
	if (dragclick() == 0)
		goto menu;
busy:
	tracesite = 0x17;
	while (mouseleft && !playerout())
		if (playmode == 1 && units[0].health > 0)
			gametick();
		else {
			readinput();
			if (needredraw(0xfd))
				drawscreen();
		}
	leftclick = 0;
out:
	indoinput = 0;
}

int
lastitem(int cell)
{
	if (map[cell] & 0x1000) {
		firstindex = map[cell] >> 16;
		for (lastindex = firstindex; !(mapitems[lastindex] & 0x4000); lastindex++)
			if ((mapitems[lastindex] & 0x2000) || !(mapitems[lastindex] & 0x8000))
				break;
	}
	return 0;
}

void
syncplayer(void)
{
	playerx = units[0].x;
	playery = units[0].y;
	rightclick = 0;
	playercell = playery * 150 + playerx;
	trackregion();
	if (mapflags[playercell] & 0x20)
		curregion = 0;
	if (units[0].pose == 1) {
		units[0].pose = 0;
		lastkey = 0;
	}
}

void
watersound(void)
{
	if (landsounds == 0 || !(blockmax & 0x80))
		return;
	if (inqueue(0x3f) || inqueue(0x40) || inqueue(0x41)
	|| playing(0x3f) || playing(0x40) || playing(0x41))
		return;
	wsr = randn(100);
	if (wsr < 0x21)
		queuesound(0x3f);
	else if (wsr < 0x42)
		queuesound(0x40);
	else
		queuesound(0x41);
}

void
ambience(void)
{
	if (landsounds == 0 || (map[playercell] & 0x2000))
		return;
	watersound();
	if (((blockmax & 0x20) || blockn >= 3)
	&& !(((unsigned char *)mapstate)[0x22] < 10 || sunlight < 10
	|| playing(0x37) || playing(0x38) || playing(0x39) || playing(0x3a)
	|| playing(0x3b) || playing(0x3c) || playing(0x3d) || playing(0x3e)
	|| randn(100) >= 12)) {
		ambr = randn(100);
		if (blockmax & 0x20) {
			if (ambr < 0x19)
				queuesound(0x37);
			else if (ambr < 0x32)
				queuesound(0x38);
			else if (ambr < 0x4b)
				queuesound(0x39);
			else
				queuesound(0x3a);
		} else {
			if (ambr < 0x19)
				queuesound(0x3b);
			else if (ambr < 0x32)
				queuesound(0x3c);
			else if (ambr < 0x4b)
				queuesound(0x3d);
			else
				queuesound(0x3e);
		}
	}
	if ((blockmax & 0x1f) != 8 && !(blockmax & 0x40))
		return;
	if (playing(0x42) || playing(0x43) || playing(0x44) || randn(100) >= 9)
		return;
	ambr = randn(100);
	if (ambr < 0x21)
		queuesound(0x42);
	else if (ambr < 0x42)
		queuesound(0x43);
	else
		queuesound(0x44);
}

void
tilesounds(void)
{
	short x, y, xx, r, yy;

	if (landsounds == 0)
		return;
	if (muted())
		return;
	for (y = -4; y <= 4; y++)
		for (x = -6; x <= 6; x++) {
			xx = playerx + x;
			yy = playery + y;
			if (!animturn(xx, yy))
				continue;
			if (findtype(xx + yy * 150, 0x5b0)) {
				r = randn(100);
				if (r < 0x21)
					queuesound(0x45);
				else if (r < 0x42)
					queuesound(0x46);
				else
					queuesound(0x47);
				return;
			}
			if (findtype(xx + yy * 150, 0x5b4)) {
				queuesound(0x34);
				return;
			}
		}
	for (y = -2; y <= 2; y++)
		for (x = -2; x <= 2; x++) {
			xx = playerx + x;
			if (findtype(xx + (yy = playery + y) * 150, 0x5e7)) {
				queuesound(0x43);
				return;
			}
		}
}

int
steplen(void)
{
	if (ptrpace == 0) {
		if (scrolling <= 2)
			return 24;
		if (scrolling == 3)
			return 12;
	} else if (ptrpace == 1) {
		if (scrolling <= 1)
			return 24;
		if (scrolling == 2)
			return 12;
		if (scrolling == 3)
			return 8;
	} else if (ptrpace == 2) {
		if (scrolling == 0)
			return 24;
		if (scrolling == 1)
			return 12;
		if (scrolling == 2)
			return 8;
		if (scrolling == 3)
			return 6;
	} else if (ptrpace >= 3) {
		if (scrolling == 0)
			return 12;
		if (scrolling == 1)
			return 8;
		if (scrolling == 2)
			return 6;
		if (scrolling == 3)
			return 4;
	}
	return 12;
}

void
fightmusic(void)
{
	if (cur >= 0x14)
		return;
	if (units[0].berserk) {
		if (cur != 0xb)
			music(0xb, 0);
		return;
	}
	if (playmode != 1) {
		if (cur != 3)
			music(3, 0);
	} else if (cur == 3)
		stopmusic();
	if (cur == 0xb)
		stopmusic();
}

void
gametick(void)
{
	readinput();
	if (talkactive && trademode) {
		if (cheat && (keys[0xbf] & 0x80) && (keys[VK_CONTROL] & 0x80))
			textat(0xfa01, "rt w dla/hanm");
		return;
	}
	if (playmode == 1 && (units[0].pose == 0 || units[0].pose > 9) && !menumode && !lostcontrol
	&& (waitforup == 0 && (rightclick || mouseright) && canstep(0, 0)
	|| dirkeys && canstepkey(dirkeys) || turnkeys && canstepkey(turnkeys))) {
		units[0].pose = 0;
		steptick = 0x17;
		lasttick = 0x17;
	} else if (units[0].pose == 1) {
		if (ticks() < walkdue) {
			if (cheat && (keys[0xbf] & 0x80) && (keys[VK_CONTROL] & 0x80))
				textat(0x11301, "rt w ts < mtw");
			return;
		}
	} else if (ticks() < standdue) {
		if (cheat && (keys[0xbf] & 0x80) && (keys[VK_CONTROL] & 0x80))
			textat(0x11301, "rt w ts < mts");
		return;
	}
	if (shown == 0 && dirkeys == 0 && turnkeys == 0 && !menumode && !lostcontrol
	&& !(units[0].pose == 1 && (mouseright || rightclick || mouseleft || leftclick))
	&& !indoinput) {
		drawcursor();
		mousemoved = 1;
		readinput();
	}
	if (playmode == 1)
		standdue = ticks() + standdelay;
	else {
		standdue = standdelay / 4;
		if (standdue < 1)
			standdue = 1;
		standdue += ticks();
	}
	if (units[0].pose == 1 && walkturns > 1 && !mouseright && !turnkeys)
		walkdue = ticks() + 10;
	else {
		gtwait = walkdelay;
		gtpct = 100;
		if (ptrpace == 0)
			gtpct = 200;
		else if (ptrpace == 1)
			gtpct = 100;
		else if (ptrpace == 2)
			gtpct = 20;
		else if (ptrpace >= 3)
			gtpct = 205;
		gtwait *= gtpct;
		gtwait *= 24;
		gtwait /= steplen();
		gtwait /= 100;
		if (!gtwait)
			gtwait++;
		walkdue = ticks() + gtwait;
	}
	if (gtdepth > 1)
		return;
	gtunused = 0;
	gtdepth++;
	stepinc = 1;
	if (steptick < 0x17) {
		if (units[0].pose == 1) {
			stepinc = 24 / steplen();
			if (stepinc)
				stepinc--;
			steptick += stepinc;
			if (steptick >= 0x17) {
				steptick = 0x17;
				goto move;
			}
		}
	} else {
	move:
		if (units[0].pose == 1) {
			units[0].x += units[0].dirx;
			playerx = units[0].x;
			units[0].y += units[0].diry;
			playery = units[0].y;
			units[0].nextx = units[0].x;
			units[0].nexty = units[0].y;
			units[0].pose = 0;
			syncplayer();
			setcurregion();
			gtmark = markerat(playercell);
			if (gtmark) {
				curregion = (gtmark >> 23) & 0xff;
				if (gtmark & 0x80000000)
					travelmap(0, curmap, curmap);
				else {
					gtspot = (unsigned short)((gtmark >> 16) & 0x7f);
					if (gtspot)
						enterspot(gtspot);
				}
			}
			if (slowframe >= 2)
				slowframe = 0;
			else
				slowframe++;
			if (fireframe >= 2)
				fireframe = 0;
			else
				fireframe++;
			sharecell(&units[0]);
		}
		sharecells();
	}
	mousemoved = 1;
	steptick++;
	if (waterpace <= 0)
		waterpace = 1;
	if (steptick % waterpace == 0) {
		if (waterframe >= 11)
			waterframe = 0;
		else
			waterframe++;
	}
	if ((steptick & 3) == 0) {
		stepframe++;
		fightmusic();
		if ((stepframe & 1) == 0) {
			if (fireframe >= 2)
				fireframe = 0;
			else
				fireframe++;
			if ((stepframe & 3) == 0) {
				if (slowframe >= 2)
					slowframe = 0;
				else
					slowframe++;
			}
		}
	}
	if ((steptick & 3) == 0)
		stepanims();
	if (playmode != 1 || talkactive || lostcontrol)
		stepframe = steptick / 4;
	if (stepframe >= 6) {
		moveunits(1);
		sharecells();
	}
	if (steptick >= 0x18) {
		steptick = 0;
		if (units[0].pose != 2) {
			if (busyturns && busyturns != 0xffff) {
				busyturns--;
				units[0].pose = 9;
			} else
				units[0].pose = 0;
		}
		if (cmbautooff)
			autocombat();
		rununits(2);
	}
	if (stepframe >= 6) {
		stepframe = 0;
		actionturn = 0;
		fidget();
		clearturn();
		if (talkactive == 0) {
			gametime++;
			if (playmode != 1 || menumode)
				;
			else {
				releaseheld();
				if (tipshown() && tipend + 7 < gametime)
					cleartip();
				for (gtmsg = 0; gtmsg < nmsgs; gtmsg++)
					if (msgtime[gtmsg] + 15 < gametime && !msgkeep[gtmsg]) {
						delmsg(gtmsg);
						gtmsg--;
					}
				layoutmsgs();
			}
			if (gametime % hourticks == 0) {
				nexthour();
				setdaylight();
			}
			burntorches();
		}
		turncount++;
		if (stepframe == 0)
			ambience();
		tilesounds();
		if (playmode == 1 || unitsrun) {
			rununits(0);
			markunits();
			playqueue();
		}
	}
	if (units[0].pose == 1 && cellflag40(map[playercell])
	&& (waterat(playerx, playery) || isswamp(map[playercell]) && randn(100) < 50)) {
		if (steptick == 6)
			playnow(0x35);
		else if (steptick == 0x12)
			playnow(0x36);
	}
	if (units[0].pose == 1 || (steptick & 3) == 0) {
		lookaround();
		lookdone();
		drawscreen();
	}
	gtdepth--;
}

void
inputhook(void)
{
}

void
sayscene(void)
{
	fillgauges();
	clearmind(&units[0]);
	sayold = sayunit->scene;
	sayunit->scene = saynum;
	talkto(indexof(sayunit));
	if (sayunit->scene == saynum)
		sayunit->scene = sayold;
}

void
playerturn(void)
{
	if (playerout())
		return;
	lookstale = 1;
	if (playmode != 1 && mouseleft && !(targetkind == 0xe6 && hoverkind == 0xe6)) {
		drawscreen();
		while (mouseleft)
			readinput();
		leftclick = 0;
	}
	if (units[0].pose == 9) {
		readinput();
		turnkeys = 0;
		rightclick = 0;
		leftclick = 0;
		dblclick = 0;
	} else {
		for (tumark = 0;; tumark++) {
			readinput();
			if (quitting)
				return;
			fightmusic();
			if (dirkeys) {
				turnkeys = dirkeys;
				hidecursor();
			}
			if (mouseright || rightclick || turnkeys) {
				if (closehover(hoverkind)) {
					waitforup = 1;
					mousemoved = 1;
					drawscreen();
					tracesite = 0x1e;
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
				} else if (units[0].pose == 0 || units[0].pose > 9) {
					if (!movecursor)
						mousemoved = 1;
					dblclick = 0;
					lastkey = 0;
					if ((hoverkind == 1 || hoverkind == 0x31) && !unconscious()) {
						dropheld();
						cleartexts();
						readinput();
						if (turnkeys == 0) {
							if (advance(0, 0)) {
								rightclick = 0;
								goto done;
							}
						} else if (turnkeys & 2) {
							turnkeys = 0;
							if (advance(0, -1)) {
								rightclick = 0;
								goto done;
							}
						} else if (turnkeys & 0x40) {
							turnkeys = 0;
							if (advance(0, 1)) {
								rightclick = 0;
								goto done;
							}
						} else if (turnkeys & 8) {
							turnkeys = 0;
							if (advance(-1, 0)) {
								rightclick = 0;
								goto done;
							}
						} else if (turnkeys & 0x10) {
							turnkeys = 0;
							if (advance(1, 0)) {
								rightclick = 0;
								goto done;
							}
						} else if (turnkeys & 1) {
							turnkeys = 0;
							if (advance(-1, -1)) {
								rightclick = 0;
								goto done;
							}
						} else if (turnkeys & 4) {
							turnkeys = 0;
							if (advance(1, -1)) {
								rightclick = 0;
								goto done;
							}
						} else if (turnkeys & 0x20) {
							turnkeys = 0;
							if (advance(-1, 1)) {
								rightclick = 0;
								goto done;
							}
						} else if (turnkeys & 0x80) {
							turnkeys = 0;
							if (advance(1, 1)) {
								rightclick = 0;
								goto done;
							}
						}
					}
				}
				turnkeys = 0;
				rightclick = 0;
			}
			edgemenus(0);
			if (quitting)
				return;
			if (playmode != 1 && (units[0].pose == 0 || units[0].pose > 9))
				doinput();
			if (quitting)
				return;
			if (!(units[0].pose == 0 || units[0].pose > 9) || playmode == 1 || units[0].health <= 0)
				break;
			if (needredraw(0xfd) || condgauge && hoverx <= 0x50 && hovery >= 0x180)
				drawscreen();
		}
	}
done:
	if (units[0].pose == 1 && (mouseright || turnkeys))
		walkturns++;
	else
		walkturns = 0;
	lookstale = 0;
	if (units[0].pose == 9)
		units[0].pose = 0;
	else if (units[0].pose == 1) {
		tumark = markerat(units[0].nexty * 150 + units[0].nextx);
		if (tumark) {
			lookstale = 1;
			curregion = (tumark >> 23) & 0xff;
		}
	}
}

void
heroend(short n)
{
	killunit(&units[0], 1, 0, 0xff, 0);
	if (playmode != 1)
		setcombat(0);
	endgame(n);
	charsheet(&units[0], 1);
}

void
runtalker(void)
{
	if (!talker)
		return;
	sayunit = &units[talker];
	if (sayunit->mode == 0xf) {
		sayunit->modea = 0;
		sayunit->modeb = 0;
		sayunit->modee = 0;
		sayunit->modef = 0;
		setunitmode(sayunit, 0);
	}
	if (sayunit->health > 0 && displayed(sayunit) && seesunit(sayunit)) {
		saynum = talkscene;
		sayscene();
	}
	talker = 0;
	talkscene = 0;
}

char patchnote[593] = " *** We dissuade from trying to patch this program. If you want to cheat, use the cheat mode, which can be activated by entering \",cheat\". *** Please do not release cracked versions of this game, or tools or descriptions for cracking it. Please do not make copies of the Full Version available for downloading. Development of this game has taken several years of programming and cost thousands of dollars for graphics and translations. Income from game sales is vital for the author to get at least part of his investment back. Therefore he will persecute anyone who violates his rights. *** ";
unsigned char cheatlen = 0xff;
char cheats[10][10] = {
	"cheat", "map", "heal", "quest", "pos", "telep"
};
int stepinc = 1;
unsigned short chbit;
int chstr;
unsigned long ctres;
unsigned char ctcls;
char ctbusy;
int ctkind;
int ctx;
int cty;
unsigned long *clword;
unsigned long *flword;
int fltype;
unsigned long flidx;
short flhard;
short flhit;
short flkind;
short fldmin;
short fldmax;
short flsnd;
short tobi;
int fores;
int atpick;
short atdx;
short atdy;
unsigned char atcls;
unsigned long atthrow;
int soold;
unsigned long sonew;
int siold;
unsigned long sinew;
unsigned short spcn;
unsigned long spcpart;
unsigned char spccls;
int puunit;
int puwcls;
unsigned char pucls;
unsigned char dicls;
short diunit;
int didone;
int digift;
unsigned long diword;
unsigned long dilook;
int dicell;
int ditop;
int dgdrop;
int dgpick;
int dgx;
int dgy;
short dcmenu;
int dcres;
int dcret;
short sqi;
short sqact;
int pkpick;
int pkx;
int pky;
short pkcalm;
short telemap;
short telex;
short teley;
int inpick;
int inres;
int lastindex;
int firstindex;
short wsr;
short ambr;
unsigned long gtmark;
int gtspot;
short gtmsg;
int gtwait;
int gtpct;
unsigned long tumark;
Unit *sayunit;
short saynum;
short sayold;
char dropsnd;
char cheatbuf[10];
char cheatc;
unsigned char cheati;
unsigned char lastkey;
unsigned char prevkey;

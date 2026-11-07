#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

typedef struct Unit Unit;
typedef struct Realm Realm;
typedef struct Metal Metal;
typedef struct Gear Gear;
typedef struct Game Game;
typedef struct Kind Kind;
typedef struct Action Action;

/* 0x1b8 bytes each (009bc240) */
struct Unit {
	unsigned char x;
	unsigned char y;
	char f2[4 - 2];
	signed char dirx;
	signed char diry;
	char f6[8 - 6];
	unsigned char pose;
	unsigned char mode;
	char modea;
	unsigned char modeb;
	unsigned char modec;
	char fd[0xeb - 0xd];
	short plea;
	short strength;
	char fef[0xf3 - 0xef];
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
	char f118[0x11c - 0x118];
	unsigned long items[3];
	unsigned long cloak;
	char f12c[0x130 - 0x12c];
	unsigned char turnmarks;
	char f131[0x134 - 0x131];
	unsigned char bewitcher;
	unsigned char asleep;
	unsigned char bewitched;
	unsigned char blessed;
	unsigned char poison;
	unsigned char cursed;
	unsigned char fleeing;
	char confused;
	unsigned char bleeding;
	unsigned char berserk;
	char drunk;
	unsigned char weakened;
	unsigned char warded;
	char f141[0x143 - 0x141];
	unsigned char traits;
	short mercy;
	signed char sympathy;
	unsigned char type;
	char flags;
	unsigned char lord;
	unsigned char handkind;
	unsigned char beltkind;
	char f14c[0x159 - 0x14c];
	unsigned char modee;
	unsigned char modef;
	char modeg;
	unsigned char lasthit;
	char f15d[0x1b8 - 0x15d];
};

/* 0x158 bytes each (005a1b78) */
struct Realm {
	unsigned long item;
	unsigned long items[50];
	char fcc[0x158 - 0xcc];
};

/* 0x1b bytes each (004c08a8): weapons and goods */
struct Metal {
	short skill;
	short minstr;
	short attack;
	short parry;
	char f8[0xc - 8];
	short wearmin;
	char fe[0x10 - 0xe];
	short dmgkind;
	short days;
	short metal;
	unsigned char kind;
	unsigned char thrown;
	unsigned char material;
	char f19[0x1b - 0x19];
};

/* 0x1a bytes each (004c0bd8) */
struct Gear {
	short prot1;
	short prot2;
	short prot3;
	short wearmin;
	char f8[0x10 - 8];
	short shieldpct;
	unsigned char hardness;
	char f13[0x17 - 0x13];
	unsigned char material;
	char f18[0x1a - 0x18];
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

/* what kindof returns for a unit */
struct Kind {
	short name;
	short dmg2;
	short dmg1;
	short dmg6;
	short dmg5;
	short dmgadd;
	char fc[0x18 - 0xc];
	short maxhealth;
};

/* 0x1a bytes each (004c3d68): the actions of the command bar */
struct Action {
	unsigned short name;
	unsigned short icon;
	unsigned char rowbreak;
	char f5;
	unsigned short cmd;
	char f8[0x1a - 8];
};

int playerdead(void);
void cleartexts(void);
void cleartip(void);
void forgetheld(void);
int wrap(char *s, int width);
int text3w(char *s);
void sentcase(char *s);
void playnow(short n);
char *itemname(unsigned long item);
unsigned char classof(int i);
void setunitmode(Unit *u, unsigned char mode);
unsigned long valueof(unsigned long item);
int cansee(int x, int y);
short isdog(Unit *u);
short isanimal(Unit *u);
void turnto(Unit *u, short x, short y);
void animalcry(Unit *u);
void playqueue(void);
short allied(Unit *u, Unit *v);
void provoke(Unit *a, Unit *b, signed char c, short d, short e);
int freegoods(short a, short b);
unsigned short idof(unsigned long item);
unsigned short itemweight(unsigned long item);
int isfirewood(unsigned long item);
char *unitname(Unit *u);
unsigned long *itemptr(unsigned char where, int n);
void deedsteal(unsigned long item);
short istrader(Unit *u);
short issmith(Unit *u);
short isfemale(Unit *u);
short isservant(Unit *u);
short myservant(Unit *u);
short sparring(Unit *u, Unit *v);
void deedambush(void);
void deedambushwoman(void);
int badunit(Unit *u);
unsigned short gearof(unsigned long item);
unsigned long baselook(unsigned long item);
void queuesound(short n);
short addwear(unsigned long *item);
unsigned short wearof(unsigned long item);
unsigned short blessof(unsigned long item);
int inview(int x, int y);
unsigned long broken(unsigned long item);
short indexof(Unit *u);
unsigned long giveitem(short n, unsigned long item, short mode);
void calcstats(Unit *u);
void dresshero(void);
int cellholds(unsigned long x);
int itemholds(unsigned long item);
unsigned long additem(int pos, unsigned long item);
unsigned short healthof(Unit *u);
short armorval(unsigned long item, short v);
void restfor(short n);
Kind *kindof(Unit *u);
unsigned short healthid(unsigned short a, unsigned short b);
char *attacktext(Unit *a, Unit *u);
int spared(short m, short k);
short isskeleton(Unit *u);
short mustflee(Unit *u);
void plotflee(int a, short n);
short isbear(Unit *u);
void deeddefeat(Unit *u);
short isspider(Unit *u);
short israt(Unit *u);
void tire(short n);
void fillgauges(void);
void deedkillflee(void);
int robbery(short m, short k);
int music(int n, int lp);
void deedkillwoman(void);
void deedkill(void);
void outrage(short flag);
void nomercy(Unit *u);
void deedbooty(unsigned long item);
void killunit(Unit *u, short a, short b, short c, short d);
short skillof(Unit *u, short a, short b, short c, short d);
void damagerange(unsigned long item, short *a, short *b);
void plotfight(short m, short k);
short induel(Unit *a, Unit *b);
int unititem(Unit *u);
void train(unsigned char k, int n);
unsigned char weaponskill(void);
short clearsight(int x0, int y0, int x1, int y1);
int deepwater(int x, int y);
unsigned short findclass(int pos, unsigned char c);
void deedsink(unsigned long item);
unsigned long randlook(unsigned long item);
void deedsleeper(void);
void deedhitwoman(void);
void setfought(Unit *u);
short hampered(short m, short k, short v);
short favored(short m, short k, short v);
short odds(short a, short b);
int canperform(Unit *u, short cmd, int n, int a, int c);
int perform(Unit *u, short cmd, int n, int a, int c, short force);
short spellset(Unit *u);
unsigned long jewelmagic(Unit *u, unsigned long mask);
short isrobber(Unit *u);
short canfrighten(short m, short k);
short nearfoe(int n);
void setflag(int n, int v);

extern Unit units[];
extern char *strs[];
extern Realm realms[125];
extern unsigned char typenames[];
extern char trademode;
extern int cheat;
extern int lostcontrol;
extern char knockedout;
extern Metal metals[];
extern Gear armors[];
extern int bloodtill;
extern short merciful;
extern short pleader;
extern unsigned long map[22500];
extern unsigned short nmapitems;
extern unsigned long mapitems[];
extern short censorship;
extern unsigned char keys[256];
extern int text3width;
extern unsigned long gametime;
extern char texts[47942];
extern unsigned char targetkind;
extern unsigned long targetitem;
extern int targetidx;
extern unsigned short curmap;
extern unsigned short notedunit;
extern short targetowner;
extern unsigned short nunits;
extern int playerx;
extern int playery;
extern short notedmap;
extern unsigned long notedtime;
extern short askedonce;
extern short askedunit;
extern int targetx;
extern int targety;
extern unsigned char notedkind;
extern unsigned char lastkind;
extern int notedposx;
extern int notedposy;
extern unsigned long noteditem;
extern int restbreak;
extern unsigned short hourticks;
extern Game game;
extern short blocker;
extern int lineokx;
extern int lineoky;
extern unsigned char oddsswitch;
extern short avengers;
extern Action actions[];
extern char empty[1];
extern char talkactive;

/* the messages at the bottom of the screen, newest last */
extern short maxmsgs;
extern short nextmsgid;

extern short li;
extern short lj;
extern short ltotal;
extern short ln;
extern short dk;
extern short fi;
extern short di;
extern short dcount;
extern short ai;
extern short ak;
extern short alen;
extern short aminprio;
extern short amin;
extern short aid;
extern char *aline;

extern char msgtext[17][200];
extern short msgprio[17];
extern short msgid[17];
extern short msgdel[17];
extern short msgkeep[17];
extern short msglines[18];
extern short msgy[18];
extern unsigned long msgtime[17];
extern short nmsgs;
extern char msgfmt[200];
extern short askslot;
extern short denyslot;
extern short failslot;
extern short newmsgs;
extern unsigned long heldtime;
extern int heldcell;
extern short heldids[8];
extern int nheld;

extern short msgfid;
extern short msgcatid;
extern short prependw;
extern short askmsgid;
extern short failmsgid;
extern short nosuccid;
extern short denymsgid;
extern short notworkid;
extern signed char rmode;
extern short ri;
extern short rd;
extern short rd2;
extern short rval;
extern short ra;
extern short rb;
extern unsigned long steallock;
extern unsigned long *stealp;
extern unsigned char ukind;
extern short ustr;
extern short stealmsg;
extern short attackmsg;
extern short freetake;
extern unsigned long hosttime;
extern short ci;
extern unsigned char wdbleed;
extern unsigned char wdpoison;
extern short wdloss;
extern short gi;
extern short diemsg;
extern short wgmin;
extern short wgprio;
extern short wgmsg;
extern unsigned char wgmat;
extern short wgbroke;
extern unsigned long wgwreck;
extern unsigned long bloodit;
extern short hprot;
extern short hgear;
extern short hprio;
extern short hdmg0;
extern short hdmg1;
extern short hdmg2;
extern unsigned long hitem;
extern unsigned long *hslot;
extern unsigned char hkind;
extern short hsound;
extern short hmsg;
extern short hsaid;
extern short hstate0;
extern short hstate1;
extern short wskill;
extern short warm;
extern short wpct;
extern short aidx;
extern short askill;
extern short apct;
extern short tmres;
extern short tmskill;
extern short satk;
extern short smin;
extern short smax;
extern short sdmg;
extern short skind;
extern short sslot;
extern short sroll;
extern short sparry;
extern short sshield;
extern short sdodge;
extern short sluck;
extern short sx;
extern short sy;
extern short smissed;
extern short sdx;
extern short sdy;
extern short sdist;
extern short sexposed;
extern short throwfail;
extern short throwx;
extern short throwy;
extern short throwdist;
extern short casti;
extern short castres;
extern int spellres;
extern short spellcmd;
extern short spellkind;
extern short spellward;
extern int heldi;
extern short reli;
extern short relmsg;
extern short skilli;
extern short skillup;
extern short skilldown;
extern short skillstr;
extern int skilld;
extern int skillmax;
extern int skillmin;

void
tipclear(void)
{
	if (!playerdead())
		cleartexts();
}

void
clearmsgs(void)
{
	if (playerdead())
		return;
	memset(msgtext, 0, sizeof msgtext);
	memset(msgprio, 0, sizeof msgprio);
	memset(msgid, 0, sizeof msgid);
	memset(msgdel, 0, sizeof msgdel);
	memset(msgkeep, 0, sizeof msgkeep);
	memset(msglines, 0, sizeof msglines);
	memset(msgy, 0, sizeof msgy);
	memset(msgtime, 0, sizeof msgtime);
	memset(msgfmt, 0, sizeof msgfmt);
	nmsgs = 0;
	askslot = 0;
	denyslot = 0;
	failslot = 0;
	newmsgs = 0;
	forgetheld();
}

/* count the lines of each message and place them up from the bottom */
void
layoutmsgs(void)
{
	ltotal = 0;
	for (li = 0; li < nmsgs; li++) {
		ln = 1;
		for (lj = 0; msgtext[li][lj]; lj++)
			if (msgtext[li][lj] == '\r' && msgtext[li][lj + 1] != 0)
				ln++;
		msglines[li] = ln;
		ltotal += ln;
	}
	msgy[0] = 480 - ltotal * 22;
	for (li = 1; li <= nmsgs; li++)
		msgy[li] = msglines[li - 1] * 22 + msgy[li - 1];
}

void
delmsg(short n)
{
	if (playerdead() || !nmsgs)
		return;
	nmsgs--;
	for (dk = n; dk < nmsgs; dk++) {
		memcpy(msgtext[dk], msgtext[dk + 1], 200);
		msgprio[dk] = msgprio[dk + 1];
		msgid[dk] = msgid[dk + 1];
		msgdel[dk] = msgdel[dk + 1];
		msgkeep[dk] = msgkeep[dk + 1];
		msglines[dk] = msglines[dk + 1];
		msgtime[dk] = msgtime[dk + 1];
		msgy[dk] = msgy[dk + 1];
	}
}

short
findmsg(short id)
{
	if (id == 0)
		return 0xff;
	for (fi = 0; fi < nmsgs; fi++)
		if (id == msgid[fi])
			return fi;
	return 0xff;
}

void
delmsgid(short id)
{
	short n;

	if (playerdead() || id == 0)
		return;
	n = findmsg(id);
	if (n == 0xff)
		return;
	delmsg(n);
	layoutmsgs();
}

/* delete the messages equal to s */
short
delmsgs(char *s)
{
	if (playerdead())
		return 0;
	dcount = 0;
	for (di = 0; di < nmsgs; di++)
		if (streqi(s, msgtext[di])) {
			delmsg(di);
			dcount++;
			di--;
		}
	layoutmsgs();
	return dcount;
}

/* add s with priority prio, dropping the lowest priority when full */
short
addmsg(short prio, char *s)
{
	if (playerdead())
		return 0;
	while (*s == ' ')
		s++;
	for (ai = 0; ai < nmsgs; ai++)
		if (msgdel[ai]) {
			delmsg(ai);
			ai--;
		}
	if (nmsgs >= maxmsgs) {
		for (ai = 0, aminprio = 9999; ai < nmsgs; ai++)
			if (msgprio[ai] < aminprio) {
				aminprio = msgprio[ai];
				amin = ai;
			}
		if (prio < aminprio)
			return 0;
		delmsg(amin);
	}
	copystr(msgtext[nmsgs], s);
	for (ai = 1; msgtext[nmsgs][ai]; ai++)
		if (msgtext[nmsgs][ai] == 13 || msgtext[nmsgs][ai] == ' ' || msgtext[nmsgs][ai] == ':' || msgtext[nmsgs][ai] == '.') {
			if (iswordc(msgtext[nmsgs][0]))
				msgtext[nmsgs][0] = upper(msgtext[nmsgs][0]);
			sentcase(msgtext[nmsgs]);
			break;
		}
	aline = msgtext[nmsgs];
	wrap(aline, 0);
	for (alen = 0; aline[alen] != '\0'; alen++)
		if (aline[alen] == 13)
			aline[alen] = '\0';
	text3w(aline);
	for (ai = 0; ai < alen; ai++)
		if (aline[ai] == 0)
			aline[ai] = 13;
	if (!(text3width < 620)) {
		if (text3width < 1240) {
			for (ak = 0; ak < 2; ak++)
				for (ai = alen / 5; ai < alen * 3 / 4; ai++)
					if ((aline[ai] == ':' || aline[ai] == '.' || aline[ai] == '!' || aline[ai] == ';' || aline[ai] == '?' || ak > 0 && aline[ai] == ',') && aline[ai + 1] == ' ') {
						aline[ai + 1] = 0;
						text3w(aline);
						if (text3width < 620) {
							text3w(aline + ai + 2);
							if (text3width < 620) {
								aline[ai + 1] = 13;
								goto store;
							}
						}
						aline[ai + 1] = ' ';
					}
		}
		wrap(msgtext[nmsgs], 620);
	}
store:
	msgprio[nmsgs] = prio;
	aid = nextmsgid;
	nextmsgid++;
	if (nextmsgid > 32000)
		nextmsgid = 1;
	msgid[nmsgs] = aid;
	msgdel[nmsgs] = 0;
	msgkeep[nmsgs] = 0;
	msgtime[nmsgs] = gametime;
	nmsgs++;
	newmsgs++;
	return aid;
}

/* drop the last message at the next addmsg */
void
droplast(void)
{
	if (nmsgs)
		msgdel[nmsgs - 1] = 1;
}

void
keeplast(void)
{
	if (nmsgs)
		msgkeep[nmsgs - 1] = 1;
}

short
msgf(short prio, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(msgfmt, fmt, ap);
	// FIXED: va_start without va_end is undefined in C. End the list after its one use.
#ifdef PORT
	va_end(ap);
#endif
	if (playerdead() || msgfmt[0] == 0)
		return 0;
	cleartip();
	msgfid = addmsg(prio, msgfmt);
	layoutmsgs();
	return msgfid;
}

short
msgcat(short prio, char *a, char *b)
{
	if (playerdead())
		return 0;
	wsprintf(msgfmt, texts + 0xb079, a, b);
	if (msgfmt[0] == 0)
		return 0;
	cleartip();
	msgcatid = addmsg(prio, msgfmt);
	layoutmsgs();
	return msgcatid;
}

/* put s before the last message, texts + 0xb080 joining them from 600 pixels */
void
prependmsg(char *s)
{
	if (playerdead() || !nmsgs)
		return;
	prependw = text3w(msgtext[nmsgs - 1]);
	prependw += text3w(s);
	if (prependw >= 600)
		wsprintf(msgfmt, texts + 0xb080, s, msgtext[nmsgs - 1]);
	else
		wsprintf(msgfmt, texts + 0xb079, s, msgtext[nmsgs - 1]);
	msgfmt[0] = upper(msgfmt[0]);
	wsprintf(msgtext[nmsgs - 1], msgfmt);
	layoutmsgs();
}

short
askmsg(char *s)
{
	if (playerdead() || s[0] == 0)
		return 0;
	askmsgid = addmsg(99, s);
	askslot = askmsgid;
	layoutmsgs();
	playnow(0x48);
	return askmsgid;
}

short
failmsg(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(msgfmt, fmt, ap);
	// FIXED: va_start without va_end is undefined in C. End the list after its one use.
#ifdef PORT
	va_end(ap);
#endif
	if (playerdead() || msgfmt[0] == 0)
		return 0;
	cleartip();
	delmsgid(askslot);
	delmsgid(denyslot);
	delmsgid(failslot);
	failmsgid = addmsg(99, msgfmt);
	failslot = failmsgid;
	layoutmsgs();
	playnow(0x49);
	return failmsgid;
}

short
nosuccess(char *s)
{
	if (playerdead())
		return 0;
	wsprintf(msgfmt, texts + 0xb079, s, strs[0x1b4]);
	if (msgfmt[0] == 0)
		return 0;
	cleartip();
	delmsgid(askslot);
	delmsgid(denyslot);
	delmsgid(failslot);
	nosuccid = addmsg(99, msgfmt);
	failslot = nosuccid;
	layoutmsgs();
	playnow(0x49);
	return nosuccid;
}

short
denymsg(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(msgfmt, fmt, ap);
	// FIXED: va_start without va_end is undefined in C. End the list after its one use.
#ifdef PORT
	va_end(ap);
#endif
	if (playerdead() || msgfmt[0] == 0)
		return 0;
	cleartip();
	delmsgid(askslot);
	delmsgid(denyslot);
	delmsgid(failslot);
	denymsgid = addmsg(99, msgfmt);
	denyslot = denymsgid;
	layoutmsgs();
	playnow(0x2c);
	return denymsgid;
}

short
notworking(char *s)
{
	if (playerdead())
		return 0;
	wsprintf(msgfmt, texts + 0xb079, s, strs[0x11f]);
	if (msgfmt[0] == 0)
		return 0;
	cleartip();
	delmsgid(askslot);
	delmsgid(denyslot);
	delmsgid(failslot);
	notworkid = addmsg(99, msgfmt);
	denyslot = notworkid;
	layoutmsgs();
	playnow(0x2c);
	return notworkid;
}

short
usefail(char *s)
{
	if (playerdead())
		return 0;
	failslot = failmsg(strs[0x618], itemname(targetitem & 0xfff), s);
	return failslot;
}

void
notetarget(void)
{
	if (notedunit != targetidx) {
		if (classof(targetitem & 0xfff) != 20)
			notedunit = 0xffff;
		else {
			notedunit = targetidx;
			notedmap = curmap;
		}
		if (notedunit != askedunit) {
			askedunit = 0xff;
			askedonce = 0;
		}
	}
	if (notedkind != targetkind || notedposx != targetx || notedposy != targety || noteditem != targetitem) {
		notedtime = gametime;
		askedonce = 0;
		askedunit = 0xff;
	}
	notedkind = targetkind;
	notedposx = targetx;
	notedposy = targety;
	noteditem = targetitem;
}

void
forgettarget(void)
{
	askedonce = 0;
	askedunit = 0xff;
	if (notedunit == 0xffff && notedkind == 0xff)
		return;
	lastkind = notedkind;
	notedunit = 0xffff;
	askedonce = 0;
	askedunit = 0xff;
	notedkind = 0xff;
}

void
wakeunit(Unit *u)
{
	if (u->mode == 0x5a)
		setunitmode(u, 1);
}

void
witness(unsigned long item, unsigned char t, int n)
{
	if (t == 10 || t == 1 || t == 4)
		rmode = 2;
	else {
		rval = valueof(item) / 10;
		if (rval < 30)
			rmode = 1;
		else if (rval < 100)
			rmode = 2;
		else
			rmode = 3;
	}
	ra = 0;
	rb = 0;
	for (ri = 1; ri < nunits; ri++)
		if (cansee(units[ri].x, units[ri].y)) {
			if (isdog(&units[ri]))
				rb = ri;
			else if (!isanimal(&units[ri]))
				ra = ri;
		}
	if (ra && rb) {
		wakeunit(&units[rb]);
		if (!units[rb].pose)
			turnto(&units[rb], playerx, playery);
		n = 100;
		animalcry(&units[rb]);
		playqueue();
	}
	for (ri = 1; ri < nunits; ri++)
		if (cansee(units[ri].x, units[ri].y) && !units[ri].asleep) {
			if (units[ri].mode == 0x5a) {
				rd = abs(playerx - units[ri].x);
				rd2 = abs(playery - units[ri].y);
				if (rd2 > rd)
					rd = rd2;
				if (randn(100) + n < rd * 15 + 10)
					continue;
			}
			if (!isanimal(&units[ri]) && !(units[ri].traits & 0x20)) {
				if (targetowner == 0xff || targetowner == 0 || allied(&units[ri], &units[targetowner]))
					provoke(units, &units[ri], rmode, 1, 0);
				else
					provoke(units, &units[ri], 1, 2, 0);
			}
		}
}

short
asksteal(short m)
{
	freetake = 0;
	if (trademode)
		return 0;
	ukind = classof(targetitem & 0xfff);
	if (cheat && (keys['W'] & 0x80))
		goto cancel;
	if (targetowner && freegoods(curmap, targetowner))
		goto cancel;
	if (ukind == 10) {
		if (m == 1)
			goto cancel;
		steallock = realms[idof(targetitem)].item;
		if (idof(steallock) && !(steallock & 0x1000))
			goto cancel;
	} else if (targetowner && istrader(&units[targetowner]) && !issmith(&units[targetowner]) && (ukind == 0x37 && typenames[targetitem & 0xfff] != 20 || ukind == 0x36 || ukind == 0x89 || ukind == 0x8b))
		;
	else if (ukind == 0x37 || ukind == 0x36 || ukind == 0x82 || ukind == 0x83 || ukind == 0x84 || ukind == 0x85 || ukind == 0x86) {
		if (m == 2)
			;
		else
			goto cancel;
	} else {
		if (ukind == 0x87) {
			if (m == 1) {
cancel:
				forgettarget();
				freetake = 1;
				if ((ukind == 0x37 || ukind == 0x36) && targetowner && targetowner != 0xff && hosttime + 15 <= gametime) {
					msgf(99, strs[isfemale(&units[targetowner]) ? 0x265 : 0x264], unitname(&units[targetowner]), itemname(targetitem));
					hosttime = gametime;
				}
				return 0;
			}
		} else if (ukind == 0x88 || ukind == 0x89 || ukind == 0x8a || ukind == 0x8b || ukind == 10 || ukind == 0x1e || ukind == 0x97 || ukind == 0x96 || ukind == 0xc8 || ukind == 0xc9 || ukind == 0xca || ukind == 0xcb || ukind == 0xd1 || ukind == 0xcc || ukind == 0xcd || ukind == 0xce || ukind == 0xcf || ukind == 0xd0 || ukind == 0x98 || ukind == 0x3a || ukind == 0x64 || ukind == 0x69 && typenames[targetitem & 0xfff] || ukind == 0x6a && typenames[targetitem & 0xfff] == 0x4b || ukind == 0) {
			if (m == 1)
				goto cancel;
		}
		if (ukind == 1) {
			if (!(targetitem & 0x8000000))
				goto cancel;
		} else if (ukind == 4 && m)
			goto cancel;
	}
	if ((ukind == 0x5a || ukind == 0x59) && itemweight(targetitem) >= 65000 && (!targetowner || targetowner == 0xff || !isfirewood(targetitem)))
		goto cancel;
	if (!(targetitem & 0x1000))
		return 0;
	if (notedkind == targetkind && notedposx == targetx && notedposy == targety && (noteditem & 0xfff) == (targetitem & 0xfff) && notedtime + 14 > gametime) {
		if (stealmsg)
			delmsgid(stealmsg);
		if (attackmsg)
			delmsgid(attackmsg);
		if (targetowner && targetowner != 0xff) {
			if ((ukind == 0x5a || ukind == 0x59) && itemweight(targetitem) >= 65000)
				msgf(99, strs[ukind == 0x5a ? 0x268 : 0x269], unitname(&units[targetowner]));
			else
				msgf(99, strs[614], itemname(targetitem), unitname(&units[targetowner]));
		} else
			msgf(99, strs[615], itemname(targetitem));
		witness(targetitem, ukind, 0);
		stealp = itemptr(targetkind, targetidx);
		if (stealp)
			*stealp &= ~0x1000;
		targetitem &= ~0x1000;
		deedsteal(targetitem);
		return 0;
	}
	notetarget();
	notedunit = 0xffff;
	askedonce = 0;
	askedunit = 0xff;
	if (stealmsg)
		delmsgid(stealmsg);
	if (attackmsg)
		delmsgid(attackmsg);
	if (targetowner && targetowner != 0xff) {
		if (m == 1)
			ustr = 0x259;
		else if (m == 2)
			ustr = 0x25b;
		else if (ukind == 10 || ukind == 1)
			ustr = 0x25d;
		else
			ustr = 0x257;
		stealmsg = msgf(99, strs[ustr], itemname(targetitem), unitname(&units[targetowner]));
	} else {
		if (m == 1)
			ustr = 0x258;
		else if (m == 2)
			ustr = 0x25a;
		else if (ukind == 10 || ukind == 1)
			ustr = 0x25c;
		else
			ustr = 0x256;
		stealmsg = msgf(99, strs[ustr], itemname(targetitem));
	}
	playnow(0x2d);
	return 1;
}

short
askattack(void)
{
	if (targetidx == 0 || lostcontrol && units[0].berserk)
		goto clear;
	if (units[targetidx].sympathy <= 0) {
		if (!isservant(&units[targetidx]) && !isfemale(&units[targetidx]))
			goto clear;
		if (askedunit == targetidx && notedmap == (short)curmap)
			return 0;
	}
	if (isanimal(&units[targetidx]) && !units[targetidx].lord)
		goto clear;
	if (isanimal(&units[targetidx]) && ((units[targetidx].mode == 0x14 || units[targetidx].mode == 0x15) && (units[targetidx].modeb == 0 || units[targetidx].modec == 0) || units[targetidx].mode == 0x19 && units[targetidx].modeb == 0))
		goto clear;
	if (sparring(units, &units[targetidx]))
		goto clear;
	if (units[targetidx].sympathy <= 0)
		;
	else {
		if (units[0].berserk)
			goto attack;
		if (notedunit == targetidx && notedmap == (short)curmap)
			;
		else
			goto ask;
	}
	if (askedunit != targetidx) {
		if (isservant(&units[targetidx])) {
			if (!myservant(&units[targetidx])) {
				attackmsg = msgf(99, strs[isfemale(&units[targetidx]) ? 0x263 : 0x262]);
				goto asked;
			}
		} else if (isfemale(&units[targetidx])) {
			attackmsg = msgf(99, strs[609]);
			goto asked;
		}
	}
attack:
	if (myservant(&units[targetidx]))
		return 0;
	if (!isanimal(&units[targetidx])) {
		if (isfemale(&units[targetidx]))
			deedambushwoman();
		else
			deedambush();
	}
	for (ci = 1; ci < nunits; ci++)
		if (allied(&units[targetidx], &units[ci]))
			units[ci].turnmarks &= ~2;
	return 0;
asked:
	askedunit = targetidx;
	askedonce = 1;
	playnow(0x2d);
	return 1;
ask:
	if (attackmsg)
		delmsgid(attackmsg);
	if (stealmsg)
		delmsgid(stealmsg);
	notedposx = 0;
	notedposy = 0;
	if (isanimal(&units[targetidx])) {
		if (units[targetidx].lord && units[targetidx].lord != 0xff)
			attackmsg = msgf(99, strs[608], unitname(&units[targetidx]), unitname(&units[units[targetidx].lord]));
		else
			attackmsg = msgf(99, strs[607], unitname(&units[targetidx]));
	} else
		attackmsg = msgf(99, strs[606], unitname(&units[targetidx]));
	playnow(0x2d);
	notedunit = targetidx;
	askedonce = 0;
	askedunit = 0xff;
	notedmap = curmap;
	return 1;
clear:
	askedonce = 0;
	askedunit = 0xff;
	return 0;
}

short
woulddie(unsigned long t)
{
	if (knockedout)
		return 0;
	t /= 8;
	wdbleed = units[0].bleeding;
	if (wdbleed > t)
		wdbleed = t;
	wdpoison = units[0].poison;
	if (wdpoison > t)
		wdpoison = t;
	wdloss = wdbleed + wdpoison;
	if (wdbleed + wdpoison >= units[0].health) {
		if (units[0].bleeding >= units[0].poison)
			diemsg = denymsg(strs[1138]);
		else
			diemsg = denymsg(strs[1139]);
		return 1;
	}
	return 0;
}

short
inmelee(Unit *u)
{
	if (badunit(u))
		return 0;
	if (u == units)
		return 1;
	if (u->mode == 0x14 && u->modeb == 0 && abs(u->x - units[0].x) <= 1 && abs(u->y - units[0].y) <= 1)
		return 1;
	return 0;
}

short
hasweapon(Unit *u)
{
	if (!badunit(u)) {
		if (u->hand || u->belt)
			return 1;
		for (gi = 0; gi < 3; gi++)
			if (classof(u->items[gi] & 0xfff) == 0x3c)
				return 1;
		if (u == units)
			for (gi = 0; gi < 50; gi++)
				if (classof(realms[0].items[gi] & 0xfff) == 0x3c)
					return 1;
	}
	return 0;
}

short
isweapon(unsigned long item)
{
	if (classof(item & 0xfff) == 0x3c)
		return 1;
	return 0;
}

short
ispickaxe(unsigned long item)
{
	if (classof(item & 0xfff) == 0x21)
		return 1;
	return 0;
}

short
isthrowing(unsigned long item)
{
	if (isweapon(item) && metals[gearof(item)].thrown)
		return 1;
	return 0;
}

short
isthrowing2(unsigned long item)
{
	if (isthrowing(item))
		return 1;
	return 0;
}

short
ismelee(unsigned long item)
{
	if (isweapon(item) && !metals[gearof(item)].thrown)
		return 1;
	return 0;
}

short
isarmor(unsigned long item)
{
	item = baselook(item) & 0xfff;
	if (!item)
		return 0;
	if (classof(item) == 0x46 && item != 0x1ad && item != 0x1b5)
		return 1;
	return 0;
}

int
isironsword(unsigned long item)
{
	switch (baselook(item & 0xfff)) {
	case 0x1cb:
	case 0x1cf:
		return 1;
	}
	return 0;
}

short
hassecond(Unit *u)
{
	if (badunit(u))
		return 0;
	if (isweapon(u->belt))
		return 1;
	return 0;
}

short
isspear(unsigned long item)
{
	if (isweapon(item) && metals[gearof(item)].kind == 5)
		return 1;
	return 0;
}

short
issword(unsigned long item)
{
	if (isweapon(item) && metals[gearof(item)].kind == 1)
		return 1;
	return 0;
}

short
isdagger(unsigned long item)
{
	if (isweapon(item) && metals[gearof(item)].kind == 2)
		return 1;
	return 0;
}

short
isaxe(unsigned long item)
{
	if (isweapon(item) && metals[gearof(item)].kind == 3)
		return 1;
	return 0;
}

short
isclub(unsigned long item)
{
	if (isweapon(item) && metals[gearof(item)].kind == 4)
		return 1;
	return 0;
}

void
setweapkinds(Unit *u)
{
	if (badunit(u))
		return;
	u->handkind = metals[gearof(u->hand)].kind;
	u->beltkind = metals[gearof(u->belt)].kind;
}

void
animalcry(Unit *u)
{
	if (badunit(u))
		return;
	if (u->type == 0x23)
		queuesound(0x2f);
	else if (u->type == 0x24)
		queuesound(0x31);
	else if (u->type == 0x25)
		queuesound(0x30);
	else if (u->type == 0x30)
		queuesound(0x32);
	else if (u->type == 0x32)
		queuesound(0x33);
}

void
rearm(Unit *u)
{
	if (badunit(u) || u->hand)
		return;
	u->hand = u->belt;
	u->belt = 0;
	if (u == units)
		bloodtill = 0;
}

void
weargear(int a, Unit *u, unsigned long *item, short n, short e)
{
	if (badunit(u) || !u || !item)
		return;
	wgmsg = 0;
	if (item == &u->body && u->cloak) {
		wgmin = armors[gearof(u->cloak)].wearmin;
		if (n >= wgmin)
			addwear(&u->cloak);
	}
	if (!*item)
		return;
	wgbroke = 1;
	switch (e) {
	case 0:
		wgmin = armors[gearof(*item)].wearmin;
		wgmat = armors[gearof(*item)].material;
		if (wgmat == 0)
			wgbroke = 0;
		else if (wgmat != 1 && (armors[gearof(*item)].hardness == 1 || armors[gearof(*item)].hardness == 2))
			wgbroke = 0;
		break;
	default:
		wgmin = metals[gearof(*item)].wearmin;
		wgmat = metals[gearof(*item)].material;
		break;
	}
	if (n < wgmin)
		return;
	if (*item && wearof(*item) == 7 && blessof(*item) == 0)
		goto use;
	wgprio = u == units ? 10 : 5;
	if (inview(u->x, u->y)) {
		if (wgmat == 0)
			queuesound(0x29);
		else if (wgmat == 1)
			queuesound(0x28);
		else
			queuesound(0x2a);
	}
	if (n >= wgmin * 2 && addwear(item) || addwear(item)) {
use:
		if (u == units)
			wgprio = 25;
		else
			wgprio = 15;
		if (inview(u->x, u->y)) {
			if (u == units) {
				delmsgid(wgmsg);
				wgmsg = msgf(wgprio, strs[wgbroke ? 0x145 : 0x146], itemname(*item));
			}
			if (wgmat == 0)
				queuesound(0x29);
			else if (wgmat == 1)
				queuesound(0x28);
			else
				queuesound(0x2a);
		}
		wgwreck = broken(*item);
		if (wgwreck)
			giveitem(indexof(u), wgwreck, 1);
		*item = 0;
		rearm(u);
		calcstats(u);
		if (u == units)
			dresshero();
	}
}

void
setpleader(Unit *u)
{
	if (badunit(u) || pleader)
		return;
	pleader = indexof(u);
}

void
setmerciful(Unit *u)
{
	if (badunit(u) || merciful)
		return;
	merciful = indexof(u);
}

int
addblood(int x, int y, int a, int b)
{
	if (a == -1)
		;
	else if (censorship == 1 || censorship == 4)
		return 0;
	if (nmapitems >= 32267)
		return 0;
	y *= 150;
	y += x;
	x = map[y];
	if (!(x & 0x4000))
		return 0;
	if (!cellholds(x))
		return 0;
	if (x & 0x1000) {
		x >>= 16;
		for (;;) {
			bloodit = mapitems[x];
			if (bloodit & 0x6000)
				break;
			if (!itemholds(bloodit))
				return 0;
			if (itemweight(bloodit) < 65000)
				return 0;
			switch (classof(bloodit & 0xfff)) {
			case 0:
			case 0x67:
			case 0x68:
			case 0x6f:
			case 0x70:
			case 0xe6:
			case 0xe7:
			case 0xe8:
				break;
			default:
				return 0;
			}
			if (a == -1)
				;
			else {
				if ((bloodit & 0xfff) >= 0x94b && (bloodit & 0xfff) <= 0x94f)
					return 1;
				if ((bloodit & 0xfff) >= 0x950 && (bloodit & 0xfff) <= 0x954) {
					if (a) {
						mapitems[x] &= 0xfffff000;
						mapitems[x] |= randn(5) + 0x94b;
					}
					return 1;
				}
			}
			if (!(bloodit & 0x8000))
				break;
			x++;
		}
	}
	if (a == -1)
		return 1;
	if (a)
		additem(y, b << 16 | randn(5) + 0x94b);
	else
		additem(y, b << 16 | randn(5) + 0x950);
	return 1;
}

/* unit u takes dmg points of damage of kind 1 to 6 from a, on slot 1 to 3 */
void
hurt(Unit *a, Unit *u, short kind, short slot, short dmg)
{
	short pt0;
	short i;

	hdmg0 = 0;
	hdmg1 = 0;
	hdmg2 = 0;
	hitem = 0;
	hkind = 0;
	hmsg = 0;
	hsound = 0;
	if (badunit(u))
		return;
	hstate0 = healthof(u);
	hsaid = 0;
	if (a && sparring(a, u)) {
		if (u->health > 80) {
			dmg /= 10;
			if (dmg < 1)
				dmg = 1;
			else if (dmg > 5)
				dmg = 5;
		} else
			dmg = 0;
		if (a->mode == 0x16)
			a->modef++;
		if (u->mode == 0x16)
			u->modee++;
	}
	hdmg0 = dmg;
	hslot = 0;
	if (kind == 6)
		;
	else {
		switch (slot) {
		case 2:
			if (!u->body)
				if (u->cloak)
					hslot = &u->cloak;
				else
					goto skip;
			else
				hslot = &u->body;
			break;
		case 1:
			if (u->head == 0)
				goto skip;
			hslot = &u->head;
			break;
		case 3:
			if (u->legs == 0)
				goto skip;
			hslot = &u->legs;
			break;
		default:
			goto skip;
		}
		hitem = *hslot;
		hgear = gearof(*hslot);
		hprot = 0;
		switch (kind) {
		case 1:
		case 5:
			hprot = armors[hgear].prot1;
			break;
		case 3:
			hprot = armors[hgear].prot3;
			break;
		case 2:
		case 4:
		default:
			hprot = armors[hgear].prot2;
			break;
		}
		hprot = armorval(*hslot, hprot);
		if (a && (kind == 1 || kind == 3 || kind == 2) && (armors[hgear].hardness == 2 || armors[hgear].hardness == 3))
			weargear(0, a, &a->hand, dmg, 1);
		// PORT: 0 for the parameter that weargear leaves unused, where 64 bits truncate the pointer
#ifdef PORT
		weargear(0, u, hslot, dmg, 0);
#else
		weargear((int)a, u, hslot, dmg, 0);
#endif
		if (u == units && restbreak) {
			restfor(0xfe);
			return;
		}
		dmg -= hprot;
		if (dmg <= 0 || hprot >= 5 && u->health - dmg >= kindof(u)->maxhealth * 4 / 5) {
			if (dmg <= 0 && u->health > 7)
				dmg = 1;
			hprio = u == units ? 10 : 5;
			if (inview(u->x, u->y)) {
				if (healthid(u->health - dmg, kindof(u)->maxhealth) != hstate0) {
					;
					hkind = armors[hgear].material;
				} else {
					if (u == units) {
						if (u->health > 1) {
							msgf(hprio, strs[classof(hitem & 0xfff) != 0x4b ? 0x147 : 0x149], attacktext(a, u), itemname(hitem));
							hsaid = 1;
						}
					} else if (a == units && u->health > 1) {
						msgf(hprio, strs[classof(hitem & 0xfff) != 0x4b ? 0x148 : 0x14a], unitname(u), itemname(hitem));
						hsaid = 1;
					}
					hkind = armors[hgear].material;
				}
				if (slot == 1) {
					if (hkind == 2)
						queuesound(0x22);
					else
						queuesound(0x24);
				} else if (hkind == 2)
					queuesound(0x23);
				else
					queuesound(0x24);
			}
			if (dmg <= 0)
				return;
		} else if (u->health - dmg >= kindof(u)->maxhealth * 4 / 5) {
			hprio = u == units ? 10 : 5;
			if (inview(u->x, u->y) && a == units)
				msgf(hprio, strs[0x14d], unitname(u));
		}
	}
skip:
	if (a == units && a->hand)
		bloodtill = hourticks / 2 + gametime;
	hdmg1 = dmg;
	switch (kind) {
	case 1:
		dmg = dmg * kindof(u)->dmg1 / 100;
		break;
	case 2:
	case 3:
		dmg = dmg * kindof(u)->dmg2 / 100;
		break;
	case 5:
		dmg = kindof(u)->dmg5 * dmg / 100;
		break;
	case 6:
		pt0 = kindof(u)->dmg6;
		dmg = pt0 * dmg / 100;
		break;
	}
	switch (slot) {
	case 1:
		dmg = dmg * 13;
		dmg = dmg / 10;
		break;
	case 2:
		break;
	case 3:
		dmg *= 4;
		dmg /= 10;
		break;
	}
	if (u->berserk)
		dmg /= 3;
	else if (u->warded) {
		dmg *= 3;
		dmg /= 4;
	} else if (dmg >= kindof(u)->maxhealth / 8 && !spared(curmap, indexof(u)) && !isskeleton(u)) {
		hprot = randn(dmg / 3);
		if (hprot + u->bleeding > 0xff)
			u->bleeding = 0xff;
		else
			u->bleeding += hprot;
		if (u->bleeding >= kindof(u)->maxhealth / 10 && inview(u->x, u->y))
			hsound = 1;
	}
	hdmg2 = dmg;
	if (dmg >= 8)
		addblood(u->x, u->y, dmg >= 13 ? 1 : 0, 0);
	if (u == units && restbreak)
		dmg = 0;
	u->health -= dmg;
	if (spared(curmap, indexof(u)) && u->health < kindof(u)->maxhealth * 3 / 4)
		u->health = kindof(u)->maxhealth * 3 / 4;
	hprot = 0;
	if (u->health <= 0) {
		if (u == units && merciful || merciful && indexof(u) == merciful || pleader && indexof(u) == pleader) {
			u->health = 9;
			goto alive;
		}
		if (a == units && !a->berserk && u->plea) {
			setpleader(u);
			u->health = 9;
			goto alive;
		}
		if (u == units && !u->berserk && a) {
			if (a->lord && units[a->lord].mercy && inview(units[a->lord].x, units[a->lord].y)) {
				setmerciful(&units[a->lord]);
				u->health = 9;
				goto alive;
			}
			if (a->mercy) {
				setmerciful(a);
				u->health = 9;
				goto alive;
			}
		}
		if (a == units && (!isanimal(u) || isbear(u))) {
			if (u->mode == 0x19 && u->modeb == 0)
				deedkillflee();
			else
				deeddefeat(u);
		}
		u->health = 0;
		if (a == units && myservant(u))
			u->sympathy = 0;
		if (inview(u->x, u->y)) {
			delmsgid(hmsg);
			if (u == units) {
				if (robbery(curmap, indexof(a))) {
					game.killer = indexof(a);
					game.fate = 1;
					units->health = 0;
					goto end;
				} else {
					msgf(0x63, strs[0x14e], attacktext(a, u));
					music(2, 0);
					queuesound(0x1b);
				}
			} else {
				if (a == units)
					msgf(u->sympathy > 0 ? 0x1a : 0x1f, strs[0x14f], unitname(u));
				else
					msgf(u->sympathy > 0 ? 0x1a : 0x1f, strs[0x150], unitname(u), unitname(a));
				if (isanimal(u))
					animalcry(u);
				else if (isfemale(u))
					queuesound(0x4b);
				else
					queuesound(0x1b);
				if (u->sympathy <= 0)
					music(0x23, 0);
			}
		}
		if (u == units && restbreak) {
			u->health = kindof(u)->maxhealth;
			goto end;
		}
		if (a == units && !isanimal(u)) {
			if (isfemale(u)) {
				deedkillwoman();
				if (!isservant(u)) {
					outrage(1);
					nomercy(u);
				}
			} else
				deedkill();
		}
		if (u == units) {
			game.fate = 1;
			if (a == 0)
				game.killer = 0;
			else if (a->lord && inview(units[a->lord].x, units[a->lord].y))
				game.killer = a->lord;
			else
				game.killer = indexof(a);
		}
		if (a == units) {
			if (u->head)
				deedbooty(u->head);
			if (u->body && isarmor(u->body))
				deedbooty(u->body);
			if (u->cloak)
				deedbooty(u->cloak);
			if (u->wrist)
				deedbooty(u->wrist);
			if (u->neck)
				deedbooty(u->neck);
			if (u->hand)
				deedbooty(u->hand);
			if (u->belt)
				deedbooty(u->belt);
			if (u->offhand)
				deedbooty(u->offhand);
			i = 0;
			while (i < 3) {
				if (u->items[i])
					deedbooty(u->items[i]);
				i++;
			}
		}
		if (!(a == units && lostcontrol == 0) && u != units && u->hand)
			if (a->hand == 0 || valueof(u->hand) >= valueof(a->hand) && blessof(u->hand) <= 9) {
				giveitem(indexof(a), u->hand, 1);
				u->hand = 0;
			}
		killunit(u, 1, 1, indexof(a), 0);
	} else {
	alive:
		hstate1 = healthof(u);
		if (inview(u->x, u->y)) {
			if (hstate1 == 0x21c) {
				if (a == units && !hsaid || u == units && hstate1 != hstate0)
					msgf(0x10, strs[u == units ? 0x157 : 0x158], attacktext(a, u));
				hsound = 2;
			} else if (hstate1 == 0x21d) {
				if (a == units && !hsaid || u == units && hstate1 != hstate0)
					msgf(0xf, strs[u == units ? 0x159 : 0x15a], attacktext(a, u));
				hsound = 2;
			} else if (hstate1 == 0x21e) {
				if (a == units && !hsaid || u == units && hstate1 != hstate0)
					msgf(0xa, strs[u == units ? 0x15b : 0x15c], attacktext(a, u));
				if (u != units && hsound == 0)
					hsound = 3;
			}
		}
		switch (hsound) {
		case 1:
		case 2:
			if (u != units) {
				if (isanimal(u))
					animalcry(u);
				else if (isfemale(u))
					queuesound(0x4b);
				else
					queuesound(0x1c);
			} else
				queuesound(0x27);
			break;
		case 3:
			if (isanimal(u) != 0)
				animalcry(u);
			else if (isfemale(u))
				queuesound(0x4b);
			else
				queuesound(0x1d);
			break;
		}
		if (mustflee(u)) {
			u->asleep = 0;
			if (u->mode != 0x19) {
				if (a == units && !a->berserk && u->plea)
					setpleader(u);
				else {
					if (inview(u->x, u->y)) {
						if (a == units)
							msgf(0x11, strs[0x152], unitname(u));
						else if (u == units)
							msgf(0x11, strs[0x151], attacktext(a, u));
						else if (u->mode == 0x14 && u->modeb != 0xff && (units[u->modeb].sympathy <= 0 || u->sympathy <= 0))
							msgf(0x11, strs[0x153], unitname(u), unitname(a));
						if (u != units && u->sympathy <= 0)
							music(0x23, 0);
					}
					setunitmode(u, 0x19);
					plotflee((short)curmap, indexof(u));
					if (a == units && (!isanimal(u) || isbear(u)))
						deeddefeat(u);
				}
			}
		}
	}
	if (a == 0)
		hprot = 0;
	else if (isspider(a)) {
		hprot = u->poison + 10 + randn(50);
		if (hprot > 0xff)
			hprot = 0xff;
		u->poison = hprot;
	} else if (israt(a)) {
		hprot = u->poison + 2 + randn(10);
		if (hprot > 0xff)
			hprot = 0xff;
		u->poison = hprot;
	}
end:
	if (u == units) {
		tire(5);
		fillgauges();
	}
	calcstats(u);
}

/* the damage *dmg, its kind, and the ranges *a and *b of u's weapon */
void
attackval(Unit *u, short *dmg, short *kind, short *a, short *b, short n)
{
	if (badunit(u))
		return;
	warm = gearof(u->hand);
	if (n && n <= 1 && isthrowing(u->hand) && warm == 0xf)
		warm = 0x11;
	wskill = u->strength - metals[warm].minstr;
	if (wskill > 0)
		wskill = 0;
	*dmg = skillof(u, 0xb, 0, 0, 0);
	wpct = skillof(u, metals[warm].skill, wskill, 0, 0) - 100;
	wpct /= 2;
	wpct += 100;
	*dmg = *dmg * wpct / 100;
	*dmg = *dmg * metals[warm].attack / 100;
	*dmg = *dmg * ((blessof(u->hand) - 7) * 3 + 100) / 100;
	if (u->cursed)
		*dmg /= 2;
	if (u->blessed)
		*dmg = *dmg * 133 / 100;
	if (*dmg <= 0)
		*dmg = 1;
	*kind = metals[warm].dmgkind;
	if (*kind == 4) {
		if ((unsigned)random() < 0x5554)
			*kind = 2;
		else
			*kind = 1;
	}
	damagerange(u->hand, a, b);
	*b += kindof(u)->dmgadd;
	wpct = skillof(u, metals[warm].skill, wskill, 0, 0);
	*a = *a * wpct / 100;
	*b = *b * wpct / 100;
	if (*a <= 0)
		*a = 1;
	if (*b <= 1)
		*b = 2;
}

/* the defense values of u: parry *a, shield *b, and dodge *c */
void
defenseval(Unit *u, short *a, short *b, short *c)
{
	if (badunit(u) || u->mode == 0x5a || u->asleep) {
		*a = 0;
		*b = 0;
		*c = 0;
		return;
	}
	if (u->hand == 0)
		*a = 0;
	else {
		aidx = gearof(u->hand);
		askill = u->strength - metals[aidx].minstr;
		if (askill > 0)
			askill = 0;
		*a = skillof(u, 0xc, 0, 0, 0);
		apct = skillof(u, metals[aidx].skill, askill, 0, 0) - 100;
		apct /= 2;
		apct += 100;
		*a = *a * apct / 100;
		*a = *a * metals[aidx].parry / 100;
		*a = *a * ((blessof(u->hand) - 7) * 3 + 100) / 100;
	}
	if (classof(u->offhand & 0xfff) == 0x55) {
		aidx = gearof(u->offhand);
		*b = skillof(u, 0xe, 0, 0, 0);
		*b = *b * armors[aidx].shieldpct / 100;
		*b = *b * ((blessof(u->offhand) - 7) * 3 + 100) / 100;
	} else
		*b = 0;
	*c = skillof(u, 0xd, 0, 0, 0);
	if (u->asleep) {
	zero:
		*a = 0;
		*b = 0;
		*c = 0;
	} else
		switch (u->mode) {
		case 4:
		case 0x32:
			*a /= 2;
			*b /= 2;
			*c /= 2;
			break;
		case 0x14:
		case 0x16:
			break;
		case 0x15:
			*a *= 2;
			*a /= 3;
			*b *= 2;
			*b /= 3;
			*c *= 2;
			*c /= 3;
			break;
		case 0x19:
			*a = 0;
			*b /= 4;
			*c /= 4;
			break;
		case 0x5a:
			goto zero;
		default:
			*a = 0;
			*b /= 6;
			*c /= 6;
			break;
		}
	if (u->warded) {
		*a += (short)(*a / 4);
		*b += (short)(*b / 4);
		*c += (short)(*c / 4);
	}
	if (u->blessed) {
		*a += (short)(*a / 5);
		*b += (short)(*b / 5);
		*c += (short)(*c / 5);
	}
	if (u->cursed) {
		*a /= 2;
		*b /= 2;
		*c /= 2;
	}
}

/* a attacks u */
void
assault(Unit *a, Unit *u)
{
	if (badunit(a) || badunit(u) || sparring(a, u))
		return;
	if (a == units)
		plotfight(curmap, indexof(u));
	else if (u == units)
		plotfight(curmap, indexof(a));
	if (induel(a, u)) {
		if (u == units)
			a->sympathy = 0;
		else if (a == units)
			u->sympathy = 0;
		return;
	}
	if (a == units && isanimal(u) && u->lord) {
		if ((u->mode == 0x14 || u->mode == 0x15) && (u->modeb == indexof(a) || u->modec == indexof(a)))
			provoke(a, u, 3, 0, 0);
		else if (u->mode == 0x19 && u->modeb == indexof(a))
			provoke(a, u, 3, 0, 0);
		else {
			if (u->lord != 0xff)
				msgf(0x63, strs[0x266], unitname(u), unitname(&units[u->lord]));
			else
				msgf(0x63, strs[0x267], unitname(u));
			deedsteal(unititem(u));
			provoke(a, u, 3, 1, 0);
			u->lord = 0;
		}
	} else
		provoke(a, u, 3, 0, 0);
	if (a == units && isfemale(u) && !isservant(u)) {
		outrage(1);
		nomercy(u);
	}
}

void
trainswing(void)
{
	tire(1);
	train(0xb, units->mode != 0x16 ? 2 : 1);
	train(weaponskill(), units->mode != 0x16 ? 2 : 1);
}

void
trainhit(void)
{
	train(0xb, units->mode != 0x16 ? 4 : 2);
	train(weaponskill(), units->mode != 0x16 ? 4 : 2);
}

/* *dx, *dy: the cell as far past (x, y) as (x, y) lies from u, *d that distance, at least 1 */
void
aimpast(Unit *u, int x, int y, short *dx, short *dy, short *d)
{
	if (badunit(u))
		return;
	*dx = x - u->x;
	*dy = y - u->y;
	// FIXED: the distance of a throw is dx alone once dx passes 1, so a throw 2 across and 10 down counts 2 cells. Take the longer of dx and dy.
#if defined(FEATURE_FIXES)
	if (abs(*dx) < abs(*dy))
#else
	if (*dx >= -1 && *dx <= 1)
#endif
		*d = *dy;
	else
		*d = *dx;
	if (*d < 0)
		*d = -*d;
	if (*d < 1)
		*d = 1;
	*dx += u->x + *dx;
	*dy += u->y + *dy;
}

short
throwmiss(Unit *u, short n)
{
	if (badunit(u))
		return 1;
	tmres = 0;
	if (n > 1) {
		tmskill = skillof(u, metals[gearof(u->hand)].skill, 0, 0, 0);
		tmskill -= (n - 1) * 2;
		if (tmskill < 50)
			tmskill = 50;
		if (randn(90) > tmskill)
			tmres = 1;
	}
	return tmres;
}

void
missmsg(Unit *a, Unit *u)
{
	if (badunit(a) || badunit(u) || !inview(a->x, a->y))
		return;
	if (a == units)
		msgf(0xc, strs[0x507]);
	else if (u == units)
		msgf(0xc, strs[0x508], attacktext(a, u));
	if (a != units)
		queuesound(0x26);
	else
		queuesound(0x25);
}

/* u throws the item in hand from (x0, y0) toward (x, y) */
short
flyon(Unit *u, short x0, short y0, short x, short y, short n, short m, short k)
{
	if (badunit(u) || n <= 1)
		return 0xff;
	if (m) {
		if (!(x0 == x && y0 == y)) {
			clearsight(x0, y0, x, y);
			if (blocker != 0xff && !k)
				return blocker;
			x = lineokx;
			y = lineoky;
		}
	} else {
		x = x0;
		y = y0;
	}
	if (deepwater(x, y))
		deedsink(u->hand);
	else {
		if (!m && findclass(y * 150 + x, 0x98)) {
			u->hand &= 0xfffff000;
			if (abs(x - u->x) > abs(y - u->y)) {
				if (u->x < x)
					u->hand |= 0x6e7;
				else
					u->hand |= 0x6e6;
			} else {
				if (u->y < y)
					u->hand |= 0x6e5;
				else
					u->hand |= 0x6e4;
			}
		} else
			u->hand = randlook(u->hand);
		additem(y * 150 + x, u->hand);
	}
	u->hand = 0;
	rearm(u);
	calcstats(u);
	if (u == units)
		dresshero();
	return 0xff;
}

short
hitroll(Unit *a, Unit *u)
{
	if (a == units) {
		if (oddsswitch)
			return randn(80) + 20;
		return randn(50);
	}
	if (u == units) {
		if (oddsswitch)
			return randn(80);
		return randn(50) + 50;
	}
	return randn(100);
}

/* a strikes u, and a thrown item goes on to the units it reaches */
void
strike(Unit *a, Unit *u)
{
	if (badunit(a) || badunit(u) || u->health <= 0)
		return;
	if (a == units) {
		trainswing();
		if ((u->mode == 0x5a || u->asleep) && !isanimal(u))
			deedsleeper();
		if (!sparring(a, u) && isfemale(u))
			deedhitwoman();
	}
	if (u->sympathy <= 0 && a->sympathy >= 1)
		setfought(a);
	if (a->sympathy <= 0 && u->sympathy >= 1)
		setfought(u);
	aimpast(a, u->x, u->y, &sdx, &sdy, &sdist);
	for (;;) {
		sx = u->x;
		sy = u->y;
		a->pose = 3;
		a->lasthit = indexof(u);
		if (a->modeb != a->lasthit) {
			a->modec = a->modeb;
			a->modeb = a->lasthit;
		}
		a->lasthit = 0;
		if (a->modeb == a->modec)
			a->modec = 0xff;
		smissed = throwmiss(a, sdist);
		if (u->bewitched && u->bewitcher == indexof(a)) {
			u->bewitched = 0;
			u->bewitcher = 0;
			u->mode = 0;
		}
		if (a == units && game.wodenblows) {
			if (game.invoketime + 9 < gametime)
				game.wodenblows = 0;
			else {
				smissed = 0;
				assault(a, u);
				attackval(a, &satk, &skind, &smin, &smax, sdist);
				hurt(a, u, skind, 2, kindof(u)->maxhealth * 2);
				game.wodenblows--;
				goto next;
			}
		}
		if (u == units && avengers && ((unsigned short)curmap == 1 || (unsigned short)curmap == 5)) {
			smissed = 0;
			assault(a, u);
			attackval(a, &satk, &skind, &smin, &smax, sdist);
			if (sdist > 1 && isspear(a->hand) && classof(u->offhand & 0xfff) == 0x55)
				goto steal;
			hurt(a, u, skind, 2, smax);
		} else if ((u->mode == 0x5a || u->asleep) && (assault(a, u), !smissed)) {
			attackval(a, &satk, &skind, &smin, &smax, sdist);
			hurt(a, u, skind, 0xff, smax * 2);
		} else {
			defenseval(u, &sparry, &sshield, &sdodge);
			if (u == units)
				;
			else if ((u->mode == 0x14 || u->mode == 0x15) && u->modeb != indexof(a)) {
				if (u->modec != indexof(a)) {
					sparry /= 2;
					sshield /= 2;
					sdodge /= 2;
				} else {
					sparry = sparry * 2 / 3;
					sshield = sshield * 2 / 3;
					sdodge = sdodge * 2 / 3;
				}
			}
			sexposed = 0;
			if (u == units)
				;
			else if (u->x < a->x && u->dirx < 0 || u->x > a->x && u->dirx > 0 || u->y < a->y && u->diry < 0 || u->y > a->y && u->diry > 0) {
				sparry /= 2;
				sshield /= 2;
				sdodge /= 2;
				sexposed = 1;
			}
			// FIXED: a call through another function type is undefined in C. Call it through its own, which returns the same 0 or 1.
#ifdef PORT
			if (hampered(curmap, indexof(u), indexof(a))) {
#else
			if (((int (*)(short, short, short))hampered)(curmap, indexof(u), indexof(a))) {
#endif
				sparry /= 2;
				sshield /= 2;
				sdodge /= 2;
				sexposed = 1;
			// FIXED: a call through another function type is undefined in C. Call it through its own, which returns the same 0 or 1.
#ifdef PORT
			} else if (favored(curmap, indexof(u), indexof(a))) {
#else
			} else if (((int (*)(short, short, short))favored)(curmap, indexof(u), indexof(a))) {
#endif
				sparry *= 2;
				sshield *= 2;
				sdodge *= 2;
				sexposed = 0;
			}
			assault(a, u);
			if (sdist > 1) {
				sparry = 0;
				sdodge += (sdist - 1) * 2;
			}
			attackval(a, &satk, &skind, &smin, &smax, sdist);
			if (u == units)
				smax = (smax - smin) / 2 + smin;
			if (a == units)
				smin = (smax - smin) / 2 + smin;
			// FIXED: a call through another function type is undefined in C. Call it through its own, which returns the same 0 or 1.
#ifdef PORT
			if (hampered(curmap, indexof(a), indexof(u))) {
#else
			if (((int (*)(short, short, short))hampered)(curmap, indexof(a), indexof(u))) {
#endif
				smax = smin;
				satk /= 2;
			// FIXED: a call through another function type is undefined in C. Call it through its own, which returns the same 0 or 1.
#ifdef PORT
			} else if (favored(curmap, indexof(a), indexof(u))) {
#else
			} else if (((int (*)(short, short, short))favored)(curmap, indexof(a), indexof(u))) {
#endif
				satk *= 2;
				smin = smax;
			}
			if (sexposed)
				sdmg = smax;
			else
				sdmg = randn(smax - smin + 1) + smin;
			provoke(a, u, 3, 0, 0);
			if (sdist > 1 && smissed) {
				missmsg(a, u);
				goto next;
			}
			if (u == units && game.wodenblows) {
				if (game.invoketime + 9 < gametime)
					game.wodenblows = 0;
				else
					goto miss;
			}
			if (u->turnmarks & 1 || a == units && cheat && (keys['W'] & 0x80))
				;
			else {
				u->turnmarks |= 1;
				sroll = randn(sparry + sshield * 2 + sdodge);
				if (sdist > 1)
					;
				else {
					if (sroll < sparry) {
						if (u == units) {
							tire(1);
							train(0xc, a->mode != 0x16 ? 2 : 1);
							train(weaponskill(), a->mode != 0x16 ? 2 : 1);
						}
						sluck = hitroll(a, u);
						if (odds(sparry, satk) <= sluck)
							goto hit;
						if (u == units) {
							train(0xc, a->mode != 0x16 ? 4 : 2);
							train(weaponskill(), a->mode != 0x16 ? 4 : 2);
						}
						if (inview(u->x, u->y)) {
							if (u == units)
								msgf(0xc, strs[0x15d], attacktext(a, u), itemname(u->hand));
							else if (a == units)
								msgf(0xc, strs[0x15e], attacktext(a, u), itemname(u->hand));
							// FIXED: a call through another function type is undefined in C. Call it through its own, with the same value.
#ifdef PORT
							queuesound(issword(u->hand) && issword(a->hand) ? 0x1f : 0x20);
#else
							((void (*)(int))queuesound)(issword(u->hand) && issword(a->hand) ? 0x1f : 0x20);
#endif
						}
						{
							/* named, for the original's stack slots */
							short r = sparring(a, u);
							unsigned long *g = &u->hand;

							if (r) {
								weargear(0, a, &a->hand, sdmg * 4 / 5, 1);
								// PORT: 0 for the parameter that weargear leaves unused, where 64 bits truncate the pointer
#ifdef PORT
								weargear(0, u, g, sdmg * 4 / 5, 1);
#else
								weargear((int)a, u, g, sdmg * 4 / 5, 1);
#endif
							} else {
								weargear(0, a, &a->hand, sdmg, 1);
								// PORT: 0 for the parameter that weargear leaves unused, where 64 bits truncate the pointer
#ifdef PORT
								weargear(0, u, g, sdmg, 1);
#else
								weargear((int)a, u, g, sdmg, 1);
#endif
							}
						}
						goto next;
					}
					{
						int t = sparry;	/* for the original's registers */

						if (sroll >= sdodge + t)
							goto shield;
					}
				}
				if (u == units) {
					tire(1);
					train(0xd, a->mode != 0x16 ? 2 : 1);
				}
				sluck = hitroll(a, u);
				if (odds(sdodge, satk) > sluck) {
			miss:
					if (u == units)
						train(0xd, a->mode != 0x16 ? 4 : 2);
					if (inview(u->x, u->y)) {
						if (a == units)
							msgf(0xc, strs[0x160], attacktext(a, u));
						else if (u == units)
							msgf(0xc, strs[0x15f], attacktext(a, u));
						// FIXED: a call through another function type is undefined in C. Call it through its own, with the same value.
#ifdef PORT
						queuesound(a != units ? 0x26 : 0x25);
#else
						((void (*)(int))queuesound)(a != units ? 0x26 : 0x25);
#endif
					}
					smissed = 1;
					goto next;
				}
				goto hit;
			shield:
				if (u == units) {
					tire(1);
					train(0xe, a->mode != 0x16 ? 2 : 1);
				}
				sluck = hitroll(a, u);
				if (odds(sshield, satk) <= sluck)
					goto hit;
				if (u == units)
					train(0xe, a->mode != 0x16 ? 4 : 2);
				if (inview(u->x, u->y)) {
					if (a == units)
						msgf(0xc, strs[0x162], attacktext(a, u), itemname(u->offhand));
					else if (u == units)
						msgf(0xc, strs[0x161], attacktext(a, u), itemname(u->offhand));
					queuesound(0x21);
				}
				if (sparring(a, u)) {
					weargear(0, a, &a->hand, sdmg * 4 / 5, 1);
					// PORT: 0 for the parameter that weargear leaves unused, where 64 bits truncate the pointer
#ifdef PORT
					weargear(0, u, &u->offhand, sdmg * 4 / 5, 0);
#else
					weargear((int)a, u, &u->offhand, sdmg * 4 / 5, 0);
#endif
				} else {
					weargear(0, a, &a->hand, sdmg, 1);
					// PORT: 0 for the parameter that weargear leaves unused, where 64 bits truncate the pointer
#ifdef PORT
					weargear(0, u, &u->offhand, sdmg, 0);
#else
					weargear((int)a, u, &u->offhand, sdmg, 0);
#endif
					// FIXED: the shield takes the blow twice, where the sparring branch and the parry wear each piece once. Wear it once.
#ifndef FEATURE_FIXES
					// PORT: 0 for the parameter that weargear leaves unused, where 64 bits truncate the pointer
#ifdef PORT
					weargear(0, u, &u->offhand, sdmg, 0);
#else
					weargear((int)a, u, &u->offhand, sdmg, 0);
#endif
#endif
				}
				goto next;
			}
		hit:
			sroll = randn(100);
			if (skind == 1) {
				if (sroll < 70)
					sslot = 2;
				else if (sroll < 85)
					sslot = 1;
				else
					sslot = 3;
			} else if (sroll < 60)
				sslot = 2;
			else if (sroll < 90)
				sslot = 1;
			else
				sslot = 3;
			if (a == units)
				trainhit();
			if (u == units)
				train(0x14, a->mode != 0x16 ? 2 : 1);
			if (a == units && cheat && (keys['W'] & 0x80))
				sdmg = 300;
			if (sdist > 1 && isspear(a->hand) && classof(u->offhand & 0xfff) == 0x55)
				goto steal;
			hurt(a, u, skind, sslot, sdmg);
		}
		goto next;
	steal:
		if (inview(u->x, u->y))
			queuesound(0x21);
		if (sparring(a, u)) {
			weargear(0, a, &a->hand, sdmg * 4 / 5, 1);
			// PORT: 0 for the parameter that weargear leaves unused, where 64 bits truncate the pointer
#ifdef PORT
			weargear(0, u, &u->offhand, sdmg * 4 / 5, 0);
#else
			weargear((int)a, u, &u->offhand, sdmg * 4 / 5, 0);
#endif
		} else {
			weargear(0, a, &a->hand, sdmg, 1);
			// PORT: 0 for the parameter that weargear leaves unused, where 64 bits truncate the pointer
#ifdef PORT
			weargear(0, u, &u->offhand, sdmg, 0);
#else
			weargear((int)a, u, &u->offhand, sdmg, 0);
#endif
		}
		u->offhand = randlook(u->offhand);
		additem(sy * 150 + sx, u->offhand);
		u->offhand = 0;
		calcstats(u);
		if (u == units)
			dresshero();
		if (a == units)
			msgf(0xc, strs[0x14b], attacktext(a, u));
		else if (u == units)
			msgf(0xc, strs[0x14c], attacktext(a, u));
	next:
		sroll = flyon(a, sx, sy, sdx, sdy, sdist, smissed, 0);
		if (sroll == 0xff)
			return;
		u = &units[sroll];
		sdist += (short)(sdist / 2);
	}
}

/* the player shoots at (x, y) */
void
throwat(short x, short y)
{
	trainswing();
	aimpast(units, x, y, &throwx, &throwy, &throwdist);
	units[0].pose = 3;
	throwfail = throwmiss(units, throwdist);
	if (randn(100) < 8)
		throwfail = 1;
	if (throwdist <= 1)
		throwfail = 0;
	if (throwfail)
		missmsg(units, 0);
	else {
		msgf(0x10, strs[0x509]);
		queuesound(0x24);
		trainhit();
	}
	flyon(units, x, y, throwx, throwy, throwdist, throwfail, 1);
}

short
castat(Unit *u, short n, short cmd)
{
	if (!badunit(u) && !badunit(&units[n])) {
		for (casti = 0; casti < 40 && actions[casti].cmd != cmd; casti++)
			;
		if (cmd == 0x17 && (units[n].cursed || units[n].weakened) || cmd == 0x15 || cmd == 0xf) {
			if (n == 0)
				setfought(u);
		} else if (units[n].sympathy <= 0)
			setfought(u);
		castres = canperform(u, cmd, n, -1, 0);
		if (castres)
			castres = perform(u, cmd, n, -1, 0, 0);
		return castres;
	}
	return 0;
}

/* u casts at unit n the spell that its spellset picks, or returns -1 */
int
castspell(Unit *u, unsigned char n)
{
	if (badunit(u) || badunit(&units[n]))
		return 0;
	spellkind = spellset(u);
	if (jewelmagic(&units[n], 0x80000))
		spellward = 1;
	else
		spellward = 0;
	if (spellkind == 1) {
		if (u->cursed || u->weakened) {
			spellcmd = 0x17;
			n = indexof(u);
		} else if (!u->blessed) {
			spellcmd = 0x15;
			n = indexof(u);
		} else if (!spellward && (units[n].blessed || units[n].warded))
			spellcmd = 0x17;
		else if (!spellward && !units[n].cursed)
			spellcmd = 0x16;
		else if (!spellward && !units[n].weakened)
			spellcmd = 0xb;
		else if (!u->warded) {
			spellcmd = 0xf;
			n = indexof(u);
		} else if (!spellward && !units[n].fleeing && !isrobber(u) && canfrighten(curmap, indexof(u))
		&& (n != 0 || randn(100) < (isfemale(u) ? 20 : 10)))
			spellcmd = 0xa;
		else
			goto fail;
	} else if (spellkind == 2) {
		if (u->cursed || u->weakened) {
			spellcmd = 0x17;
			n = indexof(u);
		} else if (!u->blessed) {
			spellcmd = 0x15;
			n = indexof(u);
		} else if (spellward || units[n].cursed || units[n].blessed || units[n].warded)
			goto fail;
		else
			spellcmd = 0x16;
	} else if (spellkind == 3 && !u->blessed && !u->cursed && !u->weakened) {
		spellcmd = 0x15;
		n = indexof(u);
	} else
		goto fail;
	spellres = castat(u, n, spellcmd);
	return spellres;
fail:
	return -1;
}

/* refuses an action in combat mode */
void
denycombat(void)
{
	if (nearfoe(0))
		denymsg(strs[0x135], strs[0x136], empty);
	else
		denymsg(strs[0x135], strs[0x137], texts + 0xb575);
}

void
denyfoes(void)
{
	denymsg(strs[0x13a]);
}

void
denypractice(void)
{
	denymsg(strs[0x139]);
}

/* whether the player bleeds enough to be warned */
int
mustdress(void)
{
	if (units[0].bleeding) {
		if (units[0].health <= units[0].bleeding + units[0].poison)
			return 1;
		if (units[0].bleeding >= 3 && !getbit(game.flags, 0x4ba))
			return 1;
	}
	return 0;
}

int
bleedmsg(void)
{
	setflag(0x4ba, 1);
	return msgf(0xe, strs[0x505]);
}

int
bleedcheck(void)
{
	if (mustdress())
		return bleedmsg();
	return 0;
}

void
forgetheld(void)
{
	nheld = 0;
	heldtime = 0;
}

void
dropheld(void)
{
	if (nheld) {
		for (heldi = 0; heldi < nheld; heldi++)
			delmsgid(heldids[heldi]);
		forgetheld();
	}
}

int
releaseheld(void)
{
	if (nheld <= 0)
		return 0;
	if (playerx + playery * 150 != heldcell)
		goto drop;
	if (heldtime + 6 > gametime)
		goto busy;
	for (reli = 0; reli < nheld; reli++) {
		relmsg = findmsg(heldids[reli]);
		if (relmsg == 0xff) {
		drop:
			dropheld();
			return 0;
		}
		msgkeep[relmsg] = 0;
		msgprio[relmsg] = 0xe;
		msgtime[relmsg] = gametime;
	}
	forgetheld();
	return 0;
busy:
	return 1;
}

void
holdmsg(void)
{
	if (nheld < 7 && nmsgs) {
		keeplast();
		msgprio[nmsgs - 1] = 99;
		heldids[nheld++] = msgid[nmsgs - 1];
		heldtime = gametime;
		heldcell = playerx + playery * 150;
	}
}

/* tells the player of the skill that changed most since the last call */
void
skillmsg(void)
{
	dropheld();
	if (!talkactive)
		holdmsg();
	skilldown = 0;
	skillup = 0;
	skillmin = 0;
	skillmax = 0;
	for (skilli = 0; skilli < 30; skilli++) {
		skilld = game.skills[skilli] - game.msgpoints[skilli];
		if (skilld) {
			if (skilld > skillmax) {
				skillup = skilli;
				skillmax = skilld;
			} else if (skilld < skillmin) {
				skilldown = skilli;
				skillmin = skilld;
			}
		}
	}
	if (abs(skillmax) < 5 && abs(skillmin) < 5) {
		skilli = 0;
		skilld = 0;
	} else if (abs(skillmax) >= abs(skillmin)) {
		skilld = 1;
		skilli = skillup;
	} else {
		skilld = -1;
		skilli = skilldown;
	}
	skillstr = 0;
	switch (skilli) {
	case 0:
		break;
	case 0x14:
		if (skilld > 0)
			skillstr = 0x4f8;
		else
			skillstr = 0x4f9;
		break;
	case 0x16:
		if (skilld > 0)
			skillstr = 0x4fa;
		else
			skillstr = 0x4fb;
		break;
	case 0x17:
		if (skilld > 0)
			skillstr = 0x4fc;
		else
			skillstr = 0x4fd;
		break;
	case 0xb:
		if (skilld > 0)
			skillstr = 0x4fe;
		else
			skillstr = 0x503;
		break;
	case 0xc:
		if (skilld > 0)
			skillstr = 0x4ff;
		else
			skillstr = 0x503;
		break;
	case 0xd:
		if (skilld > 0)
			skillstr = 0x500;
		else
			skillstr = 0x503;
		break;
	case 0xe:
		if (skilld > 0)
			skillstr = 0x501;
		else
			skillstr = 0x503;
		break;
	case 0xf:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
		if (skilld > 0)
			skillstr = 0x502;
		else
			skillstr = 0x503;
		break;
	}
	if (mustdress() && skilld >= 0) {
		bleedmsg();
		holdmsg();
		return;
	}
	if (!(skillstr && skilld < 0) && game.fightinvokes) {
		msgf(0xe, strs[0x504]);
		holdmsg();
	} else if (skillstr) {
		msgf(0xe, strs[skillstr]);
		holdmsg();
	}
	skillstr = bleedcheck();
	if (skillstr)
		holdmsg();
}

short maxmsgs = 7;
short nextmsgid = 1;
int otherver;
int verunused[3];
short li;
short lj;
short ltotal;
short ln;
short dk;
short fi;
short di;
short dcount;
short ai;
short ak;
short alen;
short aminprio;
short amin;
short aid;
char *aline;
short msgfid;
short msgcatid;
short prependw;
short askmsgid;
short failmsgid;
short nosuccid;
short denymsgid;
short notworkid;
signed char rmode;
short ri;
short rd;
short rd2;
short rval;
short ra;
short rb;
unsigned long steallock;
unsigned long *stealp;
unsigned char ukind;
short ustr;
short ci;
unsigned char wdbleed;
unsigned char wdpoison;
short wdloss;
short gi;
short wgmin;
short wgprio;
short wgmsg;
unsigned char wgmat;
short wgbroke;
unsigned long wgwreck;
unsigned long bloodit;
short hprot;
short hgear;
short hprio;
short hdmg0;
short hdmg1;
short hdmg2;
unsigned long hitem;
unsigned long *hslot;
unsigned char hkind;
short hsound;
short hmsg;
short hsaid;
short hstate0;
short hstate1;
short wskill;
short warm;
short wpct;
short aidx;
short askill;
short apct;
short tmres;
short tmskill;
short satk;
short smin;
short smax;
short sdmg;
short skind;
short sslot;
short sroll;
short sparry;
short sshield;
short sdodge;
short sluck;
short sx;
short sy;
short smissed;
short sdx;
short sdy;
short sdist;
short sexposed;
short throwfail;
short throwx;
short throwy;
short throwdist;
short casti;
short castres;
int spellres;
short spellcmd;
short spellkind;
short spellward;
int heldi;
short reli;
short relmsg;
short skilli;
short skillup;
short skilldown;
short skillstr;
int skilld;
int skillmax;
int skillmin;
int skillunused[2];
char msgtext[17][200];
short msgprio[17];
short msgid[17];
short msgdel[17];
short msgkeep[17];
short msglines[18];
short msgy[18];
unsigned long msgtime[17];
short nmsgs;
char msgfmt[200];
short askslot;
short denyslot;
short failslot;
short newmsgs;
short stealmsg;
short attackmsg;
short diemsg;
short freetake;
unsigned long hosttime;
unsigned long heldtime;
int heldcell;
short heldids[8];
int nheld;
int lzwblockbuf[64];
int lzwstackbuf[1025];
int lzwsuffixbuf[3914];
int lzwprefixbuf[2505];

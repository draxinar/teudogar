/*
 * The texts, a module of teudogar.exe (004216a0 to 004243c0).
 */

#include <windows.h>
#include <string.h>

#include "base.h"

typedef struct Unit Unit;
typedef struct Kind Kind;
typedef struct Game Game;
typedef struct Metal Metal;
typedef struct Gram Gram;

/* 0x1b8 bytes each (units, 009bc240) */
struct Unit {
	unsigned char x;
	unsigned char y;
	char f2[7];
	unsigned char mode;
	char fa[0xed - 0xa];
	short strength;
	char fef[0xf3 - 0xef];
	short health;
	char ff5[0x12c - 0xf5];
	unsigned short load;
	char f12e[0x131 - 0x12e];
	unsigned char clan;
	char f132[0x135 - 0x132];
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
	char f141[0x147 - 0x141];
	unsigned char type;
	char f148[0x161 - 0x148];
	short face;
	short number;
	char f165[0x1b8 - 0x165];
};

/* the saved game (009cc470) */
struct Game {
	char name[0x14];
	char f14[0x34 - 0x14];
	char flags[0x28a - 0x34];
	unsigned char fate;
	short killer;
	char f28d[0x3ba - 0x28d];
	char latinname[0x5f8 - 0x3ba];
};

/* 27 bytes each (004c08a8), as map's */
struct Metal {
	unsigned short skill;
	char f2[0x1b - 2];
};

/* the grammar of an item name: a letter per language (004c1028) */
struct Gram {
	char gender[6];	/* m, f, n */
	char number[6];	/* s, p */
};

/* 0x41 bytes each, what kindof() returns for a unit */
struct Kind {
	unsigned short name;
	char f2[0x12 - 2];
	short strength;
	char f14[0x18 - 0x14];
	unsigned short maxhealth;
	char f1a[0x41 - 0x1a];
};

int badunit(Unit *u);
Kind *kindof(Unit *u);
short isanimal(Unit *u);
short isfreed(Unit *u);
unsigned char classof(int i);
unsigned long ageof(unsigned long x);
unsigned long baselook(unsigned long item);
unsigned short gearof(unsigned long item);
short immobility(Unit *u);
char *itemname(unsigned long item);

extern char *strs[];
extern int lang;
extern char empty[1];
extern Unit units[];
extern Game game;
extern Metal metals[30];
extern unsigned char typenames[];
extern int cheat;
extern Gram armorgram[];
extern Gram weapongram[];
extern unsigned char keys[256];
extern short topimmob;
extern unsigned long toppiece;
extern short nextimmob;
extern char pounds;
extern char texts[47942];

extern int curdat;
extern int idfile;
extern int idok;
extern int odok;
extern DWORD sestart;
extern DWORD seend;
extern int sefile;
extern int nentries[9];
extern DWORD entryoffs[9][500];
extern DWORD entrysizes[9][500];
extern HANDLE datfh;
extern DWORD storedn;
extern DWORD decodedn;
extern char datpath[1000];
extern char datname[32];
extern char progdir[1000];
extern char savesdir[1000];
extern char mapsdir[1000];
extern int sifile;
extern int sientry;
extern unsigned short nameidx;
extern int capnext;
extern char *capstart;
extern char *statusp;
extern char *statusq;
extern char *dmglo;
extern char *dmghi;
extern unsigned char isgold;
extern char carrybuf[30];
extern unsigned short carryload;
extern unsigned short carrymax;
extern char timesbuf[24];
extern char daysbuf[24];
extern char hoursbuf[32];
extern DWORD hoursn;
extern char *titlep;
extern char *titleq;
extern short immobv;
extern short immobadv;
extern int dayic;
extern unsigned char gearcls;
extern int geartype;
extern char genderch;
extern char numberch;
extern char *yesnotext;
extern char capbuf[56];
extern char attackbuf[104];
extern char nextbuf[32];
extern char romanbuf[20];
extern short fatepic;
extern char itembuf[32];
extern char yourbuf[40];

/* the text of the current language, English when it has none */
char *
pick(char *de, char *en, char *fr, char *es, char *it, char *pl)
{
	char *s;

	s = en;
	switch (lang) {
	case 0:
		if (de)
			s = de;
		break;
	case 2:
		if (fr)
			s = fr;
		break;
	case 3:
		if (es)
			s = es;
		break;
	case 4:
		if (it)
			s = it;
		break;
	case 5:
		if (pl)
			s = pl;
		break;
	}
	return s;
}

/* one of four texts by language, kept in yesnotext */
char *
yesnokeys(void)
{
	switch (lang) {
	case 0:
		yesnotext = texts + 0xb511;
		break;
	case 1:
	case 5:
	default:
		yesnotext = texts + 0xb525;
		break;
	case 2:
		yesnotext = texts + 0xb539;
		break;
	case 3:
	case 4:
		yesnotext = texts + 0xb54d;
		break;
	}
	return yesnotext;
}

/* text n of the table at strs[459] */
char *
weartext(short n)
{
	if (n >= 0 && n <= 7)
		return strs[459 + n];
	return empty;
}

/* text n of the table at strs[484] */
char *
breaktext(short n)
{
	if (n >= 0 && n <= 7)
		return strs[484 + n];
	return empty;
}

/* the time of day of hour h */
char *
daytime(unsigned char h)
{
	switch (h) {
	case 5:
		return strs[492];
	case 6:
	case 7:
	case 8:
		return strs[493];
	case 9:
	case 10:
	case 11:
		return strs[494];
	case 12:
	case 13:
	case 14:
		return strs[495];
	case 15:
	case 16:
	case 17:
	case 18:
		return strs[496];
	case 19:
	case 20:
	case 21:
		return strs[497];
	case 22:
	case 23:
		return strs[498];
	default:
		return strs[499];
	}
}

/* the time of day of hour h, second form */
char *
atdaytime(unsigned char h)
{
	switch (h) {
	case 5:
		return strs[500];
	case 6:
	case 7:
	case 8:
		return strs[501];
	case 9:
	case 10:
	case 11:
		return strs[502];
	case 12:
	case 13:
	case 14:
		return strs[503];
	case 15:
	case 16:
	case 17:
	case 18:
		return strs[504];
	case 19:
	case 20:
	case 21:
		return strs[505];
	case 22:
	case 23:
		return strs[506];
	default:
		return strs[507];
	}
}

/* text n of the table at strs[509] */
char *
hungertext(unsigned char n)
{
	if (n <= 7)
		return strs[509 + n];
	return strs[517];
}

/* text n of the table at strs[518] */
char *
tiredtext(unsigned char n)
{
	if (n <= 7)
		return strs[518 + n];
	return strs[526];
}

/* strs[1637 + n] for n from 1 to 50 */
char *
personname(short n)
{
	if (n) {
		n += 1637;
		if (n >= 1638 && n <= 1687)
			return strs[n];
	}
	return strs[1688];
}

/* the name of unit u */
char *
unitname(Unit *u)
{
	if (badunit(u))
		return strs[1432];
	if (u == units)
		return game.name;
	if (u == NULL)
		return strs[1432];
	if (u->number == 0 || !(u->number & 0x2000) || isanimal(u)) {
		if (u->type == 0x20) {
			if (isfreed(u))
				return strs[750];
		} else if (u->type == 0x21) {
			if (isfreed(u))
				return strs[751];
		} else if (u->type == 0x25 && u->clan == 0x62)
			return strs[778];
		return strs[kindof(u)->name];
	}
	nameidx = u->number & 0x1fff;
	if ((u->number & 0x1fff) == 0x1fff)
		return strs[730];
	return personname(nameidx);
}

/* the name of unit u, capitalized */
char *
capname(Unit *u)
{
	wsprintf(capbuf, unitname(u));
	capbuf[0] = upper(capbuf[0]);
	return capbuf;
}

/* capitalize the first letter of each sentence of s */
void
sentcase(char *s)
{
	capstart = s;
	capnext = 1;
	for (; *s; s++) {
		if (*s == ' ' || *s == 13)
			;
		else if (*s == ':' || *s == '.' || *s == '!' || *s == '\xa1' || *s == '?' || *s == '\xbf')
			capnext = 1;
		else if (*s == ',')
			capnext = 0;
		else if (capnext) {
			if (s >= capstart + 2 && s[-1] == '.' && s[-2] == '.'
			|| s >= capstart + 3 && s[-1] == ' ' && s[-2] == '.' && s[-3] == '.')
				;
			else
				*s = upper(*s);
			capnext = 0;
		}
	}
}

/* the names of units a and u in one of strs[320] to strs[324] */
char *
attacktext(Unit *a, Unit *u)
{
	if (u == NULL)
		attackbuf[0] = 0;
	else if (a == units) {
		if (u == a)
			wsprintf(attackbuf, strs[320]);
		else
			wsprintf(attackbuf, unitname(u));
	} else if (a == NULL) {
		if (u == units)
			attackbuf[0] = 0;
		else
			wsprintf(attackbuf, unitname(u));
	} else if (u == units)
		wsprintf(attackbuf, strs[323], unitname(a));
	else if (a == u)
		wsprintf(attackbuf, strs[322], unitname(u));
	else
		wsprintf(attackbuf, strs[324], unitname(a), unitname(u));
	return attackbuf;
}

/* the text index 0x21b to 0x21f of health a out of b */
unsigned short
healthid(unsigned short a, unsigned short b)
{
	if (a <= 0)
		return 0x21b;
	if (a < b / 3)
		return 0x21c;
	if (a < b * 2 / 3)
		return 0x21d;
	if (a < b * 4 / 5)
		return 0x21e;
	return 0x21f;
}

/* the text index of the health of unit u */
unsigned short
healthof(Unit *u)
{
	if (badunit(u))
		return healthid(100, 100);
	return healthid(u->health, kindof(u)->maxhealth);
}

/* the health of unit u */
char *
healthtext(Unit *u)
{
	return strs[healthof(u)];
}

/* 1 when u->load exceeds u->strength * 25 */
int
overloaded(Unit *u)
{
	if (u->load > u->strength * 25)
		return 1;
	return 0;
}

/* 1 when u->load exceeds u->strength * 27 */
int
overladen(Unit *u)
{
	if (u->load > u->strength * 2700 / 100)
		return 1;
	return 0;
}

/* the health of unit u and the texts of its flags asleep to warded into buf */
void
statustext(char *buf, Unit *u)
{
	buf[0] = 0;
	if (badunit(u))
		return;
	if (u->health <= 0) {
		wsprintf(buf, strs[539]);
		return;
	}
	statusp = healthtext(u);
	wsprintf(buf, texts + 0xb0f0, statusp);
	for (statusq = buf; *statusq; statusq++)
		;
	if (u->bewitched)
		catstr(buf, strs[531]);
	if (u->fleeing || u->mode == 25)
		catstr(buf, strs[532]);
	if (u->berserk)
		catstr(buf, strs[538]);
	if (u->confused)
		catstr(buf, strs[533]);
	if (u->asleep)
		catstr(buf, strs[534]);
	if (u->cursed)
		catstr(buf, strs[535]);
	if (u->blessed)
		catstr(buf, strs[530]);
	if (u->warded)
		catstr(buf, strs[536]);
	if (u->weakened)
		catstr(buf, strs[537]);
	if (u->bleeding)
		catstr(buf, strs[529]);
	if (u->poison)
		catstr(buf, strs[528]);
	if (u->drunk)
		catstr(buf, strs[527]);
	if (overloaded(u))
		catstr(buf, strs[555]);
	if (statusp != strs[540] && statusp != strs[541] && *statusq) {
		statusp = buf;
		while ((*statusp++ = *statusq++) != 0)
			;
	}
	for (statusp = buf; *statusp; statusp++)
		;
	if (statusp[-1] == ' ' && statusp[-2] == ',')
		statusp[-2] = 0;
}

/* text of n against thresholds 55 to 145 */
char *
ratetext(short n)
{
	if (n < 55)
		return strs[779];
	if (n < 65)
		return strs[780];
	if (n < 75)
		return strs[781];
	if (n < 85)
		return strs[782];
	if (n < 95)
		return strs[783];
	if (n < 105)
		return strs[784];
	if (n < 115)
		return strs[785];
	if (n < 125)
		return strs[786];
	if (n < 135)
		return strs[787];
	if (n < 145)
		return strs[788];
	return strs[789];
}

/* text of n against thresholds 2 to 125 */
char *
vitaltext(short n)
{
	if (n < 2)
		return strs[790];
	if (n < 10)
		return strs[791];
	if (n < 20)
		return strs[792];
	if (n < 30)
		return strs[793];
	if (n < 45)
		return strs[794];
	if (n < 55)
		return strs[795];
	if (n < 66)
		return strs[796];
	if (n < 80)
		return strs[797];
	if (n < 105)
		return strs[798];
	if (n < 125)
		return strs[799];
	return strs[800];
}

/* text of n against thresholds 2 to 34 */
char *
armortext(short n)
{
	if (n < 2)
		return strs[801];
	if (n < 5)
		return strs[802];
	if (n < 9)
		return strs[803];
	if (n < 14)
		return strs[804];
	if (n < 19)
		return strs[805];
	if (n < 24)
		return strs[806];
	if (n < 26)
		return strs[807];
	if (n < 34)
		return strs[808];
	return strs[809];
}

/* text of n against thresholds 2 to 62 */
char *
dmgtext(short n)
{
	if (n < 2)
		return strs[801];
	if (n < 5)
		return strs[802];
	if (n < 10)
		return strs[803];
	if (n < 20)
		return strs[804];
	if (n < 30)
		return strs[805];
	if (n < 40)
		return strs[806];
	if (n < 49)
		return strs[807];
	if (n < 62)
		return strs[808];
	return strs[809];
}

/* the range of texts of a and b into buf */
void
dmgrange(char *buf, short a, short b)
{
	dmglo = dmgtext(a);
	dmghi = dmgtext(b);
	if (dmghi == dmglo)
		wsprintf(buf, dmglo);
	else
		wsprintf(buf, strs[1297], dmglo, dmghi);
}

/* the text index of n from 0 to 15 */
short
blessid(short n)
{
	if (n >= 0 && n <= 15)
		return n + 468;
	return 475;
}

/* the amount of an item into buf */
void
cointext(char *buf, unsigned long item)
{
	isgold = classof(item & 0xfff) == 16 ? 1 : 0;
	item >>= 16;
	if (item > 1)
		wsprintf(buf, strs[isgold ? 299 : 297], item);
	else
		wsprintf(buf, strs[isgold ? 300 : 298]);
}

/* u->load and u->strength * 25 in hundreds, rounded, as text */
char *
carrytext(Unit *u, short n)
{
	int r;

	carrybuf[0] = 0;
	if (badunit(u))
		return carrybuf;
	if (pounds == 0) {
		r = u->load % 100 >= 50 ? 1 : 0;
		carryload = u->load / 100 + r;
		r = kindof(u)->strength * 25 % 100 >= 50 ? 1 : 0;
		carrymax = u->strength * 25 / 100 + r;
	} else {
		r = u->load % 45 >= 50 ? 1 : 0;
		carryload = u->load / 45 + r;
		r = kindof(u)->strength * 25 % 45 >= 50 ? 1 : 0;
		carrymax = u->strength * 25 / 45 + r;
	}
	if (n) {
		wsprintf(carrybuf, texts + 0xb09e, carryload, strs[1290], carrymax,
			strs[pounds ? 0x330 : 0x32f]);
		if (carryload == 0)
			carrybuf[0] = '/';
	} else {
		wsprintf(carrybuf, texts + 0xb0de, carryload, carrymax, strs[pounds ? 0x330 : 0x32f]);
		if (carryload == 0)
			carrybuf[0] = '-';
	}
	return carrybuf;
}

/* a count of n */
char *
timestext(short n)
{
	switch (n) {
	case 0:
		wsprintf(timesbuf, strs[1291]);
		break;
	case 1:
		wsprintf(timesbuf, strs[1292]);
		break;
	default:
		wsprintf(timesbuf, strs[1293], n);
		break;
	}
	return timesbuf;
}

/* n hours as days and half days */
char *
daystext(short n)
{
	int h, d;

	if (n > 24) {
		h = n % 24;
		d = n / 24;
		if (h >= 12)
			wsprintf(daysbuf, texts + 0xb24f, d, 0xbd, strs[1383]);
		else if (d > 1)
			wsprintf(daysbuf, texts + 0xb1eb, d, strs[1383]);
		else
			wsprintf(daysbuf, texts + 0xb1eb, d, strs[1382]);
	} else
		wsprintf(daysbuf, texts + 0xb257, 0xbd, strs[1382]);
	return daysbuf;
}

/* n minutes as hours and quarters */
char *
hourstext(DWORD n)
{
	hoursn = n / 60;
	n %= 60;
	if (hoursn == 0) {
		if (n < 15)
			wsprintf(hoursbuf, strs[1473]);
		else if (n < 30)
			wsprintf(hoursbuf, strs[1474]);
		else if (n < 45)
			wsprintf(hoursbuf, strs[1475]);
		else
			wsprintf(hoursbuf, strs[1476]);
	} else if (hoursn == 1 && n < 15) {
		wsprintf(hoursbuf, strs[1477]);
		return hoursbuf;
	} else {
		if (n < 15)
			wsprintf(hoursbuf, strs[1478], hoursn);
		else if (n < 30)
			wsprintf(hoursbuf, strs[1479], hoursn);
		else if (n < 45)
			wsprintf(hoursbuf, strs[1480], hoursn);
		else
			wsprintf(hoursbuf, strs[1481], hoursn);
		return hoursbuf;
	}
	return hoursbuf;
}

/* the text index of the age of an item */
short
freshid(unsigned long item)
{
	unsigned long d;

	d = ageof(item);
	if (d == 0)
		return 0x491;
	if (d <= 1)
		return 0x492;
	if (d <= 2)
		return 0x493;
	if (d <= 3)
		return 0x494;
	if (d <= 4)
		return 0x495;
	if (d <= 5)
		return 0x496;
	return 0x497;
}

/* capitalize the words of English title s */
void
titleen(char *s)
{
	for (titlep = s; *titlep; titlep++) {
		if (titlep == s || titlep[-1] == ' ' || titlep[-1] == '-') {
			if (titlep[0] == 'o' && titlep[1] == 'f' && titlep[2] == ' ')
				continue;
			if (titlep[0] == 'w' && titlep[1] == 'i' && titlep[2] == 't'
			&& titlep[3] == 'h' && titlep[4] == ' ')
				continue;
			*titlep = upper(*titlep);
		}
	}
}

/* capitalize the words of Italian title s */
void
titleit(char *s)
{
	for (titleq = s; *titleq; titleq++) {
		if (titleq == s || titleq[-1] == ' ' || titleq[-1] == '-' || titleq[-1] == '\'') {
			if (titleq[0] == 'e' && titleq[1] == ' ')
				continue;
			if (titleq[0] == 'd' && (titleq[2] == ' ' || titleq[1] == '\''))
				continue;
			if (titleq[0] == 'l' && (titleq[2] == ' ' || titleq[1] == '\''))
				continue;
			if (titleq[0] == 'd' && titleq[3] == ' ')
				continue;
			if (titleq[0] == 'd' && titleq[1] == 'e' && titleq[2] == 'l' && titleq[3] == 'l')
				continue;
			if (titleq[0] == 'i' && titleq[1] == 'l' && titleq[2] == ' ')
				continue;
			if (titleq[0] == 'i' && titleq[1] == 'n' && titleq[2] == ' ')
				continue;
			if (titleq[0] == 'c' && titleq[1] == 'o' && titleq[2] == 'n' && titleq[3] == ' ')
				continue;
			if (titleq[0] == 'p' && titleq[1] == 'e' && titleq[2] == 'r' && titleq[3] == ' ')
				continue;
			if (titleq[0] == 'n' && titleq[1] == 'e' && titleq[2] == 'l')
				continue;
			if (titleq[0] == 'n' && titleq[1] == 'e' && titleq[2] == 'i' && titleq[3] == ' ')
				continue;
			*titleq = upper(*titleq);
		}
	}
}

/* capitalize the words of title s in the current language */
void
titlecase(char *s)
{
	if (lang == 0)
		return;
	if (lang == 4)
		titleit(s);
	else
		titleen(s);
}

char *
backbutton(void)
{
	return pick(texts + 0x6bd8, texts + 0x6be7, texts + 0x6bf4, texts + 0x6bf5, texts + 0x6bf6, texts + 0x6c07);
}

char *
prevbutton(void)
{
	return pick(texts + 0x6c08, texts + 0x6c17, texts + 0x6c28, texts + 0x6c29, texts + 0x6c2a, texts + 0x6c3b);
}

char *
cancelbutton(void)
{
	return pick(texts + 0x6d61, texts + 0x6d71, texts + 0x6d7e, texts + 0x6d7f, texts + 0x6d80, texts + 0x6d8f);
}

char *
nextbutton(void)
{
	return pick(texts + 0x6c3c, texts + 0x6c4b, texts + 0x6c58, texts + 0x6c59, texts + 0x6c5a, texts + 0x6c69);
}

char *
nextoff(void)
{
	return pick(texts + 0x6d06, texts + 0x6d17, texts + 0x6d26, texts + 0x6d27, texts + 0x6d28, texts + 0x6d39);
}

/* the Next > button to id n, or nextoff's disabled one for 0 */
char *
nextid(short n)
{
	if (n == 0)
		wsprintf(nextbuf, nextoff());
	else
		wsprintf(nextbuf, pick(texts + 0x6ccc, texts + 0x6cdf, texts + 0x6cf0,
			texts + 0x6cf1, texts + 0x6cf2, texts + 0x6d05), n);
	return nextbuf;
}

char *
donebutton(void)
{
	return pick(texts + 0x6d3a, texts + 0x6d47, texts + 0x6d52, texts + 0x6d53, texts + 0x6d54, texts + 0x6d60);
}

char *
titlehead(void)
{
	return pick(texts, texts + 9, texts + 0x12, texts + 0x13, texts + 0x14, texts + 0x1d);
}

char *
titletail(void)
{
	return pick(texts + 0x1e, texts + 0x37, texts + 0x53, texts + 0x54, texts + 0x55, texts + 0x6c);
}

char *
gametitle(void)
{
	return pick(texts + 0x6d, texts + 0x8e, texts + 0xb2, texts + 0xb3, texts + 0xb4, texts + 0xd3);
}

/* how immobile its armor makes unit u, as text into buf */
void
immobtext(char *buf, Unit *u)
{
	buf[0] = 0;
	if (badunit(u))
		return;
	immobv = immobility(u);
	if (immobv < 5) {
		buf[0] = 0;
		return;
	}
	if (immobv < 10)
		immobadv = 0x51a;
	else if (immobv < 16)
		immobadv = 0x51b;
	else if (immobv < 23) {
		wsprintf(buf, strs[1310]);
		return;
	} else if (immobv < 30)
		immobadv = 0x51c;
	else
		immobadv = 0x51d;
	wsprintf(buf, texts + 0x9dea, strs[immobadv], strs[1310]);
}

/* the cause of the immobility into buf: toppiece when it hinders half again as much as the next piece, else the armor */
void
immobcause(char *buf)
{
	if (topimmob < 3) {
		buf[0] = 0;
		return;
	}
	if (topimmob >= nextimmob * 15 / 10)
		wsprintf(buf, strs[1312], itemname(toppiece));
	else
		wsprintf(buf, strs[1311]);
}

/* n as a Roman numeral into p */
void
toroman(char *p, DWORD n)
{
	if (n == 0) {
		p[0] = '-';
		p[1] = 0;
		return;
	}
	while (n >= 1000) {
		n -= 1000;
		*p++ = 'M';
	}
	if (n >= 900) {
		n -= 900;
		*p++ = 'C';
		*p++ = 'M';
	}
	while (n >= 500) {
		n -= 500;
		*p++ = 'D';
	}
	if (n >= 400) {
		n -= 400;
		*p++ = 'C';
		*p++ = 'D';
	}
	while (n >= 100) {
		n -= 100;
		*p++ = 'C';
	}
	if (n >= 90) {
		n -= 90;
		*p++ = 'X';
		*p++ = 'C';
	}
	while (n >= 50) {
		n -= 50;
		*p++ = 'L';
	}
	if (n >= 40) {
		n -= 40;
		*p++ = 'X';
		*p++ = 'L';
	}
	while (n >= 10) {
		n -= 10;
		*p++ = 'X';
	}
	if (n >= 9) {
		n -= 9;
		*p++ = 'I';
		*p++ = 'X';
	}
	while (n >= 5) {
		n -= 5;
		*p++ = 'V';
	}
	if (n >= 4) {
		n -= 4;
		*p++ = 'I';
		*p++ = 'V';
	}
	while (n >= 1) {
		n--;
		*p++ = 'I';
	}
	*p++ = 0;
}

/* n as a Roman numeral */
char *
roman(DWORD n)
{
	toroman(romanbuf, n);
	return romanbuf;
}

/* the cursor tile, 0x8f to 0x95, of time of day text s */
int
dayicon(char *s)
{
	if (s == strs[492])
		dayic = 0x94;
	else if (s == strs[493])
		dayic = 0x8f;
	else if (s == strs[494])
		dayic = 0x90;
	else if (s == strs[495])
		dayic = 0x91;
	else if (s == strs[496])
		dayic = 0x92;
	else if (s == strs[497])
		dayic = 0x93;
	else if (s == strs[498])
		dayic = 0x94;
	else
		dayic = 0x95;
	return dayic;
}

/* the EQU picture of an item by its type and metal, or 0 */
int
gearicon(unsigned long item)
{
	geartype = item & 0xfff;
	if (geartype == 0)
		return 0;
	geartype = baselook(geartype);
	if (geartype >= 0x1a0 && geartype <= 0x1a3) {
		geartype -= 0x1a0;
		return geartype + 25;
	}
	if (geartype >= 0x1ad && geartype <= 0x1b5) {
		geartype -= 0x1ad;
		return geartype + 15;
	}
	if (geartype >= 0x1a7 && geartype <= 0x1a9) {
		geartype -= 0x1a7;
		return geartype + 31;
	}
	if (geartype >= 0x1ba && geartype <= 0x1bf) {
		geartype -= 0x1ba;
		if (geartype == 0)
			;
		else if (geartype <= 2)
			geartype = 2;
		else
			geartype = 1;
		return geartype + 10;
	}
	gearcls = classof(geartype);
	if (gearcls != 60)
		return 0;
	switch (metals[gearof(item)].skill) {
	case 18:
		return 2;
	case 16:
		return 3;
	case 17:
		return 4;
	case 15:
		if (geartype == 0x1ca)
			return 6;
		return 5;
	}
	return 0;
}

/* set game.fate from 1 or 4 to 2 when game.killer is 0 */
void
fixfate(void)
{
	switch (game.fate) {
	case 1:
		if (!game.killer)
			game.fate = 2;
		break;
	case 4:
		if (!game.killer)
			game.fate = 2;
		break;
	}
}

/* text of game.fate into buf, its number into fatepic */
void
fatetext(char *buf)
{
	fixfate();
	switch (game.fate) {
	case 0:
		wsprintf(buf, strs[1499]);
		fatepic = 0x208c;
		break;
	case 1:
		wsprintf(buf, strs[1487], game.name);
		fatepic = 0x208a;
		break;
	case 7:
		wsprintf(buf, strs[1505]);
		fatepic = 0x208c;
		break;
	case 2:
		wsprintf(buf, strs[1502]);
		fatepic = 0x208a;
		break;
	case 5:
		wsprintf(buf, strs[1490]);
		fatepic = 0x208c;
		break;
	case 4:
		wsprintf(buf, strs[1493]);
		fatepic = 0x208a;
		break;
	case 6:
		wsprintf(buf, strs[1496]);
		fatepic = 0x208a;
		break;
	case 15:
		wsprintf(buf, strs[1511]);
		fatepic = 0x2090;
		break;
	case 17:
		wsprintf(buf, strs[1518]);
		fatepic = 0x2090;
		break;
	case 18:
		wsprintf(buf, strs[1521]);
		fatepic = 0x208c;
		break;
	case 19:
		wsprintf(buf, strs[1524]);
		fatepic = 0x208a;
		break;
	case 13:
		wsprintf(buf, strs[1526]);
		fatepic = 0x208c;
		break;
	case 12:
		wsprintf(buf, strs[1527]);
		fatepic = 0x208c;
		break;
	case 31:
		wsprintf(buf, strs[1532]);
		fatepic = 0x2093;
		break;
	case 30:
		wsprintf(buf, strs[1529]);
		fatepic = 0x2093;
		break;
	case 20:
		wsprintf(buf, strs[1535], strs[1545]);
		fatepic = 0x208e;
		break;
	case 11:
		wsprintf(buf, strs[1514]);
		fatepic = 0x208a;
		break;
	case 25:
		wsprintf(buf, strs[1538]);
		fatepic = 0x2092;
		break;
	case 26:
		wsprintf(buf, strs[1539]);
		fatepic = 0x2092;
		break;
	case 10:
		wsprintf(buf, strs[1508], strs[1545]);
		fatepic = 0x208e;
		break;
	case 21:
		wsprintf(buf, strs[1515]);
		fatepic = 0x208c;
		break;
	default:
		wsprintf(buf, strs[1542]);
		fatepic = 0x208c;
		break;
	}
}

/* text of game.fate with the game's name into buf */
void
epitaph(char *buf)
{
	fixfate();
	switch (game.fate) {
	case 0:
		wsprintf(buf, strs[1500], game.name);
		break;
	case 1:
		wsprintf(buf, strs[1488], game.name, unitname(&units[game.killer]));
		break;
	case 7:
		wsprintf(buf, strs[1506], game.name);
		break;
	case 2:
		wsprintf(buf, strs[1503], game.name);
		break;
	case 5:
		wsprintf(buf, strs[1491], game.name);
		break;
	case 4:
		wsprintf(buf, strs[1494], game.name, unitname(&units[game.killer]));
		break;
	case 6:
		wsprintf(buf, strs[1497], game.name, unitname(&units[game.killer]));
		break;
	case 15:
		wsprintf(buf, strs[1512], game.latinname);
		break;
	case 17:
		wsprintf(buf, strs[1519], game.latinname);
		break;
	case 18:
		wsprintf(buf, strs[1522], game.latinname);
		break;
	case 19:
		wsprintf(buf, strs[1525], game.name);
		break;
	case 12:
	case 13:
		wsprintf(buf, strs[1528], game.name);
		break;
	case 31:
		wsprintf(buf, strs[1533], game.name);
		break;
	case 30:
		wsprintf(buf, strs[1530], game.name);
		break;
	case 20:
		wsprintf(buf, strs[1536], game.name);
		break;
	case 25:
	case 26:
		wsprintf(buf, strs[1540], game.name);
		break;
	case 10:
	case 11:
		wsprintf(buf, strs[1509], game.name);
		break;
	case 21:
		wsprintf(buf, strs[1516], game.latinname);
		break;
	default:
		wsprintf(buf, strs[1543], game.name);
		break;
	}
}

/* the text index of game.fate */
short
fatevision(void)
{
	fixfate();
	switch (game.fate) {
	case 0:
		return 0x5dd;
	case 1:
		return 0x5d1;
	case 7:
		return 0x5e3;
	case 2:
		return 0x5e0;
	case 5:
		return 0x5d4;
	case 4:
		return 0x5d7;
	case 6:
		return 0x5da;
	case 15:
		return 0x5e9;
	case 17:
		return 0x5f0;
	case 18:
	case 19:
		return 0x5f3;
	case 31:
		return 0x5fe;
	case 30:
		return 0x5fb;
	case 20:
		return 0x601;
	case 25:
	case 26:
		return 0x605;
	case 10:
	case 11:
		return 0x5e6;
	case 21:
		return 0x5ed;
	default:
		return 0x608;
	}
}

/* the name of the type of an item */
char *
typetext(unsigned long item)
{
	unsigned char t;

	t = typenames[item & 0xfff];
	if (t != 0)
		return strs[t];
	return empty;
}

/* set the game flag of a special item type */
void
knowpotion(unsigned long item)
{
	switch (classof(item & 0xfff)) {
	case 0x38:
	case 0x39:
		break;
	default:
		return;
	}
	switch (item & 0xfff) {
	case 0x144:
		setbit(game.flags, 0x578, 1);
		break;
	case 0x14e:
		setbit(game.flags, 0x579, 1);
		break;
	case 0x151:
		setbit(game.flags, 0x57a, 1);
		break;
	case 0x175:
		setbit(game.flags, 0x57b, 1);
		break;
	case 0x176:
		setbit(game.flags, 0x57c, 1);
		break;
	case 0x177:
		setbit(game.flags, 0x57d, 1);
		break;
	case 0x17d:
		setbit(game.flags, 0x57e, 1);
		break;
	case 0x182:
		setbit(game.flags, 0x57f, 1);
		break;
	case 0x181:
		setbit(game.flags, 0x580, 1);
		break;
	case 0x185:
		setbit(game.flags, 0x581, 1);
		break;
	case 0x18c:
		setbit(game.flags, 0x582, 1);
		break;
	case 0x18d:
		setbit(game.flags, 0x583, 1);
		break;
	case 0x192:
		setbit(game.flags, 0x584, 1);
		break;
	}
}

/* the game flag of a special item type */
int
potionknown(unsigned long item)
{
	switch (item & 0xfff) {
	case 0x144:
		if (getbit(game.flags, 0x578))
			return 1;
		return 0;
	case 0x14e:
		if (getbit(game.flags, 0x579))
			return 1;
		return 0;
	case 0x151:
		if (getbit(game.flags, 0x57a))
			return 1;
		return 0;
	case 0x175:
		if (getbit(game.flags, 0x57b))
			return 1;
		return 0;
	case 0x176:
		if (getbit(game.flags, 0x57c))
			return 1;
		return 0;
	case 0x177:
		if (getbit(game.flags, 0x57d))
			return 1;
		return 0;
	case 0x17d:
		if (getbit(game.flags, 0x57e))
			return 1;
		return 0;
	case 0x182:
		if (getbit(game.flags, 0x57f))
			return 1;
		return 0;
	case 0x181:
		if (getbit(game.flags, 0x580))
			return 1;
		return 0;
	case 0x185:
		if (getbit(game.flags, 0x581))
			return 1;
		return 0;
	case 0x18c:
		if (getbit(game.flags, 0x582))
			return 1;
		return 0;
	case 0x18d:
		if (getbit(game.flags, 0x583))
			return 1;
		return 0;
	case 0x192:
		if (getbit(game.flags, 0x584))
			return 1;
		return 0;
	}
	return 0;
}

/* the name of an item */
char *
itemname(unsigned long item)
{
	if (item == 0) {
		itembuf[0] = 0;
		return itembuf;
	}
	wsprintf(itembuf, typetext(item));
	if (itembuf[0] == 0)
		wsprintf(itembuf, strs[259]);
	switch (typenames[item & 0xfff]) {
	case 0x80:
	case 0x99:
		goto done;
	}
	switch (classof(item & 0xfff)) {
	case 0x83:
	case 0x88:
		catstr(itembuf, strs[1461]);
		break;
	case 0x85:
	case 0x8a:
		catstr(itembuf, strs[1463]);
		break;
	case 0x86:
	case 0x8b:
		catstr(itembuf, strs[1464]);
		break;
	case 0x14:
		item >>= 16;
		wsprintf(itembuf, item ? unitname(&units[item]) : strs[1432]);
		break;
	case 0x38:
	case 0x39:
		switch (item & 0xfff) {
		case 0x144:
			if (getbit(game.flags, 0x578) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1416]);
			break;
		case 0x14e:
			if (getbit(game.flags, 0x579) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1417]);
			break;
		case 0x151:
			if (getbit(game.flags, 0x57a) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1418]);
			break;
		case 0x175:
			if (getbit(game.flags, 0x57b) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1419]);
			break;
		case 0x176:
			if (getbit(game.flags, 0x57c) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1420]);
			break;
		case 0x177:
			if (getbit(game.flags, 0x57d) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1421]);
			break;
		case 0x17d:
			if (getbit(game.flags, 0x57e) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1422]);
			break;
		case 0x182:
			if (getbit(game.flags, 0x57f) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1423]);
			break;
		case 0x181:
			if (getbit(game.flags, 0x580) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1424]);
			break;
		case 0x185:
			if (getbit(game.flags, 0x581) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1425]);
			break;
		case 0x18c:
			if (getbit(game.flags, 0x582) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1426]);
			break;
		case 0x18d:
			if (getbit(game.flags, 0x583) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1427]);
			break;
		case 0x192:
			if (getbit(game.flags, 0x584) || cheat && (keys['W'] & 0x80))
				wsprintf(itembuf, strs[1428]);
			break;
		}
		break;
	case 0x82:
	case 0x87:
		if ((item & 0xfff) != 0x3b9)
			catstr(itembuf, strs[1460]);
		break;
	case 0x84:
	case 0x89:
		catstr(itembuf, strs[1462]);
		break;
	}
done:
	return itembuf;
}

/* the gender of the name of an item: 0 m, 1 f, 2 n */
short
itemgender(unsigned long item)
{
	genderch = 0;
	item = baselook(item) & 0xfff;
	if (item == 0x528)
		genderch = weapongram[26].gender[lang];
	else if (item >= 0x1a0 && item <= 0x1c6)
		genderch = armorgram[item - 0x1a0].gender[lang];
	else if (item >= 0x1c7 && item <= 0x1e0)
		genderch = weapongram[item - 0x1c7].gender[lang];
	else
		return 2;
	if (genderch == 'm')
		return 0;
	if (genderch == 'f')
		return 1;
	if (genderch == 'n')
		return 2;
	return 2;
}

/* the number of the name of an item: 0 singular, 1 plural */
short
itemplural(unsigned long item)
{
	numberch = 0;
	item = baselook(item) & 0xfff;
	if (item == 0x528)
		numberch = weapongram[26].number[lang];
	else if (item >= 0x1a0 && item <= 0x1c6)
		numberch = armorgram[item - 0x1a0].number[lang];
	else if (item >= 0x1c7 && item <= 0x1e0)
		numberch = weapongram[item - 0x1c7].number[lang];
	else
		return 0;
	if (numberch == 's')
		return 0;
	if (numberch == 'p')
		return 1;
	return 0;
}

/* the name of an item after 'your', in the form that a and b pick */
char *
youritem(unsigned long item, short a, short b)
{
	wsprintf(yourbuf, texts + 0x9dea, strs[(a ? b ? 0x344 : 0x338 : b ? 0x33e : 0x332)
		+ itemplural(item) * 3 + itemgender(item)], itemname(item));
	return yourbuf;
}

/* ' will be ready s' in the number and gender of an item, into buf */
void
readytext(char *buf, unsigned long item, char *s)
{
	wsprintf(buf, strs[898 + itemplural(item) * 3 + itemgender(item)], s);
}

/* that an item is actually brand new, named by youritem(item, 0, n), into buf */
void
brandnew(char *buf, unsigned long item, short n)
{
	wsprintf(buf, strs[1123 + itemplural(item) * 3 + itemgender(item)], youritem(item, 0, n));
}

char *months[14] = {
	empty, texts + 40513, texts + 40517, texts + 40521, texts + 40525,
	texts + 40529, texts + 40533, texts + 40537, texts + 40541,
	texts + 40545, texts + 40549, texts + 40553, texts + 40557
};
Gram armorgram[39] = {
	102, 110, 109, 109, 109, 109, 115, 112, 115, 115, 112, 112, 102, 110,
	109, 109, 109, 109, 115, 112, 115, 115, 112, 112, 102, 110, 109, 109,
	109, 109, 112, 112, 115, 115, 112, 112, 102, 110, 109, 109, 109, 109,
	115, 112, 115, 115, 112, 112, 109, 110, 109, 109, 109, 109, 115, 115,
	115, 115, 115, 115, 109, 110, 109, 109, 109, 109, 115, 115, 115, 115,
	115, 115, 109, 110, 109, 109, 109, 109, 115, 115, 115, 115, 115, 115,
	109, 110, 109, 109, 109, 109, 115, 115, 115, 115, 115, 115, 109, 110,
	109, 109, 109, 109, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109,
	109, 109, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 109, 109,
	115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 109, 109, 115, 115,
	115, 115, 115, 115, 109, 110, 109, 109, 109, 109, 115, 115, 115, 115,
	115, 115, 110, 110, 109, 109, 109, 109, 115, 115, 115, 115, 115, 115,
	109, 110, 109, 109, 109, 109, 115, 115, 115, 115, 115, 115, 109, 110,
	109, 109, 109, 109, 115, 115, 115, 115, 115, 115, 110, 110, 109, 109,
	102, 102, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 109, 109,
	115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 109, 109, 115, 115,
	115, 115, 115, 115, 102, 110, 109, 109, 109, 109, 115, 115, 115, 115,
	115, 115, 109, 110, 109, 109, 109, 109, 115, 115, 115, 115, 115, 115,
	110, 110, 109, 109, 109, 109, 115, 115, 115, 115, 115, 115, 109, 110,
	109, 109, 109, 109, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109,
	109, 109, 112, 112, 115, 115, 112, 112, 109, 110, 109, 109, 109, 109,
	112, 112, 115, 115, 115, 115, 109, 110, 109, 109, 109, 109, 115, 115,
	115, 115, 115, 115, 109, 110, 109, 109, 109, 109, 115, 115, 115, 115,
	115, 115, 109, 110, 109, 109, 109, 109, 115, 115, 115, 115, 115, 115,
	109, 110, 109, 109, 109, 109, 115, 115, 115, 115, 115, 115, 109, 110,
	109, 109, 109, 109, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109,
	109, 109, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 109, 109,
	115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 109, 109, 115, 115,
	115, 115, 115, 115, 109, 110, 109, 109, 109, 109, 115, 115, 115, 115,
	115, 115, 109, 110, 109, 109, 109, 109, 115, 115, 115, 115, 115, 115,
	109, 110, 109, 109, 109, 109, 115, 115, 115, 115, 115, 115, 109, 110,
	109, 109, 109, 109, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109,
	109, 109, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 109, 109,
	115, 115, 115, 115, 115, 115
};
Gram weapongram[27] = {
	110, 110, 109, 109, 102, 102, 115, 115, 115, 115, 115, 115, 109, 110,
	109, 109, 102, 102, 115, 115, 115, 115, 115, 115, 102, 110, 109, 109,
	102, 102, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 102, 102,
	115, 115, 115, 115, 109, 109, 110, 110, 109, 109, 102, 102, 115, 115,
	115, 115, 115, 115, 102, 110, 109, 109, 102, 102, 115, 115, 115, 115,
	115, 115, 110, 110, 109, 109, 102, 102, 115, 115, 115, 115, 115, 115,
	102, 110, 109, 109, 102, 102, 115, 115, 115, 115, 115, 115, 110, 110,
	109, 109, 102, 102, 115, 115, 115, 115, 115, 115, 110, 110, 109, 109,
	102, 102, 115, 115, 115, 115, 115, 115, 110, 110, 109, 109, 102, 102,
	115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 102, 102, 115, 115,
	115, 115, 115, 115, 102, 110, 109, 109, 102, 102, 115, 115, 115, 115,
	115, 115, 102, 110, 109, 109, 102, 102, 115, 115, 115, 115, 115, 115,
	109, 110, 109, 109, 102, 102, 115, 115, 115, 115, 115, 115, 110, 110,
	109, 109, 102, 102, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109,
	102, 102, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 102, 102,
	115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 102, 102, 115, 115,
	115, 115, 115, 115, 109, 110, 109, 109, 102, 102, 115, 115, 115, 115,
	115, 115, 109, 110, 109, 109, 102, 102, 115, 115, 115, 115, 115, 115,
	109, 110, 109, 109, 102, 102, 115, 115, 115, 115, 115, 115, 109, 110,
	109, 109, 102, 102, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109,
	102, 102, 115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 102, 102,
	115, 115, 115, 115, 115, 115, 109, 110, 109, 109, 102, 102, 115, 115,
	115, 115, 115, 115, 102, 110, 109, 109, 102, 102, 115, 115, 115, 115,
	115, 115
};
unsigned short nameidx;
int capnext;
char *capstart;
char *statusp;
char *statusq;
char *dmglo;
char *dmghi;
unsigned char isgold;
char carrybuf[30];
unsigned short carryload;
unsigned short carrymax;
char timesbuf[24];
char daysbuf[24];
char hoursbuf[32];
DWORD hoursn;
char *titlep;
char *titleq;
short immobv;
short immobadv;
int dayic;
unsigned char gearcls;
int geartype;
char genderch;
char numberch;
char namebuf[2500];
char textbuf[2500];
char tmpbuf[2500];
char *yesnotext;
char capbuf[56];
char attackbuf[104];
char nextbuf[32];
char romanbuf[20];
short fatepic;
char itembuf[32];
char yourbuf[40];

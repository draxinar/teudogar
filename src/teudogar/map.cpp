#include <windows.h>
#include <string.h>

#include "base.h"

typedef struct Metal Metal;
typedef struct Gear Gear;
typedef struct Realm Realm;
typedef struct Spot Spot;
typedef struct Troop Troop;
typedef struct Part Part;
typedef struct Region Region;
typedef struct Points Points;
typedef struct Prov Prov;

/* 0x158 bytes each (005a1b78): a container and the items it holds */
struct Realm {
	unsigned long item;
	char f4[0x158 - 4];
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

/* 0x20 bytes each (0055a078) */
struct Troop {
	short parts[5];
	char colors[4];
	unsigned char type;
	char ff[0x20 - 0xf];
};

/* 0x22 bytes each (004c6f48): the parts of a unit kind (unittype) */
struct Part {
	short noun;
	unsigned char layer[3];
	char f5[0x22 - 5];
};

/* 0xd0 bytes each (005acf80) */
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

/* 0x190 bytes each (005b9eb0) */
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

/* 0x164 bytes each (005d8510): the provinces */
struct Prov {
	char f0[0x7c];
	short id;
	short mealday;
	short meals;
	char f82[0x164 - 0x82];
};

/* 27 bytes each, indexed by bits 16 to 20 of an item (004c08a8) */
struct Metal {
	char f0[8];
	short dmgmin;
	short dmgmax;
	char wearmin[2];
	short wornpct;
	char f10[0x19 - 0x10];
	short pricepct;
};

/* 26 bytes each, indexed like Metal (004c0bd8) */
struct Gear {
	short prot1;
	short prot2;
	short prot3;
	char wearmin[2];
	short wornpct;
	char fa[0x18 - 0xa];
	short pricepct;
};

extern Metal metals[30];
extern Gear armors[];
extern char workbuf[];
extern char scrbuf[309700];
extern HANDLE datfh;
extern DWORD storedn;
extern unsigned char faceflags[288];
extern unsigned char sqrttab[46225];
extern char pics[104][2304];
extern unsigned short nspots;
extern short part1;
extern short part8;

int seekentry(int id);
DWORD unpack(void *dst, DWORD dstlen, void *src, DWORD srclen);
void closeborder(void);
int cdprog(void);
int cdsaves(void);
extern char texts[47942];
extern char trvfailmsg;
extern int maploaded;
extern char packbuf[];
DWORD readentry(void *buf, DWORD size, int id);
int unlzw(unsigned char *dst, unsigned char *src, int n);
int visited(int map);
void clearavenge(void);
extern unsigned short enteritems;
extern int entersum;
extern unsigned long gametime;
extern char mapstate[100];
DWORD pack(void *dst, DWORD dstlen, void *src, DWORD srclen);
extern short avengers;
extern Part partdefs[];
extern char recipes[1488];

extern short gameday;
extern short starttime;
extern unsigned short cellflags[4096];
extern unsigned char terrains[4096];
extern unsigned short itemflags[4096];
extern unsigned short weights[4096];
extern unsigned char classes[4096];
extern unsigned char typenames[4096];
extern unsigned short volumes[4096];
extern unsigned short prices[4096];
extern int sathours;
extern unsigned long satitem;
extern unsigned long watertype;
extern int deepkind;
extern unsigned long icpos;
extern unsigned long icidx;
extern unsigned long topidx;
extern unsigned long topfirst;
extern unsigned long blessbits;
extern unsigned long wear16bits;
extern unsigned long wearcount;
extern unsigned char repcls;
extern unsigned long alttype;
extern unsigned long basetype;
extern unsigned long easttype;
extern unsigned long southtype;
extern unsigned long randtype;
extern unsigned long brktype;
extern unsigned long unbrktype;
extern unsigned char repdcls;
extern unsigned long armorv;
extern short dmggear;
extern short dmgpct;
extern unsigned char stackcls;
extern unsigned long coinsum;
extern unsigned char fixcls;
extern unsigned long littype;
extern unsigned long *smokep;
extern unsigned long smoketype;
extern unsigned long burnleft;
extern unsigned long foodname;
extern Realm realms[125];
extern unsigned char nrealms;
extern unsigned long filltype;
extern unsigned long fillsrc;
extern unsigned long *putcell;
extern unsigned long *putp;
extern unsigned long valpct;
extern unsigned long valsum;
extern unsigned long valk;
extern unsigned char valcls;
extern Prov provs[100];
extern short nsites;
extern unsigned char provmap[22500];
extern unsigned long map[22500];
extern unsigned long mapitems[32767];
extern unsigned long rndprice;
extern unsigned long rndrem;
extern unsigned long rcitem;
extern int rccost;
extern unsigned short nmapitems;
extern unsigned short ltidx;
extern unsigned long *ltp;
extern unsigned short ntorches;
extern short torches[100];
extern short nmarks;
extern char marks[23][20];
extern short markx[23];
extern short marky[23];
extern short wmidx;
extern short wmlen;
extern unsigned char wmtag;
extern char *wmstart;
extern short rmidx;
extern short rmword;
extern unsigned char rmtag;
extern unsigned long mapflags[22500];
extern int mkidx;
extern unsigned long mkfirst;
extern unsigned long mkitem;
extern Spot spots[128];
extern int nmspot;
extern unsigned long ucidx;
extern unsigned long ucfirst;
extern unsigned char uccls;
extern short ucflags;
extern unsigned long addidx;
extern unsigned long addfirst;
extern unsigned long addj;
extern unsigned long fcidx;
extern unsigned long fcfirst;
extern unsigned long fc2idx;
extern unsigned long fc2first;
extern unsigned char fc2cls;
extern unsigned long ftidx;
extern unsigned long ftfirst;
extern unsigned long lhidx;
extern unsigned long lhfirst;
extern unsigned long delval;
extern unsigned long delj;
extern unsigned long delfirst;
extern unsigned long takeword;
extern unsigned long popidx;
extern unsigned long popfirst;
extern unsigned long popword;
extern unsigned long raidx;
extern int dotslot;
extern unsigned char dots[5390];
extern char mapblk250[250];
extern char mapblk2000[2000];
extern unsigned char blocks[30][30];
extern int indexok;
extern long objoff[80];
extern DWORD objlen[80];
extern long unitoff[80];
extern DWORD unitlen[80];
extern HANDLE rrfile;
extern DWORD rroff;
extern DWORD rrlen;
extern unsigned short gridver;
extern unsigned short lcbase;
extern DWORD lcword;
extern char *lcgrid;
extern unsigned short lcidx;
extern char gridhead[25];
extern DWORD lclen;
extern char *lcrec;
extern unsigned char lcflags;
extern short lcunused;
extern char lcname[16];
extern HANDLE lcfile;

short
uptime(void)
{
	short t, m, s;

	gettime();
	t = hour;
	m = minute;
	s = starttime;
	t = t * 60 + m;
	if (t < s)
		t += 1440;
	return t - starttime;
}

unsigned short
weightof(int i)
{
	return weights[i];
}

void
setweight(int i, short v)
{
	weights[i] = v;
}

unsigned char
classof(int i)
{
	return classes[i];
}

void
setclass(int i, unsigned char v)
{
	classes[i] = v;
}

void
flipitemflag(int i, unsigned short bit)
{
	if (itemflags[i] & bit)
		itemflags[i] ^= bit;
	else
		itemflags[i] |= bit;
}

void
flipcellflag(int i, unsigned short bit)
{
	if (cellflags[i] & bit)
		cellflags[i] ^= bit;
	else
		cellflags[i] |= bit;
}

int
itemseethru(unsigned long x)
{
	return itemflags[x & 0xfff] & 1;
}

int
itemwalkable(unsigned long x)
{
	return itemflags[x & 0xfff] & 2;
}

int
itemflag4(unsigned long x)
{
	return itemflags[x & 0xfff] & 4;
}

int
itemholds(unsigned long x)
{
	return itemflags[x & 0xfff] & 8;
}

int
isfood(unsigned long x)
{
	switch (classof(x & 0xfff)) {
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
		return 1;
	}
	return 0;
}

unsigned long
ageof(unsigned long x)
{
	x >>= 16;
	if (gameday <= x)
		return 0;
	return gameday - x;
}

int
eatable(unsigned long x)
{
	switch (classof(x & 0xfff)) {
	case 0x36:
		if (ageof(x) >= 4)
			return 0;
		return 1;
	case 0x38:
	case 0x39:
		return 0;
	}
	return isfood(x);
}

unsigned long
fishfood(unsigned long x)
{
	unsigned long n;

	n = ageof(x);
	if (n < 2)
		return 0x40017f;
	if (n == 2)
		return 0x80017f;
	if (n == 3)
		return 0x100017f;
	if (n == 4 || n == 5)
		return 0x8000017f;
	return 0x4000017f;
}

int
satiety(unsigned long x)
{
	int r;

	if (classof(x & 0xfff) == 0x36)
		x = fishfood(x);
	satitem = x;
	if (!isfood(x)) {
		satitem = 0;
		r = 0;
	} else {
		sathours = 0;
		if (x & 0x1000000)
			sathours = 0x10;
		if (x & 0x800000)
			sathours += 0x14;
		if (x & 0x400000)
			sathours += 0x18;
		r = sathours;
	}
	return r;
}

int
isfoodstuff(unsigned long x)
{
	switch (classof(x & 0xfff)) {
	case 0x36:
	case 0x37:
	case 0x38:
	case 0x39:
		return 1;
	}
	return 0;
}

int
isdrink(unsigned long x)
{
	switch (classof(x & 0xfff)) {
	case 0x38:
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
		return 1;
	}
	return 0;
}

int
ispond(unsigned long x)
{
	x &= 0xfff;
	if (x >= 0x48f && x <= 0x4d2 && x % 13 >= 10)
		return 1;
	return 0;
}

int
ispottery(unsigned long x)
{
	switch (typenames[x & 0xfff]) {
	case 6:
	case 0x19:
	case 0x33:
	case 0x55:
	case 0x56:
	case 0x57:
	case 0x7c:
		return 1;
	}
	return 0;
}

int
potsherd(void)
{
	return randn(3) + 0x541;
}

int
randspeckle(void)
{
	return randn(5) + 0x119;
}

int
ishousepart(unsigned long x)
{
	if ((x & 0xfff) < 0x9c)
		return 1;
	return 0;
}

unsigned long
breakdoor(unsigned long x)
{
	switch (x & 0xfff) {
	case 0x27:
	case 0x34:
		return x & 0xe00f000 | 0xe0000551;
	case 0x28:
	case 0x35:
		return x & 0xe00f000 | 0xe0000552;
	case 0x29:
	case 0x2a:
		return x & 0xe00f000 | 0xe0000553;
	case 0x36:
	case 0x37:
		return x & 0xe00f000 | 0xe0000554;
	}
	return x;
}

int
isswamp(unsigned long x)
{
	if (terrains[x & 0xfff] == 8)
		return 1;
	return 0;
}

int
terrain356(unsigned long x)
{
	switch (terrains[x & 0xfff]) {
	case 3:
	case 5:
	case 6:
		return 1;
	}
	return 0;
}

int
waterat(int x, int y)
{
	watertype = map[y * 150 + x];
	watertype &= 0xfff;
	if (watertype >= 0x478 && watertype < 0x5b0
	|| watertype >= 0x470 && watertype <= 0x472
	|| watertype == 0x461 || watertype == 0x459 || watertype == 0x45d)
		return 1;
	if (watertype >= 0x2e && watertype <= 0x33
	|| watertype >= 0x18e && watertype <= 0x190
	|| watertype >= 0x1fb && watertype <= 0x1fd
	|| watertype == 0x16 || watertype == 0x25 || watertype == 0x26
	|| watertype == 0x21)
		return 2;
	return 0;
}

int
deepwater(int x, int y)
{
	deepkind = waterat(x, y);
	if (deepkind == 0)
		return 0;
	if (map[x + y * 150] & 0x4000)
		return 0;
	return deepkind;
}

int
isfield(unsigned long t)
{
	t &= 0xfff;
	if (t >= 0x367 && t <= 0x373 || t == 0x382 || t == 0x387
	|| t == 0x361 || t == 0x362)
		return 1;
	return 0;
}

int
diggable(int x, int y)
{
	if (waterat(x, y))
		return 0;
	if (!(map[y * 150 + x] & 0x4000))
		return 0;
	switch (terrains[map[y * 150 + x] & 0xfff]) {
	case 5:
	case 7:
		return 0;
	}
	return 1;
}

int
buried(int k)
{
	while (mapitems[k] & 0x8000) {
		k++;
		if (classof(mapitems[k] & 0xfff) == 0x69)
			return 1;
	}
	return 0;
}

int
itemcell(int n)
{
	for (icpos = 0; icpos < 22500; icpos++) {
		if (!(map[icpos] & 0x1000))
			continue;
		for (icidx = map[icpos] >> 16; ; icidx++) {
			if (icidx == n)
				return icpos;
			if (!(mapitems[icidx] & 0x8000))
				break;
		}
	}
	return 0;
}

int
topitem(int pos)
{
	if (!(map[pos] & 0x1000))
		return 0;
	topfirst = map[pos] >> 16;
	topidx = topfirst;
	while (1) {
		if ((mapitems[topidx] & 0x4000) || (mapitems[topidx] & 0x2000)) {
			if (topidx == topfirst)
				return 0;
			return topidx - 1;
		}
		if (!(mapitems[topidx] & 0x8000))
			return topidx;
		topidx++;
	}
}

void
setwear(unsigned long *p, unsigned long v)
{
	*p &= 0x1fffffff;
	*p |= v << 29;
}

unsigned short
wearof(unsigned long x)
{
	return x >> 29;
}

unsigned short
gearof(unsigned long x)
{
	return x >> 16 & 0x1f;
}

unsigned short
blessof(unsigned long x)
{
	if (x == 0)
		return 7;
	return x >> 21 & 0xf;
}

void
setbless(unsigned long *p, unsigned short v)
{
	if (v > 15)
		v = 15;
	blessbits = v;
	blessbits <<= 21;
	*p &= 0xfe1fffff;
	*p |= blessbits;
}

unsigned long
wear16(unsigned long x)
{
	return x >> 28;
}

void
setwear16(unsigned long *p, unsigned short v)
{
	wear16bits = v;
	wear16bits <<= 28;
	*p &= 0xfffffff;
	*p |= wear16bits;
}

short
addwear(unsigned long *p)
{
	wearcount = *p >> 25;
	if (wearcount >= 0x7f)
		return 1;
	*p &= 0x1ffffff;
	wearcount++;
	wearcount <<= 25;
	*p |= wearcount;
	return 0;
}

short
repairable(unsigned long item)
{
	repcls = classof(item & 0xfff);
	switch (repcls) {
	case 0x3c:
	case 0x41:
	case 0x46:
	case 0x48:
	case 0x4b:
	case 0x50:
	case 0x55:
		return 1;
	case 0x49:
	case 0x4a:
		if (blessof(item) == 0)
			break;
		return 1;
	}
	return 0;
}

short
iswallns(unsigned long item)
{
	item &= 0xfff;
	if (item < 0x208 || item > 0x27c) {
		switch (item) {
		case 0xe2:
		case 0x164:
		case 0x165:
		case 0x16c:
		case 0x2b2:
		case 0x2b7:
		case 0x2bf:
		case 0x3b3:
		case 0x3bc:
		case 0x3c9:
		case 0x3cd:
		case 0x3d6:
		case 0x41d:
			return 1;
		}
	} else {
		switch (item % 13) {
		case 2:
		case 4:
		case 6:
		case 8:
		case 11:
			return 1;
		}
	}
	return 0;
}

short
iswallew(unsigned long item)
{
	item &= 0xfff;
	if (item < 0x208 || item > 0x27c) {
		switch (item) {
		case 0xd6:
		case 0x160:
		case 0x171:
		case 0x172:
		case 0x2a6:
		case 0x2a7:
		case 0x2c3:
		case 0x3b0:
		case 0x3b1:
		case 0x3b2:
		case 0x3d7:
		case 0x3d9:
		case 0x411:
			return 1;
		}
	} else {
		switch (item % 13) {
		case 3:
		case 5:
		case 7:
		case 9:
		case 12:
			return 1;
		}
	}
	return 0;
}

unsigned long
altlook(unsigned long item)
{
	alttype = item & 0xfff;
	if (alttype >= 0x1a0 && alttype <= 0x1c6
	|| alttype >= 0x1c7 && alttype <= 0x1e0)
		alttype += 0x4fa;
	else if (alttype >= 0x820 && alttype <= 0x853)
		alttype += 0x34;
	else
		switch (alttype) {
		case 0x18e:
			alttype = 0x141;
			break;
		case 0x533:
			alttype = 0x534;
			break;
		case 0x17f:
			alttype = 0x19d;
			break;
		default:
			goto keep;
		}
	alttype |= item & 0xfffff000;
	return alttype;
keep:
	return item;
}

unsigned long
baselook(unsigned long item)
{
	if (item == 0)
		return item;
	basetype = item & 0xfff;
	if (basetype >= 0x69a && basetype <= 0x6c0
	|| basetype >= 0x6c1 && basetype <= 0x6da)
		basetype -= 0x4fa;
	else if (basetype >= 0x659 && basetype <= 0x67f)
		basetype -= 0x4b9;
	else if (basetype >= 0x680 && basetype <= 0x699)
		basetype -= 0x4b9;
	else if (basetype >= 0x618 && basetype <= 0x63e)
		basetype -= 0x478;
	else if (basetype >= 0x63f && basetype <= 0x658)
		basetype -= 0x478;
	else if (basetype >= 0x854 && basetype <= 0x887)
		basetype -= 0x34;
	else if (basetype == 0x169 || basetype == 0x168)
		basetype = 0x221;
	else if (basetype == 0x611 || basetype == 0x60e)
		basetype = 0x604;
	else {
		switch (basetype) {
		case 0x19d:
		case 0x3fd:
		case 0x3fe:
			basetype = 0x17f;
			goto done;
		case 0x406:
		case 0x407:
			basetype = 0x18e;
			goto done;
		case 0x3f0:
		case 0x3f1:
		case 0x534:
			basetype = 0x533;
			goto done;
		}
		if (basetype >= 0x6db && basetype <= 0x6de)
			basetype = 0x1ca;
		else if (basetype >= 0x6e4 && basetype <= 0x6e7)
			basetype = 0x1d5;
		else if (basetype >= 0x141 && basetype <= 0x143)
			basetype = 0x18e;
		else
			return item;
	}
done:
	basetype |= item & 0xfffff000;
	return basetype;
}

unsigned long
eastlook(unsigned long item)
{
	easttype = baselook(item) & 0xfff;
	if (easttype >= 0x1a0 && easttype <= 0x1c6
	|| easttype >= 0x1c7 && easttype <= 0x1e0)
		easttype += 0x4b9;
	else if (easttype == 0x221)
		easttype = 0x169;
	else if (easttype == 0x604)
		easttype = 0x611;
	else if (easttype == 0x18e || easttype >= 0x141 && easttype <= 0x143)
		easttype = 0x407;
	else
		switch (easttype) {
		case 0x17f:
		case 0x19d:
			easttype = 0x3fd;
			break;
		case 0x533:
		case 0x534:
			easttype = 0x3f0;
			break;
		default:
			goto none;
		}
	easttype |= item & 0xfffff000;
	return easttype;
none:
	return 0;
}

unsigned long
southlook(unsigned long item)
{
	southtype = baselook(item) & 0xfff;
	if (southtype >= 0x1a0 && southtype <= 0x1c6
	|| southtype >= 0x1c7 && southtype <= 0x1e0)
		southtype += 0x478;
	else if (southtype == 0x221)
		southtype = 0x168;
	else if (southtype == 0x604)
		southtype = 0x60e;
	else if (southtype == 0x18e || southtype >= 0x141 && southtype <= 0x143)
		southtype = 0x406;
	else
		switch (southtype) {
		case 0x17f:
		case 0x19d:
			southtype = 0x3fe;
			break;
		case 0x533:
		case 0x534:
			southtype = 0x3f1;
			break;
		default:
			goto none;
		}
	southtype |= item & 0xfffff000;
	return southtype;
none:
	return 0;
}

unsigned long
randlook(unsigned long item)
{
	if ((item & 0xfff) == 0x1ca) {
		randtype = randn(100);
		if (randtype < 80) {
			randtype = randtype / 20 + 0x6db;
			randtype |= item & 0xfffff000;
			return randtype;
		}
	}
	if (randn(100) < 50)
		return baselook(item);
	return altlook(item);
}

void
copytype(int dst, int src)
{
	itemflags[dst] = itemflags[src];
	weights[dst] = weights[src];
	classes[dst] = classes[src];
	typenames[dst] = typenames[src];
	prices[dst] = prices[src];
	volumes[dst] = volumes[src];
}

unsigned long
broken(unsigned long item)
{
	item = baselook(item);
	brktype = item & 0xfff;
	if (brktype >= 0x1a0 && brktype <= 0x1c6)
		brktype += 0x1a0;
	else if (brktype >= 0x1c7 && brktype <= 0x1e0)
		brktype += 0x1a0;
	else
		return 0;
	item &= 0xfffff000;
	item |= brktype;
	setwear(&item, 7);
	return item;
}

unsigned long
unbroken(unsigned long item)
{
	item = baselook(item);
	unbrktype = item & 0xfff;
	if (unbrktype >= 0x340 && unbrktype <= 0x366)
		unbrktype -= 0x1a0;
	else if (unbrktype >= 0x367 && unbrktype <= 0x380)
		unbrktype -= 0x1a0;
	else
		return 0;
	item &= 0xfffff000;
	item |= unbrktype;
	setwear(&item, 7);
	return item;
}

unsigned long
repaired(unsigned long item)
{
	repdcls = classof(item & 0xfff);
	if (repdcls == 0x49 || repdcls == 0x4a)
		item = unbroken(item);
	setwear(&item, 0);
	return item;
}

void
emptyvessel(unsigned long *p)
{
	switch (*p & 0xfff) {
	case 0x144:
	case 0x14e:
	case 0x151:
	case 0x175:
	case 0x176:
	case 0x177:
	case 0x17d:
	case 0x181:
	case 0x182:
	case 0x185:
	case 0x18c:
	case 0x18d:
	case 0x192:
		*p &= 0xfffff000;
		*p |= 0x16b;
		return;
	case 0x1e8:
		*p &= 0xfffff000;
		*p |= 0x1e9;
		return;
	case 0x1eb:
		*p &= 0xfffff000;
		*p |= 0x1ec;
		return;
	}
	switch (classof(*p & 0xfff)) {
	case 0x87:
		(*p)--;
		break;
	case 0x88:
		*p -= 2;
		break;
	case 0x89:
		*p -= 3;
		break;
	case 0x8a:
		*p -= 4;
		break;
	case 0x8b:
		*p -= 5;
		break;
	}
}
int
pickcrop(unsigned long *p)
{
	switch (*p & 0xfff) {
	case 0x6f6:
		*p &= 0xfffff000;
		*p |= 0x702;
		return 1;
	case 0x702:
		*p &= 0xfffff000;
		*p |= 0x703;
		return 1;
	case 0x6fb:
		*p &= 0xfffff000;
		*p |= 0x704;
		return 1;
	case 0x704:
		*p &= 0xfffff000;
		*p |= 0x705;
		return 1;
	case 0x6fc:
		*p &= 0xfffff000;
		*p |= 0x706;
		return 1;
	case 0x706:
		*p &= 0xfffff000;
		*p |= 0x707;
		return 1;
	case 0x712:
		*p &= 0xfffff000;
		*p |= 0x738;
		return 1;
	case 0x738:
		*p &= 0xfffff000;
		*p |= 0x739;
		return 1;
	case 0x709:
		*p &= 0xfffff000;
		*p |= 0x729;
		return 1;
	case 0x70a:
		*p &= 0xfffff000;
		*p |= 0x72a;
		return 1;
	case 0x70b:
		*p &= 0xfffff000;
		*p |= 0x72b;
		return 1;
	case 0x70f:
		*p &= 0xfffff000;
		*p |= 0x736;
		return 1;
	case 0x710:
		*p &= 0xfffff000;
		*p |= 0x737;
		return 1;
	case 0x70c:
		*p &= 0xfffff000;
		*p |= 0x72c;
		return 1;
	case 0x703:
	case 0x705:
	case 0x707:
	case 0x739:
		*p &= 0xfffff000;
		*p |= 0x7ea;
		return 1;
	}
	return 0;
}

int
itemflag10(unsigned long x)
{
	return itemflags[x & 0xfff] & 0x10;
}

int
itemburns(unsigned long x)
{
	return itemflags[x & 0xfff] & 0x20;
}

int
cellseethru(unsigned long x)
{
	return cellflags[x & 0xfff] & 1;
}

int
cellwalkable(unsigned long x)
{
	return cellflags[x & 0xfff] & 2;
}

int
cellflag4(unsigned long x)
{
	return cellflags[x & 0xfff] & 4;
}

int
cellflag40(unsigned long x)
{
	return cellflags[x & 0xfff] & 0x40;
}

int
cellholds(unsigned long x)
{
	return cellflags[x & 0xfff] & 8;
}

int
nothingat(int x)
{
	if (x)
		return 0;
	return x;
}

int
cellburns(unsigned long x)
{
	return cellflags[x & 0xfff] & 0x20;
}

void
clearprovs(void)
{
	memset(provs, 0, sizeof provs);
	memset(provmap, 0, sizeof provmap);
	nsites = 0;
}

short
armorval(unsigned long item, short v)
{
	armorv = wear16(item) * (100 - armors[gearof(item)].wornpct) / 15;
	armorv = (int)v * (100 - armorv) / 100;
	armorv = armorv * ((blessof(item) - 7) * 3 + 100) / 100;
	return armorv;
}

short
protection(unsigned long item)
{
	return (armorval(item, armors[gearof(item)].prot1)
		+ armorval(item, armors[gearof(item)].prot2)
		+ armorval(item, armors[gearof(item)].prot3)) / 3;
}

void
damagerange(unsigned long item, short *a, short *b)
{
	dmggear = gearof(item);
	*a = metals[dmggear].dmgmin;
	*b = metals[dmggear].dmgmax;
	dmgpct = 100 - wear16(item) * (100 - metals[dmggear].wornpct) / 15;
	dmgpct = dmgpct * ((blessof(item) - 7) * 3 + 100) / 100;
	*a = *a * dmgpct / 100;
	*b = *b * dmgpct / 100;
}

short
canstack(unsigned long a, unsigned long b)
{
	stackcls = classof(a & 0xfff);
	if (classof(b & 0xfff) != stackcls)
		return 0;
	if (!((a & 0x1000) && (b & 0x1000) || !(a & 0x1000) && !(b & 0x1000)))
		return 0;
	if (stackcls == 15 || stackcls == 16) {
		b >>= 16;
		a >>= 16;
		if (b + a <= 15000)
			return 1;
	}
	return 0;
}

unsigned long
coinitem(unsigned long n, int flag)
{
	unsigned long r;

	if (n >= 7500)
		r = n << 16 | 0x4f1;
	else if (n >= 2000)
		r = n << 16 | 0x4f0;
	else if (n >= 750)
		r = n << 16 | 0x4ef;
	else if (n >= 375)
		r = n << 16 | 0x4ee;
	else if (n >= 175)
		r = n << 16 | 0x4ed;
	else if (n >= 75)
		r = n << 16 | 0x4ec;
	else if (n >= 38)
		r = n << 16 | 0x4eb;
	else if (n >= 11)
		r = n << 16 | 0x4ea;
	else if (n >= 1)
		r = n << 16 | n + 0x4df;
	else
		r = 0x104e0;
	if (flag)
		r += 0x1a;
	return r;
}

unsigned long
addcoins(unsigned long *p, unsigned long b)
{
	coinsum = (*p >> 16) + (b >> 16);
	if (classof(*p & 0xfff) == 0x10) {
		*p &= 0xf000;
		*p |= coinitem(coinsum, 1);
	} else {
		*p &= 0xf000;
		*p |= coinitem(coinsum, 0);
	}
	return coinsum;
}

void
fixcoins(unsigned long *p)
{
	fixcls = classof(*p & 0xfff);
	if (fixcls == 0xf || fixcls == 0x10)
		addcoins(p, 0);
}

void
clamptorch(unsigned long *p)
{
	if (*p >> 16 >= 0x400) {
		*p &= 0xffff;
		*p |= 0x3ff0000;
	}
}

short
islittorch(unsigned long item)
{
	switch (item & 0xfff) {
	case 0x604:
	case 0x60e:
	case 0x611:
		return 1;
	}
	return 0;
}

short
isunlittorch(unsigned long item)
{
	switch (item & 0xfff) {
	case 0x168:
	case 0x169:
	case 0x221:
		return 1;
	}
	return 0;
}

int
isfirewood(unsigned long item)
{
	switch (item & 0xfff) {
	case 0x171:
	case 0x581:
	case 0x59a:
	case 0x59b:
	case 0x5a7:
	case 0x5a8:
		return 1;
	}
	return 0;
}

short
setlit(unsigned long *p, short dir)
{
	littype = *p & 0xfff;
	if (dir) {
		switch (littype) {
		case 0x221:
			littype = 0x604;
			break;
		case 0x169:
			littype = 0x611;
			break;
		case 0x168:
			littype = 0x60e;
			break;
		case 0x15b:
			littype = 0x5e4;
			break;
		case 0x15c:
			littype = 0x5e7;
			break;
		case 0x16a:
			littype = 0x5f4;
			break;
		case 0x1f4:
			littype = 0x5da;
			break;
		case 0x24b:
			littype = 0x5d7;
			break;
		case 0x172:
			littype = 0x5f7;
			break;
		case 0x17e:
		case 0x171:
		case 0x581:
		case 0x59a:
		case 0x59b:
		case 0x5a7:
		case 0x5a8:
			littype = 0x5fa;
			break;
		case 0x1e6:
			littype = 0x5fe;
			break;
		case 0xee:
			littype = 0x601;
			break;
		case 0xed:
			littype = 0x607;
			break;
		case 0x15e:
			littype = 0x60b;
			break;
		default:
			return 0;
		}
	} else {
		switch (littype) {
		case 0x604:
			littype = 0x221;
			break;
		case 0x611:
			littype = 0x169;
			break;
		case 0x60e:
			littype = 0x168;
			break;
		case 0x5e4:
			littype = 0x15b;
			break;
		case 0x5e7:
			littype = 0x15c;
			break;
		case 0x5f4:
			littype = 0x16a;
			break;
		case 0x5da:
			littype = 0x1f4;
			break;
		case 0x5d7:
			littype = 0x24b;
			break;
		case 0x5f7:
			littype = 0x172;
			break;
		case 0x5fa:
			littype = 0x17e;
			break;
		case 0x5fe:
			littype = 0x1e6;
			break;
		case 0x601:
			littype = 0xee;
			break;
		case 0x607:
			littype = 0xed;
			break;
		case 0x60b:
			littype = 0x15e;
			break;
		default:
			return 0;
		}
	}
	*p &= 0xfffff000;
	*p |= littype;
	clamptorch(p);
	return 1;
}

int
setsmoke(int pos, short dir)
{
	smoketype = map[pos];
	if (!(smoketype & 0x2000))
		return 0;
	smokep = &mapitems[smoketype >> 16];
	do {
		if (*smokep & 0x4000)
			goto found;
	} while (*smokep++ & 0x8000);
	return 0;
found:
	smoketype = *smokep & 0xfff;
	if (dir) {
		switch (smoketype) {
		case 0x2d2:
			smoketype = 0x5dd;
			break;
		case 0x2d3:
			smoketype = 0x5ea;
			break;
		case 0x2d4:
			smoketype = 0x5ed;
			break;
		default:
			return 0;
		}
	} else {
		switch (smoketype) {
		case 0x5dd:
			smoketype = 0x2d2;
			break;
		case 0x5ea:
			smoketype = 0x2d3;
			break;
		case 0x5ed:
			smoketype = 0x2d4;
			break;
		default:
			return 0;
		}
	}
	*smokep &= 0xfffff000;
	*smokep |= smoketype;
	return 1;
}

short
istorch(unsigned long item)
{
	if (classof(item & 0xfff) == 0x56)
		return 1;
	return 0;
}

int
usabletorch(unsigned long item)
{
	if (classof(item & 0xfff) == 0x56 && item >> 16 > 1)
		return 1;
	return 0;
}

short
burntorch(unsigned long *p)
{
	switch (*p & 0xfff) {
	case 0x604:
	case 0x60e:
	case 0x611:
		burnleft = *p >> 16;
		if (burnleft >= 0x400)
			return 0;
		if (burnleft) {
			burnleft--;
			*p &= 0xffff;
			*p |= burnleft << 16;
		}
		if (burnleft == 0) {
			*p &= 0xfffff000;
			*p |= 0x221;
			return 1;
		}
	default:
		return 0;
	}
}

unsigned short
idof(unsigned long x)
{
	return (x & 0x1ff0000) >> 16;
}

void
setid(unsigned long *p, unsigned long v)
{
	*p &= 0xfe00ffff;
	*p |= v << 16;
}

unsigned long
newfood(unsigned long t)
{
	t &= 0xfff;
	switch (classof(t)) {
	case 0x82:
	case 0x87:
		return t;
	case 0x83:
	case 0x88:
		return t + 0x1000000;
	case 0x84:
	case 0x86:
	case 0x89:
	case 0x8b:
		return t + 0x10000000;
	case 0x85:
	case 0x8a:
		return t + 0x800000;
	}
	switch (t) {
	default:
		foodname = typenames[t & 0xfff];
		if (foodname == 0xab || foodname == 0xa9)
			return t + 0x800000;
		if (foodname != 0xaa && foodname != 0xac && foodname != 0xc0)
			break;
		/* fall through */
	case 0x141:
	case 0x142:
	case 0x143:
	case 0x17f:
	case 0x18e:
	case 0x19d:
	case 0x3fd:
	case 0x3fe:
	case 0x406:
	case 0x407:
		return t + 0x400000;
	case 0x174:
		return t + 0x90080000;
	case 0x177:
		return t + 0x100000;
	case 0x192:
		return t + 0x200000;
	case 0x14e:
		return t + 0x80000;
	case 0x176:
	case 0x183:
		return t + 0x20000000;
	case 0x17d:
		return 0xc000017d;
	case 0x178:
	case 0x4ac:
	case 0x4bd:
	case 0x4be:
		return t + 0x90100000;
	case 0x4a5:
		return t + 0xc0000000;
	case 0x181:
		return t + 0x8000000;
	case 0x144:
		return t + 0x20000;
	case 0x3f0:
	case 0x3f1:
	case 0x533:
	case 0x534:
		return t | gameday << 16;
	case 0x182:
		return t + 0x4000000;
	case 0x185:
		return t + 0x26600000;
	case 0x14f:
	case 0x184:
	case 0x190:
	case 0x191:
	case 0x488:
	case 0x495:
	case 0x496:
		return t + 0x1000000;
	case 0x173:
	case 0x18c:
	case 0x18d:
	case 0x1e8:
	case 0x1eb:
		return t + 0x10000000;
	case 0x18f:
		return t + 0x800000;
	case 0x175:
		return t + 0x40000;
	case 0x151:
		t += 0x10000;
	}
	return t;
}

unsigned long
newgear(unsigned long item)
{
	item = baselook(item);
	switch (item) {
	case 0x1a0:
		item = 0xe101a0;
		break;
	case 0x1cd:
		item = 0xe701cd;
		break;
	case 0x1c8:
		item = 0xe201c8;
		break;
	case 0x1c9:
		item = 0xe301c9;
		break;
	case 0x1ca:
		item = 0xe401ca;
		break;
	case 0x1cb:
		item = 0xe501cb;
		break;
	case 0x1cc:
		item = 0xe601cc;
		break;
	case 0x1c7:
		item = 0xe101c7;
		break;
	case 0x1ce:
		item = 0xe801ce;
		break;
	case 0x1cf:
	case 0x1d0:
	case 0x1d1:
	case 0x1d2:
	case 0x1d3:
	case 0x1d4:
		item = 0xe901cf;
		break;
	case 0x1d5:
		item = 0xef01d5;
		break;
	case 0x1d6:
		item = 0xf001d6;
		break;
	case 0x1a1:
		item = 0xe201a1;
		break;
	case 0x1a2:
		item = 0xe301a2;
		break;
	case 0x1b7:
		item = 0xe401b7;
		break;
	case 0x1b8:
		item = 0xe501b8;
		break;
	case 0x1ba:
		item = 0xe601ba;
		break;
	case 0x1bb:
		item = 0xe701bb;
		break;
	case 0x1bc:
		item = 0xe801bc;
		break;
	case 0x1bd:
		item = 0xe901bd;
		break;
	case 0x1be:
		item = 0xea01be;
		break;
	case 0x1bf:
		item = 0xf901bf;
		break;
	case 0x1ad:
		item = 0xec01ad;
		break;
	case 0x1ae:
		item = 0xed01ae;
		break;
	case 0x1af:
		item = 0xee01af;
		break;
	case 0x1b0:
		item = 0xef01b0;
		break;
	case 0x1b1:
		item = 0xf001b1;
		break;
	case 0x1b2:
		item = 0xf101b2;
		break;
	case 0x1b3:
		item = 0xf701b3;
		break;
	case 0x1b4:
		item = 0xf801b4;
		break;
	case 0x1b5:
		item = 0xec01b5;
		break;
	case 0x1b9:
		item = 0xf601b9;
		break;
	case 0x1c6:
		item = 0xfa01c6;
		break;
	case 0x1a7:
		item = 0xf201a7;
		break;
	case 0x1a8:
		item = 0xf301a8;
		break;
	case 0x1a9:
		item = 0xf401a9;
		break;
	default:
		return randlook(item);
	}
	item = randlook(item);
	setwear(&item, randn(3) + 2);
	return item;
}

unsigned long
newitem(unsigned long item)
{
	unsigned long r;

	item &= 0xfff;
	if (item == 0x186)
		return item | 0x80000000;
	switch (classof(item)) {
	case 0:
	case 0x1e:
	case 0x6f:
		return item;
	case 0x10:
		r = item & 0xfff;
		if (r == 0x503)
			r = randn(5) + 2;
		else if (r == 0x504)
			r = randn(20) + 5;
		else if (r == 0x506)
			r = randn(250) + 20;
		else
			r = randn(25) + 2;
	coin:
		r <<= 16;
		r += item;
		addcoins(&r, 0);
		return r;
	case 0xf:
		r = item & 0xfff;
		if (r == 0x4e9)
			r = randn(6) + 2;
		else if (r == 0x4ea)
			r = randn(20) + 5;
		else if (r == 0x4ec)
			r = randn(250) + 20;
		else
			r = randn(25) + 2;
		goto coin;
	case 0xa:
		// FIXED: the test lets nrealms reach 255, and the 121st container lands past realms[125]. Refuse it as the test meant.
#ifdef FEATURE_FAULTS
		if (nrealms + 5 >= 125)
#else
		if (nrealms == 0xff)
#endif
			return -1;
		memset(&realms[nrealms + 5], 0, sizeof realms[0]);
		realms[nrealms + 5].item = item & 0xfff;
		r = item + (nrealms + 5 << 16);
		nrealms++;
		setwear(&r, randn(3) + 2);
		return r;
	case 1:
		setwear(&item, randn(3) + 2);
		return item;
	case 0x56:
		r = item | 0x3ff0000;
		return r;
	case 4:
		r = item & 0xfff;
		setid(&r, 0xff);
		r |= 0xe000000;
		return r;
	case 0x67:
		if (typenames[item & 0xfff] == 0xc6)
			return item | 0x10000;
		return item | 0x20000;
	default:
		if (item >= 0x340 && item <= 0x366) {
			r = newgear(item - 0x1a0);
		conv:
			r = broken(r);
			if (r) {
				setbless(&r, 7);
				return r;
			}
			return item;
		}
		if (item >= 0x367 && item <= 0x380) {
			r = newgear(item - 0x1a0);
			goto conv;
		}
		if (isfood(item))
			return newfood(item);
		return newgear(item);
	}
}

void
filltypes(void)
{
	for (filltype = 0; filltype < 0x1000; filltype++) {
		fillsrc = baselook(filltype);
		if (filltype != fillsrc)
			copytype(filltype, fillsrc);
	}
	for (filltype = 0x340; filltype <= 0x366; filltype++) {
		fillsrc = filltype - 0x1a0;
		copytype(filltype, fillsrc);
		setclass(filltype, 0x4a);
		fillsrc = newgear(fillsrc);
		fillsrc = gearof(fillsrc);
		prices[filltype] = prices[filltype] * armors[fillsrc].pricepct / 100;
	}
	for (filltype = 0x367; filltype <= 0x380; filltype++) {
		fillsrc = filltype - 0x1a0;
		copytype(filltype, fillsrc);
		setclass(filltype, 0x49);
		fillsrc = newgear(fillsrc);
		fillsrc = gearof(fillsrc);
		prices[filltype] = prices[filltype] * metals[fillsrc].pricepct / 100;
	}
}

int
canput(int pos)
{
	putcell = &map[pos];
	if (!cellholds(*putcell))
		return 0;
	if (!(*putcell & 0x1000))
		return 1;
	for (putp = &mapitems[*putcell >> 16]; ; putp++) {
		if (!itemholds(*putp))
			return 0;
		if (!(*putp & 0x8000))
			return 1;
	}
}

unsigned long
valueof(unsigned long item)
{
	valsum = item & 0xfff;
	valcls = classof(valsum);
	switch (valcls) {
	case 0xf:
		return (item >> 16) * 10;
	case 0x10:
		return (item >> 16) * 250;
	case 0x3c:
	case 0x41:
	case 0x46:
	case 0x48:
	case 0x4b:
	case 0x50:
	case 0x55:
		valsum = prices[valsum];
		valk = blessof(item);
		if (valk < 7)
			valsum -= (7 - valk) * valsum * 12 / 100;
		else if (valk > 7)
			valsum += (valk - 7) * valsum * 12 / 100;
		if (item >> 29 == 0)
			return valsum;
		if (valcls == 0x3c) {
			valk = gearof(item);
			valk = metals[valk].pricepct;
		} else
			valk = armors[gearof(item)].pricepct;
		valpct = 100 - valk;
		item >>= 29;
		valpct = valk + valpct * (8 - item) / 8;
		valsum = valsum * valpct / 100;
		return valsum;
	case 0x49:
	case 0x4a:
		valsum = prices[valsum];
		valk = blessof(item);
		if (valk == 0)
			return 0;
		if (valk < 7)
			valsum -= (7 - valk) * valsum * 12 / 100;
		else if (valk > 7)
			valsum += valsum * (valk - 7) * 12 / 100;
		return valsum;
	case 0x62:
	case 0x63:
		valsum = prices[valsum];
		if (item & 0x200000)
			valsum /= 5;
		if (item & 0x100000)
			valsum += prices[0x1cb] * 2;
		if (item & 0x80000)
			valsum += prices[0x1cb] * 2;
		if (item & 0x40000)
			valsum += prices[0x1cb] / 2;
		if (item & 0x20000)
			valsum += prices[0x1cb];
		if (item & 0x10000)
			valsum += prices[0x1cb];
		return valsum;
	case 0x36:
		valk = ageof(item);
		if (valk >= 4)
			return 0;
		if (valk >= 3)
			valpct = 33;
		else if (valk >= 2)
			valpct = 66;
		else
			valpct = 100;
		valk = prices[valsum] * valpct / 100;
		if (valk < 1)
			valk = 1;
		return valk;
	}
	return prices[valsum];
}

unsigned long
roundprice(unsigned long n)
{
	rndprice = n / 10;
	if (n % 10 >= 5)
		rndprice++;
	if (rndprice <= 20)
		;
	else if (rndprice <= 50) {
		rndrem = rndprice % 5;
		rndprice /= 5;
		if (rndrem >= 3)
			rndprice++;
		rndprice *= 5;
	} else if (rndprice <= 300) {
		rndrem = rndprice % 10;
		rndprice /= 10;
		if (rndrem >= 5)
			rndprice++;
		rndprice *= 10;
	} else if (rndprice <= 750) {
		rndrem = rndprice % 50;
		rndprice /= 50;
		if (rndrem >= 25)
			rndprice++;
		rndprice *= 50;
	} else {
		rndrem = rndprice % 100;
		rndprice /= 100;
		if (rndrem >= 50)
			rndprice++;
		rndprice *= 100;
	}
	rndprice *= 10;
	if (rndprice == 0)
		rndprice++;
	return rndprice;
}

unsigned long
silvprice(unsigned long n)
{
	n = roundprice(n);
	return n / 10 + (n % 10 >= 5 ? 1 : 0);
}

unsigned long
silvvalue(unsigned long item)
{
	switch (classof(item & 0xfff)) {
	case 0xf:
	case 0x10:
		return valueof(item) / 10;
	}
	return silvprice(valueof(item));
}

int
percoin(unsigned long item)
{
	unsigned long v;

	v = valueof(item);
	if (v == 0)
		return 0;
	if (v >= 10)
		return 1;
	if (v <= 1)
		return 10;
	if (v <= 2)
		return 5;
	if (v <= 3)
		return 3;
	return 2;
}

int
repaircost(unsigned long item)
{
	rcitem = repaired(item);
	rccost = valueof(rcitem) - valueof(item);
	if (rccost <= 0)
		rccost = 1;
	return rccost;
}

short
volumeof(unsigned long item)
{
	return volumes[item & 0xfff];
}

int
loadtables(void)
{
	char *p;
	unsigned i;

	if (!seekentry(5000))
		return 0;
	unpack(scrbuf, 0x4b9c4, workbuf, fileread(workbuf, 1, storedn, datfh));
	p = scrbuf;
	memread(cellflags, sizeof cellflags, 1, &p);
	memread(itemflags, sizeof itemflags, 1, &p);
	memread(weights, sizeof weights, 1, &p);
	memread(classes, sizeof classes, 1, &p);
	memread(typenames, sizeof typenames, 1, &p);
	memread(prices, sizeof prices, 1, &p);
	memread(volumes, sizeof volumes, 1, &p);
	memread(terrains, sizeof terrains, 1, &p);
	filltypes();
	if (!seekentry(5001))
		return 0;
	unpack(scrbuf, 0x4b9c4, workbuf, fileread(workbuf, 1, storedn, datfh));
	p = scrbuf;
	memread(faceflags, 0x11d, 1, &p);
	if (!seekentry(5002))
		return 0;
	unpack(scrbuf, 0x4b9c4, workbuf, fileread(workbuf, 1, storedn, datfh));
	p = scrbuf;
	memread(sqrttab, 0xb491, 1, &p);
	for (i = 0; i < 1; i++) {
		if (!seekentry(5015 + i))
			return 0;
		unpack(scrbuf, 0x4b9c4, workbuf, fileread(workbuf, 1, storedn, datfh));
		p = scrbuf;
		memread(((char (*)[0x3a800])pics)[i], sizeof ((char (*)[0x3a800])pics)[i], 1, &p);
	}
	return 1;
}

void
setmapbit(int pos, unsigned long mask, char set)
{
	map[pos] &= 0xffffffff - mask;
	if (set)
		map[pos] |= mask;
}

void
setitembit(int i, unsigned long mask, char set)
{
	mapitems[i] &= 0xffffffff - mask;
	if (set)
		mapitems[i] |= mask;
}

void
listtorches(void)
{
	ntorches = 0;
	for (ltidx = 1, ltp = &mapitems[ltidx]; ltidx < nmapitems; ltidx++, ltp++)
		if (islittorch(*ltp)) {
			// FIXED: the 101st lit torch lands past torches[100]. List the first 100.
#ifdef FEATURE_FAULTS
			if (ntorches == 100)
				break;
#endif
			torches[ntorches] = ltidx;
			ntorches++;
		}
}

void
clearmarks(void)
{
	nmarks = 0;
	memset(marks, 0, sizeof marks);
	memset(markx, 0, sizeof markx);
	memset(marky, 0, sizeof marky);
}

char *
writemarks(char *p)
{
	wmstart = p;
	nmarks = 0;
	for (wmidx = 0; wmidx < 23; wmidx++) {
		if (marks[wmidx][0]) {
			wmtag = 0x63;
			nmarks++;
			memwrite(&wmtag, 1, 1, &p);
			memwrite(&wmidx, 2, 1, &p);
			memwrite(&markx[wmidx], 2, 1, &p);
			memwrite(&marky[wmidx], 2, 1, &p);
			wmlen = 20;
			memwrite(&wmlen, 2, 1, &p);
			memwrite(marks[wmidx], 20, 1, &p);
		} else {
			wmtag = 0xb;
			memwrite(&wmtag, 1, 1, &p);
		}
	}
	wmtag = 0x2d;
	memwrite(&wmtag, 1, 1, &p);
	return p;
}

char *
readmarks(char *p)
{
	clearmarks();
	nmarks = 0;
	for (rmidx = 0; rmidx < 23; rmidx++) {
		memread(&rmtag, 1, 1, &p);
		if (rmtag == 0x63) {
			nmarks++;
			memread(&rmword, 2, 1, &p);
			memread(&markx[rmidx], 2, 1, &p);
			memread(&marky[rmidx], 2, 1, &p);
			memread(&rmword, 2, 1, &p);
			memread(marks[rmidx], rmword, 1, &p);
		} else if (rmtag != 0xb)
			return p;
	}
	return p;
}

void
clearmap(void)
{
	int i;

	for (i = 0; i < 22500; i++)
		map[i] = randn(4) + 40;
	memset(mapflags, 0, sizeof mapflags);
	for (i = 0; i < 22500; i++)
		map[i] = 0xc027;
	memset(mapitems, 0, sizeof mapitems);
	nmapitems = 1;
	clearmarks();
	closeborder();
	listtorches();
}

unsigned long
markerat(int pos)
{
	if (!(map[pos] & 0x1000))
		return 0;
	mkfirst = map[pos] >> 16;
	for (mkidx = mkfirst; ; mkidx++) {
		mkitem = mapitems[mkidx];
		if (classof(mkitem & 0xfff) == 0x69)
			return 0;
		if (mkitem & 0x2000) {
			if ((mkitem & 0xfff) == 0x186)
				return mkitem;
		} else if (mkitem & 0x4000)
			return 0;
		if (!(mkitem & 0x8000))
			return 0;
	}
}

unsigned long
setspot(unsigned long item, unsigned long v)
{
	return item & 0xff80ffff | v << 16;
}

int
newspot(void)
{
	if (nspots >= 127)
		return -1;
	memset(&spots[nspots], 0, sizeof spots[0]);
	spots[nspots].unused = 0x63;
	nspots++;
	return nspots - 1;
}

unsigned long
newmarker(int a, int b, int c)
{
	nmspot = newspot();
	if (nmspot < 0)
		return 0;
	spots[nmspot].tomap = a;
	spots[nmspot].x = b;
	spots[nmspot].y = c;
	spots[nmspot].hours = 0;
	return setspot(0x186, nmspot);
}

void
updatecell(int pos)
{
	ucfirst = map[pos] >> 16;
	mapflags[pos] &= 0x238;
	if (cellseethru(map[pos])) {
		setmapbit(pos, 0x8000, 1);
		if (map[pos] & 0x1000)
			for (ucidx = ucfirst; ; ucidx++) {
				if (!itemseethru(mapitems[ucidx])) {
					setmapbit(pos, 0x8000, 0);
					break;
				}
				if (!(mapitems[ucidx] & 0x8000))
					break;
			}
	} else
		setmapbit(pos, 0x8000, 0);
	if (cellwalkable(map[pos])) {
	set:
		setmapbit(pos, 0x4000, 1);
		if (map[pos] & 0x1000)
			for (ucidx = ucfirst; ; ucidx++) {
				if (!itemwalkable(mapitems[ucidx]))
					setmapbit(pos, 0x4000, 0);
				else if (classof(mapitems[ucidx] & 0xfff) == 0x69)
					setmapbit(pos, 0x4000, 1);
				if (!(mapitems[ucidx] & 0x8000))
					break;
			}
	} else if (map[pos] & 0x1000 && classof(mapitems[ucfirst] & 0xfff) == 0xe6)
		goto set;
	else
		setmapbit(pos, 0x4000, 0);
	if (cellburns(map[pos]))
		mapflags[pos] |= 0x100;
	if (map[pos] & 0x1000)
		for (ucidx = ucfirst; ; ucidx++) {
			uccls = classof(mapitems[ucidx] & 0xfff);
			mapitems[ucidx] &= 0xffff9fff;
			if (itemburns(mapitems[ucidx]))
				mapflags[pos] |= 0x100;
			ucflags = itemflags[mapitems[ucidx] & 0xfff];
			if (ucflags & 0x800)
				mapflags[pos] |= 0x800;
			if (ucflags & 0x1000)
				mapflags[pos] |= 0x1000;
			if (ucflags & 0x2000)
				mapflags[pos] |= 0x2000;
			if (ucflags & 0x4000)
				mapflags[pos] |= 0x4000;
			if (ucflags & 0x80)
				mapflags[pos] |= 0x8000;
			if (ucflags & 0x100)
				mapflags[pos] |= 0x10000;
			if (ucflags & 0x200)
				mapflags[pos] |= 0x20000;
			if (ucflags & 0x400)
				mapflags[pos] |= 0x40000;
			if (uccls == 0xff) {
				mapitems[ucidx] |= 0x4000;
				setmapbit(pos, 0x2000, 1);
				return;
			}
			if (uccls == 0xf0) {
				mapitems[ucidx] |= 0x2000;
				mapflags[pos] |= 0x80;
			} else if (uccls == 1 && (mapitems[ucidx] & 0x8000000) == 0)
				mapflags[pos] |= 1;
			else if (uccls >= 0x6a && uccls <= 0x6d) {
				mapflags[pos] &= 0xfffffffc;
				mapflags[pos] |= uccls - 0x2a;
			} else if (uccls >= 0x78 && uccls <= 0x7b) {
				mapflags[pos] &= 0xfffffffc;
				mapflags[pos] |= uccls - 0x38;
			} else if (uccls == 0x15)
				mapflags[pos] |= 0x400;
			if (typenames[mapitems[ucidx] & 0xfff] == 0x72)
				mapflags[pos] |= 0x400;
			if (!(mapitems[ucidx] & 0x8000))
				break;
		}
	setmapbit(pos, 0x2000, 0);
}

void
setcelltype(int pos, unsigned long type)
{
	unsigned long *p;

	p = &map[pos];
	*p &= 0xffff1000;
	*p |= type & 0xfff;
	updatecell(pos);
}

unsigned long
additem(int pos, unsigned long item)
{
	// FIXED: an insertion moves the items up to mapitems[nmapitems + 1], past the 32767 items when nmapitems is 32766. Refuse the item there.
#ifdef FEATURE_FAULTS
	if (nmapitems >= 0x7ffe)
#else
	if (nmapitems >= 0x7fff)
#endif
		return 0;
	if ((item & 0xfff) == 0)
		return 0;
	item &= 0xffff1fff;
	if (classof(item & 0xfff) == 0xf0)
		item |= 0x2000;
	else if (classof(item & 0xfff) == 0xff) {
		item |= 0x4000;
		map[pos] |= 0x2000;
	}
	if ((map[pos] & 0x1000) == 0) {
		addidx = nmapitems++;
		map[pos] &= 0xffff;
		map[pos] |= (addidx << 16) + 0x1000;
		mapitems[addidx] = item;
	} else {
		addfirst = map[pos] >> 16;
		if (!(item & 0x2000) && item & 0x4000) {
			for (addidx = addfirst; !(mapitems[addidx] & 0x4000); ) {
				if (!(mapitems[addidx] & 0x8000)) {
					addidx++;
					goto insert;
				}
				addidx++;
			}
			mapitems[addidx] &= 0xfffff000;
			mapitems[addidx] |= item;
			return addidx;
		}
		for (addidx = addfirst; !(mapitems[addidx] & 0x2000) && !(mapitems[addidx] & 0x4000); ) {
			if (!(mapitems[addidx] & 0x8000)) {
				addidx++;
				break;
			}
			addidx++;
		}
	insert:
		nmapitems++;
		for (addj = nmapitems; addj > addidx; addj--)
			mapitems[addj] = mapitems[addj - 1];
		for (addj = 0; addj < 22500; addj++)
			if (map[addj] & 0x1000 && map[addj] >> 16 >= addidx)
				map[addj] += 0x10000;
		if (addidx == addfirst) {
			map[pos] -= 0x10000;
			mapitems[addfirst] = item + 0x8000;
		} else {
			mapitems[addidx] = (mapitems[addidx - 1] & 0x8000) + item;
			mapitems[addidx - 1] |= 0x8000;
		}
	}
	updatecell(pos);
	listtorches();
	return addidx;
}

unsigned short
findclass(int pos, unsigned char c)
{
	if (!(map[pos] & 0x1000))
		return 0;
	fcfirst = map[pos] >> 16;
	for (fcidx = fcfirst; ; fcidx++) {
		if (classof(mapitems[fcidx] & 0xfff) == c)
			return fcidx;
		if (!(mapitems[fcidx] & 0x8000))
			return 0;
	}
}

short
findclass2(int pos, unsigned char c1, unsigned char c2)
{
	if (!(map[pos] & 0x1000))
		return 0;
	fc2first = map[pos] >> 16;
	for (fc2idx = fc2first; ; fc2idx++) {
		fc2cls = classof(mapitems[fc2idx] & 0xfff);
		if (fc2cls == c1 || fc2cls == c2)
			return fc2idx;
		if (!(mapitems[fc2idx] & 0x8000))
			return 0;
	}
}

short
findtype(int pos, unsigned long type)
{
	// FIXED: tilesounds asks about the cells around the hero, off the map near its corners
#ifdef FEATURE_FAULTS
	if (pos < 0 || pos >= 22500)
		return 0;
#endif
	if (!(map[pos] & 0x1000))
		return 0;
	type &= 0xfff;
	ftfirst = map[pos] >> 16;
	for (ftidx = ftfirst; ; ftidx++) {
		if ((mapitems[ftidx] & 0xfff) == type)
			return ftidx;
		if (!(mapitems[ftidx] & 0x8000))
			return 0;
	}
}

short
listhas(int pos, unsigned long n)
{
	if (!(map[pos] & 0x1000))
		return 0;
	lhfirst = map[pos] >> 16;
	for (lhidx = lhfirst; ; lhidx++) {
		if (lhidx == n)
			return pos;
		if (!(mapitems[lhidx] & 0x8000))
			return 0;
	}
}

int
findcell(unsigned long n)
{
	unsigned i;

	for (i = 0; i < 22500; i++)
		if (listhas(i, n))
			return i;
	return 0;
}

void
delitem(int pos, unsigned long idx)
{
	delfirst = map[pos] >> 16;
	delval = mapitems[idx];
	nmapitems--;
	for (delj = idx; delj < nmapitems; delj++)
		mapitems[delj] = mapitems[delj + 1];
	for (delj = 0; delj < 22500; delj++)
		if (map[delj] & 0x1000 && map[delj] >> 16 >= idx)
			map[delj] -= 0x10000;
	if (idx == delfirst) {
		if ((delval & 0x8000) == 0)
			map[pos] &= 0xefff;
		else
			map[pos] += 0x10000;
	} else if (!(delval & 0x8000))
		mapitems[idx - 1] &= 0xffff7fff;
	updatecell(pos);
	listtorches();
}

unsigned long
takeitem(int pos, unsigned long idx)
{
	takeword = mapitems[idx];
	delitem(pos, idx);
	takeword &= 0xffff0fff;
	return takeword;
}

unsigned long
popitem(int pos, char any)
{
	if (!(map[pos] & 0x1000))
		return 0;
	popfirst = map[pos] >> 16;
	for (popidx = popfirst; ; popidx++) {
		if (any == 0 && (mapitems[popidx] & 0x4000 || mapitems[popidx] & 0x2000)) {
			if (popidx == popfirst)
				return 0;
			popidx--;
			break;
		}
		if ((mapitems[popidx] & 0x8000) == 0)
			break;
	}
	popword = mapitems[popidx];
	delitem(pos, popidx);
	popword &= 0xffff0fff;
	return popword;
}

unsigned long
realmat(int x, int y)
{
	raidx = (unsigned short)findclass(y * 150 + x, 10);
	if (raidx == 0)
		return 0;
	return idof(mapitems[raidx]);
}

int
adddot(int a, int b, int flag)
{
	for (dotslot = 0; dotslot < 30; dotslot++)
		if (dots[dotslot] == 0) {
			dots[dotslot] = a;
			dots[0x1e + dotslot] = b;
			dots[0x3c + dotslot] = 0;
			if (flag)
				dots[0x3c + dotslot] |= 1;
			return 1;
		}
	return 0;
}

void
closeborder(void)
{
	unsigned short i;

	for (i = 5; i <= 144; i++) {
		map[4 * 150 + i] &= ~0x4000;
		map[145 * 150 + i] &= ~0x4000;
	}
	for (i = 4; i <= 145; i++) {
		map[i * 150 + 5] &= ~0x4000;
		map[i * 150 + 144] &= ~0x4000;
	}
}

void
clear250(void)
{
	memset(mapblk250, 0, sizeof mapblk250);
}

void
clear2000(void)
{
	memset(mapblk2000, 0, sizeof mapblk2000);
}

void
clearrecipes(void)
{
	memset(recipes, 0, sizeof recipes);
	part1 = 0xff;
	part8 = 0xff;
}

void
initblocks(void)
{
	memset(blocks, 0, sizeof blocks);
	memset(blocks, 1, sizeof blocks);
}

void
cleardots(void)
{
	memset(dots, 0, 0x150e);
}

unsigned char
packcell(unsigned long pos)
{
	unsigned char r;

	r = 0;
	if (mapflags[pos] & 8)
		r |= 1;
	if (mapflags[pos] & 0x10)
		r |= 2;
	if (mapflags[pos] & 0x20)
		r |= 4;
	if (mapflags[pos] & 0x200)
		r |= 8;
	if (map[pos] & 0x1000)
		r |= 0x10;
	return r;
}

void
unpackcell(int pos, unsigned char f)
{
	if (f & 1)
		mapflags[pos] |= 8;
	if (f & 2)
		mapflags[pos] |= 0x10;
	if (f & 4)
		mapflags[pos] |= 0x20;
	if (f & 8)
		mapflags[pos] |= 0x200;
	if (f & 0x10)
		map[pos] |= 0x1000;
}

int
ismap02(int n)
{
	if (n >= 40)
		return 0;
	return 1;
}

int
isbasemap(int n)
{
	if (n == 1 || n == 5 || n == 3)
		return 0;
	return 1;
}

void
writeindex(HANDLE h)
{
	DWORD magic;

	magic = 0x19350e9;
	filewrite(&magic, 4, 1, h);
	filewrite(objoff, sizeof objoff, 1, h);
	filewrite(objlen, sizeof objlen, 1, h);
	filewrite(unitoff, sizeof unitoff, 1, h);
	filewrite(unitlen, sizeof unitlen, 1, h);
	magic = 0x19350e9;
	filewrite(&magic, 4, 1, h);
}

HANDLE
openrecs(void)
{
	HANDLE h;

	cdprog();
	fopenr(&h, texts + 0x9a69);
	cdsaves();
	return h;
}

int
readindex(void)
{
	long n;
	HANDLE h;

	indexok = 0;
	memset(objoff, 0, sizeof objoff);
	memset(objlen, 0, sizeof objlen);
	memset(unitoff, 0, sizeof unitoff);
	memset(unitlen, 0, sizeof unitlen);
	h = openrecs();
	if (!h)
		return 0;
	n = -1288;
	if (SetFilePointer(h, n, NULL, FILE_END) == 0xffffffff
	|| !fileread(&n, 4, 1, h) || n != 0x19350e9
	|| !fileread(objoff, sizeof objoff, 1, h)
	|| !fileread(objlen, sizeof objlen, 1, h)
	|| !fileread(unitoff, sizeof unitoff, 1, h)
	|| !fileread(unitlen, sizeof unitlen, 1, h)
	|| !fileread(&n, 4, 1, h) || n != 0x19350e9) {
		fileclose(&h);
		return 0;
	}
	fileclose(&h);
	indexok = 1;
	return 1;
}

int
readrec(void *buf, int i, int which)
{
	if (indexok == 0)
		return 0;
	rrlen = which ? unitlen[i] : objlen[i];
	if (rrlen == 0)
		return 0;
	rrfile = openrecs();
	if (rrfile == 0)
		return 0;
	rroff = which ? unitoff[i] : objoff[i];
	if (fileseek(rrfile, rroff) && fileread(&rroff, 4, 1, rrfile)
	&& rroff == rrlen && fileread(buf, rroff, 1, rrfile)) {
		fileclose(&rrfile);
		return 1;
	}
	fileclose(&rrfile);
	return 0;
}

int
readobjrec(void *buf, int i)
{
	return readrec(buf, i, 0);
}

int
readunitrec(void *buf, int i)
{
	return readrec(buf, i, 1);
}

int
basemap(int n)
{
	if (n == 1)
		return 2;
	if (n == 5)
		return 6;
	if (n == 3)
		return 4;
	return n;
}

int
loadcells(unsigned short map)
{
	int i, k;

	gridver = 0;
	trvfailmsg = 0;
	lcbase = basemap(map);
	if (ismap02(map))
		ismap02(lcbase);
	if (lcbase < 40) {
		lcword = lcbase;
		lcword += 2000;
	} else {
		lcword = lcbase - 40;
		lcword *= 2;
		lcword += 8001;
	}
	lcword = readentry(packbuf, 0x96640, lcword);
	if (lcword != 0) {
		lcword = unpack(workbuf, 0x96640, packbuf, lcword);
		if (lcword != 0)
			goto unpacked;
	}
	maploaded = 0;
	return 0;
unpacked:
	lcgrid = workbuf;
	if (lcgrid[0] != 0x21 || lcgrid[1] != 0x4d || lcgrid[2] != 0x21) {
		maploaded = 0;
		return 0;
	}
	memset(gridhead, 0, sizeof gridhead);
	memcpy(gridhead, lcgrid, sizeof gridhead);
	lcgrid += sizeof gridhead;
	for (lcidx = 0; lcidx < 22500; lcidx++) {
		::map[lcidx] = *(unsigned short *)lcgrid;
		lcgrid += 2;
	}
	gridver = *(unsigned short *)lcgrid;
	lcgrid += 2;
	if (gridver >= 0xf4f4) {
		memcpy(recipes, lcgrid, sizeof recipes);
		lcgrid += sizeof recipes;
	} else
		clearrecipes();
	if (gridver >= 0xf4f5) {
		memcpy(blocks, lcgrid, sizeof blocks);
		lcgrid += sizeof blocks;
	} else
		initblocks();
	if (gridver >= 0xf4f6) {
		memcpy(dots, lcgrid, 0x150e);
		lcgrid += 0x150e;
	} else
		cleardots();
	clearprovs();
	if (gridver >= 0xf4f7) {
		memread(&nsites, 2, 1, &lcgrid);
		memread(provmap, sizeof provmap, 1, &lcgrid);
		if (nsites >= 100)
			nsites = 100;
		memread(provs, 0x164, nsites, &lcgrid);
	}
	if (!visited(map)) {
		enteritems = 0xff;
		entersum = 1;
		lclen = readobjrec(packbuf, basemap(map));
		if (lclen)
			goto loaded;
	}
	wsprintf(lcname, texts + 0xa442, map);
	fopenr(&lcfile, lcname);
	if (lcfile == 0) {
		maploaded = 0;
		return 0;
	}
	lclen = fileread(packbuf, 1, 0x96640, lcfile);
	fileclose(&lcfile);
loaded:
	memset(workbuf, 0, 0x96640);
	if (*(DWORD *)packbuf == 0xf4f9f4f9)
		lclen = unpack(workbuf, 0x96640, packbuf + 4, lclen - 4);
	else
		lclen = unlzw((unsigned char *)workbuf, (unsigned char *)packbuf, lclen);
	if (lclen <= 0) {
		maploaded = 0;
		return 0;
	}
	lcrec = memopen(workbuf);
	nmapitems = 1;
	memset(mapitems, 0, sizeof mapitems);
	memset(mapflags, 0, sizeof mapflags);
	for (lcidx = 0; lcidx < 22500; lcidx++) {
		memread(&lcflags, 1, 1, &lcrec);
		unpackcell(lcidx, lcflags);
		if (::map[lcidx] & 0x1000) {
			lcword = nmapitems;
			lcword <<= 16;
			::map[lcidx] |= lcword;
			do {
				memread(&lcword, 4, 1, &lcrec);
				if ((lcword & 0xfff) == 0) {
					if (!(lcword & 0x8000)) {
						if (::map[lcidx] >> 16 < nmapitems)
							mapitems[nmapitems - 1] &= 0xffff7fff;
						else
							::map[lcidx] &= 0xefff;
					}
				} else
					mapitems[nmapitems++] = lcword;
			} while (lcword & 0x8000);
		}
		updatecell(lcidx);
	}
	lcidx = 0;
	lcunused = 0;
	if (memread(&lcidx, 2, 1, &lcrec) < 1u)
		;
	else {
		memread(mapstate, sizeof mapstate, 1, &lcrec);
		memread(mapblk250, sizeof mapblk250, 1, &lcrec);
		memread(mapblk2000, sizeof mapblk2000, 1, &lcrec);
		clearmarks();
		memread(&lcidx, 2, 1, &lcrec);
		if (lcidx == 0x6b9)
			lcrec = readmarks(lcrec);
	}
	listtorches();
	closeborder();
	if (avengers == -1 && map != 1 && map != 2 && map != 5 && map != 6)
		clearavenge();
	if ((map == 1 || map == 2 || map == 5 || map == 6) && avengers)
		for (i = 0; i < 22; i++) {
			((Troop *)(recipes + 0x20))[i].parts[0] = 0;
			if (((Troop *)(recipes + 0x20))[i].type != 0x1a && ((Troop *)(recipes + 0x20))[i].type != 0x1f)
				((Troop *)(recipes + 0x20))[i].type = 0x1e;
			for (k = 0; k < 5; k++)
				if (partdefs[((Troop *)(recipes + 0x20))[i].parts[k]].layer[0] == 2) {
					((Troop *)(recipes + 0x20))[i].parts[k] = randn(100) < 80 ? 0x20 : 0x1f;
					goto found;
				}
			for (k = 1; k < 5; k++)
				if (((Troop *)(recipes + 0x20))[i].parts[k] == 0) {
					((Troop *)(recipes + 0x20))[i].parts[k] = randn(100) < 80 ? 0x20 : 0x1f;
					break;
				}
		found:
			if (((Troop *)(recipes + 0x20))[i].parts[2] == 0 && ((Troop *)(recipes + 0x20))[i].parts[3] != 0x20 && ((Troop *)(recipes + 0x20))[i].parts[3] != 0x1f)
				((Troop *)(recipes + 0x20))[i].parts[2] = 0x1c;
			((Troop *)(recipes + 0x20))[i].parts[4] = randn(100) < 66 ? 0x31 : 0x30;
		}
	maploaded = 1;
	return 1;
}

int
savegrid(unsigned short map, char flag)
{
	char buf[16];
	HANDLE h;
	unsigned short i;
	unsigned short v;

	if (!isbasemap(map))
		return 1;
	wsprintf(buf, texts + 0xa4c3, map);
	if (flag && fopenr(&h, buf)) {
		fileclose(&h);
		return 0;
	}
	fopenw(&h, buf);
	if (h == 0)
		return 0;
	memset(gridhead, 0, sizeof gridhead);
	wsprintf(gridhead, texts + 0xb1d8, map);
	filewrite(gridhead, sizeof gridhead, 1, h);
	for (i = 0; i < 22500; i++) {
		v = ::map[i] & 0xfff;
		filewrite(&v, 2, 1, h);
	}
	i = 0xf4f9;
	filewrite(&i, 2, 1, h);
	filewrite(recipes, sizeof recipes, 1, h);
	filewrite(blocks, sizeof blocks, 1, h);
	filewrite(dots, 0x150e, 1, h);
	if (nsites >= 100)
		nsites = 100;
	filewrite(&nsites, 2, 1, h);
	filewrite(provmap, sizeof provmap, 1, h);
	filewrite(provs, 0x164, nsites, h);
	i = 1234;
	filewrite(&i, 2, 1, h);
	fileclose(&h);
	return 1;
}

int
saveobjs(unsigned short map, char flag)
{
	char buf[16];
	HANDLE h;
	DWORD v;
	char *p;
	unsigned short i;
	unsigned char c;
	unsigned short k;

	wsprintf(buf, texts + 0xa442, map);
	if (flag) {
		fopenr(&h, buf);
		if (h) {
			fileclose(&h);
			return 0;
		}
	}
	fopenw(&h, buf);
	if (h == 0)
		return 0;
	v = 0xf4f9f4f9;
	filewrite(&v, 4, 1, h);
	memset(workbuf, 0, 0x96640);
	p = memopen(workbuf);
	for (i = 0; i < 22500; i++) {
		v = ::map[i];
		c = packcell(i);
		memwrite(&c, 1, 1, &p);
		if (v & 0x1000) {
			k = v >> 16;
			do
				memwrite(&mapitems[k], 4, 1, &p);
			while (mapitems[k++] & 0x8000);
		}
	}
	(*(unsigned long *)(mapstate + 0x27)) = gametime;
	i = 0xf0f0;
	memwrite(&i, 2, 1, &p);
	memwrite(mapstate, sizeof mapstate, 1, &p);
	memwrite(mapblk250, sizeof mapblk250, 1, &p);
	memwrite(mapblk2000, sizeof mapblk2000, 1, &p);
	i = 0x6b9;
	memwrite(&i, 2, 1, &p);
	p = writemarks(p);
	i = 22500;
	memwrite(&i, 2, 1, &p);
	c = 0;
	memwrite(&c, 1, 1, &p);
	v = p - workbuf;
	v = pack(packbuf, 0x96640, workbuf, v);
	filewrite(packbuf, v, 1, h);
	fileclose(&h);
	return 1;
}

short
copymap(unsigned short dst, unsigned short src)
{
	char buf[16];
	HANDLE h;
	DWORD n;

	wsprintf(buf, texts + 0xa442, src);
	if (!fopenr(&h, buf))
		return 0;
	n = fileread(workbuf, 1, 0x96640, h);
	fileclose(&h);
	wsprintf(buf, texts + 0xa442, dst);
	if (!fopenw(&h, buf))
		return 0;
	filewrite(workbuf, n, 1, h);
	fileclose(&h);
	wsprintf(buf, texts + 0xa44f, src);
	if (fopenr(&h, buf)) {
		n = fileread(workbuf, 1, 0x96640, h);
		fileclose(&h);
	} else
		n = 0;
	wsprintf(buf, texts + 0xa44f, dst);
	if (!fopenw(&h, buf))
		return 0;
	filewrite(workbuf, n, 1, h);
	fileclose(&h);
	return 1;
}

int playerx = 8;
int playery = 6;
int targetx = 0x3e7;
int targety = 0x3e7;
unsigned char notedkind = 255;
unsigned char lastkind = 255;
int notedposx = 255;
int notedposy = 255;
unsigned long noteditem = 255;
char empty[1] = "";
unsigned short hourticks = 200;
unsigned char gamehour = 15;
unsigned char hourunused[8] = {
	199, 199, 199, 80, 107, 91, 37, 119
};
char maskedtext[4] = "";
short waterpace = 4;
unsigned short walkdelay = 11;
unsigned short standdelay = 24;
unsigned char optunused = 1;
unsigned char cmbautooff = 2;
unsigned char gaugepeace = 2;
unsigned char gaugecombat = 3;
unsigned char tallcombat = 1;
unsigned char gaugepeace2 = 2;
unsigned char gaugecombat2 = 3;
unsigned char tallcombat2 = 1;
unsigned char mode2icon = 1;
short sfxon = 1;
int playmode = 1;
short popupdelay = 200;
char quitprompt = 1;
short part1 = 255;
short part8 = 255;
short partunused = 1;
unsigned short nspots = 1;
Metal metals[30] = {
	19, 0, 0, 0, 100, 0, 0, 0, 1, 10, -1, 127, 100, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 15, 0, 70, 0, 105, 0, 95, 0, 25, 40, 25, 0, 80, 4, 0, 3, 0,
	1, 0, 1, 0, 2, 10, 18, 0, 105, 0, 80, 0, 65, 0, 10, 30, 30, 0, 90, 3,
	0, 1, 0, 0, 0, 4, 0, 1, 30, 18, 0, 100, 0, 90, 0, 75, 0, 5, 20, 25, 0,
	100, 3, 0, 1, 0, 0, 0, 4, 0, 1, 5, 15, 0, 0, 0, 90, 0, 80, 0, 10, 30,
	30, 0, 80, 1, 0, 2, 0, 1, 0, 2, 0, 2, 10, 15, 0, 70, 0, 105, 0, 95, 0,
	25, 40, 30, 0, 80, 4, 0, 3, 0, 1, 0, 1, 0, 2, 10, 17, 0, 100, 0, 95,
	0, 70, 0, 10, 40, 35, 0, 90, 2, 0, 2, 0, 1, 0, 3, 0, 2, 20, 15, 0,
	100, 0, 95, 0, 80, 0, 10, 45, 25, 0, 80, 4, 0, 3, 0, 1, 0, 1, 0, 2,
	10, 17, 0, 105, 0, 85, 0, 70, 0, 5, 50, 40, 0, 90, 2, 0, 2, 0, 1, 0,
	3, 0, 2, 20, 15, 0, 100, 0, 95, 0, 80, 0, 10, 45, 30, 0, 80, 4, 0, 3,
	0, 1, 0, 1, 0, 2, 10, 15, 0, 110, 0, 90, 0, 60, 0, 5, 55, 90, 0, 80,
	2, 0, 3, 0, 1, 0, 1, 0, 2, 10, 15, 0, 110, 0, 90, 0, 60, 0, 5, 55, 90,
	0, 80, 2, 0, 3, 0, 1, 0, 1, 0, 2, 10, 18, 0, 120, 0, 80, 0, 40, 0, 5,
	65, 90, 0, 100, 3, 0, 2, 0, 1, 0, 3, 0, 2, 30, 17, 0, 110, 0, 80, 0,
	40, 0, 5, 70, 90, 0, 90, 2, 0, 2, 0, 1, 0, 3, 0, 2, 20, 17, 0, 105, 0,
	75, 0, 40, 0, 5, 50, 45, 0, 90, 1, 0, 2, 0, 1, 0, 3, 0, 2, 20, 16, 0,
	80, 0, 95, 0, 50, 0, 10, 40, 35, 0, 90, 1, 0, 1, 0, 1, 0, 5, 1, 1, 20,
	16, 0, 80, 0, 95, 0, 70, 0, 10, 30, 90, 0, 90, 2, 0, 2, 0, 1, 0, 3, 1,
	2, 20, 16, 0, 80, 0, 75, 0, 50, 0, 10, 40, 35, 0, 90, 1, 0, 1, 0, 1,
	0, 5, 1, 1, 20
};
Gear armors[30] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 20, 0, 90, 0, 0, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 0, 20, 10, 10,
	10, 20, 0, 70, 0, 0, 0, 0, 1, 0, 100, 0, 1, 1, 0, 2, 0, 0, 20, 20, 20,
	20, 25, 0, 70, 0, 0, 0, 0, 3, 0, 100, 0, 2, 7, 0, 1, 0, 2, 40, 5, 5,
	5, 20, 0, 70, 0, 0, 0, 0, 0, 0, 100, 0, 1, 1, 0, 2, 0, 0, 20, 10, 10,
	10, 25, 0, 70, 0, 0, 0, 0, 2, 0, 100, 0, 2, 4, 0, 1, 0, 2, 40, 10, 10,
	10, 15, 0, 70, 0, 0, 0, 0, 2, 0, 100, 0, 1, 1, 0, 2, 0, 0, 20, 20, 20,
	20, 30, 0, 80, 0, 0, 0, 0, 4, 0, 100, 0, 3, 2, 0, 1, 0, 2, 30, 22, 22,
	22, 30, 0, 80, 0, 0, 0, 0, 5, 0, 100, 0, 3, 2, 0, 1, 0, 2, 30, 20, 20,
	20, 20, 0, 60, 0, 0, 0, 0, 5, 0, 100, 0, 3, 2, 0, 1, 0, 2, 30, 20, 20,
	20, 20, 0, 60, 0, 0, 0, 0, 4, 0, 100, 0, 3, 2, 0, 1, 0, 2, 30, 20, 20,
	20, 20, 0, 80, 0, 0, 0, 0, 4, 0, 100, 0, 3, 2, 0, 1, 0, 2, 30, 2, 2,
	2, 10, 0, 90, 0, 0, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 0, 20, 8, 8, 8,
	12, 0, 70, 0, 0, 0, 0, 1, 0, 100, 0, 1, 2, 0, 0, 0, 0, 20, 10, 10, 10,
	15, 0, 70, 0, 0, 0, 0, 2, 0, 100, 0, 1, 2, 0, 2, 0, 0, 20, 20, 25, 25,
	25, 0, 70, 0, 0, 0, 0, 5, 0, 100, 0, 2, 10, 0, 1, 0, 2, 40, 17, 17,
	17, 20, 0, 70, 0, 0, 0, 0, 6, 0, 100, 0, 3, 6, 0, 1, 0, 2, 40, 25, 25,
	25, 10, 0, 70, 0, 0, 0, 0, 7, 0, 100, 0, 3, 6, 0, 1, 0, 2, 30, 10, 10,
	10, 10, 0, 80, 0, 0, 0, 0, 4, 0, 110, 0, 3, 2, 0, 2, 0, 1, 20, 15, 15,
	15, 15, 0, 80, 0, 0, 0, 0, 4, 0, 120, 0, 3, 2, 0, 2, 0, 1, 20, 20, 20,
	20, 15, 0, 80, 0, 0, 0, 0, 7, 0, -106, 0, 3, 2, 0, 2, 0, 1, 20, 15,
	15, 15, 20, 0, 80, 0, 0, 0, 0, 4, 0, 100, 0, 3, 2, 0, 2, 0, 1, 20, 2,
	2, 2, 20, 0, 90, 0, 0, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 0, 20, 10,
	10, 10, 20, 0, 70, 0, 0, 0, 0, 2, 0, 100, 0, 1, 2, 0, 2, 0, 0, 20, 27,
	27, 27, 20, 0, 80, 0, 0, 0, 0, 10, 0, 100, 0, 3, 6, 0, 1, 0, 2, 20,
	20, 20, 20, 20, 0, 60, 0, 0, 0, 0, 5, 0, 100, 0, 3, 2, 0, 1, 0, 2, 30,
	3, 3, 3, 20, 0, 90, 0, 0, 0, 0, 1, 0, 100, 0, 1, 1, 0, 0, 0, 0, 20
};
short armorsunused[114] = {
	0, 0, -4, -3, -3, -4, 7, -3, -3, -3, -3, 6, -3, -2, -2, -3, 5, -2, -2,
	-2, -2, 4, -2, -1, -1, -2, 3, -1, -1, -1, -1, 2, -1, 0, 0, -1, 1, 0,
	0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 2, 3, 2, 2, 2, 2,
	4, 3, 2, 2, 3, 5, 3, 3, 3, 3, 6, 4, 3, 3, 4, 7, 4, 4, 4, 4, 8, -7, -7,
	-6, -6, -5, -5, -4, -4, -3, -3, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2, 3,
	3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8
};
int sathours;
unsigned long watertype;
int deepkind;
unsigned long icpos;
unsigned long icidx;
unsigned long topidx;
unsigned long topfirst;
unsigned long blessbits;
unsigned long wear16bits;
unsigned long wearcount;
unsigned char repcls;
unsigned long alttype;
unsigned long basetype;
unsigned long easttype;
unsigned long southtype;
unsigned long randtype;
unsigned long brktype;
unsigned long unbrktype;
unsigned char repdcls;
unsigned long armorv;
short dmggear;
short dmgpct;
unsigned char stackcls;
unsigned long coinsum;
unsigned char fixcls;
unsigned long littype;
unsigned long *smokep;
unsigned long smoketype;
unsigned long burnleft;
unsigned long foodname;
unsigned long filltype;
unsigned long fillsrc;
unsigned long *putcell;
unsigned long *putp;
unsigned long valpct;
unsigned long valsum;
unsigned long valk;
unsigned char valcls;
unsigned long rndprice;
unsigned long rndrem;
unsigned long rcitem;
int rccost;
unsigned short ltidx;
unsigned long *ltp;
short wmidx;
short wmlen;
unsigned char wmtag;
char *wmstart;
short rmidx;
short rmword;
unsigned char rmtag;
int mkidx;
unsigned long mkfirst;
unsigned long mkitem;
int nmspot;
unsigned long ucidx;
unsigned long ucfirst;
unsigned char uccls;
short ucflags;
unsigned long addidx;
unsigned long addfirst;
unsigned long addj;
unsigned long fcidx;
unsigned long fcfirst;
unsigned long fc2idx;
unsigned long fc2first;
unsigned char fc2cls;
unsigned long ftidx;
unsigned long ftfirst;
unsigned long lhidx;
unsigned long lhfirst;
unsigned long delval;
unsigned long delj;
unsigned long delfirst;
unsigned long takeword;
unsigned long popidx;
unsigned long popfirst;
unsigned long popword;
unsigned long raidx;
int dotslot;
HANDLE rrfile;
DWORD rroff;
DWORD rrlen;
HANDLE lcfile;
char lcname[16];
unsigned short lcidx;
short lcunused;
DWORD lcword;
unsigned char lcflags;
char *lcgrid;
DWORD lclen;
char *lcrec;
unsigned short lcbase;
unsigned short playercell;
unsigned short playernext;
unsigned short curmap;
unsigned char targetkind;
unsigned long targetitem;
int targetidx;
unsigned char targetcls;
short targetowner;
unsigned short notedunit;
short notedmap;
unsigned long notedtime;
short askedonce;
short askedunit;
unsigned long map[22500];
unsigned long mapflags[22500];
unsigned short nmapitems;
unsigned long mapitems[32767];
unsigned short cellflags[4096];
unsigned char terrains[4096];
unsigned short itemflags[4096];
unsigned short weights[4096];
unsigned char classes[4096];
unsigned char typenames[4096];
unsigned short volumes[4096];
unsigned short prices[4096];
unsigned long tradegoods[24];
unsigned long tradeoffer[24];
char picked[24];
unsigned short goodsrealm[24];
unsigned short goodsspot[24];
unsigned long execrc;
DWORD goodsprice;
DWORD offerprice;
DWORD goodsbase;
DWORD offerbase;
DWORD nforsale;
DWORD pricemarkup;
short dealok;
short repairmat;
unsigned long gametime;
int turncount;
short gameday;
char talkactive;
char lastspeech;
unsigned char indoinput;
unsigned short busyturns;
unsigned char savebyte1;
short gtunused;
unsigned char gtdepth;
short maintick;
DWORD walkdue;
DWORD standdue;
char lookstale;
short scrolling;
short heropace;
char mapframed;
char knockedout;
unsigned char passmode;
char unitsrun;
short cfgword1;
char tallpeace;
char iconunused;
char mode3icon;
char turnkeys;
char lbltext[60];
short lblx;
short lbly;
short skiptick;
short toggling;
short inautocombat;
short foeunit;
short combatkind;
short actionturn;
int walkunused;
short walkedsum;
short walkeddx;
short walkeddy;
char heldcmd;
char cmdunused;
int cmdunused2[3];
int tipanchorx;
int tipanchory;
unsigned walkturns;
unsigned char firststart;
unsigned char enterflag;
unsigned short enteritems;
int entersum;
char vertext[80];
short saveword1;
short saveword2;
short saveword3;
short saveword4;
short saveword5;
short saveword6;
short saveword7;
short saveword8;
short saveword9;
int hourtime;
char savebyte3;
char mapunused1;
short mapunused2;
int mapunused3[7];
unsigned char faceflags[288];
char maskedlic[80];
int cachebytes;
char partscached;
char facescached;
char overscached;
char cacheunused;
int cacheunused2;
DWORD partbytes;
DWORD facebytes;
DWORD overbytes;
DWORD travelbytes;
int sfxbytes;
int musicbytes;
char scriptfile[1000];
char savefail;
char newcfg;
char readonlydir;
char flagunused;
int flagunused2;
int cheat;
int restbreak;
int lostcontrol;
DWORD aboutticks;
short starttime;
short startunused;
short startunused2;
short nstarts;
int playtime;
short censorship;
unsigned short gridver;
char mapstate[100];
char mapblk250[250];
char mapblk2000[2000];
char recipes[1488];
unsigned char blocks[30][30];
unsigned char dots[5390];
char gridhead[25];
int maploaded;
char pics[104][2304];
unsigned char sqrttab[46225];
unsigned long satitem;
Realm realms[125];
unsigned char nrealms;
Spot spots[128];
char spotsong;
short spotday;
short curregion;
Region regions[255];
Points points[255];
short nregions;
unsigned char regionmap[22500];
Prov provs[100];
short nsites;
unsigned char provmap[22500];
int torchunused;
short torches[100];
unsigned short ntorches;
short nmarks;
int marksunused;
char marks[23][20];
short markx[23];
short marky[23];
int indexok;
long objoff[80];
DWORD objlen[80];
long unitoff[80];
DWORD unitlen[80];
char trvfailmsg;

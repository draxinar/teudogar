/*
 * The data files, a module of teudogar.exe (004210e0 to 004216a0).
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
	char f3ba[0x5f8 - 0x3ba];
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
// FIXED: as at its definition in texts.cpp, itembuf's room for every language's names
#ifdef FEATURE_FAULTS
extern char itembuf[64];
#else
extern char itembuf[32];
#endif
extern char yourbuf[40];

/* the offset of *id in [base, base + 6), *id set to base, else -1 */
int
langof(int *id, int base)
{
	int n;

	if (base > *id || base + 6 <= *id)
		return -1;
	n = *id - base;
	*id -= n;
	return n;
}

/* the language variant of resource id, English where a language lacks it */
int
langentry(int id)
{
	int n;

	n = langof(&id, 12);
	if (n >= 0) {
		if (n != 1 && n != 0 && n != 4)
			n = 1;
		return id + n;
	}
	n = langof(&id, 0x200d);
	if (n >= 0) {
		if (n != 1 && n != 0 && n != 4)
			n = 1;
		return id + n;
	}
	n = langof(&id, 0x138b);
	if (n >= 0) {
		if (n != 1 && n != 0 && n != 4)
			n = 1;
		return id + n;
	}
	n = langof(&id, 0x1391);
	if (n >= 0) {
		if (n != 1 && n != 0 && n != 4)
			n = 1;
		return id + n;
	}
	n = langof(&id, 0);
	if (n >= 0) {
		if (n != 1 && n != 0)
			n = 1;
		return id + n;
	}
	n = langof(&id, 6);
	if (n >= 0) {
		if (n != 1 && n != 0)
			n = 1;
		return id + n;
	}
	n = langof(&id, 0x1faa);
	if (n >= 0) {
		if (n != 1 && n != 0)
			n = 1;
		return id + n;
	}
	n = langof(&id, 0x1fb1);
	if (n >= 0) {
		if (n != 1 && n != 0)
			n = 1;
		return id + n;
	}
	n = langof(&id, 0x2095);
	if (n >= 0) {
		if (n != 1 && n != 0 && n != 4)
			n = 1;
		return id + n;
	}
	return id;
}

/* close the data file */
void
closedat(void)
{
	if (datfh)
		fileclose(&datfh);
	datfh = 0;
	storedn = 0;
	decodedn = 0;
	curdat = -1;
}

/* empty the entry table of data file i */
void
clearrow(int i)
{
	nentries[i] = 0;
	memset(entryoffs[i], 1, 2000);
	memset(entrysizes[i], 1, 2000);
}

/* close the data file and empty the table of file 1 */
int
inidat(void)
{
	closedat();
	idok = 1;
	idfile = 0;
	while (++idfile < 9)
		if (idfile == 1)
			clearrow(idfile);
	return idok;
}

/* enter the directory savesdir, 1 on success */
int
cdsaves(void)
{
	if (setdir(savesdir))
		return 0;
	return 1;
}

/* enter the directory mapsdir, 1 on success */
int
cdmaps(void)
{
	if (setdir(mapsdir))
		return 0;
	return 1;
}

/* enter the program's directory, 1 on success */
int
cdprog(void)
{
	if (setdir(progdir))
		return 0;
	return 1;
}

/* open data file name in the program's directory */
int
// PORT: a name for the unused parameter, which C99 wants
#ifdef PORT
opendat(char *name, int unused)
#else
opendat(char *name, int)
#endif
{
	pathjoin(datpath, progdir, name);
	if (datfh)
		closedat();
	return odok = fopenr(&datfh, datpath);
}

/* split id into file and entry, 1 when the entry exists */
int
splitid(int id)
{
	sifile = id / 1000;
	if (sifile >= 9 || sifile == 1)
		return 0;
	sientry = id - sifile * 1000;
	if (sientry >= nentries[sifile])
		return 0;
	return 1;
}

/* the packed size of entry id, 0 when missing */
int
storedlen(int id)
{
	if (!splitid(id))
		return 0;
	if (sientry >= nentries[sifile])
		return 0;
	return entryoffs[sifile][sientry + 1] - entryoffs[sifile][sientry];
}

/* the unpacked size of entry id, 0 when missing */
int
decodedlen(int id)
{
	if (!splitid(id))
		return 0;
	if (sientry >= nentries[sifile])
		return 0;
	return entrysizes[sifile][sientry + 1] - entrysizes[sifile][sientry];
}

/* the offset of entry id in its file, 0 when missing */
int
entryoff(int id)
{
	if (!splitid(id))
		return 0;
	if (sientry >= nentries[sifile])
		return 0;
	return entryoffs[sifile][sientry];
}

/* open the data file of entry id at the entry */
int
seekentry(int id)
{
	if (!splitid(id))
		return 0;
	if (sientry >= nentries[sifile])
		goto fail;
	if (sifile != 8)
		wsprintf(datname, texts + 0x9ad8, sifile);
	else
		wsprintf(datname, texts + 0x9b62);
	sefile = sifile;
	sestart = entryoffs[sifile][sientry];
	seend = entryoffs[sifile][sientry + 1];
	decodedn = entrysizes[sifile][sientry + 1] - entrysizes[sifile][sientry];
	storedn = seend - sestart;
	if (datfh) {
		if (sefile == curdat)
			goto seek;
		fileclose(&datfh);
	}
	if (opendat(datname, 0) == 0)
		goto fail;
	curdat = sefile;
seek:
	if (!fileseek(datfh, sestart))
		goto fail;
	return 1;
fail:
	datfh = 0;
	storedn = 0;
	decodedn = 0;
	curdat = 0;
	return 0;
}

/* read entry id into buf when it fits in size bytes */
DWORD
readentry(void *buf, DWORD size, int id)
{
	DWORD n;

	n = storedlen(id);
	if (n == 0)
		return 0;
	if (n > size)
		return 0;
	if (!seekentry(id))
		return 0;
	if (!fileread(buf, n, 1, datfh))
		return 0;
	return n;
}

int curdat = -1;
int idfile;
int idok;
int odok;
DWORD sestart;
DWORD seend;
int sefile;
char workbuf[616000];
char packbuf[616000];
char picbuf[308200];
char loadbuf[614400];
DWORD sheetlen;
unsigned char loadpack[614400];
DWORD sheetread;
int nentries[9];
DWORD entryoffs[9][500];
DWORD entrysizes[9][500];
HANDLE datfh;
DWORD storedn;
DWORD decodedn;
char datpath[1000];
char datname[32];
char progdir[1000];
char savesdir[1000];
char mapsdir[1000];
int sifile;
int sientry;

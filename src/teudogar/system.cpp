#include <windows.h>
#include <shlobj.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

typedef struct File File;
typedef struct Reg Reg;
typedef struct Dataoff Dataoff;
typedef struct Realm Realm;

/* the files checked at start, 12 bytes each (004c35e8) */
struct File {
	char *name;
	unsigned long f4;
	unsigned char state;	/* 0 good, 1 missing, 2 changed */
	unsigned char f9;
	unsigned char kind;	/* 2: left out */
	unsigned char scan;	/* summed by scanexe, else by crcname */
};

/* the block at 004c3938 that holds the size of the data for the
   progress bar and the version that getver returns */
struct Dataoff {
	char marker[16];	/* "DataOffset::1234" */
	int total;
	char f14[4];
	DWORD ver[3];
	char vername[8];
	DWORD build;
	char f30[0x190 - 0x30];
	char vername2[8];	/* used when it does not start with '?' */
	char f198[8];
};

/* 0x158 bytes each (005a1b78) */
struct Realm {
	unsigned long item;
	char f4[0x158 - 4];
};

/* the registration as the registry holds it, 200 bytes (009a66d8) */
struct Reg {
	char oldmajor;
	unsigned short oldminor;
	char f3[0x64 - 3];
	unsigned char key[100];
};

extern char texts[];

#define T(off) (texts + (off))

DWORD availmem(void);
unsigned long crcname(char *name, int skip, int end);
LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
char *gametitle(void);
void encodenum(char *buf, unsigned long n);
int isdebug(void);

extern MEMORYSTATUS memstat;
extern int crclen;
extern char packbuf[];
extern unsigned char lic[72];
extern unsigned char licblock[100];

extern int lowmem;
extern int licenseok;
extern int licseal;
extern HWND mainwnd;
extern HACCEL accel;
extern HINSTANCE appinst;

extern HANDLE scanfh;
extern DWORD scanpos;
extern DWORD sc[6];

extern int chkpending;
extern int licpending;
extern File files[30];
extern unsigned long crcs[30];
extern unsigned long sizes[30];
extern unsigned long minsize;

extern char maskedlic[80];
extern char namebuf[2500];

extern char exedir[1000];
extern char exepathbuf[1000];
extern char progdir[];
extern char tmpbuf[2500];
extern char winpath[2000];
extern char uninstpath[1000];
extern DWORD regmajor;
extern DWORD regminor;
extern DWORD regaddinfo;
extern char regapppath[1000];
extern char regsaves[1000];
extern char regmenu[1008];
extern HKEY hkey;
extern int hasregver;
extern int hasreglang;
extern char regvalbuf[200];
extern int regvalflag;
extern int hasregsaves;
extern Reg reg;
extern int hasregdata;
extern DWORD licagreed;
extern int startcancel;
extern int startpending;
extern int syslang;
extern HANDLE mutex;
extern int instfound;
extern int instfull;
extern HWND dlg;
extern MSG pumpmsg;
extern char dlgtext[4000];
extern char *logp;
extern int textid;
extern int focusdef;
extern int focusid;
extern char *dlgtitle;
extern int progpumps;
extern int defid;
/* the licensee's name and address, inverted (004c3858) */
extern char licensee[201];
extern char errlines[4000];
extern char errboxtext[4000];
extern char *browsestart;
extern char *months[];
extern int setuppage;
extern int installing;
extern int aborted;
extern int asksaves;
extern int instnewer;
extern char linktarget[1000];
extern char instdir[2000];
extern char instsaves[2000];
extern char instmenu[1000];
extern char linkname[1000];
extern char setupbuf[3000];
extern DWORD verminor;
extern DWORD verflags;
extern FILETIME buildtime;
extern DWORD buildno;
extern FILETIME insttime;
extern DWORD instsize;
extern int bytesdone;
extern HGLOBAL fdata_buf2;
extern HGLOBAL fdata_buf;
extern char logbuf[20484];
extern int (*logcb)(char *, char *);
extern char *loghead;
extern int lognl;
extern int logshow;
extern char savesdir[];
extern char empty[1];
extern int lang;
extern DWORD vermajor;
extern Dataoff dataoff;
/* the setup dialog's place and size (009aeb98) */
extern int winx;
extern int winy;
extern int winw;
extern int winh;
extern char *logcur;
extern int logpos;
void chooselang(char c);

typedef struct Part Part;

/* 0x22 bytes each (004c6f48, unittype.cpp's partdefs) */
struct Part {
	char noun[2];
	unsigned char layer;
	unsigned char setkind;
	char f4[0x11 - 4];
	unsigned long size;	/* of pics */
	// PORT: partpics holds the pointer, which 64 bits widen past these 4 bytes
#ifdef PORT
	char pics[4];
#else
	char *pics;
#endif
	char f19[0x1d - 0x19];
	unsigned char setflag;
	char f1e[0x22 - 0x1e];
};

extern Part partdefs[];
// PORT: the picture sets of partdefs, in unittype.cpp
#ifdef PORT
extern char *partpics[78];
#endif
extern unsigned char partkinds[][2];
extern char version[23];	/* 004c3b40, "@#$(!): 01.005 \xff      " */
extern short starttime;
extern int hour;
extern int minute;
extern char mapsdir[];
extern char maskedtext[];
extern short sfxon;
extern short (*inkeyfn)(char c);
extern void (*infieldfn)(short field);
extern void (*insetfn)(short field, char **s, short *max);
extern short initdone;
extern unsigned char *facecache[57];
extern unsigned char *overcache[57];
extern int leftclick;
extern int rightclick;
extern int minmem;
extern int nopani;
extern int nopequip;
extern int nopvari;
extern int noprun;
extern char lastscan;
extern unsigned char esckey;
extern char vertext[];
extern char licenseno[80];
extern char cfgtext1[52];
extern char cfgtext2[52];
extern char cfgtext3[52];
extern char cfgtext4[52];
extern char cfgtext5[52];
extern char cfgtext6[52];
extern char cfgtext7[60];
extern char optg[];
extern char scriptfile[];
extern char strfile[1000];
extern int cheat;
extern int titlesopt;
extern int nofani;
extern unsigned char firststart;
// PORT: a name of its own, which C wants for an overloaded function
#ifdef PORT
int unpackblock(void);
#else
int unpack(void);
#endif
void setlang(char c);
int cdprog(void);
void kbdinit(void);
int mouseinit(void);
void initrects(void);
void clearbufs(void);
int loadudta(void);
int inidat(void);
int loadtables(void);
int readindex(void);
int loadintro(void);
int music(int n, int lp);
int loadscr(int id, unsigned char *dst, unsigned char *pal, void *buf, DWORD size);
void titlecolors(unsigned char *p, int flag);
void usepal(unsigned char n);
void keepui(char *pal);
extern char scrbuf[309700];
extern char workbuf[];
extern char gamepal[768];
int initsound(void);
extern char dsready;
extern char dsmsg[304];
extern char dsshort[32];
int savegame(void);
int saveobjs(unsigned short map, char flag);
int savemap(unsigned short map);
short uptime(void);
int savecfg(void);
DWORD copyfile(char *src, char *dst, char *buf, DWORD size);
void hidecursor(void);
void unclip(void);
int ddquit(void);
void freesound(void);
int freesongs(void);
int unloadintro(void);
void closedat(void);
int tilefree(void);
int freebank(void);
extern int maploaded;
extern unsigned short curmap;
extern int playtime;
extern int copymode;
extern int modeset;
extern HGLOBAL travel0;
extern HGLOBAL travel1;
extern HGLOBAL travel2;
extern HGLOBAL travel3;
extern unsigned long runminutes;
extern int gameready;
extern int savesready;
extern char savefail;
extern char readonlydir;
extern char printbuf[];
int cdsaves(void);
short loadcfg(void);
void restoremodes(void);
void defaultcfg(void);
void adjpal(int bright, int contrast, int r, int g, int b);
extern int optbright;
extern int optcontrast;
extern int optred;
extern int optgreen;
extern int optblue;
extern char newcfg;
extern int udtaok;
void setbw(char *pal);
void initpals(void);
extern unsigned char defpal[768];
extern char banktile[0x900];
extern int talkok;
extern int memguard4;
extern int memguard5;
extern unsigned short lastslot;
extern short gearmsg;
extern int memguard6;

extern char *optword;
extern char *optscan;
extern char *errstart;
extern int optlast;
extern int memguard3;
extern int memguard2;
extern int memguard1;
extern int isi;
extern int isj;
extern short setuptries;
extern int quickstart;
extern int udtaloaded;
extern int newgameopt;
extern int filecheck;
extern int soundoff;
extern int musicoff;
extern int startlevel;
extern int noprinter;
extern int featuresoff;
char *pick(char *de, char *en, char *fr, char *es, char *it, char *pl);

extern int cachelevel;
extern int i2i;
extern int cacheparts;
extern DWORD i2len;
extern DWORD titlehold;
extern DWORD titletick;
extern int wrongcopy;
extern int quitting;
extern int playmode;
extern unsigned char menumode;
extern int afterupdate;
extern int afterinst;
extern Realm realms[125];
extern char keycode;
extern unsigned char keys[256];
extern unsigned long execrc;
extern char partscached;
extern char facescached;
extern char overscached;
extern int cachebytes;
extern DWORD partbytes;
extern DWORD facebytes;
extern DWORD overbytes;
extern DWORD travelbytes;
extern DWORD storedn;
extern HANDLE datfh;
extern unsigned long crcword;
extern DWORD facesizes[57];
extern DWORD oversizes[57];
extern int travel1len;
extern int travel2len;
extern int travel3len;
extern unsigned short shotno;
extern short nstarts;
void clearmap(void);
void alignpanels(void);
void buildparts(void);
void defclip(void);
void setcursor1(int kind, int img);
void loadstrs(void);
void loadgauges(void);
int tileinit(void);
void newgame(void);
void startgame(void);
int controls(short a, int b);
void waitinput(int ms);
int loadsongs(void);
void maketitle(void);
void showtitle(void);
void pagedone(void);
int loadsounds(void);
void soundinfo(void);
void musicinfo(void);
int allocbank(void);
int seekentry(int id);
DWORD unpack(void *dst, DWORD dstlen, void *src, DWORD srclen);
void *loadentry(DWORD *size, int id);
int __cdecl readinput(void);
int loadscript(char *name);
void sysreport(char *p);
void loadgear(void);
int loadgame(int n);
int fadeslow(int in);
void credits(void);
void intro(unsigned char mode);
void begingame(unsigned short slot, short mode);
void fillgauges(void);
void setmouse(short x, short y);
int fadefast(int in);
int flip(void);
void drawcursor(void);

void
checkmem(void)
{
	if (availmem() < 0x400000 || memstat.dwMemoryLoad >= 90)
		lowmem = 1;
}

/*
 * sum the bytes of file name but the license block, and read the two
 * words after the signature at texts + 0xab58
 */
DWORD
scanexe(char *name, DWORD *off, DWORD *v1, DWORD *v2, DWORD *sum)
{
	if (off != NULL)
		*off = 0;
	if (v1 != NULL) {
		sc[5] = *v1;
		*v1 = 0;
	}
	if (v2 != NULL)
		*v2 = 0;
	*sum = 0;
	if (!fopenr(&scanfh, name))
		return 0;
	sc[4] = 0;
	sc[3] = 0;
	if (sc[5] == 0x6b9)
		licenseok = 1;
	sc[5] = 0;
	do {
		if (sc[3] != 0) {
			memcpy(packbuf, packbuf + sc[1], sc[3]);
			if (sc[2] != 0)
				sc[2] = fileread(packbuf + sc[3], 1, 0x96640 - sc[3] - 100, scanfh);
			sc[0] = sc[3] + sc[2];
		} else {
			sc[2] = fileread(packbuf, 1, 0x96640, scanfh);
			sc[0] = sc[2];
		}
		if (sc[2] < 0x4b320)
			sc[1] = sc[0];
		else if (sc[0] > 100)
			sc[1] = sc[0] - 100;
		else
			sc[1] = sc[0];
		sc[3] = sc[0] - sc[1];
		for (scanpos = 0; scanpos < sc[1]; scanpos++) {
			if (packbuf[scanpos] == ((unsigned char *)texts)[0xab58] &&
				packbuf[scanpos + 1] == ((unsigned char *)texts)[0xab59] &&
				packbuf[scanpos + 2] == ((unsigned char *)texts)[0xab5a] &&
				packbuf[scanpos + 3] == ((unsigned char *)texts)[0xab5b] &&
				packbuf[scanpos + 4] == ((unsigned char *)texts)[0xab5c] &&
				packbuf[scanpos + 5] == ((unsigned char *)texts)[0xab5d] &&
				packbuf[scanpos + 6] == ((unsigned char *)texts)[0xab5e] &&
				packbuf[scanpos + 7] == ((unsigned char *)texts)[0xab5f]) {
				if (off != NULL)
					*off = sc[5] + scanpos + 8;
				if (v1 != NULL) {
					*v1 = *(DWORD *)&packbuf[scanpos + 8];
					*v1 = ~*v1;
				}
				if (v2 != NULL) {
					*v2 = *(DWORD *)&packbuf[scanpos + 12];
					*v2 = ~*v2;
				}
				scanpos += 16;
			}
			if ((packbuf[scanpos] ^ 0xff) == 0x28 &&
				(packbuf[scanpos + 1] ^ 0xff) == 0x43 &&
				(packbuf[scanpos + 2] ^ 0xff) == 0xae &&
				(packbuf[scanpos + 3] ^ 0xff) == 0xff &&
				(packbuf[scanpos + 4] ^ 0xff) == 0x60 &&
				(packbuf[scanpos + 5] ^ 0xff) == 0x24 &&
				(packbuf[scanpos + 6] ^ 0xff) == 0x09 &&
				(packbuf[scanpos + 7] ^ 0xff) == 0)
				scanpos += 100;
			sc[4] += packbuf[scanpos];
		}
		sc[5] += scanpos;
	} while (sc[3] != 0);
	fileclose(&scanfh);
	*sum = sc[4];
	return sc[5];
}

int
initwin(HINSTANCE inst, int show)
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = wndproc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = inst;
	wc.hIcon = LoadIcon(inst, MAKEINTRESOURCE(10));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = T(9);
	if (!RegisterClass(&wc))
		return 0;
	accel = LoadAccelerators(inst, MAKEINTRESOURCE(20));
	mainwnd = CreateWindow(T(9), T(9), WS_POPUP | WS_SYSMENU, 0, 0,
		GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		NULL, NULL, inst, NULL);
	if (mainwnd == NULL)
		return 0;
	SetWindowText(mainwnd, gametitle());
	ShowWindow(mainwnd, show);
	UpdateWindow(mainwnd);
	SetFocus(mainwnd);
	return 1;
}

/* check the files of the game: 1 when one is missing, 2 when one changed */
int
checkfiles(short all)
{
	int n;
	DWORD sum;
	int r;
	short i;

	r = 0;
	for (i = 0; *files[i].name && i < 30; i++) {
		if (files[i].kind == 2)
			continue;
		if (!exists(files[i].name)) {
		missing:
			files[i].state = 1;
			r = 1;
			if (sizes[i] != 0) {
				if (sizes[i] < minsize)
					minsize = sizes[i];
			} else if (minsize > 0x19000)
				minsize = 0x19000;
			continue;
		}
		if (files[i].name == T(0x9b28) || files[i].name == T(0x9b62) || !sizes[i]) {
			files[i].state = 0;
			continue;
		}
		if (foundsize != sizes[i]) {
			files[i].state = 2;
			r = 2;
			continue;
		}
		if (crcs[i] == 0 || all == 0 && files[i].name != T(0x9bc9) ||
			files[i].name == T(0x9b62) || files[i].name == T(0x9b28)) {
			files[i].state = 0;
			continue;
		}
		if (files[i].scan) {
			n = scanexe(files[i].name, NULL, NULL, NULL, &sum);
			if (n == 0)
				goto missing;
		} else {
			sum = crcname(files[i].name, 0, 0);
			n = crclen;
			if (n == 0)
				goto missing;
		}
		if (!(sum == crcs[i])) {
			files[i].state = 2;
			r = 2;
		} else
			files[i].state = 0;
	}
	chkpending = 0;
	return r;
}

/* whether a file can be written in the current directory */
short
canwrite(void)
{
	HANDLE h;
	char c;

	if (!fopenw(&h, T(0x9ddd)))
		return 0;
	c = 0;
	if (filewrite(&c, 1, 1, h) == 0) {
		fileclose(&h);
		deletefile(T(0x9ddd));
		return 0;
	}
	fileclose(&h);
	if (!fopenr(&h, T(0x9ddd)))
		return 0;
	fileclose(&h);
	deletefile(T(0x9ddd));
	return 1;
}

int
exeok(void)
{
	return 1;
}

/* write the license number into maskedlic, xored */
void
makelicense(void)
{
	DWORD a, b, c;
	DWORD div;
	int k;
	char *p;

	memset(maskedlic, 0, 0x50);
	memset(namebuf, 0, 0x50);
	a = licblock[9];
	b = licblock[0x62];
	c = licblock[0x60];
	if (a != 0 && b != 0 && c != 0)
		;
	else
		licseal = 1;
	licpending = 0;
	if (a >= 0x60 || b >= 0x60 || c >= 0x60)
		;
	else {
		a = ~*(DWORD *)&licblock[a];
		b = _lrotr(*(DWORD *)&licblock[b], 3);
		c = _lrotl(*(DWORD *)&licblock[c], 7);
	}
	/* redundant: the tests below choose a */
	if (a != 0) {
		if (a != b || b != c)
			;
		else
			a = b;
	}
	if (a != c) {
		if (c == b)
			a = c;
		else
			a = b;
	}
	p = namebuf;
	div = 100000000;
	k = 0;
	for (;;) {
		b = a / div;
		a -= b * div;
		b = b > 9 ? 9 : b;
		*p++ = '0' + b;
		if (k == 2 || k == 5)
			*p++ = '-';
		k++;
		if (div == 1)
			break;
		div /= 10;
	}
	*p = 0;
	licseal = 0;
	licpending = 0;
	encodenum(namebuf, (*(unsigned long *)&lic[0x14]));
	if (isdebug())
		logf("liznr2: %Xh / %lu, offiz'%s'", (*(unsigned long *)&lic[0x14]), (*(unsigned long *)&lic[0x14]), namebuf);
	xorstr(maskedlic, namebuf);
	memset(namebuf, 0, 0x50);
}

/* the path of the program into exepathbuf, its directory into exedir */
int
exepath(void)
{
	if (!modulename(exepathbuf, 1000)) {
		exepathbuf[0] = 0;
		exedir[0] = 0;
		return 0;
	}
	memcpy(exedir, exepathbuf, 1000);
	splitname(exedir, NULL);
	return 1;
}

int
gamedir(char *name)
{
	if (!exepath()) {
		getdir(progdir, 1000);
		copystr(exedir, progdir);
		if (name != NULL)
			addname(exepathbuf, progdir, name);
		return 0;
	}
	copystr(progdir, exedir);
	return 1;
}

int
regstr(HKEY key, char *name, char *buf, DWORD size)
{
	DWORD type;

	memset(buf, 0, size);
	if (RegQueryValueEx(key, name, NULL, &type, (BYTE *)buf, &size) == ERROR_SUCCESS && type == REG_SZ)
		return 1;
	*buf = 0;
	return 0;
}

int
regbin(HKEY key, char *name, char *buf, DWORD size)
{
	DWORD type;

	memset(buf, 0, size);
	// FIXED: a value longer than buf, as a registry edited by hand can hold, fails with ERROR_MORE_DATA and its length in size, and the clear writes past buf by the difference. Clear buf at most.
#ifdef FEATURE_FAULTS
	{
		DWORD room;

		room = size;
		if (RegQueryValueEx(key, name, NULL, &type, (BYTE *)buf, &size) != ERROR_SUCCESS || type != REG_BINARY) {
			if (size > room)
				size = room;
			if (size != 0)
				memset(buf, 0, size);
			return 0;
		}
	}
#else
	if (RegQueryValueEx(key, name, NULL, &type, (BYTE *)buf, &size) != ERROR_SUCCESS || type != REG_BINARY) {
		if (size != 0)
			memset(buf, 0, size);
		return 0;
	}
#endif
	return 1;
}

int
regdword(HKEY key, char *name, DWORD *v)
{
	DWORD size, type;

	*v = 0;
	size = 4;
	if (RegQueryValueEx(key, name, NULL, &type, (BYTE *)v, &size) != ERROR_SUCCESS || type != REG_DWORD)
		return 0;
	return 1;
}

/* the language of the system, and the settings in the registry */
int
readreg(void)
{
	int r;
	DWORD v;

	regmajor = 0;
	regminor = 0;
	regaddinfo = 0;
	syslang = 1;
	switch (PRIMARYLANGID(GetUserDefaultLangID())) {
	case LANG_GERMAN:
	case LANG_DUTCH:
		syslang = 0;
		break;
	case LANG_FRENCH:
		syslang = 2;
		break;
	case LANG_ITALIAN:
	case LANG_CATALAN:
	case LANG_PORTUGUESE:
	case LANG_ROMANIAN:
	case LANG_BASQUE:
		syslang = 4;
		break;
	/* a body of its own, which the compiler merges with the one above */
	case LANG_SPANISH:
		syslang = 4;
		break;
	case LANG_POLISH:
		syslang = 5;
		break;
	case LANG_ENGLISH:
	default:
		syslang = 1;
		break;
	}
	memset(regapppath, 0, 1000);
	memset(regsaves, 0, 1000);
	hasregsaves = 0;
	r = 0;
	memset(regmenu, 0, 1000);
	hasregver = 0;
	memset(regvalbuf, 0, 200);
	regvalflag = 0;
	memset(&reg, 0, 200);
	hasregdata = 0;
	licagreed = 0;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, T(0x9f3d), 0, KEY_QUERY_VALUE, &hkey) == ERROR_SUCCESS) {
		regstr(hkey, T(0x9ffd), regapppath, 1000);
		r = 1;
		regstr(hkey, T(0xa011), regmenu, 1000);
		hasregver = 1;
		if (!regdword(hkey, T(0xa024), &regmajor))
			hasregver = 0;
		if (!regdword(hkey, T(0xa031), &regminor))
			hasregver = 0;
		if (!regdword(hkey, T(0xa03e), &regaddinfo))
			hasregver = 0;
		RegCloseKey(hkey);
		hkey = 0;
	}
	if (RegOpenKeyEx(HKEY_CURRENT_USER, T(0x9f3d), 0, KEY_QUERY_VALUE, &hkey) != ERROR_SUCCESS) {
		if (RegOpenKeyEx(HKEY_CURRENT_USER, T(0x9f76), 0, KEY_QUERY_VALUE, &hkey) == ERROR_SUCCESS) {
			if (regdword(hkey, T(0xa056), &v)) {
				syslang = v;
				hasreglang = 1;
			}
			RegCloseKey(hkey);
			hkey = 0;
		}
	} else {
		r = 1;
		if (regdword(hkey, T(0xa056), &v)) {
			hasreglang = 1;
			syslang = v;
		}
		if (regstr(hkey, T(0xa04d), regsaves, 1000))
			hasregsaves = 1;
		if (regbin(hkey, T(0xa071), (char *)&reg, 200))
			hasregdata = 1;
		if (regdword(hkey, T(0xa076), &v))
			licagreed = v;
		RegCloseKey(hkey);
		hkey = 0;
	}
	return r;
}

/* the path of special folder csidl into buf, or C:\ */
int
folderpath(char *buf, int csidl)
{
	LPITEMIDLIST pidl;
	LPMALLOC m;
	HRESULT hr;
	int r;

	hr = SHGetSpecialFolderLocation(NULL, csidl, &pidl);
	if (hr != NOERROR && csidl == CSIDL_COMMON_PROGRAMS)
		hr = SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &pidl);
	if (hr != NOERROR) {
		wsprintf(buf, "C:\\");
		r = 0;
	} else {
		r = SHGetPathFromIDList(pidl, buf);
		SHGetMalloc(&m);
		// PORT: COM through its C binding
#ifdef PORT
		IMalloc_Free(m, pidl);
		IMalloc_Release(m);
#else
		m->Free(pidl);
		m->Release();
#endif
	}
	return r;
}

int
savedir(char *buf, int x, int game)
{
	if (!folderpath(buf, CSIDL_PERSONAL))
		wsprintf(buf, T(0x9cec));
	if (game)
		addname(buf, buf, T(0x9987));
	else
		addname(buf, buf, T(0x99c6));
	return 1;
}

void
windir(void)
{
	if (!GetWindowsDirectory(winpath, 1000))
		wsprintf(winpath, T(0x9cec));
	wsprintf(uninstpath, T(0x9b09), winpath);
}

/* the local time of file time ft into buf, with the time of day when full */
void
fmttime(char *buf, FILETIME *ft, int full)
{
	SYSTEMTIME st;
	FILETIME lft;
	int m;

	FileTimeToLocalFileTime(ft, &lft);
	FileTimeToSystemTime(&lft, &st);
	m = st.wMonth;
	if (m >= 1 && m <= 12)
		;
	else
		m = 1;
	if (full)
		wsprintf(buf, T(0x9e01), st.wDay, months[m], st.wYear, st.wHour, st.wMinute, st.wSecond);
	else
		wsprintf(buf, T(0x9e1a), st.wDay, months[m], st.wYear);
}

/* n into buf with a separator every three digits */
void
fmtnum(char *buf, DWORD n)
{
	if (n < 1000)
		wsprintf(buf, T(0x9e23), n);
	else if (n < 1000000)
		wsprintf(buf, T(0x9e27), n / 1000, n % 1000);
	else
		wsprintf(buf, T(0x9e31), n / 1000000, n % 1000000 / 1000, n % 1000000 % 1000);
}

unsigned short
keysum(void)
{
	int i;
	unsigned short sum;

	sum = 0;
	for (i = 0; i < 100; i++) {
		if (i == 29 || i == reg.key[29] || i == reg.key[29] + 1)
			continue;
		sum += reg.key[i];
	}
	return sum;
}

unsigned short
keyword(void)
{
	return *(unsigned short *)&reg.key[reg.key[29]];
}

void
setkey13(unsigned char v)
{
	reg.key[13] = randn(13);
	reg.key[reg.key[13]] = v;
}

void keyreg(void);

void
markkey13(void)
{
	setkey13(randn(256) | 1);
	keyreg();
}

void
setkey34(unsigned char v)
{
	reg.key[34] = randn(4) + 30;
	reg.key[reg.key[34]] = v;
}

void
markchecked(void)
{
	setkey34(randn(256) | 1);
	keyreg();
}

void
keyreg(void)
{
	reg.key[29] = randn(8) + 20;
	*(unsigned short *)&reg.key[reg.key[29]] = keysum();
}

int
iskeyset(void)
{
	if (reg.key[reg.key[34]] & 1)
		return 1;
	return 0;
}

int
initok(void)
{
	return 1;
}

void
keyinit(void)
{
	int i;

	for (i = 0; i < 100; i++)
		reg.key[i] = randn(256);
	setkey13(randn(256) & 0xfe);
	setkey34(randn(256) & 0xfe);
	keyreg();
}

int
keyvalid(void)
{
	if (reg.key[29] < 20 || reg.key[29] >= 28 || reg.key[34] < 30 || reg.key[34] >= 34)
		return 0;
	return 1;
}

int
cancelstart(void)
{
	startcancel = 1;
	startpending = 0;
	return 1;
}

/* 0 when another instance runs */
int
onlyinstance(void)
{
	mutex = CreateMutex(NULL, TRUE, "TeudogarAllianceWithRome");
	if (mutex == NULL)
		return 0;
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return 0;
	return 1;
}

void
closemutex(void)
{
	if (mutex != NULL)
		CloseHandle(mutex);
}

int
needspace(void)
{
	int n;

	n = 0xea6000;
	if (instfound != 0) {
		if (instfull != 0)
			n = 0;
		else
			n = 0x2ee000;
	}
	return n & ~1023;
}

int
pump(void)
{
	if (!PeekMessage(&pumpmsg, NULL, 0, 0, PM_NOREMOVE))
		return 1;
	if (!GetMessage(&pumpmsg, NULL, 0, 0))
		return 0;
	if (!IsDialogMessage(dlg, &pumpmsg)) {
		TranslateMessage(&pumpmsg);
		DispatchMessage(&pumpmsg);
	}
	return 1;
}

void
showlog(void)
{
	SetDlgItemText(dlg, textid, dlgtext);
}

void
cleartext(void)
{
	memset(dlgtext, 0, 4000);
	logp = dlgtext;
	showlog();
}

void
addtext(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(logp, fmt, ap);
	// FIXED: va_start without va_end is undefined in C. End the list after its one use.
#ifdef PORT
	va_end(ap);
#endif
	while (*logp)
		logp++;
	*logp++ = '\r';
	*logp++ = '\n';
	*logp = 0;
}

/* show text and the progress done of total in the dialog */
void
progress(int kind, char *text, DWORD done, DWORD total)
{
	if (kind == 5)
		wsprintf(dlgtext + 0x3e8, text);
	else {
		switch (kind) {
		case 0:
			dlgtitle = pick(T(0x58f8), T(0x58fd), T(0x5905), T(0x5906), T(0x5907), T(0x590f));
			break;
		case 1:
			dlgtitle = pick(T(0x592e), T(0x5937), T(0x5942), T(0x5943), T(0x5944), T(0x594f));
			break;
		case 2:
			dlgtitle = pick(T(0x5910), T(0x5919), T(0x5921), T(0x5922), T(0x5923), T(0x592d));
			break;
		case 3:
			dlgtitle = T(0xa259);
			break;
		case 4:
			dlgtitle = pick(T(0x5950), T(0x5959), T(0x5963), T(0x5964), T(0x5965), T(0x596f));
			break;
		}
		wsprintf(dlgtext + 0x3e8, T(0xa265), dlgtitle, text);
	}
	dlgtext[999] = 0;
	if (!streqi(dlgtext, dlgtext + 0x3e8)) {
		memcpy(dlgtext, dlgtext + 0x3e8, 256);
		dlgtext[256] = 0;
		logp = dlgtext;
		while (*logp)
			logp++;
		showlog();
	}
	if (total > 100000) {
		done /= 1000;
		total /= 1000;
	} else {
		done /= 10;
		total /= 10;
	}
	SendDlgItemMessage(dlg, 1100, PBM_SETRANGE, 0, MAKELPARAM(0, total));
	SendDlgItemMessage(dlg, 1100, PBM_SETPOS, done, 0);
	UpdateWindow(dlg);
	ShowWindow(dlg, SW_SHOW);
	for (progpumps = 0; progpumps < 250; progpumps++)
		pump();
}

void
showitem(HWND w, int id, int show)
{
	ShowWindow(GetDlgItem(w, id), show);
}

void
setdefid(HWND w, int id)
{
	defid = id;
	if (id == 0)
		return;
	SendMessage(w, DM_SETDEFID, id, 0);
	SendDlgItemMessage(w, defid, BM_SETSTYLE, BS_DEFPUSHBUTTON, TRUE);
}

void
adderr(char *fmt, ...)
{
	va_list ap;
	char *p;

	va_start(ap, fmt);
	wvsprintf(fbuf, fmt, ap);
	// FIXED: va_start without va_end is undefined in C. End the list after its one use.
#ifdef PORT
	va_end(ap);
#endif
	for (p = errlines; *p; p++)
		;
	if (errlines[0]) {
		*p++ = '\r';
		*p++ = '\n';
		*p = 0;
	}
	trim(fbuf);
	copystr(p, fbuf);
}

void
errbox(char *msg)
{
	if (errlines[0])
		wsprintf(errboxtext, T(0xa26f), msg, errlines,
			pick(T(0x5fd3), T(0x5ffd), T(0x601d), T(0x601e), T(0x601f), T(0x603e)));
	else
		wsprintf(errboxtext, T(0xa27f), msg,
			pick(T(0x5fd3), T(0x5ffd), T(0x601d), T(0x601e), T(0x601f), T(0x603e)));
	MessageBox(dlg, errboxtext, pick(T(0x155), T(0x15c), T(0x162), T(0x163), T(0x164), T(0x16b)), MB_ICONHAND);
}

void
infobox(char *msg)
{
	MessageBox(dlg, msg, pick(T(0x5970), T(0x5975), T(0x597a), T(0x597b), T(0x597c), T(0x5981)), MB_ICONINFORMATION);
}

int CALLBACK
browseproc(HWND w, UINT msg, LPARAM lp, LPARAM data)
{
	if (msg == BFFM_INITIALIZED && browsestart != NULL)
		SendMessage(w, BFFM_SETSELECTION, TRUE, (LPARAM)browsestart);
	return 0;
}

int
browse(char *buf, char *title)
{
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	LPMALLOC m;

	memset(&bi, 0, sizeof bi);
	bi.hwndOwner = dlg;
	bi.pszDisplayName = buf;
	bi.lpszTitle = title;
	bi.lpfn = browseproc;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	if (setuppage == 4) {
		if (SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_PROGRAMS, &pidl) != NOERROR)
			SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &pidl);
		bi.pidlRoot = pidl;
	} else if (setuppage == 3) {
		SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl);
		bi.pidlRoot = pidl;
	}
	pidl = SHBrowseForFolder(&bi);
	if (pidl == NULL)
		return 0;
	SHGetPathFromIDList(pidl, buf);
	m = NULL;
	if (SHGetMalloc(&m) == NOERROR) {
		// PORT: COM through its C binding
#ifdef PORT
		IMalloc_Free(m, pidl);
		IMalloc_Release(m);
#else
		m->Free(pidl);
		m->Release();
#endif
	}
	return 1;
}

/* the folder of the game in the start menu into buf */
void
menudir(char *buf, int size)
{
	char *p;

	folderpath(buf, CSIDL_COMMON_PROGRAMS);
	if (buf[0] == 0) {
		CreateDirectory(T(0xa289), NULL);
		CreateDirectory(T(0xa294), NULL);
		wsprintf(buf, T(0xa294));
	}
	for (p = buf; *p; p++)
		;
	wsprintf(p, T(0xa2aa));
	if (!SetCurrentDirectory(buf)) {
		wsprintf(p, T(0xa2b1));
		if (!SetCurrentDirectory(buf))
			*p = 0;
	}
	addname(buf, buf, T(0x98ab));
	SetCurrentDirectory(instdir);
}

/* German, English, or Italian, else English */
void
checklang(void)
{
	if (lang == 1 || lang == 0 || lang == 4)
		;
	else
		lang = 1;
}

int
setupinit(void)
{
	memset(errlines, 0, 4000);
	windir();
	copystr(instdir, progdir);
	copystr(instsaves, hasregsaves && regsaves[0] ? regsaves : savesdir);
	menudir(instmenu, 1000);
	lang = syslang;
	checklang();
	chooselang(lang);
	return 1;
}

int
setupend(int ok)
{
	SetCurrentDirectory(progdir);
	return 1;
}

int
checkinstall(void)
{
	instfound = 0;
	instnewer = 0;
	instfull = 0;
	if (regapppath[0]) {
		wsprintf(setupbuf, T(0x9977), regapppath);
		if (statfile(setupbuf)) {
			insttime = ftime;
			instsize = fsize;
			if (regmajor == 0 && regminor == 0)
				regminor = 1;
			instfound = 1;
			if (regaddinfo & 2)
				instfull = 1;
			if (regmajor * 1000 + regminor > vermajor * 1000 + verminor &&
				(regaddinfo & 2 || !(verflags & 2)))
				instnewer = 1;
			return 1;
		}
	}
	regmajor = 0;
	regminor = 0;
	regaddinfo = 0;
	regapppath[0] = 0;
	regsaves[0] = 0;
	regmenu[0] = 0;
	instsize = 0;
	return 0;
}

int
askbox(HWND w)
{
	if (MessageBoxEx(w, pick(T(0x2fb), T(0x32b), T(0x359), T(0x35a), T(0x35b), T(0x37c)),
		pick(T(0x37d), T(0x38d), T(0x399), T(0x39a), T(0x39b), T(0x3b4)),
		MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2, 0) == IDYES)
		return 1;
	return 0;
}

void
dlgtexts(HWND w)
{
	SetWindowText(w, pick(T(0x27f), T(0x2a7), T(0x2d2), T(0x2d3), T(0x2d4), T(0x2fa)));
	SetDlgItemText(w, 0x232b, pick(T(0x3b5), T(0x3bf), T(0x3cb), T(0x3cc), T(0x3cd), T(0x3d9)));
	SetDlgItemText(w, 1, pick(T(0x3da), T(0x3e4), T(0x3ec), T(0x3ed), T(0x3ee), T(0x3f8)));
	SetDlgItemText(w, 3, pick(T(0xd4), T(0xdf), T(0xe6), T(0xe7), T(0xe8), T(0xf1)));
	SetDlgItemText(w, 0x232a, pick(T(0x5497), T(0x54bf), T(0x54de), T(0x54df), T(0x54e0), T(0x5503)));
	SetDlgItemText(w, 0x2328, pick(T(0x459), T(0x468), T(0x474), T(0x475), T(0x476), T(0x487)));
	SetDlgItemText(w, 0x2329, pick(T(0x441), T(0x44a), T(0x450), T(0x451), T(0x452), T(0x458)));
	SetDlgItemText(w, 0x232d, pick(T(0x5615), T(0x5624), T(0x5635), T(0x5636), T(0x5637), T(0x564b)));
	SetDlgItemText(w, 0x232e, pick(T(0x564c), T(0x5654), T(0x565c), T(0x565d), T(0x565e), T(0x5666)));
}

void
spacetexts(HWND w)
{
	int n;

	n = needspace();
	if (n != 0)
		wsprintf(setupbuf + 0x5dc, pick(T(0xab1), T(0xac3), T(0xad5), T(0xad6), T(0xad7), T(0xaea)), n / 0x100000);
	else
		setupbuf[0x5dc] = 0;
	wsprintf(setupbuf, pick(T(0xaeb), T(0xb16), T(0xb3b), T(0xb3c), T(0xb3d), T(0xb6f)), setupbuf + 0x5dc);
	SetDlgItemText(w, 0x410, setupbuf);
	SetDlgItemText(w, 0x411, pick(T(0xb70), T(0xb8c), T(0xba6), T(0xba7), T(0xba8), T(0xbcb)));
	SetDlgItemText(w, 0x412, pick(T(0xbcc), T(0xbdf), T(0xbea), T(0xbeb), T(0xbec), T(0xbf8)));
}

void
langtexts(HWND w)
{
	checklang();
	SetDlgItemText(w, 0x3f2, pick(T(0xf2), T(0x112), T(0x130), T(0x131), T(0x132), T(0x154)));
	if (lang == 1 || lang == 0 || lang == 4)
		CheckRadioButton(w, 0x3fd, 0x402, lang + 0x3fd);
}

void
addversion(void)
{
	if (buildtime.dwLowDateTime != 0 || buildtime.dwHighDateTime != 0)
		fmttime(setupbuf, &buildtime, 0);
	else
		setupbuf[0] = 0;
	fmtnum(setupbuf + 100, buildno);
	addtext(T(0xa2d6), pick(T(0x6d), T(0x8e), T(0xb2), T(0xb3), T(0xb4), T(0xd3)),
		verflags & 2 ? pick(T(0x4d6), T(0x4e2), T(0x4ef), T(0x4f0), T(0x4f1), T(0x503)) :
		pick(T(0x4ab), T(0x4b8), T(0x4c5), T(0x4c6), T(0x4c7), T(0x4d5)),
		vermajor, verminor, verflags & 1 ? T(0xa2c9) : empty,
		setupbuf[0] ? pick(T(0x504), T(0x508), T(0x50b), T(0x50c), T(0x50d), T(0x511)) : "",
		setupbuf);
}

void
setupinfo(void)
{
	addversion();
	addtext(pick(T(0x512), T(0x6e4), T(0x884), T(0x885), T(0x886), T(0xa57)), needspace() / 0x100000);
	showlog();
	if (instfound == 0)
		SetDlgItemText(dlg, 1, pick(T(0x459), T(0x468), T(0x474), T(0x475), T(0x476), T(0x487)));
}

void
showsavedir(void)
{
	if (asksaves != 0)
		SetDlgItemText(dlg, 0x3f4, pick(T(0x5716), T(0x5740), T(0x576d), T(0x576e), T(0x576f), T(0x55a2)));
	else
		SetDlgItemText(dlg, 0x3f4, pick(T(0x5667), T(0x56a0), T(0x56da), T(0x56db), T(0x56dc), T(0x55a2)));
	SetDlgItemText(dlg, 0x3f5, instsaves);
}

/* the place of t in s, case folded, or -1 */
int
findi(char *s, char *t)
{
	int i;
	char *p, *q;

	i = 0;
	while (*s) {
		if (*s == *t) {
			p = s;
			q = t;
			do {
				p++;
				q++;
				if (*q == 0)
					return i;
				if (*p == 0)
					return -1;
			} while (upper(*p) == upper(*q));
		}
		s++;
		i++;
	}
	return -1;
}

void
browsesave(void)
{
	memcpy(winpath, instsaves, 1000);
	splitname(winpath, NULL);
	memcpy(setupbuf, winpath, 1000);
	splitname(setupbuf, NULL);
	browsestart = setupbuf;
	if (!browse(winpath, pick(T(0x5716), T(0x5740), T(0x576d), T(0x576e), T(0x576f), T(0x5792))))
		return;
	memcpy(instsaves, winpath, 1000);
	if (findi(instsaves, T(0x9987)) == -1 && findi(instsaves, T(0x99c6)) == -1 &&
		findi(instsaves, T(0x9994)) == -1)
		addname(instsaves, instsaves, T(0x9987));
	SetDlgItemText(dlg, 0x3f5, instsaves);
}

void
showmenudir(void)
{
	SetDlgItemText(dlg, 0x3f4, pick(T(0x5793), T(0x57cb), T(0x5803), T(0x5804), T(0x5805), T(0x583a)));
	SetDlgItemText(dlg, 0x3f5, instmenu);
}

void
browsemenu(void)
{
	memcpy(winpath, instmenu, 1000);
	splitname(winpath, NULL);
	memcpy(setupbuf, winpath, 1000);
	browsestart = setupbuf;
	if (!browse(winpath, pick(T(0x583b), T(0x5878), T(0x58c4), T(0x58c5), T(0x58c6), T(0x58f7))))
		return;
	memcpy(instmenu, winpath, 1000);
	addname(instmenu, instmenu, T(0x98ab));
	SetDlgItemText(dlg, 0x3f5, instmenu);
}

void
errinit(void)
{
	errbox(pick(T(0x5982), T(0x59a1), T(0x59bd), T(0x59be), T(0x59bf), T(0x59e1)));
}

void
errmkdir(void)
{
	adderr(pick(T(0x5aca), T(0x5c7b), T(0x5dd5), T(0x5dd6), T(0x5dd7), T(0x5f0e)));
	errbox(pick(T(0x59e2), T(0x5a08), T(0x5a25), T(0x5a26), T(0x5a27), T(0x5a4d)));
}

void
errmenu(void)
{
	errbox(pick(T(0x5f0f), T(0x5f33), T(0x5f55), T(0x5f56), T(0x5f57), T(0x5f7b)));
}

/* a shell link to path at link */
HRESULT
mklink(char *path, char *dir, char *link, char *desc)
{
	IPersistFile *pf;
	IShellLink *sl;
	WCHAR wsz[1000];
	HRESULT hr;

	// PORT: the IDs by address, as C passes them
#ifdef PORT
	hr = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLink, (void **)&sl);
#else
	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&sl);
#endif
	if (FAILED(hr))
		return hr;
	// PORT: COM through its C binding
#ifdef PORT
	IShellLink_SetPath(sl, path);
	IShellLink_SetDescription(sl, desc);
	IShellLink_SetWorkingDirectory(sl, dir);
	hr = IShellLink_QueryInterface(sl, &IID_IPersistFile, (void **)&pf);
#else
	sl->SetPath(path);
	sl->SetDescription(desc);
	sl->SetWorkingDirectory(dir);
	hr = sl->QueryInterface(IID_IPersistFile, (void **)&pf);
#endif
	if (SUCCEEDED(hr)) {
		MultiByteToWideChar(CP_ACP, 0, link, -1, wsz, 1000);
		// PORT: COM through its C binding
#ifdef PORT
		hr = IPersistFile_Save(pf, wsz, TRUE);
		IPersistFile_Release(pf);
#else
		hr = pf->Save(wsz, TRUE);
		pf->Release();
#endif
	}
	// PORT: COM through its C binding
#ifdef PORT
	IShellLink_Release(sl);
#else
	sl->Release();
#endif
	return hr;
}

int
mkgamedirs(void)
{
	int r;

	r = makedirs(instdir, setupbuf);
	if (!SetCurrentDirectory(instdir)) {
		if (!r)
			adderr(T(0x618e), instdir);
		else
			adderr(T(0x61ac), instdir);
		return 0;
	}
	r = makedirs(instsaves, setupbuf);
	if (!SetCurrentDirectory(instsaves)) {
		if (r)
			adderr(T(0x61ac), instsaves);
		else
			adderr(T(0x618e), instsaves);
		return 0;
	}
	return 1;
}

void
exefile(char *buf, char *name)
{
	char *p;

	wsprintf(buf, exedir);
	for (p = buf; *p; p++)
		;
	if (p > buf && p[-1] != '\\') {
		*p = '\\';
		p++;
	}
	wsprintf(p, name);
}

void
getver(DWORD *a, DWORD *b, DWORD *c, char *s, DWORD *d)
{
	if (a != NULL)
		*a = dataoff.ver[0];
	if (b != NULL)
		*b = dataoff.ver[1];
	if (c != NULL)
		*c = dataoff.ver[2];
	if (s != NULL) {
		if (dataoff.vername2[0] != '?')
			memcpy(s, dataoff.vername2, 8);
		else
			memcpy(s, dataoff.vername, 8);
	}
	if (d != NULL)
		*d = dataoff.build;
}

int
logbufs(int on)
{
	if (on) {
		fdata_buf2 = GlobalAlloc(0, 0x700000);
		fdata_buf = GlobalAlloc(0, 0x1400000);
		logcur = logbuf;
		bytesdone = 0;
		if (fdata_buf2 != NULL && fdata_buf != NULL)
			return 1;
	}
	if (fdata_buf2 != NULL) {
		GlobalFree(fdata_buf2);
		fdata_buf2 = NULL;
	}
	if (fdata_buf != NULL) {
		GlobalFree(fdata_buf);
		fdata_buf = NULL;
	}
	return 0;
}

void
logput(char *fmt, ...)
{
	va_list ap;

	if (logcur >= logbuf + 20480 - 200)
		return;
	va_start(ap, fmt);
	wvsprintf(logcur, fmt, ap);
	// FIXED: va_start without va_end is undefined in C. End the list after its one use.
#ifdef PORT
	va_end(ap);
#endif
	if (*logcur) {
		if (lognl != 0) {
			loghead = logcur;
			lognl = 0;
		} else if (loghead == NULL)
			loghead = logbuf;
		while (*logcur)
			logcur++;
		if (logcur > logbuf && logcur[-1] == '~')
			logcur--;
		else {
			*logcur++ = '\r';
			*logcur++ = '\n';
			lognl = 1;
		}
		*logcur = 0;
	}
	if (logcb != NULL)
		logcb(logbuf, loghead);
}

int
logprogress(char *buf, char *line)
{
	int k, pos, i;

	/* defect: compares the pointer line with the address of a literal */
	// FIXED: a comparison with the address of a literal is unspecified in C. Compare with NULL, which holds for every line of logbuf as the original's test does, and skips a NULL line, on which the original faults in ideqi.
#ifdef PORT
	if (line != NULL) {
#else
	if (line != "") {
#endif
		if (ideqi(line, "ERROR")) {
			adderr(line + 7);
			return 1;
		}
		SetDlgItemText(dlg, textid, line);
		logshow = 1;
	}
	k = dataoff.total / 1000;
	pos = bytesdone / 1000 * 100 / k;
	if (pos != logpos) {
		logpos = pos;
		SendDlgItemMessage(dlg, 1100, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
		SendDlgItemMessage(dlg, 1100, PBM_SETPOS, pos, 0);
		logshow = 1;
	}
	if (logshow) {
		UpdateWindow(dlg);
		ShowWindow(dlg, SW_SHOW);
		logshow = 0;
	}
	for (i = 0; i < 250; i++)
		pump();
	return 0;
}

int
loginit(void)
{
	logcur = logbuf;
	bytesdone = 0;
	logcb = logprogress;
	return 1;
}

/* links to the game in the start menu, one for each language */
int
makelinks(void)
{
	int ok, i;

	makedirs(instmenu, setupbuf);
	SetCurrentDirectory(instmenu);
	CoInitialize(NULL);
	wsprintf(setupbuf, T(0x98ab));
	wsprintf(linkname, T(0xa33b), instmenu, setupbuf);
	wsprintf(linktarget, T(0x9977), instdir);
	ok = 1;
	if (mklink(linktarget, instdir, linkname, setupbuf) < 0)
		ok = 0;
	for (i = 0; i < 6; i++) {
		if (i == 1 || i == 0 || i == 4) {
			wsprintf(setupbuf, T(0x9c20), i == 0 ? 'D' : i == 2 ? 'F' : i == 3 ? 'S' : i == 4 ? 'I' : i == 5 ? 'P' : 'E');
			wsprintf(linkname, T(0xa33b), instmenu, setupbuf);
			pathjoin(linktarget, instdir, setupbuf);
			if (mklink(linktarget, instdir, linkname, setupbuf) < 0)
				ok = 0;
		}
	}
	wsprintf(setupbuf, T(0x9bc9));
	wsprintf(linkname, T(0xa33b), instmenu, setupbuf);
	pathjoin(linktarget, instdir, T(0x9bc9));
	if (mklink(linktarget, instdir, linkname, setupbuf) < 0)
		ok = 0;
	wsprintf(setupbuf, T(0x26b));
	wsprintf(linkname, T(0xa33b), instmenu, setupbuf);
	wsprintf(linktarget, T(0x9d71), instdir);
	if (mklink(linktarget, instdir, linkname, setupbuf) < 0)
		ok = 0;
	CoUninitialize();
	if (ok)
		return 1;
	return 0;
}

int
makelinks2(void)
{
	int ok;

	if (!SetCurrentDirectory(instmenu)) {
		if (!makedirs(instmenu, setupbuf))
			return 0;
		if (!SetCurrentDirectory(instmenu))
			return 0;
	}
	CoInitialize(NULL);
	pathjoin(setupbuf + 0x5dc, instdir, T(0x9b83));
	wsprintf(setupbuf, T(0x1d7));
	wsprintf(linkname, T(0xa33b), instmenu, setupbuf);
	pathjoin(linktarget, setupbuf + 0x5dc, T(0x9da2));
	ok = 1;
	if (mklink(linktarget, instdir, linkname, setupbuf) < 0)
		ok = 0;
	wsprintf(setupbuf, T(0x1f4));
	wsprintf(linkname, T(0xa33b), instmenu, setupbuf);
	pathjoin(linktarget, setupbuf + 0x5dc, T(0x9dae));
	if (mklink(linktarget, instdir, linkname, setupbuf) < 0)
		ok = 0;
	CoUninitialize();
	if (ok)
		return 1;
	return 0;
}

int
regcreate(HKEY root)
{
	DWORD disp;

	hkey = 0;
	if (RegCreateKeyEx(root, T(0x9f3d), 0, NULL, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hkey, &disp) != ERROR_SUCCESS)
		return 0;
	return 1;
}

void
regclose(void)
{
	RegCloseKey(hkey);
	hkey = 0;
}

int
savelang(void)
{
	DWORD v;

	v = lang;
	if (RegSetValueEx(hkey, T(0xa056), 0, REG_DWORD, (BYTE *)&v, 4) != ERROR_SUCCESS)
		return 0;
	return 1;
}

int
savereg(int x, char *s)
{
	int r;
	DWORD v;

	r = 1;
	if (!savelang())
		r = 0;
	v = 1;
	if (RegSetValueEx(hkey, T(0xa076), 0, REG_DWORD, (BYTE *)&v, 4) != ERROR_SUCCESS)
		r = 0;
	if (RegSetValueEx(hkey, T(0xa04d), 0, REG_SZ, (BYTE *)s, strlen(s) + 1) != ERROR_SUCCESS)
		r = 0;
	if (RegSetValueEx(hkey, T(0xa071), 0, REG_BINARY, (BYTE *)&reg, 200) != ERROR_SUCCESS)
		r = 0;
	return r;
}

int
updatereg(int x, char *s, char *dir)
{
	if (!regcreate(HKEY_CURRENT_USER))
		return 0;
	if (!savereg(x, s)) {
		regclose();
		return 0;
	}
	regclose();
	if (*dir && !streqi(regapppath, dir)) {
		wsprintf(tmpbuf, T(0x9977), regapppath);
		if (!statfile(tmpbuf)) {
			copystr(regapppath, progdir);
			if (regcreate(HKEY_LOCAL_MACHINE)) {
				RegSetValueEx(hkey, T(0x9ffd), 0, REG_SZ, (BYTE *)regapppath, strlen(regapppath) + 1);
				regclose();
			}
		}
	}
	return 1;
}

int
reglang(void)
{
	if (!regcreate(HKEY_CURRENT_USER))
		return 0;
	if (!savelang()) {
		regclose();
		return 0;
	}
	regclose();
	return 1;
}

int
reginstall(void)
{
	DWORD disp;
	int ok;

	ok = 1;
	if (!keyvalid() || keyword() != keysum()) {
		logf("registry_id_userdata2 (priv data) !ok / csum falsch: initting w/ new vals");
		keyinit();
	}
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, T(0x9e71), 0, NULL, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hkey, &disp) != ERROR_SUCCESS) {
		adderr(T(0x61cf));
		ok = 0;
	} else {
		if (RegSetValueEx(hkey, T(0x9f21), 0, REG_SZ, (BYTE *)T(0x98ab), strlen(T(0x98ab)) + 1) != ERROR_SUCCESS)
			ok = 0;
		wsprintf(setupbuf, T(0x9d71), instdir);
		if (RegSetValueEx(hkey, T(0x9f2d), 0, REG_SZ, (BYTE *)setupbuf, strlen(setupbuf) + 1) != ERROR_SUCCESS)
			ok = 0;
		RegCloseKey(hkey);
		hkey = 0;
	}
	if (regcreate(HKEY_LOCAL_MACHINE)) {
		if (RegSetValueEx(hkey, T(0x9ffd), 0, REG_SZ, (BYTE *)instdir, strlen(instdir) + 1) != ERROR_SUCCESS)
			ok = 0;
		if (RegSetValueEx(hkey, T(0xa011), 0, REG_SZ, (BYTE *)instmenu, strlen(instmenu) + 1) != ERROR_SUCCESS)
			ok = 0;
		wsprintf(setupbuf, T(0x98f0));
		if (RegSetValueEx(hkey, T(0xa01f), 0, REG_SZ, (BYTE *)setupbuf, strlen(setupbuf) + 1) != ERROR_SUCCESS)
			ok = 0;
		if (RegSetValueEx(hkey, T(0xa024), 0, REG_DWORD, (BYTE *)&vermajor, 4) != ERROR_SUCCESS)
			ok = 0;
		if (RegSetValueEx(hkey, T(0xa031), 0, REG_DWORD, (BYTE *)&verminor, 4) != ERROR_SUCCESS)
			ok = 0;
		if (RegSetValueEx(hkey, T(0xa03e), 0, REG_DWORD, (BYTE *)&verflags, 4) != ERROR_SUCCESS)
			ok = 0;
		regclose();
	} else {
		adderr(T(0x61cf));
		ok = 0;
	}
	if (regcreate(HKEY_CURRENT_USER)) {
		if (!savereg(1, instsaves))
			ok = 0;
		regclose();
	} else {
		adderr(T(0x61cf));
		ok = 0;
	}
	if (ok)
		return 1;
	return 0;
}

/* run the game with the language as its argument */
int
rungame(void)
{
	wsprintf(linktarget, T(0x9977), instdir);
	wsprintf(setupbuf, T(0x9dea), linktarget, lang == 0 ? T(0x9959) : lang == 2 ? T(0x9963) : lang == 3 ? T(0x9968) : lang == 4 ? T(0x996d) : lang == 5 ? T(0x9972) : T(0x995e));
	SetCurrentDirectory(instdir);
	return run(linktarget, setupbuf, instdir);
}

/* show the items of the setup dialog's page for mode setuppage */
void
// PORT: a name of its own, which C wants for an overloaded function
#ifdef PORT
showsetuppage(HWND w)
#else
showpage(HWND w)
#endif
{
	int v;

	memset(dlgtext, 0, 4000);
	logp = dlgtext;
	textid = 1013;
	defid = 0;
	SendDlgItemMessage(w, 1, BM_SETSTYLE, BS_PUSHBUTTON, 0);
	SendDlgItemMessage(w, 9002, BM_SETSTYLE, BS_PUSHBUTTON, 0);
	SendDlgItemMessage(w, 9003, BM_SETSTYLE, BS_PUSHBUTTON, 0);
	SendDlgItemMessage(w, 3, BM_SETSTYLE, BS_PUSHBUTTON, 0);
	showitem(w, 1040, SW_HIDE);
	showitem(w, 1041, SW_HIDE);
	showitem(w, 1042, SW_HIDE);
	showitem(w, 1, setuppage != 2 && setuppage != 5 && setuppage != 6 ? SW_SHOW : SW_HIDE);
	showitem(w, 9002, setuppage == 2 ? SW_SHOW : SW_HIDE);
	showitem(w, 3, setuppage != 6 ? SW_SHOW : SW_HIDE);
	showitem(w, 9003, setuppage != 0 && setuppage != 5 && setuppage != 6 ? SW_SHOW : SW_HIDE);
	v = setuppage == 6 ? SW_SHOW : SW_HIDE;
	showitem(w, 9000, v);
	showitem(w, 9001, v);
	v = setuppage == 3 || setuppage == 4 ? SW_SHOW : SW_HIDE;
	showitem(w, 9005, v);
	showitem(w, 9006, v);
	showitem(w, 1050, v);
	v = setuppage == 0 ? SW_SHOW : SW_HIDE;
	showitem(w, 1021, v);
	showitem(w, 1022, v);
	showitem(w, 1023, v);
	showitem(w, 1024, v);
	showitem(w, 1025, v);
	showitem(w, 1026, v);
	showitem(w, 1010, setuppage == 0 ? SW_SHOW : SW_HIDE);
	showitem(w, 1017, SW_HIDE);
	showitem(w, 1011, setuppage == 1 ? SW_SHOW : SW_HIDE);
	v = setuppage == 5 || setuppage == 3 || setuppage == 4 || setuppage == 6 ? SW_SHOW : SW_HIDE;
	showitem(w, 1012, v);
	showitem(w, 1013, v);
	v = setuppage == 2 ? SW_SHOW : SW_HIDE;
	showitem(w, 1014, v);
	showitem(w, 1016, v);
	focusdef = 1;
	focusid = 1;
	switch (setuppage) {
	case 0:
		langtexts(w);
		break;
	case 1:
		textid = 1011;
		setupinfo();
		break;
	case 2:
		SetDlgItemText(w, 1016, pick(T(0x16ed), T(0x16fc), T(0x170e), T(0x170f), T(0x1710), T(0x1725)));
		copystr(dlgtext, T(0xa24d));
		catstr(dlgtext, pick(T(0x5424), T(0x5449), T(0x546b), T(0x546c), T(0x546d), T(0x5496)));
		catstr(dlgtext, T(0xa24d));
		copystrn(tmpbuf, licensee, sizeof licensee);
		invert(tmpbuf, sizeof licensee);
		tmpbuf[sizeof licensee] = 0;
		catstr(dlgtext, tmpbuf);
		catstr(dlgtext, T(0xa250));
		catstr(dlgtext, pick(T(0x35a5), T(0x4063), T(0x49f9), T(0x49fa), T(0x49fb), T(0x5423)));
		SetDlgItemText(w, 1014, dlgtext);
		textid = 1014;
		focusdef = 9003;
		focusid = 1014;
		break;
	case 3:
		textid = 1013;
		showsavedir();
		break;
	case 4:
		textid = 1013;
		showmenudir();
		break;
	case 5:
		SetDlgItemText(w, 1012, empty);
		SetDlgItemText(w, 1013, empty);
		textid = 1013;
		break;
	case 6:
		SetDlgItemText(w, 1012, empty);
		wsprintf(dlgtext, T(0xa345), pick(T(0x613e), T(0x615a), T(0x6171), T(0x6172), T(0x6173), T(0x618d)));
		SetDlgItemText(w, 1013, dlgtext);
		focusdef = 9000;
		focusid = 9000;
		break;
	}
	setdefid(w, focusdef);
	SetFocus(GetDlgItem(w, focusid));
}

void
savepos(HWND w)
{
	RECT r;

	GetWindowRect(w, &r);
	winx = r.left;
	winy = r.top;
}

BOOL CALLBACK
setupproc(HWND w, WORD msg, int wp, LPARAM lp)
{
	RECT r;

	switch (msg) {
	case WM_CLOSE:
		break;
	case WM_INITDIALOG:
		dlg = w;
		// PORT: the icon through intptr_t, which holds the 64-bit pointer that a LONG truncates
#ifdef PORT
	SetClassLong(w, GCL_HICON, (LONG)(intptr_t)LoadIcon(appinst, MAKEINTRESOURCE(101)));
#else
	SetClassLong(w, GCL_HICON, (LONG)LoadIcon(appinst, MAKEINTRESOURCE(101)));
#endif
		if (!winw || !winh) {
			GetWindowRect(w, &r);
			winw = r.right - r.left;
			winh = r.bottom - r.top;
			winx = GetSystemMetrics(SM_CXSCREEN) - winw;
			if (winx < 0)
				winx = 0;
			winx /= 2;
			winy = GetSystemMetrics(SM_CYSCREEN) - winh;
			if (winy < 0)
				winy = 0;
			winy /= 2;
		}
		SetWindowPos(w, NULL, winx, winy, winw, winh, SWP_NOSIZE);
		dlgtexts(w);
	show:
		SetDlgItemText(w, 1, pick(T(0x3da), T(0x3e4), T(0x3ec), T(0x3ed), T(0x3ee), T(0x3f8)));
		if (setuppage == 3 && instfound && regsaves[0] && !asksaves && SetCurrentDirectory(regsaves)) {
			SetCurrentDirectory(progdir);
			wsprintf(instsaves, regsaves);
			setuppage++;
		}
		if (setuppage == 4 && instfound && regmenu[0]) {
			wsprintf(instmenu, regmenu);
			setuppage++;
		}
		if (setuppage == 5) {
		finish:
			savepos(w);
			EndDialog(w, 1);
		} else
			// PORT: a name of its own, which C wants for an overloaded function
#ifdef PORT
			showsetuppage(w);
#else
			showpage(w);
#endif
		break;
	case WM_SYSCOMMAND:
		if (wp != SC_CLOSE)
			break;
	cancel:
		if (setuppage > 2 && setuppage != 6 && !askbox(w))
			return TRUE;
	quit:
		savepos(w);
		EndDialog(w, 0);
		return TRUE;
	case WM_COMMAND:
		switch (wp) {
		case IDOK:
		case 9002:
			if (!asksaves || setuppage != 3) {
				if (setuppage >= 6)
					return TRUE;
				setuppage++;
				goto show;
			}
			goto finish;
		case 8999:
			setuppage++;
			goto show;
		case IDABORT:
			goto cancel;
		case 9001:
		case 9004:
			goto quit;
		case 9003:
			if (setuppage <= 0)
				return FALSE;
			setuppage--;
			goto show;
		case 9005:
			if (setuppage == 3) {
				browsesave();
				return TRUE;
			}
			if (setuppage == 4)
				browsemenu();
			break;
		case 9006:
			if (setuppage == 3) {
				savedir(instsaves, 0, 1);
				SetDlgItemText(w, 1013, instsaves);
			} else if (setuppage == 4) {
				menudir(instmenu, 1000);
				SetDlgItemText(w, 1013, instmenu);
			}
			return TRUE;
		case 9000:
			if (setuppage != 6)
				return TRUE;
			savepos(w);
			EndDialog(w, 2);
			return TRUE;
		default:
			if (wp < 1021 || wp >= 1027)
				return FALSE;
			lang = wp - 1021;
			checklang();
			chooselang(lang);
			langtexts(w);
			dlgtexts(w);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK
progproc(HWND w, WORD msg, int wp, LPARAM lp)
{
	switch (msg) {
	case WM_INITDIALOG:
		dlg = w;
		// PORT: the icon through intptr_t, which holds the 64-bit pointer that a LONG truncates
#ifdef PORT
	SetClassLong(w, GCL_HICON, (LONG)(intptr_t)LoadIcon(appinst, MAKEINTRESOURCE(101)));
#else
	SetClassLong(w, GCL_HICON, (LONG)LoadIcon(appinst, MAKEINTRESOURCE(101)));
#endif
		SetWindowText(w, pick(T(0x27f), T(0x2a7), T(0x2d2), T(0x2d3), T(0x2d4), T(0x2fa)));
		SetDlgItemText(w, IDABORT, pick(T(0xd4), T(0xdf), T(0xe6), T(0xe7), T(0xe8), T(0xf1)));
		SetWindowPos(w, NULL, winx, winy, winw, winh, SWP_NOSIZE);
		textid = 1013;
		return TRUE;
	case WM_SYSCOMMAND:
		if (wp != SC_CLOSE)
			break;
	cancel:
		if (installing) {
			if (!askbox(w))
				return TRUE;
			infobox(pick(T(0x5f7c), T(0x5f9c), T(0x5fb3), T(0x5fb4), T(0x5fb5), T(0x5fd2)));
			aborted = 1;
		}
		return TRUE;
	case WM_COMMAND:
		if (wp == IDABORT)
			goto cancel;
		break;
	}
	return FALSE;
}

/* the setup of the game: the dialog, then the directories and the links */
int WINAPI
setup(HINSTANCE inst)
{
	int ok, r;

	appinst = inst;
	dlg = NULL;
	ok = 0;
	exepath();
	if (!setupinit()) {
	fail:
		errinit();
		goto done;
	}
	checkinstall();
	if (!initok())
		goto fail;
	if (asksaves)
		setuppage = 3;
	else if (hasreglang)
		setuppage = 1;
	else
		setuppage = 0;
again:
	r = DialogBoxParam(appinst, MAKEINTRESOURCE(500), NULL, (DLGPROC)setupproc, 0);
	if (r == 1) {
		dlg = NULL;
		mkgamedirs();
		if (dlg) {
			DestroyWindow(dlg);
			dlg = NULL;
		}
		installing = 0;
		ok = 1;
		makelinks();
		reginstall();
		setupend(ok);
		hasregsaves = 1;
		copystr(regsaves, instsaves);
		return 1;
	}
	if (r == 30)
		goto done;
	if (r == 31) {
		run(exedir, "", exedir);
		goto again;
	}
done:
	setupend(ok);
	return 0;
}

int
hasversion(void)
{
	if (reg.oldmajor == 0xff)
		return 0;
	if (!reg.oldmajor && !reg.oldminor)
		return 0;
	return 1;
}

int
noversion(void)
{
	if (reg.oldmajor == 0xff)
		return 1;
	return 0;
}

/* initialize the game: the version, the tables, the options in cmd, the registry, the files */
int
inisys(HINSTANCE inst, char *cmd, char *err)
{
	int x, i;

	*err = 0;
	version[7] = 0;
	inibase();
	gettime();
	starttime = (*(short *)&hour);
	starttime *= 60;
	starttime += (*(short *)&minute);
	// PORT: a name of its own, which C wants for an overloaded function
#ifdef PORT
	isi = unpackblock();
#else
	isi = unpack();
#endif
	x = 0;
	setlang(0xff);
	if (gamedir(T(0x9b28)))
		cdprog();
	savedir(savesdir, 0, 1);
	wsprintf(mapsdir, T(0xa4f3), progdir);
	gettime();
	availmem();
	vermajor = (version[8] - '0') * 10 + (version[9] - '0');
	verminor = version[13] >= '0' && version[13] <= '9' ? (version[11] - '0') * 100 + (version[12] - '0') * 10 + (version[13] - '0') : (version[11] - '0') * 10 + (version[12] - '0');
	verflags = 0;
	if (version[14] == 'e')
		verflags = 1;
	if (version[15] == 0xff)
		verflags |= 2;
	getver(NULL, NULL, NULL, (char *)&buildtime, &buildno);
	kbdinit();
	mouseinit();
	// FIXED: a tick of gametick is due each 16.67 ms, and at the timer's default timeGetTime can step by 15 ms, which holds a tick past a refresh. Ask for 1 ms.
#ifdef FEATURE_PACE
	timeBeginPeriod(1);
#endif
	seedrand(ticks());
	chooselang(1);
	setlang(0xff);
	initrects();
	inkeyfn = 0;
	infieldfn = 0;
	insetfn = 0;
	initdone = 0;
	optscan = maskedtext;
	while (*optscan)
		*optscan++ ^= 0xff;
	for (isi = 0; isi < 30; isi++) {
		sizes[isi] = ~sizes[isi];
		crcs[isi] = ~crcs[isi];
	}
	for (isj = 0; isj < 78; isj++) {
		partdefs[isj].setkind = partkinds[isj][0];
		partdefs[isj].setflag = partkinds[isj][1];
	}
	for (isi = 0; isi < 78; isi++) {
		if (partdefs[isi].setkind == 0x2f) {
			memset(&partdefs[isi], 0, sizeof partdefs[0]);
			partdefs[isi].setkind = 0x2f;
			partdefs[isi].layer = 0xff;
		}
	}
	memset(facecache, 0, sizeof facecache);
	memset(overcache, 0, sizeof overcache);
	for (isi = 0; isi < 78; isi++)
		// PORT: partpics for Part's pics
#ifdef PORT
		partpics[isi] = NULL;
#else
		partdefs[isi].pics = NULL;
#endif
	clearbufs();
	lastscan = 0;
	leftclick = 0;
	rightclick = 0;
	nopvari = 0;
	noprun = 0;
	wsprintf(vertext, T(0xb233), (version[8] - '0') * 10 + (version[9] - '0'),
		version[13] >= '0' && version[13] <= '9' ? (version[11] - '0') * 100 + (version[12] - '0') * 10 + (version[13] - '0') : (version[11] - '0') * 10 + (version[12] - '0'));
	memset(cfgtext1, 0, 50);
	memset(cfgtext2, 0, 50);
	memset(cfgtext3, 0, 50);
	memset(cfgtext4, 0, 50);
	memset(cfgtext5, 0, 50);
	memset(cfgtext6, 0, 50);
	memset(cfgtext7, 0, 50);
	memset(licenseno, 0, sizeof licenseno);
	optg[0] = 0;
	scriptfile[0] = 0;
	strfile[0] = 0;
	optscan = cmd;
	optword = cmd;
	optlast = 0;
	for (;;) {
		if (*optscan == ' ')
			break;
		if (*optscan == 0)
			break;
	next:
		optscan++;
	}
	if (*optscan == 0)
		optlast = 1;
	else
		*optscan = 0;
	if (*optword == '&' || *optword == '_') {
		if (optword[1] == 'G' || optword[1] == 'g')
			copystr(optg, optword + 2);
		else if (optword[1] == 'D' || optword[1] == 'd') {
			if (optword[3] != ':')
				wsprintf(scriptfile, T(0x9df4), progdir, optword + 2);
			else
				copystr(scriptfile, optword + 2);
		} else if (optword[1] == 'T' || optword[1] == 't') {
			if (optword[3] != ':')
				wsprintf(strfile, T(0x9df4), progdir, optword + 2);
			else
				copystr(strfile, optword + 2);
		}
	} else if (streqi(optword, T(0xa36d))) {
		quickstart = 1;
		titlesopt = 0;
	} else if (streqi(optword, T(0xa378)))
		cheat = 1;
	else if (streqi(optword, T(0xa37e)))
		newgameopt = 1;
	else if (streqi(optword, T(0x9959)))
		chooselang(0);
	else if (streqi(optword, T(0x995e)))
		chooselang(1);
	else if (streqi(optword, T(0x9963)))
		chooselang(2);
	else if (streqi(optword, T(0x9968)))
		chooselang(3);
	else if (streqi(optword, T(0x996d)))
		chooselang(4);
	else if (streqi(optword, T(0x9972)))
		chooselang(5);
	else if (streqi(optword, T(0xa3a9)))
		titlesopt = 0;
	else if (streqi(optword, T(0xa392)))
		soundoff = 1;
	else if (streqi(optword, T(0xa39a)))
		musicoff = 1;
	else if (streqi(optword, T(0xa3a2))) {
		minmem = 1;
		soundoff = 1;
		sfxon = 0;
		musicoff = 1;
	} else if (streqi(optword, T(0xa3e3)))
		lowmem = 1;
	else if (streqi(optword, T(0xa3eb)))
		nopequip = 1;
	else if (streqi(optword, T(0xa3f4)))
		noprinter = 1;
	else if (streqi(optword, T(0xa3fe)))
		noprun = 1;
	else if (streqi(optword, T(0xa405)))
		nopvari = 1;
	else if (streqi(optword, T(0xa40d)))
		nopani = 1;
	else if (streqi(optword, T(0xa414)))
		nofani = 1;
	else if (streqi(optword, T(0xa386)))
		firststart = 1;
	if (!optlast) {
		optword = optscan + 1;
		goto next;
	}
	if (loadudta())
		udtaloaded = 1;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
		*err = 0;
		isi = MessageBox(NULL, T(0x97c1), T(0x8e), MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_SETFOREGROUND);
		if (isi == IDCANCEL || isi == IDABORT)
			return 0;
		if (isi == IDYES) {
			quickstart = 1;
			titlesopt = 0;
			noprinter = 1;
			featuresoff = 1;
			soundoff = 1;
			musicoff = 1;
			lowmem = 1;
			noprun = 1;
			sfxon = 0;
		} else {
			isi = MessageBox(NULL, T(0x988b), T(0x8e), MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2);
			if (isi == IDCANCEL || isi == IDABORT)
				return 0;
			if (isi == IDYES) {
				soundoff = 1;
				sfxon = 0;
				quickstart = 1;
			}
			isi = MessageBox(NULL, T(0x989b), T(0x8e), MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2);
			if (isi == IDCANCEL || isi == IDABORT)
				return 0;
			if (isi == IDYES) {
				musicoff = 1;
				quickstart = 1;
			}
		}
	}
	if (startlevel >= 2) {
		minmem = 1;
		lowmem = 1;
		if (soundoff != 2)
			soundoff = 1;
	}
	setbw((char *)defpal);
	initpals();
	memset(banktile, 0, sizeof banktile);
	makelicense();
	memguard1 = 1234;
	memguard2 = 1234;
	memguard3 = 1234;
	licenseok = 1;
	talkok = 1;
	memguard4 = 1234;
	lastslot = 0;
	isj = isi;
	memguard5 = 1234;
	gearmsg = 0;
	memguard6 = 1234;
	isi = 0;
	readreg();
	if (!hasreglang || !hasregsaves || !hasregdata || !licagreed)
		goto install;
again:
	if (!keyvalid() || keyword() != keysum())
		keyinit();
	if (hasregsaves && regsaves[0])
		copystr(savesdir, regsaves);
	if (!udtaok)
		x = 1;
	if (!cdsaves()) {
		if (setuptries++ == 0) {
			asksaves = 1;
		install:
			if (!setup(inst)) {
				*err = 0;
				return 0;
			}
			goto again;
		}
		if (newcfg < 2)
			newcfg = 1;
		if (!firststart) {
			firststart = 1;
			defaultcfg();
		} else
			defaultcfg();
		goto check;
	}
	isi = loadcfg();
	restoremodes();
	if (isi == 0) {
		if (newcfg < 2)
			newcfg = 1;
		if (!firststart)
			firststart = 1;
		defaultcfg();
	} else if (newcfg)
		setlang(0xff);
	adjpal(optbright, optcontrast, optred, optgreen, optblue);
	if (findfirst(T(0xb30e)) || findnext() || findnext() || findnext()) {
		if (!firststart)
			firststart = 1;
	}
	findclose();
	cdprog();
check:
	if (lang == 0xff && hasreglang) {
		lang = syslang;
		chooselang(lang);
	}
	if (memguard1 != memguard3 && memguard1 != memguard2 || licseal || !exeok()) {
	error:
		xorstr(printbuf, pick(T(0x707a), T(0x70f4), T(0x714d), T(0x714e), T(0x714f), T(0x719e)));
		xorstr(printbuf + 0x7d0, T(0x6e75));
		wsprintf(err, printbuf, printbuf + 0x7d0);
		return 0;
	}
	if (iskeyset())
		filecheck = checkfiles(0);
	else {
		filecheck = checkfiles(1);
		if (!filecheck)
			markchecked();
	}
	if (filecheck) {
		optscan = err;
		wsprintf(optscan, pick(T(0x7c72), T(0x7c81), T(0x7c8e), T(0x7c8f), T(0x7c90), T(0x7c9a)),
			filecheck == 2 ? pick(T(0x7c9b), T(0x7ca7), T(0x7cb1), T(0x7cb2), T(0x7cb3), T(0x7cbc))
			: pick(T(0x7cbd), T(0x7cc6), T(0x7cce), T(0x7ccf), T(0x7cd0), T(0x7cd9)));
		while (*optscan)
			optscan++;
		errstart = optscan;
		isi = 0;
		isj = 0;
		while (*files[isi].name && isi < 30 && isj < 3) {
			if (files[isi].state == filecheck && !(filecheck == 1 && files[isi].kind == 2)) {
				wsprintf(optscan, T(0xb0f0), files[isi].name);
				while (*optscan)
					optscan++;
				isj++;
			}
			isi++;
		}
		if (optscan > errstart)
			optscan--;
		if (optscan > errstart)
			optscan--;
		*optscan++ = '\r';
		*optscan++ = '\r';
		if (filecheck == 2) {
			wsprintf(optscan, pick(T(0x7394), T(0x73ca), T(0x73ef), T(0x73f0), T(0x73f1), T(0x7412)));
			xorstr(optscan, optscan);
			while (*optscan)
				optscan++;
			return 0;
		}
		copystr(optscan, pick(T(0x7544), T(0x757a), T(0x759f), T(0x75a0), T(0x75a1), T(0x75b8)));
		return 0;
	}
	cdsaves();
	isi = canwrite();
	cdprog();
	if (chkpending)
		goto error;
	if (!savefail && !isi) {
		readonlydir = 1;
		savefail = 1;
	} else if (!savefail) {
		if (!onlyinstance()) {
			wsprintf(err, pick(T(0x7a5c), T(0x7aed), T(0x7b74), T(0x7b75), T(0x7b76), T(0x7c0d)));
			return 0;
		}
		inidat();
		if (!loadtables()) {
			wsprintf(err, pick(T(0x7c0e), T(0x7c31), T(0x7c4f), T(0x7c50), T(0x7c51), T(0x7c71)),
				pick(T(0x7544), T(0x757a), T(0x759f), T(0x75a0), T(0x75a1), T(0x75b8)));
			return 0;
		}
		readindex();
		if (esckey == 0xff)
			return 0;
		loadintro();
		if (x)
			licenseok = 0;
		if (startlevel < 2 && !quickstart)
			music(1, 0);
		if (!quickstart) {
			isj = randn(100) < 50 ? 1 : 0;
			loadscr(isj ? 0x2086 : 0xfb0, (unsigned char *)scrbuf, NULL, packbuf, 0x96640);
			loadscr(isj ? 0x2065 : 4000, (unsigned char *)workbuf, NULL, packbuf, 0x96640);
			titlecolors((unsigned char *)workbuf, 0);
			for (i = 0; i < 0x4b000; i++) {
				char c = workbuf[i];

				if (c)
					scrbuf[i] = c;
			}
		}
		usepal(0);
		keepui(gamepal);
		return 1;
	}
	wsprintf(err, pick(T(0x75b9), T(0x7688), T(0x773b), T(0x773c), T(0x773d), T(0x77e7)));
	return 0;
}

void
setupsound(void)
{
	if (!soundoff) {
		initsound();
		return;
	}
	dsready = 0;
	wsprintf(dsmsg, T(0x910f));
	wsprintf(dsshort, T(0x9183));
}

int
inisys2(char *err)
{
	clearmap();
	alignpanels();
	buildparts();
	defclip();
	setcursor1(3, 3);
	playmode = 1;
	menumode = 0;
	if (lang == 0xff)
		chooselang(1);
	if (quitting)
		return 0;
	loadstrs();
	if (newcfg) {
		savefail = 0;
		if (cdsaves()) {
			if (startlevel < 2 && !(short)savecfg())
				savefail = 1;
			cdprog();
		} else {
			if (!makedirs(savesdir, tmpbuf)) {
				savefail = 1;
				goto fail;
			}
			if (!cdsaves()) {
				savefail = 1;
				goto fail;
			}
			if (startlevel < 2 && !(short)savecfg())
				savefail = 1;
			cdprog();
		}
		if (savefail) {
		fail:
			wsprintf(err, pick(T(0x79cc), T(0x79f9), T(0x7a1c), T(0x7a1d), T(0x7a1e), T(0x7a5b)));
			return 0;
		}
	}
	if (hasversion())
		afterupdate = 1;
	if (noversion())
		afterinst = 1;
	loadgauges();
	checkmem();
	if (!tileinit()) {
		wsprintf(err, pick(T(0x77e8), T(0x787a), T(0x78ee), T(0x78ef), T(0x78f0), T(0x794e)));
		return 0;
	}
	newgame();
	startgame();
	startpending = 0;
	if (firststart && firststart != 0xff && !quickstart) {
		memset(realms, 0, 0x6b8);
		controls(1, 0);
		if (esckey == 0xff)
			return 0;
		esckey = 0;
		if (!quickstart)
			waitinput(0x2ee);
	}
	checkmem();
	setupsound();
	loadsongs();
	if (startlevel < 2 && !quickstart)
		music(1, 0);
	lastscan = 0;
	keycode = 0;
	maketitle();
	if (startlevel < 2 && !quickstart)
		showtitle();
	titletick = ticks();
	pagedone();
	if (keys[VK_SHIFT] & 0x80 && !afterinst && !afterupdate)
		quickstart = 1;
	if (startlevel < 2 && !quickstart)
		showtitle();
	if (chkpending || startpending || licpending) {
	error:
		xorstr(printbuf, pick(T(0x707a), T(0x70f4), T(0x714d), T(0x714e), T(0x714f), T(0x719e)));
		xorstr(printbuf + 0x7d0, T(0x6e75));
		wsprintf(err, printbuf, printbuf + 0x7d0);
		return 0;
	}
	execrc = crcname(exepathbuf, 0, 0) ^ 0x9f3ff2ff;
	checkmem();
	if (dsready && !loadsounds()) {
		freesound();
		dsready = 0;
	}
	if (startlevel < 2 && !quickstart)
		showtitle();
	soundinfo();
	musicinfo();
	checkmem();
	allocbank();
	checkmem();
	partscached = 0;
	cacheparts = 0;
	for (cachelevel = 10; cachelevel > 0; cachelevel--) {
		for (i2i = 0; i2i < 78; i2i++) {
			if (partdefs[i2i].setkind == 0x2f)
				partdefs[i2i].size = 0;
			else if (partdefs[i2i].size >= cachelevel) {
				if (!minmem && !lowmem && seekentry(i2i + 0x1398)) {
					i2len = storedn;
					fileread(packbuf + 0x4b320, i2len, 1, datfh);
					i2len = unpack(packbuf, 0x4b320, packbuf + 0x4b320, i2len);
					// PORT: partpics for Part's pics
#ifdef PORT
					partpics[i2i] = (char *)GlobalAlloc(GMEM_FIXED, i2len);
#else
					partdefs[i2i].pics = (char *)GlobalAlloc(GMEM_FIXED, i2len);
#endif
				} else
					// PORT: partpics for Part's pics
#ifdef PORT
					partpics[i2i] = NULL;
#else
					partdefs[i2i].pics = NULL;
#endif
				cacheparts++;
				// PORT: partpics for Part's pics
#ifdef PORT
				if (!partpics[i2i])
#else
				if (!partdefs[i2i].pics)
#endif
					partdefs[i2i].size = 0;
				else {
					cachebytes += i2len;
					partbytes += i2len;
					partscached++;
					// PORT: partpics for Part's pics
#ifdef PORT
					memcpy(partpics[i2i], packbuf, i2len);
#else
					memcpy(partdefs[i2i].pics, packbuf, i2len);
#endif
					partdefs[i2i].size = i2len;
				}
			}
		}
	}
	if (partscached) {
		if (partscached >= cacheparts)
			partscached = 2;
		else
			partscached = 1;
	}
	if (startlevel < 2 && !quickstart)
		showtitle();
	checkmem();
	facescached = 0;
	if (seekentry(0x2077)) {
		fileread(packbuf, 12, 1, datfh);
		fileread(&crcword, 4, 1, datfh);
	}
	for (i2i = 0; i2i < 57; i2i++) {
		if (!minmem && !lowmem && seekentry(i2i + 0x209c)) {
			i2len = storedn;
			fileread(packbuf, i2len, 1, datfh);
			// PORT: the cache as unsigned char, which C wants cast
#ifdef PORT
			facecache[i2i] = (unsigned char *)GlobalAlloc(GMEM_FIXED, i2len);
#else
			facecache[i2i] = (char *)GlobalAlloc(GMEM_FIXED, i2len);
#endif
		} else
			facecache[i2i] = NULL;
		if (facecache[i2i]) {
			cachebytes += i2len;
			facebytes += i2len;
			facescached++;
			memcpy(facecache[i2i], packbuf, i2len);
			facesizes[i2i] = i2len;
		} else
			facesizes[i2i] = 0;
	}
	if (facescached) {
		if (facescached >= 57)
			facescached = 2;
		else
			facescached = 1;
	}
	checkmem();
	overscached = 0;
	for (i2i = 0; i2i < 57; i2i++) {
		if (!minmem && !lowmem && seekentry(i2i + 0x1fb8)) {
			i2len = storedn;
			fileread(packbuf + 0x4b320, i2len, 1, datfh);
			if (i2len)
				i2len = unpack(packbuf, 0x4b320, packbuf + 0x4b320, i2len);
			// PORT: the cache as unsigned char, which C wants cast
#ifdef PORT
			overcache[i2i] = (unsigned char *)GlobalAlloc(GMEM_FIXED, i2len);
#else
			overcache[i2i] = (char *)GlobalAlloc(GMEM_FIXED, i2len);
#endif
		} else
			// FIXED: the entry cleared is facecache's, where overcache's is meant. Clear overcache's.
#if defined(FEATURE_FIXES)
			overcache[i2i] = NULL;
#else
			facecache[i2i] = NULL;
#endif
		if (overcache[i2i]) {
			cachebytes += i2len;
			overbytes += i2len;
			overscached++;
			memcpy(overcache[i2i], packbuf, i2len);
			oversizes[i2i] = i2len;
		} else
			oversizes[i2i] = 0;
	}
	if (overscached) {
		if (overscached >= 57)
			overscached = 2;
		else
			overscached = 1;
	}
	checkmem();
	if (!minmem && !lowmem) {
		travel1 = loadentry((unsigned long *)&travel1len, 0x206b);
		if (travel1)
			travelbytes += travel1len;
		travel2 = loadentry((unsigned long *)&travel2len, 0x206d);
		if (travel2)
			travelbytes += travel2len;
		travel3 = loadentry((unsigned long *)&travel3len, 0x206f);
		if (travel3)
			travelbytes += travel3len;
		cachebytes += travelbytes;
	}
	cdsaves();
	savesready = 1;
	for (shotno = 0; shotno < 999; shotno++) {
		wsprintf(tmpbuf, T(0xa4b1), shotno);
		if (!exists(tmpbuf))
			break;
	}
	copymode = 1;
	copyfile(T(0xa4d0), T(0x9bfd), workbuf, 0x96640);
	copymode = 0;
	xorstr(licenseno, maskedlic);
	readinput();
	loadscript(scriptfile);
	if ((execrc ^ 0x9f3ff2ff) != (crcword ^ 0xffcff3f))
		wrongcopy = 1;
	sysreport(scrbuf);
	droplf(scrbuf, workbuf);
	tocrlf(workbuf, scrbuf);
	savetext(T(0x9c13), scrbuf, 1, 0);
	loadgear();
	if (esckey == 0xff)
		return 0;
	gameready = 1;
	if (newgameopt || !loadgame(0)) {
		newgameopt = 1;
		if (esckey != 0xff)
			esckey = 0;
		if (!quickstart) {
			i2len = since(titletick);
			titlehold = 10000;
			if (i2len < titlehold) {
				i2len = titlehold - i2len;
				waitinput(i2len);
			}
			fadeslow(0);
			waitinput(0x352);
			credits();
			waitinput(0x352);
		}
		intro(1);
		waitinput(500);
		begingame(0, 2);
		starttimer3();
	} else {
		newgameopt = 0;
		if (!quickstart) {
			if (esckey) {
				credits();
				intro(2);
				starttimer3();
			} else {
				if (!quickstart) {
					i2len = since(titletick);
					titlehold = 5000;
					if (i2len < titlehold) {
						i2len = titlehold - i2len;
						waitinput(i2len);
					}
					fadeslow(0);
					waitinput(700);
					credits();
					waitinput(700);
				}
				intro(1);
			}
		} else
			starttimer3();
		if (esckey == 0xff)
			return 0;
	}
	usepal(0);
	fillgauges();
	lastslot = 0;
	hidecursor();
	setmouse(0x146, 0xf6);
	initdone = 1;
	nstarts++;
	fadefast(0);
	usepal(0);
	flip();
	setcursor1(3, 3);
	drawcursor();
	hidecursor();
	pagedone();
	if (chkpending || startpending || !udtaloaded || licpending || (execrc ^ 0x9f3ff2ff) != (crcword ^ 0xffcff3f))
		goto error;
	if (!newgameopt)
		return 1;
	return 3;
}

/* free what the game holds at its end */
int
exitsys(void)
{
	if (gameready && maploaded) {
		savegame();
		saveobjs(curmap, 0);
		savemap(curmap);
	}
	if (savesready) {
		runminutes = uptime();
		playtime += runminutes;
		savecfg();
		copymode = 2;
		copyfile(T(0x9bfd), T(0xa4d0), workbuf, 0x96640);
		copymode = 0;
	}
	if (modeset)
		hidecursor();
	unclip();
	ddquit();
	freesound();
	freesongs();
	unloadintro();
	cdprog();
	closedat();
	tilefree();
	freebank();
	if (travel0 != NULL) {
		GlobalFree(travel0);
		travel0 = NULL;
	}
	if (travel1 != NULL) {
		GlobalFree(travel1);
		travel1 = NULL;
	}
	if (travel2 != NULL) {
		GlobalFree(travel2);
		travel2 = NULL;
	}
	if (travel3 != NULL) {
		GlobalFree(travel3);
		travel3 = NULL;
	}
	for (runminutes = 0; runminutes < 57; runminutes++) {
		if (facecache[runminutes] != NULL) {
			GlobalFree(facecache[runminutes]);
			facecache[runminutes] = NULL;
		}
		if (overcache[runminutes] != NULL) {
			GlobalFree(overcache[runminutes]);
			overcache[runminutes] = NULL;
		}
	}
	for (runminutes = 0; runminutes < 78; runminutes++) {
		// PORT: partpics for Part's pics
#ifdef PORT
		if (partpics[runminutes] != NULL) {
			GlobalFree(partpics[runminutes]);
			partpics[runminutes] = NULL;
		}
#else
		if (partdefs[runminutes].pics != NULL) {
			GlobalFree(partdefs[runminutes].pics);
			partdefs[runminutes].pics = NULL;
		}
#endif
	}
	closemutex();
	return 1;
}

EXTERN_C const IID __cdecl IID_IMarshal = { 0x00000003, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IMalloc = { 0x00000002, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IMallocSpy = { 0x0000001d, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IStdMarshalInfo = { 0x00000018, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IExternalConnection = { 0x00000019, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IMultiQI = { 0x00000020, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumUnknown = { 0x00000100, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IBindCtx = { 0x0000000e, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumMoniker = { 0x00000102, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IRunnableObject = { 0x00000126, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IRunningObjectTable = { 0x00000010, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IPersist = { 0x0000010c, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IPersistStream = { 0x00000109, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IMoniker = { 0x0000000f, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IROTData = { 0xf29f6bc0, 0x5021, 0x11ce, { 0xaa, 0x15, 0x00, 0x00, 0x69, 0x01, 0x29, 0x3f } };
EXTERN_C const IID __cdecl IID_IEnumString = { 0x00000101, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_ISequentialStream = { 0x0c733a30, 0x2a1c, 0x11ce, { 0xad, 0xe5, 0x00, 0xaa, 0x00, 0x44, 0x77, 0x3d } };
EXTERN_C const IID __cdecl IID_IStream = { 0x0000000c, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumSTATSTG = { 0x0000000d, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IStorage = { 0x0000000b, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IPersistFile = { 0x0000010b, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IPersistStorage = { 0x0000010a, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_ILockBytes = { 0x0000000a, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumFORMATETC = { 0x00000103, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumSTATDATA = { 0x00000105, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IRootStorage = { 0x00000012, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IAdviseSink = { 0x0000010f, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IAdviseSink2 = { 0x00000125, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IDataObject = { 0x0000010e, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IDataAdviseHolder = { 0x00000110, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IMessageFilter = { 0x00000016, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IRpcChannelBuffer = { 0xd5f56b60, 0x593b, 0x101a, { 0xb5, 0x69, 0x08, 0x00, 0x2b, 0x2d, 0xbf, 0x7a } };
EXTERN_C const IID __cdecl IID_IRpcProxyBuffer = { 0xd5f56a34, 0x593b, 0x101a, { 0xb5, 0x69, 0x08, 0x00, 0x2b, 0x2d, 0xbf, 0x7a } };
EXTERN_C const IID __cdecl IID_IRpcStubBuffer = { 0xd5f56afc, 0x593b, 0x101a, { 0xb5, 0x69, 0x08, 0x00, 0x2b, 0x2d, 0xbf, 0x7a } };
EXTERN_C const IID __cdecl IID_IPSFactoryBuffer = { 0xd5f569d0, 0x593b, 0x101a, { 0xb5, 0x69, 0x08, 0x00, 0x2b, 0x2d, 0xbf, 0x7a } };
EXTERN_C const IID __cdecl IID_IPropertyStorage = { 0x00000138, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IPropertySetStorage = { 0x0000013a, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumSTATPROPSTG = { 0x00000139, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumSTATPROPSETSTG = { 0x0000013b, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IFillLockBytes = { 0x99caf010, 0x415e, 0x11cf, { 0x88, 0x14, 0x00, 0xaa, 0x00, 0xb5, 0x69, 0xf5 } };
EXTERN_C const IID __cdecl IID_IProgressNotify = { 0xa9d758a0, 0x4617, 0x11cf, { 0x95, 0xfc, 0x00, 0xaa, 0x00, 0x68, 0x0d, 0xb4 } };
EXTERN_C const IID __cdecl IID_ILayoutStorage = { 0x0e6d4d90, 0x6738, 0x11cf, { 0x96, 0x08, 0x00, 0xaa, 0x00, 0x68, 0x0d, 0xb4 } };
int chkpending = 1;
int startpending = 1;
int licpending = 1;
int titlesopt = 1;
int talkok = 1;
File files[30] = {
	texts + 40188, 0, 1, 0, 2, 0, texts + 39881, 0, 1, 0, 0, 0,
	texts + 39746, 0, 1, 0, 0, 0, texts + 39529, 0, 1, 0, 0, 0,
	texts + 39545, 0, 1, 0, 0, 0, texts + 39561, 0, 1, 0, 0, 0,
	texts + 39577, 0, 1, 0, 0, 0, texts + 39593, 0, 1, 0, 0, 0,
	texts + 39609, 0, 1, 0, 0, 0, texts + 39762, 0, 1, 0, 0, 0,
	texts + 39778, 0, 1, 0, 1, 1, texts + 40201, 0, 1, 0, 0, 0,
	texts + 40214, 0, 1, 0, 0, 0, texts + 40253, 0, 1, 0, 0, 0,
	texts + 39720, 0, 1, 0, 0, 1, texts + 40292, 0, 1, 0, 0, 0, "", 0, 1
};
unsigned long crcs[30] = {
	0xafd202f0, 0xfa1a5dc8, 0x24647bc3, 0xad6674f9, 0x20960ac7,
	0xb0229f74, 0xf287a2bd, 0xd52c7414, 0xd07d04f1, 0x5446b2b0,
	0xe9e127b2, 0x23e00ade, 0x799673fc, 0x87021c91, 0xf5e7ddeb,
	0x164f8d1e, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
};
unsigned long sizes[30] = {
	-16745, -30876, -716835, -563036, -208810, -1062212, -893206, -384276,
	-726729, -741875, -2872900, -32547, -29673, -30168, -1571329, -38401,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};
unsigned long minsize = 0xffffffff;
int sizeunused = 0x2710;
DWORD vermajor = 1;
int syslang = 1;	/* 004c384c */
int defid = 1;	/* 004c3850 */
char licensee[201] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	32, 32, 32, 32, 32, 32, 32
};
char *logp = dlgtext;	/* 004c3924 */
int textid = 1011;	/* 004c3928 */
int focusdef = 1;	/* 004c392c, the default button */
int focusid = 1;	/* 004c3930, the item that takes the focus */
Dataoff dataoff = {
	68, 97, 116, 97, 79, 102, 102, 115, 101, 116, 58, 58, 49, 50, 51, 52,
	0x212121, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
	0, 0, 0, 0, 104, 51, -30, 87, -70, -56, 1
};
char *logcur = logbuf;	/* 004c3ad8 */
int logpos = -1;	/* 004c3adc */
HANDLE scanfh;
DWORD scanpos;
DWORD sc[6];
int progpumps;
char *dlgtitle;	/* 009a1ec4 */
char *optword;
char *optscan;
char *errstart;
int optlast;
int memguard3;
int memguard2;
int memguard1;
int isi;
int isj;
short setuptries;
int cachelevel;
int i2i;
int cacheparts;
DWORD i2len;
DWORD titlehold;
DWORD titletick;
unsigned long runminutes;
int wrongcopy;
int gameready;
int savesready;
int quickstart;
int licseal;
int udtaloaded;
int newgameopt;
int filecheck;
int checkunused;
int soundoff;
int musicoff;
int startlevel;
int memunused;
int lowmem;
int licenseok;
int noprinter;
int featuresoff;
DWORD ordercmd;
int cfgsoundoff;
int cfgmusicoff;
int cfgnopani;
int cfgnopvari;
int cfgnoprun;
int cfgnopequip;
char linktarget[1000];
char exedir[1000];	/* 009a2358 */
char exepathbuf[1000];	/* 009a2740 */
char instdir[2000];
char instsaves[2000];
char instmenu[1000];
char winpath[2000];
char linkname[1000];
char setupbuf[3000];
char uninstpath[1000];
DWORD verminor;
DWORD verflags;
FILETIME buildtime;	/* 009a5a10 */
DWORD buildno;	/* 009a5a18 */
DWORD regmajor;
DWORD regminor;
DWORD regaddinfo;
char regapppath[1000];
char regsaves[1000];
char regmenu[1008];
FILETIME insttime;
DWORD instsize;
HKEY hkey;
int regunused;
int hasregver;
int hasreglang;
char regvalbuf[200];
int regvalflag;
int hasregsaves;
Reg reg;
int hasregdata;
DWORD licagreed;
int startcancel;
int setuppage;
int installing;	/* 009a67b0 */
int aborted;	/* 009a67b4 */
int asksaves;
int instfound;
int instnewer;
int instfull;
HWND dlg;	/* 009a67c8 */
int dlgunused[3];
HANDLE mutex;
char dlgtext[4000];	/* 009a67e0 */
char errlines[4000];	/* 009a7780 */
char errboxtext[4000];
char *browsestart;
int bytesdone;
HGLOBAL fdata_buf2;
HGLOBAL fdata_buf;
char logbuf[20484];	/* 009a96d0 */
int (*logcb)(char *, char *);	/* 009ae6d4 */
char *loghead;	/* 009ae6d8 */
int lognl;	/* 009ae6dc */
int logshow;	/* 009ae6e0 */
int logunused[301];
int winx;
int winy;
int winw;
int winh;
int afterupdate;
int afterinst;

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
// PORT: names of their own for lastx and lasty, which cursor.cpp defines with another type
#ifdef PORT
#define lastx menulastx
#define lasty menulasty
#endif

#pragma pack(1)

typedef struct Unit Unit;
typedef struct Region Region;
typedef struct Prov Prov;
typedef struct Zone Zone;
typedef struct Rect Rect;
typedef struct Savehdr Savehdr;
typedef struct Kind Kind;
typedef struct Realm Realm;
typedef struct Menu Menu;
typedef struct Fame Fame;
typedef struct Note Note;
typedef struct Game Game;
typedef struct Seal Seal;

struct Unit {
	unsigned char x;
	unsigned char y;
	char f2[2];
	unsigned char dirx;
	unsigned char diry;
	short picset;
	unsigned char pose;
	unsigned char mode;
	unsigned char modea;
	char fb[0xe9 - 0xb];
	unsigned short scene;
	short plea;
	short strength;
	short dexterity;
	short intellect;
	short health;
	short vigor;
	char leader;
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
	char f12c[0x131 - 0x12c];
	unsigned char clan;
	char f132[0x137 - 0x132];
	unsigned char blessed;
	unsigned char poison;
	char f139[0x13c - 0x139];
	unsigned char bleeding;
	unsigned char berserk;
	char f13e[0x140 - 0x13e];
	unsigned char warded;
	char f141[0x144 - 0x141];
	short mercy;
	unsigned char sympathy;
	char f147[0x14c - 0x147];
	unsigned char corpse;
	char f14d[0x161 - 0x14d];
	short face;
	short number;
	char f165[0x16a - 0x165];
	unsigned char sleepat;
	unsigned char workat1;
	unsigned char workat2;
	unsigned char eatat;
	unsigned char freeat;
	char f16f[0x1b8 - 0x16f];
};

struct Region {
	char f0[8];
	unsigned char realms[8];
	unsigned char lord;
	char f11[0x35 - 0x11];
	char tag;
	char f36[0x47 - 0x36];
	unsigned char units[16];
	unsigned char nunits;
	char f58[0x6e - 0x58];
	short id;
	char f70[0xd0 - 0x70];
};

struct Prov {
	char f0[0x7c];
	short id;
	char f7e[0x164 - 0x7e];
};

struct Zone {
	short id;
	unsigned short name;
	char f4[0x12 - 4];
	short inx;
	short iny;
	unsigned char indx;
	char f17;
	unsigned char indy;
	char f19;
	short x;
	short y;
	char f1e[0x22 - 0x1e];
};

struct Rect {
	unsigned short x;
	unsigned short y;
	unsigned short w;
	unsigned short h;
};

struct Savehdr {
	char name[0x14];
	short place;
	char hour;
	short day;
	char f19[0x5a - 0x19];
};

struct Kind {
	char f0[0x12];
	short strength;
	short dexterity;
	short intellect;
	short maxhealth;
	short maxvigor;
	unsigned char aptitude[0x41 - 0x1c];
};

struct Realm {
	unsigned long item;
	unsigned long items[50];
	unsigned short pos[50];
	char f130[0x158 - 0x130];
};

struct Menu {
	unsigned short n;
	unsigned char enabled[0x50];
	unsigned char keys2[10];
	Rect rects[50];
	char color;
	void (*fn)(short, short);
	short sel;
	short keysel;
	char f1f5[0x1f7 - 0x1f5];
};

struct Fame {
	unsigned short game;
	short unused;
	char name[0x14];
	unsigned long score;
};

/* the saved game, 0x5f8 bytes (009cc470) */
struct Game {
	char name[0x14];
	short look;
	char known[0x1e];
	char flags[0xc8];
	int skills[30];
	short tiredcount;
	unsigned char tiredlevel;
	short hungercount;
	unsigned char hungerlevel;
	unsigned char colors[4];
	short queuedtext;
	unsigned char queuedprio;
	unsigned short stolen;
	unsigned short killed;
	unsigned short sleepers;
	unsigned short ambushed;
	unsigned short broken;
	unsigned short defeated;
	unsigned long sacrificed;
	unsigned short invoked;
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
	char foes[30];
	char latinname[34];
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

struct Note {
	short id;
	unsigned char marks;
	char f3;
};

#pragma pack()

void syncplayer(void);
void closepanels(void);
void trackregion(void);
void sharecells(void);
void markunits(void);
void setcurregion(void);
void lookaround(void);
void lookdone(void);
void fillrect(int n, int l, int t, int r, int b, unsigned char color);
// PORT: a name of its own, which C wants for an overloaded function
#ifdef PORT
int waitnokey(void);
#else
int waitrelease(void);
#endif
void hidecursor(void);
void drawcursor(void);
short regionat(short x, short y);
short provat(short x, short y);
short ownerat(short x, short y);
void text2o(int n, unsigned off, char *fmt, ...);
void textat(unsigned pos, char *fmt, ...);
void drawtext2o(int n, unsigned off, char *s);
char *unitname(Unit *u);
void fmtgoods(char *buf, short k);
void primtoback(void);
void drawmap(int x, int y, short mode, short showunits);
int flip(void);
int __cdecl readinput(void);
char *pick(char *de, char *en, char *fr, char *es, char *it, char *pl);
char *titlehead(void);
char *titletail(void);
int updatereg(int x, char *s, char *dir);
void setlang(char c);
int savegame(void);
int saveobjs(unsigned short map, char flag);
int savemap(unsigned short map);
int cdprog(void);
int cdsaves(void);
void readmename(char *name);
char *backbutton(void);
int wrap(char *s, int width);
int fadefast(int in);
int textwin(char *s, char a, unsigned char b, unsigned char c, void (*pagefn)(short), Menu *m, int (*fn)(int), unsigned char mode);
DWORD availmem(void);
void soundinfo(void);
void musicinfo(void);
void unscrambleall(void);
short uptime(void);
char *hourstext(DWORD n);
void adjpal(int bright, int contrast, int r, int g, int b);
void freesound(void);
void setupsound(void);
int loadsounds(void);
void playnow(short n);
int stopmusic(void);
int music(int n, int lp);
void entermenu(void);
void nop(void);
void blankpage(void);
void menupage(void);
void listmenu(Menu *m, unsigned short n, char **items);
short runmenu(Menu *m, int (*idle)(void), short (*keyfn)(unsigned char, unsigned char), short cancel);
int needredraw(int c);
int setpal(void);
void inkwhite(void);
void inkshade(void);
int text3grey(int n, int x, int y, char *s);
int freebank(void);
int tilefree(void);
int tileinit(void);
int allocbank(void);
int togglemusic(void);
void buildrecipes(void);
void loadgear(void);
void resetdress(void);
void dresshero(void);
int autocopy(void);
int autosave(void);
void loadstrs(void);
int loadscript(char *name);
void freesurfs(void);
int ddinit(char *scr, unsigned char *pal);
void quitnow(void);
void drawscreen(void);
void expandstr(char *dst, char *src);
short unconscious(void);
void cleartexts(void);
void usepal(unsigned char n);
void pgtoback(void);
short mapzone(short cell);
int isbasemap(int cell);
int text3greyn(int n, int x, int y, char *fmt, ...);
void plotlabels(short cell);
void addcorner(Menu *m, short i, short kind);
int save0(void);
int showscr(void);
void setscr2o(int off, unsigned char c);
void puthands(void);
void backto2(void);
void pagesound(void);
void turnpage(int a, int b, int c, int d);
int __cdecl pollinput(void);
void clicksound(void);
short mayquit(short unused);
void quit(void);
short denymsg(char *fmt, ...);
void leavemenu(short anim);
void s2toback(void);
int pillshade8(int x, int y, int w, int h);
int setcursor(int kind, int img, int kind1, int img1, int kind2, int img2);
int putc1(int x, int y, int n);
void primto2(void);
int inputtext(short x, short y, char *s, short max, char mode, char flag, char toup, char filter, char cursor, short nfields, short maxw, short maxh, Menu *m);
void setbtncol(unsigned char c);
void clrbtncol(void);
void buttonon(int x, int y, int w, int h, char *s);
void buttonoff(int x, int y, int w, int h, char *s);
int text3grey2(int n, int x, int y, char *s);
void putcs(int x, int y, unsigned n);
void putpix0(int x, int y, unsigned char c);
void pagedone(void);
int langentry(int id);
int seekentry(int a);
DWORD unpack(void *dst, DWORD dstlen, void *src, DWORD srclen);
void cutchapters(char *text, char *note);
int checkquit(void);
void gettile(int s, int n, unsigned char *dst);
void clearqueue(void);
int loadgame(int n);
int loadmyface(void);
int loadface(unsigned char *dst, unsigned char *dst2, unsigned n);
void fillgauges(void);
int travel(unsigned short dest, int x, int y, unsigned long time, unsigned short depart, unsigned short a6, unsigned short mode, short a8);
void closeshop(void);
void listtorches(void);
void closeborder(void);
short msgf(short prio, char *fmt, ...);
void putpix1(int x, int y, unsigned char c);
void closedat(void);
int putgauge(int n, int x, int y, int v);
char *cancelbutton(void);
short msgbox(char *s, unsigned char c, char b);
int cdmaps(void);
void setcombat(short n);
void facetmpl(int alt);
unsigned short newunit(unsigned char kind, unsigned char type);
void delunit(short n);
void resetanims(void);
unsigned long puton(short u, unsigned long item);
void calcstats(Unit *u);
void initpanels(void);
Kind *kindof(Unit *u);
void clearavenge(void);
int packitem(int item);
unsigned long newitem(unsigned long item);
void setwear(unsigned long *p, unsigned long v);
short istorch(unsigned long a);
void setlevels(void);
void snapskills(int a);
void pickcall(void);
void openhero(void);
void homepanels(void);
void homepanel2(void);
void setdaylight(void);
void clearmsgs(void);
void clearturn(void);
void clear48(int x, int y);
void drawoutfit(int x, int y, Unit *u, short items, short flag);
int put0(char *buf);
int putwint(int x, int y, int idx);
void text3e1(int n, int x, int y, char *s);
void putface(int x, int y, char *src);
int textwidth(char *s);
void intro(unsigned char mode);
void clearvars(char *buf);
unsigned long crcname(char *name, int skip, int end);
unsigned long crcbuf(void *buf, int n);
int loadscr(int id, unsigned char *dst, unsigned char *pal, void *buf, DWORD size);
int fadeslow(int in);
void inkdark(void);
void waitinput(int ms);
void dissolvet(void);
void picink(void);
void setmouse(short x, short y);
int exportface(void);
void waitrelease(short anim);
void setflag(int n, int v);
int put48t0(int x, int y, char *src);
void pickworn(unsigned long item, short flag);
char *roman(DWORD n);
char *daytime(unsigned char h);
void keepui(char *p);
int fademed(int in);
unsigned long popitem(int pos, char all);
unsigned char classof(int i);
unsigned short itemweight(unsigned long item);
unsigned long giveitem(short n, unsigned long item, short mode);
void restfor(short n);
void quitnow(void);
short confirm(char *s);
void menutitle(void);
int putwh(unsigned char *src, int n, int x, int y, int w, int h);
void putfacen(int n, int x, int y, char *src);
void faceshadow(int x, int y, char *src);
int getwh(int n, int x, int y, unsigned char *dst, int w, int h);
int putherostates(int a, int b, int c, int d, int e);
int badunit(Unit *u);
short isunlittorch(unsigned long item);
short setlit(unsigned long *p, short dir);
void statustext(char *buf, Unit *u);
char *hungertext(unsigned char n);
char *tiredtext(unsigned char n);
void immobtext(char *buf, Unit *u);
void immobcause(char *buf);
void clearstatus(Unit *u);
void attackval(Unit *u, short *dmg, short *kind, short *a, short *b, short n);
void defenseval(Unit *u, short *a, short *b, short *c);
char *itemname(unsigned long item);
void dmgrange(char *buf, short a, short b);
char *carrytext(Unit *u, short n);
char *ratetext(short n);
char *vitaltext(short n);
short protection(unsigned long item);
char *armortext(short n);
short skillof(Unit *u, short n, short b, short c, short d);
short baseskill(Unit *u, short k);
char *timestext(short n);
int isdebug(void);
void epitaph(char *buf);
short tribefate(void);
void addhiscore(void);
void killunit(Unit *u, short a, short b, short c, short d);
void fatetext(char *buf);
void setunitmode(Unit *u, unsigned char mode);
void clearmind(Unit *u);
void clrback(void);
void melt(void);
int palblk(void);
unsigned long passtime(unsigned long t, short ask, short b);
void blitn(int a, int x, int y, int b, int dx, int dy, int w, int h);
int showscrt(void);
short drawpage(char **pp, unsigned short x, short mode, short top, short maxh, Menu *m);
void addscore(int n);
void scorenote(int n);
void deedquest(int n);
void learnquest(void);
void clearmenu(Menu *m);
void addbutton(Menu *m, short i, int x, int y, char *s, short right);
int text3(int n, int x, int y, char *s);
void pagenum(int page, int pages);
void pillring(int x, int y, int w, int h);
void setcursor1(int kind, int img);
void linebegin(int x1, int y1, int x2, int y2);
void linestep(void);
void setclip(short x0, short y0, short x1, short y1);
void drawmenubar(void);
void drawstats(void);
void drawinventory(int a);
void drawstore(int a);
int drawactions(void);
void hoversound(void);
char *nextbutton(void);
void putpage(char *s, char a, char b);
void pagein(void);
short runpage(int a, int (*fn)(void));
void demonote(short game, short offline);
void aboutdone(void);

extern Unit units[];
extern Zone zones[30];
extern Menu menu1;
extern Menu menu2;
extern Region regions[];
extern Prov provs[];
extern int mousex;
extern int mousey;
extern int mousemoved;
extern int mouseleft;
extern int mouseright;
extern int leftclick;
extern int shown;
extern unsigned char keys[256];
extern char keycode;
extern unsigned char esckey;
extern int cheat;
extern unsigned short curmap;
extern char namebuf[];
extern char tmpbuf[];
extern char printbuf[];
extern char texts[47942];
extern char version[];
extern char empty[];
extern char vertext[];
extern char licenseno[];
extern int soundoff;
extern int cfgsoundoff;
extern int cfgmusicoff;
extern int cfgnopani;
extern int cfgnopvari;
extern int cfgnoprun;
extern int cfgnopequip;
extern int musicoff;
extern int nopani;
extern int nopequip;
extern int nopvari;
extern int noprun;
extern short gamesbegun;
extern int autolevel;
extern short censorship;
extern int playtime;
extern int lang;
extern char langchar;
extern char langset;
extern char pounds;
extern char poundsset;
extern char progdir[];
extern char savesdir[];
extern Fame hiscores[13];
extern int optbright;
extern int optcontrast;
extern int optred;
extern int optgreen;
extern int optblue;
extern char savefail;
extern char newcfg;
extern char readonlydir;
extern unsigned char cmbautooff;
extern short popupdelay;
extern char quitprompt;
extern short cfgword6;
extern short cfgword5;
extern char landsounds;
extern unsigned char sfxswitch;
extern char menusounds;
extern int sfxvolume;
extern short blasterport;
extern char blasterdma;
extern char blasterirq;
extern unsigned char soundson;
extern short outalways;
extern int musicvolume;
extern int cfglong4;
extern short textbg;
extern short pageturn;
extern short scrolling;
extern char mapframed;
extern short cfgword1;
extern short nstarts;
extern short gfxram;
extern short printchoice;
extern char cfgtext1[];
extern char cfgtext2[];
extern char cfgtext3[];
extern char cfgtext4[];
extern char cfgtext5[];
extern char cfgtext6[];
extern char cfgtext7[];
extern short cfgword2;
extern short cfgword3;
extern short cfgword4;
extern short cfgword7;
extern int cfglong1;
extern char cfgbyte1;
extern char cfgbyte2;
extern char callsseen;
extern int cfglong2;
extern int cfglong5;
extern int cfglong3;
extern int maploaded;
extern char udstr0[92];
extern char udstr1[52];
extern char udstr2[82];
extern char udstr3[22];
extern char vramtext[2000];
extern char blittext[2000];
// FIXED: spritetext holds outlinecols and ink3cols in the port
#ifdef PORT
extern char spritetext[2008];
#else
extern char spritetext[2000];
#endif
extern char soundtext[2000];
extern char capstext[2000];
extern char musictext[2000];
extern MEMORYSTATUS memstat;
extern unsigned short walkdelay;
extern unsigned short standdelay;
extern short maxmsgs;
extern char dsready;
extern int featuresoff;
extern int musloaded;
extern char workbuf[];
extern int noflip;
extern short listbright;
extern short curtopic;
extern short pagefx;
extern short listw;
extern char scrbuf[309700];
extern char dsshort[32];
extern char gamepal[768];
extern char scriptfile[];
extern unsigned char showtotal;
extern int minmem;
extern short nmarks;
extern char marks[][20];
extern short markx[];
extern short marky[];
extern char marklabel[];
extern char textbuf[];
extern int hoverx;
extern int hovery;
extern char tabkey;
extern short lastlight;
extern unsigned char palblack;
extern int waitforup;
extern unsigned short busyturns;
extern int playmode;
extern char *textlines[100];
extern char *texttitle;
extern short ntextlines;
extern char notopic[];
extern unsigned char topicof[];
extern short topicfirst;
extern short ntopics;
extern short topicprev;
extern short topicnext;
extern unsigned short pagearrows;
extern char chcut[200];
extern unsigned char oddsswitch;
extern DWORD storedn;
extern HANDLE datfh;
extern char packbuf[];
extern int quitting;
extern int hasgearbank;
extern int setpics;
extern DWORD ntiles;
extern int sfxbytes;
extern int musicbytes;
extern int cachebytes;
extern char partscached;
extern char facescached;
extern char overscached;
extern DWORD partbytes;
extern DWORD facebytes;
extern DWORD overbytes;
extern DWORD travelbytes;
extern char *strs[];
extern int minsets;
extern int nofani;
extern int playerx;
extern int playery;
extern unsigned short playercell;
extern unsigned char headicons[8][2304];
extern unsigned char firststart;
extern unsigned char enterflag;
extern unsigned short enteritems;
extern int entersum;
extern int myface;
extern int photosum;
extern DWORD myfacesum;
extern char heroface[];
extern char heroface2[];
extern Game game;
extern char trvfailmsg;
extern unsigned long mapsum;
extern int afterupdate;
extern int afterinst;
extern int otherver;
extern unsigned short nmapitems;
extern int cur;
extern int loop;
extern unsigned char gamehour;
extern short gameday;
extern unsigned short lastslot;
extern short autoslot;
extern char savename[];
extern char autoname[];
extern int copymode;
extern int modeset;
extern char szvars[];
extern Kind kinds[51];
extern int bloodtill;
extern int notes[500];
extern unsigned short nnotes;
extern unsigned char minmarks;
extern Realm realms[];
extern short curregion;
extern unsigned long gametime;
extern unsigned short steptick;
extern unsigned short stepframe;
extern int turncount;
extern int hourtime;
extern short combatkind;
extern unsigned char panelstack[16];
extern unsigned char quickacts[10];
extern unsigned char lookcolors[4][8];
extern unsigned char shoewarns;
extern short regiontag;
extern int statshown;
extern short lastwarm;
extern short sensesleft;
extern char talkactive;
extern unsigned char invram[8];
extern DWORD blittime;
extern DWORD keyblittime;
extern unsigned char notedkind;
extern unsigned short notedunit;
extern short askedunit;
extern short notedmap;
extern short spotday;
extern int notedposx;
extern int notedposy;
extern unsigned long noteditem;
extern unsigned long notedtime;
extern char spotsong;
extern short askedonce;
extern char savebyte3;
extern char knockedout;
extern char mapstate[100];
extern short merciful;
extern short pleader;
extern short talker;
extern char picbuf[];
extern short curblink;
extern Seal seal;
extern char exepathbuf[1000];
extern unsigned char lum[256];
extern unsigned char darker4[256];
extern char menuslow;
extern short menusnd;
extern short menuanim;
extern int inkeep;
extern char lastscan;
extern Menu menu0;
extern int text3x;
extern int wornpic;
extern short wornx;
extern short worny;
extern char *months[14];
// FEATURE: features: the languages and texts of the build, in src/feature/feature.cpp
#ifdef FEATURES
int featlang(int c);
char *feattext(char *s);
#endif
// FEATURE: french: the elision of French, in src/feature/french.c
#ifdef FEATURE_FRENCH
EXTERN_C void frelide(char *s);
#endif
extern char romanbuf[];
extern short pagex;
extern short pagemaxh;
extern short nofade;
extern short overpic;
extern short textsong;
extern DWORD ordercmd;
extern DWORD aboutticks;
extern Unit *sheetunit;
extern char sheetdead;
extern char faceback[];
extern int sheetoff;
extern unsigned char skillsnow;
extern unsigned char combatnow;
extern short playerrank;
extern void (*menudraw)(void);
extern int lostcontrol;
extern unsigned short hourticks;
extern int waitlost;
extern short fatepic;
extern int jourturn;
extern short startpage;
extern short questsel;
extern int text3y;
extern int linestepx;
extern int linestepy;
extern int linesteps;
extern short ctrlhint;

extern short mapmode;
extern short mapflag;
extern short mapreg;
extern short mapprov;
extern short mapi;
extern short mapunit;
extern short lastreg;
extern short lastprov;
extern short mapsaved;
extern int mappos;
extern unsigned char mapkey;
extern int localen;
extern HANDLE readmefile;
extern DWORD readmelen;
extern char readmebuf[28];
extern int totaltime;
extern OSVERSIONINFO osv;
extern SYSTEM_INFO sysinfo;
extern int reportres;
extern unsigned short optn;
extern short optchanged;
extern short redress;
extern char *optitems[20];
extern unsigned char optsel;
extern char *langname;
extern int oldauto;
extern char optextra;
extern short marki;
extern short markstr;
extern short markj;
extern short markmode;
extern short lastx;
extern short lasty;
extern short hover;
extern int markv;
extern short markhi;
extern char *splitp;
extern short spliti;
extern short topiccut;
extern short pagecol;
extern short pagerow;
extern short pageline;
extern char *topicname;
extern short textsel;
extern short turndir;
extern int doclen;
extern int doclen2;
extern int docpick;
extern unsigned short resumemap;
extern int entermapsum;
extern Savehdr savehdr;
extern short descplace;
extern Savehdr *hdr;
extern short drawbarrow;
extern short drawbarcol;
extern short lookword;
extern unsigned short strword;
extern HANDLE outsav;
extern HANDLE savein;
extern DWORD savelen;
extern unsigned short savemapi;
extern unsigned short savekind;
extern DWORD copylen;
extern HANDLE copyh;
extern HANDLE autofile;
extern int loadmapi;
extern int loadkind;
extern int copymapi;
extern int copykind;
extern HANDLE insav;
extern HANDLE loadout;
extern DWORD slotpart;
extern unsigned short loadlook;
extern unsigned short loadk;
extern unsigned short loadver;
extern int loadauto;
extern short newi;
extern short newj;
extern char *latinp;
extern char *latinq;
extern short hinti;
extern short hintw;
extern short hintws[7];
extern char heroname[28];
extern DWORD begini;
extern char *namep;
extern short beginv;
extern short ownphoto;
extern short beginredo;
extern char hdrname[16];
extern HANDLE hdrfile;
extern unsigned short hdrlook;
extern unsigned short hdrver;
extern int slothelm;
extern unsigned short sloti;
extern unsigned short slotyear;
extern unsigned short slotmon;
extern unsigned short slotday;
extern unsigned short slothour;
extern unsigned short slotmin;
extern unsigned short slotsec;
extern char slottext[88];
extern short slotunused;
extern short slotplace;
extern short slotsel;
extern short slotturn;
extern DWORD readlen;
extern char *readp;
extern short ordering;
extern int fullres;
extern short mainsel;
extern short mainexit;
extern char *mainitems[20];
extern short sheetgear;
extern short sheetbars;
extern short sheetvital;
extern int sheetbarx;
extern int sheetbarw;
extern unsigned long famei;
extern char *famep;
extern char *colp;
extern char sheetstatus[256];
extern char sheettired[40];
extern char sheetimmob[64];
extern char sheetname[24];
// FIXED: as at its definition below, sheetweapon's room for any weapon's name
#ifdef FEATURE_FAULTS
extern char sheetweapon[96];
#else
extern char sheetweapon[32];
#endif
extern char sheetdmg[32];
extern char sheetshield[32];
extern char sheethelm[32];
extern char sheetarmor[32];
extern char sheetpants[25];
extern char hasshield;
extern char skillmarks[30][8];
extern char sheetcause[40];
extern char sheetepitaph[240];
// FIXED: as at its definition below, sheetverdict's room for the verdicts with their marks
#ifdef FEATURE_FAULTS
extern char sheetverdict[240];
#else
extern char sheetverdict[120];
#endif
extern char sheetfate[120];
extern short curatk;
extern short curdodge;
extern short curblock;
extern short curparry;
extern short curdmin;
extern short curdmax;
extern short sheetkind;
extern short baseatk;
extern short basedodge;
extern short baseblock;
extern short baseparry;
extern short basedmin;
extern short basedmax;
extern DWORD sheetv;
extern char sheetmode;
extern char *sheetp;
extern short sheeti;
extern short ndeeds;
extern short deedmark;
extern unsigned char oldskills;
extern unsigned char oldcombat;
extern short sheethunger;
extern short sheetweary;
extern int skilldelta;
extern short outhours;
extern char *fatep;
extern short fatesong;
extern char *notep;
extern short notei;
extern char *noteq;
extern short hasi;
extern short dropi;
extern char *bangp;
extern short nbangs;
extern short closei;
extern short closetry;
extern short closej;
extern short jouri;
extern short jourj;
extern short jourbtn;
extern short jourunused;
extern short jourfirst;
extern short jourleft;
extern short jourright;
extern short jourmost;
extern short jourimp;
extern short jourall;
extern short jourdel;
extern short jourback;
extern char *jourq;
extern char *jourp;
extern short jourlist[500];
extern short njour;
extern char jourmarks[20];
extern short jourtop;
extern short jourend;
extern DWORD glidetime;
extern short ctrlnew;
extern char *ctrltext;
extern int ctrlres;
extern short ctrlcur;
extern int demores;
extern char *docitems[20];
extern unsigned short docsel;
extern unsigned short docres;
extern short ctrlarmed;

void
encodenum(char *buf, unsigned long n)
{
	char c;

	if (n >= 100000) {
		wsprintf(buf, texts + 0xb08d, n);
		return;
	}
	wsprintf(buf, texts + 0xb090, n);
	c = buf[0];
	buf[0] = buf[1];
	buf[1] = buf[2];
	buf[2] = buf[3];
	buf[3] = c;
	if (buf[0] == '9')
		buf[0] = '0';
	else
		buf[0]++;
}

long
decodenum(char *s)
{
	int n;
	char *p, *q, c;

	n = 0;
	for (q = s; *q; q++, n++)
		;
	if (n > 5)
		return atol(s);
	p = tmpbuf;
	if (n < 5) {
		for (q = p; n < 5; n++)
			*q++ = '0';
		wsprintf(q, s);
	} else
		wsprintf(p, s);
	if (p[0] == '0')
		p[0] = '9';
	else
		p[0]--;
	c = p[3];
	p[3] = p[2];
	p[2] = p[1];
	p[1] = p[0];
	p[0] = c;
	for (; *p == '0'; p++)
		;
	return atol(p);
}

void
jumpto(int x, int y)
{
	syncplayer();
	closepanels();
	playery = y;
	playerx = x;
	playercell = playerx + playery * 150;
	units[0].x = playerx;
	units[0].y = playery;
	units[0].pose = 0;
	trackregion();
	sharecells();
	markunits();
	setcurregion();
	lookaround();
	lookdone();
}

void
clearpane(void)
{
	fillrect(1, 0x197, 0, 0x280, 0x1e0, 0);
}

char
pickmap(void)
{
	mapmode = 1;
	mapflag = 0;
	while (mouseleft != 0 || mouseright != 0)
		;
	leftclick = 0;
	mousemoved = 1;
	lastreg = 0xff;
	lastprov = 0xff;
	// PORT: a name of its own, which C wants for an overloaded function
#ifdef PORT
	waitnokey();
#else
	waitrelease();
#endif
	for (;;) {
		if (keys[VK_SHIFT] & 0x80)
			;
		else if (lastreg != 0xff || lastprov != 0xff) {
			hidecursor();
			clearpane();
			drawcursor();
			lastreg = 0xff;
			lastprov = 0xff;
		}
		if (mousemoved) {
			hidecursor();
			if (mousex < 100)
				mousex = 100;
			else if (mousex > 399)
				mousex = 399;
			if (mousey < 90)
				mousey = 90;
			else if (mousey > 389)
				mousey = 389;
			mapreg = regionat((mousex - 100) / 2, (mousey - 90) / 2);
			mapprov = provat((mousex - 100) / 2, (mousey - 90) / 2);
			if ((mapreg != lastreg || mapprov != lastprov) && (keys[VK_SHIFT] & 0x80)) {
				clearpane();
				text2o(1, 0x6598, texts + 0xb3d3, mapreg, mapreg ? regions[mapreg].id : 0);
				text2o(1, 0x8398, texts + 0xb3dd, mapprov, mapprov ? provs[mapprov].id : 0);
				for (mapi = 0; mapi < regions[mapreg].nunits && mapi * 28 + 100 < 460; mapi++) {
					mapunit = regions[mapreg].units[mapi];
					mapsaved = units[regions[mapreg].units[mapi]].number;
					if (mapsaved)
						units[mapunit].number |= 0x2000;
					copystr(namebuf, unitname(&units[mapunit]));
					units[mapunit].number = mapsaved;
					text2o(1, (mapi * 28 + 88) * 640 + 408, texts + 0xb3c7, mapunit, namebuf, units[mapunit].scene);
					text2o(1, (mapi * 28 + 100) * 640 + 408, texts + 0xb3e7, units[mapunit].sleepat, units[mapunit].workat1, units[mapunit].workat2, units[mapunit].eatat, units[mapunit].freeat);
				}
				if (mapreg) {
					mappos = 0x47598;
					for (mapi = 7; mapi >= 0; mapi--) {
						if (!regions[mapreg].realms[mapi])
							continue;
						fmtgoods(namebuf, regions[mapreg].realms[mapi]);
						if (!namebuf[0])
							continue;
						wsprintf(printbuf, "B%d.%s", regions[mapreg].realms[mapi], namebuf);
						printbuf[45] = 0;
						drawtext2o(1, mappos, printbuf);
						mappos -= 0x1e00;
					}
				}
				lastreg = mapreg;
				lastprov = mapprov;
			}
			textat(0xb2, texts + 0xb112, curmap, (mousex - 100) / 2, (mousey - 90) / 2, mapprov, mapreg, mapreg ? 'E' : 'e', mapreg ? regions[mapreg].lord : ownerat((mousex - 100) / 2, (mousey - 90) / 2));
			drawcursor();
			mousemoved = 0;
		}
		if (esckey)
			break;
		mapkey = keycode;
		if (mapkey == 0xf || mapkey == 0x39)
			break;
		if (cheat && mapkey == 0x26) {
			mapmode = 0xff;
			mapflag = 0;
redraw:
			hidecursor();
			primtoback();
			drawmap(100, 90, mapmode, mapflag);
			flip();
			drawcursor();
			// PORT: a name of its own, which C wants for an overloaded function
#ifdef PORT
			waitnokey();
#else
			waitrelease();
#endif
		} else if (cheat && mapkey == 0x22) {
			mapmode = 0xfe;
			mapflag = 1;
			goto redraw;
		} else if (cheat && mapkey == 0x31) {
			mapmode = 0;
			mapflag = 1;
			goto redraw;
		} else if (mapkey)
			break;
		readinput();
		shown = 1;
		if (mouseleft || mouseright || esckey)
			break;
	}
	if (mouseleft) {
		playerx = (mousex - 100) / 2;
		playery = (mousey - 90) / 2;
		if (playerx < 5)
			playerx = 5;
		else if (playerx > 0x90)
			playerx = 0x90;
		if (playery < 4)
			playery = 4;
		else if (playery > 0x91)
			playery = 0x91;
		jumpto(playerx, playery);
	}
	return 0;
}

void
verstr(char *buf, short n)
{
	if (n <= 1) {
		wsprintf(buf, texts + 0xb288, titlehead(), pick(texts + 0x4d6, texts + 0x4e2, texts + 0x4ef, texts + 0x4f0, texts + 0x4f1, texts + 0x503), version[14] == 'e' ? (texts + 0x6b5b) : empty, vertext, empty);
		return;
	}
	wsprintf(buf, texts + 0xb2c1, titlehead(), titletail(), pick(texts + 0x4d6, texts + 0x4e2, texts + 0x4ef, texts + 0x4f0, texts + 0x4f1, texts + 0x503), version[14] == 'e' ? (texts + 0x6b5b) : empty, vertext);
	for (; *buf; buf++)
		;
	wsprintf(buf, pick(texts + 0x7d87, texts + 0x7d97, texts + 0x7da8, texts + 0x7da9, texts + 0x7daa, texts + 0x7dbb), licenseno);
}

void
savemodes(void)
{
	cfgsoundoff = soundoff;
	cfgmusicoff = musicoff;
	cfgnopani = nopani;
	cfgnopvari = nopvari;
	cfgnoprun = noprun;
	cfgnopequip = nopequip;
}

void
restoremodes(void)
{
	if (cfgsoundoff == 2)
		soundoff = 2;
	if (cfgmusicoff == 2)
		musicoff = 2;
	if (cfgnopani == 2)
		nopani = 2;
	if (cfgnopvari == 2)
		nopvari = 2;
	if (cfgnoprun == 2)
		noprun = 2;
	if (cfgnopequip == 2)
		nopequip = 2;
}

void
defaultcfg(void)
{
	gamesbegun = 0;
	playtime = 0;
	autolevel = 1;
	censorship = 0;
	localen = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGCOUNTRY, tmpbuf, 2500);
	if (localen < 0)
		return;
	tmpbuf[localen] = 0;
	if (streqi(tmpbuf, texts + 0x8c6b))
		censorship = 2;
}

int
savecfg(void)
{
	HANDLE h;
	char *p;
	int v;

	savemodes();
	updatereg(0, savesdir, progdir);
	fopenw(&h, texts + 0x9bd3);
	if (!h)
		return 0;
	filewrite(&langset, 1, 1, h);
	memset(printbuf, 0, 0x960);
	p = printbuf;
	memwrite(&lang, 4, 1, &p);
	memwrite(&langchar, 1, 1, &p);
	memwrite(&soundson, 1, 1, &p);
	memwrite(&menusounds, 1, 1, &p);
	memwrite(namebuf, 2, 1, &p);
	memwrite(&walkdelay, 2, 1, &p);
	memwrite(&standdelay, 2, 1, &p);
	memwrite(&scrolling, 2, 1, &p);
	memwrite(&mapframed, 1, 1, &p);
	memwrite(&popupdelay, 2, 1, &p);
	memwrite(&pounds, 1, 1, &p);
	memwrite(&langset, 1, 1, &p);
	memwrite(tmpbuf, 2, 1, &p);
	memwrite(&cmbautooff, 1, 1, &p);
	memwrite(&cfgword1, 2, 1, &p);
	memwrite(&playtime, 4, 1, &p);
	memwrite(&nstarts, 2, 1, &p);
	xmemwrite(cfgtext1, 50, 1, &p);
	xmemwrite(cfgtext2, 50, 1, &p);
	xmemwrite(cfgtext3, 50, 1, &p);
	xmemwrite(cfgtext4, 50, 1, &p);
	xmemwrite(cfgtext5, 50, 1, &p);
	xmemwrite(cfgtext6, 50, 1, &p);
	xmemwrite(cfgtext7, 50, 1, &p);
	memwrite(&printchoice, 2, 1, &p);
	memwrite(&cfgword2, 2, 1, &p);
	memwrite(&cfgword3, 2, 1, &p);
	memwrite(&cfgword4, 2, 1, &p);
	memwrite(&cfgword5, 2, 1, &p);
	memwrite(&cfgword6, 2, 1, &p);
	memwrite(&cfgword7, 2, 1, &p);
	memwrite(&cfglong1, 4, 1, &p);
	memwrite(&landsounds, 1, 1, &p);
	memwrite(&sfxswitch, 1, 1, &p);
	memwrite(&gamesbegun, 2, 1, &p);
	memwrite(&cfgbyte1, 1, 1, &p);
	memwrite(&blasterport, 2, 1, &p);
	memwrite(&blasterirq, 1, 1, &p);
	memwrite(&blasterdma, 1, 1, &p);
	memwrite(&cfgbyte2, 1, 1, &p);
	// FIXED: quitnow adds 2 to the prompt option, which mayquit takes back, so a quit through quitnow saves the option plus 2. Save the option.
#if defined(FEATURE_FIXES)
	{
		char q = quitprompt >= 2 ? quitprompt - 2 : quitprompt;

		memwrite(&q, 1, 1, &p);
	}
#else
	memwrite(&quitprompt, 1, 1, &p);
#endif
	memwrite(&cfglong2, 4, 1, &p);
	memwrite(&cfglong3, 4, 1, &p);
	memwrite(&cfglong4, 4, 1, &p);
	memwrite(&cfglong5, 4, 1, &p);
	memwrite(&cfgsoundoff, 4, 1, &p);
	memwrite(&cfgmusicoff, 4, 1, &p);
	memwrite(&cfgnopani, 4, 1, &p);
	memwrite(&cfgnopvari, 4, 1, &p);
	memwrite(&cfgnoprun, 4, 1, &p);
	memwrite(&cfgnopequip, 4, 1, &p);
	memwrite(&gfxram, 2, 1, &p);
	memwrite(&musicvolume, 4, 1, &p);
	memwrite(&outalways, 2, 1, &p);
	memwrite(&sfxvolume, 4, 1, &p);
	memwrite(&censorship, 2, 1, &p);
	memwrite(&textbg, 2, 1, &p);
	memwrite(&pageturn, 2, 1, &p);
	memwrite(&callsseen, 1, 1, &p);
	v = 0xfff0101;
	memwrite(&v, 4, 1, &p);
	v = optbright - 100;
	memwrite(&v, 4, 1, &p);
	v = optcontrast - 100;
	memwrite(&v, 4, 1, &p);
	v = optred - 100;
	memwrite(&v, 4, 1, &p);
	v = optgreen - 100;
	memwrite(&v, 4, 1, &p);
	v = optblue - 100;
	memwrite(&v, 4, 1, &p);
	filewrite(printbuf, 1999, 1, h);
	fileclose(&h);
	fopenw(&h, texts + 0x9be6);
	if (!h)
		return 0;
	filewrite(((char *)hiscores), 13, 0x1c, h);
	fileclose(&h);
	return 1;
}

short
loadcfg(void)
{
	HANDLE h;
	char *p;
	DWORD v;
	int w;

	memset(((char *)hiscores), 0, 0x16c);
	fopenr(&h, texts + 0x9bd3);
	if (!h)
		return 0;
	fileread(&langset, 1, 1, h);
	memset(printbuf, 0, 0x960);
	p = printbuf;
	fileread(printbuf, 1999, 1, h);
	fileclose(&h);
	memread(&v, 4, 1, &p);
	memread(&v, 1, 1, &p);
	setlang(lang);
	memread(&soundson, 1, 1, &p);
	memread(&menusounds, 1, 1, &p);
	memread(&v, 2, 1, &p);
	memread(&walkdelay, 2, 1, &p);
	memread(&standdelay, 2, 1, &p);
	memread(&scrolling, 2, 1, &p);
	memread(&mapframed, 1, 1, &p);
	memread(&popupdelay, 2, 1, &p);
	memread(&pounds, 1, 1, &p);
	poundsset = 1;
	memread(&langset, 1, 1, &p);
	memread(tmpbuf, 2, 1, &p);
	memread(&cmbautooff, 1, 1, &p);
	memread(&cfgword1, 2, 1, &p);
	memread(&playtime, 4, 1, &p);
	memread(&nstarts, 2, 1, &p);
	xmemread(cfgtext1, 50, 1, &p);
	xmemread(cfgtext2, 50, 1, &p);
	xmemread(cfgtext3, 50, 1, &p);
	xmemread(cfgtext4, 50, 1, &p);
	xmemread(cfgtext5, 50, 1, &p);
	xmemread(cfgtext6, 50, 1, &p);
	xmemread(cfgtext7, 50, 1, &p);
	memread(&printchoice, 2, 1, &p);
	memread(&cfgword2, 2, 1, &p);
	memread(&cfgword3, 2, 1, &p);
	memread(&cfgword4, 2, 1, &p);
	memread(&cfgword5, 2, 1, &p);
	memread(&cfgword6, 2, 1, &p);
	memread(&cfgword7, 2, 1, &p);
	memread(&cfglong1, 4, 1, &p);
	memread(&landsounds, 1, 1, &p);
	memread(&sfxswitch, 1, 1, &p);
	memread(&gamesbegun, 2, 1, &p);
	memread(&cfgbyte1, 1, 1, &p);
	memread(&blasterport, 2, 1, &p);
	memread(&blasterirq, 1, 1, &p);
	memread(&blasterdma, 1, 1, &p);
	memread(&cfgbyte2, 1, 1, &p);
	memread(&quitprompt, 1, 1, &p);
	memread(&cfglong2, 4, 1, &p);
	memread(&cfglong3, 4, 1, &p);
	memread(&cfglong4, 4, 1, &p);
	memread(&cfglong5, 4, 1, &p);
	memread(&cfgsoundoff, 4, 1, &p);
	memread(&cfgmusicoff, 4, 1, &p);
	memread(&cfgnopani, 4, 1, &p);
	memread(&cfgnopvari, 4, 1, &p);
	memread(&cfgnoprun, 4, 1, &p);
	memread(&cfgnopequip, 4, 1, &p);
	memread(&gfxram, 2, 1, &p);
	memread(&musicvolume, 4, 1, &p);
	memread(&outalways, 2, 1, &p);
	memread(&sfxvolume, 4, 1, &p);
	memread(&censorship, 2, 1, &p);
	memread(&textbg, 2, 1, &p);
	memread(&pageturn, 2, 1, &p);
	memread(&callsseen, 1, 1, &p);
	memread(&w, 4, 1, &p);
	if (w == 0xfff0101) {
		memread(&w, 4, 1, &p);
		optbright = w + 100;
		if (optbright < 40)
			optbright = 40;
		else if (optbright > 300)
			optbright = 300;
		memread(&w, 4, 1, &p);
		optcontrast = w + 100;
		if (optcontrast < 0)
			optcontrast = 0;
		else if (optcontrast > 200)
			optcontrast = 200;
		memread(&w, 4, 1, &p);
		optred = w + 100;
		if (optred < 0)
			optred = 0;
		else if (optred > 200)
			optred = 200;
		memread(&w, 4, 1, &p);
		optgreen = w + 100;
		if (optgreen < 0)
			optgreen = 0;
		else if (optgreen > 200)
			optgreen = 200;
		memread(&w, 4, 1, &p);
		optblue = w + 100;
		if (optblue < 0)
			optblue = 0;
		else if (optblue > 200)
			optblue = 200;
	}
	memset(((char *)hiscores), 0, 0x16c);
	fopenr(&h, texts + 0x9be6);
	if (!h)
		return 1;
	if (fileread(((char *)hiscores), 13, 0x1c, h) == 0)
		for (v = 0; v < 13 && fileread(((char *)hiscores) + v * 0x1c, 1, 0x1c, h) != 0; v++)
			;
	fileclose(&h);
	if (savefail || newcfg || readonlydir)
		cfgbyte1 = 0;
	return 1;
}

void
saveall(void)
{
	savecfg();
	if (maploaded == 0)
		return;
	savegame();
	saveobjs(curmap, 0);
	savemap(curmap);
}

void
showfile(int flag)
{
	cdprog();
	readmename(readmebuf);
	fopenr(&readmefile, readmebuf);
	cdsaves();
	if (readmefile == 0)
		return;
	readmelen = fileread(workbuf, 1, 0x96640, readmefile);
	fileclose(&readmefile);
	workbuf[readmelen] = 0;
	catstr(workbuf, texts + 0xb2af);
	catstr(workbuf, backbutton());
	wrap(workbuf, 428);
	if (!flag)
		fadefast(0);
	textwin(workbuf, 0, 0, 0, 0, &menu1, 0, flag ? 0xff : 0);
}

DWORD
gfxmem(void)
{
	return 0;
}

DWORD
npcmem(void)
{
	return (hasgearbank ? setpics * 41472 : 0) + ntiles * 2304;
}

DWORD
datamem(void)
{
	return 0x28e8aa;
}

DWORD
sndmem(void)
{
	return sfxbytes;
}

DWORD
musmem(void)
{
	return musicbytes;
}

DWORD
cachemem(void)
{
	if (cachebytes)
		return cachebytes;
	return 0;
}

DWORD
memused(void)
{
	return gfxmem() + gfxmem() + npcmem() + gfxmem() + datamem() + sndmem() + musmem() + cachemem();
}

void
cacheinfo(char *buf)
{
	*buf = 0;
	if (cachebytes == 0)
		return;
	if (partscached) {
		wsprintf(namebuf, texts + 0xabe6, partbytes >> 10);
		catstr(buf, namebuf);
	}
	if (facescached) {
		wsprintf(namebuf, texts + 0xabee, facebytes >> 10);
		catstr(buf, namebuf);
	}
	if (overscached) {
		wsprintf(namebuf, texts + 0xabf5, overbytes >> 10);
		catstr(buf, namebuf);
	}
	if (travelbytes) {
		wsprintf(namebuf, texts + 0xac05, travelbytes >> 10);
		catstr(buf, namebuf);
	}
}

void
meminfo(char *buf)
{
	wsprintf(buf, strs[1484], memused() >> 10, availmem() >> 10);
}

void
inactive(char *out)
{
	char *p;

	p = printbuf;
	printbuf[0] = 0;
	if (sfxbytes == 0) {
		wsprintf(p, texts + 0xabb8);
		for (; *p; p++)
			;
	}
	if (musicoff) {
		wsprintf(p, texts + 0xabbd);
		for (; *p; p++)
			;
	}
	if (cachebytes == 0) {
		wsprintf(p, texts + 0xabc2);
		for (; *p; p++)
			;
	}
	if (!hasgearbank) {
		wsprintf(p, texts + 0xabc8);
		for (; *p; p++)
			;
	}
	if (noprun) {
		wsprintf(p, texts + 0xabce);
		for (; *p; p++)
			;
	}
	if (minsets || nopvari) {
		wsprintf(p, texts + 0xabd4);
		for (; *p; p++)
			;
	}
	if (minsets) {
		wsprintf(p, texts + 0xabda);
		for (; *p; p++)
			;
	}
	if (nofani) {
		wsprintf(p, texts + 0xabe0);
		for (; *p; p++)
			;
	}
	if (printbuf[0])
		wsprintf(out, strs[1485], printbuf);
}

void
sysreport(char *p)
{
	soundinfo();
	musicinfo();
	*p = 0;
	catstr(p, titlehead());
	catstr(p, texts + 0xb1f9);
	catstr(p, titletail() + 1);
	catstr(p, texts + 0xb220);
	catstr(p, pick(texts + 0x4d6, texts + 0x4e2, texts + 0x4ef, texts + 0x4f0, texts + 0x4f1, texts + 0x503));
	catstr(p, texts + 0xb1f9);
	catstr(p, vertext);
	catstr(p, texts + 0xb2ab);
	catstr(p, texts + 0x6ac6);
	catstr(p, texts + 0xb2a8);
	wsprintf(namebuf, strs[1446], licenseno);
	catstr(p, namebuf);
	unscrambleall();
	if (udstr0[0]) {
		// FIXED: the Italian format asks for five strings, where the others ask for the four that the call passes, so wsprintf reads a fifth past them. Pass an empty fifth, which the others leave.
#ifdef FEATURE_FAULTS
		wsprintf(namebuf, pick(texts + 0x7dbc, texts + 0x7de6, texts + 0x7e0f, texts + 0x7e10, texts + 0x7e11, texts + 0x7e3a), udstr0, udstr1, udstr2, udstr3, "");
#else
		wsprintf(namebuf, pick(texts + 0x7dbc, texts + 0x7de6, texts + 0x7e0f, texts + 0x7e10, texts + 0x7e11, texts + 0x7e3a), udstr0, udstr1, udstr2, udstr3);
#endif
		catstr(p, namebuf);
		catstr(p, texts + 0xb2a8);
	}
	unscrambleall();
	catstr(p, texts + 0xb220);
	catstr(p, texts + 0x6ac6);
	catstr(p, texts + 0xb2a8);
	for (; *p; p++)
		;
	wsprintf(p, strs[1482], nstarts);
	for (; *p; p++)
		;
	totaltime = playtime + uptime();
	wsprintf(p, strs[1483], hourstext(totaltime));
	for (; *p; p++)
		;
	while (p[-1] == 13 || p[-1] == 10)
		p--;
	wsprintf(p, texts + 0xb2b2);
	for (; *p; p++)
		;
	wsprintf(p, "System:\r\n\r\nWindows ");
	for (; *p; p++)
		;
	memset(&osv, 0, sizeof osv);
	osv.dwOSVersionInfoSize = sizeof osv;
	GetVersionEx(&osv);
	if (osv.dwPlatformId == 0)
		wsprintf(p, "W3.1");
	else if (osv.dwPlatformId == 1)
		wsprintf(p, "W95");
	else if (osv.dwPlatformId == 2)
		wsprintf(p, "NT");
	else
		wsprintf(p, "(%d)", osv.dwPlatformId);
	for (; *p; p++)
		;
	wsprintf(p, " version %ld.%ld (build %ld)\r\n\r\n", osv.dwMajorVersion, osv.dwMinorVersion, osv.dwPlatformId == 1 ? osv.dwBuildNumber & 0xffff : osv.dwBuildNumber);
	for (; *p; p++)
		;
	wsprintf(p, "Processor: %d ", sysinfo.dwProcessorType);
	for (; *p; p++)
		;
	GetSystemInfo(&sysinfo);
	if (sysinfo.wProcessorArchitecture == 0)
		wsprintf(p, "Intel %d %04X", sysinfo.wProcessorLevel, sysinfo.wProcessorRevision);
	else
		wsprintf(p, "%d %d %04X", sysinfo.wProcessorArchitecture, sysinfo.wProcessorLevel, sysinfo.wProcessorRevision);
	for (; *p; p++)
		;
	while (p[-1] == 13 || p[-1] == 10)
		p--;
	wsprintf(p, texts + 0xb2b2);
	for (; *p; p++)
		;
	availmem();
	wsprintf(p, texts + 0x9671, memstat.dwTotalPhys >> 10, memstat.dwAvailPhys >> 10, (memstat.dwTotalPhys - memstat.dwAvailPhys) >> 10);
	for (; *p; p++)
		;
	wsprintf(p, texts + 0x96b8, memstat.dwTotalPageFile >> 10, memstat.dwAvailPageFile >> 10, (memstat.dwTotalPageFile - memstat.dwAvailPageFile) >> 10);
	for (; *p; p++)
		;
	wsprintf(p, texts + 0x96fd, memstat.dwMemoryLoad);
	for (; *p; p++)
		;
	while (p[-1] == 13 || p[-1] == 10)
		p--;
	wsprintf(p, texts + 0xb2b2);
	for (; *p; p++)
		;
	// FIXED: the format's second %lu, the game world's memory, has no argument. Pass gfxmem's, as memtable does.
#ifdef FEATURE_FAULTS
	wsprintf(p, texts + 0x9715, datamem() >> 10, gfxmem() >> 10);
#else
	wsprintf(p, texts + 0x9715, datamem() >> 10);
#endif
	for (; *p; p++)
		;
	wsprintf(p, texts + 0x9769, npcmem() >> 10);
	for (; *p; p++)
		;
	cacheinfo(tmpbuf);
	if (tmpbuf[0])
		catstr(tmpbuf, texts + 0xa24d);
	wsprintf(p, texts + 0x9795, cachemem() >> 10, tmpbuf);
	for (; *p; p++)
		;
	inactive(p);
	for (; *p; p++)
		;
	while (p[-1] == 13 || p[-1] == 10)
		p--;
	wsprintf(p, texts + 0xb2b2);
	for (; *p; p++)
		;
	wsprintf(p, vramtext);
	for (; *p; p++)
		;
	while (p[-1] == 13 || p[-1] == 10)
		p--;
	wsprintf(p, texts + 0xb2b2);
	for (; *p; p++)
		;
	wsprintf(p, blittext);
	for (; *p; p++)
		;
	while (p[-1] == 13 || p[-1] == 10)
		p--;
	wsprintf(p, texts + 0xb2b2);
	for (; *p; p++)
		;
	wsprintf(p, spritetext);
	for (; *p; p++)
		;
	while (p[-1] == 13 || p[-1] == 10)
		p--;
	wsprintf(p, texts + 0xb2b2);
	for (; *p; p++)
		;
	wsprintf(p, soundtext);
	for (; *p; p++)
		;
	while (p[-1] == 13 || p[-1] == 10)
		p--;
	wsprintf(p, texts + 0xb2b2);
	for (; *p; p++)
		;
	wsprintf(p, capstext);
	for (; *p; p++)
		;
	while (p[-1] == 13 || p[-1] == 10)
		p--;
	wsprintf(p, texts + 0xb2b2);
	for (; *p; p++)
		;
	wrap(musictext, 428);
	wsprintf(p, musictext);
	for (; *p; p++)
		;
	while (p[-1] == 13 || p[-1] == 10)
		p--;
	wsprintf(p, texts + 0xb2b2);
	for (; *p; p++)
		;
	wsprintf(p, strs[562]);
}

void
memtable(char *buf)
{
	cacheinfo(printbuf);
	wsprintf(buf, texts + 0x8d6a, datamem() >> 10, gfxmem() >> 10, gfxmem() >> 10, npcmem() >> 10, gfxmem() >> 10, sndmem() >> 10, musmem() >> 10, cachemem() >> 10, cachebytes ? ':' : ' ', printbuf);
}

void
memreport(char *buf)
{
	char *p;

	p = buf;
	wsprintf(buf, texts + 0x6ac5, titlehead(), vertext);
	for (; *p; p++)
		;
	meminfo(p);
	for (; *p; p++)
		;
	inactive(p);
	for (; *p; p++)
		;
	memtable(p);
	for (; *p; p++)
		;
}

// PORT: the type that runmenu calls it through, where a call through another is undefined in C
#ifdef PORT
short
palkey(unsigned char unused, unsigned char c)
#else
int
palkey(void *, char c)
#endif
{
	int *v;

	if (menu1.sel == 1)
		v = &optbright;
	else if (menu1.sel == 2)
		v = &optcontrast;
	else if (menu1.sel == 3)
		v = &optred;
	else if (menu1.sel == 4)
		v = &optgreen;
	else if (menu1.sel == 5)
		v = &optblue;
	else
		return 0;
	if (c == '+') {
		if (keys[VK_SHIFT] & 0x80)
			*v += 10;
		else
			(*v)++;
	} else if (c == '-') {
		if (keys[VK_SHIFT] & 0x80)
			*v -= 10;
		else
			(*v)--;
	} else
		return 0;
	if (v == &optbright) {
		if (*v < 40)
			*v = 40;
		else if (*v > 300)
			*v = 300;
	} else {
		if (*v < 0)
			*v = 0;
		else if (*v > 200)
			*v = 200;
	}
	adjpal(optbright, optcontrast, optred, optgreen, optblue);
	return 3;
}

// PORT: the type that runmenu calls it through, where a call through another is undefined in C
#ifdef PORT
short
gfxkey(unsigned char unused, unsigned char c)
#else
int
gfxkey(void *, char c)
#endif
{
	// FIXED: items 5 and 6 are the delays only while optextra shows them, and otherwise the dialogs' and the pages' items, whose plus and minus keys change the hidden delays. Leave them alone.
#if defined(FEATURE_FIXES)
	if (!optextra && (menu1.sel == 5 || menu1.sel == 6))
		return 0;
#endif
	if (c == '+') {
		if (menu1.sel == 3) {
			if (maxmsgs < 17) {
				maxmsgs++;
				return 3;
			}
		} else if (menu1.sel == 5) {
			if (walkdelay >= 0x95) {
				walkdelay = 0x96;
				return 3;
			}
			walkdelay++;
			return 3;
		} else if (menu1.sel == 6) {
			if (standdelay >= 0x95) {
				standdelay = 0x96;
				return 3;
			}
			standdelay++;
			return 3;
		}
		return 0;
	}
	if (c == '-') {
		if (menu1.sel == 3) {
			if (maxmsgs > 3) {
				maxmsgs--;
				return 3;
			}
		} else if (menu1.sel == 5) {
			if (walkdelay <= 1) {
				walkdelay = 0;
				return 3;
			}
			walkdelay--;
			return 3;
		} else if (menu1.sel == 6) {
			if (standdelay <= 1) {
				standdelay = 0;
				return 3;
			}
			standdelay--;
			return 3;
		}
	}
	return 0;
}

// PORT: the type that runmenu calls it through, where a call through another is undefined in C
#ifdef PORT
short
ctrlkey(unsigned char unused, unsigned char c)
#else
int
ctrlkey(void *, char c)
#endif
{
	if (menu1.sel == 3) {
		if (c == '+' && censorship < 4) {
			censorship++;
			if (censorship == 3)
				censorship++;
			return 3;
		}
		if (c == '-' && censorship > 0) {
			censorship--;
			if (censorship == 3)
				censorship--;
			return 3;
		}
	}
	if (menu1.sel == 4) {
		if (c == '+' && popupdelay < 1500) {
			popupdelay += 100;
			if (popupdelay > 1500)
				popupdelay = 1500;
			return 3;
		}
		if (c == '-' && popupdelay > 0) {
			if (popupdelay <= 100)
				popupdelay = 0;
			else
				popupdelay -= 100;
			return 3;
		}
	}
	return 0;
}

void
togglesound(void)
{
	freesound();
	dsready = 0;
	if (soundoff)
		soundoff = 0;
	else
		soundoff = 2;
	setupsound();
	if (!dsready)
		return;
	if (loadsounds()) {
		featuresoff = 0;
		return;
	}
	freesound();
	dsready = 0;
}

void
resetsound(void)
{
	freesound();
	dsready = 0;
	setupsound();
	if (!dsready)
		return;
	if (loadsounds())
		return;
	freesound();
	dsready = 0;
}

// PORT: the type that runmenu calls it through, where a call through another is undefined in C
#ifdef PORT
short
soundkey(unsigned char unused, unsigned char c)
#else
int
soundkey(void *, char c)
#endif
{
	if (menu1.sel == 4 && dsready && soundson) {
		if (c == '+') {
			if (sfxvolume < 100)
				sfxvolume += 10;
			else
				sfxvolume = 100;
sound:
			resetsound();
			playnow(45);
			return 3;
		}
		if (c == '-') {
			if (sfxvolume > 10)
				sfxvolume -= 10;
			else
				sfxvolume = 10;
			goto sound;
		}
	}
	if (menu1.sel == 8 && musloaded && !musicoff) {
		if (c == '+') {
			if (musicvolume < 100)
				musicvolume += 10;
			else
				musicvolume = 100;
music:
			stopmusic();
			music(4, 0);
			return 3;
		}
		if (c == '-') {
			if (musicvolume > 10)
				musicvolume -= 10;
			else
				musicvolume = 10;
			goto music;
		}
	}
	return 0;
}

void
showreport(int flag)
{
	sysreport(workbuf);
	catstr(workbuf, texts + 0xb2af);
	catstr(workbuf, backbutton());
	reportres = textwin(workbuf, 0, 0xfe, 0, 0, &menu1, 0, flag ? 0xff : 0);
	if (esckey != 0xff)
		esckey = 0;
	if (reportres > 10000 && reportres % 100 == 1)
		showfile(1);
}

void
empty2(void)
{
}

void
nextlang(void)
{
	lang++;
	while (lang < 6) {
		// FEATURE: features: the languages of the build
#ifdef FEATURES
		if (featlang(lang))
#else
		if (lang == 1 || lang == 0 || lang == 4)
#endif
			break;
		lang++;
	}
	if (lang >= 6)
		lang = 0;
}

void
options(int ingame)
{
	int *v;

	optextra = 0;
	entermenu();
	drawcursor();
	if (ingame)
		pagefx = 2;
	else
		nop();
	for (;;) {
		if (esckey == 0xff)
			break;
		optn = 0;
		// FEATURE: features: the build's own format of the menu's title
#ifdef FEATURES
		wsprintf(((char (*)[100])scrbuf)[optn], feattext(texts + 0xb057), texts + 0xb49f, strs[547], strs[552]);
#else
		wsprintf(((char (*)[100])scrbuf)[optn], texts + 0xb057, texts + 0xb49f, strs[547], strs[552]);
#endif
		upperstr(((char (*)[100])scrbuf)[optn]);
		optitems[optn] = ((char (*)[100])scrbuf)[optn];
		optn++;
		optitems[optn++] = strs[921];
		optitems[optn++] = strs[922];
		optitems[optn++] = strs[932];
		optitems[optn++] = strs[952];
		optitems[optn++] = strs[989];
		optitems[optn++] = strs[1007];
		optitems[optn++] = strs[554];
		blankpage();
		listmenu(&menu1, optn, optitems);
		pagefx = 2;
		menu1.keys2[0] = 0x3e;
		menu1.keys2[1] = 0x18;
		optn = runmenu(&menu1, 0, 0, 0);
		if (needredraw(optn)) {
			pagefx = 0;
			continue;
		}
		switch (optn) {
		case 0:
		case 7:
			goto out;
		case 1:
			showreport(1);
			pagefx = 3;
			break;
		case 2:
			optchanged = 0;
			oldauto = autolevel;
			for (;;) {
				optn = 0;
				// FEATURE: features: the build's own format of the menu's title
#ifdef FEATURES
				wsprintf(((char (*)[100])scrbuf)[optn], feattext(texts + 0xb062), texts + 0xb49f, texts + 0xb3ff, strs[552], strs[922]);
#else
				wsprintf(((char (*)[100])scrbuf)[optn], texts + 0xb062, texts + 0xb49f, texts + 0xb3ff, strs[552], strs[922]);
#endif
				upperstr(((char (*)[100])scrbuf)[optn]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (lang == 0)
					langname = texts + 0x7d41;
				else if (lang == 1)
					langname = texts + 0x7d39;
				else if (lang == 2)
					langname = texts + 0x7d49;
				else if (lang == 3)
					langname = texts + 0x7d52;
				else if (lang == 4)
					langname = texts + 0x7d5a;
				else if (lang == 5)
					langname = texts + 0x7d63;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[923], langname);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[925], pounds ? strs[816] : strs[815]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (censorship > 4)
					censorship = 4;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[926], strs[927 + censorship]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[940], popupdelay / 1000, popupdelay % 1000 / 100, texts + 0xb561);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[935], cmbautooff == 2 ? strs[247] : strs[248]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (autolevel > 3)
					autolevel = 3;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[936], autolevel ? strs[936 + autolevel] : strs[248]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[942], quitprompt ? strs[247] : strs[248]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[943], showtotal ? strs[247] : strs[248]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				optitems[optn++] = strs[554];
				menupage();
				listw = 360;
				listmenu(&menu1, optn, optitems);
				listw = 360;
				pagefx = 0;
				optn = runmenu(&menu1, 0, (short (*)(unsigned char, unsigned char))ctrlkey, 0);
				if (needredraw(optn))
					continue;
				if (optn == 0 || optn == menu1.n) {
					if (!oldauto && autolevel) {
						autocopy();
						autosave();
					}
					if (optchanged) {
						saveall();
						dresshero();
					}
					pagefx = 3;
					break;
				}
				optchanged = 1;
				if (optn == 1) {
					nextlang();
					setlang(lang);
					loadstrs();
					loadscript(scriptfile);
				} else if (optn == 2) {
					if (pounds)
						pounds = 0;
					else
						pounds = 1;
					poundsset = 1;
				} else if (optn == 3) {
					if (censorship < 4) {
						censorship++;
						if (censorship == 3)
							censorship++;
					} else
						censorship = 0;
				} else if (optn == 4) {
					popupdelay += 100;
					if (popupdelay > 1500)
						popupdelay = 0;
				} else if (optn == 5) {
					if (cmbautooff == 2)
						cmbautooff = 1;
					else
						cmbautooff = 2;
				} else if (optn == 6) {
					autolevel++;
					if (autolevel > 3)
						autolevel = 0;
				} else if (optn == 7)
					quitprompt = quitprompt ? 0 : 1;
				else if (optn == 8)
					showtotal = showtotal ? 0 : 1;
			}
			break;
		case 3:
			optchanged = 0;
			redress = 0;
			menu1.keysel = 0;
			for (;;) {
				availmem();
				/* a byte copy of the selection and an int copy of runmenu's
				   result below give the original's registers */
				{
					unsigned char t;

					t = menu1.keysel;
					optsel = t;
				}
				optn = 0;
				// FEATURE: features: the build's own format of the menu's title
#ifdef FEATURES
				wsprintf(((char (*)[100])scrbuf)[optn], feattext(texts + 0xb062), texts + 0xb49f, texts + 0xb3ff, strs[552], strs[932]);
#else
				wsprintf(((char (*)[100])scrbuf)[optn], texts + 0xb062, texts + 0xb49f, texts + 0xb3ff, strs[552], strs[932]);
#endif
				upperstr(((char (*)[100])scrbuf)[optn]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				((char (*)[100])scrbuf)[optn][0] = 0;
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[946], nopani || nopvari ? (texts + 0xb1f7) : noprun ? strs[248] : strs[247]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[947], nopani ? (texts + 0xb1f7) : nopvari ? strs[248] : strs[247]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[948], nopequip ? strs[248] : strs[247]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[949], nopani ? strs[248] : strs[247]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (musloaded || musicoff)
					wsprintf(((char (*)[100])scrbuf)[optn], strs[999], musicoff ? strs[248] : strs[247]);
				else
					wsprintf(((char (*)[100])scrbuf)[optn], strs[999], strs[249]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[950], !soundoff && !dsready ? (texts + 0xb1f7) : soundoff ? strs[248] : strs[247]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				optitems[optn++] = strs[554];
				menupage();
				inkwhite();
				wsprintf(namebuf, strs[944], (memstat.dwAvailPhys >> 10) / 1000, (memstat.dwAvailPhys >> 10) % 1000, (memstat.dwAvailPageFile >> 10) / 1000, (memstat.dwAvailPageFile >> 10) % 1000);
				text3grey(0, 0xffff, 100, namebuf);
				wsprintf(namebuf, strs[945]);
				text3grey(0, 0xffff, 130, namebuf);
				inkshade();
				listw = 360;
				listmenu(&menu1, optn, optitems);
				menu1.enabled[0] = 0;
				listw = 360;
				pagefx = 0;
				menu1.keysel = optsel;
				{
					int t;

					// FIXED: a call through another function type is undefined in C. Call it through its own: optn keeps the same 16 bits.
#ifdef PORT
					t = runmenu(&menu1, 0, 0, 0);
#else
					t = ((int (*)(Menu *, int (*)(void), short (*)(unsigned char, unsigned char), short))runmenu)(&menu1, 0, 0, 0);
#endif
					optn = t;
				}
				if (needredraw(optn))
					continue;
				if (optn == 0 || optn == 8) {
					if (optchanged)
						saveall();
					if (redress) {
						buildrecipes();
						loadgear();
						resetdress();
						dresshero();
					}
					pagefx = 3;
					break;
				}
				optchanged = 1;
				if (optn == 2) {
					if (noprun)
						noprun = 0;
					else
						noprun = 2;
				} else if (optn == 3) {
					if (nopvari)
						nopvari = 0;
					else
						nopvari = 2;
				} else if (optn == 4) {
					if (nopequip)
						nopequip = 0;
					else
						nopequip = 2;
				} else if (optn == 5) {
					if (nopani)
						nopani = 0;
					else
						nopani = 2;
				} else {
					if (optn == 6)
						togglemusic();
					else if (optn == 7)
						togglesound();
					continue;
				}
				freebank();
				tilefree();
				tileinit();
				allocbank();
				redress = 1;
			}
			break;
		case 4:
			optchanged = 0;
			optextra = 0;
			menu1.keysel = 0;
			if (keys['W'] & 0x80)
				optextra = 1;
			for (;;) {
				optsel = menu1.keysel;
				optn = 0;
				// FEATURE: features: the build's own format of the menu's title
#ifdef FEATURES
				wsprintf(((char (*)[100])scrbuf)[optn], feattext(texts + 0xb062), texts + 0xb49f, texts + 0xb3ff, strs[552], strs[952]);
#else
				wsprintf(((char (*)[100])scrbuf)[optn], texts + 0xb062, texts + 0xb49f, texts + 0xb3ff, strs[552], strs[952]);
#endif
				upperstr(((char (*)[100])scrbuf)[optn]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[953], mapframed == 0 ? strs[247] : strs[248]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[954], strs[955 + scrolling]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[961], maxmsgs, texts + 0xb561);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (gfxram == 3)
					wsprintf(((char (*)[100])scrbuf)[optn], strs[962], strs[963]);
				else if (gfxram == 2)
					wsprintf(((char (*)[100])scrbuf)[optn], strs[962], strs[964]);
				else if (gfxram == 1)
					wsprintf(((char (*)[100])scrbuf)[optn], strs[962], strs[965]);
				else
					wsprintf(((char (*)[100])scrbuf)[optn], strs[962], strs[966]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (optextra) {
					wsprintf(((char (*)[100])scrbuf)[optn], strs[959], walkdelay, texts + 0xb561);
					optitems[optn] = ((char (*)[100])scrbuf)[optn];
					optn++;
					wsprintf(((char (*)[100])scrbuf)[optn], strs[960], standdelay, texts + 0xb561);
					optitems[optn] = ((char (*)[100])scrbuf)[optn];
					optn++;
				}
				wsprintf(((char (*)[100])scrbuf)[optn], strs[973], strs[974 + textbg]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[978], strs[979 + pageturn]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				wsprintf(((char (*)[100])scrbuf)[optn], strs[983]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				optitems[optn++] = strs[554];
				menupage();
				listw = 360;
				listmenu(&menu1, optn, optitems);
				listw = 360;
				pagefx = 0;
				menu1.keysel = optsel;
				optn = runmenu(&menu1, 0, (short (*)(unsigned char, unsigned char))gfxkey, 0);
				if (needredraw(optn))
					continue;
				if (optn == 0 || optn == menu1.n) {
					if (optchanged)
						saveall();
					pagefx = 3;
					optextra = 0;
					break;
				}
				optchanged = 1;
				if (optn == 1) {
					if (mapframed)
						mapframed = 0;
					else
						mapframed = 1;
				} else if (optn == 2) {
					scrolling++;
					if (scrolling > 3)
						scrolling = 0;
				} else if (optn == 3) {
					if (maxmsgs < 17)
						maxmsgs++;
					else
						maxmsgs = 3;
				} else if (optn == 4) {
					if (gfxram == 0)
						gfxram = 3;
					else
						gfxram = 0;
					fadefast(0);
					pagefx = 1;
					freesurfs();
					if (!ddinit(0, (unsigned char *)gamepal)) {
						quitnow();
						pagefx = 1;
						goto out;
					}
				} else if (optn == menu1.n - 1) {
					menu1.keysel = 0;
					fadefast(0);
					for (;;) {
						optsel = menu1.keysel;
						noflip = 1;
						drawscreen();
						noflip = 0;
						optn = 0;
						// FEATURE: features: the build's own format of the menu's title
#ifdef FEATURES
						wsprintf(((char (*)[100])scrbuf)[optn], feattext(texts + 0xb062), texts + 0xb49f, texts + 0xb3ff, texts + 0xb3ff, strs[983]);
#else
						wsprintf(((char (*)[100])scrbuf)[optn], texts + 0xb062, texts + 0xb49f, texts + 0xb3ff, texts + 0xb3ff, strs[983]);
#endif
						upperstr(((char (*)[100])scrbuf)[optn]);
						optitems[optn] = ((char (*)[100])scrbuf)[optn];
						optn++;
						wsprintf(((char (*)[100])scrbuf)[optn], strs[984], optbright, texts + 0xb561);
						optitems[optn] = ((char (*)[100])scrbuf)[optn];
						optn++;
						wsprintf(((char (*)[100])scrbuf)[optn], strs[985], optcontrast, texts + 0xb561);
						optitems[optn] = ((char (*)[100])scrbuf)[optn];
						optn++;
						wsprintf(((char (*)[100])scrbuf)[optn], strs[986], optred, texts + 0xb561);
						optitems[optn] = ((char (*)[100])scrbuf)[optn];
						optn++;
						wsprintf(((char (*)[100])scrbuf)[optn], strs[987], optgreen, texts + 0xb561);
						optitems[optn] = ((char (*)[100])scrbuf)[optn];
						optn++;
						wsprintf(((char (*)[100])scrbuf)[optn], strs[988], optblue, texts + 0xb561);
						optitems[optn] = ((char (*)[100])scrbuf)[optn];
						optn++;
						optitems[optn++] = strs[554];
						listbright = 1;
						listw = 360;
						listmenu(&menu1, optn, optitems);
						menu1.keysel = optsel;
						listw = 360;
						pagefx = 0;
						fadefast(1);
						setpal();
						optn = runmenu(&menu1, 0, (short (*)(unsigned char, unsigned char))palkey, 0);
						listbright = 0;
						if (needredraw(optn))
							continue;
						if (optn == 0 || optn == menu1.n)
							break;
						optchanged = 1;
						if (optn == 1)
							v = &optbright;
						else if (optn == 2)
							v = &optcontrast;
						else if (optn == 3)
							v = &optred;
						else if (optn == 4)
							v = &optgreen;
						else if (optn == 5)
							v = &optblue;
						else
							continue;
						if (*v == 80)
							*v = 85;
						else if (*v == 85)
							*v = 90;
						else if (*v == 90)
							*v = 95;
						else if (*v == 95)
							*v = 100;
						else if (*v == 100)
							*v = 105;
						else if (*v == 105)
							*v = 110;
						else if (*v == 110)
							*v = 115;
						else if (*v == 115)
							*v = 120;
						else
							*v = 80;
						adjpal(optbright, optcontrast, optred, optgreen, optblue);
					}
					if (optchanged)
						saveall();
					fadefast(0);
					pagefx = 1;
					optextra = 0;
					menu1.keysel = 0;
					if (esckey == 0xff)
						goto next;
				} else if (optn == menu1.n - 2) {
					if (pageturn < 3)
						pageturn++;
					else
						pageturn = 0;
				} else if (optn == menu1.n - 3) {
					if (textbg < 3)
						textbg++;
					else
						textbg = 0;
				} else if (optn == 5) {
					if (walkdelay >= 0x95)
						walkdelay = 0;
					else
						walkdelay++;
				} else if (optn == 6) {
					if (standdelay >= 0x95)
						standdelay = 0;
					else
						standdelay++;
				}
			}
			break;
		case 5:
			optchanged = 0;
			for (;;) {
				optn = 0;
				// FEATURE: features: the build's own format of the menu's title
#ifdef FEATURES
				wsprintf(((char (*)[100])scrbuf)[optn], feattext(texts + 0xb062), texts + 0xb49f, texts + 0xb3ff, strs[552], strs[989]);
#else
				wsprintf(((char (*)[100])scrbuf)[optn], texts + 0xb062, texts + 0xb49f, texts + 0xb3ff, strs[552], strs[989]);
#endif
				upperstr(((char (*)[100])scrbuf)[optn]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (!soundoff && !dsready)
					wsprintf(((char (*)[100])scrbuf)[optn], texts + 0x9dea, strs[556], dsshort);
				else if (minmem || featuresoff)
					wsprintf(((char (*)[100])scrbuf)[optn], texts + 0xab8d);
				else
					wsprintf(((char (*)[100])scrbuf)[optn], strs[950], !soundoff && !dsready ? (texts + 0xb1f7) : soundoff ? strs[248] : strs[247]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (!dsready)
					wsprintf(((char (*)[100])scrbuf)[optn], strs[990], texts + 0xb1f7);
				else
					wsprintf(((char (*)[100])scrbuf)[optn], strs[990], outalways ? strs[991] : strs[992]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (!dsready)
					wsprintf(((char (*)[100])scrbuf)[optn], strs[995], texts + 0xb1f7);
				else
					wsprintf(((char (*)[100])scrbuf)[optn], strs[995], soundson ? strs[247] : strs[248]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (dsready && soundson)
					wsprintf(((char (*)[100])scrbuf)[optn], strs[996], sfxvolume, texts + 0xb561);
				else
					wsprintf(((char (*)[100])scrbuf)[optn], strs[997], texts + 0xb1f7);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (!dsready || !soundson)
					wsprintf(((char (*)[100])scrbuf)[optn], strs[998], texts + 0xb1f7);
				else
					wsprintf(((char (*)[100])scrbuf)[optn], strs[998], landsounds ? strs[247] : strs[248]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (!dsready || !soundson)
					wsprintf(((char (*)[100])scrbuf)[optn], strs[1000], texts + 0xb1f7);
				else
					wsprintf(((char (*)[100])scrbuf)[optn], strs[1000], menusounds ? strs[247] : strs[248]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (musloaded || musicoff)
					wsprintf(((char (*)[100])scrbuf)[optn], strs[999], musicoff ? strs[248] : strs[247]);
				else
					wsprintf(((char (*)[100])scrbuf)[optn], strs[999], strs[249]);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				if (!musloaded || musicoff)
					wsprintf(((char (*)[100])scrbuf)[optn], strs[997], texts + 0xb1f7);
				else
					wsprintf(((char (*)[100])scrbuf)[optn], strs[996], musicvolume, texts + 0xb561);
				optitems[optn] = ((char (*)[100])scrbuf)[optn];
				optn++;
				optitems[optn++] = strs[554];
				menupage();
				listw = 360;
				listmenu(&menu1, optn, optitems);
				listw = 360;
				pagefx = 0;
				if (!dsready) {
					menu1.enabled[1] = 0;
					menu1.enabled[2] = 0;
					menu1.enabled[3] = 0;
					menu1.enabled[4] = dsready;
				}
				optn = runmenu(&menu1, 0, (short (*)(unsigned char, unsigned char))soundkey, 0);
				menu1.enabled[0] = 1;
				menu1.enabled[1] = 1;
				menu1.enabled[2] = 1;
				menu1.enabled[3] = 1;
				menu1.enabled[4] = 1;
				if (needredraw(optn))
					continue;
				if (optn == 0 || optn == 9) {
					if (optchanged)
						saveall();
					pagefx = 3;
					break;
				}
				optchanged = 1;
				if (optn == 1)
					togglesound();
				else if (optn == 2) {
					if (dsready) {
						if (outalways)
							outalways = 0;
						else
							outalways = 1;
						resetsound();
					}
				} else if (optn == 3) {
					if (dsready) {
						if (soundson)
							soundson = 0;
						else
							soundson = 1;
					}
				} else if (optn == 4) {
					if (dsready && soundson)
						playnow(45);
				} else if (optn == 5) {
					if (dsready && soundson) {
						if (landsounds)
							landsounds = 0;
						else
							landsounds = 1;
					}
				} else if (optn == 6) {
					if (dsready && soundson) {
						if (menusounds)
							menusounds = 0;
						else
							menusounds = 1;
					}
				} else if (optn == 7)
					togglemusic();
				else if (optn == 8 && musloaded && !musicoff) {
					stopmusic();
					music(4, 0);
				}
			}
			break;
		case 6:
			showfile(1);
			pagefx = 3;
			break;
		}
	next:
		;
	}
out:
	if (ingame) {
		pagefx = 3;
		return;
	}
	fadefast(0);
	pagefx = 1;
	nop();
}

void
drawmarks(short x, short y, short x2, short x3, short y2)
{
	short k;

	if (nmarks > 23)
		nmarks = 23;
	inkwhite();
	for (marki = 0; marki < 23; marki++) {
		if (!marks[marki][0])
			continue;
		marklabel[0] = marki + 'A';
		drawtext2o(0, x + markx[marki] + (y + marky[marki]) * 640, marklabel);
		drawtext2o(0, x2 + (y2 + marki * 14) * 640, marklabel);
		markstr = 0;
		memset(namebuf, 0, 2500);
		memset(textbuf, 0, 2500);
		if (marks[marki][0] == '\xa7' && marks[marki][1] == 'T' && marks[marki][2] == '#') {
			k = number(&marks[marki][3]);
			if (k >= 0 && k < 50) {
				markstr = 1691;
				markstr += k;
				for (markj = 0; markj < 20 && strs[markstr][markj]; markj++)
					namebuf[markj] = strs[markstr][markj];
				expandstr(textbuf, namebuf);
				textbuf[19] = 0;
			} else
				markstr = 0;
		}
		drawtext2o(0, x3 + (y2 + marki * 14) * 640, markstr ? textbuf : marks[marki]);
	}
	inkshade();
}

void
setmark(short i, int x, int y)
{
	markx[i] = x - 104;
	marky[i] = y - 94;
}

int
markposx(int i)
{
	return markx[i] + 104;
}

int
markposy(int i)
{
	return marky[i] + 94;
}

int
outsidemenu(void)
{
	if (menu1.rects[0].x - 3 > hoverx || menu1.rects[0].x + menu1.rects[0].w + 3 < hoverx
	|| menu1.rects[0].y - 3 > hovery || menu1.rects[0].y + menu1.rects[0].h + 3 < hovery)
		return 1;
	return 0;
}

void
markers(int param)
{
	short k;

	tabkey = 0;
	pagefx = 1;
	if (unconscious())
		goto done;
	if ((cheat && (keys['W'] & 0x80)) || param || lastlight > 5) {
		cleartexts();
		nop();
	top:
		for (;;) {
			if (pagefx < 2)
				fadefast(0);
			entermenu();
			if (pagefx < 2)
				usepal(0);
			pgtoback();
			if ((cheat && (keys['W'] & 0x80)) || param) {
				markmode = 0;
				k = 1;
			} else {
				markmode = 1;
				k = 0;
			}
			drawmap(100, 90, markmode, k);
			if (mapzone(curmap) && isbasemap(curmap) && curmap != 48)
				text3greyn(0, 100, 50, texts + 0xb079, strs[252], strs[zones[mapzone(curmap)].name]);
			else
				text3grey(0, 100, 50, strs[252]);
			plotlabels(curmap);
			memset(&menu1, 0, sizeof menu1);
			menu1.n = 0;
			addcorner(&menu1, 0, 0);
			memset(menu1.enabled, 1, menu1.n);
			markhi = 0;
			save0();
			memcpy(workbuf, ((char (*)[100])scrbuf), 640 * 480);
		restore:
			memcpy(((char (*)[100])scrbuf), workbuf, 640 * 480);
			showscr();
			drawmarks(100, 90, 402, 418, 70);
			markv = playerx * 2 + 100 + (playery * 2 + 90) * 640;
			setscr2o(markv - 1280, 48);
			setscr2o(markv - 2, 48);
			setscr2o(markv, 48);
			setscr2o(markv + 2, 48);
			setscr2o(markv + 1280, 48);
			puthands();
			backto2();
			entermenu();
			drawcursor();
			if (pagefx != 0)
				pagesound();
			if (!palblack && pagefx >= 2)
				turnpage(pagefx == 2 ? 1 : 0, 0, 480, 0);
			else {
				flip();
				fadefast(1);
			}
			pagefx = 0;
			waitforup = 1;
			pollinput();
			while (mouseleft || mouseright || keycode)
				readinput();
			waitforup = 0;
			leftclick = 0;
			mousemoved = 1;
			if ((cheat && (keys['W'] & 0x80)) || param)
				pickmap();
			else {
				lastx = -1;
				lasty = -1;
				for (;;) {
					if (lastx != mousex || lasty != mousey) {
						lastx = mousex;
						lasty = mousey;
						hover = -1;
						if (lastx >= menu1.rects[0].x && lastx < menu1.rects[0].x + menu1.rects[0].w
						&& lasty >= menu1.rects[0].y && lasty < menu1.rects[0].y + menu1.rects[0].h) {
							if (!markhi) {
								s2toback();
								pillshade8(menu1.rects[0].x, menu1.rects[0].y, menu1.rects[0].w, menu1.rects[0].h);
								flip();
								markhi = 1;
							}
						} else if (markhi) {
							s2toback();
							flip();
							markhi = 0;
						}
						if (lastx >= 100 && lastx < 400 && lasty >= 90 && lasty < 390) {
							hover = 1000;
							if (nmarks >= 23)
								hover = -1;
						} else if (lastx >= 402 && lastx < 556 && lasty >= 69 && lasty < 391 && nmarks) {
							markv = (lasty - 69) / 14;
							if (marks[markv][0])
								hover = markv;
						}
						if (hover == -1)
							markv = 3;
						else if (hover == 1000)
							markv = 206;
						else
							markv = 207;
						if (setcursor(3, markv, 0, 0, 0, 0))
							mousemoved = 1;
					}
					readinput();
					if (mouseleft || leftclick) {
						if (hover == 1000 && nmarks < 23) {
							for (markv = 0; markv < nmarks && marks[markv][0]; markv++)
								;
							hidecursor();
							putc1(lastx - 8, lasty - 8, 206);
							primto2();
							if (inputtext(320, 400, marks[markv], 19, 1, 1, 0, 0, 1, 0, 0, 0x92, 0) == 1) {
								setmark(markv, lastx, lasty);
								nmarks++;
							} else
								marks[markv][0] = 0;
							drawcursor();
							if (esckey != 0xff)
								esckey = 0;
							goto restore;
						}
						if (hover < 0 || hover >= 23 || nmarks <= 0)
							break;
						memset(marks[hover], 0, 20);
						nmarks--;
						markx[hover] = 0;
						marky[hover] = 0;
						goto restore;
					}
					pollinput();
					if (needredraw(0xfd))
						goto top;
					if (mouseright || esckey || keycode)
						break;
				}
			}
			clicksound();
			if (esckey != 0xff)
				goto leave;
			pagefx = 2;
			if (mayquit(1)) {
				quit();
				return;
			}
			pagefx = 3;
		}
	leave:
		fadefast(0);
		nop();
		goto done;
	}
	denymsg(strs[1451]);
done:
	leavemenu(1);
	if ((!cheat || !(keys['W'] & 0x80) || param) && playmode != 1) {
		busyturns += 2;
		units[0].pose = 9;
	}
	tabkey = 0;
}

void
drawitem(short i, char *s, short mode)
{
	if (mode == 2) {
		setbtncol(0x68);
		buttonon(menu2.rects[i].x, menu2.rects[i].y, menu2.rects[i].w, menu2.rects[i].h, s);
		clrbtncol();
	} else if (mode)
		buttonoff(menu2.rects[i].x, menu2.rects[i].y, menu2.rects[i].w, menu2.rects[i].h, s);
	else
		buttonon(menu2.rects[i].x, menu2.rects[i].y, menu2.rects[i].w, menu2.rects[i].h, s);
}

void
shadetopic(short i, short flag)
{
	if (i != 0xff && flag && menu2.rects[i].x >= 0x6a && menu2.rects[i].x < 0x216)
		pillshade8(menu2.rects[i].x, menu2.rects[i].y, menu2.rects[i].w, menu2.rects[i].h);
}

void
parsetext(char *text, int len)
{
	for (spliti = 0; spliti < 100; spliti++)
		textlines[spliti] = notopic;
	ntextlines = 0;
	text[len] = 0;
	texttitle = text;
	// FIXED: *splitp++ = upper(*splitp) reads and moves splitp unsequenced, undefined in C. Read *splitp first, as Watcom compiled it.
#ifdef PORT
	for (splitp = text; *splitp && *splitp != 13; splitp++)
		*splitp = upper(*splitp);
#else
	for (splitp = text; *splitp && *splitp != 13; )
		*splitp++ = upper(*splitp);
#endif
	for (splitp = text; splitp < text + len; splitp++) {
		if (*splitp == '[') {
			*splitp = 0;
			textlines[ntextlines] = splitp + 1;
			ntextlines++;
			if (ntextlines >= 100)
				break;
		}
	}
	for (spliti = 0; spliti < ntextlines; spliti++) {
		for (splitp = textlines[spliti]; *splitp != 13; ) {
			if (!*splitp)
				break;
			splitp++;
		}
		*splitp++ = 0;
		while (*splitp == 13 || *splitp == 10)
			*splitp++ = 0;
		textlines[spliti] = splitp;
		wrap(textlines[spliti], 428);
	}
}

void
textpage(short draw)
{
	if (draw)
		pgtoback();
	if (draw) {
		topicname = texttitle;
		text3grey2(0, 0xffff, 50, topicname);
	}
	memset(&menu2, 0, sizeof menu2);
	memset(topicof, 0xff, 100);
	menu2.n = 0;
	pageline = topicfirst;
	ntopics = 0;
	topicprev = 0;
	topicnext = 0;
	for (pagecol = 0; pagecol < 2; pagecol++) {
		for (pagerow = 0; pagerow < 11; pagerow++) {
			if (pageline >= ntextlines)
				goto out;
			if (draw) {
				menu2.enabled[menu2.n] = 1;
				menu2.rects[menu2.n].x = pagecol * 223 + 106;
				menu2.rects[menu2.n].y = pagerow * 30 + 90;
				menu2.rects[menu2.n].w = 204;
				menu2.rects[menu2.n].h = 24;
			}
			if (menu2.n == 0) {
				if (topicfirst > 0) {
					if (draw)
						drawitem(menu2.n, strs[1405], 2);
					topicof[menu2.n] = 0xfe;
					topicprev = 1;
					goto next;
				}
			} else if (menu2.n == 21 && pageline != ntextlines - 1) {
				if (draw)
					drawitem(menu2.n, strs[1406], 2);
				topicnext = 1;
				topicof[menu2.n] = 0xfd;
				goto next;
			}
			{
				if (draw) {
					topicname = textlines[pageline];
					if (chcut[pageline])
						topiccut = 1;
					else
						topiccut = 0;
					if (*topicname == 0)
						menu2.enabled[menu2.n] = 0;
					while (*--topicname == 0)
						;
					while (topicname[-1])
						topicname--;
					if (*topicname == '-')
						menu2.enabled[menu2.n] = 0;
					else {
						drawitem(menu2.n, topicname, topiccut);
						topicof[menu2.n] = pageline;
					}
				}
				pageline++;
				ntopics++;
			}
		next:
			menu2.n++;
		}
	}
out:
	if (!draw)
		return;
	if (pagearrows) {
		pagearrows = 0;
		setcursor(0, 0xffff, 0, 0, 0, 0);
	}
	if (topicprev) {
		putcs(0x18, 0xd8, 0x38);
		menu2.enabled[menu2.n] = 1;
		menu2.rects[menu2.n].x = 0;
		menu2.rects[menu2.n].y = 0;
		menu2.rects[menu2.n].w = 0x6a;
		menu2.rects[menu2.n].h = 0x1e0;
		topicof[menu2.n] = 0xfe;
		menu2.n++;
		pagearrows |= 5;
	}
	if (topicnext) {
		putcs(0x246, 0xd8, 0x39);
		menu2.enabled[menu2.n] = 1;
		menu2.rects[menu2.n].x = 0x216;
		menu2.rects[menu2.n].y = 0;
		menu2.rects[menu2.n].w = 0x6a;
		menu2.rects[menu2.n].h = 0x1e0;
		topicof[menu2.n] = 0xfd;
		menu2.n++;
		pagearrows |= 6;
	}
	addcorner(&menu2, menu2.n, 0);
	menu2.color = 0xf4;
	menu2.fn = shadetopic;
	puthands();
	if (cheat && (keys['W'] & 0x80) && (keys[VK_SHIFT] & 0x80) && (keys[0xe2] & 0x80) && !oddsswitch)
		putpix0(0, 0x1df, 0x1f);
	backto2();
	pagedone();
	if (palblack) {
		flip();
		fadefast(1);
		pagesound();
	}
}

short
pageup(void)
{
	curtopic = 0xfff;
	if (topicfirst == 0)
		return 0;
	topicfirst -= 21;
	if (topicfirst)
		topicfirst++;
	if (topicfirst <= 1)
		topicfirst = 0;
	return 1;
}

short
pagedown(void)
{
	curtopic = 0xfff;
	if (topicfirst + ntopics < ntextlines) {
		topicfirst += ntopics;
		return 1;
	}
	return 0;
}

short
textmenu(short mode)
{
	cleartexts();
	if (!mode) {
		nop();
		fadefast(0);
		usepal(0);
	}
	entermenu();
	if (mode == -1)
		turndir = 0;
	else if (mode)
		turndir = 1;
	else
		turndir = 0xff;
	if (curtopic != 0xfff) {
		textsel = curtopic;
		if (textsel < topicfirst || textsel >= topicfirst + ntopics) {
			topicfirst = 0;
			textpage(0);
			for (;;) {
				if (!(textsel < topicfirst || textsel >= topicfirst + ntopics))
					goto found;
				if (!topicnext || !pagedown())
					break;
				textpage(0);
			}
		}
	found:
		turndir = 0;
	}
again:
	textpage(1);
	if (turndir != 0xff)
		turnpage(turndir, 0, 480, 1);
	waitforup = 1;
	while (mouseleft || mouseright)
		readinput();
	waitforup = 0;
	leftclick = 0;
	for (;;) {
		textsel = runmenu(&menu2, 0, 0, 0);
		if (needredraw(textsel))
			goto again;
		if (!textsel)
			break;
		textsel--;
		if (topicof[textsel] == 0xfe) {
			if (pageup()) {
				turndir = 0;
				goto again;
			}
			continue;
		}
		if (topicof[textsel] == 0xfd) {
			if (pagedown()) {
				turndir = 1;
				goto again;
			}
			continue;
		}
		if (topicof[textsel] == 0xff)
			textsel = 0;
		else
			textsel = topicof[textsel] + 1;
		break;
	}
	if (pagearrows) {
		pagearrows = 0;
		setcursor(0, 0xffff, 0, 0, 0, 0);
	}
	if (!mode) {
		fadefast(0);
		nop();
	}
	return textsel;
}

short
showtext(short a, short b, char *s, short mode, short raw)
{
	short k1, k2;

	k1 = langentry(a);
	k2 = langentry(b);
	curtopic = 0xfff;
	if (k1 >= 0) {
		if (!seekentry(k1))
			return 0;
		doclen = storedn;
		fileread(packbuf, doclen, 1, datfh);
		doclen = unpack(workbuf, 0x96640, packbuf, doclen);
		if (doclen < 0)
			return 0;
		workbuf[doclen] = 0;
		if (k2 >= 0) {
			if (!seekentry(k2))
				return 0;
			doclen2 = storedn;
			fileread(packbuf, doclen2, 1, datfh);
			doclen2 = unpack(workbuf + doclen, 0x96640 - doclen, packbuf, doclen2);
			if (doclen2 < 0)
				return 0;
			doclen += doclen2;
			workbuf[doclen] = 0;
		}
		memset(packbuf, 0, 0x96640);
		droplf(workbuf, packbuf);
		for (doclen = 0; packbuf[doclen]; doclen++)
			;
		memset(chcut, 0, 200);
		if (s) {
			cutchapters(packbuf, s);
			for (doclen = 0; packbuf[doclen]; doclen++)
				;
		}
		if (raw) {
			expandstr(workbuf, packbuf);
			for (doclen = 0; workbuf[doclen]; doclen++)
				;
		} else
			memcpy(workbuf, packbuf, doclen);
	} else {
		for (doclen = 0; workbuf[doclen]; doclen++)
			;
	}
	parsetext(workbuf, doclen);
	for (docpick = 1; docpick < doclen; docpick++) {
		if (workbuf[docpick] == 123 && workbuf[docpick - 1] == 13)
			workbuf[docpick] = '[';
		else if (workbuf[docpick] == 125 && (workbuf[docpick + 1] == 123
		|| workbuf[docpick + 1] == 32 && workbuf[docpick + 2] == 13
		|| workbuf[docpick + 1] == 13))
			workbuf[docpick] = ']';
	}
	topicfirst = 0;
	for (;;) {
		checkquit();
		if (quitting)
			return 0;
		docpick = textmenu(mode);
		checkquit();
		if (quitting)
			return 0;
		/* s only shapes the code: with it docpick is tested in a register */
		// PORT: the integer as a pointer through intptr_t, which 64 bits widen
#ifdef PORT
		s = (char *)(intptr_t)docpick;
#else
		s = (char *)docpick;
#endif
		if (s == 0)
			break;
		curtopic = docpick - 1;
		if (curtopic > ntextlines)
			break;
		pagefx = 2;
		docpick = textwin(textlines[curtopic], 0, chcut[curtopic] ? 0xfe : 0, 0, 0, &menu1, 0, 0xff);
		if (docpick >= 10000) {
			docpick = 2;
			goto out;
		}
		mode = -1;
	}
	docpick = 1;
out:
	curtopic = 0xfff;
	pagefx = 1;
	return docpick;
}

void
lookicons(short n)
{
	gettile(3, n + 0x68, (unsigned char *)headicons[0]);
	gettile(3, n + 0x9c, (unsigned char *)headicons[1]);
	gettile(3, n + 0xa9, (unsigned char *)headicons[2]);
	gettile(3, n + 0xb6, (unsigned char *)headicons[3]);
}

void
photoicons(void)
{
	memcpy(headicons[0], headicons[4], 0x900);
	memcpy(headicons[1], headicons[5], 0x900);
	memcpy(headicons[2], headicons[6], 0x900);
	memcpy(headicons[3], headicons[7], 0x900);
}

void
stockicons(void)
{
	gettile(3, 0x6e, (unsigned char *)headicons[4]);
	gettile(3, 0xa2, (unsigned char *)headicons[5]);
	gettile(3, 0xaf, (unsigned char *)headicons[6]);
	gettile(3, 0xbc, (unsigned char *)headicons[7]);
}

short
entergame(unsigned short n)
{
	clearqueue();
	enterflag = 0;
	loadgame(1);
	if (loadmyface()) {
		myface = 1;
		photosum = myfacesum;
	} else {
		myface = 0;
		photosum = 0;
		loadface((unsigned char *)heroface, (unsigned char *)heroface2, units[0].face);
		lookicons(game.look);
	}
	dresshero();
	resumemap = curmap;
	curmap = 0xff;
	fillgauges();
	if (!travel(resumemap, playerx, playery, 0, 0, n, 0, 0)) {
		trvfailmsg = 0;
		travel(0xf, zones[1].x, zones[1].y, 0, 0, n, 0, 0);
		msgf(0x63, pick(texts + 0x155, texts + 0x15c, texts + 0x162, texts + 0x163, texts + 0x164, texts + 0x16b));
		if (esckey != 0xff)
			esckey = 0;
		return 0;
	}
	entermapsum = mapsum;
	if (afterupdate || afterinst || otherver || enteritems == 0xff && entersum == 1
	|| units[0].health <= 0 || enterflag || enterflag < 2 && (nmapitems != enteritems || entermapsum != entersum)) {
		enteritems = nmapitems;
		enterflag = 0;
		entersum = entermapsum;
	}
	closeshop();
	readinput();
	if (units[0].health <= 0)
		return 0;
	if (!firststart || firststart == 0xff) {
		if (units[0].berserk)
			music(0xb, 0);
		else if (playmode == 1) {
			if (cur != 0xc && cur != 1 && cur != 9)
				stopmusic();
		} else
			music(3, 0);
		loop = 0;
		listtorches();
		markunits();
		closeborder();
	}
	return 1;
}

char *
savedesc(void)
{
	memset(&savehdr, 0, sizeof savehdr);
	descplace = mapzone(curmap);
	if (descplace)
		descplace = zones[descplace].name;
	hdr = &savehdr;
	wsprintf(savehdr.name, ((char *)&game));
	descplace = descplace ? descplace : 0x62a;
	descplace -= 0x62a;
	hdr->place = descplace;
	hdr->hour = gamehour;
	hdr->day = gameday;
	return savehdr.name;
}

void
drawbar(short x)
{
	for (drawbarcol = 0; drawbarcol <= 1; drawbarcol++) {
		drawbarrow = 100;
		putpix1(x * 2 + 0x5a + drawbarcol, 100, 0xf5);
		for (drawbarrow++; drawbarrow < 120; drawbarrow++)
			putpix1(x * 2 + 0x5a + drawbarcol, drawbarrow, 0xf6);
		putpix1(x * 2 + 0x5a + drawbarcol, drawbarrow, 0xf7);
	}
}

DWORD
writedesc(HANDLE h, char *s)
{
	s[0x4f] = 0;
	return filewrite(s, 0x50, 1, h);
}

int
writelook(HANDLE h)
{
	// FIXED: writelook reports success whatever its writes did, so safeslot would take a slot whose face the disk refused. Report them.
#if defined(FEATURE_FIXES)
	int ok;

	lookword = 0xf10;
	ok = filewrite(&lookword, 2, 1, h) != 0;
	if (myface || game.look == 4) {
		lookword = 4;
		ok = filewrite(&lookword, 2, 1, h) && ok;
		ok = filewrite(headicons, 0x900, 1, h) && ok;
	} else
		ok = filewrite(&game.look, 2, 1, h) && ok;
	ok = filewrite(&units[0].head, 4, 1, h) && ok;
	ok = filewrite(&photosum, 4, 1, h) && ok;
	return ok;
#else
	lookword = 0xf10;
	filewrite(&lookword, 2, 1, h);
	if (myface || game.look == 4) {
		lookword = 4;
		filewrite(&lookword, 2, 1, h);
		filewrite(headicons, 0x900, 1, h);
	} else
		filewrite(&game.look, 2, 1, h);
	filewrite(&units[0].head, 4, 1, h);
	filewrite(&photosum, 4, 1, h);
	return 1;
#endif
}

int
savestr(HANDLE h, char *s)
{
	strword = 0x4321;
	filewrite(&strword, 2, 1, h);
	for (strword = 0; s[strword]; strword++)
		;
	filewrite(&strword, 2, 1, h);
	filewrite(s, strword, 1, h);
	return 1;
}

// FIXED: openwhole and closewhole write a file whole, for savegame, saveobjs, savemap, and safeslot below: under Teudogar_@.SAV, the name of slot 0, which serves them as the name of a file being written, then in the file's place in one rename, so that a write that fails, as on a full disk, leaves the old file whole, where CREATE_ALWAYS empties it at its open.
#if defined(FEATURE_FIXES)
/* openwhole opens Teudogar_@.SAV to write, emptied, and reports whether it could */
int
openwhole(HANDLE *h)
{
	char tmp[16];

	wsprintf(tmp, texts + 0xa492, 0x40);
	return fopenw(h, tmp);
}

/* closewhole closes h, which openwhole opened, puts its file in the place of name when ok holds and the close went through, and else deletes it, and reports whether the file took the place */
int
closewhole(HANDLE *h, char *name, int ok)
{
	char tmp[16];

	if (*h == 0 || !fileclose(h)) {
		*h = 0;
		ok = 0;
	}
	wsprintf(tmp, texts + 0xa492, 0x40);
	if (ok)
		ok = MoveFileExA(tmp, name, MOVEFILE_REPLACE_EXISTING);
	if (!ok)
		deletefile(tmp);
	return ok;
}
#endif

// FIXED: safeslot saves as saveslot below does, through openwhole and closewhole, with every write and close checked.
#if defined(FEATURE_FIXES)
char slotfailed;	/* whether the last save or load of a slot failed */

/* putfile appends the file name to the slot that h writes, its length and bytes, a length of 0 when it is missing, and reports whether the writes went through */
static int
putfile(HANDLE h, char *name)
{
	HANDLE in;

	savelen = 0;
	if (fopenr(&in, name)) {
		savelen = fileread(workbuf, 1, 0x96640, in);
		fileclose(&in);
	}
	if (!filewrite(&savelen, 4, 1, h))
		return 0;
	return savelen == 0 || filewrite(workbuf, savelen, 1, h);
}

/* safeslot saves the game into slot with the description desc, as saveslot does, and reports whether it did */
static int
safeslot(unsigned short slot, char *desc)
{
	unsigned short last;
	short wasauto;
	int ok;

	if (slot == 0)
		return 1;
	closedat();
	last = lastslot;
	wasauto = autoslot;
	lastslot = slot;
	if ((int)slot == autoslot)
		autoslot = 0;
	ok = savegame();
	if (!saveobjs(curmap, 0))
		ok = 0;
	if (!savemap(curmap))
		ok = 0;
	outsav = 0;
	if (ok)
		openwhole(&outsav);
	cdsaves();
	if (outsav == 0 || !writedesc(outsav, desc))
		ok = 0;
	if (ok && !writelook(outsav))
		ok = 0;
	if (ok) {
		if (fopenr(&savein, texts + 0xa435)) {
			fileclose(&savein);
			ok = putfile(outsav, texts + 0xa435);
		} else
			ok = 0;
	}
	if (ok) {
		savelen = 1;
		memset(workbuf, 0, 1);
		ok = filewrite(&savelen, 4, 1, outsav) && filewrite(workbuf, savelen, 1, outsav);
	}
	for (savemapi = 0; ok && savemapi < 80; savemapi++) {
		if (modeset)
			putgauge(1, 0x128, 0xf0, savemapi * 100 / 80);
		for (savekind = 2; ok && savekind <= 3; savekind++) {
			wsprintf(savename, texts + 0xa45c, savemapi, savekind);
			ok = putfile(outsav, savename);
		}
	}
	if (ok)
		ok = putfile(outsav, texts + 0x9bfd);
	wsprintf(savename, texts + 0xa492, slot + 0x40);
	ok = closewhole(&outsav, savename, ok);
	if (!ok) {
		lastslot = last;
		autoslot = wasauto;
		wsprintf(workbuf, strs[1150], cancelbutton());
		if (modeset)
			msgbox(workbuf, 0, 1);
		if (esckey != 0xff)
			esckey = 0;
	}
	return ok;
}
#endif

void
saveslot(unsigned short slot, char *desc)
{
	// FIXED: saveslot writes over the slot itself, which CREATE_ALWAYS empties at its open, and goes on past the writes that fail, so a full disk loses the slot and keycmd says that the game saved. Save through safeslot.
#if defined(FEATURE_FIXES)
	slotfailed = !safeslot(slot, desc);
#else
	if (slot == 0)
		return;
	closedat();
	if (slot)
		lastslot = slot;
	if ((int)slot == autoslot)
		autoslot = 0;
	savegame();
	saveobjs(curmap, 0);
	savemap(curmap);
	wsprintf(savename, texts + 0xa492, slot + 0x40);
	fopenw(&outsav, savename);
	cdsaves();
	if (outsav == 0 || !writedesc(outsav, desc)) {
	fail:
		wsprintf(workbuf, strs[1150], cancelbutton());
		if (modeset)
			msgbox(workbuf, 0, 1);
		if (esckey != 0xff)
			esckey = 0;
	} else {
		writelook(outsav);
		if (!fopenr(&savein, texts + 0xa435))
			goto fail;
		savelen = fileread(workbuf, 1, 0x96640, savein);
		fileclose(&savein);
		if (!filewrite(&savelen, 4, 1, outsav))
			goto fail;
		if (!filewrite(workbuf, savelen, 1, outsav) && savelen)
			goto fail;
		savelen = 1;
		memset(workbuf, 0, 1);
		filewrite(&savelen, 4, 1, outsav);
		filewrite(workbuf, savelen, 1, outsav);
		for (savemapi = 0; savemapi < 80; savemapi++) {
			if (modeset)
				putgauge(1, 0x128, 0xf0, savemapi * 100 / 80);
			for (savekind = 2; savekind <= 3; savekind++) {
				wsprintf(savename, texts + 0xa45c, savemapi, savekind);
				if (fopenr(&savein, savename)) {
					savelen = fileread(workbuf, 1, 0x96640, savein);
					fileclose(&savein);
				} else
					savelen = 0;
				filewrite(&savelen, 4, 1, outsav);
				filewrite(workbuf, savelen, 1, outsav);
			}
		}
		if (slot != 0) {
			if (fopenr(&savein, texts + 0x9bfd)) {
				savelen = fileread(workbuf, 1, 0x96640, savein);
				fileclose(&savein);
			} else
				savelen = 0;
			filewrite(&savelen, 4, 1, outsav);
			filewrite(workbuf, savelen, 1, outsav);
		} else {
			savelen = 0;
			filewrite(&savelen, 4, 1, outsav);
		}
	}
	if (outsav)
		fileclose(&outsav);
#endif
}

DWORD
copyfile(char *src, char *dst, char *buf, DWORD size)
{
	if (!fopenr(&copyh, src))
		return 0;
	copylen = fileread(buf, 1, size, copyh);
	fileclose(&copyh);
	if (copylen == 0)
		return 0;
	if (copymode == 1) {
		if (buf[0] == 0x42 && buf[1] == 0x4d) {
			buf[0] = 0x11;
			buf[1] = 0x15;
		}
	} else if (copymode == 2) {
		if (buf[0] == 0x11 && buf[1] == 0x15) {
			buf[0] = 0x42;
			buf[1] = 0x4d;
		}
	}
	if (!fopenw(&copyh, dst))
		return 0;
	copylen = filewrite(buf, 1, copylen, copyh);
	fileclose(&copyh);
	return copylen;
}

int
autohdr(void)
{
	wsprintf(printbuf, texts + 0xa492, 0x4a);
	if (!fopenw(&autofile, printbuf))
		return 0;
	writedesc(autofile, savedesc());
	writelook(autofile);
	fileclose(&autofile);
	return 1;
}

int
autosave(void)
{
	if (!maploaded)
		return 0;
	savegame();
	saveobjs(curmap, 0);
	savemap(curmap);
	autohdr();
	copyfile(texts + 0xa435, texts + 0xa46a, workbuf, 0x96640);
	wsprintf(savename, texts + 0xa45c, curmap, 2);
	wsprintf(autoname, texts + 0xa477, curmap, 2);
	copyfile(savename, autoname, workbuf, 0x96640);
	wsprintf(savename, texts + 0xa45c, curmap, 3);
	wsprintf(autoname, texts + 0xa477, curmap, 3);
	copyfile(savename, autoname, workbuf, 0x96640);
	return 1;
}

int
hasautosave(void)
{
	if (exists(texts + 0xa46a))
		return 1;
	return 0;
}

int
autoload(void)
{
	if (!hasautosave())
		return 0;
	cdsaves();
	copyfile(texts + 0xa46a, texts + 0xa435, workbuf, 0x96640);
	for (loadmapi = 0; loadmapi < 80; loadmapi++) {
		if (modeset)
			putgauge(1, 0x128, 0xf0, loadmapi * 100 / 80);
		for (loadkind = 2; loadkind <= 3; loadkind++) {
			wsprintf(savename, texts + 0xa477, loadmapi, loadkind);
			wsprintf(autoname, texts + 0xa45c, loadmapi, loadkind);
			copyfile(savename, autoname, workbuf, 0x96640);
		}
	}
	copyfile(texts + 0xa485, texts + 0x9bfd, workbuf, 0x96640);
	return 1;
}

int
autocopy(void)
{
	cdsaves();
	copyfile(texts + 0xa435, texts + 0xa46a, workbuf, 0x96640);
	for (copymapi = 0; copymapi < 80; copymapi++) {
		if (modeset)
			putgauge(1, 0x128, 0xf0, copymapi * 100 / 80);
		for (copykind = 2; copykind <= 3; copykind++) {
			wsprintf(savename, texts + 0xa477, copymapi, copykind);
			wsprintf(autoname, texts + 0xa45c, copymapi, copykind);
			copyfile(autoname, savename, workbuf, 0x96640);
		}
	}
	copyfile(texts + 0x9bfd, texts + 0xa485, workbuf, 0x96640);
	return 1;
}

// FIXED: copyhdr gives the AutoSave the header of the slot that loadslot copies into it, where loadslot describes the game that ran before the load.
#if defined(FEATURE_FIXES)
/* copyhdr copies the header of slot, as readhdr reads it, into Teudogar_J.SAV, and reports whether it did */
static int
copyhdr(unsigned short slot)
{
	HANDLE h;
	DWORD n, len;
	unsigned short ver, look;
	unsigned char *p;

	wsprintf(hdrname, texts + 0xa492, slot + 0x40);
	if (!fopenr(&h, hdrname))
		return 0;
	p = (unsigned char *)workbuf;
	n = fileread(p, 1, 0x50 + 4 + 0x900 + 8, h);
	fileclose(&h);
	if (n < 0x52)
		return 0;
	ver = p[0x50] | p[0x51] << 8;
	len = 0x52;
	look = ver;
	if (ver >= 0xf0f) {
		look = p[0x52] | p[0x53] << 8;
		len += 2;
	}
	if (look == 4)
		len += 0x900;
	if (ver >= 0xf0f)
		len += 4;
	if (ver >= 0xf10)
		len += 4;
	if (n < len)
		return 0;
	wsprintf(hdrname, texts + 0xa492, 0x4a);
	if (!fopenw(&h, hdrname))
		return 0;
	n = filewrite(p, len, 1, h);
	fileclose(&h);
	return n != 0;
}
#endif

int
loadslot(unsigned short slot, unsigned short mode)
{
	closedat();
	if (slot == 10) {
		if (autoload()) {
			lastslot = slot;
			return 1;
		}
		return 0;
	}
	loadauto = 0;
	if (!autolevel || mode == 2 || slot && autoslot == slot)
		;
	else
		loadauto = 1;
	if (slot == 0) {
		wsprintf(savename, texts + 0x9a69);
		cdprog();
	} else
		wsprintf(savename, texts + 0xa492, slot + 0x40);
	fopenr(&insav, savename);
	if (slot == 0) {
		if (mode == 2) {
			makedir(texts + 0xa4ee);
			cdmaps();
		} else
			cdsaves();
	}
	if (insav == 0)
		return 0;
	fileread(savename, 0x50, 1, insav);
	if (slot)
		lastslot = slot;
	fileread(&loadver, 2, 1, insav);
	if (loadver >= 0xf0f)
		fileread(&loadlook, 2, 1, insav);
	else
		loadlook = loadver;
	if (loadlook == 4)
		for (loadk = 0; loadk < 0x30; loadk++)
			fileread(savename, 0x30, 1, insav);
	if (loadver >= 0xf0f)
		fileread(&slotpart, 4, 1, insav);
	if (loadver >= 0xf10)
		fileread(&slotpart, 4, 1, insav);
	fileread(&slotpart, 4, 1, insav);
	if (slotpart)
		fileread(workbuf, slotpart, 1, insav);
	if (mode == 2 || mode == 3)
		;
	else if (mode || slot == 0) {
		units[0].health = 0;
		if (playmode != 1)
			setcombat(0);
		savegame();
		if (loadauto)
			copyfile(texts + 0xa435, texts + 0xa46a, workbuf, 0x96640);
	} else {
		if (fopenw(&loadout, texts + 0xa435)) {
			filewrite(workbuf, slotpart, 1, loadout);
			fileclose(&loadout);
		}
		if (loadauto && fopenw(&loadout, texts + 0xa46a)) {
			filewrite(workbuf, slotpart, 1, loadout);
			fileclose(&loadout);
		}
	}
	enterflag = 0;
	fileread(&slotpart, 4, 1, insav);
	if (slotpart)
		fileread(workbuf, slotpart, 1, insav);
	for (loadlook = 0; loadlook < 80; loadlook++) {
		if (slot && modeset)
			putgauge(1, 0x128, 0xf0, loadlook * 100 / 80);
		for (loadk = 2; loadk <= 3; loadk++) {
			fileread(&slotpart, 4, 1, insav);
			if (slotpart) {
				fileread(workbuf, slotpart, 1, insav);
				wsprintf(savename, texts + 0xa45c, loadlook, loadk);
				if (fopenw(&loadout, savename)) {
					filewrite(workbuf, slotpart, 1, loadout);
					fileclose(&loadout);
				}
				if (loadauto) {
					wsprintf(savename, texts + 0xa477, loadlook, loadk);
					if (fopenw(&loadout, savename)) {
						filewrite(workbuf, slotpart, 1, loadout);
						fileclose(&loadout);
					}
				}
			}
		}
	}
	fileread(&slotpart, 4, 1, insav);
	if (slotpart == 0) {
		if (slot != 0) {
			facetmpl(0);
			if (loadauto)
				copyfile(texts + 0x9bfd, texts + 0xa485, workbuf, 0x96640);
		}
	} else {
		fileread(workbuf, slotpart, 1, insav);
		if (slot != 0) {
			if (fopenw(&loadout, texts + 0x9bfd)) {
				filewrite(workbuf, slotpart, 1, loadout);
				fileclose(&loadout);
			}
			if (loadauto && fopenw(&loadout, texts + 0xa485)) {
				filewrite(workbuf, slotpart, 1, loadout);
				fileclose(&loadout);
			}
		}
	}
	fileclose(&insav);
	if (mode == 2)
		cdsaves();
	if (loadauto) {
		autoslot = slot;
		// FIXED: autohdr describes the game in memory, the one before the load, while the E files now hold the slot's, so the menu shows for J the hero, place, day, and face of another game. Copy the slot's header.
#if defined(FEATURE_FIXES)
		if (slot == 0 || !copyhdr(slot))
			autohdr();
#else
		autohdr();
#endif
	}
	return 1;
}

void
newgame(void)
{
	Kind *k;

	memset(((char *)&game), 0, 0x5f8);
	memset(szvars, 0, 0x190);
	memcpy(&kinds[19], &kinds[18], 0x41);
	newi = newunit(0x16, 0);
	memcpy(&units[0], &units[newi], sizeof(Unit));
	delunit(newi);
	for (newi = 0; newi < 30; newi++)
		game.skills[newi] = kinds[18].aptitude[newi] * 70;
	units[0].picset = 0;
	resetanims();
	bloodtill = 0;
	memset(((Note *)notes), 0, 0x7d0);
	nnotes = 0;
	minmarks = 1;
	memset(realms, 0, 0x6b8);
	realms[0].item = 0x2e4;
	realms[1].item = 0x2e4;
	realms[2].item = 0x2e4;
	realms[3].item = 0x2e4;
	realms[4].item = 0x2e4;
	puton(0, 0x60ef01d5);
	puton(0, 0x60e401ca);
	puton(0, 0x40ec01ad);
	puton(0, 0x40e101a0);
	puton(0, 0x60e401b7);
	for (newj = 0; newj < 3; newj++)
		units[0].items[newj] = 0;
	calcstats(&units[0]);
	curregion = 0;
	gametime = 1;
	curmap = zones[1].id;
	steptick = 0;
	stepframe = 0;
	playerx = zones[1].inx;
	playery = zones[1].iny;
	turncount = 0;
	hourtime = 0;
	units[0].x = playerx;
	units[0].y = playery;
	units[0].pose = 0;
	units[0].dirx = zones[1].indx;
	units[0].diry = zones[1].indy;
	trackregion();
	playmode = 1;
	busyturns = 0;
	combatkind = 0;
	memset(game.foes, 0, 0x1e);
	initpanels();
	memset(panelstack, 0xff, 0x10);
	memset(quickacts, 0, 0xa);
	setbit(game.known, 1, 1);
	quickacts[0] = 1;
	setbit(game.known, 2, 1);
	quickacts[1] = 2;
	setbit(game.known, 8, 1);
	quickacts[2] = 8;
	setbit(game.known, 9, 1);
	quickacts[3] = 9;
	setbit(game.known, 0xd, 1);
	quickacts[4] = 0xd;
	setbit(game.known, 0x12, 1);
	quickacts[5] = 0x12;
	setbit(game.known, 0x13, 1);
	quickacts[6] = 0x13;
	setbit(game.known, 0x1b, 1);
	quickacts[7] = 0x1b;
	setbit(game.known, 0x1c, 1);
	game.look = 2;
	units[0].face = 2;
	lookicons(2);
	game.colors[0] = lookcolors[0][4];
	game.colors[1] = lookcolors[1][4];
	game.colors[2] = lookcolors[2][4];
	game.colors[3] = lookcolors[3][4];
	wsprintf(((char *)&game), pick(texts + 0x8c29, texts + 0x8c33, texts + 0x8c3d, texts + 0x8c3e, texts + 0x8c3f, texts + 0x8c49));
	k = kindof(&units[0]);
	units[0].sympathy = 4;
	units[0].clan = 0xff;
	units[0].health = k->maxhealth;
	shoewarns = 0;
	clearavenge();
}

void
startgame(void)
{
	memset(namebuf, 0, 0x9c4);
	latinp = namebuf;
	wsprintf(namebuf, ((char *)&game));
	lowerstr(latinp);
	latinq = game.latinname;
	while (*latinp) {
		if (latinp[0] == 's' && latinp[1] == 'c' && latinp[2] == 'h') {
			*latinq++ = 'c';
			latinp += 3;
		} else if (latinp[0] == 'c' && latinp[1] == 'h') {
			*latinq++ = 'c';
			latinp += 2;
		} else if (latinp[0] == 'c' && latinp[1] == 'k') {
			*latinq++ = 'c';
			latinp += 2;
		} else if (latinp[0] == 'r' && latinp[1] == 'r') {
			*latinq++ = 'r';
			latinp += 2;
		} else if (latinp[0] == 'd' && latinp[1] == 't') {
			*latinq++ = 'd';
			latinp += 2;
		} else if (latinp[0] == 'i' && latinp[1] == 'e') {
			*latinq++ = 'i';
			latinp += 2;
		} else if (latinp[0] == 'e' && latinp[1] == 'i') {
			*latinq++ = 'i';
			latinp += 2;
		} else if (latinp[0] == 'p' && latinp[1] == 'h') {
			*latinq++ = 'f';
			latinp += 2;
		} else if (latinp[0] == 'f' && latinp[1] == 'f') {
			*latinq++ = 'f';
			latinp += 2;
		} else if (latinp[0] == 'p' && latinp[1] == 'f') {
			*latinq++ = 'f';
			latinp += 2;
		} else if (latinp[0] == 't' && latinp[1] == 'z') {
			*latinq++ = 'c';
			latinp += 2;
		} else if (latinp[0] == 't' && latinp[1] == 'h') {
			*latinq++ = 't';
			latinp += 2;
		} else if (*latinp == 'h')
			latinp++;
		else if (*latinp == 'j') {
			if (latinp[1] != 'i')
				*latinq++ = 'i';
			latinp++;
		} else if (*latinp == 'k') {
			*latinq++ = 'c';
			latinp++;
		} else if (*latinp == 'w') {
			if (latinp != namebuf)
				*latinq++ = 'v';
			latinp++;
		} else if (*latinp == 'y') {
			*latinq++ = 'i';
			latinp++;
		} else if (*latinp == 'z') {
			*latinq++ = 'c';
			latinp++;
		} else
			*latinq++ = *latinp++;
	}
	if (latinq > game.latinname) {
		if (latinq[-1] == 'r')
			*latinq++ = 'i';
		else if (latinq[-1] == 'o')
			latinq--;
		else if (latinq > game.latinname + 1 && latinq[-1] == 's'
		&& (latinq[-2] == 'u' || latinq[-2] == 'o' || latinq[-2] == 'i'
		|| latinq[-2] == 'e' || latinq[-2] == 'a'))
			latinq -= 2;
		else if (latinq[-1] == 'e') {
			latinq[-1] = 'i';
			*latinq++ = 'c';
		} else if (latinq[-1] == 'a')
			*latinq++ = 'c';
		else if (latinq[-1] == 'u')
			latinq--;
	}
	*latinq++ = 'u';
	*latinq++ = 's';
	*latinq = 0;
	game.latinname[0] = upper(game.latinname[0]);
	packitem(0xd04e9);
	packitem(0x40018e);
	packitem(0x80018f);
	packitem(0x80018f);
	packitem(newitem(0x968));
	packitem(0x3ff0221);
	packitem(0x3ff0221);
	packitem(0x3ff0221);
	units[0].health = kindof(&units[0])->maxhealth;
	if (units[0].head)
		setwear(&units[0].head, 2);
	if (units[0].body)
		setwear(&units[0].body, 2);
	if (units[0].legs)
		setwear(&units[0].legs, 2);
	if (units[0].hand)
		setwear(&units[0].hand, 2);
	if (units[0].belt)
		setwear(&units[0].belt, 2);
	if (units[0].offhand && !istorch(units[0].offhand))
		setwear(&units[0].offhand, 2);
	if (units[0].feet)
		setwear(&units[0].feet, 2);
	units[0].scene = 0;
	units[0].plea = 0;
	units[0].mercy = 0;
	gameday = 1;
	gamehour = 8;
	game.hungercount = 0xf;
	game.tiredcount = 0x14;
	setlevels();
	calcstats(&units[0]);
	snapskills(1);
	setbit(game.knownzones, 1, 1);
	pickcall();
	usepal(0);
	initpanels();
	openhero();
	memset(panelstack, 0xff, 0x10);
	homepanels();
	homepanel2();
	lastlight = 99;
	regiontag = 99;
	talkactive = 0;
	statshown = 0;
	lastwarm = 0;
	sensesleft = 0;
	busyturns = 0;
	setdaylight();
	lookdone();
	clearmsgs();
	cleartexts();
	if (firststart) {
		if (!((char *)invram)[0] || !((char *)invram)[1])
			scrolling = 3;
		else if (!((char *)invram)[4] && !((char *)invram)[5])
			scrolling = 2;
		else if (((char *)invram)[4] && ((char *)invram)[5] && ((char *)invram)[3])
			scrolling = 1;
		else
			scrolling = 1;
		if (blittime > 100 && scrolling < 1)
			scrolling = 1;
		if (keyblittime > 90 && scrolling < 1)
			scrolling = 1;
	}
	notedkind = 0xff;
	notedposx = 0xff;
	notedposy = 0xff;
	noteditem = 0xff;
	notedunit = 0xffff;
	askedonce = 0;
	askedunit = 0xff;
	notedmap = 0;
	notedtime = 0;
	spotday = 0;
	spotsong = 0;
	syncplayer();
	savebyte3 = 0;
	knockedout = 0;
	mapstate[0x22] = 99;
	clearturn();
	fillgauges();
	game.score = 0;
	merciful = 0;
	pleader = 0;
	talker = 0;
	setcurregion();
	regions[0].tag = 99;
}

void
drawhero(void)
{
	units[0].health = kindof(&units[0])->maxhealth;
	clear48(0x134, 0x20);
	drawoutfit(0xd4, 0xa, &units[0], 0, 0);
	units[0].health = 0;
}

void
drawcreate(char *a)
{
	put0(picbuf);
	putwint(0x110, 0xbe, 0x14);
	drawhero();
	text3e1(0, 0xffff, 0x104, a);
}

void
createbg(void)
{
	put0(picbuf);
	backto2();
	pagedone();
}

void
lookface(short a, char c)
{
	if (c == 0x57)
		putface(a * 108 + 0x30, 0x12c, curblink ? (workbuf + 0x1fa40) + a * 0x32a0 : workbuf + a * 0x32a0);
}

void
photohint(void)
{
	hintw = hinti = 0;
	for (; hinti < 7; hinti++) {
		hintws[hinti] = textwidth(strs[1466 + hinti]);
		if (hintws[hinti] > hintw)
			hintw = hintws[hinti];
	}
	for (hinti = 0; hinti < 7; hinti++)
		drawtext2o(0, (hintw - hintws[hinti]) / 2 + (639 - hintw) + (hinti * 14 + 172) * 640, strs[1466 + hinti]);
}

void
picklook(short a, short b)
{
	if (a == 0xff)
		return;
	showscr();
	if (b) {
		game.look = a;
		if (a == 4)
			photoicons();
		else
			lookicons(a);
		game.colors[0] = lookcolors[0][a];
		units[0].face = a;
		game.colors[1] = lookcolors[1][a];
		game.colors[2] = lookcolors[2][a];
		game.colors[3] = lookcolors[3][a];
	}
	drawhero();
	backto2();
	if (b) {
		lookface(a, 0x57);
		if (a == 4)
			photohint();
	}
}

void
begingame(unsigned short slot, short mode)
{
	int bad;
	DWORD n;

	ownphoto = 0;
	music(0xc, 1);
	clearvars(workbuf);
	hidecursor();
	bad = 0;
	if (mode == 1)
		intro(2);
	usepal(0);
	starttimer3();
	newgame();
	loadslot(0, slot);
	n = seal.n;
	if (n > 0x24)
		n = 0x24;
	// PORT: the seal sums the bytes of the sold Teudogar.Exe, which the port replaces
#ifndef PORT
	if (crcbuf(&seal.n, n - 12) != seal.crc)
		bad = 1;
	if (crcname(exepathbuf, 0, seal.off) != seal.crc1)
		bad = 1;
	if (crcname(exepathbuf, seal.off + n, 0xfffffff) != seal.crc2)
		bad = 1;
#endif
	newgame();
	units[0].health = 0;
	enterflag = 0;
	enteritems = 0xff;
	entersum = 1;
	otherver = 0;
	game.id = gamesbegun;
	gamesbegun++;
	savecfg();
	savegame();
	maploaded = 1;
	begini = 0x9c4;
	if (GetUserName(namebuf, &begini) && begini > 2 && begini <= 10 && iswordc(namebuf[0])) {
		namebuf[begini] = 0;
		namebuf[19] = 0;
		for (begini = 0; namebuf[begini] != '\0'; begini++)
			if (!iswordc(namebuf[begini]))
				goto def;
		if (streqi(namebuf, texts + 0xab74) || streqi(namebuf, texts + 0xab7a) || streqi(namebuf, texts + 0xab7f))
			goto def;
		wsprintf(heroname, namebuf);
	} else {
def:
		wsprintf(heroname, pick(texts + 0x8c29, texts + 0x8c33, texts + 0x8c3d, texts + 0x8c3e, texts + 0x8c3f, texts + 0x8c49));
	}
	if (cheat && keys['W'] & 0x80) {
		wsprintf(((char *)&game), pick(texts + 0x8c29, texts + 0x8c33, texts + 0x8c3d, texts + 0x8c3e, texts + 0x8c3f, texts + 0x8c49));
		esckey = 1;
	} else {
		memset(workbuf, 0, 0x96640);
		ownphoto = loadmyface();
		if (ownphoto) {
			loadface((unsigned char *)(workbuf + 0xca80), (unsigned char *)(workbuf + 0x2c4c0), 6);
			if (!memdiff(heroface, workbuf + 0xca80, 0x32a0) && !memdiff(heroface2, workbuf + 0x2c4c0, 0x32a0))
				ownphoto = 0;
		} else {
			loadface((unsigned char *)heroface, (unsigned char *)heroface2, 6);
			stockicons();
			lookicons(6);
		}
		for (begini = 0; begini < 4; begini++)
			loadface((unsigned char *)(workbuf + begini * 0x32a0), (unsigned char *)((workbuf + 0x1fa40) + begini * 0x32a0), begini);
		memcpy(workbuf + 0xca80, heroface, 0x32a0);
		memcpy(workbuf + 0x2c4c0, heroface2, 0x32a0);
		loadscr(0x2086, (unsigned char *)picbuf, 0, packbuf, 0x96640);
		fadeslow(0);
		game.look = 2;
		for (;;) {
			beginredo = 0;
again:
			drawcreate(strs[262]);
			memset(&menu1, 0, sizeof menu1);
			menu1.n = 5;
			for (begini = 0; begini < menu1.n; begini++) {
				menu1.rects[begini].y = 299;
				menu1.rects[begini].x = begini * 108 + 0x2f;
				menu1.rects[begini].w = 0x6e;
				menu1.rects[begini].h = 0x7a;
				menu1.enabled[begini] = 1;
				memcpy(packbuf, workbuf + begini * 0x32a0, 0x32a0);
				for (beginv = 0; beginv < 0x32a0; beginv++)
					if (packbuf[beginv])
						packbuf[beginv] = darker4[0x50 + lum[packbuf[beginv]]];
				putface(begini * 108 + 0x30, 300, packbuf);
			}
			backto2();
			pagedone();
			save0();
			menu1.fn = picklook;
			inkdark();
			waitforup = 1;
			do
				pollinput();
			while (keycode || mouseleft || mouseright);
			if (beginredo)
				flip();
			else if (!palblack) {
				waitinput(500);
				dissolvet();
			}
			readinput();
			drawcursor();
			usepal(0);
			picink();
			while (mouseleft || mouseright)
				readinput();
			leftclick = 0;
			if (esckey == 0xff)
				goto quit;
			menuslow = 1;
			if (hoverx > 0x1e0)
				setmouse(0x1e0, (short)hovery);
			readinput();
			menuanim = 1;
pick:
			begini = runmenu(&menu1, 0, 0, 0);
			if (needredraw(begini)) {
				beginredo = 1;
				goto again;
			}
			if (!begini) {
				if (esckey == 0xff) {
quit:
					loop = 0;
					menuanim = 0;
					menuslow = 0;
					menusnd = 0;
					inkshade();
					hidecursor();
					fadefast(0);
					quitting = 1;
					return;
				}
				if (mode == 2)
					goto pick;
				loop = 0;
				drawcreate(empty);
				dissolvet();
				fadeslow(0);
				usepal(0);
				menuslow = 0;
				menusnd = 0;
				inkshade();
				return;
			}
			menuanim = 0;
			menuslow = 0;
			menusnd = 0;
			waitforup = 0;
			memcpy(((char (*)[100])scrbuf), picbuf, 640 * 480);
			inkshade();
			beginredo = 0;
			for (;;) {
				hidecursor();
				drawcreate(empty);
				if (beginredo)
					flip();
				else
					dissolvet();
				drawcreate(strs[261]);
				backto2();
				pagedone();
				if (beginredo)
					flip();
				else {
					waitinput(500);
					dissolvet();
				}
				music(0xc, 0);
				if (esckey == 0xff)
					goto quit;
				esckey = 0;
				beginv = inputtext(320, 320, heroname, 16, 0, 1, 0, 2, 1, 0, 0xb8, 0x7c, 0);
				if (!needredraw(beginv))
					break;
				inkeep = 1;
				beginredo = 1;
			}
			if (esckey == 0xff)
				goto quit;
			if (!esckey && beginv == 1)
				break;
			esckey = 0;
			drawcreate(empty);
			dissolvet();
		}
		for (namep = heroname; *namep; namep++)
			if (namep == heroname || namep > heroname && namep[-1] == ' ')
				*namep = upper(*namep);
			else
				*namep = lower(*namep);
		while (namep[-1] == ' ')
			*--namep = 0;
		memcpy(((char *)&game), heroname, 0x14);
		// PORT: a name of its own, which C wants for an overloaded function
#ifdef PORT
		waitnokey();
#else
		waitrelease();
#endif
		hidecursor();
		drawcreate(empty);
		dissolvet();
		waitinput(500);
		fadeslow(0);
	}
	fadefast(0);
	usepal(0);
	if (game.look != 4) {
		if (ownphoto)
			exportface();
		facetmpl(0);
	} else if (!ownphoto)
		facetmpl(1);
	startgame();
	enterflag = 0;
	enteritems = 0xff;
	entersum = 1;
	otherver = 0;
	if (bad) {
		beginv = 0x24e;
		setflag(0x24e, 1);
	}
	savegame();
	if ((!cheat || !esckey) && mode == 1) {
		waitinput(1500);
		intro(0);
	}
	usepal(0);
	enteritems = 0xff;
	entersum = 1;
	fadeslow(0);
	entergame(1);
	waitrelease(0);
	lastslot = 0;
	if (autolevel)
		autosave();
	msgf(0x63, strs[912]);
	loop = 0;
	lastscan = 0;
}

short
readhdr(char *desc, int *mode, int *p3, char *pic, int *p5, short slot)
{
	desc[0] = 0;
	memset(pic, 0, 0x900);
	wsprintf(hdrname, texts + 0xa492, slot + 0x40);
	if (!fopenr(&hdrfile, hdrname))
		return 0;
	if (fileread(desc, 1, 0x50, hdrfile) < 0x50) {
		fileclose(&hdrfile);
		return 0;
	}
	fileread(&hdrver, 2, 1, hdrfile);
	if (hdrver >= 0xf0f)
		fileread(&hdrlook, 2, 1, hdrfile);
	else
		hdrlook = hdrver;
	*mode = hdrlook;
	if (hdrlook == 4)
		fileread(pic, 0x900, 1, hdrfile);
	else if (hdrlook < 4)
		gettile(3, hdrlook + 0x68, (unsigned char *)pic);
	if (hdrver >= 0xf0f)
		fileread(p5, 4, 1, hdrfile);
	else
		*p5 = 0;
	if (hdrver >= 0xf10)
		fileread(p3, 4, 1, hdrfile);
	else
		*p3 = 0;
	fileclose(&hdrfile);
	return 1;
}

void
slotinfo(short a, short b)
{
	short w;

	if (a == 0xff)
		return;
	a++;
	if (!b)
		return;
	pillshade8(menu0.rects[a - 1].x, menu0.rects[a - 1].y, menu0.rects[a - 1].w, menu0.rects[a - 1].h);
	if (a >= 1 && a <= 10 && ((char (*)[200])(workbuf + 0x3e8))[a][0]) {
		text3grey(0, 0xffff, 0x4a, ((char (*)[200])(workbuf + 0x3e8))[a]);
		w = text3x;
		text3grey(0, 0xffff, 0x5e, ((char (*)[200])(workbuf + 0x3e8))[a] + 0x64);
		if (w > text3x)
			w = text3x;
		w -= 60;
		w /= 4;
		w *= 4;
		put48t0(w, 0x4b, scrbuf + a * 0x900);
		slothelm = ((int (*)[2])(scrbuf + 0x6d88))[a][0];
		if (slothelm) {
			pickworn(slothelm, 1);
			if (wornpic) {
				wornx -= 0x60;
				worny -= 0x16;
				putwint(wornx + w, worny + 0x4b, (int)wornpic);
			}
		}
	}
}

void
slotmenu(char load)
{
	slottext[0] = 0x1e;
	slottext[1] = 0x30;
	slottext[2] = 0x2e;
	slottext[3] = 0x20;
	slottext[4] = 0x12;
	slottext[5] = 0x21;
	slottext[6] = 0x22;
	slottext[7] = 0x23;
	slottext[8] = 0x17;
	slottext[9] = 0x24;
	slottext[10] = 0x32;
	memset(((char (*)[100])scrbuf), 0, 0x4b9c4);
	menu0.n = 10;
	memcpy(&menu0.enabled[40], slottext, 11);
	menu0.color = 0xf4;
	menu0.keys2[0] = 0x3c;
	menu0.keys2[1] = 0x3d;
	menu0.keys2[2] = 0x1f;
	menu0.keys2[3] = 0x26;
	pgtoback();
	// FEATURE: features: the build's own format of the menu's title
#ifdef FEATURES
	wsprintf(slottext, feattext(texts + 0xb087), load ? (texts + 0xb4b5) : (texts + 0xb4aa), load ? strs[263] : strs[264]);
#else
	wsprintf(slottext, texts + 0xb087, load ? (texts + 0xb4b5) : (texts + 0xb4aa), load ? strs[263] : strs[264]);
#endif
	upperstr(slottext + 8);
	text3grey2(0, 0xffff, 0x22, slottext);
	for (sloti = 1; load && sloti <= 10 || sloti <= 9; sloti++) {
		wsprintf(slottext, texts + 0xa492, sloti + 0x40);
		slotplace = 0x62a;
		if (findfirst(slottext)) {
			findclose();
			if (sloti - 1 == 9)
				break;
			wsprintf(workbuf, texts + 0xb145, sloti + 0x40, strs[265]);
			wsprintf(((char (*)[200])(workbuf + 0x3e8))[sloti], empty);
			wsprintf(((char (*)[200])(workbuf + 0x3e8))[sloti] + 0x64, empty);
			memset((char *)((char (*)[100])scrbuf) + sloti * 0x900, 0, 0x900);
			menu0.enabled[sloti - 1] = load ? 0 : 1;
		} else {
			filedate(&slotyear, &slotmon, &slotday, &slothour, &slotmin, &slotsec);
			findclose();
			if (readhdr(workbuf, ((int (*)[2])(scrbuf + 0x7170))[sloti], ((int (*)[2])(scrbuf + 0x7558))[sloti], (char *)((char (*)[100])scrbuf) + sloti * 0x900, ((int (*)[2])(scrbuf + 0x6d88))[sloti], sloti)
			&& ((Savehdr *)workbuf)->place + 0x62a >= 0x62a && ((Savehdr *)workbuf)->place + 0x62a <= 0x640
			&& ((Savehdr *)workbuf)->hour < 24) {
				slotplace = ((Savehdr *)workbuf)->place + 0x62a;
				wsprintf(((char (*)[200])(workbuf + 0x3e8))[sloti], strs[1379], workbuf, strs[slotplace]);
				roman(((Savehdr *)workbuf)->hour >= 5 ? ((Savehdr *)workbuf)->day : ((Savehdr *)workbuf)->day - 1);
				wsprintf(((char (*)[200])(workbuf + 0x3e8))[sloti] + 0x64, strs[546], daytime(((Savehdr *)workbuf)->hour), romanbuf);
				wsprintf(slottext, workbuf);
			} else {
				wsprintf(((char (*)[200])(workbuf + 0x3e8))[sloti], empty);
				wsprintf(((char (*)[200])(workbuf + 0x3e8))[sloti] + 0x64, empty);
				wsprintf(slottext, strs[1146]);
			}
			if (sloti - 1 == 9)
				wsprintf(slottext, strs[1149]);
			// FEATURE: features: the build's own format and month of a saved game
#ifdef FEATURES
			wsprintf(workbuf, feattext(lang == 0 ? (texts + 0xb16e) : (texts + 0xb14c)), sloti + 0x40, slottext, slotday, feattext(months[slotmon]), slotyear, slothour, slotmin, slotsec);
#else
			wsprintf(workbuf, lang == 0 ? (texts + 0xb16e) : (texts + 0xb14c), sloti + 0x40, slottext, slotday, months[slotmon], slotyear, slothour, slotmin, slotsec);
#endif
			menu0.enabled[sloti - 1] = load ? 1 : 2;
			if (load == 0 && sloti - 1 == 9)
				menu0.enabled[sloti - 1] = 0;
		}
		menu0.rects[sloti - 1].x = 0x6a;
		menu0.rects[sloti - 1].y = (sloti - 1) * 30 + 0x83;
		menu0.rects[sloti - 1].w = 0x1ac;
		menu0.rects[sloti - 1].h = 0x18;
		buttonon(menu0.rects[sloti - 1].x, menu0.rects[sloti - 1].y, menu0.rects[sloti - 1].w, menu0.rects[sloti - 1].h, workbuf);
	}
	addcorner(&menu0, sloti - 1, 0);
	menu0.fn = slotinfo;
	puthands();
	backto2();
	pagedone();
}

short
loadsave(int load, int b)
{
	if (!load && units[0].health <= 0)
		return 0;
	slotturn = 1;
	for (;;) {
		slotmenu(load);
		if (!b) {
			nop();
			fadefast(0);
			usepal(0);
		}
		entermenu();
		drawcursor();
		if (b && slotturn)
			turnpage(1, 0, 0x1e0, 0);
		else
			flip();
		fadefast(1);
pick:
		slotsel = runmenu(&menu0, 0, 0, 0);
		if (needredraw(slotsel)) {
			slotturn = 0;
			continue;
		}
		// FIXED: Back is item sloti, 10 in the save menu and in a load menu without Teudogar_J.SAV, which the test takes for slot J: Back saves the game into Teudogar_J.SAV, or loads the AutoSave or the saved game. Cancel on Back.
#if defined(FEATURE_FIXES)
		if (slotsel == 0 || slotsel > 10 || slotsel == sloti) {
#else
		if (slotsel == 0 || slotsel > 10) {
#endif
			if (!b) {
				fadefast(0);
				nop();
			}
			// FIXED: a cancel clears lastslot, which hides keycmd's message of a save but leaves Shift+L without the slot that the manual promises it, the last saved or loaded. Keep it.
#ifndef FEATURE_FIXES
			lastslot = 0;
#endif
			return 0;
		}
		if (load) {
			if (menu0.enabled[slotsel - 1] == 0x11)
				goto pick;
			if (myface && (((int (*)[2])(scrbuf + 0x7170))[slotsel][0] != 4 || !(((int (*)[2])(scrbuf + 0x7558))[slotsel][0] == photosum)))
				exportface();
		}
		break;
	}
	pgtoback();
	text3grey(0, 0xffff, 200, strs[load ? 550 : 551]);
	puthands();
	hidecursor();
	flip();
	// FIXED: a load that fails, as of a Teudogar_J.SAV whose E0000000.DAT is gone, or a save that fails goes on to entergame, which enters the folder's saved game again and loses the progress since, and keycmd says that it loaded or saved. Leave the game as it was.
#if defined(FEATURE_FIXES)
	if (load)
		slotfailed = !loadslot(slotsel, 0);
	else
		saveslot(slotsel, savedesc());
	if (slotfailed) {
		pagefx = 1;
		fadefast(0);
		if (!b)
			nop();
		drawcursor();
		return 0;
	}
#else
	if (load)
		loadslot(slotsel, 0);
	else
		saveslot(slotsel, savedesc());
#endif
	pagefx = 1;
	fadefast(0);
	if (!b)
		nop();
	drawcursor();
	enteritems = 0xff;
	entersum = 1;
	entergame(0);
	return 1;
}

short
loadmenu(int b)
{
	return loadsave(1, b);
}

short
savemenu(int b)
{
	return loadsave(0, b);
}

void
readtext(char *buf, unsigned short n)
{
	buf[0] = 0;
	if (!seekentry(n))
		return;
	readlen = storedn;
	fileread(buf, readlen, 1, datfh);
	for (readp = buf; readp < buf + readlen; readp++)
		*readp ^= 0xfe;
}

void
aboutgame(void)
{
	music(1, 1);
	entermenu();
	nop();
	fadefast(0);
	loadscr(0x2086, (unsigned char *)((char (*)[100])scrbuf), (unsigned char *)gamepal, workbuf, 0x96640);
	keepui(gamepal);
	picink();
	showscr();
	flip();
	hidecursor();
	fadefast(1);
	meminfo(textbuf);
	workbuf[0] = 0;
	catstr(workbuf, titlehead());
	catstr(workbuf, texts + 0xb1f9);
	catstr(workbuf, titletail() + 1);
	catstr(workbuf, texts + 0xb220);
	catstr(workbuf, pick(texts + 0x4d6, texts + 0x4e2, texts + 0x4ef, texts + 0x4f0, texts + 0x4f1, texts + 0x503));
	catstr(workbuf, texts + 0xb1f9);
	catstr(workbuf, vertext);
	catstr(workbuf, texts + 0xb2ab);
	catstr(workbuf, texts + 0x6b02);
	catstr(workbuf, texts + 0xb2a8);
	wsprintf(namebuf, strs[1446], licenseno);
	catstr(workbuf, namebuf);
	unscrambleall();
	if (udstr0[0]) {
		// FIXED: the Italian format asks for five strings, where the others ask for the four that the call passes, so wsprintf reads a fifth past them. Pass an empty fifth, which the others leave.
#ifdef FEATURE_FAULTS
		wsprintf(namebuf, pick(texts + 0x7dbc, texts + 0x7de6, texts + 0x7e0f, texts + 0x7e10, texts + 0x7e11, texts + 0x7e3a), udstr0, udstr1, udstr2, udstr3, "");
#else
		wsprintf(namebuf, pick(texts + 0x7dbc, texts + 0x7de6, texts + 0x7e0f, texts + 0x7e10, texts + 0x7e11, texts + 0x7e3a), udstr0, udstr1, udstr2, udstr3);
#endif
		catstr(workbuf, namebuf);
		catstr(workbuf, texts + 0xb2a8);
	}
	unscrambleall();
	catstr(workbuf, texts + 0xb2a8);
	packbuf[0] = 0;
	catstr(packbuf, strs[1839]);
	wrap(packbuf, 0x1ac);
	catstr(workbuf, packbuf);
	pagex = 0x6a;
	pagemaxh = 0x1df;
	overpic = 1;
	nofade = 1;
	textwin(workbuf, 0, 0xff, 0, 0, &menu1, 0, 0);
	pagex = 0x6a;
	loop = 0;
	pagemaxh = 0x190;
	overpic = 0;
	nofade = 0;
	if (esckey == 0xff)
		;
	else
		esckey = 0;
	fademed(0);
	nop();
	waitrelease(0);
}

int
savetxt(char *s)
{
	HANDLE h;
	int n;

	wsprintf(textbuf, s);
	xorstr(textbuf, textbuf);
	for (n = 0; textbuf[n]; n++)
		;
	if (!fopenw(&h, texts + 0x9bbf))
		return 0;
	filewrite(textbuf, n, 1, h);
	fileclose(&h);
	return 1;
}

short
aboutfull(int a, int b)
{
	ordering = 1;
	workbuf[0] = 0;
	wsprintf(printbuf, strs[1821]);
	catstr(workbuf, printbuf);
	wsprintf(printbuf, strs[1822], ((char *)&game));
	catstr(workbuf, printbuf);
	wsprintf(printbuf, strs[1823], ((char *)&game));
	catstr(workbuf, printbuf);
	wsprintf(printbuf, strs[1824], a == 0 ? strs[1835] : pick(texts + 0x6c8c, texts + 0x6c95, texts + 0x6ca0, texts + 0x6ca1, texts + 0x6ca2, texts + 0x6cad));
	catstr(workbuf, printbuf);
	wrap(workbuf, 0x1ac);
	textsong = 4;
	if (esckey == 0xff)
		esckey = 0xfe;
	quitting = 0;
	fullres = textwin(workbuf, 0, 0xfe, 0, 0, &menu1, 0, b ? 0xff : 0);
	textsong = 0;
	if (fullres == 0x28a1) {
		ordercmd = 1;
set:
		quitnow();
		fadefast(0);
		ordering = 1;
	} else if (fullres == 0x28a2) {
		ordercmd = 2;
		goto set;
	} else
		ordering = 0;
	if (!b)
		nop();
	aboutticks = ticks();
	return ordering;
}

void
aftersave(void)
{
}

// PORT: the type that runmenu calls it through, where a call through another is undefined in C
#ifdef PORT
short
revivekey(unsigned char m, unsigned char c)
#else
int
revivekey(void *m, char c)
#endif
{
	int i;
	unsigned long u;

	if (c != 'r' || !(keys['W'] & 0x80) || units[0].health > 0)
		return 0;
	if (playerx && playery)
		for (i = 0; i < 3; i++) {
			u = popitem(playerx + playery * 150, 0);
			if (u && classof(u & 0xfff) != 0x15 && itemweight(u) < 0xfde8)
				giveitem(0, u, 0);
		}
	restfor(0xff);
	units[0].x = playerx;
	units[0].y = playery;
	units[0].corpse = 0;
	clearmsgs();
	return 3;
}

void
mainmenu(void)
{
	listtorches();
	markunits();
	closeborder();
	nop();
	fadefast(0);
	usepal(0);
	pagefx = 1;
	for (;;) {
		if (esckey == 0xff) {
			mainexit = 0;
ask:
			drawcursor();
			esckey = 0;
			if (!mayquit(1))
				goto three;
			if (!mainexit || !quitprompt || confirm(strs[1152]))
				goto leave;
			pagefx = 3;
			continue;
		}
		mainitems[0] = 0;
		mainitems[1] = empty;
		mainitems[2] = strs[548];
		mainitems[3] = strs[549];
		mainitems[4] = strs[550];
		mainitems[5] = strs[551];
		mainitems[6] = strs[552];
		mainitems[7] = strs[553];
		mainitems[8] = strs[554];
		mainsel = 9;
		cleartexts();
		menupage();
		menutitle();
		putwint(0x6a, 0x1c3, 0xdd);
		if (units[0].health <= 0) {
			menu1.enabled[4] = 0xff;
			menu1.enabled[7] = 0xff;
		}
		listmenu(&menu1, mainsel, mainitems);
		pagefx = 2;
		menu1.enabled[41] = 0x17;
		menu1.enabled[42] = 0x31;
		menu1.enabled[43] = 0x26;
		menu1.enabled[44] = 0x1f;
		menu1.enabled[45] = 0x18;
		menu1.enabled[46] = 0x12;
		menu1.enabled[47] = 0;
		mainsel = 8;
		drawcursor();
		waitforup = 1;
		pollinput();
		while (mouseleft || mouseright || keycode)
			readinput();
		waitforup = 1;
		leftclick = 0;
		pollinput();
		while (mouseleft || mouseright || keycode)
			readinput();
		waitforup = 0;
		leftclick = 0;
		menu1.enabled[0] = 0;
		mainsel = runmenu(&menu1, 0, (short (*)(unsigned char, unsigned char))revivekey, 0);
		if (needredraw(mainsel)) {
			pagefx = 0;
			continue;
		}
		if (esckey == 0xff) {
			mainexit = 0;
			goto ask;
		}
		if (mainsel)
			mainsel--;
		switch (mainsel) {
		case 0:
		case 7:
			if (!(units[0].health <= 0))
				goto def;
		case 0xff:
			pagefx = 0;
			break;
		case 1:
			fadeslow(0);
			hidecursor();
			starttimer3();
			intro(0x65);
			waitinput(500);
			pagefx = 0x20;
			break;
		case 2:
			if (units[0].health > 0 && (!cheat || (keys['W'] & 0x80) == 0) && !confirm(strs[563])) {
				pagefx = 3;
				break;
			}
			fadefast(0);
			pagefx = 1;
			begingame(0, 0);
			if (quitting || units[0].health > 0)
				goto out;
			break;
done:
			leavemenu(1);
			goto out;
		case 3:
			if (loadmenu(1))
				goto done;
			pagefx = 3;
			break;
		case 4:
			if (!savemenu(1)) {
				pagefx = 3;
				break;
			}
			pagefx = 2;
			/* storing 2 again through another type keeps the dead
			   store above and gives the original's registers */
			// PORT: the store through a pointer, in place of C++'s reference
#ifdef PORT
			*(unsigned short *)&pagefx = 2;
#else
			(unsigned short &)pagefx = 2;
#endif
			pagefx = 3;
			leavemenu(1);
			goto check;
		case 5:
			options(1);
three:
			pagefx = 3;
			break;
		case 6:
			mainexit = 1;
			goto ask;
leave:
			quit();
			fadefast(0);
			nop();
			waitrelease(0);
out:
			pagefx = 1;
			return;
		default:
def:
			fadefast(0);
			nop();
check:
			if (quitting == 0 && units[0].health <= 0) {
				pagefx = 0;
				break;
			}
			goto done;
		}
	}
}

void
sheetface(void)
{
	if (sheetunit != &units[0])
		return;
	putwh((unsigned char *)faceback, 2, 0x5c, 0x46, 0x6c, 0x78);
	putfacen(2, 0x5c, 0x46, curblink && units[0].health > 0 && !sheetdead ? heroface2 : heroface);
}

// PORT: the type that textwin calls it through, where a call through another is undefined in C
#ifdef PORT
void
#else
int
#endif
sheethover(short n)
{
	faceshadow(0x5c, 0x46, curblink && units[0].health > 0 && !sheetdead ? heroface2 : heroface);
	getwh(0, 0x5c, 0x46, (unsigned char *)faceback, 0x6c, 0x78);
	sheetgear = 1;
	sheetvital = 1;
	if (n == sheetoff + 1 || n == sheetoff + 2 || n == sheetoff + 3)
		sheetbars = 1;
	else
		sheetbars = 0;
	if (n == sheetoff + 2) {
		if (!combatnow) {
			sheetbars = 0;
			sheetvital = 0;
			goto show;
		}
	} else if (n == sheetoff + 3 && !skillsnow) {
		sheetgear = 0;
		sheetbars = 0;
		sheetvital = 0;
		goto show;
	}
	if (sheetbars) {
		sheetbarw = (1000 - putherostates(0, 0x5e, 1000, 1, 0)) / 20;
		sheetbarw *= 24;
		sheetbarx = 0x92 - sheetbarw / 2;
		if (sheetbarx < 0x5e || sheetbarx > 0x140)
			sheetbarx = 0x5e;
		sheetbarx -= 0x18;
		putherostates(0, sheetbarx + sheetbarw, 0x1ae, 1, 2);
	}
show:
	memcpy(printbuf, sheetunit, 0x1b8);
	if (sheetgear) {
		if (!sheetvital) {
			sheetunit->health = kindof(sheetunit)->maxhealth;
			sheetunit->bleeding = 0;
		}
	} else {
		sheetunit->health = kindof(sheetunit)->maxhealth;
		sheetunit->bleeding = 0;
		sheetunit->head = 0;
		sheetunit->cloak = 0;
		sheetunit->body = 0;
		sheetunit->legs = 0;
		sheetunit->feet = 0;
		sheetunit->hand = 0;
		sheetunit->belt = 0;
		sheetunit->offhand = 0;
		sheetunit->wrist = 0;
		sheetunit->neck = 0;
	}
	drawoutfit(0x14, 0xe6, sheetunit, 0, 1);
	memcpy(sheetunit, printbuf, 0x1b8);
	puthands();
	// PORT: void, as above
#ifndef PORT
	return 0;
#endif
}

void
fametext(char *buf)
{
	famep = buf + 5000;
	for (famei = 0; famei < 13; famei++) {
		if (((Fame *)((char *)hiscores))[famei].name[0])
			wsprintf(famep, texts + 0xb2d2, playerrank == famei ? 0x1f : 0x20, ((Fame *)((char *)hiscores))[famei].name, ((Fame *)((char *)hiscores))[famei].score);
		while (*famep)
			famep++;
	}
	wsprintf(buf, strs[1369], titlehead(), titletail(), buf + 5000);
}

void
colwrap(char *s)
{
	wrap(s, 0x136);
	colp = s;
	colp = insstr(colp, texts + 0xb2ed);
	do {
		if (*colp == 13) {
			if (colp[-1] != 13)
				colp = insstr(colp, texts + 0xb2eb);
			if (colp[1] != 13) {
				colp++;
				colp = insstr(colp, texts + 0xb2ed);
			}
		}
	} while (*colp++);
}

void
charsheet(Unit *u, short mode)
{
	if (badunit(u))
		return;
	sheetdead = mode;
	if (skillsnow > 1)
		skillsnow = 1;
	if (combatnow > 1)
		combatnow = 1;
	oldskills = skillsnow;
	oldcombat = combatnow;
	calcstats(&units[0]);
	snapskills(0);
	cleartexts();
	nop();
	fadefast(0);
	usepal(0);
	for (;;) {
		if (skillsnow > 1)
			skillsnow = 1;
		if (combatnow > 1)
			combatnow = 1;
		sheetoff = 0;
		memset(packbuf, 0, 0x96640);
		resetanims();
		if (mode) {
			restfor(0xff);
			if (isunlittorch(units[0].offhand)) {
				units[0].offhand |= 0x3ff0000;
				setlit(&units[0].offhand, 1);
			}
		}
		sheetunit = u;
		entermenu();
		inkwhite();
		calcstats(u);
		if (u == &units[0] && !mode) {
			for (curdodge = 0; curdodge < 30; curdodge++) {
				skilldelta = game.skills[curdodge] - game.snappoints[curdodge];
				skillmarks[curdodge][0] = 0;
				skillmarks[curdodge][1] = 0;
				if (skilldelta >= 70)
					skillmarks[curdodge][0] = 9;
				else if (skilldelta >= 35)
					skillmarks[curdodge][0] = 8;
				else if (skilldelta >= 17)
					skillmarks[curdodge][0] = 7;
				else if (skilldelta <= -70)
					skillmarks[curdodge][0] = 12;
				else if (skilldelta <= -35)
					skillmarks[curdodge][0] = 11;
				else if (skilldelta <= -17)
					skillmarks[curdodge][0] = 10;
			}
		} else {
			curdodge = 0;
			while (curdodge < 30) {
				skillmarks[curdodge][0] = 0;
				curdodge++;
			}
		}
		wsprintf(sheetname, unitname(u));
		sheeti = 0;
		while (sheetname[sheeti]) {
			sheetname[sheeti] = upper(sheetname[sheeti]);
			sheeti++;
		}
		statustext(sheetstatus, u);
		sheetstatus[0] = upper(sheetstatus[0]);
		wrap(sheetstatus, 0x136);
		sheetp = sheetstatus;
		while (*sheetp) {
			if (*sheetp == 13) {
				sheetp++;
				sheetp = insstr(sheetp, texts + 0xb2ed);
			} else
				sheetp++;
		}
		if (u != &units[0])
			wsprintf(sheettired, texts + 0xb072, strs[509], strs[518]);
		else
			wsprintf(sheettired, texts + 0xb072, tiredtext(game.tiredlevel), hungertext(game.hungerlevel));
		sheettired[0] = upper(sheettired[0]);
		immobtext((char *)((char (*)[100])scrbuf), u);
		immobcause(sheetcause);
		if (((char (*)[100])scrbuf)[0][0] == 0)
			sheetimmob[0] = 0;
		else {
			sheetp = sheetimmob;
			wsprintf(sheetimmob, texts + 0xb1fb);
			while (*sheetp)
				sheetp++;
			wsprintf(sheetp, texts + 0x9dea, ((char (*)[100])scrbuf), sheetcause);
			*sheetp = upper(*sheetp);
		}
		memcpy(tmpbuf, &units[0], 0x1b8);
		sheethunger = game.hungercount;
		sheetweary = game.tiredcount;
		units[0].health = kindof(&units[0])->maxhealth;
		units[0].vigor = kindof(&units[0])->maxvigor;
		clearstatus(&units[0]);
		units[0].blessed = 0;
		units[0].warded = 0;
		units[0].berserk = 0;
		game.hungercount = 0;
		game.tiredcount = 0;
		setlevels();
		calcstats(&units[0]);
		sheetmode = u->mode;
		u->mode = 0x14;
		attackval(u, &baseatk, &sheetkind, &basedmin, &basedmax, 0);
		defenseval(u, &baseparry, &baseblock, &basedodge);
		u->mode = sheetmode;
		memcpy(&units[0], tmpbuf, 0x1b8);
		game.hungercount = sheethunger;
		game.tiredcount = sheetweary;
		setlevels();
		calcstats(&units[0]);
		sheetmode = u->mode;
		u->mode = 0x14;
		attackval(u, &curatk, &sheetkind, &curdmin, &curdmax, 0);
		defenseval(u, &curparry, &curblock, &curdodge);
		u->mode = sheetmode;
		if (curatk == baseatk && curdmin == basedmin && curdmax == basedmax
		&& curparry == baseparry && curblock == baseblock && curdodge == basedodge
		&& (!cheat || !(keys['W'] & 0x80)))
			combatnow = 2;
		else if (!combatnow) {
			curatk = baseatk;
			curdmin = basedmin;
			curdmax = basedmax;
			curparry = baseparry;
			curblock = baseblock;
			curdodge = basedodge;
		}
		if (u->hand)
			wsprintf(sheetweapon, strs[1296], itemname(u->hand));
		else
			wsprintf(sheetweapon, strs[1294]);
		dmgrange(sheetdmg, curdmin, curdmax);
		if (u->head)
			wsprintf(sheethelm, itemname(u->head));
		else
			wsprintf(sheethelm, strs[1298]);
		if (u->body)
			wsprintf(sheetarmor, itemname(u->body));
		else
			wsprintf(sheetarmor, strs[1299]);
		if (u->legs)
			wsprintf(sheetpants, itemname(u->legs));
		else
			wsprintf(sheetpants, strs[1300]);
		if (classof(units[0].offhand & 0xfff) == 0x55) {
			wsprintf(sheetshield, strs[1301], itemname(u->offhand));
			hasshield = 1;
		} else {
			wsprintf(sheetshield, strs[1302]);
			hasshield = 0;
		}
		wsprintf(packbuf, strs[1321], sheetname, vitaltext(mode ? 0 : u->health),
			ratetext(u->strength), ratetext(u->dexterity), ratetext(u->intellect),
			sheetstatus, sheettired, sheetimmob, carrytext(u, 1));
		wsprintf(packbuf + 10000, strs[1322],
			combatnow > 1 ? empty : combatnow ? strs[1324] : strs[1325],
			sheetname, sheetweapon, ratetext(curatk), sheetdmg,
			hasshield ? sheetshield : strs[1302],
			hasshield ? ratetext(curblock) : empty,
			ratetext(curdodge),
			u->hand ? sheetweapon : strs[1295],
			u->hand || curparry ? ratetext(curparry) : empty,
			sheethelm, u->head ? armortext(protection(u->head)) : empty,
			sheetarmor, u->body ? armortext(protection(u->body)) : empty,
			sheetpants, u->legs ? armortext(protection(u->legs)) : empty);
		if (u->strength == kindof(u)->strength && u->dexterity == kindof(u)->dexterity && u->intellect == kindof(u)->intellect
		&& (!cheat || !(keys['W'] & 0x80)))
			skillsnow = 2;
		wsprintf(packbuf + 20000, strs[1323],
			skillsnow > 1 ? empty : skillsnow ? strs[1324] : strs[1325],
			sheetname,
			strs[1264], ratetext(skillsnow ? skillof(u, 0x14, 0, 0, 0) : baseskill(u, 0x14)), skillmarks[20],
			strs[1265], ratetext(skillsnow ? skillof(u, 0x15, 0, 0, 0) : baseskill(u, 0x15)), skillmarks[21],
			strs[1266], ratetext(skillsnow ? skillof(u, 0x16, 0, 0, 0) : baseskill(u, 0x16)), skillmarks[22],
			strs[1267], ratetext(skillsnow ? skillof(u, 0x17, 0, 0, 0) : baseskill(u, 0x17)), skillmarks[23],
			strs[1268], ratetext(skillsnow ? skillof(u, 0x18, 0, 0, 0) : baseskill(u, 0x18)), skillmarks[24],
			strs[1259], ratetext(skillsnow ? skillof(u, 0xf, 0, 0, 0) : baseskill(u, 0xf)), skillmarks[15],
			strs[1260], ratetext(skillsnow ? skillof(u, 0x10, 0, 0, 0) : baseskill(u, 0x10)), skillmarks[16],
			strs[1261], ratetext(skillsnow ? skillof(u, 0x11, 0, 0, 0) : baseskill(u, 0x11)), skillmarks[17],
			strs[1255], ratetext(skillsnow ? skillof(u, 0xb, 0, 0, 0) : baseskill(u, 0xb)), skillmarks[11],
			strs[1257], ratetext(skillsnow ? skillof(u, 0xd, 0, 0, 0) : baseskill(u, 0xd)), skillmarks[13],
			strs[1258], ratetext(skillsnow ? skillof(u, 0xe, 0, 0, 0) : baseskill(u, 0xe)), skillmarks[14],
			strs[1256], ratetext(skillsnow ? skillof(u, 0xc, 0, 0, 0) : baseskill(u, 0xc)), skillmarks[12],
			strs[1269], ratetext(skillsnow ? skillof(u, 0x19, 0, 0, 0) : baseskill(u, 0x19)), skillmarks[25],
			strs[1270], ratetext(skillsnow ? skillof(u, 0x1a, 0, 0, 0) : baseskill(u, 0x1a)), skillmarks[26]);
		scrbuf[0x88b8] = 0;
		ndeeds = 0;
		deedmark = 0;
		if (game.quests || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf, strs[1327], timestext(game.quests));
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		if (deedmark != ndeeds) {
			catstr(scrbuf + 0x88b8, texts + 0xb220);
			deedmark = ndeeds;
		}
		if (game.defeated || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf, strs[1328], timestext(game.defeated));
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		if (game.killed > game.runkills || game.runkills || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf, strs[1329], timestext(game.killed > game.runkills ? game.killed - game.runkills : 0));
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		if (game.runkills || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf, strs[1330], timestext(game.runkills));
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		sheetv = game.booty;
		sheetv = sheetv / 10 + (sheetv % 10 >= 5 ? 1 : 0);
		if (sheetv || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf + 1000, strs[1156], sheetv);
			wsprintf(printbuf, strs[1331], printbuf + 1000);
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		if (deedmark != ndeeds) {
			catstr(scrbuf + 0x88b8, texts + 0xb220);
			deedmark = ndeeds;
		}
		if (game.stolen || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf, strs[1337], timestext(game.stolen));
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		if (game.mercies || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf, strs[1332], timestext(game.mercies));
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		if (game.broken || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf, strs[1338], timestext(game.broken));
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		if (deedmark != ndeeds) {
			catstr(scrbuf + 0x88b8, texts + 0xb220);
			deedmark = ndeeds;
		}
		if (game.ambushed || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf, strs[1333], timestext(game.ambushed));
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		if (game.womenhit || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf, strs[1334], timestext(game.womenhit));
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		if (game.sleepers || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf, strs[1335], timestext(game.sleepers));
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		if (game.poisoned || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf, strs[1336], timestext(game.poisoned));
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		if (deedmark != ndeeds) {
			catstr(scrbuf + 0x88b8, texts + 0xb220);
			deedmark = ndeeds;
		}
		if (game.invoked || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf, strs[1340], timestext(game.invoked));
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		sheetv = game.sacrificed;
		sheetv = sheetv / 10 + (sheetv % 10 >= 5 ? 1 : 0);
		if (sheetv || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf + 1000, strs[1156], sheetv);
			wsprintf(printbuf, strs[1339], printbuf + 1000);
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		sheetv = game.sunk;
		sheetv = sheetv / 10 + (sheetv % 10 >= 5 ? 1 : 0);
		if (sheetv || cheat && keys['W'] & 0x80 && isdebug()) {
			wsprintf(printbuf + 1000, strs[1156], sheetv);
			wsprintf(printbuf, strs[1341], printbuf + 1000);
			catstr(scrbuf + 0x88b8, printbuf);
			ndeeds++;
		}
		if (scrbuf[0x88b8] == 0)
			packbuf[30000] = 0;
		else {
			wsprintf(packbuf + 30000, strs[1326], sheetname);
			catstr(packbuf + 30000, scrbuf + 0x88b8);
		}
		epitaph(sheetepitaph);
		if (game.fate == 0xf)
			wsprintf(sheetverdict, strs[1366]);
		else if (game.fate == 0x12 || game.fate == 0x13)
			wsprintf(sheetverdict, strs[1362]);
		else if (game.fate == 0x11)
			wsprintf(sheetverdict, strs[1363]);
		else if (game.fate == 0xd)
			wsprintf(sheetverdict, strs[1367]);
		else if (getbit(game.flags, 0x1e9))
			wsprintf(sheetverdict, strs[1360]);
		else if (getbit(game.flags, 0x1d6))
			wsprintf(sheetverdict, strs[1361]);
		else if (game.poisoned)
			wsprintf(sheetverdict, strs[1353]);
		else if (game.sleepers)
			wsprintf(sheetverdict, strs[1352]);
		else if (game.fate == 0x1f)
			wsprintf(sheetverdict, strs[1365]);
		else if (game.fate == 0x1e)
			wsprintf(sheetverdict, strs[1364]);
		else if (game.womenhit)
			wsprintf(sheetverdict, strs[1354]);
		else if (game.stolen)
			wsprintf(sheetverdict, strs[1355]);
		else if (game.ambushed)
			wsprintf(sheetverdict, strs[1356]);
		else if (game.fate == 0xc || game.fate == 0xb)
			wsprintf(sheetverdict, strs[1368]);
		else if (game.broken)
			wsprintf(sheetverdict, strs[1357]);
		else if (game.runkills)
			wsprintf(sheetverdict, strs[1358]);
		else
			wsprintf(sheetverdict, strs[1359]);
		sheetv = tribefate();
		if (game.fate == 0x1e)
			wsprintf(sheetfate, strs[1348]);
		else if (game.fate == 0x1f)
			wsprintf(sheetfate, strs[1349]);
		else if (sheetv == 2)
			wsprintf(sheetfate, strs[1347]);
		else if (sheetv == 1)
			wsprintf(sheetfate, strs[1346]);
		else if (game.mercies)
			wsprintf(sheetfate, strs[1345]);
		else if (game.defeated > 3 || game.killed > 3)
			wsprintf(sheetfate, strs[1344]);
		else if (game.fate == 0x12 || game.fate == 0x13)
			wsprintf(sheetfate, strs[1350]);
		else if (game.fate == 0xd)
			wsprintf(sheetfate, strs[1351]);
		else if (game.fate == 0xf)
			wsprintf(sheetfate, strs[1359]);
		else
			wsprintf(sheetfate, strs[1343]);
		colwrap(sheetepitaph);
		colwrap(sheetverdict);
		colwrap(sheetfate);
		wsprintf(packbuf + 40000, strs[1342], sheetname, sheetepitaph, sheetverdict, sheetfate);
		wrap(packbuf + 40000, 0x1ac);
		addhiscore();
		if (((Fame *)((char *)hiscores))[0].name[0])
			fametext(packbuf + 50000);
		else
			packbuf[50000] = 0;
		memset(((char (*)[100])scrbuf), 0, 0x4b9c4);
		if (mode) {
			catstr((char *)((char (*)[100])scrbuf), packbuf + 40000);
			sheetoff = 1;
		}
		catstr((char *)((char (*)[100])scrbuf), packbuf);
		catstr((char *)((char (*)[100])scrbuf), packbuf + 10000);
		catstr((char *)((char (*)[100])scrbuf), packbuf + 20000);
		catstr((char *)((char (*)[100])scrbuf), packbuf + 30000);
		catstr((char *)((char (*)[100])scrbuf), packbuf + 50000);
		nofade = 1;
		switch (cur) {
		case 2:
		case 0x11:
			music(cur, 1);
			break;
		case 0:
			if (mode)
				music(2, 1);
			else
				music(12, 1);
		default:
			music(12, 1);
			break;
		}
		menudraw = sheetface;
		menuanim = 1;
		// FEATURE: french: elide the articles before the names that the sheet inserts
#ifdef FEATURE_FRENCH
		if (lang == 2)
			frelide((char *)((char (*)[100])scrbuf));
#endif
		sheetv = textwin((char *)((char (*)[100])scrbuf), 0, 0, 0xff, (void (*)(short))sheethover, &menu1, 0, mode ? 1 : pagefx < 2 ? 0 : 0xff);
		menuanim = 0;
		menudraw = 0;
		nofade = 0;
		loop = 0;
		if (sheetv == sheetoff * 100 + 10217) {
			if (combatnow)
				combatnow = 0;
			else
				combatnow = 1;
			startpage = sheetoff + 1;
			continue;
		}
		if (sheetv == sheetoff * 100 + 10317) {
			if (skillsnow)
				skillsnow = 0;
			else
				skillsnow = 1;
			startpage = sheetoff + 2;
			continue;
		}
		fadefast(0);
		inkshade();
		if (mode)
			units[0].health = 0;
		skillsnow = oldskills;
		combatnow = oldcombat;
		if (esckey == 0xff) {
			pagefx = 2;
			if (!mayquit(1)) {
				pagefx = 3;
				continue;
			}
			quit();
			return;
		}
		nop();
		if (mode)
			waitrelease(0);
		else
			leavemenu(1);
		pagefx = 1;
		return;
	}
}

void
blackout(short n)
{
	if (n) {
		killunit(&units[0], 1, 0, 0xff, 0);
		fatetext((char *)((char (*)[100])scrbuf));
	} else {
		msgf(0x63, strs[1377]);
		setunitmode(&units[0], 0x5a);
		playmode = 1;
		units[0].modea = 0xff;
	}
	hidecursor();
	lostcontrol = 1;
	fillgauges();
	closepanels();
	if (n != 2)
		drawscreen();
	clearmind(&units[0]);
	lostcontrol = 0;
	hidecursor();
	if (!palblack) {
		waitforup = 1;
		do
			pollinput();
		while (mouseleft || mouseright || keycode);
		do
			pollinput();
		while (!mouseleft && !mouseright && !keycode && !esckey);
		waitforup = 0;
		leftclick = 0;
		clrback();
		melt();
		palblk();
	}
}

void
awaken(void)
{
	drawcursor();
	usepal(0);
	waitinput(1000);
	waitrelease(0);
}

int
outspan(void)
{
	outhours = game.tiredcount / 20;
	return hourticks * outhours;
}

void
outcold(void)
{
	if (units[0].bleeding + units[0].poison >= units[0].health) {
		starttimer3();
		while (elapsed3() < 500)
			readinput();
dead:
		game.fate = 5;
		game.killer = 0;
		killunit(&units[0], 1, 3, 0xff, 0);
		return;
	}
	knockedout = 1;
	passtime(outspan(), 1, 1);
	setunitmode(&units[0], 0);
	knockedout = 0;
	if (units[0].health <= 0) {
		if (game.fate == 0)
			goto dead;
		game.fate = 4;
		killunit(&units[0], 1, 2, 0xff, 0);
		return;
	}
	if (units[0].dexterity < 5 || units[0].strength < 5)
		goto dead;
}

void
endgame(short why)
{
	game.fate = why;
	switch (game.fate) {
	case 0:
	case 5:
	case 7:
		addscore(0);
		break;
	case 1:
	case 6:
		addscore(1000);
		break;
	case 2:
		addscore(2000);
		break;
	case 4:
		addscore(1500);
		break;
	case 12:
	case 13:
		addscore(2500);
		break;
	case 10:
	case 11:
		addscore(5000);
		break;
	case 19:
		addscore(15000);
		break;
	case 17:
		addscore(10000);
		break;
	case 15:
	case 26:
		addscore(50000);
		break;
	case 20:
		addscore(20000);
		break;
	case 21:
		addscore(25000);
		break;
	case 18:
		addscore(30000);
		break;
	case 25:
		addscore(40000);
		break;
	case 30:
		addscore(60000);
		break;
	case 31:
		addscore(70000);
		break;
	}
	if (getbit(game.flags, 0x281))
		addscore(5000);
	usepal(0);
	hidecursor();
	entermenu();
	do {
		loadscr(0x2088, (unsigned char *)((char (*)[100])scrbuf), 0, picbuf, 0x4b3e8);
		showscr();
		blitn(7, 0x80, 0x24, 0, 0x156, 0x22, 0x11a, 0x19c);
		fatetext(workbuf);
		loadscr(fatepic, (unsigned char *)((char (*)[100])scrbuf), 0, picbuf, 0x4b3e8);
		showscrt();
		fatetext(workbuf);
		wrap(workbuf, 0x110);
		fatep = workbuf;
		drawpage(&fatep, 0x158, 0xfe, 0, 0x1b8, 0);
		putcs(0x250, 0x1b0, 0x3d);
		if (game.fate == 0x13 || game.fate == 0x12)
			fatesong = 12;
		else if (game.fate == 0x15 || game.fate == 0xb)
			fatesong = 5;
		else if (game.fate == 0xc || game.fate == 0xd)
			fatesong = 0x18;
		else if (fatepic == 0xfb2 || fatepic == 0x208a)
			fatesong = 2;
		else if (fatepic == 0xfb4 || fatepic == 0x208e)
			fatesong = 0x12;
		else if (fatepic == 0x2092 || fatepic == 0x2090 || fatepic == 0x2093)
			fatesong = 0x13;
		else
			fatesong = 0x11;
		flip();
		drawcursor();
		fadeslow(1);
		fatetext((char *)((char (*)[100])scrbuf));
		if (fatesong)
			music(fatesong, 1);
		waitinput(-1);
	} while (waitlost);
	loop = 0;
	waitrelease(0);
	fadeslow(0);
}

int
prevhit(void)
{
	if (keycode == 0x4b)
		return 1;
	if ((menu1.sel == 0 || menu1.sel > menu1.n) && mouseleft && mousex < 320)
		return 1;
	return 0;
}

int
nexthit(void)
{
	if (keycode == 0x4d)
		return 1;
	if ((menu1.sel == 0 || menu1.sel > menu1.n) && mouseleft && mousex >= 320)
		return 1;
	return 0;
}

int
turnhit(void)
{
	if ((jourturn & 1) && nexthit())
		return 1;
	if ((jourturn & 2) && prevhit())
		return 1;
	return 0;
}

char *
notetext(char *dst, short i)
{
	expandstr(namebuf, strs[1741 + ((Note *)notes)[i].id]);
	for (notep = namebuf;; notep++) {
		*dst = *notep;
		if (*notep == 0)
			break;
		dst++;
		if (*notep == 13)
			*dst++ = 10;
	}
	*dst++ = 13;
	*dst++ = 10;
	return dst;
}

void
notelist(unsigned char marks)
{
	for (noteq = workbuf; *noteq; noteq++)
		;
	if (marks) {
		for (notei = nnotes - 1; notei >= 0; notei--)
			if (marks == ((Note *)notes)[notei].marks)
				noteq = notetext(noteq, notei);
	} else {
		for (notei = 0; notei < nnotes; notei++)
			if (marks == ((Note *)notes)[notei].marks)
				noteq = notetext(noteq, notei);
	}
	*noteq++ = 0;
}

short
hasnote(short id)
{
	for (hasi = 0; hasi < nnotes; hasi++)
		if (id == ((Note *)notes)[hasi].id)
			return 1;
	return 0;
}

short
noteclosed(short id)
{
	short i;

	for (i = 0; i < nnotes; i++)
		if (id == ((Note *)notes)[i].id && ((Note *)notes)[i].marks == 0)
			return 1;
	return 0;
}

short
noteopen(short id)
{
	if (id < 0 || id >= 500)
		return 0;
	if (!hasnote(id))
		return 0;
	if (noteclosed(id))
		return 0;
	return 1;
}

void
delnote(short i)
{
	nnotes--;
	for (; i < nnotes; i++) {
		((Note *)notes)[i].id = ((Note *)notes)[i + 1].id;
		((Note *)notes)[i].marks = ((Note *)notes)[i + 1].marks;
	}
}

void
dropnote(short id)
{
	if (nnotes > 0 && !noteclosed(id))
		for (dropi = 0; dropi < nnotes; dropi++)
			if (id == ((Note *)notes)[dropi].id) {
				delnote(dropi);
				return;
			}
}

int
notemarks(short id)
{
	bangp = strs[1741 + id];
	nbangs = 0;
	if (*bangp++ == '!')
		nbangs++;
	if (*bangp++ == '!')
		nbangs++;
	if (*bangp++ == '!')
		nbangs++;
	return nbangs;
}

short
addnote(short id)
{
	if (id < 0 || id >= 500)
		return 0;
	if (hasnote(id))
		return 0;
	if (noteclosed(id))
		return 0;
	((Note *)notes)[nnotes].id = id;
	id = notemarks(id);
	if (id >= 2)
		learnquest();
	((Note *)notes)[nnotes].marks = id;
	nnotes++;
	return 1;
}

void
closenote(short id, short sound)
{
	// FIXED: a note closed before pays its score and training again, as a script's qgel may close it twice. Pay a note once.
#if defined(FEATURE_FIXES)
	if (noteclosed(id))
		return;
#endif
	if (sound && !cur && !talkactive)
		music(0x16, 0);
	scorenote(id);
	deedquest(notemarks(id));
	closetry = 0;
	do {
		for (closei = 0; closei < nnotes; closei++)
			if (id == ((Note *)notes)[closei].id) {
				((Note *)notes)[closei].marks = 0;
				if (closei < nnotes - 1) {
					memcpy(&((Note *)notes)[nnotes], &((Note *)notes)[closei], 4);
					for (closej = closei; closej < nnotes; closej++)
						memcpy(&((Note *)notes)[closej], &((Note *)notes)[closej + 1], 4);
				}
				return;
			}
		addnote(id);
		closetry++;
	} while (closetry < 2);
}

void
journal(void)
{
	if (nnotes == 0) {
		denymsg(strs[851]);
		return;
	}
	if (minmarks < 1)
		minmarks = 1;
	else if (minmarks > 3)
		minmarks = 3;
	questsel = -1;
	cleartexts();
	nop();
	music(0x20, 0);
	if (!palblack)
		fadefast(0);
	usepal(0);
	jourtop = 0;
top:
	entermenu();
	drawcursor();
	menu1.sel = 0;
	menu1.keysel = 0;
again:
	njour = 0;
	for (jourj = 3; jourj >= minmarks; jourj--)
		for (jouri = nnotes - 1; jouri >= 0; jouri--)
			if (jourj == ((Note *)notes)[jouri].marks)
				jourlist[njour++] = jouri;
	clearmenu(&menu1);
	jourbtn = 0;
	jourunused = 0;
	pgtoback();
	pagearrows = 0;
	jourturn = 0;
	if (jourtop > 0) {
		putcs(0x18, 0xd8, 0x38);
		pagearrows |= 1;
		jourturn |= 2;
	}
	jourleft = -1;
	if (jourtop < njour - 6) {
		putcs(0x246, 0xd8, 0x39);
		pagearrows |= 2;
		jourturn |= 1;
	}
	jouri = jourtop / 6;
	jourright = -1;
	jourj = njour / 6 + (njour % 6 ? 1 : 0);
	if (jourj > 1) {
		wsprintf(tmpbuf, strs[846], jouri + 1, jourj);
		text3grey2(0, 0x6a, 0x32, tmpbuf);
		pagenum(jouri, jourj);
	} else
		text3grey2(0, 0x6a, 0x32, strs[845]);
	addbutton(&menu1, jourbtn, 0x216, 0x2f, strs[849], 1);
	if (minmarks == 3)
		text3(0, text3x, text3y, texts + 0xb5cf);
	jourmost = jourbtn++;
	addbutton(&menu1, jourbtn, menu1.rects[jourbtn - 1].x - 7, 0x2f, strs[848], 1);
	if (minmarks == 2)
		text3(0, text3x, text3y, texts + 0xb5cf);
	jourimp = jourbtn;
	jourbtn++;
	addbutton(&menu1, jourbtn, menu1.rects[jourbtn - 1].x - 7, 0x2f, strs[847], 1);
	if (minmarks == 1)
		text3(0, text3x, text3y, texts + 0xb5cf);
	jourall = jourbtn;
	jourbtn++;
	addcorner(&menu1, jourbtn, 0);
	jourback = jourbtn;
	jourbtn++;
	jourend = jourtop + 6;
	if (jourend > njour)
		jourend = njour;
	jourfirst = -1;
	jourj = 0;
	for (jouri = jourtop; jouri < jourend; jouri++, jourj++, jourbtn++) {
		expandstr(namebuf, strs[1741 + ((Note *)notes)[jourlist[jouri]].id]);
		jourp = namebuf;
		jourq = jourmarks;
		while (*jourp == '!' || *jourp == ' ')
			*jourq++ = *jourp++;
		*jourq = 0;
		text3grey(0, 0x89, jourj * 0x38 + 100, jourmarks);
		text3grey(0, 0xa1, jourj * 0x38 + 0x5a, jourp);
		while (*jourp != 13 && *jourp)
			jourp++;
		if (*jourp) {
			jourp++;
			text3grey(0, 0xa1, jourj * 0x38 + 0x6e, jourp);
		}
		menu1.rects[jourbtn].x = 0x6a;
		menu1.rects[jourbtn].w = 0x1ac;
		menu1.rects[jourbtn].y = jourj * 0x38 + 0x57;
		menu1.rects[jourbtn].h = 0x2e;
		pillring(menu1.rects[jourbtn].x, menu1.rects[jourbtn].y, menu1.rects[jourbtn].w, menu1.rects[jourbtn].h);
		if (jourfirst == -1)
			jourfirst = jourbtn;
	}
	if (questsel < 0 || questsel >= jourend - jourtop || questsel >= 6) {
		questsel = -1;
		jourdel = -1;
	} else {
		text3(0, 0x71, questsel * 0x38 + 100, texts + 0xb5d9);
		addbutton(&menu1, jourbtn, 0x6a, 0x1b7, strs[850], 0);
		jourdel = jourbtn;
		jourbtn++;
	}
	menu1.n = jourbtn;
	menu1.color = 0xf4;
	memset(menu1.enabled, 1, menu1.n);
	menu1.keys2[0] = 0x31;
	menu1.keys2[1] = 0x41;
	puthands();
	backto2();
	pagedone();
	readinput();
	mousemoved = 1;
	setcursor1(3, 3);
	if (palblack) {
		flip();
		fadefast(1);
		pagesound();
	} else if (pagefx == 2 || pagefx == 3)
		turnpage(pagefx == 2 ? 1 : 0, 0, 0x1e0, 1);
	else
		flip();
	pagefx = 0;
	waitforup = 1;
	pollinput();
	while (mouseleft || mouseright || keycode)
		readinput();
	waitforup = 0;
	leftclick = 0;
	for (;;) {
		readinput();
		jourbtn = runmenu(&menu1, turnhit, 0, 0);
		if (needredraw(jourbtn))
			goto again;
		pagefx = 0;
		if (esckey)
			goto quit;
		if (jourbtn == 0 || jourbtn == 0xfe) {
			menu1.sel = 0;
			if (prevhit())
				goto left;
			if (nexthit())
				goto right;
			goto quit;
		}
		jourbtn--;
		if (jourbtn == jourmost) {
			jourbtn = 3;
filter:
			if (minmarks != jourbtn) {
				questsel = -1;
				minmarks = jourbtn;
				jourtop = 0;
				goto again;
			}
		} else if (jourbtn == jourimp) {
			jourbtn = 2;
			goto filter;
		} else if (jourbtn == jourall) {
			jourbtn = 1;
			goto filter;
		} else if (jourbtn == jourleft) {
left:
			if (jourtop > 0) {
				jourbtn = jourtop;
				jourtop -= 6;
				if (jourtop < 0)
					jourtop = 0;
				if (jourbtn != jourtop) {
					questsel = -1;
					pagefx = 3;
					goto again;
				}
			}
		} else if (jourbtn == jourright) {
right:
			if (jourtop < njour - 6) {
				questsel = -1;
				jourbtn = jourtop;
				jourtop += 6;
				pagefx = 2;
				goto again;
			}
		} else if (jourbtn == jourdel) {
			if (questsel != -1) {
				jouri = jourlist[questsel + jourtop];
				if (((Note *)notes)[jouri].marks > 1) {
					pgtoback();
					expandstr(namebuf, strs[1741 + ((Note *)notes)[jouri].id]);
					jourj = 4;
					jourp = namebuf;
					text3grey(0, 0xffff, jourj++ * 20, jourp);
					while (*jourp != 13 && *jourp)
						jourp++;
					if (*jourp++)
						text3grey(0, 0xffff, jourj++ * 20, jourp);
					jourj++;
					puthands();
					pagefx = 2;
					if (!confirm(strs[852])) {
						if (esckey == 0xff)
							goto quit;
						pagefx = 3;
						goto top;
					}
					pagefx = 3;
				}
				delnote(jouri);
				njour--;
				questsel = -1;
				if (jourtop >= njour)
					goto left;
				goto top;
			}
		} else if (jourbtn == jourback || jourbtn < jourfirst || jourbtn > jourend - jourtop + jourfirst)
			break;
		else {
			jourbtn -= jourfirst;
			if (questsel == jourbtn)
				questsel = -1;
			else
				questsel = jourbtn;
			goto again;
		}
	}
quit:
	fadefast(0);
	pagearrows = 0;
	if (esckey == 0xff) {
		pagefx = 2;
		if (mayquit(1)) {
			quit();
			return;
		}
		pagefx = 3;
		goto top;
	}
	nop();
	leavemenu(1);
	pagefx = 0;
}

void
glidemouse(int a, int b)
{
	glidetime = 0;
	linebegin(mousex, mousey, a, b);
	for (; linesteps;) {
		linestep();
		linestep();
		linestep();
		setmouse((short)linestepx, (short)linestepy);
		setclip(mousex, mousey, mousex + 1, mousey + 1);
		mousemoved = 1;
		while (ticks() < glidetime)
			pollinput();
		readinput();
		if (mouseright && esckey != 0xff) {
			esckey = 1;
			return;
		}
		if (esckey)
			return;
		glidetime = ticks() + 20;
	}
}

int
ctrlhover(void)
{
	if (hovery < 50) {
		if (ctrlarmed)
			ctrlnew = 10;
		else
			ctrlnew = 1;
	} else if (hovery >= 430) {
		if (ctrlarmed)
			ctrlnew = 20;
		else
			ctrlnew = 1;
	} else if (hoverx > 590) {
		if (ctrlarmed)
			ctrlnew = 30;
		else
			ctrlnew = 1;
	} else if (hoverx < 50) {
		if (ctrlarmed)
			ctrlnew = 40;
		else
			ctrlnew = 1;
	} else {
		ctrlarmed = 1;
		ctrlnew = 1;
	}
	if (ctrlnew == ctrlhint)
		return 0;
	showscr();
	if (ctrlnew == 1) {
		backto2();
		pagedone();
	} else {
		pgtoback();
		puthands();
		if (ctrlnew == 10) {
			drawmenubar();
			ctrltext = strs[1454];
		} else if (ctrlnew == 20) {
			drawstats();
			ctrltext = strs[1455];
		} else if (ctrlnew == 30) {
			drawinventory(1);
			drawstore(3);
			ctrltext = strs[1456];
		} else if (ctrlnew == 40) {
			drawactions();
			ctrltext = strs[1457];
		} else
			return 0;
		drawpage(&ctrltext, 0xffff, 0xff, 0, 400, 0);
		hoversound();
	}
	ctrlhint = ctrlnew;
	flip();
	return 0;
}

int
controls(short a, int b)
{
	resetanims();
	ctrlcur = shown;
	if (!b) {
		nop();
		if (a)
			fadeslow(0);
		else
			fadefast(0);
	}
	do {
		drawcursor();
		loadscr(0xfa2, (unsigned char *)((char (*)[100])scrbuf), 0, picbuf, 0x4b3e8);
		wsprintf(picbuf, strs[911], texts + 0xb2a2, b ? backbutton() : nextbutton(), texts + 0xb2a2);
		putpage(picbuf, 1, 0);
		showscrt();
		putcs(0, 0xd8, 0x17);
		putcs(0x250, 0xd8, 0x15);
		putcs(0x128, 0, 0x16);
		putcs(0x128, 0x1b0, 0x18);
		save0();
		backto2();
		pagedone();
		ctrlhint = 1;
		ctrlarmed = 0;
		pagein();
		ctrlres = runpage(0, ctrlhover);
	} while (needredraw(ctrlres));
	if (!b) {
		if (a)
			fadeslow(0);
		else
			fadefast(0);
		nop();
	}
	waitrelease(0);
	if (!ctrlcur)
		hidecursor();
	if (ctrlres == 0 || ctrlres == 3)
		return 0;
	return 1;
}

void
empty3(void)
{
}

int
aboutdemo(int a)
{
	workbuf[0] = 0;
	wsprintf((char *)((char (*)[100])scrbuf), strs[1447], licenseno);
	wsprintf(printbuf, strs[1449], (char *)((char (*)[100])scrbuf));
	catstr(workbuf, printbuf);
	wsprintf(printbuf, strs[1450], backbutton());
	catstr(workbuf, printbuf);
	wrap(workbuf, 0x1ac);
	demores = textwin(workbuf, 0, 0xfe, 0, 0, &menu1, 0, a ? 0xff : 0);
	if (demores == 0x27d9) {
		showfile(1);
		return 2;
	}
	if (demores == 0x283d)
		return 3;
	return 0;
}

void
docmenu(void)
{
	nop();
	for (;;) {
		if (checkquit()) {
			pagefx = 3;
			continue;
		}
		if (quitting)
			goto out;
		entermenu();
		docsel = 0;
		// FEATURE: features: the build's own format of the menu's title
#ifdef FEATURES
		wsprintf((char *)((char (*)[100])scrbuf), feattext(texts + 0xb087), texts + 0xb4c0, strs[258]);
#else
		wsprintf((char *)((char (*)[100])scrbuf), texts + 0xb087, texts + 0xb4c0, strs[258]);
#endif
		docitems[docsel++] = (char *)((char (*)[100])scrbuf);
		docitems[docsel++] = strs[917];
		docitems[docsel++] = strs[913];
		docitems[docsel++] = strs[914];
		docitems[docsel++] = strs[915];
		docitems[docsel++] = strs[916];
		docitems[docsel++] = strs[918];
		docitems[docsel++] = strs[919];
		docitems[docsel++] = strs[920];
		docitems[docsel++] = strs[554];
		blankpage();
		listmenu(&menu1, docsel, docitems);
		menu1.keys2[0] = 0x3b;
		pagefx = 2;
		docsel = runmenu(&menu1, 0, 0, 0);
		if (needredraw(docsel)) {
			pagefx = 0;
			continue;
		}
		pagefx = 2;
		if (checkquit()) {
			pagefx = 3;
			continue;
		}
		if (quitting) {
out:
			fadefast(0);
			nop();
			pagefx = 1;
			return;
		}
		switch (docsel) {
		case 0:
		case 9:
quit:
			fadefast(0);
			nop();
			pagefx = 1;
			leavemenu(1);
			return;
		case 1:
			demonote(1, 0);
			copystr(picbuf, printbuf);
			showtext(lang + 0x1faa, lang + 0x1fb1, 0, 1, 0);
			waitrelease(0);
			pagefx = 3;
			break;
		case 2:
			controls(0, 1);
			pagefx = 3;
			break;
		case 3:
			showtext(lang + 0x1391, -1, 0, 1, 0);
			pagefx = 3;
			break;
		case 4:
			showfile(1);
			pagefx = 3;
			break;
		case 5:
			wsprintf(workbuf, strs[1486]);
			catstr(workbuf, texts + 0xb2af);
			catstr(workbuf, backbutton());
			wrap(workbuf, 0x1ac);
			pagefx = 2;
			docsel = textwin(workbuf, 0, 0xfe, 0, 0, &menu1, 0, 0xff);
			if (docsel == 0x283d)
				showtext(lang + 0x2095, -1, 0, 1, 1);
			pagefx = 3;
			break;
		case 6:
			pagefx = 2;
			docres = aboutdemo(1);
			if (docres == 3)
				goto ask;
			pagefx = 3;
			break;
		case 7:
ask:
			pagefx = 2;
			if (aboutfull(0, 1))
				goto quit;
			pagefx = 3;
			break;
		case 8:
			aboutgame();
			aboutdone();
			pagefx = 1;
			break;
		default:
			pagefx = 0;
			break;
		}
	}
}

char version[23] = "@#$(!): 01.005 \xff      ";
short cfgword6 = 1;
short cfgword5 = 1;
unsigned char lookcolors[4][8] = {
	200, 200, 200, 200, 200, 200, 0, 0, 66, 19, 17, 18, 17, 17, 0, 0, 31,
	31, 31, 31, 31, 31, 0, 0, 66, 66, 66, 66, 66, 66
};
int autolevel = 1;
char notopic[2] = "?";
unsigned char skillsnow = 1;
unsigned char combatnow = 1;
short questsel = -1;
short ctrlhint = 1;
short mapmode;
short mapflag;
short mapreg;
short mapprov;
short mapi;
short mapunit;
short lastreg;
short lastprov;
short mapsaved;
int mappos;
unsigned char mapkey;
int localen;
HANDLE readmefile;
DWORD readmelen;
char readmebuf[28];
int totaltime;
OSVERSIONINFO osv;
SYSTEM_INFO sysinfo;
int reportres;
unsigned short optn;
short optchanged;
short redress;
char *optitems[20];
unsigned char optsel;
char *langname;
int oldauto;
char optextra;
short marki;
short markstr;
short markj;
short markmode;
short lastx;
short lasty;
short hover;
int markv;
short markhi;
char *splitp;
short spliti;
short topiccut;
short pagecol;
short pagerow;
short pageline;
char *topicname;
short textsel;
short turndir;
int doclen;
int doclen2;
int docpick;
unsigned short resumemap;
int entermapsum;
Savehdr savehdr;
short descplace;
Savehdr *hdr;
short drawbarrow;
short drawbarcol;
short lookword;
unsigned short strword;
HANDLE outsav;
HANDLE savein;
DWORD savelen;
unsigned short savemapi;
unsigned short savekind;
DWORD copylen;
HANDLE copyh;
HANDLE autofile;
int loadmapi;
int loadkind;
int copymapi;
int copykind;
HANDLE insav;
HANDLE loadout;
DWORD slotpart;
unsigned short loadlook;
unsigned short loadk;
unsigned short loadver;
int loadauto;
short newi;
short newj;
char *latinp;
char *latinq;
short hinti;
short hintw;
short hintws[7];
char heroname[28];
DWORD begini;
char *namep;
short beginv;
short ownphoto;
short beginredo;
char hdrname[16];
HANDLE hdrfile;
unsigned short hdrlook;
unsigned short hdrver;
int slothelm;
unsigned short sloti;
unsigned short slotyear;
unsigned short slotmon;
unsigned short slotday;
unsigned short slothour;
unsigned short slotmin;
unsigned short slotsec;
char slottext[88];
short slotunused;
short slotplace;
short slotsel;
short slotturn;
DWORD readlen;
char *readp;
short ordering;
int fullres;
short mainsel;
short mainexit;
char *mainitems[20];
short sheetgear;
short sheetbars;
short sheetvital;
int sheetbarx;
int sheetbarw;
unsigned long famei;
char *famep;
char *colp;
char sheetstatus[256];
char sheettired[40];
char sheetimmob[64];
char sheetname[24];
// FIXED: a name in hand of 29 bytes or more, as the Italian ones of 32 and 33, takes "con " past sheetweapon's 32 bytes. Give it room for any name that itembuf holds.
#ifdef FEATURE_FAULTS
char sheetweapon[96];
#else
char sheetweapon[32];
#endif
char sheetdmg[32];
char sheetshield[32];
char sheethelm[32];
char sheetarmor[32];
char sheetpants[25];
char hasshield;
char skillmarks[30][8];
char sheetcause[40];
char sheetepitaph[240];
// FIXED: colwrap adds 5 bytes of marks to each line, and lengthens the Italian verdict strs[1364], 113 bytes, past sheetverdict's 120. Give it the room of sheetepitaph.
#ifdef FEATURE_FAULTS
char sheetverdict[240];
#else
char sheetverdict[120];
#endif
char sheetfate[120];
short curatk;
short curdodge;
short curblock;
short curparry;
short curdmin;
short curdmax;
short sheetkind;
short baseatk;
short basedodge;
short baseblock;
short baseparry;
short basedmin;
short basedmax;
DWORD sheetv;
char sheetmode;
char *sheetp;
short sheeti;
short ndeeds;
short deedmark;
unsigned char oldskills;
unsigned char oldcombat;
short sheethunger;
short sheetweary;
int skilldelta;
short outhours;
char *fatep;
short fatesong;
char *notep;
short notei;
char *noteq;
short hasi;
short dropi;
char *bangp;
short nbangs;
short closei;
short closetry;
short closej;
short jouri;
short jourj;
short jourbtn;
short jourunused;
short jourfirst;
short jourleft;
short jourright;
short jourmost;
short jourimp;
short jourall;
short jourdel;
short jourback;
char *jourq;
char *jourp;
short jourlist[500];
short njour;
char jourmarks[20];
DWORD glidetime;
short ctrlnew;
char *ctrltext;
int ctrlres;
short ctrlcur;
int demores;
char *docitems[20];
unsigned short docsel;
unsigned short docres;
char licenseno[80];
/* the code reads and writes 50 bytes of each: at 52, inside their
   padding to 8, wpp386 makes the original's code for autoload */
char cfgtext1[52];
char cfgtext2[52];
char cfgtext3[52];
char cfgtext4[52];
char cfgtext5[52];
char cfgtext6[52];
char cfgtext7[60];
short cfgword2;
short cfgword3;
short cfgword4;
int myface;
int photosum;
short autoslot;
short cfgword7;
int cfgunused;
int cfglong1;
unsigned char topicof[100];
short topicfirst;
short ntopics;
short topicprev;
short topicnext;
char savename[96];
char autoname[16];
int copymode;
int sheetoff;
char sheetdead;
Unit *sheetunit;
short jourtop;
short jourend;
int jourturn;
short ctrlarmed;

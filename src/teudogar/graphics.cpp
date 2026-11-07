#include <windows.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ddraw.h>
#include "base.h"

int __cdecl pollinput(void);
void drawcursor(void);
int loadsurfs(void);
void blanktiles(void);
void puttileo(int n);
char *pick(char *de, char *en, char *fr, char *es, char *it, char *pl);
int seekentry(int id);

/* the result of the last DirectDraw call and its error (0095e804) */
struct Dds {
	HRESULT hr;
	HRESULT err;
};

/* a queued source rectangle of surface 0 (0095ecc0) */
struct Spq {
	int x;
	int y;
	int kind;
	int idx;
};

// PORT: the structs' tags as type names, as C++ has them
#ifdef PORT
typedef struct Dds Dds;
typedef struct Spq Spq;
typedef struct Saved Saved;
#endif

extern char texts[];
// FEATURE: features: the texts of the build, in src/feature/feature.cpp
#ifdef FEATURES
char *feattext(char *s);
#endif

#define T(off) (texts + (off))

extern int appactive;
extern short pagefx;
extern int surfslost;
extern HANDLE datfh;
extern char dderrmsg[];
extern unsigned char defpal[];
extern short lastlight;
extern int shown;
extern int lastx;
extern RECT fullrect;
extern RECT frameleft;
extern RECT frameright;
extern RECT frametop;
extern RECT framebottom;
extern RECT sideleft;
extern RECT sideright;
extern char printbuf[];
extern unsigned char sqrttab[46225];
extern char packbuf[];
extern int pixtime;
extern unsigned char *lzwblock;
extern unsigned char *lzwstack;
extern unsigned char *lzwsuffix;
extern unsigned short *lzwprefix;
extern unsigned short lzwmask[];
extern DWORD storedn;
extern char workbuf[];
extern char loadbuf[];
extern short gfxram;
extern char vramtext[];
extern char blittext[];
extern char vidsurfs[];
extern char syssurfs[];
extern DWORD blittime;
extern DWORD keyblittime;
extern char *surfnames[];
extern unsigned char invram[];
extern HWND mainwnd;
extern int fontw[256];
extern int fontw2[256];
extern unsigned char font[256][480];
extern unsigned char font2[256][120];
extern unsigned char inkcolor;
extern int shadeon;
extern unsigned char shadecolor;
// FIXED: spritetext holds outlinecols and ink3cols in the port
#ifdef PORT
extern char spritetext[2008];
#else
extern char spritetext[2000];
#endif
extern int textmode;
extern char gauges[9][2304];
extern int sheetrows[11];
extern int fontw3[];
extern unsigned char font3[][480];
DWORD unpack(void *dst, DWORD dstlen, void *src, DWORD srclen);
extern DWORD sheetlen;
extern unsigned char loadpack[];
extern DWORD sheetread;
extern unsigned char curspack1[];
extern unsigned char curspack2[];
extern unsigned char iconpack[];
extern unsigned char paperpack[];
// FIXED: outlinecols is the tail of spritetext in the port
#ifdef PORT
#define outlinecols (spritetext + 2000)
#else
extern char outlinecols[4];
#endif
extern int setpics;
extern int dir0frm6;
extern int dir0frm7;
extern int dir1frm0;
extern int dir1frm6;
extern int dir1frm7;
extern int dir2frm0;
extern int dir2frm6;
extern int dir2frm7;
extern int dir3frm0;
extern int dir3frm6;
extern int dir3frm7;
extern int lostcontrol;
extern int ptrstates;
extern int ptrdrawn;
extern int curkind;
extern int curimg;
extern int curshape;
extern int lasty;
extern int curkind1;
extern int curimg1;
extern int curkind2;
extern int curimg2;
extern RECT msbackrect;
extern RECT ipbackrect;
extern RECT iprect;
int storedlen(int id);
extern unsigned char btncol;
extern int btnx;
extern int btny;
extern int btnfull;
/* a screen saved in the second half of packbuf, marked "WmWm" */
struct Saved {
	char magic[4];
	char scr[480][640];
};
void msstates(int n);
extern unsigned char *facecache[57];
extern DWORD facesizes[57];
extern unsigned char *overcache[57];
extern DWORD oversizes[57];
extern int overonly;
extern short shadowleft;
extern int nofani;
extern DWORD myfacesum;
extern char heroface[0x32a0];
extern char heroface2[0x32a0];
extern unsigned char headicons[8][2304];
extern int sqx;
extern int sqy;
extern int sqcol;
extern int sqw;
extern int sqrow;
extern int sqh;
extern int sqstate;
extern int bigx;
extern int bigy;
extern char bigchars[];

extern int nmaptiles;
extern int nobjtiles;
extern int ncurtiles;
extern LPDIRECTDRAW dd;
extern DDCAPS ddcaps;
extern BOOL vblank;
extern LPDIRECTDRAWSURFACE back;
extern LPDIRECTDRAWSURFACE prim;
extern LPDIRECTDRAWSURFACE surf2;
extern LPDIRECTDRAWSURFACE surfc;
extern LPDIRECTDRAWSURFACE surfm[7];
extern LPDIRECTDRAWSURFACE surfo[13];
extern LPDIRECTDRAWSURFACE surfw[5];
extern int surfwh[5];
extern LPDIRECTDRAWSURFACE surfpg;
extern LPDIRECTDRAWSURFACE surfs[8];
extern DWORD vidmem;
extern DWORD sysmem;
extern unsigned char surfwhere[8];
extern LPDIRECTDRAWSURFACE cursurf;
extern LPDIRECTDRAWPALETTE ddpal;
extern unsigned char palblack;
extern int level;
extern int fadecount;
extern DWORD tmp;
extern unsigned char *palp;
extern int setpalcount;
extern unsigned char *palq;
extern DWORD tmp2;
extern unsigned char *lgsrc;
extern unsigned char *lgdst;
extern unsigned char lgmul;
extern int lgi;
extern PALETTEENTRY pe[256];
extern char gamepal[768];
extern unsigned char lighter[256];
extern unsigned char darker[256];
extern unsigned char lighter2[256];
extern unsigned char darker2[256];
extern unsigned char darker4[256];
extern unsigned char lum[256];
extern unsigned char pals[10][768];
extern short lastwarm;
extern int palavg;
extern int getpalcount;
extern unsigned char *btp;
extern int bti;
extern int btnext;
extern int btnext2;
extern int btprevr;
extern int btprevg;
extern int btprevb;
extern int btr;
extern int btg;
extern int btb;
extern int fsi;
extern unsigned char *psdst;
extern unsigned char *pssrc;
extern int psrow;
extern DDSURFACEDESC ddsd;
extern HRESULT psres;
extern int rsfi;
extern int unlocktries;
extern unsigned char *prdst;
extern unsigned char *prsrc;
extern int prrow;
extern unsigned char *ptdst;
extern unsigned char *ptsrc;
extern unsigned char ptpix;
extern int ptx;
extern int pty;
extern unsigned char *grdst;
extern unsigned char *grsrc;
extern int grrow;
extern int reldctries;
extern DDBLTFX mirrorfx;
extern int pscrrow;
extern char *pscrsrc;
extern char *pscrdst;
extern int gscrrow;
extern char *gscrdst;
extern char *gscrsrc;
extern int prowi;
extern unsigned char *prowsrc;
extern unsigned char *prowdst;
extern int pblkrow;
extern unsigned char *pblksrc;
extern unsigned char *pblkdst;
extern int pbtx;
extern int pbty;
extern unsigned char *pbtsrc;
extern unsigned char *pbtdst;
extern int cdx;
extern int cdy;
extern int cdr2;
extern int cdx2;
extern int cdy2;
extern int cdoff;
extern int sstx;
extern int ssty;
extern char *sstsrc;
extern char *sstdst;
extern int sprow;
extern char *spsrc;
extern unsigned char *spdst;
extern int sptx;
extern int spty;
extern char *sptsrc;
extern char *sptdst;
extern int g48y;
extern int g48x;
extern int g48any;
extern int p48tx;
extern int p48ty;
extern char *p48tdst;
extern int p48row;
extern unsigned char *p48dst;
extern int p48srow;
extern unsigned char *p48sdst;
extern int p48ptx;
extern int p48pty;
extern char *p48ptdst;
extern int g48row;
extern unsigned char *g48src;
extern int g48srow;
extern unsigned char *g48ssrc;
extern unsigned char *pwhdst;
extern int pwhrow;
extern char *pcldst;
extern int pclx;
extern int pcly;
extern char *psubdst;
extern int psubx;
extern int psuby;
extern unsigned char *gwhsrc;
extern int gwhrow;
extern unsigned char *gwhssrc;
extern int gwhsrow;
extern unsigned char *gbsrc;
extern int gbrow;
extern int textflen;
extern unsigned char *shp;
extern int shx;
extern int shy;
extern unsigned char *fbp;
extern int fbx;
extern int fby;
extern unsigned char *sh2p;
extern int sh2x;
extern int sh2y;
extern unsigned char *lap;
extern int lax;
extern int lay;
extern unsigned char gppix;
extern unsigned char gpspix;
extern int dlerrx;
extern int dlerry;
extern int dldx;
extern int dldy;
extern int dlsx;
extern int dlsy;
extern unsigned char *dlp;
extern int dli;
extern int dllen;
extern int cix;
extern int ciy;
extern int cir2;
extern unsigned char *ss2p;
extern unsigned char *ss2op;
extern unsigned char *ss4p;
extern unsigned char *pcolp;
extern int zoomw;
extern int zoomh;
extern DWORD zoomtime;
extern int meltcol;
extern int meltstep;
extern int meltdone;
extern int colh[80];
extern DWORD colrnd[160];
extern int meltrnd;
extern DWORD melttime;
extern int splitmid;
extern int splitrow;
extern int splitend;
extern int splithalf;
extern DWORD splittime;
extern int dvi;
extern int dvstep;
extern DWORD dvtime;
extern int dvtsteps;
extern unsigned char lzwch;
extern int lzwclear;
extern int lzwbits;
extern int lzwtop;
extern int lzwslot;
extern int lzwcode;
extern int lzwfc;
extern int lzwc;
extern int lzwoc;
extern int lzwnbits;
extern int lzwnavail;
extern int lzwx;
extern unsigned char *lzwp;
extern unsigned char *lzwend;
extern unsigned char *lzwsp;
extern unsigned char *lzwout;
extern int lzwcount;
extern DWORD uslen;
extern char *bmptr;
extern int lbi;
extern DWORD lbx;
extern int lby;
extern int mbi;
extern DWORD mbx;
extern int mby;
extern HANDLE lbfh;
extern DWORD lbfn;
extern HANDLE sbfh;
extern int sbi;
extern DWORD sbx;
extern int sby;
extern int sbok;
extern unsigned char sbr;
extern unsigned char sbg;
extern unsigned char sbb;
extern unsigned char sbbyte;
extern unsigned char sbrow[640];
extern char *ddinfop;
extern int ddinfon;
extern DWORD pxtime;
extern int pxcol;
extern int pxrow;
extern int blti;
extern int bltj;
extern DDSURFACEDESC smdesc;
extern DDSCAPS backcaps;
extern int ddi;
extern DWORD modetime;
extern int modew;
extern int modeh;
extern int modebpp;
extern int modetry;
extern int psover;
extern unsigned char *dcdst;
extern unsigned char *dcsrc;
extern unsigned char dcpix;
extern int dchx;
extern int dchy;
extern int dcw;
extern int dcpad;
extern int dtch;
extern int dtw;
extern unsigned char *dc2dst;
extern unsigned char *dc2src;
extern unsigned char dc2pix;
extern int dc2x;
extern int dc2y;
extern int twsum;
extern int dt2ch;
extern int dt2w;
extern unsigned char *g3dst;
extern unsigned char *g3src;
extern unsigned char *g3glyph;
extern unsigned char g3pix;
extern int g3x;
extern int g3y;
extern int g3rowinked;
extern int g3pitch;
extern int g3colinked[24];
extern int spqi;
extern int t3code;
extern int t3kind;
extern int t3w;
extern int t3pass;
extern int t3x0;
extern int t3y0;
extern int t3col;
extern char *t3p;
extern int t3ores;
extern int t3omode;
extern int t3bres;
extern int t3bmode;
extern int t3gres;
extern int t3colres;
extern int t3dkres;
extern int t3cgres;
extern int t3cdkres;
extern int t3ccres;
extern int tg2res;
extern int t3fres;
extern int irpage;
extern int iri;
extern int irj;
extern int pwow;
extern int pwoh;
extern int pwoi;
extern int susheet;
extern int susurf;
extern int suhalf;
extern int sualt;
extern char *tinfop;
extern int tbx;
extern int tby;
extern int tmi;
extern int slerrx;
extern int slerry;
extern int sldx;
extern int sldy;
extern int slsx;
extern int slsy;
extern unsigned char *slp;
extern unsigned char *sllast;
extern int sli;
extern int sllen;
extern unsigned char *sr8p;
extern unsigned char sr8c;
extern int sr8x;
extern unsigned char *sr7p;
extern unsigned char sr7c;
extern int sr7x;
extern unsigned char *sr8bp;
extern unsigned char sr8bc;
extern int sr8bx;
extern unsigned char *sr9p;
extern int sr9x;
extern unsigned char *ss9p;
extern int ss9x;
extern int ss9y;
extern unsigned char *ssap;
extern int ssax;
extern int ssay;
extern unsigned char *rrdst;
extern unsigned char *rrsrc;
extern int rrx;
extern int plrr;
extern int plrw;
extern int plry;
extern int plrr2;
extern int ps8r;
extern int ps8w;
extern int ps8y;
extern int ps8r2;
extern int ps9r;
extern int ps9w;
extern int ps9y;
extern int ps9r2;
extern int ringr;
extern int ringw;
extern int ringy;
extern int ringr2;
extern int ringpx;
extern int ringpy;
extern char *btnp;
extern int baleft;
extern int baright;
extern int pgicon;
extern int wf2mid;
extern DWORD lfidx;
extern DWORD lfrun;
extern DWORD lfi;
extern unsigned char *lfover;
extern unsigned char *lfoutp;
extern int fscol0;
extern int fscol;
extern int fsw;
extern int fsrow;
extern int ftmi;
extern unsigned char *ftmp;
extern char efname[20];
extern HANDLE efh;
extern DWORD efn;
extern int efi;
extern int sqtx;
extern int sqty;
extern unsigned char *sqtdst;
extern int sq48x;
extern int sq48y;
extern unsigned char *sq48dst;
extern DWORD cpidx;
extern char *cpp;
extern int bgx0;
extern int bgy;
extern int t32n;
extern int t64n;
extern char scrbuf[309700];
extern unsigned char tilebuf[48][48];
extern unsigned char sbpal[768];
extern DDSURFACEDESC sd;
extern HDC surfdc;
extern RECT rc2;
extern RECT rc;
extern DDBLTFX fillfx;
extern int modeset;
extern int ddready;
extern int text3x;
extern int text3y;
extern int spacestop;
extern int boldtext;
extern int capsfont;
extern int text3width;
extern unsigned char *drawbuf;
extern HGDIOBJ textfont;
extern RECT srcrects[222];
extern unsigned char srcpages[222];
extern int spriteshape;
extern int spritex;
extern int spritey;
extern int msx;
extern int msy;
extern int mscx;
extern int mscy;
extern int msw;
extern int msh;
extern HRESULT ddres[2];
extern int ddlost;
extern int ddbusy;
extern int linestepx;
extern int linestepy;
extern int linesteps;
extern int lserrx;
extern int lserry;
extern int lsdx;
extern int lsdy;
extern int lssx;
extern int lssy;
extern int lsmax;
extern unsigned char *lzwsrc;
extern int lzwsize;
extern unsigned char *lzwpos;
extern unsigned char *lzwdst;
extern unsigned lzwoff;
extern int lzwplanar;
extern unsigned char lzwbuf[1024];
extern BITMAPFILEHEADER bmfh;
extern BITMAPINFOHEADER bmih;
extern int photomagic;
extern LPDIRECTDRAWSURFACE srcsurf;
extern int srcw;
extern int srch;
extern int tilepage;
extern int srcunused;
extern RECT srcrc;
extern DWORD srcflags;
extern Spq spq[10];
extern int nspq;
extern unsigned char *tilecache;
extern DWORD ntiles;
extern int minmem;
extern int nopani;
extern int nopequip;
extern int minsets;
extern int nopvari;
extern int noprun;
extern int dir0frm0;
extern unsigned char *gearbank;
extern int hasgearbank;

void
savesurfs(void)
{
	surfs[0] = back;
	surfs[1] = prim;
	surfs[2] = surf2;
	surfs[3] = surfc;
	surfs[4] = surfm[0];
	surfs[5] = surfo[0];
	surfs[6] = surfw[0];
	surfs[7] = surfpg;
}

void
selsurf(int n)
{
	cursurf = surfs[n];
}

int
pixoff(int x, int y)
{
	if (x < 0 || y < 0 || x >= 640 || y >= 480)
		return -1;
	return y * 640 + x;
}

int
needredraw(int c)
{
	if (surfslost == 0)
		return 0;
	surfslost = 0;
	if (c == 0xfd || c == 0xfd || c == 0xfd || c == 0xfd)
		return 1;
	return 0;
}

/*
 * dderrstr names a DirectDraw error. The original's literals end in two
 * NULs, so each here ends in \0.
 */
char *
dderrstr(HRESULT hr)
{
	switch (hr) {
	case DD_OK:
		return "ok\0";
	case DDERR_ALREADYINITIALIZED:
		return "obj alr initd\0";
	case DDERR_BLTFASTCANTCLIP:
		return "ret if clipper obj is attached 2 BltFast( src surf )\0";
	case DDERR_CANNOTATTACHSURFACE:
		return "surf cant be attached 2 req surf\0";
	case DDERR_CANNOTDETACHSURFACE:
		return "surf cant be detached fr req surf\0";
	case DDERR_CANTCREATEDC:
		return "Win cant create any more DCs\0";
	case DDERR_CANTDUPLICATE:
		return "cant duplicate prim/3D/implicitly created surfs\0";
	case DDERR_CLIPPERISUSINGHWND:
		return "attempt 2 set cliplst 4 clipper obj alr monit'ing an hwnd\0";
	case DDERR_COLORKEYNOTSET:
		return "no src color key specd 4 this op\0";
	case DDERR_CURRENTLYNOTAVAIL:
		return "support currently not available\0";
	case DDERR_DIRECTDRAWALREADYCREATED:
		return "DDrw obj repr this driver alr created 4 this process\0";
	case DDERR_EXCEPTION:
		return "exception while perfing req op\0";
	case DDERR_EXCLUSIVEMODEALREADYSET:
		return "attempt 2 set coop lev when alr set 2 excl\0";
	case DDERR_GENERIC:
		return "generic failure\0";
	case DDERR_HEIGHTALIGN:
		return "height of prov rect != multiple of reqd alignm\0";
	case DDERR_HWNDALREADYSET:
		return "CoopLev HWND alr set; cant be reset while process has surfs/pals\0";
	case DDERR_HWNDSUBCLASSED:
		return "HWND used by DDrw CoopLev subclassed, this prev DDrw fr restor state\0";
	case DDERR_IMPLICITLYCREATED:
		return "surf cant be rstord bec it is implicitly cr surf\0";
	case DDERR_INCOMPATIBLEPRIMARY:
		return "cant match prim surf cr req w/ exist prim surf\0";
	case DDERR_INVALIDCAPS:
		return "1/more of caps bits passed 2 callbck incorr\0";
	case DDERR_INVALIDCLIPLIST:
		return "DDrw doesnt supp prov cliplist\0";
	case DDERR_INVALIDDIRECTDRAWGUID:
		return "GUID passed 2 DDrwCr inval DDrw driver identif\0";
	case DDERR_INVALIDMODE:
		return "DDrw doesnt supp reqd mode\0";
	case DDERR_INVALIDOBJECT:
		return "DDrw recd ptr: inval DDrw obj\0";
	case DDERR_INVALIDPARAMS:
		return "1/more pars passed 2 f() incorr\0";
	case DDERR_INVALIDPIXELFORMAT:
		return "pix form inval as specd\0";
	case DDERR_INVALIDPOSITION:
		return "ret wh dest overlay pos > legal 4 that dest\0";
	case DDERR_INVALIDRECT:
		return "inval rect provided\0";
	case DDERR_LOCKEDSURFACES:
		return "!op() bec 1/more surfs locked\0";
	case DDERR_NO3D:
		return "no 3D present\0";
	case DDERR_NOALPHAHW:
		return "!op() bec no alpha accel hardw present/available\0";
	case DDERR_NOBLTHW:
		return "no blitter hardw present\0";
	case DDERR_NOCLIPLIST:
		return "no cliplist available\0";
	case DDERR_NOCLIPPERATTACHED:
		return "no clipper obj attached 2 surf obj\0";
	case DDERR_NOCOLORCONVHW:
		return "!op() bec no color convers hardw present/available\0";
	case DDERR_NOCOLORKEY:
		return "surf doesn't currently have color key\0";
	case DDERR_NOCOLORKEYHW:
		return "!op() bec no hardwsupp of dest color key\0";
	case DDERR_NOCOOPERATIVELEVELSET:
		return "Cr() called w/out DDrw obj meth SetCoopLev\0";
	case DDERR_NODC:
		return "no DC w ever created 4 this surf\0";
	case DDERR_NODDROPSHW:
		return "no DDrw ROP hardw\0";
	case DDERR_NODIRECTDRAWHW:
		return "hardw-only DDrw obj creation attempted but driver didnt supp any hardw\0";
	case DDERR_NOEMULATION:
		return "softw emulation not available\0";
	case DDERR_NOEXCLUSIVEMODE:
		return "op requires app 2 have exclusive mode (Close all other programs before starting this game)\0";
	case DDERR_NOFLIPHW:
		return "flipping visible surfs !suppd\0";
	case DDERR_NOGDI:
		return "no GDI present\0";
	case DDERR_NOHWND:
		return "Clipper notif reqs HWND / no HWND prev set as CoopLev HWND\0";
	case DDERR_NOMIRRORHW:
		return "!op() bec no hardw present/available\0";
	case DDERR_NOOVERLAYDEST:
		return "ret wh GetOverlayPos called on ovrl that UpdOvrl hasnt called on 2 establ dest\0";
	case DDERR_NOOVERLAYHW:
		return "!op() bec no overlay hardw present /available\0";
	case DDERR_NOPALETTEATTACHED:
		return "no pal obj attached 2 this surf\0";
	case DDERR_NOPALETTEHW:
		return "no hardw supp 4 16/256 colr pals\0";
	case DDERR_NORASTEROPHW:
		return "!op() bec no approp raster op hardw present/available\0";
	case DDERR_NOROTATIONHW:
		return "!op() bec no rotation hardw present/available\0";
	case DDERR_NOSTRETCHHW:
		return "!op() bec no hardwsupp 4 stretching\0";
	case DDERR_NOT4BITCOLOR:
		return "reqd op reqs 4 bit colr pal & DDSurf isnt\0";
	case DDERR_NOT4BITCOLORINDEX:
		return "reqd op reqs 4 bit colr ix pal & DDSurf isnt\0";
	case DDERR_NOT8BITCOLOR:
		return "reqd op requires 8 bit color & DDSurf isnt\0";
	case DDERR_NOTAOVERLAYSURFACE:
		return "ret wh overlay memb is called 4 non-overlay surf\0";
	case DDERR_NOTEXTUREHW:
		return "!op() bec no texture mapping hardw present/available\0";
	case DDERR_NOTFLIPPABLE:
		return "attempt 2 flip surf that isnt flippable\0";
	case DDERR_NOTFOUND:
		return "reqd item not found\0";
	case DDERR_NOTLOCKED:
		return "attempt 2 unlock surf that wasnt locked at all/by this process\0";
	case DDERR_NOTPALETTIZED:
		return "surf being used !pal-based surf\0";
	case DDERR_NOVSYNCHW:
		return "!op() bec no hardw supp 4 vert blank synchr ops\0";
	case DDERR_NOZBUFFERHW:
		return "!op() bec no hardw supp 4 zbuffer blit\0";
	case DDERR_NOZOVERLAYHW:
		return "overlay surfs couldnt be z layered based on their BltOrder bec hardw doesnt supp z layering of overlays\0";
	case DDERR_OUTOFCAPS:
		return "hardw needed 4 reqd op alr allocated\0";
	case DDERR_OUTOFMEMORY:
		return "DDrw !enough mem 2 perf op\0";
	case DDERR_OUTOFVIDEOMEMORY:
		return "DDrw !enough vmem 2 perf op\0";
	case DDERR_OVERLAYCANTCLIP:
		return "hardw doesnt supp clipped overlays\0";
	case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
		return "can only have 1 color key active at one time 4 overlays\0";
	case DDERR_OVERLAYNOTVISIBLE:
		return "ret wh GetOverlayPos is called on hidden overlay\0";
	case DDERR_PALETTEBUSY:
		return "access 2 this pal refused bec pal is alr locked by another thread\0";
	case DDERR_PRIMARYSURFACEALREADYEXISTS:
		return "this process alr has created prim surf\0";
	case DDERR_REGIONTOOSMALL:
		return "region passed 2 Clipper::GetClipList too small\0";
	case DDERR_SURFACEALREADYATTACHED:
		return "this surf is alr attached 2 surf its being attd 2\0";
	case DDERR_SURFACEALREADYDEPENDENT:
		return "this surf is alr dependency of surf its being md dep of\0";
	case DDERR_SURFACEBUSY:
		return "access 2 this surf refused bec surf alr locked by another thread\0";
	case DDERR_SURFACEISOBSCURED:
		return "access 2 surf refused bec surf is obscured\0";
	case DDERR_SURFACELOST:
		return "access 2 this surf refused bec surf mem gone DDSurf obj repr this surf should restore()\0";
	case DDERR_SURFACENOTATTACHED:
		return "reqd surf not attached\0";
	case DDERR_TOOBIGHEIGHT:
		return "height reqd by DDrw too large\0";
	case DDERR_TOOBIGSIZE:
		return "size reqd by DDrw too large, but indiv height&width ok\0";
	case DDERR_TOOBIGWIDTH:
		return "width reqd by DDrw too large\0";
	case DDERR_UNSUPPORTED:
		return "action not suppd\0";
	case DDERR_UNSUPPORTEDFORMAT:
		return "FOURCC format reqd unsuppd by DDrw\0";
	case DDERR_UNSUPPORTEDMASK:
		return "bitmask in pix form reqd unsuppd by DDrw\0";
	case DDERR_VERTICALBLANKINPROGRESS:
		return "vert blank in progress\0";
	case DDERR_WASSTILLDRAWING:
		return "previous Blt which is transf inf 2/fr this surf is incompl\0";
	case DDERR_WRONGMODE:
		return "this surf cant be restord bec it w cr in diffr mode\0";
	case DDERR_XALIGN:
		return "rect prov w not hor aligned on reqd boundary\0";
	default:
		return "unknown err\0";
	}
}

int
ddfailed(char *where, HRESULT hr)
{
	(*(Dds *)ddres).hr = hr;
	ddlost = 0;
	ddbusy = 0;
	if (hr != DD_OK) {
		if (hr == DDERR_SURFACELOST && appactive) {
			ddlost = 1;
			return 2;
		}
		if ((*(Dds *)ddres).hr == DDERR_WASSTILLDRAWING) {
			ddbusy = 1;
			return 2;
		}
		(*(Dds *)ddres).err = (*(Dds *)ddres).hr;
		wsprintf(dderrmsg, T(0x8e4a), where, dderrstr((*(Dds *)ddres).hr), (*(Dds *)ddres).hr, HRESULT_SEVERITY((*(Dds *)ddres).hr), HRESULT_FACILITY((*(Dds *)ddres).hr), HRESULT_CODE((*(Dds *)ddres).hr));
		return 1;
	}
	return 0;
}

void
keephigh(char *pal)
{
	memcpy(pal + 0x2a0, pals[0] + 0x2a0, 0x60);
}

void
keepui(char *pal)
{
	memcpy(pal + 0x2a0, pals[0] + 0x2a0, 0x15);
	memcpy(pal + 0x2cd, pals[0] + 0x2cd, 0x33);
}

void nop(void);

void
keepink(char *pal)
{
	memcpy(pal + 0x2d0, pals[0] + 0x2d0, 0x12);
	nop();
}

void
nop(void)
{
}

void
picink(void)
{
	gamepal[0x2d0] = gamepal[0x13b];
	gamepal[0x2d1] = gamepal[0x13c];
	gamepal[0x2d2] = gamepal[0x13d];
	gamepal[0x2d3] = gamepal[0x2d0] - 1;
	gamepal[0x2d4] = gamepal[0x2d1] - 1;
	gamepal[0x2d5] = gamepal[0x2d2] - 1;
	gamepal[0x2d6] = gamepal[0x13e];
	gamepal[0x2d7] = gamepal[0x13f];
	gamepal[0x2d8] = gamepal[0x140];
	gamepal[0x2d9] = gamepal[0x2d6] - 1;
	gamepal[0x2da] = gamepal[0x2d7] - 1;
	gamepal[0x2db] = gamepal[0x2d8] - 1;
}

void
setbw(char *pal)
{
	pal[0] = 0;
	pal[1] = 0;
	pal[2] = 0;
	pal[0x2fd] = 0x3f;
	pal[0x2fe] = 0x3f;
	pal[0x2ff] = 0x3f;
}

void
waitvbl(void)
{
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (IDirectDraw_GetVerticalBlankStatus(dd, &vblank) == DD_OK && vblank)
#else
	while (dd->GetVerticalBlankStatus(&vblank) == DD_OK && vblank)
#endif
		;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (IDirectDraw_GetVerticalBlankStatus(dd, &vblank) == DD_OK && !vblank)
#else
	while (dd->GetVerticalBlankStatus(&vblank) == DD_OK && !vblank)
#endif
		;
}

void
waitvb(void)
{
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (IDirectDraw_WaitForVerticalBlank(dd, DDWAITVB_BLOCKBEGIN, NULL) == DDERR_WASSTILLDRAWING)
#else
	while (dd->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, NULL) == DDERR_WASSTILLDRAWING)
#endif
		;
}

int
fade(int in, int step)
{
	int i;

	if (in) {
		if (!palblack)
			return 0;
		palblack = 0;
		level = 0;
		if (!appactive)
			return 1;
	} else {
		pagefx = 1;
		if (palblack)
			return 0;
		palblack = 1;
		level = 100;
		if (!appactive)
			return 1;
	}
	for (;;) {
		// PORT: the palette as unsigned char, which C wants cast
#ifdef PORT
		palp = (unsigned char *)gamepal;
#else
		palp = gamepal;
#endif
		for (i = 0; i < 256; i++) {
			tmp = 0;
			tmp = *palp++ << 2;
			tmp *= level;
			tmp /= 100;
			pe[i].peRed = tmp;
			tmp = 0;
			tmp = *palp++ << 2;
			tmp *= level;
			tmp /= 100;
			pe[i].peGreen = tmp;
			tmp = 0;
			tmp = *palp++ << 2;
			tmp *= level;
			tmp /= 100;
			pe[i].peBlue = tmp;
			pe[i].peFlags = 0;
		}
		fadecount = 256;
		tmp = ticks();
		waitvbl();
		// PORT: DirectDraw through its C binding
#ifdef PORT
		(*(Dds *)ddres).hr = IDirectDrawPalette_SetEntries(ddpal, 0, 0, 256, pe);
#else
		(*(Dds *)ddres).hr = ddpal->SetEntries(0, 0, 256, pe);
#endif
		if (ddfailed("pal_bl.sEntr", (*(Dds *)ddres).hr))
			return 0;
		if (in) {
			if (level >= 100)
				return 1;
			level += step;
			if (level > 100)
				level = 100;
		} else {
			if (level <= 0)
				return 1;
			level -= step;
			if (level < 0)
				level = 0;
		}
		do
			pollinput();
		while (since(tmp) < 20);
	}
}

int
fadefast(int in)
{
	return fade(in, 25);
}

int
fademed(int in)
{
	return fade(in, 13);
}

int
fadeslow(int in)
{
	return fade(in, 3);
}

int
setpal(void)
{
	int i;

	palblack = 0;
	// PORT: the palette as unsigned char, which C wants cast
#ifdef PORT
	palq = (unsigned char *)gamepal;
#else
	palq = gamepal;
#endif
	for (i = 0; i < 256; i++) {
		tmp2 = 0;
		tmp2 = *palq++ << 2;
		pe[i].peRed = tmp2;
		tmp2 = 0;
		tmp2 = *palq++ << 2;
		pe[i].peGreen = tmp2;
		tmp2 = 0;
		tmp2 = *palq++ << 2;
		pe[i].peBlue = tmp2;
		pe[i].peFlags = 0;
	}
	setpalcount = 256;
	waitvb();
	// PORT: DirectDraw through its C binding
#ifdef PORT
	(*(Dds *)ddres).hr = IDirectDrawPalette_SetEntries(ddpal, 0, 0, 256, pe);
#else
	(*(Dds *)ddres).hr = ddpal->SetEntries(0, 0, 256, pe);
#endif
	if (ddfailed("pal_s.sEntr", (*(Dds *)ddres).hr))
		return 0;
	return 1;
}

int
lightpal(int n, int which)
{
	lgsrc = which ? pals[1] : pals[0];
	// PORT: the palette as unsigned char, which C wants cast
#ifdef PORT
	lgdst = (unsigned char *)gamepal;
#else
	lgdst = gamepal;
#endif
	lgmul = n + 1;
	for (lgi = 0; lgi < 224; lgi++) {
		*lgdst++ = *lgsrc++ * lgmul >> 4;
		*lgdst++ = *lgsrc++ * lgmul >> 4;
		*lgdst++ = *lgsrc++ * lgmul >> 4;
	}
	for (lgsrc = pals[0] + lgi * 3; lgi < 256; lgi++) {
		*lgdst++ = *lgsrc++;
		*lgdst++ = *lgsrc++;
		*lgdst++ = *lgsrc++;
	}
	if (palblack)
		return 0;
	return setpal();
}

int
palblk(void)
{
	palblack = 1;
	memset(pe, 0, sizeof pe);
	// PORT: DirectDraw through its C binding
#ifdef PORT
	if (ddfailed("pal_blck.sEntr", IDirectDrawPalette_SetEntries(ddpal, 0, 0, 256, pe)))
#else
	if (ddfailed("pal_blck.sEntr", ddpal->SetEntries(0, 0, 256, pe)))
#endif
		return 0;
	return 1;
}

int
getpal(unsigned char *pal)
{
	int i;
	unsigned char *p;

	// PORT: DirectDraw through its C binding
#ifdef PORT
	if (ddfailed("g_cpal.gEntr", IDirectDrawPalette_GetEntries(ddpal, 0, 0, 256, pe))) {
#else
	if (ddfailed("g_cpal.gEntr", ddpal->GetEntries(0, 0, 256, pe))) {
#endif
		memcpy(pal, pals[0], 0x300);
		return 0;
	}
	p = pal;
	for (i = 0; i < 256; i++) {
		p[0] = pe[i].peRed >> 2;
		p[1] = pe[i].peGreen >> 2;
		p[2] = pe[i].peBlue >> 2;
		p += 3;
	}
	getpalcount = 256;
	return 1;
}

int
loadpal(unsigned char *pal, int id)
{
	memcpy(pal, defpal, 0x300);
	if (!seekentry(id))
		return 0;
	if (!fileread(pal, 0x300, 1, datfh))
		return 0;
	pal[0] = 0;
	pal[1] = 0;
	pal[2] = 0;
	pal[0x2fd] = 0x3f;
	pal[0x2fe] = 0x3f;
	pal[0x2ff] = 0x3f;
	return 1;
}

void
buildtables(void)
{
	for (bti = 0; bti < 256; bti++) {
		lum[bti] = (gamepal[bti * 3 + 2] + (gamepal[bti * 3] + gamepal[bti * 3 + 1])) / 12;
		if (lum[bti] > 15)
			lum[bti] = 15;
		lum[bti] = 15 - lum[bti];
		darker[bti] = lighter[bti] = 0xff;
		darker2[bti] = bti;
		darker4[bti] = bti;
		lighter2[bti] = bti;
	}
	/* naming btprevb first gives it edi and btprevg esi, as the original */
	(void)btprevb;
	// PORT: the palette as unsigned char, which C wants cast
#ifdef PORT
	btp = (unsigned char *)gamepal;
#else
	btp = gamepal;
#endif
	btprevr = *btp++;
	btprevg = *btp++;
	btprevb = *btp;
	btp = btp + 1;
	for (bti = 1; bti < 256; bti++) {
		int b, g, r;

		r = *btp++;
		g = *btp++;
		b = *btp;
		btr = r;
		btg = g;
		btp = btp + 1;
		btb = b;
		if (r >= btprevr - 11 && r <= btprevr && g >= btprevg - 11 && g <= btprevg && b >= btprevb - 11 && b <= btprevb) {
			lighter[bti] = bti - 1;
			darker[bti - 1] = bti;
		}
		btprevg = btg;
		btprevr = btr;
		btprevb = btb;
	}
	for (bti = 1; bti < 256; bti++) {
		btnext = darker[bti];
		if (btnext != 0xff) {
			btnext2 = darker[btnext];
			if (btnext2 != 0xff)
				darker2[bti] = btnext2;
			else
				darker2[bti] = btnext;
		}
		btnext = lighter[bti];
		if (btnext != 0xff) {
			btnext2 = lighter[btnext];
			if (btnext2 != 0xff)
				lighter2[bti] = btnext2;
			else
				lighter2[bti] = btnext;
		}
	}
	for (bti = 1; bti < 256; bti++) {
		btnext = darker2[bti];
		if (btnext != 0xff) {
			btnext2 = darker2[btnext];
			if (btnext2 != 0xff)
				darker4[bti] = btnext2;
			else
				darker4[bti] = btnext;
		}
	}
	for (bti = 0xe0; bti < 256; bti++) {
		lighter[bti] = 0xff;
		darker[bti] = 0xff;
	}
}

void
usepal(unsigned char n)
{
	/*
	 * unused: a temporary of the code generator, which gives
	 * savebmpf's multiply by 3 the original's lea
	 */
	int u0;

	memcpy(gamepal, pals[n], 0x300);
	buildtables();
	lastlight = 0xff;
	lastwarm = 0xff;
}

void
initpals(void)
{
	int i;

	memcpy(pals[0], defpal, 0x300);
	memcpy(pals[9], defpal, 0x300);
	palavg = 0;
	for (i = 0; i < 0x300; i++)
		palavg += defpal[i];
	palavg /= 256;
}

void
makepals(void)
{
	int i;

	pals[0][0] = 0;
	pals[0][1] = 0;
	pals[0][2] = 0;
	pals[1][0] = 0;
	pals[1][1] = 0;
	pals[1][2] = 0;
	for (i = 1; i < 256; i++) {
		unsigned char r, g, b;
		short v;

		r = pals[0][i * 3];
		g = pals[0][i * 3 + 1];
		b = pals[0][i * 3 + 2];
		v = r + 4;
		if (v > 63)
			v = 63;
		pals[1][i * 3] = v;
		v = g - 2;
		if (v < 0)
			v = 0;
		pals[1][i * 3 + 1] = v;
		v = b - 2;
		if (v < 0)
			v = 0;
		pals[1][i * 3 + 2] = v;
	}
	for (i = 184; i < 192; i++) {
		unsigned char r, g, b;
		short v;

		r = pals[0][i * 3];
		g = pals[0][i * 3 + 1];
		b = pals[0][i * 3 + 2];
		v = r + 2;
		if (v > 63)
			v = 63;
		pals[1][i * 3] = v;
		v = g - 1;
		if (v < 0)
			v = 0;
		pals[1][i * 3 + 1] = v;
		v = b - 1;
		if (v < 0)
			v = 0;
		pals[1][i * 3 + 2] = v;
	}
	for (i = 1; i < 256; i++) {
		unsigned char r, g, b;
		short v;

		r = pals[0][i * 3];
		g = pals[0][i * 3 + 1];
		b = pals[0][i * 3 + 2];
		v = (r + g + b) / 3 + 10;
		if (v > 63)
			v = 63;
		pals[2][i * 3] = v;
		pals[2][i * 3 + 1] = v;
		v += v >> 2;
		if (v > 63)
			v = 63;
		pals[2][i * 3 + 2] = v;
	}
}

unsigned char
scale(int v, int p1, int p2)
{
	v *= p2;
	v = v / 100 * p1 / 100;
	if (v < 0)
		v = 0;
	else if (v > 63)
		v = 63;
	return v;
}

int
tint(int n, int idx, int pct)
{
	int r, lim;

	if (pct == 100)
		return 0;
	int g = pals[n][idx * 3 + 1];
	r = pals[n][idx * 3];
	int b = pals[n][idx * 3 + 2];
	int sum = g + r + b;
	lim = palavg * 60 / 100;
	/* d is clamped and never used: a defect of the original */
	if (sum < lim) {
		int d = lim - sum;
		if (d < 0)
			d = 0;
		else if (d > 99)
			d = 99;
		if (pct < 100)
			pct = 100 - pct + 100;
		else
			pct = 200 - pct;
	} else if (sum > lim) {
		int d = sum - lim;
		if (d < 0)
			d = 0;
		else if (d > 99)
			d = 99;
	} else
		return 0;
	if (pct < 1)
		pct = 1;
	r *= pct;
	r /= 100;
	if (r < 0)
		r = 0;
	else if (r > 63)
		r = 63;
	g *= pct;
	g /= 100;
	if (g < 0)
		g = 0;
	else if (g > 63)
		g = 63;
	b *= pct;
	b /= 100;
	if (b < 0)
		b = 0;
	else if (b > 63)
		b = 63;
	pals[n][idx * 3] = r;
	pals[n][idx * 3 + 1] = g;
	pals[n][idx * 3 + 2] = b;
	return 1;
}

void
adjpal(int bright, int contrast, int r, int g, int b)
{
	int i;

	if (bright < 40)
		bright = 40;
	else if (bright > 300)
		bright = 300;
	if (contrast < 0)
		contrast = 0;
	else if (contrast > 200)
		contrast = 200;
	for (i = 0; i < 256; i++) {
		pals[0][i * 3] = scale(defpal[i * 3], bright, r);
		pals[0][i * 3 + 1] = scale(defpal[i * 3 + 1], bright, g);
		pals[0][i * 3 + 2] = scale(defpal[i * 3 + 2], bright, b);
		tint(0, i, contrast);
	}
	memcpy(pals[9], pals[0], 0x300);
	makepals();
	usepal(0);
}

void
freesurfs(void)
{
	if (prim) {
		// PORT: DirectDraw through its C binding
#ifdef PORT
		IDirectDrawSurface_Release(prim);
#else
		prim->Release();
#endif
		prim = NULL;
	}
	if (surf2) {
		// PORT: DirectDraw through its C binding
#ifdef PORT
		IDirectDrawSurface_Release(surf2);
#else
		surf2->Release();
#endif
		surf2 = NULL;
	}
	if (surfc) {
		// PORT: DirectDraw through its C binding
#ifdef PORT
		IDirectDrawSurface_Release(surfc);
#else
		surfc->Release();
#endif
		surfc = NULL;
	}
	for (fsi = 0; fsi < 7; fsi++)
		if (surfm[fsi]) {
			// PORT: DirectDraw through its C binding
#ifdef PORT
			IDirectDrawSurface_Release(surfm[fsi]);
#else
			surfm[fsi]->Release();
#endif
			surfm[fsi] = NULL;
		}
	for (fsi = 0; fsi < 13; fsi++)
		if (surfo[fsi]) {
			// PORT: DirectDraw through its C binding
#ifdef PORT
			IDirectDrawSurface_Release(surfo[fsi]);
#else
			surfo[fsi]->Release();
#endif
			surfo[fsi] = NULL;
		}
	for (fsi = 0; fsi < 5; fsi++)
		if (surfw[fsi]) {
			// PORT: DirectDraw through its C binding
#ifdef PORT
			IDirectDrawSurface_Release(surfw[fsi]);
#else
			surfw[fsi]->Release();
#endif
			surfw[fsi] = NULL;
		}
	if (surfpg) {
		// PORT: DirectDraw through its C binding
#ifdef PORT
		IDirectDrawSurface_Release(surfpg);
#else
		surfpg->Release();
#endif
		surfpg = NULL;
	}
	if (ddpal) {
		// PORT: DirectDraw through its C binding
#ifdef PORT
		IDirectDrawPalette_Release(ddpal);
#else
		ddpal->Release();
#endif
		ddpal = NULL;
	}
	if (textfont) {
		DeleteObject(textfont);
		textfont = NULL;
	}
	ddready = 0;
}

int
putsurf(unsigned char *src, LPDIRECTDRAWSURFACE s, int x, int y, int w, int h)
{
	memset(&ddsd, 0, sizeof ddsd);
	ddsd.dwSize = sizeof ddsd;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	if (IDirectDrawSurface_Lock(s, NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
#else
	if (s->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
#endif
		return 0;
	psdst = (unsigned char *)ddsd.lpSurface + x + y * ddsd.lPitch;
	pssrc = src;
	for (psrow = 0; psrow < h; psrow++) {
		memcpy(psdst, pssrc, w);
		psdst += ddsd.lPitch;
		pssrc += w;
	}
	for (;;) {
		// PORT: DirectDraw through its C binding
#ifdef PORT
		psres = IDirectDrawSurface_Unlock(s, NULL);
#else
		psres = s->Unlock(NULL);
#endif
		if (psres == DD_OK)
			return 1;
		if (psres != DDERR_SURFACELOST)
			return 0;
		// PORT: DirectDraw through its C binding
#ifdef PORT
		if (IDirectDrawSurface_Restore(s) != DD_OK)
#else
		if (s->Restore() != DD_OK)
#endif
			return 0;
	}
}

int
restoresurfs(void)
{
	if (!appactive)
		return 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	if (ddfailed("sfs_r.DDS P.Rst", IDirectDrawSurface_Restore(prim)))
#else
	if (ddfailed("sfs_r.DDS P.Rst", prim->Restore()))
#endif
		goto fail;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	if (surf2 && ddfailed("sfs_r.DDS S2.Rst", IDirectDrawSurface_Restore(surf2)))
#else
	if (surf2 && ddfailed("sfs_r.DDS S2.Rst", surf2->Restore()))
#endif
		goto fail;
	surfslost = 1;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	IDirectDrawSurface_Blt(back, NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx);
	IDirectDrawSurface_Blt(prim, NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx);
	IDirectDrawSurface_Blt(surf2, NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx);
	if (surfc && ddfailed("sfs_r.DDS SC.Rst", IDirectDrawSurface_Restore(surfc)))
#else
	back->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx);
	prim->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx);
	surf2->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx);
	if (surfc && ddfailed("sfs_r.DDS SC.Rst", surfc->Restore()))
#endif
		goto fail;
	for (rsfi = 0; rsfi < 7; rsfi++)
		// PORT: DirectDraw through its C binding
#ifdef PORT
		if (surfm[rsfi] && ddfailed("sfs_r.DDS SM.Rst", IDirectDrawSurface_Restore(surfm[rsfi])))
#else
		if (surfm[rsfi] && ddfailed("sfs_r.DDS SM.Rst", surfm[rsfi]->Restore()))
#endif
			goto fail;
	for (rsfi = 0; rsfi < 13; rsfi++)
		// PORT: DirectDraw through its C binding
#ifdef PORT
		if (surfo[rsfi] && ddfailed("sfs_r.DDS SO.Rst", IDirectDrawSurface_Restore(surfo[rsfi])))
#else
		if (surfo[rsfi] && ddfailed("sfs_r.DDS SO.Rst", surfo[rsfi]->Restore()))
#endif
			goto fail;
	for (rsfi = 0; rsfi < 5; rsfi++)
		// PORT: DirectDraw through its C binding
#ifdef PORT
		if (surfw[rsfi] && ddfailed("sfs_r.DDS W.Rst", IDirectDrawSurface_Restore(surfw[rsfi])))
#else
		if (surfw[rsfi] && ddfailed("sfs_r.DDS W.Rst", surfw[rsfi]->Restore()))
#endif
			goto fail;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	if (surfpg && ddfailed("sfs_r.DDS Pg.Rst", IDirectDrawSurface_Restore(surfpg)))
#else
	if (surfpg && ddfailed("sfs_r.DDS Pg.Rst", surfpg->Restore()))
#endif
		goto fail;
	savesurfs();
	loadsurfs();
	return 1;
fail:
	savesurfs();
	return 0;
}

int
lock(LPDIRECTDRAWSURFACE s)
{
	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("SfLck", IDirectDrawSurface_Lock(s, NULL, &sd, DDLOCK_WAIT, NULL)))
#else
	while (ddfailed("SfLck", s->Lock(NULL, &sd, DDLOCK_WAIT, NULL)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return 0;
	return 1;
}

int
unlock(LPDIRECTDRAWSURFACE s)
{
	unlocktries = 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("SfUlck", IDirectDrawSurface_Unlock(s, NULL))) {
#else
	while (ddfailed("SfUlck", s->Unlock(NULL))) {
#endif
		if (ddlost && restoresurfs())
			continue;
		if (unlocktries++ >= 5)
			return 0;
	}
	return 1;
}

void
putrect(unsigned char *src, int x, int y, int w, int h)
{
	prdst = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	prsrc = src;
	for (prrow = 0; prrow < h; prrow++) {
		memcpy(prdst, prsrc, w);
		prdst += sd.lPitch;
		prsrc += w;
	}
}

void
putrectt(unsigned char *src, int x, int y, int w, int h)
{
	ptdst = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	ptsrc = src;
	for (pty = 0; pty < h; pty++) {
		for (ptx = 0; ptx < w; ptx++) {
			ptpix = *ptsrc++;
			if (ptpix)
				ptdst[ptx] = ptpix;
		}
		ptdst += sd.lPitch;
	}
}

void
getrect(int x, int y, int w, int h, unsigned char *dst)
{
	grdst = dst;
	grsrc = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (grrow = 0; grrow < h; grrow++) {
		memcpy(grdst, grsrc, w);
		grdst += w;
		grsrc += sd.lPitch;
	}
}

int
getdc(LPDIRECTDRAWSURFACE s)
{
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("Sf_gDC", IDirectDrawSurface_GetDC(s, &surfdc))) {
#else
	while (ddfailed("Sf_gDC", s->GetDC(&surfdc))) {
#endif
		if (ddlost && restoresurfs())
			continue;
		if (!ddbusy)
			return 0;
	}
	return 1;
}

int
releasedc(LPDIRECTDRAWSURFACE s)
{
	reldctries = 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("Sf_rDC", IDirectDrawSurface_ReleaseDC(s, surfdc))) {
#else
	while (ddfailed("Sf_rDC", s->ReleaseDC(surfdc))) {
#endif
		if (ddlost && restoresurfs())
			continue;
		if (reldctries++ >= 5)
			return 0;
	}
	return 1;
}

// PORT: whether the next flip shows a play frame, in which the pointer roams the wide screen
#ifdef PORT
int playflip;
#endif

// FIXED: as in flip
#ifdef FEATURE_PACE
static DWORD lastflip;
#endif

int
flip(void)
{
	// PORT: a play frame's flip lets the pointer roam, and any other flip ends that
#ifdef PORT
	int roamed;

	roamed = sysroam;
	syssetroam(playflip);
	playflip = 0;
#endif
	// FIXED: a page turn and a talk's slide have only Flip's wait for the refresh to pace them, and Flip returns at once under Wine. Keep 15 ms between flips, under a frame of 60 Hz, which leaves the flips of a display of up to 66 Hz as they are.
#ifdef FEATURE_PACE
	while (ticks() - lastflip < 15)
		Sleep(1);
	lastflip = ticks();
#endif
	for (;;) {
		// PORT: DirectDraw through its C binding
#ifdef PORT
		if (!ddfailed("w_s_s.flp", IDirectDrawSurface_Flip(prim, NULL, DDFLIP_WAIT)))
#else
		if (!ddfailed("w_s_s.flp", prim->Flip(NULL, DDFLIP_WAIT)))
#endif
			break;
		if (!ddlost || !restoresurfs())
			return 0;
	}
	if (shown) {
		if (spriteshape == 0)
			spriteshape = 1;
		else if (spriteshape == 1)
			spriteshape = 0;
		lastx = 999;
		drawcursor();
	}
	// PORT: drawcursor can restore a square in a wing of the back buffer, which the next page would show
#ifdef PORT
	if (roamed && !sysroam)
		drawclearback();
#endif
	return 1;
}

void
scroll(int x, int y, int dx, int dy, int w, int h)
{
	rc.left = x;
	rc.top = y;
	rc.right = x + w;
	rc.bottom = y + h;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("sb2s_ss.bltf", IDirectDrawSurface_BltFast(back, dx, dy, back, &rc, DDBLTFAST_WAIT)) && ddlost && restoresurfs())
#else
	while (ddfailed("sb2s_ss.bltf", back->BltFast(dx, dy, back, &rc, DDBLTFAST_WAIT)) && ddlost && restoresurfs())
#endif
		;
}

void
blitfull(LPDIRECTDRAWSURFACE src, LPDIRECTDRAWSURFACE dst)
{
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("sb2s_ss.bltf", IDirectDrawSurface_BltFast(dst, 0, 0, src, &fullrect, DDBLTFAST_WAIT)) && ddlost && restoresurfs())
#else
	while (ddfailed("sb2s_ss.bltf", dst->BltFast(0, 0, src, &fullrect, DDBLTFAST_WAIT)) && ddlost && restoresurfs())
#endif
		;
}

void
blit(LPDIRECTDRAWSURFACE src, int x, int y, LPDIRECTDRAWSURFACE dst, int dx, int dy, int w, int h)
{
	rc.left = x;
	rc.top = y;
	rc.right = x + w;
	rc.bottom = y + h;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("css2ssbxyw.bltf", IDirectDrawSurface_BltFast(dst, dx, dy, src, &rc, DDBLTFAST_WAIT)) && ddlost && restoresurfs())
#else
	while (ddfailed("css2ssbxyw.bltf", dst->BltFast(dx, dy, src, &rc, DDBLTFAST_WAIT)) && ddlost && restoresurfs())
#endif
		;
}

void
blitkey(LPDIRECTDRAWSURFACE src, int x, int y, LPDIRECTDRAWSURFACE dst, int dx, int dy, int w, int h)
{
	rc.left = x;
	rc.top = y;
	rc.right = x + w;
	rc.bottom = y + h;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("css2ssbxywn0.bltf", IDirectDrawSurface_BltFast(dst, dx, dy, src, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)) && ddlost && restoresurfs())
#else
	while (ddfailed("css2ssbxywn0.bltf", dst->BltFast(dx, dy, src, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)) && ddlost && restoresurfs())
#endif
		;
}

void
blitmirror(LPDIRECTDRAWSURFACE src, int x, int y, LPDIRECTDRAWSURFACE dst, int dx, int dy, int w, int h)
{
	memset(&mirrorfx, 0, sizeof mirrorfx);
	rc.left = x;
	rc.right = x + w;
	rc.top = y;
	rc.bottom = y + h;
	mirrorfx.dwSize = sizeof mirrorfx;
	mirrorfx.dwDDFX = DDBLTFX_MIRRORUPDOWN;
	rc2.left = dx;
	rc2.right = dx + w;
	rc2.top = dy;
	rc2.bottom = dy + h;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("css2ssbxywfl.blt", IDirectDrawSurface_Blt(dst, &rc2, src, &rc, DDBLT_WAIT, &mirrorfx)) && ddlost && restoresurfs())
#else
	while (ddfailed("css2ssbxywfl.blt", dst->Blt(&rc2, src, &rc, DDBLT_WAIT, &mirrorfx)) && ddlost && restoresurfs())
#endif
		;
}

void
blitsurfs(int a, int b)
{
	blitfull(surfs[a], surfs[b]);
}

void
blitn(int a, int x, int y, int b, int dx, int dy, int w, int h)
{
	blit(surfs[a], x, y, surfs[b], dx, dy, w, h);
}

void
backto2(void)
{
	blitfull(back, surf2);
}

void
primtoback(void)
{
	blitfull(prim, back);
}

void
s2toback(void)
{
	blitfull(surf2, back);
}

void
surftoback(int n)
{
	selsurf(n);
	blitfull(cursurf, back);
}

void
s2tobackrows(int y, int dy, int h)
{
	rc.top = y;
	rc.left = 0;
	rc.right = 640;
	rc.bottom = y + h;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("ss2xz2sa.bltf", IDirectDrawSurface_BltFast(back, 0, dy, surf2, &rc, DDBLTFAST_WAIT)) && ddlost && restoresurfs())
#else
	while (ddfailed("ss2xz2sa.bltf", back->BltFast(0, dy, surf2, &rc, DDBLTFAST_WAIT)) && ddlost && restoresurfs())
#endif
		;
}

void
backto2rows(int y, int dy, int h)
{
	rc.top = y;
	rc.left = 0;
	rc.right = 640;
	rc.bottom = y + h;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("ss2xz2sa.bltf", IDirectDrawSurface_BltFast(surf2, 0, dy, back, &rc, DDBLTFAST_WAIT)) && ddlost && restoresurfs())
#else
	while (ddfailed("ss2xz2sa.bltf", surf2->BltFast(0, dy, back, &rc, DDBLTFAST_WAIT)) && ddlost && restoresurfs())
#endif
		;
}

void
primto2(void)
{
	blitfull(prim, surf2);
}

void
s2toprim(void)
{
	blitfull(surf2, prim);
}

void
clearscr(void)
{
	memset(scrbuf, 0, sizeof scrbuf);
}

int
putscreen(char *src, int n)
{
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	pscrsrc = src;
	pscrdst = (char *)sd.lpSurface;
	for (pscrrow = 0; pscrrow < 480; pscrrow++) {
		memcpy(pscrdst, pscrsrc, 640);
		pscrdst += sd.lPitch;
		pscrsrc += 640;
	}
	if (!unlock(cursurf))
		return 0;
	return 1;
}

int
getscreen(int n, char *dst)
{
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	gscrdst = dst;
	gscrsrc = (char *)sd.lpSurface;
	for (gscrrow = 0; gscrrow < 480; gscrrow++) {
		memcpy(gscrdst, gscrsrc, 640);
		gscrsrc += sd.lPitch;
		gscrdst += 640;
	}
	if (!unlock(cursurf))
		return 0;
	return 1;
}

int
putrows(unsigned char *buf, int y, int h)
{
	int ok;

	if (lock(back)) {
		prowdst = (unsigned char *)sd.lpSurface + y * sd.lPitch;
		prowsrc = buf + y * 640;
		for (prowi = 0; prowi < h; prowi++) {
			memcpy(prowdst, prowsrc, 640);
			prowdst += sd.lPitch;
			prowsrc += 640;
		}
		if (unlock(back))
			ok = 1;
		else
			ok = 0;
	} else
		ok = 0;
	return ok;
}

int
putblock(int n, int x, int y, unsigned char *buf, int bx, int by, int w, int h)
{
	int ok;

	selsurf(n);
	if (lock(cursurf)) {
		pblkdst = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
		pblksrc = buf + bx + by * 640;
		for (pblkrow = 0; pblkrow < h; pblkrow++) {
			memcpy(pblkdst, pblksrc, w);
			pblkdst += sd.lPitch;
			pblksrc += 640;
		}
		if (unlock(cursurf))
			ok = 1;
		else
			ok = 0;
	} else
		ok = 0;
	return ok;
}

int
putblockt(int n, int x, int y, unsigned char *buf, int bx, int by, int w, int h)
{
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	pbtdst = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	pbtsrc = buf + bx + by * 640;
	for (pbty = 0; pbty < h; pbty++) {
		for (pbtx = 0; pbtx < w; pbtx++)
			if (pbtsrc[pbtx])
				pbtdst[pbtx] = pbtsrc[pbtx];
		pbtsrc += 640;
		pbtdst += sd.lPitch;
	}
	if (!unlock(cursurf))
		return 0;
	return 1;
}

void
copydisc(unsigned char *dst, unsigned char *src, int cx, int cy, int r)
{
	cdr2 = r * r;
	for (cdy = 0; cdy <= r; cdy++) {
		cdy2 = cdy * cdy;
		for (cdx = 0; cdx <= r; cdx++) {
			cdx2 = cdx * cdx;
			if (cdx2 + cdy2 >= cdr2)
				break;
			cdoff = pixoff(cx + cdx, cy - cdy);
			if (cdoff >= 0)
				dst[cdoff] = src[cdoff];
			cdoff = pixoff(cx + cdx, cy + cdy);
			if (cdoff >= 0)
				dst[cdoff] = src[cdoff];
			cdoff = pixoff(cx - cdx, cy + cdy);
			if (cdoff >= 0)
				dst[cdoff] = src[cdoff];
			cdoff = pixoff(cx - cdx, cy - cdy);
			if (cdoff >= 0)
				dst[cdoff] = src[cdoff];
		}
	}
}

int
showscr(void)
{
	return putscreen(scrbuf, 0);
}

int
showscrt(void)
{
	if (!lock(back))
		return 0;
	sstdst = (char *)sd.lpSurface;
	sstsrc = scrbuf;
	for (ssty = 0; ssty < 480; ssty++) {
		for (sstx = 640; sstx--; )
			if (*sstsrc != '\0')
				*sstdst++ = *sstsrc++;
			else {
				sstdst++;
				sstsrc++;
			}
		sstdst += sd.lPitch - 640;
	}
	if (!unlock(back))
		return 0;
	return 1;
}

int
showprim(void)
{
	if (!lock(prim))
		return 0;
	spdst = (unsigned char *)sd.lpSurface;
	spsrc = scrbuf;
	for (sprow = 0; sprow < 480; sprow++) {
		memcpy(spdst, spsrc, 640);
		spdst += sd.lPitch;
		spsrc += 640;
	}
	if (!unlock(prim))
		return 0;
	return 1;
}

int
showprimt(void)
{
	if (!lock(prim))
		return 0;
	sptdst = (char *)sd.lpSurface;
	sptsrc = scrbuf;
	for (spty = 0; spty < 480; spty++) {
		for (sptx = 640; sptx--; )
			if (*sptsrc != '\0')
				*sptdst++ = *sptsrc++;
			else {
				sptdst++;
				sptsrc++;
			}
		sptdst += sd.lPitch - 640;
	}
	if (!unlock(prim))
		return 0;
	return 1;
}

unsigned char
getpixel(unsigned char *buf, int x, int y)
{
	return buf[y * 640 + x];
}

int
grab48(int x, int y, unsigned char *dst)
{
	g48any = 0;
	for (g48y = 0; g48y < 48; g48y++)
		for (g48x = 0; g48x < 48; g48x++) {
			*dst = scrbuf[x + g48x + (y + g48y) * 640];
			if (*dst)
				g48any = 1;
			dst++;
		}
	return g48any;
}

int
save2(void)
{
	return getscreen(2, scrbuf);
}

int
put2(char *buf)
{
	return putscreen(buf, 2);
}

int
put0(char *buf)
{
	return putscreen(buf, 0);
}

int
save1(void)
{
	return getscreen(1, scrbuf);
}

int
save0(void)
{
	return getscreen(0, scrbuf);
}

int
put48t(int n, int x, int y, char *src)
{
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	p48tdst = (char *)sd.lpSurface + x + y * sd.lPitch;
	for (p48ty = 0; p48ty < 48; p48ty++) {
		for (p48tx = 48; p48tx--; )
			if (*src != '\0')
				*p48tdst++ = *src++;
			else {
				p48tdst++;
				src++;
			}
		p48tdst += sd.lPitch - 48;
	}
	if (!unlock(cursurf))
		return 0;
	return 1;
}

int
put48t0(int x, int y, char *src)
{
	return put48t(0, x, y, src);
}

int
put48(int n, int x, int y, unsigned char *src)
{
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	p48dst = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (p48row = 0; p48row < 48; p48row++) {
		memcpy(p48dst, src, 48);
		p48dst += sd.lPitch;
		src += 48;
	}
	if (!unlock(cursurf))
		return 0;
	return 1;
}

int
put48s(LPDIRECTDRAWSURFACE s, int x, int y, unsigned char *src)
{
	if (!lock(s))
		return 0;
	p48sdst = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (p48srow = 0; p48srow < 48; p48srow++) {
		memcpy(p48sdst, src, 48);
		p48sdst += sd.lPitch;
		src += 48;
	}
	if (!unlock(s))
		return 0;
	return 1;
}

int
put480(int x, int y, unsigned char *src)
{
	return put48(0, x, y, src);
}

int
put481(int x, int y, unsigned char *src)
{
	return put48(1, x, y, src);
}

int
put48pt(int n, int x, int y, char *src, int sx, int sy, int w, int h)
{
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	p48ptdst = (char *)sd.lpSurface + x + y * sd.lPitch;
	src += sx + sy * 48;
	for (p48pty = 0; p48pty < h; p48pty++) {
		for (p48ptx = 0; p48ptx < w; p48ptx++)
			if (*src)
				*p48ptdst++ = *src++;
			else {
				p48ptdst++;
				src++;
			}
		p48ptdst += sd.lPitch - w;
		src += 48 - w;
	}
	if (!unlock(cursurf))
		return 0;
	return 1;
}

int
get48(unsigned char *dst, int n, int x, int y)
{
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	g48src = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (g48row = 0; g48row < 48; g48row++) {
		memcpy(dst, g48src, 48);
		dst += 48;
		g48src += sd.lPitch;
	}
	if (!unlock(cursurf))
		return 0;
	return 1;
}

int
get48s(unsigned char *dst, LPDIRECTDRAWSURFACE s, int x, int y)
{
	if (!lock(s))
		return 0;
	g48ssrc = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (g48srow = 0; g48srow < 48; g48srow++) {
		memcpy(dst, g48ssrc, 48);
		dst += 48;
		g48ssrc += sd.lPitch;
	}
	if (!unlock(s))
		return 0;
	return 1;
}

int
putwh(unsigned char *src, int n, int x, int y, int w, int h)
{
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	pwhdst = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (pwhrow = 0; pwhrow < h; pwhrow++) {
		memcpy(pwhdst, src, w);
		src += w;
		pwhdst += sd.lPitch;
	}
	if (!unlock(cursurf))
		return 0;
	return 1;
}

int
putclip(char *src, int n, int x, int y, int w, int h)
{
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	pcldst = (char *)sd.lpSurface + x + y * sd.lPitch;
	pcly = 0;
	if (y < 0) {
		pcly = y;
		pcly = -pcly;
		src += pcly * w;
		pcldst += pcly * w;
	}
	for (; pcly < h; pcly++) {
		pclx = 0;
		if (x < 8) {
			pclx = 8 - x;
			src += 8 - x;
			pcldst += 8 - x;
		}
		for (; pclx < w; pclx++)
			if (*src)
				*pcldst++ = *src++;
			else {
				pcldst++;
				src++;
			}
		pcldst += sd.lPitch - w;
	}
	if (!unlock(cursurf))
		return 0;
	return 1;
}

int
putsub(char *src, int sx, int sy, int stride, int on, int n, int x, int y, int w, int h)
{
	if (!on)
		return 0;
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	psubdst = (char *)sd.lpSurface + x + y * sd.lPitch;
	src += sx + sy * stride;
	for (psuby = 0; psuby < h; psuby++) {
		for (psubx = 0; psubx < w; psubx++)
			if (*src)
				*psubdst++ = *src++;
			else {
				psubdst++;
				src++;
			}
		psubdst += sd.lPitch - w;
		src += stride - w;
	}
	if (!unlock(cursurf))
		return 0;
	return 1;
}

int
getwh(int n, int x, int y, unsigned char *dst, int w, int h)
{
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	gwhsrc = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (gwhrow = 0; gwhrow < h; gwhrow++) {
		memcpy(dst, gwhsrc, w);
		dst += w;
		gwhsrc += sd.lPitch;
	}
	if (!unlock(cursurf))
		return 0;
	return 1;
}

int
getwhs(LPDIRECTDRAWSURFACE s, int x, int y, unsigned char *dst, int w, int h)
{
	if (!lock(s))
		return 0;
	gwhssrc = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (gwhsrow = 0; gwhsrow < h; gwhsrow++) {
		memcpy(dst, gwhssrc, w);
		dst += w;
		gwhssrc += sd.lPitch;
	}
	if (!unlock(s))
		return 0;
	return 1;
}

int
getbuf(unsigned char *buf, int x, int y, unsigned char *dst, int w, int h)
{
	gbsrc = buf + x + y * 640;
	for (gbrow = 0; gbrow < h; gbrow++) {
		memcpy(dst, gbsrc, w);
		dst += w;
		gbsrc += 640;
	}
	return 1;
}

void
clearsurf(int n, unsigned char color)
{
	selsurf(n);
	fillfx.dwFillColor = color;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("clss.blt", IDirectDrawSurface_Blt(cursurf, NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#else
	while (ddfailed("clss.blt", cursurf->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#endif
		;
}

void
clrprim(void)
{
	if (!appactive)
		return;
	fillfx.dwFillColor = 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("clss.blt", IDirectDrawSurface_Blt(prim, NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#else
	while (ddfailed("clss.blt", prim->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#endif
		;
}

void
clrback(void)
{
	if (!appactive)
		return;
	fillfx.dwFillColor = 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("cls.blt", IDirectDrawSurface_Blt(back, NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#else
	while (ddfailed("cls.blt", back->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#endif
		;
}

void
clrboth(void)
{
	if (!appactive)
		return;
	clrprim();
	clrback();
}

void
fillrect(int n, int l, int t, int r, int b, unsigned char color)
{
	selsurf(n);
	rc2.left = l;
	rc2.top = t;
	rc2.right = r;
	rc2.bottom = b;
	fillfx.dwFillColor = color;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("dr_sld_r.blt", IDirectDrawSurface_Blt(cursurf, &rc2, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#else
	while (ddfailed("dr_sld_r.blt", cursurf->Blt(&rc2, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#endif
		;
}

void
clrframe(void)
{
	// PORT: the frame's black border reaches the wide screen's edges
#ifdef PORT
	frameleft.left = sysscreen.left;
	frameright.right = sysscreen.right;
#endif
	fillfx.dwFillColor = 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("pscrr.blt.l", IDirectDrawSurface_Blt(back, &frameleft, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#else
	while (ddfailed("pscrr.blt.l", back->Blt(&frameleft, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#endif
		;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("pscrr.blt.l", IDirectDrawSurface_Blt(back, &frameright, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#else
	while (ddfailed("pscrr.blt.l", back->Blt(&frameright, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#endif
		;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("pscrr.blt.l", IDirectDrawSurface_Blt(back, &frametop, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#else
	while (ddfailed("pscrr.blt.l", back->Blt(&frametop, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#endif
		;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("pscrr.blt.l", IDirectDrawSurface_Blt(back, &framebottom, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#else
	while (ddfailed("pscrr.blt.l", back->Blt(&framebottom, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#endif
		;
}

void
clrsides(void)
{
	// PORT: the map of the wide view covers these strips
#ifdef PORT
	if (sysscreen.left < 0)
		return;
#endif
	fillfx.dwFillColor = 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("pscrr.blt.l", IDirectDrawSurface_Blt(back, &sideleft, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#else
	while (ddfailed("pscrr.blt.l", back->Blt(&sideleft, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#endif
		;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("pscrr.blt.r", IDirectDrawSurface_Blt(back, &sideright, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#else
	while (ddfailed("pscrr.blt.r", back->Blt(&sideright, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)) && ddlost && restoresurfs())
#endif
		;
}

int
textf(int n, int x, int y, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(printbuf, fmt, ap);
	va_end(ap);
	if (printbuf[0] == 0)
		return 0;
	textflen = lstrlen(printbuf);
	selsurf(n);
	if (!getdc(cursurf))
		return 0;
	SelectObject(surfdc, textfont);
	SetTextColor(surfdc, 0xffffff);
	SetBkMode(surfdc, TRANSPARENT);
	TextOut(surfdc, x, y, printbuf, textflen);
	if (!releasedc(cursurf))
		return 0;
	return 1;
}

int
shade(int x, int y, int w, int h)
{
	if (drawbuf) {
		shp = drawbuf + x + y * 640;
		for (shy = 0; shy < h; shy++) {
			for (shx = 0; shx < w; shx++) {
				*shp = darker2[*shp];
				shp++;
			}
			shp += 640 - w;
		}
		return 1;
	}
	if (!lock(back))
		return 0;
	shp = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (shy = 0; shy < h; shy++) {
		for (shx = 0; shx < w; shx++) {
			*shp = darker2[*shp];
			shp++;
		}
		shp += sd.lPitch - w;
	}
	if (!unlock(back))
		return 0;
	return 1;
}

int
fillbox(int x, int y, int w, int h, unsigned char color)
{
	if (drawbuf) {
		fbp = drawbuf + x + y * 640;
		for (fby = 0; fby < h; fby++) {
			for (fbx = 0; fbx < w; fbx++)
				*fbp++ = color;
			fbp += 640 - w;
		}
		return 1;
	}
	if (!lock(back))
		return 0;
	fbp = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (fby = 0; fby < h; fby++) {
		for (fbx = 0; fbx < w; fbx++)
			*fbp++ = color;
		fbp += sd.lPitch - w;
	}
	if (!unlock(back))
		return 0;
	return 1;
}

int
shade2(int x, int y, int w, int h)
{
	if (drawbuf) {
		sh2p = drawbuf + x + y * 640;
		for (sh2y = 0; sh2y < h; sh2y++) {
			for (sh2x = 0; sh2x < w; sh2x++) {
				*sh2p = lighter2[*sh2p];
				sh2p++;
			}
			sh2p += 640 - w;
		}
		return 1;
	}
	if (!lock(back))
		return 0;
	sh2p = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (sh2y = 0; sh2y < h; sh2y++) {
		for (sh2x = 0; sh2x < w; sh2x++) {
			*sh2p = lighter2[*sh2p];
			sh2p++;
		}
		sh2p += sd.lPitch - w;
	}
	if (!unlock(back))
		return 0;
	return 1;
}

int
lumadd(int x, int y, int w, int h, unsigned char add)
{
	if (drawbuf) {
		lap = drawbuf + x + y * 640;
		for (lay = 0; lay < h; lay++) {
			for (lax = 0; lax < w; lax++) {
				if (*lap)
					*lap = lum[*lap] + add;
				lap++;
			}
			lap += 640 - w;
		}
		return 1;
	}
	if (!lock(back))
		return 0;
	lap = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (lay = 0; lay < h; lay++) {
		for (lax = 0; lax < w; lax++) {
			if (*lap)
				*lap = lum[*lap] + add;
			lap++;
		}
		lap += sd.lPitch - w;
	}
	if (!unlock(back))
		return 0;
	return 1;
}

void
putpixel(int n, int x, int y, unsigned char c)
{
	selsurf(n);
	if (lock(cursurf)) {
		*((unsigned char *)sd.lpSurface + x + y * sd.lPitch) = c;
		unlock(cursurf);
	}
}

void
putpix2(int n, int x, int y, unsigned char c)
{
	selsurf(n);
	if (lock(cursurf)) {
		*((unsigned char *)sd.lpSurface + x + y * sd.lPitch) = c;
		*((unsigned char *)sd.lpSurface + x + 1 + y * sd.lPitch) = c;
		*((unsigned char *)sd.lpSurface + x + (y + 1) * sd.lPitch) = c;
		*((unsigned char *)sd.lpSurface + x + 1 + (y + 1) * sd.lPitch) = c;
		unlock(cursurf);
	}
}

unsigned char
getpix(int n, int x, int y)
{
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	gppix = *((unsigned char *)sd.lpSurface + x + y * sd.lPitch);
	unlock(cursurf);
	return gppix;
}

unsigned char
getpixs(LPDIRECTDRAWSURFACE s, int x, int y)
{
	if (!lock(s))
		return 0;
	gpspix = *((unsigned char *)sd.lpSurface + x + y * sd.lPitch);
	unlock(s);
	return gpspix;
}

void
line(int x1, int y1, int x2, int y2, unsigned char color)
{
	dlerrx = 0;
	dldx = x2 - x1;
	dlerry = 0;
	dldy = y2 - y1;
	if (!lock(back))
		return;
	if (dldx > 0)
		dlsx = 1;
	else if (dldx == 0)
		dlsx = 0;
	else {
		dldx = -dldx;
		dlsx = -1;
	}
	if (dldy > 0)
		dlsy = sd.lPitch;
	else if (dldy == 0)
		dlsy = 0;
	else {
		dldy = -dldy;
		dlsy = sd.lPitch;
		dlsy = -dlsy;
	}
	if (dldx > dldy)
		dllen = dldx;
	else
		dllen = dldy;
	dlp = (unsigned char *)sd.lpSurface + x1 + y1 * sd.lPitch;
	for (dli = 0; dli <= dllen + 1; dli++) {
		*dlp = color;
		dlerrx += dldx;
		dlerry += dldy;
		if (dlerrx > dllen) {
			dlerrx -= dllen;
			if (dlsx == -1) {
				x1--;
				dlp--;
			} else if (dlsx) {
				x1++;
				dlp++;
			}
		}
		if (dlerry > dllen) {
			dlerry -= dllen;
			dlp += dlsy;
		}
	}
	unlock(back);
}

void
circle(int cx, int cy, int r, unsigned char color)
{
	if (!lock(back))
		return;
	for (ciy = 0; ciy <= r; ciy++)
		for (cix = 0; cix <= r; cix++) {
			cir2 = cix * cix + ciy * ciy;
			if (cir2 < 0xb491 && (r == sqrttab[cir2] || sqrttab[cir2] + 1 == r)) {
				*((unsigned char *)sd.lpSurface + (cx + cix) + (cy + ciy) * sd.lPitch) = color;
				*((unsigned char *)sd.lpSurface + (cx + cix) + (cy - ciy) * sd.lPitch) = color;
				*((unsigned char *)sd.lpSurface + (cx - cix) + (cy + ciy) * sd.lPitch) = color;
				*((unsigned char *)sd.lpSurface + (cx - cix) + (cy - ciy) * sd.lPitch) = color;
			}
		}
	unlock(back);
}

void
linebegin(int x1, int y1, int x2, int y2)
{
	lserrx = 0;
	lserry = 0;
	lsdx = x2 - x1;
	lsdy = y2 - y1;
	if (lsdx > 0)
		lssx = 1;
	else if (lsdx == 0)
		lssx = 0;
	else {
		lsdx = -lsdx;
		lssx = -1;
	}
	if (lsdy > 0)
		lssy = 1;
	else if (lsdy == 0)
		lssy = 0;
	else {
		lsdy = -lsdy;
		lssy = -1;
	}
	if (lsdx > lsdy)
		lsmax = linesteps = lsdx;
	else
		lsmax = linesteps = lsdy;
	linestepx = x1;
	linestepy = y1;
	linesteps++;
}

void
linestep(void)
{
	if (linesteps <= 0)
		return;
	linesteps--;
	lserrx += lsdx;
	lserry += lsdy;
	if (lserrx > lsmax) {
		lserrx -= lsmax;
		linestepx += lssx;
	}
	if (lserry > lsmax) {
		lserry -= lsmax;
		linestepy += lssy;
	}
}

void
putpix0(int x, int y, unsigned char c)
{
	putpixel(0, x, y, c);
}

void
putpix1(int x, int y, unsigned char c)
{
	putpixel(1, x, y, c);
}

void
putpixeli(int n, int x, int y, int c)
{
	putpixel(n, x, y, c);
}

void
putpix2i(int n, int x, int y, int c)
{
	putpix2(n, x, y, c);
}

void
putpixoff(unsigned off, unsigned char c)
{
	putpixel(0, off % 640, off / 640, c);
}

void
setscr(int x, int y, unsigned char c)
{
	scrbuf[x + y * 640] = c;
}

void
setscr2(int x, int y, unsigned char c)
{
	ss2p = (unsigned char *)scrbuf + x + y * 640;
	ss2p[0] = c;
	ss2p[1] = c;
	ss2p[640] = c;
	ss2p[641] = c;
}

void
setscr2o(int off, unsigned char c)
{
	ss2op = (unsigned char *)scrbuf + off;
	ss2op[0] = c;
	ss2op[1] = c;
	ss2op[640] = c;
	ss2op[641] = c;
}

void
setscr4(int off, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	ss4p = (unsigned char *)scrbuf + off;
	ss4p[0] = a;
	ss4p[1] = b;
	ss4p[640] = c;
	ss4p[641] = d;
}

void
setscroff(unsigned off, unsigned char c)
{
	scrbuf[off % 640 + off / 640 * 640] = c;
}

void
set48(unsigned char *buf, int x, int y, unsigned char c)
{
	buf[x + y * 48] = c;
}

void
set48x2(unsigned char *buf, int x, int y, unsigned char c)
{
	buf[x + y * 48] = c;
	buf[x + 1 + y * 48] = c;
	buf[x + (y + 1) * 48] = c;
	buf[x + 1 + (y + 1) * 48] = c;
}

void
rectline(int x, int y, int w, int h, unsigned char color)
{
	line(x, y, x + w - 1, y, color);
	line(x + w - 1, y, x + w - 1, y + h - 1, color);
	line(x + w - 1, y + h - 1, x, y + h - 1, color);
	line(x, y + h - 1, x, y, color);
}

void
putcol(int n, int x, int y, int h, unsigned char *src)
{
	selsurf(n);
	if (!lock(cursurf))
		return;
	pcolp = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	for (; h; h--) {
		*pcolp = *src++;
		pcolp += sd.lPitch;
	}
	unlock(cursurf);
}

void
zoom(int in)
{
	rc.left = 0;
	rc.right = 640;
	rc.top = 0;
	rc.bottom = 480;
	if (!in)
		zoomw = 320;
	else
		zoomw = 2;
	for (;;) {
		zoomh = zoomw * 480 / 640;
		zoomtime = ticks();
		clrback();
		rc2.left = 320 - zoomw;
		if (rc2.left < 0)
			rc2.left = 0;
		rc2.right = zoomw + 320;
		if (rc2.right > 640)
			rc2.right = 640;
		rc2.top = 240 - zoomh;
		if (rc2.top < 0)
			rc2.top = 0;
		rc2.bottom = zoomh + 240;
		if (rc2.bottom > 480)
			rc2.bottom = 480;
		// PORT: DirectDraw through its C binding
#ifdef PORT
		while (ddfailed("pspir.blt", IDirectDrawSurface_Blt(back, &rc2, surf2, &rc, DDBLT_WAIT, NULL)) && ddlost && restoresurfs())
#else
		while (ddfailed("pspir.blt", back->Blt(&rc2, surf2, &rc, DDBLT_WAIT, NULL)) && ddlost && restoresurfs())
#endif
			;
		flip();
		if (in) {
			zoomw += 4;
			if (zoomw >= 320)
				break;
		} else {
			zoomw -= 4;
			if (zoomw <= 2)
				break;
		}
		do
			pollinput();
		while (since(zoomtime) < 25);
	}
	if (in) {
		s2toback();
		flip();
	} else {
		clrback();
		flip();
	}
}

void
melt(void)
{
	for (meltcol = 0; meltcol < 80; meltcol++)
		colh[meltcol] = 0;
	for (meltcol = 0; meltcol < 160; meltcol++)
		colrnd[meltcol] = random();
	for (meltstep = 0; meltstep < 640; meltstep++) {
		melttime = ticks();
		meltrnd = randn(80);
		meltdone = 1;
		for (meltcol = 0; meltcol < 80; meltcol++) {
			if (colh[meltcol] < 480)
				meltdone = 0;
			if (colrnd[meltrnd + meltcol] >= 0x1999) {
				colh[meltcol] += 2;
				if (colh[meltcol] > 480)
					colh[meltcol] = 480;
				rc.left = meltcol * 8;
				rc.top = 0;
				rc.right = meltcol * 8 + 8;
				rc.bottom = colh[meltcol];
				if (rc.bottom > 0)
					// PORT: DirectDraw through its C binding
#ifdef PORT
					ddfailed("sa2ss", IDirectDrawSurface_BltFast(prim, meltcol * 8, 0, back, &rc, DDBLTFAST_WAIT));
#else
					ddfailed("sa2ss", prim->BltFast(meltcol * 8, 0, back, &rc, DDBLTFAST_WAIT));
#endif
			}
		}
		if (meltdone)
			break;
		do
			pollinput();
		while (since(melttime) < 6);
	}
	flip();
}

void
split(int x, int y, int w, int h)
{
	splithalf = h / 2;
	splitmid = y + splithalf;
	splitend = h + y;
	backto2();
	blit(surfs[1], x, y, surfs[2], 0, 0, w, h);
	for (splitrow = 1; splitrow <= splithalf; splitrow++) {
		splittime = ticks();
		blit(surfs[2], 0, 0, surfs[0], x, y, w, h);
		blitkey(surfs[2], x, splitend - splitrow, surfs[0], x, splitmid, w, splitrow);
		blitkey(surfs[2], x, y, surfs[0], x, splitmid - splitrow, w, splitrow);
		flip();
		do
			pollinput();
		while (since(splittime) < 1);
	}
}

void
blitto(int b, int dx, int dy, int a, int x, int y, int w, int h)
{
	blit(surfs[a], x, y, surfs[b], dx, dy, w, h);
}

void
dissolve(int n)
{
	for (dvi = 0; dvi < 0x3700; dvi++) {
		packbuf[dvi] = randn(n);
		packbuf[dvi]++;
	}
	for (dvi = 1; dvi < 24; dvi++)
		// FIXED: memcpy over overlapping bytes is undefined in C. Copy forward, as Watcom's memcpy did, which repeats the pattern.
#ifdef PORT
		fwdcopy(packbuf + dvi * 0x3200, packbuf + randn(640), 0x3200);
#else
		memcpy(packbuf + dvi * 0x3200, packbuf + randn(640), 0x3200);
#endif
	getscreen(0, packbuf + 0x4b000);
	for (dvstep = 0; dvstep < n + 1; dvstep++) {
		if (dvstep) {
			dvtime = ticks();
			do
				pollinput();
			while (since(dvtime) < 250 / (n + 1));
		}
		if (!lock(prim)) {
			putscreen(packbuf + 0x4b000, 1);
			return;
		}
		for (dvi = 0; dvi < 0x4b000; dvi++) {
			char c = packbuf[dvi];
			if (c > '\0') {
				if (c == '\1')
					*((char *)sd.lpSurface + dvi % 640 + dvi / 640 * sd.lPitch) = packbuf[0x4b000 + dvi];
				else
					packbuf[dvi]--;
			}
		}
		unlock(prim);
	}
}

/* dissolve with fewer steps the longer pixtime says drawing takes */
void
dissolvet(void)
{
	if (pixtime == -1)
		dvtsteps = 4;
	else if (pixtime >= 1100)
		dvtsteps = 2;
	else if (pixtime >= 750)
		dvtsteps = 3;
	else if (pixtime >= 500)
		dvtsteps = 4;
	else if (pixtime >= 300)
		dvtsteps = 5;
	else
		dvtsteps = 7;
	dvtsteps *= 2;
	dissolve(dvtsteps);
}

int
lzwread(void *buf, int n)
{
	if (lzwsrc + lzwsize <= lzwpos)
		return 0;
	if (n > lzwsrc + lzwsize - lzwpos)
		n = lzwsrc + lzwsize - lzwpos;
	memcpy(buf, lzwpos, n);
	lzwpos += n;
	return n;
}

void
lzwwrite(unsigned char *src, int n)
{
	if (lzwplanar) {
		while (n--) {
			lzwdst[(lzwoff >> 2) + (lzwoff & 3) * 16000] = *src++;
			lzwoff++;
		}
	} else {
		while (n--)
			lzwdst[lzwoff++] = *src++;
	}
}

int
lzwbyte(void)
{
	if (lzwread(&lzwch, 1) == 1)
		return lzwch;
	return -1;
}

/* lzwfill reads the next data block of the codes */
// PORT: inline as C99 has it, for this module alone
#ifdef PORT
static inline int
#else
inline int
#endif
lzwfill(void)
{
	lzwnavail = lzwbyte();
	if (lzwnavail < 1)
		return 0;
	lzwp = lzwblock;
	lzwend = lzwp + lzwread(lzwblock, lzwnavail);
	if (lzwblock + lzwnavail > lzwend)
		return 0;
	return 1;
}

/* decode the GIF LZW codes of lzwsrc into lzwdst, returning the count or -1 to -5 */
int
lzwdecode(int size)
{
	volatile int bits;

	lzwout = lzwbuf;
	lzwcount = 0;
	lzwend = lzwblock;
	lzwp = lzwblock;
	lzwnbits = 8;
	if (size < 2 || size > 8)
		return -5;
	lzwoc = -1;
	lzwfc = -1;
	lzwclear = 1 << size;
	bits = size + 1;
	lzwbits = bits;
	lzwslot = lzwclear;
	lzwtop = 1 << lzwbits;
	lzwslot += 2;
	for (;;) {
		if (lzwnbits == 8) {
			if (++lzwp >= lzwend && !lzwfill())
				return -4;
			lzwnbits = 0;
		}
		lzwcode = *lzwp;
		lzwc = lzwbits + lzwnbits;
		if (lzwc <= 8) {
			*lzwp >>= lzwbits;
			lzwnbits = lzwc;
		} else {
			if (++lzwp >= lzwend && !lzwfill())
				return -4;
			lzwx = *lzwp << (8 - lzwnbits);
			lzwcode |= lzwx;
			if (lzwc <= 16) {
				lzwnbits = lzwc - 8;
				*lzwp >>= lzwnbits;
			} else {
				if (++lzwp >= lzwend && !lzwfill())
					return -4;
				lzwx = *lzwp << (16 - lzwnbits);
				lzwcode |= lzwx;
				lzwnbits = lzwc - 16;
				*lzwp >>= lzwnbits;
			}
		}
		lzwcode &= lzwmask[lzwbits];
		lzwc = lzwcode;
		if (lzwcode == lzwclear + 1)
			break;
		if (lzwc > lzwslot)
			return -3;
		if (lzwc == lzwclear) {
			lzwbits = bits;
			lzwslot = lzwclear;
			lzwslot += 2;
			lzwtop = 1 << bits;
			lzwoc = -1;
			lzwfc = -1;
			continue;
		}
		lzwsp = lzwstack;
		if (lzwc == lzwslot) {
			if (lzwoc == -1)
				return -2;
			*lzwsp++ = lzwfc;
			lzwcode = lzwoc;
		}
		while (lzwcode >= lzwclear) {
			*lzwsp++ = lzwsuffix[lzwcode];
			lzwcode = lzwprefix[lzwcode];
		}
		lzwfc = lzwcode;
		for (;;) {
			*lzwout++ = lzwcode;
			if (lzwout == lzwbuf + sizeof lzwbuf) {
				lzwwrite(lzwbuf, sizeof lzwbuf);
				lzwout = lzwbuf;
				lzwcount += sizeof lzwbuf;
			}
			if (lzwsp <= lzwstack)
				break;
			lzwcode = *--lzwsp;
		}
		if (lzwslot < 0x1000 && lzwoc != -1) {
			lzwprefix[lzwslot] = lzwoc;
			lzwsuffix[lzwslot] = lzwfc;
			lzwslot++;
			if (lzwslot >= lzwtop && lzwbits < 12) {
				lzwbits++;
				lzwtop = 1 << lzwbits;
			}
		}
		lzwoc = lzwc;
	}
	if (lzwbyte() != 0)
		return -1;
	if (lzwout - lzwbuf) {
		lzwwrite(lzwbuf, lzwout - lzwbuf);
		lzwcount += lzwout - lzwbuf;
	}
	return lzwcount;
}

int
unlzw(unsigned char *dst, unsigned char *src, int n)
{
	if (n == 0)
		return 0;
	lzwsrc = src;
	lzwpos = src;
	lzwsize = n;
	lzwdst = dst;
	lzwoff = 0;
	lzwplanar = 0;
	return lzwdecode(8);
}

/* unpackscr unpacks a 640x480 screen and its palette, packed by zlib */
int
unpackscr(unsigned char *dst, unsigned char *pal, void *src, DWORD n)
{
	memset(dst, 0, 0x4b000);
	uslen = unpack(dst, 0x4b300, src, n);
	if (uslen == 0) {
		if (dst)
			memset(dst, 0, 0x4b000);
		if (pal)
			memcpy(pal, defpal, 0x300);
		return 0;
	}
	if (pal)
		memcpy(pal, dst + 0x4b000, 0x300);
	return 1;
}

/* loadscr reads the packed screen of data file entry id */
int
loadscr(int id, unsigned char *dst, unsigned char *pal, void *buf, DWORD size)
{
	memset(dst, 0, 0x4b000);
	if (!seekentry(id))
		return 0;
	if (size > storedn)
		size = storedn;
	if (!fileread(buf, size, 1, datfh))
		return 0;
	return unpackscr(dst, pal, buf, size);
}

/* read a 640x480 BMP of 8 bits (magic BM, or 0x11 0x15) from src, returning its size */
DWORD
loadbmp(unsigned char *dst, unsigned char *pal, char *src)
{
	bmptr = src;
	memread(&bmfh, sizeof bmfh, 1, &bmptr);
	memread(&bmih, sizeof bmih, 1, &bmptr);
	if ((((unsigned char *)&bmfh)[0] != 0x42 || ((unsigned char *)&bmfh)[1] != 0x4d) &&
	    (((unsigned char *)&bmfh)[0] != 0x11 || ((unsigned char *)&bmfh)[1] != 0x15))
		return 0;
	if (bmih.biWidth != 640 || bmih.biHeight != 480 || bmih.biPlanes != 1 || bmih.biBitCount != 8 || bmih.biCompression != 0)
		return 0;
	for (lbi = 0; lbi < 1 << bmih.biBitCount; lbi++) {
		pal[lbi * 3] = ((unsigned char *)bmptr)[lbi * 4 + 2] >> 2;
		pal[lbi * 3 + 1] = ((unsigned char *)bmptr)[lbi * 4 + 1] >> 2;
		pal[lbi * 3 + 2] = ((unsigned char *)bmptr)[lbi * 4] >> 2;
	}
	bmptr = src + bmfh.bfOffBits;
	clearscr();
	for (lby = bmih.biHeight - 1; lby >= 0; lby--)
		for (lbx = 0; lbx < bmih.biWidth; lbx++)
			dst[lbx + lby * 640] = *bmptr++;
	return bmfh.bfSize;
}

/* makebmp writes the 640x480 screen src with palette pal as a BMP into dst */
DWORD
makebmp(char *dst, unsigned char *src, unsigned char *pal)
{
	memset(&bmfh, 0, sizeof bmfh);
	memset(&bmih, 0, sizeof bmih);
	((unsigned char *)&bmfh)[0] = 0x42;
	((unsigned char *)&bmfh)[1] = 0x4d;
	bmfh.bfSize = 0x4b436;
	bmfh.bfOffBits = 0x436;
	bmih.biSize = sizeof bmih;
	bmih.biWidth = 640;
	bmih.biHeight = 480;
	bmih.biPlanes = 1;
	bmih.biBitCount = 8;
	bmih.biCompression = 0;
	bmih.biSizeImage = 0x4b000;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;
	bmih.biXPelsPerMeter = 0x1274;
	bmih.biYPelsPerMeter = 0x1274;
	memset(dst, 0, 0x4b436);
	memwrite(&bmfh, sizeof bmfh, 1, &dst);
	memwrite(&bmih, sizeof bmih, 1, &dst);
	for (mbi = 0; mbi < 1 << bmih.biBitCount; mbi++) {
		*dst++ = pal[mbi * 3 + 2] << 2;
		*dst++ = pal[mbi * 3 + 1] << 2;
		*dst++ = pal[mbi * 3] << 2;
		*dst++ = 0;
	}
	for (mby = bmih.biHeight - 1; mby >= 0; mby--)
		for (mbx = 0; mbx < bmih.biWidth; mbx++)
			*dst++ = src[mbx + mby * 640];
	return bmfh.bfSize;
}

/* loadbmpf reads the BMP file name into scrbuf and its palette */
DWORD
loadbmpf(char *name)
{
	if (fopenr(&lbfh, name)) {
		lbfn = fileread(workbuf, 1, 0x96640, lbfh);
		fileclose(&lbfh);
		if (lbfn == 0)
			goto fail;
		return lbfn = loadbmp((unsigned char *)scrbuf, (unsigned char *)scrbuf + 0x4b000, workbuf);
	}
fail:
	memset(scrbuf, 0, 0x4b000);
	memcpy(scrbuf + 0x4b000, pals[0], 0x300);
	return 0;
}

/* savebmpf writes the screen of surface n as the BMP file name */
DWORD
savebmpf(char *name, int n, unsigned char *pal)
{
	if (!fopenw(&sbfh, name))
		return 0;
	memset(&bmfh, 0, sizeof bmfh);
	memset(&bmih, 0, sizeof bmih);
	if (photomagic) {
		((unsigned char *)&bmfh)[0] = 0x11;
		((unsigned char *)&bmfh)[1] = 0x15;
	} else {
		((unsigned char *)&bmfh)[0] = 0x42;
		((unsigned char *)&bmfh)[1] = 0x4d;
	}
	bmfh.bfSize = 0x4b436;
	bmfh.bfOffBits = 0x436;
	bmih.biSize = sizeof bmih;
	bmih.biWidth = 640;
	bmih.biHeight = 480;
	bmih.biPlanes = 1;
	bmih.biBitCount = 8;
	bmih.biCompression = 0;
	bmih.biSizeImage = 0x4b000;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;
	bmih.biXPelsPerMeter = 0x1274;
	bmih.biYPelsPerMeter = 0x1274;
	if (!filewrite(&bmfh, sizeof bmfh, 1, sbfh)) {
		fileclose(&sbfh);
		return 0;
	}
	filewrite(&bmih, sizeof bmih, 1, sbfh);
	if (!pal)
		getpal(sbpal);
	for (sbi = 0; sbi < 1 << bmih.biBitCount; sbi++) {
		if (pal) {
			sbr = pal[sbi * 3];
			sbg = pal[sbi * 3 + 1];
			sbb = pal[sbi * 3 + 2];
		} else {
			sbr = sbpal[sbi * 3];
			sbg = sbpal[sbi * 3 + 1];
			sbb = sbpal[sbi * 3 + 2];
		}
		sbbyte = sbb << 2;
		filewrite(&sbbyte, 1, 1, sbfh);
		sbbyte = sbg << 2;
		filewrite(&sbbyte, 1, 1, sbfh);
		sbbyte = sbr << 2;
		filewrite(&sbbyte, 1, 1, sbfh);
		sbbyte = 0;
		filewrite(&sbbyte, 1, 1, sbfh);
	}
	if (getscreen(n, loadbuf))
		sbok = 1;
	else
		sbok = 0;
	for (sby = bmih.biHeight - 1; sby >= 0; sby--) {
		for (sbx = 0; sbx < bmih.biWidth; sbx++)
			if (sbok)
				sbrow[sbx] = loadbuf[sbx + sby * 640];
			else
				sbrow[sbx] = 0;
		filewrite(sbrow, 640, 1, sbfh);
	}
	fileclose(&sbfh);
	return bmfh.bfSize;
}

// PORT: Alt+P writes the whole screen, while savebmpf's face template stays 640 by 480
#ifdef PORT
/* saveshot writes the primary as the BMP file name, at the screen's width in the wide view */
DWORD
saveshot(char *name)
{
	int w, y, origin;

	if (sysscreen.left == 0)
		return savebmpf(name, 1, 0);
	w = sysscreen.right - sysscreen.left;
	if (!fopenw(&sbfh, name))
		return 0;
	memset(&bmfh, 0, sizeof bmfh);
	memset(&bmih, 0, sizeof bmih);
	((unsigned char *)&bmfh)[0] = 0x42;
	((unsigned char *)&bmfh)[1] = 0x4d;
	bmfh.bfSize = 0x436 + w * 480;
	bmfh.bfOffBits = 0x436;
	bmih.biSize = sizeof bmih;
	bmih.biWidth = w;
	bmih.biHeight = 480;
	bmih.biPlanes = 1;
	bmih.biBitCount = 8;
	bmih.biSizeImage = w * 480;
	bmih.biXPelsPerMeter = 0x1274;
	bmih.biYPelsPerMeter = 0x1274;
	if (!filewrite(&bmfh, sizeof bmfh, 1, sbfh)) {
		fileclose(&sbfh);
		return 0;
	}
	filewrite(&bmih, sizeof bmih, 1, sbfh);
	getpal(sbpal);
	for (y = 0; y < 256; y++) {
		sbbyte = sbpal[y * 3 + 2] << 2;
		filewrite(&sbbyte, 1, 1, sbfh);
		sbbyte = sbpal[y * 3 + 1] << 2;
		filewrite(&sbbyte, 1, 1, sbfh);
		sbbyte = sbpal[y * 3] << 2;
		filewrite(&sbbyte, 1, 1, sbfh);
		sbbyte = 0;
		filewrite(&sbbyte, 1, 1, sbfh);
	}
	origin = sysdrawx;
	sysdrawx = 0;
	if (lock(prim)) {
		for (y = 479; y >= 0; y--)
			filewrite((char *)sd.lpSurface + sysscreen.left + y * sd.lPitch, w, 1, sbfh);
		unlock(prim);
	}
	sysdrawx = origin;
	fileclose(&sbfh);
	return bmfh.bfSize;
}
#endif

/* mksurf creates an offscreen surface 640 wide for n 48x48 tiles, 13 to a row */
LPDIRECTDRAWSURFACE
mksurf(unsigned n)
{
	LPDIRECTDRAWSURFACE s;
	unsigned rows;

	rows = n / 13;
	if (n % 13)
		rows++;
	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	sd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	sd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	sd.dwWidth = 640;
	sd.dwHeight = rows * 48;
	if (gfxram == 1 && sd.dwHeight >= 0x800 || gfxram == 2 && sd.dwHeight >= 0x400 || gfxram == 3)
		sd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	if (ddfailed("cr_dta_s.xs.ddCrSrf", IDirectDraw_CreateSurface(dd, &sd, &s, NULL)))
#else
	if (ddfailed("cr_dta_s.xs.ddCrSrf", dd->CreateSurface(&sd, &s, NULL)))
#endif
		return NULL;
	return s;
}

/* mksurfwh creates an offscreen surface of w by h pixels */
LPDIRECTDRAWSURFACE
mksurfwh(DWORD w, DWORD h)
{
	LPDIRECTDRAWSURFACE s;

	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	sd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	sd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	sd.dwWidth = w;
	sd.dwHeight = h;
	if (gfxram == 1 && sd.dwHeight >= 0x800 || gfxram == 2 && sd.dwHeight >= 0x400 || gfxram == 3)
		sd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	if (ddfailed("cr_dta_s.xyw.ddCrSrf", IDirectDraw_CreateSurface(dd, &sd, &s, NULL)))
#else
	if (ddfailed("cr_dta_s.xyw.ddCrSrf", dd->CreateSurface(&sd, &s, NULL)))
#endif
		return NULL;
	return s;
}

/* setkey makes black transparent on s, restoring it while it is lost */
int
setkey(LPDIRECTDRAWSURFACE s)
{
	DDCOLORKEY key;

	key.dwColorSpaceLowValue = 0;
	key.dwColorSpaceHighValue = 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("s_dta_s_trnsp", IDirectDrawSurface_SetColorKey(s, DDCKEY_SRCBLT, &key))) {
#else
	while (ddfailed("s_dta_s_trnsp", s->SetColorKey(DDCKEY_SRCBLT, &key))) {
#endif
		if (ddbusy)
			continue;
		if (!ddlost)
			return 0;
		// PORT: DirectDraw through its C binding
#ifdef PORT
		if (ddfailed("s_dta_s_trnsp.r_sf", IDirectDrawSurface_Restore(s)))
#else
		if (ddfailed("s_dta_s_trnsp.r_sf", s->Restore()))
#endif
			return 0;
		savesurfs();
	}
	return 1;
}

int
getcaps(void)
{
	memset(&ddcaps, 0, sizeof ddcaps);
	ddcaps.dwSize = sizeof ddcaps;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	if (ddfailed("DD.gCaps", IDirectDraw_GetCaps(dd, &ddcaps, NULL)))
#else
	if (ddfailed("DD.gCaps", dd->GetCaps(&ddcaps, NULL)))
#endif
		return 0;
	return 1;
}

/* ddinfo writes the video memory and the blitter's abilities into two texts */
void
ddinfo(void)
{
	getcaps();
	ddinfop = vramtext;
	wsprintf(ddinfop, T(0x8e88), ddcaps.dwVidMemTotal >> 10, ddcaps.dwVidMemFree >> 10, (ddcaps.dwVidMemTotal - ddcaps.dwVidMemFree) >> 10);
	while (*ddinfop)
		ddinfop++;
	wsprintf(ddinfop, T(0x8ece), vidmem >> 10);
	while (*ddinfop)
		ddinfop++;
	if (vidsurfs[0])
		wsprintf(ddinfop, T(0x8efb), vidsurfs);
	while (*ddinfop)
		ddinfop++;
	wsprintf(ddinfop, T(0x8f02), sysmem >> 10);
	while (*ddinfop)
		ddinfop++;
	if (syssurfs[0])
		wsprintf(ddinfop, T(0x8efb), syssurfs);
	while (*ddinfop)
		ddinfop++;
	ddinfop = blittext;
	wsprintf(ddinfop, T(0x8f30), ddcaps.dwCaps & DDCAPS_BLT ? T(0x6b87) : T(0x6b91));
	while (*ddinfop)
		ddinfop++;
	wsprintf(ddinfop, T(0x8f54), (ddcaps.dwCaps & DDCAPS_COLORKEY) && (ddcaps.dwCKeyCaps & DDCKEYCAPS_SRCBLT) ? T(0x6b87) : T(0x6b91));
	while (*ddinfop)
		ddinfop++;
	wsprintf(ddinfop, T(0x8f74), ddcaps.dwCaps & DDCAPS_BLTCOLORFILL ? T(0x6b87) : T(0x6b91));
	while (*ddinfop)
		ddinfop++;
	ddinfon = blittime;
	if (ddinfon == -1)
		ddinfon = 0;
	wsprintf(ddinfop, T(0x8f94), ddinfon);
	while (*ddinfop)
		ddinfop++;
	ddinfon = keyblittime;
	if (ddinfon == -1)
		ddinfon = 0;
	wsprintf(ddinfop, T(0x8fb8), ddinfon);
	while (*ddinfop)
		ddinfop++;
	if (pixtime == -1)
		ddinfon = 0;
	else
		ddinfon = pixtime;
	wsprintf(ddinfop, T(0x8fe4), ddinfon);
	while (*ddinfop)
		ddinfop++;
}

/* pixtest times drawing 213 by 480 pixels, in milliseconds */
int
pixtest(void)
{
	pxtime = ticks();
	for (pxrow = 0; pxrow < 480; pxrow++)
		for (pxcol = 0; pxcol < 213; pxcol++)
			putpix0(pxcol, pxrow, 0x3a);
	pxtime = since(pxtime);
	if (pxtime == 0)
		pxtime++;
	return pxtime;
}

/* blittest times 12000 blits of a 48x48 tile, plain and with the color key */
void
blittest(void)
{
	blittime = ticks();
	for (bltj = 0; bltj < 50; bltj++)
		for (blti = 0; blti < 240; blti++)
			blit(surfc, 0x150, 0x60, back, blti + bltj, blti, 48, 48);
	blittime = since(blittime);
	if (blittime == 0)
		blittime++;
	keyblittime = ticks();
	for (bltj = 0; bltj < 50; bltj++)
		for (blti = 0; blti < 240; blti++)
			blitkey(surfc, 0x150, 0x60, back, blti + bltj, blti, 48, 48);
	keyblittime = since(keyblittime);
	if (keyblittime == 0)
		keyblittime++;
}

/* surfmem adds the memory of surface s, number n, to the video or system memory in use */
void
surfmem(LPDIRECTDRAWSURFACE s, int n)
{
	memset(&smdesc, 0, sizeof smdesc);
	smdesc.dwSize = sizeof smdesc;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	IDirectDrawSurface_GetSurfaceDesc(s, &smdesc);
#else
	s->GetSurfaceDesc(&smdesc);
#endif
	if (smdesc.ddsCaps.dwCaps & DDSCAPS_SYSTEMMEMORY) {
		if (!(surfwhere[n] & 1)) {
			surfwhere[n] |= 1;
			if (syssurfs[0])
				catstr(syssurfs, T(0xb1f9));
			catstr(syssurfs, surfnames[n]);
		}
		sysmem += smdesc.dwHeight * smdesc.lPitch;
	} else if (smdesc.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY) {
		if (!(surfwhere[n] & 2)) {
			surfwhere[n] |= 2;
			invram[n] = 1;
			if (vidsurfs[0])
				catstr(vidsurfs, T(0xb1f9));
			catstr(vidsurfs, surfnames[n]);
		}
		vidmem += smdesc.dwHeight * smdesc.lPitch;
	}
}

int ddclose(void);

/* ddinit creates the surfaces and the palette, shows scr if given, and measures the drawing */
int
ddinit(char *scr, unsigned char *pal)
{
	getcaps();
	memset(&sd, 0, sizeof sd);
	sd.dwSize = sizeof sd;
	sd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	sd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	sd.dwBackBufferCount = 1;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	if (ddfailed("DD.CrSfP", IDirectDraw_CreateSurface(dd, &sd, &prim, NULL))) {
#else
	if (ddfailed("DD.CrSfP", dd->CreateSurface(&sd, &prim, NULL))) {
#endif
fail:
		ddclose();
		return 0;
	}
	for (;;) {
		memset(&backcaps, 0, sizeof backcaps);
		backcaps.dwCaps = DDSCAPS_BACKBUFFER;
		// PORT: DirectDraw through its C binding
#ifdef PORT
		if (!ddfailed("DDSP.gAtSf", IDirectDrawSurface_GetAttachedSurface(prim, &backcaps, &back)))
#else
		if (!ddfailed("DDSP.gAtSf", prim->GetAttachedSurface(&backcaps, &back)))
#endif
			break;
		if (!ddlost)
			goto fail;
		// PORT: DirectDraw through its C binding
#ifdef PORT
		if (ddfailed("DDSP.gAtSf.rSf", IDirectDrawSurface_Restore(prim)))
#else
		if (ddfailed("DDSP.gAtSf.rSf", prim->Restore()))
#endif
			goto fail;
		savesurfs();
	}
	nmaptiles = 0;
	nobjtiles = 0;
	ncurtiles = 0;
	for (ddi = 0; ddi < 7; ddi++)
		if ((surfm[ddi] = mksurfwh(640, 0x300)) == NULL)
			goto fail;
	nmaptiles = 0x5b0;
	if ((surfc = mksurfwh(640, 0x3f0)) == NULL)
		goto fail;
	ncurtiles = 0x111;
	if (!setkey(surfc))
		goto fail;
	if ((surf2 = mksurfwh(640, 480)) == NULL)
		goto fail;
	// PORT: surf2 holds copies of the screen, wings and all
#ifdef PORT
	screensurface(surf2);
#endif
	if (!setkey(surf2))
		goto fail;
	for (ddi = 0; ddi < 13; ddi++) {
		if ((surfo[ddi] = mksurfwh(640, 0x300)) == NULL)
			goto fail;
		if (!setkey(surfo[ddi]))
			goto fail;
	}
	nobjtiles = 0xa90;
	for (ddi = 0; ddi < 5; ddi++) {
		if ((surfw[ddi] = mksurfwh(640, surfwh[ddi])) == NULL)
			goto fail;
		if (!setkey(surfw[ddi]))
			goto fail;
	}
	if ((surfpg = mksurfwh(640, 480)) == NULL)
		goto fail;
	savesurfs();
	memset(pe, 0, sizeof pe);
	// PORT: DirectDraw through its C binding
#ifdef PORT
	if (ddfailed("DD.CrPal", IDirectDraw_CreatePalette(dd, DDPCAPS_8BIT | DDPCAPS_ALLOW256, pe, &ddpal, NULL)))
#else
	if (ddfailed("DD.CrPal", dd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, pe, &ddpal, NULL)))
#endif
		goto fail;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("DDS P.sPal", IDirectDrawSurface_SetPalette(prim, ddpal)) && ddlost) {
		if (ddfailed("DDS P.sPal.rSf", IDirectDrawSurface_Restore(prim)))
#else
	while (ddfailed("DDS P.sPal", prim->SetPalette(ddpal)) && ddlost) {
		if (ddfailed("DDS P.sPal.rSf", prim->Restore()))
#endif
			break;
		savesurfs();
	}
	palblack = 1;
	if (pal)
		memcpy(gamepal, pal, sizeof gamepal);
	else
		for (ddi = 0; ddi < 256; ddi++) {
			gamepal[ddi * 3] = ((ddi >> 5) & 7) * 255 / 7 >> 2;
			gamepal[ddi * 3 + 1] = ((ddi >> 2) & 7) * 255 / 7 >> 2;
			gamepal[ddi * 3 + 2] = (ddi & 3) * 255 / 3 >> 2;
		}
	ddready = 1;
	textfont = CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, "Times New Roman");
	vidmem = 0;
	vidsurfs[0] = 0;
	syssurfs[0] = 0;
	sysmem = 0;
	memset(invram, 0, 8);
	memset(surfwhere, 0, sizeof surfwhere);
	for (ddi = 0; ddi < 8; ddi++) {
		selsurf(ddi);
		if (!(ddi == 4 || ddi == 5 || ddi == 6))
			surfmem(cursurf, ddi);
	}
	for (ddi = 0; ddi < 7; ddi++)
		surfmem(surfm[ddi], 4);
	for (ddi = 0; ddi < 13; ddi++)
		surfmem(surfo[ddi], 5);
	for (ddi = 0; ddi < 5; ddi++)
		surfmem(surfw[ddi], 6);
	ShowCursor(FALSE);
	if (scr) {
		putscreen(scr, 0);
		flip();
		fademed(1);
	}
	pixtime = pixtest();
	blittest();
	ddinfo();
	if (loadsurfs())
		return 1;
	wsprintf(dderrmsg, pick(T(0x7cda), T(0x7cfc), T(0x7d1a), T(0x7d1b), T(0x7d1c), T(0x7d38)));
	goto fail;
}

/* ddstart opens DirectDraw in 640x480x8, waiting while the mode is busy, then calls ddinit */
int
ddstart(char *scr, unsigned char *pal)
{
	memset(&fillfx, 0, sizeof fillfx);
	fillfx.dwSize = sizeof fillfx;
	if (ddfailed("DD.Cr", DirectDrawCreate(NULL, &dd, NULL)))
		return 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	if (ddfailed("DD.sCoopL", IDirectDraw_SetCooperativeLevel(dd, mainwnd, DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT | DDSCL_FULLSCREEN)))
#else
	if (ddfailed("DD.sCoopL", dd->SetCooperativeLevel(mainwnd, DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT | DDSCL_FULLSCREEN)))
#endif
		return 0;
	modew = 640;
	modeh = 480;
	modebpp = 8;
	modetry = 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("DD.sDspM", IDirectDraw_SetDisplayMode(dd, modew, modeh, modebpp))) {
#else
	while (ddfailed("DD.sDspM", dd->SetDisplayMode(modew, modeh, modebpp))) {
#endif
		if ((*(Dds *)ddres).err != DDERR_SURFACEBUSY || modetry++ >= 3) {
			ddclose();
			return 0;
		}
		modetime = ticks();
		do
			pollinput();
		while (since(modetime) < 750);
	}
	modeset = 1;
	modetime = ticks();
	do
		pollinput();
	while (since(modetime) < 1000);
	return ddinit(scr, pal);
}

int
ddquit(void)
{
	if (modeset) {
		fadefast(0);
		clrboth();
		ShowCursor(TRUE);
		DeleteObject(textfont);
	}
	return ddclose();
}

int
ddclose(void)
{
	freesurfs();
	// PORT: DirectDraw through its C binding
#ifdef PORT
	IDirectDraw_Release(dd);
#else
	dd->Release();
#endif
	dd = NULL;
	modeset = 0;
	return 1;
}

/* tilepos puts the corner of 48x48 tile n, 13 to a row, in srcrc, and its page in tilepage */
void
tilepos(int n, int paged)
{
	srcrc.left = n % 13 * 48;
	srcrc.top = n / 13 * 48;
	if (!paged) {
		tilepage = 0;
		return;
	}
	tilepage = srcrc.top / 768;
	srcrc.top -= tilepage * 768;
}

/* setsrc selects what putsrc blits: kinds 1 to 3 a 48x48 tile, 4 to 8 a small sprite of surfc, 9 a rectangle of srcrects */
void
setsrc(int kind, int n)
{
	srcflags = DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT;
	switch (kind) {
	case 1:
		tilepos(n, 1);
		srcw = 48;
		srch = 48;
		srcsurf = surfm[tilepage];
		srcflags &= ~DDBLTFAST_SRCCOLORKEY;
		break;
	case 2:
		tilepos(n, 1);
		srcw = 48;
		srcsurf = surfo[tilepage];
		srch = 48;
		break;
	case 3:
		tilepos(n, 0);
		srcw = 48;
		srch = 48;
		srcsurf = surfc;
		break;
	case 4:
		srcsurf = surfc;
		srcw = 24;
		srch = 20;
		srcrc.left = n % 26 * 24;
		srcrc.top = n / 26 * 20 + 0x300;
		break;
	case 9:
		srcrc = srcrects[n];
		tilepage = srcpages[n];
		srcsurf = surfw[tilepage];
		srcw = srcrc.right - srcrc.left;
		srch = srcrc.bottom - srcrc.top;
		return;
	case 5:
		srcw = 16;
		srch = 20;
		srcrc.left = n * 16 + 0x90;
		srcsurf = surfc;
		srcrc.top = 0x3dc;
		break;
	case 6:
		srcw = 16;
		srcsurf = surfc;
		srch = 16;
		srcrc.left = n * 16;
		srcrc.top = 0x360;
		break;
	case 7:
		srcw = 16;
		srch = 18;
		srcsurf = surfc;
		srcrc.left = n * 16 + 0x90;
		srcrc.top = 0x3c0;
		break;
	case 8:
		srcw = 16;
		srch = 20;
		srcsurf = surfc;
		srcrc.left = n * 16 + 0x90;
		srcrc.top = 0x390;
		break;
	default:
		srcsurf = back;
		srcw = 0;
		srch = 0;
		srcflags &= ~DDBLTFAST_SRCCOLORKEY;
		break;
	}
	srcrc.right = srcrc.left + srcw;
	srcrc.bottom = srcrc.top + srch;
}

/* putsrc blits the source that setsrc selects to x, y of surface n, clipped to the screen on surfaces 0 to 2 */
int
putsrc(int n, int x, int y, int kind, int idx)
{
	setsrc(kind, idx);
	if (n <= 2) {
		if (x < 0) {
			srcrc.left -= x;
			x = 0;
		} else {
			psover = x + srcw;
			if (psover > 640) {
				psover -= 640;
				srcrc.right -= psover;
			}
		}
		if (y < 0) {
			srcrc.top -= y;
			y = 0;
		} else {
			psover = y + srch;
			if (psover > 480) {
				psover -= 480;
				srcrc.bottom -= psover;
			}
		}
	}
	selsurf(n);
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("gogr.bltf", IDirectDrawSurface_BltFast(cursurf, x, y, srcsurf, &srcrc, srcflags)))
#else
	while (ddfailed("gogr.bltf", cursurf->BltFast(x, y, srcsurf, &srcrc, srcflags)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return 0;
	return 1;
}

/* drawchar draws character c of the big font at x, y of the locked surface */
void
drawchar(int c, int x, int y)
{
	if (y > 460)
		y = 460;
	dcdst = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	dcsrc = font[c];
	dcw = fontw[c];
	if (dcw >= 16) {
		dcw = 16;
		dcpad = 0;
	} else if (dcw < 16)
		dcpad = 16 - dcw;
	for (dchy = 0; dchy < 20; dchy++) {
		memset(dcdst, 0, 16);
		for (dchx = 0; dchx < dcw; dchx++) {
			dcpix = *dcsrc++;
			dcdst[dchx + dcpad] = dcpix;
		}
		dcsrc += 24 - dcw;
		dcdst += sd.lPitch;
	}
}

int
putch(int c, int x, int y)
{
	if (!lock(back))
		return 0;
	drawchar(c, x, y);
	if (!unlock(back))
		return 0;
	return 1;
}

/* drawtext draws s in the big font at x, y of surface n */
int
drawtext(int n, int x, int y, char *s)
{
	if (x < 0 || y < 0 || y > 464)
		return 0;
	selsurf(n);
	if (!lock(cursurf))
		return 0;
	while (*s) {
		dtch = *s;
		dtw = 16;
		if (x + dtw > 640)
			break;
		drawchar(dtch, x, y);
		s++;
		x += dtw;
	}
	if (!unlock(cursurf))
		return 0;
	return 1;
}

/* textat formats into the big font at pos, counted in steps of 4 pixels along the rows of back */
void
textat(unsigned pos, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(printbuf, fmt, ap);
	va_end(ap);
	drawtext(1, pos % 160 * 4, pos / 160, printbuf);
}

void
textn(int n, int x, int y, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(printbuf, fmt, ap);
	va_end(ap);
	drawtext(n, x, y, printbuf);
}

void
inkwhite(void)
{
	inkcolor = 0xe6;
	shadecolor = 0xf4;
	shadeon = 0;
}

void
inkshade(void)
{
	inkcolor = 0xf0;
	shadecolor = 0xf4;
	shadeon = 1;
}

void
inkdark(void)
{
	inkcolor = 0x5b;
	shadecolor = 0xf4;
	shadeon = 0;
}

void
setink(unsigned char ink, unsigned char shade, int on)
{
	inkcolor = ink;
	shadecolor = shade;
	shadeon = on;
}

void
noshade(void)
{
	shadeon = 0;
}

/* drawchar2 draws character c of the small font at x, y of the locked surface: 0 is transparent, 0xf4 the shade, 0xf0 and above the ink */
void
drawchar2(int c, int x, int y)
{
	dc2dst = (unsigned char *)sd.lpSurface + x + y * sd.lPitch;
	dc2src = font2[c];
	for (dc2y = 0; dc2y < 10; dc2y++) {
		for (dc2x = 0; dc2x < 12; dc2x++) {
			dc2pix = *dc2src++;
			if (dc2pix == 0)
				continue;
			if (dc2pix == 0xf4) {
				if (!shadeon)
					continue;
				dc2pix = shadecolor;
			} else if (dc2pix >= 0xf0)
				dc2pix = inkcolor;
			dc2dst[dc2x] = dc2pix;
		}
		dc2dst += sd.lPitch;
	}
}

int
textwidth(char *s)
{
	for (twsum = 0; *s; s++)
		twsum += fontw2[*s];
	return twsum;
}

/* drawtext2 draws s in the small font at x, y of surface n, centered when x is 0xffff */
void
drawtext2(int n, int x, int y, char *s)
{
	if (x == 0xffff) {
		x = 320 - textwidth(s) / 2;
		if (x < 0)
			x = 0;
	}
	if (x < 0 || y < 0 || y > 470)
		return;
	selsurf(n);
	if (!lock(cursurf))
		return;
	while (*s) {
		dt2ch = *s;
		dt2w = fontw2[dt2ch];
		if (x + dt2w > 640)
			break;
		drawchar2(dt2ch, x, y);
		s++;
		x += dt2w;
	}
	unlock(cursurf);
}

void
drawtext2o(int n, unsigned off, char *s)
{
	drawtext2(n, off % 640, off / 640, s);
}

void
text2n(int n, int x, int y, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(printbuf, fmt, ap);
	va_end(ap);
	drawtext2(n, x, y, printbuf);
}

void
text2o(int n, unsigned off, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(printbuf, fmt, ap);
	va_end(ap);
	drawtext2(n, off % 640, off / 640, printbuf);
}

/* glyph3 draws glyph c of the 24x20 font, w pixels wide, at x, y of the locked surface or of drawbuf. Textmode 0 draws the ink, 1 the ink and the shade, 2 and 3 an outline of shade, and 4 or a drawbuf shades the background through darker4. With capsfont the glyph comes from font3, outlined in textmode 5. A glyph byte other than 0 and 0xf0 to 0xf4 leaves the pointer behind in modes 1 and 4 (a defect) */
void
glyph3(int c, int x, int y, int w)
{
	int v;

	if (!drawbuf) {
		g3pitch = sd.lPitch;
		g3dst = x + (unsigned char *)sd.lpSurface + y * g3pitch;
	} else {
		g3pitch = 640;
		g3dst = drawbuf + x + y * 640;
	}
	if (capsfont == 0) {
		g3src = font[c];
		if (textmode == 2 || textmode == 3) {
			if (c == ' ' && textmode != 3)
				return;
			memset(g3colinked, 0, sizeof g3colinked);
			for (g3y = 0; g3y < 20; g3y++) {
				g3rowinked = 0;
				for (g3x = 0; g3x < 24; g3x++) {
					g3pix = *g3src++;
					v = g3pix;
					if (v == 0) {
						if (textmode == 3 && g3x < w)
							g3dst[g3x] = 0xf4;
					} else if (v >= 0xf0 && v <= 0xf3) {
						g3dst[g3x] = ((unsigned char *)spritetext)[0x6e4 + v];
						g3rowinked = 1;
						g3colinked[g3x] = 1;
					} else if (g3pix == 0xf4) {
						g3dst[g3x] = 0xf4;
						if (!g3colinked[g3x]) {
							g3dst[g3x - g3pitch] = 0xf4;
							g3dst[g3x - g3pitch * 2] = 0xf4;
							g3dst[g3x - g3pitch * 3] = 0xf4;
						}
						if (!g3rowinked) {
							g3dst[g3x - 1] = 0xf4;
							g3dst[g3x - 2] = 0xf4;
							g3dst[g3x - 3] = 0xf4;
						}
						g3dst[g3x + 1] = 0xf4;
						g3dst[g3x + 2] = 0xf4;
						g3dst[g3x + 3] = 0xf4;
						g3dst[g3x + g3pitch + 1] = 0xf4;
						g3dst[g3x + g3pitch] = 0xf4;
						g3dst[g3x + g3pitch * 2] = 0xf4;
						g3dst[g3x + g3pitch * 3] = 0xf4;
						g3rowinked = 1;
						g3colinked[g3x] = 1;
					}
				}
				g3dst += g3pitch;
			}
		} else if (textmode != 0) {
			if (drawbuf || textmode == 4) {
				for (g3y = 0; g3y < 20; g3y++) {
					for (g3x = 0; g3x < w; g3x++) {
						g3pix = *g3src++;
						if (g3pix == 0)
							// FIXED: *p++ = t[*p] reads and moves p unsequenced, undefined in C. Shade the pixel, then step, as Watcom compiled it.
#ifdef PORT
							*g3dst = darker4[*g3dst], g3dst++;
#else
							*g3dst++ = darker4[*g3dst];
#endif
						else if (g3pix >= 0xf0 && g3pix <= 0xf3)
							*g3dst++ = ((unsigned char *)spritetext)[0x6e4 + g3pix];
						else if (g3pix == 0xf4)
							*g3dst++ = g3pix;
					}
					g3src += 24 - g3x;
					g3dst += g3pitch - g3x;
				}
			} else {
				if (c == ' ')
					return;
				for (g3y = 0; g3y < 20; g3y++) {
					for (g3x = 0; g3x <= w; g3x++) {
						g3pix = *g3src++;
						if (g3pix == 0)
							g3dst++;
						else if (g3pix >= 0xf0 && g3pix <= 0xf3)
							*g3dst++ = ((unsigned char *)spritetext)[0x6e4 + g3pix];
						else if (g3pix == 0xf4)
							*g3dst++ = g3pix;
					}
					g3src += 24 - g3x;
					g3dst += g3pitch - g3x;
				}
			}
		} else {
			if (c == ' ')
				return;
			for (g3y = 0; g3y < 20; g3y++) {
				for (g3x = 0; g3x <= w; g3x++) {
					g3pix = *g3src++;
					if (g3pix == 0)
						g3dst++;
					else if (g3pix >= 0xf0 && g3pix <= 0xf3)
						*g3dst++ = ((unsigned char *)spritetext)[0x6e4 + g3pix];
					else
						g3dst++;
				}
				g3src += 24 - g3x;
				g3dst += g3pitch - g3x;
			}
		}
		return;
	}
	c = upper(c);
	g3src = font3[c];
	g3glyph = g3src;
	for (g3y = 0; g3y < 20; g3y++) {
		for (g3x = 0; g3x < 24; g3x++) {
			g3pix = *g3src++;
			if (!g3pix)
				continue;
			g3dst[g3x] = g3pix;
			if (textmode == 5) {
				if (g3y > 0) {
					if (g3x > 0 && g3glyph[(g3y - 1) * 24 + g3x - 1] != 0 && g3glyph[g3y * 24 + g3x - 1] == 0)
						g3dst[g3x - 1] = ((unsigned char *)spritetext)[0x7d1];
					if (g3x < 23 && g3glyph[(g3y - 1) * 24 + g3x + 1] != 0 && g3glyph[g3y * 24 + g3x + 1] == 0)
						g3dst[g3x + 1] = ((unsigned char *)spritetext)[0x7d1];
				}
				if (g3y < 19) {
					if (g3x > 0 && g3glyph[(g3y + 1) * 24 + g3x - 1] != 0 && g3glyph[g3y * 24 + g3x - 1] == 0)
						g3dst[g3x - 1] = ((unsigned char *)spritetext)[0x7d1];
					if (g3x < 23 && g3glyph[(g3y + 1) * 24 + g3x + 1] != 0 && g3glyph[g3y * 24 + g3x + 1] == 0)
						g3dst[g3x + 1] = ((unsigned char *)spritetext)[0x7d1];
				}
			}
		}
		g3dst += g3pitch;
	}
}

/* putsrcv is putsrc with sources taller than 20 pixels raised to stand on the same line */
void
putsrcv(int n, int x, int y, int kind, int idx)
{
	if (srch > 20)
		y -= (srch - 20) / 2;
	if (kind == 4 && idx == 0) {
		put48pt(n, x, y, gauges[0], 0, 0, 24, 20);
		return;
	}
	putsrc(n, x, y, kind, idx);
}

/* drawspq draws the queued sources on surface 0 */
void
drawspq(void)
{
	for (spqi = 0; spqi < nspq; spqi++) {
		setsrc(spq[spqi].kind, spq[spqi].idx);
		putsrcv(0, spq[spqi].x, spq[spqi].y, spq[spqi].kind, spq[spqi].idx);
	}
}

// FIXED: t3whole and t3fits test the marks of text3, which reads an automap label typed with && at its end past its 20 bytes, and fontw or surfm past their ends for a large code
#ifdef FEATURE_FAULTS
/* t3whole reports whether p holds n characters before its end */
static int
t3whole(const char *p, int n)
{
	int i;

	for (i = 0; i < n; i++)
		if (p[i] == 0)
			return 0;
	return 1;
}

/* t3fits reports whether code, of the mark at p, lies in the table of its kind: the font's 256 glyphs, or surfm's or surfo's pages of 13 by 16 tiles (tilepos) */
static int
t3fits(const char *p, int code)
{
	if (p[2] == 'C' && p[3] == 'H' && p[4] == 'R')
		return code >= 0 && code < 256;
	if (p[2] == 'M' && p[3] == 'A' && p[4] == 'P')
		return code >= 0 && code < (int)(sizeof surfm / sizeof surfm[0]) * 13 * 16;
	if (p[2] == 'O' && p[3] == 'B' && p[4] == 'J')
		return code >= 0 && code < (int)(sizeof surfo / sizeof surfo[0]) * 13 * 16;
	return 1;
}
#endif

/* text3 draws s in the 24x20 font at x, y of surface n: x 0xffff centers it, x and y 0xffff measure it into text3width. &&CHRnnnn draws glyph nnnn, &&MAPnnnn to &&TSTnnnn a source of setsrc, }}nnnn and }}X move to a column. It stops at {, at a return, and at a space when spacestop is set */
int
text3(int n, int x, int y, char *s)
{
	int cx;

	if (y == 0xffff)
		;
	else if (y > 460)
		y = 460;
	text3x = x;
	t3x0 = x;
	text3y = y;
	t3y0 = y;
	if (y == 0xffff)
		text3y = 0;
	if (text3x == 0xffff) {
		if (y == 0xffff) {
			t3pass = 2;
			y = 0;
		} else
			t3pass = 1;
		text3x = 0;
		if (t3y0 != 0xffff && s[0] == '&' && s[9] == ' ')
			t3p = s + 10;
		else
			goto start;
	} else {
		t3pass = 0;
		goto start;
	}
again:
	cx = text3x;
	text3width = 0;
	if (t3pass == 0 || t3pass == 4) {
		if (cx < 0 || y < 0 || y > 460)
			return 0;
		if (drawbuf == NULL) {
			selsurf(n);
			if (!lock(cursurf))
				return 0;
		}
	}
	while (*t3p) {
		// FIXED: text3 reads 9 bytes from && and 4 or 8 from }}, past the end of a text that ends inside the mark. End the text there.
#ifdef FEATURE_FAULTS
		if (*t3p == '&' && t3p[1] == '&' && !t3whole(t3p, 9))
			break;
		if (*t3p == '}' && t3p[1] == '}' && !t3whole(t3p, isdec(t3p[2]) ? 8 : 4))
			break;
#endif
		if (*t3p == '&' && t3p[1] == '&') {
			t3p += 8;
			t3code = (t3p[-3] - '0') * 1000 + (t3p[-2] - '0') * 100 + (t3p[-1] - '0') * 10 + (t3p[0] - '0');
			// FIXED: the code of &&CHR, &&MAP, or &&OBJ indexes fontw or surfm unbounded. Draw nothing for a code outside the font or the tile pages.
#ifdef FEATURE_FAULTS
			if (!t3fits(t3p - 8, t3code)) {
				t3w = 0;
				goto next;
			}
#endif
			if (t3p[-6] == 'C' && t3p[-5] == 'H' && t3p[-4] == 'R')
				goto glyph;
			if (t3p[-6] == 'M' && t3p[-5] == 'A' && t3p[-4] == 'P') {
				t3kind = 1;
src:
				setsrc(t3kind, t3code);
				if (drawbuf) {
					if (nspq < 10) {
						spq[nspq].x = cx;
						spq[nspq].y = y;
						spq[nspq].kind = t3kind;
						spq[nspq].idx = t3code;
						nspq++;
					}
					if (textmode)
						glyph3(' ', cx, y, srcw);
				} else if (t3pass == 3)
					putsrcv(n, cx, y, t3kind, t3code);
				else if (t3pass == 0)
					t3pass = 4;
				t3w = srcw;
				goto next;
			}
			if (t3p[-6] == 'O' && t3p[-5] == 'B' && t3p[-4] == 'J') {
				t3kind = 2;
				goto src;
			}
			if (t3p[-6] == 'C' && t3p[-5] == 'U' && t3p[-4] == 'R') {
				t3kind = 3;
				goto src;
			}
			if (t3p[-6] == 'I' && t3p[-5] == 'C' && t3p[-4] == 'O') {
				t3kind = 4;
				goto src;
			}
			if (t3p[-6] == 'P' && t3p[-5] == 'R' && t3p[-4] == 'Z') {
				t3kind = 5;
				goto src;
			}
			if (t3p[-6] == 'E' && t3p[-5] == 'Q' && t3p[-4] == 'U') {
				t3kind = 6;
				goto src;
			}
			if (t3p[-6] == 'R' && t3p[-5] == 'U' && t3p[-4] == 'N') {
				t3kind = 7;
				goto src;
			}
			if (t3p[-6] == 'T' && t3p[-5] == 'S' && t3p[-4] == 'T') {
				t3kind = 8;
				goto src;
			}
			t3w = 0;
			goto next;
		}
		if (*t3p == '{' || *t3p == ' ' && spacestop || *t3p == 13)
			break;
		if (*t3p == '}' && t3p[1] == '}' && isdec(t3p[2])) {
			t3col = (t3p[2] - '0') * 1000 + (t3p[3] - '0') * 100 + (t3p[4] - '0') * 10 + (t3p[5] - '0');
			if (t3col < 0 || t3col >= 640)
				;
			else
				cx = t3col;
			t3p += 7;
		} else if (*t3p == '}' && t3p[1] == '}') {
			cx = t3p[2] - 'A';
			cx *= 32;
			// FIXED: a column left of }}A puts the next letters left of the row. Take the screen's left.
#ifdef FEATURE_FAULTS
			if (cx < 0)
				cx = 0;
#endif
			t3p += 3;
		} else
			goto plain;
		if (t3pass == 2)
			text3width = 0;
		else if (cx > text3x)
			text3width = cx - text3x;
		else
			text3width = 0;
		t3w = 0;
		goto next;
plain:
		t3code = *t3p;
glyph:
		if (capsfont)
			t3w = fontw3[t3code];
		else
			t3w = fontw[t3code];
		if (boldtext)
			t3w++;
		if (t3w + cx > 640)
			break;
		if (t3pass == 0 || t3pass == 4) {
			glyph3(t3code, cx, y, t3w);
			if (boldtext)
				glyph3(t3code, cx + 1, y, t3w);
		}
next:
		t3p++;
		cx += t3w;
		text3width += t3w;
	}
	if (t3pass == 2)
		return text3width;
	if (t3pass == 1) {
		if (text3width > 640)
			text3width = 640;
		text3x = (640 - text3width) / 2;
		if (t3x0 == 0xffff && t3y0 != 0xffff && s[0] == '&' && s[9] == ' ')
			text3x -= 55;
		t3pass = 0;
start:
		t3p = s;
		goto again;
	}
	if (t3pass == 4) {
		if (drawbuf == NULL) {
			if (!unlock(cursurf))
				return 0;
			t3pass = 3;
			goto start;
		}
	} else if (t3pass != 0)
		return 1;
	else if (drawbuf == NULL) {
		if (unlock(cursurf))
			goto done;
		return 0;
	}
	if (textmode == 1) {
		shade(text3x, y - 2, text3width, 2);
		shade(text3x, y + 20, text3width, 1);
		shade(text3x - 4, y - 2, 4, 23);
		shade(text3x + text3width, y - 2, 4, 23);
	}
done:
	return 1;
}

/* text3w measures s in the 24x20 font into text3width */
int
text3w(char *s)
{
	text3(0, 0xffff, 0xffff, s);
	return text3width;
}

/* text3o draws s outlined: the outline in mode 2 or 3, then the letters in mode 0 */
int
text3o(int n, int x, int y, char *s)
{
	t3omode = textmode;
	if (!(textmode == 2 || textmode == 3))
		textmode = 2;
	t3ores = text3(n, x, y, s);
	textmode = 0;
	text3(n, x, y, s);
	textmode = t3omode;
	return t3ores;
}

int
text3m(int n, int x, int y, char *s)
{
	if (textmode == 2 || textmode == 3)
		return text3o(n, x, y, s);
	return text3(n, x, y, s);
}

void
ink3std(void)
{
	((unsigned char *)spritetext)[0x7d4] = 0xf0;
	((unsigned char *)spritetext)[0x7d5] = 0xf1;
	((unsigned char *)spritetext)[0x7d6] = 0xf2;
	((unsigned char *)spritetext)[0x7d7] = 0xf3;
}

void
ink3b(void)
{
	((unsigned char *)spritetext)[0x7d4] = 0xf2;
	((unsigned char *)spritetext)[0x7d5] = 0xf3;
	((unsigned char *)spritetext)[0x7d6] = 0xf3;
	((unsigned char *)spritetext)[0x7d7] = 0xf3;
}

void
ink3grey(void)
{
	((unsigned char *)spritetext)[0x7d4] = 0xe6;
	((unsigned char *)spritetext)[0x7d5] = 0xe6;
	((unsigned char *)spritetext)[0x7d6] = 0xe0;
	((unsigned char *)spritetext)[0x7d7] = 0xfe;
}

void
ink3d(void)
{
	((unsigned char *)spritetext)[0x7d4] = 0x76;
	((unsigned char *)spritetext)[0x7d5] = 0x76;
	((unsigned char *)spritetext)[0x7d6] = 0x76;
	((unsigned char *)spritetext)[0x7d7] = 0x76;
}

void
ink3set(unsigned char c)
{
	memset(&((unsigned char *)spritetext)[0x7d4], c, 4);
}

int
text3b(int n, int x, int y, char *s)
{
	t3bmode = textmode;
	ink3b();
	textmode = 1;
	t3bres = text3(n, x, y, s);
	ink3std();
	textmode = t3bmode;
	return t3bres;
}

int
text3grey(int n, int x, int y, char *s)
{
	ink3grey();
	textmode = 0;
	t3gres = text3(n, x, y, s);
	ink3std();
	textmode = 1;
	return t3gres;
}

int
text3col(int n, int x, int y, char *s, unsigned char c)
{
	ink3set(c);
	textmode = 0;
	t3colres = text3(n, x, y, s);
	ink3std();
	textmode = 1;
	return t3colres;
}

int
text3dk(int n, int x, int y, char *s)
{
	ink3d();
	textmode = 0;
	t3dkres = text3(n, x, y, s);
	ink3std();
	textmode = 1;
	return t3dkres;
}

/* text3c draws s centered on cx */
int
text3c(int n, int cx, int y, char *s)
{
	ink3std();
	textmode = 1;
	text3w(s);
	return text3(n, cx - text3width / 2, y, s);
}

int
text3cgrey(int n, int cx, int y, char *s)
{
	ink3grey();
	textmode = 0;
	text3w(s);
	t3cgres = text3(n, cx - text3width / 2, y, s);
	ink3std();
	textmode = 1;
	return t3cgres;
}

int
text3cdk(int n, int cx, int y, char *s)
{
	ink3d();
	textmode = 0;
	text3w(s);
	t3cdkres = text3(n, cx - text3width / 2, y, s);
	ink3std();
	textmode = 1;
	return t3cdkres;
}

int
text3ccol(int n, int cx, int y, char *s, unsigned char c)
{
	ink3set(c);
	textmode = 0;
	text3w(s);
	t3ccres = text3(n, cx - text3width / 2, y, s);
	ink3std();
	textmode = 1;
	return t3ccres;
}

int
text3grey2(int n, int x, int y, char *s)
{
	tg2res = text3grey(n, x, y, s);
	return tg2res;
}

/* text3f draws s with the glyphs of font3 */
int
text3f(int n, int x, int y, char *s)
{
	capsfont = 1;
	t3fres = text3(n, x, y, s);
	capsfont = 0;
	return t3fres;
}

void
text3e1(int n, int x, int y, char *s)
{
	textmode = 5;
	((unsigned char *)spritetext)[0x7d1] = 0x6c;
	text3f(n, x, y, s);
	textmode = 1;
}

void
text3e2(int n, int x, int y, char *s)
{
	textmode = 5;
	((unsigned char *)spritetext)[0x7d1] = 0x76;
	text3f(n, x, y, s);
	textmode = 1;
}

int
text3n(int n, int x, int y, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(printbuf, fmt, ap);
	va_end(ap);
	return text3(n, x, y, printbuf);
}

int
text3cn(int n, int cx, int y, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(printbuf, fmt, ap);
	va_end(ap);
	text3w(printbuf);
	cx -= text3width / 2;
	if (cx < 0)
		cx = 0;
	if (y < 0)
		y = 0;
	return text3(n, cx, y, printbuf);
}

int
text3greyn(int n, int x, int y, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	// FEATURE: features: the build's own format of a text
#ifdef FEATURES
	wvsprintf(printbuf, feattext(fmt), ap);
#else
	wvsprintf(printbuf, fmt, ap);
#endif
	va_end(ap);
	return text3grey(n, x, y, printbuf);
}

int
text3cgreyn(int n, int cx, int y, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(printbuf, fmt, ap);
	va_end(ap);
	return text3cgrey(n, cx, y, printbuf);
}

/* setrect records rectangle idx of the window surfaces, given in units of 2 pixels */
void
setrect(int page, int idx, int x, int y, int w, int h)
{
	if (idx >= 222)
		return;
	srcpages[idx] = page / 2;
	x *= 2;
	y *= 2;
	if (page % 2)
		y += sheetrows[page - 1];
	srcrects[idx].left = x;
	srcrects[idx].top = y;
	srcrects[idx].right = x + w;
	srcrects[idx].bottom = y + h;
}

unsigned char
getpixr(int idx, int x, int y)
{
	srcrc = srcrects[idx];
	tilepage = srcpages[idx];
	return getpixs(surfw[tilepage], srcrects[idx].left + x, srcrects[idx].top + y);
}

int
rectw(int idx)
{
	return srcrects[idx].right - srcrects[idx].left;
}

int
recth(int idx)
{
	return srcrects[idx].bottom - srcrects[idx].top;
}

/* initrects lays out the rectangles of the window surfaces, and their heights from the pairs of sheetrows */
void
initrects(void)
{
	memset(srcrects, 0, sizeof srcrects);
	memset(srcpages, 0, sizeof srcpages);
	for (iri = 0; iri < 5; iri++) {
		surfwh[iri] = 0;
		for (irj = 0; irj < 2; irj++)
			surfwh[iri] += sheetrows[iri * 2 + irj];
	}
	irpage = 0;
	setrect(irpage, 1, 0, 1, 0xd8, 200);
	setrect(irpage, 5, 0x6c, 1, 0xd8, 200);
	setrect(irpage, 0xb, 4, 0x7e, 0x270, 0x48);
	setrect(irpage, 0xc, 4, 0xa7, 0x270, 0x30);
	setrect(irpage, 0x13, 0xdc, 0x5b, 0xc0, 0x32);
	setrect(irpage, 0x14, 0xdc, 1, 0x70, 0x24);
	setrect(irpage, 0x98, 0x120, 0, 0x18, 0xb2);
	setrect(irpage, 0x99, 0x130, 0, 0x18, 0xb2);
	setrect(irpage, 0x9a, 0x6c, 0x6a, 0xb8, 0x16);
	irpage = 2;
	setrect(irpage, 6, 0, 1, 0xd4, 200);
	setrect(irpage, 7, 0x10, 0x69, 0xf4, 0xba);
	setrect(irpage, 8, 0xaa, 0x16, 0x118, 0x92);
	setrect(irpage, 9, 0xb0, 0x62, 0x10c, 0xca);
	irpage = 3;
	for (irj = 0; irj < 4; irj++)
		for (iri = 0; iri < 9; iri++)
			setrect(irpage, iri + 0x9b + irj * 9, iri * 64 / 2, irj * 0x5e / 2, 0x40, 0x5e);
	irpage = 4;
	for (iri = 0; iri < 6; iri++)
		for (irj = 0; irj < 3; irj++)
			setrect(irpage, irj + iri * 3 + 0x16, irj * 0x28 / 2 + 0x104, iri * 0x2a / 2, 0x28, 0x2a);
	setrect(irpage, 0x2b, 0x104, iri * 0x2a / 2, 0x28, 0x2a);
	for (irj = 0; irj < 4; irj++)
		for (iri = 0; iri < 6; iri++)
			setrect(irpage, irj * 6 + iri + 0x2c, iri * 0x50 / 2, irj * 0x5a / 2 + 1, 0x50, 0x5a);
	irpage = 5;
	setrect(irpage, 10, 0, 0x3b, 0xd4, 0x118);
	for (iri = 0; iri < 2; iri++)
		setrect(irpage, iri + 0x53, 0x110, iri * 0x1c / 2 + 0xa2, 0x40, 0x1c);
	setrect(irpage, 0x55, 0x110, 0x60, 0x40, 0x1c);
	for (iri = 0; iri < 3; iri++) {
		setrect(irpage, iri + 199, 0x110, iri * 10 + 0x41, 0x58, 0x10);
		setrect(irpage, iri + 0xca, 0x110, iri * 10 + 0x41, 0x20, 0x10);
	}
	setrect(irpage, 0xcd, 0xb8, 0x94, 0x20, 0x10);
	for (iri = 0; iri < 3; iri++)
		setrect(irpage, iri + 0xce, 0xf8, iri * 10 + 0x41, 0x28, 0xc);
	setrect(irpage, 0x57, 0x110, 0x93, 0x40, 0x1c);
	setrect(irpage, 0x58, 0xb8, 0xb4, 0x18, 0x18);
	setrect(irpage, 0x59, 0x100, 0xb4, 0x18, 0x18);
	setrect(irpage, 0x5a, 0x128, 0xb4, 0x18, 0x18);
	setrect(irpage, 0x5b, 0x118, 0xb4, 0x18, 0x18);
	setrect(irpage, 0x44, 0xd0, 0x79, 0x50, 0x90);
	for (irj = 0; irj < 2; irj++)
		for (iri = 0; iri < 6; iri++)
			setrect(irpage, iri + 0x45 + irj * 6, iri * 0x38 / 2, irj * 0x4e / 2 + 7, 0x38, 0x4e);
	setrect(irpage, 0x51, 0x114, 1, 0x50, 0x7a);
	setrect(irpage, 0x52, 0xd0, 0x23, 0x50, 0x2a);
	iri = 0;
	setrect(irpage, 0xd8, 1, iri * 30 + 1, 0x199, 0x1d);
	iri++;
	setrect(irpage, 0xd9, 1, iri * 30 + 1, 0x199, 0x1d);
	iri++;
	setrect(irpage, 0xda, 1, iri * 30 + 1, 0x199, 0x1d);
	iri++;
	setrect(irpage, 0xdb, 1, iri * 30 + 1, 0x199, 0x1d);
	iri++;
	setrect(irpage, 0xdc, 1, iri * 30 + 1, 0x199, 0x1d);
	irpage = 6;
	for (iri = 0; iri < 10; iri++)
		for (irj = 0; irj < 3; irj++)
			setrect(irpage, iri * 3 + 0x5c + irj, (iri < 5 ? iri * 0x48 : (iri - 5) * 0x48) / 2, (iri < 5 ? irj * 0x3c : (irj + 3) * 0x3c) / 2 + 1, 0x48, 0x3c);
	setrect(irpage, 0xd1, 0xb4, 0, 4, 200);
	setrect(irpage, 0xbf, 0xd0, 0, 0x58, 0x78);
	setrect(irpage, 0xc0, 0x104, 0, 0x58, 0x78);
	for (iri = 0; iri < 3; iri++)
		setrect(irpage, iri + 0xc1, iri * 0x58 / 2 + 0xbc, 0x46, 0x58, 0x3c);
	for (iri = 0; iri < 3; iri++)
		setrect(irpage, iri + 0xc4, iri * 0x58 / 2 + 0xbc, 0x6e, 0x58, 0x3c);
	irpage = 7;
	setrect(irpage, 2, 0xd4, 0x1e, 0xd8, 200);
	setrect(irpage, 0xd, 0, 0x84, 0x270, 100);
	setrect(irpage, 0x10, 0xd4, 2, 0x58, 0x30);
	setrect(irpage, 0x11, 0x104, 2, 0x58, 0x30);
	setrect(irpage, 0x12, 0xa4, 2, 0x58, 0x30);
	for (iri = 0; iri < 10; iri++)
		setrect(irpage, iri + 0x7a, (iri < 5 ? iri * 0x48 : (iri - 5) * 0x48) / 2, iri < 5 ? 0x1b : 0x39, 0x48, 0x3c);
	setrect(irpage, 0x15, 0, 0x59, 0xdc, 0x3e);
	setrect(irpage, 0xdd, 0x6f, 0x59, 0x32, 0x13);
	irpage = 8;
	for (iri = 0; iri < 10; iri++)
		setrect(irpage, iri + 0x84, (iri < 5 ? iri : iri - 5) * 0x40 / 2, iri < 5 ? 1 : iri / 5 * 0x5a / 2 + 1, 0x40, 0x5a);
	for (iri = 0; iri < 10; iri++)
		setrect(irpage, iri + 0x8e, (iri < 5 ? iri : iri - 5) * 0x40 / 2 + 0xa0, iri < 5 ? 1 : iri / 5 * 0x5a / 2 + 1, 0x40, 0x5a);
	setrect(irpage, 0xd2, 0, 0x70, 0x6a, 0xff);
	setrect(irpage, 0xd3, 0x109, 0x9b, 0x6e, 0xaa);
	setrect(irpage, 0xd4, 0x37, 0x5c, 100, 0xa0);
	setrect(irpage, 0xd5, 0x6a, 0x5c, 100, 0xa0);
	setrect(irpage, 0xd6, 0x9d, 0x5c, 100, 0xa0);
	setrect(irpage, 0xd7, 0xd0, 0x5c, 100, 0xa0);
	irpage = 1;
	setrect(irpage, 0xe, 0, 0, 0x130, 0x1e0);
	setrect(irpage, 0xf, 0x9c, 0, 10, 0x1e0);
	setrect(irpage, 4, 0xd0, 0, 0xd8, 200);
	setrect(irpage, 3, 0xd0, 100, 0xd8, 200);
}

/* putwin blits window rectangle idx to x, y of back */
int
putwin(int x, int y, int idx)
{
	srcrc = srcrects[idx];
	tilepage = srcpages[idx];
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("gogr.bltf", IDirectDrawSurface_BltFast(back, x, y, surfw[tilepage], &srcrc, DDBLTFAST_WAIT)))
#else
	while (ddfailed("gogr.bltf", back->BltFast(x, y, surfw[tilepage], &srcrc, DDBLTFAST_WAIT)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return 0;
	return 1;
}

/* putwint blits window rectangle idx to x, y of back, black transparent */
int
putwint(int x, int y, int idx)
{
	srcrc = srcrects[idx];
	tilepage = srcpages[idx];
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("gogr.bltf", IDirectDrawSurface_BltFast(back, x, y, surfw[tilepage], &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))
#else
	while (ddfailed("gogr.bltf", back->BltFast(x, y, surfw[tilepage], &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return 0;
	return 1;
}

/* putwinp blits the part of window rectangle idx inside the margins l, t, r, b to x, y of back, black transparent */
int
putwinp(int x, int y, int idx, int l, int t, int r, int b)
{
	srcrc = srcrects[idx];
	tilepage = srcpages[idx];
	srcrc.left += l;
	srcrc.top += t;
	srcrc.right -= l + r;
	if (srcrc.right <= srcrc.left)
		return 0;
	srcrc.bottom -= t + b;
	if (srcrc.bottom <= srcrc.top)
		return 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("gogr.bltf", IDirectDrawSurface_BltFast(back, x, y, surfw[tilepage], &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))
#else
	while (ddfailed("gogr.bltf", back->BltFast(x, y, surfw[tilepage], &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return 0;
	return 1;
}

/* putwino draws the outline of window rectangle idx at x, y of back in color outlinecols[0] */
int
putwino(int x, int y, int idx)
{
	srcrc = srcrects[idx];
	tilepage = srcpages[idx];
	pwow = srcrc.right - srcrc.left;
	pwoh = srcrc.bottom - srcrc.top;
	getwhs(surfw[tilepage], srcrc.left, srcrc.top, (unsigned char *)loadbuf, pwow, pwoh);
	for (pwoi = 0; pwoi < pwow * pwoh; pwoi++)
		if (loadbuf[pwoi])
			loadbuf[pwoi] = outlinecols[0];
	putclip(loadbuf, 0, x - 1, y, pwow, pwoh);
	putclip(loadbuf, 0, x + 1, y, pwow, pwoh);
	putclip(loadbuf, 0, x, y - 1, pwow, pwoh);
	putclip(loadbuf, 0, x, y + 1, pwow, pwoh);
	return 1;
}

void
pgtoback(void)
{
	blitfull(surfpg, back);
}

/* pgsides copies the left and right strips of surfpg to back */
void
pgsides(void)
{
	blit(surfpg, 0, 0, back, 0, 0, 0x6c, 480);
	blit(surfpg, 0x20a, 0, back, 0x20a, 0, 0x76, 480);
}

void
puthands(void)
{
	putwint(0, 0xe1, 0xd2);
	putwint(0x212, 0x136, 0xd3);
}

/* loadsurfs unpacks the pictures of surfc and surfpg and loads those of surfm, surfo, and surfw from the data files */
int
loadsurfs(void)
{
	sheetlen = unpack(loadbuf, 0x96000, curspack1, 0x6fb7);
	if (sheetlen)
		putsurf((unsigned char *)loadbuf, surfc, 0, 0, 640, 384);
	sheetlen = unpack(loadbuf, 0x96000, curspack2, 0x5beb);
	if (sheetlen)
		putsurf((unsigned char *)loadbuf, surfc, 0, 384, 640, 384);
	sheetlen = unpack(loadbuf, 0x96000, iconpack, 0x36b2);
	if (sheetlen)
		putsurf((unsigned char *)loadbuf, surfc, 0, 768, 640, 240);
	sheetlen = unpack(loadbuf, 0x96000, paperpack, 0x162e4);
	if (sheetlen)
		putsurf((unsigned char *)loadbuf, surfpg, 0, 0, 640, 480);
	sualt = randn(100) < 50 ? 1 : 0;
	if (!seekentry(sualt ? 6000 : 6000))
		return 0;
	susurf = 0;
	suhalf = 0;
	for (susheet = 0; susheet < 14; susheet++) {
		if (susheet != 0 && susheet % 2 == 0) {
			susurf++;
			suhalf = 0;
		}
		storedn = storedlen((sualt ? 6000 : 6000) + susheet);
		sheetread = fileread(loadpack, 1, storedn, datfh);
		sheetlen = unpack(loadbuf, 0x96000, loadpack, sheetread);
		if (sheetlen)
			putsurf((unsigned char *)loadbuf, surfm[susurf], 0, suhalf * 384, 640, 384);
		suhalf++;
	}
	sualt = randn(100) < 50 ? 1 : 0;
	if (!seekentry(sualt ? 3000 : 3000))
		return 0;
	susurf = 0;
	suhalf = 0;
	for (susheet = 0; susheet < 26; susheet++) {
		if (susheet != 0 && susheet % 2 == 0) {
			susurf++;
			suhalf = 0;
		}
		storedn = storedlen((sualt ? 3000 : 3000) + susheet);
		sheetread = fileread(loadpack, 1, storedn, datfh);
		sheetlen = unpack(loadbuf, 0x96000, loadpack, sheetread);
		if (sheetlen)
			putsurf((unsigned char *)loadbuf, surfo[susurf], 0, suhalf * 384, 640, 384);
		suhalf++;
	}
	sualt = randn(100) < 50 ? 1 : 0;
	if (!seekentry(sualt ? 18 : 18))
		return 0;
	susurf = 0;
	suhalf = 0;
	for (susheet = 0; susheet < 9; susheet++) {
		if (susheet != 0 && susheet % 2 == 0) {
			susurf++;
			suhalf = 0;
		}
		storedn = storedlen((sualt ? 18 : 18) + susheet);
		sheetread = fileread(loadpack, 1, storedn, datfh);
		sheetlen = unpack(loadbuf, 0x96000, loadpack, sheetread);
		if (sheetlen)
			putsurf((unsigned char *)loadbuf, surfw[susurf], 0, suhalf ? sheetrows[susheet - 1] : 0, 640, sheetrows[susheet]);
		suhalf++;
	}
	blanktiles();
	return 1;
}

/* putm puts tile n of surfm at x, y of back */
int
putm(int x, int y, int n)
{
	return putsrc(0, x, y, 1, n);
}

int
putm1(int x, int y, int n)
{
	return putsrc(1, x, y, 1, n);
}

/* clear48 fills the 48x48 square at x, y of back with black */
void
clear48(int x, int y)
{
	rc2.left = x;
	rc2.top = y;
	rc2.right = x + 48;
	rc2.bottom = y + 48;
	fillfx.dwFillColor = 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("op_s_s.blt", IDirectDrawSurface_Blt(back, &rc2, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)))
#else
	while (ddfailed("op_s_s.blt", back->Blt(&rc2, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return;
}

/* clrputo clears the square at x, y of back and puts tile n of surfo there */
int
clrputo(int x, int y, int n)
{
	clear48(x, y);
	return putsrc(0, x, y, 2, n);
}

/* puto puts tile n of surfo at x, y of back */
int
puto(int x, int y, int n)
{
	return putsrc(0, x, y, 2, n);
}

/* putcs puts tile n of surfc at x, y of back, black transparent */
void
putcs(int x, int y, unsigned n)
{
	srcrc.left = n % 13 * 48;
	srcrc.top = n / 13 * 48;
	srcrc.right = srcrc.left + 48;
	srcrc.bottom = srcrc.top + 48;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("op_csp.bltf", IDirectDrawSurface_BltFast(back, x, y, surfc, &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))
#else
	while (ddfailed("op_csp.bltf", back->BltFast(x, y, surfc, &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return;
}

/* putc1 puts tile n of surfc at x, y of prim */
int
putc1(int x, int y, int n)
{
	return putsrc(1, x, y, 3, n);
}

int
putcn(int s, int x, int y, int n)
{
	return putsrc(s, x, y, 3, n);
}

/* putic puts icon n, 24x20 and 26 to a row from y 768 of surfc, at x, y of surface s */
void
putic(int s, int x, int y, unsigned n)
{
	srcrc.left = n % 26 * 24;
	srcrc.top = n / 26 * 20 + 768;
	srcrc.right = srcrc.left + 24;
	srcrc.bottom = srcrc.top + 20;
	selsurf(s);
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("op_ic.bltf", IDirectDrawSurface_BltFast(cursurf, x, y, surfc, &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))
#else
	while (ddfailed("op_ic.bltf", cursurf->BltFast(x, y, surfc, &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return;
}

/* tileinfo writes the sizes and kinds of the tile caches as text into spritetext */
void
tileinfo(void)
{
	tinfop = spritetext;
	// FIXED: the size_t of sizeof for %ld, which reads 32 bits, is undefined in C where it has 64. Pass an int.
#ifdef PORT
	wsprintf(tinfop, T(0x9004), (int)(ntiles * sizeof tilebuf / 1024));
#else
	wsprintf(tinfop, T(0x9004), ntiles * sizeof tilebuf / 1024);
#endif
	while (*tinfop)
		tinfop++;
	wsprintf(tinfop, T(0x9032), minsets ? T(0x9044) : T(0x9051));
	while (*tinfop)
		tinfop++;
	wsprintf(tinfop, T(0x905a), nopvari ? T(0x9077) : T(0x9090));
	while (*tinfop)
		tinfop++;
	wsprintf(tinfop, T(0x90a2), noprun ? T(0x90b6) : T(0x90cb));
	while (*tinfop)
		tinfop++;
	// FIXED: as above, pass an int
#ifdef PORT
	wsprintf(tinfop, T(0x90dd), (int)(hasgearbank ? setpics * 18 * sizeof tilebuf / 1024 : 0));
#else
	wsprintf(tinfop, T(0x90dd), hasgearbank ? setpics * 18 * sizeof tilebuf / 1024 : 0);
#endif
	while (*tinfop)
		tinfop++;
}

/* tileinit allocates the biggest tile cache that memory allows, and sets the tile layout to match */
int
tileinit(void)
{
	setpics = 0x4c;
	dir0frm0 = 0;
	dir0frm6 = 6;
	dir0frm7 = 7;
	dir1frm0 = 8;
	dir1frm6 = 14;
	dir1frm7 = 15;
	dir2frm0 = 16;
	dir2frm6 = 22;
	dir2frm7 = 23;
	dir3frm0 = 24;
	dir3frm6 = 30;
	dir3frm7 = 31;
	if (minmem || nopani)
		goto least;
	if (!nopvari) {
		if (!noprun) {
			ntiles = 0xafc;
			tilecache = (unsigned char *)malloc(ntiles * sizeof tilebuf);
			if (tilecache) {
				minsets = 0;
				nopvari = 0;
				noprun = 0;
				goto done;
			}
		}
		setpics = 0x40;
		ntiles = 0x940;
		tilecache = (unsigned char *)malloc(ntiles * sizeof tilebuf);
		if (tilecache) {
			minsets = 0;
			nopvari = 0;
			if (noprun != 2)
				noprun = 1;
			goto done;
		}
	}
	setpics = 0x20;
	ntiles = 0x4a0;
	tilecache = (unsigned char *)malloc(ntiles * sizeof tilebuf);
	if (tilecache) {
		minsets = 0;
		if (nopvari != 2)
			nopvari = 1;
		if (noprun != 2)
			noprun = 1;
		goto done;
	}
least:
	setpics = 4;
	dir0frm0 = 0;
	dir0frm6 = 0;
	dir0frm7 = 0;
	dir1frm0 = 1;
	dir1frm6 = 1;
	dir1frm7 = 1;
	dir2frm0 = 2;
	dir2frm6 = 2;
	dir2frm7 = 2;
	dir3frm0 = 3;
	dir3frm6 = 3;
	dir3frm7 = 3;
	ntiles = 0x94;
	tilecache = (unsigned char *)malloc(ntiles * sizeof tilebuf);
	if (nopvari != 2)
		nopvari = 1;
	minsets = 1;
	if (noprun != 2)
		noprun = 1;
done:
	tileinfo();
	if (tilecache == NULL)
		return 0;
	return 1;
}

int
tilefree(void)
{
	if (tilecache)
		free(tilecache);
	tilecache = NULL;
	return 1;
}

/* allocbank allocates the second cache when minmem and nopequip are clear */
int
allocbank(void)
{
	int r;

	gearbank = NULL;
	hasgearbank = 0;
	r = 0;
	if (minmem == 0 && nopequip == 0) {
		gearbank = (unsigned char *)malloc(setpics * 18 * sizeof tilebuf);
		if (gearbank == NULL)
			hasgearbank = 0;
		else
			hasgearbank = 1;
		r = 1;
	}
	tileinfo();
	return r;
}

int
freebank(void)
{
	if (gearbank)
		free(gearbank);
	gearbank = NULL;
	return 1;
}

unsigned char
tilepix(int x, int y)
{
	return tilebuf[y][x];
}

/* tilebox finds the box of the nonzero pixels of tilebuf */
void
tilebox(int *x0, int *y0, int *x1, int *y1)
{
	*x0 = 0xff;
	*x1 = 0;
	*y0 = 0xff;
	*y1 = 0;
	for (tbx = 0; tbx < 48; tbx++)
		for (tby = 0; tby < 48; tby++)
			if (tilepix(tbx, tby)) {
				if (tbx < *x0)
					*x0 = tbx;
				if (tbx > *x1)
					*x1 = tbx;
				if (tby < *y0)
					*y0 = tby;
				if (tby > *y1)
					*y1 = tby;
			}
	if (*x0 > *x1)
		*x0 = *x1;
	if (*y0 > *y1)
		*y0 = *y1;
}

/* gettileo copies tile n of surfo into tilebuf */
void
gettileo(int n)
{
	tilepos(n, 1);
	get48s((unsigned char *)tilebuf, surfo[tilepage], srcrc.left, srcrc.top);
}

/* puttilec copies tilebuf to tile n of surfc */
void
puttilec(int n)
{
	tilepos(n, 0);
	put48(3, srcrc.left, srcrc.top, (unsigned char *)tilebuf);
}

/* gettilec copies tile n of surfc into tilebuf */
void
gettilec(int n)
{
	tilepos(n, 0);
	get48((unsigned char *)tilebuf, 3, srcrc.left, srcrc.top);
}

/* gettile copies tile n of surface s into dst */
void
gettile(int s, int n, unsigned char *dst)
{
	tilepos(n, 0);
	get48(dst, s, srcrc.left, srcrc.top);
}

void
tileclr(void)
{
	memset(tilebuf, 0, sizeof tilebuf);
}

void
tilecpy(unsigned char *src, unsigned char *dst)
{
	memcpy(dst, src, sizeof tilebuf);
}

/* blanktiles clears tiles 0x186, 0x3b7, and 0x792 of surfo */
void
blanktiles(void)
{
	tileclr();
	puttileo(0x186);
	puttileo(0x3b7);
	puttileo(0x792);
}

/* puttileo copies tilebuf to tile n of surfo */
void
puttileo(int n)
{
	tilepos(n, 1);
	put48s(surfo[tilepage], srcrc.left, srcrc.top, (unsigned char *)tilebuf);
}

/* putcache puts tile n of tilecache at x, y of back */
void
putcache(int x, int y, int n)
{
	put48(0, x, y, tilecache + n * sizeof tilebuf);
}

void
putcachet(int x, int y, int n)
{
	put48t(0, x, y, (char *)tilecache + n * sizeof tilebuf);
}

void
putcachet1(int x, int y, int n)
{
	put48t(1, x, y, (char *)tilecache + n * sizeof tilebuf);
}

/* tilemask clears the pixels of dst where src is not zero */
void
tilemask(unsigned char *dst, unsigned char *src)
{
	for (tmi = 0; tmi < 48 * 48; tmi++) {
		if (*src++)
			*dst++ = 0;
		else
			dst++;
	}
}

/* msrestore puts back what the mouse pointer covers */
void
msrestore(void)
{
	// PORT: DirectDraw through its C binding
#ifdef PORT
	ddfailed("rst_ms_hgr", IDirectDrawSurface_BltFast(surfs[spriteshape], spritex, spritey, surfm[0], &msbackrect, DDBLTFAST_WAIT));
#else
	ddfailed("rst_ms_hgr", surfs[spriteshape]->BltFast(spritex, spritey, surfm[0], &msbackrect, DDBLTFAST_WAIT));
#endif
}

/* mssave saves the 48x48 square under the mouse pointer */
void
mssave(void)
{
	rc.left = spritex;
	rc.top = spritey;
	rc.right = rc.left + 48;
	rc.bottom = rc.top + 48;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	ddfailed("gt_ms_hgr", IDirectDrawSurface_BltFast(surfm[0], msbackrect.left, msbackrect.top, surfs[spriteshape], &rc, DDBLTFAST_WAIT));
#else
	ddfailed("gt_ms_hgr", surfm[0]->BltFast(msbackrect.left, msbackrect.top, surfs[spriteshape], &rc, DDBLTFAST_WAIT));
#endif
}

void
iprestore(int s)
{
	// PORT: DirectDraw through its C binding, at the wide screen's left edge
#ifdef PORT
	int origin;

	origin = sysdrawx;
	sysdrawx = hudleft();
	ddfailed("rst_ip_hgr", IDirectDrawSurface_BltFast(surfs[s], 8, 0x118, surfw[0], &ipbackrect, DDBLTFAST_WAIT));
	sysdrawx = origin;
#else
	ddfailed("rst_ip_hgr", surfs[s]->BltFast(8, 0x118, surfw[0], &ipbackrect, DDBLTFAST_WAIT));
#endif
}

void
ipsave(int s)
{
	// PORT: DirectDraw through its C binding, at the wide screen's left edge
#ifdef PORT
	int origin;

	origin = sysdrawx;
	sysdrawx = hudleft();
	ddfailed("gt_ip_hgr", IDirectDrawSurface_BltFast(surfw[0], ipbackrect.left, ipbackrect.top, surfs[s], &iprect, DDBLTFAST_WAIT));
	sysdrawx = origin;
#else
	ddfailed("gt_ip_hgr", surfw[0]->BltFast(ipbackrect.left, ipbackrect.top, surfs[s], &iprect, DDBLTFAST_WAIT));
#endif
}

/* msdraw draws tile n of surfc (kind 3) or surfo (kind 2), clipped, at the pointer's place */
void
msdraw(int kind, int n)
{
	if (kind == 3) {
		tilepos(n, 0);
		srcsurf = surfc;
	} else if (kind == 2) {
		tilepos(n, 1);
		srcsurf = surfo[tilepage];
	}
	srcrc.left += mscx;
	srcrc.right = srcrc.left + msw;
	srcrc.top += mscy;
	srcrc.bottom = srcrc.top + msh;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("zms.bltf", IDirectDrawSurface_BltFast(cursurf, msx, msy, srcsurf, &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)) && ddlost && restoresurfs())
#else
	while (ddfailed("zms.bltf", cursurf->BltFast(msx, msy, srcsurf, &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)) && ddlost && restoresurfs())
#endif
		;
}

/* mousedraw saves what the pointer covers and draws the pointer and its two marks, clipped to the screen */
void
mousedraw(void)
{
	if (lostcontrol)
		return;
	if (ptrstates && ptrstates != ptrdrawn && lastx < 640)
		msstates(1);
	ptrdrawn = ptrstates;
	mssave();
	mscx = 0;
	mscy = 0;
	msw = 48;
	msh = 48;
	msx = lastx - 16;
	if (msx < 0) {
		mscx -= msx;
		msw += msx;
		msx = 0;
	} else if (msx > 640 - 48)
		msw -= msx - (640 - 48);
	// PORT: the pointer roaming the wide screen stands at its true place, clipped to the screen
#ifdef PORT
	if (sysroam) {
		mscx = 0;
		msw = 48;
		msx = mousewx - 16;
		if (msx < sysscreen.left) {
			mscx = sysscreen.left - msx;
			msw -= mscx;
			msx = sysscreen.left;
		} else if (msx > sysscreen.right - 48)
			msw -= msx - (sysscreen.right - 48);
	}
#endif
	msy = lasty - 16;
	if (msy < 0) {
		mscy -= msy;
		msh += msy;
		msy = 0;
	} else if (msy > 480 - 48)
		msh -= msy - (480 - 48);
	selsurf(curshape);
	msdraw(curkind, curimg);
	if (curkind1)
		msdraw(curkind1, curimg1);
	if (curkind2)
		msdraw(curkind2, curimg2);
}

/* shadeline shades the pixels of back on the line from x0, y0 to x1, y1, one step lighter when mode > 0, else two steps darker */
void
shadeline(int x0, int y0, int x1, int y1, int mode)
{
	unsigned char c, d;

	slerrx = 0;
	slerry = 0;
	sldx = x1 - x0;
	sldy = y1 - y0;
	if (sldx > sldy)
		sllen = sldx;
	else
		sllen = sldy;
	if (sldx > 0)
		slsx = 1;
	else if (sldx == 0)
		slsx = 0;
	else {
		sldx = -sldx;
		slsx = -1;
	}
	if (!lock(back))
		return;
	slp = (unsigned char *)sd.lpSurface + x0 + y0 * sd.lPitch;
	if (sldy > 0)
		slsy = sd.lPitch;
	else if (sldy == 0)
		slsy = 0;
	else {
		slsy = sd.lPitch;
		sldy = -sldy;
		slsy = -slsy;
	}
	sllast = NULL;
	for (sli = 0; sli <= sllen + 1; sli++) {
		if (slp != sllast) {
			if (mode > 0) {
				c = lighter[*slp];
				if (c != 0xff)
					*slp = c;
			} else {
				c = darker[*slp];
				if (c != 0xff) {
					d = darker[c];
					if (d != 0xff)
						*slp = d;
					else
						*slp = c;
				}
			}
			sllast = slp;
		}
		slerrx += sldx;
		slerry += sldy;
		if (slerrx > sllen) {
			slerrx -= sllen;
			slp += slsx;
		}
		if (slerry > sllen) {
			slerry -= sllen;
			slp += slsy;
		}
	}
	unlock(back);
}

void
clrbtncol(void)
{
	btncol = 0;
}

void
setbtncol(unsigned char c)
{
	btncol = c;
}

/* shaderow8 darkens the pixels x0 to x1 of row y of p through darker, where it has an entry */
void
shaderow8(unsigned char *p, int x0, int y, int x1, int pitch)
{
	sr8p = p + x0 + y * pitch;
	for (sr8x = x0; sr8x <= x1; sr8x++) {
		sr8c = darker[*sr8p];
		if (sr8c != 0xff)
			*sr8p = sr8c;
		sr8p++;
	}
}

/* shaderow7 lightens the pixels x0 to x1 of row y of p through lighter */
void
shaderow7(unsigned char *p, int x0, int y, int x1, int pitch)
{
	sr7p = p + x0 + y * pitch;
	for (sr7x = x0; sr7x <= x1; sr7x++) {
		sr7c = lighter[*sr7p];
		if (sr7c != 0xff)
			*sr7p = sr7c;
		sr7p++;
	}
}

void
shaderow8b(unsigned char *p, int x0, int y, int x1, int pitch)
{
	sr8bp = p + x0 + y * pitch;
	for (sr8bx = x0; sr8bx <= x1; sr8bx++) {
		sr8bc = darker[*sr8bp];
		if (sr8bc != 0xff)
			*sr8bp = sr8bc;
		sr8bp++;
	}
}

void
shadepix8(unsigned char *p, int x, int y, int pitch)
{
	shaderow8b(p, x, y, x, pitch);
}

void
shadepix7(unsigned char *p, int x, int y, int pitch)
{
	shaderow7(p, x, y, x, pitch);
}

/* shaderow9 maps the pixels x0 to x1 of row y of p through lighter2 */
void
shaderow9(unsigned char *p, int x0, int y, int x1, int pitch)
{
	sr9p = p + x0 + y * pitch;
	for (sr9x = x0; sr9x <= x1; sr9x++) {
		*sr9p = lighter2[*sr9p];
		sr9p++;
	}
}

void
savedclr(void)
{
	memset(&(*(Saved *)(packbuf + 0x4b63c)), 0, sizeof (*(Saved *)(packbuf + 0x4b63c)));
}

/* savedok tells whether saved holds a screen, marked "WmWm" */
int
savedok(void)
{
	if ((*(Saved *)(packbuf + 0x4b63c)).magic[0] == 0x57 && (*(Saved *)(packbuf + 0x4b63c)).magic[1] == 0x6d && (*(Saved *)(packbuf + 0x4b63c)).magic[2] == 0x57 && (*(Saved *)(packbuf + 0x4b63c)).magic[3] == 0x6d)
		return 1;
	return 0;
}

void
savedmark(void)
{
	(*(Saved *)(packbuf + 0x4b63c)).magic[0] = 'W';
	(*(Saved *)(packbuf + 0x4b63c)).magic[1] = 'm';
	(*(Saved *)(packbuf + 0x4b63c)).magic[2] = 'W';
	(*(Saved *)(packbuf + 0x4b63c)).magic[3] = 'm';
}

void
savescr(void)
{
	getscreen(0, (char *)(*(Saved *)(packbuf + 0x4b63c)).scr);
	savedmark();
}

void
restscr(void)
{
	putscreen((char *)(*(Saved *)(packbuf + 0x4b63c)).scr, 0);
	savedmark();
}

/* shadesaved9 maps the rectangle x0, y0 to x1, y1 of the saved screen through lighter2 */
void
shadesaved9(int x0, int y0, int x1, int y1)
{
	for (ss9y = y0; ss9y <= y1; ss9y++) {
		ss9p = (unsigned char *)(*(Saved *)(packbuf + 0x4b63c)).scr + x0 + ss9y * 640;
		for (ss9x = x0; ss9x <= x1; ss9x++) {
			*ss9p = lighter2[*ss9p];
			ss9p++;
		}
	}
}

void
shadesaveda(int x0, int y0, int x1, int y1)
{
	for (ssay = y0; ssay <= y1; ssay++) {
		ssap = (unsigned char *)(*(Saved *)(packbuf + 0x4b63c)).scr + x0 + ssay * 640;
		for (ssax = x0; ssax <= x1; ssax++) {
			*ssap = darker2[*ssap];
			ssap++;
		}
	}
}

/* restrow copies the pixels x0 to x1 of row y of the saved screen to dst */
void
restrow(unsigned char *dst, int x0, int y, int x1, int pitch)
{
	rrdst = dst + x0 + y * pitch;
	rrsrc = (unsigned char *)(*(Saved *)(packbuf + 0x4b63c)).scr + x0 + y * 640;
	for (rrx = x0; rrx <= x1; rrx++)
		*rrdst++ = *rrsrc++;
}

int
pillrest(int x, int y, int w, int h)
{
	plrr = h / 2;
	if (!lock(back))
		return 0;
	plrr2 = plrr * plrr;
	for (plry = 0; plry < plrr; plry++) {
		plrw = plrr2 - plry * plry;
		if (plrw > 0 && plrw < 0xb491)
			plrw = sqrttab[plrw];
		else
			plrw = 0;
		if (plrw >= plrr)
			plrw = plrr - 1;
		restrow((unsigned char *)sd.lpSurface, x + (plrr - 1) - plrw, y - 1 + plrr - plry, x + w - 1 - (plrr - 1) + plrw, sd.lPitch);
		restrow((unsigned char *)sd.lpSurface, x + (plrr - 1) - plrw, y + plrr + plry, x + w - 1 - (plrr - 1) + plrw, sd.lPitch);
	}
	return unlock(back) ? 1 : 0;
}

int
pillshade8(int x, int y, int w, int h)
{
	ps8r = h / 2;
	if (!lock(back))
		return 0;
	ps8r2 = ps8r * ps8r;
	for (ps8y = 0; ps8y < ps8r; ps8y++) {
		ps8w = ps8r2 - ps8y * ps8y;
		if (ps8w > 0 && ps8w < 0xb491)
			ps8w = sqrttab[ps8w];
		else
			ps8w = 0;
		if (ps8w >= ps8r)
			ps8w = ps8r - 1;
		shaderow8b((unsigned char *)sd.lpSurface, x + (ps8r - 1) - ps8w, y - 1 + ps8r - ps8y, x + w - 1 - (ps8r - 1) + ps8w, sd.lPitch);
		shaderow8b((unsigned char *)sd.lpSurface, x + (ps8r - 1) - ps8w, y + ps8r + ps8y, x + w - 1 - (ps8r - 1) + ps8w, sd.lPitch);
	}
	return unlock(back) ? 1 : 0;
}

int
pillshade9(int x, int y, int w, int h)
{
	ps9r = h / 2;
	if (!lock(back))
		return 0;
	ps9r2 = ps9r * ps9r;
	for (ps9y = 0; ps9y < ps9r; ps9y++) {
		ps9w = ps9r2 - ps9y * ps9y;
		if (ps9w > 0 && ps9w < 0xb491)
			ps9w = sqrttab[ps9w];
		else
			ps9w = 0;
		if (ps9w >= ps9r)
			ps9w = ps9r - 1;
		shaderow9((unsigned char *)sd.lpSurface, x + (ps9r - 1) - ps9w, y - 1 + ps9r - ps9y, x + w - 1 - (ps9r - 1) + ps9w, sd.lPitch);
		shaderow9((unsigned char *)sd.lpSurface, x + (ps9r - 1) - ps9w, y + ps9r + ps9y, x + w - 1 - (ps9r - 1) + ps9w, sd.lPitch);
	}
	return unlock(back) ? 1 : 0;
}

/* pillring draws the rim of a pill of w by h at x, y on back, light above and left, dark below and right */
void
pillring(int x, int y, int w, int h)
{
	ringr = h / 2;
	if (!lock(back))
		return;
	ringr2 = ringr * ringr;
	ringy = ringr - 1;
	ringw = ringr2 - ringy * ringy;
	if (ringw > 0 && ringw < 0xb491)
		ringw = sqrttab[ringw];
	else
		ringw = ringr - 1;
	ringw -= 2;
	shaderow7((unsigned char *)sd.lpSurface, x + (ringr - 1) - ringw - 1, y - 1, x + w - 1 - (ringr - 1) + ringw + 1, sd.lPitch);
	shaderow8((unsigned char *)sd.lpSurface, x + (ringr - 1) - ringw - 1, y + h, x + w - 1 - (ringr - 1) + ringw + 1, sd.lPitch);
	for (ringy = 0; ringy < ringr; ringy++) {
		ringw = ringr2 - ringy * ringy;
		if (ringw > 0 && ringw < 0xb491)
			ringw = sqrttab[ringw];
		else
			ringw = 0;
		if (ringw >= ringr)
			ringw = ringr - 1;
		ringpx = x + (ringr - 1) - ringw - 1;
		ringpy = y - 1 + ringr - ringy;
		shadepix7((unsigned char *)sd.lpSurface, ringpx, ringpy, sd.lPitch);
		shadepix7((unsigned char *)sd.lpSurface, ringpx - 1, ringpy, sd.lPitch);
		shadepix7((unsigned char *)sd.lpSurface, ringpx, ringpy - 1, sd.lPitch);
		ringpx = x + w - 1 - (ringr - 1) + ringw + 1;
		ringpy = y - 1 + ringr - ringy;
		shadepix8((unsigned char *)sd.lpSurface, ringpx, ringpy, sd.lPitch);
		shadepix8((unsigned char *)sd.lpSurface, ringpx + 1, ringpy, sd.lPitch);
		shadepix8((unsigned char *)sd.lpSurface, ringpx, ringpy - 1, sd.lPitch);
		ringpx = x + (ringr - 1) - ringw - 1;
		ringpy = y + ringr + ringy;
		shadepix7((unsigned char *)sd.lpSurface, ringpx, ringpy, sd.lPitch);
		shadepix7((unsigned char *)sd.lpSurface, ringpx + 1, ringpy, sd.lPitch);
		shadepix7((unsigned char *)sd.lpSurface, ringpx, ringpy + 1, sd.lPitch);
		ringpx = x + w - 1 - (ringr - 1) + ringw + 1;
		ringpy = y + ringr + ringy;
		shadepix8((unsigned char *)sd.lpSurface, ringpx, ringpy, sd.lPitch);
		shadepix8((unsigned char *)sd.lpSurface, ringpx + 1, ringpy, sd.lPitch);
		shadepix8((unsigned char *)sd.lpSurface, ringpx, ringpy + 1, sd.lPitch);
	}
	unlock(back);
}

/* button draws a pill button at x, y, w by h with the text of s up to its first ], [, or CR, centered: grey or in btncol when lit, dark when not */
void
button(int x, int y, int w, int h, char *s, unsigned char lit)
{
	pillring(x, y, w, h);
	// FIXED: printbuf - 1 points before the array, undefined in C. Start at printbuf and step after the test.
#ifdef PORT
	btnp = printbuf;
	for (;;) {
		*btnp = *s;
		s++;
		if (!(*btnp != 0 && *btnp != ']' && *btnp != '[' && *btnp != 13 && btnp < printbuf + 0x95e))
			break;
		btnp++;
	}
#else
	btnp = printbuf - 1;
	do {
		*++btnp = *s;
		s++;
	} while (*btnp != 0 && *btnp != ']' && *btnp != '[' && *btnp != 13 && btnp < printbuf + 0x95e);
#endif
	btnx = x + w / 2;
	btny = y + h / 2 - 9;
	*btnp = 0;
	if (lit) {
		if (!btncol) {
			if (lit == 0xfe || btnfull)
				text3c(0, btnx, btny, printbuf);
			else
				text3cgrey(0, btnx, btny, printbuf);
		} else
			text3ccol(0, btnx, btny, printbuf, btncol);
	} else
		text3cdk(0, btnx, btny, printbuf);
}

void
buttonon(int x, int y, int w, int h, char *s)
{
	button(x, y, w, h, s, 1);
}

void
buttonoff(int x, int y, int w, int h, char *s)
{
	button(x, y, w, h, s, 0);
}

/* buttonarr draws a lit button with the marks T(0xb57f) and T(0xb589) on each side of its text */
void
buttonarr(int x, int y, int w, int h, char *s)
{
	buttonon(x, y, w, h, s);
	baleft = btnx - text3width / 2 - 19;
	baright = btnx + text3width / 2 + 3;
	text3(0, baleft, btny, T(0xb57f));
	text3(0, baright, btny, T(0xb589));
}

/* putgauge puts the icon of surfc that shows v in tenths, 0xc3 to 0xcc, at x, y of surface n */
int
putgauge(int n, int x, int y, int v)
{
	if (v < 13)
		pgicon = 0;
	else if (v < 26)
		pgicon = 1;
	else if (v < 39)
		pgicon = 2;
	else if (v < 52)
		pgicon = 3;
	else if (v < 63)
		pgicon = 4;
	else if (v < 74)
		pgicon = 5;
	else if (v < 83)
		pgicon = 6;
	else if (v < 90)
		pgicon = 7;
	else if (v < 95)
		pgicon = 8;
	else
		pgicon = 9;
	pgicon += 0xc3;
	return putcn(n, x, y, pgicon);
}

/* winframe draws a frame of w by h at x, y of back from window rectangle 14, closed on the right by rectangle 15 when keyed */
void
winframe(int x, int y, int w, int h, int keyed)
{
	srcrc = srcrects[14];
	tilepage = srcpages[14];
	w -= 8;
	h -= 8;
	blit(surfw[tilepage], srcrc.left, srcrc.top, back, x, y, w, h);
	blit(surfw[tilepage], srcrc.left, srcrc.top + 0x1d8, back, x, y + h, w, 8);
	if (keyed) {
		srcrc = srcrects[15];
		tilepage = srcpages[15];
		blitkey(surfw[tilepage], srcrc.left, srcrc.top, back, x + w, y, 10, h + 8);
	} else {
		blit(surfw[tilepage], srcrc.left + 0x128, srcrc.top + 0x1d8, back, x + w, y + h, 8, 8);
		blit(surfw[tilepage], srcrc.left + 0x128, srcrc.top, back, x + w, y, 8, h);
	}
}

/* winframe2 draws a frame of w by h at x, y of back from window rectangle 14: its corners, edges, and a middle taken from its left part */
void
winframe2(int x, int y, int w, int h)
{
	srcrc = srcrects[14];
	tilepage = srcpages[14];
	w -= 8;
	h -= 8;
	blit(surfw[tilepage], srcrc.left, srcrc.top, back, x, y, 8, h);
	blit(surfw[tilepage], srcrc.left + 0x128, srcrc.top, back, x + w, y, 8, h);
	blit(surfw[tilepage], srcrc.left, srcrc.top + 0x1d8, back, x, y + h, w, 8);
	blit(surfw[tilepage], srcrc.left + 0x128, srcrc.top + 0x1d8, back, x + w, y + h, 8, 8);
	wf2mid = 0x130 - w - 8;
	if (wf2mid < 8)
		wf2mid = 8;
	blit(surfw[tilepage], srcrc.left + wf2mid, srcrc.top, back, x + 8, y, w - 8, h);
}

/* loadface loads portrait n of 285, 108 by 120, five to a data file, into dst, and into dst2 its second picture: dst with the runs of record n % 5 of an "fd" file laid over it */
int
loadface(unsigned char *dst, unsigned char *dst2, unsigned n)
{
	if (n >= 0x11d)
		n = 0x11c;
	lfidx = n / 5;
	if (!facecache[lfidx]) {
		if (!seekentry(lfidx + 0x209c))
			return 0;
		lfidx = fileread(((unsigned char *)packbuf) + 0x4b320, 1, storedn, datfh);
	} else {
		memcpy(((unsigned char *)packbuf) + 0x4b320, facecache[lfidx], facesizes[lfidx]);
		lfidx = facesizes[lfidx];
	}
	lfidx = unpack(packbuf, 0x4b320, ((unsigned char *)packbuf) + 0x4b320, lfidx);
	memcpy(dst, packbuf + n % 5 * 0x32a0, 0x32a0);
	if (dst2 == NULL)
		goto done;
	if (nofani) {
		memcpy(dst2, dst, 0x32a0);
		return 1;
	}
	if (overonly == 0)
		memcpy(dst2, dst, 0x32a0);
	else
		memset(dst2, 0, 0x32a0);
	lfidx = n / 5;
	lfover = ((unsigned char *)packbuf) + 0x4b320;
	if (!overcache[lfidx]) {
		if (!seekentry(lfidx + 0x1fb8))
			return 0;
		lfidx = fileread(lfover + 0x25990, 1, storedn, datfh);
		if (lfidx)
			lfidx = unpack(lfover, 0x25990, lfover + 0x25990, lfidx);
	} else {
		memcpy(lfover, overcache[lfidx], oversizes[lfidx]);
		lfidx = oversizes[lfidx];
	}
	for (lfi = 0; lfi <= n % 5; lfi++) {
		if (*lfover++ != 0x66)
			return 2;
		if (*lfover++ != 0x64)
			return 2;
		lfrun = *(unsigned short *)lfover;
		if (lfrun < 3)
			return 2;
		lfover += lfrun;
	}
	lfrun -= 2;
	lfover -= lfrun;
	lfoutp = dst2;
	for (lfi = 0; lfi < lfrun && lfoutp < dst2 + 0x32a0; ) {
		if (*lfover == 0xff) {
			lfover++;
			lfi++;
			if (lfoutp + *lfover > dst2 + 0x32a0)
				return 2;
			lfoutp += *lfover++;
			lfi++;
		} else {
			*lfoutp++ = *lfover++;
			lfi++;
		}
	}
done:
	return 1;
}

void
putface(int x, int y, char *src)
{
	putclip(src, 0, x, y, 0x6c, 0x78);
}

void
putfacen(int n, int x, int y, char *src)
{
	putclip(src, n, x, y, 0x6c, 0x78);
}

/* faceshadow shades a shadow below and beside the portrait src at x, y of back, following its outline */
void
faceshadow(int x, int y, char *src)
{
	char *p;

	// FIXED: the scans of the portrait's last row read past its end, into the next portrait in WLINK's layout. Stop them at the end of the row.
#ifdef FEATURE_FAULTS
	fscol0 = 0;
	while (fscol0 < 0x6c && src[0x3234 + fscol0] == 0)
		fscol0++;
	if (fscol0 >= 0x6c)
		return;
	fsw = 0;
	for (p = src + fscol0; p < src + 0x6c && p[0x3234]; p++)
		fsw++;
#else
	fscol0 = 0;
	while (src[0x3234 + fscol0] == 0) {
		if (fscol0 >= 0x6c)
			return;
		fscol0++;
	}
	fsw = 0;
	for (p = src + fscol0; p[0x3234]; p++)
		fsw++;
#endif
	if (0x6c - fscol0 < fsw)
		fsw = 0x6c - fscol0;
	if (shadowleft)
		shade(x + fscol0, y + 0x76, fsw, 2);
	else
		shade(x + fscol0, y + 0x78, fsw, 2);
	fsrow = 0x77;
	fscol = fscol0 + fsw - 1;
	if (src[fscol0 + fsw - 1 + fsrow * 0x6c]) {
		// FIXED: at the right edge of the last row the test reads past the portrait, into the next one in WLINK's layout. Take that byte as 0, the background.
#ifdef FEATURE_FAULTS
		for (; src[fsrow * 0x6c + fscol] != '\0' && !(fscol + 1 + fsrow * 0x6c < 0x6c * 0x78 && src[fscol + 1 + fsrow * 0x6c] != '\0') && fsrow > 1; fsrow--)
#else
		for (; src[fsrow * 0x6c + fscol] != '\0' && !(src[fscol + 1 + fsrow * 0x6c] != '\0') && fsrow > 1; fsrow--)
#endif
			;
		fsrow++;
		if (shadowleft)
			shade(x + fscol - 1, y + fsrow, 2, 0x78 - fsrow + 1);
		else
			shade(x + fscol + 1, y + fsrow, 2, 0x78 - fsrow + 1);
	}
	fscol = fscol0;
	fsrow = 0x77;
	if (src[fscol + fsrow * 0x6c]) {
		for (; src[fsrow * 0x6c + fscol] != '\0' && !(src[fscol - 1 + fsrow * 0x6c] != '\0') && fsrow > 1; fsrow--)
			;
		fsrow++;
		if (shadowleft)
			shade(x + fscol, y + fsrow, 2, 0x78 - fsrow + 1);
		else
			shade(x + fscol - 2, y + fsrow, 2, 0x78 - fsrow + 1);
	}
}

void
frame48(int x, int y)
{
	line(x - 1, y - 1, x + 48, y - 1, 0x30);
	line(x - 1, y + 48, x + 48, y + 48, 0x30);
	line(x - 1, y - 1, x - 1, y + 48, 0x30);
	line(x + 48, y - 1, x + 48, y + 48, 0x30);
}

void
frameface(int x, int y)
{
	line(x - 1, y - 1, x + 0x6c, y - 1, 0x30);
	line(x - 1, y + 0x78, x + 0x6c, y + 0x78, 0x30);
	line(x - 1, y - 1, x - 1, y + 0x78, 0x30);
	line(x + 0x6c, y - 1, x + 0x6c, y + 0x78, 0x30);
}

/* faceset draws at x, y of back the frames of four tiles and of two portraits, and when n is not -1 the tiles a to d and the two pictures of portrait n */
void
faceset(int x, int y, int n, int a, int b, int c, int d)
{
	x += 8;
	frame48(x, y);
	frame48(x + 0x38, y);
	frame48(x + 0x70, y);
	frame48(x + 0xa8, y);
	frameface(x, y + 0x38);
	frameface(x + 0x74, y + 0x38);
	if (n == -1)
		return;
	putcs(x, y, a);
	putcs(x + 0x38, y, b);
	putcs(x + 0x70, y, c);
	putcs(x + 0xa8, y, d);
	loadface((unsigned char *)scrbuf, (unsigned char *)scrbuf + 0x32a0, n);
	putface(x, y + 0x38, scrbuf);
	putface(x + 0x74, y + 0x38, scrbuf + 0x32a0);
}

/* facetmpl draws the template for a player's own portrait, its frames and help texts, on back and writes it as a picture */
void
facetmpl(int alt)
{
	clrback();
	faceset(0, 0xf0, -1, 0, 0, 0, 0);
	faceset(0x140, 0xf0, 3, 0x6b, 0x9f, 0xac, 0xb9);
	text3grey(0, 8, 0x28, T(0x8c79));
	text3grey(0, 8, 0x3c, T(0x8cb2));
	text3grey(0, 8, 0x64, T(0x8cd0));
	text3grey(0, 8, 0x78, T(0x8cff));
	text3grey(0, 8, 0xa0, T(0x8d27));
	text3grey(0, 8, 0xd4, T(0x8d57));
	text3grey(0, 0x148, 0xd4, T(0x8d62));
	if (alt)
		faceset(0, 0xf0, 6, 0x6e, 0xa2, 0xaf, 0xbc);
	save0();
	for (ftmi = 0, ftmp = (unsigned char *)scrbuf; ftmi < 0x4b000; ftmi++) {
		if (*ftmp >= 0xe0)
			*ftmp = 0x30;
		ftmp++;
	}
	showscr();
	memcpy(packbuf, pals[0], 0x300);
	memset(packbuf + 0x2a0, 0x3f, 0x60);
	photomagic = 1;
	savebmpf(T(0x9bfd), 0, (unsigned char *)packbuf);
	photomagic = 0;
}

/* loadmyface reads the player's own portrait, its two pictures and four tiles, from the template picture, and returns 1 when the portrait holds a pixel */
int
loadmyface(void)
{
	int i, x, y, any;
	unsigned char c;

	myfacesum = 0;
	if (!loadbmpf(T(0x9bfd)))
		return 0;
	x = 8;
	y = 0xf0;
	getbuf((unsigned char *)scrbuf, x, 0x128, (unsigned char *)heroface, 0x6c, 0x78);
	getbuf((unsigned char *)scrbuf, 0x7c, 0x128, (unsigned char *)heroface2, 0x6c, 0x78);
	any = 0;
	for (i = 0; i < 0x32a0; i++) {
		c = heroface[i];
		myfacesum += c;
		if (c != 0)
			any = 1;
	}
	grab48(x, y, headicons[0]);
	grab48(x + 0x38, y, headicons[1]);
	grab48(x + 0x70, y, headicons[2]);
	grab48(x + 0xa8, y, headicons[3]);
	memcpy(headicons[4], headicons[0], 0x900);
	memcpy(headicons[5], headicons[1], 0x900);
	memcpy(headicons[6], headicons[2], 0x900);
	memcpy(headicons[7], headicons[3], 0x900);
	for (i = 0; i < 0x900; i++) {
		c = headicons[0][i];
		myfacesum += c;
		if (c != 0)
			any = 1;
	}
	return any;
}

/* exportface copies the template picture to the first free numbered file name, as a BMP */
int
exportface(void)
{
	if (!fopenr(&efh, T(0x9bfd)))
		return 0;
	efn = fileread(packbuf, 1, 0x96640, efh);
	fileclose(&efh);
	if (!efn)
		return 0;
	for (efi = 0; efi < 999; efi++) {
		wsprintf(efname, T(0xa4da), efi);
		if (!exists(efname))
			break;
	}
	fopenw(&efh, efname);
	if (!efh)
		return 0;
	packbuf[0] = 'B';
	packbuf[1] = 'M';
	efn = filewrite(packbuf, efn, 1, efh);
	fileclose(&efh);
	if (!efn)
		return 0;
	return 1;
}

/* grabtile copies the 48x48 square at x, y of surface s into place k of the row at y 672 of surfm[0] */
void
grabtile(int k, int s, int x, int y)
{
	rc.left = x;
	rc.top = y;
	rc.right = x + 48;
	rc.bottom = y + 48;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	ddfailed("cangbk.bltf", IDirectDrawSurface_BltFast(surfm[0], k * 48, 0x2a0, surfs[s], &rc, DDBLTFAST_WAIT));
#else
	ddfailed("cangbk.bltf", surfm[0]->BltFast(k * 48, 0x2a0, surfs[s], &rc, DDBLTFAST_WAIT));
#endif
}

void
puttile3(int k, int idx)
{
	putsrc(4, k * 48, 0x2a0, 3, idx);
}

void
puttile1(int k, int idx)
{
	putsrc(4, k * 48, 0x2a0, 1, idx + 0xb6);
}

void
putm1b(int n, int x, int y, int idx)
{
	putsrc(n, x, y, 1, idx + 0xb6);
}

/* sqclip clips a 48x48 square at x, y to the playfield: sqx, sqy its place, sqcol, sqrow its first column and row, sqw, sqh its width and height, sqstate 0 whole, 1 clipped, 2 outside */
void
sqclip(int x, int y)
{
	sqstate = 0;
	sqcol = 0;
	sqw = 48;
	// PORT: the map of the wide view reaches the screen's edges, past the original's strips of 8 columns
#ifdef PORT
	{
		int x0, x1;

		x0 = sysscreen.left < 0 ? sysscreen.left - sysdrawx : 8;
		x1 = sysscreen.left < 0 ? sysscreen.right - sysdrawx : 632;
		if (x < x0) {
			if (x <= x0 - 48) {
				sqstate = 2;
				return;
			}
			sqcol = x0 - x;
			sqstate = 1;
			x = x0;
			sqw -= sqcol;
		} else if (x > x1 - 48) {
			if (x >= x1) {
				sqstate = 2;
				return;
			}
			sqstate = 1;
			sqw -= x - (x1 - 48);
		}
	}
#else
	if (x < 8) {
		if (x <= -40) {
			sqstate = 2;
			return;
		}
		sqcol = 8 - x;
		sqstate = 1;
		x = 8;
		sqw -= sqcol;
	} else if (x > 584) {
		if (x >= 632) {
			sqstate = 2;
			return;
		}
		sqstate = 1;
		sqw -= x - 584;
	}
#endif
	sqrow = 0;
	sqh = 48;
	if (y < 0) {
		if (y <= -48) {
			sqstate = 2;
			return;
		}
		sqrow = y;
		sqrow = -sqrow;
		sqstate = 1;
		sqh -= sqrow;
		y = 0;
	} else if (y > 432) {
		if (y >= 480) {
			sqstate = 2;
			return;
		}
		sqstate = 1;
		sqh -= y - 432;
	}
	sqx = x;
	sqy = y;
}

void
sqclear(void)
{
	rc2.left = sqx;
	rc2.top = sqy;
	rc2.right = rc2.left + sqw;
	rc2.bottom = rc2.top + sqh;
	fillfx.dwFillColor = 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("G_ps_swz.blt", IDirectDrawSurface_Blt(back, &rc2, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)))
#else
	while (ddfailed("G_ps_swz.blt", back->Blt(&rc2, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return;
}

void
sqclear48(void)
{
	rc2.left = sqx;
	rc2.top = sqy;
	rc2.right = rc2.left + 48;
	rc2.bottom = rc2.top + 48;
	fillfx.dwFillColor = 0;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("G_ps_swz.blt", IDirectDrawSurface_Blt(back, &rc2, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)))
#else
	while (ddfailed("G_ps_swz.blt", back->Blt(&rc2, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &fillfx)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return;
}

void
sqputm(unsigned n)
{
	srcrc.left = n % 13 * 48 + sqcol;
	srcrc.top = n / 13 * 48 + sqrow;
	tilepage = srcrc.top / 768;
	srcrc.top -= tilepage * 768;
	srcrc.right = srcrc.left + sqw;
	srcrc.bottom = srcrc.top + sqh;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("G_ps_m_cl.bltf", IDirectDrawSurface_BltFast(back, sqx, sqy, surfm[tilepage], &srcrc, DDBLTFAST_WAIT)))
#else
	while (ddfailed("G_ps_m_cl.bltf", back->BltFast(sqx, sqy, surfm[tilepage], &srcrc, DDBLTFAST_WAIT)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return;
}

void
sqputm48(unsigned n)
{
	srcrc.left = n % 13 * 48;
	srcrc.top = n / 13 * 48;
	tilepage = srcrc.top / 768;
	srcrc.top -= tilepage * 768;
	srcrc.right = srcrc.left + 48;
	srcrc.bottom = srcrc.top + 48;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("G_ps_m.bltf", IDirectDrawSurface_BltFast(back, sqx, sqy, surfm[tilepage], &srcrc, DDBLTFAST_WAIT)))
#else
	while (ddfailed("G_ps_m.bltf", back->BltFast(sqx, sqy, surfm[tilepage], &srcrc, DDBLTFAST_WAIT)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return;
}

void
sqputo(unsigned n)
{
	srcrc.left = n % 13 * 48 + sqcol;
	srcrc.top = n / 13 * 48 + sqrow;
	tilepage = srcrc.top / 768;
	srcrc.top -= tilepage * 768;
	srcrc.right = srcrc.left + sqw;
	srcrc.bottom = srcrc.top + sqh;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("G_ps_o_cl.bltf", IDirectDrawSurface_BltFast(back, sqx, sqy, surfo[tilepage], &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))
#else
	while (ddfailed("G_ps_o_cl.bltf", back->BltFast(sqx, sqy, surfo[tilepage], &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return;
}

void
sqputo48(unsigned n)
{
	srcrc.left = n % 13 * 48;
	srcrc.top = n / 13 * 48;
	tilepage = srcrc.top / 768;
	srcrc.top -= tilepage * 768;
	srcrc.right = srcrc.left + 48;
	srcrc.bottom = srcrc.top + 48;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("G_ps_o.bltf", IDirectDrawSurface_BltFast(back, sqx, sqy, surfo[tilepage], &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))
#else
	while (ddfailed("G_ps_o.bltf", back->BltFast(sqx, sqy, surfo[tilepage], &srcrc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return;
}

void
sqputo48o(unsigned n)
{
	srcrc.left = n % 13 * 48;
	srcrc.top = n / 13 * 48;
	tilepage = srcrc.top / 768;
	srcrc.top -= tilepage * 768;
	srcrc.right = srcrc.left + 48;
	srcrc.bottom = srcrc.top + 48;
	// PORT: DirectDraw through its C binding
#ifdef PORT
	while (ddfailed("G_ps_o(.bltf", IDirectDrawSurface_BltFast(back, sqx, sqy, surfo[tilepage], &srcrc, DDBLTFAST_WAIT)))
#else
	while (ddfailed("G_ps_o(.bltf", back->BltFast(sqx, sqy, surfo[tilepage], &srcrc, DDBLTFAST_WAIT)))
#endif
		if (ddlost == 0 || !restoresurfs())
			return;
}

/* sqputt copies the clipped square of the 48x48 picture src to back, black transparent */
void
sqputt(char *src)
{
	if (!lock(back))
		return;
	sqtdst = (unsigned char *)sd.lpSurface + sqx + sqy * sd.lPitch;
	src += sqrow * 48 + sqcol;
	for (sqty = 0; sqty < sqh; sqty++) {
		for (sqtx = 0; sqtx < sqw; sqtx++) {
			if (*src)
				*sqtdst++ = *src++;
			else {
				sqtdst++;
				src++;
			}
		}
		src += 48 - sqw;
		sqtdst += sd.lPitch - sqw;
	}
	unlock(back);
}

void
sqput48t(char *src)
{
	if (!lock(back))
		return;
	sq48dst = (unsigned char *)sd.lpSurface + sqx + sqy * sd.lPitch;
	for (sq48y = 0; sq48y < 48; sq48y++) {
		sq48x = 48;
		while (sq48x--) {
			if (*src != '\0')
				*sq48dst++ = *src++;
			else {
				sq48dst++;
				src++;
			}
		}
		sq48dst += sd.lPitch - 48;
	}
	unlock(back);
}

void
pagedone(void)
{
}

/* charpos finds c in the charset of the big fonts and puts the place of its glyph, w by h, in bigx, bigy */
int
charpos(char c, int w, int h)
{
	if (c >= 'a' && c <= 'z')
		c -= 0x20;
	for (cpp = bigchars, cpidx = 0; *cpp; cpidx++)
		if (*cpp++ == c)
			goto found;
	return 0;
found:
	bigx = cpidx % 10 * w;
	bigy = cpidx / 10 * h + 2;
	return 1;
}

void
loadbigfont(void)
{
	loadscr(0xfa1, (unsigned char *)workbuf, NULL, packbuf, 0x96640);
}

/* bigtext draws s in a big font of w by h glyphs from the picture font at x, y of surface n, up to a return */
void
bigtext(int n, int x, int y, char *s, unsigned char *font, int w, int h)
{
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	bgy = y;
	bgx0 = x;
	for (; *s; s++) {
		if (*s == 13) {
			bgy += h + 2;
			return;
		}
		if (*s == ' ')
			x += w;
		else if (charpos(*s, w, h)) {
			putblockt(n, x, y, font, bigx, bigy, w, h);
			x += w;
		}
	}
}

void
text32(int n, int x, int y, char *s)
{
	bigtext(n, x, y, s, (unsigned char *)workbuf, 0x20, 0x20);
}

void
text64(int n, int x, int y, char *s)
{
	bigtext(n, x, y, s, (unsigned char *)(workbuf + 0x4b000), 0x40, 0x40);
}

/* text32c draws s in the 32x32 font centered on x, or on the screen when x is 0 */
void
text32c(int n, int x, int y, char *s)
{
	for (t32n = 0; s[t32n] && !(s[t32n] == 13); t32n++)
		;
	if (x == 0)
		x = 0x140;
	text32(n, x - t32n * 32 / 2, y, s);
}

void
text64c(int n, int x, int y, char *s)
{
	for (t64n = 0; s[t64n] && !(s[t64n] == 13); t64n++)
		;
	if (x == 0)
		x = 0x140;
	text64(n, x - t64n * 64 / 2, y, s);
}

extern int lzwblockbuf[64];
extern int lzwstackbuf[1025];
extern int lzwsuffixbuf[3914];
extern int lzwprefixbuf[2505];
RECT frameleft = {
	0, 0, 56, 0x1e0
};
RECT frameright = {
	0x248, 0, 0x280, 0x1e0
};
RECT frametop = {
	56, 0, 0x248, 47
};
RECT framebottom = {
	56, 0x1b1, 0x248, 0x1e0
};
RECT sideleft = {
	0, 0, 8, 0x1e0
};
RECT sideright = {
	0x278, 0, 0x280, 0x1e0
};
int sheetrows[11] = {
	0x190, 0x1e0, 0x190, 0x190, 0x190, 0x190, 0x190, 0x190, 0x1e0
};
char *surfnames[9] = {
	texts + 42340, texts + 42344, texts + 42348, texts + 42351,
	texts + 42355, texts + 42359, texts + 42363, texts + 42367
};
char vidsurfs[40] = "";
char syssurfs[40] = "";
int optbright = 100;
int optcontrast = 100;
int optred = 100;
int optgreen = 100;
int optblue = 100;
short regiontag = 99;
short sunlight = 15;
short lastlight = 99;
short lightlvl = 99;
int pixtime = -1;
DWORD blittime = -1;
DWORD keyblittime = -1;
RECT tilerect = {
	0, 0, 48, 48
};
RECT tilerect2 = {
	0, 0, 48, 48
};
RECT fullrect = {
	0, 0, 0x280, 0x1e0
};
RECT fullrect2 = {
	0, 0, 0x280, 0x1e0
};
char vramtext[2000] = "";
char blittext[2000] = "";
// FIXED: the font code reaches past spritetext into outlinecols and ink3cols in WLINK's layout alone. Make the three one array.
#ifdef PORT
char spritetext[2008] = { [2000] = '^', 'v', 0, 0, 240, 241, 242, 243 };
#else
char spritetext[2000] = "";
char outlinecols[4] = "^v";
unsigned char ink3cols[4] = {
	240, 241, 242, 243
};
#endif
int textmode = 1;
RECT msbackrect = {
	0x120, 0, 0x150, 48
};
RECT ipbackrect = {
	0x154, 0x190, 0x16c, 0x258
};
RECT iprect = {
	8, 0x118, 32, 0x1e0
};
char dderrmsg[1504] = "";
unsigned char soldmark1[16] = {
	213, 21, 166, 67, 147, 56, 233, 122, 0, 0, 245, 246, 214, 183, 239,
	236
};
unsigned char *lzwblock = (unsigned char *)&lzwblockbuf;
unsigned char *lzwstack = (unsigned char *)&lzwstackbuf;
unsigned char *lzwsuffix = (unsigned char *)&lzwsuffixbuf;
unsigned short *lzwprefix = (unsigned short *)&lzwprefixbuf;
unsigned short lzwmask[16] = {
	0, 1, 3, 7, 15, 31, 63, 127, 255, 0x1ff, 0x3ff, 0x7ff, 0xfff, 0x1fff,
	0x3fff, 0x7fff
};
unsigned char inkcolor = 240;
int shadeon = 1;
unsigned char shadecolor = 244;
int setpics = 76;
int dir0frm6 = 6;
int dir0frm7 = 7;
int dir1frm0 = 8;
int dir1frm6 = 14;
int dir1frm7 = 15;
int dir2frm0 = 16;
int dir2frm6 = 22;
int dir2frm7 = 23;
int dir3frm0 = 24;
int dir3frm6 = 30;
int dir3frm7 = 31;
char bigchars[52] = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ.,;:-!?()+/\x84\x94\x81";
BOOL vblank;
int level;
int fadecount;
DWORD tmp;
unsigned char *palp;
int setpalcount;
unsigned char *palq;
DWORD tmp2;
unsigned char *lgsrc;
unsigned char *lgdst;
unsigned char lgmul;
int lgi;
int getpalcount;
unsigned char *btp;
int bti;
int btnext;
int btnext2;
int btprevr;
int btprevg;
int btprevb;
int btr;
int btg;
int btb;
int fsi;
unsigned char *psdst;
unsigned char *pssrc;
int psrow;
DDSURFACEDESC ddsd;
HRESULT psres;
int rsfi;
int unlocktries;
unsigned char *prdst;
unsigned char *prsrc;
int prrow;
unsigned char *ptdst;
unsigned char *ptsrc;
unsigned char ptpix;
int ptx;
int pty;
unsigned char *grdst;
unsigned char *grsrc;
int grrow;
int reldctries;
DDBLTFX mirrorfx;
int pscrrow;
char *pscrsrc;
char *pscrdst;
int gscrrow;
char *gscrdst;
char *gscrsrc;
int prowi;
unsigned char *prowsrc;
unsigned char *prowdst;
int pblkrow;
unsigned char *pblksrc;
unsigned char *pblkdst;
int pbtx;
int pbty;
unsigned char *pbtsrc;
unsigned char *pbtdst;
int cdx;
int cdy;
int cdr2;
int cdx2;
int cdy2;
int cdoff;
int sstx;
int ssty;
char *sstsrc;
char *sstdst;
int sprow;
char *spsrc;
unsigned char *spdst;
int sptx;
int spty;
char *sptsrc;
char *sptdst;
int g48x;
int g48y;
int g48any;
int p48tx;
int p48ty;
char *p48tdst;
int p48row;
unsigned char *p48dst;
int p48srow;
unsigned char *p48sdst;
int p48ptx;
int p48pty;
char *p48ptdst;
int g48row;
unsigned char *g48src;
int g48srow;
unsigned char *g48ssrc;
unsigned char *pwhdst;
int pwhrow;
char *pcldst;
int pclx;
int pcly;
char *psubdst;
int psubx;
int psuby;
unsigned char *gwhsrc;
int gwhrow;
unsigned char *gwhssrc;
int gwhsrow;
unsigned char *gbsrc;
int gbrow;
int textflen;
unsigned char *shp;
int shx;
int shy;
unsigned char *fbp;
int fbx;
int fby;
unsigned char *sh2p;
int sh2x;
int sh2y;
unsigned char *lap;
int lax;
int lay;
unsigned char gppix;
unsigned char gpspix;
int dlerrx;
int dlerry;
int dldx;
int dldy;
int dlsx;
int dlsy;
unsigned char *dlp;
int dli;
int dllen;
int cix;
int ciy;
int cir2;
unsigned char *ss2p;
unsigned char *ss2op;
unsigned char *ss4p;
unsigned char *pcolp;
int zoomw;
int zoomh;
DWORD zoomtime;
int meltcol;
int meltstep;
int meltdone;
int colh[80];
DWORD colrnd[160];
int meltrnd;
DWORD melttime;
int splitmid;
int splitrow;
int splitend;
int splithalf;
DWORD splittime;
int dvi;
int dvstep;
DWORD dvtime;
int dvtsteps;
unsigned char lzwch;
int lzwclear;
int lzwbits;
int lzwtop;
int lzwslot;
int lzwcode;
int lzwfc;
int lzwc;
int lzwoc;
int lzwnbits;
int lzwnavail;
int lzwx;
unsigned char *lzwp;
unsigned char *lzwend;
unsigned char *lzwsp;
unsigned char *lzwout;
int lzwcount;
DWORD uslen;
char *bmptr;
int lbi;
DWORD lbx;
int lby;
int mbi;
DWORD mbx;
int mby;
HANDLE lbfh;
DWORD lbfn;
HANDLE sbfh;
int sbi;
DWORD sbx;
int sby;
int sbok;
unsigned char sbr;
unsigned char sbg;
unsigned char sbb;
unsigned char sbbyte;
unsigned char sbrow[640];
char *ddinfop;
int ddinfon;
DWORD pxtime;
int pxcol;
int pxrow;
int blti;
int bltj;
DDSURFACEDESC smdesc;
DDSCAPS backcaps;
int ddi;
DWORD modetime;
int modew;
int modeh;
int modebpp;
int modetry;
int psover;
unsigned char *dcdst;
unsigned char *dcsrc;
unsigned char dcpix;
int dchx;
int dchy;
int dcw;
int dcpad;
int dtch;
int dtw;
unsigned char *dc2dst;
unsigned char *dc2src;
unsigned char dc2pix;
int dc2x;
int dc2y;
int twsum;
int dt2ch;
int dt2w;
unsigned char *g3dst;
unsigned char *g3src;
unsigned char *g3glyph;
unsigned char g3pix;
int g3x;
int g3y;
int g3rowinked;
int g3pitch;
int g3colinked[24];
int spqi;
int t3code;
int t3kind;
int t3w;
int t3pass;
int t3x0;
int t3y0;
int t3col;
char *t3p;
int t3ores;
int t3omode;
int t3bres;
int t3bmode;
int t3gres;
int t3colres;
int t3dkres;
int t3cgres;
int t3cdkres;
int t3ccres;
int tg2res;
int t3fres;
int irpage;
int iri;
int irj;
int pwow;
int pwoh;
int pwoi;
int susheet;
int susurf;
int suhalf;
int sualt;
char *tinfop;
int tbx;
int tby;
int tmi;
int slerrx;
int slerry;
int sldx;
int sldy;
int slsx;
int slsy;
unsigned char *slp;
unsigned char *sllast;
int sli;
int sllen;
unsigned char *sr8p;
unsigned char sr8c;
int sr8x;
unsigned char *sr7p;
unsigned char sr7c;
int sr7x;
unsigned char *sr8bp;
unsigned char sr8bc;
int sr8bx;
unsigned char *sr9p;
int sr9x;
unsigned char *ss9p;
int ss9x;
int ss9y;
unsigned char *ssap;
int ssax;
int ssay;
unsigned char *rrdst;
unsigned char *rrsrc;
int rrx;
int plrr;
int plrw;
int plry;
int plrr2;
int ps8r;
int ps8w;
int ps8y;
int ps8r2;
int ps9r;
int ps9w;
int ps9y;
int ps9r2;
int ringr;
int ringw;
int ringy;
int ringr2;
int ringpx;
int ringpy;
char *btnp;
int baleft;
int baright;
int pgicon;
int wf2mid;
DWORD lfidx;
DWORD lfrun;
DWORD lfi;
unsigned char *lfover;
unsigned char *lfoutp;
int fscol0;
int fscol;
int fsw;
int fsrow;
int ftmi;
unsigned char *ftmp;
char efname[20];
HANDLE efh;
DWORD efn;
int efi;
int sqtx;
int sqty;
unsigned char *sqtdst;
int sq48x;
int sq48y;
unsigned char *sq48dst;
DWORD cpidx;
char *cpp;
int bgx0;
int bgy;
int t32n;
int t64n;
int nmaptiles;
int nobjtiles;
int ncurtiles;
LPDIRECTDRAW dd;
DDCAPS ddcaps;
LPDIRECTDRAWSURFACE back;
LPDIRECTDRAWSURFACE prim;
LPDIRECTDRAWSURFACE surf2;
LPDIRECTDRAWSURFACE surfc;
LPDIRECTDRAWSURFACE surfm[7];
LPDIRECTDRAWSURFACE surfo[13];
LPDIRECTDRAWSURFACE surfw[5];
int surfwh[5];
LPDIRECTDRAWSURFACE surfpg;
LPDIRECTDRAWSURFACE surfs[8];
DWORD vidmem;
DWORD sysmem;
unsigned char surfwhere[8];
LPDIRECTDRAWSURFACE cursurf;
LPDIRECTDRAWPALETTE ddpal;
PALETTEENTRY pe[256];
unsigned char palblack;
char gamepal[768];
unsigned char lighter[256];
unsigned char darker[256];
unsigned char lighter2[256];
unsigned char darker2[256];
unsigned char darker4[256];
unsigned char lum[256];
unsigned char pals[10][768];
short lastwarm;
short lightwarm;
short sensesleft;
int palavg;
char scrbuf[309700];
char printbuf[4800];
unsigned char tilebuf[48][48];
unsigned char sbpal[768];
int surfslost;
short gfxram;
char optg[1000];
DDSURFACEDESC sd;
HDC surfdc;
RECT rc2;
RECT rc;
int fxunused[26];
DDBLTFX fillfx;
int modeset;
int ddready;
unsigned char invram[8];
int text3x;
int text3y;
int spacestop;
int boldtext;
int capsfont;
int text3width;
unsigned char *drawbuf;
HGDIOBJ textfont;
RECT srcrects[222];
unsigned char srcpages[222];
int spriteshape;
int spritex;
int spritey;
int msx;
int msy;
int mscx;
int mscy;
int msw;
int msh;
HRESULT ddres[2];
int ddlost;
int ddbusy;
int linestepx;
int linestepy;
int linesteps;
int lserrx;
int lserry;
int lsdx;
int lsdy;
int lssx;
int lssy;
int lsmax;
unsigned char *lzwsrc;
int lzwsize;
unsigned char *lzwpos;
unsigned char *lzwdst;
unsigned lzwoff;
int lzwplanar;
unsigned char lzwbuf[1024];
BITMAPFILEHEADER bmfh;
BITMAPINFOHEADER bmih;
int photomagic;
LPDIRECTDRAWSURFACE srcsurf;
int srcw;
int srch;
int tilepage;
int srcunused;
RECT srcrc;
DWORD srcflags;
Spq spq[10];
int nspq;
unsigned char *tilecache;
DWORD ntiles;
int minmem;
int nopani;
int nopequip;
int minsets;
int nopvari;
int noprun;
int dir0frm0;
unsigned char *gearbank;
int hasgearbank;
char banktile[2304];
char grassmask[175104];
char *grassbody;
char *gearbody;
unsigned char btncol;
int btnx;
int btny;
int btnfull;
int nofani;
char heroface[12960];
char heroface2[12960];
unsigned char headicons[8][2304];
char curface[12960];
char curface2[12960];
char talkpic[12960];
char faceback[12960];
unsigned char *facecache[57];
DWORD facesizes[57];
unsigned char *overcache[57];
DWORD oversizes[57];
int overonly;
short shadowleft;
HGLOBAL travel0;
int travel0len;
HGLOBAL travel1;
int travel1len;
HGLOBAL travel2;
int travel2len;
HGLOBAL travel3;
int travel3len;
DWORD myfacesum;
int sqx;
int sqy;
int sqcol;
int sqw;
int sqrow;
int sqh;
int sqstate;
int bigx;
int bigy;

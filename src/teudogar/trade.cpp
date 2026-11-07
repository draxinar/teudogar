#include <windows.h>
#include <string.h>

#include "base.h"

typedef struct Unit Unit;
typedef struct Metal Metal;
typedef struct Gear Gear;

/* 0x1b8 bytes each (009bc240) */
struct Unit {
	char f0[0x13e];
	unsigned char drunk;	/* prices 20 percent off from 90 up, 10 percent below */
	char f13f[0x146 - 0x13f];
	signed char sympathy;
	char f147[0x1b8 - 0x147];
};

/* 0x1b bytes each (004c08a8): kinds of goods */
struct Metal {
	char f0[0x12];
	short days;
	short metal;	/* material: 1 metal */
	char f16[0x1b - 0x16];
};

/* 0x1a bytes each (004c0bd8): kinds of food */
struct Gear {
	char f0[0x13];
	short days;
	short metal;
	char f17[0x1a - 0x17];
};

unsigned long valueof(unsigned long item);
unsigned long valuefor(unsigned long item, Unit *u);
unsigned long charmprice(unsigned long v);
short myservant(Unit *u);
unsigned long silvprice(unsigned long n);
unsigned long roundprice(unsigned long n);
int repaircost(unsigned long item);
short repairable(unsigned long item);
unsigned char classof(int i);
unsigned short gearof(unsigned long item);
unsigned short wearof(unsigned long item);
void brandnew(char *dst, unsigned long item, short n);
int wrap(char *s, int width);
void clrback(void);
void winframe(int x, int y, int w, int h, int keyed);
void winframe2(int x, int y, int w, int h);
void lookdone(void);
unsigned long baselook(unsigned long item);
void nop(void);
int fadefast(int in);
void usepal(unsigned char n);
void cleartexts(void);
void resetanims(void);
void resetpose(Unit *u, unsigned char v);
void entermenu(void);
void savepanels(void);
void homepanels(void);
void openpanel(unsigned char n);
void savescr(void);
void shadesaved9(int x0, int y0, int x1, int y1);
void drawscreen(void);
int music(int n, int lp);
int __cdecl readinput(void);
int edgemenus(short mode);
int clicktarget(short m);
short showlabel(void);
int tipshown(void);
void hoversound(void);
void clicksound(void);
void fidget(void);
void stepanims(void);
void peektarget(void);
void clickhover(void);
int picktarget(void);
int dragclick(void);
short notepoison(short n);
unsigned long giveitem(short n, unsigned long item, short mode);
int checkpoison(short n);
void restorepanels(void);

extern Unit units[];
extern char *strs[];
extern unsigned short curunit;
extern Metal metals[];
extern Gear armors[];
extern char speech[];
extern int dutalk;
extern short freetrade;
extern int lang;
extern char trademode;
extern unsigned long tradegoods[24];
extern unsigned long tradeoffer[24];
extern char picked[24];
extern DWORD goodsprice;
extern DWORD offerprice;
extern DWORD goodsbase;
extern DWORD offerbase;
extern DWORD nforsale;
extern DWORD pricemarkup;
extern short repairmat;
extern short lastlight;
extern short dealok;
extern int playmode;
extern int mousemoved;
extern unsigned char panelstack[16];
extern int cur;
extern int mouseleft;
extern int mouseright;
extern int leftclick;
extern int rightclick;
extern unsigned char keys[256];
extern unsigned char esckey;
extern int hoverkind;
extern int hoverx;
extern int hovery;
extern unsigned long targetitem;
extern unsigned char targetkind;
extern short lblx;
extern short lbly;
extern int cheat;
extern DWORD walkdue;
extern DWORD standdue;
extern unsigned short walkdelay;
extern unsigned short standdelay;
extern int targetx;

extern int tcostslot;
extern int tcostmat;
extern unsigned long tcostvalue;
extern unsigned char tcostclass;
extern int offerslot;
extern unsigned long offerworth;
extern int tradepick;
extern short armed;
extern int savedmode;
extern DWORD tiptime;
extern char musicon;
extern short redraw;
extern short frame;
extern unsigned long tipitem;
extern short hotbutton;
extern unsigned long waresilv;
extern unsigned lockedslots;
extern int tradedone;
extern short lastbutton;

/* the price of the marked goods, or the smith's answer when repairing */
void
tradecost(void)
{
	waresilv = 0;
	for (tcostslot = 0; tcostslot < 24; tcostslot++)
		if (tradegoods[tcostslot] == 0)
			picked[tcostslot] = 0;
	if (nforsale == 1 && tradegoods[23] != 0)
		picked[23] = 1;
	if (trademode <= 2) {
		goodsprice = 0;
		goodsbase = 0;
		for (tcostslot = 0; tcostslot < 24; tcostslot++) {
			if (picked[tcostslot]) {
				tcostvalue = valueof(tradegoods[tcostslot]);
				if (!curunit || pricemarkup)
					goodsprice += tcostvalue;
				else
					goodsprice += valuefor(tradegoods[tcostslot], &units[curunit]);
				goodsbase += tcostvalue;
			}
		}
		goodsprice *= pricemarkup + 100;
		goodsprice /= 100;
		goodsprice = charmprice(goodsprice);
		if (curunit) {
			if (units[curunit].drunk >= 90 && goodsprice < 10000) {
				goodsprice *= 80;
				goodsprice /= 100;
				if (!goodsprice)
					goodsprice++;
			} else if (units[curunit].drunk && goodsprice < 15000) {
				goodsprice *= 90;
				goodsprice /= 100;
				if (!goodsprice)
					goodsprice++;
			}
		}
		if (goodsprice && curunit && myservant(&units[curunit]))
			goodsprice = 1;
		if (nforsale == 1)
			waresilv = silvprice(goodsprice);
	} else if (trademode == 3) {
		goodsbase = goodsprice;
		if (goodsprice && curunit && myservant(&units[curunit]))
			goodsprice = 1;
	} else if (trademode == 4) {
		memset(tradegoods, 0, 0x60);
		goodsprice = 0;
		tcostslot = 23;
		offerprice = 23;
		for (; tcostslot >= 0; tcostslot--) {
			if (tradeoffer[tcostslot] != 0) {
				if (!repairable(tradeoffer[tcostslot])) {
					wsprintf(speech, strs[1119]);
					goto done;
				}
				tcostclass = classof(tradeoffer[tcostslot] & 0xfff);
				if (tcostclass == 0x3c || tcostclass == 0x49)
					tcostmat = metals[gearof(tradeoffer[tcostslot])].metal;
				else
					tcostmat = armors[gearof(tradeoffer[tcostslot])].metal;
				if (tcostmat && repairmat != tcostmat) {
					if (tcostmat == 1)
						wsprintf(speech, strs[1120]);
					else
						wsprintf(speech, strs[1121]);
					goto done;
				}
				if (tcostmat == 0 && repairmat == 1) {
					wsprintf(speech, strs[1122]);
					goto done;
				}
				if (!(tcostclass == 0x49 || tcostclass == 0x4a) && wearof(tradeoffer[tcostslot]) < 3) {
					brandnew(speech, tradeoffer[tcostslot], dutalk);
					if (lang)
						lowerstr(speech + 1);
					goto done;
				}
				tradegoods[offerprice--] = tradeoffer[tcostslot];
				goodsprice += repaircost(tradeoffer[tcostslot]);
			}
		}
		if (offerprice < 17) {
			wsprintf(speech, strs[1132]);
		done:
			goodsprice = 0;
			goodsbase = 0;
			wrap(speech, 0xa4);
			return;
		}
		goodsbase = goodsprice;
		if (goodsprice) {
			goodsprice = charmprice(goodsprice);
			if (curunit) {
				if (units[curunit].drunk >= 90 && goodsprice < 10000) {
					goodsprice *= 80;
					goodsprice /= 100;
					if (!goodsprice)
						goodsprice++;
				} else if (units[curunit].drunk && goodsprice < 15000) {
					goodsprice *= 90;
					goodsprice /= 100;
					if (!goodsprice)
						goodsprice++;
				}
			}
			goodsprice = roundprice(goodsprice);
			if (curunit && (myservant(&units[curunit]) || freetrade))
				goodsprice = 1;
			tcostmat = goodsprice / 10;
			if (goodsprice % 10)
				tcostmat++;
			if (tcostmat == 0)
				tcostmat++;
			if (curunit && (myservant(&units[curunit]) || freetrade))
				wsprintf(speech, strs[offerprice > 17 ? 0x46a : 0x46e]);
			else
				wsprintf(speech, strs[offerprice > 17 ? 0x469 : 0x46d], tcostmat);
		} else
			wsprintf(speech, strs[1131]);
		wrap(speech, 0xa4);
	} else {
		goodsprice = 0;
		goodsbase = 0;
	}
}

/* the value of the goods the player offers */
void
offervalue(void)
{
	offerprice = 0;
	offerbase = 0;
	for (offerslot = 0; offerslot < 24; offerslot++) {
		offerworth = valueof(tradeoffer[offerslot]);
		if (curunit)
			offerprice += valuefor(tradeoffer[offerslot], &units[curunit]);
		else
			offerprice += offerworth;
		offerbase += offerworth;
	}
	if (curunit) {
		if (units[curunit].drunk >= 90 && goodsprice < 10000) {
			offerprice *= 120;
			offerprice /= 100;
		} else if (units[curunit].drunk && goodsprice < 15000) {
			offerprice *= 110;
			offerprice /= 100;
		}
	}
	if (curunit && (myservant(&units[curunit]) || freetrade))
		offerprice = 0xffff;
}

void
tradeframe(void)
{
	clrback();
	winframe(8, 0, 0xc0, 0x1e0, 0);
	winframe2(0xd4, 0, 0xc0, 0x1e0);
}

/* the trade screen: 0 when no trade is set up, else 1 when it closes */
int
trade(void)
{
	musicon = 0;
	redraw = 1;
	frame = 0;
	for (tradepick = 0; tradepick < 24; tradepick++)
		if (tradegoods[tradepick])
			tradegoods[tradepick] = baselook(tradegoods[tradepick]);
	armed = 0;
	tiptime = 0;
	tipitem = 0;
	tradedone = 0;
	dealok = 0;
	if (!trademode)
		return 0;
	nop();
	fadefast(0);
	usepal(0);
	cleartexts();
	resetanims();
	resetpose(units, 2);
	savedmode = playmode;
	playmode = 1;
	entermenu();
	mousemoved = 1;
	memset(panelstack, 0xff, 0x10);
	savepanels();
	homepanels();
	openpanel(1);
	openpanel(3);
	memset(tradeoffer, 0, 0x60);
	memset(picked, 0, 0x18);
	mousemoved = 1;
	lastbutton = 0xff;
	hotbutton = 0;
	tradeframe();
	savescr();
	shadesaved9(0x10, 8, 0xb7, 0x1cf);
	shadesaved9(0xdc, 8, 0x183, 0x1cf);
again:
	tradecost();
	offervalue();
	drawscreen();
	fadefast(1);
	if (!musicon) {
		musicon = 1;
		if (cur != 0x1a && cur != 0x16 && cur != 4 && cur != 0x15)
			music(13, 0);
	}
	while (mouseleft || mouseright)
		readinput();
	armed = 0;
	rightclick = 0;
	leftclick = 0;
	hotbutton = 0;
	lastbutton = 0xff;
	for (;;) {
		readinput();
		edgemenus(2);
		if (mouseleft || mouseright || (keys[VK_CONTROL] & 0x80)
		    || !(hoverkind == 0x29 || hoverkind == 0x2b || hoverkind == 0xd9 || hoverkind == 0xda)
		    || !(picktarget() == 1))
			targetitem = 0;
		if (targetitem != tipitem) {
			cleartexts();
			if (targetitem) {
				if (!clicktarget(2) && showlabel())
					redraw = 1;
				tiptime = ticks();
			}
			redraw = 1;
			tipitem = targetitem;
		}
		if (tipshown() && (ticks() >= tiptime + 4000
		    || ticks() >= tiptime + 1000
		    && (lblx - 30 > hoverx || lblx + 30 < hoverx
		    || lbly - 20 > hovery || lbly + 40 < hovery))) {
			cleartexts();
			redraw = 1;
		}
		lastbutton = hotbutton;
		hotbutton = 0;
		if (hoverkind == 0xdb) {
			if (trademode == 4) {
				if (goodsprice)
					hotbutton = 1;
			} else if (cheat && (keys['W'] & 0x80)
			    || goodsprice && offerprice && !(goodsprice > offerprice))
				hotbutton = 1;
		} else if (hoverkind == 0xdc)
			hotbutton = 2;
		if (hotbutton != lastbutton) {
			if (hotbutton == 1 || hotbutton == 2)
				hoversound();
			redraw = 1;
		}
		if (!(ticks() < standdue)) {
			walkdue = ticks() + walkdelay;
			standdue = ticks() + standdelay;
			frame++;
			if (frame == 24) {
				frame = 0;
				fidget();
			}
			if ((frame & 3) == 0) {
				stepanims();
				redraw = 1;
			}
		}
		if (redraw) {
			drawscreen();
			redraw = 0;
		}
		if (armed && !mouseleft) {
			armed = 0;
			if (hotbutton)
				goto leave;
		}
		if (keys[VK_CONTROL] & 0x80)
			peektarget();
		if (!mouseleft && !leftclick)
			goto idle;
		readinput();
		clickhover();
		if ((keys[VK_SHIFT] & 0x80) && (hoverkind == 0xda || hoverkind == 0x29 || hoverkind == 0x2b)
		    && !((tradepick = picktarget()) == 0 || tradepick == 3)) {
			clicktarget(4);
			goto again;
		}
		if (hotbutton) {
			armed = 1;
			continue;
		}
		if (hoverkind == 0x29 || hoverkind == 0x2b || hoverkind == 0xd9 || hoverkind == 0xda) {
			tradepick = picktarget();
			if (tipshown()) {
				cleartexts();
				redraw = 1;
			}
			if (mouseleft)
				dragclick();
			if (hoverkind == 0xd9 && trademode != 3 && trademode != 4) {
				picktarget();
				if (targetx != 999) {
					if (picked[targetx])
						picked[targetx] = 0;
					else
						picked[targetx] = 1;
					hoversound();
				}
			}
			tradepick = picktarget();
			if (tradepick != 0 && tradepick != 3
			    && (!(targetkind == 0xd9 && targetx != 999 && !picked[targetx])
			    || trademode == 3 || (trademode == 1 || trademode == 2) && nforsale == 1)
			    && clicktarget(2) == 0) {
				if (showlabel())
					redraw = 1;
				tiptime = ticks();
			}
			goto again;
		}
		while (mouseleft)
			readinput();
		leftclick = 0;
	idle:
		if (esckey || mouseright || rightclick) {
			lastbutton = 0xff;
			hotbutton = 0;
			goto leave;
		}
	}
leave:
	tradedone = 1;
	if (trademode != 4)
		goodsprice = 0;
	if (hotbutton == 1) {
		clicksound();
		dealok = 1;
		if (trademode == 1 || trademode == 2 || trademode == 3) {
			notepoison(curunit);
			for (tradepick = 0; tradepick < 24; tradepick++)
				if (tradeoffer[tradepick])
					giveitem(curunit, tradeoffer[tradepick], 1);
			checkpoison(curunit);
		}
		if (trademode == 1) {
			for (tradepick = 0; tradepick < 24; tradepick++) {
				tradeoffer[tradepick] = 0;
				if (picked[tradepick]) {
					giveitem(0, tradegoods[tradepick], 1);
					tradegoods[tradepick] = 0;
					picked[tradepick] = 0;
				}
			}
			if (curunit && myservant(&units[curunit]) && offerbase == 0) {
				if (goodsbase / 10 >= 50) {
					if (units[curunit].sympathy >= 3)
						units[curunit].sympathy--;
				} else if (goodsbase / 10 >= 5 && units[curunit].sympathy == 3)
					units[curunit].sympathy = 2;
			}
		} else if (trademode == 2) {
			memset(tradeoffer, 0, 0x60);
			drawscreen();
			memcpy(tradeoffer, tradegoods, 0x60);
			memset(tradegoods, 0, 0x60);
			for (tradepick = 0; tradepick < 24; tradepick++)
				if (picked[tradepick]) {
					tradegoods[goodsprice++] = tradeoffer[tradepick];
					picked[tradepick] = 0;
				}
			goto wait;
		} else
			goto wait;
	} else {
		if (hotbutton == 2)
			clicksound();
		for (tradepick = 0; tradepick < 24; tradepick++)
			if (tradeoffer[tradepick])
				giveitem(0, tradeoffer[tradepick], 1);
	}
	drawscreen();
wait:
	while (mouseleft)
		readinput();
	leftclick = 0;
	fadefast(0);
	nop();
	cleartexts();
	restorepanels();
	memset(panelstack, 0xff, 0x10);
	playmode = savedmode;
	return 1;
}

void
endtrade(void)
{
	freetrade = 0;
	trademode = 0;
	lastlight = 0xff;
	lockedslots = 0;
	lookdone();
}

short lastbutton = 0xff;	/* 004c35cc */
int tcostslot;
int tcostmat;
unsigned long tcostvalue;
unsigned char tcostclass;
int offerslot;
unsigned long offerworth;
int tradepick;
short armed;	/* 009a1e6c: a button went down, act when the mouse is released */
int savedmode;
DWORD tiptime;	/* 009a1e74: ticks when tipitem changed */
char musicon;	/* 009a1e78: the trade music (track 13) has started */
short redraw;	/* 009a1e7a */
short frame;	/* 009a1e7c: animation frame, 0 to 23 */
unsigned long tipitem;	/* 009a1e80: the item under the cursor */
short hotbutton;	/* 009a1e84: the button under the cursor: 1 deal, 2 cancel */
unsigned long waresilv;
unsigned lockedslots;
int tradedone;

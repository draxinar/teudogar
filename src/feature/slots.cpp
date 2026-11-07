// FEATURE: slots: 23 slots of saved games and three AutoSaves, in three pages of the menus of the slots

/*
 * The pages of the slots: the menus of the saved games show the
 * original's ten rows on each of three pages, the slots A to I, K to S,
 * and T to Z, with the AutoSave J in the tenth row of the load menu. A
 * slot is Teudogar_<letter>.SAV, as the original's nine are, so that
 * saveslot, loadslot, and the messages of a save and a load take K to Z
 * as they stand. Y and Z hold the two AutoSaves before J: a journey over
 * the travel map moves Y to Z and packs J into Y as a slot, with the
 * date of Teudogar_J.SAV, before J takes the journey's start (travelmap,
 * world.cpp). The load menu shows them as AutoSave, and the save menu
 * hides them. The page's edges turn the pages, as those of the
 * encyclopedia's index do (textpage, mainmenu.cpp), and so do the keys
 * Left and Right, as in the journal. The page's number shows below
 * Back. A save takes a note, which the player types into the row as an
 * automap's label, and which the row then shows in place of the hero's
 * name: it lives in the bytes 0x19 to 0x2c of the description, which
 * the original leaves zero and reads past. A save over the game of
 * another hero asks first, as a new game does. Shift+S saves into the
 * last slot saved or loaded without the menu, as Shift+L loads it.
 * slotmenu and loadsave (mainmenu.cpp), keycmd (input.cpp), and
 * travelmap (world.cpp) call the functions here.
 */

#include <windows.h>
#include <string.h>
#include "base.h"

#ifndef FEATURE_SLOTS
#error slots.cpp is the module of the feature slots: FEATURE_SLOTS is missing
#endif

typedef struct Rect Rect;
typedef struct Menu Menu;

/* a menu item's rectangle, 8 bytes */
struct Rect {
	unsigned short x;
	unsigned short y;
	unsigned short w;
	unsigned short h;
};

/* a menu of up to 40 items, numbered from 1 in sel, 0x1f7 bytes */
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

enum {
	Npages = 3,
	Lastrow = 10,	/* the AutoSave's row, the last of a page */
	Autoslot = 10,	/* J, the AutoSave */
	Yslot = 25,	/* Y, the AutoSave before J */
	Zslot = 26,	/* Z, the AutoSave before Y, and the last slot */
	Backitem = 10,	/* the index of Back, the item after the rows, which runmenu numbers 11 */
	Leftitem = 11,	/* the page's left edge, after Back */
	Rightitem = 12,	/* its right edge */
	Edgew = 0x6a,	/* an edge's width, as textpage's */
	Screenw = 640,
	Screenh = 480,
	Arrowy = 0xd8,	/* the y of an edge's arrow, as textpage's */
	Leftarrow = 0x38,	/* the cursor sprites of the arrows */
	Rightarrow = 0x39,
	Backkey = 0x32,	/* M, Back's key in the original's load menu */
	Leftkey = 0x4b,
	Rightkey = 0x4d,
	Rowtexts = 0x3e8,	/* workbuf's texts of the rows, 200 bytes a row */
	Rowtextlen = 200,
	Rowy = 0x83,	/* the y of the first row, as slotmenu lays it */
	Rowh = 30,	/* a row's pitch */
	Textdy = 3,	/* the y of a row's text in it, as button places it */
	Desclen = 0x50,	/* the description that opens a slot */
	Namelen = 0x14,	/* the hero's name in it */
	Noteoff = 0x19,	/* the note in it */
	Notelen = 19,	/* a note's characters, as many as a label of the automap */
	Notew = 0xb8,	/* a note's pixels, as many as begingame gives the hero's name, which leave the row room for its date */
	Asklen = 200,	/* the question before a save over another hero, as composed */
	Slotname = 0xa492,	/* the texts' Teudogar_%c.SAV, a slot's file */
	Autogame = 0xa46a,	/* E0000000.DAT, the AutoSave's saved game */
	Automap = 0xa477,	/* E%04d%03d.DAT, the AutoSave's records of a map */
	Autophoto = 0xa485,	/* E099999P.DAT, the AutoSave's PHOTO.DAT */
	Photo = 0x9bfd,	/* PHOTO.DAT, the template of the player's own portrait */
	Nmaps = 80,
	Facever = 0xf0f,	/* the first version of a slot that holds the face apart, and the item word */
	Sumver = 0xf10,	/* the first that holds the sum of the portrait */
	Ownface = 4,	/* the face of the player's own portrait, whose first tile the header holds */
	Facelen = 0x900,	/* that tile, 48 by 48 */
	Headmax = Desclen + 4 + Facelen + 8,	/* a slot's header with the player's own face, as readhdr reads it */
	Partmax = 0x96640	/* a file of a slot, as saveslot reads it into workbuf */
};

extern Menu menu0;
extern unsigned short sloti;
extern char workbuf[];
extern char texts[47942];
extern int lang;
extern int autolevel;
extern unsigned short lastslot;
extern short autoslot;
int exists(char *pattern);
int hasautosave(void);
short confirm(char *s);
void putcs(int x, int y, unsigned n);
void pagenum(int page, int pages);
void slotmenu(char load);
int flip(void);
int inputtext(short x, short y, char *s, short max, char mode, char flag, char toup, char filter, char cursor, short nfields, short maxw, short maxh, Menu *m);

short slotspage;	/* the page shown, 0 to 2, kept from menu to menu */
short slotsdir;	/* the turn that loadsave shows: 0 none, 1 back, 2 forward */
short slotsblank;	/* the row that slotmenu draws without its text, for a note's input, or 0 */
short slotsmirror;	/* the slot whose game the AutoSave holds since a load of this run copied it there (loadslot), or 0 */
static char notebuf[Notelen + 1];	/* the note of the next save, empty for none */

#ifdef FEATURE_FIXES
extern char slotfailed;	/* whether the last save or load of a slot failed, which safeslot reports (mainmenu.cpp) */
#else
static char slotfailed;	/* 0, as the original's saveslot reports success whatever its writes did */
#endif

/* the first slot of each page, and their count */
static unsigned char firsts[Npages] = { 1, 11, 20 };
static unsigned char counts[Npages] = { 9, 9, 7 };

/*
 * the scan code of each slot's letter, from slot 1, A: the place of
 * its key, as keys.cpp reads it. Five letters take 0: S and L close the
 * menu, M is Back, hotkey (menus.cpp) makes an item of Y answer Z, J,
 * O, S, and T too, and a German keyboard swaps the places of Y and Z.
 */
static unsigned char letterkeys[27] = {
	0,
	0x1e, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24,
	0x25, 0, 0, 0x31, 0x18, 0x19, 0x10, 0x13, 0,
	0x14, 0x16, 0x2f, 0x11, 0x2d, 0, 0
};

/* slotsat returns the slot of a row of the page shown, or 0 for a row without one */
short
slotsat(short row)
{
	if (row == Lastrow)
		return Autoslot;
	if (row >= 1 && row <= counts[slotspage])
		return firsts[slotspage] + row - 1;
	return 0;
}

/* slotschar returns the letter of the slot of the row sloti */
char
slotschar(void)
{
	return (char)(slotsat(sloti) + 0x40);
}

/* isauto reports whether slot is an AutoSave: J, Y, or Z */
static int
isauto(short slot)
{
	return slot == Autoslot || slot == Yslot || slot == Zslot;
}

/* slotsauto reports whether the row shows an AutoSave, which reads AutoSave in place of its hero's name */
short
slotsauto(short row)
{
	return isauto(slotsat(row));
}

/*
 * slotshide hides the row sloti when it shows no slot: a row past the
 * page's slots, an AutoSave in the save menu, or one of a load menu
 * without its file. It reports whether it did.
 */
short
slotshide(char load)
{
	char name[16];
	short slot;
	Rect *r;

	slot = slotsat(sloti);
	if (slot != 0 && !isauto(slot))
		return 0;
	if (slot != 0 && load) {
		wsprintf(name, texts + Slotname, slot + 0x40);
		if (exists(name))
			return 0;
	}
	r = &menu0.rects[sloti - 1];
	r->x = 0;
	r->y = 0;
	r->w = 0;
	r->h = 0;
	menu0.enabled[sloti - 1] = 0;
	workbuf[Rowtexts + sloti * Rowtextlen] = 0;
	return 1;
}

/*
 * slotskeys gives the page's items their keys: each row the letter of
 * its slot, Back M, as in the original's load menu, and the edges Left
 * and Right, as the journal turns its pages
 */
void
slotskeys(void)
{
	short row;

	for (row = 1; row <= Lastrow; row++)
		menu0.keys[row - 1] = letterkeys[slotsat(row)];
	menu0.keys[Backitem] = Backkey;
	menu0.keys[Leftitem] = Leftkey;
	menu0.keys[Rightitem] = Rightkey;
}

/* edge makes item i the edge at x, on when a page lies that way, with its arrow */
static void
edge(short i, short x, short on, short arrowx, short arrow)
{
	Rect *r;

	r = &menu0.rects[i];
	r->x = x;
	r->y = 0;
	r->w = Edgew;
	r->h = Screenh;
	menu0.enabled[i] = (unsigned char)on;
	if (on)
		putcs(arrowx, Arrowy, arrow);
}

/*
 * slotsedges adds the page's edges as the items after Back, as textpage
 * adds those of the encyclopedia's index, and shows the page's number
 * below Back
 */
void
slotsedges(void)
{
	edge(Leftitem, 0, slotspage > 0, 0x18, Leftarrow);
	edge(Rightitem, Screenw - Edgew, slotspage < Npages - 1, 0x246, Rightarrow);
	menu0.n += 2;
	pagenum(slotspage, Npages);
}

/* slotsflip turns the page when sel is an edge's item, and reports whether it did */
short
slotsflip(short sel)
{
	if (sel == Leftitem + 1 && slotspage > 0) {
		slotspage--;
		slotsdir = 1;
		return 1;
	}
	if (sel == Rightitem + 1 && slotspage < Npages - 1) {
		slotspage++;
		slotsdir = 2;
		return 1;
	}
	return 0;
}

/* copyn copies at most n characters of s into d, which takes n + 1 bytes, and ends them with a NUL */
static void
copyn(char *d, const char *s, int n)
{
	int i;

	for (i = 0; i < n && s[i] != 0; i++)
		d[i] = s[i];
	d[i] = 0;
}

/* readhead reads the description of slot's file into head, Desclen bytes, and reports whether it could */
static int
readhead(short slot, char *head)
{
	char name[16];
	HANDLE h;
	int ok;

	memset(head, 0, Desclen);
	wsprintf(name, texts + Slotname, slot + 0x40);
	if (!fopenr(&h, name))
		return 0;
	ok = fileread(head, 1, Desclen, h) == Desclen;
	fileclose(&h);
	head[Desclen - 1] = 0;
	return ok;
}

/* askformat returns the question before a save over another hero's game, in the current language, with %s for that hero's name */
static char *
askformat(void)
{
	switch (lang) {
	case 0:
		return "Wollen Sie wirklich den Spielstand von %s \xfc" "berschreiben?";
#ifdef FEATURE_FRENCH
	case 2:
		return "Voulez-vous vraiment \xe9" "craser la sauvegarde de %s\xa0?";
#endif
	case 4:
		return "Vuoi davvero sovrascrivere lo status partita di %s?";
	default:
		return "Do you really want to overwrite the game status of %s?";
	}
}

/* copyfmt copies at most n characters of s into d, which takes 2n + 1 bytes, with each % doubled for wsprintf, which confirm hands its text to as a format */
static void
copyfmt(char *d, const char *s, int n)
{
	int i;

	for (i = 0; i < n && s[i] != 0; i++) {
		if (s[i] == '%')
			*d++ = '%';
		*d++ = s[i];
	}
	*d = 0;
}

/*
 * slotsask asks before a save into the row of the save menu when its
 * slot holds the game of another hero than hero, as the game asks
 * before a new game. It reports whether the save goes on: at once for
 * a slot that is empty, holds an empty name, or holds the hero's own
 * game, else on Yes.
 */
short
slotsask(short row, char *hero)
{
	char head[Desclen];
	char name[2 * Namelen + 1];
	char q[Asklen];

	if (!readhead(slotsat(row), head) || head[0] == 0 || strncmp(head, hero, Namelen) == 0)
		return 1;
	copyfmt(name, head, Namelen);
	wsprintf(q, askformat(), name);
	return confirm(q);
}

/*
 * slotsnote asks the note of a save into its row of the save menu, as
 * markers asks an automap's label, prefilled with the slot's note when
 * the slot holds the hero's game, else with the hero's name. It reports
 * whether the player took it, with Enter or a click, and keeps it for
 * the save's description, none when it is the hero's name.
 */
short
slotsnote(short row, char *hero)
{
	char head[Desclen];
	char note[Notelen + 1];

	copyn(note, hero, Notelen);
	if (readhead(slotsat(row), head) && strncmp(head, hero, Namelen) == 0 && head[Noteoff] != 0)
		copyn(note, head + Noteoff, Notelen);
	slotsblank = row;
	slotmenu(0);
	flip();
	slotsblank = 0;
	if (inputtext(320, Rowy + (row - 1) * Rowh + Textdy, note, Notelen, 1, 1, 0, 1, 1, 0, Notew, 0, 0) != 1)
		return 0;
	memset(notebuf, 0, sizeof notebuf);
	if (strncmp(note, hero, Namelen) != 0)
		copyn(notebuf, note, Notelen);
	return 1;
}

/*
 * slotsquick reports whether slot takes a quick save: a slot of the
 * player's that holds the game of hero. It keeps the slot's note for
 * the save.
 */
short
slotsquick(short slot, char *hero)
{
	char head[Desclen];

	if (slot <= 0 || slot > Zslot || isauto(slot))
		return 0;
	if (!readhead(slot, head) || strncmp(head, hero, Namelen) != 0)
		return 0;
	copyn(notebuf, head + Noteoff, Notelen);
	return 1;
}

/* slotsfailed reports whether the last save of a slot failed, which the build with fixes alone tells */
short
slotsfailed(void)
{
	return slotfailed;
}

/* slotsputnote writes the note of the next save into the description desc, and forgets it */
void
slotsputnote(char *desc)
{
	memcpy(desc + Noteoff, notebuf, sizeof notebuf);
	memset(notebuf, 0, sizeof notebuf);
}

/* slotsrowtext writes into dst the label of a row: the note of the description desc, or else the hero's name */
void
slotsrowtext(char *dst, char *desc)
{
	copyn(dst, desc[Noteoff] != 0 ? desc + Noteoff : desc, Notelen);
}

/* putpart appends the file name to the slot that h writes, as saveslot does: its length and bytes, a length of 0 when it is missing. It reports whether the writes went through. */
static int
putpart(HANDLE h, char *name)
{
	HANDLE in;
	DWORD n;

	n = 0;
	if (fopenr(&in, name)) {
		n = fileread(workbuf, 1, Partmax, in);
		fileclose(&in);
	}
	if (!filewrite(&n, 4, 1, h))
		return 0;
	return n == 0 || filewrite(workbuf, n, 1, h);
}

/*
 * headlen returns the length of the header that opens the n bytes of a
 * slot at p, as loadslot reads it: the description, the version, and by
 * the version the face, its tile for the player's own, the item word,
 * and the sum of the portrait. It returns 0 when the bytes hold less.
 */
static DWORD
headlen(unsigned char *p, DWORD n)
{
	DWORD len, ver, face;

	len = Desclen + 2;
	if (n < len)
		return 0;
	ver = p[Desclen] | p[Desclen + 1] << 8;
	face = ver;
	if (ver >= Facever) {
		if (n < len + 2)
			return 0;
		face = p[len] | p[len + 1] << 8;
		len += 2;
	}
	if (face == Ownface)
		len += Facelen;
	if (ver >= Facever)
		len += 4;
	if (ver >= Sumver)
		len += 4;
	return n >= len ? len : 0;
}

/*
 * packauto writes the AutoSave into the file name as a slot: the header
 * of Teudogar_J.SAV, whose length its version gives, since a build
 * without fixes or slots saves a whole slot there, then the E files in
 * the order of saveslot's D files, with the spare file of 1 byte after
 * the saved game, and E099999P.DAT, or else PHOTO.DAT, as autoload
 * leaves it. The file takes the date of Teudogar_J.SAV. It reports
 * whether it did.
 */
static int
packauto(char *name)
{
	char part[16];
	FILETIME t;
	HANDLE h, in;
	DWORD n, one;
	int ok, map, kind;

	wsprintf(part, texts + Slotname, Autoslot + 0x40);
	if (!fopenr(&in, part))
		return 0;
	ok = GetFileTime(in, NULL, NULL, &t);
	n = headlen((unsigned char *)workbuf, fileread(workbuf, 1, Headmax, in));
	fileclose(&in);
	if (!ok || n == 0 || !fopenw(&h, name))
		return 0;
	one = 1;
	ok = filewrite(workbuf, n, 1, h) && putpart(h, texts + Autogame);
	workbuf[0] = 0;
	if (ok)
		ok = filewrite(&one, 4, 1, h) && filewrite(workbuf, 1, 1, h);
	for (map = 0; ok && map < Nmaps; map++)
		for (kind = 2; ok && kind <= 3; kind++) {
			wsprintf(part, texts + Automap, map, kind);
			ok = putpart(h, part);
		}
	if (ok)
		ok = putpart(h, exists(texts + Autophoto) ? texts + Autophoto : texts + Photo);
	if (ok)
		ok = SetFileTime(h, NULL, NULL, &t);
	if (!fileclose(&h))
		ok = 0;
	return ok;
}

/* follow keeps lastslot and autoslot on the game of the slot from as it takes the name of the slot to, or lets them go of it when to is 0 */
static void
follow(short from, short to)
{
	if (lastslot == from)
		lastslot = to;
	if (autoslot == from)
		autoslot = to;
}

/*
 * slotsrotate keeps the two AutoSaves before the one that a journey
 * saves: Z takes Y, and Y takes J, packed through Teudogar_@.SAV, the
 * name of slot 0, which serves the saves as the name of a slot being
 * written, each move in one rename over the slot that it replaces.
 * While the AutoSave holds the game of a slot that a load of this run
 * copied there, which stays in that slot, Y and Z stay as they are.
 * slotsmirror lives in memory, as the original's autoslot does, so after
 * a restart the first journey packs such a game into Y. A pack or a move
 * of Y that fails leaves Y and Z as they were. A move of the pack that
 * fails moves Y back from Z, which leaves Z empty, since the move of Y
 * replaced Z's game.
 */
void
slotsrotate(void)
{
	char tmp[16], y[16], z[16];
	int moved;

	if (!autolevel || !hasautosave() || (slotsmirror != 0 && slotsmirror == autoslot))
		return;
	wsprintf(tmp, texts + Slotname, 0x40);
	wsprintf(y, texts + Slotname, Yslot + 0x40);
	wsprintf(z, texts + Slotname, Zslot + 0x40);
	if (!packauto(tmp)) {
		deletefile(tmp);
		return;
	}
	moved = 0;
	if (exists(y)) {
		if (!MoveFileExA(y, z, MOVEFILE_REPLACE_EXISTING)) {
			deletefile(tmp);
			return;
		}
		follow(Zslot, 0);
		follow(Yslot, Zslot);
		moved = 1;
	}
	if (!MoveFileExA(tmp, y, MOVEFILE_REPLACE_EXISTING)) {
		deletefile(tmp);
		if (moved && MoveFileExA(z, y, MOVEFILE_REPLACE_EXISTING))
			follow(Zslot, Yslot);
	}
}

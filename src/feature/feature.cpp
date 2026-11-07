// FEATURE: features: the plumbing that every feature build shares

/*
 * The plumbing of the feature builds: the tables of
 * the build's data files and the languages it offers, the patch of the
 * program's texts, the columns of the grammar tables, and the glyphs
 * of the fonts. cmd/lang writes them as tables.h, texts.h, gram.h, and
 * font.h from the sources in lang/.
 */

#include <windows.h>
#include <string.h>

typedef struct Feattext Feattext;
typedef struct Featgram Featgram;
typedef struct Featglyph Featglyph;
typedef struct Featwidth Featwidth;
typedef struct Gram Gram;

/* a string of the program's texts that the build replaces */
struct Feattext {
	unsigned short off;	/* its offset in texts */
	short lang;	/* the language it holds for, or -1 for all */
	char *s;
};

/* the gender and the number of an item name in one language */
struct Featgram {
	short type;	/* the item's type */
	short lang;
	char gender;	/* m, f, or n */
	char number;	/* s or p */
};

/* a glyph that the build replaces, its pixels row by row */
struct Featglyph {
	short font;	/* 0 font, 1 font2, 2 font3 */
	short code;
	char *pix;
};

/* a width that the build sets */
struct Featwidth {
	short font;	/* 0 fontw, 1 fontw2, 2 fontw3 */
	short code;
	int width;
};

/* the grammar of an item name: a letter per language (texts.cpp) */
struct Gram {
	char gender[6];	/* m, f, n */
	char number[6];	/* s, p */
};

#include "tables.h"
#include "texts.h"
#include "gram.h"
#include "font.h"

extern int nentries[9];
extern DWORD entryoffs[9][500];
extern DWORD entrysizes[9][500];
extern unsigned long crcs[30];
extern unsigned long sizes[30];
extern char texts[47942];
extern int lang;
extern Gram armorgram[];
extern Gram weapongram[];
extern unsigned char font[256][480];
extern unsigned char font2[256][120];
extern unsigned char font3[256][480];
extern int fontw[256];
extern int fontw2[256];
extern int fontw3[256];

/* the glyphs of the three fonts, the bytes of a glyph, and the widths */
static unsigned char *glyphs[3] = { font[0], font2[0], font3[0] };
static int glyphsize[3] = { sizeof font[0], sizeof font2[0], sizeof font3[0] };
static int *widths[3] = { fontw, fontw2, fontw3 };

/* copy the tables of the build's data files over those of the packed block */
void
feattables(void)
{
	memcpy(nentries, featcounts, sizeof nentries);
	memcpy(entryoffs, featoffs, sizeof entryoffs);
	memcpy(entrysizes, featlens, sizeof entrysizes);
	memcpy(sizes, featsizes, sizeof sizes);
	memcpy(crcs, featcrcs, sizeof crcs);
}

/* set the letters of the grammar tables that the build gives */
void
featgram(void)
{
	Featgram *g;
	Gram *r;
	int i;

	for (i = 0; i < nfeatgrams; i++) {
		g = &featgrams[i];
		if (g->type == 0x528)
			r = &weapongram[26];
		else if (g->type >= 0x1a0 && g->type <= 0x1c6)
			r = &armorgram[g->type - 0x1a0];
		else
			r = &weapongram[g->type - 0x1c7];
		r->gender[g->lang] = g->gender;
		r->number[g->lang] = g->number;
	}
}

/* copy the glyphs and the widths that the build gives over the fonts */
void
featfont(void)
{
	Featglyph *g;
	Featwidth *w;
	int i;

	for (i = 0; i < nfeatglyphs; i++) {
		g = &featglyphs[i];
		memcpy(glyphs[g->font] + g->code * glyphsize[g->font], g->pix, glyphsize[g->font]);
	}
	for (i = 0; i < nfeatwidths; i++) {
		w = &featwidths[i];
		widths[w->font][w->code] = w->width;
	}
}

/* the plumbing that unpack (unpack.cpp) runs once the packed data are in place */
void
featunpack(void)
{
	feattables();
	featgram();
	featfont();
}

/* report whether the build offers language c */
int
featlang(int c)
{
	return c >= 0 && c < sizeof featlangs && featlangs[c];
}

/* report whether the build fills entry id, which the release holds a placeholder in */
int
featfilled(int id)
{
	int i;

	for (i = 0; i < nfeatids; i++)
		if (featids[i] == id)
			return 1;
	return 0;
}

/* the string of the texts at s as the build gives it in the current language */
char *
feattext(char *s)
{
	int lo, hi, mid, off;
	char *any;

	// FIXED: s may point into another object than texts, as msgf's formats in strbuf do, and the order of pointers into different objects is undefined in C. Compare the addresses as integers.
#ifdef PORT
	if ((uintptr_t)s - (uintptr_t)texts >= sizeof texts)
		return s;
#else
	if (s < texts || s >= texts + sizeof texts)
		return s;
#endif
	off = s - texts;
	lo = 0;
	hi = nfeattexts;
	while (lo < hi) {
		mid = (lo + hi) / 2;
		if (feattexts[mid].off < off)
			lo = mid + 1;
		else
			hi = mid;
	}
	any = s;
	for (; lo < nfeattexts && feattexts[lo].off == off; lo++) {
		if (feattexts[lo].lang == lang)
			return feattexts[lo].s;
		if (feattexts[lo].lang == -1)
			any = feattexts[lo].s;
	}
	return any;
}

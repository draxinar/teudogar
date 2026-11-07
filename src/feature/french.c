/* FEATURE: french: the rules of French for the texts that the game composes */

/*
 * The rules of French for the texts that the game composes: it inserts
 * names and items after words that a translation cannot adjust, as in
 * "de $N1" or "le $objh", and changes the case of letters by the rules
 * of English and German. frelide rewrites le, la, de, and que before a
 * vowel or a mute h into l', d', and qu'. Correct French already elides
 * in these cases, whatever the word, so the pass may run over a whole
 * text, and since each rewrite shortens the text, it runs in place.
 * frposs puts ton for ta before such a word, frsentcase capitalizes the
 * sentences of a text, and frtitle a title. The texts are in code page
 * 1252.
 *
 * internal/lang compiles this file through cgo, and cmd/lang runs
 * frelide over the French texts to check their elisions. gcc makes
 * char signed where Watcom makes it unsigned, so the bytes are read
 * as unsigned char throughout.
 */

#include <string.h>

enum {
	Rquote = 0x92,	/* ’ in code page 1252, a letter of a word for iswordc */
	Nbsp = 0xa0,	/* the no-break space before : ; ! ? and inside « » */
	Lguil = 0xab	/* « */
};

/* the words with a mute h that the game inserts, stems before a plural s */
static char *muteh[] = {
	"hydromel",
	"herbe",
	"huile",
	"homme",
	"honneur",
	NULL
};

/* the words before which French keeps the article whole */
static char *whole[] = {
	"onze",
	"oui",
	"una",	/* Latin, script line 541: "de una bellissima meretricula" */
	NULL
};

/*
 * whether c is a letter of code page 1252: a byte of a word for iswordc
 * (str.cpp) other than a digit, _, or the left quote 0x91
 */
static int
isletter(unsigned char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c >= 0xc0)
		return 1;
	return c == 0x8a || c == 0x8c || c == 0x9a || c == 0x9c || c == 0x9f || c == Rquote;
}

/* the small letter of c, with the capitals 0x8a and 0x9f kept */
static unsigned char
small(unsigned char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 0xc0 && c <= 0xde && c != 0xd7))
		return c + 0x20;
	if (c == 0x8c)
		return 0x9c;
	return c;
}

/* the capital of c, as upper (str.cpp) gives it, with the sign 0xf7 kept */
static unsigned char
big(unsigned char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 0xe0 && c <= 0xfe && c != 0xf7))
		return c - 0x20;
	if (c == 0x9a)
		return 0x8a;
	if (c == 0x9c)
		return 0x8c;
	if (c == 0xff)
		return 0x9f;
	return c;
}

/* whether c is a vowel, bare or accented */
static int
isvowel(unsigned char c)
{
	switch (small(c)) {
	case 'a': case 'e': case 'i': case 'o': case 'u':
	case 0xe0: case 0xe2: case 0xe4: case 0xe6:
	case 0xe8: case 0xe9: case 0xea: case 0xeb:
	case 0xec: case 0xed: case 0xee: case 0xef:
	case 0xf2: case 0xf3: case 0xf4: case 0xf6:
	case 0xf9: case 0xfa: case 0xfb: case 0xfc:
	case 0x9c:
		return 1;
	}
	return 0;
}

/* whether the word at p is w, or w and an s, in any case */
static int
isword(unsigned char *p, char *w)
{
	int i;

	for (i = 0; w[i] != '\0'; i++)
		if (small(p[i]) != (unsigned char)w[i])
			return 0;
	if (small(p[i]) == 's')
		i++;
	return !isletter(p[i]);
}

/* whether the word at p takes the elision of the article before it */
static int
elides(unsigned char *p)
{
	int i;

	for (i = 0; whole[i] != NULL; i++)
		if (isword(p, whole[i]))
			return 0;
	if (isvowel(p[0]))
		return 1;
	for (i = 0; muteh[i] != NULL; i++)
		if (isword(p, muteh[i]))
			return 1;
	return 0;
}

/*
 * whether p starts a word: at the text's start, or after a byte other
 * than a letter, an apostrophe, or a hyphen
 */
static int
startsword(unsigned char *s, unsigned char *p)
{
	if (p == s)
		return 1;
	return !isletter(p[-1]) && p[-1] != '\'' && p[-1] != '-';
}

/* the length of the article or the que that elides at p: 2, 3, or 0 */
static int
article(unsigned char *p)
{
	unsigned char a, b;

	a = small(p[0]);
	b = small(p[1]);
	if (a == 'q' && b == 'u' && small(p[2]) == 'e')
		return 3;
	if ((a == 'l' && (b == 'e' || b == 'a')) || (a == 'd' && b == 'e'))
		return 2;
	return 0;
}

/*
 * elide le, la, de, and que before a vowel or a mute h in s, across a
 * space, or across the CR that wrap (lang.cpp) writes over the space
 * where it breaks the line of a string, which the texts that insert
 * words wrap again
 */
void
frelide(char *s)
{
	unsigned char *p, *q;
	int n;

	for (p = (unsigned char *)s; *p != '\0'; p++) {
		if (!startsword((unsigned char *)s, p))
			continue;
		n = article(p);
		if (n == 0)
			continue;
		q = p + n;
		if ((q[0] != ' ' && q[0] != '\r') || !elides(q + 1))
			continue;
		/* the vowel becomes the apostrophe, and the space goes */
		q[-1] = '\'';
		memmove(q, q + 1, strlen((char *)q + 1) + 1);
	}
}

/*
 * put ton for the ta that starts s, a buffer of size bytes, before a
 * word that elides, as in ton épée
 */
void
frposs(char *s, int size)
{
	unsigned char *p;
	int n;

	p = (unsigned char *)s;
	if (small(p[0]) != 't' || small(p[1]) != 'a' || p[2] != ' ' || !elides(p + 3))
		return;
	n = strlen(s);
	if (n + 2 > size)
		return;
	memmove(s + 3, s + 2, n - 1);
	if (p[1] == 'A') {
		s[1] = 'O';
		s[2] = 'N';
	} else {
		s[1] = 'o';
		s[2] = 'n';
	}
}

/* whether p follows an ellipsis of s, as sentcase (texts.cpp) finds it */
static int
afterdots(unsigned char *s, unsigned char *p)
{
	if (p >= s + 2 && p[-1] == '.' && p[-2] == '.')
		return 1;
	return p >= s + 3 && p[-1] == ' ' && p[-2] == '.' && p[-3] == '.';
}

/*
 * elide s, then capitalize the first letter of each of its sentences,
 * as sentcase (texts.cpp) does, but for French: a colon continues the
 * sentence, and the no-break space and « may come before the capital
 */
void
frsentcase(char *s)
{
	unsigned char *p;
	int cap;

	frelide(s);
	cap = 1;
	for (p = (unsigned char *)s; *p != '\0'; p++) {
		switch (*p) {
		case ' ':
		case '\r':
		case Nbsp:
		case Lguil:
			continue;
		case '.':
		case '!':
		case '?':
			cap = 1;
			continue;
		case ':':
		case ',':
			cap = 0;
			continue;
		}
		if (cap && !afterdots((unsigned char *)s, p))
			*p = big(*p);
		cap = 0;
	}
}

/* elide title s, then capitalize its first letter */
void
frtitle(char *s)
{
	unsigned char *p;

	frelide(s);
	for (p = (unsigned char *)s; *p != '\0'; p++) {
		if (isletter(*p)) {
			*p = big(*p);
			return;
		}
	}
}

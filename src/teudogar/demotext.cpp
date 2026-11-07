#include <windows.h>
#include <string.h>

void cutstr(char *s, int n);
char *insstr(char *s, char *t);
char *pick(char *de, char *en, char *fr, char *es, char *it, char *pl);

extern char texts[];

#define T(off) (texts + (off))

extern char *chp;
extern char *chq;
extern short nchap;
extern short nsect;
extern unsigned chlen;
extern char chcut[200];

/* replace the text of each chapter that starts with a slash by note */
void
cutchapters(char *text, char *note)
{
	memset(chcut, 0, 200);
	if (note == NULL)
		return;
	nchap = 0;
	nsect = 0;
	chp = text;
	while (*chp) {
		if (*chp == 13 && chp[1] == '[') {
			if (chp[2] != '-') {
				nchap++;
				nsect++;
				while (*chp == 13 || *chp == '{')
					chp++;
				while (*chp && *chp != 13 && *chp == '{')
					chp++;
				while (*chp == 13 || *chp == '{')
					chp++;
				while (*chp && !((*chp == 13 || *chp == '{') && (chp[1] == 13 || chp[1] == '{')))
					chp++;
				while (*chp == 13 || *chp == '{')
					chp++;
				chq = chp + 1;
				while (*chq && (*chq != 13 || chq[1] != 13 || chq[2] != '['))
					chq++;
				if (*chp == '/') {
					chlen = chq - chp;
					if (chlen)
						chlen--;
					cutstr(chp, chlen);
					chp = insstr(chp, note);
					chcut[nsect - 1] = 1;
				} else
					chp = chq;
			} else {
				nsect++;
				chp += 2;
			}
		} else
			chp++;
	}
}

extern char printbuf[4800];

/* write the note on the chapters the demo lacks, for the game or a web page */
void
demonote(short game, short offline)
{
	char *p;

	p = printbuf;
	if (game) {
		wsprintf(printbuf, pick(T(0x83a9), T(0x86cb), T(0x8995), T(0x8998), T(0x899b), T(0x8c0f)));
		for (; *p; p++)
			;
	} else {
		if (offline) {
			wsprintf(printbuf, pick(T(0x8488), T(0x8790), T(0x8996), T(0x8999), T(0x8a42), T(0x8c10)));
			for (; *p; p++)
				;
		} else {
			wsprintf(printbuf, pick(T(0x859d), T(0x8888), T(0x8997), T(0x899a), T(0x8b1e), T(0x8c11)));
			for (; *p; p++)
				;
		}
		wsprintf(p, pick(T(0x8326), T(0x8350), T(0x837c), T(0x837d), T(0x837e), T(0x83a8)));
		for (; *p; p++)
			;
	}
}

char *chp;
char *chq;
short nchap;
short nsect;
unsigned chlen;
char chcut[200];

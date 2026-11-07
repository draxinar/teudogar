#include <windows.h>
#include <string.h>

void copystr(char *dst, char *src);
int fopenr(HANDLE *h, char *name);
int fileclose(HANDLE *h);
DWORD fileread(void *buf, DWORD size, DWORD n, HANDLE h);
DWORD unpack(void *dst, DWORD dstlen, void *src, DWORD srclen);
int text3w(char *s);
int langentry(int id);
int seekentry(int id);

extern char workbuf[616000];
extern char packbuf[616000];
extern HANDLE datfh;
extern DWORD storedn;
extern int spacestop;
extern int text3width;

extern char *wrq;
extern char *wrp;
extern int wrn;
extern int wrindent;
extern char *wrline;

/* join the lines of s and break them again at width pixels */
int
wrap(char *s, int width)
{
	char c;

	if (*s == '\0' || s[0] == 0x5c && s[1] == 0x5c)
		return 0;
	wrline = s;
	wrindent = 0;
	wrp = s;
	wrn = 0;
	do {
		if (*wrp == 10) {
			wrq = wrp;
			do
				*wrq = wrq[1];
			while (*wrq++ != '\0');
		} else
			wrn++;
		// FIXED: a text that starts with a line's end reads the byte before it
#ifdef FEATURE_FAULTS
		if (*wrp == 13 && wrp > s && wrp[-1] == ' ')
#else
		if (*wrp == 13 && wrp[-1] == ' ')
#endif
			while (wrp > s && wrp[-1] == ' ') {
				wrp--;
				wrq = wrp;
				do
					*wrq = wrq[1];
				while (*wrq++ != '\0');
			}
	} while (*wrp++ != '\0');
	if (*s == 0x5c)
		return 0;
	wrp = s;
	wrline = s;
	spacestop = 0;
	while (*wrp) {
		if (*wrp == 13 && wrp > s) {
			if (wrp[-1] == 13)
				wrindent = 0;
			if (wrp[1] == 13) {
				wrindent = 0;
				wrp++;
				continue;
			}
			if (wrp[1] == '[') {
				wrindent = 0;
				wrp++;
				continue;
			}
			if (wrp[2] == 13 && (wrp[1] == '.' || wrp[1] == '+')) {
				wrp += 2;
				wrindent = 0;
				continue;
			}
			// FIXED: a text whose second byte ends a line reads the byte before the text. Read inside the text alone.
#ifdef FEATURE_FAULTS
			if (wrp - s >= 2 && wrp[-2] == '[' && wrp[-1] == '-') {
#else
			if (wrp[-2] == '[' && wrp[-1] == '-') {
#endif
				wrp += 2;
				continue;
			}
			switch (wrp[-1]) {
			case 13:
			case '!':
			case '"':
			case '\'':
			case ')':
			case '*':
			case '+':
			case ':':
			case '?':
			case ']':
				break;
			case '.':
				// FIXED: a line of two or three bytes that ends in a period at the text's start, as the German reply "Ja.", reads up to 2 bytes before the text. Read inside the text alone.
#ifdef FEATURE_FAULTS
				if ((wrp - s >= 3 && wrp[-3] == '.') || (wrp - s >= 4 && wrp[-4] == '.'))
#else
				if (wrp[-3] == '.' || wrp[-4] == '.')
#endif
					*wrp = ' ';
				break;
			case '{':
				wrp[-1] = ' ';
				break;
			default:
				*wrp = ' ';
				break;
			}
		}
		switch (*wrp) {
		case 13:
			if (wrp[1] == '-' && wrp[2] == '-' && wrp[3] == ' ') {
				wrindent = 1;
				wrp += 2;
			}
			wrline = wrp + 1;
			break;
		case ' ':
			if (width) {
				wrq = wrp + 1;
				while (*wrq && *wrq != 32 && *wrq != 13)
					wrq++;
				if (*wrq == 32 || *wrq == 13 || *wrq == 0) {
					c = *wrq;
					*wrq = 0;
					text3w(wrline);
					if (wrindent)
						text3width += 16;
					*wrq = c;
					if (text3width > width) {
						*wrp = 13;
						wrline = wrp + 1;
					}
				}
			}
			break;
		}
		wrp++;
	}
	return wrn;
}

extern int lang;
extern char langchar;
extern char langcode[5];
extern char langset;

void
setlang(char c)
{
	copystr(langcode, c == 0 ? "de" : c == 2 ? "fr" : c == 3 ? "sp" : c == 4 ? "it" : c == 5 ? "pl" : "en");
	if (!(c == 0xff || c == 1 || c == 0 || c == 4))
		c = 1;
	lang = c;
	langchar = lang == 0 ? 'D' : lang == 2 ? 'F' : lang == 3 ? 'S' : lang == 4 ? 'I' : lang == 5 ? 'P' : 'E';
}

extern char strbuf[102400];
extern char *strs[2300];
extern int nstrs;
extern char pounds;
extern char poundsset;
extern char strfile[1000];
extern int strsfromfile;

/* set language c, or English, and the default of pounds */
void
chooselang(char c)
{
	if (c != 0xff && c != 1 && c != 0 && c != 4)
		c = 1;
	setlang(c);
	if (!poundsset) {
		if (c == '\1')
			pounds = 1;
		else
			pounds = 0;
	}
	langset = 1;
}

/* load the strings of the language from strfile or the data file */
void
loadstrs(void)
{
	HANDLE h;
	int n;
	unsigned j;
	char *p, *q;
	int i;

	strsfromfile = 0;
	if (strfile[0] && fopenr(&h, strfile) &&
		(n = fileread(workbuf, 1, sizeof strbuf, h), fileclose(&h), n))
		strsfromfile = 1;
	else {
		if (!seekentry(langentry(lang + 5003)))
			return;
		n = storedn;
		n = unpack(workbuf, 0x96640, packbuf,
			fileread(packbuf, 1, n, datfh));
		if (n < 0)
			return;
		for (j = 0; j < n; j++)
			workbuf[j] ^= 0xfe;
	}
	p = workbuf;
	q = strbuf;
	for (nstrs = 0; nstrs < 2300 && p < workbuf + n; nstrs++) {
		strs[nstrs] = q;
		for (; *p != '#' && p < workbuf + n; )
			*q++ = *p++;
		p += 3;
		*q++ = 0;
	}
	for (i = nstrs; i < 2300; i++)
		strs[i] = strbuf;
	for (i = 0; i < nstrs; i++) {
		wrap(strs[i], 428);
		if (*strs[i] == 0x5c)
			strs[i]++;
		if (*strs[i] == 0x5c)
			strs[i]++;
	}
}

int lang = 1;
char langchar = 'E';
char langcode[5] = "en";
char langset = 1;
char *wrq;
char *wrp;
int wrn;
int wrindent;
char *wrline;
int wrunused;
char strbuf[102400];
char *strs[2300];
int nstrs;
char pounds;
char poundsset;
char strfile[1000];
int strsfromfile;

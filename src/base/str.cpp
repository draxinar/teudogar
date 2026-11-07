#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"

char numsep = ',';

char *trimstart;
char lc1, lc2;
char pc1, pc2;
char wc1, wc2;
char ic1, ic2;
int nwords;
char *crdst, *crsrc;
char *lfdst, *lfsrc;
char *incp;
char *insnp;
char *insp;
int insn;
int hexv, hexi, hexm;
int numv;
DWORD numm;
char *trimq, *trimp, *trimo;
int xori;
char *tailstart;
char lessa, lessb;
char tokbuf[800];
char exprbuf[150];
char numbuf[32];
char linebuf[150];
int nlines;
char fmtbuf[10][20];
int fmtidx;
char sizebuf[32];
char shortbuf[100];

int
isletter(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return 1;
	return 0;
}

int
iswordc(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' ||
		(c >= '\xe0' && c <= '\xfe') || c == '\xdf' || c == '\x9a' || c == '\x9c' || c == '\xff' ||
		(c >= '\xc0' && c <= '\xde') || c == '\x8a' || c == '\x8c' || c == '\x9f' ||
		c == '\x91' || c == '\x92')
		return 1;
	return 0;
}

int
isconsonant(char c)
{
	if (c >= 'A' && c <= 'Z')
		c += ' ';
	switch (c) {
	case 'b':
	case 'c':
	case 'd':
	case 'f':
	case 'g':
	case 'h':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':
		return 1;
	}
	return 0;
}

int
isdec(char c)
{
	if (c >= '0' && c <= '9')
		return 1;
	return 0;
}

int
istokc(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '%')
		return 1;
	return 0;
}

int
isvarc(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' ||
		c == '/' || c == '*')
		return 1;
	return 0;
}

int
issep(char c)
{
	if (c == ',' || c == ' ' || c == 13 || c == 10)
		return 1;
	return 0;
}

int
strsize(char *s)
{
	int n;

	n = 0;
	while (s[n++])
		;
	return n;
}

/*
 * fracchar returns a character for the fraction pct percent comes to.
 * 0xbc, 0xbd, and 0xbe are the signs for a quarter, a half, and three
 * quarters in Windows-1252.
 */
char
fracchar(int pct)
{
	if (pct < 10)
		return 0x2f;
	if (pct >= 90)
		return 0x31;
	if (pct >= 70)
		return 0xbe;
	if (pct >= 40)
		return 0xbd;
	if (pct >= 30)
		return 4;
	if (pct >= 23)
		return 0xbc;
	if (pct >= 16)
		return 5;
	return 6;
}

int
isfracchar(char c)
{
	switch (c) {
	case 4:
	case 5:
	case 6:
	case 0x2f:
	case 0xbc:
	case 0xbd:
	case 0xbe:
		return 1;
	}
	return 0;
}

char
upper(char c)
{
	if (c >= 'a' && c <= 'z')
		return c - ' ';
	if (c >= '\xe0' && c <= '\xfe')
		return c - ' ';
	if (c == '\x9a')
		return '\x8a';
	if (c == '\x9c')
		return '\x8c';
	if (c == '\xff')
		return '\x9f';
	return c;
}

char
lower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c + ' ';
	if (c >= '\xc0' && c <= '\xde')
		return c + ' ';
	if (c == '\x8a')
		return '\x9a';
	if (c == '\x8c')
		return '\x9c';
	if (c == '\x9f')
		return '\xff';
	return c;
}

char
foldchar(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c;
	if (c >= 'a' && c <= 'z')
		return c - ' ';
	if (c >= '\xc0' && c <= '\xc6')
		return 'A';
	if (c >= '\xc8' && c <= '\xcb')
		return 'E';
	if (c >= '\xcc' && c <= '\xcf')
		return 'I';
	if (c >= '\xd2' && c <= '\xd8')
		return 'O';
	if (c >= '\xd9' && c <= '\xdc')
		return 'U';
	if (c >= '\xe0' && c <= '\xe6')
		return 'A';
	if (c >= '\xe8' && c <= '\xeb')
		return 'E';
	if (c >= '\xec' && c <= '\xef')
		return 'I';
	if (c >= '\xf2' && c <= '\xf8')
		return 'O';
	if (c >= '\xf9' && c <= '\xfc')
		return 'U';
	switch (c) {
	case ' ':
	case '-':
	case '_':
		return c;
	case '@':
		return 'A';
	case '\x80':
	case '3':
		return 'E';
	case '1':
		return 'I';
	case '\x8c':
	case '\x9c':
	case '0':
		return 'O';
	case '\xc7':
	case '\xe7':
		return 'C';
	case '\xd0':
	case '\xde':
	case '\xf0':
	case '\xfe':
		return 'D';
	case '\xd1':
	case '\xf1':
		return 'N';
	case '\x9f':
	case '\xa5':
	case '\xdd':
	case '\xfd':
	case '\xff':
		return 'Y';
	case '\xdf':
	case '8':
		return 'B';
	case '\x8a':
	case '\x9a':
	case '5':
		return 'S';
	case '2':
		return 'Z';
	case '6':
		return 'G';
	case '9':
		return 'P';
	case '4':
	case '\xa3':
		return 'L';
	case '7':
		return 'T';
	case '!':
	case '&':
	case '+':
	case ',':
	case '.':
	case '/':
	case ':':
	case ';':
	case '=':
	case '?':
	case '\xa1':
	case '\xbf':
		return '-';
	default:
		return '_';
	}
}

/*
 * chop cuts s before its last character other than a space.
 */
void
chop(char *s)
{
	if (s[0] == 0)
		return;
	while (*s)
		s++;
	for (s--; *s == ' '; s--)
		;
	*s = 0;
}

/*
 * trimlines cuts a run of carriage returns at the end of s down to one.
 * The loop over CR LF pairs never runs: it wants s[-2] to be both '\r'
 * and '\n'.
 */
void
trimlines(char *s)
{
	if (s[0] == 0)
		return;
	trimstart = s;
	for (; *s; s++)
		;
	if (s >= trimstart + 2 && s[-1] == '\r' && s[-2] == '\r') {
		while (s >= trimstart + 2 && s[-1] == '\r' && s[-2] == '\r')
			*--s = 0;
		return;
	}
	while (s >= trimstart + 4 && s[-2] == '\r' && s[-1] == '\n' && s[-4] == '\r' && s[-2] == '\n') {
		s -= 2;
		*s = 0;
	}
}

void
upperstr(char *s)
{
	for (; *s; s++)
		*s = upper(*s);
}

void
lowerstr(char *s)
{
	for (; *s; s++)
		*s = lower(*s);
}

void
translit(char *dst, char *src)
{
	for (;;) {
		switch (*src) {
		case 0:
			*dst = 0;
			return;
		case '\xc4':
			*dst++ = 'A';
			*dst++ = 'E';
			break;
		case '\xd6':
			*dst++ = 'O';
			*dst++ = 'E';
			break;
		case '\xdc':
			*dst++ = 'U';
			*dst++ = 'E';
			break;
		case '\xdf':
			*dst++ = 'S';
			*dst++ = 'S';
			break;
		case ' ':
		case '-':
			*dst++ = '_';
			break;
		default:
			*dst++ = *src;
			break;
		}
		src++;
	}
}

int
haschar(char *s, char c)
{
	for (; *s; s++)
		if (*s == c)
			return 1;
	return 0;
}

int
prefixlen(char *a, char *b)
{
	int n;

	n = 0;
	for (;;) {
		if (*b == 0)
			return n;
		if (*a != *b || *a == 0)
			return 0;
		a++;
		b++;
		n++;
	}
}

void
copystr(char *dst, char *src)
{
	char c;

	do {
		c = *src++;
		*dst++ = c;
	} while (c != 0);
}

void
copystrn(char *dst, char *src, int n)
{
	int i;

	i = 0;
	do {
		dst[i] = src[i];
		if (!dst[i])
			return;
		i++;
	} while (i < n - 1);
	dst[i] = 0;
}

void
catstr(char *dst, char *src)
{
	for (; *dst; dst++)
		;
	do {
		*dst++ = *src;
	} while (*src++);
}

int
streqi(char *a, char *b)
{
	do {
		lc1 = lower(*a++);
		lc2 = lower(*b++);
		if (lc1 != lc2)
			return 0;
	} while (lc1 && lc2);
	return 1;
}

int
prefixeq(char *a, char *b)
{
	do {
		pc1 = *a++;
		if (!pc1)
			return 1;
		pc2 = *b++;
		if (!pc2)
			return 1;
	} while (pc1 == pc2);
	return 0;
}

int
wordeqi(char *a, char *b)
{
	do {
		wc1 = lower(*a++);
		wc2 = lower(*b++);
		if (!iswordc(wc1)) {
			if (!iswordc(wc2))
				return 1;
			return 0;
		}
		if (!iswordc(wc2))
			return 0;
		if (wc1 != wc2)
			return 0;
	} while (wc1 && wc2);
	return 1;
}

int
memeq(char *a, char *b, int n)
{
	while (n--)
		if (*a++ != *b++)
			return 0;
	return 1;
}

int
hasprefix(char *s, char *pre)
{
	while (*pre != 0)
		if (*s++ != *pre++)
			return 0;
	return 1;
}

char *
findstr(char *s, char *t)
{
	for (;;) {
		if (hasprefix(s, t))
			return s;
		if (*s == 0)
			return NULL;
		s++;
	}
}

char *
findstrn(char *s, int n, char *t)
{
	while (n-- > 0) {
		if (hasprefix(s, t))
			return s;
		s++;
	}
	return NULL;
}

int
ideqi(char *a, char *b)
{
	do {
		ic1 = upper(*a++);
		ic2 = upper(*b++);
		if (!((ic1 >= 'A' && ic1 <= 'Z') || (ic1 >= '0' && ic1 <= '9') || ic1 == '_' ||
			(ic2 >= 'A' && ic2 <= 'Z') || (ic2 >= '0' && ic2 <= '9') || ic2 == '_'))
			return 1;
		if (ic1 != ic2)
			return 0;
	} while (ic1 && ic2);
	return 1;
}

int
countwords(char *s)
{
	if (s[0] == 0)
		return 0;
	nwords = 0;
	for (;;) {
		if (*s == ' ' || *s == ',' || *s == '.' || *s == ';' || *s == 0 || *s == 13 || *s == '{') {
			nwords++;
			while (*s == ' ' || *s == ',' || *s == '.' || *s == ';')
				s++;
			if (*s == 0 || *s == 13 || *s == '{')
				return nwords;
		} else
			s++;
	}
}

char *
skipspace(char *s)
{
	while (*s == 13 || *s == 10 || *s == ' ')
		s++;
	return s;
}

char *
skipword(char *s)
{
	for (; *s != ' ' && *s != 13 && *s; s++)
		;
	return s;
}

char *
skipfield(char *s)
{
	for (; *s != ',' && *s != ' ' && *s != 13 && *s; s++)
		;
	return s;
}

char *
skipline(char *s)
{
	for (; *s && *s != 13; s++)
		;
	return s;
}

int
nextfield(char **pp)
{
	while (**pp != ',') {
		if (**pp == 0 || **pp == 13 || **pp == ' ')
			return 0;
		(*pp)++;
	}
	(*pp)++;
	return 1;
}

int
wordstart(char *s, int pos, int n)
{
	if (pos > 0) {
		if (!iswordc(s[pos - 1]))
			pos--;
		while (!iswordc(s[pos]) && pos > 0)
			pos--;
		while (iswordc(s[pos]) && pos > 0)
			pos--;
		while (!iswordc(s[pos]) && pos < n)
			pos++;
	}
	return pos;
}

int
wordnext(char *s, int pos, int n)
{
	if (pos < n) {
		while (iswordc(s[pos]) && pos < n)
			pos++;
		while (!iswordc(s[pos]) && pos < n)
			pos++;
	}
	return pos;
}

void
tocrlf(char *src, char *dst)
{
	crsrc = src;
	crdst = dst;
	do {
		if (*crsrc == 13 && crsrc[1] != 10) {
			*crdst++ = 13;
			*crdst++ = 10;
		} else if (*crsrc == '{' && crsrc[1] == 13)
			;
		else
			*crdst++ = *crsrc;
	} while (*crsrc++);
}

void
droplf(char *src, char *dst)
{
	lfdst = dst;
	lfsrc = src;
	do {
		if (*lfsrc != 10)
			*lfdst++ = *lfsrc;
	} while (*lfsrc++);
}

/*
 * cutstr removes the first n characters of s. The loop that clears the
 * tail starts a byte late: it keeps the byte after the new terminator and
 * clears the byte after the old one. Watcom reads s[n] before it
 * increments s, and makes s+n a pointer in the entry block, which keeps
 * the register saves there and lets the exit share droplf's epilogue.
 */
void
cutstr(char *s, int n)
{
	while (*s++ = s[n])
		;
	while (n--)
		*++s = 0;
}

char *
inschar(char *s, char c)
{
	incp = s;
	while (*incp++)
		;
	do
		*incp = incp[-1];
	while (--incp != s);
	*s = c;
	return s + 1;
}

/*
 * insstrn inserts the first n characters of t at s and returns the end
 * of the inserted text.
 */
char *
insstrn(char *s, char *t, int n)
{
	if (*t == 0 || n <= 0)
		return s;
	insnp = s;
	while (*insnp++)
		;
	insnp += n - 1;
	do
		*insnp = insnp[-n];
	while (--insnp != s + n - 1);
	while (n--)
		*s++ = *t++;
	return s;
}

char *
insstr(char *s, char *t)
{
	if (*t == 0)
		return s;
	insn = 0;
	while (t[insn])
		insn++;
	insp = s;
	while (*insp++)
		;
	insp += insn - 1;
	do
		*insp = insp[-insn];
	while (--insp != s + insn - 1);
	while (insn--)
		*s++ = *t++;
	return s;
}

int
ishex(char c)
{
	if (c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F')
		return 1;
	return 0;
}

int
hexdigit(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return 0;
}

int
hextoi(char *s)
{
	hexi = 0;
	while (ishex(s[hexi]))
		hexi++;
	if (hexi == 0)
		return 0;
	hexv = 0;
	hexm = 1;
	for (hexi--; hexi >= 0; hexi--) {
		hexv += hexdigit(s[hexi]) * hexm;
		hexm *= 16;
	}
	return hexv;
}

/*
 * number returns the value of the decimal number at s, or of the
 * hexadecimal one after 0x, in capital digits. It finds the last digit and
 * adds the digits from there back, 7 decimal or 8 hexadecimal at most.
 */
int
number(char *s)
{
	numv = 0;
	if (s[0] == '0' && s[1] == 'x') {
		s += 2;
		while (s[1] >= '0' && s[1] <= '9' || s[1] >= 'A' && s[1] <= 'F')
			s++;
		numm = 1;
		for (;;) {
			if (*s >= 'A' && *s <= 'F')
				numv += (*s - 'A' + 10) * numm;
			else
				numv += (*s - '0') * numm;
			s--;
			if (!(*s >= '0' && *s <= '9' || *s >= 'A' && *s <= 'F'))
				break;
			if (numm >= 0x10000000)
				break;
			numm <<= 4;
		}
		return numv;
	}
	while (s[1] >= '0' && s[1] <= '9')
		s++;
	numm = 1;
	do {
		numv += (*s - '0') * numm;
		s--;
		if (*s < '0' || *s > '9')
			break;
		numm *= 10;
	} while (numm < 10000000);
	return numv;
}

/*
 * trimcr removes the spaces before each carriage return in the *len bytes
 * at buf, working in the buffer tmp, and sets *len to the new length. The
 * last carriage return and the text after it go too.
 */
void
trimcr(char *buf, int *len, char *tmp)
{
	trimq = buf;
	trimo = tmp;
	for (trimp = buf; trimp < buf + *len; trimp++) {
		if (*trimp == 13) {
			while (trimp[-1] == ' ' && trimp - 1 > buf)
				trimp--;
			memcpy(trimo, trimq, trimp - trimq);
			trimo += trimp - trimq;
			while (*trimp == ' ')
				trimp++;
			trimq = trimp;
		}
	}
	*len = trimo - tmp;
	memcpy(buf, tmp, *len);
}

/*
 * xorstr copies src to dst with each byte XORed with 0xff, and with 0xfe
 * at odd offsets.
 */
void
xorstr(char *dst, char *src)
{
	char *p, *q;

	xori = 0;
	p = dst;
	q = src;
	for (; *q; q++) {
		*p++ = *q ^ (0xff - (xori & 1));
		xori++;
	}
	*p = 0;
}

/*
 * fmtints writes the n integers of v to buf in decimal, cols to a line,
 * separated by commas, and returns the length with the terminator.
 */
int
fmtints(char *buf, int *v, int n, int cols)
{
	char *p;
	int i, col;

	p = buf;
	i = 0;
	col = 0;
	for (; i < n; i++) {
		wsprintf(p, "%d", v[i]);
		while (*p)
			p++;
		if (++col >= cols) {
			*p++ = '\r';
			*p++ = '\n';
			col = 0;
		} else
			*p++ = ',';
	}
	*p++ = 0;
	return p - buf;
}

/*
 * scanints reads the integers in the len bytes at s into v and returns
 * their count.
 */
int
scanints(int *v, char *s, int len)
{
	char *p;
	int n;

	p = s;
	n = 0;
	do {
		if (p[0] == '0' && p[1] == 'x') {
			v[n++] = number(p);
			p += 2;
		} else if (*p == '-') {
			v[n++] = -number(p);
			p++;
		} else
			v[n++] = number(p);
		while (istokc(*p))
			p++;
		while (issep(*p))
			p++;
	} while (p < s + len && *p);
	return n;
}

/*
 * skipto moves *pp past the next t and returns 1, or to the end of the
 * string and returns 0.
 */
int
skipto(char **pp, char *t)
{
	while (!hasprefix(*pp, t)) {
		if (**pp == 0)
			return 0;
		(*pp)++;
	}
	while (*t++)
		(*pp)++;
	return 1;
}

/*
 * copyto copies the text at *pp to *dp up to the next t, which it skips,
 * and returns 1, or copies the rest of the string and returns 0.
 */
int
copyto(char **pp, char **dp, char *t)
{
	while (!hasprefix(*pp, t)) {
		if (**pp == 0) {
			**dp = 0;
			return 0;
		}
		*(*dp)++ = *(*pp)++;
	}
	**dp = 0;
	while (*t++)
		(*pp)++;
	return 1;
}

/*
 * getexpr copies the expression at src to dst, size bytes at most with
 * the terminator, up to a NUL, a carriage return, or a closing
 * parenthesis. It drops the spaces, and each L that ends a word but the
 * one after the digits that end a name (x12L), and returns the byte after
 * the terminator.
 */
char *
getexpr(char *dst, char *src, int size)
{
	char *p, *q;
	char c;
	int n;

	p = src;
	n = 0;
	for (;;) {
		c = *p++;
		if (c == 0 || c == 13 || c == ')')
			break;
		if (c == ' ')
			continue;
		if (c == 'L' && !isletter(*p) && !isdec(*p) && *p != '_') {
			q = p - 2;
			if (!isdec(*q))
				continue;
			while (q >= src && isdec(*q))
				q--;
			if (!(isletter(*q) || *q == '_'))
				continue;
		}
		dst[n++] = c;
		if (n >= size - 1)
			break;
	}
	dst[n++] = 0;
	return dst + n;
}

/*
 * operand finds the number that ends before the operator at p, for a
 * negative dir, or that starts after it. It stores its value in *val, or
 * -1 for a missing number, and its length in *len, and returns its start.
 */
char *
operand(char *s, char *p, int dir, int *val, int *len)
{
	char *end;

	if (dir < 0) {
		if (!isdec(*--p)) {
			*val = -1;
			return s;
		}
		end = p + 1;
		while (p >= s && isdec(*p))
			p--;
		s = p + 1;
	} else {
		if (!isdec(*++p)) {
			*val = -1;
			return s;
		}
		s = p;
		while (isdec(*p))
			p++;
		end = p;
	}
	*len = end - s;
	*val = number(s);
	if (*val == 0 && !(*s == '0' && !isdec(s[1])))
		*val = -1;
	return s;
}

/*
 * evalop applies the operator at op in expr to the numbers on either
 * side, and writes the result in decimal in their place. It returns the
 * result, or -1 for a missing number or an operator other than '*',
 * '/', '+', and '-'.
 */
int
evalop(char *expr, char *op)
{
	int a, b, len, c;
	char *s;

	c = *op;
	s = operand(expr, op, -1, &a, &len);
	if (a == -1)
		return -1;
	cutstr(s, len);
	op -= len;
	operand(expr, op, 1, &b, &len);
	if (b == -1)
		return -1;
	cutstr(op, len + 1);
	switch (c) {
	case '*':
		a *= b;
		break;
	case '/':
		a /= b;
		break;
	case '+':
		a += b;
		break;
	case '-':
		a -= b;
		break;
	default:
		return -1;
	}
	wsprintf(numbuf, "%d", a);
	insstr(op, numbuf);
	return a;
}

/*
 * evalexpr evaluates the integer expression s: every * first, then
 * every /, then the additions and subtractions from left to right. It
 * returns 0 for an expression with parentheses or one that evalop
 * rejects.
 */
int
evalexpr(char *s)
{
	char *p;

	getexpr(exprbuf, s, sizeof exprbuf);
	for (p = exprbuf; *p; p++)
		if (*p == '(')
			return 0;
mul:
	for (p = exprbuf; *p; p++)
		if (*p == '*') {
			if (evalop(exprbuf, p) == -1)
				return 0;
			goto mul;
		}
div:
	for (p = exprbuf; *p; p++)
		if (*p == '/') {
			if (evalop(exprbuf, p) == -1)
				return 0;
			goto div;
		}
add:
	for (p = exprbuf; *p; p++)
		if (*p == '+' || *p == '-') {
			if (evalop(exprbuf, p) == -1)
				return 0;
			goto add;
		}
	return number(exprbuf);
}

char *
firstline(char *s)
{
	int i;

	i = 0;
	while (*s != 0 && *s != 13 && *s != 10 && i < sizeof linebuf - 1)
		linebuf[i++] = *s++;
	linebuf[i] = 0;
	return linebuf;
}

/*
 * nextline moves *pp to the start of the next line, past the CRs, LFs,
 * spaces, tabs, and backspaces there, and reports whether text remains.
 */
int
nextline(char **pp)
{
	while (**pp != 13) {
		if (**pp == 0)
			return 0;
		(*pp)++;
	}
	while (**pp == 13 || **pp == 10 || **pp == 32 || **pp == 8 || **pp == 9)
		(*pp)++;
	if (**pp == 0)
		return 0;
	return 1;
}

int
skipcomma(char **pp)
{
	while (**pp != ',') {
		if (**pp == 0)
			return 0;
		(*pp)++;
	}
	(*pp)++;
	return 1;
}

int
lineat(char *s, char *p)
{
	char *q;
	int n;

	n = 1;
	for (q = s; q <= p; q++) {
		if (*q == 13)
			n++;
		else if (*q == 0)
			break;
	}
	return n;
}

int
contains(char *s, char *t)
{
	for (; *s; s++)
		if (*s == *t && prefixlen(s, t))
			return 1;
	return 0;
}

/*
 * pathcat joins dir and name in dst, with a separator unless dir is
 * empty or ends in one: a slash when dir holds slashes and no
 * backslash, else a backslash.
 */
void
pathcat(char *dst, char *dir, char *name)
{
	int nback, n, nslash;
	char c;

	if (*dir == 0)
		;
	else {
		nslash = 0;
		nback = 0;
		n = 0;
		do {
			c = *dir++;
			if (c == '\\')
				nback++;
			else if (c == '/')
				nslash++;
			*dst++ = c;
			n++;
		} while (c);
		dst--;
		if (n > 1 && dst[-1] != '\\' && dst[-1] != '/') {
			if (!nslash || nback)
				*dst++ = '\\';
			else
				*dst++ = '/';
		}
	}
	while ((*dst++ = *name++) != 0)
		;
}

/*
 * splitname cuts the last component off path and copies it to name
 * unless name is NULL. Its scan starts at the byte after the
 * terminating NUL.
 */
void
splitname(char *path, char *name)
{
	char *p;

	if (path[0] == 0)
		return;
	p = path;
	while (*p++)
		;
	for (;;) {
		if (p <= path)
			break;
		if (*p == '\\' || *p == '/') {
			if (name)
				copystr(name, p + 1);
			*p = 0;
			return;
		}
		if (p <= path || p[-1] != ':')
			p--;
		else
			break;
	}
	if (name)
		copystr(name, p);
	*p = 0;
}

/*
 * addname copies dir to dst and appends name, after a backslash unless
 * dir ends in a slash, a backslash, or a colon.
 */
void
addname(char *dst, char *dir, char *name)
{
	char *p;

	if (dst != dir)
		copystr(dst, dir);
	for (p = dst; *p; p++)
		;
	if (p > dst)
		p--;
	if (*p == 0x5c || *p == '/' || *p == ':')
		p++;
	else {
		while (*p)
			p++;
		*p++ = '\\';
	}
	copystr(p, name);
}

/*
 * trimtail cuts the CRs, LFs, and spaces off the end of s, keeping its
 * first character, sets tailstart to s, and returns the new end.
 */
char *
trimtail(char *s)
{
	tailstart = s;
	for (; *s; s++)
		;
	while ((*s == 13 || *s == 10 || *s == ' ' || *s == 0) && s > tailstart)
		s--;
	*++s = 0;
	return s;
}

void
crlfs(char *s, int n)
{
	while (n-- > 0) {
		*s++ = '\r';
		*s++ = '\n';
	}
	*s = 0;
}

void
countlines(char *s, int max)
{
	for (nlines = 0; *s && nlines != max; s++)
		if (*s == 13)
			nlines++;
}

void
subchar(char *s, char from, char to)
{
	for (; *s; s++)
		if (*s == from)
			*s = to;
}

int
lineno(char *s, char *p)
{
	int n;

	n = 1;
	for (; s <= p; s++)
		if (*s == 13)
			n++;
	return n;
}

/*
 * fmtcomma formats n with numsep between the thousands in the next of
 * ten buffers, so that ten results can be in use at once.
 */
char *
fmtcomma(DWORD n)
{
	DWORD d, q;
	int started;
	char *p;

	if (!numsep)
		numsep = ',';
	if (++fmtidx >= 10)
		fmtidx = 0;
	p = fmtbuf[fmtidx];
	started = 0;
	d = 1000000000;
	for (;;) {
		q = n / d;
		n %= d;
		if (q != 0 || started || !(d >= 10)) {
			*p++ = q + '0';
			started = 1;
			if (d == 1000000 || d == 1000)
				*p++ = numsep;
		}
		if (d == 1)
			break;
		d /= 10;
		if (d == 0)
			d = 1;
	}
	*p = 0;
	return fmtbuf[fmtidx];
}

/*
 * fmtsize formats the byte count n in megabytes with two decimals, or
 * in kilobytes when the part below a megabyte is under 10240 bytes.
 */
char *
fmtsize(long n)
{
	int frac;

	frac = n % 1048576 / 10240;
	if (frac >= 1)
		wsprintf(sizebuf, "%s.%02d MB", fmtcomma(n / 1048576), frac);
	else
		wsprintf(sizebuf, "%s kB", fmtcomma(n / 1024));
	return sizebuf;
}

char *
skipwhite(char *s)
{
	while (*s == ' ' || *s == 13 || *s == 10 || *s == 9)
		s++;
	return s;
}

/*
 * skipcomment skips white space and the lines that start with //.
 */
char *
skipcomment(char *s)
{
	do {
		s = skipwhite(s);
		if (s[0] != '/' || s[1] != '/')
			break;
		while (*s != '\0' && *s != 13)
			s++;
	} while (*s != '\0');
	return s;
}

/*
 * gettoken copies the item that the comma-separated list s starts with
 * to dst, and returns the start of the next item.
 */
char *
gettoken(char *dst, char *s)
{
	s = skipcomment(s);
	for (; *s && *s != ',' && *s != 13;)
		*dst++ = *s++;
	*dst = 0;
	if (*s == ',')
		s++;
	return skipcomment(s);
}

char *
getlong(long *v, char *s)
{
	s = gettoken(tokbuf, s);
	*v = atol(tokbuf);
	return s;
}

char *
strend(char *s)
{
	for (; *s; s++)
		;
	return s;
}

void
trim(char *s)
{
	char *p;

	for (p = s; *p; p++)
		;
	while (p > s && (p[-1] == 13 || p[-1] == 10 || p[-1] == ' ' || p[-1] == 9))
		p--;
	*p = 0;
}

void
trimleft(char *s)
{
	char *p;

	p = skipwhite(s);
	while ((*s++ = *p++) != 0)
		;
}

void
strip(char *s, char c)
{
	char *p, *d;

	for (p = s; *p == c; p++)
		;
	d = s;
	while ((*d++ = *p++) != 0)
		;
	for (p = s; *p; p++)
		;
	while (p > s && p[-1] == c)
		p--;
	*p = 0;
}

char *
crlf(char *s)
{
	*s++ = '\r';
	*s++ = '\n';
	return s;
}

/*
 * getarg copies at most n-2 characters of s to dst, up to a comma or a
 * closing parenthesis, and returns s after the comma. An n of 0 or less
 * sets no limit.
 */
char *
getarg(char *dst, int n, char *s)
{
	if (n <= 0)
		n = 0xfffffff;
	for (;;) {
		if (*s == ',' || *s == ')' || *s == 0 || --n <= 1)
			break;
		*dst++ = *s++;
	}
	*dst = 0;
	if (*s == ',')
		s++;
	return s;
}

void
cutline(char *s)
{
	for (; *s; s++)
		if (*s == 13) {
			*s = 0;
			return;
		}
}

char *
copyline(char *dst, int n, char *s)
{
	for (; *s && *s != 13 && n-- > 1;)
		*dst++ = *s++;
	*dst = 0;
	return s;
}

char *
copyword(char *dst, int n, char *s)
{
	for (; *s && *s != 13 && *s != ' ' && n-- > 1;)
		*dst++ = *s++;
	*dst = 0;
	return s;
}

char *
fileext(char *s)
{
	char *p;

	for (p = strend(s); p > s; p--)
		if (*p == '.')
			return p;
	return s;
}

void
cutext(char *s)
{
	char *p;

	for (p = strend(s); p > s; p--)
		if (*p == '.') {
			*p = 0;
			return;
		}
}

/*
 * lessnocase reports whether a sorts before b, with the letters A to Z
 * taken as lowercase.
 */
int
lessnocase(char *a, char *b)
{
	for (; *b; a++, b++) {
		if (!*a)
			return 1;
		lessa = *a;
		if (lessa >= 'A' && lessa <= 'Z')
			lessa += 32;
		lessb = *b;
		if (lessb >= 'A' && lessb <= 'Z')
			lessb += 32;
		if (lessa < lessb)
			return 1;
		if (lessb < lessa)
			return 0;
	}
	return 0;
}

/*
 * addsorted appends a comma to a and inserts it into the comma-separated
 * list before the first item that a sorts before by less, or by
 * lessnocase when less is NULL, or else at the end. It reads each item
 * into tok.
 */
void
addsorted(char *a, char *list, char *tok, int (*less)(char *, char *))
{
	char *prev;

	if (less == NULL)
		less = lessnocase;
	do {
		prev = list;
		list = gettoken(tok, list);
		if (tok[0] == 0)
			break;
		if (less(a, tok)) {
			catstr(a, ",");
			insstr(prev, a);
			return;
		}
	} while (list && *list);
	if (list) {
		catstr(a, ",");
		catstr(list, a);
	}
}

/*
 * sortlist builds in dst the items of the list s in sorted order and
 * returns their count.
 */
int
sortlist(char *dst, char *s, char *tok, char *buf, int (*less)(char *, char *))
{
	int n;

	dst[0] = 0;
	n = 0;
	do {
		s = gettoken(tok, s);
		if (tok[0] == 0)
			break;
		addsorted(tok, dst, buf, less);
		n++;
	} while (s && *s);
	return n;
}

/*
 * shortpath returns a copy of s, or when s is max characters long or
 * longer, its two ends joined by [...]. A max over 100 counts as 100.
 */
char *
shortpath(char *s, int max)
{
	char *d, *p;
	int n, k;

	d = shortbuf;
	if (max > sizeof shortbuf)
		max = sizeof shortbuf;
	n = 0;
	for (p = s; *p; p++, n++)
		;
	if (n < max) {
		copystr(d, s);
		return shortbuf;
	}
	k = max / 2 - 3;
	memcpy(d, s, k);
	d += k;
	*d++ = '[';
	*d++ = '.';
	*d++ = '.';
	*d++ = '.';
	*d++ = ']';
	memcpy(d, p - k, k);
	d[k] = 0;
	return shortbuf;
}

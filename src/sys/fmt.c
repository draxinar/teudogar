/*
 * wsprintfA and wvsprintfA, as USER32 formats: at most 1,024 bytes
 * with the NUL, the conversions c, C, d, i, u, x, X, p, s, and S with
 * the flags -, #, and 0, a width, and a precision, and l meaning the
 * 32 bits of Windows' long, which the game's long has in the port.
 * An unknown conversion, a float's among them, pads an empty field from
 * its precision to its width, unless the field aligns left, and then
 * prints its letter. The count of a full buffer, 1,023 bytes, is 1,024,
 * as USER32's. The prefix of # stays within the 1,024 bytes, where
 * USER32's runs past them.
 */

#include <stdarg.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <windows.h>

#include "sys.h"

enum {
	Maxout = 1024,	/* the bytes wsprintf writes at most, with the NUL */
	Left = 1 << 0,	/* - */
	Hexprefix = 1 << 1,	/* # */
	Zero = 1 << 2,	/* 0 */
	Wide = 1 << 3,	/* w, or l with c and s */
	Short = 1 << 4,	/* h */
	Upper = 1 << 5	/* X, p */
};

/* the kinds of conversion */
enum {
	Kunknown,
	Kchar,
	Kwchar,
	Kstring,
	Kwstring,
	Ksigned,
	Kunsigned,
	Khex
};

static const WCHAR nowide[1];	/* the empty wide string */

typedef struct Spec Spec;
struct Spec {
	int flags;
	int width;
	int prec;
	int kind;
};

/* parse reads the conversion after a % into sp and returns where it ends. */
static const char *
parse(const char *f, Spec *sp)
{
	memset(sp, 0, sizeof *sp);
	if (*f == '-') {
		sp->flags |= Left;
		f++;
	}
	if (*f == '#') {
		sp->flags |= Hexprefix;
		f++;
	}
	if (*f == '0') {
		sp->flags |= Zero;
		f++;
	}
	while (*f >= '0' && *f <= '9')
		sp->width = sp->width * 10 + (*f++ - '0');
	if (*f == '.') {
		f++;
		while (*f >= '0' && *f <= '9')
			sp->prec = sp->prec * 10 + (*f++ - '0');
	}
	if (*f == 'l' || *f == 'w')
		sp->flags |= Wide, f++;
	else if (*f == 'h')
		sp->flags |= Short, f++;
	switch (*f) {
	case 'c':
		sp->kind = sp->flags & Wide ? Kwchar : Kchar;
		break;
	case 'C':
		sp->kind = sp->flags & Short ? Kchar : Kwchar;
		break;
	case 'd':
	case 'i':
		sp->kind = Ksigned;
		break;
	case 'u':
		sp->kind = Kunsigned;
		break;
	case 'p':
		sp->width = 8;
		sp->flags |= Zero | Upper;
		sp->kind = Khex;
		break;
	case 'X':
		sp->flags |= Upper;
		sp->kind = Khex;
		break;
	case 'x':
		sp->kind = Khex;
		break;
	case 's':
		sp->kind = sp->flags & Wide ? Kwstring : Kstring;
		break;
	case 'S':
		sp->kind = sp->flags & Short ? Kstring : Kwstring;
		break;
	default:
		sp->kind = Kunknown;
		return f;
	}
	return f + 1;
}

/* digits writes n in base into buf, most significant first, and returns their count. */
static int
digits(char *buf, uint32_t n, int base, int upper)
{
	const char *set;
	char tmp[16];
	int i, len;

	set = upper ? "0123456789ABCDEF" : "0123456789abcdef";
	i = 0;
	do {
		tmp[i++] = set[n % base];
		n /= base;
	} while (n != 0);
	for (len = 0; i > 0; len++)
		buf[len] = tmp[--i];
	return len;
}

int
wvsprintfA(LPSTR buf, LPCSTR f, va_list ap)
{
	char num[16], one[1], *p;
	const char *s;
	const WCHAR *ws;
	int left, len, prec, sign, prefix, pad, i;
	uint32_t u;
	int32_t v;
	Spec sp;

	p = buf;
	left = Maxout;
	while (*f != '\0' && left > 1) {
		if (*f != '%') {
			*p++ = *f++;
			left--;
			continue;
		}
		f++;
		if (*f == '%') {
			*p++ = *f++;
			left--;
			continue;
		}
		f = parse(f, &sp);
		/* the prefix takes its room first, so the bounds below hold the rest */
		prefix = sp.kind == Khex && (sp.flags & Hexprefix) && left > 3;
		if (prefix)
			left -= 2;
		if (sp.width > left - 1)
			sp.width = left - 1;
		if (sp.kind == Kunknown) {
			if (!(sp.flags & Left))
				for (pad = sp.prec; pad < sp.width && left > 1; pad++, left--)
					*p++ = ' ';
			continue;
		}
		if (sp.flags & Left)
			sp.flags &= ~Zero;
		s = NULL;
		ws = NULL;
		sign = 0;
		switch (sp.kind) {
		case Kchar:
			one[0] = (char)va_arg(ap, int);
			s = one;
			len = 1;
			prec = 1;
			break;
		case Kwchar:
			one[0] = (char)(WCHAR)va_arg(ap, int);
			s = one;
			len = 1;
			prec = 1;
			break;
		case Kstring:
			s = va_arg(ap, const char *);
			if (s == NULL)
				s = "(null)";
			for (len = 0; (sp.prec == 0 || len < sp.prec) && s[len] != '\0'; len++)
				;
			if (len > left - 1)
				len = left - 1;
			prec = len;
			break;
		case Kwstring:
			ws = va_arg(ap, const WCHAR *);
			if (ws == NULL)
				ws = nowide;
			for (len = 0; (sp.prec == 0 || len < sp.prec) && ws[len] != 0; len++)
				;
			if (len > left - 1)
				len = left - 1;
			prec = len;
			break;
		default:
			if (sp.kind == Ksigned) {
				v = (int32_t)va_arg(ap, int);
				if (v < 0) {
					sign = 1;
					u = (uint32_t)0 - (uint32_t)v;
				} else
					u = (uint32_t)v;
			} else
				u = (uint32_t)va_arg(ap, unsigned int);
			len = digits(num, u, sp.kind == Khex ? 16 : 10, sp.flags & Upper);
			len += sign;
			if (len > left - 1)
				len = left - 1;
			prec = sp.prec < len ? len : sp.prec;
			if (prec > left - 1)
				prec = left - 1;
			if ((sp.flags & Zero) && sp.width > prec)
				prec = sp.width;
			break;
		}
		if (!(sp.flags & Left))
			for (pad = prec; pad < sp.width && left > 1; pad++, left--)
				*p++ = ' ';
		switch (sp.kind) {
		case Kchar:
		case Kwchar:
		case Kstring:
			memcpy(p, s, len);
			p += len;
			break;
		case Kwstring:
			for (i = 0; i < len; i++)
				*p++ = (char)ws[i];
			break;
		default:
			if (prefix) {
				*p++ = '0';
				*p++ = sp.flags & Upper ? 'X' : 'x';
			}
			if (sign)
				*p++ = '-';
			for (i = len; i < prec; i++, left--)
				*p++ = '0';
			memcpy(p, num, len - sign);
			p += len - sign;
			break;
		}
		if (sp.flags & Left)
			for (pad = prec; pad < sp.width && left > 1; pad++, left--)
				*p++ = ' ';
		left -= len;
	}
	*p = '\0';
	return left > 1 ? (int)(p - buf) : Maxout;
}

int
wsprintfA(LPSTR buf, LPCSTR f, ...)
{
	va_list ap;
	int n;

	va_start(ap, f);
	n = wvsprintfA(buf, f, ap);
	va_end(ap);
	return n;
}

#include <windows.h>
#include <stdarg.h>
#include <string.h>
#include "base.h"

int xwritten;
int xreadn;

void
invert(char *p, int n)
{
	for (; n; n--)
		*p++ ^= 0xff;
}

char *
memopen(char *buf)
{
	return buf;
}

int
memwrite(void *buf, int size, int n, char **p)
{
	size *= n;
	memcpy(*p, buf, size);
	*p += size;
	return size;
}

int
xmemwrite(void *buf, int size, int n, char **p)
{
	invert((char *)buf, size * n);
	xwritten = memwrite(buf, size, n, p);
	invert((char *)buf, size * n);
	return xwritten;
}

int
memread(void *buf, int size, int n, char **p)
{
	size *= n;
	memcpy(buf, *p, size);
	*p += size;
	return size;
}

int
memio(void *buf, int size, int n, char **p, int write)
{
	if (write)
		return memwrite(buf, size, n, p);
	return memread(buf, size, n, p);
}

int
xmemread(void *buf, int size, int n, char **p)
{
	xreadn = memread(buf, size, n, p);
	invert((char *)buf, size * n);
	return xreadn;
}

int
memputc(int c, char **p)
{
	*(*p)++ = c;
	return c;
}

void
memprintf(char **p, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(*p, fmt, ap);
	// FIXED: va_start without va_end is undefined in C. End the list after its one use.
#ifdef PORT
	va_end(ap);
#endif
	while (**p)
		(*p)++;
}

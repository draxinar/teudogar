/*
 * The calls of zlib that zpack.cpp makes, for the port, which links the
 * system's zlib. The game passes lengths of 32 bits, which zlib's uLong
 * exceeds on LP64, so these wrappers pass zlib values of its own types.
 */

#include <zlib.h>

#include <SDL3/SDL.h>
#include <windows.h>

#include "sys.h"

int zcompress(unsigned char *dst, DWORD *dstlen, const unsigned char *src, DWORD srclen);
int zuncompress(unsigned char *dst, DWORD *dstlen, const unsigned char *src, DWORD srclen);
DWORD zcrc32(DWORD crc, const unsigned char *buf, unsigned int len);
const char *zerror(int err);

int
zcompress(unsigned char *dst, DWORD *dstlen, const unsigned char *src, DWORD srclen)
{
	uLongf n;
	int r;

	n = *dstlen;
	r = compress(dst, &n, src, srclen);
	*dstlen = (DWORD)n;
	return r;
}

int
zuncompress(unsigned char *dst, DWORD *dstlen, const unsigned char *src, DWORD srclen)
{
	uLongf n;
	int r;

	n = *dstlen;
	r = uncompress(dst, &n, src, srclen);
	*dstlen = (DWORD)n;
	return r;
}

DWORD
zcrc32(DWORD crc, const unsigned char *buf, unsigned int len)
{
	return (DWORD)crc32(crc, buf, len);
}

const char *
zerror(int err)
{
	return zError(err);
}

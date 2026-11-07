#include <windows.h>
#include <string.h>
#include "base.h"

// PORT: zlib through the wrappers of src/sys/zlib.c, which take the game's 32-bit lengths
#ifdef PORT
#define compress zcompress
#define uncompress zuncompress
#define crc32 zcrc32
#define zError zerror
int compress(unsigned char *dst, DWORD *dstlen, const unsigned char *src, DWORD srclen);
int uncompress(unsigned char *dst, DWORD *dstlen, const unsigned char *src, DWORD srclen);
DWORD crc32(DWORD crc, const unsigned char *buf, unsigned int len);
const char *zError(int err);
#else
extern "C" {
int compress(unsigned char *dst, unsigned long *dstlen, const unsigned char *src, unsigned long srclen);
int uncompress(unsigned char *dst, unsigned long *dstlen, const unsigned char *src, unsigned long srclen);
unsigned long crc32(unsigned long crc, const unsigned char *buf, unsigned int len);
const char *zError(int err);
}
#endif

extern int zret;
extern int crclen;

DWORD
pack(void *dst, DWORD dstlen, void *src, DWORD srclen)
{
	zret = compress((unsigned char *)dst, &dstlen, (unsigned char *)src, srclen);
	if (zret == 0)
		return dstlen;
	logf("zlib_compress %ld -> %ld (ret%ld): %s\r\n", srclen, dstlen, zret, zError(zret));
	return 0;
}

DWORD
unpack(void *dst, DWORD dstlen, void *src, DWORD srclen)
{
	zret = uncompress((unsigned char *)dst, &dstlen, (unsigned char *)src, srclen);
	if (zret == 0)
		return dstlen;
	logf("zlib_decompress %ld -> %ld (ret%ld): %s\r\n", srclen, dstlen, zret, zError(zret));
	return 0;
}

/*
 * crcfile reads past skip bytes of h and returns the CRC-32 of the
 * bytes from there up to offset end, or up to the end of the file when
 * end is 0, through a buffer of its own. It sets crclen to their count.
 */
unsigned long
crcfile(HANDLE h, int skip, int end)
{
	unsigned char *buf;
	int n, left, chunk;
	unsigned long crc;

	crclen = 0;
	if (end != 0 && end <= skip)
		return 0;
	buf = (unsigned char *)GlobalAlloc(0, 0x40000);
	if (buf == NULL)
		return 0;
	left = end;
	if (end == 0)
		left = 0x1fffffff;
	left -= skip;
	while (skip > 0) {
		chunk = 0x40000;
		if (skip < chunk)
			chunk = skip;
		n = fileread(buf, 1, chunk, h);
		if (n < chunk)
			goto fail;
		skip -= n;
	}
	crc = crc32(0, NULL, 0);
	do {
		if (left > 0x40000)
			chunk = 0x40000;
		else
			chunk = left;
		memset(buf, 0, 0x40000);
		n = fileread(buf, 1, chunk, h);
		if (n == 0)
			break;
		left -= n;
		crc = crc32(crc, buf, n);
		crclen += n;
	} while (n >= chunk && left > 0);
	GlobalFree(buf);
	return crc;
fail:
	GlobalFree(buf);
	return 0;
}

unsigned long
crcname(char *name, int skip, int end)
{
	HANDLE h;
	unsigned long crc;

	crclen = 0;
	if (end != 0 && end <= skip)
		return 0;
	if (!fopenr(&h, name))
		return 0;
	crc = crcfile(h, skip, end);
	fileclose(&h);
	return crc;
}

unsigned long
crcbuf(void *buf, int n)
{
	return crc32(crc32(0, NULL, 0), (unsigned char *)buf, n);
}

int zret;
int crclen;

//go:build cgo

// Package zlib114 compresses data with the deflate of zlib 1.1.4, the
// release linked into the game, the installer, and the tool that wrote
// the game's data files. Its output equals theirs byte for byte, where Go's
// compress/zlib, and later releases of zlib, choose other matches.
//
// The C files are zlib 1.1.4's, as the tarball that sum/zlib.sha256 pins
// holds them, under the license at the head of zlib.h.
package zlib114

/*
#cgo CFLAGS: -DZ_PREFIX -UTARGET_OS_MAC -std=gnu89
// zconf.h leaves Byte to the old Mac OS wherever TARGET_OS_MAC is
// defined, as Apple's clang defines it and Apple's headers do again:
// -UTARGET_OS_MAC takes the compiler's away, and zlib.h comes before
// string.h, as in zlib's own files.
#include "zlib.h"
#include <string.h>

// zcompress does what compress2 does, with memlevel for deflateInit2.
static int
zcompress(unsigned char *dst, unsigned long *dstlen, const unsigned char *src, unsigned long srclen, int level, int memlevel)
{
	z_stream z;
	int err;

	memset(&z, 0, sizeof z);
	err = deflateInit2(&z, level, Z_DEFLATED, MAX_WBITS, memlevel, Z_DEFAULT_STRATEGY);
	if (err != Z_OK)
		return err;
	z.next_in = (Bytef *)src;
	z.avail_in = srclen;
	z.next_out = dst;
	z.avail_out = *dstlen;
	err = deflate(&z, Z_FINISH);
	if (err != Z_STREAM_END) {
		deflateEnd(&z);
		return err == Z_OK ? Z_BUF_ERROR : err;
	}
	*dstlen = z.total_out;
	return deflateEnd(&z);
}
*/
import "C"

import (
	"fmt"
	"unsafe"
)

// The compression levels, as zlib.h defines them, and the memory levels,
// as zconf.h and zutil.h do.
const (
	NoCompression      = 0
	BestSpeed          = 1
	BestCompression    = 9
	DefaultCompression = -1 // level 6
	DefaultMemLevel    = 8
	MaxMemLevel        = 9
)

// Compress returns src compressed in the zlib format (RFC 1950) at level,
// with the deflate state's memory level memLevel, from 1 to 9, as zlib
// 1.1.4's deflate writes it in one call. Its compress and compress2 give
// DefaultMemLevel. The data files hold streams that compress, at
// DefaultCompression, wrote.
func Compress(src []byte, level, memLevel int) ([]byte, error) {
	if level < DefaultCompression || level > BestCompression {
		return nil, fmt.Errorf("zlib114: level %d: want -1 to 9", level)
	}
	if memLevel < 1 || memLevel > MaxMemLevel {
		return nil, fmt.Errorf("zlib114: memory level %d: want 1 to 9", memLevel)
	}
	// Deflate ends a block once it holds 1<<(memLevel+6) less 1 symbols,
	// each of a byte or more, or at level 0 once it holds 4<<(memLevel+6)
	// less 5 bytes, and a block takes at most 5 bytes beyond its data: 4%
	// more at memLevel 1.
	dst := make([]byte, len(src)+len(src)/16+64)
	n := C.ulong(len(dst))
	var p *C.uchar
	if len(src) > 0 {
		p = (*C.uchar)(unsafe.Pointer(&src[0]))
	}
	err := C.zcompress((*C.uchar)(unsafe.Pointer(&dst[0])), &n, p, C.ulong(len(src)), C.int(level), C.int(memLevel))
	if err != 0 {
		return nil, fmt.Errorf("zlib114: deflate: error %d", int(err))
	}
	return dst[:n], nil
}

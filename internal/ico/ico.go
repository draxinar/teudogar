// Package ico decodes Windows icon files, as cmd/rc writes the release's:
// a directory of images, each a device-independent bitmap of twice the
// icon's height, its color rows over its transparency mask.
package ico

import (
	"encoding/binary"
	"errors"
	"fmt"
)

// ErrFormat is the error that Decode wraps for input outside the kinds of
// icon it reads.
var ErrFormat = errors.New("ico: bad format")

// Image is one image of an icon, top row first.
type Image struct {
	W, H int
	Pix  []uint32 // W*H pixels, each 0xAARRGGBB
}

const (
	dirLen   = 6
	entryLen = 16
	infoLen  = 40
	maxSide  = 256
)

// Decode returns the largest image of the icon file b, the first of
// those that tie. It reads uncompressed bitmaps of 1, 4, 8, 24, and 32
// bits a pixel.
func Decode(b []byte) (*Image, error) {
	if len(b) < dirLen {
		return nil, fmt.Errorf("%w: %d bytes of directory", ErrFormat, len(b))
	}
	le := binary.LittleEndian
	if le.Uint16(b) != 0 || le.Uint16(b[2:]) != 1 {
		return nil, fmt.Errorf("%w: reserved %d, type %d", ErrFormat, le.Uint16(b), le.Uint16(b[2:]))
	}
	n := int(le.Uint16(b[4:]))
	if n == 0 || len(b) < dirLen+n*entryLen {
		return nil, fmt.Errorf("%w: %d images in %d bytes", ErrFormat, n, len(b))
	}
	best, area := -1, 0
	for i := range n {
		w, h := side(b[dirLen+i*entryLen]), side(b[dirLen+i*entryLen+1])
		if w*h > area {
			best, area = i, w*h
		}
	}
	e := b[dirLen+best*entryLen:]
	size, off := int64(le.Uint32(e[8:])), int64(le.Uint32(e[12:]))
	if off+size > int64(len(b)) {
		return nil, fmt.Errorf("%w: image of %d bytes at %d in %d", ErrFormat, size, off, len(b))
	}
	return decodeDIB(b[off:off+size], side(e[0]), side(e[1]))
}

// side returns the side that a directory's byte gives, where 0 means 256.
func side(b byte) int {
	if b == 0 {
		return maxSide
	}
	return int(b)
}

func decodeDIB(d []byte, w, h int) (*Image, error) {
	le := binary.LittleEndian
	if len(d) < infoLen {
		return nil, fmt.Errorf("%w: %d bytes of bitmap header", ErrFormat, len(d))
	}
	hdr := int64(le.Uint32(d))
	dw, dh := int32(le.Uint32(d[4:])), int32(le.Uint32(d[8:]))
	bits := int(le.Uint16(d[14:]))
	comp := le.Uint32(d[16:])
	used := int64(le.Uint32(d[32:]))
	if hdr < infoLen || hdr > int64(len(d)) {
		return nil, fmt.Errorf("%w: bitmap header of %d bytes", ErrFormat, hdr)
	}
	if int64(dw) != int64(w) || int64(dh) != 2*int64(h) {
		return nil, fmt.Errorf("%w: bitmap of %dx%d for an icon of %dx%d", ErrFormat, dw, dh, w, h)
	}
	if comp != 0 {
		return nil, fmt.Errorf("%w: compression %d", ErrFormat, comp)
	}
	switch bits {
	case 1, 4, 8, 24, 32:
	default:
		return nil, fmt.Errorf("%w: %d bits a pixel", ErrFormat, bits)
	}
	var pal []byte
	p := hdr
	if bits <= 8 {
		colors := used
		if colors == 0 {
			colors = 1 << bits
		}
		if colors > 1<<bits || p+colors*4 > int64(len(d)) {
			return nil, fmt.Errorf("%w: palette of %d colors", ErrFormat, colors)
		}
		pal = d[p : p+colors*4]
		p += colors * 4
	}
	stride := int64((w*bits + 31) / 32 * 4)
	maskStride := int64((w + 31) / 32 * 4)
	if p+stride*int64(h)+maskStride*int64(h) > int64(len(d)) {
		return nil, fmt.Errorf("%w: rows of %d and %d bytes past %d", ErrFormat, stride, maskStride, len(d))
	}
	colors, mask := d[p:p+stride*int64(h)], d[p+stride*int64(h):]
	m := &Image{W: w, H: h, Pix: make([]uint32, w*h)}
	for y := range h {
		row := colors[int64(h-1-y)*stride:]
		mrow := mask[int64(h-1-y)*maskStride:]
		for x := range w {
			var c uint32
			switch bits {
			case 24:
				c = 0xff000000 | uint32(row[x*3+2])<<16 | uint32(row[x*3+1])<<8 | uint32(row[x*3])
			case 32:
				c = le.Uint32(row[x*4:])
			default:
				i := int(row[x*bits/8]>>(8-bits-x*bits%8)) & (1<<bits - 1)
				if i*4+3 >= len(pal) {
					return nil, fmt.Errorf("%w: color %d of %d", ErrFormat, i, len(pal)/4)
				}
				c = 0xff000000 | uint32(pal[i*4+2])<<16 | uint32(pal[i*4+1])<<8 | uint32(pal[i*4])
			}
			if bits != 32 && mrow[x/8]>>(7-x%8)&1 != 0 {
				c = 0
			}
			m.Pix[y*w+x] = c
		}
	}
	return m, nil
}

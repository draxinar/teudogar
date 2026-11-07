// Package packed reads and writes the data that teudogar.exe carries
// packed with zlib (RFC 1950) in DGROUP. The block at 004f1388, which
// unpack (004930d0, unpack.cpp) inflates at startup and then clears,
// holds the program's three fonts, the tables of the entries of the
// data files, and its texts. Four more streams hold the pictures that
// loadsurfs (0043db50, graphics.cpp) inflates into the surfaces surfc and
// surfpg.
//
// The program's code gives the address and the length of each stream as
// constants, so the writers keep each stream's length: they fail for
// data that zlib 1.1.4's compress, which packed the release's streams,
// packs into another length. Words are 32 bits, little-endian.
package packed

import (
	"bytes"
	"compress/zlib"
	"encoding/binary"
	"errors"
	"fmt"
	"io"

	"github.com/draxinar/teudogar/internal/zlib114"
)

// The block's address in teudogar.exe and its length.
const (
	BlockAddr = 0x4f1388
	BlockLen  = 0x8038
)

// The sizes of the glyphs: 24 by 20 pixels in the font and the capitals,
// 12 by 10 in the small font, a byte a pixel, row by row from the top.
const (
	NGlyphs   = 256
	GlyphW    = 24
	GlyphH    = 20
	SmallW    = 12
	SmallH    = 10
	glyphSize = GlyphW * GlyphH
	smallSize = SmallW * SmallH
)

// NFiles is the count of data files, and NEntries the length of their
// rows in the tables of entries.
const (
	NFiles   = 9
	NEntries = 500
)

// TextsLen is the length of the texts, and TextsKey the byte that each
// byte of the stored texts is XORed with.
const (
	TextsLen = 0xbb46
	TextsKey = 0xf7
)

// countsOff is the offset in the block of the entry counts, which unpack
// copies as they are.
const countsOff = 0x30f5

// A Block holds the parts of the block in the order of the block, each
// as unpack leaves it in memory.
type Block struct {
	// Font holds the glyphs of 24 by 20 (font, 009cf9c0) that glyph3
	// (graphics.cpp) draws: 0 is clear, 0xf0 to 0xf3 the ink, and 0xf4 the
	// shade.
	Font [NGlyphs][GlyphW * GlyphH]byte

	// Small holds the glyphs of 12 by 10 (font2, 009ed9c0) that
	// drawchar2 draws.
	Small [NGlyphs][SmallW * SmallH]byte

	// Capitals holds colored glyphs of 24 by 20 (font3, 009f51c0),
	// which glyph3 draws for the capital of each letter under text3f.
	Capitals [NGlyphs][GlyphW * GlyphH]byte

	// Counts holds the count of entries of each data file (0090015c),
	// less than NEntries.
	Counts [NFiles]uint32

	// Offsets holds, for each data file, the offset of each entry and
	// the end of the last (00900180), and Lengths the running sum of
	// their decoded lengths (009047d0). Entry e of file f spans
	// Offsets[f][e] to Offsets[f][e+1]. Words past a file's row hold
	// 0x01010101.
	Offsets [NFiles][NEntries]uint32
	Lengths [NFiles][NEntries]uint32

	// Texts holds the program's texts (texts, 00a131c0), which T(off)
	// names by their offsets, XORed back.
	Texts [TextsLen]byte
}

// A part is a stream of the block: its offset and length in the block,
// as unpack passes them to unpack (0040f7a8, zpack.cpp), and the length
// of the buffer that it fills.
type part struct {
	name   string
	off, n int
	size   int
}

// parts lists the streams of the block in block order. The entry counts
// lie between the capitals and the offsets.
var parts = [...]part{
	{"font", 0x0000, 0x143c, NGlyphs * glyphSize},
	{"small font", 0x143c, 0x0835, NGlyphs * smallSize},
	{"capitals", 0x1c71, 0x1484, NGlyphs * glyphSize},
	{"offsets", 0x3119, 0x093e, NFiles * NEntries * 4},
	{"lengths", 0x3a57, 0x093c, NFiles * NEntries * 4},
	{"texts", 0x4393, 0x3ca5, TextsLen},
}

// ErrFormat reports bytes that are not a block or a picture as the
// program reads them. Errors from DecodeBlock and Picture.Decode wrap
// it.
var ErrFormat = errors.New("packed: bad format")

func formatError(format string, args ...any) error {
	return fmt.Errorf("%w: %s", ErrFormat, fmt.Sprintf(format, args...))
}

// inflate returns the bytes that b holds: a zlib stream that ends b and
// fills size bytes, as the program's buffer takes it.
func inflate(b []byte, size int) ([]byte, error) {
	// A bytes.Reader serves flate byte by byte, so Len counts the
	// bytes that follow the stream.
	r := bytes.NewReader(b)
	zr, err := zlib.NewReader(r)
	if err != nil {
		return nil, err
	}
	data, err := io.ReadAll(io.LimitReader(zr, int64(size)+1))
	if err != nil {
		return nil, err
	}
	switch {
	case len(data) > size:
		return nil, fmt.Errorf("stream holds more than %d bytes", size)
	case len(data) < size:
		return nil, fmt.Errorf("stream holds %d bytes, want %d", len(data), size)
	case r.Len() != 0:
		return nil, fmt.Errorf("%d bytes follow the stream", r.Len())
	}
	return data, nil
}

// deflate returns the zlib stream that holds data as zlib 1.1.4's
// compress packs it, which must take n bytes.
func deflate(data []byte, n int) ([]byte, error) {
	z, err := zlib114.Compress(data, zlib114.DefaultCompression, zlib114.DefaultMemLevel)
	if err != nil {
		return nil, err
	}
	if len(z) != n {
		return nil, fmt.Errorf("stream of %d bytes, and the program reads %d", len(z), n)
	}
	return z, nil
}

// DecodeBlock returns the block that b, the BlockLen bytes at BlockAddr,
// holds. Each stream must fill the span that unpack reads and the
// buffer that it inflates into, and each count of entries must be less
// than NEntries, so that its row of offsets holds the end of its last
// entry.
func DecodeBlock(b []byte) (*Block, error) {
	if len(b) != BlockLen {
		return nil, formatError("block of %d bytes, want %d", len(b), BlockLen)
	}
	var data [len(parts)][]byte
	for i, p := range parts {
		d, err := inflate(b[p.off:p.off+p.n], p.size)
		if err != nil {
			return nil, formatError("%s at %#x: %v", p.name, p.off, err)
		}
		data[i] = d
	}
	blk := new(Block)
	for i := range NGlyphs {
		copy(blk.Font[i][:], data[0][i*glyphSize:])
		copy(blk.Small[i][:], data[1][i*smallSize:])
		copy(blk.Capitals[i][:], data[2][i*glyphSize:])
	}
	for f := range NFiles {
		off := countsOff + 4*f
		blk.Counts[f] = binary.LittleEndian.Uint32(b[off:])
		if blk.Counts[f] >= NEntries {
			return nil, formatError("count of file %d at %#x: %d entries, want 0 to %d", f, off, blk.Counts[f], NEntries-1)
		}
		for e := range NEntries {
			blk.Offsets[f][e] = binary.LittleEndian.Uint32(data[3][4*(f*NEntries+e):])
			blk.Lengths[f][e] = binary.LittleEndian.Uint32(data[4][4*(f*NEntries+e):])
		}
	}
	for i, c := range data[5] {
		blk.Texts[i] = c ^ TextsKey
	}
	return blk, nil
}

// EncodeBlock returns the block that holds blk, laid out as unpack reads
// it. It fails when a part's stream takes another length than the
// program's code gives.
func EncodeBlock(blk *Block) ([]byte, error) {
	var data [len(parts)][]byte
	for i := range data {
		data[i] = make([]byte, 0, parts[i].size)
	}
	for i := range NGlyphs {
		data[0] = append(data[0], blk.Font[i][:]...)
		data[1] = append(data[1], blk.Small[i][:]...)
		data[2] = append(data[2], blk.Capitals[i][:]...)
	}
	for f := range NFiles {
		for e := range NEntries {
			data[3] = binary.LittleEndian.AppendUint32(data[3], blk.Offsets[f][e])
			data[4] = binary.LittleEndian.AppendUint32(data[4], blk.Lengths[f][e])
		}
	}
	for _, c := range blk.Texts {
		data[5] = append(data[5], c^TextsKey)
	}
	b := make([]byte, BlockLen)
	for i, p := range parts {
		z, err := deflate(data[i], p.n)
		if err != nil {
			return nil, fmt.Errorf("packed: %s at %#x: %v", p.name, p.off, err)
		}
		copy(b[p.off:], z)
	}
	for f, n := range blk.Counts {
		binary.LittleEndian.PutUint32(b[countsOff+4*f:], n)
	}
	return b, nil
}

// A Picture is one of the pictures that loadsurfs inflates from DGROUP:
// a picture of W by H pixels, a byte a pixel, row by row from the top,
// packed into the Len bytes at Addr. loadsurfs passes Addr and Len to
// unpack.
type Picture struct {
	Addr uint32
	Len  int
	W, H int
}

// Pictures holds the four pictures in the order of loadsurfs. The first
// three hold 640 by 384 pixels each, sheets of 104 tiles of 48 by 48,
// and the fourth 640 by 480, a sheet of window pictures.
var Pictures = [...]Picture{
	{0x4c9f40, 0x6fb7, 640, 384},  // surfc at y 0: tiles of 48 by 48, 13 to a row
	{0x4d0ef8, 0x5beb, 640, 384},  // surfc at y 384: more tiles
	{0x4d6ae8, 0x36b2, 640, 384},  // surfc at y 768: icons of 24 by 20, 26 to a row, in 240 rows that loadsurfs copies
	{0x4da4a0, 0x162e4, 640, 480}, // surfpg: the scroll behind the pages of text
}

// Decode returns the pixels of p that b, the Len bytes at Addr, holds.
func (p Picture) Decode(b []byte) ([]byte, error) {
	if len(b) != p.Len {
		return nil, formatError("picture at %08x: %d bytes, want %d", p.Addr, len(b), p.Len)
	}
	pix, err := inflate(b, p.W*p.H)
	if err != nil {
		return nil, formatError("picture at %08x: %v", p.Addr, err)
	}
	return pix, nil
}

// Encode returns the Len bytes at Addr that hold pix, the pixels of p.
// It fails when the stream takes another length than Len.
func (p Picture) Encode(pix []byte) ([]byte, error) {
	if len(pix) != p.W*p.H {
		return nil, fmt.Errorf("packed: picture at %08x: %d pixels, want %d by %d", p.Addr, len(pix), p.W, p.H)
	}
	z, err := deflate(pix, p.Len)
	if err != nil {
		return nil, fmt.Errorf("packed: picture at %08x: %v", p.Addr, err)
	}
	return z, nil
}

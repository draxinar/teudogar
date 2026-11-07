package lang

import (
	"bytes"
	"compress/zlib"
	"fmt"
	"io"
	"os"
	"path/filepath"

	"github.com/draxinar/teudogar/internal/packed"
	"github.com/draxinar/teudogar/internal/pe"
)

// A Release holds the files of the game that the build starts from: the
// data files, and the packed block of teudogar.exe with the tables of
// their entries and the program's texts.
type Release struct {
	Dir   string // the release's folder
	Exe   []byte
	Block *packed.Block

	// Files holds the data files by number: Teudogar_00.Dat and
	// Teudogar_02.Dat to Teudogar_07.Dat, and teudogar.dat as file 8.
	// Teudogar_01.Dat keeps an index of its own and stays out.
	Files [packed.NFiles][]byte
}

// DataName returns the name of data file f in the release's folder,
// with the lowercase names of build/zip.
func DataName(f int) string {
	if f == 8 {
		return "teudogar.dat"
	}
	return fmt.Sprintf("teudogar_%02d.dat", f)
}

// OpenRelease reads the release in the folder dir: teudogar.exe and the
// data files.
func OpenRelease(dir string) (*Release, error) {
	exe, err := os.ReadFile(filepath.Join(dir, "teudogar.exe"))
	if err != nil {
		return nil, err
	}
	blk, err := ReadBlock(exe)
	if err != nil {
		return nil, err
	}
	r := &Release{Dir: dir, Exe: exe, Block: blk}
	for f := range packed.NFiles {
		if f == 1 {
			continue
		}
		r.Files[f], err = os.ReadFile(filepath.Join(dir, DataName(f)))
		if err != nil {
			return nil, err
		}
	}
	return r, nil
}

// ReadBlock returns the packed block of the program exe.
func ReadBlock(exe []byte) (*packed.Block, error) {
	f, err := pe.Parse(exe)
	if err != nil {
		return nil, err
	}
	b := f.Bytes(packed.BlockAddr-f.Opt.ImageBase, packed.BlockLen)
	if b == nil {
		return nil, fmt.Errorf("lang: packed block at %08x outside the program's data", packed.BlockAddr)
	}
	return packed.DecodeBlock(b)
}

// split returns the file and the entry of id, as splitid (datfile.cpp)
// splits it.
func (r *Release) split(id int) (f, e int, err error) {
	f, e = id/1000, id%1000
	if f < 0 || f >= packed.NFiles || f == 1 || e >= int(r.Block.Counts[f]) {
		return 0, 0, fmt.Errorf("lang: entry %d: no such entry", id)
	}
	return f, e, nil
}

// Stored returns the bytes of entry id as its file stores them.
func (r *Release) Stored(id int) ([]byte, error) {
	f, e, err := r.split(id)
	if err != nil {
		return nil, err
	}
	lo, hi := r.Block.Offsets[f][e], r.Block.Offsets[f][e+1]
	if lo > hi || int(hi) > len(r.Files[f]) {
		return nil, fmt.Errorf("lang: entry %d: bytes %#x to %#x of %s, which holds %#x", id, lo, hi, DataName(f), len(r.Files[f]))
	}
	return r.Files[f][lo:hi], nil
}

// DecodedLen returns the decoded length that the tables give entry id.
func (r *Release) DecodedLen(id int) (int, error) {
	f, e, err := r.split(id)
	if err != nil {
		return 0, err
	}
	return int(r.Block.Lengths[f][e+1] - r.Block.Lengths[f][e]), nil
}

// Decoded returns the decoded bytes of entry id: its zlib stream
// inflated, as unpack (zpack.cpp) inflates it, or its stored bytes when
// it holds no stream, as a placeholder or entry 8311 does.
func (r *Release) Decoded(id int) ([]byte, error) {
	b, err := r.Stored(id)
	if err != nil {
		return nil, err
	}
	n, err := r.DecodedLen(id)
	if err != nil {
		return nil, err
	}
	if !IsStream(b) {
		return b, nil
	}
	d, err := zlibDecode(b)
	if err != nil {
		return nil, fmt.Errorf("lang: entry %d: %v", id, err)
	}
	if len(d) != n {
		return nil, fmt.Errorf("lang: entry %d decodes to %d bytes, the tables give %d", id, len(d), n)
	}
	return d, nil
}

// zlibDecode returns the decoded bytes of the zlib stream z.
func zlibDecode(z []byte) ([]byte, error) {
	zr, err := zlib.NewReader(bytes.NewReader(z))
	if err != nil {
		return nil, err
	}
	return io.ReadAll(zr)
}

// IsStream reports whether b starts as a zlib stream (RFC 1950) of the
// deflate method, without a preset dictionary.
func IsStream(b []byte) bool {
	if len(b) < 2 {
		return false
	}
	cmf, flg := b[0], b[1]
	return cmf&0x0f == 8 && cmf>>4 <= 7 && (uint(cmf)<<8|uint(flg))%31 == 0 && flg&0x20 == 0
}

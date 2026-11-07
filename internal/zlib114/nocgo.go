//go:build !cgo

package zlib114

import "errors"

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

// Compress returns an error: the deflate of zlib 1.1.4 needs cgo.
func Compress(src []byte, level, memLevel int) ([]byte, error) {
	return nil, errors.New("zlib114: built without cgo")
}

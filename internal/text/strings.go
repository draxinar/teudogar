package text

import (
	"bytes"
	"fmt"
	"strings"
)

// The game keeps at most MaxStrings strings in strs (lang.cpp), and
// loadstrs copies them, each with a NUL, into strbuf, which holds
// MaxStringBytes bytes.
const (
	MaxStrings     = 2300
	MaxStringBytes = 0x19000
)

// stringEnd closes each string of an entry of strings.
const stringEnd = "#\r\n"

// DecodeStrings returns the strings of an entry of strings (5003, 5004,
// or 5007), as loadstrs (004a815c, lang.cpp) reads them. The entry,
// each byte XORed with Key, holds each string followed by "#" and CR
// LF. A string may span lines, and ends at its first "#".
func DecodeStrings(b []byte) ([]string, error) {
	t := xor(b)
	var strs []string
	size := 0
	for off := 0; off < len(t); {
		n := bytes.IndexByte(t[off:], '#')
		if n < 0 {
			return nil, formatError("string %d at %#x: missing the closing #", len(strs), off)
		}
		if !bytes.HasPrefix(t[off+n:], []byte(stringEnd)) {
			return nil, formatError("string %d at %#x: # at %#x, want # and CR LF", len(strs), off, off+n)
		}
		if len(strs) == MaxStrings {
			return nil, formatError("string at %#x past the limit of %d", off, MaxStrings)
		}
		size += n + 1
		if size > MaxStringBytes {
			return nil, formatError("string %d at %#x: strings fill %d bytes, want at most %d", len(strs), off, size, MaxStringBytes)
		}
		strs = append(strs, string(t[off:off+n]))
		off += n + len(stringEnd)
	}
	return strs, nil
}

// EncodeStrings returns the entry that holds strs, the inverse of
// DecodeStrings.
func EncodeStrings(strs []string) ([]byte, error) {
	if len(strs) > MaxStrings {
		return nil, fmt.Errorf("text: %d strings, want at most %d", len(strs), MaxStrings)
	}
	var b []byte
	size := 0
	for i, s := range strs {
		if strings.IndexByte(s, '#') >= 0 {
			return nil, fmt.Errorf("text: string %d holds a #", i)
		}
		size += len(s) + 1
		b = append(b, s...)
		b = append(b, stringEnd...)
	}
	if size > MaxStringBytes {
		return nil, fmt.Errorf("text: strings fill %d bytes, want at most %d", size, MaxStringBytes)
	}
	return xor(b), nil
}

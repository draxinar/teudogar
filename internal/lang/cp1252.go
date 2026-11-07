package lang

import (
	"fmt"
	"strings"
	"unicode/utf8"
)

// high maps the bytes 0x80 to 0x9f of Windows code page 1252 to their
// runes. The five bytes that the code page leaves unassigned map to the
// C1 controls of the same value, as Windows' MultiByteToWideChar maps
// them, so that every byte survives a round trip.
var high = [32]rune{
	0x20ac, 0x0081, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021,
	0x02c6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008d, 0x017d, 0x008f,
	0x0090, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
	0x02dc, 0x2122, 0x0161, 0x203a, 0x0153, 0x009d, 0x017e, 0x0178,
}

// fromHigh is the inverse of high.
var fromHigh = func() map[rune]byte {
	m := make(map[rune]byte, len(high))
	for i, r := range high {
		m[r] = byte(0x80 + i)
	}
	return m
}()

// FromCP1252 returns the text that b, in code page 1252, holds, in UTF-8.
func FromCP1252(b []byte) string {
	var s strings.Builder
	s.Grow(len(b))
	for _, c := range b {
		switch {
		case c < 0x80, c >= 0xa0:
			s.WriteRune(rune(c))
		default:
			s.WriteRune(high[c-0x80])
		}
	}
	return s.String()
}

// ToCP1252 returns s, in UTF-8, in code page 1252. It fails on a byte
// sequence that is not UTF-8 and on a rune that the code page lacks,
// naming its line.
func ToCP1252(s string) ([]byte, error) {
	b := make([]byte, 0, len(s))
	line := 1
	for i, r := range s {
		switch {
		case r == utf8.RuneError && !strings.HasPrefix(s[i:], "\uFFFD"):
			return nil, fmt.Errorf("line %d: bytes that are not UTF-8", line)
		case r < 0x80, r >= 0xa0 && r <= 0xff:
			b = append(b, byte(r))
		default:
			c, ok := fromHigh[r]
			if !ok {
				return nil, fmt.Errorf("line %d: %q (U+%04X) is outside code page 1252", line, r, r)
			}
			b = append(b, c)
		}
		if r == '\n' {
			line++
		}
	}
	return b, nil
}

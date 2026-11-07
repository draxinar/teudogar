package lang

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"strconv"
	"strings"

	"github.com/draxinar/teudogar/internal/packed"
)

// The fonts of the packed block, as glyphs.txt names them: font and
// fontw, small and fontw2, capitals and fontw3 (unpack.cpp).
const (
	Font = iota
	Small
	Capitals
	NFonts
)

// FontNames names the fonts in glyphs.txt.
var FontNames = [NFonts]string{"font", "small", "capitals"}

// fontSize returns the width and the height of the glyphs of font f.
func fontSize(f int) (w, h int) {
	if f == Small {
		return packed.SmallW, packed.SmallH
	}
	return packed.GlyphW, packed.GlyphH
}

// A GlyphEdit is an edit of glyphs.txt: the cells to change in a glyph,
// and its width, or the glyph to derive it from with an accent.
type GlyphEdit struct {
	Font   int
	Code   int
	Width  int     // the glyph's new width, or -1 to keep it
	Cells  [][]int // a row of cells for each row of the glyph: -1 keeps the pixel, else its new color
	Base   int     // the code of the glyph that the edit derives this one from, or -1
	Accent string  // the accent of the derived glyph: a key of Accents
	Right  int     // the code of the glyph that a join puts right of Base's first At columns, or -1
	At     int
	Line   int
}

// Accents holds the shapes of the accents that a derived glyph draws in
// its three top rows, # for a pixel, centered over the body: two rows of
// accent, and a row that keeps it apart from the letter.
var Accents = map[string][]string{
	"acute":      {"..###", "###..", "....."},
	"grave":      {"###..", "..###", "....."},
	"circumflex": {"..##..", "##..##", "......"},
	"diaeresis":  {"##..##", "##..##", "......"},
}

// ReadGlyphs parses glyphs.txt. An edit starts with a line that names
// the font (font, small, or capitals), the code of the glyph in hex,
// and its new width in pixels, or - to keep it, separated by blanks.
// Then come the rows of the glyph, 20 of 24 cells (10 of 12 for small),
// each cell two characters: two dots keep the pixel, two hyphens clear
// it, and two hex digits set its color. An edit that derives a glyph
// is a line alone: the font, the code, the word from, the code of the
// base glyph, the name of an accent of Accents, and the width. So is an
// edit that joins two glyphs: the font, the code, the word join, the
// codes of the left and the right glyphs, the column where the right
// one starts, and the width. Blank lines and lines that start with #
// are comments.
func ReadGlyphs(rd io.Reader) ([]GlyphEdit, error) {
	var es []GlyphEdit
	var e *GlyphEdit
	sc := bufio.NewScanner(rd)
	for n := 1; sc.Scan(); n++ {
		l := sc.Text()
		if l == "" || l[0] == '#' {
			if e != nil {
				return nil, fmt.Errorf("line %d: glyph of line %d with %d rows, want %d", n, e.Line, len(e.Cells), rowsOf(e.Font))
			}
			continue
		}
		if e == nil {
			g, err := glyphHead(l, n)
			if err != nil {
				return nil, err
			}
			es = append(es, g)
			if g.Base < 0 {
				e = &es[len(es)-1]
			}
			continue
		}
		w, h := fontSize(e.Font)
		if len(l) != 2*w {
			return nil, fmt.Errorf("line %d: row of %d characters, want %d cells of 2", n, len(l), w)
		}
		row := make([]int, w)
		for x := range w {
			c := l[2*x : 2*x+2]
			switch c {
			case "..":
				row[x] = -1
			case "--":
				row[x] = 0
			default:
				v, err := strconv.ParseUint(c, 16, 8)
				if err != nil {
					return nil, fmt.Errorf("line %d: cell %q, want .., --, or two hex digits", n, c)
				}
				row[x] = int(v)
			}
		}
		e.Cells = append(e.Cells, row)
		if len(e.Cells) == h {
			e = nil
		}
	}
	if err := sc.Err(); err != nil {
		return nil, err
	}
	if e != nil {
		return nil, fmt.Errorf("glyph of line %d with %d rows, want %d", e.Line, len(e.Cells), rowsOf(e.Font))
	}
	return es, nil
}

// rowsOf returns the height of the glyphs of font f.
func rowsOf(f int) int {
	_, h := fontSize(f)
	return h
}

// glyphHead parses the first line of an edit, line n of glyphs.txt.
func glyphHead(l string, n int) (GlyphEdit, error) {
	f := strings.Fields(l)
	if len(f) != 3 && !(len(f) == 6 && f[2] == "from") && !(len(f) == 7 && f[2] == "join") {
		return GlyphEdit{}, fmt.Errorf("line %d: %q, want a font, a code in hex, and a width, or a font, a code, from, a code, an accent, and a width, or a font, a code, join, two codes, a column, and a width", n, l)
	}
	e := GlyphEdit{Font: -1, Base: -1, Right: -1, Line: n}
	for i, name := range FontNames {
		if f[0] == name {
			e.Font = i
		}
	}
	if e.Font < 0 {
		return GlyphEdit{}, fmt.Errorf("line %d: font %q, want font, small, or capitals", n, f[0])
	}
	c, err := strconv.ParseUint(f[1], 16, 8)
	if err != nil {
		return GlyphEdit{}, fmt.Errorf("line %d: code %q, want two hex digits", n, f[1])
	}
	e.Code = int(c)
	if len(f) == 7 {
		gw, _ := fontSize(e.Font)
		left, err1 := strconv.ParseUint(f[3], 16, 8)
		right, err2 := strconv.ParseUint(f[4], 16, 8)
		at, err3 := strconv.Atoi(f[5])
		w, err4 := strconv.Atoi(f[6])
		if err1 != nil || err2 != nil || err3 != nil || err4 != nil || at < 1 || at >= gw || w < 1 || w > gw {
			return GlyphEdit{}, fmt.Errorf("line %d: %q, want join, two codes in hex, a column, and a width up to %d", n, l, gw)
		}
		e.Base, e.Right, e.At, e.Width = int(left), int(right), at, w
		return e, nil
	}
	if e.Width, err = glyphWidth(f[len(f)-1], e.Font); err != nil {
		return GlyphEdit{}, fmt.Errorf("line %d: %v", n, err)
	}
	if len(f) == 6 {
		b, err := strconv.ParseUint(f[3], 16, 8)
		if err != nil {
			return GlyphEdit{}, fmt.Errorf("line %d: base %q, want two hex digits", n, f[3])
		}
		if _, ok := Accents[f[4]]; !ok {
			return GlyphEdit{}, fmt.Errorf("line %d: accent %q, want acute, grave, circumflex, or diaeresis", n, f[4])
		}
		e.Base, e.Accent = int(b), f[4]
		return e, nil
	}
	return e, nil
}

// glyphWidth returns the width s of a glyph of font f: 1 to the width
// of its cells, or -1 for -, which keeps the release's.
func glyphWidth(s string, f int) (int, error) {
	if s == "-" {
		return -1, nil
	}
	gw, _ := fontSize(f)
	w, err := strconv.Atoi(s)
	if err != nil || w < 1 || w > gw {
		return 0, fmt.Errorf("width %q, want 1 to %d or -", s, gw)
	}
	return w, nil
}

// A GlyphPatch is a glyph that the build puts in place of the
// release's: its pixels, a byte each, row by row.
type GlyphPatch struct {
	Font int
	Code int
	Pix  []byte
}

// A WidthPatch is a width that the build gives a code of a font.
type WidthPatch struct {
	Font  int
	Code  int
	Width int
}

// Apply returns the glyph of e over the release's glyph of the block
// blk, and the widths that e sets: a width of the capitals goes to the
// capital's code and to its small letter's, since text3 (graphics.cpp)
// advances by the width of the code as written and draws the capital's
// glyph.
func (e GlyphEdit) Apply(blk *packed.Block) (GlyphPatch, []WidthPatch) {
	glyph := func(c int) []byte {
		switch e.Font {
		case Font:
			return blk.Font[c][:]
		case Small:
			return blk.Small[c][:]
		}
		return blk.Capitals[c][:]
	}
	w, h := fontSize(e.Font)
	var pix []byte
	switch {
	case e.Right >= 0:
		pix = join(glyph(e.Base), glyph(e.Right), w, h, e.At)
	case e.Base >= 0:
		pix = derive(glyph(e.Base), w, h, Accents[e.Accent])
	default:
		old := glyph(e.Code)
		pix = make([]byte, len(old))
		copy(pix, old)
	}
	for y, row := range e.Cells {
		for x, c := range row {
			if c >= 0 {
				pix[y*w+x] = byte(c)
			}
		}
	}
	g := GlyphPatch{Font: e.Font, Code: e.Code, Pix: pix}
	if e.Width < 0 {
		return g, nil
	}
	ws := []WidthPatch{{e.Font, e.Code, e.Width}}
	if s := int(small(byte(e.Code))); e.Font == Capitals && s != e.Code {
		ws = append(ws, WidthPatch{e.Font, s, e.Width})
	}
	return g, ws
}

// small returns the small letter of c in code page 1252.
func small(c byte) byte {
	switch {
	case c >= 'A' && c <= 'Z', c >= 0xc0 && c <= 0xde && c != 0xd7:
		return c + 0x20
	case c == 0x8a, c == 0x8c:
		return c + 0x10
	case c == 0x9f:
		return 0xff
	}
	return c
}

// derive returns the glyph base, w by h pixels, with accent in its top
// rows. The body keeps its bottom row, and to make room it loses the
// rows that differ least from the row above them, keeping its two first
// and last rows. The accent takes the commonest color of the body, and
// centers over it: an accent wider than the room left of the body moves
// the glyph right by the columns that it lacks.
func derive(base []byte, w, h int, accent []string) []byte {
	rows := make([][]byte, h)
	top, bottom := -1, -1
	for y := range h {
		rows[y] = base[y*w : (y+1)*w]
		if bytes.Count(rows[y], []byte{0}) < w {
			if top < 0 {
				top = y
			}
			bottom = y
		}
	}
	out := make([]byte, w*h)
	if top < 0 {
		return out
	}
	body := rows[top : bottom+1]
	drop := make(map[int]bool)
	for range len(body) - (bottom + 1 - len(accent)) {
		best, cost := -1, w+1
		for i := 2; i < len(body)-2; i++ {
			if drop[i] || drop[i-1] || drop[i+1] {
				continue
			}
			if c := rowDiff(body[i], body[i-1]); c < cost {
				best, cost = i, c
			}
		}
		if best < 0 {
			break
		}
		drop[best] = true
	}
	var kept [][]byte
	for i, r := range body {
		if !drop[i] {
			kept = append(kept, r)
		}
	}
	start := bottom + 1 - len(kept)
	lo, hi := w, -1
	count := make(map[byte]int)
	for _, r := range kept {
		for x, p := range r {
			if p != 0 {
				lo, hi = min(lo, x), max(hi, x)
				count[p]++
			}
		}
	}
	var color byte
	for p, n := range count {
		if n > count[color] || (n == count[color] && p < color) {
			color = p
		}
	}
	x0 := lo + (hi-lo+1-len(accent[0]))/2
	shift := max(-x0, 0)
	for i, r := range kept {
		copy(out[(start+i)*w+shift:(start+i+1)*w], r)
	}
	for y, row := range accent {
		for x, c := range row {
			if c == '#' && x0+shift+x < w {
				out[y*w+x0+shift+x] = color
			}
		}
	}
	return out
}

// join returns the first at columns of the glyph left, w by h pixels,
// with the glyph right drawn from column at on, over left's pixels.
func join(left, right []byte, w, h, at int) []byte {
	out := make([]byte, w*h)
	for y := range h {
		copy(out[y*w:y*w+at], left[y*w:y*w+at])
		for x := 0; at+x < w; x++ {
			if p := right[y*w+x]; p != 0 {
				out[y*w+at+x] = p
			}
		}
	}
	return out
}

// rowDiff returns the count of the pixels that one of the rows a and b
// draws and the other leaves clear.
func rowDiff(a, b []byte) int {
	n := 0
	for x := range a {
		if (a[x] != 0) != (b[x] != 0) {
			n++
		}
	}
	return n
}

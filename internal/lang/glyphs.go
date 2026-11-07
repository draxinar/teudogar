package lang

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

package lang

import (
	"bytes"
	"cmp"
	"fmt"
	"hash/crc32"
	"os"
	"path/filepath"
	"slices"
	"sort"
	"strings"

	"github.com/draxinar/teudogar/internal/packed"
	"github.com/draxinar/teudogar/internal/pe"
	"github.com/draxinar/teudogar/internal/zlib114"
)

// A Build holds what a feature build writes: the data files, with the
// entries that its features replace, the readmes, and the tables of the
// program that describe them: the tables of entries, the sizes and the
// CRC-32 of the files that checkfiles (system.cpp) checks, the patch of the
// program's texts, and the columns of the grammar tables.
type Build struct {
	r       *Release
	entries [packed.NFiles][]entry
	readmes map[string][]byte
	filled  []int // the ids of the entries that the release holds placeholders in, and the build fills
	Langs   [NLangs]bool
	// Composed holds the calls that compose each string of strs into a
	// buffer (ScanComposed), and Room the bytes that a string's buffers
	// hold for it, where they were checked.
	Composed map[int][]Site
	Room     map[int]int
	Texts    []TextPatch
	Gram     []GramPatch
	Glyphs   []GlyphPatch
	Widths   []WidthPatch
}

// An entry is an entry of a data file: its bytes as the file stores
// them, and the length the tables give its decoded bytes.
type entry struct {
	stored  []byte
	decoded int
}

// A TextPatch replaces the string of the program's texts at Off. Lang is
// the language it holds for, or -1 for all.
type TextPatch struct {
	Off  int
	Lang int
	Text []byte // in code page 1252, as the program draws it
}

// A GramPatch sets the gender and the number of the item names of one
// language in a row of armorgram or weapongram (texts.cpp).
type GramPatch struct {
	Type   int // the item's type: 0x1a0 to 0x1c6 for armorgram, 0x1c7 to 0x1e0 and 0x528 for weapongram
	Lang   int
	Gender byte // m, f, or n
	Number byte // s or p
}

// NewBuild returns the build that starts from the release r: its data
// files and readmes as the release has them, and empty lists of patches.
func NewBuild(r *Release) (*Build, error) {
	b := &Build{r: r, readmes: make(map[string][]byte)}
	b.Langs[German], b.Langs[English], b.Langs[Italian] = true, true, true
	for f := range packed.NFiles {
		if f == 1 {
			continue
		}
		for e := range int(r.Block.Counts[f]) {
			id := 1000*f + e
			s, err := r.Stored(id)
			if err != nil {
				return nil, err
			}
			d, err := r.DecodedLen(id)
			if err != nil {
				return nil, err
			}
			b.entries[f] = append(b.entries[f], entry{s, d})
		}
	}
	return b, nil
}

// SetEntry makes d the decoded bytes of entry id, stored as a zlib
// stream as zlib 1.1.4's compress writes it, which the game's readers
// inflate.
func (b *Build) SetEntry(id int, d []byte) error {
	f, e, err := b.r.split(id)
	if err != nil {
		return err
	}
	z, err := zlib114.Compress(d, zlib114.DefaultCompression, zlib114.DefaultMemLevel)
	if err != nil {
		return fmt.Errorf("lang: entry %d: %v", id, err)
	}
	b.entries[f][e] = entry{z, len(d)}
	return nil
}

// Fill makes d the decoded bytes of entry id, which holds a placeholder
// in the release. langentry (datfile.cpp) reads a filled entry in its
// own language.
func (b *Build) Fill(id int, d []byte) error {
	old, err := b.r.Decoded(id)
	if err != nil {
		return err
	}
	if len(old) > 3 {
		return fmt.Errorf("lang: entry %d holds text in the release, want a placeholder", id)
	}
	if err := b.SetEntry(id, d); err != nil {
		return err
	}
	if !slices.Contains(b.filled, id) {
		b.filled = append(b.filled, id)
	}
	return nil
}

// Entry returns the decoded bytes of entry id as the build holds them.
func (b *Build) Entry(id int) ([]byte, error) {
	f, e, err := b.r.split(id)
	if err != nil {
		return nil, err
	}
	en := b.entries[f][e]
	if !IsStream(en.stored) {
		return en.stored, nil
	}
	return inflate(en.stored, en.decoded)
}

// File returns data file f as the build writes it: its entries one
// after another, which cover the file.
func (b *Build) File(f int) []byte {
	if f == 1 {
		return b.r.Files[1]
	}
	var buf bytes.Buffer
	for _, en := range b.entries[f] {
		buf.Write(en.stored)
	}
	return buf.Bytes()
}

// SetReadme makes data the readme name of the program's folder.
func (b *Build) SetReadme(name string, data []byte) {
	b.readmes[name] = data
}

// Readme returns the readme name as the build holds it.
func (b *Build) Readme(name string) ([]byte, error) {
	if d, ok := b.readmes[name]; ok {
		return d, nil
	}
	return os.ReadFile(filepath.Join(b.r.Dir, name))
}

// Tables returns the tables of entries that the program needs for the
// build's data files: the count of entries of each file, and rows of
// offsets and of running decoded lengths, laid out as unpack (unpack.cpp)
// leaves them, with 0x01010101 past each row's end.
func (b *Build) Tables() (counts [packed.NFiles]uint32, offs, lens [packed.NFiles][packed.NEntries]uint32) {
	counts = b.r.Block.Counts
	offs, lens = b.r.Block.Offsets, b.r.Block.Lengths
	for f := range packed.NFiles {
		if f == 1 {
			continue
		}
		var o, l uint32
		for e, en := range b.entries[f] {
			offs[f][e], lens[f][e] = o, l
			o += uint32(len(en.stored))
			l += uint32(en.decoded)
		}
		n := len(b.entries[f])
		offs[f][n], lens[f][n] = o, l
	}
	return counts, offs, lens
}

// Checked lists the files of files[] (system.cpp, 16 rows), whose sizes and
// CRC-32 checkfiles compares with sizes[] and crcs[], each inverted.
// checkfiles skips Pad_File.Xml and only looks for teudogar.dat and
// teudogar.exe, and the features leave Teudogar_01.Dat, whose records
// the release holds, as it is, so the words of these four stay as the
// release has them.
var Checked = []string{
	"pad_file.xml", "order.htm",
	"teudogar_00.dat", "teudogar_01.dat", "teudogar_02.dat", "teudogar_03.dat",
	"teudogar_04.dat", "teudogar_05.dat", "teudogar_06.dat", "teudogar_07.dat",
	"teudogar.dat", "readme_d.txt", "readme_e.txt", "readme_i.txt",
	"teudogar.exe", "uninstal.exe",
}

// The addresses of crcs and sizes (system.cpp) in teudogar.exe, 30 words
// each.
const (
	crcsAddr  = 0x4c3750
	sizesAddr = 0x4c37c8
	nChecked  = 30
)

// Sums returns the words of sizes[] and crcs[] for the build's files.
func (b *Build) Sums() (sizes, crcs [nChecked]uint32, err error) {
	f, err := pe.Parse(b.r.Exe)
	if err != nil {
		return sizes, crcs, err
	}
	for i := range nChecked {
		s, ok1 := f.Word(sizesAddr + 4*uint32(i) - f.Opt.ImageBase)
		c, ok2 := f.Word(crcsAddr + 4*uint32(i) - f.Opt.ImageBase)
		if !ok1 || !ok2 {
			return sizes, crcs, fmt.Errorf("lang: sizes and crcs of teudogar.exe outside its data")
		}
		sizes[i], crcs[i] = s, c
	}
	for i, name := range Checked {
		if keepsSums(name) {
			continue
		}
		d, err := b.FileByName(name)
		if err != nil {
			return sizes, crcs, err
		}
		sizes[i], crcs[i] = ^uint32(len(d)), ^crc32.ChecksumIEEE(d)
	}
	return sizes, crcs, nil
}

// keepsSums reports whether the file name of Checked keeps the
// release's words in sizes[] and crcs[].
func keepsSums(name string) bool {
	return name == "pad_file.xml" || name == "teudogar.dat" || name == "teudogar.exe" || name == DataName(1)
}

// FileByName returns the file name of the program's folder as the build
// writes it.
func (b *Build) FileByName(name string) ([]byte, error) {
	for f := range packed.NFiles {
		if DataName(f) == name {
			return b.File(f), nil
		}
	}
	if strings.HasPrefix(name, "readme_") {
		return b.Readme(name)
	}
	return os.ReadFile(filepath.Join(b.r.Dir, name))
}

// Changed returns the names of the files of the program's folder that
// the build writes otherwise than the release has them.
func (b *Build) Changed() []string {
	var names []string
	for f := range packed.NFiles {
		if f != 1 && !bytes.Equal(b.File(f), b.r.Files[f]) {
			names = append(names, DataName(f))
		}
	}
	for name, d := range b.readmes {
		old, err := os.ReadFile(filepath.Join(b.r.Dir, name))
		if err != nil || !bytes.Equal(old, d) {
			names = append(names, name)
		}
	}
	sort.Strings(names)
	return names
}

// WriteHeaders writes into dir the C headers of the build for
// src/feature: tables.h, texts.h, gram.h, and font.h. cmd names the
// command that writes them, which their first lines record. The patches
// of the texts go in the order of their offsets and languages, and those
// of the grammar in the order of their types and languages, so that the
// features give the same headers in any order.
func (b *Build) WriteHeaders(dir, cmd string) error {
	sizes, crcs, err := b.Sums()
	if err != nil {
		return err
	}
	counts, offs, lens := b.Tables()
	var t bytes.Buffer
	head(&t, cmd)
	fmt.Fprintf(&t, "/* the tables of entries of the build's data files, and the sums of files[] */\n")
	words(&t, "int featcounts[9]", counts[:])
	rows(&t, "DWORD featoffs[9][500]", offs[:])
	rows(&t, "DWORD featlens[9][500]", lens[:])
	words(&t, "unsigned long featsizes[30]", sizes[:])
	words(&t, "unsigned long featcrcs[30]", crcs[:])
	fmt.Fprintf(&t, "\n/* the languages that the build offers, by number */\nchar featlangs[%d] = {", NLangs)
	for l, ok := range b.Langs {
		if l > 0 {
			t.WriteString(",")
		}
		if ok {
			t.WriteString(" 1")
		} else {
			t.WriteString(" 0")
		}
	}
	t.WriteString(" };\n")
	fmt.Fprintf(&t, "\n/* the entries that the release holds placeholders in, and the build fills */\n")
	filled := slices.Sorted(slices.Values(b.filled))
	fmt.Fprintf(&t, "int featids[%d] = {", len(filled)+1)
	for _, id := range filled {
		fmt.Fprintf(&t, " %d,", id)
	}
	fmt.Fprintf(&t, " -1 };\nint nfeatids = %d;\n", len(filled))
	if err := write(filepath.Join(dir, "tables.h"), t.String()); err != nil {
		return err
	}

	var x bytes.Buffer
	head(&x, cmd)
	fmt.Fprintf(&x, "/* the strings of the program's texts that the build replaces, by offset */\n")
	fmt.Fprintf(&x, "Feattext feattexts[] = {\n")
	patches := slices.Clone(b.Texts)
	slices.SortFunc(patches, func(a, c TextPatch) int { return cmp.Or(cmp.Compare(a.Off, c.Off), cmp.Compare(a.Lang, c.Lang)) })
	seen := make(map[[2]int]bool)
	for _, p := range patches {
		k := [2]int{p.Off, p.Lang}
		if seen[k] {
			return fmt.Errorf("lang: two patches of the texts at %#x for language %d", p.Off, p.Lang)
		}
		seen[k] = true
	}
	for _, p := range patches {
		fmt.Fprintf(&x, "\t{0x%04x, %d, %s},\n", p.Off, p.Lang, cString(p.Text))
	}
	fmt.Fprintf(&x, "\t{0xffff, -1, \"\"}\n};\n")
	fmt.Fprintf(&x, "int nfeattexts = %d;\n", len(patches))
	if err := write(filepath.Join(dir, "texts.h"), x.String()); err != nil {
		return err
	}

	var g bytes.Buffer
	head(&g, cmd)
	fmt.Fprintf(&g, "/* the gender and the number of the item names that the build sets */\n")
	fmt.Fprintf(&g, "Featgram featgrams[] = {\n")
	grams := slices.Clone(b.Gram)
	slices.SortFunc(grams, func(a, c GramPatch) int { return cmp.Or(cmp.Compare(a.Type, c.Type), cmp.Compare(a.Lang, c.Lang)) })
	for i, p := range grams {
		if i > 0 && grams[i-1].Type == p.Type && grams[i-1].Lang == p.Lang {
			return fmt.Errorf("lang: two patches of the grammar of type %#x for language %d", p.Type, p.Lang)
		}
		fmt.Fprintf(&g, "\t{0x%03x, %d, '%c', '%c'},\n", p.Type, p.Lang, p.Gender, p.Number)
	}
	fmt.Fprintf(&g, "\t{0, -1, 0, 0}\n};\n")
	fmt.Fprintf(&g, "int nfeatgrams = %d;\n", len(b.Gram))
	if err := write(filepath.Join(dir, "gram.h"), g.String()); err != nil {
		return err
	}

	var f bytes.Buffer
	head(&f, cmd)
	fmt.Fprintf(&f, "/* the glyphs of font (0), font2 (1), and font3 (2) that the build replaces */\n")
	fmt.Fprintf(&f, "Featglyph featglyphs[] = {\n")
	for _, p := range b.Glyphs {
		fmt.Fprintf(&f, "\t{%d, 0x%02x, %s},\n", p.Font, p.Code, cString(p.Pix))
	}
	fmt.Fprintf(&f, "\t{-1, 0, \"\"}\n};\nint nfeatglyphs = %d;\n\n", len(b.Glyphs))
	fmt.Fprintf(&f, "/* the widths of fontw (0), fontw2 (1), and fontw3 (2) that the build sets */\n")
	fmt.Fprintf(&f, "Featwidth featwidths[] = {\n")
	for _, p := range b.Widths {
		fmt.Fprintf(&f, "\t{%d, 0x%02x, %d},\n", p.Font, p.Code, p.Width)
	}
	fmt.Fprintf(&f, "\t{-1, 0, 0}\n};\nint nfeatwidths = %d;\n", len(b.Widths))
	return write(filepath.Join(dir, "font.h"), f.String())
}

// head writes the first lines of a generated header.
func head(w *bytes.Buffer, cmd string) {
	fmt.Fprintf(w, "/* Code generated by cmd/lang from lang/; DO NOT EDIT. */\n")
	fmt.Fprintf(w, "/* %s */\n\n", cmd)
}

// words writes a C array of 32-bit words.
func words(w *bytes.Buffer, decl string, v []uint32) {
	fmt.Fprintf(w, "%s = {", decl)
	for i, x := range v {
		if i%8 == 0 {
			w.WriteString("\n\t")
		} else {
			w.WriteString(" ")
		}
		fmt.Fprintf(w, "0x%08x,", x)
	}
	w.WriteString("\n};\n")
}

// rows writes a C array of rows of 32-bit words.
func rows(w *bytes.Buffer, decl string, v [][packed.NEntries]uint32) {
	fmt.Fprintf(w, "%s = {\n", decl)
	for _, r := range v {
		w.WriteString("{")
		for i, x := range r {
			if i%8 == 0 {
				w.WriteString("\n\t")
			} else {
				w.WriteString(" ")
			}
			fmt.Fprintf(w, "0x%08x,", x)
		}
		w.WriteString("\n},\n")
	}
	w.WriteString("};\n")
}

// cString returns s as C string literals, one per 64 bytes, which the
// compiler joins: printable ASCII as it is, and every other byte, the
// quote, the backslash, and the question mark in octal, which keeps
// trigraphs out.
func cString(s []byte) string {
	if len(s) == 0 {
		return `""`
	}
	var b strings.Builder
	for i := 0; i < len(s); i += 64 {
		if i > 0 {
			b.WriteString("\n\t\t")
		}
		b.WriteByte('"')
		for _, c := range s[i:min(i+64, len(s))] {
			if c >= 0x20 && c < 0x7f && c != '"' && c != '\\' && c != '?' {
				b.WriteByte(c)
				continue
			}
			fmt.Fprintf(&b, "\\%03o", c)
		}
		b.WriteByte('"')
	}
	return b.String()
}

// inflate returns the decoded bytes of the zlib stream z, which must
// decode to n bytes.
func inflate(z []byte, n int) ([]byte, error) {
	d, err := zlibDecode(z)
	if err != nil {
		return nil, err
	}
	if len(d) != n {
		return nil, fmt.Errorf("lang: stream decodes to %d bytes, want %d", len(d), n)
	}
	return d, nil
}

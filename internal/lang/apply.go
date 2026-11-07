package lang

import (
	"bufio"
	"bytes"
	"errors"
	"fmt"
	"io"
	"io/fs"
	"maps"
	"os"
	"path/filepath"
	"slices"
	"strconv"
	"strings"

	"github.com/draxinar/teudogar/internal/pe"
	"github.com/draxinar/teudogar/internal/text"
)

// Apply applies the sources of the feature name in the folder dir
// (lang/) to the build b, and returns the problems that its checks find.
func Apply(name string, b *Build, picks []Pick, dir string) ([]Problem, error) {
	switch name {
	case "errata":
		return applyErrata(b, picks, dir)
	}
	return nil, fmt.Errorf("lang: unknown feature %q, want errata", name)
}

// source returns the text of the file name of dir, or false when it is
// missing.
func source(dir, name string) (string, bool, error) {
	b, err := os.ReadFile(filepath.Join(dir, name))
	if errors.Is(err, fs.ErrNotExist) {
		return "", false, nil
	}
	if err != nil {
		return "", false, err
	}
	return string(b), true, nil
}

// splitStrings returns the strings of the source form s of an entry of
// strings.
func splitStrings(s string) []string {
	return strings.Split(strings.TrimSuffix(s, "#\n"), "#\n")
}

// within prefixes the units of ps with the file they come from.
func within(file string, ps []Problem) []Problem {
	for i := range ps {
		ps[i].Unit = file + ": " + ps[i].Unit
	}
	return ps
}

// groupDE returns the offset of the German string of the call of pick
// whose strings include the one at off, or off when no call passes it.
func groupDE(picks []Pick, off int) int {
	for _, p := range picks {
		if slices.Contains(p.Off[:], off) {
			return p.Off[German]
		}
	}
	return off
}

// storedText returns the string d of the texts at off as the program
// keeps it: XORed when its call of pick is one of Encoded.
func storedText(picks []Pick, off int, d []byte) []byte {
	if slices.Contains(Encoded, groupDE(picks, off)) {
		return XorStr(d)
	}
	return d
}

// GramTypes lists the item types of armorgram and weapongram
// (texts.cpp) that have a name: those whose entry of typenames (entry
// 5000) holds the index of a string.
var GramTypes = []int{
	0x1a0, 0x1a1, 0x1a2, 0x1a3, 0x1a7, 0x1a8, 0x1a9, 0x1ad, 0x1ae, 0x1af,
	0x1b0, 0x1b1, 0x1b2, 0x1b3, 0x1b4, 0x1b5, 0x1b7, 0x1b8, 0x1b9, 0x1ba,
	0x1bb, 0x1bc, 0x1bd, 0x1be, 0x1bf, 0x1c6,
	0x1c7, 0x1c8, 0x1c9, 0x1ca, 0x1cb, 0x1cc, 0x1cd, 0x1ce, 0x1cf, 0x1d0,
	0x1d1, 0x1d2, 0x1d3, 0x1d4, 0x1d5, 0x1d6, 0x1d7, 0x1d8, 0x528,
}

// An Erratum is a line of errata.txt: the unit that it fixes, and the
// text that it replaces there.
type Erratum struct {
	Unit     string // strings 36, manual 4, script 1049, texts 787a, gram 1ca, or readme
	Old, New string
	Line     int
}

// ReadErrata parses errata.txt: a comment line that starts with # says
// what each fix corrects, and a fix is the unit, a tab, the old text, a
// tab, and the new text, with the escapes of Escape.
func ReadErrata(rd io.Reader) ([]Erratum, error) {
	var es []Erratum
	sc := bufio.NewScanner(rd)
	sc.Buffer(nil, 1<<20)
	for n := 1; sc.Scan(); n++ {
		l := sc.Text()
		if l == "" || l[0] == '#' {
			continue
		}
		f := strings.Split(l, "\t")
		if len(f) != 3 {
			return nil, fmt.Errorf("line %d: want a unit, the old text, and the new text, separated by tabs", n)
		}
		old, err1 := Unescape(f[1])
		nw, err2 := Unescape(f[2])
		if err := errors.Join(err1, err2); err != nil {
			return nil, fmt.Errorf("line %d: %v", n, err)
		}
		if old == "" || old == nw {
			return nil, fmt.Errorf("line %d: want an old text, and a new one that differs", n)
		}
		if strings.ContainsRune(old+nw, 0) {
			return nil, fmt.Errorf("line %d: a NUL, which ends a string of the game", n)
		}
		es = append(es, Erratum{Unit: f[0], Old: old, New: nw, Line: n})
	}
	return es, sc.Err()
}

// errataLangs lists the languages whose texts errata fixes.
var errataLangs = []int{German, English, Italian}

// applyErrata puts lang/<code>/errata.txt into the build for German,
// English, and Italian.
func applyErrata(b *Build, picks []Pick, dir string) ([]Problem, error) {
	var ps []Problem
	for _, l := range errataLangs {
		file := filepath.Join("lang", Codes[l], "errata.txt")
		s, ok, err := source(filepath.Join(dir, Codes[l]), "errata.txt")
		if err != nil {
			return nil, err
		}
		if !ok {
			ps = append(ps, problem(file, "missing"))
			continue
		}
		es, err := ReadErrata(strings.NewReader(s))
		if err != nil {
			ps = append(ps, problem(file, "%v", err))
			continue
		}
		eps, err := b.fix(l, picks, es)
		if err != nil {
			return nil, err
		}
		ps = append(ps, within(file, eps)...)
	}
	return ps, nil
}

// fix applies the errata es of language l to the build. picks gives
// the calls of pick, whose groups of Encoded the program keeps XORed. A
// fix of the texts lies at the slot of l in a call of pick, whose string
// the game passes through feattext, and holds for l alone.
func (b *Build) fix(l int, picks []Pick, es []Erratum) ([]Problem, error) {
	var ps []Problem
	srcs := make(map[int]string)  // the source forms of the entries that the errata change
	texts := make(map[int]string) // the strings of the texts that they change, by offset
	grams := make(map[int]string) // the letters of the rows that they change, by type
	for _, e := range es {
		unit := fmt.Sprintf("line %d (%s)", e.Line, e.Unit)
		kind, arg, _ := strings.Cut(e.Unit, " ")
		switch kind {
		case "texts":
			off, err := strconv.ParseUint(arg, 16, 16)
			if err != nil {
				ps = append(ps, problem(unit, "offset %q, want hex", arg))
				continue
			}
			tx := b.r.Block.Texts[:]
			if int(off) >= len(tx) || (off > 0 && tx[off-1] != 0) {
				ps = append(ps, problem(unit, "offset %#x, want the start of a string of the texts", off))
				continue
			}
			if !slices.ContainsFunc(picks, func(p Pick) bool { return p.Off[l] == int(off) }) {
				ps = append(ps, problem(unit, "offset %#x, want a slot of pick for this language", off))
				continue
			}
			old, ok := texts[int(off)]
			if !ok {
				old = FromCP1252(b.r.plain(int(off), groupDE(picks, int(off))))
			}
			nw, err := replaceOnce(old, e.Old, e.New)
			if err != nil {
				ps = append(ps, problem(unit, "%v", err))
				continue
			}
			if _, err := ToCP1252(nw); err != nil {
				ps = append(ps, problem(unit, "%v", err))
				continue
			}
			// The fixed string keeps the marks of the German of its call
			// of pick, which a fix of a lost or a stray mark restores.
			de := groupDE(picks, int(off))
			if m, w := marks(nw, stringMarks), marks(FromCP1252(b.r.plain(de, de)), stringMarks); !slices.Equal(m, w) {
				ps = append(ps, problem(unit, "marks %q, want %q as the German has them", m, w))
				continue
			}
			texts[int(off)] = nw
		case "gram":
			t, err := strconv.ParseUint(arg, 16, 16)
			if err != nil || !slices.Contains(GramTypes, int(t)) || !isGram(e.New) {
				ps = append(ps, problem(unit, "want gram, a type of GramTypes in hex, and the gender (m, f, n) and number (s, p) it has and takes"))
				continue
			}
			old, ok := grams[int(t)]
			if !ok {
				if old, err = b.r.gram(int(t), l); err != nil {
					return nil, err
				}
			}
			if e.Old != old {
				ps = append(ps, problem(unit, "type %x is %s, want %s as the erratum reads", t, old, e.Old))
				continue
			}
			grams[int(t)] = e.New
		case "readme":
			name := Readmes[l]
			d, err := b.Readme(name)
			if err != nil {
				return nil, err
			}
			nw, err := replaceOnce(Plain(string(d)), e.Old, e.New)
			if err != nil {
				ps = append(ps, problem(unit, "%v", err))
				continue
			}
			c, err := Stored(nw)
			if err != nil {
				ps = append(ps, problem(unit, "%v", err))
				continue
			}
			b.SetReadme(name, []byte(c))
		default:
			tx, ok := textNamed(kind)
			if !ok {
				ps = append(ps, problem(unit, "unknown unit, want strings, manual, encyclopedia1, encyclopedia2, script, walkthrough, texts, gram, or readme"))
				continue
			}
			id := tx.ID + l
			s, ok := srcs[id]
			if !ok {
				d, err := b.Entry(id)
				if err != nil {
					return nil, err
				}
				if s, err = Source(tx.Kind, d); err != nil {
					return nil, err
				}
			}
			s, err := fixUnit(tx, s, arg, e.Old, e.New)
			if err != nil {
				ps = append(ps, problem(unit, "%v", err))
				continue
			}
			srcs[id] = s
		}
	}
	for _, off := range slices.Sorted(maps.Keys(texts)) {
		d, _ := ToCP1252(texts[off])
		st := storedText(picks, off, d)
		if slices.Contains(Encoded, groupDE(picks, off)) && bytes.ContainsAny(st, "\x00%") {
			ps = append(ps, problem(fmt.Sprintf("texts %04x", off), "holds a NUL or a %% once XORed, from ÿ, þ, Ú, or Û"))
			continue
		}
		b.Texts = append(b.Texts, TextPatch{Off: off, Lang: l, Text: st})
	}
	for _, t := range slices.Sorted(maps.Keys(grams)) {
		g := grams[t]
		b.Gram = append(b.Gram, GramPatch{Type: t, Lang: l, Gender: g[0], Number: g[1]})
	}
	for _, id := range slices.Sorted(maps.Keys(srcs)) {
		s := srcs[id]
		tx, _ := textOf(id)
		cps, err := b.checkFixed(tx, s)
		if err != nil {
			return nil, err
		}
		ps = append(ps, cps...)
		d, err := Entry(tx.Kind, s)
		if err != nil {
			ps = append(ps, problem(fmt.Sprintf("entry %d", id), "%v", err))
			continue
		}
		if err := b.SetEntry(id, d); err != nil {
			return nil, err
		}
	}
	return ps, nil
}

// checkFixed runs on the source s of tx, a language's text as errata
// fixes it, the checks against the release's German, English, and
// Italian, and for strings the check of their lengths. The release's
// Italian, where it holds a placeholder, reads the English.
func (b *Build) checkFixed(tx Text, s string) ([]Problem, error) {
	var rel [3]string
	for i, l := range errataLangs {
		d, err := b.r.Decoded(tx.ID + l)
		if err != nil {
			return nil, err
		}
		if l == Italian && len(d) <= 3 {
			rel[i] = rel[1]
			continue
		}
		if rel[i], err = Source(tx.Kind, d); err != nil {
			return nil, err
		}
	}
	switch tx.Kind {
	case Strings:
		ds, es, is, fs := splitStrings(rel[0]), splitStrings(rel[1]), splitStrings(rel[2]), splitStrings(s)
		ps := checkStrings(ds, es, is, fs)
		return append(ps, CheckLengths(b.Composed, b.Room, ds, es, is, fs)...), nil
	case Script:
		return checkScript(rel[0], rel[1], rel[2], s), nil
	}
	dd, err := Entry(Doc, rel[0])
	if err != nil {
		return nil, err
	}
	gd, err := text.DecodeDoc(dd)
	if err != nil {
		return nil, err
	}
	fd, err := Entry(Doc, s)
	if err != nil {
		return []Problem{problem(tx.Name, "%v", err)}, nil
	}
	doc, err := text.DecodeDoc(fd)
	if err != nil {
		return []Problem{problem(tx.Name, "%v", err)}, nil
	}
	return CheckDoc(gd, doc), nil
}

// isGram reports whether s is a gender (m, f, n) and a number (s, p).
func isGram(s string) bool {
	return len(s) == 2 && strings.IndexByte("mfn", s[0]) >= 0 && strings.IndexByte("sp", s[1]) >= 0
}

// The addresses of armorgram and weapongram (texts.cpp) in teudogar.exe,
// whose rows of 12 bytes hold a gender for each language, then a number.
const (
	armorgramAddr  = 0x4c1028
	weapongramAddr = 0x4c1200
	gramRow        = 12
)

// gram returns the gender and the number that the release gives the
// name of item type t in language l.
func (r *Release) gram(t, l int) (string, error) {
	var a int
	switch {
	case t == 0x528:
		a = weapongramAddr + 26*gramRow
	case t >= 0x1a0 && t <= 0x1c6:
		a = armorgramAddr + (t-0x1a0)*gramRow
	case t >= 0x1c7 && t <= 0x1e0:
		a = weapongramAddr + (t-0x1c7)*gramRow
	default:
		return "", fmt.Errorf("lang: type %#x outside armorgram and weapongram", t)
	}
	f, err := pe.Parse(r.Exe)
	if err != nil {
		return "", err
	}
	row := f.Bytes(uint32(a)-f.Opt.ImageBase, gramRow)
	if row == nil {
		return "", fmt.Errorf("lang: the grammar of type %#x outside the data of teudogar.exe", t)
	}
	return string([]byte{row[l], row[NLangs+l]}), nil
}

// textNamed returns the text whose file is named name.
func textNamed(name string) (Text, bool) {
	for _, tx := range Texts {
		if tx.Name == name {
			return tx, true
		}
	}
	return Text{}, false
}

// textOf returns the text whose group holds entry id.
func textOf(id int) (Text, bool) {
	for _, tx := range Texts {
		if id >= tx.ID && id < tx.ID+NLangs {
			return tx, true
		}
	}
	return Text{}, false
}

// fixUnit replaces old with new in the unit arg of the source form s of
// tx: a string's index, a topic's index, or a scene's mark.
func fixUnit(tx Text, s, arg, old, nw string) (string, error) {
	n, err := strconv.Atoi(arg)
	if err != nil {
		return "", fmt.Errorf("unit %q, want a number", arg)
	}
	switch tx.Kind {
	case Strings:
		strs := splitStrings(s)
		if n < 0 || n >= len(strs) {
			return "", fmt.Errorf("string %d of %d", n, len(strs))
		}
		if strs[n], err = replaceOnce(strs[n], old, nw); err != nil {
			return "", err
		}
		return strings.Join(strs, "#\n") + "#\n", nil
	case Doc:
		d, err := Entry(Doc, s)
		if err != nil {
			return "", err
		}
		doc, err := text.DecodeDoc(d)
		if err != nil {
			return "", err
		}
		if n < 0 || n >= len(doc.Topics) {
			return "", fmt.Errorf("topic %d of %d", n, len(doc.Topics))
		}
		tp := &doc.Topics[n]
		fixed, err := replaceOnce(Plain(tp.Name+"\r\n"+tp.Text), old, nw)
		if err != nil {
			return "", err
		}
		name, txt, _ := strings.Cut(fixed, "\n")
		if tp.Name, err = Stored(name); err != nil {
			return "", err
		}
		if tp.Text, err = Stored(txt); err != nil {
			return "", err
		}
		e, err := text.EncodeDoc(doc)
		if err != nil {
			return "", err
		}
		return Source(Doc, e)
	}
	d, err := Entry(Script, s)
	if err != nil {
		return "", err
	}
	sc, err := text.DecodeScript(d)
	if err != nil {
		return "", err
	}
	for i := range sc.Scenes {
		if sc.Scenes[i].Mark != n {
			continue
		}
		fixed, err := replaceOnce(Plain(sc.Scenes[i].Text), old, nw)
		if err != nil {
			return "", err
		}
		if sc.Scenes[i].Text, err = Stored(fixed); err != nil {
			return "", err
		}
		e, err := text.EncodeScript(sc)
		if err != nil {
			return "", err
		}
		return Source(Script, e)
	}
	return "", fmt.Errorf("scene %d missing", n)
}

// Plain returns a text of an entry, in code page 1252 with CR LF line
// ends, in UTF-8 with LF line ends.
func Plain(s string) string {
	return FromCP1252([]byte(strings.ReplaceAll(s, "\r\n", "\n")))
}

// Stored is the inverse of Plain.
func Stored(s string) (string, error) {
	b, err := ToCP1252(s)
	if err != nil {
		return "", err
	}
	return strings.ReplaceAll(string(b), "\n", "\r\n"), nil
}

// replaceOnce replaces old with nw in s, where old must occur once.
func replaceOnce(s, old, nw string) (string, error) {
	switch strings.Count(s, old) {
	case 0:
		return "", fmt.Errorf("%q missing", old)
	case 1:
		return strings.Replace(s, old, nw, 1), nil
	}
	return "", fmt.Errorf("%q occurs %d times, want once", old, strings.Count(s, old))
}

package lang

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
)

// Readmes names the readme of each language in the release's folder.
var Readmes = map[int]string{
	German:  "readme_d.txt",
	English: "readme_e.txt",
	Italian: "readme_i.txt",
}

// Dump writes the release's texts in their source forms into dir:
// dir/<code>/<name>.txt for each text that a language has, its readme
// as dir/<code>/readme.txt, and its strings of the program's texts by
// offset as dir/<code>/texts.txt, the units of its errata. The strings
// that the code XORs through xorstr are written XORed back.
func Dump(r *Release, dir string, picks []Pick) error {
	for _, tx := range Texts {
		for l := range NLangs {
			b, err := r.Decoded(tx.ID + l)
			if err != nil {
				return err
			}
			if len(b) <= 3 {
				continue
			}
			s, err := Source(tx.Kind, b)
			if err != nil {
				return fmt.Errorf("entry %d: %v", tx.ID+l, err)
			}
			if err := write(filepath.Join(dir, Codes[l], tx.Name+".txt"), s); err != nil {
				return err
			}
		}
	}
	for l, name := range Readmes {
		b, err := os.ReadFile(filepath.Join(r.Dir, name))
		if err != nil {
			return err
		}
		s, err := Source(Doc, b)
		if err != nil {
			return fmt.Errorf("%s: %v", name, err)
		}
		if err := write(filepath.Join(dir, Codes[l], "readme.txt"), s); err != nil {
			return err
		}
		if err := write(filepath.Join(dir, Codes[l], "texts.txt"), r.langDump(picks, l)); err != nil {
			return err
		}
	}
	return nil
}

// langDump returns the strings of the texts that the calls of pick give
// language l, each by its offset after a comment line that names its
// first call, in the order of the calls.
func (r *Release) langDump(picks []Pick, l int) string {
	var b strings.Builder
	fmt.Fprintf(&b, "# The strings of the program's texts in %s: the offset, then the string.\n", Codes[l])
	seen := make(map[int]bool)
	for _, p := range picks {
		off := p.Off[l]
		if seen[off] {
			continue
		}
		seen[off] = true
		fmt.Fprintf(&b, "# %s:%d\n%04x\t%s\n", p.File, p.Line, off, Escape(FromCP1252(r.plain(off, p.Off[German]))))
	}
	return b.String()
}

// plain returns the string of the texts at off, XORed back through
// XorStr when the group whose German string lies at de is one of
// Encoded.
func (r *Release) plain(off, de int) []byte {
	s := CString(r.Block.Texts[:], off)
	for _, e := range Encoded {
		if e == de {
			return XorStr(s)
		}
	}
	return s
}

// write writes s to the file name, making its directory.
func write(name, s string) error {
	if err := os.MkdirAll(filepath.Dir(name), 0o755); err != nil {
		return err
	}
	return os.WriteFile(name, []byte(s), 0o644)
}

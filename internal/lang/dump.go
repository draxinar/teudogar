package lang

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
)

// Bypass lists the strings of the texts that the code shows without
// pick, and that French shows from an entry of texts.txt at the English
// string's offset, through feattext.
var Bypass = []struct {
	Off  int
	Note string
}{
	{0x9dea, "msgf (combat.cpp), for a key (actions.cpp) and a picked lock (input.cpp): an item and the state of its lock"},
	{0x9e01, "fmttime (system.cpp): a date and a time"},
	{0x9e1a, "fmttime (system.cpp): a date"},
	{0x9e41, "months[1] (texts.cpp)"},
	{0x9e45, "months[2]"},
	{0x9e49, "months[3]"},
	{0x9e4d, "months[4]"},
	{0x9e51, "months[5]"},
	{0x9e55, "months[6]"},
	{0x9e59, "months[7]"},
	{0x9e5d, "months[8]"},
	{0x9e61, "months[9]"},
	{0x9e65, "months[10]"},
	{0x9e69, "months[11]"},
	{0x9e6d, "months[12]"},
	{0xb057, "options (mainmenu.cpp): the options menu's title"},
	{0xb079, "msgf, msgcat, prependmsg, nosuccess, notworking, denymsg (combat.cpp), text3greyn (graphics.cpp): two texts joined by a colon"},
	{0xb080, "prependmsg (combat.cpp): two texts joined by a colon and a line break"},
	{0xb062, "options (mainmenu.cpp): the title of a menu of the options"},
	{0xb087, "slotmenu and docmenu (mainmenu.cpp): a menu's title"},
	{0xb14c, "slotmenu (mainmenu.cpp): a saved game's slot"},
	{0xb318, "msgf (combat.cpp): an item and its state"},
}

// Readmes names the readme of each language in the release's folder.
var Readmes = map[int]string{
	German:  "readme_d.txt",
	English: "readme_e.txt",
	Italian: "readme_i.txt",
}

// Dump writes the release's texts in their source forms into dir:
// dir/<code>/<name>.txt for each text that a language has, its readme
// as dir/<code>/readme.txt, and its strings of the program's texts by
// offset as dir/<code>/texts.txt, the units of its errata. dir/texts.txt
// lists each group of picks by its French slot with its German,
// English, and Italian strings, and then the strings of Bypass. The
// strings that the code XORs through xorstr are written XORed back.
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
	return write(filepath.Join(dir, "texts.txt"), r.textsDump(picks))
}

// langDump returns the strings of the texts that the calls of pick give
// language l, each by its offset after a comment line that names its
// first call, in the order of the calls. ReadTexts reads it.
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

// textsDump returns the contents of the dump's texts.txt.
func (r *Release) textsDump(picks []Pick) string {
	var b strings.Builder
	b.WriteString("# The groups of pick: the French slot, then the German, English, and Italian strings.\n")
	seen := make(map[int]bool)
	for _, p := range picks {
		fr := p.Off[French]
		if seen[fr] {
			continue
		}
		seen[fr] = true
		fmt.Fprintf(&b, "%04x", fr)
		for _, l := range []int{German, English, Italian} {
			fmt.Fprintf(&b, "\t%s", Escape(FromCP1252(r.plain(p.Off[l], p.Off[German]))))
		}
		fmt.Fprintf(&b, "\t# %s:%d\n", p.File, p.Line)
	}
	b.WriteString("# The strings that the code shows without pick: the English offset, then the string.\n")
	for _, s := range Bypass {
		fmt.Fprintf(&b, "%04x\t%s\t# %s\n", s.Off, Escape(FromCP1252(CString(r.Block.Texts[:], s.Off))), s.Note)
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

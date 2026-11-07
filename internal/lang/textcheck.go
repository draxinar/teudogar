package lang

import (
	"fmt"
	"slices"
	"strings"
)

// CheckTexts checks the lines of the French texts.txt: each lies at the
// French slot of a group of pick, or at a string of Bypass, and keeps
// the marks and the accelerators of its German, or of its English for a
// string of Bypass. The strings of the groups of Encoded must stay clear
// of NUL and % once XORed, since the code hands one of them to wsprintf
// as a format, and the farewell at Bye must be one word, which byenode
// (actions.cpp) compares with the first word of a choice. With complete
// set, every slot and every string of Bypass needs its line.
func CheckTexts(r *Release, picks []Pick, lines []TextLine, complete bool) []Problem {
	var ps []Problem
	slot := make(map[int]Pick)
	for _, p := range picks {
		if _, ok := slot[p.Off[French]]; !ok {
			slot[p.Off[French]] = p
		}
	}
	bypass := make(map[int]bool)
	for _, b := range Bypass {
		bypass[b.Off] = true
	}
	have := make(map[int]bool)
	for _, l := range lines {
		unit := fmt.Sprintf("texts %04x", l.Off)
		have[l.Off] = true
		fr, err := ToCP1252(l.Text)
		if err != nil {
			ps = append(ps, problem(unit, "%v", err))
			continue
		}
		var want []byte
		if p, ok := slot[l.Off]; ok {
			de := p.Off[German]
			want = r.plain(de, de)
			en, it := r.plain(p.Off[English], de), r.plain(p.Off[Italian], de)
			if de := FromCP1252(want); shared(string(want), string(en), string(it)) && l.Text != de && l.Text != Spaces(de) {
				ps = append(ps, problem(unit, "reads %q, want %q as German, English, and Italian have it", l.Text, de))
			}
			if slices.Contains(Encoded, de) && strings.ContainsAny(string(XorStr(fr)), "\x00%") {
				ps = append(ps, problem(unit, "holds a NUL or a %% once XORed, from ÿ, þ, Ú, or Û"))
			}
		} else if bypass[l.Off] {
			want = CString(r.Block.Texts[:], l.Off)
		} else {
			ps = append(ps, problem(unit, "lies at neither a French slot of pick nor a string of Bypass"))
			continue
		}
		if m, w := marks(string(fr), stringMarks), marks(string(want), stringMarks); !slices.Equal(m, w) {
			ps = append(ps, problem(unit, "marks %q, want %q", m, w))
		}
		if a, w := accels(string(fr)), accels(string(want)); a != w {
			ps = append(ps, problem(unit, "%d accelerators (&), want %d", a, w))
		}
		if l.Off == Bye && (len(fr) == 0 || len(firstWord(fr)) != len(fr)) {
			ps = append(ps, problem(unit, "farewell %q, want one word", l.Text))
		}
	}
	if complete {
		for off, p := range slot {
			if !have[off] {
				ps = append(ps, problem(fmt.Sprintf("texts %04x", off), "missing: the French slot of %s:%d", p.File, p.Line))
			}
		}
		for _, b := range Bypass {
			if !have[b.Off] {
				ps = append(ps, problem(fmt.Sprintf("texts %04x", b.Off), "missing: %s", b.Note))
			}
		}
	}
	slices.SortFunc(ps, func(a, b Problem) int { return strings.Compare(a.Unit, b.Unit) })
	return ps
}

// accels returns the count of the accelerators of a dialog's label: the
// & that mark the key, as against the && of text3's marks.
func accels(s string) int {
	n := 0
	for i := 0; i < len(s); i++ {
		if s[i] != '&' {
			continue
		}
		if i+1 < len(s) && s[i+1] == '&' {
			i++
			continue
		}
		n++
	}
	return n
}

// Bye is the offset of the French slot of the farewell that byenode
// (actions.cpp) compares with the first word of a choice's label.
const Bye = 0x6bc9

// CheckFarewell checks the farewell bye, the French text at Bye, against
// the French script fr: bye is one word, every choice whose German label
// is the farewell reads bye, and no other choice of its scene begins
// with bye, since wordeqi (str.cpp) compares first words alone.
func CheckFarewell(de, fr, bye string) []Problem {
	var ps []Problem
	b, err := ToCP1252(bye)
	if err != nil || len(b) == 0 || len(firstWord(b)) != len(b) {
		return append(ps, problem(fmt.Sprintf("texts %04x", Bye), "farewell %q, want one word", bye))
	}
	dl, fl := strings.Split(de, "\n"), strings.Split(fr, "\n")
	if len(dl) != len(fl) {
		return ps
	}
	scene := "head"
	for i := range dl {
		if kindOf(dl[i]) == markLine {
			scene = "scene " + strings.Fields(dl[i])[1]
		}
		if kindOf(dl[i]) != choiceLine {
			continue
		}
		_, dlab, _, _ := splitChoice(dl[i])
		_, flab, _, _ := splitChoice(fl[i])
		dlab, flab = strings.TrimPrefix(dlab, "!"), strings.TrimPrefix(flab, "!")
		lab, err := ToCP1252(flab)
		if err != nil {
			continue
		}
		switch {
		case dlab == "Wiedersehen" && flab != bye:
			ps = append(ps, problem(fmt.Sprintf("line %d", i+1), "farewell %q, want %q (%s)", flab, bye, scene))
		case dlab != "Wiedersehen" && strings.EqualFold(FromCP1252(firstWord(lab)), bye):
			ps = append(ps, problem(fmt.Sprintf("line %d", i+1), "choice %q begins with the farewell %q (%s)", flab, bye, scene))
		}
	}
	return ps
}

// firstWord returns the bytes of b, in code page 1252, up to the first
// byte outside a word, as iswordc (str.cpp) reads it.
func firstWord(b []byte) []byte {
	n := 0
	for n < len(b) && isWordByte(b[n]) {
		n++
	}
	return b[:n]
}

// isWordByte reports whether c belongs to a word, as iswordc (str.cpp)
// decides: a letter, a digit, an underscore, a letter of code page 1252
// from 0xc0 on, or one of š, œ, ÿ, Š, Œ, Ÿ, ‘, and ’.
func isWordByte(c byte) bool {
	switch {
	case 'a' <= c && c <= 'z', 'A' <= c && c <= 'Z', '0' <= c && c <= '9', c == '_':
		return true
	case c >= 0xc0:
		return true
	}
	switch c {
	case 0x8a, 0x8c, 0x91, 0x92, 0x9a, 0x9c, 0x9f:
		return true
	}
	return false
}

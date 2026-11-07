package lang

import (
	"fmt"
	"regexp"
	"slices"
	"strings"

	"github.com/draxinar/teudogar/internal/text"
)

// A Problem is a fault that a check finds in the sources of a feature:
// the unit that holds it, such as "string 36" or "line 541", and what
// is wrong.
type Problem struct {
	Unit string
	Msg  string
}

func (p Problem) String() string { return p.Unit + ": " + p.Msg }

func problem(unit, format string, args ...any) Problem {
	return Problem{Unit: unit, Msg: fmt.Sprintf(format, args...)}
}

// The marks that the text of a feature keeps in place, as text3
// (graphics.cpp), wrap (lang.cpp), drawpage (menus.cpp), and wsprintf
// read them.
var (
	kindMark    = regexp.MustCompile(`&&[A-Z]{3}(?:\d{4}|%04d)`)
	columnMark  = regexp.MustCompile(`\}\}(?:\d{4}..|[A-Z].)`)
	buttonRow   = regexp.MustCompile(`\[\[?\d+[<>x]?(?:\.(?:R%d|[A-Z]))?,`)
	printfVerb  = regexp.MustCompile(`%[-+ #0]*\d*(?:\.\d+)?l?[sdcuxXo%]`)
	dollarTerm  = regexp.MustCompile(`\$(?:obj.[eE]?|[A-Za-z_]+\d*)`)
	percentTerm = regexp.MustCompile(`%[A-Za-z_][A-Za-z0-9_/*]*|%\d+`)
)

// The sets of marks that marks finds. A string keeps every mark of its
// German in order, wsprintf's verbs included, since wsprintf reads it as
// a format. A document keeps its pictures and its rows of buttons, and
// may lay out its columns (}}F, }}G) as its lines need, as the English
// and Italian manuals do.
var (
	stringMarks = []*regexp.Regexp{kindMark, columnMark, buttonRow, printfVerb}
	docMarks    = []*regexp.Regexp{kindMark, buttonRow}
)

// marks returns the marks of s, of the set res, that the text of a
// feature must keep in their order, with the leading backslashes and
// the control bytes.
func marks(s string, res []*regexp.Regexp) []string {
	var m []string
	for i := 0; i < len(s) && s[i] == '\\'; i++ {
		m = append(m, `\`)
	}
	type at struct {
		off, end int
		s        string
	}
	var all []at
	for _, re := range res {
		for _, loc := range re.FindAllStringIndex(s, -1) {
			all = append(all, at{loc[0], loc[1], s[loc[0]:loc[1]]})
		}
	}
	for i := 0; i < len(s); i++ {
		if c := s[i]; c < 0x20 && c != '\r' && c != '\n' && c != '\t' {
			all = append(all, at{i, i + 1, fmt.Sprintf("byte %#02x", c)})
		}
	}
	slices.SortStableFunc(all, func(a, b at) int { return a.off - b.off })
	end := 0
	for _, a := range all {
		// A verb inside a mark, as in &&RUN%04d, is part of the mark.
		if a.off < end {
			continue
		}
		m = append(m, a.s)
		end = a.end
	}
	return m
}

// terms returns the $ terms of s that expand (dialog.cpp) replaces, and
// its % terms when percent is set, sorted, with the suffixes that a
// language may choose dropped: the form of $obj, the case of $Stamm,
// and the genitive s of $N. The strings take no % terms: a % there
// opens a verb of wsprintf.
func terms(s string, percent bool) []string {
	found := dollarTerm.FindAllString(s, -1)
	if percent {
		found = append(found, percentTerm.FindAllString(s, -1)...)
	}
	var t []string
	for _, x := range found {
		switch {
		case strings.HasPrefix(x, "$obj"):
			x = x[:min(len(x), 5)]
		case strings.HasPrefix(x, "$Stamm"), strings.HasPrefix(x, "$N"):
			x = strings.TrimRight(x, "gdas")
		case strings.HasPrefix(x, "$ort"):
			x = "$Ort" + x[4:]
		}
		t = append(t, x)
	}
	slices.Sort(t)
	return t
}

// shared reports whether the German, English, and Italian versions of a
// unit read the same, as names, Latin, and placeholders do.
func shared(de, en, it string) bool {
	return de == en && de == it
}

// checkStrings checks the strings tr of a language that errata fixes
// against the German de, with en and it for the units that the three
// share.
func checkStrings(de, en, it, tr []string) []Problem {
	var ps []Problem
	if len(tr) != len(de) {
		return append(ps, problem("strings", "%d strings, want %d", len(tr), len(de)))
	}
	if len(en) != len(de) || len(it) != len(de) {
		return append(ps, problem("strings", "the German, English, and Italian hold %d, %d, and %d strings", len(de), len(en), len(it)))
	}
	for i := range de {
		unit := fmt.Sprintf("string %d", i)
		keep := shared(de[i], en[i], it[i])
		ps = append(ps, checkUnit(unit, de[i], tr[i], keep)...)
		ps = append(ps, checkString(unit, i, de[i], tr[i])...)
	}
	return ps
}

// checkUnit checks the marks and the terms of one unit, and with keep
// set, that the text tr reads as the German, which the English and the
// Italian share.
func checkUnit(unit, de, tr string, keep bool) []Problem {
	var ps []Problem
	if keep && tr != de {
		ps = append(ps, problem(unit, "reads %q, want %q as German, English, and Italian have it", tr, de))
		return ps
	}
	if m, w := marks(tr, stringMarks), marks(de, stringMarks); !slices.Equal(m, w) {
		ps = append(ps, problem(unit, "marks %q, want %q", m, w))
	}
	if t, w := terms(tr, false), terms(de, false); !slices.Equal(t, w) {
		ps = append(ps, problem(unit, "terms %q, want %q", t, w))
	}
	return ps
}

// checkString checks what the strings of a few ranges demand of their
// text: the importance marks of the quest notes, the prefix of the
// signs, and the runes, whose name's first letter picks the picture.
func checkString(unit string, i int, de, tr string) []Problem {
	var ps []Problem
	switch {
	case i >= 1210 && i <= 1243 && i%2 == 0:
		if tr == "" || de == "" || tr[0] != de[0] {
			ps = append(ps, problem(unit, "rune name %q, want the first letter of %q", tr, de))
		}
	case i >= 1211 && i <= 1243:
		if strings.Contains(tr, ",") != strings.Contains(de, ",") {
			ps = append(ps, problem(unit, "rune meaning %q, want a comma where %q has one", tr, de))
		}
	case i >= 1691 && i <= 1740:
		if strings.HasPrefix(strings.TrimLeft(tr, `\`), "!!U ") != strings.HasPrefix(strings.TrimLeft(de, `\`), "!!U ") {
			ps = append(ps, problem(unit, "sign %q, want the prefix of %q", tr, de))
		}
	case i >= 1741 && i <= 1823:
		if bangs(tr) != bangs(de) {
			ps = append(ps, problem(unit, "quest note %q, want the marks of %q", tr, de))
		}
	}
	return ps
}

// bangs returns the importance mark of a quest note: the run of ! after
// its backslashes.
func bangs(s string) string {
	s = strings.TrimLeft(s, `\`)
	n := 0
	for n < len(s) && s[n] == '!' {
		n++
	}
	return s[:n]
}

// CheckDoc checks the document tr against the German de: the same
// topics, the disabled items of the menu where the German has them,
// and the marks of each topic. A topic may name a person or a place
// where the German writes a pronoun, as the English and Italian
// walkthroughs do, so its $ and % terms need only be among the German
// document's.
func CheckDoc(de, tr *text.Doc) []Problem {
	var ps []Problem
	if m, w := marks(tr.Head, docMarks), marks(de.Head, docMarks); !slices.Equal(m, w) {
		ps = append(ps, problem("head", "marks %q, want %q", m, w))
	}
	if len(tr.Topics) != len(de.Topics) {
		return append(ps, problem("topics", "%d topics, want %d", len(tr.Topics), len(de.Topics)))
	}
	known := make(map[string]bool)
	for _, tp := range de.Topics {
		for _, x := range terms(tp.Name+"\n"+tp.Text, true) {
			known[x] = true
		}
	}
	for i := range de.Topics {
		d, f := de.Topics[i], tr.Topics[i]
		unit := fmt.Sprintf("topic %d (%s)", i, FromCP1252([]byte(d.Name)))
		if strings.HasPrefix(f.Name, "-") != strings.HasPrefix(d.Name, "-") {
			ps = append(ps, problem(unit, "name %q, want a - where the German disables the item", FromCP1252([]byte(f.Name))))
		}
		if m, w := marks(f.Text, docMarks), marks(d.Text, docMarks); !slices.Equal(m, w) {
			ps = append(ps, problem(unit, "marks %q, want %q", m, w))
		}
		for _, x := range terms(f.Name+"\n"+f.Text, true) {
			if !known[x] {
				ps = append(ps, problem(unit, "term %s, which the German document lacks", x))
			}
		}
	}
	return ps
}

// A lineKind is the role of a line of the dialog script, as parsescene
// and runnode (dialog.cpp) read it from its first byte.
type lineKind int

const (
	blankLine  lineKind = iota
	markLine            // "// " and a digit: a scene's mark
	codeLine            // a command, a node, a brace, or a command commented out
	choiceLine          // "#cond#label{": a choice
	textLine            // the text of a node
)

func kindOf(l string) lineKind {
	t := strings.TrimLeft(l, " ")
	switch {
	case l == "":
		return blankLine
	case strings.HasPrefix(t, "// ") && len(t) > 3 && isdigit(t[3]):
		return markLine
	case l[0] == '#':
		return choiceLine
	case strings.ContainsRune(`"{}\`, rune(l[0])), strings.HasPrefix(l, "//"):
		return codeLine
	}
	return textLine
}

func isdigit(c byte) bool { return '0' <= c && c <= '9' }

// splitChoice splits a choice line "#cond#label{tail" into its parts.
func splitChoice(l string) (cond, label, tail string, ok bool) {
	rest, ok := strings.CutPrefix(l, "#")
	if !ok {
		return "", "", "", false
	}
	cond, rest, ok = strings.Cut(rest, "#")
	if !ok {
		return "", "", "", false
	}
	label, tail, ok = strings.Cut(rest, "{")
	return cond, label, "{" + tail, ok
}

// checkScript checks the script tr of a language that errata fixes
// against the German de, line for line, with en and it for the lines
// that the three share. The lines are those of the source form, LF
// ended.
func checkScript(de, en, it, tr string) []Problem {
	var ps []Problem
	dl, el, il, fl := strings.Split(de, "\n"), strings.Split(en, "\n"), strings.Split(it, "\n"), strings.Split(tr, "\n")
	if len(fl) != len(dl) || len(el) != len(dl) || len(il) != len(dl) {
		return append(ps, problem("script", "%d lines, want %d", len(fl), len(dl)))
	}
	for i := range dl {
		unit := fmt.Sprintf("line %d", i+1)
		d, f := dl[i], fl[i]
		k := kindOf(d)
		if kindOf(f) != k {
			ps = append(ps, problem(unit, "%q, want a line of the German's kind: %q", f, d))
			continue
		}
		switch k {
		case blankLine, markLine, codeLine:
			if f != d {
				ps = append(ps, problem(unit, "%q, want %q", f, d))
			}
		case choiceLine:
			dc, dlab, dt, _ := splitChoice(d)
			fc, flab, ft, ok := splitChoice(f)
			if !ok || fc != dc || ft != dt {
				ps = append(ps, problem(unit, "choice %q, want the condition and the tail of %q", f, d))
				continue
			}
			_, elab, _, _ := splitChoice(el[i])
			_, ilab, _, _ := splitChoice(il[i])
			ps = append(ps, checkText(unit, dlab, elab, ilab, flab)...)
		case textLine:
			ps = append(ps, checkText(unit, d, el[i], il[i], f)...)
		}
	}
	ps = append(ps, checkScenes(dl, fl)...)
	return ps
}

// checkText checks a text line or a choice's label of the script.
func checkText(unit, de, en, it, tr string) []Problem {
	if strings.ContainsAny(tr, `"#{}\`) {
		return []Problem{problem(unit, "%q holds a byte of the script's structure (\" # { } \\)", tr)}
	}
	if shared(de, en, it) && tr != de {
		return []Problem{problem(unit, "reads %q, want %q as German, English, and Italian have it", tr, de)}
	}
	return nil
}

// checkScenes checks that each scene holds the German's $ and % terms,
// in any order: a text may move them, and the per-line check cannot see
// a term that moves to another line.
func checkScenes(de, tr []string) []Problem {
	var ps []Problem
	var dt, ft []string
	scene := "head"
	flush := func() {
		slices.Sort(dt)
		slices.Sort(ft)
		if !slices.Equal(dt, ft) {
			ps = append(ps, problem(scene, "terms %q, want %q", ft, dt))
		}
		dt, ft = nil, nil
	}
	for i := range de {
		if kindOf(de[i]) == markLine {
			flush()
			scene = "scene " + strings.Fields(de[i])[1]
		}
		dt = append(dt, terms(de[i], true)...)
		ft = append(ft, terms(tr[i], true)...)
	}
	flush()
	return ps
}

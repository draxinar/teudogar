package lang

import (
	"bytes"
	"fmt"

	"github.com/draxinar/teudogar/internal/text"
)

// breaks returns, for each line break of the text s of a page, in code
// page 1252 with CR LF line ends, whether wrap (lang.cpp) keeps it. wrap
// drops the LFs and the spaces before a CR, keeps a CR before a CR or
// a [, after a line of a lone . or +, after [-, and after a CR ! " ' ) *
// + : ? ] or {, and after a period that ends no abbreviation or
// ellipsis. It turns every other CR into a space.
func breaks(s []byte) []bool {
	t := bytes.ReplaceAll(s, []byte("\n"), nil)
	for bytes.Contains(t, []byte(" \r")) {
		t = bytes.ReplaceAll(t, []byte(" \r"), []byte("\r"))
	}
	at := func(i int) byte {
		if i < 0 || i >= len(t) {
			return 0
		}
		return t[i]
	}
	var kept []bool
	for i := 1; i < len(t); i++ {
		if t[i] != '\r' {
			continue
		}
		var k bool
		switch {
		case at(i+1) == '\r', at(i+1) == '[':
			k = true
		case at(i+2) == '\r' && (at(i+1) == '.' || at(i+1) == '+'):
			k = true
		case at(i-2) == '[' && at(i-1) == '-':
			k = true
		default:
			switch at(i - 1) {
			case '\r', '!', '"', '\'', ')', '*', '+', ':', '?', ']', '{':
				k = true
			case '.':
				k = at(i-3) != '.' && at(i-4) != '.'
			}
		}
		kept = append(kept, k)
	}
	return kept
}

// checkBreaks reports each line of a topic of the French document fr
// whose break wrap keeps where it joins the German de's, or joins where
// it keeps it. A topic whose line count differs from the German's, as
// where the French splits a line to fit the page, must keep at least as
// many breaks.
func checkBreaks(de, fr *text.Doc) []Problem {
	var ps []Problem
	for i := range min(len(de.Topics), len(fr.Topics)) {
		unit := fmt.Sprintf("topic %d (%s)", i, FromCP1252([]byte(de.Topics[i].Name)))
		db, fb := breaks([]byte(de.Topics[i].Text)), breaks([]byte(fr.Topics[i].Text))
		if len(db) != len(fb) {
			if dn, fn := count(db), count(fb); fn < dn {
				ps = append(ps, problem(unit, "%d breaks that wrap keeps, want %d as the German keeps", fn, dn))
			}
			continue
		}
		for j := range db {
			switch {
			case db[j] && !fb[j]:
				ps = append(ps, problem(unit, "line %d: wrap joins the next line, where the German's keeps its break", j+1))
			case !db[j] && fb[j]:
				ps = append(ps, problem(unit, "line %d: wrap keeps the break, where it joins the German's next line", j+1))
			}
		}
	}
	return ps
}

// count returns the number of true values of v.
func count(v []bool) int {
	n := 0
	for _, b := range v {
		if b {
			n++
		}
	}
	return n
}

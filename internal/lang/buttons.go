package lang

import (
	"fmt"
	"os"
	"path/filepath"
	"regexp"
	"strconv"
	"strings"
)

// ReadFontWidths returns fontw, the widths of the glyphs of font, from
// its initializer in unpack.cpp of the game's source dir.
func ReadFontWidths(dir string) ([256]int, error) {
	var w [256]int
	b, err := os.ReadFile(filepath.Join(dir, "unpack.cpp"))
	if err != nil {
		return w, err
	}
	_, rest, ok := strings.Cut(string(b), "int fontw[256] = {")
	body, _, ok2 := strings.Cut(rest, "};")
	if !ok || !ok2 {
		return w, fmt.Errorf("lang: unpack.cpp holds no initializer of fontw")
	}
	n := 0
	for _, f := range strings.FieldsFunc(body, func(r rune) bool { return r == ',' || r == ' ' || r == '\t' || r == '\n' }) {
		v, err := strconv.Atoi(f)
		if err != nil || n == len(w) {
			return w, fmt.Errorf("lang: fontw: entry %d %q, want 256 numbers", n, f)
		}
		w[n] = v
		n++
	}
	if n != len(w) {
		return w, fmt.Errorf("lang: fontw: %d entries, want 256", n)
	}
	return w, nil
}

// buttonLabel matches a button of a row: [, the number that gives its
// width, a key, a comma, and its label, which ends at [ or ], or at the
// end of its line as the page layout (menus.cpp) ends it.
var buttonLabel = regexp.MustCompile(`\[\[?(\d+)[<>x]?(?:\.(?:R%d|[A-Z]))?,([^][\r\n]*)`)

// buttonMargin is the room in pixels that a button's label leaves on its
// two sides.
const buttonMargin = 8

// buttonWidth returns the width in pixels of a button whose mark gives n,
// as the page layout reads it (menus.cpp): 130 for 72, and twice n up to
// 428 for the others.
func buttonWidth(n int) int {
	if n == 72 {
		return 130
	}
	return min(2*n, 428)
}

// textWidth returns the width of s, in code page 1252, in pixels of font.
func textWidth(fontw [256]int, s string) int {
	b, err := ToCP1252(s)
	if err != nil {
		return 0
	}
	n := 0
	for _, c := range b {
		n += fontw[c]
	}
	return n
}

// checkButtons reports each button of the French fr whose label draws
// wider than its button holds, less the margins.
func checkButtons(fontw [256]int, unit, fr string) []Problem {
	var ps []Problem
	for _, m := range buttonLabel.FindAllStringSubmatch(fr, -1) {
		n, err := strconv.Atoi(m[1])
		if err != nil {
			continue
		}
		room := buttonWidth(n) - buttonMargin
		if w := textWidth(fontw, m[2]); w > room {
			ps = append(ps, problem(unit, "button %q of %d pixels, wider than the %d its button holds", m[2], w, room))
		}
	}
	return ps
}

// choiceRoom is the width in pixels that a choice of a dialog draws in:
// shownode (dialog.cpp) starts each row of choices at x 152, and text3
// (graphics.cpp) stops drawing at x 640.
const choiceRoom = 640 - 152

// checkChoices reports each choice of the script fr whose label draws
// wider than choiceRoom, with its terms measured as written.
func checkChoices(fontw [256]int, fr string) []Problem {
	var ps []Problem
	for i, l := range strings.Split(fr, "\n") {
		if kindOf(l) != choiceLine {
			continue
		}
		_, label, _, ok := splitChoice(l)
		if !ok {
			continue
		}
		// settext (dialog.cpp) skips the ! of a label that has one.
		label = strings.TrimPrefix(label, "!")
		if w := textWidth(fontw, label); w > choiceRoom {
			ps = append(ps, problem(fmt.Sprintf("line %d", i+1), "choice %q of %d pixels, wider than the %d that a choice draws in", label, w, choiceRoom))
		}
	}
	return ps
}

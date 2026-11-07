package lang

import (
	"regexp"
	"strings"
)

// nbsp makes the spaces of French typography no-break: before a colon, a
// semicolon, an exclamation mark, and a question mark, after « and
// before », and those of the years av. J.-C. and apr. J.-C.
var nbsp = strings.NewReplacer(
	" :", "\u00a0:",
	" ;", "\u00a0;",
	" !", "\u00a0!",
	" ?", "\u00a0?",
	"« ", "«\u00a0",
	" »", "\u00a0»",
	" av. J.-C.", "\u00a0av.\u00a0J.-C.",
	" apr. J.-C.", "\u00a0apr.\u00a0J.-C.",
)

// groups matches a space between the groups of three digits of a
// number, as French writes 120 000, units the space between a number
// and its unit, as in 1,70 m, and percent the space before a percent
// sign, written % in a page and %% in a format, and kept apart from a
// printf verb such as %s.
var (
	groups  = regexp.MustCompile(`([0-9]) ([0-9]{3})\b`)
	units   = regexp.MustCompile(`([0-9]) (m|cm|mm|km|kg|g|l|ha|h|Mo|Ko)\b`)
	percent = regexp.MustCompile(`([0-9]) (%%|%)([^%A-Za-z0-9_]|$)`)
)

// Spaces returns the French text s with the spaces that French
// typography keeps on the line of the next or the last word made
// no-break (U+00A0): those of nbsp, those between the groups of digits
// of a number, those between a number and its unit, and those before a
// percent sign. The sources of lang/fr write them as plain spaces.
func Spaces(s string) string {
	s = units.ReplaceAllString(nbsp.Replace(s), "$1\u00a0$2")
	s = percent.ReplaceAllString(s, "$1\u00a0$2$3")
	for {
		t := groups.ReplaceAllString(s, "$1\u00a0$2")
		if t == s {
			return s
		}
		s = t
	}
}

// scriptSpaces returns the French script s with Spaces applied to its
// text lines and to the labels of its choices, the parts that the
// checks compare as text: its other lines hold the structure of the
// script, byte for byte the German's.
func scriptSpaces(s string) string {
	lines := strings.Split(s, "\n")
	for i, l := range lines {
		switch kindOf(l) {
		case textLine:
			lines[i] = Spaces(l)
		case choiceLine:
			if cond, label, tail, ok := splitChoice(l); ok {
				lines[i] = "#" + cond + "#" + Spaces(label) + tail
			}
		}
	}
	return strings.Join(lines, "\n")
}

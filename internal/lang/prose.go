package lang

import (
	"regexp"
	"strings"
)

// The marks and terms that Prose drops: the game's marks, the $ terms
// with their suffixes ($Stamm2g, $objpE), the printf verbs before the %
// terms, so that %03ld goes whole, the URLs, the accelerators of the
// dialogs, the ellipses, and the list markers. The verbs here take no
// space flag, which would read the "% s" of "7 % seulement" as one.
var proseMarks = []*regexp.Regexp{
	kindMark,
	columnMark,
	buttonRow,
	regexp.MustCompile(`\$[A-Za-z_]+\d*[A-Za-z]*`),
	regexp.MustCompile(`%[-+#0]*\d*(?:\.\d+)?l?[sdcuxXo%]`),
	percentTerm,
	regexp.MustCompile(`(?i)(?:https?://|www\.)\S+`),
	regexp.MustCompile(`&|\.\.\.`),
	regexp.MustCompile(`^\\*!!U|^-- `),
}

// Prose returns the words of the French source fr of kind k that a
// spelling checker reads, line by line: the text lines and the labels of
// the choices of a script, the strings, and a document or a plain text
// whole, without the game's marks, its $ and % terms, and its printf
// verbs. A line of a script or a string that reads as the German de has
// it (a name, a Latin line, a test scene) stays out.
func Prose(k Kind, de, fr string) string {
	var b strings.Builder
	switch k {
	case Script:
		dl := strings.Split(de, "\n")
		for i, l := range strings.Split(fr, "\n") {
			if i < len(dl) && dl[i] == l {
				continue
			}
			switch kindOf(l) {
			case textLine:
				proseLine(&b, l)
			case choiceLine:
				if _, label, _, ok := splitChoice(l); ok {
					proseLine(&b, label)
				}
			}
		}
	case Strings:
		ds := splitStrings(de)
		for i, s := range splitStrings(fr) {
			if i < len(ds) && ds[i] == s {
				continue
			}
			for _, l := range strings.Split(s, "\n") {
				proseLine(&b, l)
			}
		}
	default:
		for _, l := range strings.Split(fr, "\n") {
			proseLine(&b, l)
		}
	}
	return b.String()
}

// proseLine writes the line l to b without its marks and terms, with
// one space between its words.
func proseLine(b *strings.Builder, l string) {
	for _, re := range proseMarks {
		l = re.ReplaceAllString(l, " ")
	}
	if l = strings.Join(strings.Fields(l), " "); l != "" {
		b.WriteString(l)
		b.WriteByte('\n')
	}
}

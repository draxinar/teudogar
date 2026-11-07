package lang

import (
	"bytes"
	"fmt"
	"strings"
)

// checkElision reports the French text s of unit when frelide changes
// it. The game runs frelide over the French texts that it composes, and
// correct French comes back as it is, since it already elides where
// French elides. A build without cgo skips the check.
func checkElision(unit, s string) []Problem {
	b, err := ToCP1252(s)
	if err != nil {
		return nil // the checks of the text report it
	}
	e, err := Elide(b)
	if err != nil || bytes.Equal(e, b) {
		return nil
	}
	return []Problem{problem(unit, "elides to %q, want the elision written", FromCP1252(e))}
}

// CheckElision runs checkElision over the texts of the French source s
// of tx that frelide reads in the game: the strings, the text lines and
// labels of the script, and the walkthrough, which expandstr (dialog.cpp)
// expands. The manual and the encyclopedia draw their texts as they are.
func CheckElision(tx Text, s string) []Problem {
	var ps []Problem
	switch {
	case tx.Kind == Strings:
		for i, str := range splitStrings(s) {
			ps = append(ps, checkElision(fmt.Sprintf("string %d", i), str)...)
		}
	case tx.Kind == Script:
		for i, l := range strings.Split(s, "\n") {
			unit := fmt.Sprintf("line %d", i+1)
			switch kindOf(l) {
			case textLine:
				ps = append(ps, checkElision(unit, l)...)
			case choiceLine:
				if _, label, _, ok := splitChoice(l); ok {
					ps = append(ps, checkElision(unit, label)...)
				}
			}
		}
	case tx.Name == "walkthrough":
		for i, l := range strings.Split(s, "\n") {
			ps = append(ps, checkElision(fmt.Sprintf("line %d", i+1), l)...)
		}
	}
	return ps
}

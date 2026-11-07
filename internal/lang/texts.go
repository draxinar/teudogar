package lang

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strconv"
	"strings"
)

// A TextLine is a line of texts.txt: a string of the program's texts
// (packed.Block.Texts) that the build gives the French, at its offset.
type TextLine struct {
	Off  int    // the offset in the texts
	Text string // the string, in UTF-8
	Line int    // the line of texts.txt
}

// ReadTexts parses texts.txt: each line holds an offset in hex, a tab,
// and a string, with the escapes of Escape. Blank lines and lines that
// start with # are comments.
func ReadTexts(r io.Reader) ([]TextLine, error) {
	var lines []TextLine
	seen := make(map[int]int)
	sc := bufio.NewScanner(r)
	sc.Buffer(nil, 1<<20)
	for n := 1; sc.Scan(); n++ {
		l := sc.Text()
		if l == "" || l[0] == '#' {
			continue
		}
		hex, s, ok := strings.Cut(l, "\t")
		if !ok {
			return nil, fmt.Errorf("line %d: want an offset, a tab, and a string", n)
		}
		off, err := strconv.ParseUint(hex, 16, 16)
		if err != nil {
			return nil, fmt.Errorf("line %d: offset %q, want hex from 0 to ffff", n, hex)
		}
		if prev, ok := seen[int(off)]; ok {
			return nil, fmt.Errorf("line %d: offset %x already on line %d", n, off, prev)
		}
		seen[int(off)] = n
		t, err := Unescape(s)
		if err != nil {
			return nil, fmt.Errorf("line %d: %v", n, err)
		}
		lines = append(lines, TextLine{Off: int(off), Text: t, Line: n})
	}
	if err := sc.Err(); err != nil {
		return nil, err
	}
	return lines, nil
}

// Escape returns s with CR, LF, tab, and backslash written as \r, \n,
// \t, and \\, and the other control characters as \x and two hex
// digits, so that it fits on a line and shows every byte.
func Escape(s string) string {
	var b strings.Builder
	for i := 0; i < len(s); i++ {
		switch c := s[i]; {
		case c == '\\':
			b.WriteString(`\\`)
		case c == '\r':
			b.WriteString(`\r`)
		case c == '\n':
			b.WriteString(`\n`)
		case c == '\t':
			b.WriteString(`\t`)
		case c < 0x20 || c == 0x7f:
			fmt.Fprintf(&b, `\x%02x`, c)
		default:
			b.WriteByte(c)
		}
	}
	return b.String()
}

// Unescape is the inverse of Escape. It fails on any other escape.
func Unescape(s string) (string, error) {
	if strings.IndexByte(s, '\\') < 0 {
		return s, nil
	}
	var b strings.Builder
	for i := 0; i < len(s); i++ {
		if s[i] != '\\' {
			b.WriteByte(s[i])
			continue
		}
		if i+1 == len(s) {
			return "", fmt.Errorf("\\ at the end of the line, want \\\\")
		}
		i++
		switch s[i] {
		case 'r':
			b.WriteByte('\r')
		case 'n':
			b.WriteByte('\n')
		case 't':
			b.WriteByte('\t')
		case '\\':
			b.WriteByte('\\')
		case 'x':
			v, err := strconv.ParseUint(s[i+1:min(i+3, len(s))], 16, 8)
			if err != nil || i+3 > len(s) || (v >= 0x20 && v != 0x7f) {
				return "", fmt.Errorf("escape \\x%s, want \\x and two hex digits of a control character", s[i+1:min(i+3, len(s))])
			}
			b.WriteByte(byte(v))
			i += 2
		default:
			return "", fmt.Errorf("escape \\%c, want \\r, \\n, \\t, \\\\, or \\x and two hex digits", s[i])
		}
	}
	return b.String(), nil
}

// A Pick is a call of pick (texts.cpp): the offsets of its six texts,
// one per language, in the order of the languages.
type Pick struct {
	Off  [NLangs]int
	File string // the source file of the call
	Line int
}

// pickArg matches an argument of pick that names a string of the texts:
// T(0x...), texts + an offset in hex or decimal, or texts itself.
var pickArg = regexp.MustCompile(`^(?:T\((0x[0-9a-f]+)\)|texts(?: \+ (0x[0-9a-f]+|[0-9]+))?)$`)

// ScanPicks returns the calls of pick in the C++ files of dir, in the
// order of the files' names and of the calls in each.
func ScanPicks(dir string) ([]Pick, error) {
	names, err := filepath.Glob(filepath.Join(dir, "*.cpp"))
	if err != nil {
		return nil, err
	}
	sort.Strings(names)
	var picks []Pick
	for _, name := range names {
		src, err := os.ReadFile(name)
		if err != nil {
			return nil, err
		}
		p, err := scanPicks(src)
		if err != nil {
			return nil, fmt.Errorf("%s: %v", name, err)
		}
		for i := range p {
			p[i].File = filepath.Base(name)
		}
		picks = append(picks, p...)
	}
	return picks, nil
}

// scanPicks returns the calls of pick in src. A call is "pick(" after a
// byte that cannot end an identifier, then six arguments that name
// strings of the texts. pick's own definition, whose arguments are
// declarations, is skipped.
func scanPicks(src []byte) ([]Pick, error) {
	var picks []Pick
	for off := 0; ; {
		i := bytes.Index(src[off:], []byte("pick("))
		if i < 0 {
			return picks, nil
		}
		i += off
		off = i + len("pick(")
		if i > 0 && isIdent(src[i-1]) {
			continue
		}
		args := splitArgs(src[off:], ')')
		if args == nil {
			return nil, fmt.Errorf("line %d: pick without its closing parenthesis", lineOf(src, i))
		}
		if len(args) != NLangs {
			continue
		}
		var p Pick
		ok := true
		for l, a := range args {
			m := pickArg.FindStringSubmatch(a)
			if m == nil {
				ok = false
				break
			}
			num := m[1] + m[2]
			if num == "" {
				continue
			}
			v, err := strconv.ParseUint(num, 0, 16)
			if err != nil {
				return nil, fmt.Errorf("line %d: argument %q: %v", lineOf(src, i), a, err)
			}
			p.Off[l] = int(v)
		}
		if !ok {
			if strings.HasPrefix(args[0], "char *") {
				continue
			}
			return nil, fmt.Errorf("line %d: pick with arguments %q, want T(x), texts + x, or texts", lineOf(src, i), args)
		}
		p.Line = lineOf(src, i)
		picks = append(picks, p)
	}
}

// splitArgs splits b, the text after an opening parenthesis, into the
// arguments up to the closing one, each with its blanks trimmed. It
// returns nil when the parenthesis stays open.
func splitArgs(b []byte, closing byte) []string {
	var args []string
	depth := 0
	start := 0
	for i, c := range b {
		switch {
		case c == '(':
			depth++
		case c == ')' && depth > 0:
			depth--
		case c == closing && depth == 0:
			return append(args, strings.Join(strings.Fields(string(b[start:i])), " "))
		case c == ',' && depth == 0:
			args = append(args, strings.Join(strings.Fields(string(b[start:i])), " "))
			start = i + 1
		}
	}
	return nil
}

func isIdent(c byte) bool {
	return c == '_' || '0' <= c && c <= '9' || 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z'
}

// lineOf returns the line of src that holds the byte at off.
func lineOf(src []byte, off int) int {
	return 1 + bytes.Count(src[:off], []byte("\n"))
}

// Encoded lists the groups of the texts whose strings the code passes
// through xorstr (str.cpp): the program keeps them XORed, and xorstr
// XORs them back. Each group is given by its German offset.
var Encoded = []int{
	0x707a, // "corrupted", inisys and inisys2 (system.cpp)
	0x7394, // "Please try to re-install", inisys (system.cpp)
	0x719f, // the NPC's tip, talkto (actions.cpp)
}

// XorStr returns s XORed as xorstr (str.cpp) XORs it: byte i with 0xff,
// less 1 when i is odd. The operation is its own inverse.
func XorStr(s []byte) []byte {
	t := make([]byte, len(s))
	for i, c := range s {
		t[i] = c ^ (0xff - byte(i&1))
	}
	return t
}

// CString returns the string of the texts b that starts at off and ends
// at a NUL.
func CString(b []byte, off int) []byte {
	if off < 0 || off >= len(b) {
		return nil
	}
	n := bytes.IndexByte(b[off:], 0)
	if n < 0 {
		return b[off:]
	}
	return b[off : off+n]
}

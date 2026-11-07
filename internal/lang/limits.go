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

// A Site is a call that composes a string of strs into a buffer: its
// source file and line, and the buffer it writes.
type Site struct {
	File   string
	Line   int
	Target string
}

// A composer is a function that copies or formats strings into a
// buffer: the argument that names the buffer, or -1 for msgfmt[200] of
// the messages (combat.cpp), and the first argument that may name a
// string of strs.
type composer struct {
	name         string
	target, from int
}

// composers lists wsprintf, wvsprintf, catstr, and copystr (str.cpp),
// and the functions of combat.cpp that compose a message in msgfmt[200]
// or copy it into a row of msgtext, as many bytes.
var composers = []composer{
	{"wsprintf(", 0, 1},
	{"wvsprintf(", 0, 1},
	{"catstr(", 0, 1},
	{"copystr(", 0, 1},
	{"msgf(", -1, 1},
	{"msgcat(", -1, 1},
	{"addmsg(", -1, 1},
	{"denymsg(", -1, 0},
	{"failmsg(", -1, 0},
	{"askmsg(", -1, 0},
	{"nosuccess(", -1, 0},
	{"notworking(", -1, 0},
	{"usefail(", -1, 0},
	{"prependmsg(", -1, 0},
}

var (
	strsArg = regexp.MustCompile(`^strs\[(.*)\]$`)
	number  = regexp.MustCompile(`\b(0x[0-9a-f]+|[0-9]+)\b`)
)

// ScanComposed returns the calls of the C++ files of dir that compose a
// string of strs into a buffer, by the string's index: each index that
// an argument after the buffer names, the format and the strings that
// it inserts alike, and the six slots of a table that it picks by an
// item's number and gender (itemplural, itemgender).
func ScanComposed(dir string) (map[int][]Site, error) {
	names, err := filepath.Glob(filepath.Join(dir, "*.cpp"))
	if err != nil {
		return nil, err
	}
	sort.Strings(names)
	sites := make(map[int][]Site)
	for _, name := range names {
		src, err := os.ReadFile(name)
		if err != nil {
			return nil, err
		}
		for _, c := range composers {
			for off := 0; ; {
				i := bytes.Index(src[off:], []byte(c.name))
				if i < 0 {
					break
				}
				i += off
				off = i + len(c.name)
				if i > 0 && isIdent(src[i-1]) {
					continue
				}
				// A definition starts its line, and composes nothing.
				if i == 0 || src[i-1] == '\n' {
					continue
				}
				args := splitArgs(src[off:], ')')
				if len(args) <= c.from || c.target >= len(args) {
					continue
				}
				s := Site{File: filepath.Base(name), Line: lineOf(src, i), Target: "msgfmt"}
				if c.target >= 0 {
					s.Target = args[c.target]
				}
				for _, a := range args[c.from:] {
					m := strsArg.FindStringSubmatch(strings.TrimSpace(a))
					if m == nil {
						continue
					}
					for _, n := range number.FindAllString(m[1], -1) {
						v, err := strconv.ParseUint(n, 0, 16)
						if err != nil || v < 10 {
							continue
						}
						k := int(v)
						sites[k] = append(sites[k], s)
						if strings.Contains(m[1], "itemplural") {
							for j := 1; j < 6; j++ {
								sites[k+j] = append(sites[k+j], s)
							}
						}
					}
				}
			}
		}
	}
	return sites, nil
}

// ReadRooms parses buffers.txt: each line holds the index of a string
// of strs, a tab, the most bytes in code page 1252 that the string may
// take in its buffers, and a tab and the reckoning, which names the
// buffers and their sizes. Blank lines and lines that start with # are
// comments.
func ReadRooms(rd io.Reader) (map[int]int, error) {
	rooms := make(map[int]int)
	sc := bufio.NewScanner(rd)
	for n := 1; sc.Scan(); n++ {
		l := sc.Text()
		if l == "" || l[0] == '#' {
			continue
		}
		f := strings.SplitN(l, "\t", 3)
		if len(f) != 3 {
			return nil, fmt.Errorf("line %d: want a string's index, its room, and the reckoning, separated by tabs", n)
		}
		i, err1 := strconv.Atoi(f[0])
		room, err2 := strconv.Atoi(f[1])
		if err1 != nil || err2 != nil || i < 0 || room < 1 {
			return nil, fmt.Errorf("line %d: %q and %q, want an index and a room in bytes", n, f[0], f[1])
		}
		if _, ok := rooms[i]; ok {
			return nil, fmt.Errorf("line %d: string %d given twice", n, i)
		}
		rooms[i] = room
	}
	return rooms, sc.Err()
}

// CheckLengths reports the strings of strs (the French, or a language
// that errata fixes) that the game composes into buffers and that run
// longer than the longest of the release's German, English, and
// Italian, which the buffers hold: each needs its buffer checked, and
// buffers lists the strings checked so, by index, with the bytes that
// their buffers hold for them. A string of buffers keeps to its room
// whether or not sites names it.
func CheckLengths(sites map[int][]Site, buffers map[int]int, de, en, it, strs []string) []Problem {
	var ps []Problem
	// A room holds wherever the game composes its string, which the scan
	// may see through a table, as tiredtext (texts.cpp) picks its string.
	idx := make([]int, 0, len(sites)+len(buffers))
	for i := range sites {
		idx = append(idx, i)
	}
	for i := range buffers {
		if _, ok := sites[i]; !ok {
			idx = append(idx, i)
		}
	}
	sort.Ints(idx)
	for _, i := range idx {
		if i >= len(strs) || i >= len(de) {
			continue
		}
		f, err := ToCP1252(strs[i])
		if err != nil {
			continue
		}
		longest := 0
		for _, l := range [][]string{de, en, it} {
			if i < len(l) {
				if b, err := ToCP1252(l[i]); err == nil {
					longest = max(longest, len(b))
				}
			}
		}
		if room, ok := buffers[i]; ok {
			if len(f) > room {
				ps = append(ps, problem(fmt.Sprintf("string %d", i), "%d bytes, want %d at most, the room of its buffer", len(f), room))
			}
			continue
		}
		if len(f) > longest {
			s := sites[i][0]
			ps = append(ps, problem(fmt.Sprintf("string %d", i), "%d bytes, longer than the release's %d, composed into %s (%s:%d): check its buffer", len(f), longest, s.Target, s.File, s.Line))
		}
	}
	return ps
}

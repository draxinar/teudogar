package text

import (
	"bytes"
	"fmt"
	"strconv"
)

// The script's limits: loadscript (00419a38, dialog.cpp) copies the text
// into script, which holds MaxScript bytes, and indexmarks (00419944)
// records the offsets of the marks and of the end in markoff, which
// holds 350 offsets.
const (
	MaxScript = 0x6e000
	MaxScenes = 349
)

// A Script is the dialog script: entries 8205, 8206, and 8209, and the
// demo's 12, 13, and 16. Its text is a run of scenes, each opened by a
// mark, a line "// n" whose number n names the scene. parsescene
// (00419fcc, dialog.cpp) parses a scene into its nodes when it plays.
type Script struct {
	Head   string // the text before the first mark: blank lines
	Scenes []Scene
}

// A Scene is the text from a mark up to the next one.
type Scene struct {
	// Mark is the number of the scene's mark, from 0 to 65535, since
	// marknum keeps it in 16 bits.
	Mark int

	// Text follows the number: the rest of the mark's line, the
	// scene's nodes, and the line ends before the next mark.
	Text string
}

// DecodeScript returns the script that the entry b holds. The entry,
// each byte XORed with Key, holds the text. A mark is "// " and a digit
// at the end of a run of CR, LF, and spaces that holds a CR, as
// indexmarks (00419944, dialog.cpp) finds it. Its number is the run of
// digits from there, which must read as strconv.Itoa writes a number
// from 0 to 65535.
func DecodeScript(b []byte) (*Script, error) {
	if len(b) > MaxScript {
		return nil, formatError("script of %d bytes, want at most %d", len(b), MaxScript)
	}
	t := xor(b)
	offs := marks(t)
	if len(offs) > MaxScenes {
		return nil, formatError("%d scenes, want at most %d", len(offs), MaxScenes)
	}
	s := &Script{Head: string(t)}
	if len(offs) > 0 {
		s.Head = string(t[:offs[0]])
	}
	for i, off := range offs {
		end := len(t)
		if i+1 < len(offs) {
			end = offs[i+1]
		}
		p := off + len("// ")
		q := p
		for q < end && isdigit(t[q]) {
			q++
		}
		digits := string(t[p:q])
		n, err := strconv.Atoi(digits)
		if err != nil || n > 0xffff || strconv.Itoa(n) != digits {
			return nil, formatError("scene %d at %#x: mark %.10q, want a number from 0 to 65535", i, off, digits)
		}
		s.Scenes = append(s.Scenes, Scene{Mark: n, Text: string(t[q:end])})
	}
	return s, nil
}

// EncodeScript returns the entry that holds s, the inverse of
// DecodeScript. The head, and the text of each scene but the last, must
// end in a run of CR, LF, and spaces that holds a CR, so that the next
// mark starts a line. The head and the texts must hold no other mark,
// and a text must start with a byte other than a digit.
func EncodeScript(s *Script) ([]byte, error) {
	if len(s.Scenes) > MaxScenes {
		return nil, fmt.Errorf("text: %d scenes, want at most %d", len(s.Scenes), MaxScenes)
	}
	t := []byte(s.Head)
	want := make([]int, len(s.Scenes))
	for i, sc := range s.Scenes {
		if sc.Mark < 0 || sc.Mark > 0xffff {
			return nil, fmt.Errorf("text: scene %d: mark %d, want 0 to 65535", i, sc.Mark)
		}
		if sc.Text != "" && isdigit(sc.Text[0]) {
			return nil, fmt.Errorf("text: scene %d (mark %d): text starts with a digit", i, sc.Mark)
		}
		want[i] = len(t)
		t = append(t, "// "...)
		t = strconv.AppendInt(t, int64(sc.Mark), 10)
		t = append(t, sc.Text...)
	}
	if len(t) > MaxScript {
		return nil, fmt.Errorf("text: script of %d bytes, want at most %d", len(t), MaxScript)
	}
	offs := marks(t)
	for k := range max(len(offs), len(want)) {
		switch {
		case k < len(offs) && k < len(want) && offs[k] == want[k]:
			continue
		case k < len(offs) && (k == len(want) || offs[k] < want[k]):
			if k == 0 {
				return nil, fmt.Errorf("text: head holds a mark at %#x", offs[k])
			}
			return nil, fmt.Errorf("text: scene %d (mark %d) holds a mark at %#x", k-1, s.Scenes[k-1].Mark, offs[k])
		default:
			return nil, fmt.Errorf("text: scene %d (mark %d) starts no line", k, s.Scenes[k].Mark)
		}
	}
	return xor(t), nil
}

// marks returns the offsets of the marks of the text t, as indexmarks
// (00419944, dialog.cpp) finds them: from each CR it skips CR, LF, and
// spaces, and takes "// " and a digit there for a mark.
func marks(t []byte) []int {
	var offs []int
	for i := 0; i < len(t); {
		cr := bytes.IndexByte(t[i:], '\r')
		if cr < 0 {
			break
		}
		j := i + cr
		for j < len(t) && (t[j] == '\r' || t[j] == '\n' || t[j] == ' ') {
			j++
		}
		if j+3 < len(t) && string(t[j:j+3]) == "// " && isdigit(t[j+3]) {
			offs = append(offs, j)
			j += 3
		}
		i = j
	}
	return offs
}

func isdigit(c byte) bool { return '0' <= c && c <= '9' }

package text

import (
	"bytes"
	"errors"
	"fmt"
	"strings"
)

// MaxTopics is the count of topics that parsetext (0045fb3c, mainmenu.cpp)
// records in textlines. It leaves the text from a 101st "[" on in the
// last topic.
const MaxTopics = 100

// A Doc is a document that showtext (00460500, mainmenu.cpp) shows: the
// manual (entries 5009, 5010, and 5013), a part of the encyclopedia
// (8106 and 8113 in German, 8107 and 8114 in English), or the
// walkthrough (8341, 8342, and 8345). The text is plain. Each "[" opens
// a topic, whose first line is its name.
//
// showtext joins the two parts of the encyclopedia, drops the LF bytes,
// and runs the walkthrough through expandstr (00419800, dialog.cpp) before
// parsetext (0045fb3c) cuts the text into topics. The second part of
// the encyclopedia starts with a blank line and its first topic.
type Doc struct {
	// Head is the text before the first topic: the title line, which
	// parsetext puts in capitals, and blank lines.
	Head string

	Topics []Topic
}

// A Topic is a page of a document.
type Topic struct {
	// Name is the rest of the line of the "[": the label of the
	// topic's item in the menu, which a name of "-" disables.
	Name string

	// Text is the lines after it, up to the next "[": the page, whose
	// first line is its heading.
	Text string
}

// DecodeDoc returns the document that the entry b holds. Each "[" opens
// a topic, and the name of a topic ends at the first CR, which an LF
// must follow.
func DecodeDoc(b []byte) (*Doc, error) {
	parts := bytes.Split(b, []byte("["))
	if len(parts)-1 > MaxTopics {
		return nil, formatError("%d topics, want at most %d", len(parts)-1, MaxTopics)
	}
	d := &Doc{Head: string(parts[0])}
	off := len(parts[0])
	for i, p := range parts[1:] {
		name, text, ok := bytes.Cut(p, []byte("\r"))
		if !ok {
			return nil, formatError("topic %d at %#x: name without a line end", i, off)
		}
		if len(text) == 0 || text[0] != '\n' {
			return nil, formatError("topic %d at %#x: name ends in CR without LF", i, off)
		}
		d.Topics = append(d.Topics, Topic{Name: string(name), Text: string(text[1:])})
		off += 1 + len(p)
	}
	return d, nil
}

// EncodeDoc returns the entry that holds d, the inverse of DecodeDoc.
// The head and the topics must hold no "[", and a name no CR.
func EncodeDoc(d *Doc) ([]byte, error) {
	if len(d.Topics) > MaxTopics {
		return nil, fmt.Errorf("text: %d topics, want at most %d", len(d.Topics), MaxTopics)
	}
	if strings.IndexByte(d.Head, '[') >= 0 {
		return nil, errors.New("text: head holds a [")
	}
	b := []byte(d.Head)
	for i, t := range d.Topics {
		if strings.ContainsAny(t.Name, "[\r") {
			return nil, fmt.Errorf("text: topic %d: name %q holds a [ or a CR", i, t.Name)
		}
		if strings.IndexByte(t.Text, '[') >= 0 {
			return nil, fmt.Errorf("text: topic %d (%s): text holds a [", i, t.Name)
		}
		b = append(b, '[')
		b = append(b, t.Name...)
		b = append(b, "\r\n"...)
		b = append(b, t.Text...)
	}
	return b, nil
}

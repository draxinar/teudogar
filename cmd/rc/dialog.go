package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
)

// dsSetFont is the style bit of a dialog whose template names a font.
const dsSetFont = 0x40

// The classes that a control names by ordinal.
var classNames = map[uint16]string{
	0x80: "BUTTON",
	0x81: "EDIT",
	0x82: "STATIC",
	0x83: "LISTBOX",
	0x84: "SCROLLBAR",
	0x85: "COMBOBOX",
}

// A dlgReader reads a dialog template and keeps the first error.
type dlgReader struct {
	data []byte
	off  int
	err  error
}

func (r *dlgReader) fail(format string, args ...any) {
	if r.err == nil {
		r.err = fmt.Errorf("%w: at %#x: %s", errFormat, r.off, fmt.Sprintf(format, args...))
	}
}

func (r *dlgReader) u16() uint16 {
	if r.err != nil || r.off+2 > len(r.data) {
		r.fail("template ends")
		return 0
	}
	v := binary.LittleEndian.Uint16(r.data[r.off:])
	r.off += 2
	return v
}

func (r *dlgReader) u32() uint32 {
	lo := r.u16()
	return uint32(r.u16())<<16 | uint32(lo)
}

func (r *dlgReader) i16() int16 { return int16(r.u16()) }

// str reads a NUL-terminated UTF-16 string and returns it in Latin-1, as
// the script holds it.
func (r *dlgReader) str() string {
	var b []byte
	for {
		c := r.u16()
		if r.err != nil || c == 0 {
			return string(b)
		}
		if c < 0x20 || c > 0xff || c == '\\' {
			r.fail("character %#x in a string", c)
			return ""
		}
		b = append(b, byte(c))
	}
}

// strOrOrd reads a string or, after 0xffff, an ordinal, and returns the
// string, or the ordinal with isOrd true.
func (r *dlgReader) strOrOrd() (s string, ord uint16, isOrd bool) {
	if r.off+2 <= len(r.data) && binary.LittleEndian.Uint16(r.data[r.off:]) == 0xffff {
		r.u16()
		return "", r.u16(), true
	}
	return r.str(), 0, false
}

// align moves to the next multiple of 4 in the template.
func (r *dlgReader) align() {
	r.off = (r.off + 3) &^ 3
}

// dialog writes the DIALOG resource data as a DIALOG statement named id,
// with a CONTROL statement for each control.
func dialog(b *bytes.Buffer, id string, data []byte) error {
	r := &dlgReader{data: data}
	style := r.u32()
	if style == 0xffff0001 {
		return fmt.Errorf("%w: an extended template", errFormat)
	}
	exstyle := r.u32()
	n := int(r.u16())
	x, y, cx, cy := r.i16(), r.i16(), r.i16(), r.i16()
	menu, menuOrd, menuIsOrd := r.strOrOrd()
	class, classOrd, classIsOrd := r.strOrOrd()
	title := r.str()
	var size uint16
	var font string
	if style&dsSetFont != 0 {
		size = r.u16()
		font = r.str()
	}
	if r.err != nil {
		return r.err
	}
	fmt.Fprintf(b, "%s DIALOG %d, %d, %d, %d\n", id, x, y, cx, cy)
	fmt.Fprintf(b, "STYLE %#x\n", style)
	if exstyle != 0 {
		fmt.Fprintf(b, "EXSTYLE %#x\n", exstyle)
	}
	if title != "" {
		fmt.Fprintf(b, "CAPTION %s\n", quote(title))
	}
	switch {
	case menuIsOrd:
		fmt.Fprintf(b, "MENU %d\n", menuOrd)
	case menu != "":
		fmt.Fprintf(b, "MENU %s\n", menu)
	}
	switch {
	case classIsOrd:
		fmt.Fprintf(b, "CLASS %d\n", classOrd)
	case class != "":
		fmt.Fprintf(b, "CLASS %s\n", quote(class))
	}
	if style&dsSetFont != 0 {
		fmt.Fprintf(b, "FONT %d, %s\n", size, quote(font))
	}
	b.WriteString("BEGIN\n")
	for range n {
		r.align()
		style := r.u32()
		exstyle := r.u32()
		x, y, cx, cy := r.i16(), r.i16(), r.i16(), r.i16()
		cid := r.i16()
		class, classOrd, classIsOrd := r.strOrOrd()
		text, textOrd, textIsOrd := r.strOrOrd()
		extra := r.u16()
		if r.err != nil {
			return r.err
		}
		if extra != 0 {
			return fmt.Errorf("%w: control %d: %d bytes of creation data", errFormat, cid, extra)
		}
		if classIsOrd {
			name, ok := classNames[classOrd]
			if !ok {
				return fmt.Errorf("%w: control %d: class %#x", errFormat, cid, classOrd)
			}
			class = name
		}
		t := quote(text)
		if textIsOrd {
			t = fmt.Sprint(textOrd)
		}
		fmt.Fprintf(b, "\tCONTROL %s, %d, %s, %#x, %d, %d, %d, %d", t, cid, quote(class), style, x, y, cx, cy)
		if exstyle != 0 {
			fmt.Fprintf(b, ", %#x", exstyle)
		}
		b.WriteString("\n")
	}
	b.WriteString("END\n")
	for _, c := range data[min(r.off, len(data)):] {
		if c != 0 || len(data)-r.off > 3 {
			return fmt.Errorf("%w: %d bytes past the last control", errFormat, len(data)-r.off)
		}
	}
	return nil
}

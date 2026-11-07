// Rc writes the resources of a PE file as the source that Watcom's
// resource compiler, wrc, compiles back into the same bytes: a script and
// the files it names.
//
// Usage:
//
//	rc file dir
//
// Rc prints the script and writes into dir the files it names: <id>.bmp
// for each bitmap, the resource after a BITMAPFILEHEADER, and <id>.ico
// for each icon group, the group's directory with its images. The script
// holds the resources in the order of the directory, by type and then by
// ID: each bitmap and icon group as a line that names its file, each
// dialog with its controls, as CONTROL statements that give the class
// and the style in full, and each accelerator table. wrc numbers the
// images of the icon groups in the order of the script, so rc fails when
// the groups' images run in another order, and on any resource of
// another type.
package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"strings"

	"github.com/draxinar/teudogar/internal/pe"
)

// Resource types.
const (
	typeBitmap    = 2
	typeIcon      = 3
	typeDialog    = 5
	typeAccel     = 9
	typeGroupIcon = 14
)

func usage() {
	fmt.Fprintf(os.Stderr, "usage: rc file dir\n")
	os.Exit(2)
}

func main() {
	log.SetFlags(0)
	log.SetPrefix("rc: ")
	if len(os.Args) != 3 {
		usage()
	}
	name, dir := os.Args[1], os.Args[2]
	data, err := os.ReadFile(name)
	if err != nil {
		log.Fatal(err)
	}
	f, err := pe.Parse(data)
	if err != nil {
		log.Fatalf("%s: %v", name, err)
	}
	script, files, err := source(f.Resources)
	if err != nil {
		log.Fatalf("%s: %v", name, err)
	}
	for n, b := range files {
		if err := os.WriteFile(filepath.Join(dir, n), b, 0o666); err != nil {
			log.Fatal(err)
		}
	}
	if _, err := os.Stdout.Write(script); err != nil {
		log.Fatal(err)
	}
}

// source returns the script for res and the files it names, by name.
func source(res []pe.Resource) ([]byte, map[string][]byte, error) {
	icons := make(map[uint32][]byte)
	for _, r := range res {
		if r.Type == (pe.ResID{ID: typeIcon}) && r.Name.Name == "" {
			if _, ok := icons[r.Name.ID]; !ok {
				icons[r.Name.ID] = r.Data
			}
		}
	}
	var b bytes.Buffer
	files := make(map[string][]byte)
	next := uint32(1) // the ID wrc gives the next icon image
	for _, r := range res {
		if r.Type.Name != "" {
			return nil, nil, fmt.Errorf("%w: resource of type %q: want a standard type", errFormat, r.Type.Name)
		}
		id := r.Name.String()
		if r.Name.Name != "" {
			id = quote(r.Name.Name)
		}
		switch r.Type.ID {
		case typeIcon:
			// The icon groups name the images.
		case typeBitmap:
			bmp, err := bitmap(r.Data)
			if err != nil {
				return nil, nil, fmt.Errorf("bitmap %s: %w", r.Name, err)
			}
			name, err := fileName(r.Name, ".bmp")
			if err != nil {
				return nil, nil, fmt.Errorf("bitmap %s: %w", r.Name, err)
			}
			files[name] = bmp
			fmt.Fprintf(&b, "%s BITMAP \"%s\"\n", id, name)
		case typeGroupIcon:
			ids, err := iconIDs(r.Data)
			if err != nil {
				return nil, nil, fmt.Errorf("icon group %s: %w", r.Name, err)
			}
			for _, i := range ids {
				if i != next {
					return nil, nil, fmt.Errorf("%w: icon group %s: image %d where wrc numbers %d", errFormat, r.Name, i, next)
				}
				next++
			}
			ic, err := ico(r.Data, icons)
			if err != nil {
				return nil, nil, fmt.Errorf("icon group %s: %w", r.Name, err)
			}
			name, err := fileName(r.Name, ".ico")
			if err != nil {
				return nil, nil, fmt.Errorf("icon group %s: %w", r.Name, err)
			}
			files[name] = ic
			fmt.Fprintf(&b, "%s ICON \"%s\"\n", id, name)
		case typeDialog:
			if err := dialog(&b, id, r.Data); err != nil {
				return nil, nil, fmt.Errorf("dialog %s: %w", r.Name, err)
			}
		case typeAccel:
			if err := accel(&b, id, r.Data); err != nil {
				return nil, nil, fmt.Errorf("accelerators %s: %w", r.Name, err)
			}
		default:
			return nil, nil, fmt.Errorf("%w: resource %s of type %d: want a bitmap, icon, dialog, or accelerator table", errFormat, r.Name, r.Type.ID)
		}
	}
	return b.Bytes(), files, nil
}

// fileName returns the name of the file in dir that holds the resource
// id, with the extension ext. A name that id gives as a string must hold
// no path and no quote, since the script quotes the file's name.
func fileName(id pe.ResID, ext string) (string, error) {
	if strings.ContainsAny(id.Name, `/\:"`) {
		return "", fmt.Errorf("%w: name %q, want a file name without a path or a quote", errFormat, id.Name)
	}
	return id.String() + ext, nil
}

// bitmapFileHeaderLen is the size of the BITMAPFILEHEADER that a .bmp
// file holds before the bitmap that a resource holds.
const bitmapFileHeaderLen = 14

// bitmap returns the .bmp file of a bitmap resource: a
// BITMAPFILEHEADER, then the resource, a BITMAPINFOHEADER, its colors,
// and the bits.
func bitmap(dib []byte) ([]byte, error) {
	if len(dib) < 40 {
		return nil, fmt.Errorf("%w: %d bytes, want a BITMAPINFOHEADER of 40 at least", errFormat, len(dib))
	}
	hdr := binary.LittleEndian.Uint32(dib)
	bits := binary.LittleEndian.Uint16(dib[14:])
	used := binary.LittleEndian.Uint32(dib[32:])
	colors := used
	if colors == 0 && bits <= 8 {
		colors = 1 << bits
	}
	off := bitmapFileHeaderLen + int(hdr) + 4*int(colors)
	if hdr < 40 || off > bitmapFileHeaderLen+len(dib) {
		return nil, fmt.Errorf("%w: header of %d bytes and %d colors in %d bytes", errFormat, hdr, colors, len(dib))
	}
	b := []byte("BM")
	b = binary.LittleEndian.AppendUint32(b, uint32(bitmapFileHeaderLen+len(dib)))
	b = binary.LittleEndian.AppendUint32(b, 0)
	b = binary.LittleEndian.AppendUint32(b, uint32(off))
	return append(b, dib...), nil
}

// iconIDs returns the IDs of the images of an icon group, in order.
func iconIDs(group []byte) ([]uint32, error) {
	if len(group) < dirLen {
		return nil, fmt.Errorf("%w: %d bytes, want %d at least", errFormat, len(group), dirLen)
	}
	n := int(binary.LittleEndian.Uint16(group[4:]))
	if len(group) != dirLen+n*groupEntLen {
		return nil, fmt.Errorf("%w: %d bytes for %d entries, want %d", errFormat, len(group), n, dirLen+n*groupEntLen)
	}
	var ids []uint32
	for i := range n {
		ids = append(ids, uint32(binary.LittleEndian.Uint16(group[dirLen+i*groupEntLen+12:])))
	}
	return ids, nil
}

// Accelerator flags.
const (
	accVirtKey  = 0x01
	accNoInvert = 0x02
	accShift    = 0x04
	accControl  = 0x08
	accAlt      = 0x10
	accLast     = 0x80
)

// accel writes the accelerator table data as an ACCELERATORS statement
// named id: each entry's key, in hex, its command, and its flags.
func accel(b *bytes.Buffer, id string, data []byte) error {
	const entLen = 8 // flags, key, command, padding
	if len(data) == 0 || len(data)%entLen != 0 {
		return fmt.Errorf("%w: %d bytes, want entries of %d", errFormat, len(data), entLen)
	}
	fmt.Fprintf(b, "%s ACCELERATORS\nBEGIN\n", id)
	for off := 0; off < len(data); off += entLen {
		flags := binary.LittleEndian.Uint16(data[off:])
		key := binary.LittleEndian.Uint16(data[off+2:])
		cmd := binary.LittleEndian.Uint16(data[off+4:])
		last := off+entLen == len(data)
		if flags&^(accVirtKey|accNoInvert|accShift|accControl|accAlt|accLast) != 0 || (flags&accLast != 0) != last {
			return fmt.Errorf("%w: entry %d: flags %#x", errFormat, off/entLen, flags)
		}
		fmt.Fprintf(b, "\t%#x, %d", key, cmd)
		if flags&accVirtKey != 0 {
			b.WriteString(", VIRTKEY")
		} else {
			b.WriteString(", ASCII")
		}
		for _, f := range []struct {
			bit  uint16
			name string
		}{{accShift, "SHIFT"}, {accControl, "CONTROL"}, {accAlt, "ALT"}, {accNoInvert, "NOINVERT"}} {
			if flags&f.bit != 0 {
				b.WriteString(", " + f.name)
			}
		}
		b.WriteString("\n")
	}
	b.WriteString("END\n")
	return nil
}

// quote returns s as an .rc string: in double quotes, with each quote
// doubled.
func quote(s string) string {
	return `"` + strings.ReplaceAll(s, `"`, `""`) + `"`
}

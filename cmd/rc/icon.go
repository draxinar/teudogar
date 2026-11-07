package main

import (
	"encoding/binary"
	"errors"
	"fmt"
)

// The sizes of the headers of an icon group and of an .ico file.
const (
	dirLen      = 6  // reserved, type, count
	groupEntLen = 14 // width, height, colors, reserved, planes, bits, size, ID
	icoEntLen   = 16 // the same with the image's offset for the ID
)

var errFormat = errors.New("bad resource")

// ico returns the .ico file of the icon group group, whose entries name
// images in icons by ID.
func ico(group []byte, icons map[uint32][]byte) ([]byte, error) {
	if len(group) < dirLen {
		return nil, fmt.Errorf("%w: %d bytes, want %d at least", errFormat, len(group), dirLen)
	}
	if typ := binary.LittleEndian.Uint16(group[2:]); typ != 1 {
		return nil, fmt.Errorf("%w: type %d, want 1", errFormat, typ)
	}
	n := int(binary.LittleEndian.Uint16(group[4:]))
	if len(group) != dirLen+n*groupEntLen {
		return nil, fmt.Errorf("%w: %d bytes for %d entries, want %d", errFormat, len(group), n, dirLen+n*groupEntLen)
	}
	out := append([]byte(nil), group[:dirLen]...)
	off := dirLen + n*icoEntLen
	var images []byte
	for i := range n {
		e := group[dirLen+i*groupEntLen:][:groupEntLen]
		size := binary.LittleEndian.Uint32(e[8:])
		id := uint32(binary.LittleEndian.Uint16(e[12:]))
		img, ok := icons[id]
		if !ok {
			return nil, fmt.Errorf("%w: icon %d missing", errFormat, id)
		}
		if uint32(len(img)) != size {
			return nil, fmt.Errorf("%w: icon %d holds %d bytes, want %d", errFormat, id, len(img), size)
		}
		out = append(out, e[:12]...)
		out = binary.LittleEndian.AppendUint32(out, uint32(off+len(images)))
		images = append(images, img...)
	}
	return append(out, images...), nil
}

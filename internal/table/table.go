// Package table reads and writes the tables of Teudogar_05.Dat that
// loadtables (00412d54, map.cpp) reads at startup: the type tables of
// entry 5000, the portrait flags of entry 5001, and the square roots of
// entry 5002. Each function takes or returns the decoded bytes of one
// entry, the zlib stream inflated. Words are 16 bits, little-endian.
package table

import (
	"encoding/binary"
	"errors"
	"fmt"
)

// ErrFormat reports bytes that are not a well-formed table. Errors from
// DecodeTypes, DecodeFaces, and DecodeRoots wrap it.
var ErrFormat = errors.New("table: bad format")

func formatError(format string, args ...any) error {
	return fmt.Errorf("%w: %s", ErrFormat, fmt.Sprintf(format, args...))
}

// NTypes is the count of types. A map cell and an item word name a
// type in their low 12 bits.
const NTypes = 0x1000

// Types holds the eight arrays of entry 5000, each indexed by type, in
// the order of the entry. loadtables reads them into the arrays whose
// addresses the comments give, and filltypes (004124f0) then fills some
// types from others: types 0x340 to 0x380 from types 0x1a0 to 0x1e0.
type Types struct {
	// CellFlags holds the bits that the game tests on a cell's own
	// type: 1 (cellseethru, 0041101c), 2 (cellwalkable, 00411034), 4
	// (cellflag4, 0041104c), 8 (cellholds, 0041107c), 0x20 (cellburns,
	// 0041109c), and 0x40 (cellflag40, 00411064).
	CellFlags [NTypes]uint16 // 0054beec

	// ItemFlags holds the bits that the game tests on the types of
	// the items in a cell's list: 1 to 0x20 (itemseethru, 0040fa1c, to
	// itemburns, 00411004), and 0x80 to 0x4000, which updatecell
	// (0041351c) copies into the cell's flags (00515f5c).
	ItemFlags [NTypes]uint16 // 0054eef0

	// Weight is the weight of an item of the type (weightof,
	// 0040f994). From 65000 on, the item is too heavy to move
	// (itemweight, 004960dc).
	Weight [NTypes]uint16 // 00550ef0

	// Class sorts the types (classof, 0040f9ac). A type of class 10 is
	// a container, which itemweight weighs with its items, and the item
	// words of classes 15 and 16 carry an amount in their high 16 bits.
	Class [NTypes]uint8 // 00552ef0

	// Name is the index in strs of the type's name, or 0, for which
	// typetext (0042372c, texts.cpp) returns the string at 004c0870.
	Name [NTypes]uint8 // 00553ef0

	// Price is the base price that valueof (004126d8) computes prices
	// from.
	Price [NTypes]uint16 // 00556ef0

	// Size is the room an item takes in a container (itemvolume,
	// 004962a4), which nofit (00496474) sums against the container's
	// capacity.
	Size [NTypes]int16 // 00554ef0

	// Terrain is the kind of ground (isswamp 0040fd70, terrain356
	// 0040fd90).
	Terrain [NTypes]uint8 // 0054def0
}

// TypesSize is the length of entry 5000: 13 bytes for each type.
const TypesSize = 13 * NTypes

// DecodeTypes returns the type tables that entry 5000 holds.
func DecodeTypes(b []byte) (*Types, error) {
	if len(b) != TypesSize {
		return nil, formatError("type tables of %d bytes, want %d", len(b), TypesSize)
	}
	t := new(Types)
	off := 0
	words := func(a *[NTypes]uint16) {
		for i := range a {
			a[i] = binary.LittleEndian.Uint16(b[off+2*i:])
		}
		off += 2 * NTypes
	}
	octets := func(a *[NTypes]uint8) {
		copy(a[:], b[off:])
		off += NTypes
	}
	words(&t.CellFlags)
	words(&t.ItemFlags)
	words(&t.Weight)
	octets(&t.Class)
	octets(&t.Name)
	words(&t.Price)
	for i := range t.Size {
		t.Size[i] = int16(binary.LittleEndian.Uint16(b[off+2*i:]))
	}
	off += 2 * NTypes
	octets(&t.Terrain)
	return t, nil
}

// EncodeTypes returns entry 5000, which holds t.
func EncodeTypes(t *Types) []byte {
	b := make([]byte, 0, TypesSize)
	words := func(a *[NTypes]uint16) {
		for _, v := range a {
			b = binary.LittleEndian.AppendUint16(b, v)
		}
	}
	words(&t.CellFlags)
	words(&t.ItemFlags)
	words(&t.Weight)
	b = append(b, t.Class[:]...)
	b = append(b, t.Name[:]...)
	words(&t.Price)
	for _, v := range t.Size {
		b = binary.LittleEndian.AppendUint16(b, uint16(v))
	}
	return append(b, t.Terrain[:]...)
}

// NFaces is the count of bytes of entry 5001, and NPortraits the count
// of portraits, whose bytes loadtables reads into 00559178. The bytes
// past them are zero in the release.
const (
	NFaces     = 1000
	NPortraits = 285
)

// Faces holds a byte for each portrait. A portrait panel (stepanims
// 004ab284, panel.cpp) shows the portrait's first picture half as long
// when bit 1 is set, and its second picture when bit 2 is set. The
// release sets bit 4 alone, on portraits 5, 7, and 8.
type Faces [NFaces]uint8

// DecodeFaces returns the portrait flags that entry 5001 holds.
func DecodeFaces(b []byte) (*Faces, error) {
	if len(b) != NFaces {
		return nil, formatError("portrait flags of %d bytes, want %d", len(b), NFaces)
	}
	f := new(Faces)
	copy(f[:], b)
	return f, nil
}

// EncodeFaces returns entry 5001, which holds f.
func EncodeFaces(f *Faces) []byte {
	return append([]byte(nil), f[:]...)
}

// NRoots is the length of entry 5002: a byte for each n from 0 to
// 46,224, so that the root of each n stays below 215.
const NRoots = 215 * 215

// Roots holds the integer square root of each n, which the game reads
// from sqrttab (005966e0): celldist (0046b348) to measure distances,
// and circle and pillrest (graphics.cpp) to draw round shapes.
type Roots [NRoots]uint8

// DecodeRoots returns the square roots that entry 5002 holds.
func DecodeRoots(b []byte) (*Roots, error) {
	if len(b) != NRoots {
		return nil, formatError("square roots of %d bytes, want %d", len(b), NRoots)
	}
	r := new(Roots)
	copy(r[:], b)
	return r, nil
}

// EncodeRoots returns entry 5002, which holds r.
func EncodeRoots(r *Roots) []byte {
	return append([]byte(nil), r[:]...)
}

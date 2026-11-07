// Package pe reads 32-bit Portable Executable (PE) files, the format of
// Win32 programs, as Watcom's WLINK writes them. WLINK leaves each
// section's VirtualSize zero, and the loader then maps RawSize bytes.
package pe

import (
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"
	"strconv"
	"unicode/utf16"
)

// A File is a parsed PE file.
type File struct {
	Data      []byte     // the whole file
	Offset    int        // file offset of the PE signature, from e_lfanew
	Header    Header     // the COFF file header
	Opt       OptHeader  // the optional header
	Dirs      []Dir      // the data directories, indexed by ImportDir and the rest
	Sections  []Section  // in header order
	Imports   []Import   // one per DLL, in table order
	Relocs    []Reloc    // the base relocations, in table order
	Resources []Resource // the leaves of the resource tree, in tree order
	ResDirs   []ResDir   // the tables of the resource tree, in tree order
}

// Header holds the COFF file header.
type Header struct {
	Machine         uint16
	SectionCount    uint16
	TimeDateStamp   uint32 // the link time, in seconds since 1970 UTC
	SymbolOffset    uint32 // file offset of the COFF symbol table, or 0
	SymbolCount     uint32
	OptSize         uint16 // size of the optional header
	Characteristics uint16
}

// OptHeader holds the optional header of a PE32 file. Its addresses are
// RVAs, relative to ImageBase.
type OptHeader struct {
	Magic                          uint16
	LinkerMajor, LinkerMinor       uint8
	CodeSize, DataSize, BSSSize    uint32
	Entry                          uint32
	CodeBase, DataBase             uint32
	ImageBase                      uint32
	SectionAlign, FileAlign        uint32
	OSMajor, OSMinor               uint16
	ImageMajor, ImageMinor         uint16
	SubsystemMajor, SubsystemMinor uint16
	Win32Version                   uint32
	ImageSize, HeadersSize         uint32
	Checksum                       uint32
	Subsystem                      uint16
	DLLCharacteristics             uint16
	StackReserve, StackCommit      uint32
	HeapReserve, HeapCommit        uint32
	LoaderFlags                    uint32
	DirCount                       uint32
}

// Indices of the data directories in File.Dirs.
const (
	ExportDir   = 0
	ImportDir   = 1
	ResourceDir = 2
	RelocDir    = 5
	DebugDir    = 6
	IATDir      = 12
)

// A Dir is a data directory: the RVA and the size of a table.
type Dir struct {
	RVA  uint32
	Size uint32
}

// A Section is one entry of the section table.
type Section struct {
	Name            string
	VirtualSize     uint32 // bytes mapped, or 0 to map RawSize
	RVA             uint32
	RawSize         uint32
	RawOffset       uint32 // file offset of the data, or 0 for uninitialized data
	RelocOffset     uint32
	LineOffset      uint32
	RelocCount      uint16
	LineCount       uint16
	Characteristics uint32
	Data            []byte // the RawSize bytes at RawOffset, or nil for uninitialized data
}

// Size returns the bytes the loader maps for s: VirtualSize, or RawSize
// when VirtualSize is 0.
func (s *Section) Size() uint32 {
	if s.VirtualSize != 0 {
		return s.VirtualSize
	}
	return s.RawSize
}

// An Import is one DLL's descriptor in the import directory.
type Import struct {
	DLL            string
	LookupRVA      uint32 // the import lookup table, or 0 when the address table serves
	TimeDateStamp  uint32
	ForwarderChain uint32
	IATRVA         uint32 // the import address table
	Funcs          []Func
}

// A Func is one imported function.
type Func struct {
	Name    string // "" for an import by ordinal
	Hint    uint16
	Ordinal uint16 // for an import by ordinal
	Slot    uint32 // RVA of its entry in the import address table
}

// Types of base relocation.
const (
	Absolute = 0 // padding that fills a block to a multiple of 4 bytes
	HighLow  = 3 // a 32-bit address
)

// A Reloc is one base relocation: the site of an address that the loader
// adjusts when it maps the image away from ImageBase.
type Reloc struct {
	Type uint8
	RVA  uint32 // the site
}

// A ResID names a resource directory entry, by string or by number.
type ResID struct {
	Name string // the name, or "" for an entry named by number
	ID   uint32
}

// String returns the name, or the number in decimal.
func (id ResID) String() string {
	if id.Name != "" {
		return id.Name
	}
	return strconv.FormatUint(uint64(id.ID), 10)
}

// typeNames names the standard resource types, as winedump spells them.
var typeNames = map[uint32]string{
	1:  "CURSOR",
	2:  "BITMAP",
	3:  "ICON",
	4:  "MENU",
	5:  "DIALOG",
	6:  "STRING",
	7:  "FONTDIR",
	8:  "FONT",
	9:  "ACCELERATOR",
	10: "RCDATA",
	11: "MESSAGETABLE",
	12: "GROUP_CURSOR",
	14: "GROUP_ICON",
	16: "VERSION",
	17: "DLGINCLUDE",
	19: "PLUGPLAY",
	20: "VXD",
	21: "ANICURSOR",
	22: "ANIICON",
	23: "HTML",
	24: "MANIFEST",
}

// TypeName returns the name of the resource type id: its own name, the
// standard name of its number, or the number in decimal.
func TypeName(id ResID) string {
	if id.Name == "" {
		if s, ok := typeNames[id.ID]; ok {
			return s
		}
	}
	return id.String()
}

// A ResDir is a table of the resource tree: the root, a type's names, or
// a name's languages. WLINK writes into each a time that it reads
// separately from the file header's.
type ResDir struct {
	Offset        int    // file offset of the table
	TimeDateStamp uint32 // in seconds since 1970 UTC
}

// A Resource is a leaf of the resource tree: the data of one type, name,
// and language.
type Resource struct {
	Type, Name, Lang ResID
	RVA              uint32 // of the data
	Size             uint32
	CodePage         uint32
	Data             []byte
}

// ErrFormat reports a file that is not a well-formed PE32 file. Errors
// from Parse wrap it.
var ErrFormat = errors.New("pe: bad format")

func formatError(format string, args ...any) error {
	return fmt.Errorf("%w: %s", ErrFormat, fmt.Sprintf(format, args...))
}

// A reader decodes little-endian values from data and keeps the first
// error, which Parse then reports.
type reader struct {
	data []byte
	err  error
}

func (r *reader) check(off, n int, what string) bool {
	if r.err != nil {
		return false
	}
	if off < 0 || n < 0 || off > len(r.data)-n {
		r.err = formatError("%s at %#x: %d bytes past end of file", what, off, n)
		return false
	}
	return true
}

func (r *reader) u8(off int, what string) uint8 {
	if !r.check(off, 1, what) {
		return 0
	}
	return r.data[off]
}

func (r *reader) u16(off int, what string) uint16 {
	if !r.check(off, 2, what) {
		return 0
	}
	return binary.LittleEndian.Uint16(r.data[off:])
}

func (r *reader) u32(off int, what string) uint32 {
	if !r.check(off, 4, what) {
		return 0
	}
	return binary.LittleEndian.Uint32(r.data[off:])
}

func (r *reader) bytes(off, n int, what string) []byte {
	if !r.check(off, n, what) {
		return nil
	}
	return r.data[off : off+n]
}

// Limits that keep a damaged file from asking for huge tables.
const (
	maxSections = 96 // the Windows loader's limit
	maxDirs     = 16
)

// Parse parses the PE32 file data.
func Parse(data []byte) (*File, error) {
	r := &reader{data: data}
	f := &File{Data: data}
	if string(r.bytes(0, 2, "MZ signature")) != "MZ" {
		if r.err != nil {
			return nil, r.err
		}
		return nil, formatError("missing MZ signature")
	}
	pe := int(r.u32(0x3c, "MZ header"))
	if string(r.bytes(pe, 4, "PE signature")) != "PE\x00\x00" {
		if r.err != nil {
			return nil, r.err
		}
		return nil, formatError("missing PE signature at %#x", pe)
	}
	f.Offset = pe

	h := &f.Header
	h.Machine = r.u16(pe+4, "file header")
	h.SectionCount = r.u16(pe+6, "file header")
	h.TimeDateStamp = r.u32(pe+8, "file header")
	h.SymbolOffset = r.u32(pe+12, "file header")
	h.SymbolCount = r.u32(pe+16, "file header")
	h.OptSize = r.u16(pe+20, "file header")
	h.Characteristics = r.u16(pe+22, "file header")
	if r.err != nil {
		return nil, r.err
	}

	o := pe + 24
	opt := &f.Opt
	opt.Magic = r.u16(o, "optional header")
	if r.err != nil {
		return nil, r.err
	}
	if opt.Magic != 0x10b {
		return nil, formatError("optional header magic %#x, want 0x10b (PE32)", opt.Magic)
	}
	opt.LinkerMajor = r.u8(o+2, "optional header")
	opt.LinkerMinor = r.u8(o+3, "optional header")
	opt.CodeSize = r.u32(o+4, "optional header")
	opt.DataSize = r.u32(o+8, "optional header")
	opt.BSSSize = r.u32(o+12, "optional header")
	opt.Entry = r.u32(o+16, "optional header")
	opt.CodeBase = r.u32(o+20, "optional header")
	opt.DataBase = r.u32(o+24, "optional header")
	opt.ImageBase = r.u32(o+28, "optional header")
	opt.SectionAlign = r.u32(o+32, "optional header")
	opt.FileAlign = r.u32(o+36, "optional header")
	opt.OSMajor = r.u16(o+40, "optional header")
	opt.OSMinor = r.u16(o+42, "optional header")
	opt.ImageMajor = r.u16(o+44, "optional header")
	opt.ImageMinor = r.u16(o+46, "optional header")
	opt.SubsystemMajor = r.u16(o+48, "optional header")
	opt.SubsystemMinor = r.u16(o+50, "optional header")
	opt.Win32Version = r.u32(o+52, "optional header")
	opt.ImageSize = r.u32(o+56, "optional header")
	opt.HeadersSize = r.u32(o+60, "optional header")
	opt.Checksum = r.u32(o+64, "optional header")
	opt.Subsystem = r.u16(o+68, "optional header")
	opt.DLLCharacteristics = r.u16(o+70, "optional header")
	opt.StackReserve = r.u32(o+72, "optional header")
	opt.StackCommit = r.u32(o+76, "optional header")
	opt.HeapReserve = r.u32(o+80, "optional header")
	opt.HeapCommit = r.u32(o+84, "optional header")
	opt.LoaderFlags = r.u32(o+88, "optional header")
	opt.DirCount = r.u32(o+92, "optional header")
	if r.err != nil {
		return nil, r.err
	}
	if opt.DirCount > maxDirs {
		return nil, formatError("data directory count %d out of range 0 to %d", opt.DirCount, maxDirs)
	}
	if need := 96 + 8*int(opt.DirCount); int(h.OptSize) < need {
		return nil, formatError("optional header size %d, want %d or more", h.OptSize, need)
	}
	f.Dirs = make([]Dir, opt.DirCount)
	for i := range f.Dirs {
		f.Dirs[i].RVA = r.u32(o+96+8*i, "data directory")
		f.Dirs[i].Size = r.u32(o+100+8*i, "data directory")
	}

	if h.SectionCount > maxSections {
		return nil, formatError("section count %d out of range 0 to %d", h.SectionCount, maxSections)
	}
	st := o + int(h.OptSize)
	f.Sections = make([]Section, h.SectionCount)
	for i := range f.Sections {
		so := st + 40*i
		s := &f.Sections[i]
		name := r.bytes(so, 8, "section header")
		if n := bytes.IndexByte(name, 0); n >= 0 {
			name = name[:n]
		}
		s.Name = string(name)
		s.VirtualSize = r.u32(so+8, "section header")
		s.RVA = r.u32(so+12, "section header")
		s.RawSize = r.u32(so+16, "section header")
		s.RawOffset = r.u32(so+20, "section header")
		s.RelocOffset = r.u32(so+24, "section header")
		s.LineOffset = r.u32(so+28, "section header")
		s.RelocCount = r.u16(so+32, "section header")
		s.LineCount = r.u16(so+34, "section header")
		s.Characteristics = r.u32(so+36, "section header")
		if s.RawOffset != 0 {
			s.Data = r.bytes(int(s.RawOffset), int(s.RawSize), "section "+s.Name+" data")
		}
	}
	if r.err != nil {
		return nil, r.err
	}

	if err := f.parseImports(r); err != nil {
		return nil, err
	}
	if err := f.parseRelocs(r); err != nil {
		return nil, err
	}
	if err := f.parseResources(r); err != nil {
		return nil, err
	}
	return f, nil
}

// Overlay returns the file offset past the headers and the data of every
// section, where data that the loader leaves unmapped begins.
func (f *File) Overlay() int {
	end := int(f.Opt.HeadersSize)
	for i := range f.Sections {
		s := &f.Sections[i]
		if s.Data != nil {
			end = max(end, int(s.RawOffset)+len(s.Data))
		}
	}
	return min(end, len(f.Data))
}

// VA returns the virtual address of rva: ImageBase plus rva.
func (f *File) VA(rva uint32) uint32 { return f.Opt.ImageBase + rva }

// Dir returns data directory i, or the zero Dir when the file has fewer.
func (f *File) Dir(i int) Dir {
	if i >= len(f.Dirs) {
		return Dir{}
	}
	return f.Dirs[i]
}

// Section returns the section that maps rva, or nil.
func (f *File) Section(rva uint32) *Section {
	for i := range f.Sections {
		s := &f.Sections[i]
		if rva >= s.RVA && rva-s.RVA < s.Size() {
			return s
		}
	}
	return nil
}

// Bytes returns the n bytes of file data that the loader maps at rva, or
// nil when they run past the data of the section holding rva.
func (f *File) Bytes(rva uint32, n int) []byte {
	s := f.Section(rva)
	if s == nil || n < 0 {
		return nil
	}
	off := uint64(rva - s.RVA)
	if off+uint64(n) > uint64(len(s.Data)) {
		return nil
	}
	return s.Data[off : off+uint64(n)]
}

// Word returns the 32-bit word that the loader maps at rva, as Bytes
// finds it.
func (f *File) Word(rva uint32) (uint32, bool) {
	b := f.Bytes(rva, 4)
	if b == nil {
		return 0, false
	}
	return binary.LittleEndian.Uint32(b), true
}

// offset returns the file offset of the n bytes at rva, or an error naming
// what lies there when the bytes run past their section's data.
func (f *File) offset(rva uint32, n int, what string) (int, error) {
	s := f.Section(rva)
	if s == nil || s.Data == nil {
		return 0, formatError("%s at RVA %#x outside the sections' data", what, rva)
	}
	if uint64(rva-s.RVA)+uint64(n) > uint64(len(s.Data)) {
		return 0, formatError("%s at RVA %#x: %d bytes past end of section %s", what, rva, n, s.Name)
	}
	return int(s.RawOffset) + int(rva-s.RVA), nil
}

// cstring returns the NUL-terminated string at rva.
func (f *File) cstring(rva uint32, what string) (string, error) {
	off, err := f.offset(rva, 1, what)
	if err != nil {
		return "", err
	}
	s := f.Section(rva)
	rest := s.Data[rva-s.RVA:]
	n := bytes.IndexByte(rest, 0)
	if n < 0 {
		return "", formatError("%s at %#x: unterminated", what, off)
	}
	return string(rest[:n]), nil
}

func (f *File) parseImports(r *reader) error {
	d := f.Dir(ImportDir)
	if d.RVA == 0 {
		return nil
	}
	// A function's entry in a lookup table takes 4 bytes of the file,
	// and a name takes bytes of its own, so a file of n bytes holds at
	// most n/4 functions and n bytes of names. A walk that finds more
	// reads shared entries or names: descriptors that share a table,
	// tables that overlap, or entries that point at one name, which
	// multiply the functions and their names.
	limit, funcs, names := len(f.Data)/4, 0, 0
	readName := func(rva uint32, what string) (string, error) {
		s, err := f.cstring(rva, what)
		names += len(s) + 1
		if err == nil && names > len(f.Data) {
			err = formatError("import names take more than the file's %d bytes", len(f.Data))
		}
		return s, err
	}
	for i := uint32(0); ; i++ {
		off, err := f.offset(d.RVA+20*i, 20, "import descriptor")
		if err != nil {
			return err
		}
		im := Import{
			LookupRVA:      r.u32(off, "import descriptor"),
			TimeDateStamp:  r.u32(off+4, "import descriptor"),
			ForwarderChain: r.u32(off+8, "import descriptor"),
			IATRVA:         r.u32(off+16, "import descriptor"),
		}
		name := r.u32(off+12, "import descriptor")
		if r.err != nil {
			return r.err
		}
		if im.LookupRVA == 0 && im.TimeDateStamp == 0 && im.ForwarderChain == 0 && name == 0 && im.IATRVA == 0 {
			return nil
		}
		if im.DLL, err = readName(name, "DLL name"); err != nil {
			return err
		}
		table := im.LookupRVA
		if table == 0 {
			table = im.IATRVA
		}
		for j := uint32(0); ; j++ {
			off, err := f.offset(table+4*j, 4, im.DLL+" lookup table")
			if err != nil {
				return err
			}
			e := r.u32(off, "lookup table")
			if e == 0 {
				break
			}
			funcs++
			if funcs > limit {
				return formatError("import tables name more than %d functions, one for each 4 bytes of the file", limit)
			}
			fn := Func{Slot: im.IATRVA + 4*j}
			if e&0x80000000 != 0 {
				fn.Ordinal = uint16(e)
			} else {
				hoff, err := f.offset(e, 2, im.DLL+" hint")
				if err != nil {
					return err
				}
				fn.Hint = r.u16(hoff, "hint")
				if fn.Name, err = readName(e+2, im.DLL+" function name"); err != nil {
					return err
				}
			}
			im.Funcs = append(im.Funcs, fn)
		}
		f.Imports = append(f.Imports, im)
	}
}

func (f *File) parseRelocs(r *reader) error {
	d := f.Dir(RelocDir)
	if d.RVA == 0 {
		return nil
	}
	for p := uint32(0); p < d.Size; {
		off, err := f.offset(d.RVA+p, 8, "relocation block")
		if err != nil {
			return err
		}
		page := r.u32(off, "relocation block")
		size := r.u32(off+4, "relocation block")
		if r.err != nil {
			return r.err
		}
		if size < 8 || size%2 != 0 || size > d.Size-p {
			return formatError("relocation block at RVA %#x: size %d out of range 8 to %d", d.RVA+p, size, d.Size-p)
		}
		if _, err := f.offset(d.RVA+p, int(size), "relocation block"); err != nil {
			return err
		}
		for k := 8; k < int(size); k += 2 {
			e := r.u16(off+k, "relocation")
			f.Relocs = append(f.Relocs, Reloc{Type: uint8(e >> 12), RVA: page + uint32(e&0xfff)})
		}
		p += size
	}
	return r.err
}

// A resource walk ends with an error once it has visited more entries
// than the directory has room for, one for each 8 bytes, or once the
// names that it has decoded take more bytes than the directory. The
// directory's room is its size, or the bytes that its section holds
// from it when those are fewer. A damaged tree whose tables or names
// are shared thus yields at most one entry for each 8 bytes of its
// section, and names of at most the section's bytes.
type resWalk struct {
	f      *File
	r      *reader
	base   int // file offset of the resource directory
	size   int // the directory's room
	visits int
	names  int // bytes of the names decoded
}

func (f *File) parseResources(r *reader) error {
	d := f.Dir(ResourceDir)
	if d.RVA == 0 {
		return nil
	}
	base, err := f.offset(d.RVA, 16, "resource directory")
	if err != nil {
		return err
	}
	s := f.Section(d.RVA)
	room := uint32(len(s.Data)) - (d.RVA - s.RVA)
	w := &resWalk{f: f, r: r, base: base, size: int(min(d.Size, room))}
	return w.dir(0, nil)
}

// dir walks the directory at off, relative to the resource directory,
// whose entries path leads to: on level len(path), 0 for types, 1 for
// names, and 2 for languages.
func (w *resWalk) dir(off uint32, path []ResID) error {
	r := w.r
	at := w.base + int(off)
	stamp := r.u32(at+4, "resource directory")
	n := int(r.u16(at+12, "resource directory")) + int(r.u16(at+14, "resource directory"))
	if r.err != nil {
		return r.err
	}
	w.f.ResDirs = append(w.f.ResDirs, ResDir{Offset: at, TimeDateStamp: stamp})
	for i := range n {
		w.visits++
		if w.visits > w.size/8 {
			return formatError("resource tree visits more than %d entries", w.size/8)
		}
		e := at + 16 + 8*i
		nameField := r.u32(e, "resource entry")
		target := r.u32(e+4, "resource entry")
		if r.err != nil {
			return r.err
		}
		id := ResID{ID: nameField}
		if nameField&0x80000000 != 0 {
			so := w.base + int(nameField&0x7fffffff)
			units := int(r.u16(so, "resource name"))
			w.names += 2 + 2*units
			if w.names > w.size {
				return formatError("resource names take more than the directory's %d bytes", w.size)
			}
			b := r.bytes(so+2, 2*units, "resource name")
			if r.err != nil {
				return r.err
			}
			u := make([]uint16, units)
			for k := range u {
				u[k] = binary.LittleEndian.Uint16(b[2*k:])
			}
			id = ResID{Name: string(utf16.Decode(u))}
		}
		p := append(path[:len(path):len(path)], id)
		if target&0x80000000 != 0 {
			if len(p) >= 3 {
				return formatError("resource directory at %#x deeper than 3 levels", w.base+int(target&0x7fffffff))
			}
			if err := w.dir(target&0x7fffffff, p); err != nil {
				return err
			}
			continue
		}
		if len(p) != 3 {
			return formatError("resource leaf at %#x on level %d, want 3", w.base+int(target), len(p))
		}
		leaf := w.base + int(target)
		res := Resource{
			Type:     p[0],
			Name:     p[1],
			Lang:     p[2],
			RVA:      r.u32(leaf, "resource leaf"),
			Size:     r.u32(leaf+4, "resource leaf"),
			CodePage: r.u32(leaf+8, "resource leaf"),
		}
		if r.err != nil {
			return r.err
		}
		off, err := w.f.offset(res.RVA, int(res.Size), "resource data")
		if err != nil {
			return err
		}
		res.Data = w.f.Data[off : off+int(res.Size)]
		w.f.Resources = append(w.f.Resources, res)
	}
	return nil
}

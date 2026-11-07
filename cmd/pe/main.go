// Pe prints the structure of a 32-bit Portable Executable file.
//
// Usage:
//
//	pe [-l | -r] file
//
// By default, pe prints the headers and the tables, one item per line,
// with tab-separated fields:
//
//	pe	machine sections time date symoffset symcount optsize characteristics
//	opt	magic linker code data bss entry codebase database imagebase salign falign os image subsystem win32 imagesize headers checksum subsystem dllchars stackreserve stackcommit heapreserve heapcommit loaderflags dirs
//	dir	n va size
//	section	n name va vsize rawsize rawoffset characteristics
//	import	dll lookup time chain iat funcs
//	func	dll slot hint name
//	resource	type name lang va size codepage
//	resdir	offset time date
//
// Addresses are virtual addresses, the image base plus the RVA, in
// eight hex digits. Versions read major.minor, and date is the time in
// UTC. A resdir line gives a table of the resource tree, at its file
// offset, with the time stamped in it. Other numbers are hex with a 0x
// prefix, except counts, indices, hints, and the subsystem, which are
// decimal. A function imported by ordinal shows #n in place of its hint
// and name.
//
// The -l flag prints the regions of the file instead: offset, length, and
// what the region holds (headers, a section, the overlay past the last
// section, or a gap).
//
// The -r flag prints one line per base relocation instead: the site, the
// type, and for a 32-bit address, the target and the section holding it,
// or - for a target outside the sections.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"
	"slices"
	"time"

	"github.com/draxinar/teudogar/internal/pe"
)

var (
	lflag = flag.Bool("l", false, "print the regions of the file")
	rflag = flag.Bool("r", false, "print the base relocations")
)

func usage() {
	fmt.Fprintf(os.Stderr, "usage: pe [-l | -r] file\n")
	os.Exit(2)
}

func main() {
	log.SetFlags(0)
	log.SetPrefix("pe: ")
	flag.Usage = usage
	flag.Parse()
	if flag.NArg() != 1 || *lflag && *rflag {
		usage()
	}
	data, err := os.ReadFile(flag.Arg(0))
	if err != nil {
		log.Fatal(err)
	}
	f, err := pe.Parse(data)
	if err != nil {
		log.Fatalf("%s: %v", flag.Arg(0), err)
	}
	w := bufio.NewWriter(os.Stdout)
	switch {
	case *lflag:
		regions(w, f)
	case *rflag:
		relocs(w, f)
	default:
		tables(w, f)
	}
	if err := w.Flush(); err != nil {
		log.Fatal(err)
	}
}

func tables(w *bufio.Writer, f *pe.File) {
	h := &f.Header
	fmt.Fprintf(w, "pe\t%#x\t%d\t%#x\t%s\t%#x\t%d\t%#x\t%#x\n",
		h.Machine, h.SectionCount, h.TimeDateStamp,
		time.Unix(int64(h.TimeDateStamp), 0).UTC().Format(time.RFC3339),
		h.SymbolOffset, h.SymbolCount, h.OptSize, h.Characteristics)
	o := &f.Opt
	fmt.Fprintf(w, "opt\t%#x\t%d.%d\t%#x\t%#x\t%#x\t%08x\t%08x\t%08x\t%08x\t%#x\t%#x\t%d.%d\t%d.%d\t%d.%d\t%#x\t%#x\t%#x\t%#x\t%d\t%#x\t%#x\t%#x\t%#x\t%#x\t%#x\t%d\n",
		o.Magic, o.LinkerMajor, o.LinkerMinor, o.CodeSize, o.DataSize, o.BSSSize,
		f.VA(o.Entry), f.VA(o.CodeBase), f.VA(o.DataBase), o.ImageBase,
		o.SectionAlign, o.FileAlign, o.OSMajor, o.OSMinor, o.ImageMajor, o.ImageMinor,
		o.SubsystemMajor, o.SubsystemMinor, o.Win32Version, o.ImageSize, o.HeadersSize,
		o.Checksum, o.Subsystem, o.DLLCharacteristics, o.StackReserve, o.StackCommit,
		o.HeapReserve, o.HeapCommit, o.LoaderFlags, o.DirCount)
	for i, d := range f.Dirs {
		if d.RVA != 0 || d.Size != 0 {
			fmt.Fprintf(w, "dir\t%d\t%08x\t%#x\n", i, f.VA(d.RVA), d.Size)
		}
	}
	for i := range f.Sections {
		s := &f.Sections[i]
		fmt.Fprintf(w, "section\t%d\t%s\t%08x\t%#x\t%#x\t%#x\t%#x\n",
			i+1, s.Name, f.VA(s.RVA), s.VirtualSize, s.RawSize, s.RawOffset, s.Characteristics)
	}
	for _, im := range f.Imports {
		lookup := uint32(0)
		if im.LookupRVA != 0 {
			lookup = f.VA(im.LookupRVA)
		}
		fmt.Fprintf(w, "import\t%s\t%08x\t%#x\t%#x\t%08x\t%d\n",
			im.DLL, lookup, im.TimeDateStamp, im.ForwarderChain, f.VA(im.IATRVA), len(im.Funcs))
		for _, fn := range im.Funcs {
			if fn.Name == "" {
				fmt.Fprintf(w, "func\t%s\t%08x\t#%d\n", im.DLL, f.VA(fn.Slot), fn.Ordinal)
				continue
			}
			fmt.Fprintf(w, "func\t%s\t%08x\t%d\t%s\n", im.DLL, f.VA(fn.Slot), fn.Hint, fn.Name)
		}
	}
	for _, r := range f.Resources {
		fmt.Fprintf(w, "resource\t%s\t%s\t%s\t%08x\t%#x\t%d\n",
			pe.TypeName(r.Type), r.Name, r.Lang, f.VA(r.RVA), r.Size, r.CodePage)
	}
	for _, d := range f.ResDirs {
		fmt.Fprintf(w, "resdir\t%#x\t%#x\t%s\n", d.Offset, d.TimeDateStamp,
			time.Unix(int64(d.TimeDateStamp), 0).UTC().Format(time.RFC3339))
	}
}

func relocs(w *bufio.Writer, f *pe.File) {
	for _, r := range f.Relocs {
		switch r.Type {
		case pe.Absolute:
			fmt.Fprintf(w, "%08x\tabsolute\n", f.VA(r.RVA))
		case pe.HighLow:
			target, ok := f.Word(r.RVA)
			if !ok {
				fmt.Fprintf(w, "%08x\thighlow\t-\t-\n", f.VA(r.RVA))
				continue
			}
			name := "-"
			if s := f.Section(target - f.Opt.ImageBase); s != nil {
				name = s.Name
			}
			fmt.Fprintf(w, "%08x\thighlow\t%08x\t%s\n", f.VA(r.RVA), target, name)
		default:
			fmt.Fprintf(w, "%08x\t%d\n", f.VA(r.RVA), r.Type)
		}
	}
}

// A region is a run of bytes of the file.
type region struct {
	off, n int
	what   string
}

func regions(w *bufio.Writer, f *pe.File) {
	rs := []region{{0, int(f.Opt.HeadersSize), "headers"}}
	for i := range f.Sections {
		s := &f.Sections[i]
		if s.Data != nil {
			rs = append(rs, region{int(s.RawOffset), len(s.Data), "section " + s.Name})
		}
	}
	if end := f.Overlay(); end < len(f.Data) {
		rs = append(rs, region{end, len(f.Data) - end, "overlay"})
	}
	slices.SortStableFunc(rs, func(a, b region) int { return a.off - b.off })
	at := 0
	for _, r := range rs {
		if r.off > at {
			fmt.Fprintf(w, "%#x\t%#x\tgap\n", at, r.off-at)
		}
		fmt.Fprintf(w, "%#x\t%#x\t%s\n", r.off, r.n, r.what)
		at = max(at, r.off+r.n)
	}
}

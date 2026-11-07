// Wmap prints the symbols of a link map that Watcom's WLINK writes.
//
// Usage:
//
//	wmap [-m] [-f funcs] map
//
// Wmap prints one line per symbol of the map's memory map, in the map's
// order, with tab-separated fields:
//
//	address class module name declaration
//
// The class is that of the segment the address lies in (CODE, DATA,
// BSS, or - outside every segment), and the module is the object, or the
// library and its member, that defines the symbol. The declaration is
// the map's text: a C name as the linker sees it (adler32_), or the
// declaration that WLINK demangles from a C++ name (void near
// logf( char near *, ... )). The name is the C name, or the declared
// identifier, with its class for a member (MTRand::real).
//
// The -f flag names a file of the original program's functions, one per
// line: an address in hex and a module as the map names it (map.obj).
// wmap then gives the code symbols of each module that the file lists
// the addresses it lists: the symbols at the module's k-th code address
// in the map take the k-th address listed for the module. It drops the
// code symbols of the modules it leaves unlisted that lie past the first
// listed module, since the map tells nothing of where they lie in the
// original. A program whose functions differ in size from the
// original's still names the original's functions this way.
//
// The -m flag prints the modules instead, one per line in link order,
// for each module that defines code:
//
//	start end module
//
// The code of a module runs from its first symbol of class CODE to the
// first symbol of class CODE of the next module, or to the end of the
// segment for the last module. A map that WLINK writes under option
// statics names the static functions and tables too, marked s, which
// brings each module's start to its first code but the code that only
// the compiler names, such as a static initializer.
package main

import (
	"bufio"
	"cmp"
	"flag"
	"fmt"
	"io"
	"log"
	"os"
	"path"
	"regexp"
	"slices"
	"strconv"
	"strings"
)

var (
	mflag = flag.Bool("m", false, "print the modules and their code")
	fflag = flag.String("f", "", "move the code symbols of the modules that `funcs` lists to its addresses")
)

func usage() {
	fmt.Fprintf(os.Stderr, "usage: wmap [-m] [-f funcs] map\n")
	os.Exit(2)
}

func main() {
	log.SetFlags(0)
	log.SetPrefix("wmap: ")
	flag.Usage = usage
	flag.Parse()
	if flag.NArg() != 1 {
		usage()
	}
	f, err := os.Open(flag.Arg(0))
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()
	segs, syms, err := parse(f)
	if err != nil {
		log.Fatalf("%s: %v", flag.Arg(0), err)
	}
	if *fflag != "" {
		ff, err := os.Open(*fflag)
		if err != nil {
			log.Fatal(err)
		}
		funcs, err := readFuncs(ff)
		ff.Close()
		if err != nil {
			log.Fatalf("%s: %v", *fflag, err)
		}
		if syms, err = place(syms, funcs); err != nil {
			log.Fatalf("%s: %v", *fflag, err)
		}
	}
	w := bufio.NewWriter(os.Stdout)
	if *mflag {
		for _, m := range modules(segs, syms) {
			fmt.Fprintf(w, "%08x\t%08x\t%s\n", m.start, m.end, m.name)
		}
	} else {
		for _, s := range syms {
			fmt.Fprintf(w, "%08x\t%s\t%s\t%s\t%s\n", s.addr, s.class, s.module, s.name, s.decl)
		}
	}
	if err := w.Flush(); err != nil {
		log.Fatal(err)
	}
}

// A segment is a line of the map's segment table.
type segment struct {
	name, class string
	addr, size  uint32
}

// A symbol is a line of the map's memory map.
type symbol struct {
	addr                      uint32
	class, module, name, decl string
}

var (
	symLine = regexp.MustCompile(`^([0-9a-f]{8})[+*s]?\s+(\S.*?)\s*$`)
	segLine = regexp.MustCompile(`^(\S.*?)\s+(\S+)\s+(\S+)\s+([0-9a-f]{8})\s+([0-9a-f]{8})\s*$`)
)

// parse reads a map's segment table and memory map.
func parse(f *os.File) ([]segment, []symbol, error) {
	var (
		segs    []segment
		syms    []symbol
		section string
		module  string
	)
	sc := bufio.NewScanner(f)
	sc.Buffer(make([]byte, 64*1024), 1024*1024)
	for sc.Scan() {
		line := strings.TrimRight(sc.Text(), "\r")
		if t := strings.TrimSpace(line); strings.HasPrefix(t, "|") && strings.HasSuffix(t, "|") {
			section = strings.TrimSpace(strings.Trim(t, "|"))
			continue
		}
		switch section {
		case "Segments":
			if m := segLine.FindStringSubmatch(line); m != nil {
				a, _ := strconv.ParseUint(m[4], 16, 32)
				n, _ := strconv.ParseUint(m[5], 16, 32)
				segs = append(segs, segment{m[1], m[2], uint32(a), uint32(n)})
			}
		case "Memory Map":
			if mod, ok := strings.CutPrefix(line, "Module: "); ok {
				module = moduleName(mod)
				continue
			}
			m := symLine.FindStringSubmatch(line)
			if m == nil {
				continue
			}
			if module == "" {
				return nil, nil, fmt.Errorf("symbol %s before any module", m[2])
			}
			a, _ := strconv.ParseUint(m[1], 16, 32)
			syms = append(syms, symbol{addr: uint32(a), module: module, name: identifier(m[2]), decl: m[2]})
		}
	}
	if err := sc.Err(); err != nil {
		return nil, nil, err
	}
	if len(segs) == 0 {
		return nil, nil, fmt.Errorf("no segment table")
	}
	for i := range syms {
		syms[i].class = classOf(segs, syms[i].addr)
	}
	return segs, syms, nil
}

// A module is the code of one module.
type module struct {
	start, end uint32
	name       string
}

// modules returns the code of each module that defines code, in address
// order.
func modules(segs []segment, syms []symbol) []module {
	var mods []module
	for _, s := range syms {
		if s.class != "CODE" {
			continue
		}
		if n := len(mods); n > 0 && mods[n-1].name == s.module {
			mods[n-1].start = min(mods[n-1].start, s.addr)
			continue
		}
		mods = append(mods, module{start: s.addr, name: s.module})
	}
	slices.SortStableFunc(mods, func(a, b module) int { return cmp.Compare(a.start, b.start) })
	for i := range mods {
		if i+1 < len(mods) {
			mods[i].end = mods[i+1].start
			continue
		}
		for _, sg := range segs {
			if mods[i].start >= sg.addr && mods[i].start < sg.addr+sg.size {
				mods[i].end = sg.addr + sg.size
			}
		}
	}
	return mods
}

// moduleName returns the module of a map's Module line: the object's
// name without the source path that follows it, or the library's name
// with its member.
func moduleName(s string) string {
	name, _, _ := strings.Cut(s, "(")
	if strings.HasSuffix(strings.ToLower(name), ".lib") {
		lib := path.Base(strings.ReplaceAll(name, `\`, "/"))
		_, member, _ := strings.Cut(s, "(")
		return lib + "(" + member
	}
	return name
}

// readFuncs returns the addresses of each module's functions that r
// lists, in its order, one function per line: an address in hex and a
// module.
func readFuncs(r io.Reader) (map[string][]uint32, error) {
	funcs := make(map[string][]uint32)
	sc := bufio.NewScanner(r)
	for n := 1; sc.Scan(); n++ {
		f := strings.Fields(sc.Text())
		if len(f) == 0 {
			continue
		}
		if len(f) != 2 {
			return nil, fmt.Errorf("line %d: want an address and a module", n)
		}
		a, err := strconv.ParseUint(f[0], 16, 32)
		if err != nil {
			return nil, fmt.Errorf("line %d: %q: want an address in hex", n, f[0])
		}
		funcs[f[1]] = append(funcs[f[1]], uint32(a))
	}
	return funcs, sc.Err()
}

// place returns syms with the code symbols of each module of funcs at
// the addresses that funcs lists for it: the symbols at the module's
// k-th code address take its k-th address. It drops the code symbols of
// other modules that lie past the first module of funcs. A module of
// funcs must hold as many code addresses in syms as funcs lists.
func place(syms []symbol, funcs map[string][]uint32) ([]symbol, error) {
	addrs := make(map[string][]uint32)
	first := ^uint32(0)
	for _, s := range syms {
		if _, ok := funcs[s.module]; ok && s.class == "CODE" {
			addrs[s.module] = append(addrs[s.module], s.addr)
			first = min(first, s.addr)
		}
	}
	moved := make(map[string]map[uint32]uint32)
	for m, want := range funcs {
		have := slices.Sorted(slices.Values(addrs[m]))
		have = slices.Compact(have)
		if len(have) != len(want) {
			return nil, fmt.Errorf("%s: %d code addresses in the map, %d listed", m, len(have), len(want))
		}
		moved[m] = make(map[uint32]uint32)
		for k, a := range have {
			moved[m][a] = want[k]
		}
	}
	var out []symbol
	for _, s := range syms {
		if s.class == "CODE" {
			if mv, ok := moved[s.module]; ok {
				s.addr = mv[s.addr]
			} else if s.addr > first {
				continue
			}
		}
		out = append(out, s)
	}
	return out, nil
}

// classOf returns the class of the segment that holds address a, the
// last such segment in the table, or - for none.
func classOf(segs []segment, a uint32) string {
	class := "-"
	for _, s := range segs {
		if a >= s.addr && a < s.addr+s.size {
			class = s.class
		}
	}
	return class
}

// identifier returns the name a declaration declares: decl itself for a
// C name, else the identifier that the declarator holds, with a member's
// class.
func identifier(decl string) string {
	if !strings.ContainsAny(decl, " ([") {
		return decl
	}
	d := decl
	if i := strings.Index(d, "operator"); i >= 0 {
		j := strings.Index(d[i:], "(")
		if j < 0 {
			return strings.TrimSpace(d[i:])
		}
		return strings.TrimSpace(d[i : i+j])
	}
	// A pointer to a function names its identifier inside the first
	// parentheses: void (near * near loghook)().
	if i := strings.Index(d, "("); i >= 0 && strings.Contains(d[:i+1], " (") {
		inner := d[i+1:]
		if j := strings.Index(inner, ")"); j >= 0 {
			d = inner[:j]
		}
	} else if i >= 0 {
		d = d[:i]
	}
	if i := strings.Index(d, "["); i >= 0 {
		d = d[:i]
	}
	f := strings.Fields(d)
	if len(f) == 0 {
		return decl
	}
	return f[len(f)-1]
}

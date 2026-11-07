// Seal makes a feature build's teudogar.exe and teudogar.dat agree with
// themselves as the release's do, by the step that the author's release
// took after the link: the licence bytes, the seal, and the copy word.
//
// Usage:
//
//	seal sold release.syms release.exe game.syms game.exe game.dat
//
// sold lists the runs of licence bytes (src/teudogar/sold.txt), each an
// address and a length, then the run's offset in the object that holds
// it and that object as C declares it. The .syms files are cmd/wmap's
// dumps of the link maps of the original build and of the feature
// build. Seal copies each run from release.exe into game.exe, at the
// same offset in the same object, found by its module and name, which
// game.syms places. Each run must lie inside one object in both maps,
// in the object and at the offset that sold.txt gives it. It then
// writes the seal (seal, npc.cpp) that begingame (mainmenu.cpp) checks:
// the seal's file offset, the CRC-32 of the exe before the seal and
// after it, and the CRC-32 of the seal's fields from its length on.
// Last, it writes into entry 8311 of game.dat the copy word that
// inisys2 (system.cpp) compares with the CRC-32 of the exe, XORed with
// 0x0ffcff3f. It rewrites game.exe and game.dat in place, and checks
// the seal and the copy word before it exits.
package main

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"flag"
	"fmt"
	"hash/crc32"
	"io"
	"log"
	"os"
	"sort"
	"strconv"
	"strings"

	"github.com/draxinar/teudogar/internal/pe"
)

func usage() {
	fmt.Fprintf(os.Stderr, "usage: seal sold release.syms release.exe game.syms game.exe game.dat\n")
	os.Exit(2)
}

func main() {
	log.SetFlags(0)
	log.SetPrefix("seal: ")
	flag.Usage = usage
	flag.Parse()
	if flag.NArg() != 6 {
		usage()
	}
	a := flag.Args()
	runs, err := readRuns(a[0])
	if err != nil {
		log.Fatal(err)
	}
	relSyms, err := readSyms(a[1])
	if err != nil {
		log.Fatal(err)
	}
	rel, err := os.ReadFile(a[2])
	if err != nil {
		log.Fatal(err)
	}
	gameSyms, err := readSyms(a[3])
	if err != nil {
		log.Fatal(err)
	}
	exe, err := os.ReadFile(a[4])
	if err != nil {
		log.Fatal(err)
	}
	dat, err := os.ReadFile(a[5])
	if err != nil {
		log.Fatal(err)
	}
	if err := copyRuns(runs, relSyms, rel, gameSyms, exe); err != nil {
		log.Fatal(err)
	}
	if err := writeSeal(gameSyms, exe); err != nil {
		log.Fatalf("%s: %v", a[4], err)
	}
	if err := writeWord(exe, dat); err != nil {
		log.Fatalf("%s: %v", a[5], err)
	}
	if err := check(gameSyms, exe, dat); err != nil {
		log.Fatal(err)
	}
	if err := replace(a[4], exe); err != nil {
		log.Fatal(err)
	}
	if err := replace(a[5], dat); err != nil {
		log.Fatal(err)
	}
}

// A run is a line of sold.txt: an address and a length, then the run's
// offset in the object that holds it and the object's name.
type run struct {
	addr uint32
	n    int
	off  int
	name string
}

func readRuns(name string) ([]run, error) {
	f, err := os.Open(name)
	if err != nil {
		return nil, err
	}
	defer f.Close()
	var runs []run
	sc := bufio.NewScanner(f)
	for line := 1; sc.Scan(); line++ {
		fs := strings.Fields(sc.Text())
		if len(fs) == 0 {
			continue
		}
		if len(fs) < 5 {
			return nil, fmt.Errorf("%s:%d: want an address, a length, an offset, and a declaration", name, line)
		}
		addr, err1 := strconv.ParseUint(fs[0], 16, 32)
		n, err2 := strconv.Atoi(fs[1])
		off, err3 := strconv.Atoi(fs[2])
		if err1 != nil || err2 != nil || err3 != nil || n <= 0 || off < 0 {
			return nil, fmt.Errorf("%s:%d: %q, want an address in hex, a length, and an offset", name, line, sc.Text())
		}
		runs = append(runs, run{uint32(addr), n, off, strings.TrimSuffix(fs[len(fs)-1], "[]")})
	}
	return runs, sc.Err()
}

// A sym is a data object of a link map, as cmd/wmap prints it: its
// address, its module, and its name, which two modules may share.
type sym struct {
	addr uint32
	mod  string
	name string
}

// syms holds the data objects of a map, sorted by address.
type syms []sym

// readSyms reads cmd/wmap's dump of a map: address, class, module,
// name, and declaration, separated by tabs. It keeps the objects of the
// classes DATA and BSS.
func readSyms(name string) (syms, error) {
	f, err := os.Open(name)
	if err != nil {
		return nil, err
	}
	defer f.Close()
	return parseSyms(f, name)
}

func parseSyms(r io.Reader, name string) (syms, error) {
	var s syms
	sc := bufio.NewScanner(r)
	sc.Buffer(nil, 1<<20)
	for line := 1; sc.Scan(); line++ {
		fs := strings.Split(sc.Text(), "\t")
		if len(fs) < 4 {
			return nil, fmt.Errorf("%s:%d: want address, class, module, and name", name, line)
		}
		if fs[1] != "DATA" && fs[1] != "BSS" {
			continue
		}
		a, err := strconv.ParseUint(fs[0], 16, 32)
		if err != nil {
			return nil, fmt.Errorf("%s:%d: address %q", name, line, fs[0])
		}
		s = append(s, sym{uint32(a), fs[2], fs[3]})
	}
	if err := sc.Err(); err != nil {
		return nil, err
	}
	sort.SliceStable(s, func(i, j int) bool { return s[i].addr < s[j].addr })
	return s, nil
}

// at returns the object that holds the n bytes at the address a: the
// last at or below a, which the next object must leave room for.
func (s syms) at(a uint32, n int) (sym, error) {
	i := sort.Search(len(s), func(i int) bool { return s[i].addr > a })
	if i == 0 {
		return sym{}, fmt.Errorf("no object holds %08x", a)
	}
	if i < len(s) && uint64(a)+uint64(n) > uint64(s[i].addr) {
		return sym{}, fmt.Errorf("%d bytes at %08x run into %s (%s) at %08x", n, a, s[i].name, s[i].mod, s[i].addr)
	}
	return s[i-1], nil
}

// addr returns the address of the object name of the module mod, the
// one of that name in the map when mod is empty.
func (s syms) addr(mod, name string) (uint32, error) {
	var a []uint32
	for _, x := range s {
		if x.name == name && (mod == "" || x.mod == mod) {
			a = append(a, x.addr)
		}
	}
	switch len(a) {
	case 0:
		return 0, fmt.Errorf("%s (%s) missing", name, mod)
	case 1:
		return a[0], nil
	}
	return 0, fmt.Errorf("%s (%s) given %d times", name, mod, len(a))
}

// fileOffset returns the file offset of the n bytes at the address a of
// the program exe.
func fileOffset(exe []byte, a uint32, n int) (int, error) {
	f, err := pe.Parse(exe)
	if err != nil {
		return 0, err
	}
	rva := a - f.Opt.ImageBase
	s := f.Section(rva)
	if s == nil || int(rva-s.RVA)+n > len(s.Data) {
		return 0, fmt.Errorf("%d bytes at %08x outside the sections' data", n, a)
	}
	return int(s.RawOffset) + int(rva-s.RVA), nil
}

// copyRuns copies the runs from the release's exe into the game's, each
// at the same offset in its object.
func copyRuns(runs []run, relSyms syms, rel []byte, gameSyms syms, exe []byte) error {
	for _, r := range runs {
		obj, err := relSyms.at(r.addr, r.n)
		if err != nil {
			return fmt.Errorf("run at %08x: release: %v", r.addr, err)
		}
		if obj.name != r.name || r.addr-obj.addr != uint32(r.off) {
			return fmt.Errorf("run at %08x: the release holds it at %d of %s, and sold.txt at %d of %s", r.addr, r.addr-obj.addr, obj.name, r.off, r.name)
		}
		to, err := gameSyms.addr(obj.mod, obj.name)
		if err != nil {
			return fmt.Errorf("run at %08x: game: %v", r.addr, err)
		}
		to += r.addr - obj.addr
		if g, err := gameSyms.at(to, r.n); err != nil || g.mod != obj.mod || g.name != obj.name {
			return fmt.Errorf("run at %08x: the game's %s (%s) holds it at %08x short of %d bytes", r.addr, obj.name, obj.mod, to, r.n)
		}
		src, err := fileOffset(rel, r.addr, r.n)
		if err != nil {
			return fmt.Errorf("release: %v", err)
		}
		dst, err := fileOffset(exe, to, r.n)
		if err != nil {
			return fmt.Errorf("game: %v", err)
		}
		copy(exe[dst:dst+r.n], rel[src:src+r.n])
	}
	return nil
}

// The seal's layout (Seal, npc.cpp): 8 bytes of magic, then crc, n,
// off, crc1, crc2, f1c, and f20, 32-bit words each.
const (
	sealCRC  = 8
	sealN    = 12
	sealOff  = 16
	sealCRC1 = 20
	sealCRC2 = 24
	sealLen  = 36
)

var sealMagic = []byte{58, 36, 63, 47, 167, 45, 42, 59}

// sealAt returns the file offset of the seal of exe, which syms places.
func sealAt(s syms, exe []byte) (int, error) {
	a, err := s.addr("", "seal")
	if err != nil {
		return 0, err
	}
	o, err := fileOffset(exe, a, sealLen)
	if err != nil {
		return 0, err
	}
	if !bytes.Equal(exe[o:o+len(sealMagic)], sealMagic) {
		return 0, fmt.Errorf("seal at %08x lacks its magic", a)
	}
	if n := binary.LittleEndian.Uint32(exe[o+sealN:]); n != sealLen {
		return 0, fmt.Errorf("seal at %08x gives a length of %d, want %d", a, n, sealLen)
	}
	return o, nil
}

// writeSeal writes the seal's file offset and its three CRCs, as
// begingame (mainmenu.cpp) checks them.
func writeSeal(s syms, exe []byte) error {
	o, err := sealAt(s, exe)
	if err != nil {
		return err
	}
	le := binary.LittleEndian
	le.PutUint32(exe[o+sealOff:], uint32(o))
	le.PutUint32(exe[o+sealCRC1:], crc32.ChecksumIEEE(exe[:o]))
	le.PutUint32(exe[o+sealCRC2:], crc32.ChecksumIEEE(exe[o+sealLen:]))
	le.PutUint32(exe[o+sealCRC:], crc32.ChecksumIEEE(exe[o+sealN:o+sealLen]))
	return nil
}

// The copy word: entry 8311 of teudogar.dat holds 12 bytes, the word,
// and 4 bytes, and inisys2 (system.cpp) compares the word, XORed with
// wordKey, with the CRC-32 of teudogar.exe.
var (
	wordHead = []byte("O4GzP3TlAXo0")
	wordTail = []byte("5Fda")
)

const wordKey = 0x0ffcff3f

// wordAt returns the offset of the copy word in dat: the one place where
// the entry's head and tail lie four bytes apart.
func wordAt(dat []byte) (int, error) {
	at := -1
	for off := 0; ; {
		i := bytes.Index(dat[off:], wordHead)
		if i < 0 {
			break
		}
		w := off + i + len(wordHead)
		if w+4+len(wordTail) <= len(dat) && bytes.Equal(dat[w+4:w+4+len(wordTail)], wordTail) {
			if at >= 0 {
				return 0, fmt.Errorf("two entries 8311, at %#x and %#x", at-len(wordHead), w-len(wordHead))
			}
			at = w
		}
		off += i + 1
	}
	if at < 0 {
		return 0, fmt.Errorf("entry 8311 missing")
	}
	return at, nil
}

// writeWord writes into dat the copy word of exe.
func writeWord(exe, dat []byte) error {
	w, err := wordAt(dat)
	if err != nil {
		return err
	}
	binary.LittleEndian.PutUint32(dat[w:], crc32.ChecksumIEEE(exe)^wordKey)
	return nil
}

// check checks the seal of exe and the copy word of dat, as the game
// checks them.
func check(s syms, exe, dat []byte) error {
	o, err := sealAt(s, exe)
	if err != nil {
		return err
	}
	le := binary.LittleEndian
	switch {
	case le.Uint32(exe[o+sealOff:]) != uint32(o):
		return fmt.Errorf("seal: offset %#x, want %#x", le.Uint32(exe[o+sealOff:]), o)
	case le.Uint32(exe[o+sealCRC1:]) != crc32.ChecksumIEEE(exe[:o]):
		return fmt.Errorf("seal: crc1 differs from the CRC-32 of the exe before the seal")
	case le.Uint32(exe[o+sealCRC2:]) != crc32.ChecksumIEEE(exe[o+sealLen:]):
		return fmt.Errorf("seal: crc2 differs from the CRC-32 of the exe after the seal")
	case le.Uint32(exe[o+sealCRC:]) != crc32.ChecksumIEEE(exe[o+sealN:o+sealLen]):
		return fmt.Errorf("seal: crc differs from the CRC-32 of the seal's fields")
	}
	w, err := wordAt(dat)
	if err != nil {
		return err
	}
	if le.Uint32(dat[w:])^wordKey != crc32.ChecksumIEEE(exe) {
		return fmt.Errorf("copy word differs from the CRC-32 of the exe")
	}
	return nil
}

// replace writes data to the file name through a temporary file, which
// it removes if the write or the rename fails.
func replace(name string, data []byte) error {
	tmp := name + ".tmp"
	err := os.WriteFile(tmp, data, 0o644)
	if err == nil {
		err = os.Rename(tmp, name)
	}
	if err != nil {
		os.Remove(tmp) // the error of the write or the rename is the one to report
	}
	return err
}

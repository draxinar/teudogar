// Xor XORs a range of a PE file's image with a byte, and writes the file
// to standard output.
//
// Usage:
//
//	xor key start end file
//
// Key is a byte in hex, and start and end are virtual addresses in hex,
// as Ghidra shows them: xor changes the bytes from start up to end. The
// range must lie in the raw data of one section.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"
	"strconv"

	"github.com/draxinar/teudogar/internal/pe"
)

func usage() {
	fmt.Fprintf(os.Stderr, "usage: xor key start end file\n")
	os.Exit(2)
}

func main() {
	log.SetFlags(0)
	log.SetPrefix("xor: ")
	flag.Usage = usage
	flag.Parse()
	if flag.NArg() != 4 {
		usage()
	}
	key, err := strconv.ParseUint(flag.Arg(0), 16, 8)
	if err != nil {
		log.Fatalf("key %s: want a byte in hex", flag.Arg(0))
	}
	start, err1 := strconv.ParseUint(flag.Arg(1), 16, 32)
	end, err2 := strconv.ParseUint(flag.Arg(2), 16, 32)
	if err1 != nil || err2 != nil || start > end {
		log.Fatalf("range %s to %s: want two addresses in hex, in order", flag.Arg(1), flag.Arg(2))
	}
	data, err := os.ReadFile(flag.Arg(3))
	if err != nil {
		log.Fatal(err)
	}
	f, err := pe.Parse(data)
	if err != nil {
		log.Fatalf("%s: %v", flag.Arg(3), err)
	}
	if err := xor(data, f, byte(key), uint32(start), uint32(end)); err != nil {
		log.Fatalf("%s: %v", flag.Arg(3), err)
	}
	w := bufio.NewWriter(os.Stdout)
	if _, err := w.Write(data); err != nil {
		log.Fatal(err)
	}
	if err := w.Flush(); err != nil {
		log.Fatal(err)
	}
}

// xor XORs with key the bytes of data, the file f, from virtual address
// start up to end.
func xor(data []byte, f *pe.File, key byte, start, end uint32) error {
	base := f.Opt.ImageBase
	for _, s := range f.Sections {
		lo, hi := base+s.RVA, base+s.RVA+s.RawSize
		if s.Data == nil || start < lo || end > hi {
			continue
		}
		off := int(s.RawOffset) + int(start-lo)
		for i := range int(end - start) {
			data[off+i] ^= key
		}
		return nil
	}
	return fmt.Errorf("range %08x to %08x lies in no section's raw data", start, end)
}

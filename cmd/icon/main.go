// Icon writes the C file that gives the port the game's window icon, the
// largest image of an icon file as 32-bit pixels.
//
// Usage:
//
//	icon file.ico
//
// Icon prints on standard output a C file that defines sysiconw and
// sysiconh, the icon's size, and sysicon, its pixels as 0xAARRGGBB, top
// row first. The picture is the release's, so make writes the file
// below build/, which git ignores.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"

	"github.com/draxinar/teudogar/internal/ico"
)

func main() {
	log.SetFlags(0)
	log.SetPrefix("icon: ")
	flag.Usage = func() {
		fmt.Fprintln(os.Stderr, "usage: icon file.ico")
		os.Exit(2)
	}
	flag.Parse()
	if flag.NArg() != 1 {
		flag.Usage()
	}
	b, err := os.ReadFile(flag.Arg(0))
	if err != nil {
		log.Fatal(err)
	}
	m, err := ico.Decode(b)
	if err != nil {
		log.Fatalf("%s: %v", flag.Arg(0), err)
	}
	w := bufio.NewWriter(os.Stdout)
	fmt.Fprintf(w, "/* The window's icon, from the release, which cmd/icon writes. */\n\n")
	fmt.Fprintf(w, "const int sysiconw = %d;\nconst int sysiconh = %d;\n", m.W, m.H)
	fmt.Fprintf(w, "const unsigned int sysicon[%d] = {\n", len(m.Pix))
	for i, c := range m.Pix {
		fmt.Fprintf(w, "0x%08x,", c)
		if i%8 == 7 {
			fmt.Fprintln(w)
		}
	}
	fmt.Fprintln(w, "};")
	if err := w.Flush(); err != nil {
		log.Fatal(err)
	}
}

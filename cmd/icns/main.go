// Icns writes the icon of a macOS app from a Windows icon file, the
// largest image of the icon file enlarged by whole numbers.
//
// Usage:
//
//	icns file.ico
//
// Icns prints on standard output an .icns file of PNG images 64, 128,
// 256, 512, and 1024 pixels a side, of those that the image fits. Each
// holds the image at the largest whole multiple of its size that fits,
// every pixel a square of pixels, in the middle of a clear square. The
// picture is the release's, so make writes the file below build/, which
// git ignores.
package main

import (
	"bytes"
	"encoding/binary"
	"flag"
	"fmt"
	"image"
	"image/color"
	"image/png"
	"log"
	"os"

	"github.com/draxinar/teudogar/internal/ico"
)

// sizes lists the images of an .icns file: the type of each and its side
// in pixels.
var sizes = []struct {
	typ  string
	side int
}{
	{"icp6", 64},
	{"ic07", 128},
	{"ic08", 256},
	{"ic09", 512},
	{"ic10", 1024},
}

func main() {
	log.SetFlags(0)
	log.SetPrefix("icns: ")
	flag.Usage = func() {
		fmt.Fprintln(os.Stderr, "usage: icns file.ico")
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
	f, err := icns(m)
	if err != nil {
		log.Fatalf("%s: %v", flag.Arg(0), err)
	}
	if _, err := os.Stdout.Write(f); err != nil {
		log.Fatal(err)
	}
}

// icns returns the .icns file of m: its magic and its length, then each
// image as its type, its length, and its PNG data.
func icns(m *ico.Image) ([]byte, error) {
	var body []byte
	for _, s := range sizes {
		if s.side < m.W || s.side < m.H {
			continue
		}
		var p bytes.Buffer
		if err := png.Encode(&p, enlarge(m, s.side)); err != nil {
			return nil, err
		}
		body = append(body, s.typ...)
		body = binary.BigEndian.AppendUint32(body, uint32(8+p.Len()))
		body = append(body, p.Bytes()...)
	}
	f := []byte("icns")
	f = binary.BigEndian.AppendUint32(f, uint32(8+len(body)))
	return append(f, body...), nil
}

// enlarge returns m at the largest whole multiple of its size that fits
// a square of side pixels, in the middle of that square, which is clear
// around it. m must fit the square.
func enlarge(m *ico.Image, side int) image.Image {
	k := min(side/m.W, side/m.H)
	x0 := (side - k*m.W) / 2
	y0 := (side - k*m.H) / 2
	p := image.NewNRGBA(image.Rect(0, 0, side, side))
	for y := range k * m.H {
		for x := range k * m.W {
			c := m.Pix[y/k*m.W+x/k]
			p.SetNRGBA(x0+x, y0+y, color.NRGBA{R: uint8(c >> 16), G: uint8(c >> 8), B: uint8(c), A: uint8(c >> 24)})
		}
	}
	return p
}

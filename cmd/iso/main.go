// Iso extracts the files of an ISO 9660 image.
//
// Usage:
//
//	iso image dir
//
// Iso reads the primary volume descriptor and walks the directories from
// the root, and writes each file into the new directory dir, at its path
// in the image, in lowercase and without its version (;1). It reads the
// directory records themselves, so a record flag that other readers
// refuse, such as the reserved 0x40 on the Watcom C/C++ 11.0B CD, does no
// harm. It skips a damaged directory, and a directory that a record
// names again, each of which it reports. It extracts into dir.tmp and
// renames that to dir at the end.
package main

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"
	"strings"
)

const sectorSize = 2048

// Directory record flags.
const (
	flagDir         = 0x02
	flagMultiExtent = 0x80
)

// maxDepth bounds the walk, as ISO 9660 allows 8 levels.
const maxDepth = 16

func usage() {
	fmt.Fprintf(os.Stderr, "usage: iso image dir\n")
	os.Exit(2)
}

func main() {
	log.SetFlags(0)
	log.SetPrefix("iso: ")
	if len(os.Args) != 3 {
		usage()
	}
	image, dir := os.Args[1], os.Args[2]
	if _, err := os.Stat(dir); err == nil {
		log.Fatalf("%s: already exists", dir)
	}
	f, err := os.Open(image)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()
	tmp := dir + ".tmp"
	if err := os.RemoveAll(tmp); err != nil {
		log.Fatal(err)
	}
	if err := extract(f, tmp); err != nil {
		os.RemoveAll(tmp)
		log.Fatalf("%s: %v", image, err)
	}
	if err := os.Rename(tmp, dir); err != nil {
		log.Fatal(err)
	}
}

// A record is one directory record.
type record struct {
	name   string
	extent int64 // first sector
	length int64
	flags  byte
}

// extract writes the files of the image r into dir.
func extract(r io.ReaderAt, dir string) error {
	pvd := make([]byte, sectorSize)
	if _, err := r.ReadAt(pvd, 16*sectorSize); err != nil {
		return fmt.Errorf("primary volume descriptor: %v", err)
	}
	if pvd[0] != 1 || string(pvd[1:6]) != "CD001" {
		return errors.New("no primary volume descriptor at sector 16")
	}
	root, ok := parseRecord(pvd[156 : 156+34])
	if !ok {
		return errors.New("bad root directory record")
	}
	return walk(r, root, dir, 0, make(map[int64]bool))
}

// parseRecord decodes the directory record at the start of b.
func parseRecord(b []byte) (record, bool) {
	if len(b) < 34 || int(b[0]) > len(b) || int(b[0]) < 34 || 33+int(b[32]) > int(b[0]) {
		return record{}, false
	}
	return record{
		name:   string(b[33 : 33+b[32]]),
		extent: int64(binary.LittleEndian.Uint32(b[2:])),
		length: int64(binary.LittleEndian.Uint32(b[10:])),
		flags:  b[25],
	}, true
}

// walk writes the files of directory d of the image r into dir, and
// the directories below d into directories of dir. walked holds the
// first sectors of the directories walked before, which walk skips.
func walk(r io.ReaderAt, d record, dir string, depth int, walked map[int64]bool) error {
	if depth > maxDepth {
		return fmt.Errorf("%s: directories nest deeper than %d", dir, maxDepth)
	}
	if err := os.MkdirAll(dir, 0o777); err != nil {
		return err
	}
	data, err := io.ReadAll(io.NewSectionReader(r, d.extent*sectorSize, d.length))
	if err != nil {
		return fmt.Errorf("directory %s: %v", dir, err)
	}
	if int64(len(data)) != d.length {
		return fmt.Errorf("directory %s: %d of %d bytes in the image", dir, len(data), d.length)
	}
	// A directory opens with its own record, ".", which points back at
	// its extent. Where it does not, the image is damaged there: the
	// Watcom C/C++ 11.0B CD's samples/win/datactl/win16 points at sector
	// 353, which holds other data. Iso reports the directory and goes on.
	if self, ok := parseRecord(data); !ok || self.name != "\x00" || self.extent != d.extent {
		log.Printf("%s: skipped, no directory at sector %d", dir, d.extent)
		return nil
	}
	if walked[d.extent] {
		log.Printf("%s: skipped, sector %d holds a directory walked before", dir, d.extent)
		return nil
	}
	walked[d.extent] = true
	for i := 0; i < len(data); {
		n := int(data[i])
		if n == 0 {
			// Records never cross a sector: the rest of this one is empty.
			i = (i/sectorSize + 1) * sectorSize
			continue
		}
		rec, ok := parseRecord(data[i:min(i+n, len(data))])
		if !ok {
			return fmt.Errorf("directory %s: bad record at %d", dir, i)
		}
		i += n
		if rec.name == "\x00" || rec.name == "\x01" {
			continue
		}
		name := strings.ToLower(rec.name)
		if k := strings.IndexByte(name, ';'); k >= 0 {
			name = name[:k]
		}
		name = strings.TrimSuffix(name, ".")
		if name == "" || name == "." || name == ".." || strings.ContainsAny(name, "/\\") {
			return fmt.Errorf("directory %s: bad name %q", dir, rec.name)
		}
		path := filepath.Join(dir, name)
		if rec.flags&flagMultiExtent != 0 {
			return fmt.Errorf("%s: a file in several extents", path)
		}
		if rec.flags&flagDir != 0 {
			if err := walk(r, rec, path, depth+1, walked); err != nil {
				return err
			}
			continue
		}
		if err := copyFile(r, rec, path); err != nil {
			return err
		}
	}
	return nil
}

func copyFile(r io.ReaderAt, rec record, path string) error {
	w, err := os.OpenFile(path, os.O_WRONLY|os.O_CREATE|os.O_EXCL, 0o666)
	if err != nil {
		return err
	}
	n, err := io.Copy(w, io.NewSectionReader(r, rec.extent*sectorSize, rec.length))
	if cerr := w.Close(); err == nil {
		err = cerr
	}
	if err == nil && n != rec.length {
		err = fmt.Errorf("%d of %d bytes in the image", n, rec.length)
	}
	if err != nil {
		return fmt.Errorf("%s: %v", path, err)
	}
	return nil
}

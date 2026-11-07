// Pack writes a folder as an archive whose bytes depend only on the
// folder's files and on the time given.
//
// Usage:
//
//	pack -t time dir archive
//
// Pack writes the files of dir, and of the directories below it, into
// archive: a zip file when the name ends in .zip, and a gzipped tar
// file when it ends in .tar.gz or .tgz. Pack refuses any other name.
// The archive holds the files under one directory, named as the archive
// without its suffix, in the order of their names. Each takes the time
// given, as RFC 3339 writes it (2008-05-20T08:23:29Z), the mode 0755
// when its owner may run it and 0644 otherwise, and in a tar file the
// owner 0. Two runs on the same files write the same bytes. Pack writes
// the archive under a temporary name and renames it at the end.
package main

import (
	"archive/tar"
	"archive/zip"
	"compress/gzip"
	"flag"
	"fmt"
	"io"
	"io/fs"
	"log"
	"os"
	"path"
	"path/filepath"
	"slices"
	"strings"
	"time"
)

func usage() {
	fmt.Fprintf(os.Stderr, "usage: pack -t time dir archive\n")
	os.Exit(2)
}

func main() {
	log.SetFlags(0)
	log.SetPrefix("pack: ")
	when := flag.String("t", "", "the time of the archive's files, as RFC 3339 writes it")
	flag.Usage = usage
	flag.Parse()
	if flag.NArg() != 2 || *when == "" {
		usage()
	}
	t, err := time.Parse(time.RFC3339, *when)
	if err != nil {
		log.Fatalf("-t %s: want a time such as 2008-05-20T08:23:29Z", *when)
	}
	if err := pack(flag.Arg(0), flag.Arg(1), t.UTC()); err != nil {
		log.Fatal(err)
	}
}

// A file is a file of the folder: its name below the folder, with
// slashes, and whether its owner may run it.
type file struct {
	name string
	exec bool
}

// files lists the regular files below dir, sorted by name. A file of
// another kind, such as a symbolic link, is an error.
func files(dir string) ([]file, error) {
	var fl []file
	err := filepath.WalkDir(dir, func(p string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}
		if d.IsDir() {
			return nil
		}
		info, err := d.Info()
		if err != nil {
			return err
		}
		if !info.Mode().IsRegular() {
			return fmt.Errorf("%s: not a regular file", p)
		}
		rel, err := filepath.Rel(dir, p)
		if err != nil {
			return err
		}
		fl = append(fl, file{filepath.ToSlash(rel), info.Mode()&0o100 != 0})
		return nil
	})
	slices.SortFunc(fl, func(a, b file) int { return strings.Compare(a.name, b.name) })
	return fl, err
}

// mode returns the mode that an archive gives f.
func (f file) mode() fs.FileMode {
	if f.exec {
		return 0o755
	}
	return 0o644
}

// pack writes the files of dir into the archive name, each with the
// time t.
func pack(dir, name string, t time.Time) (err error) {
	fl, err := files(dir)
	if err != nil {
		return err
	}
	if len(fl) == 0 {
		return fmt.Errorf("%s: no file to pack", dir)
	}
	base := filepath.Base(name)
	write := writeTar
	switch {
	case strings.HasSuffix(base, ".zip"):
		base = strings.TrimSuffix(base, ".zip")
		write = writeZip
	case strings.HasSuffix(base, ".tar.gz"):
		base = strings.TrimSuffix(base, ".tar.gz")
	case strings.HasSuffix(base, ".tgz"):
		base = strings.TrimSuffix(base, ".tgz")
	default:
		return fmt.Errorf("%s: want a name that ends in .zip, .tar.gz, or .tgz", name)
	}
	tmp := name + ".tmp"
	out, err := os.Create(tmp)
	if err != nil {
		return err
	}
	defer func() {
		if err != nil {
			out.Close()
			os.Remove(tmp) // the error of the write is the one to report
		}
	}()
	if err := write(out, dir, base, fl, t); err != nil {
		return err
	}
	if err := out.Close(); err != nil {
		return err
	}
	return os.Rename(tmp, name)
}

// writeTar writes the files as a gzipped tar file.
func writeTar(w io.Writer, dir, base string, fl []file, t time.Time) error {
	zw, err := gzip.NewWriterLevel(w, gzip.BestCompression)
	if err != nil {
		return err
	}
	tw := tar.NewWriter(zw)
	for _, f := range fl {
		err := copyFile(dir, f, func(size int64) (io.Writer, error) {
			h := &tar.Header{
				Typeflag: tar.TypeReg,
				Name:     path.Join(base, f.name),
				Mode:     int64(f.mode()),
				Size:     size,
				ModTime:  t,
			}
			return tw, tw.WriteHeader(h)
		})
		if err != nil {
			return err
		}
	}
	if err := tw.Close(); err != nil {
		return err
	}
	return zw.Close()
}

// writeZip writes the files as a zip file.
func writeZip(w io.Writer, dir, base string, fl []file, t time.Time) error {
	zw := zip.NewWriter(w)
	for _, f := range fl {
		err := copyFile(dir, f, func(int64) (io.Writer, error) {
			h := &zip.FileHeader{
				Name:     path.Join(base, f.name),
				Method:   zip.Deflate,
				Modified: t,
			}
			h.SetMode(f.mode())
			return zw.CreateHeader(h)
		})
		if err != nil {
			return err
		}
	}
	return zw.Close()
}

// copyFile copies the file f of dir to the writer that begin returns for
// its size, and fails when the file's size changes on the way.
func copyFile(dir string, f file, begin func(size int64) (io.Writer, error)) error {
	in, err := os.Open(filepath.Join(dir, filepath.FromSlash(f.name)))
	if err != nil {
		return err
	}
	defer in.Close()
	info, err := in.Stat()
	if err != nil {
		return err
	}
	w, err := begin(info.Size())
	if err != nil {
		return fmt.Errorf("%s: %v", f.name, err)
	}
	n, err := io.Copy(w, in)
	if err != nil {
		return fmt.Errorf("%s: %v", f.name, err)
	}
	if n != info.Size() {
		return fmt.Errorf("%s: read %d bytes of a file of %d", f.name, n, info.Size())
	}
	return nil
}

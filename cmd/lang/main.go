// Lang puts the translations and the errata of lang/ into the game's
// files, or writes the release's texts in the forms that lang/ keeps.
//
// Usage:
//
//	lang -o dir [-f features] [-p] [-r rooms] release src lang
//	lang -x dir release src
//	lang -s release lang
//
// With -o, lang writes the data of a feature build into dir: dir/data
// holds the files of the program's folder that the features change, and
// dir/include the C headers of src/feature: tables.h, texts.h, gram.h,
// and font.h. features lists the features by name, separated by blanks,
// as make lists them. lang checks each feature's sources in lang before
// it writes, and with -p it takes the release's English for the texts
// that a translation still lacks. rooms (lang/buffers.txt) gives the
// room that the buffers of the game hold for the composed strings that
// run longer than the release's (ReadRooms). The second line of each
// header records the run by its flags but -o, so that the headers hold
// the same bytes wherever lang writes them.
//
// With -x, lang writes into dir the release's German, English, and
// Italian texts in their source forms, as the translators' reference:
// dir/<code>/<name>.txt for each text (strings, manual, encyclopedia1,
// encyclopedia2, script, walkthrough), readme.txt, and texts.txt, the
// language's strings of the program's texts by offset, and
// dir/texts.txt, which lists each group of pick in the program's texts
// by its French slot.
//
// With -s, lang prints the words of lang/fr that a spelling checker
// reads (lang.Prose): the French texts without the game's marks and
// terms, less the lines and strings that read as the release's German.
//
// release is the release's folder (build/zip/teudogar), src the game's
// source (src/teudogar), whose calls of pick lang reads, and lang the
// folder of the translations.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"slices"
	"strings"

	"github.com/draxinar/teudogar/internal/lang"
)

func usage() {
	fmt.Fprintf(os.Stderr, "usage: lang -o dir [-f features] [-p] [-r rooms] release src lang\n")
	fmt.Fprintf(os.Stderr, "       lang -x dir release src\n")
	fmt.Fprintf(os.Stderr, "       lang -s release lang\n")
	os.Exit(2)
}

func main() {
	log.SetFlags(0)
	out := flag.String("o", "", "write a feature build's data into `dir`")
	dump := flag.String("x", "", "write the release's texts into `dir`")
	list := flag.String("f", "", "the `features` of the build")
	partial := flag.Bool("p", false, "take the English for the texts a translation lacks")
	rooms := flag.String("r", "", "read the room of composed strings from `file` (lang/buffers.txt)")
	spell := flag.Bool("s", false, "print the words of lang/fr for a spelling checker")
	flag.Usage = usage
	flag.Parse()
	switch {
	case *spell && *out == "" && *dump == "" && flag.NArg() == 2:
		r, err := lang.OpenRelease(flag.Arg(0))
		if err != nil {
			log.Fatal(err)
		}
		if err := prose(r, flag.Arg(1)); err != nil {
			log.Fatal(err)
		}
	case *dump != "" && *out == "" && flag.NArg() == 2:
		r, picks := open(flag.Arg(0), flag.Arg(1))
		if err := lang.Dump(r, *dump, picks); err != nil {
			log.Fatal(err)
		}
	case *out != "" && *dump == "" && flag.NArg() == 3:
		r, picks := open(flag.Arg(0), flag.Arg(1))
		names := strings.Fields(*list)
		b, err := lang.NewBuild(r)
		if err != nil {
			log.Fatal(err)
		}
		sites, err := lang.ScanComposed(flag.Arg(1))
		if err != nil {
			log.Fatal(err)
		}
		b.Composed = sites
		if b.FontW, err = lang.ReadFontWidths(flag.Arg(1)); err != nil {
			log.Fatal(err)
		}
		if *rooms != "" {
			f, err := os.Open(*rooms)
			if err != nil {
				log.Fatal(err)
			}
			b.Room, err = lang.ReadRooms(f)
			f.Close()
			if err != nil {
				log.Fatalf("%s: %v", *rooms, err)
			}
		}
		var problems []lang.Problem
		for i, name := range names {
			if slices.Contains(names[:i], name) {
				log.Fatalf("lang: feature %s given twice", name)
			}
			ps, err := lang.Apply(name, b, picks, flag.Arg(2), *partial)
			if err != nil {
				log.Fatal(err)
			}
			problems = append(problems, ps...)
		}
		for _, p := range problems {
			fmt.Fprintf(os.Stderr, "lang: %v\n", p)
		}
		if len(problems) > 0 {
			os.Exit(1)
		}
		if err := write(b, *out, record(flag.CommandLine)); err != nil {
			log.Fatal(err)
		}
	default:
		usage()
	}
}

// record returns the command line that the headers record: lang with
// the flags that fs was given, but -o, in the order of their names.
func record(fs *flag.FlagSet) string {
	args := []string{"lang"}
	fs.Visit(func(f *flag.Flag) {
		switch f.Name {
		case "o":
		case "p":
			args = append(args, "-p")
		default:
			args = append(args, "-"+f.Name, f.Value.String())
		}
	})
	return quote(args)
}

// quote returns args as a command line for sh.
func quote(args []string) string {
	q := make([]string, len(args))
	for i, a := range args {
		q[i] = a
		if a == "" || strings.ContainsAny(a, " \t'\"\\$*?[]{}()<>|&;`#~") {
			q[i] = "'" + strings.ReplaceAll(a, "'", `'\''`) + "'"
		}
	}
	return strings.Join(q, " ")
}

// prose prints the words of the French of the folder dir (lang/) that
// a spelling checker reads, against the release r's German.
func prose(r *lang.Release, dir string) error {
	fr := filepath.Join(dir, lang.Codes[lang.French])
	w := bufio.NewWriter(os.Stdout)
	for _, tx := range lang.Texts {
		s, err := os.ReadFile(filepath.Join(fr, tx.Name+".txt"))
		if err != nil {
			return err
		}
		d, err := r.Decoded(tx.ID + lang.German)
		if err != nil {
			return err
		}
		de, err := lang.Source(tx.Kind, d)
		if err != nil {
			return err
		}
		w.WriteString(lang.Prose(tx.Kind, de, string(s)))
	}
	f, err := os.Open(filepath.Join(fr, "texts.txt"))
	if err != nil {
		return err
	}
	lines, err := lang.ReadTexts(f)
	f.Close()
	if err != nil {
		return fmt.Errorf("%s: %v", f.Name(), err)
	}
	for _, l := range lines {
		w.WriteString(lang.Prose(lang.Doc, "", l.Text))
	}
	s, err := os.ReadFile(filepath.Join(fr, "readme.txt"))
	if err != nil {
		return err
	}
	w.WriteString(lang.Prose(lang.Doc, "", string(s)))
	return w.Flush()
}

// open reads the release and the calls of pick in the source.
func open(release, src string) (*lang.Release, []lang.Pick) {
	r, err := lang.OpenRelease(release)
	if err != nil {
		log.Fatal(err)
	}
	picks, err := lang.ScanPicks(src)
	if err != nil {
		log.Fatal(err)
	}
	return r, picks
}

// write writes the build into dir: the changed files into dir/data and
// the headers into dir/include.
func write(b *lang.Build, dir, cmd string) error {
	data := filepath.Join(dir, "data")
	if err := os.MkdirAll(data, 0o755); err != nil {
		return err
	}
	for _, name := range b.Changed() {
		d, err := b.FileByName(name)
		if err != nil {
			return err
		}
		if err := os.WriteFile(filepath.Join(data, name), d, 0o644); err != nil {
			return err
		}
	}
	return b.WriteHeaders(filepath.Join(dir, "include"), cmd)
}

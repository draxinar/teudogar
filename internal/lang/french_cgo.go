//go:build cgo

package lang

/*
#include <stdlib.h>
#include <string.h>

void frelide(char *s);
void frposs(char *s, int size);
void frsentcase(char *s);
void frtitle(char *s);
*/
import "C"

import (
	"bytes"
	"errors"
	"unsafe"
)

// The functions below return s, a text in code page 1252, as the C of
// src/feature/french.c leaves it: the game runs the same code over the
// French texts that it composes.

// Elide returns s as frelide leaves it, with le, la, de, and que elided
// before a vowel or a mute h.
func Elide(s []byte) ([]byte, error) {
	return run(s, 0, func(p *C.char, _ C.int) { C.frelide(p) })
}

// Possessive returns s as frposs leaves it in a buffer of size bytes,
// with ton for a ta that starts it before a vowel or a mute h.
func Possessive(s []byte, size int) ([]byte, error) {
	if len(s) >= size {
		return nil, errors.New("lang: text overflows its buffer")
	}
	return run(s, size-len(s)-1, func(p *C.char, n C.int) { C.frposs(p, n) })
}

// SentCase returns s as frsentcase leaves it, elided, with a capital
// at the start of each sentence.
func SentCase(s []byte) ([]byte, error) {
	return run(s, 0, func(p *C.char, _ C.int) { C.frsentcase(p) })
}

// Title returns s as frtitle leaves it, elided, with a capital at its
// start.
func Title(s []byte) ([]byte, error) {
	return run(s, 0, func(p *C.char, _ C.int) { C.frtitle(p) })
}

// run copies s into a C buffer of len(s)+1+extra bytes, calls f with
// the buffer and its size, and returns the string that f leaves there.
func run(s []byte, extra int, f func(*C.char, C.int)) ([]byte, error) {
	if bytes.IndexByte(s, 0) >= 0 {
		return nil, errors.New("lang: text holds a NUL")
	}
	n := len(s) + 1 + extra
	p := C.malloc(C.size_t(n))
	defer C.free(p)
	buf := unsafe.Slice((*byte)(p), n)
	clear(buf)
	copy(buf, s)
	f((*C.char)(p), C.int(n))
	return C.GoBytes(p, C.int(C.strlen((*C.char)(p)))), nil
}

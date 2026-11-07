//go:build !cgo

package lang

// The C of src/feature/french.c builds through cgo. In a build with
// cgo off, the functions below return ErrNoCgo.

// Elide returns ErrNoCgo.
func Elide(s []byte) ([]byte, error) {
	return nil, ErrNoCgo
}

// Possessive returns ErrNoCgo.
func Possessive(s []byte, size int) ([]byte, error) {
	return nil, ErrNoCgo
}

// SentCase returns ErrNoCgo.
func SentCase(s []byte) ([]byte, error) {
	return nil, ErrNoCgo
}

// Title returns ErrNoCgo.
func Title(s []byte) ([]byte, error) {
	return nil, ErrNoCgo
}

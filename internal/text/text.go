// Package text reads and writes the texts of the game's data files: the
// strings of entries 5003, 5004, and 5007, the documents (the manual,
// the encyclopedia, and the walkthrough), and the dialog script of
// entries 8205, 8206, and 8209, with the demo's copies of the last two
// in Teudogar_00.Dat. Each function takes or returns the decoded bytes
// of one entry, the zlib stream inflated.
//
// The texts use Windows code page 1252 (e4 is ä, 80 is €), and their
// lines end in CR LF. The strings and the script are stored with each
// byte XORed with [Key], and the documents as they are. The types keep
// the bytes as the entries hold them, inline marks included.
package text

import (
	"errors"
	"fmt"
)

// Key is the byte that loadstrs (004a815c, lang.cpp) and loadscript
// (00419a38, dialog.cpp) XOR each byte of their entries with.
const Key = 0xfe

// ErrFormat reports bytes that are not a well-formed entry. Errors from
// DecodeStrings, DecodeDoc, and DecodeScript wrap it.
var ErrFormat = errors.New("text: bad format")

func formatError(format string, args ...any) error {
	return fmt.Errorf("%w: %s", ErrFormat, fmt.Sprintf(format, args...))
}

// xor returns a copy of b with each byte XORed with Key.
func xor(b []byte) []byte {
	t := make([]byte, len(b))
	for i, c := range b {
		t[i] = c ^ Key
	}
	return t
}

/*
 * The prelude of the game's files in the port. make compiles each
 * module of src/base and src/teudogar as C99 with -include of this
 * file, before the module's own includes.
 *
 * It includes every header the modules include, so that each later
 * #include finds its guard set, then gives the names of the game the
 * meaning Watcom gave them:
 * - The game's logf takes a name of its own: C99 gives logf to libm,
 *   and the compilers know it as a built-in.
 * - __cdecl, __stdcall, and EXTERN_C expand to empty: C has one
 *   calling convention and one linkage.
 * - _lrotl and _lrotr rotate 32 bits, as Watcom's intrinsics do.
 * - On LP64, long is int, the 32 bits of Watcom's long, which the
 *   game's structs, saved games, checksums, and formats assume.
 */

#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <dsound.h>
#include <shlobj.h>

#define logf baselogf

#undef __cdecl
#define __cdecl
#undef __stdcall
#define __stdcall
#undef EXTERN_C
#define EXTERN_C

#define _lrotl sysrotl
#define _lrotr sysrotr
DWORD _lrotl(DWORD, int);
DWORD _lrotr(DWORD, int);

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

/* fwdcopy copies n bytes from src to dst one at a time, upward, as
 * Watcom's memcpy does when they overlap */
void fwdcopy(void *dst, const void *src, size_t n);

#if LONG_MAX > 0x7fffffffL
#define long int
#endif

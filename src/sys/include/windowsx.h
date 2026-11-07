/*
 * The macros of windowsx.h that snd.cpp names: memory blocks by
 * pointer, which the MIDI player allocates for its file image and its
 * stream buffers.
 */

#ifndef SYS_WINDOWSX_H
#define SYS_WINDOWSX_H

#include <windows.h>

#define GlobalPtrHandle(lp) ((HGLOBAL)GlobalHandle(lp))
#define GlobalAllocPtr(flags, cb) (GlobalLock(GlobalAlloc((flags), (cb))))
#define GlobalReAllocPtr(lp, cbNew, flags) \
	(GlobalUnlock(GlobalPtrHandle(lp)), \
	GlobalLock(GlobalReAlloc(GlobalPtrHandle(lp), (cbNew), (flags))))
#define GlobalFreePtr(lp) \
	(GlobalUnlock(GlobalPtrHandle(lp)), (BOOL)(ULONG_PTR)GlobalFree(GlobalPtrHandle(lp)))

#endif

/*
 * The part of the Win32 API that the game names, for the port: the
 * types, structs, and constants of Watcom C/C++ 11.0's headers, with
 * fixed widths, and the functions that src/sys implements on SDL3.
 * Each function compiles to a symbol of its own, sys followed by its
 * name, apart from the names of mingw-w64's import libraries.
 *
 * Game files and the files of src/sys both include this header, and
 * the pack pragmas give its structs one layout in both, whatever the
 * packing of the file that includes it.
 */

#ifndef SYS_WINDOWS_H
#define SYS_WINDOWS_H

#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#pragma pack(push, 8)

/*
 * DWORD, LONG, and ULONG are Watcom's long types where long has 32
 * bits, so that the game may mix them with long as it does, and int
 * where long has 64, where the game's files read long as int.
 */
#if ULONG_MAX == 0xffffffffUL
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned long ULONG;
#else
typedef unsigned int DWORD;
typedef int LONG;
typedef unsigned int ULONG;
#endif

#ifndef WINAPI
#define WINAPI
#endif
#define APIENTRY
#define CALLBACK
#define PASCAL
#define FAR
#define NEAR
#define CONST const
#define VOID void

typedef int BOOL;
typedef unsigned char BYTE;
typedef uint16_t WORD;
typedef int16_t SHORT;
typedef uint16_t USHORT;
typedef int INT;
typedef unsigned int UINT;
typedef char CHAR;
typedef unsigned char UCHAR;
typedef uint16_t WCHAR;
typedef uint64_t DWORDLONG;
typedef uintptr_t WPARAM;
typedef intptr_t LPARAM;
typedef intptr_t LRESULT;
typedef uintptr_t DWORD_PTR;
typedef uintptr_t UINT_PTR;
typedef uintptr_t ULONG_PTR;
typedef intptr_t INT_PTR;
typedef intptr_t LONG_PTR;
typedef LONG HRESULT;
typedef WORD ATOM;
typedef WORD LANGID;
typedef DWORD LCID;
typedef DWORD COLORREF;
typedef DWORD LCTYPE;

typedef void *LPVOID;
typedef const void *LPCVOID;
typedef char *LPSTR;
typedef const char *LPCSTR;
typedef char *LPTSTR;
typedef const char *LPCTSTR;
typedef WCHAR *LPWSTR;
typedef const WCHAR *LPCWSTR;
typedef BYTE *LPBYTE;
typedef BYTE *PBYTE;
typedef WORD *LPWORD;
typedef DWORD *LPDWORD;
typedef DWORD *PDWORD;
typedef LONG *LPLONG;
typedef LONG *PLONG;
typedef BOOL *LPBOOL;
typedef int *LPINT;

/* Every handle is a void pointer, as in windows.h before STRICT. */
typedef void *HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
typedef HANDLE *LPHANDLE;
typedef HANDLE *PHANDLE;
typedef HANDLE HWND;
typedef HANDLE HDC;
typedef HANDLE HINSTANCE;
typedef HANDLE HMODULE;
typedef HANDLE HKEY;
typedef HANDLE *PHKEY;
typedef HANDLE HICON;
typedef HANDLE HCURSOR;
typedef HANDLE HBRUSH;
typedef HANDLE HFONT;
typedef HANDLE HGDIOBJ;
typedef HANDLE HACCEL;
typedef HANDLE HMENU;
typedef HANDLE HGLOBAL;
typedef HANDLE HLOCAL;
typedef HANDLE HTASK;
typedef HANDLE HPALETTE;
typedef HANDLE HBITMAP;

#define TRUE 1
#define FALSE 0

#define MAKEWORD(a, b) ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b) ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l) ((WORD)(l))
#define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xffff))
#define LOBYTE(w) ((BYTE)(w))
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xff))
#define MAKELPARAM(l, h) ((LPARAM)MAKELONG(l, h))
#define MAKEWPARAM(l, h) ((WPARAM)(DWORD)MAKELONG(l, h))
#define MAKEINTRESOURCE(i) ((LPSTR)(ULONG_PTR)(WORD)(i))
#define RGB(r, g, b) ((COLORREF)(((BYTE)(r) | ((WORD)((BYTE)(g)) << 8)) | \
	(((DWORD)(BYTE)(b)) << 16)))

/* HRESULT */
#define S_OK ((HRESULT)0)
#define S_FALSE ((HRESULT)1)
#define NOERROR 0
#define MAKE_HRESULT(sev, fac, code) \
	((HRESULT)(((DWORD)(sev) << 31) | ((DWORD)(fac) << 16) | ((DWORD)(code))))
#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)
#define FAILED(hr) ((HRESULT)(hr) < 0)
#define HRESULT_CODE(hr) ((hr) & 0xFFFF)
#define HRESULT_FACILITY(hr) (((hr) >> 16) & 0x1fff)
#define HRESULT_SEVERITY(hr) (((hr) >> 31) & 0x1)
#define E_NOTIMPL ((HRESULT)0x80004001)
#define E_FAIL ((HRESULT)0x80004005)
#define E_OUTOFMEMORY ((HRESULT)0x8007000E)
#define E_INVALIDARG ((HRESULT)0x80070057)
#define CLASS_E_NOAGGREGATION ((HRESULT)0x80040110)

/* error codes of GetLastError */
#define ERROR_SUCCESS 0
#define ERROR_FILE_NOT_FOUND 2
#define ERROR_PATH_NOT_FOUND 3
#define ERROR_ACCESS_DENIED 5
#define ERROR_INVALID_HANDLE 6
#define ERROR_NOT_ENOUGH_MEMORY 8
#define ERROR_NO_MORE_FILES 18
#define ERROR_WRITE_FAULT 29
#define ERROR_READ_FAULT 30
#define ERROR_HANDLE_EOF 38
#define ERROR_FILE_EXISTS 80
#define ERROR_INVALID_PARAMETER 87
#define ERROR_DISK_FULL 112
#define ERROR_CALL_NOT_IMPLEMENTED 120
#define ERROR_INSUFFICIENT_BUFFER 122
#define ERROR_ALREADY_EXISTS 183
#define ERROR_MORE_DATA 234

typedef struct GUID GUID;
struct GUID {
	DWORD Data1;
	WORD Data2;
	WORD Data3;
	BYTE Data4[8];
};
typedef GUID IID;
typedef GUID CLSID;
typedef const IID *REFIID;
typedef const CLSID *REFCLSID;

#ifndef EXTERN_C
#define EXTERN_C extern
#endif

typedef struct POINT POINT;
struct POINT {
	LONG x;
	LONG y;
};
typedef POINT *LPPOINT;
typedef POINT *PPOINT;

typedef struct RECT RECT;
struct RECT {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
};
typedef RECT *LPRECT;
typedef RECT *PRECT;
typedef const RECT *LPCRECT;

typedef struct SIZE SIZE;
struct SIZE {
	LONG cx;
	LONG cy;
};
typedef SIZE *LPSIZE;

typedef struct FILETIME FILETIME;
struct FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
};
typedef FILETIME *LPFILETIME;
typedef FILETIME *PFILETIME;

typedef struct SYSTEMTIME SYSTEMTIME;
struct SYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
};
typedef SYSTEMTIME *LPSYSTEMTIME;
typedef SYSTEMTIME *PSYSTEMTIME;

typedef union ULARGE_INTEGER ULARGE_INTEGER;
union ULARGE_INTEGER {
	struct {
		DWORD LowPart;
		DWORD HighPart;
	} u;
	DWORDLONG QuadPart;
};
typedef ULARGE_INTEGER *PULARGE_INTEGER;

typedef struct SECURITY_ATTRIBUTES SECURITY_ATTRIBUTES;
struct SECURITY_ATTRIBUTES {
	DWORD nLength;
	LPVOID lpSecurityDescriptor;
	BOOL bInheritHandle;
};
typedef SECURITY_ATTRIBUTES *LPSECURITY_ATTRIBUTES;

typedef struct OVERLAPPED OVERLAPPED;
typedef OVERLAPPED *LPOVERLAPPED;

/* files */
#define MAX_PATH 260
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)
#define GENERIC_READ 0x80000000
#define GENERIC_WRITE 0x40000000
#define FILE_SHARE_READ 0x00000001
#define FILE_SHARE_WRITE 0x00000002
#define CREATE_NEW 1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define TRUNCATE_EXISTING 5
#define FILE_ATTRIBUTE_READONLY 0x00000001
#define FILE_ATTRIBUTE_HIDDEN 0x00000002
#define FILE_ATTRIBUTE_SYSTEM 0x00000004
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define FILE_ATTRIBUTE_ARCHIVE 0x00000020
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define FILE_FLAG_WRITE_THROUGH 0x80000000
#define MOVEFILE_REPLACE_EXISTING 0x00000001
#define FILE_BEGIN 0
#define FILE_CURRENT 1
#define FILE_END 2

typedef struct WIN32_FIND_DATAA WIN32_FIND_DATAA;
struct WIN32_FIND_DATAA {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	DWORD dwReserved0;
	DWORD dwReserved1;
	CHAR cFileName[MAX_PATH];
	CHAR cAlternateFileName[14];
};
typedef WIN32_FIND_DATAA WIN32_FIND_DATA;
typedef WIN32_FIND_DATAA *LPWIN32_FIND_DATAA;
typedef WIN32_FIND_DATAA *LPWIN32_FIND_DATA;

/* memory */
#define GMEM_FIXED 0x0000
#define GMEM_MOVEABLE 0x0002
#define GMEM_ZEROINIT 0x0040
#define GMEM_DDESHARE 0x2000
#define GMEM_SHARE 0x2000
#define GHND (GMEM_MOVEABLE | GMEM_ZEROINIT)
#define GPTR (GMEM_FIXED | GMEM_ZEROINIT)
#define LMEM_FIXED 0x0000
#define LMEM_MOVEABLE 0x0002
#define LMEM_ZEROINIT 0x0040
#define LHND (LMEM_MOVEABLE | LMEM_ZEROINIT)
#define LPTR (LMEM_FIXED | LMEM_ZEROINIT)

typedef struct MEMORYSTATUS MEMORYSTATUS;
struct MEMORYSTATUS {
	DWORD dwLength;
	DWORD dwMemoryLoad;
	DWORD dwTotalPhys;
	DWORD dwAvailPhys;
	DWORD dwTotalPageFile;
	DWORD dwAvailPageFile;
	DWORD dwTotalVirtual;
	DWORD dwAvailVirtual;
};
typedef MEMORYSTATUS *LPMEMORYSTATUS;

/* the system and its version */
#define VER_PLATFORM_WIN32s 0
#define VER_PLATFORM_WIN32_WINDOWS 1
#define VER_PLATFORM_WIN32_NT 2

typedef struct OSVERSIONINFOA OSVERSIONINFOA;
struct OSVERSIONINFOA {
	DWORD dwOSVersionInfoSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformId;
	CHAR szCSDVersion[128];
};
typedef OSVERSIONINFOA OSVERSIONINFO;
typedef OSVERSIONINFOA *LPOSVERSIONINFOA;
typedef OSVERSIONINFOA *LPOSVERSIONINFO;

typedef struct SYSTEM_INFO SYSTEM_INFO;
struct SYSTEM_INFO {
	WORD wProcessorArchitecture;
	WORD wReserved;
	DWORD dwPageSize;
	LPVOID lpMinimumApplicationAddress;
	LPVOID lpMaximumApplicationAddress;
	DWORD_PTR dwActiveProcessorMask;
	DWORD dwNumberOfProcessors;
	DWORD dwProcessorType;
	DWORD dwAllocationGranularity;
	WORD wProcessorLevel;
	WORD wProcessorRevision;
};
typedef SYSTEM_INFO *LPSYSTEM_INFO;

#define PROCESSOR_INTEL_386 386
#define PROCESSOR_INTEL_486 486
#define PROCESSOR_INTEL_PENTIUM 586

/* languages and locales */
#define LANG_NEUTRAL 0x00
#define LANG_CATALAN 0x03
#define LANG_GERMAN 0x07
#define LANG_ENGLISH 0x09
#define LANG_SPANISH 0x0a
#define LANG_FRENCH 0x0c
#define LANG_ITALIAN 0x10
#define LANG_DUTCH 0x13
#define LANG_POLISH 0x15
#define LANG_PORTUGUESE 0x16
#define LANG_ROMANIAN 0x18
#define LANG_BASQUE 0x2d
#define SUBLANG_NEUTRAL 0x00
#define SUBLANG_DEFAULT 0x01
#define SUBLANG_SYS_DEFAULT 0x02
#define SORT_DEFAULT 0x0
#define MAKELANGID(p, s) ((((WORD)(s)) << 10) | (WORD)(p))
#define PRIMARYLANGID(lgid) ((WORD)(lgid) & 0x3ff)
#define SUBLANGID(lgid) ((WORD)(lgid) >> 10)
#define MAKELCID(lgid, srtid) ((DWORD)((((DWORD)((WORD)(srtid))) << 16) | ((DWORD)((WORD)(lgid)))))
#define LANG_USER_DEFAULT (MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))
#define LOCALE_USER_DEFAULT (MAKELCID(LANG_USER_DEFAULT, SORT_DEFAULT))
#define LOCALE_SENGCOUNTRY 0x00001002
#define CP_ACP 0

/* the registry */
#define HKEY_CLASSES_ROOT ((HKEY)(ULONG_PTR)0x80000000)
#define HKEY_CURRENT_USER ((HKEY)(ULONG_PTR)0x80000001)
#define HKEY_LOCAL_MACHINE ((HKEY)(ULONG_PTR)0x80000002)
#define KEY_QUERY_VALUE 0x0001
#define KEY_SET_VALUE 0x0002
#define KEY_ALL_ACCESS 0xF003F
#define REG_NONE 0
#define REG_SZ 1
#define REG_BINARY 3
#define REG_DWORD 4
#define REG_OPTION_NON_VOLATILE 0x00000000
#define REG_CREATED_NEW_KEY 0x00000001
#define REG_OPENED_EXISTING_KEY 0x00000002
typedef DWORD REGSAM;

/* windows and their messages */
typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef BOOL (*DLGPROC)(HWND, UINT, WPARAM, LPARAM);

typedef struct WNDCLASSA WNDCLASSA;
struct WNDCLASSA {
	UINT style;
	WNDPROC lpfnWndProc;
	int cbClsExtra;
	int cbWndExtra;
	HINSTANCE hInstance;
	HICON hIcon;
	HCURSOR hCursor;
	HBRUSH hbrBackground;
	LPCSTR lpszMenuName;
	LPCSTR lpszClassName;
};
typedef WNDCLASSA WNDCLASS;
typedef WNDCLASSA *LPWNDCLASSA;
typedef WNDCLASSA *LPWNDCLASS;

/* SDL3 declares MSG on Windows, for its hook of messages */
#if !(defined(SDL_system_h_) && defined(SDL_PLATFORM_WINDOWS))
typedef struct tagMSG MSG;
#endif
struct tagMSG {
	HWND hwnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	DWORD time;
	POINT pt;
};
typedef MSG *LPMSG;
typedef MSG *PMSG;

#define CS_VREDRAW 0x0001
#define CS_HREDRAW 0x0002
#define WS_POPUP 0x80000000
#define WS_VISIBLE 0x10000000
#define WS_SYSMENU 0x00080000
#define CW_USEDEFAULT ((int)0x80000000)

#define WM_NULL 0x0000
#define WM_CREATE 0x0001
#define WM_DESTROY 0x0002
#define WM_MOVE 0x0003
#define WM_SIZE 0x0005
#define WM_ACTIVATE 0x0006
#define WM_SETFOCUS 0x0007
#define WM_KILLFOCUS 0x0008
#define WM_PAINT 0x000F
#define WM_CLOSE 0x0010
#define WM_QUIT 0x0012
#define WM_ACTIVATEAPP 0x001C
#define WM_SETCURSOR 0x0020
#define WM_KEYDOWN 0x0100
#define WM_KEYUP 0x0101
#define WM_CHAR 0x0102
#define WM_SYSKEYDOWN 0x0104
#define WM_SYSKEYUP 0x0105
#define WM_SYSCHAR 0x0106
#define WM_INITDIALOG 0x0110
#define WM_COMMAND 0x0111
#define WM_SYSCOMMAND 0x0112
#define WM_MOUSEMOVE 0x0200
#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202
#define WM_LBUTTONDBLCLK 0x0203
#define WM_RBUTTONDOWN 0x0204
#define WM_RBUTTONUP 0x0205
#define WM_RBUTTONDBLCLK 0x0206
#define WM_USER 0x0400

#define SC_SIZE 0xF000
#define SC_MOVE 0xF010
#define SC_MINIMIZE 0xF020
#define SC_MAXIMIZE 0xF030
#define SC_CLOSE 0xF060
#define SC_KEYMENU 0xF100
#define SC_SCREENSAVE 0xF140
#define SC_MONITORPOWER 0xF170

#define PM_NOREMOVE 0x0000
#define PM_REMOVE 0x0001

#define SW_HIDE 0
#define SW_SHOWNORMAL 1
#define SW_NORMAL 1
#define SW_SHOWMINIMIZED 2
#define SW_SHOWMAXIMIZED 3
#define SW_SHOW 5
#define SW_MINIMIZE 6
#define SW_RESTORE 9

#define SWP_NOSIZE 0x0001
#define SWP_NOMOVE 0x0002
#define SWP_NOZORDER 0x0004

#define SM_CXSCREEN 0
#define SM_CYSCREEN 1
#define SM_SWAPBUTTON 23

#define GCL_HICON (-14)

#define IDC_ARROW MAKEINTRESOURCE(32512)
#define IDI_APPLICATION MAKEINTRESOURCE(32512)

#define MK_LBUTTON 0x0001
#define MK_RBUTTON 0x0002

/* message boxes and dialogs */
#define MB_OK 0x00000000
#define MB_OKCANCEL 0x00000001
#define MB_ABORTRETRYIGNORE 0x00000002
#define MB_YESNOCANCEL 0x00000003
#define MB_YESNO 0x00000004
#define MB_RETRYCANCEL 0x00000005
#define MB_ICONHAND 0x00000010
#define MB_ICONERROR MB_ICONHAND
#define MB_ICONSTOP MB_ICONHAND
#define MB_ICONQUESTION 0x00000020
#define MB_ICONEXCLAMATION 0x00000030
#define MB_ICONWARNING MB_ICONEXCLAMATION
#define MB_ICONASTERISK 0x00000040
#define MB_ICONINFORMATION MB_ICONASTERISK
#define MB_DEFBUTTON1 0x00000000
#define MB_DEFBUTTON2 0x00000100
#define MB_DEFBUTTON3 0x00000200
#define MB_SETFOREGROUND 0x00010000
#define MB_TYPEMASK 0x0000000F
#define MB_ICONMASK 0x000000F0
#define MB_DEFMASK 0x00000F00

#define IDOK 1
#define IDCANCEL 2
#define IDABORT 3
#define IDRETRY 4
#define IDIGNORE 5
#define IDYES 6
#define IDNO 7

#define BM_SETSTYLE 0x00F4
#define BS_PUSHBUTTON 0x00000000
#define BS_DEFPUSHBUTTON 0x00000001
#define DM_SETDEFID (WM_USER + 1)
#define PBM_SETRANGE (WM_USER + 1)
#define PBM_SETPOS (WM_USER + 2)

/* keys */
#define VK_LBUTTON 0x01
#define VK_RBUTTON 0x02
#define VK_CANCEL 0x03
#define VK_MBUTTON 0x04
#define VK_BACK 0x08
#define VK_TAB 0x09
#define VK_CLEAR 0x0C
#define VK_RETURN 0x0D
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_PAUSE 0x13
#define VK_CAPITAL 0x14
#define VK_ESCAPE 0x1B
#define VK_SPACE 0x20
#define VK_PRIOR 0x21
#define VK_NEXT 0x22
#define VK_END 0x23
#define VK_HOME 0x24
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_SELECT 0x29
#define VK_PRINT 0x2A
#define VK_SNAPSHOT 0x2C
#define VK_INSERT 0x2D
#define VK_DELETE 0x2E
#define VK_HELP 0x2F
#define VK_LWIN 0x5B
#define VK_RWIN 0x5C
#define VK_APPS 0x5D
#define VK_NUMPAD0 0x60
#define VK_NUMPAD1 0x61
#define VK_NUMPAD2 0x62
#define VK_NUMPAD3 0x63
#define VK_NUMPAD4 0x64
#define VK_NUMPAD5 0x65
#define VK_NUMPAD6 0x66
#define VK_NUMPAD7 0x67
#define VK_NUMPAD8 0x68
#define VK_NUMPAD9 0x69
#define VK_MULTIPLY 0x6A
#define VK_ADD 0x6B
#define VK_SEPARATOR 0x6C
#define VK_SUBTRACT 0x6D
#define VK_DECIMAL 0x6E
#define VK_DIVIDE 0x6F
#define VK_F1 0x70
#define VK_F2 0x71
#define VK_F3 0x72
#define VK_F4 0x73
#define VK_F5 0x74
#define VK_F6 0x75
#define VK_F7 0x76
#define VK_F8 0x77
#define VK_F9 0x78
#define VK_F10 0x79
#define VK_F11 0x7A
#define VK_F12 0x7B
#define VK_NUMLOCK 0x90
#define VK_SCROLL 0x91
#define VK_LSHIFT 0xA0
#define VK_RSHIFT 0xA1
#define VK_LCONTROL 0xA2
#define VK_RCONTROL 0xA3
#define VK_LMENU 0xA4
#define VK_RMENU 0xA5
#define VK_OEM_1 0xBA
#define VK_OEM_PLUS 0xBB
#define VK_OEM_COMMA 0xBC
#define VK_OEM_MINUS 0xBD
#define VK_OEM_PERIOD 0xBE
#define VK_OEM_2 0xBF
#define VK_OEM_3 0xC0
#define VK_OEM_4 0xDB
#define VK_OEM_5 0xDC
#define VK_OEM_6 0xDD
#define VK_OEM_7 0xDE
#define VK_OEM_8 0xDF
#define VK_OEM_102 0xE2

/* accelerators */
#define FVIRTKEY 0x01
#define FNOINVERT 0x02
#define FSHIFT 0x04
#define FCONTROL 0x08
#define FALT 0x10

/* GDI */
typedef struct PALETTEENTRY PALETTEENTRY;
struct PALETTEENTRY {
	BYTE peRed;
	BYTE peGreen;
	BYTE peBlue;
	BYTE peFlags;
};
typedef PALETTEENTRY *LPPALETTEENTRY;

typedef struct RGBQUAD RGBQUAD;
struct RGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
};

typedef struct BITMAPINFOHEADER BITMAPINFOHEADER;
struct BITMAPINFOHEADER {
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
};
typedef BITMAPINFOHEADER *LPBITMAPINFOHEADER;
typedef BITMAPINFOHEADER *PBITMAPINFOHEADER;

/* BITMAPFILEHEADER is 14 bytes, as a BMP file holds it. */
#pragma pack(push, 2)
typedef struct BITMAPFILEHEADER BITMAPFILEHEADER;
struct BITMAPFILEHEADER {
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
};
#pragma pack(pop)
typedef BITMAPFILEHEADER *LPBITMAPFILEHEADER;
typedef BITMAPFILEHEADER *PBITMAPFILEHEADER;

#define BI_RGB 0

#define BLACK_BRUSH 4
#define TRANSPARENT 1
#define OPAQUE 2
#define MM_TEXT 1
#define HORZRES 8
#define VERTRES 10
#define LOGPIXELSY 90
#define PHYSICALWIDTH 110
#define PHYSICALHEIGHT 111
#define PHYSICALOFFSETX 112
#define PHYSICALOFFSETY 113
#define FW_NORMAL 400
#define FW_BOLD 700
#define DEFAULT_CHARSET 1
#define OUT_TT_PRECIS 4
#define CLIP_DEFAULT_PRECIS 0
#define DEFAULT_QUALITY 0
#define VARIABLE_PITCH 2
#define FF_ROMAN (1 << 4)

/* printing */
typedef struct DOCINFOA DOCINFOA;
struct DOCINFOA {
	int cbSize;
	LPCSTR lpszDocName;
	LPCSTR lpszOutput;
	LPCSTR lpszDatatype;
	DWORD fwType;
};
typedef DOCINFOA DOCINFO;
typedef DOCINFOA *LPDOCINFOA;
typedef DOCINFOA *LPDOCINFO;

typedef struct PRINTER_INFO_2A PRINTER_INFO_2A;
struct PRINTER_INFO_2A {
	LPSTR pServerName;
	LPSTR pPrinterName;
	LPSTR pShareName;
	LPSTR pPortName;
	LPSTR pDriverName;
	LPSTR pComment;
	LPSTR pLocation;
	void *pDevMode;
	LPSTR pSepFile;
	LPSTR pPrintProcessor;
	LPSTR pDatatype;
	LPSTR pParameters;
	void *pSecurityDescriptor;
	DWORD Attributes;
	DWORD Priority;
	DWORD DefaultPriority;
	DWORD StartTime;
	DWORD UntilTime;
	DWORD Status;
	DWORD cJobs;
	DWORD AveragePPM;
};
typedef PRINTER_INFO_2A PRINTER_INFO_2;
typedef PRINTER_INFO_2A *LPPRINTER_INFO_2;
#define PRINTER_ENUM_LOCAL 0x00000002

/* processes */
typedef struct STARTUPINFOA STARTUPINFOA;
struct STARTUPINFOA {
	DWORD cb;
	LPSTR lpReserved;
	LPSTR lpDesktop;
	LPSTR lpTitle;
	DWORD dwX;
	DWORD dwY;
	DWORD dwXSize;
	DWORD dwYSize;
	DWORD dwXCountChars;
	DWORD dwYCountChars;
	DWORD dwFillAttribute;
	DWORD dwFlags;
	WORD wShowWindow;
	WORD cbReserved2;
	LPBYTE lpReserved2;
	HANDLE hStdInput;
	HANDLE hStdOutput;
	HANDLE hStdError;
};
typedef STARTUPINFOA STARTUPINFO;
typedef STARTUPINFOA *LPSTARTUPINFOA;
typedef STARTUPINFOA *LPSTARTUPINFO;

typedef struct PROCESS_INFORMATION PROCESS_INFORMATION;
struct PROCESS_INFORMATION {
	HANDLE hProcess;
	HANDLE hThread;
	DWORD dwProcessId;
	DWORD dwThreadId;
};
typedef PROCESS_INFORMATION *LPPROCESS_INFORMATION;

/* the clipboard and error messages */
#define CF_TEXT 1
#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#define FORMAT_MESSAGE_IGNORE_INSERTS 0x00000200
#define FORMAT_MESSAGE_FROM_SYSTEM 0x00001000

/* COM */
typedef struct IUnknown IUnknown;
typedef IUnknown *LPUNKNOWN;
#define CLSCTX_INPROC_SERVER 0x1

/* KERNEL32 */
#define CloseHandle sysCloseHandle
#define CreateDirectory CreateDirectoryA
#define CreateDirectoryA sysCreateDirectoryA
#define CreateFile CreateFileA
#define CreateFileA sysCreateFileA
#define CreateMutex CreateMutexA
#define CreateMutexA sysCreateMutexA
#define CreateProcess CreateProcessA
#define CreateProcessA sysCreateProcessA
#define DeleteFile DeleteFileA
#define DeleteFileA sysDeleteFileA
#define FileTimeToDosDateTime sysFileTimeToDosDateTime
#define FileTimeToLocalFileTime sysFileTimeToLocalFileTime
#define FileTimeToSystemTime sysFileTimeToSystemTime
#define FindClose sysFindClose
#define FindFirstFile FindFirstFileA
#define FindFirstFileA sysFindFirstFileA
#define FindNextFile FindNextFileA
#define FindNextFileA sysFindNextFileA
#define FormatMessage FormatMessageA
#define FormatMessageA sysFormatMessageA
#define GetCurrentDirectory GetCurrentDirectoryA
#define GetCurrentDirectoryA sysGetCurrentDirectoryA
#define GetDiskFreeSpace GetDiskFreeSpaceA
#define GetDiskFreeSpaceA sysGetDiskFreeSpaceA
#define GetDiskFreeSpaceEx GetDiskFreeSpaceExA
#define GetDiskFreeSpaceExA sysGetDiskFreeSpaceExA
#define GetEnvironmentVariable GetEnvironmentVariableA
#define GetEnvironmentVariableA sysGetEnvironmentVariableA
#define GetFileSize sysGetFileSize
#define GetFileTime sysGetFileTime
#define GetLastError sysGetLastError
#define GetLocaleInfo GetLocaleInfoA
#define GetLocaleInfoA sysGetLocaleInfoA
#define GetLocalTime sysGetLocalTime
#define GetModuleFileName GetModuleFileNameA
#define GetModuleFileNameA sysGetModuleFileNameA
#define GetSystemInfo sysGetSystemInfo
#define GetUserDefaultLangID sysGetUserDefaultLangID
#define GetVersionEx GetVersionExA
#define GetVersionExA sysGetVersionExA
#define GetVolumeInformation GetVolumeInformationA
#define GetVolumeInformationA sysGetVolumeInformationA
#define GetWindowsDirectory GetWindowsDirectoryA
#define GetWindowsDirectoryA sysGetWindowsDirectoryA
#define GlobalAlloc sysGlobalAlloc
#define GlobalFree sysGlobalFree
#define GlobalHandle sysGlobalHandle
#define GlobalLock sysGlobalLock
#define GlobalMemoryStatus sysGlobalMemoryStatus
#define GlobalReAlloc sysGlobalReAlloc
#define GlobalUnlock sysGlobalUnlock
#define LocalAlloc sysLocalAlloc
#define LocalFree sysLocalFree
#define LocalLock sysLocalLock
#define LocalReAlloc sysLocalReAlloc
#define LocalUnlock sysLocalUnlock
#define MoveFile MoveFileA
#define MoveFileA sysMoveFileA
#define MoveFileEx MoveFileExA
#define MoveFileExA sysMoveFileExA
#define MulDiv sysMulDiv
#define MultiByteToWideChar sysMultiByteToWideChar
#define ReadFile sysReadFile
#define RemoveDirectory RemoveDirectoryA
#define RemoveDirectoryA sysRemoveDirectoryA
#define SetCurrentDirectory SetCurrentDirectoryA
#define SetCurrentDirectoryA sysSetCurrentDirectoryA
#define SetFilePointer sysSetFilePointer
#define SetFileTime sysSetFileTime
#define SetLastError sysSetLastError
#define Sleep sysSleep
#define WriteFile sysWriteFile
#define lstrcpy lstrcpyA
#define lstrcpyA syslstrcpyA
#define lstrlen lstrlenA
#define lstrlenA syslstrlenA

BOOL CloseHandle(HANDLE);
BOOL CreateDirectoryA(LPCSTR, LPSECURITY_ATTRIBUTES);
HANDLE CreateFileA(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
HANDLE CreateMutexA(LPSECURITY_ATTRIBUTES, BOOL, LPCSTR);
BOOL CreateProcessA(LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES,
	BOOL, DWORD, LPVOID, LPCSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION);
BOOL DeleteFileA(LPCSTR);
BOOL FileTimeToDosDateTime(const FILETIME *, LPWORD, LPWORD);
BOOL FileTimeToLocalFileTime(const FILETIME *, LPFILETIME);
BOOL FileTimeToSystemTime(const FILETIME *, LPSYSTEMTIME);
BOOL FindClose(HANDLE);
HANDLE FindFirstFileA(LPCSTR, LPWIN32_FIND_DATAA);
BOOL FindNextFileA(HANDLE, LPWIN32_FIND_DATAA);
DWORD FormatMessageA(DWORD, LPCVOID, DWORD, DWORD, LPSTR, DWORD, va_list *);
DWORD GetCurrentDirectoryA(DWORD, LPSTR);
BOOL GetDiskFreeSpaceA(LPCSTR, LPDWORD, LPDWORD, LPDWORD, LPDWORD);
BOOL GetDiskFreeSpaceExA(LPCSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);
DWORD GetEnvironmentVariableA(LPCSTR, LPSTR, DWORD);
DWORD GetFileSize(HANDLE, LPDWORD);
BOOL GetFileTime(HANDLE, LPFILETIME, LPFILETIME, LPFILETIME);
DWORD GetLastError(void);
int GetLocaleInfoA(LCID, LCTYPE, LPSTR, int);
void GetLocalTime(LPSYSTEMTIME);
DWORD GetModuleFileNameA(HMODULE, LPSTR, DWORD);
void GetSystemInfo(LPSYSTEM_INFO);
LANGID GetUserDefaultLangID(void);
BOOL GetVersionExA(LPOSVERSIONINFOA);
BOOL GetVolumeInformationA(LPCSTR, LPSTR, DWORD, LPDWORD, LPDWORD, LPDWORD, LPSTR, DWORD);
UINT GetWindowsDirectoryA(LPSTR, UINT);
HGLOBAL GlobalAlloc(UINT, DWORD);
HGLOBAL GlobalFree(HGLOBAL);
HGLOBAL GlobalHandle(LPCVOID);
LPVOID GlobalLock(HGLOBAL);
void GlobalMemoryStatus(LPMEMORYSTATUS);
HGLOBAL GlobalReAlloc(HGLOBAL, DWORD, UINT);
BOOL GlobalUnlock(HGLOBAL);
HLOCAL LocalAlloc(UINT, UINT);
HLOCAL LocalFree(HLOCAL);
LPVOID LocalLock(HLOCAL);
HLOCAL LocalReAlloc(HLOCAL, UINT, UINT);
BOOL LocalUnlock(HLOCAL);
BOOL MoveFileA(LPCSTR, LPCSTR);
BOOL MoveFileExA(LPCSTR, LPCSTR, DWORD);
int MulDiv(int, int, int);
int MultiByteToWideChar(UINT, DWORD, LPCSTR, int, LPWSTR, int);
BOOL ReadFile(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
BOOL RemoveDirectoryA(LPCSTR);
BOOL SetCurrentDirectoryA(LPCSTR);
DWORD SetFilePointer(HANDLE, LONG, PLONG, DWORD);
BOOL SetFileTime(HANDLE, const FILETIME *, const FILETIME *, const FILETIME *);
void SetLastError(DWORD);
void Sleep(DWORD);
BOOL WriteFile(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
LPSTR lstrcpyA(LPSTR, LPCSTR);
int lstrlenA(LPCSTR);

/* ADVAPI32 */
#define GetUserName GetUserNameA
#define GetUserNameA sysGetUserNameA
#define RegCloseKey sysRegCloseKey
#define RegCreateKeyEx RegCreateKeyExA
#define RegCreateKeyExA sysRegCreateKeyExA
#define RegOpenKeyEx RegOpenKeyExA
#define RegOpenKeyExA sysRegOpenKeyExA
#define RegQueryValueEx RegQueryValueExA
#define RegQueryValueExA sysRegQueryValueExA
#define RegSetValueEx RegSetValueExA
#define RegSetValueExA sysRegSetValueExA

BOOL GetUserNameA(LPSTR, LPDWORD);
LONG RegCloseKey(HKEY);
LONG RegCreateKeyExA(HKEY, LPCSTR, DWORD, LPSTR, DWORD, REGSAM,
	LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD);
LONG RegOpenKeyExA(HKEY, LPCSTR, DWORD, REGSAM, PHKEY);
LONG RegQueryValueExA(HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD);
LONG RegSetValueExA(HKEY, LPCSTR, DWORD, DWORD, const BYTE *, DWORD);

/* USER32 */
#define CheckRadioButton sysCheckRadioButton
#define ClipCursor sysClipCursor
#define CloseClipboard sysCloseClipboard
#define CreateWindow CreateWindowA
#define CreateWindowA(cls, name, style, x, y, w, h, parent, menu, inst, param) \
	CreateWindowExA(0, cls, name, style, x, y, w, h, parent, menu, inst, param)
#define CreateWindowEx CreateWindowExA
#define CreateWindowExA sysCreateWindowExA
#define DefWindowProc DefWindowProcA
#define DefWindowProcA sysDefWindowProcA
#define DestroyWindow sysDestroyWindow
#define DialogBoxParam DialogBoxParamA
#define DialogBoxParamA sysDialogBoxParamA
#define DispatchMessage DispatchMessageA
#define DispatchMessageA sysDispatchMessageA
#define EmptyClipboard sysEmptyClipboard
#define EndDialog sysEndDialog
#define GetAsyncKeyState sysGetAsyncKeyState
#define GetClipboardData sysGetClipboardData
#define GetCursorPos sysGetCursorPos
#define GetDlgItem sysGetDlgItem
#define GetForegroundWindow sysGetForegroundWindow
#define GetKeyboardState sysGetKeyboardState
#define GetMessage GetMessageA
#define GetMessageA sysGetMessageA
#define GetSystemMetrics sysGetSystemMetrics
#define GetWindowRect sysGetWindowRect
#define IsDialogMessage IsDialogMessageA
#define IsDialogMessageA sysIsDialogMessageA
#define IsIconic sysIsIconic
#define LoadAccelerators LoadAcceleratorsA
#define LoadAcceleratorsA sysLoadAcceleratorsA
#define LoadCursor LoadCursorA
#define LoadCursorA sysLoadCursorA
#define LoadIcon LoadIconA
#define LoadIconA sysLoadIconA
#define MapVirtualKey MapVirtualKeyA
#define MapVirtualKeyA sysMapVirtualKeyA
#define MessageBox MessageBoxA
#define MessageBoxA sysMessageBoxA
#define MessageBoxEx MessageBoxExA
#define MessageBoxExA sysMessageBoxExA
#define OpenClipboard sysOpenClipboard
#define PeekMessage PeekMessageA
#define PeekMessageA sysPeekMessageA
#define PostMessage PostMessageA
#define PostMessageA sysPostMessageA
#define PostQuitMessage sysPostQuitMessage
#define RegisterClass RegisterClassA
#define RegisterClassA sysRegisterClassA
#define SendDlgItemMessage SendDlgItemMessageA
#define SendDlgItemMessageA sysSendDlgItemMessageA
#define SendMessage SendMessageA
#define SendMessageA sysSendMessageA
#define SetClassLong SetClassLongA
#define SetClassLongA sysSetClassLongA
#define SetClipboardData sysSetClipboardData
#define SetCursorPos sysSetCursorPos
#define SetDlgItemText SetDlgItemTextA
#define SetDlgItemTextA sysSetDlgItemTextA
#define SetFocus sysSetFocus
#define SetWindowPos sysSetWindowPos
#define SetWindowText SetWindowTextA
#define SetWindowTextA sysSetWindowTextA
#define ShowCursor sysShowCursor
#define ShowWindow sysShowWindow
#define ToAscii sysToAscii
#define TranslateAccelerator TranslateAcceleratorA
#define TranslateAcceleratorA sysTranslateAcceleratorA
#define TranslateMessage sysTranslateMessage
#define UpdateWindow sysUpdateWindow
#define WaitMessage sysWaitMessage
#define wsprintf wsprintfA
#define wsprintfA syswsprintfA
#define wvsprintf wvsprintfA
#define wvsprintfA syswvsprintfA

int CheckRadioButton(HWND, int, int, int);
BOOL ClipCursor(const RECT *);
BOOL CloseClipboard(void);
HWND CreateWindowExA(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int,
	HWND, HMENU, HINSTANCE, LPVOID);
LRESULT DefWindowProcA(HWND, UINT, WPARAM, LPARAM);
BOOL DestroyWindow(HWND);
INT_PTR DialogBoxParamA(HINSTANCE, LPCSTR, HWND, DLGPROC, LPARAM);
LRESULT DispatchMessageA(const MSG *);
BOOL EmptyClipboard(void);
BOOL EndDialog(HWND, INT_PTR);
SHORT GetAsyncKeyState(int);
HANDLE GetClipboardData(UINT);
BOOL GetCursorPos(LPPOINT);
HWND GetDlgItem(HWND, int);
HWND GetForegroundWindow(void);
BOOL GetKeyboardState(PBYTE);
BOOL GetMessageA(LPMSG, HWND, UINT, UINT);
int GetSystemMetrics(int);
BOOL GetWindowRect(HWND, LPRECT);
BOOL IsDialogMessageA(HWND, LPMSG);
BOOL IsIconic(HWND);
HACCEL LoadAcceleratorsA(HINSTANCE, LPCSTR);
HCURSOR LoadCursorA(HINSTANCE, LPCSTR);
HICON LoadIconA(HINSTANCE, LPCSTR);
UINT MapVirtualKeyA(UINT, UINT);
int MessageBoxA(HWND, LPCSTR, LPCSTR, UINT);
int MessageBoxExA(HWND, LPCSTR, LPCSTR, UINT, WORD);
BOOL OpenClipboard(HWND);
BOOL PeekMessageA(LPMSG, HWND, UINT, UINT, UINT);
BOOL PostMessageA(HWND, UINT, WPARAM, LPARAM);
void PostQuitMessage(int);
ATOM RegisterClassA(const WNDCLASSA *);
LRESULT SendDlgItemMessageA(HWND, int, UINT, WPARAM, LPARAM);
LRESULT SendMessageA(HWND, UINT, WPARAM, LPARAM);
DWORD SetClassLongA(HWND, int, LONG);
HANDLE SetClipboardData(UINT, HANDLE);
BOOL SetCursorPos(int, int);
BOOL SetDlgItemTextA(HWND, int, LPCSTR);
HWND SetFocus(HWND);
BOOL SetWindowPos(HWND, HWND, int, int, int, int, UINT);
BOOL SetWindowTextA(HWND, LPCSTR);
int ShowCursor(BOOL);
BOOL ShowWindow(HWND, int);
int ToAscii(UINT, UINT, const BYTE *, LPWORD, UINT);
int TranslateAcceleratorA(HWND, HACCEL, LPMSG);
BOOL TranslateMessage(const MSG *);
BOOL UpdateWindow(HWND);
BOOL WaitMessage(void);
int wsprintfA(LPSTR, LPCSTR, ...);
int wvsprintfA(LPSTR, LPCSTR, va_list);

/* GDI32 */
#define CreateDC CreateDCA
#define CreateDCA sysCreateDCA
#define CreateFont CreateFontA
#define CreateFontA sysCreateFontA
#define DeleteDC sysDeleteDC
#define DeleteObject sysDeleteObject
#define EndDoc sysEndDoc
#define EndPage sysEndPage
#define GetDeviceCaps sysGetDeviceCaps
#define GetStockObject sysGetStockObject
#define GetTextExtentPoint32 GetTextExtentPoint32A
#define GetTextExtentPoint32A sysGetTextExtentPoint32A
#define SelectObject sysSelectObject
#define SetBkMode sysSetBkMode
#define SetMapMode sysSetMapMode
#define SetTextColor sysSetTextColor
#define StartDoc StartDocA
#define StartDocA sysStartDocA
#define StartPage sysStartPage
#define TextOut TextOutA
#define TextOutA sysTextOutA

HDC CreateDCA(LPCSTR, LPCSTR, LPCSTR, const void *);
HFONT CreateFontA(int, int, int, int, int, DWORD, DWORD, DWORD, DWORD,
	DWORD, DWORD, DWORD, DWORD, LPCSTR);
BOOL DeleteDC(HDC);
BOOL DeleteObject(HGDIOBJ);
int EndDoc(HDC);
int EndPage(HDC);
int GetDeviceCaps(HDC, int);
HGDIOBJ GetStockObject(int);
BOOL GetTextExtentPoint32A(HDC, LPCSTR, int, LPSIZE);
HGDIOBJ SelectObject(HDC, HGDIOBJ);
int SetBkMode(HDC, int);
int SetMapMode(HDC, int);
COLORREF SetTextColor(HDC, COLORREF);
int StartDocA(HDC, const DOCINFOA *);
int StartPage(HDC);
BOOL TextOutA(HDC, int, int, LPCSTR, int);

/* WINSPOOL */
#define EnumPrinters EnumPrintersA
#define EnumPrintersA sysEnumPrintersA

BOOL EnumPrintersA(DWORD, LPSTR, DWORD, LPBYTE, DWORD, LPDWORD, LPDWORD);

/* SHELL32 and OLE32 */
#define ShellExecute ShellExecuteA
#define ShellExecuteA sysShellExecuteA
#define CoCreateInstance sysCoCreateInstance
#define CoInitialize sysCoInitialize
#define CoUninitialize sysCoUninitialize

HINSTANCE ShellExecuteA(HWND, LPCSTR, LPCSTR, LPCSTR, LPCSTR, int);
HRESULT CoCreateInstance(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID *);
HRESULT CoInitialize(LPVOID);
void CoUninitialize(void);

#pragma pack(pop)

#include <mmsystem.h>

#endif

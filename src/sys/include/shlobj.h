/*
 * The part of the shell that the game's setup code names: special
 * folders, the folder browser, the shell's allocator, and shell links,
 * in the C binding of the SDK. The port presets the registry, so the
 * game skips its setup, and src/sys answers each call with an error or
 * an empty result.
 */

#ifndef SYS_SHLOBJ_H
#define SYS_SHLOBJ_H

#include <windows.h>

#pragma pack(push, 8)

typedef struct ITEMIDLIST ITEMIDLIST;
typedef ITEMIDLIST *LPITEMIDLIST;
typedef const ITEMIDLIST *LPCITEMIDLIST;

typedef struct IMalloc IMalloc;
typedef IMalloc *LPMALLOC;
typedef struct IShellLinkA IShellLinkA;
typedef IShellLinkA IShellLink;
typedef struct IPersistFile IPersistFile;

#define CSIDL_PROGRAMS 0x0002
#define CSIDL_PERSONAL 0x0005
#define CSIDL_COMMON_PROGRAMS 0x0017

typedef int (*BFFCALLBACK)(HWND, UINT, LPARAM, LPARAM);

typedef struct BROWSEINFOA BROWSEINFOA;
struct BROWSEINFOA {
	HWND hwndOwner;
	LPCITEMIDLIST pidlRoot;
	LPSTR pszDisplayName;
	LPCSTR lpszTitle;
	UINT ulFlags;
	BFFCALLBACK lpfn;
	LPARAM lParam;
	int iImage;
};
typedef BROWSEINFOA BROWSEINFO;
typedef BROWSEINFOA *LPBROWSEINFOA;
typedef BROWSEINFOA *LPBROWSEINFO;

#define BIF_RETURNONLYFSDIRS 0x0001
#define BFFM_INITIALIZED 1
#define BFFM_SELCHANGED 2
#define BFFM_SETSTATUSTEXTA (WM_USER + 100)
#define BFFM_ENABLEOK (WM_USER + 101)
#define BFFM_SETSELECTIONA (WM_USER + 102)
#define BFFM_SETSELECTION BFFM_SETSELECTIONA

typedef struct IMallocVtbl IMallocVtbl;
struct IMallocVtbl {
	ULONG (*Release)(IMalloc *);
	void (*Free)(IMalloc *, void *);
};
struct IMalloc {
	const IMallocVtbl *lpVtbl;
};

typedef struct IShellLinkAVtbl IShellLinkAVtbl;
struct IShellLinkAVtbl {
	HRESULT (*QueryInterface)(IShellLinkA *, REFIID, void **);
	ULONG (*Release)(IShellLinkA *);
	HRESULT (*SetDescription)(IShellLinkA *, LPCSTR);
	HRESULT (*SetPath)(IShellLinkA *, LPCSTR);
	HRESULT (*SetWorkingDirectory)(IShellLinkA *, LPCSTR);
};
struct IShellLinkA {
	const IShellLinkAVtbl *lpVtbl;
};

typedef struct IPersistFileVtbl IPersistFileVtbl;
struct IPersistFileVtbl {
	ULONG (*Release)(IPersistFile *);
	HRESULT (*Save)(IPersistFile *, LPCWSTR, BOOL);
};
struct IPersistFile {
	const IPersistFileVtbl *lpVtbl;
};

#define IMalloc_Release(p) (p)->lpVtbl->Release(p)
#define IMalloc_Free(p, a) (p)->lpVtbl->Free(p, a)
#define IShellLink_QueryInterface(p, a, b) (p)->lpVtbl->QueryInterface(p, a, b)
#define IShellLink_Release(p) (p)->lpVtbl->Release(p)
#define IShellLink_SetDescription(p, a) (p)->lpVtbl->SetDescription(p, a)
#define IShellLink_SetPath(p, a) (p)->lpVtbl->SetPath(p, a)
#define IShellLink_SetWorkingDirectory(p, a) (p)->lpVtbl->SetWorkingDirectory(p, a)
#define IPersistFile_Release(p) (p)->lpVtbl->Release(p)
#define IPersistFile_Save(p, a, b) (p)->lpVtbl->Save(p, a, b)

#define CLSID_ShellLink sysCLSID_ShellLink
#define IID_IShellLinkA sysIID_IShellLinkA
#define IID_IShellLink IID_IShellLinkA

extern const CLSID CLSID_ShellLink;
extern const IID IID_IShellLinkA;
extern const IID IID_IPersistFile;

#define SHBrowseForFolder SHBrowseForFolderA
#define SHBrowseForFolderA sysSHBrowseForFolderA
#define SHGetMalloc sysSHGetMalloc
#define SHGetPathFromIDList SHGetPathFromIDListA
#define SHGetPathFromIDListA sysSHGetPathFromIDListA
#define SHGetSpecialFolderLocation sysSHGetSpecialFolderLocation

LPITEMIDLIST SHBrowseForFolderA(LPBROWSEINFOA);
HRESULT SHGetMalloc(LPMALLOC *);
BOOL SHGetPathFromIDListA(LPCITEMIDLIST, LPSTR);
HRESULT SHGetSpecialFolderLocation(HWND, int, LPITEMIDLIST *);

#pragma pack(pop)

#endif

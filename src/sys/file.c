/*
 * The file API of KERNEL32 on the game's virtual paths, for the port.
 *
 * The game names its files as on Windows, under the paths that the
 * port's registry gives it: C:\teudogar, the game's folder, which the
 * port keeps read only, and C:\teudogar.sav, the per-user folder of
 * its saved games. Every other path is missing. A virtual current
 * folder resolves relative names, as the game changes it at will.
 * Names resolve with case folded, as on Windows, and the files
 * the port creates take lowercase names. FindFirstFile walks a folder
 * sorted with case folded, so that every platform sees one order. A
 * file that an open truncates or creates is written under a temporary
 * name and takes its own at CloseHandle, when every write succeeded, so
 * that a failed save leaves the old one. SetFileTime dates the last
 * write of a file open to write, which the file takes as it closes. In
 * a replay, sysfull can give the folder a room in bytes, as a disk that
 * fills, and sysinplace can send its writes straight into their files,
 * as Windows does, for the game's saves that fail. CreateMutexA holds
 * the game to one run on each per-user folder.
 */

#ifndef _WIN32
/* a time_t of 64 bits for utime, on 32-bit Linux too */
#define _FILE_OFFSET_BITS 64
#define _TIME_BITS 64
#endif

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#include <share.h>
#include <sys/stat.h>
#include <wchar.h>
#else
#include <sys/file.h>
#include <unistd.h>
#include <utime.h>
#endif

#include <SDL3/SDL.h>
#include <windows.h>

#include "sys.h"

enum {
	Filemagic = 0x46696c65,
	Kfile = 1,
	Kfind,
	Klock,
	Maxparts = 64
};

/* the virtual folders, longest first */
static const struct {
	const char *name;	/* the first component after C:\ */
	int user;	/* whether it is the per-user folder, else the game's */
} mounts[] = {
	{ "teudogar.sav", 1 },
	{ "teudogar", 0 },
};

static const char exepath[] = "C:\\teudogar\\Teudogar.Exe";

typedef struct File File;
struct File {
	uint32_t magic;
	int kind;
	SDL_IOStream *io;
	char path[Maxpath];	/* the host path */
	char tmp[Maxpath];	/* the host path written in its place until CloseHandle, or empty */
	int writing;	/* whether the file was opened to write */
	int failed;	/* whether a write fell short */
	int settime;	/* whether SetFileTime gave the time of the last write */
	SDL_Time mtime;	/* that time, which the file takes at its close */
	int fd;	/* a lock's descriptor, or -1 */
	char **names;	/* a find's matches */
	int nnames;
	int next;
};

static char cwd[Maxpath] = "C:\\teudogar";	/* the virtual current folder */
static long room = -1;	/* the bytes that writes may still take, or -1 for no end */
static int inplace;	/* whether a write that empties its file writes the file itself, as Windows does, in place of a temporary one */

static void droplock(File *f);

/*
 * In a replay, the files that the run wrote and the virtual time at
 * which it closed each, which the times of files report, so that the
 * dates of the saved games come out the same on every platform. Their
 * paths compare with case folded, as the names of the folders resolve:
 * on Windows a path keeps the case that the game gave its name.
 */
typedef struct Written Written;
struct Written {
	char path[Maxpath];
	SDL_Time t;
};
static Written *written;
static int nwritten;
static SDL_Time replaystart;

void
fileinit(void)
{
	replaystart = sysdatetime();
}

/* wrote records that the run wrote path at the virtual time t. */
static void
wrote(const char *path, SDL_Time t)
{
	Written *w;
	int i;

	for (i = 0; i < nwritten; i++)
		if (SDL_strcasecmp(written[i].path, path) == 0) {
			written[i].t = t;
			return;
		}
	w = realloc(written, (nwritten + 1) * sizeof *written);
	if (w == NULL)
		return;
	written = w;
	SDL_strlcpy(written[nwritten].path, path, sizeof written[nwritten].path);
	written[nwritten].t = t;
	nwritten++;
}

/* filetime returns the time to report for path: the host's, or in a replay the run's. */
static SDL_Time
filetime(const char *path, SDL_Time host)
{
	int i;

	if (!replaying)
		return host;
	for (i = 0; i < nwritten; i++)
		if (SDL_strcasecmp(written[i].path, path) == 0)
			return written[i].t;
	return replaystart;
}

static int
isslash(char c)
{
	return c == '\\' || c == '/';
}

/* lowercopy copies src into dst in lowercase. */
static void
lowercopy(char *dst, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i + 1 < n && src[i] != '\0'; i++)
		dst[i] = (char)SDL_tolower((unsigned char)src[i]);
	dst[i] = '\0';
}

/*
 * absolute writes the absolute, normalized form of the virtual path p
 * into parts, one component to an entry after the drive, and returns
 * their count, or -1 for a path that leaves the drive's root.
 */
static int
absolute(const char *p, char parts[][MAX_PATH], int max)
{
	char buf[2 * Maxpath];
	char *s, *e;
	int n;

	if (p[0] != '\0' && p[1] == ':')
		SDL_strlcpy(buf, p + 2, sizeof buf);
	else if (isslash(p[0]))
		SDL_strlcpy(buf, p, sizeof buf);
	else {
		SDL_strlcpy(buf, cwd + 2, sizeof buf);
		SDL_strlcat(buf, "\\", sizeof buf);
		SDL_strlcat(buf, p, sizeof buf);
	}
	n = 0;
	for (s = buf; *s != '\0'; s = e) {
		while (isslash(*s))
			s++;
		if (*s == '\0')
			break;
		for (e = s; *e != '\0' && !isslash(*e); e++)
			;
		if (e - s == 1 && s[0] == '.')
			continue;
		if (e - s == 2 && s[0] == '.' && s[1] == '.') {
			if (n == 0)
				return -1;
			n--;
			continue;
		}
		if (n == max || e - s >= MAX_PATH)
			return -1;
		memcpy(parts[n], s, e - s);
		parts[n][e - s] = '\0';
		n++;
	}
	return n;
}

/* the state of a folder's walk for a name */
typedef struct Lookup Lookup;
struct Lookup {
	const char *want;
	char found[MAX_PATH];
};

static SDL_EnumerationResult
lookupone(void *arg, const char *dir, const char *name)
{
	Lookup *l;

	(void)dir;
	l = arg;
	if (SDL_strcasecmp(name, l->want) == 0) {
		SDL_strlcpy(l->found, name, sizeof l->found);
		return SDL_ENUM_SUCCESS;
	}
	return SDL_ENUM_CONTINUE;
}

/* lookup finds name in the host folder dir, with case folded. */
static int
lookup(const char *dir, const char *name, char *found, size_t n)
{
	Lookup l;
	char path[Maxpath];

	SDL_snprintf(path, sizeof path, "%s%s", dir, name);
	if (SDL_GetPathInfo(path, NULL)) {
		SDL_strlcpy(found, name, n);
		return 1;
	}
	l.want = name;
	l.found[0] = '\0';
	SDL_EnumerateDirectory(dir, lookupone, &l);
	if (l.found[0] == '\0')
		return 0;
	SDL_strlcpy(found, l.found, n);
	return 1;
}

/*
 * hostpath writes into host the path of the host for the virtual path
 * win. With create, the last component may be missing, and takes its
 * name in lowercase. It returns 0 for a path outside the virtual
 * folders or a missing component, with the error of Windows set.
 */
int
hostpath(const char *win, char *host, size_t n, int create)
{
	char parts[Maxparts][MAX_PATH];
	char name[MAX_PATH];
	int np, i, m;

	np = absolute(win, parts, Maxparts);
	if (np < 1)
		goto missing;
	m = -1;
	for (i = 0; i < (int)(sizeof mounts / sizeof mounts[0]); i++)
		if (SDL_strcasecmp(parts[0], mounts[i].name) == 0)
			m = i;
	if (m < 0)
		goto missing;
	SDL_strlcpy(host, mounts[m].user ? sysuserdir : sysgamedir, n);
	for (i = 1; i < np; i++) {
		if (!lookup(host, parts[i], name, sizeof name)) {
			if (!create || i < np - 1)
				goto missing;
			lowercopy(name, parts[i], sizeof name);
		}
		SDL_strlcat(host, name, n);
		if (i < np - 1)
			SDL_strlcat(host, "/", n);
	}
	return 1;
missing:
	syssetlasterror(ERROR_PATH_NOT_FOUND);
	return 0;
}

/* writable reports whether the virtual path win lies in the per-user folder. */
static int
writable(const char *win)
{
	char parts[Maxparts][MAX_PATH];
	int np;

	np = absolute(win, parts, Maxparts);
	return np >= 1 && SDL_strcasecmp(parts[0], mounts[0].name) == 0;
}

static File *
fileof(HANDLE h, int kind)
{
	File *f;

	if (h == NULL || h == INVALID_HANDLE_VALUE)
		return NULL;
	f = h;
	if (f->magic != Filemagic || f->kind != kind)
		return NULL;
	return f;
}

HANDLE
CreateFileA(LPCSTR name, DWORD access, DWORD share, LPSECURITY_ATTRIBUTES sa,
	DWORD disp, DWORD flags, HANDLE tmpl)
{
	char host[Maxpath];
	const char *mode;
	SDL_PathInfo info;
	int exists, write;
	File *f;

	(void)share, (void)sa, (void)flags, (void)tmpl;
	write = (access & GENERIC_WRITE) != 0;
	if (!hostpath(name, host, sizeof host, disp != OPEN_EXISTING && disp != TRUNCATE_EXISTING)) {
		syssetlasterror(ERROR_FILE_NOT_FOUND);
		return INVALID_HANDLE_VALUE;
	}
	exists = SDL_GetPathInfo(host, &info);
	if (exists && info.type != SDL_PATHTYPE_FILE) {
		syssetlasterror(ERROR_ACCESS_DENIED);
		return INVALID_HANDLE_VALUE;
	}
	switch (disp) {
	case CREATE_NEW:
		if (exists) {
			syssetlasterror(ERROR_FILE_EXISTS);
			return INVALID_HANDLE_VALUE;
		}
		mode = "w+b";
		break;
	case CREATE_ALWAYS:
		mode = "w+b";
		break;
	case OPEN_EXISTING:
		if (!exists) {
			syssetlasterror(ERROR_FILE_NOT_FOUND);
			return INVALID_HANDLE_VALUE;
		}
		mode = write ? "r+b" : "rb";
		break;
	case OPEN_ALWAYS:
		mode = exists ? (write ? "r+b" : "rb") : "w+b";
		break;
	case TRUNCATE_EXISTING:
		if (!exists) {
			syssetlasterror(ERROR_FILE_NOT_FOUND);
			return INVALID_HANDLE_VALUE;
		}
		mode = "w+b";
		break;
	default:
		syssetlasterror(ERROR_INVALID_PARAMETER);
		return INVALID_HANDLE_VALUE;
	}
	/* a mode of w writes the file, whatever the access asks */
	if (mode[0] == 'w')
		write = 1;
	if (write && !writable(name)) {
		syssetlasterror(ERROR_ACCESS_DENIED);
		return INVALID_HANDLE_VALUE;
	}
	f = calloc(1, sizeof *f);
	if (f == NULL) {
		syssetlasterror(ERROR_NOT_ENOUGH_MEMORY);
		return INVALID_HANDLE_VALUE;
	}
	f->magic = Filemagic;
	f->kind = Kfile;
	f->writing = write;
	SDL_strlcpy(f->path, host, sizeof f->path);
	if (mode[0] == 'w' && !inplace && SDL_snprintf(f->tmp, sizeof f->tmp, "%s.tmp", host) >= (int)sizeof f->tmp) {
		free(f);
		syssetlasterror(ERROR_PATH_NOT_FOUND);
		return INVALID_HANDLE_VALUE;
	}
	f->io = SDL_IOFromFile(f->tmp[0] != '\0' ? f->tmp : host, mode);
	if (f->io == NULL) {
		free(f);
		syssetlasterror(ERROR_ACCESS_DENIED);
		return INVALID_HANDLE_VALUE;
	}
	syssetlasterror(exists && (disp == OPEN_ALWAYS || disp == CREATE_ALWAYS) ?
		ERROR_ALREADY_EXISTS : ERROR_SUCCESS);
	return f;
}

BOOL
ReadFile(HANDLE h, LPVOID buf, DWORD n, LPDWORD got, LPOVERLAPPED ov)
{
	File *f;
	size_t r;

	(void)ov;
	f = fileof(h, Kfile);
	if (f == NULL) {
		syssetlasterror(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	r = n == 0 ? 0 : SDL_ReadIO(f->io, buf, n);
	if (got != NULL)
		*got = (DWORD)r;
	if (r < n && SDL_GetIOStatus(f->io) == SDL_IO_STATUS_ERROR) {
		syssetlasterror(ERROR_READ_FAULT);
		return FALSE;
	}
	return TRUE;
}

BOOL
WriteFile(HANDLE h, LPCVOID buf, DWORD n, LPDWORD put, LPOVERLAPPED ov)
{
	File *f;
	size_t r;
	DWORD m;

	(void)ov;
	f = fileof(h, Kfile);
	if (f == NULL) {
		syssetlasterror(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	m = n;
	if (room >= 0 && (unsigned long)room < m)
		m = (DWORD)room;
	r = m == 0 ? 0 : SDL_WriteIO(f->io, buf, m);
	if (room >= 0)
		room -= (long)r;
	if (put != NULL)
		*put = (DWORD)r;
	if (r < n) {
		f->failed = 1;
		syssetlasterror(m < n ? ERROR_DISK_FULL : ERROR_WRITE_FAULT);
		return FALSE;
	}
	return TRUE;
}

/* sysfull gives the writes n more bytes, as a disk that fills, or with -1 all they want. */
void
sysfull(long n)
{
	room = n;
}

/*
 * sysinplace makes an open that empties a file empty the file itself,
 * as Windows' does, so that a write that fails leaves a part of it,
 * or with 0 gives such an open a temporary file again.
 */
void
sysinplace(int on)
{
	inplace = on;
}

DWORD
SetFilePointer(HANDLE h, LONG dist, PLONG high, DWORD method)
{
	SDL_IOWhence whence;
	Sint64 off, pos;
	File *f;

	f = fileof(h, Kfile);
	if (f == NULL) {
		syssetlasterror(ERROR_INVALID_HANDLE);
		return 0xFFFFFFFF;
	}
	off = dist;
	if (high != NULL)
		off = (Sint64)((uint64_t)(uint32_t)*high << 32 | (uint32_t)dist);
	switch (method) {
	case FILE_BEGIN:
		whence = SDL_IO_SEEK_SET;
		break;
	case FILE_CURRENT:
		whence = SDL_IO_SEEK_CUR;
		break;
	case FILE_END:
		whence = SDL_IO_SEEK_END;
		break;
	default:
		syssetlasterror(ERROR_INVALID_PARAMETER);
		return 0xFFFFFFFF;
	}
	pos = SDL_SeekIO(f->io, off, whence);
	if (pos < 0) {
		syssetlasterror(ERROR_INVALID_PARAMETER);
		return 0xFFFFFFFF;
	}
	if (high != NULL)
		*high = (LONG)(pos >> 32);
	syssetlasterror(ERROR_SUCCESS);
	return (DWORD)pos;
}

DWORD
GetFileSize(HANDLE h, LPDWORD high)
{
	Sint64 n;
	File *f;

	f = fileof(h, Kfile);
	if (f == NULL) {
		syssetlasterror(ERROR_INVALID_HANDLE);
		return INVALID_FILE_SIZE;
	}
	n = SDL_GetIOSize(f->io);
	if (n < 0)
		return INVALID_FILE_SIZE;
	if (high != NULL)
		*high = (DWORD)(n >> 32);
	return (DWORD)n;
}

BOOL
GetFileTime(HANDLE h, LPFILETIME ctime, LPFILETIME atime, LPFILETIME mtime)
{
	SDL_PathInfo info;
	File *f;

	f = fileof(h, Kfile);
	if (f == NULL || !SDL_GetPathInfo(f->tmp[0] != '\0' ? f->tmp : f->path, &info)) {
		syssetlasterror(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	if (ctime != NULL)
		systotime(filetime(f->path, info.create_time), ctime);
	if (atime != NULL)
		systotime(filetime(f->path, info.access_time), atime);
	if (mtime != NULL)
		systotime(filetime(f->path, info.modify_time), mtime);
	return TRUE;
}

/*
 * SetFileTime gives a file open to write the time of its last write,
 * which it takes at its close, in a replay the run's. The times of
 * its creation and its last access stay the host's.
 */
BOOL
SetFileTime(HANDLE h, const FILETIME *ctime, const FILETIME *atime, const FILETIME *mtime)
{
	File *f;

	(void)ctime, (void)atime;
	f = fileof(h, Kfile);
	if (f == NULL) {
		syssetlasterror(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	if (!f->writing) {
		syssetlasterror(ERROR_ACCESS_DENIED);
		return FALSE;
	}
	if (mtime != NULL) {
		f->mtime = SDL_TimeFromWindows(mtime->dwLowDateTime, mtime->dwHighDateTime);
		f->settime = 1;
	}
	return TRUE;
}

#ifdef _WIN32
#undef SetFileTime
/* kernel32's own SetFileTime, whose name windows.h gives the port's above */
__declspec(dllimport) BOOL __stdcall SetFileTime(HANDLE, const FILETIME *, const FILETIME *, const FILETIME *);
#endif

/*
 * sethosttime sets the time of the last write of the host's file path
 * to t, and reports whether it could. Windows takes it through
 * kernel32, since the CRT's _futime64 converts through the zone's
 * offset of the day it runs, which puts a time across a change of the
 * daylight saving time an hour off.
 */
static int
sethosttime(const char *path, SDL_Time t)
{
#ifdef _WIN32
	FILETIME ft;
	wchar_t *w;
	int fd;
	BOOL ok;

	w = (wchar_t *)SDL_iconv_string("UTF-16LE", "UTF-8", path, strlen(path) + 1);
	if (w == NULL)
		return 0;
	fd = _wopen(w, _O_RDWR | _O_BINARY);
	SDL_free(w);
	if (fd < 0)
		return 0;
	systotime(t, &ft);
	ok = SetFileTime((HANDLE)_get_osfhandle(fd), NULL, NULL, &ft);
	_close(fd);
	return ok;
#else
	SDL_PathInfo info;
	struct utimbuf u;

	if (!SDL_GetPathInfo(path, &info))
		return 0;
	u.actime = SDL_NS_TO_SECONDS(info.access_time);
	u.modtime = SDL_NS_TO_SECONDS(t);
	return utime(path, &u) == 0;
#endif
}

static void
freefind(File *f)
{
	int i;

	for (i = 0; i < f->nnames; i++)
		free(f->names[i]);
	free(f->names);
}

/*
 * CloseHandle ends a file or a lock. A file written under a
 * temporary name takes its own name when every write and the close
 * succeeded, and else goes, leaving the old file as it was. Any other
 * file closes as on Windows, whatever its writes did, and takes the
 * time of its last write.
 */
BOOL
CloseHandle(HANDLE h)
{
	File *f;
	int ok, tmp;

	f = fileof(h, Klock);
	if (f != NULL) {
		droplock(f);
		f->magic = 0;
		return TRUE;
	}
	f = fileof(h, Kfile);
	if (f == NULL)
		return h != NULL && h != INVALID_HANDLE_VALUE;
	ok = SDL_CloseIO(f->io) && !f->failed;
	tmp = f->tmp[0] != '\0';
	if (tmp) {
		if (ok && !SDL_RenamePath(f->tmp, f->path))
			ok = 0;
		if (!ok)
			SDL_RemovePath(f->tmp);
	}
	if (!ok)
		SDL_Log("teudogar: %s: the write failed", f->path);
	if (ok || !tmp) {
		if (f->writing && replaying)
			wrote(f->path, f->settime ? f->mtime : sysdatetime());
		else if (f->settime && !sethosttime(f->path, f->mtime))
			SDL_Log("teudogar: %s: the time of the last write stays the host's", f->path);
	}
	f->magic = 0;
	free(f);
	if (!ok && tmp) {
		syssetlasterror(ERROR_WRITE_FAULT);
		return FALSE;
	}
	return TRUE;
}

/* match reports whether name matches the wildcards of pat, with case folded. */
static int
match(const char *pat, const char *name)
{
	if (strcmp(pat, "*.*") == 0)
		return 1;
	for (; *pat != '\0'; pat++, name++) {
		if (*pat == '*') {
			while (pat[1] == '*')
				pat++;
			if (pat[1] == '\0')
				return 1;
			for (; *name != '\0'; name++)
				if (match(pat + 1, name))
					return 1;
			return match(pat + 1, name);
		}
		if (*name == '\0')
			return 0;
		if (*pat != '?' && SDL_tolower((unsigned char)*pat) != SDL_tolower((unsigned char)*name))
			return 0;
	}
	return *name == '\0';
}

/* the state of a folder's walk for the names a pattern matches */
typedef struct Walk Walk;
struct Walk {
	const char *pat;
	File *f;
	int cap;
	int nomem;
};

static SDL_EnumerationResult
walkone(void *arg, const char *dir, const char *name)
{
	Walk *w;
	char **names;

	(void)dir;
	w = arg;
	if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0 || !match(w->pat, name))
		return SDL_ENUM_CONTINUE;
	if (w->f->nnames == w->cap) {
		w->cap = w->cap ? 2 * w->cap : 16;
		names = realloc(w->f->names, w->cap * sizeof w->f->names[0]);
		if (names == NULL) {
			w->nomem = 1;
			return SDL_ENUM_FAILURE;
		}
		w->f->names = names;
	}
	w->f->names[w->f->nnames] = SDL_strdup(name);
	if (w->f->names[w->f->nnames] == NULL) {
		w->nomem = 1;
		return SDL_ENUM_FAILURE;
	}
	w->f->nnames++;
	return SDL_ENUM_CONTINUE;
}

static int
namecmp(const void *a, const void *b)
{
	return SDL_strcasecmp(*(char *const *)a, *(char *const *)b);
}

/* fillfind fills fd with the next match of f, and reports whether there was one. */
static int
fillfind(File *f, LPWIN32_FIND_DATAA fd)
{
	char path[Maxpath];
	SDL_PathInfo info;
	const char *name;

	if (f->next >= f->nnames)
		return 0;
	name = f->names[f->next++];
	SDL_snprintf(path, sizeof path, "%s%s", f->path, name);
	memset(fd, 0, sizeof *fd);
	if (SDL_GetPathInfo(path, &info)) {
		fd->dwFileAttributes = info.type == SDL_PATHTYPE_DIRECTORY ?
			FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_ARCHIVE;
		fd->nFileSizeLow = (DWORD)info.size;
		fd->nFileSizeHigh = (DWORD)(info.size >> 32);
		systotime(filetime(path, info.create_time), &fd->ftCreationTime);
		systotime(filetime(path, info.access_time), &fd->ftLastAccessTime);
		systotime(filetime(path, info.modify_time), &fd->ftLastWriteTime);
	}
	SDL_strlcpy(fd->cFileName, name, sizeof fd->cFileName);
	return 1;
}

HANDLE
FindFirstFileA(LPCSTR pattern, LPWIN32_FIND_DATAA fd)
{
	char dir[Maxpath], host[Maxpath];
	const char *pat;
	size_t n;
	File *f;
	Walk w;

	pat = pattern;
	for (n = strlen(pattern); n > 0; n--)
		if (isslash(pattern[n-1]) || pattern[n-1] == ':') {
			pat = pattern + n;
			break;
		}
	if (n >= sizeof dir) {
		syssetlasterror(ERROR_PATH_NOT_FOUND);
		return INVALID_HANDLE_VALUE;
	}
	memcpy(dir, pattern, n);
	dir[n] = '\0';
	if (n == 0)
		SDL_strlcpy(dir, ".", sizeof dir);
	if (!hostpath(dir, host, sizeof host - 1, 0)) {
		syssetlasterror(ERROR_PATH_NOT_FOUND);
		return INVALID_HANDLE_VALUE;
	}
	n = strlen(host);
	if (n > 0 && !isslash(host[n-1]))
		SDL_strlcat(host, "/", sizeof host);
	f = calloc(1, sizeof *f);
	if (f == NULL) {
		syssetlasterror(ERROR_NOT_ENOUGH_MEMORY);
		return INVALID_HANDLE_VALUE;
	}
	f->magic = Filemagic;
	f->kind = Kfind;
	SDL_strlcpy(f->path, host, sizeof f->path);
	w.pat = pat;
	w.f = f;
	w.cap = 0;
	w.nomem = 0;
	SDL_EnumerateDirectory(host, walkone, &w);
	if (w.nomem || f->nnames == 0) {
		freefind(f);
		free(f);
		syssetlasterror(w.nomem ? ERROR_NOT_ENOUGH_MEMORY : ERROR_FILE_NOT_FOUND);
		return INVALID_HANDLE_VALUE;
	}
	qsort(f->names, f->nnames, sizeof f->names[0], namecmp);
	fillfind(f, fd);
	return f;
}

BOOL
FindNextFileA(HANDLE h, LPWIN32_FIND_DATAA fd)
{
	File *f;

	f = fileof(h, Kfind);
	if (f == NULL) {
		syssetlasterror(ERROR_INVALID_HANDLE);
		return FALSE;
	}
	if (!fillfind(f, fd)) {
		syssetlasterror(ERROR_NO_MORE_FILES);
		return FALSE;
	}
	return TRUE;
}

BOOL
FindClose(HANDLE h)
{
	File *f;

	f = fileof(h, Kfind);
	if (f == NULL)
		return FALSE;
	freefind(f);
	f->magic = 0;
	free(f);
	return TRUE;
}

BOOL
CreateDirectoryA(LPCSTR name, LPSECURITY_ATTRIBUTES sa)
{
	char host[Maxpath];
	SDL_PathInfo info;

	(void)sa;
	if (!writable(name) || !hostpath(name, host, sizeof host, 1)) {
		syssetlasterror(ERROR_PATH_NOT_FOUND);
		return FALSE;
	}
	if (SDL_GetPathInfo(host, &info)) {
		syssetlasterror(ERROR_ALREADY_EXISTS);
		return FALSE;
	}
	if (!SDL_CreateDirectory(host)) {
		syssetlasterror(ERROR_ACCESS_DENIED);
		return FALSE;
	}
	return TRUE;
}

static BOOL
removepath(LPCSTR name)
{
	char host[Maxpath];

	if (!writable(name)) {
		syssetlasterror(ERROR_ACCESS_DENIED);
		return FALSE;
	}
	if (!hostpath(name, host, sizeof host, 0)) {
		syssetlasterror(ERROR_FILE_NOT_FOUND);
		return FALSE;
	}
	if (!SDL_RemovePath(host)) {
		syssetlasterror(ERROR_ACCESS_DENIED);
		return FALSE;
	}
	return TRUE;
}

BOOL
DeleteFileA(LPCSTR name)
{
	return removepath(name);
}

/*
 * MoveFileExA gives the file src the name dst in the per-user folder,
 * in lowercase when dst is new. A dst that exists fails the move, as
 * Windows' does, but with MOVEFILE_REPLACE_EXISTING, which puts src in
 * its place in one rename, so that either file stands whole whatever
 * happens. In a replay the file keeps the time of its last write.
 */
BOOL
MoveFileExA(LPCSTR src, LPCSTR dst, DWORD flags)
{
	char from[Maxpath], to[Maxpath];
	int i;

	if (!writable(src) || !writable(dst)) {
		syssetlasterror(ERROR_ACCESS_DENIED);
		return FALSE;
	}
	if (!hostpath(src, from, sizeof from, 0)) {
		syssetlasterror(ERROR_FILE_NOT_FOUND);
		return FALSE;
	}
	if (hostpath(dst, to, sizeof to, 0)) {
		if (!(flags & MOVEFILE_REPLACE_EXISTING)) {
			syssetlasterror(ERROR_ALREADY_EXISTS);
			return FALSE;
		}
	} else if (!hostpath(dst, to, sizeof to, 1)) {
		syssetlasterror(ERROR_PATH_NOT_FOUND);
		return FALSE;
	}
	if (!SDL_RenamePath(from, to)) {
		syssetlasterror(ERROR_ACCESS_DENIED);
		return FALSE;
	}
	for (i = 0; i < nwritten; i++)
		if (SDL_strcasecmp(written[i].path, to) == 0)
			written[i].path[0] = '\0';
	for (i = 0; i < nwritten; i++)
		if (SDL_strcasecmp(written[i].path, from) == 0)
			SDL_strlcpy(written[i].path, to, sizeof written[i].path);
	return TRUE;
}

/* MoveFileA moves as MoveFileExA does without its flags. */
BOOL
MoveFileA(LPCSTR src, LPCSTR dst)
{
	return MoveFileExA(src, dst, 0);
}

BOOL
RemoveDirectoryA(LPCSTR name)
{
	return removepath(name);
}

BOOL
SetCurrentDirectoryA(LPCSTR name)
{
	char parts[Maxparts][MAX_PATH];
	char host[Maxpath], dir[Maxpath];
	SDL_PathInfo info;
	int np, i;

	if (!hostpath(name, host, sizeof host, 0) || !SDL_GetPathInfo(host, &info) ||
	    info.type != SDL_PATHTYPE_DIRECTORY) {
		syssetlasterror(ERROR_PATH_NOT_FOUND);
		return FALSE;
	}
	np = absolute(name, parts, Maxparts);
	SDL_strlcpy(dir, "C:", sizeof dir);
	for (i = 0; i < np; i++) {
		SDL_strlcat(dir, "\\", sizeof dir);
		SDL_strlcat(dir, parts[i], sizeof dir);
	}
	SDL_strlcpy(cwd, dir, sizeof cwd);
	return TRUE;
}

DWORD
GetCurrentDirectoryA(DWORD n, LPSTR buf)
{
	size_t len;

	len = strlen(cwd);
	if (n <= len)
		return (DWORD)len + 1;
	memcpy(buf, cwd, len + 1);
	return (DWORD)len;
}

DWORD
GetModuleFileNameA(HMODULE m, LPSTR buf, DWORD n)
{
	(void)m;
	if (n == 0)
		return 0;
	SDL_strlcpy(buf, exepath, n);
	return (DWORD)strlen(buf);
}

/*
 * locks holds the locks of CreateMutexA, the game's one and a test's
 * second. A closed lock keeps its place, so that a second CloseHandle
 * of it reads a lock that is closed: the game closes its mutex twice
 * when its start fails, as when the window closes at the title.
 */
static File locks[2];

/* nolock answers a CreateMutexA while both locks are open, and CloseHandle passes it by */
static int nolock;

#ifdef _WIN32
/*
 * takelock locks the per-user folder by holding its file instance.lock
 * open alone, and reports whether another run holds it.
 */
static int
takelock(File *f)
{
	wchar_t *w;
	int held;

	SDL_snprintf(f->path, sizeof f->path, "%sinstance.lock", sysuserdir);
	w = (wchar_t *)SDL_iconv_string("UTF-16LE", "UTF-8", f->path, strlen(f->path) + 1);
	if (w == NULL)
		return 0;
	f->fd = _wsopen(w, _O_RDWR | _O_CREAT | _O_BINARY | _O_NOINHERIT, _SH_DENYRW, _S_IREAD | _S_IWRITE);
	held = f->fd < 0 && errno == EACCES;
	SDL_free(w);
	return held;
}

/* droplock ends the lock and removes instance.lock. */
static void
droplock(File *f)
{
	if (f->fd < 0)
		return;
	_close(f->fd);
	SDL_RemovePath(f->path);
}
#else
/*
 * takelock locks the per-user folder itself with flock, which locks per
 * open descriptor and leaves the folder's files as they are, and reports
 * whether another run holds it. The descriptor closes on exec, so the
 * programs that ShellExecuteA starts leave the lock to the game.
 */
static int
takelock(File *f)
{
	int held;

	f->fd = open(sysuserdir, O_RDONLY);
	if (f->fd < 0)
		return 0;
	fcntl(f->fd, F_SETFD, FD_CLOEXEC);
	if (flock(f->fd, LOCK_EX | LOCK_NB) == 0)
		return 0;
	held = errno == EWOULDBLOCK;
	close(f->fd);
	f->fd = -1;
	return held;
}

/* droplock ends the lock. */
static void
droplock(File *f)
{
	if (f->fd >= 0)
		close(f->fd);
}
#endif

/*
 * CreateMutexA holds the game to one run on each per-user folder, as the
 * original's named mutex held it to one on the machine: a second run gets
 * ERROR_ALREADY_EXISTS, and the game says so and stops. A crash ends the
 * lock with the run, and a folder that gives no lock lets the game run.
 */
HANDLE
CreateMutexA(LPSECURITY_ATTRIBUTES sa, BOOL own, LPCSTR name)
{
	File *f;

	(void)sa, (void)own, (void)name;
	for (f = locks; f < locks + SDL_arraysize(locks); f++)
		if (f->magic != Filemagic)
			break;
	if (f == locks + SDL_arraysize(locks)) {
		syssetlasterror(ERROR_SUCCESS);
		return &nolock;
	}
	memset(f, 0, sizeof *f);
	f->magic = Filemagic;
	f->kind = Klock;
	f->fd = -1;
	syssetlasterror(takelock(f) ? ERROR_ALREADY_EXISTS : ERROR_SUCCESS);
	return f;
}

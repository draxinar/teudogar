/*
 * The files of src/sys/file.c on virtual paths: C:\teudogar is the
 * game's folder, read only, and C:\teudogar.sav the per-user folder.
 * Names resolve with case folded, new files take lowercase names,
 * FindFirstFile walks a folder in the order of its names with case
 * folded, and the current folder resolves relative names. A file that
 * an open truncates keeps its bytes until its close, and on Linux a
 * write past a limit of 2 bytes, as on a full disk, keeps them for
 * good, as a write past the room that sysfull gives does everywhere,
 * unless sysinplace sends the writes into the file itself, which then
 * keeps the part that went in, closes as on Windows, and in a replay
 * takes the virtual time of that write.
 * MoveFileA renames a file into a new lowercase name and refuses an
 * existing one, which MoveFileExA replaces with MOVEFILE_REPLACE_EXISTING.
 * SetFileTime dates a file's last write, which
 * FindFirstFile then reports, and in a replay a file reports the
 * virtual time of its write, which it keeps when MoveFileA renames it
 * by a name in another case. CreateMutexA gives a folder to one run at
 * a time. GetUserNameA of src/sys/kernel.c gives the user's name in
 * code page 1252, and GlobalAlloc, LocalAlloc, and GlobalReAlloc refuse
 * a block that their header carries past a size_t of 32 bits. fileurl
 * of src/sys/kernel.c writes a host's path as a file URL, its blanks
 * and other bytes outside a URL percent-encoded. The test
 * makes both folders in the folder that its argument names, and prints
 * each check that fails.
 */

#include <stdio.h>
#include <string.h>

#ifndef _WIN32
#include <signal.h>
#include <sys/resource.h>
#endif

#include <SDL3/SDL.h>
#include <windows.h>
#include <ddraw.h>

#include "sys.h"
#include "test.h"

/* the times that checktime gives, in seconds since 1970: May 5, 2008, and January 1, 2040, past a time_t of 32 bits */
static const Sint64 testtimes[] = { 1210000000, 2208988800 };

/* the user's names in UTF-8, which GetUserNameA gives in code page 1252 */
static const struct {
	const char *user;
	const char *want;
} users[] = {
	{ "Strau\xc3\x9f", "Strau\xdf" },
	{ "\xe2\x82\xac\xc4\x81", "\x80?" },	/* the euro sign, and a letter outside the code page */
	{ "Bad\xff", "Bad?" },	/* a byte outside UTF-8 */
};

/* a block whose size and the header of GlobalAlloc's pass the 32 bits of a size_t */
static const DWORD hugeblock = 0xfffffff8;

/* the paths that checkurl gives fileurl, and the URLs that it wants */
static const struct {
	const char *path;
	const char *want;
} urls[] = {
	{ "/data/teudogar/readme_e.txt", "file:///data/teudogar/readme_e.txt" },
	{ "/Users/a/Library/Application Support/teudogar/x.txt", "file:///Users/a/Library/Application%20Support/teudogar/x.txt" },
	{ "/a/1 - read me (first).html", "file:///a/1%20-%20read%20me%20%28first%29.html" },
	{ "/a/b#c%d?e", "file:///a/b%23c%25d%3Fe" },
	{ "/Jeux vid\xc3\xa9o/x", "file:///Jeux%20vid%C3%A9o/x" },
};

/* the final separator of the per-user folder as SDL_GetPrefPath gives it, which checkreplaytime takes */
#ifdef _WIN32
static const char prefsep = '\\';
#else
static const char prefsep = '/';
#endif

/* put writes s into the host file dir/name. */
static void
put(const char *dir, const char *name, const char *s)
{
	char path[Maxpath];
	SDL_IOStream *io;

	SDL_snprintf(path, sizeof path, "%s%s", dir, name);
	io = SDL_IOFromFile(path, "wb");
	if (io == NULL)
		sysfatal("file: %s: %s", path, SDL_GetError());
	SDL_WriteIO(io, s, strlen(s));
	SDL_CloseIO(io);
}

/* get returns the text of the host file dir/name in buf of n bytes, or "" when it is missing. */
static const char *
get(const char *dir, const char *name, char *buf, size_t n)
{
	char path[Maxpath];
	char *data;

	SDL_snprintf(path, sizeof path, "%s%s", dir, name);
	buf[0] = '\0';
	data = SDL_LoadFile(path, NULL);
	if (data != NULL) {
		SDL_strlcpy(buf, data, n);
		SDL_free(data);
	}
	return buf;
}

/* exists reports whether the host file dir/name exists. */
static int
exists(const char *dir, const char *name)
{
	char path[Maxpath];

	SDL_snprintf(path, sizeof path, "%s%s", dir, name);
	return SDL_GetPathInfo(path, NULL);
}

/* checkreplace replaces save.dat and wants its old bytes until the close. */
static void
checkreplace(void)
{
	char buf[64];
	HANDLE h;
	DWORD n;

	put(sysuserdir, "save.dat", "old");
	h = CreateFileA("C:\\teudogar.sav\\save.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	check(h != INVALID_HANDLE_VALUE, "save.dat opens to be replaced");
	if (h == INVALID_HANDLE_VALUE)
		return;
	check(WriteFile(h, "newer", 5, &n, NULL) && n == 5, "save.dat takes 5 bytes");
	check(strcmp(get(sysuserdir, "save.dat", buf, sizeof buf), "old") == 0,
		"save.dat keeps its old bytes while it is written");
	check(CloseHandle(h), "the close of save.dat succeeds");
	check(strcmp(get(sysuserdir, "save.dat", buf, sizeof buf), "newer") == 0,
		"save.dat holds its new bytes after the close");
}

/* checklock takes the per-user folder twice, as two runs of the game would. */
static void
checklock(void)
{
	HANDLE h, h2;

	h = CreateMutexA(NULL, TRUE, "TeudogarAllianceWithRome");
	check(h != NULL && GetLastError() == ERROR_SUCCESS, "the first run takes the folder");
	h2 = CreateMutexA(NULL, TRUE, "TeudogarAllianceWithRome");
	check(h2 != NULL && GetLastError() == ERROR_ALREADY_EXISTS, "a second run finds the folder taken");
	CloseHandle(h2);
	CloseHandle(h);
	h = CreateMutexA(NULL, TRUE, "TeudogarAllianceWithRome");
	check(h != NULL && GetLastError() == ERROR_SUCCESS, "the folder is free once both runs end");
	CloseHandle(h);
	/* the game's exit after a failed start closes its mutex again */
	CloseHandle(h);
	h = CreateMutexA(NULL, TRUE, "TeudogarAllianceWithRome");
	check(h != NULL && GetLastError() == ERROR_SUCCESS, "a second close leaves the folder free");
	CloseHandle(h);
}

/* checkleft wants the per-user folder free of locks and temporaries. */
static void
checkleft(void)
{
	WIN32_FIND_DATAA fd;
	HANDLE h;
	size_t n;
	int ok;

	ok = 1;
	h = FindFirstFileA("C:\\teudogar.sav\\*.*", &fd);
	while (h != INVALID_HANDLE_VALUE) {
		n = strlen(fd.cFileName);
		if (strcmp(fd.cFileName, "instance.lock") == 0 || (n > 4 && strcmp(fd.cFileName + n - 4, ".tmp") == 0)) {
			printf("file: %s stays in the per-user folder\n", fd.cFileName);
			ok = 0;
		}
		if (!FindNextFileA(h, &fd))
			break;
	}
	if (h != INVALID_HANDLE_VALUE)
		FindClose(h);
	check(ok, "the per-user folder keeps no lock and no temporary");
}

/* checkmove renames files of the per-user folder, and wants an existing name refused but with MOVEFILE_REPLACE_EXISTING, and the game's folder refused. */
static void
checkmove(void)
{
	char buf[64];

	put(sysuserdir, "slot.tmp", "new");
	put(sysuserdir, "keep.sav", "old");
	check(MoveFileA("C:\\teudogar.sav\\SLOT.TMP", "C:\\teudogar.sav\\Moved.SAV"), "SLOT.TMP takes the name Moved.SAV");
	check(!exists(sysuserdir, "slot.tmp") && strcmp(get(sysuserdir, "moved.sav", buf, sizeof buf), "new") == 0,
		"Moved.SAV lands as moved.sav with the bytes of slot.tmp");
	check(!MoveFileA("C:\\teudogar.sav\\moved.sav", "C:\\teudogar.sav\\KEEP.SAV") && GetLastError() == ERROR_ALREADY_EXISTS,
		"a move onto keep.sav fails, as it exists");
	check(strcmp(get(sysuserdir, "keep.sav", buf, sizeof buf), "old") == 0, "keep.sav keeps its bytes");
	put(sysuserdir, "other.sav", "newer");
	check(MoveFileExA("C:\\teudogar.sav\\Other.SAV", "C:\\teudogar.sav\\KEEP.SAV", MOVEFILE_REPLACE_EXISTING),
		"Other.SAV takes the place of keep.sav with MOVEFILE_REPLACE_EXISTING");
	check(!exists(sysuserdir, "other.sav") && strcmp(get(sysuserdir, "keep.sav", buf, sizeof buf), "newer") == 0,
		"keep.sav holds the bytes of other.sav, under its own name");
	check(!MoveFileA("C:\\teudogar.sav\\none.sav", "C:\\teudogar.sav\\other.sav") && GetLastError() == ERROR_FILE_NOT_FOUND,
		"a missing file fails to move");
	check(!MoveFileA("C:\\teudogar.sav\\moved.sav", "C:\\teudogar\\moved.sav") && GetLastError() == ERROR_ACCESS_DENIED,
		"the game's folder refuses a move into it");
	DeleteFileA("C:\\teudogar.sav\\moved.sav");
	DeleteFileA("C:\\teudogar.sav\\keep.sav");
}

/* checkroom writes 16 bytes into save.dat with a room of 4, as on a full disk, and wants the write and the close to fail and the old bytes kept. */
static void
checkroom(void)
{
	char buf[64];
	HANDLE h;
	DWORD n;
	BOOL wrote, closed;

	put(sysuserdir, "save.dat", "old");
	h = CreateFileA("C:\\teudogar.sav\\save.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (h == INVALID_HANDLE_VALUE) {
		check(0, "save.dat opens to be replaced");
		return;
	}
	sysfull(4);
	wrote = WriteFile(h, "0123456789abcdef", 16, &n, NULL);
	check(!wrote && n == 4 && GetLastError() == ERROR_DISK_FULL, "a write past the room fails with the disk full after 4 bytes");
	SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL);
	closed = CloseHandle(h);
	SDL_ResetLogPriorities();
	sysfull(-1);
	check(!closed && GetLastError() == ERROR_WRITE_FAULT, "a save past the room fails at its close");
	check(strcmp(get(sysuserdir, "save.dat", buf, sizeof buf), "old") == 0, "a save past the room keeps the old bytes");
}

/* lastwrite returns the time of the last write that FindFirstFile reports for the file name, or zero for a missing one. */
static FILETIME
lastwrite(const char *name)
{
	WIN32_FIND_DATAA fd;
	FILETIME ft;
	HANDLE h;

	memset(&ft, 0, sizeof ft);
	h = FindFirstFileA(name, &fd);
	if (h != INVALID_HANDLE_VALUE) {
		ft = fd.ftLastWriteTime;
		FindClose(h);
	}
	return ft;
}

/* checktime gives save.dat each time of testtimes as its last write before its close, and wants FindFirstFile to report it and a file open to read to refuse one. */
static void
checktime(void)
{
	FILETIME ft, got;
	HANDLE h;
	DWORD n;
	size_t i;

	for (i = 0; i < sizeof testtimes / sizeof testtimes[0]; i++) {
		systotime((SDL_Time)testtimes[i] * SDL_NS_PER_SECOND, &ft);
		h = CreateFileA("C:\\teudogar.sav\\save.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		if (h == INVALID_HANDLE_VALUE) {
			check(0, "save.dat opens to write");
			return;
		}
		check(WriteFile(h, "new", 3, &n, NULL) && n == 3, "save.dat takes 3 bytes");
		check(SetFileTime(h, NULL, NULL, &ft), "save.dat takes a time of its last write");
		check(CloseHandle(h), "save.dat closes");
		got = lastwrite("C:\\teudogar.sav\\save.dat");
		check(memcmp(&got, &ft, sizeof got) == 0, "FindFirstFile reports the time of each last write, in 2008 and in 2040");
	}
	h = CreateFileA("C:\\teudogar.sav\\save.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	check(h != INVALID_HANDLE_VALUE && !SetFileTime(h, NULL, NULL, &ft) && GetLastError() == ERROR_ACCESS_DENIED,
		"save.dat open to read refuses a time");
	if (h != INVALID_HANDLE_VALUE)
		CloseHandle(h);
	DeleteFileA("C:\\teudogar.sav\\save.dat");
}

/*
 * checkreplaytime writes slot.tmp in a replay 20 virtual seconds after
 * its start, and wants FindFirstFile to report that time for it, and
 * for Moved.SAV once slot.tmp takes that name, as a save of the slots
 * does, with the per-user folder's final separator as SDL_GetPrefPath
 * gives it.
 */
static void
checkreplaytime(void)
{
	FILETIME start, want, got;
	HANDLE h;
	DWORD n;

	sysuserdir[strlen(sysuserdir) - 1] = prefsep;
	replaying = 1;
	fileinit();
	systotime(sysdatetime(), &start);
	replayadvance(20000);
	systotime(sysdatetime(), &want);
	h = CreateFileA("C:\\teudogar.sav\\slot.tmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	check(h != INVALID_HANDLE_VALUE && WriteFile(h, "new", 3, &n, NULL) && CloseHandle(h), "slot.tmp takes 3 bytes in a replay");
	got = lastwrite("C:\\teudogar.sav\\*.tmp");
	check(memcmp(&got, &want, sizeof got) == 0 && memcmp(&got, &start, sizeof got) != 0,
		"FindFirstFile reports the virtual time of slot.tmp's write");
	check(MoveFileA("C:\\teudogar.sav\\SLOT.TMP", "C:\\teudogar.sav\\Moved.SAV"), "SLOT.TMP takes the name Moved.SAV in a replay");
	got = lastwrite("C:\\teudogar.sav\\*.sav");
	check(memcmp(&got, &want, sizeof got) == 0, "Moved.SAV keeps the time of slot.tmp's write");
	DeleteFileA("C:\\teudogar.sav\\moved.sav");
	replaying = 0;
	sysuserdir[strlen(sysuserdir) - 1] = '/';
}

/* checkinplace writes 16 bytes into save.dat with a room of 4 and the writes in place, as on Windows, and wants the file emptied at the open and its first 4 bytes left. */
static void
checkinplace(void)
{
	char buf[64];
	HANDLE h;
	DWORD n;
	BOOL closed;

	put(sysuserdir, "save.dat", "old");
	sysinplace(1);
	h = CreateFileA("C:\\teudogar.sav\\save.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (h == INVALID_HANDLE_VALUE) {
		sysinplace(0);
		check(0, "save.dat opens to be written in place");
		return;
	}
	check(get(sysuserdir, "save.dat", buf, sizeof buf)[0] == '\0', "an open in place empties save.dat at once");
	sysfull(4);
	check(!WriteFile(h, "0123456789abcdef", 16, &n, NULL) && n == 4, "a write in place past the room puts 4 bytes");
	SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL);
	closed = CloseHandle(h);
	SDL_ResetLogPriorities();
	sysfull(-1);
	sysinplace(0);
	check(closed, "a close in place succeeds after a write that failed, as on Windows");
	check(strcmp(get(sysuserdir, "save.dat", buf, sizeof buf), "0123") == 0, "save.dat keeps the 4 bytes that went in place");
	DeleteFileA("C:\\teudogar.sav\\save.dat");
}

/*
 * checkinplacetime writes 10 bytes into save.dat in a replay, and 10
 * virtual minutes later 16 in place with a room of 4, and wants
 * FindFirstFile to report the 4 bytes at the time of the second write,
 * as Windows dates a file that a write changed, whatever the write did.
 */
static void
checkinplacetime(void)
{
	WIN32_FIND_DATAA fd;
	FILETIME want;
	HANDLE h;
	DWORD n;

	replaying = 1;
	h = CreateFileA("C:\\teudogar.sav\\save.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	check(h != INVALID_HANDLE_VALUE && WriteFile(h, "0123456789", 10, &n, NULL) && CloseHandle(h),
		"save.dat takes 10 bytes in a replay");
	replayadvance(600000);
	systotime(sysdatetime(), &want);
	sysinplace(1);
	h = CreateFileA("C:\\teudogar.sav\\save.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (h != INVALID_HANDLE_VALUE) {
		sysfull(4);
		WriteFile(h, "0123456789abcdef", 16, &n, NULL);
		SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL);
		CloseHandle(h);
		SDL_ResetLogPriorities();
		sysfull(-1);
	}
	sysinplace(0);
	h = FindFirstFileA("C:\\teudogar.sav\\save.dat", &fd);
	check(h != INVALID_HANDLE_VALUE && fd.nFileSizeLow == 4 && memcmp(&fd.ftLastWriteTime, &want, sizeof want) == 0,
		"a write in place that fails dates save.dat 10 virtual minutes later, with its 4 bytes");
	if (h != INVALID_HANDLE_VALUE)
		FindClose(h);
	DeleteFileA("C:\\teudogar.sav\\save.dat");
	replaying = 0;
}

/* checkuser gives $USER each name of users, and wants GetUserNameA to give it in code page 1252 and to count its NUL. */
static void
checkuser(void)
{
	char buf[16], what[128], *old;
	const char *env;
	DWORD n, want;
	size_t i;

	env = SDL_getenv("USER");
	old = env != NULL ? SDL_strdup(env) : NULL;
	for (i = 0; i < sizeof users / sizeof users[0]; i++) {
		SDL_setenv_unsafe("USER", users[i].user, 1);
		want = (DWORD)strlen(users[i].want) + 1;
		n = sizeof buf;
		SDL_snprintf(what, sizeof what, "GetUserNameA gives name %d of users in code page 1252", (int)i);
		check(GetUserNameA(buf, &n) && n == want && strcmp(buf, users[i].want) == 0, what);
		n = want - 1;
		SDL_snprintf(what, sizeof what, "GetUserNameA wants room for name %d of users in code page 1252 and its NUL", (int)i);
		check(!GetUserNameA(buf, &n) && GetLastError() == ERROR_INSUFFICIENT_BUFFER && n == want, what);
	}
	if (old != NULL)
		SDL_setenv_unsafe("USER", old, 1);
	else
		SDL_unsetenv_unsafe("USER");
	SDL_free(old);
}

/* checkurl wants fileurl to write each path of urls percent-encoded, and a URL that fills its buffer, and to refuse one a byte longer. */
static void
checkurl(void)
{
	char buf[128], what[160];
	size_t i;

	for (i = 0; i < sizeof urls / sizeof urls[0]; i++) {
		SDL_snprintf(what, sizeof what, "fileurl gives %s", urls[i].want);
		check(fileurl(buf, sizeof buf, urls[i].path) && strcmp(buf, urls[i].want) == 0, what);
	}
	check(fileurl(buf, 14, "/a b") && strcmp(buf, "file:///a%20b") == 0, "fileurl writes file:///a%20b in 14 bytes");
	check(!fileurl(buf, 13, "/a b"), "fileurl refuses file:///a%20b in 13 bytes");
	check(!fileurl(buf, 7, ""), "fileurl refuses file:// in 7 bytes");
}

/* checkmemory wants GlobalAlloc, LocalAlloc, and GlobalReAlloc to refuse hugeblock where a size_t holds 32 bits, as Win32 refuses a block past the address space. */
static void
checkmemory(void)
{
	HGLOBAL h, r;

	if (SIZE_MAX != UINT32_MAX)
		return;
	check(GlobalAlloc(0, hugeblock) == NULL, "GlobalAlloc refuses 0xfffffff8 bytes");
	check(LocalAlloc(0, hugeblock) == NULL, "LocalAlloc refuses 0xfffffff8 bytes");
	h = GlobalAlloc(0, 4);
	r = GlobalReAlloc(h, hugeblock, 0);
	check(h != NULL && r == NULL, "GlobalReAlloc refuses 0xfffffff8 bytes and keeps the block");
	GlobalFree(r != NULL ? r : h);
}

#ifndef _WIN32
/* checkfull writes save.dat past a limit of 2 bytes of a file, as on a full disk, and wants the old bytes kept. */
static void
checkfull(void)
{
	static char big[65536];
	static const DWORD sizes[] = { 16, sizeof big };
	struct rlimit old, lim;
	char buf[64];
	HANDLE h;
	DWORD n;
	BOOL closed;
	size_t i;

	signal(SIGXFSZ, SIG_IGN);
	if (getrlimit(RLIMIT_FSIZE, &old) != 0)
		sysfatal("file: getrlimit failed");
	for (i = 0; i < sizeof sizes / sizeof sizes[0]; i++) {
		put(sysuserdir, "save.dat", "old");
		h = CreateFileA("C:\\teudogar.sav\\save.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		if (h == INVALID_HANDLE_VALUE) {
			check(0, "save.dat opens to be replaced");
			continue;
		}
		/* the test's own log is a file, which the limit holds too */
		fflush(stdout);
		SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL);
		lim = old;
		lim.rlim_cur = 2;
		setrlimit(RLIMIT_FSIZE, &lim);
		WriteFile(h, big, sizes[i], &n, NULL);
		closed = CloseHandle(h);
		setrlimit(RLIMIT_FSIZE, &old);
		SDL_ResetLogPriorities();
		check(!closed && GetLastError() == ERROR_WRITE_FAULT, "a save past the disk's room fails at its close");
		check(strcmp(get(sysuserdir, "save.dat", buf, sizeof buf), "old") == 0, "a failed save keeps the old bytes");
	}
}
#endif

int
main(int argc, char *argv[])
{
	static const char *const order[] = { "A.dat", "b.dat", "Data.DAT" };
	WIN32_FIND_DATAA fd;
	HANDLE h;
	char buf[64];
	DWORD n;
	int i;

	if (argc != 2) {
		fprintf(stderr, "usage: file dir\n");
		return 2;
	}
	SDL_snprintf(sysgamedir, sizeof sysgamedir, "%s/game/", argv[1]);
	SDL_snprintf(sysuserdir, sizeof sysuserdir, "%s/user/", argv[1]);
	if (!SDL_CreateDirectory(sysgamedir) || !SDL_CreateDirectory(sysuserdir))
		sysfatal("file: %s: %s", argv[1], SDL_GetError());
	put(sysgamedir, "Data.DAT", "abc");
	put(sysgamedir, "b.dat", "b");
	put(sysgamedir, "A.dat", "a");
	put(sysgamedir, "readme.TXT", "r");
	fileinit();

	n = GetCurrentDirectoryA(sizeof buf, buf);
	check(n > 0 && strcmp(buf, "C:\\teudogar") == 0, "the current folder starts at C:\\teudogar");

	h = CreateFileA("C:\\teudogar\\data.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	check(h != INVALID_HANDLE_VALUE, "data.dat opens Data.DAT, with case folded");
	if (h != INVALID_HANDLE_VALUE) {
		memset(buf, 0, sizeof buf);
		check(ReadFile(h, buf, sizeof buf, &n, NULL) && n == 3 && memcmp(buf, "abc", 3) == 0,
			"Data.DAT reads abc");
		check(GetFileSize(h, NULL) == 3, "Data.DAT holds 3 bytes");
		check(SetFilePointer(h, 1, NULL, FILE_BEGIN) == 1, "the pointer moves to 1");
		check(ReadFile(h, buf, 1, &n, NULL) && n == 1 && buf[0] == 'b', "the byte at 1 is b");
		CloseHandle(h);
	}
	h = CreateFileA("readme.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	check(h != INVALID_HANDLE_VALUE, "a relative name opens in the current folder");
	if (h != INVALID_HANDLE_VALUE)
		CloseHandle(h);

	h = FindFirstFileA("C:\\teudogar\\*.dat", &fd);
	check(h != INVALID_HANDLE_VALUE, "*.dat finds the data files");
	for (i = 0; h != INVALID_HANDLE_VALUE && i < 3; i++) {
		check(strcmp(fd.cFileName, order[i]) == 0, "*.dat walks A.dat, b.dat, Data.DAT in that order");
		if (!FindNextFileA(h, &fd))
			break;
	}
	check(i == 2, "*.dat finds three files");
	if (h != INVALID_HANDLE_VALUE)
		FindClose(h);

	h = CreateFileA("C:\\teudogar\\new.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	check(h == INVALID_HANDLE_VALUE, "the game's folder refuses a new file");
	if (h != INVALID_HANDLE_VALUE)
		CloseHandle(h);

	check(SetCurrentDirectoryA("C:\\teudogar.sav"), "the current folder moves to C:\\teudogar.sav");
	h = CreateFileA("New.TXT", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	check(h != INVALID_HANDLE_VALUE, "New.TXT opens in the per-user folder");
	if (h != INVALID_HANDLE_VALUE) {
		check(WriteFile(h, "xyz", 3, &n, NULL) && n == 3, "New.TXT takes 3 bytes");
		CloseHandle(h);
	}
	check(exists(sysuserdir, "new.txt"), "New.TXT lands as new.txt");
	check(DeleteFileA("NEW.txt"), "NEW.txt deletes new.txt");
	check(!exists(sysuserdir, "new.txt"), "new.txt is gone");

	check(CreateDirectoryA("C:\\teudogar.sav\\Sub", NULL), "a folder opens in the per-user folder");
	check(exists(sysuserdir, "sub"), "Sub lands as sub");
	check(RemoveDirectoryA("C:\\teudogar.sav\\sub"), "sub goes away");

	checkreplace();
	checklock();
	checkmove();
	checkroom();
	checkinplace();
	checktime();
	checkreplaytime();
	checkinplacetime();
	checkuser();
	checkmemory();
	checkurl();
#ifndef _WIN32
	checkfull();
#endif
	checkleft();
	return failed;
}

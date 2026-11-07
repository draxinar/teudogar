/*
 * KERNEL32's time, memory, locale, and errors, and the few calls of
 * ADVAPI32, SHELL32, and OLE32 that the game makes outside the
 * registry, for the port.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <windows.h>
#include <shlobj.h>

#include "sys.h"

enum {
	/* timeGetTime counts from here, as on a machine up for hours, since
	 * the game takes a time of 0 for unset */
	Tickbase = 10000000,
	Heapmagic = 0x48656170
};

const CLSID CLSID_ShellLink = { 0x00021401, 0, 0, { 0xc0, 0, 0, 0, 0, 0, 0, 0x46 } };
const IID IID_IShellLinkA = { 0x000214ee, 0, 0, { 0xc0, 0, 0, 0, 0, 0, 0, 0x46 } };

static DWORD lasterror;

/* sysclock returns the milliseconds of timeGetTime. */
DWORD
sysclock(void)
{
	if (replaying)
		return replayclock();
	return (DWORD)(Tickbase + SDL_GetTicks());
}

/* sysdelay waits ms milliseconds, or moves the virtual clock by them. */
void
sysdelay(DWORD ms)
{
	if (replaying)
		replayadvance(ms);
	else
		SDL_Delay(ms);
}

DWORD
timeGetTime(void)
{
	return sysclock();
}

/* timeBeginPeriod returns at once, since SDL's clock counts single milliseconds. */
MMRESULT
timeBeginPeriod(UINT ms)
{
	(void)ms;
	return 0;
}

void
Sleep(DWORD ms)
{
	mididrain();
	sysdelay(ms);
}

/* sysdatetime returns the time of day: the host's, or the virtual clock's in a replay. */
SDL_Time
sysdatetime(void)
{
	SDL_Time t;

	if (replaying)
		return (SDL_Time)(1210000000 + (Sint64)replayclock() / 1000) * SDL_NS_PER_SECOND;
	if (!SDL_GetCurrentTime(&t))
		t = 0;
	return t;
}

/*
 * sysnow returns the local time: the host's, or in a replay the virtual
 * clock's in UTC, so that a replay leaves the same times in any zone.
 */
void
sysnow(SYSTEMTIME *st)
{
	SDL_Time t;
	SDL_DateTime dt;

	t = sysdatetime();
	if (!SDL_TimeToDateTime(t, &dt, !replaying))
		memset(&dt, 0, sizeof dt);
	st->wYear = dt.year;
	st->wMonth = dt.month;
	st->wDayOfWeek = dt.day_of_week;
	st->wDay = dt.day;
	st->wHour = dt.hour;
	st->wMinute = dt.minute;
	st->wSecond = dt.second;
	st->wMilliseconds = dt.nanosecond / 1000000;
}

void
GetLocalTime(LPSYSTEMTIME st)
{
	sysnow(st);
}

/* systotime converts an SDL time into a FILETIME in UTC. */
void
systotime(SDL_Time t, FILETIME *ft)
{
	Uint32 lo, hi;

	SDL_TimeToWindows(t, &lo, &hi);
	ft->dwLowDateTime = lo;
	ft->dwHighDateTime = hi;
}

/* FileTimeToLocalFileTime adds the offset of sysnow's zone, none in a replay. */
BOOL
FileTimeToLocalFileTime(const FILETIME *ft, LPFILETIME local)
{
	SDL_Time t;
	SDL_DateTime dt;
	uint64_t v;

	t = SDL_TimeFromWindows(ft->dwLowDateTime, ft->dwHighDateTime);
	if (!SDL_TimeToDateTime(t, &dt, !replaying))
		return FALSE;
	v = ((uint64_t)ft->dwHighDateTime << 32 | ft->dwLowDateTime) +
		(int64_t)dt.utc_offset * 10000000;
	local->dwLowDateTime = (DWORD)v;
	local->dwHighDateTime = (DWORD)(v >> 32);
	return TRUE;
}

BOOL
FileTimeToSystemTime(const FILETIME *ft, LPSYSTEMTIME st)
{
	SDL_Time t;
	SDL_DateTime dt;

	t = SDL_TimeFromWindows(ft->dwLowDateTime, ft->dwHighDateTime);
	if (!SDL_TimeToDateTime(t, &dt, false))
		return FALSE;
	st->wYear = dt.year;
	st->wMonth = dt.month;
	st->wDayOfWeek = dt.day_of_week;
	st->wDay = dt.day;
	st->wHour = dt.hour;
	st->wMinute = dt.minute;
	st->wSecond = dt.second;
	st->wMilliseconds = dt.nanosecond / 1000000;
	return TRUE;
}

BOOL
FileTimeToDosDateTime(const FILETIME *ft, LPWORD date, LPWORD time)
{
	SYSTEMTIME st;

	if (!FileTimeToSystemTime(ft, &st) || st.wYear < 1980 || st.wYear > 2107)
		return FALSE;
	*date = (WORD)((st.wYear - 1980) << 9 | st.wMonth << 5 | st.wDay);
	*time = (WORD)(st.wHour << 11 | st.wMinute << 5 | st.wSecond / 2);
	return TRUE;
}

/*
 * Global and local memory: a handle is the block's address, which a
 * header before it sizes. Every block starts zeroed, and so does what
 * a reallocation adds, so that every byte reads the same in every build.
 */
typedef struct Heap Heap;
struct Heap {
	uint32_t magic;
	size_t size;
	double align;	/* aligns the block that follows */
};

static void *
heapalloc(size_t n)
{
	Heap *h;

	if (n > SIZE_MAX - sizeof *h)
		return NULL;
	h = calloc(1, sizeof *h + n);
	if (h == NULL)
		return NULL;
	h->magic = Heapmagic;
	h->size = n;
	return h + 1;
}

static Heap *
heapof(void *p)
{
	Heap *h;

	if (p == NULL)
		return NULL;
	h = (Heap *)p - 1;
	if (h->magic != Heapmagic)
		sysfatal("free of a block that GlobalAlloc never gave");
	return h;
}

static void *
heaprealloc(void *p, size_t n)
{
	Heap *h, *nh;

	if (p == NULL)
		return heapalloc(n);
	h = heapof(p);
	if (n > SIZE_MAX - sizeof *nh)
		return NULL;
	nh = realloc(h, sizeof *nh + n);
	if (nh == NULL)
		return NULL;
	if (n > nh->size)
		memset((char *)(nh + 1) + nh->size, 0, n - nh->size);
	nh->size = n;
	return nh + 1;
}

static void
heapfree(void *p)
{
	Heap *h;

	h = heapof(p);
	if (h != NULL) {
		h->magic = 0;
		free(h);
	}
}

HGLOBAL
GlobalAlloc(UINT flags, DWORD n)
{
	(void)flags;
	return heapalloc(n);
}

HGLOBAL
GlobalFree(HGLOBAL h)
{
	heapfree(h);
	return NULL;
}

HGLOBAL
GlobalHandle(LPCVOID p)
{
	return (HGLOBAL)p;
}

LPVOID
GlobalLock(HGLOBAL h)
{
	return h;
}

HGLOBAL
GlobalReAlloc(HGLOBAL h, DWORD n, UINT flags)
{
	(void)flags;
	return heaprealloc(h, n);
}

BOOL
GlobalUnlock(HGLOBAL h)
{
	(void)h;
	return TRUE;
}

HLOCAL
LocalAlloc(UINT flags, UINT n)
{
	(void)flags;
	return heapalloc(n);
}

HLOCAL
LocalFree(HLOCAL h)
{
	heapfree(h);
	return NULL;
}

LPVOID
LocalLock(HLOCAL h)
{
	return h;
}

HLOCAL
LocalReAlloc(HLOCAL h, UINT n, UINT flags)
{
	(void)flags;
	return heaprealloc(h, n);
}

BOOL
LocalUnlock(HLOCAL h)
{
	(void)h;
	return TRUE;
}

/*
 * GlobalMemoryStatus reports 2 GB, memory enough for every cache of the
 * game, on every host alike.
 */
void
GlobalMemoryStatus(LPMEMORYSTATUS ms)
{
	DWORD total;

	total = 0x80000000;
	ms->dwLength = sizeof *ms;
	ms->dwMemoryLoad = 50;
	ms->dwTotalPhys = total;
	ms->dwAvailPhys = total / 2;
	ms->dwTotalPageFile = total;
	ms->dwAvailPageFile = total / 2;
	ms->dwTotalVirtual = 0x7ffe0000;
	ms->dwAvailVirtual = 0x7ff00000;
}

BOOL
GetVersionExA(LPOSVERSIONINFOA vi)
{
	vi->dwMajorVersion = 6;
	vi->dwMinorVersion = 1;
	vi->dwBuildNumber = 7601;
	vi->dwPlatformId = VER_PLATFORM_WIN32_NT;
	snprintf(vi->szCSDVersion, sizeof vi->szCSDVersion, "SDL %d.%d.%d",
		SDL_VERSIONNUM_MAJOR(SDL_GetVersion()), SDL_VERSIONNUM_MINOR(SDL_GetVersion()),
		SDL_VERSIONNUM_MICRO(SDL_GetVersion()));
	return TRUE;
}

void
GetSystemInfo(LPSYSTEM_INFO si)
{
	memset(si, 0, sizeof *si);
	si->dwPageSize = 4096;
	si->dwNumberOfProcessors = SDL_GetNumLogicalCPUCores();
	si->dwProcessorType = PROCESSOR_INTEL_PENTIUM;
	si->dwAllocationGranularity = 65536;
	si->wProcessorLevel = 6;
}

/*
 * The languages of SDL's locales as Windows' primary language ids, and
 * the English names of the countries that the game may ask for, of
 * which it tests "United States".
 */
static const struct {
	const char *code;
	WORD lang;
} langs[] = {
	{ "ca", LANG_CATALAN },
	{ "de", LANG_GERMAN },
	{ "en", LANG_ENGLISH },
	{ "es", LANG_SPANISH },
	{ "eu", LANG_BASQUE },
	{ "fr", LANG_FRENCH },
	{ "it", LANG_ITALIAN },
	{ "nl", LANG_DUTCH },
	{ "pl", LANG_POLISH },
	{ "pt", LANG_PORTUGUESE },
	{ "ro", LANG_ROMANIAN },
};

static const struct {
	const char *code;
	const char *name;
} countries[] = {
	{ "AT", "Austria" },
	{ "AU", "Australia" },
	{ "BE", "Belgium" },
	{ "CA", "Canada" },
	{ "CH", "Switzerland" },
	{ "DE", "Germany" },
	{ "ES", "Spain" },
	{ "FR", "France" },
	{ "GB", "United Kingdom" },
	{ "IE", "Ireland" },
	{ "IT", "Italy" },
	{ "LU", "Luxembourg" },
	{ "NL", "Netherlands" },
	{ "PL", "Poland" },
	{ "US", "United States" },
};

/*
 * syslocale returns the language of the first locale, and its country
 * in country. A replay runs in English and in Great Britain, whatever
 * the host's locale.
 */
char *
syslocale(char *country, size_t n)
{
	static char language[8];
	SDL_Locale **locs;
	int count;

	SDL_strlcpy(language, "en", sizeof language);
	if (country != NULL && n > 0)
		country[0] = '\0';
	if (replaying) {
		if (country != NULL)
			SDL_strlcpy(country, "GB", n);
		return language;
	}
	locs = SDL_GetPreferredLocales(&count);
	if (locs == NULL)
		return language;
	if (count > 0) {
		SDL_strlcpy(language, locs[0]->language, sizeof language);
		if (country != NULL && locs[0]->country != NULL)
			SDL_strlcpy(country, locs[0]->country, n);
	}
	SDL_free(locs);
	return language;
}

LANGID
GetUserDefaultLangID(void)
{
	const char *code;
	size_t i;

	code = syslocale(NULL, 0);
	for (i = 0; i < sizeof langs / sizeof langs[0]; i++)
		if (SDL_strcasecmp(code, langs[i].code) == 0)
			return MAKELANGID(langs[i].lang, SUBLANG_DEFAULT);
	return MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT);
}

int
GetLocaleInfoA(LCID lcid, LCTYPE type, LPSTR buf, int n)
{
	char country[8];
	const char *name;
	size_t i;

	(void)lcid;
	if (type != LOCALE_SENGCOUNTRY || n <= 0)
		return 0;
	syslocale(country, sizeof country);
	name = country;
	for (i = 0; i < sizeof countries / sizeof countries[0]; i++)
		if (SDL_strcasecmp(country, countries[i].code) == 0)
			name = countries[i].name;
	SDL_strlcpy(buf, name, n);
	return (int)strlen(buf) + 1;
}

/*
 * GetUserNameA gives the user's name in code page 1252, with ? for a
 * character outside it, as Windows converts it, and its count includes
 * the NUL. In a replay it returns ERROR_FILE_NOT_FOUND, so that the
 * hero takes the game's own name.
 */
BOOL
GetUserNameA(LPSTR buf, LPDWORD n)
{
	const char *name;
	size_t len;
	int c;

	name = NULL;
	if (!replaying) {
		name = SDL_getenv("USER");
		if (name == NULL || name[0] == '\0')
			name = SDL_getenv("USERNAME");
	}
	if (name == NULL || name[0] == '\0') {
		lasterror = ERROR_FILE_NOT_FOUND;
		return FALSE;
	}
	len = SDL_utf8strlen(name) + 1;
	if (len > *n) {
		*n = (DWORD)len;
		lasterror = ERROR_INSUFFICIENT_BUFFER;
		return FALSE;
	}
	while (*name != '\0') {
		c = tocp1252(SDL_StepUTF8(&name, NULL));
		*buf++ = c != 0 ? (char)c : '?';
	}
	*buf = '\0';
	*n = (DWORD)len;
	return TRUE;
}

BOOL
CreateProcessA(LPCSTR app, LPSTR cmd, LPSECURITY_ATTRIBUTES pa, LPSECURITY_ATTRIBUTES ta,
	BOOL inherit, DWORD flags, LPVOID env, LPCSTR dir, LPSTARTUPINFOA si,
	LPPROCESS_INFORMATION pi)
{
	(void)app, (void)cmd, (void)pa, (void)ta, (void)inherit, (void)flags;
	(void)env, (void)dir, (void)si, (void)pi;
	lasterror = ERROR_CALL_NOT_IMPLEMENTED;
	return FALSE;
}

/* urlchar reports whether a file URL holds the byte c as it is. */
static int
urlchar(int c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
		(c != '\0' && strchr("-._~/:", c) != NULL);
}

/*
 * fileurl writes into url, of n bytes, the file URL of the host's path,
 * with each byte other than the unreserved characters of RFC 3986, the
 * slash, and the colon percent-encoded, so that macOS's CFURL, which
 * SDL_OpenURL hands the URL, takes a path with a blank, as that of
 * ~/Library/Application Support. It reports whether the URL fits.
 */
int
fileurl(char *url, size_t n, const char *path)
{
	static const char hex[] = "0123456789ABCDEF";
	static const char scheme[] = "file://";
	const unsigned char *p;
	size_t i, need;

	if (n < sizeof scheme)
		return 0;
	memcpy(url, scheme, sizeof scheme - 1);
	i = sizeof scheme - 1;
	for (p = (const unsigned char *)path; *p != '\0'; p++) {
		/* need bytes for the byte, and one more for the NUL */
		need = urlchar(*p) ? 1 : 3;
		if (n - i <= need)
			return 0;
		if (urlchar(*p)) {
			url[i++] = *p;
		} else {
			url[i++] = '%';
			url[i++] = hex[*p >> 4];
			url[i++] = hex[*p & 0xf];
		}
	}
	url[i] = '\0';
	return 1;
}

/*
 * ShellExecuteA opens a URL in the browser, or a file of the game's
 * folders in the program that shows its kind. A result above 32 means
 * success. Windows' shell takes the file's path in its URL as it is.
 */
HINSTANCE
ShellExecuteA(HWND w, LPCSTR op, LPCSTR file, LPCSTR params, LPCSTR dir, int show)
{
	char host[Maxpath], url[3 * Maxpath + 16];
	char *u;
	int ok;

	(void)w, (void)op, (void)params, (void)dir, (void)show;
	if (SDL_strncasecmp(file, "http:", 5) == 0 || SDL_strncasecmp(file, "https:", 6) == 0) {
		u = winutf8(file);
		ok = SDL_OpenURL(u);
		SDL_free(u);
	} else {
		if (!hostpath(file, host, sizeof host, 0))
			return (HINSTANCE)(UINT_PTR)ERROR_FILE_NOT_FOUND;
#ifdef _WIN32
		snprintf(url, sizeof url, "file://%s", host);
#else
		if (!fileurl(url, sizeof url, host))
			return (HINSTANCE)(UINT_PTR)ERROR_FILE_NOT_FOUND;
#endif
		ok = SDL_OpenURL(url);
	}
	return (HINSTANCE)(UINT_PTR)(ok ? 33 : ERROR_FILE_NOT_FOUND);
}

/* The shell's folders and links serve the setup alone, which the port skips. */
HRESULT
SHGetSpecialFolderLocation(HWND w, int csidl, LPITEMIDLIST *pidl)
{
	(void)w, (void)csidl;
	*pidl = NULL;
	return E_FAIL;
}

BOOL
SHGetPathFromIDListA(LPCITEMIDLIST pidl, LPSTR buf)
{
	(void)pidl;
	buf[0] = '\0';
	return FALSE;
}

LPITEMIDLIST
SHBrowseForFolderA(LPBROWSEINFOA bi)
{
	(void)bi;
	return NULL;
}

HRESULT
SHGetMalloc(LPMALLOC *m)
{
	*m = NULL;
	return E_FAIL;
}

HRESULT
CoInitialize(LPVOID p)
{
	(void)p;
	return S_OK;
}

void
CoUninitialize(void)
{
}

HRESULT
CoCreateInstance(REFCLSID cls, LPUNKNOWN outer, DWORD ctx, REFIID iid, LPVOID *p)
{
	(void)cls, (void)outer, (void)ctx, (void)iid;
	*p = NULL;
	return E_FAIL;
}

int
MulDiv(int a, int b, int c)
{
	int64_t p, q;

	if (c == 0)
		return -1;
	p = (int64_t)a * b;
	if ((p < 0) != (c < 0))
		q = (p - c / 2) / c;
	else
		q = (p + c / 2) / c;
	if (q > INT32_MAX || q < INT32_MIN)
		return -1;
	return (int)q;
}

int
MultiByteToWideChar(UINT cp, DWORD flags, LPCSTR s, int n, LPWSTR w, int wn)
{
	int i;

	(void)cp, (void)flags;
	if (n < 0)
		n = (int)strlen(s) + 1;
	if (wn == 0)
		return n;
	for (i = 0; i < n && i < wn; i++)
		w[i] = (unsigned char)s[i];
	return i;
}

LPSTR
lstrcpyA(LPSTR dst, LPCSTR src)
{
	return strcpy(dst, src);
}

int
lstrlenA(LPCSTR s)
{
	return s == NULL ? 0 : (int)strlen(s);
}

DWORD
sysrotl(DWORD v, int n)
{
	uint32_t x;

	x = (uint32_t)v;
	n &= 31;
	return n == 0 ? x : (x << n | x >> (32 - n));
}

DWORD
sysrotr(DWORD v, int n)
{
	uint32_t x;

	x = (uint32_t)v;
	n &= 31;
	return n == 0 ? x : (x >> n | x << (32 - n));
}

void
fwdcopy(void *dst, const void *src, size_t n)
{
	unsigned char *d;
	const unsigned char *s;

	d = dst;
	s = src;
	while (n-- > 0)
		*d++ = *s++;
}

DWORD
GetLastError(void)
{
	return lasterror;
}

void
SetLastError(DWORD err)
{
	lasterror = err;
}

void
syssetlasterror(DWORD err)
{
	lasterror = err;
}

DWORD
FormatMessageA(DWORD flags, LPCVOID src, DWORD id, DWORD lang, LPSTR buf, DWORD n, va_list *args)
{
	char text[64];
	char **out;
	size_t len;

	(void)src, (void)lang, (void)args;
	len = (size_t)snprintf(text, sizeof text, "error %lu", (unsigned long)id);
	if (flags & FORMAT_MESSAGE_ALLOCATE_BUFFER) {
		out = (char **)buf;
		*out = LocalAlloc(0, (UINT)len + 1);
		if (*out == NULL)
			return 0;
		memcpy(*out, text, len + 1);
		return (DWORD)len;
	}
	if (n == 0)
		return 0;
	SDL_strlcpy(buf, text, n);
	return (DWORD)strlen(buf);
}

DWORD
GetEnvironmentVariableA(LPCSTR name, LPSTR buf, DWORD n)
{
	(void)name, (void)buf, (void)n;
	lasterror = ERROR_FILE_NOT_FOUND;
	return 0;
}

UINT
GetWindowsDirectoryA(LPSTR buf, UINT n)
{
	SDL_strlcpy(buf, "C:\\Windows", n);
	return 10;
}

BOOL
GetVolumeInformationA(LPCSTR root, LPSTR name, DWORD nn, LPDWORD serial, LPDWORD maxlen,
	LPDWORD flags, LPSTR fs, DWORD fsn)
{
	(void)root, (void)name, (void)nn, (void)serial, (void)maxlen, (void)flags;
	(void)fs, (void)fsn;
	lasterror = ERROR_CALL_NOT_IMPLEMENTED;
	return FALSE;
}

BOOL
GetDiskFreeSpaceA(LPCSTR root, LPDWORD spc, LPDWORD bps, LPDWORD freec, LPDWORD totalc)
{
	(void)root;
	*spc = 8;
	*bps = 512;
	*freec = 0x100000;
	*totalc = 0x200000;
	return TRUE;
}

BOOL
GetDiskFreeSpaceExA(LPCSTR root, PULARGE_INTEGER avail, PULARGE_INTEGER total, PULARGE_INTEGER freeb)
{
	(void)root;
	if (avail != NULL)
		avail->QuadPart = (DWORDLONG)4 << 30;
	if (total != NULL)
		total->QuadPart = (DWORDLONG)8 << 30;
	if (freeb != NULL)
		freeb->QuadPart = (DWORDLONG)4 << 30;
	return TRUE;
}

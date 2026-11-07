/*
 * The registry of ADVAPI32 for the port: the values the game keeps
 * under its key, in registry.txt of the per-user folder, one to a
 * line of five fields separated by tabs: the hive, the key, the name,
 * the type (sz, dword, or binary), and the data, as text, decimal, or
 * hex. reginit writes the values of a first run, the ones that the
 * installer and the setup would, so that the game skips its setup:
 * the language of the system's locale, the saved games in
 * C:\teudogar.sav, the licence agreed, and the installer's mark of a
 * first install. The port keeps its own settings under the subkey
 * Port of the game's key.
 */

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <windows.h>

#include "sys.h"

enum {
	Regmagic = 0x5265674b,
	Maxkey = 256,
	Maxname = 64,
	Maxdata = 1024,
	Maxvalues = 64
};

static const char gamekey[] = "Software\\Wolf Mittag\\Teudogar and the Alliance with Rome";
static const char portkey[] = "Software\\Wolf Mittag\\Teudogar and the Alliance with Rome\\Port";

typedef struct Value Value;
struct Value {
	int hive;	/* 1 for HKLM, 0 for HKCU */
	char key[Maxkey];
	char name[Maxname];
	DWORD type;
	BYTE data[Maxdata];
	DWORD size;
};

typedef struct Key Key;
struct Key {
	uint32_t magic;
	int hive;
	char key[Maxkey];
};

static Value values[Maxvalues];
static int nvalues;
static char regpath[Maxpath];

static int
hiveof(HKEY h)
{
	return h == HKEY_LOCAL_MACHINE;
}

static Value *
find(int hive, const char *key, const char *name)
{
	int i;

	for (i = 0; i < nvalues; i++)
		if (values[i].hive == hive && SDL_strcasecmp(values[i].key, key) == 0 &&
		    SDL_strcasecmp(values[i].name, name) == 0)
			return &values[i];
	return NULL;
}

/* szlen returns the length of the string in v's data, at most its size. */
static int
szlen(const Value *v)
{
	DWORD n;

	for (n = 0; n < v->size && v->data[n] != '\0'; n++)
		;
	return (int)n;
}

/* writef writes into io as SDL_IOprintf does, and clears *ok when the write falls short. */
static void
writef(SDL_IOStream *io, int *ok, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	if (SDL_IOvprintf(io, fmt, ap) == 0)
		*ok = 0;
	va_end(ap);
}

/*
 * save writes every value into registry.txt.tmp, which replaces
 * registry.txt when every write and the close succeeded.
 */
static void
save(void)
{
	char tmp[Maxpath];
	SDL_IOStream *io;
	DWORD i;
	int v, ok;

	SDL_snprintf(tmp, sizeof tmp, "%s.tmp", regpath);
	io = SDL_IOFromFile(tmp, "w");
	if (io == NULL) {
		SDL_Log("teudogar: %s: %s", tmp, SDL_GetError());
		return;
	}
	ok = 1;
	for (v = 0; v < nvalues; v++) {
		writef(io, &ok, "%s\t%s\t%s\t", values[v].hive ? "HKLM" : "HKCU", values[v].key, values[v].name);
		switch (values[v].type) {
		case REG_DWORD:
			writef(io, &ok, "dword\t%lu\n", (unsigned long)(values[v].data[0] |
				values[v].data[1] << 8 | values[v].data[2] << 16 | (DWORD)values[v].data[3] << 24));
			break;
		case REG_SZ:
			writef(io, &ok, "sz\t%.*s\n", szlen(&values[v]), (char *)values[v].data);
			break;
		default:
			writef(io, &ok, "binary\t");
			for (i = 0; i < values[v].size; i++)
				writef(io, &ok, "%02x", values[v].data[i]);
			writef(io, &ok, "\n");
			break;
		}
	}
	if (!SDL_CloseIO(io) || !ok || !SDL_RenamePath(tmp, regpath)) {
		SDL_Log("teudogar: %s: the write failed", regpath);
		SDL_RemovePath(tmp);
	}
}

static Value *
set(int hive, const char *key, const char *name, DWORD type, const BYTE *data, DWORD size)
{
	Value *v;

	if (size > Maxdata)
		return NULL;
	v = find(hive, key, name);
	if (v == NULL) {
		if (nvalues == Maxvalues)
			return NULL;
		v = &values[nvalues++];
		v->hive = hive;
		SDL_strlcpy(v->key, key, sizeof v->key);
		SDL_strlcpy(v->name, name, sizeof v->name);
	}
	v->type = type;
	memcpy(v->data, data, size);
	v->size = size;
	return v;
}

static void
setdw(int hive, const char *key, const char *name, DWORD d)
{
	BYTE b[4];

	b[0] = d & 0xff;
	b[1] = d >> 8 & 0xff;
	b[2] = d >> 16 & 0xff;
	b[3] = d >> 24 & 0xff;
	set(hive, key, name, REG_DWORD, b, 4);
}

static void
setsz(int hive, const char *key, const char *name, const char *s)
{
	set(hive, key, name, REG_SZ, (const BYTE *)s, (DWORD)strlen(s) + 1);
}

static int
hexval(int c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

/* load reads registry.txt, and reports whether it was there. */
static int
load(void)
{
	char *text, *line, *next, *f[5], *p;
	BYTE data[Maxdata];
	DWORD n;
	int i, hi, lo, hive;

	text = SDL_LoadFile(regpath, NULL);
	if (text == NULL)
		return 0;
	for (line = text; line != NULL && *line != '\0'; line = next) {
		next = strchr(line, '\n');
		if (next != NULL)
			*next++ = '\0';
		line[strcspn(line, "\r")] = '\0';
		p = line;
		for (i = 0; i < 5; i++) {
			f[i] = p;
			if (i == 4)
				break;
			p = strchr(p, '\t');
			if (p == NULL)
				break;
			*p++ = '\0';
		}
		if (i < 4)
			continue;
		hive = strcmp(f[0], "HKLM") == 0;
		if (strcmp(f[3], "dword") == 0)
			setdw(hive, f[1], f[2], (DWORD)strtoul(f[4], NULL, 10));
		else if (strcmp(f[3], "sz") == 0)
			setsz(hive, f[1], f[2], f[4]);
		else if (strcmp(f[3], "binary") == 0) {
			for (n = 0; n < Maxdata && (hi = hexval(f[4][2*n])) >= 0 &&
			    (lo = hexval(f[4][2*n+1])) >= 0; n++)
				data[n] = (BYTE)(hi << 4 | lo);
			set(hive, f[1], f[2], REG_BINARY, data, n);
		}
	}
	SDL_free(text);
	return 1;
}

/* the language that readreg (system.cpp) takes from the system's */
static DWORD
syslang(void)
{
	static const struct {
		const char *code;
		DWORD lang;
	} map[] = {
		{ "de", 0 }, { "nl", 0 },
		{ "fr", 2 },
		{ "it", 4 }, { "ca", 4 }, { "pt", 4 }, { "ro", 4 }, { "eu", 4 }, { "es", 4 },
		{ "pl", 5 },
	};
	const char *code;
	size_t i;

	code = syslocale(NULL, 0);
	for (i = 0; i < sizeof map / sizeof map[0]; i++)
		if (SDL_strcasecmp(code, map[i].code) == 0)
			return map[i].lang;
	return 1;
}

void
reginit(void)
{
	BYTE data[200];

	SDL_snprintf(regpath, sizeof regpath, "%sregistry.txt", sysuserdir);
	if (load())
		return;
	setsz(1, gamekey, "AppPath", "C:\\teudogar");
	setdw(1, gamekey, "VersionMajor", 1);
	setdw(1, gamekey, "VersionMinor", 5);
	setdw(1, gamekey, "VersionAddInfo", 2);
	setdw(0, gamekey, "SelectedLanguage", syslang());
	setdw(0, gamekey, "LicenseAgreem", 1);
	setsz(0, gamekey, "DataPath", "C:\\teudogar.sav");
	memset(data, 0, sizeof data);
	data[0] = 0xff;
	set(0, gamekey, "Data", REG_BINARY, data, sizeof data);
	save();
}

DWORD
sysregdword(const char *name, DWORD def)
{
	Value *v;

	v = find(0, portkey, name);
	if (v == NULL || v->type != REG_DWORD)
		return def;
	return v->data[0] | v->data[1] << 8 | v->data[2] << 16 | (DWORD)v->data[3] << 24;
}

void
syssetregdword(const char *name, DWORD d)
{
	setdw(0, portkey, name, d);
	save();
}

int
sysregstring(const char *name, char *buf, size_t n)
{
	Value *v;

	v = find(0, portkey, name);
	if (v == NULL || v->type != REG_SZ)
		return 0;
	SDL_strlcpy(buf, (char *)v->data, n);
	return 1;
}

static Key *
keyof(HKEY h)
{
	Key *k;

	k = h;
	if (k == NULL || k->magic != Regmagic)
		return NULL;
	return k;
}

/* exists reports whether some value lies under key. */
static int
exists(int hive, const char *key)
{
	int i;

	for (i = 0; i < nvalues; i++)
		if (values[i].hive == hive && SDL_strcasecmp(values[i].key, key) == 0)
			return 1;
	return 0;
}

static LONG
openkey(HKEY root, LPCSTR sub, PHKEY out, int create)
{
	Key *k;
	int hive;

	if (root != HKEY_LOCAL_MACHINE && root != HKEY_CURRENT_USER)
		return ERROR_FILE_NOT_FOUND;
	hive = hiveof(root);
	if (!create && !exists(hive, sub))
		return ERROR_FILE_NOT_FOUND;
	k = calloc(1, sizeof *k);
	if (k == NULL)
		return ERROR_NOT_ENOUGH_MEMORY;
	k->magic = Regmagic;
	k->hive = hive;
	SDL_strlcpy(k->key, sub, sizeof k->key);
	*out = k;
	return ERROR_SUCCESS;
}

LONG
RegOpenKeyExA(HKEY root, LPCSTR sub, DWORD opts, REGSAM sam, PHKEY out)
{
	(void)opts, (void)sam;
	return openkey(root, sub, out, 0);
}

LONG
RegCreateKeyExA(HKEY root, LPCSTR sub, DWORD reserved, LPSTR cls, DWORD opts, REGSAM sam,
	LPSECURITY_ATTRIBUTES sa, PHKEY out, LPDWORD disp)
{
	int hive;

	(void)reserved, (void)cls, (void)opts, (void)sam, (void)sa;
	hive = hiveof(root);
	if (disp != NULL)
		*disp = exists(hive, sub) ? REG_OPENED_EXISTING_KEY : REG_CREATED_NEW_KEY;
	return openkey(root, sub, out, 1);
}

LONG
RegCloseKey(HKEY h)
{
	Key *k;

	k = keyof(h);
	if (k == NULL)
		return ERROR_INVALID_HANDLE;
	k->magic = 0;
	free(k);
	return ERROR_SUCCESS;
}

LONG
RegQueryValueExA(HKEY h, LPCSTR name, LPDWORD reserved, LPDWORD type, LPBYTE data, LPDWORD size)
{
	Value *v;
	Key *k;

	(void)reserved;
	k = keyof(h);
	if (k == NULL)
		return ERROR_INVALID_HANDLE;
	v = find(k->hive, k->key, name);
	if (v == NULL)
		return ERROR_FILE_NOT_FOUND;
	if (type != NULL)
		*type = v->type;
	if (data == NULL) {
		if (size != NULL)
			*size = v->size;
		return ERROR_SUCCESS;
	}
	if (size == NULL || *size < v->size) {
		if (size != NULL)
			*size = v->size;
		return ERROR_MORE_DATA;
	}
	memcpy(data, v->data, v->size);
	*size = v->size;
	return ERROR_SUCCESS;
}

LONG
RegSetValueExA(HKEY h, LPCSTR name, DWORD reserved, DWORD type, const BYTE *data, DWORD size)
{
	Key *k;

	(void)reserved;
	k = keyof(h);
	if (k == NULL)
		return ERROR_INVALID_HANDLE;
	if (set(k->hive, k->key, name, type, data, size) == NULL)
		return ERROR_NOT_ENOUGH_MEMORY;
	save();
	return ERROR_SUCCESS;
}

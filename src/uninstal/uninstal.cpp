#include <windows.h>
#include <shlobj.h>
#include <stdarg.h>
#include <string.h>

extern char first[], last[];
extern char s_regkey[], s_apppath[], s_startmenupath[];
extern char s_versionmajor[], s_versionminor[], s_versionaddinfo[];
extern char s_selectedlanguage[], s_runfromcd[], s_datapath[], s_data[], s_licenseagreem[];
extern int lang;
extern int syslang;
extern HKEY key;
extern char apppath[1000], startmenupath[1000], datapath[1000];
extern DWORD versionmajor, versionminor, versionaddinfo;
extern int installed, langset, havedatapath, havedata;
extern DWORD runfromcd, licenseagreem;
extern char regvalbuf[200], data[200];
extern int regvalflag;
extern char pathbuf[];
extern char s_bslash_s[];
extern DWORD regtype;
extern char s_cv[], s_programfilesdir[], s_c_programfiles[];
extern char s_title[], s_teudogar[], s_c_root[];
extern char s_savedgames[], s_teudogar_sav[];
extern char modfile[1000], moddir[1000];
extern char windir[1000], uninstpath[];
extern char s_uninstexe[];
extern FILETIME filetime;
extern DWORD filesize;
extern char *months[13];
extern char s_timefmt[], s_ld[], s_ld_03ld[], s_ld_03ld_03ld[];
extern char s_open[];
extern char logbuf[], line[];
extern HANDLE hfind;
extern WIN32_FIND_DATA fd;
extern DWORD foundsize;
extern int delfailed, errors, regdenied, keyfailed;
extern DWORD regresult;
extern char s_cantdelete[], s_regdenied[], s_regerror[], s_cantremove[];
extern char curdir[1000], gamepath[1000], savepath[1000], menupath[1000];
extern int removesaves, hkcudone;
extern char errmsg[];
extern char s_exe[], s_runself[], s_devpath[];
extern char s_cantremovedir[], s_cantsetdir[], s_bslash_s2[], s_encyclopedia[];
extern char s_d0dat[], s_e0dat[], s_dltxt[], s_quests[], s_ordertxt[], s_orderhtm[];
extern char s_configdat[], s_logtxt[], s_hiscore[], s_photobmp[], s_photodat[];
extern char s_readme2[], s_techinfo[], s_intromid[], s_combatmid[];
extern char s_teudogarbmp[], s_teudogarsav[], s_teudogarlnk[];
extern char s_ehtm[], s_agif[], s_ajpg[], s_lnk[];
extern char s_fileid[], s_padfile[], s_readmes[], s_teudogarexe[], s_teudogardats[];
extern char s_teudogardat[], s_uninstalexe[], s_orderhtm2[];
extern char s_apppathiscd[], s_home[];
extern char s_wolfmittag_key[], s_title2[], s_software[], s_wolfmittag[];
extern char s_uninstallkey[], s_title3[];

#define LANGS(x) extern char x##_de[], x##_en[], x##_fr[], x##_es[], x##_it[], x##_pl[]
#define L(x) pick(x##_de, x##_en, x##_fr, x##_es, x##_it, x##_pl)

LANGS(done);
LANGS(already);
LANGS(uninstall);
LANGS(sure);
LANGS(confirm);
LANGS(saves);
LANGS(success);
LANGS(notall);

void
decrypt(void)
{
	char *p;

	p = first;
	do {
		*p ^= 0xf7;
		p++;
	} while (p < last);
}

char *
pick(char *de, char *en, char *fr, char *it, char *pl, char *x)
{
	if (lang == 0)
		return de;
	if (lang == 2)
		return fr;
	if (lang == 3)
		return it;
	if (lang == 4)
		return pl;
	if (lang == 5)
		return x;
	return en;
}

char
upper(char c)
{
	if (c >= 'a' && c <= 'z')
		return c - ' ';
	if (c >= '\xe0' && c <= '\xfe')
		return c - ' ';
	if (c == '\x9a')
		return '\x8a';
	if (c == '\x9c')
		return '\x8c';
	if (c == '\xff')
		return '\x9f';
	return c;
}

int
regstring(HKEY key, char *name, char *buf, DWORD size)
{
	DWORD len, type;

	len = size;
	memset(buf, 0, size);
	if (RegQueryValueEx(key, name, NULL, &type, (LPBYTE)buf, &len) != ERROR_SUCCESS || type != REG_SZ) {
		*buf = '\0';
		return 0;
	}
	return 1;
}

int
regbinary(HKEY key, char *name, char *buf, DWORD size)
{
	DWORD len, type;

	len = size;
	memset(buf, 0, size);
	if (RegQueryValueEx(key, name, NULL, &type, (LPBYTE)buf, &len) != ERROR_SUCCESS || type != REG_BINARY) {
		if (len != 0)
			memset(buf, 0, len);
		return 0;
	}
	return 1;
}

int
regdword(HKEY key, char *name, DWORD *value)
{
	DWORD len, type;

	*value = 0;
	len = sizeof *value;
	if (RegQueryValueEx(key, name, NULL, &type, (LPBYTE)value, &len) != ERROR_SUCCESS || type != REG_DWORD)
		return 0;
	return 1;
}

int
readsettings(void)
{
	DWORD v;
	int ok;

	versionmajor = 0;
	versionminor = 0;
	versionaddinfo = 0;
	syslang = 1;
	switch ((DWORD)PRIMARYLANGID(GetUserDefaultLangID())) {
	default:
		syslang = 1;
		break;
	case LANG_GERMAN:
		syslang = 0;
		break;
	case LANG_FRENCH:
		syslang = 2;
		break;
	case LANG_SPANISH:
	case LANG_PORTUGUESE:
		syslang = 3;
		break;
	case LANG_ITALIAN:
		syslang = 4;
		break;
	case LANG_POLISH:
		syslang = 5;
		break;
	}
	apppath[0] = '\0';
	memset(datapath, 0, sizeof datapath);
	havedatapath = 0;
	startmenupath[0] = '\0';
	installed = 0;
	memset(regvalbuf, 0, sizeof regvalbuf);
	regvalflag = 0;
	memset(data, 0, sizeof data);
	havedata = 0;
	licenseagreem = 0;
	ok = 0;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, s_regkey, 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS) {
		ok = 1;
		regstring(key, s_apppath, apppath, sizeof apppath);
		regstring(key, s_startmenupath, startmenupath, sizeof startmenupath);
		installed = 1;
		if (!regdword(key, s_versionmajor, &versionmajor))
			installed = 0;
		if (!regdword(key, s_versionminor, &versionminor))
			installed = 0;
		if (!regdword(key, s_versionaddinfo, &versionaddinfo))
			installed = 0;
		RegCloseKey(key);
		key = 0;
	}
	if (RegOpenKeyEx(HKEY_CURRENT_USER, s_regkey, 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS) {
		ok = 1;
		if (regdword(key, s_selectedlanguage, &v)) {
			syslang = v;
			langset = 1;
		}
		if (regdword(key, s_runfromcd, &v))
			runfromcd = v;
		if (regstring(key, s_datapath, datapath, sizeof datapath))
			havedatapath = 1;
		if (regbinary(key, s_data, data, sizeof data))
			havedata = 1;
		if (regdword(key, s_licenseagreem, &v))
			licenseagreem = v;
		RegCloseKey(key);
		key = 0;
	}
	return ok;
}

int
getfolder(char *path, int csidl)
{
	LPITEMIDLIST pidl;
	LPMALLOC m;
	HRESULT r;

	r = SHGetSpecialFolderLocation(NULL, csidl, &pidl);
	if (r != NOERROR && csidl == CSIDL_COMMON_PROGRAMS)
		r = SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &pidl);
	if (r != NOERROR) {
		wsprintf(path, "C:\\");
		csidl = 0;
	} else {
		csidl = SHGetPathFromIDList(pidl, path);
		SHGetMalloc(&m);
		m->Free(pidl);
		m->Release();
	}
	return csidl;
}

void
setname(char *dst, char *src, char *name)
{
	char *p;

	wsprintf(dst, src);
	p = dst;
	while (*p)
		p++;
	while (p > pathbuf && *p != 0x5c)
		p--;
	if (*p != 0x5c)
		while (*p)
			p++;
	*p = '\\';
	wsprintf(p + 1, name);
}

void
stripname(char *path)
{
	char *p;

	p = path;
	while (*p)
		p++;
	while (p > path && *p != 0x5c)
		p--;
	if (*p == 0x5c) {
		if (p > path && (p[-1] == ':' || p[-1] == 0x5c))
			p++;
		*p = '\0';
	}
}

void
makedirs(char *path, char *buf)
{
	char *p;

	p = buf;
	for (;;) {
		wsprintf(buf, path);
		while (*p && *p != 0x5c)
			p++;
		if (*p != 0x5c)
			break;
		if (p[-1] != ':' && p[-1] != 0x5c) {
			*p = '\0';
			CreateDirectory(buf, NULL);
			SetCurrentDirectory(buf);
		}
		p++;
	}
	CreateDirectory(path, NULL);
}

void
addname(char *dst, char *name)
{
	char *p;

	for (p = dst; *p; p++)
		;
	if (p > dst && p[-1] == 0x5c)
		p--;
	wsprintf(p, s_bslash_s, name);
}

LONG
programfiles(char *buf, DWORD size)
{
	LONG r;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, s_cv, 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS)
		r = RegQueryValueEx(key, s_programfilesdir, NULL, &regtype, (LPBYTE)buf, &size);
	RegCloseKey(key);
	key = 0;
	if (buf[0] == 0) {
		wsprintf(buf, s_c_programfiles);
		CreateDirectory(buf, NULL);
	}
	return r;
}

LONG
gamedir(char *buf, DWORD size, int full)
{
	LONG r;

	r = programfiles(buf, size);
	addname(buf, full ? s_title : s_teudogar);
	return r;
}

int
savedir(char *buf, char *dir, int full)
{
	if (!getfolder(buf, CSIDL_PERSONAL))
		wsprintf(buf, s_c_root);
	addname(buf, full ? s_savedgames : s_teudogar_sav);
	return 1;
}

int
modulepath(void)
{
	char *p;

	if (GetModuleFileName(NULL, modfile, sizeof modfile) == 0) {
		moddir[0] = '\0';
		modfile[0] = '\0';
		return 0;
	}
	memcpy(moddir, modfile, sizeof moddir);
	p = moddir;
	while (*p)
		p++;
	while (p > moddir && *p != 0x5c)
		p--;
	if (*p == 0x5c)
		*p = '\0';
	return 1;
}

void
uninstexe(void)
{
	if (GetWindowsDirectory(windir, sizeof windir) == 0)
		wsprintf(windir, s_c_root);
	wsprintf(uninstpath, s_uninstexe, windir);
}

int
fileinfo(char *path)
{
	HANDLE h;

	h = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (h == INVALID_HANDLE_VALUE || h == NULL)
		return 0;
	GetFileTime(h, NULL, NULL, &filetime);
	filesize = GetFileSize(h, NULL);
	CloseHandle(h);
	return 1;
}

void
fmttime(char *buf, FILETIME *ft)
{
	SYSTEMTIME st;
	FILETIME lt;
	int m;

	FileTimeToLocalFileTime(ft, &lt);
	FileTimeToSystemTime(&lt, &st);
	m = st.wMonth;
	if (m < 1 || m > 12)
		m = 1;
	wsprintf(buf, s_timefmt, st.wDay, months[m], st.wYear, st.wHour, st.wMinute, st.wSecond);
}

void
fmtnum(char *buf, DWORD n)
{
	if (n < 1000)
		wsprintf(buf, s_ld, n);
	else if (n < 1000000)
		wsprintf(buf, s_ld_03ld, n / 1000, n % 1000);
	else
		wsprintf(buf, s_ld_03ld_03ld, n / 1000000, n % 1000000 / 1000, n % 1000000 % 1000);
}

int
streqi(char *a, char *b)
{
	char c, d;

	do {
		c = *a++;
		if (c >= 'A' && c <= 'Z')
			c += ' ';
		d = *b++;
		if (d >= 'A' && d <= 'Z')
			d += ' ';
		if (c != d)
			return 0;
	} while (c && d);
	return 1;
}

int
run(char *file, char *params, char *dir)
{
	if ((UINT)ShellExecute(NULL, s_open, file, params, dir, SW_SHOWNORMAL) > 32)
		return 1;
	return 0;
}

void
copystr(char *dst, char *src)
{
	while ((*dst++ = *src++) != 0)
		;
}

void
logf(char *fmt, ...)
{
	va_list ap;
	char *p;

	va_start(ap, fmt);
	wvsprintf(line, fmt, ap);
	p = logbuf;
	while (*p)
		p++;
	if (logbuf[0]) {
		*p++ = '\r';
		*p++ = '\n';
		*p = '\0';
	}
	copystr(p, line);
}

int
findfirst(char *pattern)
{
	foundsize = 0;
	hfind = FindFirstFile(pattern, &fd);
	if (hfind == INVALID_HANDLE_VALUE)
		return -1;
	foundsize = fd.nFileSizeLow;
	return 0;
}

int
findnext(void)
{
	if (hfind == INVALID_HANDLE_VALUE)
		return -1;
	foundsize = 0;
	if (!FindNextFile(hfind, &fd))
		return -1;
	foundsize = fd.nFileSizeLow;
	return 0;
}

int
findclose(void)
{
	if (hfind != INVALID_HANDLE_VALUE && FindClose(hfind)) {
		hfind = INVALID_HANDLE_VALUE;
		return 0;
	}
	return 1;
}

int
delfile(char *path)
{
	if (!DeleteFile(path)) {
		if (!delfailed) {
			logf(s_cantdelete, path);
			delfailed = 1;
		}
		errors = 1;
		return 0;
	}
	return 1;
}

int
delfiles(char *pattern)
{
	int any;

	any = 0;
	if (findfirst(pattern) == 0) {
		do {
			if (delfile(fd.cFileName))
				any = 1;
		} while (findnext() == 0);
	}
	findclose();
	return any;
}

void
regerror(void)
{
	switch (regresult) {
	case ERROR_FILE_NOT_FOUND:
		break;
	case ERROR_ACCESS_DENIED:
		logf(s_regdenied);
		regdenied = 1;
		errors = 1;
		break;
	default:
		logf(s_regerror, regresult & 0xffff);
		errors = 1;
		break;
	}
}

void
delkey(HKEY root, char *subkey, char *name)
{
	regresult = RegOpenKeyEx(root, subkey, 0, KEY_ALL_ACCESS, &key);
	if (regresult == ERROR_SUCCESS) {
		if (RegDeleteKey(key, name) != ERROR_SUCCESS) {
			if (!keyfailed) {
				logf(s_cantremove);
				keyfailed = 1;
			}
			errors = 1;
		}
		RegCloseKey(key);
		key = 0;
	} else
		regerror();
}

int
haskeys(HKEY root, char *subkey, char *name, DWORD size)
{
	DWORD len;
	LONG r;
	int ok;

	regresult = RegOpenKeyEx(root, subkey, 0, DELETE | KEY_ENUMERATE_SUB_KEYS, &key);
	if (regresult != ERROR_SUCCESS)
		return 0;
	len = size;
	r = RegEnumKeyEx(key, 0, name, &len, NULL, NULL, NULL, NULL);
	ok = 0;
	if (r != ERROR_NO_MORE_ITEMS && r == ERROR_SUCCESS)
		ok = 1;
	RegCloseKey(key);
	return ok;
}

int PASCAL
WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show)
{
	FILETIME wt, at, ct;
	DWORD nread, nwritten;
	char *p;
	HGLOBAL mem;
	HANDLE h;
	int tries;

	decrypt();
	GetCurrentDirectory(sizeof curdir, curdir);
	modulepath();
	uninstexe();
	readsettings();
	memcpy(gamepath, apppath, sizeof gamepath);
	memcpy(savepath, datapath, sizeof savepath);
	memcpy(menupath, startmenupath, sizeof menupath);
	lang = syslang;
	p = cmdline;
	while (*p && *p != '&')
		p++;
	if (*p == '&') {
		p++;
		if (!gamepath[0] || !streqi(p, gamepath)) {
			wsprintf(pathbuf, s_exe, p);
			if (fileinfo(pathbuf)) {
				wsprintf(gamepath, p);
				savepath[0] = 0;
			}
		}
	}
	if (gamepath[0] == 0) {
		wsprintf(pathbuf, s_exe, curdir);
		if (fileinfo(pathbuf))
			wsprintf(gamepath, curdir);
		else {
			gamedir(windir, sizeof windir, 1);
			wsprintf(pathbuf, s_exe, windir);
			if (fileinfo(pathbuf))
				wsprintf(gamepath, windir);
			else {
				gamedir(windir, sizeof windir, 0);
				wsprintf(pathbuf, s_exe, windir);
				if (!fileinfo(pathbuf)) {
				notinstalled:
					MessageBox(NULL, L(already), L(uninstall), MB_ICONINFORMATION);
					return 0;
				}
				wsprintf(gamepath, windir);
			}
		}
	}
	if (moddir[0] && streqi(gamepath, moddir)) {
		if (fileinfo(uninstpath))
			goto runcopy;
		if (moddir[0]) {
			mem = GlobalAlloc(GMEM_FIXED, 0x19000);
			if (mem) {
				h = CreateFile(modfile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
				if (h != INVALID_HANDLE_VALUE && h != NULL) {
					if (!ReadFile(h, mem, 0x19000, &nread, NULL))
						nread = 0;
					GetFileTime(h, &ct, &at, &wt);
					CloseHandle(h);
					if (nread != 0) {
						h = CreateFile(uninstpath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
						if (h != INVALID_HANDLE_VALUE && h != NULL) {
							if (!WriteFile(h, mem, nread, &nwritten, NULL))
								nwritten = 0;
							SetFileTime(h, &ct, &at, &wt);
							CloseHandle(h);
							if (nwritten == nread) {
								GlobalFree(mem);
								if (fileinfo(uninstpath))
									goto runcopy;
							}
						}
					}
				}
				GlobalFree(mem);
			}
		}
		goto checkdev;
	runcopy:
		SetCurrentDirectory(gamepath);
		wsprintf(pathbuf, s_runself, uninstpath, gamepath);
		run(uninstpath, pathbuf, gamepath);
		return 0;
	}
checkdev:
	if (streqi(gamepath, s_devpath)) {
		gamepath[0] = 0;
		savepath[0] = 0;
		goto notinstalled;
	}
	if (savepath[0] == 0 && gamepath[0] != 0)
		savedir(savepath, gamepath, 1);
	if (MessageBox(NULL, L(sure), L(confirm), MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES)
		return 1;
	switch (MessageBox(NULL, L(saves), L(confirm), MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)) {
	case IDCANCEL:
		return 1;
	case IDYES:
		removesaves = 1;
		break;
	}
	if (savepath[0] != 0 && removesaves) {
		tries = 0;
		for (;;) {
			if (SetCurrentDirectory(savepath)) {
				delfiles(s_d0dat);
				delfiles(s_e0dat);
				delfiles(s_quests);
				delfiles(s_ordertxt);
				delfiles(s_orderhtm);
				delfiles(s_configdat);
				delfiles(s_logtxt);
				delfiles(s_dltxt);
				delfiles(s_hiscore);
				delfiles(s_photobmp);
				delfiles(s_photodat);
				delfiles(s_intromid);
				delfiles(s_combatmid);
				delfiles(s_readme2);
				delfiles(s_teudogarbmp);
				delfiles(s_teudogarsav);
				delfiles(s_techinfo);
				delfiles(s_teudogarlnk);
				SetCurrentDirectory(s_c_root);
				if (!RemoveDirectory(savepath)) {
					logf(s_cantremovedir, savepath);
					errors = 1;
				}
				break;
			}
			logf(s_cantsetdir, savepath);
			if (++tries != 1) {
				errors = 1;
				break;
			}
			savedir(savepath, gamepath, 0);
		}
	}
	if (gamepath[0] != 0) {
		wsprintf(pathbuf, s_bslash_s2, gamepath, s_encyclopedia);
		if (SetCurrentDirectory(pathbuf)) {
			delfiles(s_ehtm);
			delfiles(s_agif);
			delfiles(s_ajpg);
			SetCurrentDirectory(gamepath);
			if (!RemoveDirectory(pathbuf)) {
				logf(s_cantremovedir, pathbuf);
				errors = 1;
			}
		}
		if (SetCurrentDirectory(gamepath)) {
			delfiles(s_fileid);
			delfiles(s_padfile);
			delfiles(s_orderhtm);
			delfiles(s_readmes);
			delfiles(s_teudogarexe);
			delfiles(s_teudogardats);
			delfiles(s_teudogardat);
			delfiles(s_uninstalexe);
			SetCurrentDirectory(s_c_root);
			if (!RemoveDirectory(gamepath)) {
				logf(s_cantremovedir, gamepath);
				errors = 1;
			}
		} else {
			logf(s_cantsetdir, gamepath);
			errors = 1;
		}
	}
	if (menupath[0] != 0) {
		if (SetCurrentDirectory(menupath)) {
			delfiles(s_lnk);
			SetCurrentDirectory(s_c_root);
			if (!RemoveDirectory(menupath)) {
				logf(s_cantremovedir, menupath);
				errors = 1;
			}
		} else {
			logf(s_cantsetdir, menupath);
			errors = 1;
		}
	}
	regresult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, s_regkey, 0, KEY_ALL_ACCESS, &key);
	if (regresult == ERROR_SUCCESS) {
		if (RegDeleteValue(key, s_apppath) != ERROR_SUCCESS) {
			logf(s_cantremove);
			keyfailed = 1;
			errors = 1;
		}
		RegDeleteValue(key, s_apppathiscd);
		RegDeleteValue(key, s_startmenupath);
		RegDeleteValue(key, s_home);
		RegDeleteValue(key, s_versionmajor);
		RegDeleteValue(key, s_versionminor);
		RegDeleteValue(key, s_versionaddinfo);
		RegCloseKey(key);
		key = 0;
	} else
		regerror();
	delkey(HKEY_LOCAL_MACHINE, s_wolfmittag_key, s_title2);
	if (!haskeys(HKEY_LOCAL_MACHINE, s_wolfmittag_key, pathbuf, 3000))
		delkey(HKEY_LOCAL_MACHINE, s_software, s_wolfmittag);
	hkcudone = 0;
	regresult = RegOpenKeyEx(HKEY_CURRENT_USER, s_regkey, 0, KEY_ALL_ACCESS, &key);
	if (regresult == ERROR_SUCCESS) {
		if (RegDeleteValue(key, s_licenseagreem) != ERROR_SUCCESS) {
			if (!keyfailed) {
				logf(s_cantremove);
				keyfailed = 1;
			}
			errors = 1;
		}
		if (removesaves)
			RegDeleteValue(key, s_datapath);
		RegDeleteValue(key, s_selectedlanguage);
		RegDeleteValue(key, s_runfromcd);
		hkcudone = 1;
		RegDeleteValue(key, s_data);
		RegCloseKey(key);
		key = 0;
	} else
		regerror();
	if (removesaves && hkcudone) {
		delkey(HKEY_CURRENT_USER, s_wolfmittag_key, s_title2);
		if (!haskeys(HKEY_CURRENT_USER, s_wolfmittag_key, pathbuf, 3000))
			delkey(HKEY_CURRENT_USER, s_software, s_wolfmittag);
	}
	delkey(HKEY_LOCAL_MACHINE, s_uninstallkey, s_title3);
	if (errors) {
		wsprintf(errmsg, L(notall), logbuf);
		MessageBox(NULL, errmsg, L(done), MB_ICONINFORMATION);
	}
	MessageBox(NULL, L(success), L(done), MB_ICONINFORMATION);
	return 0;
}

/*
 * The data, in the order of the original: the interface IDs of
 * objidl.h, which wpp386 places in CONST2, then the strings, from first
 * to last, which decrypt XORs with 0xf7 at the start.
 */
EXTERN_C const IID __cdecl IID_IMarshal = { 0x00000003, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IMalloc = { 0x00000002, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IMallocSpy = { 0x0000001d, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IStdMarshalInfo = { 0x00000018, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IExternalConnection = { 0x00000019, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IMultiQI = { 0x00000020, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumUnknown = { 0x00000100, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IBindCtx = { 0x0000000e, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumMoniker = { 0x00000102, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IRunnableObject = { 0x00000126, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IRunningObjectTable = { 0x00000010, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IPersist = { 0x0000010c, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IPersistStream = { 0x00000109, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IMoniker = { 0x0000000f, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IROTData = { 0xf29f6bc0, 0x5021, 0x11ce, { 0xaa, 0x15, 0x00, 0x00, 0x69, 0x01, 0x29, 0x3f } };
EXTERN_C const IID __cdecl IID_IEnumString = { 0x00000101, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_ISequentialStream = { 0x0c733a30, 0x2a1c, 0x11ce, { 0xad, 0xe5, 0x00, 0xaa, 0x00, 0x44, 0x77, 0x3d } };
EXTERN_C const IID __cdecl IID_IStream = { 0x0000000c, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumSTATSTG = { 0x0000000d, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IStorage = { 0x0000000b, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IPersistFile = { 0x0000010b, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IPersistStorage = { 0x0000010a, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_ILockBytes = { 0x0000000a, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumFORMATETC = { 0x00000103, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumSTATDATA = { 0x00000105, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IRootStorage = { 0x00000012, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IAdviseSink = { 0x0000010f, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IAdviseSink2 = { 0x00000125, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IDataObject = { 0x0000010e, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IDataAdviseHolder = { 0x00000110, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IMessageFilter = { 0x00000016, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IRpcChannelBuffer = { 0xd5f56b60, 0x593b, 0x101a, { 0xb5, 0x69, 0x08, 0x00, 0x2b, 0x2d, 0xbf, 0x7a } };
EXTERN_C const IID __cdecl IID_IRpcProxyBuffer = { 0xd5f56a34, 0x593b, 0x101a, { 0xb5, 0x69, 0x08, 0x00, 0x2b, 0x2d, 0xbf, 0x7a } };
EXTERN_C const IID __cdecl IID_IRpcStubBuffer = { 0xd5f56afc, 0x593b, 0x101a, { 0xb5, 0x69, 0x08, 0x00, 0x2b, 0x2d, 0xbf, 0x7a } };
EXTERN_C const IID __cdecl IID_IPSFactoryBuffer = { 0xd5f569d0, 0x593b, 0x101a, { 0xb5, 0x69, 0x08, 0x00, 0x2b, 0x2d, 0xbf, 0x7a } };
EXTERN_C const IID __cdecl IID_IPropertyStorage = { 0x00000138, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IPropertySetStorage = { 0x0000013a, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumSTATPROPSTG = { 0x00000139, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IEnumSTATPROPSETSTG = { 0x0000013b, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
EXTERN_C const IID __cdecl IID_IFillLockBytes = { 0x99caf010, 0x415e, 0x11cf, { 0x88, 0x14, 0x00, 0xaa, 0x00, 0xb5, 0x69, 0xf5 } };
EXTERN_C const IID __cdecl IID_IProgressNotify = { 0xa9d758a0, 0x4617, 0x11cf, { 0x95, 0xfc, 0x00, 0xaa, 0x00, 0x68, 0x0d, 0xb4 } };
EXTERN_C const IID __cdecl IID_ILayoutStorage = { 0x0e6d4d90, 0x6738, 0x11cf, { 0x96, 0x08, 0x00, 0xaa, 0x00, 0x68, 0x0d, 0xb4 } };
char first[] = "TEUDOGAR";
char brand_en[] = "TEUDOGAR";
char brand_fr[] = "TEUDOGAR";
char brand_es[] = "";
char brand_it[] = "TEUDOGAR";
char brand_pl[] = "TEUDOGAR";
char subtitle_de[] = " UND DAS B\334NDNIS MIT ROM";
char subtitle_en[] = " AND THE ALLIANCE WITH ROME";
char subtitle_fr[] = " ET L'ALLIANCE AVEC ROME";
char subtitle_es[] = "";
char subtitle_it[] = " E L'ALLEANZA CON ROMA";
char subtitle_pl[] = " I SOJUSZ Z RZYMEM";
char title_de[] = "Teudogar und das B\374ndnis mit Rom";
char title_en[] = "Teudogar and the Alliance with Rome";
char title_fr[] = "Teudogar et l'Alliance avec Rome";
char title_es[] = "";
char title_it[] = "Teudogar e l'Alleanza con Roma";
char title_pl[] = "Teudogar i Sojusz z Rzymem";
char cancel_de[] = "&Abbrechen";
char cancel_en[] = "&Abort";
char cancel_fr[] = "&Annuler";
char cancel_es[] = "";
char cancel_it[] = "&Annulla";
char cancel_pl[] = "&Anuluj";
char chooselang_de[] = "Bitte w\344hlen Sie Ihre Sprache:\r";
char chooselang_en[] = "Please select your language:\r";
char chooselang_fr[] = "Veuillez choisir votre langue :\r";
char chooselang_es[] = "";
char chooselang_it[] = "Per favore seleziona una lingua:\r";
char chooselang_pl[] = "Prosze wybrac jezyk:\r";
char errtitle_de[] = "Fehler";
char errtitle_en[] = "Error";
char errtitle_fr[] = "Erreur";
char errtitle_es[] = "";
char errtitle_it[] = "Errore";
char errtitle_pl[] = "Blad";
char done_de[] = "Fertig";
char done_en[] = "Done";
char done_fr[] = "Termin\351";
char done_es[] = "";
char done_it[] = "Completo";
char done_pl[] = "Zakonczone";
char manual_de[] = "Bedienung und Spielregeln";
char manual_en[] = "Operation and Game Rules";
char manual_fr[] = "Contr\364les et R\350gles du Jeu";
char manual_es[] = "";
char manual_it[] = "Operazione e Regole di gioco";
char manual_pl[] = "Operacje i Reguly Gry";
char encycl_de[] = "Lexikon - Germanen und R\366mer";
char encycl_en[] = "Encyclopedia - Teutons and Romans";
char encycl_fr[] = "Encyclop\351die - Germains et Romains";
char encycl_es[] = "";
char encycl_it[] = "Enciclopedia - Teutoni e Romani";
char encycl_pl[] = "Encyklopedia - Teutoni i Rzymianie";
char walkthrough_de[] = "Komplettl\366sung";
char walkthrough_en[] = "Walkthrough";
char walkthrough_fr[] = "Conseils";
char walkthrough_es[] = "";
char walkthrough_it[] = "Consigli";
char walkthrough_pl[] = "Rozwiazanie";
char s_readmename[] = "ReadMe.Txt";
char s_uninstall[] = "Un-Install Teudogar";
char already_de[] = "Programm ist schon de-installiert worden.";
char already_en[] = "Program has already been uninstalled.";
char already_fr[] = "Le Programme a d\351j\340 \351t\351 d\351sinstall\351.";
char already_es[] = "";
char already_it[] = "Il programma \350 gi\340 disinstallato.";
char already_pl[] = "Program juz zostal zdyzynstalowany.";
char uninstall_de[] = "Un-Installieren";
char uninstall_en[] = "Un-Install";
char uninstall_fr[] = "D\351sinstaller";
char uninstall_es[] = "";
char uninstall_it[] = "Dis-Installa";
char uninstall_pl[] = "Dyzynstaluj";
char sure_de[] = "\nSind Sie sicher, da\337 Sie\n\n  'Teudogar und das B\374ndnis mit Rom'\n\nvollst\344ndig von Ihrem System ENTFERNEN wollen?\n\n";
char sure_en[] = "\nAre you sure you want to completely REMOVE\n\n  'Teudogar and the Alliance with Rome'\n\nfrom your system?\n\n";
char sure_fr[] = "\nEtes-vous sur de vouloir compl\350tement ENLEVER\n\n  'Teudogar et l'Alliance avec Rome'\n\nde votre syst\350me ?\n\n";
char sure_es[] = "";
char sure_it[] = "\nSei sicuro di volere RIMUOVERE completamente\n\n  'Teudogar e l'Alleanza con Roma'\n\ndal tuo sistema?\n\n";
char sure_pl[] = "\nJestes pewien ze chcesz calkowicie USUNAC\n\n  'Teudogar i Sojusz z Rzymem'\n\nz twojego systemu?\n\n";
char confirm_de[] = "L\366schen von Dateien best\344tigen";
char confirm_en[] = "Confirm File Deletion";
char confirm_fr[] = "Le Fichier a bien \351t\351 Supprim\351";
char confirm_es[] = "";
char confirm_it[] = "Conferma cancellazione";
char confirm_pl[] = "Potwierdz wymazanie";
char saves_de[] = "\nM\366chten Sie auch Ihre gespeicherten Spielst\344nde entfernen?\n\n";
char saves_en[] = "\nDo you also want to remove your saved games?\n\n";
char saves_fr[] = "\nVoulez-vous \351galement effacer vos parties enregistr\351es ?\n\n";
char saves_es[] = "";
char saves_it[] = "\nVuoi cancellare anche i salvataggi?\n\n";
char saves_pl[] = "\nCzy chcesz rowniez usunac twoje uratowane gry?\n\n";
char success_de[] = "De-Installation erfolgreich abgeschlossen.";
char success_en[] = "Uninstall successfully completed.";
char success_fr[] = "D\351sinstallation effectu\351e avec succ\350s.";
char success_es[] = "";
char success_it[] = "Disinstallazione completata.";
char success_pl[] = "Dyzynstalacja zakonczona pomyslnie.";
char notall_de[] = "Nicht alle Komponenten konnten von Ihrem System entfernt werden:\r\n\r\n%s";
char notall_en[] = "Uninstall was not able to remove all components from your system:\r\n\r\n%s";
char notall_fr[] = "Certains composants n'ont pu \352tre effaces avec success de votre syst\350me :\r\n\r\n%s";
char notall_es[] = "";
char notall_it[] = "La disinstallazione non ha cancellato tutti i componenti:\r\n\r\n%s";
char notall_pl[] = "Dyzynstalacja nie mogla usunac wszystkie komponenty z twojego systemu:\r\n\r\n%s";
char s_cantdelete[] = "Couldn't delete %s.";
char s_cantremovedir[] = "Couldn't remove folder '%s'.";
char s_cantsetdir[] = "Couldn't set current folder to '%s'.";
char s_cantremove[] = "Couldn't remove information from registry.";
char s_regdenied[] = "Access for removing information from registry denied.";
char s_regerror[] = "Error %d while removing information from registry.";
char s_title[] = "Teudogar and the Alliance with Rome";
char s_teudogar[] = "Teudogar";
char s_homeurl[] = "http://www.teudogar.com";
char s_deut[] = "deut";
char s_engl[] = "engl";
char s_fran[] = "fran";
char s_espa[] = "espa";
char s_ital[] = "ital";
char s_pols[] = "pols";
char s_exe[] = "%s\\Teudogar.Exe";
char s_savedgames[] = "Teudogar and the Alliance with Rome - saved games";
char s_teudogar_sav[] = "Teudogar.SAV";
char s_mydocs[] = "c:\\My Documents\\teudogar";
char s_c_teudogar[] = "C:\\TEUDOGAR";
char s_defexe[] = "C:\\Teudogar\\Teudogar.Exe";
char s_teudogarbmp[] = "Teudogar*.Bmp";
char s_teudogarsav[] = "Teudogar_?.Sav";
char s_teudogarlnk[] = "Teudogar*.Lnk";
char s_dat00[] = "Teudogar_00.Dat";
char s_dat01[] = "Teudogar_01.Dat";
char s_dat02[] = "Teudogar_02.Dat";
char s_dat03[] = "Teudogar_03.Dat";
char s_dat04[] = "Teudogar_04.Dat";
char s_dat05[] = "Teudogar_05.Dat";
char s_dat06[] = "Teudogar_06.Dat";
char s_dat07[] = "Teudogar_07.Dat";
char s_teudogardat[] = "Teudogar.Dat";
char s_teudogardats[] = "Teudogar_??.Dat";
char s_datfmt[] = "Teudogar_%02d.Dat";
char s_idxfmt[] = "Teudogar_%02d.Idx";
char s_idx[] = "Teudogar.Idx";
char s_uninstexe[] = "%s\\UnInstall_Teudogar_0815.Exe";
char s_teudogarexe[] = "Teudogar.Exe";
char s_ico[] = "Teudogar.Ico";
char s_othersoft[] = "Other Software";
char s_datadir[] = "Data";
char s_encyclopedia[] = "Encyclopedia";
char s_d0dat[] = "D0*.dat";
char s_e0dat[] = "E0*.dat";
char s_dltxt[] = "dl*.txt";
char s_quests[] = "Quests.Txt";
char s_ordertxt[] = "Order.Txt";
char s_orderhtm[] = "Order.htm";
char s_configdat[] = "Config.Dat";
char s_logtxt[] = "Log.Txt";
char s_hiscore[] = "HiScore.Dat";
char s_photobmp[] = "Photo*.Bmp";
char s_photodat[] = "PHOTO.DAT";
char s_readme2[] = "ReadMe2.Txt";
char s_techinfo[] = "TechInfo.Txt";
char s_intromid[] = "intro.mid";
char s_combatmid[] = "combat.mid";
char s_c_root[] = "C:\\";
char s_fileid[] = "File_Id.Diz";
char s_padfile[] = "Pad_File.Xml";
char s_readmede[] = "ReadMe_D.Txt";
char s_readmeen[] = "ReadMe_E.Txt";
char s_readmeit[] = "ReadMe_I.Txt";
char s_readmefr[] = "ReadMe_F.Txt";
char s_readmepl[] = "ReadMe_P.Txt";
char s_readmes[] = "ReadMe_?.Txt";
char s_uninstalexe[] = "UnInstal.Exe";
char s_uninstalfmt[] = "%s\\UnInstal.Exe";
char s_setupexe[] = "Setup.Exe";
char s_autorun[] = "AutoRun.Inf";
char s_readmetxt[] = "ReadMe.Txt";
char s_encycldhtm[] = "encycld.htm";
char s_encyclhtm[] = "encycl.htm";
char s_encyclfr[] = "encyclf.htm";
char s_encycles[] = "encycls.htm";
char s_encyclit[] = "encycli.htm";
char s_testtmp[] = "TEST.TMP";
char s_allfiles[] = "*.*";
char s_twostrs[] = "%s %s";
char s_bslash_s[] = "\\%s";
char s_bslash_s2[] = "%s\\%s";
char s_drivefmt[] = "%c:\\%s";
char s_timefmt[] = "%d.%s.%d, %02d:%02d:%02d";
char s_ld[] = "%ld";
char s_ld_03ld[] = "%ld,%03ld";
char s_ld_03ld_03ld[] = "%ld,%03ld,%03ld";
char s_jan[] = "JAN";
char s_feb[] = "FEB";
char s_mar[] = "MAR";
char s_apr[] = "APR";
char s_may[] = "MAY";
char s_jun[] = "JUN";
char s_jul[] = "JUL";
char s_aug[] = "AUG";
char s_sep[] = "SEP";
char s_oct[] = "OCT";
char s_nov[] = "NOV";
char s_dec[] = "DEC";
char s_uninstkey[] = "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Teudogar and the Alliance with Rome";
char s_uninstallkey[] = "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
char s_title3[] = "Teudogar and the Alliance with Rome";
char s_displayname[] = "DisplayName";
char s_uninststring[] = "UninstallString";
char s_regkey[] = "Software\\Wolf Mittag\\Teudogar and the Alliance with Rome";
char s_wolfmittag_key[] = "Software\\Wolf Mittag";
char s_title2[] = "Teudogar and the Alliance with Rome";
char s_software[] = "Software";
char s_wolfmittag[] = "Wolf Mittag";
char s_apppath[] = "AppPath";
char s_apppathiscd[] = "AppPathIsCD";
char s_startmenupath[] = "StartMenuPath";
char s_home[] = "Home";
char s_versionmajor[] = "VersionMajor";
char s_versionminor[] = "VersionMinor";
char s_versionaddinfo[] = "VersionAddInfo";
char s_datapath[] = "DataPath";
char s_selectedlanguage[] = "SelectedLanguage";
char s_runfromcd[] = "RunFromCD";
char s_data[] = "Data";
char s_licenseagreem[] = "LicenseAgreem";
char s_cv[] = "Software\\Microsoft\\Windows\\CurrentVersion";
char s_programfilesdir[] = "ProgramFilesDir";
char s_c_programfiles[] = "C:\\Program Files";
char s_open[] = "open";
char s_htmlhead[] = "<HTML> <HEAD> <TITLE> %s: %s </TITLE> <style type=\"text/css\"> <!-- a:hover {text-decoration: underline; color: #AA0000; } //--></style> </HEAD> <body bgcolor=\"#FFFFF0\" text=\"#000000\" link=\"#000000\" vlink=\"#000000\" alink=\"#000000\"> ";
char s_htmlsect[] = "<br> %s: <br> <h2><font face=\"Times New Roman\"> %s </font></h2> ";
char s_htmlh2[] = "<h2><font face=\"Times New Roman\"> %s </font></h2> ";
char s_htmltail[] = "<br> </body> </HTML>";
char s_runself[] = "%s &%s";
char s_devpath[] = "C:\\DAT\\P-SURT";
char s_lnk[] = "*.lnk";
char s_ehtm[] = "e*.htm";
char s_agif[] = "a*.gif";
char s_ajpg[] = "a*.jpg";
char last[4] = { 1 };
int syslang = 1;
char nomonth[2] = "";
int lang = 1;
char *months[13] = {
	nomonth,
	s_jan, s_feb, s_mar, s_apr, s_may, s_jun,
	s_jul, s_aug, s_sep, s_oct, s_nov, s_dec,
};
HANDLE hfind = INVALID_HANDLE_VALUE;

char findunused[20];
DWORD versionmajor, versionminor, versionaddinfo;
char apppath[1000], datapath[1000], startmenupath[1000];
DWORD runfromcd;
char cdunused[12];
char curdir[1000], moddir[1000], modfile[1000], gamepath[1000];
char pathunused1[1000];
char savepath[1000];
char pathunused2[1000];
char menupath[1000], windir[1000];
char pathunused3[2000];
char pathbuf[3000];
char uninstpath[1000];
HKEY key;
DWORD regtype;
int installed, langset;
char regvalbuf[200];
int regvalflag, havedatapath;
char data[200];
int havedata;
DWORD licenseagreem;
FILETIME filetime;
DWORD filesize;
DWORD regresult;
int removesaves, hkcudone, errors, delfailed, regdenied, keyfailed;
char logbuf[4000], errmsg[4000], line[4000];
WIN32_FIND_DATA fd;
DWORD foundsize;

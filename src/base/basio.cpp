#include <windows.h>
#include <stdarg.h>
#include <string.h>
#include "base.h"

static unsigned long mag01[2] = { 0, 0x9908b0df };
HANDLE hfind = INVALID_HANDLE_VALUE;
char root[4] = "";
char defname[] = "Printer";

DWORD written;
HANDLE hlog;
char *lineend;
int loglen;
int filelen;
int textlen;
char *errcur;
char *msgbuf;
DWORD errnum;
DWORD fwritten;
DWORD freadn;
DWORD strlenw;
HANDLE th;
DWORD tn;
HANDLE th2;
DWORD tn2;
WORD dosdate, dostime;
FILETIME lft;
char *wrapstart, *wrapend, *wrapline, *wrapspace;
char wrapch;
SIZE wrapsize;
BOOL enumok;
DWORD needed;
DWORD nreturned;
DWORD bufsize;
int pnum;
int tries;
char *pname;
int prret;
HDC prdc;
DOCINFO prdoc;
int linelen;
SIZE ccsize;
int pagew, pageh;
int lineh;
int linex, liney;
int startx, starty;
DWORD textpos;
DWORD textend;
HFONT prfont;
HGDIOBJ oldfont;
int rnd;
void (*loghook)(void);
void (*errhook)(void);
int year, month, day, hour, minute, second;
int msec;
SYSTEMTIME systime;
char logline[8000];
int crflag;
char logdir[104];
char logpath[112];
char errpath[112];
char filepath[200];
char fbuf[5000];
char errtext[500];
DWORD errcode;
char msgline[1000];
DWORD savederr;
FILETIME ftime;
DWORD fsize;
DWORD lasterr;
LONG seekpos;
int settime;
char envbuf[80];
WIN32_FIND_DATA fd;
DWORD foundsize;
DWORD spc, bps, freec, totalc;
DWORD freemb, totalmb;
HGLOBAL hprinters;
char *printers[20];
int nprinters;
DWORD timer1, timer2, timer3;
char unused8[8];
MTRand rng(0);
DWORD serial;

void gettime(void);
int fileseek(HANDLE h, LONG pos);

void MTRand::seed(unsigned long s)
{
	double one;
	unsigned long *p;

	mt[0] = s;
	for (mti = 1; mti < 624; mti++)
		mt[mti] = 1812433253 * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti;
	one = 1.0;
	p = (unsigned long *)&one;
	if (p[1] == 0x3ff00000)
		endian = 0;
	else if (p[0] == 0x3ff00000)
		endian = 1;
	else
		endian = 2;
}

void MTRand::seedarray(unsigned long *key, int n)
{
	int i, j, k;

	seed(19650218);
	if (n <= 0)
		return;
	i = 1;
	j = 0;
	k = 624 > n ? 624 : n;
	for (; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525)) + key[j] + j;
		i++;
		j++;
		if (i >= 624) {
			mt[0] = mt[623];
			i = 1;
		}
		if (j >= n)
			j = 0;
	}
	for (k = 623; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1566083941)) - i;
		i++;
		if (i >= 624) {
			mt[0] = mt[623];
			i = 1;
		}
	}
	mt[0] = 0x80000000;
}

unsigned long MTRand::genrand()
{
	unsigned long y;
	int kk;

	if (mti >= 624) {
		for (kk = 0; kk < 624 - 397; kk++) {
			y = (mt[kk] & 0x80000000) | (mt[kk + 1] & 0x7fffffff);
			mt[kk] = mt[kk + 397] ^ (y >> 1) ^ mag01[y & 1];
		}
		for (; kk < 623; kk++) {
			y = (mt[kk] & 0x80000000) | (mt[kk + 1] & 0x7fffffff);
			mt[kk] = mt[kk + (397 - 624)] ^ (y >> 1) ^ mag01[y & 1];
		}
		y = (mt[623] & 0x80000000) | (mt[0] & 0x7fffffff);
		mt[623] = mt[396] ^ (y >> 1) ^ mag01[y & 1];
		mti = 0;
	}
	y = mt[mti++];
	y ^= y >> 11;
	y ^= (y << 7) & 0x9d2c5680;
	y ^= (y << 15) & 0xefc60000;
	y ^= y >> 18;
	return y;
}

double MTRand::real()
{
	unsigned long x;
	double d;

	x = genrand();
	switch (endian) {
	case 0:
		((unsigned long *)&d)[0] = x << 20;
		((unsigned long *)&d)[1] = x >> 12 | 0x3ff00000;
		return d - 1.0;
	case 1:
		((unsigned long *)&d)[1] = x << 20;
		((unsigned long *)&d)[0] = x >> 12 | 0x3ff00000;
		return d - 1.0;
	case 2:
		break;
	}
	return x / 4294967296.0;
}

int MTRand::range(int lo, int hi)
{
	double n;
	int r;

	n = hi - lo + 1;
	r = (int)(real() * n) + lo;
	if (r > hi)
		r = hi;
	if (hi < lo)
		r = 0x80000000;
	return r;
}

void
pathjoin(char *dst, char *dir, char *name)
{
	char *p;

	p = dst;
	while ((*p++ = *dir++) != 0)
		;
	p--;
	if (p > dst && p[-1] == 0x5c)
		p--;
	*p++ = '\\';
	while ((*p++ = *name++) != 0)
		;
}

DWORD
appendfile(char *name, void *buf, DWORD len)
{
	hlog = CreateFile(name, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);
	if (hlog == INVALID_HANDLE_VALUE || hlog == NULL)
		return 0;
	SetFilePointer(hlog, 0, NULL, FILE_END);
	WriteFile(hlog, buf, len, &written, NULL);
	CloseHandle(hlog);
	hlog = 0;
	return written;
}

int
fixline(char *s)
{
	lineend = s;
	while (*lineend) {
		if (*lineend == 0x0d && crflag) {
			*lineend = 0;
			break;
		}
		lineend++;
	}
	if (lineend > s && lineend[-1] == '~')
		lineend--;
	else {
		*lineend++ = '\r';
		*lineend++ = '\n';
	}
	*lineend = 0;
	return lineend - s;
}

void
setlogdir(char *dir)
{
	if (dir && *dir)
		strcpy(logdir, dir);
	if (logdir[0] == 0)
		wsprintf(logdir, "C:");
	wsprintf(logpath, "%s\\%s", logdir, "log.txt");
	wsprintf(errpath, "%s\\%s", logdir, "errors.txt");
}

void
logf(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(logline, fmt, ap);
	if (loghook)
		loghook();
	loglen = fixline(logline);
	appendfile(logpath, logline, loglen);
}

void
logfile(char *name, char *fmt, ...)
{
	va_list ap;

	if (name[0] == '>')
		;
	else if (name[1] == ':')
		wsprintf(filepath, name);
	else
		wsprintf(filepath, "%s\\%s", logdir, name);
	if (fmt == NULL) {
		DeleteFile(filepath);
		return;
	}
	va_start(ap, fmt);
	wvsprintf(logline, fmt, ap);
	if (loghook)
		loghook();
	filelen = fixline(logline);
	appendfile(filepath, logline, filelen);
}

void
logtext(char *name, char *text)
{
	if (name[1] == ':')
		wsprintf(filepath, name);
	else
		wsprintf(filepath, "%s\\%s", logdir, name);
	if (text == NULL) {
		DeleteFile(filepath);
		return;
	}
	textlen = 0;
	while (text[textlen])
		textlen++;
	appendfile(filepath, text, textlen);
}

void
logerr(char *fmt, ...)
{
	va_list ap;

	errcur = logline;
	gettime();
	wsprintf(errcur, "ERROR! %04d_%02d_%02d %02d:%02d:%02d\r\n", year, month, day, hour, minute, second);
	while (*errcur)
		errcur++;
	if (errhook)
		errhook();
	while (*errcur)
		errcur++;
	*errcur++ = ' ';
	*errcur++ = ' ';
	va_start(ap, fmt);
	wvsprintf(errcur, fmt, ap);
	while (*errcur)
		errcur++;
	*errcur++ = '\r';
	*errcur++ = '\n';
	appendfile(logpath, logline, errcur - logline);
	*errcur++ = '\r';
	*errcur++ = '\n';
	appendfile(errpath, logline + 7, errcur - logline - 7);
}

void
clearerr(void)
{
	SetLastError(0);
	lasterr = 0;
	errtext[0] = 0;
	errcode = 0;
}

void
savelasterr(void)
{
	lasterr = GetLastError();
}

void
errmsg(char *buf, char *during)
{
	if (lasterr == 0) {
		if (during == NULL)
			wsprintf(buf, "Error.\r\n");
		else
			wsprintf(buf, "Error during %s.\r\n", during);
		return;
	}
	errnum = lasterr;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, errnum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&msgbuf, 0, NULL);
	if (during == NULL)
		wsprintf(buf, "Error %d:\r\n%s", errnum, msgbuf);
	else
		wsprintf(buf, "Error %d during %s:\r\n%s", errnum, during, msgbuf);
	LocalFree(msgbuf);
}

void
logwinerr(char *during)
{
	savelasterr();
	errmsg(msgline, during);
	savederr = lasterr;
	logf(msgline);
}

void
seterr(char *during)
{
	savelasterr();
	errcode = lasterr;
	if (during == NULL)
		errtext[0] = 0;
	else {
		errmsg(errtext, during);
		logf(errtext);
	}
	lasterr = errcode;
}

void
checkerr(char *during)
{
	savelasterr();
	if (lasterr == 0)
		return;
	seterr(during);
	logf(errtext);
}

void
showerr(void)
{
	if (errcode != 0 && errtext[0])
		MessageBox(NULL, errtext, "GetLastError", MB_ICONINFORMATION);
}

int
fopenw(HANDLE *h, char *name)
{
	clearerr();
	*h = CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (*h != INVALID_HANDLE_VALUE && *h != NULL)
		return 1;
	*h = NULL;
	seterr("b_fop_w.CrF");
	return 0;
}

int
fopenwt(HANDLE *h, char *name)
{
	*h = CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
	if (*h != INVALID_HANDLE_VALUE && *h != NULL)
		return 1;
	*h = NULL;
	return 0;
}

int
fopena(HANDLE *h, char *name)
{
	clearerr();
	*h = CreateFile(name, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);
	if (*h == INVALID_HANDLE_VALUE || *h == NULL) {
		*h = NULL;
		seterr("b_fop_a.CrF");
		return 0;
	}
	clearerr();
	if (SetFilePointer(*h, 0, NULL, FILE_END) == 0xffffffff) {
		seterr("b_fop_a.SFPnt");
		return 0;
	}
	return 1;
}

int
fopenr(HANDLE *h, char *name)
{
	clearerr();
	*h = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (*h != INVALID_HANDLE_VALUE && *h != NULL)
		return 1;
	*h = NULL;
	seterr(NULL);
	return 0;
}

int
fileopen(HANDLE *h, char *name, int write)
{
	if (write)
		return fopenw(h, name);
	return fopenr(h, name);
}

int
fileclose(HANDLE *h)
{
	clearerr();
	if (CloseHandle(*h)) {
		*h = NULL;
		return 1;
	}
	seterr("b_fcl");
	return 0;
}

DWORD
filewrite(void *buf, DWORD size, DWORD n, HANDLE h)
{
	if (size == 0 || n == 0)
		return 0;
	n *= size;
	clearerr();
	if (!WriteFile(h, buf, n, &fwritten, NULL)) {
		seterr("b_fwr");
		return 0;
	}
	return size ? fwritten / size : fwritten;
}

DWORD
fileread(void *buf, DWORD size, DWORD n, HANDLE h)
{
	if (size == 0 || n == 0)
		return 0;
	clearerr();
	if (!ReadFile(h, buf, size * n, &freadn, NULL)) {
		seterr("b_fr");
		logf("  during b_fr( buf, %ld,%ld )", size, n);
		return 0;
	}
	return size ? freadn / size : freadn;
}

DWORD
fileio(void *buf, DWORD size, DWORD n, HANDLE h, int write)
{
	if (write)
		return filewrite(buf, size, n, h);
	return fileread(buf, size, n, h);
}

DWORD
loadfile(void *buf, DWORD size, char *name)
{
	HANDLE h;
	DWORD n;

	memset(buf, 0, size);
	if (!fopenr(&h, name))
		return 0;
	GetFileTime(h, NULL, NULL, &ftime);
	if (seekpos) {
		n = fileseek(h, seekpos);
		seekpos = 0;
		if (n == 0)
			goto done;
	}
	n = fileread(buf, 1, size, h);
done:
	fileclose(&h);
	return n;
}

DWORD
readfile(void *buf, DWORD size, char *name)
{
	HANDLE h;
	DWORD n;

	if (!fopenr(&h, name))
		return 0;
	n = fileread(buf, 1, size, h);
	fileclose(&h);
	return n;
}

DWORD
savefile(void *buf, DWORD size, char *name)
{
	HANDLE h;
	DWORD n;

	if (size == 0)
		return 0;
	if (!fopenw(&h, name))
		return 0;
	n = filewrite(buf, 1, size, h);
	if (settime) {
		SetFileTime(h, &ftime, &ftime, &ftime);
		settime = 0;
	}
	fileclose(&h);
	return n;
}

DWORD
writestr(HANDLE h, char *s)
{
	strlenw = 0;
	while (s[strlenw])
		strlenw++;
	return filewrite(s, 1, strlenw, h);
}

DWORD
writef(HANDLE h, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	wvsprintf(fbuf, fmt, ap);
	return writestr(h, fbuf);
}

int
fileseek(HANDLE h, LONG pos)
{
	DWORD r;

	clearerr();
	if ((r = SetFilePointer(h, pos, NULL, FILE_BEGIN)) == 0xffffffff) {
		seterr("b_fsk");
		return 0;
	}
	if (r == 0)
		return 1;
	return r;
}

DWORD
fileseekcur(HANDLE h, LONG pos)
{
	DWORD r;

	clearerr();
	r = SetFilePointer(h, pos, NULL, FILE_CURRENT);
	if (r == 0xffffffff) {
		seterr("b_fskc");
		return 0;
	}
	return r;
}

DWORD
fileseekend(HANDLE h, LONG pos)
{
	DWORD r;

	clearerr();
	r = SetFilePointer(h, pos, NULL, FILE_END);
	if (r == 0xffffffff) {
		seterr("b_fske");
		return 0;
	}
	return r;
}

DWORD
savetext(char *name, char *text, int calc, DWORD len)
{
	HANDLE h;

	if (calc) {
		len = 0;
		while (text[len])
			len++;
	}
	if (!fopenw(&h, name))
		return 0;
	len = filewrite(text, 1, len, h);
	fileclose(&h);
	return len;
}

DWORD
loadtext(char *buf, DWORD size, char *name)
{
	if (!fopenr(&th, name)) {
		buf[0] = 0;
		return 0;
	}
	tn = fileread(buf, 1, size, th);
	if (tn < size)
		buf[tn] = 0;
	fileclose(&th);
	return tn;
}

DWORD
savebuf(char *name, void *buf, DWORD n)
{
	if (!fopenw(&th2, name))
		return 0;
	tn2 = filewrite(buf, 1, n, th2);
	fileclose(&th2);
	return tn2;
}

int
memdiff(char *a, char *b, int n)
{
	while (n--)
		if (*a++ != *b++)
			return 1;
	return 0;
}

int
setdir(char *dir)
{
	if (SetCurrentDirectory(dir))
		return 0;
	return -1;
}

int
makedir(char *dir)
{
	if (CreateDirectory(dir, NULL))
		return 0;
	return -1;
}

int
removedir(char *dir)
{
	if (RemoveDirectory(dir))
		return 0;
	return -1;
}

void
getdir(char *buf, DWORD size)
{
	if (GetCurrentDirectory(size, buf) == 0)
		buf[0] = 0;
}

char *
getenvvar(char *name)
{
	if (GetEnvironmentVariable(name, envbuf, sizeof envbuf))
		return envbuf;
	return NULL;
}

int
statfile(char *name)
{
	HANDLE h;

	h = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (h == INVALID_HANDLE_VALUE || h == NULL)
		return 0;
	GetFileTime(h, NULL, NULL, &ftime);
	fsize = GetFileSize(h, NULL);
	CloseHandle(h);
	return 1;
}

int
findfirst(char *pattern)
{
	foundsize = 0;
	clearerr();
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
	clearerr();
	if (FindNextFile(hfind, &fd)) {
		foundsize = fd.nFileSizeLow;
		return 0;
	}
	savelasterr();
	if (lasterr == ERROR_NO_MORE_FILES)
		return 1;
	return -1;
}

int
findclose(void)
{
	if (hfind == INVALID_HANDLE_VALUE || !FindClose(hfind))
		return 1;
	hfind = INVALID_HANDLE_VALUE;
	return 0;
}

int
hassubdir(void)
{
	if (findfirst("*.*") != 0) {
		findclose();
		return 0;
	}
	do {
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			continue;
		if (fd.cFileName[0] == '.') {
			if (fd.cFileName[1] == 0)
				continue;
			if (fd.cFileName[1] == '.' && fd.cFileName[2] == 0)
				continue;
		}
		findclose();
		return 1;
	} while (findnext() == 0);
	findclose();
	return 0;
}

int
exists(char *pattern)
{
	int r;

	r = findfirst(pattern);
	findclose();
	if (r)
		return 0;
	return 1;
}

void
filedate(WORD *year, WORD *month, WORD *day, WORD *hour, WORD *min, WORD *sec)
{
	FileTimeToLocalFileTime(&fd.ftLastWriteTime, &lft);
	FileTimeToDosDateTime(&lft, &dosdate, &dostime);
	*day = dosdate;
	*day &= 0x1f;
	*month = dosdate;
	*month &= 0x1e0;
	*month >>= 5;
	*year = dosdate;
	*year &= 0xfe00;
	*year >>= 9;
	*year += 1980;
	*hour = dostime;
	*hour >>= 11;
	*min = dostime;
	*min &= 0x7e0;
	*min >>= 5;
	*sec = dostime;
	*sec &= 0x1f;
	*sec *= 2;
}

int
deletefile(char *name)
{
	if (DeleteFile(name))
		return 0;
	return 1;
}

long
mb64(FILETIME *ft)
{
	DWORD n, hi;

	n = ft->dwHighDateTime;
	if (n & 0xfff00000)
		return -1;
	hi = n;
	n = ft->dwLowDateTime >> 20;
	hi <<= 12;
	return n | hi;
}

DWORD
diskfree(char *path)
{
	OSVERSIONINFO vi;
	ULARGE_INTEGER avail, total, free;
	int i;

	totalmb = 0;
	freemb = 0;
	for (i = 0; i < 3; i++)
		root[i] = path[i];
	root[3] = 0;
	clearerr();
	memset(&vi, 0, sizeof vi);
	vi.dwOSVersionInfoSize = sizeof vi;
	GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && (vi.dwBuildNumber & 0xffff) <= 1000) {
	old:
		if (!GetDiskFreeSpace(root, &spc, &bps, &freec, &totalc)) {
			seterr("b_fop_w.GDskFr");
			return 0;
		}
		freemb = spc;
		freemb *= bps;
		freemb >>= 10;
		freemb *= freec;
		totalmb = spc;
		totalmb *= bps;
		totalmb >>= 10;
		totalmb *= totalc;
		freemb >>= 10;
		totalmb >>= 10;
	} else {
		if (!GetDiskFreeSpaceEx(root, &avail, &total, &free)) {
			if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)
				goto old;
			seterr("b_fop_w.GDskFr");
			return 0;
		}
		freemb = mb64((FILETIME *)&avail);
		totalmb = mb64((FILETIME *)&total);
	}
	return freemb;
}

int
wraptext(HDC dc, char *src, char *dst, int width)
{
	int c;
	char *p;

	while (*src == 13 || *src == 10)
		src++;
	wrapstart = dst;
	for (;;) {
		c = *src;
		if (c == 13) {
			*dst++ = c;
			src++;
		} else if (c == 10)
			src++;
		else if (c == 0)
			break;
		else {
			*dst++ = c;
			src++;
		}
	}
	*dst = c;
	src = dst;
	*++dst = 0;
	*++dst = 0;
	*src = 0;
	wrapend = src;
	p = wrapstart;
	wrapline = p;
	wrapspace = NULL;
	for (; p < wrapend; p++) {
		c = *p;
		if (c == 13) {
			wrapline = p + 1;
			*p = 0;
		} else if (*p == ' ' || c == 0) {
			wrapch = *p;
			*p = 0;
			GetTextExtentPoint32(dc, wrapline, strlen(wrapline), &wrapsize);
			*p = wrapch;
			if (wrapsize.cx > width) {
				if (wrapspace == NULL) {
					*p = 0;
					wrapline = p + 1;
				} else {
					*wrapspace = 0;
					wrapline = wrapspace + 1;
				}
				wrapspace = NULL;
			} else if (wrapch == ' ')
				wrapspace = p;
			if (wrapch == 0)
				wrapline = p + 1;
		}
	}
	return wrapend - wrapstart;
}
int
getprinters(void)
{
	logf("basio_get_verfuegb_drucker()");
	if (hprinters)
		GlobalFree(hprinters);
	hprinters = NULL;
	nprinters = 0;
	for (pnum = 0; pnum < 20; pnum++)
		printers[pnum] = defname;
	memset(msgline, 0, sizeof msgline);
	clearerr();
	enumok = EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, (LPBYTE)msgline, 0, &bufsize, &nreturned);
	if (enumok)
		return 0;
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
		logwinerr("b_g_vf_dr.enum_pr0");
		return -1;
	}
	tries = 0;
again:
	bufsize += 0x800;
	logf("  hPrinterInfo = GlobalAlloc( %lu )", bufsize);
	hprinters = GlobalAlloc(GMEM_FIXED, bufsize);
	if (hprinters == NULL) {
		logwinerr("b_g_vf_dr.GlAlloc");
		return -1;
	}
	clearerr();
	enumok = EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, (LPBYTE)hprinters, bufsize, &needed, &nreturned);
	if (!enumok) {
		logwinerr("b_g_vf_dr.enum_pr");
		if (savederr == ERROR_INSUFFICIENT_BUFFER) {
			if (hprinters) {
				GlobalFree(hprinters);
				hprinters = NULL;
			}
			if (tries++ < 5) {
				logf("buffer zu klein: %lu / %lu - versuche es nochmal", bufsize, needed);
				bufsize = needed;
				bufsize += 0x800;
				goto again;
			}
		}
	}
	logf("EnumPrinters.r%d ( buf %lu Bytes (sizeof struct %lu); need %lu, ret %lu )",
		enumok, bufsize, sizeof(PRINTER_INFO_2), needed, nreturned);
	if (!enumok) {
		if (hprinters) {
			GlobalFree(hprinters);
			hprinters = NULL;
		}
		return -1;
	}
	nprinters = nreturned;
	for (pnum = 0; pnum < nprinters; pnum++) {
		logf("  Printer No. %d:", pnum);
		pname = ((PRINTER_INFO_2 *)hprinters)[pnum].pPrinterName;
		if (pname) {
			logf("    PrinterName:     %s", pname);
			printers[pnum] = pname;
		}
	}
	logf("  Printer enum complete");
	return nprinters;
}

void
freeprinters(void)
{
	if (hprinters) {
		GlobalFree(hprinters);
		hprinters = NULL;
	}
	nprinters = 0;
}

int
printtext(int n, char *title, char *text, char *buf)
{
	if (n >= nprinters || hprinters == NULL) {
		wsprintf(msgline, "Invalid printer (%d) or enum_pr not called.", n);
		return 0;
	}
	/*
	 * Whatever n, the document goes to the first printer.
	 */
	logf("vor CreateDC( No. %d )", 0);
	clearerr();
	prdc = CreateDC(((PRINTER_INFO_2 *)hprinters)->pDriverName,
		((PRINTER_INFO_2 *)hprinters)->pPrinterName, NULL, NULL);
	if (prdc == NULL) {
		logwinerr("b_dr_tx.CrDC");
		return 0;
	}
	logf("CreateDC( %s ).r%d", ((PRINTER_INFO_2 *)hprinters)->pPrinterName, prdc);
	memset(&prdoc, 0, sizeof prdoc);
	prdoc.cbSize = sizeof prdoc;
	prdoc.lpszDocName = title;
	clearerr();
	prret = StartDoc(prdc, &prdoc);
	if (prret <= 0) {
		logwinerr("b_dr_tx.StDoc");
		logf("StartDoc.Unable to start print job -> ret");
		DeleteDC(prdc);
		return 0;
	}
	logf("StartDoc.r%d", prret);
	if (!SetMapMode(prdc, MM_TEXT))
		logwinerr("b_dr_tx.SMpM");
	clearerr();
	prfont = CreateFont(-MulDiv(11, GetDeviceCaps(prdc, LOGPIXELSY), 72), 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH | FF_ROMAN, "Times New Roman");
	if (prfont == NULL)
		logwinerr("b_dr_tx.CrFnt");
	clearerr();
	oldfont = SelectObject(prdc, prfont);
	if (oldfont == NULL)
		logwinerr("b_dr_tx.SlObj.Fnt");
	GetTextExtentPoint32(prdc, "CC", 2, &ccsize);
	lineh = ccsize.cy;
	pagew = GetDeviceCaps(prdc, HORZRES);
	pageh = GetDeviceCaps(prdc, VERTRES);
	startx = pagew / 11;
	starty = pageh / 11;
	pagew -= 2 * startx;
	logf("Seite: %d,%d   Startpos: %d,%d   Zeilenhoehe: %d", pagew, pageh, startx, starty, lineh);
	logf("Res %d,%d, Phys %d,%d, Offs %d,%d ", GetDeviceCaps(prdc, HORZRES), GetDeviceCaps(prdc, VERTRES),
		GetDeviceCaps(prdc, PHYSICALWIDTH), GetDeviceCaps(prdc, PHYSICALHEIGHT),
		GetDeviceCaps(prdc, PHYSICALOFFSETX), GetDeviceCaps(prdc, PHYSICALOFFSETY));
	textend = wraptext(prdc, text, buf, pagew);
	clearerr();
	prret = StartPage(prdc);
	if (prret <= 0)
		logwinerr("b_dr_tx.StPg");
	logf("StartPage.r%d", prret);
	if (!SetMapMode(prdc, MM_TEXT))
		logwinerr("b_dr_tx.SMpM");
	clearerr();
	if (!SelectObject(prdc, prfont))
		logwinerr("b_dr_tx.SlObj.Fnt");
	linex = startx;
	textpos = 0;
	liney = starty;
	for (;;) {
		if (loghook)
			loghook();
		while (buf[textpos] == 0) {
			liney += lineh;
			if (++textpos >= textend)
				goto end;
		}
		if (liney + lineh >= pageh - starty) {
			logf("Seitenende");
			clearerr();
			prret = EndPage(prdc);
			if (prret <= 0)
				logwinerr("b_dr_tx.EndPg");
			logf("EndPage.r%d", prret);
			clearerr();
			prret = StartPage(prdc);
			if (prret <= 0)
				logwinerr("b_dr_tx.StPg");
			logf("StartPage.r%d", prret);
			if (!SetMapMode(prdc, MM_TEXT))
				logwinerr("b_dr_tx.SMpM");
			clearerr();
			if (SelectObject(prdc, prfont) == NULL)
				logwinerr("b_dr_tx.SlObj.Fnt");
			linex = startx;
			liney = starty;
		}
		linelen = lstrlen(buf + textpos);
		logf("Printing '%s' to %d,%d (%d,%d)", buf + textpos, linex, liney, pagew, pageh);
		clearerr();
		if (!TextOut(prdc, linex, liney, buf + textpos, linelen))
			logwinerr("b_dr_tx.TxOut");
		liney += lineh;
		do {
			if (++textpos >= textend)
				goto end;
		} while (buf[textpos] != 0);
		if (++textpos >= textend)
			break;
	}
end:
	logf("Textende");
	clearerr();
	prret = EndPage(prdc);
	if (prret <= 0)
		logwinerr("b_dr_tx.EndPg");
	logf("EndPage.r%d", prret);
	clearerr();
	prret = EndDoc(prdc);
	if (prret <= 0)
		logwinerr("b_dr_tx.EndDoc");
	logf("EndDoc.r%d", prret);
	if (oldfont) {
		clearerr();
		if (SelectObject(prdc, oldfont) == NULL)
			logwinerr("b_dr_tx.SlObj.OFnt");
	}
	if (prfont && !DeleteObject(prfont))
		logf("b_dr_tx.DlObj.Fnt failed");
	if (prdc && !DeleteDC(prdc))
		logf("b_dr_tx.DlDC.Prn failed");
	return 1;
}

int
inibase(void)
{
	return 1;
}

DWORD
ticks(void)
{
	return timeGetTime();
}

void
starttimer1(void)
{
	timer1 = ticks();
}

DWORD
since(DWORD t)
{
	return ticks() - t;
}

DWORD
elapsed1(void)
{
	return ticks() - timer1;
}

void
starttimer2(void)
{
	timer2 = ticks();
}

DWORD
elapsed2(void)
{
	return ticks() - timer2;
}

void
starttimer3(void)
{
	timer3 = ticks();
}

DWORD
elapsed3(void)
{
	return ticks() - timer3;
}

void
gettime(void)
{
	GetLocalTime(&systime);
	year = systime.wYear;
	month = systime.wMonth;
	day = systime.wDay;
	hour = systime.wHour;
	minute = systime.wMinute;
	second = systime.wSecond;
	msec = systime.wMilliseconds;
}

int
getbit(char *p, int n)
{
	if (p[n >> 3] & (1 << (n & 7)))
		return 1;
	return 0;
}

void
setbit(char *p, int n, int v)
{
	if (v)
		p[n >> 3] |= 1 << (n & 7);
	else
		p[n >> 3] &= 0xff - (1 << (n & 7));
}

void
seedrand(unsigned long s)
{
	rng.seed(s);
}

int
random(void)
{
	return rng.range(0, 32766);
}
int
randn(int n)
{
	rnd = rng.range(0, n - 1);
	if (rnd >= n)
		logerr("FEHLER: zuf_bis(%d).ret%d !!!", n, rnd);
	return rnd;
}

DWORD
regquery(HKEY root, char *subkey, char *name, BYTE *buf, DWORD size)
{
	HKEY key;
	DWORD type;
	LONG r;

	if (RegOpenKeyEx(root, subkey, 0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS)
		return 0;
	r = RegQueryValueEx(key, name, NULL, &type, buf, &size);
	RegCloseKey(key);
	if (r != ERROR_SUCCESS)
		return 0;
	return size;
}

DWORD
volserial(void)
{
	char root[8], *p;
	DWORD maxlen, flags;

	serial = 0;
	p = root;
	*p++ = 'C';
	*p++ = ':';
	*p++ = '\\';
	*p = 0;
	if (!GetVolumeInformation(root, NULL, 0, &serial, &maxlen, &flags, NULL, 0)) {
		seterr("b_gvsn");
		return 0;
	}
	return serial;
}

int
setclip(HWND w, char *s)
{
	HGLOBAL h;
	int ok;

	ok = 0;
	if (!OpenClipboard(w))
		return ok;
	if (EmptyClipboard()) {
		h = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, strlen(s) + 1);
		if (h) {
			lstrcpy((char *)GlobalLock(h), s);
			GlobalUnlock(h);
			if (SetClipboardData(CF_TEXT, h))
				ok = 1;
		}
	}
	CloseClipboard();
	return ok;
}

int
getclip(HWND w, char *buf, int size)
{
	HANDLE h;
	char *p;
	int i, n;

	n = 0;
	if (!OpenClipboard(w))
		return n;
	memset(buf, 0, size);
	h = GetClipboardData(CF_TEXT);
	if (h) {
		p = (char *)GlobalLock(h);
		if (p) {
			for (i = 0; i < size - 1 && p[i] != 0; i++)
				buf[i] = p[i];
			GlobalUnlock(h);
			n = i + 1;
		}
	}
	CloseClipboard();
	return n;
}

int
makedirs(char *path, char *buf)
{
	char *p, *q;

	p = buf;
	for (;;) {
		wsprintf(buf, path);
		for (q = p; *q && *q != 0x5c; q++)
			;
		if (*q != 0x5c)
			break;
		if (q[-1] != ':' && q[-1] != 0x5c) {
			*q = '\0';
			CreateDirectory(buf, NULL);
			SetCurrentDirectory(buf);
		}
		p = q + 1;
	}
	return CreateDirectory(path, NULL);
}

int
run(char *file, char *params, char *dir)
{
	if ((UINT)ShellExecute(NULL, "open", file, params, dir, SW_SHOWNORMAL) > 32)
		return 1;
	return 0;
}

int
modulename(char *buf, DWORD size)
{
	if (GetModuleFileName(NULL, buf, size) == 0) {
		buf[0] = 0;
		return 0;
	}
	return 1;
}

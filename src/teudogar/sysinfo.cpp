#include <windows.h>
#include <string.h>

int fopenr(HANDLE *h, char *name);
int fileclose(HANDLE *h);
DWORD fileread(void *buf, DWORD size, DWORD n, HANDLE h);
DWORD fileseekcur(HANDLE h, LONG off);
int storedlen(int id);
DWORD readentry(void *buf, DWORD size, int id);

extern char workbuf[];
extern char version[23];

extern HANDLE vfh;
extern DWORD vpos;
extern DWORD vlim;
extern DWORD vread;
extern DWORD voff;
extern MEMORYSTATUS memstat;

DWORD
availmem(void)
{
	GlobalMemoryStatus(&memstat);
	return memstat.dwAvailPhys;
}

/* read entry id of the data file into a global block, its size into *size */
void *
loadentry(DWORD *size, int id)
{
	void *buf;

	*size = storedlen(id);
	if (*size == 0 || (buf = GlobalAlloc(0, *size)) == NULL) {
		*size = 0;
		return NULL;
	}
	if (!readentry(buf, *size, id)) {
		GlobalFree(buf);
		*size = 0;
		return NULL;
	}
	return buf;
}

int
isdebug(void)
{
	return 0;
}

/* copy the version stamp in file name to buf and return its offset */
DWORD
findversion(char *buf, char *name)
{
	*buf = 0;
	if (!fopenr(&vfh, name))
		return 0;
	for (;;) {
		vread = fileread(workbuf, 1, 0x96640, vfh);
		vlim = vread;
		if (vlim > 40)
			vlim -= 40;
		for (vpos = 0; vpos < vlim; vpos++) {
			if (workbuf[vpos] == ((unsigned char *)version)[0] && workbuf[vpos + 1] == ((unsigned char *)version)[1] &&
				workbuf[vpos + 2] == ((unsigned char *)version)[2] && workbuf[vpos + 3] == ((unsigned char *)version)[3] &&
				workbuf[vpos + 4] == ((unsigned char *)version)[4] && workbuf[vpos + 5] == ((unsigned char *)version)[5] &&
				workbuf[vpos + 6] == ((unsigned char *)version)[6]) {
				memcpy(buf, workbuf + vpos, 25);
				voff = fileseekcur(vfh, 0) + vpos;
				if (voff > vread)
					voff -= vread;
				else
					voff = 0;
				fileclose(&vfh);
				return voff;
			}
		}
		if (vlim + 40 < 0x96640) {
			fileclose(&vfh);
			return 0;
		}
		if (fileseekcur(vfh, 0) >= 40)
			fileseekcur(vfh, -40);
	}
}

HANDLE vfh;
DWORD vpos;
DWORD vlim;
DWORD vread;
DWORD voff;
MEMORYSTATUS memstat;

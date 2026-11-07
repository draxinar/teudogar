/*
 * The formats that the game passes to wsprintfA, and their edges. Built
 * against src/sys/include and fmt.c, the table prints the port's
 * results, and built by mingw-w64 against Windows' own headers (make
 * fmtref), it prints USER32's under Wine, which src/sys/test/fmt.txt
 * holds: make test compares the two. Each line holds the call's
 * arguments, the count wsprintfA returns, and the text, with the bytes
 * outside 0x20 to 0x7e as \xhh.
 */

#include <stdio.h>
#include <string.h>
#include <windows.h>

static char buf[2048];

static void
show(const char *args, int n)
{
	const unsigned char *p;

	printf("%s -> %d ", args, n);
	for (p = (const unsigned char *)buf; *p != '\0'; p++)
		if (*p < 0x20 || *p > 0x7e || *p == '\\')
			printf("\\x%02x", *p);
		else
			putchar(*p);
	putchar('\n');
}

#define T(...) show(#__VA_ARGS__, wsprintfA(buf, __VA_ARGS__))

#ifdef PORT
/* limit prints a line, which fails make test, unless the count and the length are as wanted. */
static void
limit(const char *args, int n, int wantn, int wantlen)
{
	if (n != wantn || (int)strlen(buf) != wantlen)
		printf("%s -> %d, length %d, want %d and %d\n", args, n, (int)strlen(buf), wantn, wantlen);
}

#define L(wantn, wantlen, ...) limit(#__VA_ARGS__, wsprintfA(buf, __VA_ARGS__), wantn, wantlen)
#endif

int
main(void)
{
	static const WCHAR wide[] = { 'w', 'i', 'd', 'e', 0xe9, 0 };
	static char longs[1100];
	int i;

	T("plain text");
	T("%%");
	T("100%% and %");
	T("%d", 0);
	T("%d", -1);
	T("%d", 2147483647);
	T("%d", (int)0x80000000u);
	T("%i", -42);
	T("%u", 4294967295u);
	T("%x", 0xbeef);
	T("%X", 0xbeef);
	T("%#x", 0xbeef);
	T("%#X", 0);
	T("%08x", 0x1234);
	T("%-8x|", 0x1234);
	T("%8d|", -42);
	T("%-8d|", -42);
	T("%08d", -42);
	T("%05u", 42u);
	T("%.3d", 5);
	T("%.0d", 0);
	T("%+d", 5);
	T("% d", 5);
	T("%-#10x|", 255);
	T("%ld", (LONG)-5);
	T("%lu", (DWORD)4000000000u);
	T("%lx", (DWORD)0xdeadbeefu);
	T("%lX", (DWORD)0xdeadbeefu);
	T("%05lu", (DWORD)42);
	T("%hd", 70000);
	T("%hu", 70000);
	T("%hx", 0x12345);
	T("%c", 'A');
	T("%c", 0xe9);
	T("%3c|", 'z');
	T("%-3c|", 'z');
	T("%C", (WCHAR)0x78);
	T("%s", "abc");
	T("%.2s", "abc");
	T("%5s|", "ab");
	T("%-5s|", "ab");
	T("%5.1s|", "ab");
	T("%s", "caf\xe9");
	T("%s", (char *)NULL);
	T("%S", wide);
	T("%.3S", wide);
	T("%y");
	T("%2y|");
	T("%-3y|");
	T("%02y|");
	T("%5.2y|");
	T("%05.2y|");
	T("%-5.2y|");
	T("%3.9y|");
	T("%5.5y|");
	T("%5.2f|");
	T("%s and %d and %c", "one", 2, '3');
	memset(longs, 'x', sizeof longs - 1);
	T("%s", longs);
	T("[%s]", longs);
	longs[1023] = '\0';
	T("%s", longs);
	longs[1022] = '\0';
	T("%s", longs);
	T("%s%c", longs, 'z');
	T("%s%%", longs);
	T("%s%-5d|", longs, 7);
	longs[1021] = '\0';
	T("%s%c", longs, 'z');
	T("%s%d", longs, -42);
	T("%1023d", 7);
	T("%1022d", 7);
	T("%1500d", 7);
	T("%-1500d|", 7);
	T("%01500d", 7);
	T("%.1023d", 7);
	T("%.1022d", 7);
	T("%.1500d", -7);
	T("%1023c", 'z');
	T("%1022c", 'z');
	T("%1023s", "ab");
	T("%1022s", "ab");
	T("%1023y|");
	T("%1022y|");
	longs[1019] = '\0';
	T("%s%10.2y|", longs);
	longs[1017] = '\0';
	T("%s%#x", longs, 0xbeef);
	longs[1016] = '\0';
	T("%s%#x", longs, 0xbeef);
	longs[1015] = '\0';
	T("%s%#x", longs, 0xbeef);
	longs[1000] = '\0';
	T("%s%#xABCDEFGHIJKLMNOPQRSTUVWXYZ", longs, 0xbeef);
	T("%s%-#10x|", longs, 255);
	T("%s%#010x|", longs, 255);
	T("%#.1021x", 0xbeef);
	T("%#1021x", 0xbeef);
	T("%-#1021x|", 0xbeef);
	for (i = 0; i < 3; i++)
		T("%d%%", i * 33);
#ifdef PORT
	/* USER32 runs past its 1,024 bytes on these, where the port stops at 1,023. */
	memset(longs, 'x', sizeof longs - 1);
	for (i = 1018; i <= 1022; i++) {
		longs[i] = '\0';
		L(1024, 1023, "%s%#x", longs, 0xbeef);
		longs[i] = 'x';
	}
	longs[1021] = '\0';
	L(1022, 1022, "%s%#x", longs, 0xb);
	L(1024, 1023, "%#.1022x", 0xbeef);
	L(1024, 1023, "%#1500x", 0xbeef);
	L(1024, 1023, "%-#1500x", 0xbeef);
	L(1024, 1023, "%#01500x", 0xbeef);
#endif
	return 0;
}

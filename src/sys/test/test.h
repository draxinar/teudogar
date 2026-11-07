/*
 * What the unit tests share, which stub.c defines. Each test includes
 * <windows.h> and <ddraw.h> first.
 */

extern int failed;	/* whether a check failed, the test's status */

void check(int ok, const char *what);
int fails(void (*f)(void *), void *arg);
void setrect(RECT *r, int x0, int y0, int x1, int y1);
LPDIRECTDRAWSURFACE offscreen(LPDIRECTDRAW dd, int w, int h);
unsigned char *lock(LPDIRECTDRAWSURFACE s, LONG *pitch);

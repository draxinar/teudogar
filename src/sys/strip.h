/*
 * The wide screen's strips, which src/sys and the game's hooks share: its
 * first and last columns on the map stand for the frame's, where the
 * gestures and the hot corner lie.
 */

enum {
	Leftstrip = 29,	/* the columns that stand for the frame's first */
	Rightstrip = 40	/* and for the frame's last */
};

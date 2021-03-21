#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include "winconf.h"

#define BOARD_LEFT 0
#define BOARD_TOP 0
#define BOARD_RIGHT (WIN_SIZE_X - 24)
#define BOARD_BOTTOM (WIN_SIZE_Y - 1)
#define BOARD_SIZE_X_DELTA (BOARD_RIGHT - 1 - BOARD_LEFT + 1)
#define BOARD_SIZE_Y_DELTA (BOARD_BOTTOM - 1 - BOARD_TOP + 1)

struct board {
	RECT rect;
	char sym;	
};

struct board * board_alloc(RECT rect, char sym);
void board_draw(const struct board *b, int delay);
void board_free(struct board *b);

#endif /* BOARD_H */

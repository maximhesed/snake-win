#include "board.h"

struct board *
board_alloc(RECT rect, char sym)
{
	struct board *b = malloc(sizeof(struct board));
	
	b->rect = rect;
	b->sym = sym;
	
	return b;	
}

void
board_draw(const struct board *b, int delay)
{
	SetConsoleTextAttribute(h, FG_LIGHTGRAY | 0);
	
	for (int x = b->rect.left + 1; x < b->rect.right; x++) {
		Sleep(delay);

		SetConsoleCursorPosition(h, (COORD) {x, b->rect.top});
		printf("%c", b->sym);

		Sleep(delay);
		
		SetConsoleCursorPosition(h, (COORD) {x, b->rect.bottom});
		printf("%c", b->sym);
	}

	for (int y = b->rect.top + 1; y < b->rect.bottom; y++) {
		Sleep(delay);
		
		SetConsoleCursorPosition(h, (COORD) {b->rect.left, y});
		printf("%c", b->sym);

		Sleep(delay);
		
		SetConsoleCursorPosition(h, (COORD) {b->rect.right, y});
		printf("%c", b->sym);
	}
	
	Sleep(200);
}

void
board_free(struct board *b)
{
	free(b);	
}

#include "score.h"

void
score_draw(int x, int y, struct snake *s)
{
	SetConsoleTextAttribute(h, FG_LIGHTGRAY | 0);
	
	SetConsoleCursorPosition(h, (COORD) {x, y});
	printf("score: %d", s->score);

	SetConsoleCursorPosition(h, (COORD) {x, y + 1});
	printf("length: %d", s->length);
}

void
score_add(int v, struct snake *s)
{
	s->score += v;
}

void
score_add_draw(struct food *f, int yfset)
{
	SetConsoleTextAttribute(h, FG_LIGHTGRAY | 0);
	
	for (int i = 0; i < 6; i++) {
		SetConsoleCursorPosition(h, (COORD) {WIN_SIZE_X - 7 + i, BOARD_TOP + yfset});
		printf("%c", ' ');
	}

	SetConsoleTextAttribute(h, FG_LIGHTGREEN | 0);
	SetConsoleCursorPosition(h, (COORD) {WIN_SIZE_X - 5, BOARD_TOP + yfset});
	printf("%4d", f->score);
	SetConsoleCursorPosition(h, (COORD) {WIN_SIZE_X - digits_count(f->score) - 2, BOARD_TOP + yfset});
	printf("%c", '+');
	SetConsoleTextAttribute(h, FG_LIGHTGRAY | 0);
}

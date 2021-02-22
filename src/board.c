#include "board.h"

void
board_draw(int delay)
{
	SetConsoleTextAttribute(h, FG_LIGHTGRAY | 0);
	
	for (int x = BOARD_LEFT + 1; x < BOARD_RIGHT; x++) {
		Sleep(delay);

		SetConsoleCursorPosition(h, (COORD) {x, BOARD_TOP});
		printf("%c", BOARD_SYM);

		Sleep(delay);
		
		SetConsoleCursorPosition(h, (COORD) {x, BOARD_BOTTOM});
		printf("%c", BOARD_SYM);
	}

	for (int y = BOARD_TOP + 1; y < BOARD_BOTTOM; y++) {
		Sleep(delay);
		
		SetConsoleCursorPosition(h, (COORD) {BOARD_LEFT, y});
		printf("%c", BOARD_SYM);

		Sleep(delay);
		
		SetConsoleCursorPosition(h, (COORD) {BOARD_RIGHT, y});
		printf("%c", BOARD_SYM);
	}
	
	Sleep(200);
}

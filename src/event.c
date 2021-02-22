#include "event.h"

void
evt_menu(struct menu *m)
{
	system("cls");
	
	menu_init(m);
}

void
evt_pause(struct menu *m)
{
	bool isPause = true;
	
	SetConsoleCursorPosition(h, (COORD) {WIN_SIZE_X - 6, WIN_SIZE_Y - 1});
	SetConsoleTextAttribute(h, FG_WHITE | BG_GREEN);
	printf("PAUSE");
	SetConsoleTextAttribute(h, FG_LIGHTGRAY | 0);
	
	while (isPause) {
		int key = 0;
		
		while (kbhit())
			key = getch();
		
		switch (key) {
			case KEY_ESCAPE:
				evt_menu(m);
				
				isPause = false;
				
				break;
		
			case 'p':
				SetConsoleCursorPosition(h, (COORD) {WIN_SIZE_X - 6, WIN_SIZE_Y - 1});
				printf("     ");
			
				isPause = false;
			
				break;
		}
	}	
}

void
evt_gameover(void)
{
	Sleep(500);

	system("cls");

	SetConsoleCursorPosition(h, (COORD) {EVT_END_X, EVT_END_Y});
	printf("GAME OVER!");

	Sleep(2000);
}

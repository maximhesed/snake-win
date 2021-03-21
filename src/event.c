#include "event.h"

static void clear(int yfset);

static void
clear(int yfset)
{
	for (int y = 0 + yfset; y < WIN_SIZE_Y + yfset; y++) {
		for (int x = 0; x < WIN_SIZE_X; x++) {
			SetConsoleCursorPosition(h, (COORD) {x, y});
			printf("%c", ' ');
		}
	}
}

void
evt_menu(struct menu *m)
{
	system("cls");
	
	menu_init(m);
}

int
evt_pause(struct game_ctx *ctx, int yfset)
{	
	SetConsoleCursorPosition(h, (COORD) {WIN_SIZE_X - 6, WIN_SIZE_Y - 1 + yfset});
	SetConsoleTextAttribute(h, FG_WHITE | BG_GREEN);
	printf("PAUSE");
	SetConsoleTextAttribute(h, FG_LIGHTGRAY | 0);

	ctx->isPause = true;
	while (ctx->isPause) {
		int key = 0;
		
		while (kbhit())
			key = getch();
		
		switch (key) {
			case 'p':
				if (!ctx->friend.isPause) {
					if (ctx->conOpt != CON_SINGLE) {	
						if (req_send_resume(ctx) < 0)
							return -1;
					}
					
					ctx->isPause = false;
				}
			
				break;
		}
		
		Sleep(50);
		
		if (ctx->conOpt != CON_SINGLE) {
			if (req_recv(ctx) < 0)
				return -1;
		}
	}
	
	SetConsoleCursorPosition(h, (COORD) {WIN_SIZE_X - 6, WIN_SIZE_Y - 1 + yfset});
	printf("     ");
	
	return 0;
}

void
evt_win(int yfset)
{
	Sleep(500);

	clear(yfset);
	wprintfr((COORD) {WIN_SIZE_X / 2 - 4, WIN_SIZE_Y / 2 - 1 + yfset}, "YOU WON!");

	Sleep(2000);	
}

void
evt_lose(int yfset)
{
	Sleep(500);

	clear(yfset);
	wprintfr((COORD) {WIN_SIZE_X / 2 - 4, WIN_SIZE_Y / 2 - 1 + yfset}, "YOU LOSE.");

	Sleep(2000);
}

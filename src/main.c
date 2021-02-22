#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "winconf.h"
#include "snake.h"
#include "food.h"
#include "board.h"
#include "score.h"
#include "event.h"
#include "menu.h"
#include "game_ctx.h"
#include "log.h"

static void menu_cb_new_game(void *data);
static void menu_cb_exit(void *data);

int main(void)
{
	SMALL_RECT rect = {0, 0, WIN_SIZE_X - 1, WIN_SIZE_Y - 1};
	COORD size = {WIN_SIZE_X, WIN_SIZE_Y};
	CONSOLE_CURSOR_INFO ci = {100, false};
	extern HANDLE h;
	extern HWND hWnd;
	struct menu *m = menu_alloc();
	struct game_ctx ctx;
	bool isExit = false;
	extern FILE *file_log;

	if ((file_log = fopen("log", "w")) == NULL)
		return -1;

	h = GetStdHandle(STD_OUTPUT_HANDLE);
	hWnd = GetConsoleWindow();
	
	SetConsoleWindowInfo(h, true, &rect);
	SetConsoleScreenBufferSize(h, size);
	SetWindowPos(hWnd, NULL, WIN_POS_X, WIN_POS_Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	SetConsoleCursorInfo(h, &ci);
	
	memset(&ctx, 0, sizeof(struct game_ctx));
	
	menu_add_item(m, "New game", menu_cb_new_game, &ctx);
	menu_add_item(m, "Exit", menu_cb_exit, &isExit);
	
	srand(time(NULL));

	evt_menu(m);
	
	while (!isExit) {
		int key = 0;
		
		if (snake_check_init(ctx.s)) {
			while (kbhit())
				key = getch();
		}
		
		snake_update_dir(ctx.s, key);
		snake_update_head(ctx.s);
		snake_move(ctx.s);

		if (snake_check_collide_board(ctx.s)) {
			evt_gameover();
			evt_menu(m);
			
			continue;
		}
		
		food_draw(ctx.f);
		snake_draw(ctx.s);
		snake_erase_tail_end(ctx.s);
		snake_check_collide_food(ctx.s, ctx.f);
		
		if (snake_check_collide_self(ctx.s)) {
			evt_gameover();
			evt_menu(m);
		
			continue;	
		}
		
		score_draw(SCORE_POS_X, SCORE_POS_Y, ctx.s);
		score_add(SCORE_ADD_VALUE, ctx.s);
		
		switch (key) {
			case KEY_ESCAPE:
				evt_menu(m);
				
				break;
				
			case 'p':
				evt_pause(m);
				
				break;
		}
		
		Sleep(50);
	}
	
	fclose(file_log);
	menu_free(m);
	snake_free(ctx.s);
	food_free(ctx.f);
	
	return 0;
}

static void
menu_cb_new_game(void *data)
{
	struct game_ctx *ctx = data;
	
	system("cls");

	snake_free(ctx->s);
	food_free(ctx->f);
	
	ctx->s = snake_alloc();
	ctx->f = food_alloc();

	board_draw(10);
	
	food_gen(ctx->f, ctx->s);
	food_draw(ctx->f);
}

static void
menu_cb_exit(void *data)
{
	bool *isExit = (bool *) data;
	
	*isExit = true;
}

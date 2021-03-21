#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
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
#include "reqs.h"

static void menu_cb_new_game(void *data);
static void menu_cb_create_con(void *data);
static void menu_cb_connect_to_con(void *data);
static void menu_cb_network_game(void *data);
static void menu_cb_exit(void *data);
static int inet_pton(int inet, const char *src, void *dst);

int main(void)
{	
	SMALL_RECT rect = {0, 0, WIN_SIZE_X - 1, WIN_SIZE_Y - 1};
	COORD size = {WIN_SIZE_X, WIN_SIZE_Y};
	CONSOLE_CURSOR_INFO ci = {100, false};
	CONSOLE_FONT_INFOEX finfo = {
		.cbSize = sizeof(CONSOLE_FONT_INFOEX),
		.nFont = 0,
		.dwFontSize = (COORD) {8, 8},
		.FontFamily = FF_DONTCARE,
		.FontWeight = FW_NORMAL
	};
	extern HANDLE h;
	extern HWND hWnd;
	struct menu *m = menu_alloc();
	struct game_ctx ctx;
	extern FILE *fileLog;
	char secs[16];
	int fstat;
	
	memset(secs, 0, sizeof(secs));
	sprintf(secs, "%lu", (unsigned long) time(NULL));
	wcscpy(finfo.FaceName, L"Terminal");

	fileLog = fopen(secs, "w");
	if (fileLog == NULL)
		return -1;
		
	fstat = setvbuf(fileLog, NULL, _IONBF, 0);
	if (fstat != 0)
		return -1;

	h = GetStdHandle(STD_OUTPUT_HANDLE);
	hWnd = GetConsoleWindow();
	
	SetConsoleWindowInfo(h, true, &rect);
	SetConsoleScreenBufferSize(h, size);
	SetWindowPos(hWnd, NULL, WIN_POS_X, WIN_POS_Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	SetConsoleCursorInfo(h, &ci);
	SetCurrentConsoleFontEx(h, false, &finfo);
	
	memset(&ctx, 0, sizeof(struct game_ctx));
	memset(&ctx.fc, 0, sizeof(struct food_ctx));
	memset(&ctx.friend, 0, sizeof(struct friend));

	menu_add_item(m, "New game", menu_cb_new_game, &ctx);
	menu_add_item(m, "Network game", menu_cb_network_game, &ctx);
	menu_add_item(m, "Exit", menu_cb_exit, &ctx);
	
	srand(time(NULL));

	ctx.isMenu = true;
	
	while (!ctx.isExit) {
		int key;
		enum dir dir;
		
		if (ctx.isMenu) {
			ctx.isMenu = false;
			
			if (ctx.conOpt != CON_SINGLE) {
				char cmd[32];
				
				closesocket(ctx.curFd);
				closesocket(ctx.friendFd);
				WSACleanup();
				
				memset(cmd, 0, sizeof(cmd));
				sprintf(cmd, "mode %d, %d", WIN_SIZE_X, WIN_SIZE_Y);
				system(cmd);
				
				ctx.conOpt = CON_SINGLE;
			}
			
			evt_menu(m);
			
			continue;
		}
		
		dir = ctx.s->u[0].dir;
		
		if (snake_check_init(ctx.s)) {
			while (kbhit())
				key = getch();
		}
		
		snake_update_dir(ctx.s, key);
		snake_update_head(ctx.s);
		snake_erase_tail_end(ctx.s);
		
		if (ctx.s->u[0].dir != dir) {
			if (ctx.conOpt != CON_SINGLE) {
				if (req_send_snake(&ctx) < 0)
					continue;
			}
		}
		
		snake_move(ctx.s);

		if (snake_check_collide_board(ctx.s, 0))
			snake_cross_board(ctx.s, 0);
		
		food_draw_super(ctx.f, &ctx.fc);
		
		snake_draw(ctx.s);
		
		if (snake_check_collide_food(ctx.s, ctx.f)) {
			ctx.s->length++;
			ctx.s->score += ctx.f->score;
			
			if (ctx.conOpt != CON_SINGLE) {
				if (req_send_snake_collide_food(&ctx) < 0)
					continue;
			}

			score_add_draw(ctx.f, 0);
	
			food_gen(ctx.f, &ctx.fc, ctx.s);
			food_draw(ctx.f);
			
			if (ctx.conOpt != CON_SINGLE) {
				if (req_send_food(&ctx) < 0)
					continue;
			}
			
			if (snake_check_win(ctx.s)) {
				if (ctx.conOpt != CON_SINGLE) {
					if (req_send_event_win(&ctx) < 0)
						continue;	
				}
				
				evt_win(0);
				
				ctx.isMenu = true;
				
				continue;	
			}
		}
		
		if (snake_check_collide_self(ctx.s)) {
			if (ctx.conOpt != CON_SINGLE) {
				if (req_send_event_lose(&ctx) < 0)
					continue;	
			}
			
			evt_lose(0);
			
			ctx.isMenu = true;
		
			continue;	
		}
		
		score_draw(SCORE_POS_X, SCORE_POS_Y, ctx.s);
		score_add(SCORE_ADD_VALUE, ctx.s);
		
		switch (key) {
			case 'f':
				ctx.isMenu = true;
				
				continue;
				
			case 'p':
				if (ctx.conOpt != CON_SINGLE) {
					if (req_send_pause(&ctx) < 0)
						continue;
				}
					
				if (evt_pause(&ctx, 0) < 0)
					continue;
				
				break;
				
			default:
				break;
		}
		
		if (ctx.conOpt != CON_SINGLE) {
			snake_erase_tail_end(ctx.friend.s);
			
			snake_move(ctx.friend.s);
			
			if (snake_check_collide_board(ctx.friend.s, WIN_SIZE_Y + 2))
				snake_cross_board(ctx.friend.s, WIN_SIZE_Y + 2);
			
			food_draw_super(ctx.friend.f, &ctx.fc);
			
			snake_draw(ctx.friend.s);
			
			score_draw(SCORE_POS_X, SCORE_POS_Y + WIN_SIZE_Y + 2, ctx.friend.s);
			score_add(SCORE_ADD_VALUE, ctx.friend.s);
			
			if (req_recv(&ctx) < 0)	
				continue;
		}

		Sleep(50);
	}
	
	fclose(fileLog);
	menu_free(m);
	snake_free(ctx.s);
	food_free(ctx.f);
	board_free(ctx.b);
	list_free(ctx.fc.syms, NULL);
	list_free(ctx.fc.colors, NULL);
	list_free(ctx.fc.scores, NULL);
	
	if (ctx.conOpt != CON_SINGLE) {
		closesocket(ctx.curFd);
		closesocket(ctx.friendFd);
		WSACleanup();
		
		snake_free(ctx.friend.s);
		food_free(ctx.friend.f);
		board_free(ctx.friend.b);
	}
	
	return 0;
}

static void
menu_cb_new_game(void *data)
{
	struct game_ctx *ctx = data;
	RECT board_rect = {BOARD_LEFT, BOARD_TOP, BOARD_RIGHT, BOARD_BOTTOM};
	
	if (ctx->isExit)
		return;
	
	if (ctx->conOpt != CON_SINGLE) {
		char cmd[32];
		
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd, "mode %d, %d", WIN_SIZE_X, WIN_SIZE_Y + WIN_SIZE_Y + 3);
		system(cmd);
	}
	
	system("cls");

	snake_free(ctx->s);
	food_free(ctx->f);
	board_free(ctx->b);
	list_free(ctx->fc.syms, NULL);
	list_free(ctx->fc.colors, NULL);
	list_free(ctx->fc.scores, NULL);
	
	ctx->s = snake_alloc(0);
	ctx->f = food_alloc();
	ctx->b = board_alloc(board_rect, '@');
	ctx->isPause = false;
	ctx->isExit = false;
	ctx->isMenu = false;
	ctx->fc.syms = list_alloc();
	ctx->fc.colors = list_alloc();
	ctx->fc.scores = list_alloc();
	
	list_appenda(ctx->fc.syms, sizeof(char), &(char) {'*'});
	list_appenda(ctx->fc.syms, sizeof(char), &(char) {'$'});
	list_appenda(ctx->fc.syms, sizeof(char), &(char) {'f'});
	list_appenda(ctx->fc.colors, sizeof(int), &(int) {FG_BROWN});
	list_appenda(ctx->fc.colors, sizeof(int), &(int) {FG_LIGHTGREEN});
	list_appenda(ctx->fc.colors, sizeof(int), &(int) {FG_LIGHTCYAN});
	list_appenda(ctx->fc.colors, sizeof(int), &(int) {FG_LIGHTRED});
	list_appenda(ctx->fc.colors, sizeof(int), &(int) {FG_LIGHTMAGENTA});
	list_appenda(ctx->fc.colors, sizeof(int), &(int) {FG_YELLOW});
	list_appenda(ctx->fc.scores, sizeof(int), &(int) {5});
	list_appenda(ctx->fc.scores, sizeof(int), &(int) {10});
	list_appenda(ctx->fc.scores, sizeof(int), &(int) {25});
	list_appenda(ctx->fc.scores, sizeof(int), &(int) {100});
	list_appenda(ctx->fc.scores, sizeof(int), &(int) {250});
	list_appenda(ctx->fc.scores, sizeof(int), &(int) {500});
	list_appenda(ctx->fc.scores, sizeof(int), &(int) {1000});
	
	if (ctx->conOpt != CON_SERVER && ctx->conOpt != CON_CLIENT)
		ctx->conOpt = CON_SINGLE;

	board_draw(ctx->b, 10);
		
	snake_update_head(ctx->s);
	
	food_gen(ctx->f, &ctx->fc, ctx->s);
	food_draw(ctx->f);
	
	if (ctx->conOpt != CON_SINGLE) {
		snake_free(ctx->friend.s);
		food_free(ctx->friend.f);
		board_free(ctx->friend.b);
		
		board_rect.top = BOARD_TOP + WIN_SIZE_Y + 2;
		board_rect.bottom = BOARD_BOTTOM + WIN_SIZE_Y + 2;
		
		ctx->friend.s = snake_alloc(WIN_SIZE_Y + 2);
		ctx->friend.f = food_alloc();
		ctx->friend.b = board_alloc(board_rect, '@');
		ctx->friend.isPause = false;
		
		if (req_send_snake(ctx) < 0)
			return;
		
		if (req_send_food(ctx) < 0)
			return;
			
		board_draw(ctx->friend.b, 10);
	}
}

static void
menu_cb_create_con(void *data)
{
	struct game_ctx *ctx = data;
	int fstat;
	
	ctx->conOpt = CON_SERVER;
	ctx->wsaVer = MAKEWORD(2, 2);
	
	SetConsoleCursorPosition(h, (COORD) {0, 0});
	
	fstat = WSAStartup(ctx->wsaVer, &ctx->wsaData);
	if (fstat != 0) {
		fprintf(fileLog, "WSAStartup()\n");
		
		ctx->isExit = true;
		
		return;
	}
	
	if (LOBYTE(ctx->wsaData.wVersion) != 2 || HIBYTE(ctx->wsaData.wVersion) != 2) {
		fprintf(fileLog, "Could not find a usable version of Winsock.dll.\n");
		
		ctx->isExit = true;
		
		return;
	}
	
	ctx->curFd = socket(PF_INET, SOCK_STREAM, 0);
	if (ctx->curFd == INVALID_SOCKET) {
		fprintf(fileLog, "socket()\n");
		
		ctx->isExit = true;
		
		return;
	}
	
	memset(&ctx->comAddr, 0, sizeof(ctx->comAddr));
	ctx->comAddr.sin_family = PF_INET;
	ctx->comAddr.sin_port = htons(55555); /* Snake's port. */
	ctx->comAddr.sin_addr.s_addr = INADDR_ANY; /* All gateway's machines. */
	
	fstat = bind(ctx->curFd, (LPSOCKADDR) &ctx->comAddr, sizeof(ctx->comAddr));
	if (fstat == SOCKET_ERROR) {
		fprintf(fileLog, "bind()\n");

		ctx->isExit = true;
		
		return;
	}
	
	fstat = listen(ctx->curFd, 1);
	if (fstat == SOCKET_ERROR) {
		fprintf(fileLog, "listen()\n");
		
		ctx->isExit = true;
		
		return;
	}
	
	system("cls");
	wprintfr((COORD) {0, 0}, "Waiting for a friend...");
	
	ctx->friendFd = accept(ctx->curFd, (LPSOCKADDR) NULL, NULL);
	if (ctx->friendFd == INVALID_SOCKET) {
		fprintf(fileLog, "accept()\n");
		
		ctx->isExit = true;
		
		return;
	}
	
	system("cls");
	wprintfr((COORD) {0, 0}, "A friend has been found.");
	
	if (req_send_start(ctx) < 0)
		return;
}

static void
menu_cb_connect_to_con(void *data)
{
	struct game_ctx *ctx = data;
	int fstat;
	char addr[32];
	int br;
	char buf[1024];
	
	memset(addr, 0, sizeof(addr));
	
	system("cls");
	wprintfr((COORD) {0, 0}, "Type IP address of the friend: ");
	scanf("%s", addr);
	
	ctx->conOpt = CON_CLIENT;
	ctx->wsaVer = MAKEWORD(1, 0);
	
	WSAStartup(ctx->wsaVer, &ctx->wsaData);
	
	ctx->friendFd = socket(PF_INET, SOCK_STREAM, 0);
	if (ctx->friendFd == INVALID_SOCKET) {
		fprintf(fileLog, "socket()\n");
		
		ctx->isExit = true;
		
		return;
	}
	
	memset(&ctx->comAddr, 0, sizeof(ctx->comAddr));
	ctx->comAddr.sin_family = PF_INET;
	ctx->comAddr.sin_port = htons(55555);
	
	fstat = inet_pton(PF_INET, addr, &ctx->comAddr.sin_addr);
	if (fstat == -1) {
		fprintf(fileLog, "inet_pton()\n");
		
		ctx->isExit = true;
		
		return;
	}
	
	fstat = connect(ctx->friendFd, (LPSOCKADDR) &ctx->comAddr, sizeof(ctx->comAddr));
	if (fstat == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAECONNREFUSED) {
			system("cls");
			wprintfr((COORD) {0, 0}, "Couldn't connect to the friend.");
			
			getch();
			
			ctx->isMenu = true;
		}
		else {
			fprintf(fileLog, "connect()\n");
		
			ctx->isExit = true;
		}
		
		return;
	}
	
	br = recv(ctx->friendFd, buf, sizeof(buf), 0);
	if (br == 0) {
		fprintf(fileLog, "The connection was closed.\n");
		
		ctx->isExit = true;
		
		return;
	}
	else if (br < 0) {
		fprintf(fileLog, "Failed to read a data (an error's code: %d).\n", WSAGetLastError());
		
		ctx->isExit = true;
		
		return;
	}
}

static void
menu_cb_network_game(void *data)
{
	struct game_ctx *ctx = data;
	struct menu *m = menu_alloc();
	
	menu_add_item(m, "Create connection", menu_cb_create_con, ctx);
	menu_add_item(m, "Connect to connection", menu_cb_connect_to_con, ctx);
	
	evt_menu(m);
	
	/* If the menu flag was set, then something is wrong.
	 * For example, a friend wasn't found. */
	if (!ctx->isMenu) {
		int fstat;
		
		fstat = ioctlsocket(ctx->friendFd, FIONBIO, &(u_long) {1});
		if (fstat != 0) {
			fprintf(fileLog, "Failed to set non-blocking mode for ctx->friendFd.\n");
		
			ctx->isExit = true;
			
			return;
		}
		
		/* Initialize a game by default, but save user's connection option. */
		menu_cb_new_game(ctx);
	}
	
	menu_free(m);
}

static void
menu_cb_exit(void *data)
{
	struct game_ctx *ctx = data;
	
	ctx->isExit = true;
}

static int
inet_pton(int inet, const char *src, void *dst) 
{
	struct sockaddr_storage storage;
	int size = sizeof(storage);
	char src_copy[INET6_ADDRSTRLEN + 1];
	
	memset(&storage, 0, sizeof(storage));
	
	strncpy(src_copy, src, INET6_ADDRSTRLEN);
	
	if (WSAStringToAddress(src_copy, inet, NULL, (LPSOCKADDR) &storage, &size) == 0) {
		switch (inet) {
			case PF_INET:
				*(struct in_addr *) dst = ((struct sockaddr_in *) &storage)->sin_addr;
				
				return 0;
				
			case PF_INET6:
				*(struct in6_addr *) dst = ((struct sockaddr_in6 *) &storage)->sin6_addr;
				
				return 0;
		}	
	}
	
	return -1;
}

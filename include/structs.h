#ifndef STRUCTS_H
#define STRUCTS_H

#include <windows.h>

enum dir {
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN
};

struct unit {
	char sym;
	COORD pos;
	COORD prev;
	int color;
	enum dir dir;
};

struct snake {
	struct unit *u;
	int length;
	int score;
};

struct food {
	char sym;
	COORD pos;
	int color;
	int score;
};

enum menu_status {
	MENU_STATUS_OPENED,
	MENU_STATUS_CLOSED	
};

struct menu {
	int offset;
	struct list *items;
	enum menu_status status;
	int key;
	unsigned item_len_max;
};

#endif /* STRUCTS_H */

#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "winconf.h"
#include "keys.h"
#include "game_ctx.h"
#include "event.h"
#include "structs.h"
#include "log.h"

#define getmposx(m) (WIN_SIZE_X / 2 - (m)->item_len_max / 2 + 1)
#define getmposy(m) (WIN_SIZE_Y / 2 - list_count((m)->items) + 1)

typedef void (*menu_cb)(void *);

struct item {
	char name[32];
	unsigned indent;
	menu_cb cb;
	void *data;
};

struct menu * menu_alloc(void);
void menu_add_item(struct menu *m, const char *name, menu_cb cb, void *data);
void menu_draw(const struct menu *m);
void menu_refresh(struct menu *m);
void menu_process(struct menu *m);
void menu_init(struct menu *m);
void menu_free(struct menu *m);

#endif /* MENU_H */

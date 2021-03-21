#ifndef EVENT_H
#define EVENT_H

#include <stdio.h>
#include <stdbool.h>
#include "winconf.h"
#include "menu.h"
#include "structs.h"
#include "board.h"
#include "food.h"
#include "snake.h"
#include "game_ctx.h"
#include "reqs.h"

void evt_menu(struct menu *m);
int evt_pause(struct game_ctx *ctx, int yfset);
void evt_win(int yfset);
void evt_lose(int yfset);

#endif /* EVENT_H */

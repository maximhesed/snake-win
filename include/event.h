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

#define EVT_WELCOME_X (WIN_SIZE_X / 2 - 13)
#define EVT_WELCOME_Y (WIN_SIZE_Y / 2 - 1)
#define EVT_END_X (WIN_SIZE_X / 2 - 4)
#define EVT_END_Y (WIN_SIZE_Y / 2)

void evt_menu(struct menu *m);
void evt_pause(struct menu *m);
void evt_gameover(void);

#endif /* EVENT_H */

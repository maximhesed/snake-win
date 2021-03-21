#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <stdbool.h>
#include "structs.h"
#include "board.h"
#include "unit.h"
#include "score.h"
#include "food.h"
#include "game_ctx.h"
#include "reqs.h"

#define SNAKE_LEN_MAX (BOARD_SIZE_X_DELTA * BOARD_SIZE_Y_DELTA)
#define SNAKE_TAIL_SYM '+'
#define SNAKE_POS_X_START 10
#define SNAKE_POS_Y_START 6
#define SNAKE_LEN_START 3

struct snake * snake_alloc(int yfset);
void snake_move(struct snake *s);
void snake_draw(const struct snake *s);
void snake_update_dir(struct snake *s, int key);
void snake_update_head(struct snake *s);
void snake_erase_tail_end(const struct snake *s);
bool snake_check_init(const struct snake *s);
bool snake_check_collide_food(const struct snake *s, const struct food *f);
bool snake_check_collide_self(const struct snake *s);
bool snake_check_collide_board(const struct snake *s, int yfset);
bool snake_check_win(const struct snake *s);
void snake_cross_board(struct snake *s, int yfset);
void snake_free(struct snake *s);

#endif /* SNAKE_H */

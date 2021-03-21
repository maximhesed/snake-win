#ifndef FOOD_H
#define FOOD_H

#include <time.h>
#include "structs.h"
#include "winconf.h"
#include "board.h"
#include "utils.h"
#include "list.h"
#include "game_ctx.h"

struct food * food_alloc(void);
void food_draw(struct food *f);
void food_draw_super(struct food *f, struct food_ctx *fc);
void food_gen(struct food *f, struct food_ctx *fc, const struct snake *s);
void food_free(struct food *f);

#endif /* FOOD_H */

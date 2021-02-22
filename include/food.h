#ifndef FOOD_H
#define FOOD_H

#include <time.h>
#include "structs.h"
#include "winconf.h"
#include "board.h"
#include "utils.h"
#include "list.h"

struct food * food_alloc(void);
void food_draw(struct food *f);
void food_gen(struct food *f, const struct snake *s);
void food_free(struct food *f);

#endif /* FOOD_H */

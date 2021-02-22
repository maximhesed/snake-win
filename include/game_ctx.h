#ifndef GAME_CTX_H
#define GAME_CTX_H

#include "structs.h"
#include "snake.h"
#include "food.h"

struct game_ctx {
	struct snake *s;
	struct food *f;	
};

#endif /* GAME_CTX_H */

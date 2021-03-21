#ifndef REQS_H
#define REQS_H

#include <stdio.h>
#include <stdlib.h>
#include "game_ctx.h"
#include "log.h"
#include "utils.h"
#include "winconf.h"
#include "snake.h"
#include "event.h"

enum req_id {
	REQ_SNAKE = 1,
	REQ_SNAKE_COLLIDE_FOOD,
	REQ_FOOD,
	REQ_PAUSE,
	REQ_RESUME,
	REQ_EVENT_WIN,
	REQ_EVENT_LOSE
};

int req_send_start(struct game_ctx *ctx);
int req_send_snake(struct game_ctx *ctx);
int req_send_snake_collide_board(struct game_ctx *ctx);
int req_send_snake_collide_food(struct game_ctx *ctx);
int req_send_food(struct game_ctx *ctx);
int req_send_pause(struct game_ctx *ctx);
int req_send_resume(struct game_ctx *ctx);
int req_send_event_win(struct game_ctx *ctx);
int req_send_event_lose(struct game_ctx *ctx);
int req_recv(struct game_ctx *ctx);
int poll(struct game_ctx *ctx, const struct list *req);

#endif /* REQS_H */

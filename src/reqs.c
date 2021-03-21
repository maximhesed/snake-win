#include "reqs.h"

/* Structure of a snake's packet:
 *
 * /XX: /decoding | data[, data, ...]/\n
 *  ^    ^                            ^ 
 *  |    |                            query terminator
 *  |    either decoding or values, separated by commas
 *  4 symbols query's identifier field
 */

static int req_send(struct game_ctx *ctx, const char *fmt, ...);
static int req_snake_cb(struct game_ctx *ctx, const struct list *v);
static int req_snake_collide_food_cb(struct game_ctx *ctx, const struct list *v);
static int req_food_cb(struct game_ctx *ctx, const struct list *v);
static int req_pause_cb(struct game_ctx *ctx, const struct list *v);
static int req_resume_cb(struct game_ctx *ctx, const struct list *v);
static int req_event_win_cb(struct game_ctx *ctx, const struct list *v);
static int req_event_lose_cb(struct game_ctx *ctx, const struct list *v);

static int
req_send(struct game_ctx *ctx, const char *fmt, ...)
{
	int fstat;
	char *buf = NULL;
	va_list ap;
	int len;
	
	va_start(ap, fmt);
	
	len = vsnprintf(NULL, 0, fmt, ap);
	buf = calloc(sizeof(char), len + 1);
	
	vsnprintf(buf, len, fmt, ap);
	
	va_end(ap);
	
	fprintf(fileLog, "Send request: %s", buf);
	
	fstat = send(ctx->friendFd, buf, len, 0);
	if (fstat == SOCKET_ERROR) {
		fprintf(fileLog, "Failed to send a data (an error's code: %d)\n", WSAGetLastError());
		
		free(buf);
		
		ctx->isExit = true;
		
		return -1;
	}
	
	free(buf);
	
	return 0;
}

/* It's the formally request to start a game without delay 
 * after successful connection. */
int
req_send_start(struct game_ctx *ctx)
{
	int fstat;
	
	fstat = req_send(ctx, "00: START\n");
	if (fstat < 0)
		return -1;
		
	return 0;
}

int 
req_send_snake(struct game_ctx *ctx)
{
	int fstat;
	
	fstat = req_send(ctx, "01: {%c, %d, %d, %d, %d, %d, %d}\n", 
		ctx->s->u[0].sym,
		ctx->s->u[0].pos.X, 
		ctx->s->u[0].pos.Y + WIN_SIZE_Y + 2,
		ctx->s->u[0].prev.X,
		ctx->s->u[0].prev.Y + WIN_SIZE_Y + 2,
		ctx->s->u[0].color,
		ctx->s->u[0].dir);
	if (fstat < 0)
		return -1;
	
	return 0;
}

int
req_send_snake_collide_food(struct game_ctx *ctx)
{
	int fstat;
	
	fstat = req_send(ctx, "02: COLLIDE FOOD\n");
	if (fstat < 0)
		return -1;
	
	return 0;
}

int
req_send_food(struct game_ctx *ctx)
{
	int fstat;
	
	fstat = req_send(ctx, "03: {%c, %d, %d, %d, %d}\n", 
		ctx->f->sym, 
		ctx->f->pos.X, 
		ctx->f->pos.Y + WIN_SIZE_Y + 2, 
		ctx->f->color, 
		ctx->f->score);
	if (fstat < 0)
		return -1;
	
	return 0;
}

/* The pause request. */
int
req_send_pause(struct game_ctx *ctx)
{
	int fstat;
	
	fstat = req_send(ctx, "04: PAUSE\n");
	if (fstat < 0)
		return -1;
	
	return 0;
}

/* The resume request. */
int
req_send_resume(struct game_ctx *ctx)
{
	int fstat;
	
	fstat = req_send(ctx, "05: RESUME\n");
	if (fstat < 0)
		return -1;
	
	return 0;
}

int
req_send_event_win(struct game_ctx *ctx)
{
	int fstat;
	
	fstat = req_send(ctx, "06: EVENT WIN\n");
	if (fstat < 0)
		return -1;
	
	return 0;	
}

int
req_send_event_lose(struct game_ctx *ctx)
{
	int fstat;
	
	fstat = req_send(ctx, "07: EVENT LOSE\n");
	if (fstat < 0)
		return -1;
	
	return 0;
}

int
req_recv(struct game_ctx *ctx)
{
	int br; /* Bytes read. */
	struct list *reqs = NULL;
	char buf[1024];
	
	memset(buf, 0, sizeof(buf));

	br = recv(ctx->friendFd, buf, sizeof(buf), 0);
	if (br == 0) {
		fprintf(fileLog, "The connection was closed.\n");
		
		ctx->isExit = true;
		
		return -1;	
	}
	else if (br < 0) {
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			return 0;
			
		fprintf(fileLog, "Failed to read a data (an error's code: %d)\n", WSAGetLastError());
		
		ctx->isExit = true;
		
		return -1;
	}
	
	fprintf(fileLog, "Received buffer:\n%s", buf);
	
	reqs = strtkn(buf, '\n');
	for (int i = 0; i < list_count(reqs); i++) {
		const char *req_str;
		struct list *req;
		
		req_str = list_nth_data(reqs, i);
		req = strtkn(req_str, ':');
		req_trunc(req);
		
		if (poll(ctx, req) < 0) {
			fprintf(fileLog, "Poll error. Last request: %s\n", req_str);
			
			ctx->isExit = true;
			
			list_free(req, NULL);
			
			return -1;
		}
		
		fprintf(fileLog, "Parsed request: %s\n", req_str);

		list_free(req, NULL);
	}
	
	list_free(reqs, NULL);
	
	return 0;
}

int
poll(struct game_ctx *ctx, const struct list *req)
{
	enum req_id rid;
	int fstat;
	struct list *v = NULL;

	rid = atoi(list_nth_data(req, 0));

	/* Extract values from the data section. */
	v = strtkn(list_nth_data(req, 1), ';');
	req_trunc(v);
	
	switch (rid) {			
		case REQ_SNAKE:
			fstat = req_snake_cb(ctx, v);
			
			break;

		case REQ_SNAKE_COLLIDE_FOOD:
			fstat = req_snake_collide_food_cb(ctx, v);
			
			break;
				
		case REQ_FOOD:
			fstat = req_food_cb(ctx, v);
			
			break;
			
		case REQ_PAUSE:
			fstat = req_pause_cb(ctx, v);
			
			break;

		case REQ_RESUME:
			fstat = req_resume_cb(ctx, v);
			
			break;
			
		case REQ_EVENT_WIN:
			fstat = req_event_win_cb(ctx, v);
			
			break;
			
		case REQ_EVENT_LOSE:
			fstat = req_event_lose_cb(ctx, v);
			
			break;
			
		default:
			fstat = -1;
			
			break;
	}
	
	if (fstat < 0) {
		fprintf(fileLog, "Failed to execute the callback for the query with id %d\n", rid);
	
		list_free(v, NULL);
		
		return -1;
	}
	
	list_free(v, NULL);
	
	return 0;
}

static int
req_snake_cb(struct game_ctx *ctx, const struct list *v)
{	
	char units_str[64];
	struct list *units = NULL;
	
	memset(units_str, 0, sizeof(units_str));
	strcpy(units_str, (const char *) list_nth_data(v, 0));
	
	strdch(units_str, '{', TARGET_LEAD);
	strdch(units_str, '}', TARGET_TRAIL);
	
	units = strtkn(units_str, ',');
	req_trunc(units);
	
	/* Sometimes, due to delay, friend's snake can to move
	 * a little bit longer, than on the side of the friend. 
	 * Thereby, after commiting a head's data to current 
	 * snake the previous drawn head will remain on the field. */
	SetConsoleCursorPosition(h, ctx->friend.s->u[0].pos);
	printf("%c", ' ');
	
	ctx->friend.s->u[0].sym = *(const char *) list_nth_data(units, 0);
	ctx->friend.s->u[0].pos.X = atoi(list_nth_data(units, 1));
	ctx->friend.s->u[0].pos.Y = atoi(list_nth_data(units, 2));
	ctx->friend.s->u[0].prev.X = atoi(list_nth_data(units, 3));
	ctx->friend.s->u[0].prev.Y = atoi(list_nth_data(units, 4));
	ctx->friend.s->u[0].color = atoi(list_nth_data(units, 5));
	ctx->friend.s->u[0].dir = atoi(list_nth_data(units, 6));
	
	list_free(units, NULL);
		
	return 0;
}

static int
req_snake_collide_food_cb(struct game_ctx *ctx, const struct list *v)
{
	ctx->friend.s->length++;
	ctx->friend.s->score += ctx->friend.f->score;
	
	score_add_draw(ctx->friend.f, WIN_SIZE_Y + 2);
	
	return 0;
}

static int
req_food_cb(struct game_ctx *ctx, const struct list *v)
{
	char food_str[64];
	struct list *food = NULL;
	
	memset(food_str, 0, sizeof(food_str));
	strcpy(food_str, (const char *) list_nth_data(v, 0));
	
	strdch(food_str, '{', TARGET_LEAD);
	strdch(food_str, '}', TARGET_TRAIL);
	
	food = strtkn(food_str, ',');
	req_trunc(food);
	
	ctx->friend.f->sym = *(const char *) list_nth_data(food, 0);
	ctx->friend.f->pos.X = atoi(list_nth_data(food, 1));
	ctx->friend.f->pos.Y = atoi(list_nth_data(food, 2));
	ctx->friend.f->color = atoi(list_nth_data(food, 3));
	ctx->friend.f->score = atoi(list_nth_data(food, 4));
	
	food_draw(ctx->friend.f);
	
	list_free(food, NULL);
	
	return 0;
}

static int
req_pause_cb(struct game_ctx *ctx, const struct list *v)
{
	ctx->friend.isPause = true;
	
	if (evt_pause(ctx, WIN_SIZE_Y + 2) < 0)
		return -1;
	
	return 0;
}

static int
req_resume_cb(struct game_ctx *ctx, const struct list *v)
{
	ctx->isPause = false;
	ctx->friend.isPause = false;
	
	return 0;
}

static int
req_event_win_cb(struct game_ctx *ctx, const struct list *v)
{
	evt_win(WIN_SIZE_Y + 2);
	
	ctx->isMenu = true;
	
	return 0;
}

static int
req_event_lose_cb(struct game_ctx *ctx, const struct list *v)
{
	evt_lose(WIN_SIZE_Y + 2);
	
	ctx->isMenu = true;
	
	return 0;
}

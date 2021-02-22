#include "snake.h"

struct snake *
snake_alloc(void)
{
	struct snake *s = calloc(sizeof(struct snake), 1);

	s->u = calloc(sizeof(struct unit), SNAKE_LEN_MAX);

	/* The head. */
	s->u[0].sym = ' ';
	s->u[0].pos = (COORD) {SNAKE_POS_X_START, SNAKE_POS_Y_START};
	s->u[0].color = FG_MAGENTA;
	s->u[0].dir = DIR_RIGHT;

	/* The tail. */
	for (int i = 1; i < SNAKE_LEN_MAX; i++) {
		s->u[i].sym = SNAKE_TAIL_SYM;
		s->u[i].pos= (COORD) {SNAKE_POS_X_START - 1, SNAKE_POS_Y_START};
		s->u[i].color = FG_GRAY;
	}

	s->length = SNAKE_LEN_START + 1;
	s->score = 0;

	return s;
}

void
snake_move(struct snake *s)
{
	int i;

	for (i = 0; i < s->length; i++)
		s->u[i].prev = s->u[i].pos;

	unit_move(&s->u[0]);

	for (i = 0; i < s->length - 1; i++) {
		unit_follow_to_unit(&s->u[i + 1], &s->u[i]);
		
		s->u[i + 1].pos = s->u[i].prev;
		s->u[i + 1].dir = s->u[i].dir;
	}
}

void
snake_draw(const struct snake *s)
{
	for (int i = 0; i < s->length; i++)
		unit_draw(&s->u[i]);
}

void
snake_update_dir(struct snake *s, int key)
{
	for (int i = 0; i < s->length; i++)
		unit_refresh_dir(&s->u[i], key);
}

void
snake_update_head(struct snake *s)
{
	switch (s->u[0].dir) {
		case DIR_LEFT:
			s->u[0].sym = '<';
			
			break;
			
		case DIR_RIGHT:
			s->u[0].sym = '>';
			
			break;
			
		case DIR_UP:
			s->u[0].sym = '^';
			
			break;
			
		case DIR_DOWN:
			s->u[0].sym = 'v';
			
			break;
	}
}

void
snake_erase_tail_end(const struct snake *s)
{
	SetConsoleCursorPosition(h, s->u[s->length - 1].pos);
	SetConsoleTextAttribute(h, FG_LIGHTGRAY | 0);
	printf("%c", ' ');
}

void
snake_check_collide_food(struct snake *s, struct food *f)
{
	for (int i = 0; i < s->length; i++) {
		if ((s->u[i].pos.X == f->pos.X) && (s->u[i].pos.Y == f->pos.Y)) {
			s->length++;
			s->score += f->score;

			score_add_draw(f);

			food_gen(f, s);
		}
	}
}

bool
snake_check_collide_self(const struct snake *s)
{
	for (int i = 1; i < s->length; i++) {
		if ((s->u[0].pos.X == s->u[i].pos.X) && (s->u[0].pos.Y == s->u[i].pos.Y)) {
			SetConsoleCursorPosition(h, s->u[0].pos);
			SetConsoleTextAttribute(h, s->u[0].color | 0);
			printf("%c", s->u[0].sym);
			SetConsoleTextAttribute(h, FG_LIGHTGRAY | 0);

			return true;
		}
	}

	return false;
}

bool
snake_check_collide_board(const struct snake *s)
{
	for (int i = 0; i < s->length; i++) {
		if (unit_check_board(&s->u[i]))
			return true;
	}

	return false;
}

bool
snake_check_init(const struct snake *s)
{
	for (int i = 0; i < s->length - 1; i++) {
		if ((s->u[i].pos.X == s->u[i + 1].pos.X) && (s->u[i].pos.Y == s->u[i + 1].pos.Y))
			return false;
	}

	return true;
}

void
snake_free(struct snake *s)
{
	if (s != NULL)
		unit_free(s->u);
	
	free(s);
}

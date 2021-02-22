#include "food.h"

static const char food_sym[] = {
	'*',
	'$'
};

static const int food_score[] = {
	5,
	10,
	25,
	100,
	250,
	500,
	1000
};

static const int food_color[] = {
	FG_LIGHTGREEN,
	FG_LIGHTCYAN,
	FG_LIGHTRED,
	FG_LIGHTMAGENTA,
	FG_YELLOW
};

struct food *
food_alloc(void)
{
	return calloc(sizeof(struct food), 1);
}

void
food_draw(struct food *f)
{
	if (f->score == food_score[6])
		f->color = food_color[random(0, 4)];

	SetConsoleCursorPosition(h, f->pos);
	SetConsoleTextAttribute(h, f->color | 0);
	printf("%c", f->sym);
	SetConsoleTextAttribute(h, FG_LIGHTGRAY | 0);
}

/* TODO: Create generation table. */
void
food_gen(struct food *f, const struct snake *s)
{
	struct list *vcrds = list_alloc();
	
	for (int y = BOARD_TOP + 1; y < BOARD_BOTTOM; y++) {
		for (int x = BOARD_LEFT + 1; x < BOARD_RIGHT; x++) {
			bool isEmpty = true;
			
			for (int i = 0; i < s->length; i++) {
				if (x == s->u[i].pos.X && y == s->u[i].pos.Y)
					isEmpty = false;
			}
			
			if (isEmpty) {
				COORD *crd = malloc(sizeof(COORD));
				
				*crd = (COORD) {x, y};
				
				list_append(vcrds, crd);
			}
		}
	}

	f->pos = *(COORD *) list_nth_data(vcrds, random(0, BOARD_SIZE_X_DELTA * BOARD_SIZE_Y_DELTA - 1));
	f->sym = food_sym[random(0, 1)];
	f->color = food_color[random(0, 4)];
	f->score = food_score[random(0, 6)];
	
	list_free(vcrds, NULL);
}

void
food_free(struct food *f)
{
	free(f);	
}

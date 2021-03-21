#include "unit.h"

void
unit_move(struct unit *u)
{
	switch (u->dir) {
		case DIR_LEFT:
			u->pos.X--;
			
			break;
			
		case DIR_RIGHT:
			u->pos.X++;
			
			break;
			
		case DIR_UP:
			u->pos.Y--;
			
			break;
			
		case DIR_DOWN:
			u->pos.Y++;
		
			break;	
	}
}

void
unit_draw(const struct unit *u)
{
	SetConsoleCursorPosition(h, u->pos);
	SetConsoleTextAttribute(h, u->color | 0);
	printf("%c", u->sym);
	SetConsoleTextAttribute(h, FG_LIGHTGRAY | 0);
}

void
unit_update_dir(struct unit *u, int key)
{
	switch (key) {
		case KEY_LEFT:
			if (u->prev.Y != u->pos.Y)
				u->dir = DIR_LEFT;
			
			break;
			
		case KEY_RIGHT:
			if (u->prev.Y != u->pos.Y)
				u->dir = DIR_RIGHT;
			
			break;
			
		case KEY_UP:
			if (u->prev.X != u->pos.X)
				u->dir = DIR_UP;
			
			break;
			
		case KEY_DOWN:
			if (u->prev.X != u->pos.X)
				u->dir = DIR_DOWN;
		
			break;	
	}
}

void
unit_follow_to_unit(struct unit *u2, const struct unit *u1)
{
	switch (u1->dir) {
		case DIR_LEFT:
			u2->pos = (COORD) {u1->pos.X + 1, u1->pos.Y};
			
			break;
			
		case DIR_RIGHT:
			u2->pos = (COORD) {u1->pos.X - 1, u1->pos.Y};
			
			break;
			
		case DIR_UP:
			u2->pos = (COORD) {u1->pos.X, u1->pos.Y + 1};
			
			break;
			
		case DIR_DOWN:
			u2->pos = (COORD) {u1->pos.X, u1->pos.Y - 1};
			
			break;
	}	
}

void
unit_bend(struct unit *u2, const struct unit *u1)
{
	if ((u2->pos.X == u1->prev.X) && (u2->pos.Y == u1->prev.Y)) {
		u2->dir = u1->dir;
		u2->prev = u1->prev;
	}
}

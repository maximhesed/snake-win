#ifndef UNIT_H
#define UNIT_H

#include <stdio.h>
#include <stdbool.h>
#include "structs.h"
#include "board.h"
#include "keys.h"

void unit_move(struct unit *u);
void unit_draw(const struct unit *u);
void unit_update_dir(struct unit *u, int key);
void unit_follow_to_unit(struct unit *u2, const struct unit *u1);
void unit_bend(struct unit *u2, const struct unit *u1);

#endif /* UNIT_H */

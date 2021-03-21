#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include "utils.h"

typedef void (*cleaner_t)(void *data);

struct list {
	int id;
	void *data;
	struct list *next;
};

struct list * list_alloc(void);
void list_append(struct list *list, void *data);
void list_appenda(struct list *list, int size, const void *data);
bool list_empty(const struct list *list);
int list_count(const struct list *list);
void * list_nth_data(const struct list *list, int n);
void * list_nth_data_rand(const struct list *list);
void list_print(const struct list *list);
void list_free(struct list *list, cleaner_t);

#endif /* LIST_H */

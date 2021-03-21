#include "list.h"

static struct list * get_last_node(struct list *list);
static void _append(struct list *list, void *data);

static struct list *
get_last_node(struct list *list)
{
	if (list == NULL)
		return NULL;
	
	while (list->next != NULL)
		list = list->next;
	
	return list;	
}

static void
_append(struct list *list, void *data)
{
	struct list *last;
	
	last = get_last_node(list);
	if (last == NULL)
		return;
	
	if (!list_empty(last)) {
		last->next = list_alloc();
		
		last->next->id = last->id + 1;
		
		last = last->next;	
	}
	
	last->data = data;
}

struct list *
list_alloc(void)
{
	struct list *list = malloc(sizeof(struct list));
	
	list->id = 0;
	list->data = NULL;
	list->next = NULL;
	
	return list;
}

void
list_append(struct list *list, void *data)
{
	_append(list, data);	
}

void
list_appenda(struct list *list, int size, const void *data)
{
	void *data_copy = malloc(size);
	
	memcpy(data_copy, data, size);

	_append(list, data_copy);
}

bool
list_empty(const struct list *list)
{
	return list->data == NULL;
}

int
list_count(const struct list *list)
{
	if (list == NULL || list_empty(list))
		return 0;
	
	while (list->next != NULL)
		list = list->next;
		
	return list->id + 1;
}

void *
list_nth_data(const struct list *list, int n)
{
	while (list != NULL) {
		if (list->id == n)
			return list->data;
			
		list = list->next;
	}
	
	return NULL;
}

void * 
list_nth_data_rand(const struct list *list)
{
	return list_nth_data(list, random(0, list_count(list) - 1));	
}

void
list_print(const struct list *list)
{
	while (list != NULL) {
		printf("%p {%d, %p, %p}\n", (void *) list, list->id, list->data, (void *) list->next);
		
		list = list->next;
	}
}

void
list_free(struct list *list, cleaner_t cleaner)
{
	struct list *tmp;
	
	while (list != NULL) {
		tmp = list, list = list->next;
		
		if (cleaner != NULL)
			cleaner(tmp->data);
		else
			free(tmp->data);
	
		free(tmp);
	}
}

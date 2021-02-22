#include "list.h"

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
	if (list_empty(list)) {
		list->data = data;
		
		return;
	}
	
	while (list->next != NULL)
		list = list->next;
	
	list->next = list_alloc();
	
	list->next->id = list->id + 1;
	list->next->data = data;
}

bool
list_empty(const struct list *list)
{
	return list == NULL || list->data == NULL;
}

int 
list_count(const struct list *list)
{
	if (list == NULL)
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

void
list_print(const struct list *list)
{
	while (list != NULL) {
		printf("%d: %p\n", list->id, list->data);
		
		list = list->next;
	}
}

void
list_free(struct list *list, cleaner_t cleaner)
{
	struct list *tmp = NULL;
	
	while (list != NULL) {
		tmp = list, list = tmp->next;
		
		if (cleaner != NULL)
			cleaner(tmp->data);
	
		free(tmp);
	}
}

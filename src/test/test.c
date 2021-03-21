#include "test.h"

void
test_recv_food(void)
{
	char req[64] = "{$, 10, 10, 4, 1000}";
	struct list *lst_req = NULL, *walker;
	struct food *food = food_alloc();
	
	printf("req before: %s\n", req);
	
	strdch(req, '{', TARGET_LEAD);
	strdch(req, '}', TARGET_TRAIL);
	
	printf("req after: %s\n", req);
	
	lst_req = strtkn(req, ',');
	req_trunc(lst_req);
	
	walker = lst_req;
	while (walker != NULL) {
		printf("lst_req data: %s\n", (const char *) walker->data);
		
		walker = walker->next;
	}
	
	food->sym = *(const char *) list_nth_data(lst_req, 0);
	food->pos.X = atoi(list_nth_data(lst_req, 1));
	food->pos.Y = atoi(list_nth_data(lst_req, 2));
	food->color = atoi(list_nth_data(lst_req, 3));
	food->score = atoi(list_nth_data(lst_req, 4));
	
	food_draw(food);
	
	list_free(lst_req, NULL);
	food_free(food);
}

void
test_list(void)
{
	struct list *list = list_alloc();
	char *buf[4];
	
	buf[0] = strdup("asd");
	buf[1] = strdup("hello");
	buf[2] = strdup("vasd");
	buf[3] = strdup("kit");
	
	list_append(list, buf[0]);
	list_append(list, buf[1]);
	list_append(list, buf[2]);
	list_append(list, buf[3]);
	
	list_print(list);
	getch();
	
	list_free(list, NULL);	
}

void
test_reqs(void)
{
	const char *buf = "01: {>, 10, 36, 0, 30, 5, 1}\n03: {$, 1, 49, 11, 250}\n";
	struct list *reqs = NULL;
	
	reqs = strtkn(buf, '\n');
	req_trunc(reqs);
	
	for (int i = 0; i < list_count(reqs); i++) {
		struct list *req = NULL;
		
		req = strtkn(list_nth_data(reqs, i), ':');
		req_trunc(req);
		
		list_free(req, NULL);
	}
	
	list_free(reqs, NULL);
}

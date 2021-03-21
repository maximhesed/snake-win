#include "menu.h"

static void cursor_erase(const struct menu *m);
static void cursor_draw(const struct menu *m);
static void calc_indents(struct menu *m);

static void
cursor_erase(const struct menu *m)
{
	struct item *item = list_nth_data(m->items, m->offset);

	SetConsoleCursorPosition(h, (COORD) {getmposx(m) - 2 + item->indent, getmposy(m) + m->offset});
	printf("  ");
	
	SetConsoleCursorPosition(h, (COORD) {getmposx(m) + strlen(item->name) + item->indent, getmposy(m) + m->offset});
	printf("  ");
}

static void
cursor_draw(const struct menu *m)
{
	struct item *item = list_nth_data(m->items, m->offset);
	
	SetConsoleCursorPosition(h, (COORD) {getmposx(m) - 2 + item->indent, getmposy(m) + m->offset});
	printf("> ");
	
	SetConsoleCursorPosition(h, (COORD) {getmposx(m) + strlen(item->name) + item->indent, getmposy(m) + m->offset});
	printf(" <");	
}

static void
calc_indents(struct menu *m)
{
	struct item *item;
	int i;
	
	for (i = 0; i < list_count(m->items); i++) {
		item = list_nth_data(m->items, i);
		
		if (strlen(item->name) > m->item_len_max)
			m->item_len_max = strlen(item->name);
	}
	
	for (i = 0; i < list_count(m->items); i++) {
		item = list_nth_data(m->items, i); 
		
		item->indent = (m->item_len_max - strlen(item->name)) / 2;
	}	
}

struct menu *
menu_alloc(void)
{
	struct menu *m = malloc(sizeof(struct menu));
	
	m->offset = 0;
	m->items = list_alloc();
	m->status = MENU_STATUS_CLOSED;
	m->key = 0;
	m->item_len_max = 0;
	
	return m;
}

void
menu_add_item(struct menu *m, const char *name, menu_cb cb, void *data)
{
	struct item item;
	
	strcpy(item.name, name);
	item.indent = 0;
	item.cb = cb;
	item.data = data;
	
	list_appenda(m->items, sizeof(struct item), &item);
}

void
menu_draw(const struct menu *m)
{	
	for (int i = 0; i < list_count(m->items); i++) {
		struct item *item = list_nth_data(m->items, i);
		
		if (i == m->offset)
			cursor_draw(m);
		
		SetConsoleCursorPosition(h, (COORD) {getmposx(m) + item->indent, getmposy(m) + i});
		printf(item->name);
	}
}

void
menu_refresh(struct menu *m)
{	
	switch (m->key) {
		case KEY_DOWN:
			cursor_erase(m);
			
			m->offset++;
			
			if (m->offset > list_count(m->items) - 1)
				m->offset = list_count(m->items) - 1;
			
			cursor_draw(m);
			
			break;
		
		case KEY_UP:
			cursor_erase(m);
			
			m->offset--;
			
			if (m->offset < 0)
				m->offset = 0;
				
			cursor_draw(m);
			
			break;
	}
}

void
menu_process(struct menu *m)
{
	struct item *item;
	
	switch (m->key) {
		case KEY_ENTER:
			item = list_nth_data(m->items, m->offset);
			
			item->cb(item->data);
			
			m->status = MENU_STATUS_CLOSED;

			break;
	}
}

void
menu_init(struct menu *m)
{
	calc_indents(m);
	
	menu_draw(m);
	
	m->status = MENU_STATUS_OPENED;
	
	while (m->status == MENU_STATUS_OPENED) {
		m->key = 0;
		
		while (kbhit())
			m->key = getch();
			
		menu_refresh(m);
		menu_process(m);
		
		Sleep(25);	
	}
}

void
menu_free(struct menu *m)
{
	list_free(m->items, NULL);
	free(m);
}

#include "utils.h"

static void _strdch(char *str, int i);

static void
_strdch(char *str, int i)
{
	if (i == (int) strlen(str))
		exit(EXIT_FAILURE);
	
	memmove(str + i, str + i + 1, strlen(str + i + 1) + 1);
}

int
random(int min, int max)
{
	return min + rand() % (max + 1 - min);
}

int
digits_count(int v)
{
	int q = 0;

	for (; v != 0; v /= 10, q++);

	return q;
}

struct list *
strtkn(const char *str, const char delim)
{
	const char *seg = str;
	struct list *list = list_alloc();
	int i;
	bool eos = false;
	
	while (!eos) {
		char *token = NULL;
		int len = 0;
		
		while (*str != delim) {
			if (*str == '\0') {
				eos = true;
				
				break;
			}
			
			len++;
			str++;
		}
		
		if (len == 0) {
			if (eos)
				break;
			
			str++;
			seg = str;
			
			list_append(list, NULL);
			
			continue;
		}
		
		token = calloc(sizeof(char), len + 1);
		
		str = seg;
		
		for (i = 0; i < len; i++) {
			if (*str != delim) {
				token[i] = *str;
				
				str++;
			}
		}
		
		str++;
		seg = str;
		
		list_append(list, token);
	}	
	
	return list;
}

bool
strdch(char *str, const char exile, int mask)
{
	bool changed = false;
	
	if (exile == '\0')
		return false;
	
	if (mask & TARGET_FIRST) {
		for (int i = 0; i < (int) strlen(str); i++) {
			if (str[i] == exile) {
				_strdch(str, i);
				
				changed = true;
				
				break;
			}
		}
	}
	
	if (mask & TARGET_LEAD) {
		for (int i = 0; i < (int) strlen(str); i++) {
			if (str[i] == exile) {
				_strdch(str, i);
				
				changed = true;
			
				i--;
			}
			else
				break;
		}
	}

	if (mask & TARGET_TRAIL) {
		for (int i = (int) strlen(str) - 1; i >= 0; i--) {
			if (str[i] == exile) {
				_strdch(str, i);
			
				changed = true;	
			}
			else 
				break;
		}
	}
		
	if (mask & TARGET_ALL) {
		for (int i = 0; i < (int) strlen(str); i++) {
			if (str[i] == exile) {
				_strdch(str, i);
				
				changed = true;
			
				i--;
			}
		}
	}
	
	return changed;
}

void
req_trunc(struct list *list)
{
	while (list != NULL) {
		strdch(list->data, ' ', TARGET_LEAD);
		strdch(list->data, '\n', TARGET_TRAIL);
		
		list = list->next;
	}	
}

void
wprintfr(COORD crd, const char *fmt, ...)
{
	char *str = NULL;
	int len, i, *buf = NULL, *pos = NULL;
	va_list ap;
	
	va_start(ap, fmt);
	
	len = vsnprintf(NULL, 0, fmt, ap);
	str = calloc(sizeof(char), len + 1);
	
	vsnprintf(str, len, fmt, ap);
	
	va_end(ap);
	
	buf = calloc(sizeof(int), len + 1);
	pos = calloc(sizeof(int), len + 1);
	
	for (i = 0; i < len; i++)
		buf[i] = i;
	
	for (i = 0; i < len; i++) {
		int len_short = len - 1 - i;
		int n = buf[random(0, len_short)];
		char skip = false;
		
		pos[i] = n;
		
		for (int j = 0; j < len_short; j++) {
			if (buf[j] == n)
				skip = true;
				
			buf[j] = buf[skip ? j + 1 : j];
		}
	}
	
	for (i = 0; i < len; i++) {
		SetConsoleCursorPosition(h, (COORD) {crd.X + pos[i], crd.Y});
		
		printf("%c", str[pos[i]]);
		
		Sleep(10);
	}
	
	SetConsoleCursorPosition(h, (COORD) {crd.X + len, crd.Y});
	
	free(str);
	free(buf);
	free(pos);
}

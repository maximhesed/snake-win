#include "utils.h"

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

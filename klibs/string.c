#include <mikoOS/string.h>

/**
 * It works as memset(3).
 */
void memset(void *addr, int c, size_t size)
{
	size_t i;
	char *p = addr;

	for (i = 0; i < size; i++)
		p[i] = c;
}


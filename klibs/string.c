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

/**
 * It works as strcmp(3).
 */
int strcmp(const char *s1, const char *s2)
{
	for ( ; *s1 == *s2; s1++, s2++) 
		if (*s1 == '\0')
			return 0;
	
	return *s1 - *s2;
}

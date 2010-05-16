#include <mikoOS/string.h>

/**
 * It works as memset(3).
 */
void memset(void *addr, int c, size_t size)
{
	size_t i;
	register char *p = addr;

	for (i = 0; i < size; i++)
		p[i] = c;
}

/**
 * It works as memcpy(3).
 */
void *memcpy(void *dest, const void *src, size_t n)
{
        size_t i;
        register unsigned char *d;
	register const unsigned char *s;

        d = dest;
	s = src;

        for (i = 0; i < n; i++)
                d[i] = s[i];

        return dest;
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

/**
 * It works as strncmp(3).
 */
int strncmp(const char *s1, const char *s2, size_t n)
{
	while (n) {
		if (*s1 != *s2)
			return *s1 - *s2;

		if (*s1 == '\0')
                        break;
		n--;
		s1++;
		s2++;
	}
        return 0;
}

/**
 * It words as strlen(3).
 */
size_t strlen(const char *s)
{
	const char *p = s;

	while (*s++) 
		;

	return s - p;
}

/**
 * It words as strcpy(3).
 */
char *strcpy(char *dest, const char *src)
{
	while (*dest++ = *src++)
		;
	return dest;
}

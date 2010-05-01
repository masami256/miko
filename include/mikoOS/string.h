#ifndef __MIKOOS_STRING_H
#define __MIKOOS_STRING_H 1

#include <mikoOS/kernel.h>
#include <sys/types.h>

void memset(void *addr, int c, size_t size);
void *memcpy(void *dest, const void *src, size_t n);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

#endif // __MIKOOS_STRING_H

#ifndef __IO_H
#define __IO_H 1
#include <sys/types.h>

static inline void outb(u_int16_t port, u_int8_t val)
{
	__asm__ __volatile__ ("outb %0, %1" : : "a" (val), "dN" (port));
}

static inline u_int8_t inb(u_int16_t port)
{
	u_int8_t val;
	__asm__ __volatile__("inb %1, %0" : "=a" (val) : "dN" (port));
	return val;
}

#endif // __IO_H 

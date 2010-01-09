#ifndef __MIKOOS_IO_H
#define __MIKOOS_IO_H 1
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

static inline void outw(u_int16_t port, u_int16_t val)
{
	__asm__ __volatile__ ("outw %0, %1" : : "a" (val), "dN" (port));
}

static inline u_int16_t inw(u_int16_t port)
{
	u_int16_t val;
	__asm__ __volatile__("inw %1, %0" : "=a" (val) : "dN" (port));
	return val;
}

static inline void outl(u_int16_t port, u_int32_t val)
{
	__asm__ __volatile__ ("outl %0, %1" : : "a" (val), "dN" (port));
}

static inline u_int32_t inl(u_int16_t port)
{
	u_int32_t val;
	__asm__ __volatile__("inl %1, %0" : "=a" (val) : "dN" (port));
	return val;
}


#endif // __MIKOOS_IO_H

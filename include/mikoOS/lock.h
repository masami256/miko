#ifndef __LOCK_H
#define __LOCK_H 1

static inline void cli(void)
{
	__asm__ __volatile__("cli");
}

static inline void sti(void)
{
	__asm__ __volatile__("sti");
}

#endif // __LOCK_H


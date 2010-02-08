#ifndef __MIKOOS_LOCK_H
#define __MIKOOS_LOCK_H 1

static inline void cli(void)
{
	__asm__ __volatile__("cli");
}

static inline void sti(void)
{
	__asm__ __volatile__("sti");
}

#endif // __MIKOOS_LOCK_H


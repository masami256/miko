#ifndef __MIKOOS_SYSCALL_H
#define __MIKOOS_SYSCALL_H 1

#include <mikoOS/register.h>

#define NR_SYSCALLS (6 + 1)

struct syscall_table {
	long (*f)(struct registers *regs);
};

extern struct syscall_table syscalls[NR_SYSCALLS];

#endif // __MIKOOS_SYSCALL_H

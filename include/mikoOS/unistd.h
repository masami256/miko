#ifndef __MIKOOS_UNISTD_H
#define __MIKOOS_UNISTD_H 1

#include <asm/register.h>

// define syscall.
long sys_unused(struct registers *regs); // 0
long sys_exit(struct registers *regs);
long sys_fork(struct registers *regs);
long sys_open(struct registers *regs);
long sys_close(struct registers *regs);

long sys_read(struct registers *regs); // 5
long sys_write(struct registers *regs);

#endif // __MIKOOS_UNISTD_H

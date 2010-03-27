#include <mikoOS/kernel.h>
#include <mikoOS/syscall.h>
#include <mikoOS/unistd.h>

struct syscall_table syscalls[NR_SYSCALLS] = {
	sys_unused,
	sys_exit,
	sys_fork,
	sys_open,
	sys_close,
	sys_read,
	sys_write
};

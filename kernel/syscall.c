#include <mikoOS/kernel.h>
#include <mikoOS/unistd.h>
#include <mikoOS/printk.h>
#include <asm/register.h>

long sys_unused(struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

long sys_exit(struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

long sys_fork(struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

long sys_open(struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

long sys_close(struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

long sys_read(struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

long sys_write(struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}



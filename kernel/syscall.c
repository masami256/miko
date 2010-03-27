#include <mikoOS/kernel.h>
#include <mikoOS/unistd.h>
#include <mikoOS/printk.h>
#include <mikoOS/register.h>

long sys_unused(UNUSED struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

long sys_exit(UNUSED struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

long sys_fork(UNUSED struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

long sys_open(UNUSED struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

long sys_close(UNUSED struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

long sys_read(UNUSED struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

long sys_write(UNUSED struct registers *regs)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}



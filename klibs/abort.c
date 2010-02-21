#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/lock.h>
#include <mikoOS/interrupt.h>

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static void general_protection_fault_handler(struct registers regs);

static void general_protection_fault_handler(struct registers regs)
{
	printk("General Protection Fault occured\n");
	cli();
	while (1);
}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////
void kern_abort(const char *file, int line, const char *func, char *msg)
{
	printk("%s %s:%s at %d\n", msg, file, func, line);
	while (1);
}

void setup_fault_handler(void)
{
	set_handler_func(0xd, &general_protection_fault_handler);
}



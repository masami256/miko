#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>

void kern_abort(const char *file, int line, const char *func, char *msg)
{
	printk("%s %s:%s at %d\n", msg, file, func, line);
	while (1);
}

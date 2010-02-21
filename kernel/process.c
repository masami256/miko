#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include "process.h"
#include "gdt.h"

int setup_tss(void)
{
	printk("Setup TSS\n");

	return 0;
}

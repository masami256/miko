#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/abort.h>
#include <mikoOS/string.h>
#include <mikoOS/kmalloc.h>
#include <mikoOS/lock.h>
#include "process.h"
#include "gdt.h"

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////

static void test_task1(void)
{
	int i;
	for (i = 0; i < 10; i++)
		printk("AAAAA");
	printk("\n");
}

static void test_task2(void)
{
	int i;
	for (i = 0; i < 10; i++)
		printk("BBBBB");
	printk("\n");
}

static void test_task3(void)
{
	int i;
	for (i = 0; i < 10; i++)
		printk("CCCCC");
	printk("\n");
}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////
int setup_tss(void)
{
	return 0;
}


#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/abort.h>
#include <mikoOS/string.h>
#include <mikoOS/kmalloc.h>
#include "process.h"
#include "gdt.h"

#define FLAG_INT_ENABLE  0x200;

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////

static void test_task1(void)
{
	while (1)
		printk("AAAAA");
}

static void test_task2(void)
{
	while (1)
		printk("BBBBB");
}

static void test_task3(void)
{
	while (1)
		printk("CCCCC");
}

struct tss_struct *set_tss(u_int16_t cs, u_int16_t ds,
			   u_int32_t eip, u_int32_t eflags,
			   u_int32_t esp, u_int16_t ss,
			   u_int32_t esp0, u_int16_t ss0)
{
	struct tss_struct *p = NULL;

	p = kmalloc(sizeof(*p));
	if (!p)
		KERN_ABORT("kmalloc failed");

	memset(p, 0x0, sizeof(*p));

	p->cs = cs;
	p->eip = eip;
	p->eflags = eflags;
	p->esp = esp;
	p->ds = p->es = 0;
	p->ss = ss;
	p->esp = esp0;
	p->ss0 = ss0;

	return p;
}

static inline void ltr(u_int16_t sel)
{
	__asm__ __volatile__ ("ltr %0;\n\t" ::"m"(sel));
}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////
int setup_tss(void)
{
	u_int16_t cs, ds, ss;
	u_int32_t esp;
	struct tss_struct *tss[3];
	
	printk("Setup TSS\n");
	
	__asm__ __volatile__("movw %%cs, %0\n\t;"
			     "movw %%ds, %1\n\t;"
			     "movw %%ss, %2\n\t;"
			     "movl %%esp, %3\n\t;"
			     :"=g"(cs), "=g"(ds), "=g"(ss), "=g"(esp)
		);
	
	tss[0] = set_tss(cs, ds, (u_int32_t) &test_task1, 0x200,
			 esp + PROCESS_STACK_SIZE, ss, 
			 esp, ss);

	tss[1] = set_tss(cs, ds, (u_int32_t) &test_task2, 0x200,
			 esp + (PROCESS_STACK_SIZE * 2), ss, 
			 esp, ss);

	tss[2] = set_tss(cs, ds, (u_int32_t) &test_task3, 0x200,
			 esp + (PROCESS_STACK_SIZE * 3), ss, 
			 esp, ss);
	
	ltr((u_int16_t) tss[0]);

	return 0;
}


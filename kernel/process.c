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
	int i;
	for (i = 0; i < 10; i++)
		printk("AAAAA");
}

static void test_task2(void)
{
	int i;
	for (i = 0; i < 10; i++)
		printk("BBBBB");
}

static void test_task3(void)
{
	int i;
	for (i = 0; i < 10; i++)
		printk("CCCCC");
}

u_int16_t tasks[3];

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

static inline void switch_task(u_int16_t sel)
{
	__asm__ __volatile__("ljmp *%0;\n\t"
			     ::"m"(sel)
		);

}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////
int setup_tss(void)
{
	u_int16_t cs, ds, ss;
	u_int32_t esp;
	struct tss_struct *tss[3];
	int i;
	
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


	for (i = 0; i < 3; i++) {
		tasks[i] = search_unused_gdt_index();
		if (tasks[i] == 0)
			KERN_ABORT("Do not have enough room for add TSS descriptor\n");

		set_gdt_values(tasks[i], (u_int32_t) tss[i], sizeof(struct tss_struct), SEG_TYPE_TSS, 0xca);
//		printk("Find index:task[%d] is 0x%x\n", i, tasks[i]);
	}

	ltr((u_int16_t) (tasks[0] * 8));
	gdt_types();

	test_task1();

	return 0;
}

void scheduler(void)
{

	static int count = 1;
	u_int16_t next = 0;
	int i;

	if (count == 4)
		count = 0;
	next = tasks[count] * 8;

	printk("next is 0x%x\n", next);
	switch_task(next);

	count++;

}

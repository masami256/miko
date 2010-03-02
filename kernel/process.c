#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/abort.h>
#include <mikoOS/string.h>
#include <mikoOS/kmalloc.h>
#include <mikoOS/lock.h>
#include <mikoOS/timer.h>
#include "process.h"
#include "gdt.h"
#include "interrupt_handler.h"

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static void switch_task(u_int16_t sel) __attribute__ ((noinline));

struct tss_struct tss[2];

static void test_task1(void)
{
	u_int32_t i;
	u_int32_t eflags;
	
	for (i = 0; ; i++) {
		wait_loop_usec(500);
		if (!(i % 100)) {
		    printk("AAAAA");
		    switch_task(0x30);
		}
	}
}

static void test_task2(void)
{
	u_int32_t i;
	u_int32_t eflags;

	for (i = 0; ; i++) {
		wait_loop_usec(500);
		if (!(i % 100)) {
		    printk("BBBBB");
		    switch_task(0x28);
		}
	}
}
 
struct tss_struct *set_tss(u_int16_t cs, u_int16_t ds,
			   u_int32_t eip, u_int32_t eflags,
			   u_int32_t esp, u_int16_t ss,
			   u_int32_t esp0, u_int16_t ss0)
{
	struct tss_struct *p = NULL;
	static int cnt = 0;

	p = &tss[cnt];
//	p = kmalloc(sizeof(*p));
	if (!p)
		KERN_ABORT("kmalloc failed");
 
	memset(p, 0x0, sizeof(*p));

	printk("cs:0x%x ds:0x%x ss:0x%x esp:0x%x\n", cs, ds, ss, esp);

	p->cs = cs;
	p->eip = eip;
	p->eflags = eflags;
	p->esp = esp;

	p->ds = ds;
	p->es = 0x10;
	p->fs = 0x10;
	p->gs = 0x10;
	p->ss = ss;
	p->esp = esp0;
	p->ss0 = ss0;

	p->io_bitmap = 0x80000000;

	cnt++;
	return p;
}


static void switch_task(u_int16_t sel)
{
	struct far_pointer {
		u_int32_t a;
		u_int16_t b;
	} tmp __attribute__((packed));

	tmp.a = 0;
	tmp.b = sel;

	__asm__ __volatile__ ("ljmp %0\n\t" ::"m"(tmp)); 
}


/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////


int setup_tss(void)
{
	u_int16_t cs, ds, ss;
	u_int32_t esp;

	int i;
 
	printk("Setup TSS\n");
 
	__asm__ __volatile__("movw %%cs, %0\n\t;"
			     "movw %%ds, %1\n\t;"
			     "movw %%ss, %2\n\t;"
			     "movl %%esp, %3\n\t;"
			     :"=g"(cs), "=g"(ds), "=g"(ss), "=g"(esp)
		);
 
	printk("cs:0x%x ds:0x%x ss:0x%x esp:0x%x\n", cs, ds, ss, esp);

	set_tss(cs, ds, (u_int32_t) &test_task1, 0x202,
			 esp - PROCESS_STACK_SIZE, ss,
			 esp, ss);
 
	set_tss(cs, ds, (u_int32_t) &test_task2, 0x202,
			 esp - (PROCESS_STACK_SIZE * 2), ss,
			 esp, ss);
 

	set_gdt_values(SEL_TSS, (u_int32_t) &tss[0], sizeof(struct tss_struct), SEG_TYPE_TSS); 
	set_gdt_values(0x30, (u_int32_t) &tss[1], sizeof(struct tss_struct), SEG_TYPE_TSS); 

	ltr();

	return 0;
}

void schedule(void)
{
	test_task1();
}



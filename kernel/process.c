#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/abort.h>
#include <mikoOS/string.h>
#include <mikoOS/kmalloc.h>
#include <mikoOS/lock.h>
#include <mikoOS/timer.h>
#include <mikoOS/mm.h>
#include "process.h"
#include "gdt.h"
#include "interrupt_handler.h"

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static void switch_task(u_int16_t sel);

struct tss_struct tss[2];
static char process_stack[2][8192];

static void test_task1(void)
{
	while (1) {
		wait_loop_usec(500);
		printk("A");
		switch_task(0x30);
	}
	return ;
}

static void test_task2(void)
{
	while (1) {
		wait_loop_usec(500);
		printk("B");
		switch_task(0x28);
	}
	return ;
}

static inline void ltr(u_int16_t sel)
{
	__asm__ __volatile__ ("ltr %0\n\t" ::"m"(sel));
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
 
	printk("cs:0x%x ds:0x%x ss:0x%x esp:0x%lx esp0:0x%lx ss0:0x%x\n", cs, ds, ss, esp, esp0, ss0);

	p->cs = cs;
	p->eip = eip;
	p->eflags = eflags;
	p->esp = esp;

	p->cr3 = get_cr3();
	p->ds = ds;
	p->es = ds;
	p->fs = ds;
	p->gs = ds;
	p->ss = ss;
	p->esp0 = esp0;
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
	} tmp;

	tmp.a = 0;
	tmp.b = sel;

	__asm__ __volatile__ ("ljmp *%0\n\t" ::"m"(tmp)); 
//	__asm__ __volatile__ ("lcall %0\n\t" ::"m"(tmp)); 
	return ;

}


/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////

int setup_tss(void)
{
	u_int16_t cs, ds, ss;
	u_int32_t esp;

	printk("Setup TSS\n");
 
	__asm__ __volatile__("movw %%cs, %0\n\t;"
			     "movw %%ds, %1\n\t;"
			     "movw %%ss, %2\n\t;"
			     "movl %%esp, %3\n\t;"
			     :"=g"(cs), "=g"(ds), "=g"(ss), "=g"(esp)
		);
 
//	printk("cs:0x%x ds:0x%x ss:0x%x esp:0x%x\n", cs, ds, ss, esp);

 	set_tss(cs, ds, (u_int32_t) &test_task1, 0x202,
		(u_int32_t) &process_stack[0], ss,
		esp, ss);
	
	set_tss(cs, ds, (u_int32_t) &test_task2, 0x202,
		(u_int32_t) &process_stack[1], ss,
		esp, ss);
 
	
	set_gdt_values(0x28, (u_int32_t) &tss[0], sizeof(struct tss_struct), SEG_TYPE_TSS); 
	set_gdt_values(0x30, (u_int32_t) &tss[1], sizeof(struct tss_struct), SEG_TYPE_TSS); 

	ltr(0x28);

	return 0;
}

void schedule(void)
{
#if 1
	test_task1();
#endif
	return ;
}

#include <mikoOS/kernel.h>
#include <mikoOS/lock.h>
#include <mikoOS/printk.h>
#include <mikoOS/interrupt.h>
#include <asm/io.h>
#include "interrupt_handler.h"


#define INTERRUPT_HANDLER_NUM 256

// IDTR.
struct descriptor_table idtr;

// Interrupt Descriptor Table.
struct gate_descriptor intr_table[INTERRUPT_HANDLER_NUM];

// Interrupt hander function which is called from ISR or IRQ handler.
struct handler_function {
	void (*func) (struct registers regs);
};
struct handler_function handlers[INTERRUPT_HANDLER_NUM];

	
/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static void remap_irq(void);
static void set_interrupt_handler(void);

static void set_handler(int idx, u_int32_t base,
			u_int16_t selector, u_int8_t type)
{
	struct gate_descriptor *p = &intr_table[idx];

	p->offsetL = base & 0xffff;
	p->offsetH = (base >> 16) & 0xffff;
	p->selector = selector;
	p->type = type;

	p->count = 0; // unused.
}

/**
 * Remap IRQ to from 0x20.
 */
static void remap_irq(void)
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

/**
 * Setup interrupt handler to Interrupt descriptor table.
 */
static void set_interrupt_handler(void)
{
	int i;
	int size = sizeof(handler_info) / sizeof(handler_info[0]);
	struct handler_define *p = &handler_info[0];

	for (i = 0; i < size; i++, p++)
		set_handler(i, p->base, p->selector, p->type);

}

/**
 * Do lidt instruction.
 */
static void lidt(void)
{
	idtr.limit = sizeof(intr_table) - 1;
	idtr.base = (u_int32_t) intr_table;

	// load the idtr.
	__asm__ __volatile__ ("lidt %0;\n\t" ::"m"(idtr));

}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////

/**
 * Initialize interrupt service routine.
 */
void setup_inir(void)
{
	int i;

	remap_irq();

	set_interrupt_handler();

	// set dummy handler function.
	for (i = 0; i < INTERRUPT_HANDLER_NUM; i++)
		set_handler_func(i, NULL);

	lidt();

	// we are able to enable interrput.
	sti();

}

/**
 * Interrupt service routine.
 * @param regs is register.
 */
void isr_handler(struct registers regs) 
{
	printk("ISR 0x%x\n", regs.int_no);

	if (handlers[regs.int_no].func != NULL)
		handlers[regs.int_no].func(regs);

}

/**
 * IRQ interrupt  routine.
 * @param regs is register.
 */
void irq_handler(struct registers regs) 
{
	if (regs.int_no >= 40)
		outb(0xA0, 0x20);
       
	outb(0x20, 0x20);

	if (handlers[regs.int_no].func != NULL)
		handlers[regs.int_no].func(regs);
}

/**
 * Set interrupt handler function to function table.
 * @param f is handler function.
 */
inline void set_handler_func(int idx, void (*f)(struct registers regs))
{
	handlers[idx].func = f;
}


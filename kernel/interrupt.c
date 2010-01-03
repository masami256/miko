#include <mikoOS/kernel.h>
#include <mikoOS/lock.h>
#include <asm/io.h>
#include "interrupt.h"
#include "printk.h"

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

struct handler_define {
	u_int32_t base;
	u_int16_t selector;
	u_int8_t type;
};
struct handler_define  handler_info[] = {
	{ (u_int32_t) isr_gate0, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate1, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate2, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate3, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate4, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate5, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate6, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate7, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate8, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate9, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate10, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate11, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate12, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate13, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate14, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate15, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate16, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate17, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate18, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate19, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate20, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate21, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate22, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate23, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate24, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate25, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate26, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate27, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate28, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate29, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate30, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) isr_gate31, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate0, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate1, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate2, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate3, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate4, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate5, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate6, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate7, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate8, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate9, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate10, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate11, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate12, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate13, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate14, KERN_CS, GATE_TYPE_INTR_GATE },
	{ (u_int32_t) irq_gate15, KERN_CS, GATE_TYPE_INTR_GATE }
};
	
/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static void remap_irq(void);
static void set_handler_func(int idx, void (*f)(struct registers regs));
static void set_interrupt_handler(void);
static void timer_handler(struct registers regs);

static inline void set_handler_func(int idx, void (*f)(struct registers regs))
{
	handlers[idx].func = f;
}

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


static void set_interrupt_handler(void)
{
	unsigned  i;
	struct handler_define *p = &handler_info[0];

	for (i = 0; i < sizeof(handler_info) / sizeof(handler_info[0]); i++, p++)
		set_handler(i, p->base, p->selector, p->type);
}

static void lidt(void)
{
	idtr.limit = sizeof(intr_table) - 1;
	idtr.base = (u_int32_t) intr_table;

	// load the idtr.
	__asm__ __volatile__ ("lidt %0;\n\t" ::"m"(idtr));

}

static void timer_handler(struct registers regs)
{
	printk("Timer hanlder\n");
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

	set_handler_func(0x20, &timer_handler);

	lidt();

	// we are able to enable interrput.
	sti();

}


void isr_handler(struct registers regs) 
{
	printk("ISR 0x%x\n", regs.int_no);
}

void irq_handler(struct registers regs) 
{
	if (regs.int_no >= 40)
		outb(0xA0, 0x20);
       
	outb(0x20, 0x20);

	if (handlers[regs.int_no].func != NULL)
		handlers[regs.int_no].func(regs);
}


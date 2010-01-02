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

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static void remap_irq(void);
static void set_handler_func(int idx, void (*f)(struct registers regs));
static void set_isr_handler(void);
static void set_irq_handler(void);
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

static void set_isr_handler(void)
{
	set_handler(0, (u_int32_t) isr_gate0, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(1, (u_int32_t) isr_gate1, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(2, (u_int32_t) isr_gate2, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(3, (u_int32_t) isr_gate3, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(4, (u_int32_t) isr_gate4, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(5, (u_int32_t) isr_gate5, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(6, (u_int32_t) isr_gate6, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(7, (u_int32_t) isr_gate7, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(8, (u_int32_t) isr_gate8, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(9, (u_int32_t) isr_gate9, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(10, (u_int32_t) isr_gate10, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(11, (u_int32_t) isr_gate11, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(12, (u_int32_t) isr_gate12, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(13, (u_int32_t) isr_gate13, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(14, (u_int32_t) isr_gate14, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(15, (u_int32_t) isr_gate15, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(16, (u_int32_t) isr_gate16, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(17, (u_int32_t) isr_gate17, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(18, (u_int32_t) isr_gate18, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(19, (u_int32_t) isr_gate19, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(20, (u_int32_t) isr_gate20, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(21, (u_int32_t) isr_gate21, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(22, (u_int32_t) isr_gate22, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(23, (u_int32_t) isr_gate23, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(24, (u_int32_t) isr_gate24, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(25, (u_int32_t) isr_gate25, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(26, (u_int32_t) isr_gate26, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(27, (u_int32_t) isr_gate27, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(28, (u_int32_t) isr_gate28, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(29, (u_int32_t) isr_gate29, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(30, (u_int32_t) isr_gate30, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(31, (u_int32_t) isr_gate31, KERN_CS, GATE_TYPE_INTR_GATE);

}

static void set_irq_handler(void)
{
	set_handler(32, (u_int32_t) irq_gate0, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(33, (u_int32_t) irq_gate1, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(34, (u_int32_t) irq_gate2, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(35, (u_int32_t) irq_gate3, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(36, (u_int32_t) irq_gate4, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(37, (u_int32_t) irq_gate5, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(38, (u_int32_t) irq_gate6, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(39, (u_int32_t) irq_gate7, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(40, (u_int32_t) irq_gate8, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(41, (u_int32_t) irq_gate9, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(42, (u_int32_t) irq_gate10, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(43, (u_int32_t) irq_gate11, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(44, (u_int32_t) irq_gate12, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(45, (u_int32_t) irq_gate13, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(46, (u_int32_t) irq_gate14, KERN_CS, GATE_TYPE_INTR_GATE);
	set_handler(47, (u_int32_t) irq_gate15, KERN_CS, GATE_TYPE_INTR_GATE);

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

	set_isr_handler();
	set_irq_handler();

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


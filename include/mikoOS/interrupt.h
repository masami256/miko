#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include <asm/register.h>
#include <mikoOS/arch/x86/descriptor_common.h>

extern void setup_inir(void);
extern void isr_handler(struct registers regs);
extern void irq_handler(struct registers regs);
extern void set_handler_func(int idx, void (*f)(struct registers regs));

#endif // __INTERRUPT_H

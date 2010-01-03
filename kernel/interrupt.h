#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include <asm/register.h>
#include "descriptor_common.h"

extern void setup_inir(void);
extern void isr_handler(struct registers regs);
extern void irq_handler(struct registers regs);

#endif // __INTERRUPT_H

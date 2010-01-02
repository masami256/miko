#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include <asm/register.h>
#include "descriptor_common.h"

extern void setup_inir(void);
extern void isr_handler(struct registers regs);
extern void irq_handler(struct registers regs);

// defined in intr_gate.S
extern void isr_gate0(void);
extern void isr_gate1(void);
extern void isr_gate2(void);
extern void isr_gate3(void);
extern void isr_gate4(void);
extern void isr_gate5(void);
extern void isr_gate6(void);
extern void isr_gate7(void);
extern void isr_gate8(void);
extern void isr_gate9(void);
extern void isr_gate10(void);
extern void isr_gate11(void);
extern void isr_gate12(void);
extern void isr_gate13(void);
extern void isr_gate14(void);
extern void isr_gate15(void);
extern void isr_gate16(void);
extern void isr_gate17(void);
extern void isr_gate18(void);
extern void isr_gate19(void);
extern void isr_gate20(void);
extern void isr_gate21(void);
extern void isr_gate22(void);
extern void isr_gate23(void);
extern void isr_gate24(void);
extern void isr_gate25(void);
extern void isr_gate26(void);
extern void isr_gate27(void);
extern void isr_gate28(void);
extern void isr_gate29(void);
extern void isr_gate30(void);
extern void isr_gate31(void);
extern void irq_gate0(void);
extern void irq_gate1(void);
extern void irq_gate2(void);
extern void irq_gate3(void);
extern void irq_gate4(void);
extern void irq_gate5(void);
extern void irq_gate6(void);
extern void irq_gate7(void);
extern void irq_gate8(void);
extern void irq_gate9(void);
extern void irq_gate10(void);
extern void irq_gate11(void);
extern void irq_gate12(void);
extern void irq_gate13(void);
extern void irq_gate14(void);
extern void irq_gate15(void);

#endif // __INTERRUPT_H

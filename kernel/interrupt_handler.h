#ifndef __INTERRUPT_HANDLER_H
#define __INTERRUPT_HANDLER_H 1

#include <mikoOS/kernel.h>

#define HANDLER_INFO_NUM 49

struct handler_define {
	u_int32_t base;
	u_int16_t selector;
	u_int8_t type;
};

extern struct handler_define handler_info[HANDLER_INFO_NUM];

// Interrupt handler for 0x80.
extern void soft_intr_handler(void);

// defined in intr_gate.S
// fourty eight functions.
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

#include <mikoOS/kernel.h>
#include "descriptor_common.h"
#include "interrupt_handler.h"

struct handler_define  handler_info[HANDLER_INFO_NUM] = {
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
	{ (u_int32_t) isr_gate13, KERN_CS, GATE_TYPE_TRAP_GATE },
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
	{ (u_int32_t) irq_gate15, KERN_CS, GATE_TYPE_INTR_GATE },
};
.code32
.align 4
	
#define isr_gate_stub(num) \
.globl isr_gate##num		;\
isr_gate##num: 			;\
	cli			;\
	pushal			;\
	pushl	%esp		;\
	pushl 	$##num		;\
	call 	isr_handler	;\
	addl 	$8, %esp	;\
	popal			;\
	sti			;\
	iret
	
#define irq_gate_stub(num_a,num_b) \
.globl irq_gate##num_a 		;\
irq_gate##num_a: 		;\
	cli			;\
	pushal			;\
	pushl 	%esp		;\
	pushl 	$##num_b	;\
	call 	irq_handler	;\
	addl 	$8, %esp	;\
	popal			;\
	sti			;\
	iret

isr_gate_stub(0)
isr_gate_stub(1)
isr_gate_stub(2)
isr_gate_stub(3)
isr_gate_stub(4)
isr_gate_stub(5)
isr_gate_stub(6)
isr_gate_stub(7)
isr_gate_stub(8)
isr_gate_stub(9)
isr_gate_stub(10)
isr_gate_stub(11)
isr_gate_stub(12)
isr_gate_stub(13)
isr_gate_stub(14)
isr_gate_stub(15)
isr_gate_stub(16)
isr_gate_stub(17)
isr_gate_stub(18)
isr_gate_stub(19)
isr_gate_stub(20)
isr_gate_stub(21)
isr_gate_stub(22)
isr_gate_stub(23)
isr_gate_stub(24)
isr_gate_stub(25)
isr_gate_stub(26)
isr_gate_stub(27)
isr_gate_stub(28)
isr_gate_stub(29)
isr_gate_stub(30)
isr_gate_stub(31)
irq_gate_stub(0,32)
irq_gate_stub(1,33)
irq_gate_stub(2,34)
irq_gate_stub(3,35)
irq_gate_stub(4,36)
irq_gate_stub(5,37)
irq_gate_stub(6,38)
irq_gate_stub(7,39)
irq_gate_stub(8,40)
irq_gate_stub(9,41)
irq_gate_stub(10,42)
irq_gate_stub(11,43)
irq_gate_stub(12,44)
irq_gate_stub(13,45)
irq_gate_stub(14,46)
irq_gate_stub(15,47)
	
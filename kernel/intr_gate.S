.code32
.align 4

.extern soft_intr_handler
.globl soft_intr_handler
	cli
	pusha
	mov	%ds, %ax
	push 	%eax

	mov	$0x10, %ax 
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs 

	call	irq_handler

	pop	%ebx
	mov	%bx, %ds
	mov	%bx, %es
	mov	%bx, %fs
	mov	%bx, %gs

	popa
	add 	$0x8, %esp
	sti
	iret

#define isr_gate_stub(num) \
.globl isr_gate##num		;\
isr_gate##num: 			;\
	cli			;\
	push $##num		;\
	jmp isr_common		;\


#define irq_gate_stub(num_a,num_b) \
.globl irq_gate##num_a 		;\
irq_gate##num_a: 		;\
	cli 			;\
	push $0 		;\
	push $##num_b 		;\
	jmp irq_common		;\
	
	
.extern isr_handler
isr_common:
	pusha
	mov	%ds, %ax
	push	%eax

	mov	$0x10, %ax 
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs 
	
	call 	isr_handler

	pop	%ebx
	mov	%bx, %ds
	mov	%bx, %es
	mov	%bx, %fs
	mov	%bx, %gs

	popa
	add 	$0x8, %esp
	sti
	iret

.extern irq_handler
irq_common:
	pusha
	mov	%ds, %ax
	push 	%eax

	mov	$0x10, %ax 
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs 

	call	irq_handler

	pop	%ebx
	mov	%bx, %ds
	mov	%bx, %es
	mov	%bx, %fs
	mov	%bx, %gs

	popa
	add 	$0x8, %esp
	sti
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
	
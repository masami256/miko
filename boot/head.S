/*
 * The _start is the first function of bootstrap
 * GRUB will call this function
 */
#define ASM 1
#include <mikoOS/multiboot.h>
	
.section .entry 
.code32
	
.globl _start
_start:
	jmp	entry
	/* align for 32bit mode */
	.align 	4

	/* This is the multiboot header for GRUB */
multiboot_hader :
	.long 	MULTIBOOT_HEADER_MAGIC
	.long 	MULTIBOOT_HEADER_FLAGS
	.long 	CHECKSUM

entry:
	.comm stack, KERNEL_STACK_SIZE, 32     /*reserve 16k stack on a quadword boundary */
	.comm pgd, 2046, 4
	push 	%ebx /* ebx point to 32bit address of the multiboot information structure */
	push 	%eax /* eax must be 0x2BADB002 */
	call 	cmain  /* call c function */

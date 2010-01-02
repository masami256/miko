#include <mikoOS/kernel.h>
#include <mikoOS/multiboot.h>
#include "gdt.h"
#include "interrupt.h"
#include "printk.h"

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

// cmain() is called from head.S.
extern void cmain(unsigned long magic, unsigned long addr);

/**
 * This function is called from head.S
 * @param magic it's a magic number by GRUB's multiboot protocol.
 * @param addr it points to structure of multiboot_info_t.
 */
void cmain(unsigned long magic, unsigned long addr)
{
	multiboot_info_t *mbi = (multiboot_info_t *) addr;

	// initialize console to display messages.
	cls();

	if (magic != MULTIBOOT_HEADER_MAGIC) 
		printk("Invalid magic number : 0x%x\n", (unsigned) magic);

	// Even though, GRUG has set GDT, we need to setup it again.
	printk("Initialize gdt.\n");
	setup_gdt();
	
	// Then, we are going to enable interrupts.

	if (CHECK_FLAG (mbi->flags, 0))
		printk("mem_lower = %uKB, mem_upper = %uKB\n",
			(unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

	// Setup initerrupt service routine.
	printk("Setup initerrupt service routine.\n");
	setup_inir();

	printk("Welcome to mikoOS!\n");

	while (1);

}


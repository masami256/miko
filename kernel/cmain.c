#include <mikoOS/kernel.h>
#include <mikoOS/multiboot.h>
#include <mikoOS/printk.h>
#include <mikoOS/pci.h>
#include <mikoOS/mm.h>
#include <mikoOS/kmalloc.h>

#include "gdt.h"
#include "interrupt.h"


#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

// cmain() is called from head.S.
extern void cmain(unsigned long magic, unsigned long addr);

void show_startup_message(void)
{
	char *p;
	u_int32_t addr;

	addr = get_free_pages(2);
	if (addr) {
		int i;
		p = (char *) addr;

		for (i = 0; i < 26; i++)
			p[i] = i + 'a';
		for (i = 0 ; i < 26; i++)
			p[i + 26] = i + 'A';
		printk("get_free_page()test [%s]\n", p);

	}
	printk("Welcome to mikoOS!\n");

}

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
	
	if (CHECK_FLAG (mbi->flags, 0))
		printk("mem_lower = %uKB, mem_upper = %uKB\n",
			(unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

	// Setup initerrupt service routine.
	printk("Setup initerrupt service routine.\n");
	setup_inir();

	// Setup paging.
	setup_paging();

	init_kmalloc_area();
	// Init PCI
	find_pci_device();

	show_startup_message();

	kmalloc_test();

	while (1);

}


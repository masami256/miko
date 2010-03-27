#include <mikoOS/kernel.h>
#include <mikoOS/multiboot.h>
#include <mikoOS/printk.h>
#include <mikoOS/interrupt.h>
#include <mikoOS/pci.h>
#include <mikoOS/mm.h>
#include <mikoOS/kmalloc.h>
#include <mikoOS/block_driver.h>
#include <mikoOS/ata.h>
#include <mikoOS/timer.h>
#include <mikoOS/abort.h>
#include <mikoOS/lock.h>

#include "gdt.h"

#include "process.h"

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

// cmain() is called from head.S.
extern void cmain(unsigned long magic, unsigned long addr);

static void test_syscall(void)
{
	__asm__ ("xorl %eax, %eax\n\t"
		 "movl $0, %eax\n\t"
		 "int $0x80\n\t");

	__asm__ ("xorl %eax, %eax\n\t"
		 "movl $0x1, %eax\n\t"
		 "int $0x80\n\t");

	__asm__ ("xorl %eax, %eax\n\t"
		 "movl $0x2, %eax\n\t"
		 "int $0x80\n\t");

	__asm__ ("xorl %eax, %eax\n\t"
		 "movl $0x3, %eax\n\t"
		 "int $0x80\n\t");

	__asm__ ("xorl %eax, %eax\n\t"
		 "movl $0x4, %eax\n\t"
		 "int $0x80\n\t");

	__asm__ ("xorl %eax, %eax\n\t"
		 "movl $0x5, %eax\n\t"
		 "int $0x80\n\t");

	__asm__ ("xorl %eax, %eax\n\t"
		 "movl $0x6, %eax\n\t"
		 "int $0x80\n\t");

	__asm__ ("xorl %eax, %eax\n\t"
		 "movl $0x7, %eax\n\t"
		 "int $0x80\n\t");

	__asm__ ("xorl %eax, %eax\n\t"
		 "movl $0xbeef, %eax\n\t"
		 "int $0x80\n\t");

}

static void show_startup_message(void)
{
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
	struct blk_device_drivers *p;
	
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

	cli();

	// setup fault handler.
	setup_fault_handler();

	// Init timer handler.
	timer_init();

	// Setup paging.
	setup_paging();

	// setup memory for kmalloc().
	init_kmalloc_area();

	// Init PCI
	find_pci_device();

	// Init ATA device.
	init_ata_disk_driver();

	show_all_registered_driver();

	// setup tss for processes.
	setup_tss();

	sti();

	show_startup_message();

	test_syscall();

	while (1);

}


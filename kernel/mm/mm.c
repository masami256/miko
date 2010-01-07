#include <mikoOS/kernel.h>
#include "mm.h"
#include "../printk.h"
#include "../interrupt.h"

static void page_fault(struct registers regs);

// This should be page aligned.
pgd_t page_directory[1024] __attribute__((aligned(0x1000))); 
pte_t pte[1024];

static void setup_pte(void)
{
	unsigned int address = 0; 
	int i;

	page_directory[0] = (pgd_t) &pte | PAGE_PRESENT | PAGE_RW;

	for(i = 0; i < 1024; i++) {
		// PAGE is present and readable/writable.
		pte[i] = address | PAGE_PRESENT | PAGE_RW; 
		address = address + 4096; 
	}
}

static void set_cr3(void)
{
	__asm__ __volatile__("mov %0, %%cr3":: "b"(page_directory));
}

static void set_cr0(void)
{
	u_int32_t cr0;

	__asm__ __volatile__("mov %%cr0, %0": "=b"(cr0));
	cr0 |= 0x80000000;
	__asm__ __volatile__("mov %0, %%cr0":: "b"(cr0));
}

void setup_paging(void)
{
	printk("Setup paging.\n");

	printk("Start setup_pte.\n");
	setup_pte();

	printk("Set page fault handler.\n");
	set_handler_func(0x0e, &page_fault);

	printk("Set cr3: load page directory table.\n");
	set_cr3();

	printk("set cr0: enable paging\n");
	set_cr0();
}

/**
 * Page fault handler test function currently.
 */
static void page_fault(struct registers regs)
{
	printk("Page Fault\n");
	while (1);
}

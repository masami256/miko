#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include "mm.h"
#include "../interrupt.h"

static void page_fault(struct registers regs);

// This should be page aligned.
pgd_t page_directory[1024] __attribute__((aligned(0x1000))); 
pte_t pte[1024];

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
/**
 * Setup PGD and PTE.
 */
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

/**
 * Load PGD into cr3 register.
 */
static void set_cr3(void)
{
	__asm__ __volatile__("mov %0, %%cr3":: "b"(page_directory));
}

/**
 * Enable Paging feature.
 */
static void set_cr0(void)
{
	u_int32_t cr0;

	__asm__ __volatile__("mov %%cr0, %0": "=b"(cr0));
	cr0 |= 0x80000000;
	__asm__ __volatile__("mov %0, %%cr0":: "b"(cr0));
}

/**
 * Page fault handler test function currently.
 */
static void page_fault(struct registers regs)
{
	printk("Page Fault\n");
	while (1);
}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////

/**
 * Setup PGD, PTE then enable paging.
 */
void setup_paging(void)
{
	printk("Setup paging.\n");

	setup_pte();

	set_handler_func(0x0e, &page_fault);

	set_cr3();

	set_cr0();
}


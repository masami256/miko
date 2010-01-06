#include <mikoOS/kernel.h>
#include "mm.h"
#include "../printk.h"
#include "../interrupt.h"

static void page_fault(struct registers regs);

// This variable should be aligned to page boundary.
//pgd_t pgd[1024] __attribute__((aligned(0x1000)));
extern pgd_t pgd;

pgd_t *page_directory;
pte_t *pte;

static void setup_pte(void)
{
#if 0
	int i;
	pte = (pte_t *) &pgd;
	u_int32_t addr = 0;

	for (i = 0; i < sizeof(pgd) / sizeof(pgd[0]); i++) {
		pte[i] = addr | PAGE_PRESENT | PAGE_RW;
		addr += 0x1000;
	}
#else
	unsigned int page_aligned_end = (pgd & 0xfffff000) + 0x1000;
	int i = 0;
	unsigned int address = 0; 

	page_directory = (pgd_t *) page_aligned_end;

	for(i = 0; i < 1024; i++) {
		//attribute: supervisor level, read/write, not present.
		page_directory[i] = 0 | 2; 
	}
	
	pte = page_directory + 0x1000;
	
        //we will fill all 1024 entries, mapping 4 megabytes
	for(i = 0; i < 1024; i++) {
		pte[i] = address | 3; // attributes: supervisor level, read/write, present.
		address = address + 4096; //advance the address to the next page boundary
	}

	page_directory[0] = pte; 
	page_directory[0] |= 3;// attributes: supervisor level, read/write, present
#endif


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
}

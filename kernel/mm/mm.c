#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/mm.h>
#include <mikoOS/interrupt.h>

// This should be page aligned.
pgd_t page_directory[PGD_SIZE] __attribute__((aligned(0x1000))); 
pte_t pte[PTE_SIZE];

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static void page_fault(struct registers *regs);

/**
 * Setup PGD and PTE.
 */
static void setup_pte(void)
{
	unsigned long address = 0; 
	int i;

	page_directory[0] = (pgd_t) &pte | PAGE_PRESENT | PAGE_RW;

	for(i = 0; i < PTE_SIZE; i++) {
		// PAGE is present and readable/writable.
		pte[i] = address | PAGE_PRESENT | PAGE_RW; 
		address = address + PAGE_SIZE; 
	}

	// Address 0 is reserved.
	pte[0] |= PAGE_ACCESS;
}

/**
 * Load PGD into cr3 register.
 */
static void set_cr3(void)
{
	__asm__ __volatile__("mov %0, %%cr3\n\t":: "r"(page_directory));
}

/**
 * Enable Paging feature.
 */
static void set_cr0(void)
{
	u_int32_t cr0;

	__asm__ __volatile__("mov %%cr0, %0\n\t": "=r"(cr0));
	cr0 |= 0x80000000;
	__asm__ __volatile__("mov %0, %%cr0\n\t":: "r"(cr0));
}

/**
 * Page fault handler test function currently.
 */
static void page_fault(UNUSED struct registers *regs)
{
	printk("Page Fault\n");
	while (1);
}

/**
 * Check access bit.
 * @param index of pte.
 * @return if haven't accessed yet it returns 0.
 */
static int is_free_page(u_int32_t idx)
{
	return pte[idx] & PAGE_ACCESS;
}

/**
 * Clear access bit.
 * @param index of pte.
 */
static void clear_access_bit(u_int32_t idx)
{
	pte[idx] &= ~PAGE_ACCESS;
}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////

/**
 * Find free page(s).
 * @param  how many pages?
 * @return if there is no page it returns 0. if there is enough page, it returns first page's address.
 */
u_int32_t get_free_pages(u_int32_t pages)
{
	// current index for pte.
	static u_int32_t current_idx = 0;
	u_int32_t i;
	u_int32_t enough;
	u_int32_t ret = 0;
	bool found = false;

	enough = 0;
	for (i = 0; i < PTE_SIZE; i++) {
		if (current_idx == PTE_SIZE)
			current_idx = 0;

		if (!enough)
			ret = pte[current_idx] & 0xfffff000;

		if (!is_free_page(current_idx)) {
			enough++;
			clear_access_bit(current_idx);
			if (enough == pages) {
				current_idx++;
				found = true;
				break;
			}
		} else {
			enough = 0;
		}
		current_idx++;
	} 
	return found ? ret : 0;
}

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

u_int32_t get_cr3(void)
{
	u_int32_t cr3 = 0;

	__asm__ __volatile__("mov %%cr3, %0\n\t" :"=r"(cr3));
	return cr3;
}

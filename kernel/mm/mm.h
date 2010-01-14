#ifndef __MM_H
#define __MM_H

typedef u_int32_t pgd_t;
typedef u_int32_t pte_t;

#define PAGE_PRESENT 0x01 // Present bit
#define PAGE_RW      0x02 // R/W bit
#define PAGE_US      0x04 // U/S bit
#define PAGE_ACCESS  0x20 // Access bit
#define PAGE_DIRTY   0x40 // Dirty bit

#define PAGE_SIZE    0x1000 // 4KB page
#define PGD_SIZE     1024
#define PTE_SIZE     1024

void setup_paging(void);

#endif // __MM_H

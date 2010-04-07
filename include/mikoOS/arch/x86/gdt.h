#ifndef __MIKOOS_X86_GDT_H
#define __MIKOOS_X86_GDT_H 1

#include <mikoOS/arch/x86/descriptor_common.h>

// Number of GDT table.
#define GDT_TABLE_NUM 64
	
// gdt.c
void setup_gdt(void);
void set_gdt_values(u_int32_t index, u_int32_t base, 
		    u_int32_t limit, u_int8_t type);

void set_ldt_values(u_int32_t index, u_int32_t base, 
		    u_int32_t limit, u_int8_t type);

int search_unused_gdt_index(void);

#endif // __MIKOOS_X86_GDT_H

#include <mikoOS/kernel.h>
#include "gdt.h"

static struct descriptor_table gdtr;
static struct segment_descriptor gdt[GDT_TABLE_NUM];

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static void set_values(u_int32_t index, u_int32_t base, 
			u_int32_t limit, u_int8_t type);
static void setup_gdt_descriptor(void);
static void lgdt(void);

/**
 * set gdt descriptor values.
 * @param index is index of gdt descriptor tabel.
 * @param base bass address.
 * @param limit limit.
 * @param type what type is this descriptor.
 */
static void 
set_values(u_int32_t index, u_int32_t base, 
	   u_int32_t limit, u_int8_t type)
{
	struct segment_descriptor *p = &gdt[index];

	p->limit0 = (limit >> 16) & 0xffff;

	p->base0 = base & 0xff;
	p->base1 = (base >> 8) & 0xff;
	p->base2 = (base >> 16) & 0xff;
	p->base3 = (base >> 24) & 0xff;

	p->attr1 = type;
	p->attr2 = 0xc0;
}

/**
 * setup gdt descriptors.
 */
static void setup_gdt_descriptor(void)
{
	set_values(0, 0, 0, 0);
	set_values(1, 0, 0xffffffff, SEG_TYPE_CODE);
	set_values(2, 0, 0xffffffff, SEG_TYPE_DATA);
	set_values(3, 0, 0, SEG_TYPE_STACK);
	set_values(4, 0, 0xffffffff, SEG_TYPE_USER_CODE);
	set_values(5, 0, 0xffffffff, SEG_TYPE_USER_DATA);
}

/**
 * load gdt table to memory and do segment jump.
 */
static void lgdt(void)
{
	// setup gdtr.
	gdtr.limit = sizeof(gdt) - 1;
	gdtr.base = (u_int32_t) gdt;

	// now, we can load new gdt value.
	__asm__ __volatile__ ("lgdt %0;\n\t" ::"m"(gdtr));

	// after loading gdt, we need to do segment jump.
	__asm__ (
		"ljmp $0x08, $1f;\n\t"
		"1:;\n\t"
		"movl $0x10, %eax;\n\t"
		"movl %eax, %ds;\n\t"
		"movl %eax, %es\n\t;"
		"movl %eax, %fs\n\t"
		"movl %eax, %gs\n\t"
		"movl %eax, %ss\n\t"
		);

}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////
/**
 * Setup GDTR and segment descriptor for GDT.
 */
void setup_gdt(void)
{
	// setup gdt descriptors.
	setup_gdt_descriptor();

	// load gdtr to memory.
	lgdt();
}

#include <mikoOS/kernel.h>
#include "gdt.h"
#include "process.h"

static struct descriptor_table gdtr;
static struct segment_descriptor gdt[GDT_TABLE_NUM];

static u_int32_t gdt_count;

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static void setup_gdt_descriptor(void);
static void lgdt(void);

/**
 * setup gdt descriptors.
 */
static void setup_gdt_descriptor(void)
{
	set_gdt_values(0, 0, 0, 0);
	set_gdt_values(SEL_KERN_CS, 0, 0xffffffff, SEG_TYPE_CODE);
	set_gdt_values(SEL_KERN_DS, 0, 0xffffffff, SEG_TYPE_DATA);
//	set_gdt_values(SEL_KERN_SS, 0, 0, SEG_TYPE_STACK);
	set_gdt_values(SEL_USER_CODE, 0, 0xffffffff, SEG_TYPE_USER_CODE);
	set_gdt_values(SEL_USER_DATA, 0, 0xffffffff, SEG_TYPE_USER_DATA);
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
		"movw $0x10, %ax;\n\t"
		"movw %ax, %ds;\n\t"
		"movw %ax, %es\n\t;"
		"movw %ax, %fs\n\t"
		"movw %ax, %gs\n\t"
		"movw %ax, %ss\n\t"
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


/**
 * set gdt descriptor values.
 * @param index is index of gdt descriptor tabel.
 * @param base bass address.
 * @param limit limit.
 * @param type what type is this descriptor.
 */
void set_gdt_values(u_int32_t index, u_int32_t base, 
		    u_int32_t limit, u_int8_t type)
{
	struct segment_descriptor *p = &gdt[index >> 3];

	// Change Sz and Gr bit by limit size.
	if (limit > 65536)
		p->limit_h = 0xc0;
	else
		p->limit_h = 0x40;

	p->limit_l = limit & 0xffff;

	p->base_l = base & 0xffff;
	p->base_m = (base >> 16) & 0xff;
	p->base_h = (base >> 24) & 0xff;

	p->type = type;

	p->limit_h |= (limit >> 16) & 0xf;

	printk("0x%x p->type:0x%x p->limit_h:0x%x\n", index, p->type, p->limit_h);
	gdt_count++;
}


/**
 * Search unused gdt table.
 * @retun If there is empty table it returns index.
 */
int search_unused_gdt_index(void)
{
	int i, idx;
	for (i = 0, idx = gdt_count; i < GDT_TABLE_NUM; i++, idx++) {
		if (i >= GDT_TABLE_NUM) 
			idx = 1;

		if (gdt[idx].type == 0)
			return idx << 8;

	}

	return 0;

}

void gdt_types(void)
{
	int i;
	for (i = 1; i < 10; i++) {
		printk("gdt[%d:0x%x]'s type:0x%x\n", i, i << 3, gdt[i].type);

	}
}


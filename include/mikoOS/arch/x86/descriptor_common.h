#ifndef __DESCRIPTOR_COMMON_H
#define __DESCRIPTOR_COMMON_H 1

#include <mikoOS/kernel.h>

// selectors.
#define SEL_KERN_CS 0x08
#define SEL_KERN_DS 0x10
//#define SEL_KERN_SS 0x18
#define SEL_USER_CODE 0x18
#define SEL_USER_DATA 0x20
#define SEL_TSS 0x28

// gate types.
#define GATE_TYPE_CALL_GATE 0x84
#define GATE_TYPE_INTR_GATE 0x8e
#define GATE_TYPE_TRAP_GATE 0x8f
#define GATE_TYPE_TASK_GATE 0x85

// segment type.
#define SEG_TYPE_CODE 0x9a
#define SEG_TYPE_DATA 0x92
#define SEG_TYPE_STACK 0x96
#define SEG_TYPE_LDT 0x82
#define SEG_TYPE_TSS 0x89
#define SEG_TYPE_TSS_BUSY 0x8b
#define SEG_TYPE_USER_CODE 0xf2
#define SEG_TYPE_USER_DATA 0xfa


/**
 * this structure represent of segment descriptor
 */
struct segment_descriptor {
	u_int16_t limit_l;
	u_int16_t base_l;
	u_int8_t base_m;
	u_int8_t type;
	u_int8_t limit_h;
	u_int8_t base_h;
} __attribute__ ((packed));

/**
 * Gate descriptor table.
 */
struct gate_descriptor {
	u_int16_t offsetL;
	u_int16_t selector;
	u_int8_t count;
	u_int8_t type;
	u_int16_t offsetH;
} __attribute__((packed));

/** 
 * This structure uses for descriptor table
 * e.g. gdt, interruput and etc.
 * This structure shouldn't have any padding space.
 */
struct descriptor_table {
	u_int16_t limit;
	u_int32_t base;
} __attribute__ ((packed));


#endif // __DESCRIPTOR_COMMON_H

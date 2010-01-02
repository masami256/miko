#ifndef SEGMENT_H
#define SEGMENT_H 1

#include "descriptor_common.h"

// Number of GDT table.
#define GDT_TABLE_NUM 6
	
#define SEG_TYPE_CODE 0x9a
#define SEG_TYPE_DATA 0x92
#define SEG_TYPE_STACK 0x96
#define SEG_TYPE_LDT 0x82
#define SEG_TYPE_TSS 0x89
#define SEG_TYPE_TSS_BUSY 0x8b
#define SEG_TYPE_USER_CODE 0xf2
#define SEG_TYPE_USER_DATA 0xfa

// gdt.c
void setup_gdt(void);

#endif // SEGMENT_H

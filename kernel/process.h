#ifndef __MIKOOS_PROCESS_H
#define __MIKOOS_PROCESS_H 1
#include <sys/types.h>

#define PROCESS_STACK_SIZE 4096

// This structure is for Intel TSS feature.
struct tss_struct {
	u_int16_t backlink;
	u_int16_t dummy_backlink;
	u_int32_t esp0;
	u_int16_t ss0;
	u_int16_t dummy_ss0;
	u_int32_t esp1;
	u_int16_t ss1;
	u_int16_t ss1_dummy;
	u_int32_t esp2;
	u_int16_t ss2;
	u_int16_t ss2_dummy;
	u_int32_t cr3;
	u_int32_t eip;
	u_int32_t eflags;
	u_int32_t eax;
	u_int32_t ecx;
	u_int32_t edx;
	u_int32_t ebx;
	u_int32_t esp;
	u_int32_t ebp;
	u_int32_t esi;
	u_int32_t edi;
	u_int16_t es;
	u_int16_t es_dummy;
	u_int16_t cs;
	u_int16_t cs_dummy;
	u_int16_t ss;
	u_int16_t ss_dummy;
	u_int16_t ds;
	u_int16_t ds_dummy;
	u_int16_t fs;
	u_int16_t fs_dummy;
	u_int16_t gs;
	u_int16_t gs_dummy;
	u_int16_t ldtr;
	u_int16_t ldtr_dummy;
	u_int32_t io_bitmap;
} __attribute__((packed));

void scheduler(void);

#endif // __MIKOOS_PROCESS_H

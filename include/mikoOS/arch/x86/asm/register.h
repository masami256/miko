#ifndef __MIKOOS_REGISTER_H
#define __MIKOOS_REGISTER_H 1

#include <sys/types.h>

struct registers {
	u_int32_t dummy; // don't know what is it.
	u_int32_t edi;
	u_int32_t esi;
	u_int32_t ebp;
	u_int32_t esp;
	u_int32_t ebx;
	u_int32_t edx;
	u_int32_t ecx;
	u_int32_t eax;
	u_int32_t eip;
	u_int32_t eflags;
};


#endif // __MIKOOS_REGISTER_H

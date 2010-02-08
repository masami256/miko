#ifndef __MIKOOS_REGISTER_H
#define __MIKOOS_REGISTER_H 1

#include <sys/types.h>

struct registers
{
	u_int32_t ds;                  // Data segment selector.
	u_int32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
	u_int32_t int_no, err_code;    // Interrupt number and error code (if applicable).
	u_int32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.

};

#endif // __MIKOOS_REGISTER_H

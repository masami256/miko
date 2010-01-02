#ifndef __PRINTK_H
#define __PRINTK_H

#include <mikoOS/stdarg.h>

extern void printk(char *format, ...)  __attribute__((format(printf, 1, 2)));
extern void cls(void);

#endif // __PRINTK_H


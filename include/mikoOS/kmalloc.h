#ifndef __MIKOOS_KMALLOC_H
#define __MIKOOS_KMALLOC_H 1

int init_kmalloc_area(void);
void *kmalloc(size_t size);
void kfree(void *ptr);
int kmalloc_test(void);

#endif // __MIKOOS_KMALLOC_H


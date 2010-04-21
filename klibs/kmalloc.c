#include <mikoOS/kernel.h>
#include <mikoOS/kmalloc.h>
#include <mikoOS/mm.h>
#include <mikoOS/printk.h>
#include <mikoOS/string.h>

// kmalloc header has 2 member.
// one is prev which is point previous allocated address.
// size is allocated size plus this header size.
struct  kmalloc_header {
	unsigned long prev;
	u_int32_t size;
};
static char *kmalloc_area;

// linked list for kmalloc_header.
static struct kmalloc_header *base;
static struct kmalloc_header *next_free_area;

#define KMALLOC_MAX_SIZE 512
#define KMALLOC_ALIGN sizeof(void *)
#define KMALLOC_USED 0x01

// 8KB is reserved for kmalloc.x
#define KMALLOC_MEMORY_SIZE (PAGE_SIZE * 8)

/**
 * Init buffer for kmalloc.
 * Get some pages and init linked list.
 * @return if returns -1 we don't have enough pages. 0 is success.
 */
int init_kmalloc_area(void)
{
	unsigned long n;

	kmalloc_area = (char *) get_free_pages(32);
	if (!kmalloc_area)
		return -1;

	memset(kmalloc_area, 0, KMALLOC_MEMORY_SIZE);

	n = (unsigned long) kmalloc_area;
	base = (struct kmalloc_header *) n;

	base->size = KMALLOC_MEMORY_SIZE;
	base->prev = 0;
	next_free_area = base;

//	printk("base address is 0x%x\n", base);
	return 0;
}

/**
 * This is the main logic.
 * @param size will be celling to align sizeof(void *).
 * @return non null if success.
 */
void *kmalloc(size_t size)
{
	struct kmalloc_header *p;
	void *ret = NULL;

	if (size > KMALLOC_MAX_SIZE)
		return NULL;

	// Ceilling 
	size = (size + sizeof(struct kmalloc_header) + KMALLOC_ALIGN - 1) & -KMALLOC_ALIGN;

//	printk("header size = %d : kmalloc size = %d\n", sizeof(struct kmalloc_header), size);

	p = next_free_area;
	do {
		if (p->size >= size) {
			next_free_area = (struct kmalloc_header *) ((char *) p + size);
			next_free_area->size = p->size - size;
			next_free_area->prev = (unsigned long) p;
			((struct  kmalloc_header *) next_free_area->prev)->prev = p->prev;

			// This address is using.
			next_free_area->prev ^= KMALLOC_USED;

			p->size = size;
#if 0
			printk("free size is %d : alloc address is 0x%x : next_free_area->prev is 0x%x, prev->prev is 0x%x\n", 
			       next_free_area->size, p, 
			       next_free_area->prev, 
			       ((struct kmalloc_header *) next_free_area->prev)->prev);
#endif
			ret = (void *) ((char *) p + sizeof(struct kmalloc_header));
			break;
		} 
#if 0
		else 
			printk("next_free_area->size is %d. so not enough size to allocate %d byte(0x%x)\n", next_free_area->size, size, p); 
#endif
		if (p->prev & KMALLOC_USED) {
			while (p->prev & KMALLOC_USED) 
				p = (struct kmalloc_header *) (p->prev ^ KMALLOC_USED);
		} else {
			p = (struct kmalloc_header *) p->prev;
		}

	} while (p != base);

	return ret;

}

/**
 * free allocated area.
 * @param ptr has to point to head address whcic kmalloc returned.
 */
void kfree(void *ptr)
{
	unsigned long addr = 0; 
	struct kmalloc_header *p, *prev;
	struct kmalloc_header *tmp;
	u_int32_t new_size = 0;

	if (!ptr)
		return ;

	addr = (unsigned long) (((char *) ptr) - sizeof(struct kmalloc_header));
	p = next_free_area;

	do {
		if ((p->prev ^ KMALLOC_USED) == addr) {
			p->prev ^= KMALLOC_USED;

			tmp = (struct kmalloc_header *) p->prev;

			printk("%lu byte is going to be freed\n", tmp->size);

			// if next_free_area is free area, make big free area.
			if (next_free_area == p) {
				new_size = next_free_area->size + tmp->size;
				next_free_area = (struct kmalloc_header *) p->prev;
				next_free_area->size = new_size;
			}

			// if previous header isn't used, make big free area
			prev = (struct kmalloc_header *) tmp->prev;
			if (prev && !(prev->prev & KMALLOC_USED)) {
				new_size = prev->size + next_free_area->size;
				next_free_area = prev;
				next_free_area->size = new_size;
			}
					
			break;
		}

		p = (struct kmalloc_header *) (p->prev ^ KMALLOC_USED);

	} while (p != base);

}

// for test.
#ifdef KMALLOC_TEST
int kmalloc_test(void)
{
	char *p, *pp;

	if (init_kmalloc_area()) {
		printk("init_kmalloc_area is failed\n");
		return -1;
	}

	p = kmalloc(400);
	printk("ret is 0x%x\n", p);
	kfree(p);

	p = kmalloc(400);
	printk("ret is 0x%x\n", p);

	pp = kmalloc(400);
	printk("ret is 0x%x\n", pp);

	kfree(p);
	p = kmalloc(500);
	printk("ret is 0x%x\n", p);

	kfree(pp);
	p = kmalloc(500);
	printk("ret is 0x%x\n", p);

	return 0;
}
#endif // KMALLOC_TEST


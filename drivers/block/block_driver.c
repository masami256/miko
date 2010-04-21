#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/kmalloc.h>
#include <mikoOS/block_driver.h>
#include <mikoOS/abort.h>
#include <mikoOS/string.h>

struct blk_device_drivers blk_drivers_head = {
	.op = NULL,
	.next = &blk_drivers_head,
};
	
/**
 * Regist block driver operation to the list.
 * @param op
 */
void register_blk_driver(struct blk_dev_driver_operations *op)
{
	struct blk_device_drivers *p;

	p = kmalloc(sizeof(*p));
	if (!p)
		KERN_ABORT("kmalloc failed");
	
	p->op = op;
	p->next = blk_drivers_head.next;
	blk_drivers_head.next = p;

}

/**
 * Remove block driver operation from the list.
 * @param op should be removed from the list.
 */
void remove_blk_driver(struct blk_dev_driver_operations *op)
{
	struct blk_device_drivers *p, *q;

	for (p = blk_drivers_head.next; p != &blk_drivers_head; p = q) {
		if (p->op != op) {
			q = p;
		} else {
			q = p->next;
			kfree(p);
		}
	}
}

// for debug.
void show_all_registered_driver(void)
{
	struct blk_device_drivers *p;

	for (p = blk_drivers_head.next; p != &blk_drivers_head; p = p->next)
		printk("registered driver [%s]\n", p->op->name);

}

/**
 * Search driver via name.
 * @param name is device driver name.
 * @return non null / null.
 */
struct blk_device_drivers *get_blk_driver(const char *name) 
{
	struct blk_device_drivers *p;

	for (p = blk_drivers_head.next; p != &blk_drivers_head; p = p->next)
		if (!strcmp(p->op->name, name))
			return p;
	return NULL;
}

int read_blocks(block_data_t *out, int count)
{
	return 0;
}

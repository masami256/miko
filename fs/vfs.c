#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/fs.h>

static struct file_system_type fs_type_head = {
	.name = "none",
	.next = NULL,
};

int register_file_system(struct file_system_type *fs_type)
{
	fs_type->next = fs_type_head.next;
	fs_type_head.next = fs_type;

	return 0;
}

void show_all_registered_file_systems(void)
{
	struct file_system_type *p;

	for (p = fs_type_head.next; p; p = p->next)
		printk("fs type = %s\n", p->name);
}


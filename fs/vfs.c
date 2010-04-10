#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/fs.h>
#include <mikoOS/string.h>

static struct file_system_type fs_type_head = {
	.name = "none",
	.next = NULL,
};

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static struct file_system_type *find_file_system_type(const char *name)
{
	struct file_system_type *p;

	for (p = fs_type_head.next; p; p = p->next) 
		if (!strcmp(p->name, name))
			return p;

	return NULL;
}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////
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


int read_super_block(const char *fs_name)
{
	struct file_system_type *p;

	p = find_file_system_type(fs_name);
	if (!p) {
		printk("%s file system hasn't been registered yet\n", fs_name);
		return -1;
	}

	return p->get_sb();
}

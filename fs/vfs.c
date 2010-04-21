#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/vfs.h>
#include <mikoOS/string.h>
#include <mikoOS/block_driver.h>

static struct file_system_type fs_type_head = {
	.name = "none",
	.next = NULL,
};

static struct vfs_mount mount_points_head = {
	.next = &mount_points_head,
};

static struct vfs_mount root_fs;

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static struct vfs_mount *get_mount_point(const char *mount_point)
{
	struct vfs_mount *p;

	for (p = mount_points_head.next; p != &mount_points_head; p = mount_points_head.next) {
		if (!strcmp(p->m_point, mount_point))
			return p;
	}

	return NULL;
}

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

int set_mount_point(const char *name, struct blk_device_drivers *driver) 
{
	root_fs.m_point = name;
	root_fs.blk_op = driver->op;

	root_fs.next = mount_points_head.next;
	mount_points_head.next = &root_fs;
}

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


int read_super_block(const char *fs_name, const char *mount_point)
{
	struct file_system_type *p;
	struct vfs_mount *mnt;

	p = find_file_system_type(fs_name);
	if (!p) {
		printk("%s file system hasn't been registered yet\n", fs_name);
		return -1;
	}

	mnt = get_mount_point(mount_point);
	if (!mnt) {
		printk("there is no mount point for [%s]\n", mount_point);
		return -1;
	}

	printk("mount point is [%s]\n", mnt->m_point);

	return p->get_sb();
}

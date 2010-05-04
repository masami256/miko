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
/**
 * Get vfs_mount structure by mount point name.
 * @param mount_point is mount point name.
 * @return NULL if there is no "mount_point" in mount_points_head.
 */
static struct vfs_mount *get_mount_point(const char *mount_point)
{
	struct vfs_mount *p;

	for (p = mount_points_head.next; p != &mount_points_head; p = p->next)
		if (!strcmp(p->m_point, mount_point))
			return p;

	return NULL;
}

/**
 * To find file system name in file system type list.
 * @return NULL if there is no "mount_point" in fs type list.
 */
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
/**
 * To make releashionship between mount point and device driver.
 * @param name is mount point.
 * @param fs_name is file system name for the mount point.
 * @param driver is the driver for that device.
 */ 
void set_mount_point(const char *name, const char *fs_name, struct blk_device_drivers *driver) 
{
	root_fs.m_point = name;
	root_fs.fs_name = fs_name;
	root_fs.driver = driver;

	root_fs.next = mount_points_head.next;
	mount_points_head.next = &root_fs;
}

/**
 * Regist file system type to the file system type list.
 * @param fs_type is file system name.
 */
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

/**
 * Read super block from required mount point. 
 * @param fs_name is file system name. such as ext2.
 * @param mount_point is mount point.
 * @return 0: succeeded. negative value: something wrong.
 */
int read_super_block(const char *fs_name, const char *mount_point)
{
	struct file_system_type *p;
	struct vfs_mount *point;

	p = find_file_system_type(fs_name);
	if (!p) {
		printk("%s file system hasn't been registered yet\n", fs_name);
		return -1;
	}

	point = get_mount_point(mount_point);
	if (!point) {
		printk("there is no mount point for [%s]\n", mount_point);
		return -1;
	}

	printk("mount point is [%s]\n", point->m_point);

	return p->s_op->get_sb(point);
}

ssize_t vfs_read(const char *fname, char *buf, size_t num)
{
	struct vfs_mount *mpoint;
	struct file_system_type *fs_type;
	char mount_point[2] = { 0 };

	if (!fname)
		return -1;

	// relative path isn't supported yet.
	if (fname[0] != '/')
		return -2;

	mount_point[0] = fname[0];
	mpoint = get_mount_point(mount_point);
	if (!mpoint) {
		printk("there is no mount point for [%s]\n", mount_point);
		return -1;
	}

	fs_type = find_file_system_type("minix");
	if (!fs_type) {
		printk("%s file system hasn't been registered yet\n", "minix");
		return -1;
	}
	
	return fs_type->f_op->read(mpoint, fname, buf, num);

}

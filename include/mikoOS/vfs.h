#ifndef __MIKOOS_VFS_H
#define __MIKOOS_VFS_H 1

#include <mikoOS/kernel.h>

#include <mikoOS/block_driver.h>


struct vfs_mount {
	const char *m_point;
	const char *fs_name;
	const struct blk_device_drivers *driver;
	struct vfs_mount *next;
};

struct file_operations {
	ssize_t (*read) (struct vfs_mount *vmount, const char *fname, char *buf, size_t num);
};

struct super_operations {
	int (*get_sb)(struct vfs_mount *vmount);
};

struct file_system_type {
	const char *name;
	const struct super_operations *s_op;
	const struct file_operations *f_op;
	struct file_system_type *next;
};


int register_file_system(struct file_system_type *fs_type);
void show_all_registered_file_systems(void);
int read_super_block(const char *fs_name, const char *mount_point);
void set_mount_point(const char *name, const char *fs_name, struct blk_device_drivers *driver);

ssize_t vfs_read(const char *fname, char *buf, size_t num);

#endif // __MIKOOS_VFS_H

#ifndef __MIKOOS_VFS_H
#define __MIKOOS_VFS_H 1

#include <mikoOS/kernel.h>
#include <mikoOS/fs/ext2.h>

#include <mikoOS/block_driver.h>

struct vfs_mount {
	const char *m_point;
	const struct blk_device_drivers *driver;
	struct vfs_mount *next;
};

struct file_system_type {
	const char *name;
	int (*get_sb)(struct vfs_mount *vmount);
	struct file_system_type *next;
};

int register_file_system(struct file_system_type *fs_type);
void show_all_registered_file_systems(void);
int read_super_block(const char *fs_name, const char *mount_point);
void set_mount_point(const char *name, struct blk_device_drivers *driver);

#endif // __MIKOOS_VFS_H

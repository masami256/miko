#ifndef __MIKOOS_VFS_H
#define __MIKOOS_VFS_H 1

#include <mikoOS/kernel.h>
#include <mikoOS/fs/ext2.h>

struct file_system_type {
	const char *name;
	int (*get_sb)(void);
	struct file_system_type *next;
};

int register_file_system(struct file_system_type *fs_type);
void show_all_registered_file_systems(void);
int read_super_block(const char *fs_name);

#endif // __MIKOOS_VFS_H

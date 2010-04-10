#ifndef __MIKOOS_FS_H
#define __MIKOOS_FS_H 1

#include <mikoOS/kernel.h>
#include <mikoOS/fs/ext2.h>

struct file_system_type {
	const char *name;
	int (*get_sb)(const char *fs_name);
	struct file_system_type *next;
};

int register_file_system(struct file_system_type *fs_type);
void show_all_registered_file_systems(void);

#endif // __MIKOOS_FS_H

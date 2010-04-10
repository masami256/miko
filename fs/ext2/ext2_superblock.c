#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/mm.h>
#include <mikoOS/abort.h>
#include <mikoOS/vfs.h>
#include <mikoOS/string.h>

#include "ext2fs.h"

static struct ext2_superblock ext2_sb;

static int ext2_get_sb(void);

static struct file_system_type ext2_fs_type = {
	.name = "ext2",
	.get_sb = &ext2_get_sb,
};

static int ext2_get_sb(void)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

/**
 * Register myself to the kernel.
 */
void ext2_fs_init(void)
{
	register_file_system(&ext2_fs_type);
}

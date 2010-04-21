#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/mm.h>
#include <mikoOS/abort.h>
#include <mikoOS/vfs.h>
#include <mikoOS/string.h>

#include "ext2fs.h"

static struct ext2_superblock ext2_sb;

static int ext2_get_sb(struct vfs_mount *vmount);

static struct file_system_type ext2_fs_type = {
	.name = "ext2",
	.get_sb = &ext2_get_sb,
};

static int ext2_get_sb(struct vfs_mount *vmount)
{
	block_data_t sblock;
	int ret;

	printk("%s\n", __FUNCTION__);

	ret = read_one_block(vmount->driver, &sblock);

	{
		int i;
		for (i = 0; i < 32; i++)
			printk("0x%x ", sblock.data[i]);
	}
	return 0;
}

/**
 * Register myself to the kernel.
 */
void ext2_fs_init(void)
{
	register_file_system(&ext2_fs_type);
}

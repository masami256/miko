#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/mm.h>
#include <mikoOS/abort.h>
#include <mikoOS/vfs.h>
#include <mikoOS/string.h>
#include <mikoOS/fs/minixfs.h>

#include "minix_superblock.h"
#include "minix_dentry.h"
#include "minix_inode.h"

static struct minix_superblock minix_sb;

static int minix_get_sb(struct vfs_mount *vmount);

static struct file_system_type minix_fs_type = {
	.name = "minix",
	.get_sb = &minix_get_sb,
};

static int minix_get_sb(struct vfs_mount *vmount)
{
	block_data_t sblock;
	int ret;

	printk("%s\n", __FUNCTION__);

	ret = read_one_block(vmount->driver, &sblock);

	{
		int i;
		for (i = 0; i < 32; i++)
			printk("0x%x ", sblock.data[i] & 0xff);
	}
	return 0;
}

/**
 * Register myself to the kernel.
 */
void minix_fs_init(void)
{
	register_file_system(&minix_fs_type);
}

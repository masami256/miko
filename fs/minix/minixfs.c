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
	memcpy(&minix_sb, sblock.data, sizeof(minix_sb));

	printk(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	printk("Superblock info\n");
	printk("s_ninodes: 0x%x\n", minix_sb.s_ninodes);
	printk("s_nzones:  0x%x\n", minix_sb.s_nzones);
	printk("s_imap_blocks: 0x%x\n", minix_sb.s_imap_blocks);
	printk("s_zmap_blocks: 0x%x\n", minix_sb.s_zmap_blocks);
	printk("s_firstdatazone: 0x%x\n", minix_sb.s_firstdatazone);
	printk("s_log_zone_size: 0x%x\n", minix_sb.s_log_zone_size);
	printk("s_max_size: 0x%x\n", minix_sb.s_max_size);
	printk("s_magic: 0x%x\n", minix_sb.s_magic);
	printk("s_pad: 0x%x\n", minix_sb.s_pad);
	printk("s_zones: 0x%x\n", minix_sb.s_zones);
	printk(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");

	return 0;
}

/**
 * Register myself to the kernel.
 */
void minix_fs_init(void)
{
	register_file_system(&minix_fs_type);
}

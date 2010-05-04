#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/mm.h>
#include <mikoOS/abort.h>
#include <mikoOS/vfs.h>
#include <mikoOS/string.h>
#include <mikoOS/fs/minixfs.h>
#include <mikoOS/kmalloc.h>

#include "minix_superblock.h"
#include "minix_dentry.h"
#include "minix_inode.h"

static struct minix_superblock minix_sb;
static block_data_t sblock;

static int minix_get_sb(struct vfs_mount *vmount);
static int get_file_type(struct minix_inode *inode);
static int read_dentry(struct vfs_mount *vmount, struct minix_dentry *dentry, unsigned long address, unsigned long offset);
static int read_inode(struct vfs_mount *vmount, u_int16_t inode_num, struct minix_inode *inode, unsigned long addr);
static int count_delimita_length(const char *str, char c);
static u_int16_t find_file(struct vfs_mount *vmount, struct minix_superblock *sb, unsigned long address, const char *fname);

static struct file_system_type minix_fs_type = {
	.name = "minix",
	.get_sb = &minix_get_sb,
};

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
/**
 * Get file type from inode structure.
 * @param inode
 * @return file type.
 */
static int get_file_type(struct minix_inode *inode)
{
	u_int16_t mode = inode->i_mode;

	if ((mode & I_TYPE) == I_REGULAR)
		return I_FT_REGULAR;
	else if ((mode & I_TYPE) == I_BLOCK_SPECIAL)
		return I_FT_BLOCK;
	else if ((mode & I_TYPE) == I_DIRECTORY)
		return I_FT_DIR;
	else if ((mode & I_TYPE) == I_CHAR_SPECIAL)
		return I_FT_CHAR;
	else if ((mode & I_TYPE) == I_NAMED_PIPE)
		return I_FT_NAMED_PIPE;

	return I_FT_UNKNOWN;
}

/**
 * read inode.
 * @param mount point.
 * @param inode number.
 * @param inode structure.
 * @param data zone.
 * @param 0 is success.
 */
static int read_inode(struct vfs_mount *vmount, u_int16_t inode_num, 
		       struct minix_inode *inode, unsigned long addr)
{
	int ret;

	ret = read_one_sector(vmount->driver, &sblock, addr);
	if (ret)
		return -1;

	memcpy(inode, sblock.data + ((inode_num - 1) * sizeof(*inode)), sizeof(*inode));
	
	return 0;
}

/**
 * Read directory entry.
 * @param dentry structure to store the result.
 * @param address of data zone.
 * @param offset from data zone.
 * @param 0 is success.
 */ 
static int read_dentry(struct vfs_mount *vmount, struct minix_dentry *dentry, 
			unsigned long address, unsigned long offset)
{
	int ret;

	ret = read_one_sector(vmount->driver, &sblock, address);
	if (ret) {
		printk("read sector error\n");
		return ret;
	}

	// dentry->name is 15 bytes which reserved for '\0'.
	memcpy(dentry, sblock.data + offset, sizeof(*dentry) - 1);

	return 0;
}

/**
 * Count byte until to find a delimita.
 * @param string.
 * @param delimita
 * @return bytes.
 */
static int count_delimita_length(const char *str, char c)
{
	int i = 0;
	int len = strlen(str);

	for (i = 0; i < len; i++) {
		if (str[i] == c)
			return i;
	}

	return -1;
	
}

/**
 * Find file by file name.
 * @param mount point.
 * @param super block.
 * @param Address of data zone.
 * @param file name.
 * @return if find a file, it returns inode number.
 */ 
static u_int16_t find_file(struct vfs_mount *vmount, struct minix_superblock *sb, unsigned long address, const char *fname)
{
	unsigned long offset = 0;
	struct minix_dentry dentry;
	struct minix_inode inode;
	unsigned long inode_tbl_bass = get_inode_table_address(*sb);
	const char *tmp;
	u_int16_t ret = 0;
	
	int result;
	int len = 0;
	int ftype;
	while (1) {
		// read first entry.
		result = read_dentry(vmount, &dentry, address, offset);
		if (result) 
			KERN_ABORT("read hdd error\n");

		if (dentry.inode == 0)
			break;

		result = read_inode(vmount, dentry.inode, &inode, inode_tbl_bass);
		if (result) 
			KERN_ABORT("read hdd error\n");

		tmp = fname;
		if (tmp[0] == '/') 
			tmp = tmp + 1;

		ftype = get_file_type(&inode); 
		if (ftype == I_FT_DIR) {
			len = count_delimita_length(tmp, '/');
			if (len == -1) {
				if (!strcmp(tmp, dentry.name))
					return dentry.inode;
			} else if (!strncmp(tmp, dentry.name, len)) {
				ret = find_file(vmount, sb, get_data_zone(inode.i_zone[0]), tmp + len);
			} else {
				// if final character is '/', finish searching.
				if (!strcmp(tmp + len, "/"))
					return dentry.inode;
			}
		} else if (ftype == I_FT_REGULAR) {
			if (!strcmp(dentry.name, tmp))
				return dentry.inode;
		}
		if (ret)
			return ret;

		offset += sizeof(dentry) - 1;
	}

	return 0;

}

static int read_file(struct vfs_mount *vmount, struct minix_superblock *sb, const char *fname)
{
	u_int16_t ino;
	struct minix_inode inode;
	unsigned long inode_tbl_bass = get_inode_table_address(*sb);
	char *data;
	int i, ret;

	ino = find_file(vmount, sb, get_first_data_zone(*sb), fname);
	if (!ino) {
		printk("file %s not found\n", fname);
		return 0;
	}

	ret = read_inode(vmount, ino, &inode, inode_tbl_bass);
	if (ret)
		KERN_ABORT("read inode error\n");

	printk("file %s: size is 0x%x\n", fname, inode.i_size);

	ret = read_one_sector(vmount->driver, &sblock, get_data_zone(inode.i_zone[0]));
	if (ret) {
		printk("read sector error\n");
		return ret;
	}

	data = kmalloc(inode.i_size);
	if (!data)
		KERN_ABORT("kmalloc failed\n");

	memcpy(data, sblock.data, inode.i_size);
	printk("file %s's data is %s", fname, data);

//	for (i = 0; i < inode.i_size; i++)
//		printk("0x%x ", data[i]);
	
//	printk("\n");

	return 0;
}

/**
 * Read minix file system's super block.
 * @param mount point.
 * @return 0 is success, negative numbers are fail.
 */ 
static int minix_get_sb(struct vfs_mount *vmount)
{
	int ret;

	printk("%s\n", __FUNCTION__);

	ret = read_one_sector(vmount->driver, &sblock, 0x400 / BLOCK_SIZE);
	memcpy(&minix_sb, sblock.data, sizeof(minix_sb));
#if 0
	cls();
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
#else
	cls();
	//ret = find_file(vmount, &minix_sb, get_first_data_zone(minix_sb), "/dir_a/dir_b/foobar.txt");
	read_file(vmount, &minix_sb, "/dir_a/dir_b/foobar.txt");
#endif
	return 0;
}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////
/**
 * Register myself to the kernel.
 */
void minix_fs_init(void)
{
	register_file_system(&minix_fs_type);
}

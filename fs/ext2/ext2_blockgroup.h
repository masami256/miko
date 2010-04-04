#ifndef __MIKOOS_BLOCKGROUP_H
#define __MIKOOS_BLOCKGROUP_H 1

#inlude <sys/types.h>

// Block group descriptor table.
struct ext2_blockgroup {
	u_int32_t bg_block_bitmap;
	u_int32_t bg_inode_bitmap;
	u_int32_t bg_inode_table;
	u_int16_t bg_free_blocks_count;
	u_int16_t bg_free_inodes_count;
	u_int16_t bg_used_dirs_count;
	u_int16_t bg_pad;
	u_int8_t bg_reserved[12];
};

#endif // __MIKOOS_BLOCKGROUP_H

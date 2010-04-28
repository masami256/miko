#ifndef __MIKOOS_EXT2_DENTRY_H
#define __MIKOOS_EXT2_DENTRY_H

#define EXT2_MAX_NAME_LENGTH 255

// for file_type.
enum {
	EXT2_FT_UNKNOWN = 0, // Unknown File Type
	EXT2_FT_REG_FILE, // Regular File
	EXT2_FT_DIR, // Directory File
	EXT2_FT_CHRDEV, // Character Device
	EXT2_FT_BLKDEV, // Block Device
	EXT2_FT_FIFO, // Buffer File
	EXT2_FT_SOCK, // Socket File
	EXT2_FT_SYMLINK, // Symbolic Link
};

struct ext2_dentry {
	u_int32_t inode;
	u_int16_t rec_len;
	u_int8_t name_len;
	u_int8_t file_type;
	char name[0];
};

// hash versions.
enum {
	DX_HASH_LEGACY = 0, 
	DX_HASH_HALF_MD4,
	DX_HASH_TEA,
};

struct ext2_indexed_dentry {
	u_int32_t hash;
	u_int32_t block;
};

struct ext2_indexed_entry_count_and_limit {
	u_int16_t limit;
	u_int16_t count;
};

#endif // __MIKOOS_EXT2_DENTRY_H 

#ifndef __MIKOOS_EXT2_INODE_H
#define __MIKOOS_EXT2_INODE_H 1

enum {
	EXT2_BAD_INO = 1, // bad blocks inode
	EXT2_ROOT_INO, // root directory inode
	EXT2_ACL_IDX_INO, // ACL index inode 
	EXT2_ACL_DATA_INO, // ACL data inode
	EXT2_BOOT_LOADER_INO, // boot loader inode
	EXT2_UNDEL_DIR_INO, // undelete directory inode
};

/* file format */
#define EXT2_S_IFSOCK 0xC000 // socket
#define EXT2_S_IFLNK 0xA000 // symbolic link
#define EXT2_S_IFREG 0x8000 // regular file
#define EXT2_S_IFBLK 0x6000 // block device
#define EXT2_S_IFDIR 0x4000 // directory
#define EXT2_S_IFCHR 0x2000 // character device
#define EXT2_S_IFIFO 0x1000 // fifo

/* process execution user/group override */

#define EXT2_S_ISUID 0x0800 // Set process User ID
#define EXT2_S_ISGID 0x0400 // Set process Group ID
#define EXT2_S_ISVTX 0x0200 // sticky bit

/* access rights */
#define EXT2_S_IRUSR 0x0100 // user read
#define EXT2_S_IWUSR 0x0080 // user write
#define EXT2_S_IXUSR 0x0040 // user execute
#define EXT2_S_IRGRP 0x0020 // group read
#define EXT2_S_IWGRP 0x0010 // group write
#define EXT2_S_IXGRP 0x0008 // group execute
#define EXT2_S_IROTH 0x0004 // others read
#define EXT2_S_IWOTH 0x0002 // others write
#define EXT2_S_IXOTH 0x0001 //others execute

// i_flags values.
#define EXT2_SECRM_FL 0x00000001 // secure deletion
#define EXT2_UNRM_FL 0x00000002 // record for undelete
#define EXT2_COMPR_FL 0x00000004 // compressed file
#define EXT2_SYNC_FL 0x00000008 // synchronous updates
#define EXT2_IMMUTABLE_FL 0x00000010 // immutable file
#define EXT2_APPEND_FL 0x00000020 // append only
#define EXT2_NODUMP_FL 0x00000040 // do not dump/delete file
#define EXT2_NOATIME_FL 0x00000080 // do not update .i_atime
/* Reserved for compression usage */
#define EXT2_DIRTY_FL 0x00000100 // Dirty (modified)
#define EXT2_COMPRBLK_FL 0x00000200 // compressed blocks
#define EXT2_NOCOMPR_FL 0x00000400 // access raw compressed data
#define EXT2_ECOMPR_FL 0x00000800 // compression error
/* End of compression flags */
#define EXT2_BTREE_FL 0x00010000 // b-tree format directory
#define EXT2_INDEX_FL 0x00010000 // hash indexed directory
#define EXT2_IMAGIC_FL 0x00020000 // AFS directory
#define EXT3_JOURNAL_DATA_FL 0x00040000 // journal file data
#define EXT2_RESERVED_FL 0x80000000 // reserved for ext2 library

// inode table.
struct ext2_inode {
	u_int16_t i_mode;
	u_int16_t i_uid;
	u_int32_t i_size;
	u_int32_t i_atime;
	u_int32_t i_ctime;
	u_int32_t i_mtime;
	u_int32_t i_dtime;
	u_int16_t i_gid;
	u_int32_t i_blocks;
	u_int32_t i_flags;
	u_int32_t i_osd1;
	u_int32_t i_block[15];
	u_int32_t i_generation;
	u_int32_t i_file_acl;
	u_int32_t i_dir_acl;
	u_int32_t i_faddr;
	struct i_osd2 {
		u_int8_t l_i_frag;
		u_int8_t l_i_fsize;
		u_int8_t reserved1[2];
		u_int16_t l_i_uid_high;
		u_int16_t l_i_gid_high;
		u_int8_t reserved2[2];
	} i_osd2;
};

#endif // __MIKOOS_EXT2_INODE_H

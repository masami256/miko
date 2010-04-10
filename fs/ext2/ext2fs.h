#ifndef __MIKOOS_EXT2FS_H
#define __MIKOOS_EXT2FS_H 1

#include <sys/types.h>

// Reference.
// http://www.nongnu.org/ext2-doc/ext2.html
// http://www.freebsd.org/cgi/cvsweb.cgi/src/sys/fs/ext2fs/ext2fs.h?rev=1.1

// values for s_state.
enum {
	EXT2_VALID_FS = 1, // Unmounted cleanly
	EXT2_ERROR_FS, // Errors detected
};

// values for s_errors.
enum {
	EXT2_ERRORS_CONTINUE = 1, // continue as if nothing happened
	EXT2_ERRORS_RO, // remount read-only
	EXT2_ERRORS_PANIC, // cause a kernel panic
};

// values for s_creator_os.
enum {
	EXT2_OS_LINUX = 0, // Linux
	EXT2_OS_HURD, // GNU HURD
	EXT2_OS_MASIX, // MASIX
	EXT2_OS_FREEBSD, // FreeBSD
	EXT2_OS_LITES, // Lites
};

// values for s_rev_level.
enum {
	EXT2_GOOD_OLD_REV = 0, // Revision 0
	EXT2_DYNAMIC_REV, // Revision 1 with variable inode sizes, extended attributes, etc.
};

// for s_def_resuid.
#define	EXT2_DEF_RESUID 0  // Linux

// for s_def_resgid.
#define EXT2_DEF_RESGID 0 // Linux

// for s_first_ino.
#define EXT2_GOOD_OLD_FIRST_INO 11 // fixed.

// for s_inode_size.
#define EXT2_GOOD_OLD_INODE_SIZE 128

// for s_feature_compat.
#define EXT2_FEATURE_COMPAT_DIR_PREALLOC 0x0001 // Block pre-allocation for new directories
#define EXT2_FEATURE_COMPAT_IMAGIC_INODES 0x0002 
#define EXT3_FEATURE_COMPAT_HAS_JOURNAL 0x0004 // An Ext3 journal exists
#define EXT2_FEATURE_COMPAT_EXT_ATTR 0x0008 // Extended inode attributes are present
#define EXT2_FEATURE_COMPAT_RESIZE_INO 0x0010 // Non-standard inode size used
#define EXT2_FEATURE_COMPAT_DIR_INDEX 0x0020 // Directory indexing (HTree)

// for s_feature_incompat.
#define EXT2_FEATURE_INCOMPAT_COMPRESSION 0x0001 // Disk/File compression is used
#define EXT2_FEATURE_INCOMPAT_FILETYPE 0x0002
#define EXT3_FEATURE_INCOMPAT_RECOVER 0x0004
#define EXT3_FEATURE_INCOMPAT_JOURNAL_DEV 0x0008
#define EXT2_FEATURE_INCOMPAT_META_BG 0x0010

// for s_feature_ro_compat.
#define EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER 0x0001 // Sparse Superblock
#define EXT2_FEATURE_RO_COMPAT_LARGE_FILE 0x0002 // Large file support, 64-bit file size
#define EXT2_FEATURE_RO_COMPAT_BTREE_DIR 0x0004 // Binary tree sorted directory files

// for s_algo_bitmap.
enum {
	EXT2_LZV1_ALG = 0, // Binary value of 0x00000001
	EXT2_LZRW3A_ALG, // Binary value of 0x00000002
	EXT2_GZIP_ALG, // Binary value of 0x00000004
	EXT2_BZIP2_ALG, // Binary value of 0x00000008
	EXT2_LZO_ALG, // Binary value of 0x00000010
};

struct ext2_superblock {
	u_int32_t s_inodes_count;
	u_int32_t s_blocks_count;
	u_int32_t s_r_blocks_count;
	u_int32_t s_free_blocks_count;
	u_int32_t s_free_inodes_count;
	u_int32_t s_first_data_block;
	u_int32_t s_log_block_size;
	u_int32_t s_log_frag_size;
	u_int32_t s_frags_per_group;
	u_int32_t s_inodes_per_group;
	u_int32_t s_mtime;
	u_int32_t s_wtime;
	u_int16_t s_mnt_count;
	u_int16_t s_max_mnt_count;
	u_int16_t s_magic;
	u_int16_t s_state;
	u_int16_t s_errors;
	u_int16_t s_minor_rev_level;
	u_int32_t s_lastcheck;
	u_int32_t s_checkinterval;
	u_int32_t s_creator_os;
	u_int32_t s_rev_level;
	u_int16_t s_def_resuid;
	u_int16_t s_def_resgid;
	/* EXT2_DYNAMIC_REV Specific */
	u_int32_t s_first_ino;
	u_int16_t s_inode_size;
	u_int16_t s_block_group_nr;
	u_int32_t s_feature_compat;
	u_int32_t s_feature_incompat;
	u_int32_t s_feature_ro_compat;
	u_int8_t s_uuid[16];
	u_int8_t s_volume_name[16];
	u_int8_t s_last_mounted[64];
	u_int32_t s_algo_bitmap;
	/* Performance Hints */
	u_int8_t s_prealloc_blocks;
	u_int8_t s_prealloc_dir_blocks;
	u_int8_t aling[2];
	/* Journaling Support */
	u_int8_t s_journal_uuid[16];
	u_int32_t s_journal_inum;
	u_int32_t s_journal_dev;
	u_int32_t s_last_orphan;
	/* Directory Indexing Support */
	u_int32_t s_hash_seed[4];
	u_int8_t s_def_hash_version;
	u_int8_t paddiing[3];
	/* Other options */
	u_int32_t s_default_mount_options;
	u_int32_t s_first_meta_bg;
	u_int8_t reserved[760];
};

void ext2_fs_init(void);

#endif // __MIKOOS_EXT2FS_H

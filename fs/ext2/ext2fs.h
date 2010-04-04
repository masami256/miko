#ifndef __MIKOOS_EXT2FS_H
#define __MIKOOS_EXT2FS_H 1

#include <sys/types.h>

// Reference.
// http://www.nongnu.org/ext2-doc/ext2.html
// http://www.freebsd.org/cgi/cvsweb.cgi/src/sys/fs/ext2fs/ext2fs.h?rev=1.1
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

#endif // __MIKOOS_EXT2FS_H

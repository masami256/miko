/**
 * This file supports minix V2 file system.
 */
#ifndef MIKOOS_MINIX_SUPERBLOCK_H
#define MIKOOS_MINIX_SUPERBLOCK_H 1

#include <sys/types.h>

struct minix_superblock {
	u_int16_t s_ninodes;
	u_int16_t s_nzones;
	u_int16_t s_imap_blocks;
	u_int16_t s_zmap_blocks;
	u_int16_t s_firstdatazone;
	u_int16_t s_log_zone_size;
	u_int32_t s_max_size;
	u_int16_t s_magic;
	u_int16_t s_pad;
	u_int32_t s_zones;
} __attribute__((packed));

#define ZONE_SIZE 0x400

#if 0
#define get_first_data_zone(sb) (sb).s_firstdatazone * ZONE_SIZE
#define get_inode_table_address(sb) 0x800 + ((sb).s_imap_blocks * ZONE_SIZE) + ((sb).s_zmap_blocks * ZONE_SIZE)
#define get_data_zone(zone) (zone) * ZONE_SIZE
#else
#define get_first_data_zone(sb) ((sb).s_firstdatazone * ZONE_SIZE) / BLOCK_SIZE
#define get_inode_table_address(sb) (0x800 + ((sb).s_imap_blocks * ZONE_SIZE) + ((sb).s_zmap_blocks * ZONE_SIZE)) / BLOCK_SIZE
#define get_data_zone(zone) ((zone) * ZONE_SIZE) / BLOCK_SIZE
#endif

#endif // MIKOOS_MINIX_SUPERBLOCK_H

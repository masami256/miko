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

#endif // MIKOOS_MINIX_SUPERBLOCK_H

#ifndef MIKOOS_MINIX_INODE_H
#define MIKOOS_MINIX_INODE_H 1

#define NR_I_ZONE 10

struct minix_inode {
	u_int16_t i_mode;
	u_int16_t i_nlinks;
	u_int16_t i_uid;
	u_int16_t i_gid;
	u_int32_t i_size;
	u_int32_t i_atime;
	u_int32_t i_mtime;
	u_int32_t i_ctime;
	u_int32_t i_zone[NR_I_ZONE];
} __attribute__((packed));

#endif // MIKOOS_MINIX_INODE_H

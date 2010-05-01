#ifndef MIKOOS_MINIX_DENTRY_H
#define MIKOOS_MINIX_DENTRY_H 1

#include <sys/types.h>

#define MAX_NAME_LEN 30 + 1

struct minix_dentry {
	u_int16_t inode;
	char name[MAX_NAME_LEN];
} __attribute__((packed));

#endif // MIKOOS_MINIX_DENTRY_H

#ifndef MIKOOS_MINIXFS_H
#define MIKOOS_MINIXFS_H 1

#define I_TYPE          0170000 /* this field gives inode type */
#define I_REGULAR       0100000 /* regular file, not dir or special */
#define I_BLOCK_SPECIAL 0060000 /* block special file */
#define I_DIRECTORY     0040000 /* file is a directory */
#define I_CHAR_SPECIAL  0020000 /* character special file */
#define I_NAMED_PIPE    0010000 /* named pipe (FIFO) */

enum {
	I_FT_UNKNOWN = -1,
	I_FT_REGULAR = 0,
	I_FT_BLOCK,
	I_FT_DIR,
	I_FT_CHAR,
	I_FT_NAMED_PIPE,
};

void minix_fs_init(void);

#endif // MIKOOS_MINIXFS_H

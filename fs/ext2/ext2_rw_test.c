#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>

#include "ext2fs.h"
#include "ext2_inode.h"
#include "ext2_blockgroup.h"
#include "ext2_dentry.h"

int test_ext2_read(void)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

int test_ext2_write(void)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

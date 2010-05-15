#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/block_driver.h>
#include <mikoOS/vfs.h>
#include "mount_root.h"

static struct blk_device_drivers *driver;


int mount_root_fs(void)
{
	char buf[32] = { 0 };
	ssize_t ret = 0;

	driver = get_blk_driver("ATA disk");
	if (!driver) {
		printk("There is no ATA disk driver!\n");
		return -1;
	}

	if (driver->op->open()) {
		printk("Couldn't open disk\n");
		return -1;
	}

	set_mount_point("/", "minix", driver);
	
	read_super_block("minix", "/");

	printk("rootfs mount finished\n");

	ret = vfs_read("/dir_a/dir_b/foobar.txt", buf, sizeof(buf) - 1);
	printk("/dir_a/dir_b/foobar.txt's size is %d bytes and data is %s", ret, buf);

	memset(buf, 0, sizeof(buf));
	ret = vfs_read("/hello", buf, sizeof(buf) - 1);
	printk("Read /hello: %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3]);
 
	return 0;
}

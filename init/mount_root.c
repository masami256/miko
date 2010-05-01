#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/block_driver.h>
#include <mikoOS/vfs.h>
#include "mount_root.h"

static struct blk_device_drivers *driver;


int mount_root_fs(void)
{
	driver = get_blk_driver("ATA disk");
	if (!driver) {
		printk("There is no ATA disk driver!\n");
		return -1;
	}

	if (driver->op->open()) {
		printk("Couldn't open disk\n");
		return -1;
	}

	set_mount_point("/", driver);
	
	read_super_block("minix", "/");

	printk("rootfs mount finished\n");
	return 0;
}

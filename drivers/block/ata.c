#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/pci.h>
#include <mikoOS/block_driver.h>

// driver operations.
static int open_ATA_disk(void);
static int close_ATA_disk(void);

struct pci_device_info ata_inifo[] = {
	{ 0x8086, 0x7010, 1 }, // PIIX3 IDE Interface (Triton II)
};

struct blk_dev_driver_operations ata_dev = {
	.name = "ATA driver",
	.open = &open_ATA_disk,
	.close = &close_ATA_disk,
};

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////

int open_ATA_disk(void)
{
	printk("open_ATA_disk\n");

	return 0;
}

int close_ATA_disk(void)
{
	printk("%s\n", __FUNCTION__);

	remove_blk_driver(&ata_dev);

	return 0;
}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////
void init_ata_disk_driver(void)
{
	printk("%s\n", __FUNCTION__);

	// register myself.
	register_blk_driver(&ata_dev);
}

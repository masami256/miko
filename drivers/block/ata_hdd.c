#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/pci.h>
#include <mikoOS/block_driver.h>
#include <mikoOS/timer.h>
#include <asm/io.h>

#define STATUS_REGISTER 0x01f7
#define ALTERNATE_STATUS_REGISTER 0x03f6
#define DEVICE_HEAD_REGISTER 0x01f6
#define FEATURES_REGISTER 0x01f1
#define SECTOR_COUNT_REGISTER 0x01f2
#define CYLINDER_LOW_REGISTER 0x01f4
#define CYLINDER_HIGH_REGISTER 0x01f5
#define COMMAND_REGISTER 0x01f7
#define DATA_REGISTER 0x01f0
#define ERROR_REGISTER 0x01f1

// driver operations.
static int open_ATA_disk(void);
static int close_ATA_disk(void);

// For find IDE interface.
static struct pci_device_info ata_info[] = {
	{ 0x8086, 0x7010, 1 }, // PIIX3 IDE Interface (Triton II)
};

struct blk_dev_driver_operations ata_dev = {
	.name = "ATA driver",
	.open = &open_ATA_disk,
	.close = &close_ATA_disk,
};

static struct pci_device *this_device;

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static bool find_ata_device(void);
static void set_bus_master_bit(void);
static void get_base_address(void);

static int open_ATA_disk(void)
{
	printk("open_ATA_disk\n");

	return 0;
}

static int close_ATA_disk(void)
{
	printk("%s\n", __FUNCTION__);

	remove_blk_driver(&ata_dev);

	return 0;
}

static bool find_ata_device(void)
{
	int i;
	int size = sizeof(ata_info) / sizeof(ata_info[0]);

	for (i = 0; i < size; i++) {
		this_device = get_pci_device(ata_info[i].vender, ata_info[i].devid, ata_info[i].func);
		if (this_device) {
			printk("Found ATA device %x:%x:%d\n", ata_info[i].vender, ata_info[i].devid, ata_info[i].func);
			return true;
		}
	}
	return false;

}

/**
 * Set bus master bit in command register in CONFIG_DATA.
 */
static void set_bus_master_bit(void)
{
	u_int32_t data;

	data = pci_data_read(this_device, 0x4) | 0x04;
	pci_data_write(this_device, 0x04, data);
}

/**
 *Get base address.
 */
static void get_base_address(void)
{
	int i;

	for (i = 0x10; i <= 0x24; i += 0x04) {
		this_device->base_address = pci_data_read(this_device, i);
		if (this_device->base_address) {
			printk("ATA Base Address is 0x%x(0x%x)\n", this_device->base_address, i);
			break;
		}
	}

}

static bool wait_until_BSY_and_DRQ_are_zero(u_int16_t port)
{
	u_int8_t data = 0xff;
	u_int8_t bsy, drq;
	int i = 0;

	do {
		data = inb(port);
		bsy = (data >> 7) & 0x01;
		drq = (data >> 3) & 0x01;
		i++;
	} while (!(!bsy && !drq) || i < 1000);

	return (bsy == 0 && drq == 0) ? true : false;
}

static bool wait_until_BSY_is_zero(u_int16_t port)
{
	u_int8_t data = 0xff;
	u_int8_t bsy;
	int i = 0;

	do {
		data = inb(port);
		bsy = (data >> 7) & 0x01;
		i++;
	} while (bsy || i < 1000);

	return (bsy == 0) ? true : false;

}

static void set_device_number(void)
{
	u_int8_t data = 0;

	// Device number is zero.
	data = inb((u_int16_t) DEVICE_HEAD_REGISTER);
	data &= 0xf7;

	outb((u_int16_t) DEVICE_HEAD_REGISTER, data);

	// five usec should be enough time to wait.
	wait_loop_usec(5);
}

static bool do_device_secection_protocol(void)
{
	bool ret = false;

	ret = wait_until_BSY_and_DRQ_are_zero(ALTERNATE_STATUS_REGISTER);
	if (ret) {
		set_device_number();
		ret = wait_until_BSY_and_DRQ_are_zero(ALTERNATE_STATUS_REGISTER);
	}

	return ret;
}

static inline void set_cylinder_register(u_int8_t high, u_int8_t low)
{
	outb(CYLINDER_LOW_REGISTER, low);
	outb(CYLINDER_HIGH_REGISTER, high);
}

static inline void set_sector_count_register(u_int8_t data)
{
	outb(SECTOR_COUNT_REGISTER, data);
}

static u_int8_t get_DRDY(void)
{
	u_int8_t data = inb(STATUS_REGISTER);

	return (data >> 6) & 0x01;
}

static inline void write_command(u_int8_t com)
{
	outb(COMMAND_REGISTER, com);
}

static inline bool is_error(u_int8_t data)
{
	return (data & 0x01) == 0 ? false : true;
}

static inline bool is_drq_enable(u_int8_t data)
{
	return (((data >> 3) & 0x01) == 1) ? true : false;
}

static void print_error_register(void)
{
	if (do_device_secection_protocol()) {
		u_int8_t err = inb(ERROR_REGISTER);
		printk("error is 0x%x\n", err);
	}
}

static bool do_identify_device(void)
{
	bool ret = false;
	u_int8_t data;
	char value[512];
	int i, addr;

	do_device_secection_protocol();
	ret = get_DRDY();
	if (ret) {
		set_device_number();

		write_command(0xec);

	read_bsy:
		wait_loop_usec(5);

		if (!wait_until_BSY_is_zero(STATUS_REGISTER)) {
			printk("wait failed\n");
			return false;
		}

		data = inb(ALTERNATE_STATUS_REGISTER);

	read_status_register:
		data = inb(STATUS_REGISTER);

		if (is_error(data)) {
			printk("error occured:0x%x\n", data);
			print_error_register();
			return false;
		}

		if (!is_drq_enable(data)) {
			printk("drq is 0\n");
			goto read_status_register;
		}

		for (i = 0, addr = DATA_REGISTER; i < 10; i++, addr++) {
			printk("read %d sector\n", i);
			value[i] = inb(addr);
		}

	} 

	for (i = 0; i < 10; i++)
		printk("0x%x ", value[i]);
	printk("\n");

	return true;

}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////
bool init_ata_disk_driver(void)
{
	if (!find_ata_device())
		return false;
	
	set_bus_master_bit();

	get_base_address();

	if (!do_device_secection_protocol()) 
		printk("device section protocol is failed\n");

	if (!do_identify_device()) 
		printk("identify device failed\n");

	// register myself.
	register_blk_driver(&ata_dev);

	return true;
}

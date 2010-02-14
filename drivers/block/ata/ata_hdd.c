#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/pci.h>
#include <mikoOS/block_driver.h>
#include <mikoOS/timer.h>
#include <asm/io.h>
#include <mikoOS/string.h>

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
// for PCI.
static bool find_ata_device(void);
static void set_bus_master_bit(void);
static void get_base_address(void);

// for ATA device.
static bool wait_until_BSY_and_DRQ_are_zero(u_int16_t port);
static bool wait_until_BSY_is_zero(u_int16_t port);
static void set_device_number(int device);
static bool do_device_secection_protocol(void);
static inline void set_cylinder_register(u_int8_t high, u_int8_t low);
static inline void set_sector_count_register(u_int8_t data);
static u_int8_t get_DRDY(void);
static inline void write_command(u_int8_t com);
static inline bool is_error(u_int8_t data);
static inline bool is_drq_active(u_int8_t data);
static void print_error_register(void);
static bool is_device_fault(void);
static inline void read_one_sector_data(u_int16_t *buf);
static bool do_identify_device(u_int16_t *buf);
static bool do_soft_reset(int device);
static bool initialize_ata(void);

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

/**
 * Wait sometime until BSY and DRQ bits are zero.
 * @param port which can be STATUS REGISTER or ALTERNATE STATUS REGISTER.
 * @return true if both bits are zero in few usec.
 */
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

/**
 * Wait sometime until BSY  bit is zero.
 * @param port which can be STATUS REGISTER or ALTERNATE STATUS REGISTER.
 * @return true if BSY bit is zero in few usec.
 */
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

/**
 * Set device number to Device/Head register.
 * @param device must be 0 or 1.
 */
static void set_device_number(int device)
{
	u_int8_t data = 0;

	// Device number is zero.
	data = inb((u_int16_t) DEVICE_HEAD_REGISTER);

	if (!device)
		data &= 0xf;
	else
		data |= 0x10;

	outb((u_int16_t) DEVICE_HEAD_REGISTER, data);

	// five usec should be enough time to wait.
	wait_loop_usec(5);
}

/**
 * Device selection protocol.
 * @return true if success.
 */
static bool do_device_secection_protocol(void)
{
	bool ret = false;

	ret = wait_until_BSY_and_DRQ_are_zero(ALTERNATE_STATUS_REGISTER);
	if (ret) {
		set_device_number(0);
		ret = wait_until_BSY_and_DRQ_are_zero(ALTERNATE_STATUS_REGISTER);
	}

	return ret;
}

/**
 * Set data to Cylinder High and Low  registers.
 * @param high data to Cylinder High register.
 * @param log data to Cylinder Low register.
 */
static inline void set_cylinder_register(u_int8_t high, u_int8_t low)
{
	outb(CYLINDER_LOW_REGISTER, low);
	outb(CYLINDER_HIGH_REGISTER, high);
}

/**
 * Set data to Sector Count register.
 * @param data
 */
static inline void set_sector_count_register(u_int8_t data)
{
	outb(SECTOR_COUNT_REGISTER, data);
}

/**
 * Returns DRDY bit which in Status register.
 * @return 1 or 0.
 */
static u_int8_t get_DRDY(void)
{
	u_int8_t data = inb(STATUS_REGISTER);

	return (data >> 6) & 0x01;
}

/**
 * Execute ATA command.
 * @param com is command number.
 */
static inline void write_command(u_int8_t com)
{
	outb(COMMAND_REGISTER, com);
}

/**
 * Check if error bit is active .
 * @return false means "no error".
 */
static inline bool is_error(u_int8_t data)
{
	return (data & 0x01) == 0 ? false : true;
}

/**
 * Check if DRQ bit is active.
 * @return true is this bit is active.
 */
static inline bool is_drq_active(u_int8_t data)
{
	return (((data >> 3) & 0x01) == 1) ? true : false;
}

/**
 * Printing error register data.
 */
static void print_error_register(void)
{
	if (do_device_secection_protocol()) {
		u_int8_t err = inb(ERROR_REGISTER);
		printk("error is 0x%x\n", err);
	}
}

/**
 * Check if Device Fault bit is active.
 * @return true means some error occured.
 */
static bool is_device_fault(void)
{
	u_int8_t data;

	data = inb(STATUS_REGISTER);
	
	return (data >> 5 & 0x01) == 1 ? true : false;
}

/**
 * Reading one sector.
 * @param buf is to store data.
 */
static inline void read_one_sector_data(u_int16_t *buf)
{
	int i, addr;

	for (i = 0, addr = DATA_REGISTER; i < 32; i++, addr += 2)
		buf[i] = inw(addr);

	for (i = 0; i < 32; i++) {
		printk("%x ", buf[i]);
		if (i >= 16 && i % 16 == 0)
			printk("\n");
	}

	printk("\n");
}

/**
 * Execute Identify Device command.
 * @param buf is to store data.
 * @param false is failed Identify Device command.
 */
static bool do_identify_device(u_int16_t *buf)
{
	bool ret = false;
	u_int8_t data;

	do_device_secection_protocol();
	ret = get_DRDY();
	if (ret) {

		write_command(0xec);

//	read_bsy: // unused.
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

		if (!is_drq_active(data)) {
			printk("drq is 0\n");
			goto read_status_register;
		}

		if (is_device_fault()) {
			printk("some error occured\n");
			return false;
		}

		read_one_sector_data(buf);
	} 

	return true;

}

/**
 * Do sotf reset.
 * @param device should be 0(master) or 1(slave).
 * @param false means error occured.
 */
static bool do_soft_reset(int device)
{
	u_int16_t buf[32];

	memset(buf, 0x0, sizeof(buf));

	// Initialize master/slave.
	set_device_number(device);

	// Do DEVICE RESET command. 
	write_command(0x08);

	// Wait sometime after DEVICE RESET.
	wait_loop_usec(5);

	if (!do_identify_device(buf)) {
		printk("identify device failed\n");
		return false;
	}
	
	return true;
}

/**
 * Initialize ATA device when use it.
 */
static bool initialize_ata(void)
{
	bool ret = false;

	ret = do_soft_reset(0);

	return ret;
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

	initialize_ata();

	// register myself.
	register_blk_driver(&ata_dev);

	return true;
}

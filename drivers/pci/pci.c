#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <asm/io.h>
#include <mikoOS/pci.h>
#include <mikoOS/string.h>

// PCI CONFIG_ADDRESS
#define PCI_CONFIG_ADDRESS 0x0cf8

// Config Data register range is 0x0cfc - 0x0cff.
#define CONFIG_DATA_1 0x0cfc
#define CONFIG_DATA_2 0x0cfd
#define CONFIG_DATA_3 0x0cfe
#define CONFIG_DATA_4 0x0cff

#define PCI_BUS_MAX 255
#define PCI_DEVICE_MAX 31
#define PCI_FUNCTION_MAX 7


/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static void finish_access_to_config_data(struct pci_configuration_register *reg);
static void write_pci_config_address(struct pci_configuration_register *reg);

static u_int32_t read_pci_data(struct pci_configuration_register *reg);
static u_int32_t read_pci_reg00(struct pci_configuration_register *reg);
static u_int8_t is_multi_function_dev(struct pci_configuration_register *reg);
static u_int32_t read_pci_class(struct pci_configuration_register *reg);

static u_int32_t find_pci_data(u_int8_t bus, u_int8_t dev);

/**
 * Set ENABLE bit to 0 and write data to CONFIG_ADDRESS.
 */
static void finish_access_to_config_data(struct pci_configuration_register *reg)
{
	reg->enable_bit = 0;
	write_pci_config_address(reg);
}

/**
 * Read CONFIG_DATA.
 * @param reg it should be set bus, device, function and so forth.
 * @return data from CONFIG_DATA.
 */
static u_int32_t read_pci_data(struct pci_configuration_register *reg)
{
	u_int32_t data;

	// Enable bit should be 1 before read PCI_DATA.
	reg->enable_bit = 1;

	// write data to CONFIG_ADDRESS.
	write_pci_config_address(reg);
	
	data = inl(CONFIG_DATA_1);
	finish_access_to_config_data(reg);

	return data;
}

/**
 * Write data to CONFIG_ADDRESS.
 * @param reg it should be set bus, device, function and so forth.
 */
static void write_pci_config_address(struct pci_configuration_register *reg)
{
	u_int32_t data;

	data = (reg->enable_bit << 31) |
		(reg->reserved << 24) | 
		(reg->bus_num << 16) | 
		(reg->dev_num << 11) | 
		(reg->func_num << 8) |
		(reg->reg_num << 2) |
		reg->bit0;

	outl(PCI_CONFIG_ADDRESS, data);	
}

/**
 * Check if this device is multi function or not.
 * @param reg it should be set bus, device, function and so forth.
 * @return 1 if it is a multi function, 0 is not multi function.
 */
static u_int8_t is_multi_function_dev(struct pci_configuration_register *reg)
{
	u_int32_t data = 0;
	
	reg->reg_num = 0x0c;

	data = read_pci_data(reg);

	return ((data & 0x800000) >> 23) & 0x01;
}

/**
 * Read pci class.
 * @param reg it should be set bus, device, function and so forth.
 * @return PCI class.
 */
static u_int32_t read_pci_class(struct pci_configuration_register *reg)
{
	reg->reg_num = 0x08;

	return read_pci_data(reg);
}

/**
 * Read CONFIG_DATA by register 0x00.
 * @param reg it should be set bus, device, function and so forth.
 * @return vendor id and device id.
 */
static u_int32_t read_pci_reg00(struct pci_configuration_register *reg)
{
	reg->reg_num = 0;

	return read_pci_data(reg);
}

/**
 * Find PCI device by bus number and device number.
 * @param bus bus numer.
 * @param dev device number.
 * @return always 0.
 */
static u_int32_t find_pci_data(u_int8_t bus, u_int8_t dev)
{
	u_int32_t data;
	struct pci_configuration_register reg;

	// At first, check function number zero.
	memset(&reg, 0, sizeof(reg));
	reg.bus_num = bus;
	reg.dev_num = dev;

	// get vender id and device id.
	data = read_pci_reg00(&reg);
	if (data != 0xffffffff) {
		u_int32_t class;
		int i;

		// Check all function numbers.
		for (i = 0; i < PCI_FUNCTION_MAX; i++) {
			reg.func_num = i;
			
			data = read_pci_reg00(&reg);
			class = read_pci_class(&reg);
			
			if (class != 0xffffffff) {
				printk("Found Device: Bus %d : Devfn %d : Vender 0x%x : Device 0x%x : func_num %d : Class 0x%x\n", 
				       bus, dev, data & 0xffff, (data >> 16) & 0xffff, i, class);
			}
		} 
	} 
	
	return 0;
}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////
/**
 * Find all PCI devices.
 */
void find_pci_device(void)
{
	int bus, dev;

	printk("start find_pci_device\n");

	for (bus = 0; bus < PCI_BUS_MAX; bus++) {
		for (dev = 0; dev < PCI_DEVICE_MAX; dev++) {
			find_pci_data(bus, dev);
		}
	}
}

	

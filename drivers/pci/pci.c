#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <asm/io.h>
#include <mikoOS/pci.h>

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

static void finish_access_to_config_data(struct pci_configuration_register *reg);
static void write_pci_config_address(struct pci_configuration_register *reg);

static void memset(void *addr, int c, size_t size)
{
	size_t i;
	char *p = addr;

	for (i = 0; i < size; i++)
		p[i] = c;
}

static void finish_access_to_config_data(struct pci_configuration_register *reg)
{
	reg->enable_bit = 0;
	write_pci_config_address(reg);
}

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


static u_int8_t is_multi_function_dev(struct pci_configuration_register *reg)
{
	u_int32_t data = 0;
	
	reg->reg_num = 0x0c;

	data = read_pci_data(reg);

	// FIXME: need to check device correctly.
//	return ((data & 0x800000) >> 23) & 0x01;
	return 1;
}

static u_int32_t read_pci_class(struct pci_configuration_register *reg)
{
	u_int32_t data;

	reg->reg_num = 0x08;

	data = read_pci_data(reg);

	return data;
}


static u_int32_t read_pci_reg00(struct pci_configuration_register *reg)
{
	reg->reg_num = 0;

	return read_pci_data(reg);
}

static u_int32_t find_pci_data(u_int8_t bus, u_int8_t dev)
{
	u_int32_t data, ret;
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

		// Function number 0 can be multi function device.
		// Check if it's multi funtion device.
		ret = is_multi_function_dev(&reg);

		// if it's multi funtion we get its class.
		if (ret) {
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
	}
	
	return 0;
}

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

	

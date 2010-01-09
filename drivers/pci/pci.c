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

static void memset(void *addr, int c, size_t size)
{
	size_t i;
	char *p = addr;

	for (i = 0; i < size; i++)
		p[i] = c;
}

static void write_pci_config_address(u_int8_t bus, u_int8_t dev, u_int8_t enable)
{
	struct pci_configuration_register reg;
	u_int32_t data;

	memset(&reg, 0, sizeof(reg));

	reg.bus_num = bus;
	reg.dev_num = dev;
	reg.enable_bit = enable;

	data = (reg.enable_bit << 31) |
		(reg.reserved << 24) | 
		(reg.bus_num << 16) | 
		(reg.dev_num << 11) | 
		(reg.func_num << 8) |
		(reg.reg_num << 2) |
		reg.bit0;

	outl(PCI_CONFIG_ADDRESS, data);	
}

static u_int32_t read_config_data(u_int8_t bus, u_int8_t dev)
{
	u_int32_t data;

	// write data to CONFIG_ADDRESS.
	write_pci_config_address(bus, dev, 1);
	
	data = inl(CONFIG_DATA_1);

	if (data != 0xffffffff)
		printk("Found Device at Bus 0x%x : Dev 0x%x. CONFIG_DATA is 0x%x\n", bus, dev, data);

	write_pci_config_address(bus, dev, 0);

	return 0;
}

void find_pci_device(void)
{
	int bus, dev;

	printk("start find_pci_device\n");

	for (bus = 0; bus < PCI_BUS_MAX; bus++) {
		for (dev = 0; dev < PCI_DEVICE_MAX; dev++) {
			read_config_data(bus, dev);
		}
	}
}

	

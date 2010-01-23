#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <asm/io.h>
#include <mikoOS/pci.h>
#include <mikoOS/string.h>
#include <mikoOS/kmalloc.h>

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

// This structure represents PCI's CONFIG_ADDRESS register.
struct pci_configuration_register {
	u_int8_t enable_bit;      // 31: enable bit.
	u_int8_t reserved;        // 24-30: reserved.
	u_int8_t bus_num;         // 16-23: bus number.
	u_int8_t dev_num;         // 11-15: device number.
	u_int8_t func_num;        // 8-10: function number.
	u_int8_t reg_num;         // 2-7: regster number.
	u_int8_t bit0;            // 0-1: always 0.
};

// Store PCI device information.
struct pci_device {
	u_int8_t bus;
	u_int8_t devfn;
	u_int8_t vender;
	u_int8_t devid;
	u_int32_t class;
	u_int8_t func;
	struct pci_device *next;
};

static struct pci_device pci_device_head;

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

static bool store_pci_device_to_list(u_int8_t bus, u_int8_t devfn, u_int32_t data, u_int8_t func, u_int32_t class);

static bool 
store_pci_device_to_list(u_int8_t bus, u_int8_t devfn, u_int32_t data, u_int8_t func, u_int32_t class)
{
	struct pci_device *p;

	p = kmalloc(sizeof(*p));
	if (!p)
		return false;

	p->bus = bus;
	p->devfn = devfn;
	p->vender = data & 0xffff;
	p->devid = (data >> 16) & 0xffff;
	p->class = class;
	p->func = func;
	
	p->next = pci_device_head.next;
	pci_device_head.next = p;

	return true;
}


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
	bool b;

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
				b = store_pci_device_to_list(bus, dev, data, i, class);
				if (!b) {
					printk("kmalloc failed %s:%s at %d\n", __FILE__, __FUNCTION__, __LINE__);
					while (1);
				}
			}
		} 
	} 
	
	return 0;
}

static void init_pci_data_list(void)
{
	pci_device_head.next = &pci_device_head;
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

	// setup pci device list structure.
	init_pci_data_list();

	for (bus = 0; bus < PCI_BUS_MAX; bus++) {
		for (dev = 0; dev < PCI_DEVICE_MAX; dev++) {
			find_pci_data(bus, dev);
		}
	}
	show_all_pci_device();
}

/**
 * Printing out all PCI devices which kernel found.
 */
void show_all_pci_device(void)
{
	struct pci_device *p;

	for (p = pci_device_head.next; p != &pci_device_head; p = p->next)
		printk("Found Device: Bus %d : Devfn %d : Vender 0x%x : Device 0x%x : func_num %d : Class 0x%x\n", 
		       p->bus, p->devfn, p->vender, p->devid, p->func, p->class);

}


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
	u_int8_t bus;             // bus number.
	u_int8_t devfn;           // device number.
	u_int8_t func;            // function number.
	// 0x0
	u_int16_t vender;         // vender id.
	u_int16_t devid;          // device id.

	// 0x08
	u_int8_t revid;           // revision id.
	u_int32_t class;          // class code.

	// 0x0c 
	u_int8_t header_type;     // header type.
	u_int8_t multi;           // multi device.
};

struct pci_device_list {
	struct pci_device data;
	struct pci_device_list *next;
};

union pci_bios32 {
	struct {
		u_int8_t sig[4];         // _32_.
		u_int32_t entry;         // entry point.
		u_int8_t rev;            // revision.
		u_int8_t len;            // length.
		u_int8_t checksum;       // checksum.
		u_int8_t reserved[5];    // reserved.
	} fields;
	char data[16];
};

static struct pci_device_list pci_device_head = {
	.next = &pci_device_head,
};

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static inline void finish_access_to_config_data(struct pci_configuration_register *reg);
static inline void write_pci_config_address(struct pci_configuration_register *reg);

static void write_pci_data(struct pci_configuration_register *reg, u_int32_t data);
static u_int32_t read_pci_data(struct pci_configuration_register *reg);

static inline u_int32_t read_pci_reg00(struct pci_configuration_register *reg);
static inline u_int32_t read_pci_class(struct pci_configuration_register *reg);
static inline u_int32_t read_pci_header_type(struct pci_configuration_register *reg);
static u_int32_t find_pci_data(u_int8_t bus, u_int8_t dev);

static bool store_pci_device_to_list(u_int8_t bus, u_int8_t devfn, 
				     u_int32_t data, u_int8_t func, 
				     u_int32_t class, u_int32_t header);

static bool find_pci_bios32(void);

static bool 
store_pci_device_to_list(u_int8_t bus, u_int8_t devfn, 
			 u_int32_t data, u_int8_t func, 
			 u_int32_t class, u_int32_t header)
{
	struct pci_device_list *p;

	p = kmalloc(sizeof(*p));
	if (!p)
		return false;

	p->data.bus = bus;
	p->data.devfn = devfn;
	p->data.vender = data & 0xffff;
	p->data.devid = (data >> 16) & 0xffff;
	p->data.class = class >> 8;
	p->data.func = func;
	p->data.header_type = ((header >> 16) & 0xff) & 0x7f;
	p->data.multi = ((header & 0x800000) >> 23) & 0x01;
	p->next = pci_device_head.next;
	pci_device_head.next = p;

	return true;
}


/**
 * Set ENABLE bit to 0 and write data to CONFIG_ADDRESS.
 */
static inline void finish_access_to_config_data(struct pci_configuration_register *reg)
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
 * Write to CONFIG_DATA.
 * @param reg it should be set bus, device, function and so forth.
 * @param data should be write to CONFIG_DATA
 * @return data from CONFIG_DATA.
 */
static void write_pci_data(struct pci_configuration_register *reg, u_int32_t data)
{
	// Enable bit should be 1 before read PCI_DATA.
	reg->enable_bit = 1;

	// write data to CONFIG_ADDRESS.
	write_pci_config_address(reg);
	
	outl(CONFIG_DATA_1, data);
	finish_access_to_config_data(reg);
}


/**
 * Write data to CONFIG_ADDRESS.
 * @param reg it should be set bus, device, function and so forth.
 */
static inline void write_pci_config_address(struct pci_configuration_register *reg)
{
	u_int32_t data = 0;

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
 * Read pci class.
 * @param reg it should be set bus, device, function and so forth.
 * @return PCI class.
 */
static inline u_int32_t read_pci_class(struct pci_configuration_register *reg)
{
	reg->reg_num = 0x08;

	return read_pci_data(reg);
}

/**
 * Read CONFIG_DATA by register 0x00.
 * @param reg it should be set bus, device, function and so forth.
 * @return vendor id and device id.
 */
static inline u_int32_t read_pci_reg00(struct pci_configuration_register *reg)
{
	reg->reg_num = 0;

	return read_pci_data(reg);
}

/**
 * Read CONFIG_DATA by register 0x0c to check if it's PCI brigdge or not.
 * @param reg it should be set bus, device, function and so forth.
 * @return vendor id and device id.
 */
static inline u_int32_t read_pci_header_type(struct pci_configuration_register *reg)
{
	reg->reg_num = 0x0c;

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
	u_int32_t class;
	u_int32_t header;
	int i;
	struct pci_configuration_register reg;
	bool b;

	// At first, check function number zero.
	memset(&reg, 0, sizeof(reg));
	reg.bus_num = bus;
	reg.dev_num = dev;

	// Check all function numbers.
	for (i = 0; i < PCI_FUNCTION_MAX; i++) {
		reg.func_num = i;		
		data = read_pci_reg00(&reg);
		if (data != 0xffffffff) {
			
			class = read_pci_class(&reg);
			header = read_pci_header_type(&reg);
			
			b = store_pci_device_to_list(bus, dev, data, i, class, header);
			if (!b) {
				printk("kmalloc failed %s:%s at %d\n", __FILE__, __FUNCTION__, __LINE__);
				while (1);
			}
		}
	} 
	
	return 0;
}

/**
 * Find PCI BIOS.
 * @ret bool PCI BIOS is found or not.
 */
static bool find_pci_bios32(void)
{
	unsigned long addr = 0;
	union pci_bios32 *bios32;
	int sum, i;
	int len;

	for (addr = 0xe0000; addr < 0xffff0; addr += 16) {
		bios32 = (union pci_bios32 *) addr;

		if (bios32 != NULL && 
		    bios32->fields.sig[0] == '_' &&
		    bios32->fields.sig[1] == '3' &&
		    bios32->fields.sig[2] == '2' &&
		    bios32->fields.sig[3] == '_') {

			len = bios32->fields.len * 16;
			if (len) {
				for (i = 0, sum = 0; i < len; i++)
					sum += bios32->data[i];
				
				if (!sum) {
					printk("found pci bios32 at 0x%x\n", addr);
					printk("PCI BIOS32 entry point is 0x%x\n", bios32->fields.entry);
					printk("PCI BIOS32 revision is 0x%x\n", bios32->fields.rev);
					return true;
				}
			}
		}
				
	}
	printk("pci bios32 not found\n");
	return false;
 
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

	for (bus = 0; bus < PCI_BUS_MAX; bus++) {
		for (dev = 0; dev < PCI_DEVICE_MAX; dev++)
			find_pci_data(bus, dev);
	}
//	show_all_pci_device();
	find_pci_bios32();
}

/**
 * Printing out all PCI devices which kernel found.
 */
void show_all_pci_device(void)
{
	struct pci_device_list *p;

	for (p = pci_device_head.next; p != &pci_device_head; p = p->next)
		printk("Found Device: Bus %d:Devfn %d:Vender 0x%x:Device 0x%x:func %d:header 0x%x:Class 0x%x:Multi %d\n", 
		       p->data.bus, p->data.devfn, 
		       p->data.vender, p->data.devid, 
		       p->data.func, p->data.header_type,
		       p->data.class, p->data.multi);
}

/**
 * Check PCI device.
 * @param vender id.
 * @param device number.
 * @param function number.
 * @return if found it returns pci device information structure.
 */
struct pci_device *get_pci_device(u_int16_t vender, u_int16_t device, u_int8_t function)
{
	struct pci_device_list *p;

	for (p = pci_device_head.next; p != &pci_device_head; p = p->next) {
		if (p->data.vender == vender &&
		    p->data.devid == device &&
		    p->data.func == function)
			return &p->data;
	}

	return NULL;
}

/**
 * Read data from PCI_DATA.
 * @param pci
 * @param reg_num which register you want to read.
 * @return data from PCI_DATA.
 */
u_int32_t pci_data_read(struct pci_device *pci, u_int8_t reg_num)
{
	struct pci_configuration_register reg;

	memset(&reg, 0, sizeof(reg));

	reg.reg_num = reg_num;
	reg.func_num = pci->func;
	reg.dev_num = pci->devfn;
	reg.bus_num = pci->bus;

	return read_pci_data(&reg);
}

/**
 * Write data to PCI_DATA.
 * @param pci
 * @param reg_num which register you want to write.
 * @param data.
 */
void pci_data_write(struct pci_device *pci, u_int8_t reg_num, u_int32_t data)
{
	struct pci_configuration_register reg;

	memset(&reg, 0, sizeof(reg));

	reg.reg_num = reg_num;
	reg.func_num = pci->func;
	reg.dev_num = pci->devfn;
	reg.bus_num = pci->bus;
	
	write_pci_data(&reg, data);
}

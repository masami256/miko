#ifndef __MIKOOS_PCI_H
#define __MIKOOS_PCI_H 1

// To store  pci device information for each device drivers.
struct pci_device_info {
	u_int16_t vender;
	u_int16_t devid;
	u_int8_t func;
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
	u_int8_t pg_if;           // program interface.
	u_int32_t sub_class;	  // sub class.
	u_int32_t base_class;     // base class.

	// 0x0c 
	u_int8_t header_type;     // header type.
	u_int8_t multi;           // multi device.

	// 0x2c
	u_int16_t sub_vender;     // sub system vender id.
	u_int16_t sub_devid;      // sub system device id.

	// 0x10-0x24
	u_int32_t base_address;   // base address.
};

void find_pci_device(void);
void show_all_pci_device(void);
struct pci_device *get_pci_device(u_int16_t vender, u_int16_t device, u_int8_t function);

void pci_data_write(struct pci_device *pci, u_int8_t reg_num, u_int32_t data);
u_int32_t pci_data_read(struct pci_device *pci, u_int8_t reg_num);


#endif // __MIKOOS_PCI_H

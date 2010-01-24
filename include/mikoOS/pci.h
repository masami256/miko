#ifndef __MIKOOS_PCI_H
#define __MIKOOS_PCI_H 1

// To store  pci device information for each device drivers.
struct pci_device_info {
	u_int16_t vender;
	u_int16_t devid;
	u_int8_t func;
};

void find_pci_device(void);
void show_all_pci_device(void);
struct pci_device *get_pci_device(u_int16_t vender, u_int16_t device, u_int8_t function);

void pci_data_write(struct pci_device *pci, u_int8_t reg_num, u_int32_t data);
u_int32_t pci_data_read(struct pci_device *pci, u_int8_t reg_num);


#endif // __MIKOOS_PCI_H

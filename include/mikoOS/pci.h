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
bool has_pci_device(u_int16_t vender, u_int16_t device, u_int8_t function);

#endif // __MIKOOS_PCI_H

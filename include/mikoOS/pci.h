#ifndef __MIKOOS_PCI_H
#define __MIKOOS_PCI_H 1

// This structure represents PCI's CONFIG_ADDRESS register.
struct pci_configuration_register {
	u_int8_t enable_bit:1;      // 31: enable bit.
	u_int8_t reserved:7;        // 24-30: reserved.
	u_int8_t bus_num:8;         // 16-23: bus number.
	u_int8_t dev_num:5;         // 11-15: device number.
	u_int8_t func_num:3;        // 8-10: function number.
	u_int8_t reg_num:6;         // 2-7: regster number.
	u_int8_t bit0:2;            // 0-1: always 0.
} __attribute__((packed));
	
void find_pci_device(void);

#endif // __MIKOOS_PCI_H

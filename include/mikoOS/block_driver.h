#ifndef __MIKOOS_BLOCK_DRIVER_H
#define __MIKOOS_BLOCK_DRIVER_H 1

#include <sys/types.h>

struct blk_dev_driver_operations {
	char name[32]; // driver name.
	int (*open)(void);
	int (*close)(void);
	int (*read)(int device, u_int32_t sector, 
		    sector_t *buf, size_t buf_size);
	int (*write)(int device, u_int32_t sector,
		     sector_t *buf, size_t buf_size);
	int (*ioctl)(void); // temporary definition.
	int (*scattered_io)(void); // temporary definition.
};

struct blk_device_drivers {
	struct blk_dev_driver_operations *op;
	struct blk_device_drivers *next;
};

void register_blk_driver(struct blk_dev_driver_operations *op);
void remove_blk_driver(struct blk_dev_driver_operations *op);
void show_all_registered_driver(void);
struct blk_device_drivers *get_blk_driver(const char *name);

#define read_one_sector(driver, out, sector_num) blk_read_sector((driver), (out), (sector_num), 1)
int blk_read_sector(const struct blk_device_drivers *blk_driver,  block_data_t *out, unsigned long sector_num, int count);

#endif // __MIKOOS_BLOCK_DRIVER_H

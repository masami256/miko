#ifndef __MIKOIS_ATA_H
#define __MIKOIS_ATA_H 1

#include <sys/types.h>

#define SECTOR_SIZE 256
typedef u_int16_t sector_t;

bool init_ata_disk_driver(void);

#endif // __MIKOOS_BLOCK_DRIVER_H

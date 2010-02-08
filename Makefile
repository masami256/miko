include ./Makefile.inc

LDFLAGS = -T linker.ld
LDFLAGS += -m elf_i386

MOUNTPATH = /mnt

kernel = kernel.img

boot_dir = boot
kern_dir = kernel
kern_mm_dir = $(kern_dir)/mm

drivers_dir = drivers
driver_pci_dir = $(drivers_dir)/pci
driver_blk_dir = $(drivers_dir)/block

klib_dir = klibs

objs = $(boot_dir)/head.o \
	$(kern_dir)/cmain.o \
	$(kern_dir)/gdt.o \
	$(kern_dir)/interrupt_handler.o \
	$(kern_dir)/interrupt.o \
	$(kern_dir)/intr_gate.o \
	$(kern_mm_dir)/mm.o \
	$(driver_pci_dir)/pci.o \
	$(driver_blk_dir)/block_driver.o \
	$(driver_blk_dir)/ata.o \
	$(klib_dir)/printk.o \
	$(klib_dir)/string.o \
	$(klib_dir)/kmalloc.o \
	$(klib_dir)/stdlib.o	

all: build $(objs)
	$(LD) $(LDFLAGS) $(objs) -o $(kernel) 

build:
	cd $(boot_dir) && make
	cd $(kern_mm_dir) && make
	cd $(kern_dir) && make
	cd $(driver_pci_dir) && make
	cd $(driver_blk_dir) && make
	cd $(klib_dir) && make

install: loop_dev_install
test: bochs_test

loop_dev_install:
	sudo /sbin/losetup /dev/loop1 test/img/floppy.img
	sudo mount /dev/loop1 $(MOUNTPATH)
	sudo cp $(kernel) $(MOUNTPATH)/$(kernel)
	sudo umount /dev/loop1
	sudo /sbin/losetup -d /dev/loop1

bochs_test:
	cd test && bochs -f .bochsrc 2>&1 | tee test.log&

qemu_test:
	kvm -boot a -fda test/img/floppy.img -hda test/img/hda.img&

clean:
	cd $(boot_dir) && make clean 
	cd $(kern_dir) && make clean
	cd $(kern_mm_dir) && make clean
	cd $(driver_pci_dir) && make clean
	cd $(driver_blk_dir) && make clean
	cd $(klib_dir) && make clean
	-rm -f $(kernel) *.log
	find . -name '*~' -exec rm {} \; 

.PHONY: clean all install test

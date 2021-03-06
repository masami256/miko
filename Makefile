include ./Makefile.inc

LDFLAGS = -T linker.ld
LDFLAGS += -m elf_i386

MOUNTPATH = /mnt

kernel = kernel.img

boot_dir = boot
kern_dir = kernel
kern_mm_dir = $(kern_dir)/mm

init_dir = init

drivers_dir = drivers
driver_pci_dir = $(drivers_dir)/pci
driver_blk_dir = $(drivers_dir)/block

fs_dir = fs
ext2_fs_dir = $(fs_dir)/ext2
minix_fs_dir = $(fs_dir)/minix

klib_dir = klibs

objs = $(boot_dir)/head.o \
	$(init_dir)/cmain.o \
	$(init_dir)/mount_root.o \
	$(kern_dir)/gdt.o \
	$(kern_dir)/interrupt_handler.o \
	$(kern_dir)/interrupt.o \
	$(kern_dir)/intr_gate.o \
	$(kern_dir)/timer.o \
	$(kern_dir)/process.o \
	$(kern_dir)/syscall.o \
	$(kern_dir)/syscall_table.o \
	$(kern_dir)/elf.o \
	$(kern_mm_dir)/mm.o \
	$(driver_pci_dir)/pci.o \
	$(driver_blk_dir)/block_driver.o \
	$(driver_blk_dir)/ata/ata_hdd.o \
	$(fs_dir)/vfs.o \
	$(ext2_fs_dir)/ext2_superblock.o \
	$(minix_fs_dir)/minixfs.o \
	$(klib_dir)/printk.o \
	$(klib_dir)/string.o \
	$(klib_dir)/kmalloc.o \
	$(klib_dir)/stdlib.o \
	$(klib_dir)/abort.o 

all: build $(objs)
	$(LD) $(LDFLAGS) $(objs) -o $(kernel) 

build:
	cd $(boot_dir) && make
	cd $(init_dir) && make
	cd $(kern_mm_dir) && make
	cd $(kern_dir) && make
	cd $(driver_pci_dir) && make
	cd $(driver_blk_dir) && make
	cd $(fs_dir) && make
	cd $(ext2_fs_dir) && make
	cd $(minix_fs_dir) && make
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
	cd test && bochs -q -f .bochsrc 2>&1 | tee test.log&

qemu_test:
	qemu-kvm -boot a -fda test/img/floppy.img -hda test/img/hda.img&

create_test_data:
	sudo mount test/img/hda.img /media/test -o loop
	mkdir -p /media/test/dir_a/dir_b /media/test/dir_A/dir_B
	echo "foobar" > /media/test/dir_a/dir_b/foobar.txt 
	echo "ABCDE" > /media/test/test.txt
	cd test/test_program/ && make && cp hello /media/test/.
	sudo umount /media/test 

clean:
	cd $(boot_dir) && make clean 
	cd $(init_dir) && make clean
	cd $(kern_dir) && make clean
	cd $(kern_mm_dir) && make clean
	cd $(driver_pci_dir) && make clean
	cd $(driver_blk_dir) && make clean
	cd $(fs) && make clean
	cd $(ext2_fs_dir) && make clean
	cd $(minix_fs_dir) && make
	cd $(klib_dir) && make clean
	cd test/test_program && make clean
	-rm -f $(kernel) *.log
	find . -name '*~' -exec rm {} \; 

.PHONY: clean all install test

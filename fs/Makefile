include ../Makefile.inc

CFLAGS += -I../include -I.

ext2_dir = ext2
minix_dir = minix

objs = vfs.o \
	$(ext2_dir)/ext2_superblock.o \
	$(minix_dir)/minixfs.o


all: $(objs)
	cd $(ext2_dir) && make
	cd $(minix_dir) && make
.c.o:
	$(CC) $(CFLAGS) $< -c

.S.o:
	$(CC) $(CFLAGS) $< -c

clean:
	cd $(ext2_dir) && make clean
	cd $(minix_dir) && make clean
	-rm *.o  *~

.PHONY: clean all

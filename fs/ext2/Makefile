include ../../Makefile.inc

CFLAGS += -I../../include -I.

objs = ext2_superblock.o \
	ext2_rw_test.o

all: $(objs)
	
.c.o:
	$(CC) $(CFLAGS) $< -c

.S.o:
	$(CC) $(CFLAGS) $< -c

clean:
	-rm *.o  *~

.PHONY: clean all

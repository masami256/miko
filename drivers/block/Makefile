include ../../Makefile.inc

CFLAGS += -I../../include -I.

objs = block_driver.o \
	ata/ata_hdd.o

all: $(objs)
	cd ata && make
.c.o:
	$(CC) $(CFLAGS) $< -c

.S.o:
	$(CC) $(CFLAGS) $< -c

clean:
	-rm *.o  *~
	cd ata && make clean

.PHONY: clean all

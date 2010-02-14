include ../Makefile.inc

CFLAGS += -I../include -I.

all:
	cd pci && make
	cd block && make
	cd block/ata && make

clean:
	cd pci && make clean
	cd block && make clean
	cd block/ata && make clean
	-rm *.o  *~

.PHONY: clean all

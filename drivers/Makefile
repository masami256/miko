include ../Makefile.inc

CFLAGS += -I../include -I.

all:
	cd pci && make
	cd block && make

clean:
	cd pci && make clean
	cd block && make clean
	-rm *.o  *~

.PHONY: clean all

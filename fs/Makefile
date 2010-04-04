include ../Makefile.inc

CFLAGS += -I../include -I.

all:
	cd ext2 && make

clean:
	cd ext2 && make clean
	-rm *.o  *~

.PHONY: clean all

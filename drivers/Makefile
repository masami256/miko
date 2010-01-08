include ../Makefile.inc

CFLAGS += -I../include -I.

objs = block/disk_driver.o

all: $(objs)
	cd block && make
.c.o:
	$(CC) $(CFLAGS) $< -c

.S.o:
	$(CC) $(CFLAGS) $< -c

clean:
	cd block && make clean
	-rm *.o  *~

.PHONY: clean all

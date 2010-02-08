include ../Makefile.inc

CFLAGS += -I../include  -I.

objs = string.o \
	printk.o \
	kmalloc.o \
	stdlib.o

all: $(objs)

.c.o:
	$(CC) $(CFLAGS) $< -c

.S.o:
	$(CC) $(CFLAGS) $< -c

clean:
	-rm *.o  *~

.PHONY: clean all

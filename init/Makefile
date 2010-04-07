include ../Makefile.inc

CFLAGS += -I../include -I.

objs = cmain.o \
	mount_root.o

all: $(objs)

.c.o:
	$(CC) $(CFLAGS) $< -c

.S.o:
	$(CC) $(CFLAGS) $< -c

clean:
	-rm *.o  *~

.PHONY: clean all

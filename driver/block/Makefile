include ../../Makefile.inc

CFLAGS += -I../../include -I.

objs = disk_driver.o

all: $(objs)
	
.c.o:
	$(CC) $(CFLAGS) $< -c

.S.o:
	$(CC) $(CFLAGS) $< -c

clean:
	-rm *.o  *~

.PHONY: clean all

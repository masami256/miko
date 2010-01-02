include ../Makefile.inc

CFLAGS += -I../include -I.

objs = head.o

all: $(objs)

.S.o:
	$(CC) $(CFLAGS) $< -c

clean:
	-rm *.o $(kernel) *~

.PHONY: clean all

include ../Makefile.inc

CFLAGS += -I../include -I.

objs = gdt.o \
	intr_gate.o \
	interrupt_handler.o \
	interrupt.o \
	timer.o \
	mm/mm.o \
	process.o \
	syscall.o \
	syscall_table.o \
	elf.o

all: $(objs)
	cd mm && make
.c.o:
	$(CC) $(CFLAGS) $< -c

.S.o:
	$(CC) $(CFLAGS) $< -c

clean:
	-rm *.o  *~

.PHONY: clean all

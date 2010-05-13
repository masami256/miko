CC = gcc
LD = ld

target = hello

CFLAGS = -Wall -Wextra -fno-builtin
CFLAGS += -fno-common -nostdlib -nostdinc -nostartfiles -nodefaultlibs 
CFLAGS += -m32
CFLAGS += -mno-red-zone -ffreestanding -fno-stack-protector

LDFLAGS = -m elf_i386 -Bstatic --entry=main

objs = $(target).o

$(target): $(objs)

	$(LD) $(LDFLAGS) $(objs) -o $(target)

.c.o:
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f *~ $(target) $(objs)


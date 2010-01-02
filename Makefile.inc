# common definitions
CC = gcc
LD = ld

CFLAGS = -Wall -Wextra -fno-builtin
CFLAGS += -fno-common -nostdlib -nostdinc -nostartfiles -nodefaultlibs 
CFLAGS += -m32
CFLAGS += -mno-red-zone -ffreestanding -fno-stack-protector


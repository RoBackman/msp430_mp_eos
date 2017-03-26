OBJECTS=main.o sheduler.o led.o uart.o trafic.o ports.o

GCC_DIR = C:/ti/msp430_gcc/bin
SUPPORT_FILE_DIRECTORY = $(GCC_DIR)/../include

DEVICE  = msp430g2553
CC      = $(GCC_DIR)/msp430-elf-gcc
GDB     = $(GCC_DIR)/msp430-elf-gdb

CFLAGS = -I $(SUPPORT_FILE_DIRECTORY) -mmcu=$(DEVICE) -O2 -g
LFLAGS = -L $(SUPPORT_FILE_DIRECTORY)

all: ${OBJECTS}
	$(CC) $(CFLAGS) $(LFLAGS) $? -o master.out

debug: all
	$(GDB) $(DEVICE).out
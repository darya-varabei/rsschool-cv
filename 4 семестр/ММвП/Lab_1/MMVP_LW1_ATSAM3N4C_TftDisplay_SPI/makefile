# Target file name (without extension)
TARGET=MMVP_LW1_ATSAM3N4C_TftDisplay_SPI


# Define commands
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
SIZE=arm-none-eabi-size
OBJDUMP=arm-none-eabi-objdump
LDSCRIPT=flash.ld
REMOVE=	rm -f
COPY= cp -f
MOVE= mv -f
REMOVEDIR = rmdir
MAKEDIR= mkdir
CM3_DIR=../cm3
CM3_BOOT_DIR=../cm3/boot
CM3_UTL_DIR=../cm3/utility
CM3_SPL_DIR=../cm3/SPL

LINKER_FLAGS=-nostartfiles -Xlinker -o$(TARGET).elf -Xlinker -M -Xlinker -Map=$(TARGET).map

DEBUG=-g
OPTIM=-O0
M_CPU=cortex-m3

CFLAGS=$(DEBUG) -I. \
	-I $(CM3_DIR)/include \
	-I $(CM3_BOOT_DIR)/inc \
	-I $(CM3_UTL_DIR)/inc \
	-I $(CM3_SPL_DIR)/inc \
	-D __SAM3N4C__ \
	-mthumb -mcpu=$(M_CPU) $(OPTIM) \
	-T $(LDSCRIPT) -ggdb

SOURCE=startup_sam3n.c \
	main.c \
	newlib_stubs.c \
	image.c \
	$(CM3_SPL_DIR)/src/pmc.c \
	$(CM3_SPL_DIR)/src/pio.c \
	$(CM3_SPL_DIR)/src/spi.c


OBJS = $(SOURCE:.c=.o)

all: begin Debug $(TARGET).bin size finished move end
$(TARGET).bin : $(TARGET).elf
	$(OBJCOPY) $(TARGET).elf -O ihex $(TARGET).hex
$(TARGET).elf : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LINKER_FLAGS)
$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
$(TARGET).lst : $(TARGET).elf
	$(OBJDUMP) -h -S -C -D -t $(TARGET).elf
size:
	 $(SIZE) --format=berkeley $(TARGET).elf

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_MOVE = ------  Moving output files into Debug folder  ------
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
begin:
	@echo
	@echo $(MSG_BEGIN)
finished:
	@echo $(MSG_ERRORS_NONE)
move: Debug
	@echo
	@echo $(MSG_MOVE)
	@echo
	$(MOVE) $(OBJS) Debug
	$(MOVE) $(TARGET).elf Debug
	$(MOVE) $(TARGET).hex Debug
	$(MOVE) $(TARGET).map Debug
end:
	@echo $(MSG_END)
	@echo
Debug:
	$(MAKEDIR) Debug
clean : begin clean_list end
clean_list :
	touch Makefile
	@echo $(MSG_CLEANING)
	$(REMOVE) $(OBJS)
	$(REMOVE) *.lst
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) Debug/*
	$(REMOVEDIR) Debug

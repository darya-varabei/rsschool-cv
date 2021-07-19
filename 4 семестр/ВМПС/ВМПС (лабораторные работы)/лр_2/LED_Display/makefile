# Target file name (without extension)
TARGET=LED_Display

# Define commands
CC=avr-gcc
OBJCOPY=avr-objcopy
SIZE = avr-size
REMOVE= rm -f
COPY= cp -f
MOVE= mv -f
REMOVEDIR = rmdir
MAKEDIR= mkdir

DEBUG=-gdwarf-2
OPTIM=-O0
M_CPU=atmega128

LINKER_FLAGS= -I. -Wl,-Map,$(TARGET).map -mmcu=$(M_CPU)

CFLAGS= -I. \
	-D _AVR_IO_H_ \
	-mmcu=$(M_CPU) -Wall $(OPTIM) $(DEBUG)  \
	-std=gnu99 \
	-funsigned-char -funsigned-bitfields -fpack-struct \
	-fshort-enums -MMD -MP

SOURCE= main.c

OBJS = $(SOURCE:.c=.o)
all: begin Debug $(TARGET).bin size finished move end
$(TARGET).bin : $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $(TARGET).elf $(TARGET).hex
$(TARGET).elf : $(OBJS)
	$(CC) $(LINKER_FLAGS) -o $(TARGET).elf $(OBJS)
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<
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
	$(MOVE) *.d Debug
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
	$(REMOVE) *.d
	$(REMOVE) $(OBJS)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) Debug/*
	$(REMOVEDIR) Debug
	
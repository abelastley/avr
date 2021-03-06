###############################################################################
# Generic makefile for atmel 8 bit avr project v1.01
# new in v0.91: fuse definitions
# new in v1.00: create scratch/env.txt which is used by flasher SW
# new in v1.01: 
###############################################################################

## User parameters (change these as appropriate)

PROJECT = bike


MCU = attiny13a
#MAK_F_CPU = 9600000
MAK_F_CPU = 128000

#MCU = attiny45
#MAK_F_CPU = 8000000


MAK_CLOCKDIVIDE8 = 0
ifeq ($(MAK_F_CPU),16000000)
	MAK_STARTUPDELAY = 69
else
	MAK_STARTUPDELAY = 64
endif

#MAK_BROWNOUT = 18
MAK_BROWNOUT = 0



MAK_SELF_PROGRAM_ENABLE	= FALSE
MAK_DEBUG_WIRE_ENABLE	= FALSE


## General Flags
TARGET = $(PROJECT).elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99  -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -DF_CPU=$(MAK_F_CPU)UL -DCLOCKDIVIDE8=$(MAK_CLOCKDIVIDE8) -DSTARTUPDELAY=$(MAK_STARTUPDELAY) \
	-DBROWNOUT=$(MAK_BROWNOUT) -DMAK_SELF_PROGRAM_ENABLE=$(MAK_SELF_PROGRAM_ENABLE) \
	-DMAK_DEBUG_WIRE_ENABLE=$(MAK_DEBUG_WIRE_ENABLE)
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d -IC:/atmel/src/myUtils



## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=$(PROJECT).map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = $(PROJECT).o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss size
	@mkdir scratch
	@printf "MYAVR $(MCU)\n" > scratch/env.txt
	@printf "MYFREQ $(MAK_F_CPU)\n" >> scratch/env.txt
	@printf "MYCLKDIVIDE $(MAK_CLOCKDIVIDE8)\n" >> scratch/env.txt
	@printf "Project $(PROJECT)\n" >> scratch/env.txt

	
## Compile
$(PROJECT).o: $(PROJECT).c
	$(CC) $(INCLUDES) $(CFLAGS) -c -ffreestanding $<


##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss $(PROJECT).map scratch/*
	-rmdir scratch


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

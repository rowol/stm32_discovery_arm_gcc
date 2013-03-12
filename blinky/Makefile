# put your source files here, make should handle the rest!
SRCS=main.c system_stm32f4xx.c

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJ_NAME=main

# Put your stlink folder here so make burn will work.
STLINK=/home/rowol/stlink.git

# Put your STM32F4 library code directory here
STM_COMMON=/home/rowol/stm32_discovery_arm_gcc.git/STM32F4-Discovery_FW_V1.1.0

# Normally you shouldn't need to change anything below this line!

#######################################################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS  = -g -O2 -Wall -Tstm32_flash.ld 
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -I.

# Include files from STM libraries
CFLAGS += -I$(STM_COMMON)/Utilities/STM32F4-Discovery
CFLAGS += -I$(STM_COMMON)/src/CMSIS/Include -I$(STM_COMMON)/src/CMSIS/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/src/STM32F4xx_StdPeriph_Driver/inc


# add startup file to build
SRCS += $(STM_COMMON)/src/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s 
OBJS = $(SRCS:.c=.o)


.PHONY: proj

all: proj

proj: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -L../STM32F4-Discovery_FW_V1.1.0/lib -lstm32f4
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin


# Flash the STM32F4
burn: proj
	$(STLINK)/st-flash write $(PROJ_NAME).bin 0x8000000


# Source files 
SRCS=main.c system_stm32f4xx.c 

# Binary will be generated with this name (.elf, etc)
PROJ_NAME=hello_world

# Put your STM32F4 library code directory here
STM_COMMON=../STM32F4-Discovery_FW_V1.1.0


# Normally you shouldn't need to change anything below this line!
#######################################################################################

CC=arm-none-eabi-gcc
SIZE=arm-none-eabi-size

#Debug
CFLAGS  = -ggdb -O0 -Wall -Tstm32_flash.ld 

CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -I.

# Include files from STM libraries
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Include -I$(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Include

# add startup file to build
SRCS += $(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s 
OBJS = $(SRCS:.c=.o)

SEMIHOSTING_FLAGS = --specs=rdimon.specs -lc -lrdimon 



.PHONY: all

all: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(SEMIHOSTING_FLAGS) $(CFLAGS) $^ -o $@    
	$(SIZE) $@    

clean:
	rm -f $(PROJ_NAME).elf 


#######################################################
# Debugging targets
#######################################################
gdb: all
	arm-none-eabi-gdb $(PROJ_NAME).elf

# Start OpenOCD GDB server (supports semihosting)
openocd: 
	openocd -f board/stm32f4discovery.cfg 


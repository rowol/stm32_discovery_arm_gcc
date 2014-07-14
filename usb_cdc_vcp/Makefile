
# Put your stlink folder here so make burn will work.
STLINK=~/stlink.git

STM_COMMON = ../STM32F4-Discovery_FW_V1.1.0

# Sources
SRCS = main.c usart.c stm32f4xx_it.c system_stm32f4xx.c 

# USB
SRCS += usbd_usr.c usbd_cdc_vcp.c usbd_desc.c usb_bsp.c

# Library code
SRCS += stm32f4xx_exti.c stm32f4xx_gpio.c stm32f4xx_rcc.c stm32f4xx_usart.c misc.c
SRCS += usb_core.c usb_dcd.c usb_dcd_int.c
SRCS += usbd_cdc_core.c usbd_core.c usbd_ioreq.c usbd_req.c

# add startup file to build
SRCS += $(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s 


# Project name
PROJ_NAME=stm32f4_usb_cdc
OUTPATH=build

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
SIZE=arm-none-eabi-size


# Choose debug or release...
CFLAGS = -g -O2           # Normal
#CFLAGS = -ggdb -O0       # RSW - for GDB debugging, disable optimizer

CFLAGS += -Wall -Tstm32_flash.ld
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -nostartfiles -mcpu=cortex-m4
CFLAGS += -msoft-float

###################################################

vpath %.c \
$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/src \
$(STM_COMMON)/Libraries/STM32_USB_OTG_Driver/src \
$(STM_COMMON)/Libraries/STM32_USB_Device_Library/Core/src \
$(STM_COMMON)/Libraries/STM32_USB_Device_Library/Class/cdc/src


# Includes
CFLAGS += -I.
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Include
CFLAGS += -I$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc
CFLAGS += -I$(STM_COMMON)/Libraries/STM32_USB_OTG_Driver/inc
CFLAGS += -I$(STM_COMMON)/Libraries/STM32_USB_Device_Library/Core/inc
CFLAGS += -I$(STM_COMMON)/Libraries/STM32_USB_Device_Library/Class/cdc/inc


OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj dir_tree

all:  dir_tree proj

dir_tree:
	mkdir -p $(OUTPATH)

proj: $(OUTPATH)/$(PROJ_NAME).elf
	$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf

$(OUTPATH)/$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBPATHS) $(LIBS)
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin

clean:
	find . -name \*.o -type f -delete
	find . -name \*.lst -type f -delete
	rm -f $(OUTPATH)/$(PROJ_NAME).elf
	rm -f $(OUTPATH)/$(PROJ_NAME).hex
	rm -f $(OUTPATH)/$(PROJ_NAME).bin

# Flash the STM32F4
burn: proj
	$(STLINK)/st-flash write $(OUTPATH)/$(PROJ_NAME).bin 0x8000000


# Put your stlink folder here so make burn will work.
STLINK=~/stlink.git

SRCS=main.c selftest.c stm32f4xx_it.c system_stm32f4xx.c usb_bsp.c usbd_desc.c usbd_usr.c

# Library modules
SRCS += stm32f4xx_adc.c stm32f4xx_dac.c stm32f4xx_dma.c stm32f4xx_exti.c stm32f4xx_flash.c
SRCS += stm32f4xx_gpio.c stm32f4xx_i2c.c stm32f4xx_rcc.c stm32f4xx_spi.c stm32f4xx_syscfg.c
SRCS += stm32f4xx_tim.c misc.c
SRCS += stm32f4_discovery.c stm32f4_discovery_audio_codec.c stm32f4_discovery_lis302dl.c
SRCS += usbd_core.c usbd_hid_core.c usbd_ioreq.c usbd_req.c
SRCS += usb_core.c usb_dcd.c usb_dcd_int.c

# Binaries will be generated with this name (.elf, .bin, .hex, etc)
PROJ_NAME=demonstration

#######################################################################################

STM_COMMON=../..

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

#CFLAGS  = -g -O2 -Wall -Tstm32_flash.ld 
CFLAGS  = -g -O2 -Tstm32_flash.ld 
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -I.
CFLAGS += -DUSE_OTG_FS_CORE
CFLAGS += -DUSE_USB_OTG_FS

# Include files from STM libraries
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Include 
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc
CFLAGS += -I$(STM_COMMON)/Utilities/STM32F4-Discovery
CFLAGS += -I$(STM_COMMON)/Libraries/STM32_USB_OTG_Driver/inc
CFLAGS += -I$(STM_COMMON)/Libraries/STM32_USB_Device_Library/Core/inc
CFLAGS += -I$(STM_COMMON)/Libraries/STM32_USB_Device_Library/Class/hid/inc


# add startup file to build
SRCS += $(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s 

OBJS = $(SRCS:.c=.o)

vpath %.c $(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/src \
          $(STM_COMMON)/Utilities/STM32F4-Discovery \
          $(STM_COMMON)/Libraries/STM32_USB_Device_Library/Core/src \
          $(STM_COMMON)/Libraries/STM32_USB_Device_Library/Class/hid/src \
          $(STM_COMMON)/Libraries/STM32_USB_OTG_Driver/src


.PHONY: proj

all: proj

proj: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ 
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

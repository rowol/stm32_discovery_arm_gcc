
# Put your stlink folder here so make burn will work.
STLINK=~/stlink.git

SRCS=main.c command.c flash_if.c stm32f4xx_it.c system_stm32f4xx.c usb_bsp.c usbh_usr.c

# Library modules
SRCS += stm32f4xx_exti.c stm32f4xx_flash.c stm32f4xx_gpio.c stm32f4xx_rcc.c 
SRCS += stm32f4xx_syscfg.c stm32f4xx_tim.c misc.c
SRCS += stm32f4_discovery.c
SRCS += usb_core.c usb_hcd.c usb_hcd_int.c
SRCS += usbh_core.c usbh_hcs.c usbh_ioreq.c usbh_msc_bot.c usbh_msc_core.c 
SRCS += usbh_msc_fatfs.c usbh_msc_scsi.c usbh_stdreq.c
SRCS += fattime.c ff.c

# Binaries will be generated with this name (.elf, .bin, .hex, etc)
PROJ_NAME=fw_upgrade

#######################################################################################

STM_COMMON=../../..

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS  = -g -O2 -Wall -Tstm32_flash.ld 
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -I../inc

# Choose fullspeed or highseed (?)
CFLAGS += -DUSE_OTG_FS_CORE -DUSE_USB_OTG_FS
#CFLAGS += -DUSE_OTG_HS_CORE -DUSE_USB_OTG_HS
# Choose fullspeed or highseed (?)

# Include files from STM libraries
CFLAGS += -I$(STM_COMMON)/Utilities/STM32F4-Discovery
CFLAGS += -I$(STM_COMMON)/Utilities/Third_Party/fat_fs/inc
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Include 
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc
CFLAGS += -I$(STM_COMMON)/Libraries/STM32_USB_HOST_Library/Core/inc
CFLAGS += -I$(STM_COMMON)/Libraries/STM32_USB_HOST_Library/Class/MSC/inc
CFLAGS += -I$(STM_COMMON)/Libraries/STM32_USB_OTG_Driver/inc


# add startup file to build
SRCS += $(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s 

OBJS = $(SRCS:.c=.o)

vpath %.c $(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/src \
          $(STM_COMMON)/Libraries/STM32_USB_OTG_Driver/src \
          $(STM_COMMON)/Libraries/STM32_USB_HOST_Library/Core/src \
          $(STM_COMMON)/Libraries/STM32_USB_HOST_Library/Class/MSC/src \
          $(STM_COMMON)/Utilities/STM32F4-Discovery \
          $(STM_COMMON)/Utilities/Third_Party/fat_fs/src

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

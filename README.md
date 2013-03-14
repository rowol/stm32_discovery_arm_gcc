
This repository contains source code for the article, [STM32 Discovery Development on Linux](http://www.wolinlabs.com/blog/linux.stm32.discovery.gcc.html)


Contents:

1. LED blinker example (buildable)
2. STM libraries and headers
3. STM peripheral_examples (buildable)


STM libraries, headers, and examples have been modified to work with GNU Tools for ARM Embedded Processors toolchain and STM32F4 Discovery EVB.   Generally this just involved adding Makefiles and a slightly modified version of the TrueStudio linker file.

STM's approach toward 'library code' is somewhat non-traditional.  Rather than building libraries up front and linking them with your source, STM appears to intend for you to include their individual source files in your code's build.   Some of their source files include local headers and modules (i.e. stm32f4xx_conf.h, stm32f4xx_it.c/h, system_stm32f4xx.c, etc) that you modify store in your source tree.  Because of this, building the library up front wouldn't work properly, as the configuration for some library modules could vary from project to project.






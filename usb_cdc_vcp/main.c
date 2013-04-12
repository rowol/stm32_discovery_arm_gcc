#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "main.h"

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "stm32f4_discovery.h"

// Private variables
volatile uint32_t time_var1, time_var2;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;




//Configure pins and clocks
void hw_init()
{
   GPIO_InitTypeDef  GPIO_InitStructure;



	// ---------- SysTick timer -------- //
	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (true)    // Capture error
         ;
	}

	// ---------- GPIO -------- //
	// GPIOD Periph clock enable,
   // Need to enable GPIOA because that's where the UART pins are.
   // (Some of the USB is also on that port, and usb modules turn it on later...
   //  but anyway, UART started working correctly when I turned clock on first)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);      // The LEDs are on GPIOD


	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN|LED_ORANGE_PIN|LED_RED_PIN|LED_BLUE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);



   // ------------- USART -------------- //
   RCC_APB1PeriphClockCmd(DISCOVERY_COM_CLK, ENABLE);         //USART1+6=APB2, 2-5=APB1
	

   /* Configure USART Tx+Rx as alternate function  */
   GPIO_InitStructure.GPIO_Pin = DISCOVERY_COM_TX_PIN| DISCOVERY_COM_RX_PIN;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            
   GPIO_Init(GPIOA, &GPIO_InitStructure);    //Both signals are on port GPIOA

   /* Connect PXx to USARTx_Tx* + Rx*/
   GPIO_PinAFConfig(DISCOVERY_COM_TX_GPIO_PORT, DISCOVERY_COM_TX_SOURCE, DISCOVERY_COM_TX_AF);
   GPIO_PinAFConfig(DISCOVERY_COM_RX_GPIO_PORT, DISCOVERY_COM_RX_SOURCE, DISCOVERY_COM_RX_AF);


   

   // ------------- USB -------------- //
	USBD_Init(&USB_OTG_dev,
             USB_OTG_FS_CORE_ID,
             &USR_desc,
             &USBD_CDC_cb,
             &USR_cb);
}



/*
 * Called from systick handler
 */
void timing_handler()
{
	if (time_var1) 
		time_var1--;

	time_var2++;
}



/*
 * Delay a number of systick cycles (1ms)
 */
void Delay(volatile uint32_t nCount)
{
	time_var1 = nCount;
	while (time_var1)
      ;
}




int main(void)
{
	hw_init();

   for(;;) {
      GPIO_ToggleBits(LED_GREEN_GPIO_PORT, LED_GREEN_PIN);
		Delay(500);
	}
   return 0;
}



//Something the runtime startup is trying to call this
void _init() 
{
}



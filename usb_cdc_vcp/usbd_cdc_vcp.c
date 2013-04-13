/**
 ******************************************************************************
 * @file    usbd_cdc_vcp.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    22-July-2011
 * @brief   Generic media access Layer.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED 
#pragma     data_alignment = 4 
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_vcp.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery.h"
#include "usart.h"


LINE_CODING g_lc;





/* These are external variables imported from CDC core to be used for IN 
 transfer management. */
extern uint8_t APP_Rx_Buffer[]; /* Write CDC received data in this buffer.
 These data will be sent over USB IN endpoint
 in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in; /* Increment this pointer or roll it back to
 start address when writing received data
 in the buffer APP_Rx_Buffer. */

/* Private function prototypes -----------------------------------------------*/
static uint16_t VCP_Init(void);
static uint16_t VCP_DeInit(void);
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataRx(uint8_t* Buf, uint32_t Len);

CDC_IF_Prop_TypeDef VCP_fops = {VCP_Init, VCP_DeInit, VCP_Ctrl, VCP_DataTx, VCP_DataRx };

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  VCP_Init
 *         Initializes the Media on the STM32
 * @param  None
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
static uint16_t VCP_Init(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;


   //RSW HACK, don't set up the port here, because we don't know what the OS wants for speed
   //settings yet...  do this in VCP_Ctrl instead..

   /* Enable USART Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = DISCOVERY_COM_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

	return USBD_OK;
}



/**
 * @brief  VCP_DeInit
 *         DeInitializes the Media on the STM32
 * @param  None
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
static uint16_t VCP_DeInit(void)
{
   USART_ITConfig(DISCOVERY_COM, USART_IT_RXNE, DISABLE);
   USART_Cmd(DISCOVERY_COM, DISABLE);
   return USBD_OK;
}




/**
 * @brief  VCP_Ctrl
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{
   LINE_CODING* plc = (LINE_CODING*)Buf;

   assert_param(Len>=sizeof(LINE_CODING));

   switch (Cmd) {
       /* Not  needed for this driver, AT modem commands */   
      case SEND_ENCAPSULATED_COMMAND:
      case GET_ENCAPSULATED_RESPONSE:
         break;

      // Not needed for this driver
      case SET_COMM_FEATURE:                  
      case GET_COMM_FEATURE:
      case CLEAR_COMM_FEATURE:
         break;

         
      //Note - hw flow control on UART 1-3 and 6 only
      case SET_LINE_CODING: 
         if (!ust_config(DISCOVERY_COM, plc))
            return USBD_FAIL;
         
         ust_cpy(&g_lc, plc);           //Copy into structure to save for later
         break;
         
         
      case GET_LINE_CODING:
         ust_cpy(plc, &g_lc);
         break;

         
      case SET_CONTROL_LINE_STATE:
         /* Not  needed for this driver */
         //RSW - This tells how to set RTS and DTR
         break;

      case SEND_BREAK:
         /* Not  needed for this driver */
         break;

      default:
         break;
	}

   return USBD_OK;
}




/**
 * @brief  VCP_DataTx
 *         CDC received data to be send over USB IN endpoint are managed in
 *         this function.
 * @param  Buf: Buffer of data to be sent
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
 */
static uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len)
{
	uint32_t i = 0;

   GPIO_ToggleBits(LED_ORANGE_GPIO_PORT, LED_ORANGE_PIN);

   //If no buffer, we're supposed to receive USART, send USB
   if (Buf==NULL) {
      if (g_lc.datatype == 7)
         APP_Rx_Buffer[APP_Rx_ptr_in] = USART_ReceiveData(DISCOVERY_COM) & 0x7F;
      else if (g_lc.datatype == 8)
         APP_Rx_Buffer[APP_Rx_ptr_in] = USART_ReceiveData(DISCOVERY_COM);

      APP_Rx_ptr_in++;
  
      /* To avoid buffer overflow */
      if(APP_Rx_ptr_in == APP_RX_DATA_SIZE)
         APP_Rx_ptr_in = 0;
   }
   else {      //If we were passed a buffer, transmit that
      while (i < Len) {
         APP_Rx_Buffer[APP_Rx_ptr_in] = *(Buf + i);
         APP_Rx_ptr_in++;
         i++;

         /* To avoid buffer overflow */
         if (APP_Rx_ptr_in == APP_RX_DATA_SIZE) 
            APP_Rx_ptr_in = 0;
      }
   }
   
	return USBD_OK;
}



/**
 * @brief  VCP_DataRx
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 *
 *         @note
 *         This function will block any OUT packet reception on USB endpoint
 *         until exiting this function. If you exit this function before transfer
 *         is complete on CDC interface (ie. using DMA controller) it will result
 *         in receiving more data while previous ones are still not sent.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
 */

static uint16_t VCP_DataRx(uint8_t* Buf, uint32_t Len)
{
   //RSW - Receive USB, send USART
   while (Len-- > 0) {
      USART_SendData(DISCOVERY_COM, *Buf++);
      while(USART_GetFlagStatus(DISCOVERY_COM, USART_FLAG_TXE) == RESET)
         ; 
   } 

   GPIO_ToggleBits(LED_RED_GPIO_PORT, LED_RED_PIN);
   return USBD_OK;
}



void DISCOVERY_COM_IRQHandler(void)
{
   GPIO_ToggleBits(LED_BLUE_GPIO_PORT, LED_BLUE_PIN);

   // Send the received data to the PC Host
   if (USART_GetITStatus(DISCOVERY_COM, USART_IT_RXNE) != RESET) 
     VCP_DataTx(0,0);    //Copies RS232 data to USB

   /* If overrun condition occurs, clear the ORE flag and recover communication */
   if (USART_GetFlagStatus(DISCOVERY_COM, USART_FLAG_ORE) != RESET)
      USART_ReceiveData(DISCOVERY_COM);
}

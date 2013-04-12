//
// USART.C
//

#include <stdbool.h>

#include "stm32f4xx_conf.h"
#include "stm32f4_discovery.h"
#include "usart.h"


void ust_cpy(LINE_CODING* plc2, const LINE_CODING* plc1)
{
   plc2->bitrate    = plc1->bitrate;
   plc2->format     = plc1->format;
   plc2->paritytype = plc1->paritytype;
   plc2->datatype   = plc1->datatype; 
}



//Configure the usart based on the LINE_CODING structure settings
//Note - hw flow control on USART 1-3 and 6 only
bool ust_config(USART_TypeDef* pUsart, LINE_CODING* lc)
{
   //RSW - HACK, need to disable USART while doing this?
      
   //RSW - Sets bit rate, number of stop bits, parity, and number of data bits
   USART_InitTypeDef USART_InitStructure;

   //Set baudrate
   USART_InitStructure.USART_BaudRate = lc->bitrate;

         
   //Set word length (STM includes possible parity bit in data bit count, CDC spec doesn't)
   switch (lc->datatype) {
      case 7: USART_InitStructure.USART_WordLength=USART_WordLength_8b; break;
      case 8: USART_InitStructure.USART_WordLength=USART_WordLength_9b; break;

      case 5:      //Not supported
      case 6:      //Not supported
      case 16:     //Not supported
         return false;

      default:
         assert_param(false);
         return false;
   }


   //Set parity 
   switch (lc->paritytype) {
      case 0: USART_InitStructure.USART_Parity=USART_Parity_No;   break;
      case 1: USART_InitStructure.USART_Parity=USART_Parity_Odd;   break;
      case 2: USART_InitStructure.USART_Parity=USART_Parity_Even;  break;
               
      case 3:    //MARK, not supported
      case 4:    //SPACE, not supported
         return false;

      default:
         assert_param(false);
         return false;
   }
         

   //Set stop bits
   switch (lc->format) {
      case 0: USART_InitStructure.USART_StopBits = USART_StopBits_1;   break;
      case 1: USART_InitStructure.USART_StopBits = USART_StopBits_1_5; break;
      case 2: USART_InitStructure.USART_StopBits = USART_StopBits_2;   break;

      default:
         assert_param(false);
         return false;
   }
         

   //Set up the rest
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

   USART_Init(pUsart, &USART_InitStructure);
   USART_ITConfig(pUsart, USART_IT_RXNE, ENABLE);
   USART_Cmd(pUsart, ENABLE);
   return true;
}
         

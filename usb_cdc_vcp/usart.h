//
// USART.H
//

#ifndef USART_H
#define USART_H

#include <stdbool.h>
#include "usbd_cdc_vcp.h"

void ust_cpy(LINE_CODING* lc2, const LINE_CODING* lc1);
bool ust_config(USART_TypeDef* usart, LINE_CODING* lc);

#endif //USART_H

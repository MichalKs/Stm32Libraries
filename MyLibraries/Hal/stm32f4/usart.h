/**
 * @file    uart.h
 * @brief   Controlling UART
 * @date    08.10.2016
 * @author  Michal Ksiezopolski
 * 
 * @verbatim
 * Copyright (c) 2016 Michal Ksiezopolski.
 * All rights reserved. This program and the 
 * accompanying materials are made available 
 * under the terms of the GNU Public License 
 * v3.0 which accompanies this distribution, 
 * and is available at 
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#ifndef UART_H_
#define UART_H_

#include "utils.h"

/**
 * @defgroup  UART UART
 * @brief     UART low level functions
 */

/**
 * @addtogroup UART
 * @{
 */

typedef enum {
  USART_HAL_USART1,
  USART_HAL_USART2,
  USART_HAL_USART3,
  USART_HAL_USART4
} UsartNumber;

#define UART_BUF_LEN_TX 512 ///< Buffer length for UART peripheral

void    UART_Initialize   (UsartNumber usart, int baud, void(*rxCb)(char), int(*txCb)(char*));
Boolean UART_IsSendingData(void);
void    UART_SendDataIrq  (void);
void    UART_EnableIrq    (void);
void    UART_DisableIrq   (void);
/**
 * @}
 */

#endif /* UART_H_ */

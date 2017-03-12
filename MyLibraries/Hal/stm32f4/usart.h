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
} UsartNumber;

#define UART_BUF_LEN_TX 512 ///< Buffer length for UART peripheral

void    Usart_initialize   (UsartNumber usart, int baud, void(*rxCb)(char), int(*txCb)(char*));
Boolean Usart_isSendingData(UsartNumber usart);
void    Usart_sendDataIrq  (UsartNumber usart);
void    Usart_enableIrq    (UsartNumber usart);
void    Usart_disableIrq   (UsartNumber usart);
/**
 * @}
 */

#endif /* UART_H_ */

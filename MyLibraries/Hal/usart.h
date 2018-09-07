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
  USART_HAL_USART2,
  USART_HAL_USART6,
} UsartNumber;

#include "boards.h"

#ifdef BOARD_STM32F4_DISCOVERY
  #define DEBUG_CONSOLE_USART USART_HAL_USART2
#endif
#ifdef BOARD_STM32F7_DISCOVERY
  #define DEBUG_CONSOLE_USART USART_HAL_USART6
#endif

typedef struct {
  char * transmitBuffer;
  int bufferLength;
} UsartTransmission;

typedef struct {
  int baudRate;
  void (*sendDataToUpperLayer)(char receivedCharacter);
  UsartTransmission (*getMoreDataToTransmit)(void);
} UsartHalInitialization;

void    Usart_initialize   (UsartNumber usart, UsartHalInitialization * usartInitialization);
Boolean Usart_isSendingData(UsartNumber usart);
void    Usart_sendDataIrq  (UsartNumber usart);
void    Usart_enableIrq    (UsartNumber usart);
void    Usart_disableIrq   (UsartNumber usart);
/**
 * @}
 */

#endif /* UART_H_ */

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

/**
 * @brief Usart number
 */
typedef enum {
  USART_HAL_USART2 = 0,//!< USART_HAL_USART2
  USART_HAL_USART6 = 1,//!< USART_HAL_USART6
  USART_HAL_EMPTY,
} UsartNumber;

#include "boards.h"

#ifdef BOARD_STM32F4_DISCOVERY
  #define DEBUG_CONSOLE_USART USART_HAL_USART2
#endif
#ifdef BOARD_STM32F7_DISCOVERY
  #define DEBUG_CONSOLE_USART USART_HAL_USART6
#endif

/**
 * @brief Single USART transmission
 */
typedef struct {
  char * transmitBuffer;
  int bufferLength;
} UsartTransmission;
/**
 * @brief Usart initialization structure
 */
typedef struct {
  int baudRate;                                         ///< The requested baud rate
  void (*sendDataToUpperLayer)(char receivedCharacter); ///< Function for sending received data to upper layer
  UsartTransmission (*getMoreDataToTransmit)(void);     ///< Function for getting more data to transmit
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

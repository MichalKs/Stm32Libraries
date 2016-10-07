/**
 * @file    uart.h
 * @brief   Controlling UART
 * @date    14.04.2016
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

#include <inttypes.h>
#include "utils.h"

/**
 * @defgroup  UART UART
 * @brief     UART low level functions
 */

/**
 * @addtogroup UART
 * @{
 */

#define UART_BUF_LEN_TX 512 ///< Buffer length for UART peripheral

void    UART_Initialize(int baud, void(*rxCb)(char), int(*txCb)(char*));
Boolean UART_IsSendingData(void);
void    UART_SendDataIrq(void);
void    UART_EnableIrq(void);
void    UART_DisableIrq(void);
/**
 * @}
 */

#endif /* UART_H_ */

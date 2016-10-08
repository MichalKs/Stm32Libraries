/**
 * @file    systick.c
 * @brief   Managing the SysTick
 * @date    25 sie 2014
 * @author  Michal Ksiezopolski
 * 
 * @verbatim
 * Copyright (c) 2014 Michal Ksiezopolski.
 * All rights reserved. This program and the 
 * accompanying materials are made available 
 * under the terms of the GNU Public License 
 * v3.0 which accompanies this distribution, 
 * and is available at 
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#include "systick.h"
#include <stm32f4xx_hal.h>

/**
 * @defgroup  SYSTICK SYSTICK
 * @brief     SYSTICK control functions.
 */

/**
 * @addtogroup SYSTICK
 * @{
 */

static volatile unsigned int systemClockMillis;  ///< System clock timer.

/**
 * @brief Get the system time
 * @return System time.
 */
unsigned int SYSTICK_GetTimeMillis(void) {
  return systemClockMillis;
}
/**
 * @brief Interrupt handler for SysTick.
 */
void SysTick_Handler(void) {
  HAL_IncTick();
  systemClockMillis++; // Update system time
}

/**
 * @}
 */


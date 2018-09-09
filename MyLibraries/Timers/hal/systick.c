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
#include "common_hal.h"

/**
 * @defgroup  SYSTICK SYSTICK
 * @brief     SYSTICK control functions.
 */

/**
 * @addtogroup SYSTICK
 * @{
 */
static void (*tickHandler)(void); ///< Tick callback

/**
 * @brief Initializes the SysTick
 * @details SysTick is actually initialized by the STM32 HAL so we
 * only set handler here
 * @param tickCb Tick event handler callback
 */
void SysTick_initialize(void (*tickCb)(void)) {
  tickHandler = tickCb;
}
/**
 * @brief Interrupt handler for SysTick.
 */
void SysTick_Handler(void) {
  HAL_IncTick();
  if (tickHandler) {
    tickHandler();
  }
}
/**
 * @}
 */

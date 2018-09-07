/**
 * @file    led_hal.h
 * @brief   HAL for using LEDs
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

#ifndef LED_HAL_H_
#define LED_HAL_H_

#include "utils.h"
#include "boards.h"

/**
 * @defgroup  LED_HAL LED_HAL
 * @brief     HAL - Light Emitting Diode control functions.
 */

/**
 * @addtogroup LED_HAL
 * @{
 */

void LedHal_initialize     (int led);
void LedHal_toggle         (int led);
void LedHal_changeLedState (int led, Boolean state);

/**
 * @}
 */

#endif /* LED_HAL_H_ */

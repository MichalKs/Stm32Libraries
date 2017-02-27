/**
 * @file    led.h
 * @brief   Light Emitting Diodes control functions.
 * @date    08.10.2016
 * @author  Michal Ksiezopolski
 * @details A simple library to add an abstraction
 * layer to blinking LEDs.
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

#ifndef LED_H_
#define LED_H_

#include "utils.h"

/**
 * @defgroup  LED LED
 * @brief     Light Emitting Diode control functions.
 */

/**
 * @addtogroup LED
 * @{
 */

/**
 * @brief LED enum - for identifying an LED.
 */
typedef enum {
  LED_NUMBER0,//!< LED0
  LED_NUMBER1,//!< LED1
  LED_NUMBER2,//!< LED2
  LED_NUMBER3,//!< LED3
} LedNumber;
/**
 * @brief State of an LED.
 */
typedef enum {
  LED_UNUSED, //!< LED_UNUSED LED not initialized
  LED_OFF,    //!< LED_OFF    Turn off LED
  LED_ON,     //!< LED_ON     Turn on LED
} LedState;
/**
 * @brief LED errors
 */
typedef enum {
  LED_OK,            //!< LED_OK
  LED_NOT_INITALIZED,//!< LED_NOT_INITALIZED
  LED_TOO_MANY_LEDS, //!< LED_TOO_MANY_LEDS
  LED_INCORRECT_LED_NUMBER,
} LedResultCode;

LedResultCode Led_addNewLed   (LedNumber led);
LedResultCode Led_toggle      (LedNumber led);
LedResultCode Led_changeState (LedNumber led, LedState state);

/**
 * @}
 */

#endif /* LED_H_ */

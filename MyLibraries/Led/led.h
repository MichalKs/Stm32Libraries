/**
 * @file    led.h
 * @brief   Light Emitting Diodes control functions.
 * @date    08.10.2016
 * @author  Michal Ksiezopolski
 *
 * @details A simple library to add an abstraction
 * layer to blinking LEDs.
 * To use the library you need to call LED_Init using
 * one of the LEDs defined in LED_Number_TypeDef and then
 * use LED_Toggle or LED_ChangeState with the initialized
 * LED number.
 * The various LED ports and pins are defined in
 * led_hal.c and led_hal.h.
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
  _LED0,//!< LED0
  _LED1,//!< LED1
  _LED2,//!< LED2
  _LED3,//!< LED3
  _LED4,//!< LED4
  _LED5,//!< LED5
  _LED6,//!< LED6
  _LED7,//!< LED7
  _LED8,//!< LED8
  _LED9,//!< LED9

} LED_Number_TypeDef;
/**
 * @brief State of an LED.
 */
typedef enum {
  LED_UNUSED, //!< LED_UNUSED LED not initialized
  LED_OFF,    //!< LED_OFF    Turn off LED
  LED_ON,     //!< LED_ON     Turn on LED
} LED_State_TypeDef;
/**
 * @brief LED errors
 */
typedef enum {
  LED_OK,            //!< LED_OK
  LED_NOT_INITALIZED,//!< LED_NOT_INITALIZED
  LED_TOO_MANY_LEDS, //!< LED_TOO_MANY_LEDS
  LED_INCORRECT_LED_NUMBER,
} LED_ErrorTypedef;

LED_ErrorTypedef LED_Add          (LED_Number_TypeDef led);
LED_ErrorTypedef LED_Toggle       (LED_Number_TypeDef led);
LED_ErrorTypedef LED_ChangeState  (LED_Number_TypeDef led, LED_State_TypeDef state);

/**
 * @}
 */

#endif /* LED_H_ */

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

#include <stdio.h>
#include <led.h>
#include <led_hal.h>

/**
 * @addtogroup LED
 * @{
 */

static LED_State_TypeDef ledState[MAX_LEDS]; ///< States of the LEDs (MAX_LEDS is hardware dependent)

/**
 * @brief Add an LED.
 * @param led LED init structure.
 */
LED_ErrorTypedef LED_Add(LED_Number_TypeDef led) {

  if (led >= MAX_LEDS) {
    return LED_TOO_MANY_LEDS;
  }

  LED_HAL_Init(led);
  ledState[led] = LED_OFF; // LED initially off
  return LED_OK;
}
/**
 * @brief Change the state of an LED.
 * @param led LED number.
 * @param state New state.
 */
LED_ErrorTypedef LED_ChangeState(LED_Number_TypeDef led, LED_State_TypeDef state) {

  if (led >= MAX_LEDS) {
    return LED_INCORRECT_LED_NUMBER;
  }

  if (ledState[led] == LED_UNUSED) {
    return LED_NOT_INITALIZED;
  } else {
    if (state == LED_OFF) {
      LED_HAL_ChangeState(led, FALSE); // turn off LED
    } else if (state == LED_ON) {
      LED_HAL_ChangeState(led, TRUE); // light up LED
    }
  }

  ledState[led] = state; // update LED state
  return LED_OK;
}
/**
 * @brief Toggle an LED.
 * @param led LED number.
 */
LED_ErrorTypedef LED_Toggle(LED_Number_TypeDef led) {

  if (led >= MAX_LEDS) {
    return LED_INCORRECT_LED_NUMBER;
  }

  if (ledState[led] == LED_UNUSED) {
    return LED_NOT_INITALIZED;
  } else {
    if (ledState[led] == LED_OFF) {
      ledState[led] = LED_ON;
    } else if (ledState[led] == LED_ON) {
      ledState[led]= LED_OFF;
    }
    LED_HAL_Toggle(led);
  }
  return LED_OK;
}
/**
 * @}
 */

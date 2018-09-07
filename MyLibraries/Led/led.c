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

#include "led.h"
#include "led_hal.h"

/**
 * @addtogroup LED
 * @{
 */

static LedState ledState[BOARD_MAXIMUM_AVAILABLE_LEDS]; ///< States of the LEDs (MAX_LEDS is hardware dependent)

/**
 * @brief Add an LED.
 * @param led LED init structure.
 */
LedResultCode Led_addNewLed(LedNumber led) {
  if (led >= BOARD_MAXIMUM_AVAILABLE_LEDS) {
    return LED_INCORRECT_LED_NUMBER;
  }
  LedHal_initialize(led);
  ledState[led] = LED_OFF;
  return LED_OK;
}
/**
 * @brief Change the state of an LED.
 * @param led LED number.
 * @param state New state.
 */
LedResultCode Led_changeState(LedNumber led, LedState state) {
  if (led >= BOARD_MAXIMUM_AVAILABLE_LEDS) {
    return LED_INCORRECT_LED_NUMBER;
  }
  if (ledState[led] == LED_UNUSED) {
    return LED_NOT_INITALIZED;
  } else {
    if (state == LED_OFF) {
      LedHal_changeLedState(led, FALSE);
    } else if (state == LED_ON) {
      LedHal_changeLedState(led, TRUE);
    }
  }
  ledState[led] = state;
  return LED_OK;
}
/**
 * @brief Toggle an LED.
 * @param led LED number.
 */
LedResultCode Led_toggle(LedNumber led) {
  if (led >= BOARD_MAXIMUM_AVAILABLE_LEDS) {
    return LED_INCORRECT_LED_NUMBER;
  }
  if (ledState[led] == LED_UNUSED) {
    return LED_NOT_INITALIZED;
  } else {
    LedHal_toggle(led);
    if (ledState[led] == LED_OFF) {
      ledState[led] = LED_ON;
    } else if (ledState[led] == LED_ON) {
      ledState[led]= LED_OFF;
    }
  }
  return LED_OK;
}
/**
 * @}
 */

/**
 * @file    led_hal.c
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

#include "led_hal.h"

#ifdef USE_F4_DISCOVERY
  #include <stm32f4xx_hal.h>
  #include "f4_discovery_defs.h"
#endif
#ifdef USE_F7_DISCOVERY
  #include <stm32f7xx_hal.h>
  #include "f7_discovery_defs.h"
#endif

/**
 * @addtogroup LED_HAL
 * @{
 */

/**
 * @brief LED GPIO ports
 */
static GPIO_TypeDef* ledPort[MAX_LEDS] = {
    LED1_PORT,
    LED2_PORT,
    LED3_PORT,
    LED4_PORT
};
/**
 * @brief LED pin numbers
 */
static uint32_t ledPin[MAX_LEDS] = {
    LED1_PIN,
    LED2_PIN,
    LED3_PIN,
    LED4_PIN
};

/**
 * @brief Add an LED.
 * @param led LED number.
 */
void LedHal_initialize(int led) {
  if (ledPort[led] == GPIOA) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
  } else if (ledPort[led] == GPIOB) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
  } else if (ledPort[led] == GPIOC) {
    __HAL_RCC_GPIOC_CLK_ENABLE();
  } else if (ledPort[led] == GPIOD) {
    __HAL_RCC_GPIOD_CLK_ENABLE();
  } else if (ledPort[led] == GPIOE) {
    __HAL_RCC_GPIOE_CLK_ENABLE();
  } else if (ledPort[led] == GPIOF) {
    __HAL_RCC_GPIOF_CLK_ENABLE();
  } else if (ledPort[led] == GPIOG) {
    __HAL_RCC_GPIOG_CLK_ENABLE();
  } else if (ledPort[led] == GPIOH) {
    __HAL_RCC_GPIOH_CLK_ENABLE();
  } else if (ledPort[led] == GPIOI) {
    __HAL_RCC_GPIOI_CLK_ENABLE();
  }
  GPIO_InitTypeDef gpioInitialization;
  gpioInitialization.Mode  = GPIO_MODE_OUTPUT_PP;
  gpioInitialization.Pull  = GPIO_PULLUP;
  gpioInitialization.Speed = GPIO_SPEED_FREQ_LOW;
  gpioInitialization.Pin = ledPin[led];
  HAL_GPIO_Init(ledPort[led], &gpioInitialization);
  HAL_GPIO_WritePin(ledPort[led], ledPin[led], GPIO_PIN_RESET); // turn LED off
}
/**
 * @brief Toggle an LED.
 * @param led LED number.
 */
void LedHal_toggle(int led) {
  ledPort[led]->ODR ^= ledPin[led];
}
/**
 * @brief Change the state of an LED.
 * @param led LED number.
 * @param isLedOn Is LED on
 */
void LedHal_changeLedState(int led, Boolean isLedOn) {
  if (isLedOn == TRUE) {
    ledPort[led]->BSRR = ledPin[led]; // set bit
  } else {
    ledPort[led]->BSRR = (ledPin[led] << 16); // reset bit
  }
}
/**
 * @}
 */

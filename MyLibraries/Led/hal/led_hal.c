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
#include "common_hal.h"

#ifdef BOARD_STM32F4_DISCOVERY
  #include <stm32f4xx_hal.h>
#endif
#ifdef BOARD_STM32F7_DISCOVERY
  #include <stm32f7xx_hal.h>
#endif

typedef struct {
  GPIO_TypeDef * port;
  uint32_t pin;
} LedHalDefinition;

#ifdef BOARD_STM32F4_DISCOVERY
  #include "f4_discovery_defs.h"
#endif
#ifdef BOARD_STM32F7_DISCOVERY
  #include "f7_discovery_defs.h"
#endif

/**
 * @addtogroup LED_HAL
 * @{
 */

/**
 * @brief Add an LED.
 * @param led LED number.
 */
void LedHal_initialize(int led) {
  if (led >= BOARD_MAXIMUM_AVAILABLE_LEDS) {
    return;
  }
  if (ledHalDefinitions[led].port == GPIOA) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
  } else if (ledHalDefinitions[led].port == GPIOB) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
  } else if (ledHalDefinitions[led].port == GPIOC) {
    __HAL_RCC_GPIOC_CLK_ENABLE();
  } else if (ledHalDefinitions[led].port == GPIOD) {
    __HAL_RCC_GPIOD_CLK_ENABLE();
  } else if (ledHalDefinitions[led].port == GPIOE) {
    __HAL_RCC_GPIOE_CLK_ENABLE();
  } else if (ledHalDefinitions[led].port == GPIOF) {
    __HAL_RCC_GPIOF_CLK_ENABLE();
  } else if (ledHalDefinitions[led].port == GPIOG) {
    __HAL_RCC_GPIOG_CLK_ENABLE();
  } else if (ledHalDefinitions[led].port == GPIOH) {
    __HAL_RCC_GPIOH_CLK_ENABLE();
  } else if (ledHalDefinitions[led].port == GPIOI) {
    __HAL_RCC_GPIOI_CLK_ENABLE();
  }
  GPIO_InitTypeDef gpioInitialization;
  gpioInitialization.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioInitialization.Pull   = GPIO_PULLUP;
  gpioInitialization.Speed  = GPIO_SPEED_FREQ_LOW;
  gpioInitialization.Pin    = ledHalDefinitions[led].pin;
  HAL_GPIO_Init(ledHalDefinitions[led].port, &gpioInitialization);
  HAL_GPIO_WritePin(ledHalDefinitions[led].port,
      ledHalDefinitions[led].pin, GPIO_PIN_RESET); // turn LED off
}
/**
 * @brief Toggle an LED.
 * @param led LED number.
 */
void LedHal_toggle(int led) {
  if (led >= BOARD_MAXIMUM_AVAILABLE_LEDS) {
    return;
  }
  ledHalDefinitions[led].port->ODR ^= ledHalDefinitions[led].pin;
}
/**
 * @brief Change the state of an LED.
 * @param led LED number.
 * @param isLedOn Is LED on
 */
void LedHal_changeLedState(int led, Boolean isLedOn) {
  if (led >= BOARD_MAXIMUM_AVAILABLE_LEDS) {
    return;
  }
  if (isLedOn == TRUE) {
    // set bit
    ledHalDefinitions[led].port->BSRR = ledHalDefinitions[led].pin;
  } else {
    // reset bit
    ledHalDefinitions[led].port->BSRR = (ledHalDefinitions[led].pin << 16);
  }
}
/**
 * @}
 */

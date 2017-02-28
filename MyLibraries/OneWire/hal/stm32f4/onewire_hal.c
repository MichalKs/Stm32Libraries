/**
 * @file: 	onewire_hal.c
 * @brief:	ONEWIRE low level functions
 * @date: 	9 paź 2014
 * @author: Michal Ksiezopolski
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

#include "onewire_hal.h"
#include <stm32f4xx.h>

#define ONEWIRE_PIN          GPIO_PIN_1
#define ONEWIRE_PORT         GPIOC
#define ONEWIRE_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

/**
 * @brief Initialize ONEWIRE hardware as open drain output
 */
void OnewireHal_initialize(void) {
  ONEWIRE_CLK_ENABLE();
  GPIO_InitTypeDef gpioInitialization;
  gpioInitialization.Mode  = GPIO_MODE_OUTPUT_OD;
  gpioInitialization.Pull  = GPIO_NOPULL;
  gpioInitialization.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioInitialization.Pin   = ONEWIRE_PIN;
  OnewireHal_releaseBus();
  HAL_GPIO_Init(ONEWIRE_PORT, &gpioInitialization);
}
/**
 * @brief Release the bus. Resistor will pull it up.
 */
void OnewireHal_releaseBus(void) {
  HAL_GPIO_WritePin(ONEWIRE_PORT, ONEWIRE_PIN, GPIO_PIN_SET);
}
/**
 * @brief Pull bus low.
 */
void OnewireHal_busLow(void) {
  HAL_GPIO_WritePin(ONEWIRE_PORT, ONEWIRE_PIN, GPIO_PIN_RESET);
}
/**
 * @brief Read the bus
 * @return Read bus state (high or low)
 */
Boolean OnewireHal_readBus(void) {
  return HAL_GPIO_ReadPin(ONEWIRE_PORT, ONEWIRE_PIN);
}

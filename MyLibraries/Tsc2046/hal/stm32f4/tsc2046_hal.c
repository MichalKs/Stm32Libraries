/**
 * @file    tsc2046_hal.c
 * @brief	  
 * @date    16 gru 2014
 * @author  Michal Ksiezopolski
 *
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

#include "tsc2046_hal.h"
#include <stm32f4xx.h>

static void (*penirqCallback)(void); ///< PENIRQ interrupt callback function

/**
 * @brief Initialize PENIRQ signal and interrupt.
 * @param penirqCb Callback function for PENIRQ interrupt
 */
void TSC2046_HAL_PenirqInit(void (*penirqCb)(void)) {

  penirqCallback = penirqCb;

  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* Configure PC.13 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Enable and set EXTI lines 15 to 10 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}
/**
 * @brief
 * @return
 */
uint8_t TSC2046_HAL_ReadPenirq(void) {
  return HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2);
}
/**
 * @brief Clear PENIRQ flag and enable PENIRQ interrupt.
 */
void TSC2046_HAL_EnablePenirq(void) {
  NVIC_ClearPendingIRQ(EXTI2_IRQn);
  NVIC_EnableIRQ(EXTI2_IRQn);
}
/**
 * @brief Disable PENIRQ interrupt.
 */
void TSC2046_HAL_DisablePenirq(void) {
  NVIC_DisableIRQ(EXTI2_IRQn);
}
/**
 * @brief Handler for PENIRQ interrupt.
 */
void EXTI2_IRQHandler(void) {
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}
/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == GPIO_PIN_2) {
    penirqCallback();
  }
}

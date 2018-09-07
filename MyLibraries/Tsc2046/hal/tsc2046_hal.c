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
#include "common_hal.h"

static void (*penirqCallback)(void); ///< PENIRQ interrupt callback function

#define PENIRQ_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define PENIRQ_PIN          GPIO_PIN_2
#define PENIRQ_PORT         GPIOD
#define PENIRQ_IRQ_NUMBER   EXTI2_IRQn
#define PENIRQ_PRIORITY     2

/**
 * @brief Initialize PENIRQ signal and interrupt.
 * @param penirqCb Callback function for PENIRQ interrupt
 */
void TSC2046_HAL_PenirqInit(void (*penirqCb)(void)) {

  penirqCallback = penirqCb;

  PENIRQ_CLK_ENABLE();
  GPIO_InitTypeDef   gpioInitialization;
  gpioInitialization.Mode = GPIO_MODE_IT_FALLING;
  gpioInitialization.Pull = GPIO_PULLUP;
  gpioInitialization.Pin = PENIRQ_PIN;
  HAL_GPIO_Init(PENIRQ_PORT, &gpioInitialization);

  HAL_NVIC_SetPriority(PENIRQ_IRQ_NUMBER, PENIRQ_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(PENIRQ_IRQ_NUMBER);
}
/**
 * @brief Read PENIRQ state
 * @retval TRUE PENIRQ is high
 * @retval FALSE PENIRQ is low
 */
Boolean TSC2046_HAL_ReadPenirq(void) {
  return HAL_GPIO_ReadPin(PENIRQ_PORT, PENIRQ_PIN);
}
/**
 * @brief Clear PENIRQ flag and enable PENIRQ interrupt.
 */
void TSC2046_HAL_EnablePenirq(void) {
  NVIC_ClearPendingIRQ(PENIRQ_IRQ_NUMBER);
  NVIC_EnableIRQ(PENIRQ_IRQ_NUMBER);
}
/**
 * @brief Disable PENIRQ interrupt.
 */
void TSC2046_HAL_DisablePenirq(void) {
  NVIC_DisableIRQ(PENIRQ_IRQ_NUMBER);
}
/**
 * @brief Handler for PENIRQ interrupt.
 */
void EXTI2_IRQHandler(void) {
  HAL_GPIO_EXTI_IRQHandler(PENIRQ_PIN);
}
/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == PENIRQ_PIN) {
    penirqCallback();
  }
}

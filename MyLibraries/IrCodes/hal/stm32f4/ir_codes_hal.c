/**
 * @file: 	ir_hal.c
 * @brief:	
 * @date: 	3 gru 2014
 * @author: Michal Ksiezopolski
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

#include <ir_codes_hal.h>
#include "common_hal.h"
#include <stm32f4xx.h>

static void (*readDataCallback)(uint16_t pulseWidth, uint8_t edge); ///< Callback for sending received pulses to higher layer
static void (*resetFrameCallback)(void); ///< Callback for resetting frame if timeout occurs.
static TIM_HandleTypeDef timer4Handle;   ///< Timer 4 handle

/**
 * @brief Initialize hardware for decoding IR codes.
 */
void IR_HAL_Init(
    void (*readDataCb)(uint16_t pulseWidth, uint8_t edge),
    void (*resetFrameCb)(void),
    uint32_t timeout) {

  readDataCallback = readDataCb;
  resetFrameCallback = resetFrameCb;

  __HAL_RCC_TIM4_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // TIM4 IC2 pin configuration (PB7)
  GPIO_InitTypeDef  gpioInitialization;
  gpioInitialization.Pin       = GPIO_PIN_7;
  gpioInitialization.Mode      = GPIO_MODE_AF_PP;
  gpioInitialization.Pull      = GPIO_PULLUP;
  gpioInitialization.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioInitialization.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOB, &gpioInitialization);

  // Timer clock is 84MHz
  // Update flag will cause timeout of frame
  TIM_HandleTypeDef * timerHandle = &timer4Handle;
  timerHandle->Instance = TIM4;
  timerHandle->Init.Period = timeout;
  timerHandle->Init.Prescaler = 84 - 1; // 1 tick = 1 us
  timerHandle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  timerHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
  timerHandle->Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(timerHandle) != HAL_OK) {
    CommonHal_errorHandler();
  }
  HAL_NVIC_SetPriority(TIM4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(TIM4_IRQn);

  /* TIM4 configuration: PWM Input mode for measuring IR signal pulses.
   * The external signal is connected to TIM4 CH2 pin (PB7).
   * The falling edge is used as active edge - IC counters will be reset at falling edge
   * The TIM4 CCR2 is used to compute the period (from which we derive high pulse value)
   * The TIM4 CCR1 is used to compute the low pulse value
   */
  TIM_IC_InitTypeDef sConfig;
  sConfig.ICPolarity = TIM_ICPOLARITY_FALLING; // Falling edge triggers CC2
  sConfig.ICPrescaler = TIM_ICPSC_DIV1; // count every edge
  sConfig.ICFilter = 0;
  sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI; // Standard connections
  HAL_TIM_IC_ConfigChannel(timerHandle, &sConfig, TIM_CHANNEL_2);

  sConfig.ICPolarity = TIM_ICPOLARITY_RISING; // Falling edge triggers CC2
  sConfig.ICPrescaler = TIM_ICPSC_DIV1; // count every edge
  sConfig.ICFilter = 0;
  sConfig.ICSelection = TIM_ICSELECTION_INDIRECTTI; // Standard connections
  HAL_TIM_IC_ConfigChannel(timerHandle, &sConfig, TIM_CHANNEL_1);

  // Select the TIM4 Input Trigger: TI2FP2
  uint32_t tmpsmcr = 0U;

   /* Get the TIMx SMCR register value */
   tmpsmcr = TIM4->SMCR;
   /* Reset the TS Bits */
   tmpsmcr &= ~TIM_SMCR_TS;
   /* Set the Input Trigger source and the slave mode*/
   tmpsmcr |= TIM_TS_TI2FP2 | TIM_SLAVEMODE_EXTERNAL1;
   /* Write to TIMx SMCR */
   TIM4->SMCR = tmpsmcr;

  // Select the slave Mode: Reset Mode
  /* Reset the SMS Bits */
  TIM4->SMCR &= (uint16_t)~TIM_SMCR_SMS;

  /* Select the Slave Mode */
  TIM4->SMCR |= TIM_SLAVEMODE_RESET;
  /* Reset the MSM Bit */
  TIM4->SMCR &= (uint16_t)~TIM_SMCR_MSM;

  /* Set or Reset the MSM Bit */
  TIM4->SMCR |= TIM_MASTERSLAVEMODE_ENABLE;
  TIM4->CR1 |= TIM_CR1_URS;

  HAL_TIM_IC_Start_IT(timerHandle, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(timerHandle, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(timerHandle);
}
/**
 * @brief Timer 4 Handler
 *
 * @details Used for decoding RC4 frames using PWMI (input capture) measurement.
 */
void TIM4_IRQHandler(void) {

  static uint16_t high = 0;  // high pulse duration
  static uint16_t low = 0;   // low pulse duration

  if (__HAL_TIM_GET_FLAG(&timer4Handle, TIM_FLAG_CC1) != RESET) {
    if (__HAL_TIM_GET_IT_SOURCE(&timer4Handle, TIM_IT_CC1) != RESET) {
      __HAL_TIM_CLEAR_IT(&timer4Handle, TIM_IT_CC1);
      // Get the Input Capture value
      low = HAL_TIM_ReadCapturedValue(&timer4Handle, TIM_CHANNEL_1);
      readDataCallback(low, 0); // decode low pulse
    }
  }

  if (__HAL_TIM_GET_FLAG(&timer4Handle, TIM_FLAG_CC2) != RESET) {
    if (__HAL_TIM_GET_IT_SOURCE(&timer4Handle, TIM_IT_CC2) != RESET) {
      __HAL_TIM_CLEAR_IT(&timer4Handle, TIM_IT_CC2);
      // Get the Input Capture value
      high = HAL_TIM_ReadCapturedValue(&timer4Handle, TIM_CHANNEL_2);
      readDataCallback(high - low, 1); // decode high pulse
    }
  }

  if (__HAL_TIM_GET_FLAG(&timer4Handle, TIM_FLAG_UPDATE) != RESET) {
    if (__HAL_TIM_GET_IT_SOURCE(&timer4Handle, TIM_IT_UPDATE) != RESET) {
      __HAL_TIM_CLEAR_IT(&timer4Handle, TIM_IT_UPDATE);
      // If timeout occurs, clear frame state.
      resetFrameCallback();
    }
  }

}


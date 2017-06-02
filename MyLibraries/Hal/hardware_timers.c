/*
 * hardware_timers.c
 *
 *  Created on: 28.02.2017
 *      Author: mik
 */

#include "hardware_timers.h"
#include "common_hal.h"
#ifdef USE_F4_DISCOVERY
  #include <stm32f4xx_hal.h>
#endif
#ifdef USE_F7_DISCOVERY
  #include <stm32f7xx_hal.h>
#endif

static TIM_HandleTypeDef timer5Handle; ///< Timer 5 handle
static void (*callbackTimer5)(void);   ///< Callback for timer 5

/**
 * @brief Initalize system timer
 * @param systemTimerFunction Callback for performing overall system tasks
 */
void HardwareTimers_configureTimerAsIrqWithCallback(HardwareTimers timer, int frequency,
    void (*callback)(void)) {

  TIM_HandleTypeDef * timerHandle;

  switch(timer) {
  case HARDWARE_TIMERS_TIMER5:
    // TIM5 clock runs at 84MHz
    callbackTimer5 = callback;
    __HAL_RCC_TIM5_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM5_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
    timerHandle = &timer5Handle;
    timerHandle->Instance = TIM5;
    break;
  default:
    return;
  }
  // timer clock is 10 times faster than the desired frequency, to have 10 ticks per IRQ
  const uint32_t TIMER_CLOCK_FREQUENCY = frequency * 10;
  // interrupt every 10 clock cycles to reach desired frequency
  const uint32_t TIMER_PERIOD = 10 - 1;
  uint32_t clockPrescaler = (uint32_t) ((SystemCoreClock / 2) / TIMER_CLOCK_FREQUENCY) - 1;

  timerHandle->Init.Period = TIMER_PERIOD;
  timerHandle->Init.Prescaler = clockPrescaler;
  timerHandle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  timerHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
  timerHandle->Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(timerHandle) != HAL_OK) {
    CommonHal_errorHandler();
  }
  if (HAL_TIM_Base_Start_IT(timerHandle) != HAL_OK) {
    CommonHal_errorHandler();
  }
}
/**
 * @brief This function handles TIM5 interrupt request
 */
void TIM5_IRQHandler(void) {
  if (__HAL_TIM_GET_FLAG(&timer5Handle, TIM_FLAG_UPDATE) != RESET) {
    if (__HAL_TIM_GET_IT_SOURCE(&timer5Handle, TIM_IT_UPDATE) != RESET) {
      __HAL_TIM_CLEAR_IT(&timer5Handle, TIM_IT_UPDATE);
      if (callbackTimer5) {
        callbackTimer5();
      }
    }
  }
}

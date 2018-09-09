/*
 * hardware_timers.c
 *
 *  Created on: 28.02.2017
 *      Author: mik
 */

#include "hardware_timers.h"
#include "common_hal.h"

#define NUMBER_OF_HARDWARE_TIMERS 1

#define TIMER5_IRQ_PRIORITY 10

typedef struct {
  TIM_HandleTypeDef * handle;
  void (*overflowCb)(void);
  Boolean isInitialzed;
} TimerControl;

static TimerControl timerControl[NUMBER_OF_HARDWARE_TIMERS];
static TIM_HandleTypeDef timer5Handle; ///< Timer 5 handle

/**
 * @brief Initalize system timer
 * @param timer Timer number
 * @param frequencyHz Frequency of clock in Hz
 * @param overflowCb Callback for timeout
 */
void HardwareTimers_configureTimerAsIrqWithCallback(HardwareTimers timer, int frequencyHz,
    void (*overflowCb)(void)) {
  uint32_t coreClockToTimerClockRatio;
  switch (timer) {
  case HARDWARE_TIMERS_TIMER5:
    // TIM5 clock runs at 84MHz
    coreClockToTimerClockRatio = 2;
    __HAL_RCC_TIM5_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM5_IRQn, TIMER5_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
    timerControl[timer].handle = &timer5Handle;
    timer5Handle.Instance = TIM5;
    break;
  default:
    return;
  }
  timerControl[timer].isInitialzed = TRUE;
  timerControl[timer].overflowCb = overflowCb;
  // timer clock is x times faster than the desired frequency, to have x ticks per IRQ
  // TODO TIMER_CLOCK_TO_IRQ_CLOCK_HZ will have to vary on the requested frequency
  const uint32_t TIMER_CLOCK_TO_IRQ_CLOCK_HZ = 1;
  const uint32_t TIMER_CLOCK_FREQUENCY_HZ = frequencyHz * TIMER_CLOCK_TO_IRQ_CLOCK_HZ;
  const int TIMER_START_COUNT_FROM_ZERO_OFFSET = 1;
  uint32_t timerPeriodTicks = SystemCoreClock / coreClockToTimerClockRatio /
      TIMER_CLOCK_FREQUENCY_HZ;

  timerControl[timer].handle->Init.Period = timerPeriodTicks -
      TIMER_START_COUNT_FROM_ZERO_OFFSET;
  timerControl[timer].handle->Init.Prescaler = TIMER_CLOCK_TO_IRQ_CLOCK_HZ -
      TIMER_START_COUNT_FROM_ZERO_OFFSET;
  timerControl[timer].handle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  timerControl[timer].handle->Init.CounterMode = TIM_COUNTERMODE_UP;
  timerControl[timer].handle->Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(timerControl[timer].handle) != HAL_OK) {
    CommonHal_errorHandler();
  }
  if (HAL_TIM_Base_Start_IT(timerControl[timer].handle) != HAL_OK) {
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
      if (timerControl[HARDWARE_TIMERS_TIMER5].overflowCb) {
        timerControl[HARDWARE_TIMERS_TIMER5].overflowCb();
      }
    }
  }
}

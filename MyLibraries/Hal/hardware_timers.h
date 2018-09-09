/*
 * hardware_timers.h
 *
 *  Created on: 28.02.2017
 *      Author: mik
 */

#ifndef MYLIBRARIES_HAL_STM32F4_HARDWARE_TIMERS_H_
#define MYLIBRARIES_HAL_STM32F4_HARDWARE_TIMERS_H_

#include "utils.h"

typedef enum {
  HARDWARE_TIMERS_TIMER5 = 0,
} HardwareTimers;

void HardwareTimers_configureTimerAsIrqWithCallback(HardwareTimers timer, int frequency,
    void (*overflowCb)(void));

#endif /* MYLIBRARIES_HAL_STM32F4_HARDWARE_TIMERS_H_ */

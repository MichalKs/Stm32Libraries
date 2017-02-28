/**
 * @file    timers.h
 * @brief   Timing control functions.
 * @date    08.10.2016
 * @author  Michal Ksiezopolski
 * 
 * Control of the SysTick and software timers
 * incremented based on SysTick interrupts.
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

#ifndef TIMERS_H_
#define TIMERS_H_

#include "utils.h"

/**
 * @defgroup  TIMER TIMER
 * @brief     Timing control functions
 */

/**
 * @addtogroup TIMER
 * @{
 */
/**
 * @brief Timer errors
 */
typedef enum {
  TIMER_OK = 0,                //!< TIMER_OK
  TIMER_TOO_MANY_TIMERS = -100,//!< TIMER_TOO_MANY_TIMERS
} TimerResultCode;

void         Timer_delayMicros          (unsigned int micros);
void         Timer_delayMillis          (unsigned int millis);
void         Timer_startSoftwareTimer   (int id);
void         Timer_softwareTimersUpdate (void);
Boolean      Timer_delayTimer           (unsigned int millis, unsigned int startTimeMillis);
unsigned int Timer_getTimeMillis        (void);
int          Timer_addSoftwareTimer     (unsigned int overflowValue, void (*overflowCb)(void));
/**
 * @}
 */

#endif /* TIMERS_H_ */

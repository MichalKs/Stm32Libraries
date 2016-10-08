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
} TIMER_ErrorTypedef;

void         TIMER_DelayMillis     (unsigned int millis);
void         TIMER_StartSoftTimer  (int id);
void         TIMER_SoftTimersUpdate(void);
Boolean      TIMER_DelayTimer      (unsigned int millis, unsigned int startTimeMillis);
unsigned int TIMER_GetTimeMillis   (void);
int          TIMER_AddSoftTimer    (unsigned int overflowValue, void (*overflowCb)(void));
/**
 * @}
 */

#endif /* TIMERS_H_ */

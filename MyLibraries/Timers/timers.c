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

#include "timers.h"
#include "systick.h"
#include "hardware_timers.h"
#include <stdio.h>

#ifndef DEBUG_TIMERS
  #define DEBUG_TIMERS
#endif

#ifdef DEBUG_TIMERS
  #define print(str, args...) printf("TIMER--> "str"%s",##args,"\r")
  #define println(str, args...) printf("TIMER--> "str"%s",##args,"\r\n")
#else
  #define print(str, args...) (void)0
  #define println(str, args...) (void)0
#endif

/**
 * @addtogroup TIMER
 * @{
 */

#define MAXIMUM_SOFT_TIMERS   10        ///< Maximum number of soft timers.
#define ID_TO_ARRAY_INDEX(x)  ((x) - 1) ///< Converts timer ID to array index

/**
 * @brief Soft timer structure.
 */
typedef struct {
  int id;                           ///< Timer ID
  unsigned int currentCountMillis;  ///< Current count value
  unsigned int timeoutMillis;       ///< Overflow value
  Boolean isActive;                 ///< Is timer active?
  void (*overflowCb)(void);         ///< Function called on overflow event
} SoftTimer;

static SoftTimer softTimers[MAXIMUM_SOFT_TIMERS];   ///< Array of soft timers
static int softTimerCount;                          ///< Count number of soft timers
static volatile unsigned int systemClockMillis;     ///< System clock timer.
static volatile unsigned int systemClockMicros;     ///< Microsecond counter
static Boolean isMicrosCounterInitialized = FALSE;  ///< Is us counter initialized

/**
 * @brief Updates the system time in ms
 */
static void updateMillisCounter(void) {
  systemClockMillis++;
}
/**
 * @brief Updates the us counter
 */
static void updateMicrosCounter(void) {
  systemClockMicros++;
}
/**
 * @brief Initialize the timer module (SysTick)
 */
void Timer_initialize(void) {
  SysTick_initialize(updateMillisCounter);
}
/**
 * @brief Returns the system time.
 * @return System time
 */
unsigned int Timer_getTimeMillis(void) {
  return systemClockMillis;
}
/**
 * @brief Blocking delay function.
 * @param millis Milliseconds to delay.
 * @warning This is a blocking function. Use with care!
 */
void Timer_delayMillis(unsigned int millis) {
  unsigned int startTimeMillis = Timer_getTimeMillis();
  unsigned int currentTimeMillis;

  while (TRUE) {
    currentTimeMillis = Timer_getTimeMillis();
    if ((currentTimeMillis >= startTimeMillis) &&
        (currentTimeMillis - startTimeMillis >= millis)) {
      break;
    }
    // account for system timer overflow
    if ((currentTimeMillis < startTimeMillis) &&
        (UINT32_MAX - startTimeMillis + currentTimeMillis >= millis)) {
      break;
    }
  }
}
/**
 * @brief Blocking delay function.
 * @param micros Microseconds to delay
 */
void Timer_delayMicros(unsigned int micros) {
  if (!isMicrosCounterInitialized) {
    const int TIMER_FREQUENCY_HZ = 1000000;
    HardwareTimers_configureTimerAsIrqWithCallback(HARDWARE_TIMERS_TIMER5, TIMER_FREQUENCY_HZ,
        updateMicrosCounter);
    isMicrosCounterInitialized = TRUE;
  }

  unsigned int startTimeMicros = systemClockMicros;
  unsigned int currentTimeMicros;
  while (TRUE) {
    currentTimeMicros = systemClockMicros;
    if ((currentTimeMicros >= startTimeMicros) &&
        (currentTimeMicros - startTimeMicros >= micros)) {
      break;
    }
    // account for system timer overflow
    if ((currentTimeMicros < startTimeMicros) &&
        (UINT32_MAX - startTimeMicros + currentTimeMicros >= micros)) {
      break;
    }
  }
}
/**
 * @brief Nonblocking delay function
 * @param millis Delay time
 * @param startTimeMillis System time at start of delay (this has to be written before delay using TIMER_GetTime())
 * @retval FALSE Delay value has not been reached (wait longer)
 * @retval TRUE Delay value has been reached
 */
Boolean Timer_delayTimer(unsigned int millis, unsigned int startTimeMillis) {
  unsigned int currentTimeMillis = Timer_getTimeMillis();
  if ((currentTimeMillis >= startTimeMillis) &&
      (currentTimeMillis - startTimeMillis >= millis)) {
    return TRUE;
  // account for system timer overflow
  } else if ((currentTimeMillis < startTimeMillis) &&
      (UINT32_MAX - startTimeMillis + currentTimeMillis >= millis)) {
    return TRUE;
  } else {
    return FALSE;
  }
}
/**
 * @brief Adds a soft timer
 * @param timeoutMillis Overflow value of timer in ms
 * @param overflowCb Function called on overflow (should return void and accept no parameters)
 * @return Returns the ID of the new counter or error code (count start from 1)
 * @retval TIMER_TOO_MANY_TIMERS Too many timers
 */
int Timer_addSoftwareTimer(unsigned int timeoutMillis, void (*overflowCb)(void)) {
  if (softTimerCount > MAXIMUM_SOFT_TIMERS) {
    println("Reached maximum number of timers!");
    return TIMER_TOO_MANY_TIMERS;
  }
  softTimerCount++;

  softTimers[ID_TO_ARRAY_INDEX(softTimerCount)].id                  = softTimerCount;
  softTimers[ID_TO_ARRAY_INDEX(softTimerCount)].overflowCb          = overflowCb;
  softTimers[ID_TO_ARRAY_INDEX(softTimerCount)].timeoutMillis       = timeoutMillis;
  softTimers[ID_TO_ARRAY_INDEX(softTimerCount)].currentCountMillis  = 0;
  softTimers[ID_TO_ARRAY_INDEX(softTimerCount)].isActive            = FALSE; // inactive on startup
  return softTimerCount;
}
/**
 * @brief Starts the timer (zeroes out current count value).
 * @param id Timer ID
 */
void Timer_startSoftwareTimer(int id) {
  softTimers[ID_TO_ARRAY_INDEX(id)].currentCountMillis = 0;
  softTimers[ID_TO_ARRAY_INDEX(id)].isActive = TRUE;
}
/**
 * @brief Pauses given timer (current count value unchanged)
 * @param id Timer ID
 */
void Timer_pauseSoftwareTimer(int id) {
  softTimers[ID_TO_ARRAY_INDEX(id)].isActive = FALSE;
}
/**
 * @brief Resumes a timer (starts counting from last value).
 * @param id Timer ID
 */
void Timer_resumeSoftwareTimer(int id) {
  softTimers[ID_TO_ARRAY_INDEX(id)].isActive = TRUE;
}
/**
 * @brief Updates all the timers and calls the overflow functions as
 * necessary
 * @details This function can be called periodically in the main
 * loop of the program
 */
void Timer_softwareTimersUpdate(void) {

  static unsigned int previousTimeMillis;
  unsigned int deltaMillis;
  unsigned int currentTimeMillis = Timer_getTimeMillis();

  if (currentTimeMillis >= previousTimeMillis) {
    // How much time passed from previous run
    deltaMillis = currentTimeMillis - previousTimeMillis;
  } else {
    // if overflow occurs
    // the difference is the value that previousTimeMillis
    // has to UINT32_MAX + the new number of currentTimeMillis
    deltaMillis = UINT32_MAX - previousTimeMillis + currentTimeMillis;
  }

  previousTimeMillis += deltaMillis; // update time for the function

  for (int i = 0; i < softTimerCount; i++) {
    if (softTimers[i].isActive == TRUE) {
      softTimers[i].currentCountMillis += deltaMillis;
      if (softTimers[i].currentCountMillis >= softTimers[i].timeoutMillis) {
        softTimers[i].currentCountMillis = 0; // zero out timer
        if (softTimers[i].overflowCb != NULL) {
          softTimers[i].overflowCb();
        }
      }
    }
  }
}
/**
 * @}
 */

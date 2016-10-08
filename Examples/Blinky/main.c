/**
 * @file    main.c
 * @brief   Blinky test
 * @date    07.10.2016
 * @author  Michal Ksiezopolski
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

#include <stdio.h>
#include <string.h>
#include "common_hal.h"
#include "timers.h"
#include "comm.h"
#include "led.h"
#include "utils.h"

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf(""str"%s",##args,"")
#define println(str, args...) printf("MAIN--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

/**
 * @brief Callback for performing periodic tasks
 */
void softTimerCallback(void) {

  LED_Toggle(_LED2);
  println("Hello world");

  const int FRAME_MAX_SIZE = 10;
  char frameBuffer[FRAME_MAX_SIZE];   // buffer for receiving commands from PC
  int length;                         // length of command

  // check for new frames from PC
  if (COMM_GetFrame(frameBuffer, &length, FRAME_MAX_SIZE) == COMM_GOT_FRAME) {
    println("Got frame of length %d: %s", (int)length, (char*)frameBuffer);

    // control LED0 from terminal
    if (!strcmp((char*)frameBuffer, ":LED 0 ON")) {
      LED_ChangeState(_LED0, LED_ON);
    }
    if (!strcmp((char*)frameBuffer, ":LED 0 OFF")) {
      LED_ChangeState(_LED0, LED_OFF);
    }
    if (!strcmp((char*)frameBuffer, ":LED 1 ON")) {
      LED_ChangeState(_LED1, LED_ON);
    }
    if (!strcmp((char*)frameBuffer, ":LED 1 OFF")) {
      LED_ChangeState(_LED1, LED_OFF);
    }
  }
}
/**
  * @brief  Main program
  */
int main(void) {

  COMMON_HAL_Init();

  const int COMM_BAUD_RATE = 115200;
  COMM_Initialize(COMM_BAUD_RATE);
  println("Starting program"); // Print a string to terminal

  LED_Add(_LED0);
  LED_Add(_LED1);
  LED_Add(_LED2);

  // Add a soft timer with callback
  const int SOFT_TIMER_PERIOD_MILLIS = 1000;
  int timerId = TIMER_AddSoftTimer(SOFT_TIMER_PERIOD_MILLIS, softTimerCallback);
  TIMER_StartSoftTimer(timerId);

  while (TRUE) {
    TIMER_SoftTimersUpdate();
  }

  return 0;
}


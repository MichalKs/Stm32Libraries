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
#include "led.h"
#include "utils.h"
#include "serial_port.h"

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf(""str"%s",##args,"")
#define println(str, args...) printf("MAIN--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

/**
 * @brief Checks serial port frames
 */
static void checkForSerialPortFrames(void) {
  const int FRAME_MAX_SIZE = 32;
  char frameBuffer[FRAME_MAX_SIZE];   // buffer for receiving commands from PC
  int commandLength;

  // check for new frames from PC
  // if it does not work play with the CR=CR+LF setting in the PC terminal
  if (SerialPort_getFrame(frameBuffer, &commandLength, FRAME_MAX_SIZE) == SERIAL_PORT_GOT_FRAME) {
    println("Got frame of length %d: %s", (int)commandLength, (char*)frameBuffer);

    // control LED0 from terminal
    if (!strcmp((char*)frameBuffer, ":LED 0 ON")) {
      Led_changeState(LED_NUMBER0, LED_ON);
    }
    if (!strcmp((char*)frameBuffer, ":LED 0 OFF")) {
      Led_changeState(LED_NUMBER0, LED_OFF);
    }
    if (!strcmp((char*)frameBuffer, ":LED 1 ON")) {
      Led_changeState(LED_NUMBER1, LED_ON);
    }
    if (!strcmp((char*)frameBuffer, ":LED 1 OFF")) {
      Led_changeState(LED_NUMBER1, LED_OFF);
    }
  }
}
/**
 * @brief Callback for performing periodic tasks
 */
static void softTimerCallback(void) {
  Led_toggle(LED_NUMBER3);
//  println("Hello world!!!");
}
/**
  * @brief  Main program
  */
int main(void) {

  CommonHal_initialize();
  Timer_initialize();

  const int COMM_BAUD_RATE = 115200;
  SerialPort_initialize(COMM_BAUD_RATE);
  println("Starting program"); // Print a string to terminal

  Led_addNewLed(LED_NUMBER0);
  Led_addNewLed(LED_NUMBER1);
  Led_addNewLed(LED_NUMBER2);
  Led_addNewLed(LED_NUMBER3);
  Led_changeState(LED_NUMBER0, LED_ON);
  Led_changeState(LED_NUMBER1, LED_ON);
  Led_changeState(LED_NUMBER2, LED_ON);
  Led_changeState(LED_NUMBER3, LED_ON);

  // Add a soft timer with callback
  const int SOFT_TIMER_PERIOD_MILLIS = 1000;
  int timerId = Timer_addSoftwareTimer(SOFT_TIMER_PERIOD_MILLIS,
      softTimerCallback);
  Timer_startSoftwareTimer(timerId);

  while (TRUE) {
    Timer_softwareTimersUpdate();
    checkForSerialPortFrames();
    Timer_delayMicros(100*1000);
    Led_toggle(LED_NUMBER2);
  }
  return 0;
}


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
#include "onewire.h"
#include "ds18b20.h"

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
  Led_toggle(LED_NUMBER2);

  const int FRAME_MAX_SIZE = 10;
  char frameBuffer[FRAME_MAX_SIZE];   // buffer for receiving commands from PC
  int length;                         // length of command

  if (SerialPort_getFrame(frameBuffer, &length, FRAME_MAX_SIZE) == SERIAL_PORT_GOT_FRAME) {
    println("Got frame of length %d: %s", (int)length, (char*)frameBuffer);
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

  static int counter;
  float temperatureCelsius;
  // get temperature every 2 seconds
  switch (counter % 2) {
  case 0:
    Ds18b20_conversionStart();
    break;
  case 1:
    temperatureCelsius = Ds18b20_readTemperatureCelsius();
    println("Temperature = %.2f", temperatureCelsius);
    break;
  }
  counter++;
}
/**
  * @brief  Main program
  */
int main(void) {
  CommonHal_initialize();
  const int COMM_BAUD_RATE = 115200;
  SerialPort_initialize(COMM_BAUD_RATE);
  println("Starting program");
  Led_addNewLed(LED_NUMBER0);
  Led_addNewLed(LED_NUMBER1);
  Led_addNewLed(LED_NUMBER2);
  const int SOFT_TIMER_PERIOD_MILLIS = 1000;
  int timerId = Timer_addSoftwareTimer(SOFT_TIMER_PERIOD_MILLIS, softTimerCallback);
  Timer_startSoftwareTimer(timerId);
  Onewire_initialize();
  Ds18b20_initialize();

  while (TRUE) {
    Timer_softwareTimersUpdate();
  }
  return 0;
}

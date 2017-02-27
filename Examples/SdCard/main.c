/**
 * @file    main.c
 * @brief   LED test
 * @date    9 kwi 2014
 * @author  Michal Ksiezopolski
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

#include "timers.h"
#include "led.h"
#include "serial_port.h"
#include "common_hal.h"
#include "keys.h"
#include "fat.h"
#include "sdcard.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

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

  const int FRAME_MAX_SIZE = 10;
  char frameBuffer[FRAME_MAX_SIZE];   // buffer for receiving commands from PC
  int length;                         // length of command

  // check for new frames from PC
  if (SerialPort_getFrame(frameBuffer, &length, FRAME_MAX_SIZE) == SERIAL_PORT_GOT_FRAME) {
    println("Got frame of length %d: %s", (int)length, (char*)frameBuffer);

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
 * @brief Main function
 */
int main(void) {

  CommonHal_initialize();

  const int COMM_BAUD_RATE = 115200;
  SerialPort_initialize(COMM_BAUD_RATE);
  println("Starting program"); // Print a string to terminal

  Led_addNewLed(LED_NUMBER0);
  Led_addNewLed(LED_NUMBER1);
  Led_addNewLed(LED_NUMBER2);

  // Add a soft timer with callback
  const int SOFT_TIMER_PERIOD_MILLIS = 1000;
  int timerId = Timer_addSoftwareTimer(SOFT_TIMER_PERIOD_MILLIS, softTimerCallback);
  Timer_startSoftwareTimer(timerId);

  FAT_Init(SD_Initialize, SD_ReadSectors, SD_WriteSectors);
  int hello = FAT_OpenFile("HELLO   TXT");
  uint8_t data[100];

  FAT_MoveRdPtr(hello, 500);

  int i = FAT_ReadFile(hello, data, 5);
  i += FAT_ReadFile(hello, data+i, 60);
  UTILS_HexdumpWithCharacters(data, i);

  int hamlet = FAT_OpenFile("HAMLET  TXT");

  FAT_MoveRdPtr(hamlet, 184120);

  i = FAT_ReadFile(hamlet, data, 5);
  i += FAT_ReadFile(hamlet, data+i, 30);
  UTILS_HexdumpWithCharacters(data, i);

  char message[] = "Hello world, from STM32 to FAT driver new one"; // length 37

//  FAT_MoveWrPtr(hello, 500);
//
//  FAT_WriteFile(hello, (uint8_t*)message, strlen(message));

  while (TRUE) {
    Timer_softwareTimersUpdate(); // run timers
  }
}

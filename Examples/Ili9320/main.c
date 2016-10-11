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
#include "comm.h"
#include "common_hal.h"
#include "keys.h"
#include "graphics.h"
#include "font_14x27.h"
#include "font_21x39.h"
#include "font_10x20.h"
#include "font_8x16.h"
#include "tsc2046.h"
#include "gui.h"
#include "fat.h"
#include "sdcard.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ili9320.h"
#include "example_bmp.h"

static void tscEvent1(int x, int y);
static void tscEvent2(int x, int y);

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf(""str"%s",##args,"")
#define println(str, args...) printf("MAIN--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

//#define TEST_SD
//#define USE_GUI

/**
 * @brief Callback for performing periodic tasks
 */
void softTimerCallback(void) {

//  println("Hello world");

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
 * @brief Main function
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


#ifndef USE_GUI
  GRAPH_LcdDriverTypedef lcdDriver;
  lcdDriver.initialize = ILI9320_Initializtion;
  lcdDriver.setWindow = ILI9320_SetWindow;
  lcdDriver.drawPixel = ILI9320_DrawPixel;
  lcdDriver.drawNextPixel = ILI9320_DrawNextPixel;
  lcdDriver.setGramAddress = ILI9320_SetCursor;
  lcdDriver.horizontalGramUpdate = ILI9320_SetHorizontalGramUpdateDirection;
  lcdDriver.verticalGramUpdate = ILI9320_SetVerticalGramUpdateDirection;
  lcdDriver.width = 320;
  lcdDriver.height = 240;
  GRAPH_Initialize(&lcdDriver);
  GRAPH_DrawFilledCircle(100, 100, 50, 0x00ff00ul);
  GRAPH_DrawFilledRectangle(10, 100, 50, 100, GRAPH_RED);
  GRAPH_DrawRectangle(70, 100, 50, 100, 10, GRAPH_RED);
  GRAPH_SetFont(font21x39Info);
  GRAPH_DrawString("Hello World", 50, 50, GRAPH_WHITE, GRAPH_BLUE);
  GRAPH_DrawFilledRectangle(10, 100, 50, 100, GRAPH_RED);
  GRAPH_DrawLine(50, 50, 200, 200, GRAPH_RED);
//  GRAPH_DrawFilledCircle(100, 100, 50, 0x00ff00ul);
//  GRAPH_DrawBox(100, 100, 100, 100, 5, GRAPH_BLUE);
//  GRAPH_DrawFilledCircle(50, 50, 50, GRAPH_BLUE);
//  GRAPH_SetFont(font21x39Info);
//  GRAPH_DrawChar('A', 120, 50, GRAPH_WHITE, GRAPH_RED);
//  GRAPH_DrawString("Hello World", 200, 0, GRAPH_WHITE, GRAPH_RED);
//  GRAPH_SetFont(font14x27Info);
//  GRAPH_DrawString("A mouse is", 240, 0, GRAPH_WHITE, GRAPH_RED);
//  GRAPH_SetFont(font10x20Info);
//  GRAPH_DrawString("not a lion.", 280, 0, GRAPH_WHITE, GRAPH_RED);
//  GRAPH_SetFont(font8x16Info);
//  GRAPH_DrawString("To be or not to be", 170, 0, GRAPH_WHITE, GRAPH_RED);

  // draw image test
//  TIMER_DelayMillis(3000);
//  /**
//   * @brief Example image to be drawn on screen.
//   */
//  GRAPH_ImageTypedef displayedImage = {
//      example_bmp,
//      192,
//      256,
//      3
//  };
//  GRAPH_ClearScreen(GRAPH_BLACK);
//  GRAPH_DrawImage(30, 30, &displayedImage, FALSE);

  // data for example graph - sinusoidal signal
  uint8_t graphData[320];
  double x = 0.0;

  for (int i = 0; i < 320; i++, x+=0.01*M_PI) {
    graphData[i] = (uint8_t)(sin(x)*100 + 100);
  }

  TIMER_DelayMillis(3000);
  GRAPH_ClearScreen(GRAPH_WHITE);
  GRAPH_DrawGraph(graphData, 290, 0, 0, GRAPH_RED, GRAPH_BLUE);

  GRAPH_DrawLine(50, 50, 200, 200, GRAPH_RED);


//   draw example bar chart
//  TIMER_DelayMillis(3000);
//  GRAPH_ClearScreen(0);
//  GRAPH_DrawBarChart(graphData+30, 32, 0, 0, 5);

  TSC2046_Initialize();
  // register an event for a given region
  TSC2046_RegisterEvent(0, 0, 1500, 4000, tscEvent1);
  TSC2046_RegisterEvent(0, 0, 4000, 1500, tscEvent2);

#endif


#ifdef TEST_SD
  FAT_Init(SD_Init, SD_ReadSectors, SD_WriteSectors);
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

  for (int k = 0; k < 5; k++) {
    data[k] = 'y';
  }

  char message[] = "Hello world, from STM32 to FAT driver"; // length 37

  FAT_MoveWrPtr(hello, 500);

  FAT_WriteFile(hello, (uint8_t*)message, strlen(message));
#endif


#ifdef USE_GUI
  GUI_Init();

  GUI_AddButton(50, 50, 50, 100, tscEvent1, "LED 0", 0x867474, GRAPH_WHITE);
  GUI_AddButton(200, 50, 50, 100, tscEvent2, "LED 1", 0x867474, GRAPH_WHITE);

#endif

  int position = 0;
  while (TRUE) {
//    GRAPH_DrawRectangle(position, 100, 50, 100, GRAPH_RED);
//    position+=1;
//    GRAPH_DrawRectangle(position-1, 100, 1, 100, GRAPH_BLACK);
//    TSC2046_Update(); // run touchscreen functions
//    TIMER_SoftTimersUpdate(); // run timers
  }
}
/**
 * @brief Example touchscreen event handler.
 * @param x X coordinate of touch.
 * @param y Y coordinate of touch.
 */
void tscEvent1(int x, int y) {
  println("Event 1");
  LED_Toggle(_LED0);
}
/**
 * @brief Example touchscreen event handler.
 * @param x X coordinate of touch.
 * @param y Y coordinate of touch.
 */
void tscEvent2(int x, int y) {
  println("Event 2");
  LED_Toggle(_LED1);
}

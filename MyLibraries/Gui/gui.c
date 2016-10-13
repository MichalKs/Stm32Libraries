/**
 * @file    gui.c
 * @brief   Graphical user interface library for touchscreen and TFT LCD
 * @date    16 gru 2014
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

#include "graphics.h"
#include "tsc2046.h"
#include "font_8x16.h"
#include "ili9320.h"
#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf(""str"%s",##args,"")
#define println(str, args...) printf("GUI--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

/**
 * @addtogroup GUI
 * @{
 */

static void convertLCD2TSC(int *x, int *y, int *width, int *height);

/**
 * @brief Initialize GUI.
 */
void GUI_Init(void) {
  TSC2046_Initialize();
  GRAPH_LcdDriverTypedef lcdDriver;
  lcdDriver.initialize = ILI9320_Initializtion;
  lcdDriver.setWindow = ILI9320_SetWindow;
  lcdDriver.drawPixel = ILI9320_DrawPixel;
  lcdDriver.drawNextPixel = ILI9320_DrawNextPixel;
  lcdDriver.setGramAddress = ILI9320_SetCursor;
  lcdDriver.width = 320;
  lcdDriver.height = 240;
  GRAPH_Initialize(&lcdDriver);
  GRAPH_SetFont(font8x16Info);
}
/**
 * @brief Adds a button to the GUI.
 *
 * @details All coordinates as per LCD (not TSC).
 *
 * @param x X coordinate of button origin.
 * @param y Y coordinate of button origin.
 * @param w Width of button
 * @param h Height of button
 * @param cb Callback for button press event.
 * @param text Description of button (shown on screen).
 */
void GUI_AddButton(int x, int y, int width, int height,
    void (*eventCb)(int x, int y), const char* buttonText, unsigned int buttonColor,
    unsigned int textColor) {

  GRAPH_DrawFilledRectangle(x,y,width,height, buttonColor);

  // TODO Derive position of button text from string and font size
  GRAPH_DrawString(buttonText, x+width/4, y+height/4, textColor, buttonColor);

  convertLCD2TSC(&x, &y, &width, &height);
  println("Creating button on event on x=%d,y=%d,w=%d,h=%d",
      x, y, width, height);

  TSC2046_RegisterEvent(x, y, width, height, eventCb);

}
/**
 * @brief Draws a label on screen
 * @param x X coordinate of label origin.
 * @param y Y coordinate of label origin.
 * @param w Maximum width of label
 * @param h Maximum height of label
 * @param text Label contents
 */
void GUI_AddLabel(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
    const char* text) {

}

/**
 * @brief Converts LCD coordinates (320x240) to TSC coordinates.
 *
 * @details The axes are interchanged between the two devices in
 * my setting. The X axis on the LCD corresponds to -Y on the TSC
 * and the Y axis on the LCD to the -X axis on the TSC.
 *
 * @param x X coordinate
 * @param y Y coordinate
 * @param w Width
 * @param h height
 */
static void convertLCD2TSC(int *x, int *y, int *width, int *height) {

  // basically I derived those manually
  // by analyzing touchscreen readings
  const int TSC_MAX_Y = 3800;
  const int TSC_MIN_Y = 400;
  const int TSC_MAX_X = 3700;
  const int TSC_MIN_X = 300;
  const int TSC_X_TOTAL_LENGTH = TSC_MAX_X - TSC_MIN_X;
  const int TSC_Y_TOTAL_LENGTH = TSC_MAX_Y - TSC_MIN_Y;

  const int LCD_WIDTH = 320;
  const int LCD_HEIGHT = 240;

  int touchscreenX, touchscreenY, touchscreenWidth, touchscreenHeight;
  int startPositionOnTouchscreenX, startPositionOnTouchscreenY;

  startPositionOnTouchscreenY = *x + *width;
  startPositionOnTouchscreenX = *y + *height;
  touchscreenWidth = *height;
  touchscreenHeight = *width;

  // Y axis is inverted X axis of the LCD
  touchscreenY = TSC_MAX_Y - startPositionOnTouchscreenY * TSC_Y_TOTAL_LENGTH/LCD_WIDTH;
  // X axis is inverted Y axis of the LCD
  touchscreenX = TSC_MAX_X - startPositionOnTouchscreenX * TSC_X_TOTAL_LENGTH/LCD_HEIGHT;

  *y = touchscreenY;
  *x = touchscreenX;

  *height = touchscreenHeight * TSC_Y_TOTAL_LENGTH/LCD_WIDTH;
  *width = touchscreenWidth * TSC_X_TOTAL_LENGTH/LCD_HEIGHT;

}

/**
 * @}
 */

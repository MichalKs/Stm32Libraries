/**
 * @file   ili9320.c
 * @brief  Library for ILI9320 TFT LCD driver.
 * @date   22 maj 2014
 * @author Michal Ksiezopolski
 * 
 * @details The ILI9320 driver can handle 512 pixels horizontally
 * by 256 pixels vertically. My display has only 320x240 pixels,
 * however - the data wraps around.
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

#include "ili9320.h"
#include "timers.h"
#include "ili9320_hal.h"
#include <stdio.h>

#define DEBUG_ILI9320

#ifdef DEBUG_ILI9320
#define print(str, args...) printf(""str"%s",##args,"")
#define println(str, args...) printf("ILI9320--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

/**
 * @addtogroup ILI9320
 * @{
 */

/**
 * @brief ILI9320 commands
 */
typedef enum {
  ILI9320_START_OSCILLATION = 0x00, //!< ILI9320_START_OSCILLATION
  ILI9320_READ_ID           = 0x00, //!< ILI9320_READ_ID
  ILI9320_DRIVER_OUTPUT     = 0x01, //!< ILI9320_DRIVER_OUTPUT
  ILI9320_DRIVING_WAVE      = 0x02, //!< ILI9320_DRIVING_WAVE
  ILI9320_ENTRY_MODE        = 0x03, //!< ILI9320_ENTRY_MODE
  ILI9320_RESIZE            = 0x04, //!< ILI9320_RESIZE
  ILI9320_DISP1             = 0x07, //!< ILI9320_DISP1
  ILI9320_DISP2             = 0x08, //!< ILI9320_DISP2
  ILI9320_DISP3             = 0x09, //!< ILI9320_DISP3
  ILI9320_DISP4             = 0x0a, //!< ILI9320_DISP4
  ILI9320_RGB_DISP1         = 0x0c, //!< ILI9320_RGB_DISP1
  ILI9320_FRAME_MARKER      = 0x0d, //!< ILI9320_FRAME_MARKER
  ILI9320_RGB_DISP2         = 0x0f, //!< ILI9320_RGB_DISP2
  ILI9320_POWER1            = 0x10, //!< ILI9320_POWER1
  ILI9320_POWER2            = 0x11, //!< ILI9320_POWER2
  ILI9320_POWER3            = 0x12, //!< ILI9320_POWER3
  ILI9320_POWER4            = 0x13, //!< ILI9320_POWER4
  ILI9320_HOR_GRAM_ADDR     = 0x20, //!< ILI9320_HOR_GRAM_ADDR
  ILI9320_VER_GRAM_ADDR     = 0x21, //!< ILI9320_VER_GRAM_ADDR
  ILI9320_WRITE_TO_GRAM     = 0x22, //!< ILI9320_WRITE_TO_GRAM
  ILI9320_POWER7            = 0x29, //!< ILI9320_POWER7
  ILI9320_FRAME_RATE        = 0x2b, //!< ILI9320_FRAME_RATE
  ILI9320_GAMMA1            = 0x30, //!< ILI9320_GAMMA1
  ILI9320_GAMMA2            = 0x31, //!< ILI9320_GAMMA2
  ILI9320_GAMMA3            = 0x32, //!< ILI9320_GAMMA3
  ILI9320_GAMMA4            = 0x35, //!< ILI9320_GAMMA4
  ILI9320_GAMMA5            = 0x36, //!< ILI9320_GAMMA5
  ILI9320_GAMMA6            = 0x37, //!< ILI9320_GAMMA6
  ILI9320_GAMMA7            = 0x38, //!< ILI9320_GAMMA7
  ILI9320_GAMMA8            = 0x39, //!< ILI9320_GAMMA8
  ILI9320_GAMMA9            = 0x3c, //!< ILI9320_GAMMA9
  ILI9320_GAMMA10           = 0x3d, //!< ILI9320_GAMMA10
  ILI9320_HOR_ADDR_START    = 0x50, //!< ILI9320_HOR_ADDR_START
  ILI9320_HOR_ADDR_END      = 0x51, //!< ILI9320_HOR_ADDR_END
  ILI9320_VER_ADDR_START    = 0x52, //!< ILI9320_VER_ADDR_START
  ILI9320_VER_ADDR_END      = 0x53, //!< ILI9320_VER_ADDR_END
  ILI9320_DRIVER_OUTPUT2    = 0x60, //!< ILI9320_DRIVER_OUTPUT2
  ILI9320_BASE_IMAGE        = 0x61, //!< ILI9320_BASE_IMAGE
  ILI9320_VERTICAL_SCROLL   = 0x6a, //!< ILI9320_VERTICAL_SCROLL
  ILI9320_PARTIAL1_POS      = 0x80, //!< ILI9320_PARTIAL1_POS
  ILI9320_PARTIAL1_START    = 0x81, //!< ILI9320_PARTIAL1_START
  ILI9320_PARTIAL1_END      = 0x82, //!< ILI9320_PARTIAL1_END
  ILI9320_PARTIAL2_POS      = 0x83, //!< ILI9320_PARTIAL2_POS
  ILI9320_PARTIAL2_START    = 0x84, //!< ILI9320_PARTIAL2_START
  ILI9320_PARTIAL2_END      = 0x85, //!< ILI9320_PARTIAL2_END
  ILI9320_PANEL_INTERFACE1  = 0x90, //!< ILI9320_PANEL_INTERFACE1
  ILI9320_PANEL_INTERFACE2  = 0x92, //!< ILI9320_PANEL_INTERFACE2
  ILI9320_PANEL_INTERFACE3  = 0x93, //!< ILI9320_PANEL_INTERFACE3
  ILI9320_PANEL_INTERFACE4  = 0x95, //!< ILI9320_PANEL_INTERFACE4
  ILI9320_PANEL_INTERFACE5  = 0x97, //!< ILI9320_PANEL_INTERFACE5
  ILI9320_PANEL_INTERFACE6  = 0x98, //!< ILI9320_PANEL_INTERFACE6
} ILI9320_CommandsTypedef;

/**
 * @brief Initialize the ILI9320 TFT LCD driver.
 */
void ILI9320_Initializtion(void) {

  const int RESET_DELAY = 50;
  const int INIT_DELAY  = 100;
  const int ILI9320_ID  = 0x9320;
  ILI9320_HAL_Initialize(); // GPIO and FSMC init

  // Reset the LCD
  ILI9320_HAL_ResetOff();
  Timer_delayMillis(RESET_DELAY);
  ILI9320_HAL_ResetOn();
  Timer_delayMillis(RESET_DELAY);
  ILI9320_HAL_ResetOff();
  Timer_delayMillis(RESET_DELAY);

  ILI9320_HAL_WriteReg(ILI9320_START_OSCILLATION, TRUE);
  Timer_delayMillis(RESET_DELAY);

  int id = ILI9320_HAL_ReadReg(ILI9320_READ_ID);

  println("ID TFT LCD = %x", id);

  // Add more LCD init codes here
  if (id == ILI9320_ID) {
    // SS = 1 - Data displayed from left to right
    const int SS_BIT = 0x0100;
    const int SM_BIT = 0x0400;
    ILI9320_HAL_WriteReg(ILI9320_DRIVER_OUTPUT, 0);

    // Line inversion - dunno what this does yet
    const int BC_BIT = 0x0600;
    const int EOR_BIT = 0x0100;
    ILI9320_HAL_WriteReg(ILI9320_DRIVING_WAVE, BC_BIT | EOR_BIT);

    const int GRAM_VERTICAL_UPDATE_DIRECTION_BIT = 0x0008;
    const int SWAP_RGB_TO_BGR_IN_GRAM_BIT = 0x1000;
    const int HORIZONTAL_INCREMENT_VERTICAL_DECREMENT = 0x0030;
    ILI9320_HAL_WriteReg(ILI9320_ENTRY_MODE,
        SWAP_RGB_TO_BGR_IN_GRAM_BIT | HORIZONTAL_INCREMENT_VERTICAL_DECREMENT);

    const int NO_RESIZE = 0;
    ILI9320_HAL_WriteReg(ILI9320_RESIZE, NO_RESIZE);

    ILI9320_HAL_WriteReg(ILI9320_DISP1, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_DISP2, 0x0202); // two lines back porch, two line front porch
    ILI9320_HAL_WriteReg(ILI9320_DISP3, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_DISP4, 0x0000);

    ILI9320_HAL_WriteReg(ILI9320_RGB_DISP1, 0x0001);

    ILI9320_HAL_WriteReg(ILI9320_FRAME_MARKER, 0x0000); // 0th line for frame marker
    ILI9320_HAL_WriteReg(ILI9320_RGB_DISP2, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_DISP1, 0x0101);
    ILI9320_HAL_WriteReg(ILI9320_POWER1, 0x10c0);
    ILI9320_HAL_WriteReg(ILI9320_POWER2, 0x0007);
    ILI9320_HAL_WriteReg(ILI9320_POWER3, 0x0110);
    ILI9320_HAL_WriteReg(ILI9320_POWER4, 0x0b00);
    ILI9320_HAL_WriteReg(ILI9320_POWER7, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_FRAME_RATE, 0x4010);

    // Set window
    ILI9320_HAL_WriteReg(ILI9320_HOR_ADDR_START, 0);
    ILI9320_HAL_WriteReg(ILI9320_HOR_ADDR_END, 239);
    ILI9320_HAL_WriteReg(ILI9320_VER_ADDR_START, 0);
    ILI9320_HAL_WriteReg(ILI9320_VER_ADDR_END, 319);
    ILI9320_HAL_WriteReg(ILI9320_DRIVER_OUTPUT2, 0x2700);
    ILI9320_HAL_WriteReg(ILI9320_BASE_IMAGE, 0x0001);
    ILI9320_HAL_WriteReg(ILI9320_VERTICAL_SCROLL, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_PARTIAL1_POS, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_PARTIAL1_START, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_PARTIAL1_END, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_PARTIAL2_POS, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_PARTIAL2_START, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_PARTIAL2_END, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_PANEL_INTERFACE1, 0x0010);
    ILI9320_HAL_WriteReg(ILI9320_PANEL_INTERFACE2, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_PANEL_INTERFACE3, 0x0001);
    ILI9320_HAL_WriteReg(ILI9320_PANEL_INTERFACE4, 0x0110);
    ILI9320_HAL_WriteReg(ILI9320_PANEL_INTERFACE5, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_PANEL_INTERFACE6, 0x0000);
    ILI9320_HAL_WriteReg(ILI9320_DISP1, 0x0173);
  }

  Timer_delayMillis(INIT_DELAY);
}
void ILI9320_SetHorizontalGramUpdateDirection(void) {
  ILI9320_HAL_WriteReg(ILI9320_ENTRY_MODE, 0x1000 | 0x0038);
}
void ILI9320_SetVerticalGramUpdateDirection(void) {
  const int GRAM_VERTICAL_UPDATE_DIRECTION_BIT = 0x0008;
  const int SWAP_RGB_TO_BGR_IN_GRAM_BIT = 0x1000;
  const int HORIZONTAL_INCREMENT_VERTICAL_DECREMENT = 0x0030;
  ILI9320_HAL_WriteReg(ILI9320_ENTRY_MODE,
      SWAP_RGB_TO_BGR_IN_GRAM_BIT | HORIZONTAL_INCREMENT_VERTICAL_DECREMENT);
}
/**
 * @brief Move cursor to given coordinates.
 * @param x X coordinate
 * @param y Y coordinate
 */
void ILI9320_SetCursor(int x, int y) {
  ILI9320_HAL_WriteReg(ILI9320_HOR_GRAM_ADDR, y);
  ILI9320_HAL_WriteReg(ILI9320_VER_GRAM_ADDR, x);
}
/**
 * @brief Draws a pixel on the LCD.
 * @param x X coordinate of pixel.
 * @param y Y coordinate of pixel.
 * @param rgbColor Color value.
 */
void ILI9320_DrawPixel(int x, int y, unsigned int rgbColor) {
  ILI9320_SetCursor(x, y);
  ILI9320_HAL_WriteReg(ILI9320_WRITE_TO_GRAM, rgbColor);
}
/**
 * @brief Draws a pixel on the LCD in the next address of the GRAM
 * @param rgbColor Color value.
 */
void ILI9320_DrawNextPixel(unsigned int rgbColor) {
  ILI9320_HAL_WriteReg(ILI9320_WRITE_TO_GRAM, rgbColor);
}
/**
 * @brief Set work window to draw data.
 * @param x X coordinate of start point.
 * @param y Y coordinate of start point.
 * @param width Width of window.
 * @param height Height of window.
 */
void ILI9320_SetWindow(int x, int y, int width, int height) {
  ILI9320_SetCursor(x, y);
  ILI9320_HAL_WriteReg(ILI9320_HOR_ADDR_START, y);
  ILI9320_HAL_WriteReg(ILI9320_HOR_ADDR_END, y + height - 1);
  ILI9320_HAL_WriteReg(ILI9320_VER_ADDR_START, x);
  ILI9320_HAL_WriteReg(ILI9320_VER_ADDR_END, x + width - 1);
}

/**
 * @}
 */

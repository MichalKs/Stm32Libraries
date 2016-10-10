/**
 * @file    graphics.c
 * @brief   Graphic library for TFT LCD.
 * @date    28 maj 2014
 * @author  Michal Ksiezopolski
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
#include "ili9320.h"
#include <math.h>
#include <string.h>

/**
 * @addtogroup GRAPHICS
 * @{
 */

#define RGB_TO_UNSIGNED_INT(red, green, blue) ((unsigned int)(((red)<<16)|((green)<<8)|(blue)))

GRAPH_LcdDriverTypedef lcdDriver;

/**
 * @brief Structure for reading BMP files
 */
typedef struct {
  uint16_t signature;
  uint32_t size;
  uint32_t reserved;
  uint32_t dataOffset;
  uint32_t headerSize;
  uint32_t width;
  uint32_t height;
  uint32_t planes;
  uint32_t bitsPerPixel;
  uint32_t compressionType;
  uint32_t imageSize;
  uint32_t resolutionH;
  uint32_t resolutionV;
  uint32_t colorsInImage;
  uint32_t importantColors;
} BMP_File;

static GRAPH_FontTypedef currentFont;         ///< Currently set font

/**
 * @brief Convert RGB value to 565 format.
 * @param rgbColor Color
 * @return Converted value of color in 565 format.
 */
unsigned int GRAPH_ConvertRgbTo565(unsigned int rgbColor) {
  const int RED_MASK = 0x1f;
  const int GREEN_MASK = 0x3f;
  const int BLUE_MASK = 0x1f;
  unsigned int red    = (rgbColor >> 19) & RED_MASK;
  unsigned int green  = (rgbColor >> 10) & GREEN_MASK;
  unsigned int blue   = (rgbColor >> 3) & BLUE_MASK;
  return (red << 11) | (green << 5) | (blue);
}
/**
 * @brief Initialized graphics - TFT LCD ILI9320.
 */
void GRAPH_Initialize(GRAPH_LcdDriverTypedef * driver) {
  lcdDriver = *driver;
  lcdDriver.initialize();
  GRAPH_ClearScreen(GRAPH_BLACK);
}
/**
 * @brief Clears the screen with given color.
 */
void GRAPH_ClearScreen(unsigned int rgbColor) {
  GRAPH_DrawFilledRectangle(0, 0, lcdDriver.width, lcdDriver.height, rgbColor);
}
/**
 * @brief Draws a rectangle (filled).
 * @param x X coordinate of start point
 * @param y Y coordinate of start point
 * @param width Width
 * @param height Height
 * @param color Color
 */
void GRAPH_DrawFilledRectangle(int x, int y, int width, int height, unsigned int color) {

  unsigned int convertedColor = GRAPH_ConvertRgbTo565(color);

  lcdDriver.setWindow(x, y, width, height);
  lcdDriver.setGramAddress(x, y);
  for (int i = 0; i < width * height; i++) {
    lcdDriver.drawNextPixel(convertedColor);
  }
}
/**
 * @brief Sets the currently used font.
 * @details This function should be called before attempting
 * to write a string to the LCD.
 * @param font Font information structure.
 */
void GRAPH_SetFont(GRAPH_FontTypedef font) {
  currentFont = font;
}
/**
 * @brief Draws an image on screen.
 * @param x X coordinate of top right corner.
 * @param y Y coordinate of top right corner.
 * TODO Make this more general. Implement flipping
 */
void GRAPH_DrawImage(int x, int y, GRAPH_ImageTypedef * image, Boolean flipped) {

  const int RED_PIXEL_DATA = 0;
  const int GREEN_PIXEL_DATA = 1;
  const int BLUE_PIXEL_DATA = 2;

  unsigned int red, green, blue, rgbColor;
  int currentPosition;
  lcdDriver.horizontalGramUpdate();
  lcdDriver.setWindow(x, y, image->columns, image->rows);
  lcdDriver.setGramAddress(x, y);

  for (int i = 0; i < image->rows; i++) {
    for (int j = 0; j < image->columns; j++) {
      currentPosition = (i*image->columns+j)*image->bytesPerPixel;
      red = image->data[currentPosition+RED_PIXEL_DATA];
      green = image->data[currentPosition+GREEN_PIXEL_DATA];
      blue = image->data[currentPosition+BLUE_PIXEL_DATA];
      rgbColor = RGB_TO_UNSIGNED_INT(red, green, blue);
      lcdDriver.drawNextPixel(GRAPH_ConvertRgbTo565(rgbColor));
    }
  }
  lcdDriver.verticalGramUpdate();
}
/**
 * @brief Draws a character on screen.
 * @param character Character to draw (ASCII code)
 * @param x X coordinate of character
 * @param y T coordinate of character
 */
void GRAPH_DrawChar(char character, int x, int y, unsigned int foregroundColor,
    unsigned int backgroundColor) {

  const int BITS_PER_BYTE = 8;

  // no font set
  if (currentFont.data == 0) {
    return;
  }

  unsigned int convertedForegroundColor = GRAPH_ConvertRgbTo565(foregroundColor);
  unsigned int convertedBackgroundColor = GRAPH_ConvertRgbTo565(backgroundColor);

  // Font usually skips first chars (useless)
  int rowInCharacterTable = character - currentFont.firstCharacter;

  // if nonexisting char
  if (rowInCharacterTable >= currentFont.numberOfCharacters) {
    return;
  }

  lcdDriver.setWindow(x, y,
      currentFont.columnCount, currentFont.bytesPerColumn * BITS_PER_BYTE);
  lcdDriver.setGramAddress(x, y);

  const int currentPosition = currentFont.columnCount *
      currentFont.bytesPerColumn * rowInCharacterTable; // first byte of row

  int bitmask;

  for (int i = 0; i < currentFont.columnCount; i++) {
    for (int j = 0; j < currentFont.bytesPerColumn; j++) {
      bitmask = 0x01; // start from lowest bit
      for (int k = 0; k < BITS_PER_BYTE; k++, bitmask <<= 1) { // for 8 bits in byte
        if (currentFont.data[currentPosition + i * currentFont.bytesPerColumn + j] & bitmask) {
          lcdDriver.drawNextPixel(convertedForegroundColor);
        } else {
          lcdDriver.drawNextPixel(convertedBackgroundColor);
        }
      }
    }
  }
}
/**
 * @brief Writes a string on the LCD
 * @param s String to write
 * @param x X coordinate
 * @param y Y coordinate
 * TODO Enable drawing vertical and horizontal strings.
 */
void GRAPH_DrawString(const char* stringToDisplay, int x, int y,
    unsigned int foregroundColor, unsigned int backgroundColor) {
  // skip columnCount pixel columns for next char
  for (unsigned int i = 0;
      i < strlen(stringToDisplay);
      i++, x += currentFont.columnCount) {
    GRAPH_DrawChar(stringToDisplay[i], x, y, foregroundColor, backgroundColor);
  }
}
/**
 * @brief Draws a box (empty rectangle).
 * @param x X coordinate of start point
 * @param y Y coordinate of start point
 * @param w Width
 * @param h Height
 * @param lineWidth Width of borders
 */
void GRAPH_DrawRectangle(int x, int y, int width, int height,
    int lineWidth, unsigned int color) {

  // Draw borders
  GRAPH_DrawFilledRectangle(x, y, lineWidth, height, color);
  GRAPH_DrawFilledRectangle(x+lineWidth, y, width-2*lineWidth, lineWidth, color);
  GRAPH_DrawFilledRectangle(x+width-lineWidth, y, lineWidth, height, color);
  GRAPH_DrawFilledRectangle(x+lineWidth, y+height-lineWidth, width-2*lineWidth,
      lineWidth, color);
}
/**
 * @brief Draws a graph portraying data (measurements, etc.).
 * @param data Buffer for displayed data.
 * @param len Length of data vector.
 * @param x X coordinate of start point
 * @param y Y coordinate of start point
 *
 * TODO Add graph scaling. Add axes and their
 * descriptions, graph title.
 *
 */
void GRAPH_DrawGraph(const uint8_t* data, int len, int x, int y,
    unsigned int foregroundColor, unsigned int backgroundColor) {

  const uint16_t xOffset = 30; // offset for axis and description
  x += xOffset;

  const uint16_t yOffset = 30; // offset for axis and description
  y += yOffset;

  const uint16_t maxDataLen = 320 - xOffset - 20;

  // X axis description
  GRAPH_DrawString("Voltage [V]", 5, 50, foregroundColor, backgroundColor);
  // X axis
  GRAPH_DrawLine(x-2, y-2, x-2, 230, foregroundColor);
  GRAPH_DrawLine(x-2, 230, x-12, 220, foregroundColor);
  GRAPH_DrawLine(x-2, 230, x+8, 220, foregroundColor);
  // Y axis
  GRAPH_DrawLine(x-2, y-2, 310, y-2, foregroundColor);
  GRAPH_DrawLine(310, y-2, 300, y-12, foregroundColor);
  GRAPH_DrawLine(310, y-2, 300, y+8, foregroundColor);

  if (len > maxDataLen)
    len = maxDataLen;

//  for (int i = 0; i < len; i++) {
//    // draw pixels up and down to make line more visible
//    lcdDriver.drawPixel(x+i,y+data[i]-1,foregroundColor);
//    lcdDriver.drawPixel(x+i,y+data[i], foregroundColor);
//    lcdDriver.drawPixel(x+i,y+data[i]+1,foregroundColor);
//  }

}
/**
 * @brief Draws a bar chart portraying data (measurements, etc.).
 * @param data Buffer for displayed data.
 * @param len Length of data vector.
 * @param x X coordinate of start point
 * @param y Y coordinate of start point
 * @param width Width of single bar.
 *
 * TODO Add graph scaling.
 *
 */
void GRAPH_DrawBarChart(const uint8_t* data, int lengthOfData,
    int x, int y, int widthOfSingleBar, unsigned int color) {

  int currentPosition = x;

  const int SPACE = 5; // space between bars

  for (int i = 0; i < lengthOfData; i++, currentPosition += widthOfSingleBar + SPACE) {
    // draw pixels up and down to make line more visible
    GRAPH_DrawFilledRectangle(currentPosition, 0, widthOfSingleBar, data[i], color);
  }
}
/**
 * @brief Draws a circle
 * @param x0 Center X coordinate.
 * @param y0 Center Y coordinate.
 * @param radius Circle radius.
 */
void GRAPH_DrawCircle(int x, int y, int radius, unsigned int color) {

  int newX = radius;
  int newY = 0;
  int error = 1-newX;
  unsigned int convertedColor = GRAPH_ConvertRgbTo565(color);

  while(newX >= newY) {
    lcdDriver.drawPixel(newX + x, newY + y, convertedColor);
    lcdDriver.drawPixel(newY + x, newX + y, convertedColor);
    lcdDriver.drawPixel(-newX + x, newY +  y, convertedColor);
    lcdDriver.drawPixel(-newY + x, newX + y, convertedColor);
    lcdDriver.drawPixel(-newX + x, -newY + y, convertedColor);
    lcdDriver.drawPixel(-newY + x, -newX + y, convertedColor);
    lcdDriver.drawPixel(newX + x, -newY + y, convertedColor);
    lcdDriver.drawPixel(newY + x, -newX + y, convertedColor);

    newY++;

    if (error<0) {
      error += 2 * newY + 1;
    } else {
      newX--;
      error += 2 * (newY - newX + 1);
    }
  }
}
/**
 * @brief Draws a filled circle
 * @param x0 Center X coordinate.
 * @param y0 Center Y coordinate.
 * @param radius Circle radius.
 * TODO Change implmentation - currenly not all pixels drawn
 */
void GRAPH_DrawFilledCircle(int x, int y, int radius, unsigned int color) {
  while (radius--) {
    GRAPH_DrawCircle(x,y,radius, color);
  }
}
/**
 * @brief This function draws a line.
 *
 * @param x1 Starting point X coordinate
 * @param y1 Starting point Y coordinate
 * @param x2 End point X coordinate
 * @param y2 End point Y coordinate
 */
void GRAPH_DrawLine(int x1, int y1, int x2, int y2, unsigned int foregroundColor) {

  int dx = x2>x1 ? (x2-x1) : (x1-x2); // slope
  int sx = x1<x2 ? 1 : -1; // sign
  int dy = y2>y1 ? (y2-y1) : (y1-y2); // slope
  int sy = y1<y2 ? 1 : -1; // sign

  int err = (dx>dy ? dx : -dy)/2; // error
  int tmpErr;

  unsigned int convertedColor = GRAPH_ConvertRgbTo565(foregroundColor);

  lcdDriver.setWindow(0, 0, lcdDriver.width, lcdDriver.height);
  while(TRUE) {

    lcdDriver.drawPixel(x1, y1, convertedColor);

    // if end of line
    if (x1==x2 && y1==y2) {
      break;
    }

    tmpErr = err;

    if (tmpErr >-dx) {
      err -= dy;
      x1 += sx;
    }

    if (tmpErr < dy) {
      err += dx;
      y1 += sy;
    }
  }
}

/**
 * @}
 */

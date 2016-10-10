/**
 * @file    graphics.h
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

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "utils.h"

/**
 * @defgroup  GRAPHICS GRAPHICS
 * @brief     Graphics library
 */

/**
 * @addtogroup GRAPHICS
 * @{
 */

/**
 * @brief Structure containing information about
 * a font.
 *
 * @details A font is assumed to be strcutred the following way.
 * The first bytesPerColumn bytes are for the first pixel column of
 * the font character. The next bytesPerColumn bytes are for the second
 * pixel column, etc. until columnCount is reached. The first pixel in a
 * column corresponds to the LSB of the first byte, so the MSB bits
 * of the last byte may not be used.
 *
 * TODO Ignore the MSB bits of last byte - this isn't very problematic
 * since for now we draw strings from top to bottom.
 *
 */
typedef struct {
  const uint8_t* data;        ///< Font pixel data
  uint8_t columnCount;        ///< How many columns does the font have (we assume every char is in different row)
  uint8_t bytesPerColumn;     ///< Number of bytes per columns
  uint8_t firstCharacter;     ///< First character in font in ASCII code
  uint8_t numberOfCharacters; ///< Number of characters in font
} GRAPH_FontTypedef;
/**
 * @brief Structure containing information about
 * an image.
 *
 * @details The image is assumed to be structured in the following way.
 * Image data starts in the top right corner. The first three bytes are
 * the RGB bytes of the last column of the first row, next three bytes
 * are the second to last column of row 1, etc. After column 1 is reached
 * the next row starts.
 */
typedef struct {
  const uint8_t* data;///< Image data
  int rows;           ///< Number of pixel rows
  int columns;        ///< Number of pixel columns
  int bytesPerPixel;  ///< Number of bytes per pixel
} GRAPH_ImageTypedef;

typedef struct {
  int width;
  int height;
  void (*initialize)(void);
  void (*drawPixel)(int x, int y, unsigned int rgbColor);
  void (*setGramAddress)(int x, int y);
  void (*drawNextPixel)(unsigned int rgbColor);
  void (*setWindow)(int x, int y, int width, int height);
  void (*horizontalGramUpdate)(void);
  void (*verticalGramUpdate)(void);
} GRAPH_LcdDriverTypedef;

typedef enum {
  GRAPH_BLACK = 0x000000,
  GRAPH_RED   = 0xff0000,
  GRAPH_BLUE  = 0x0000ff,
  GRAPH_GREEN = 0x00ff00,
  GRAPH_WHITE = 0xffffff,
} GRAPH_ColorTypedef;

void GRAPH_Initialize(GRAPH_LcdDriverTypedef * driver);
void GRAPH_ClearScreen(unsigned int rgbColor);
void GRAPH_SetFont(GRAPH_FontTypedef font);
void GRAPH_DrawChar(char character, int x, int y, unsigned int foregroundColor,
    unsigned int backgroundColor);
void GRAPH_DrawString(const char* s, int x, int y, unsigned int foregroundColor,
    unsigned int backgroundColor);
void GRAPH_DrawLine(int x1, int y1, int x2, int y2, unsigned int foregroundColor);
void GRAPH_DrawFilledRectangle(int x, int y, int width, int height, unsigned int color);
void GRAPH_DrawRectangle(int x, int y, int width, int height,
    int lineWidth, unsigned int color);
void GRAPH_DrawCircle(int x, int y, int radius, unsigned int color);
void GRAPH_DrawFilledCircle(int x, int y, int radius, unsigned int color);
void GRAPH_DrawImage(int x, int y, GRAPH_ImageTypedef * image, Boolean flipped);
void GRAPH_DrawGraph(const uint8_t* data, int len, int x, int y,
    unsigned int foregroundColor, unsigned int backgroundColor);
void GRAPH_DrawBarChart(const uint8_t* data, int lengthOfData,
    int x, int y, int widthOfSingleBar, unsigned int color);

/**
 * @}
 */

#endif /* GRAPHICS_H_ */

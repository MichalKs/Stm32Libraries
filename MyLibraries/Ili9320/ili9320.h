/**
 * @file    ili9320.h
 * @brief   Library for ILI9320 TFT LCD driver.
 * @date    22 maj 2014
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

#ifndef ILI9320_H_
#define ILI9320_H_

#include <inttypes.h>

/**
 * @defgroup  ILI9320 ILI9320
 * @brief     ILI9320 TFT LCD library
 */

/**
 * @addtogroup ILI9320
 * @{
 */

void ILI9320_Initializtion(void);
void ILI9320_SetWindow(int x, int y, int width, int height);
void ILI9320_DrawPixel(int x, int y, unsigned int color);
unsigned int ILI9320_RGBDecode(unsigned int rgbColor);

/**
 * @}
 */

#endif /* ILI9320_H_ */

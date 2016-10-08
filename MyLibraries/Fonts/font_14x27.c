/**
 * @file    font_14x27.c
 * @brief	  
 * @date    13 gru 2014
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

#include <font_14x27.h>

/**
 * @brief Font (14 pixels by 27 pixels)
 *
 * @details Every 4 bytes in an array row are for one pixel column.
 * There are 14 columns, which means there are
 * 56 bytes per one character (array row).
 * There are 96 characters.
 * The first pixel in a column is described by the LSB of the first
 * byte of the 5-byte column data.
 */
const uint8_t font14x27[5376] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0xE0, 0x7F, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char !
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char "
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x84, 0x38, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x70, 0x84, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x84, 0x38, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x70, 0x84, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char #
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0x03, 0x00, 0x80, 0x08, 0x01, 0x00, 0x40, 0x08, 0x02, 0x00, 0x40, 0x10, 0x02, 0x00, 0x70, 0x10, 0x3E, 0x00, 0x40, 0x10, 0x02, 0x00, 0xC0, 0x30, 0x03, 0x00, 0xC0, 0xE1, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char $
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x23, 0x00, 0x00, 0x60, 0x24, 0x00, 0x00, 0x20, 0x94, 0x07, 0x00, 0x20, 0xD4, 0x08, 0x00, 0x60, 0x54, 0x08, 0x00, 0xC0, 0x53, 0x08, 0x00, 0x00, 0x48, 0x08, 0x00, 0x00, 0x88, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char %
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x3F, 0x08, 0x00, 0x80, 0x21, 0x08, 0x00, 0x80, 0xC0, 0x08, 0x00, 0x80, 0x00, 0x07, 0x00, 0x80, 0x81, 0x05, 0x00, 0x80, 0x40, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char &
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char '
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x03, 0x00, 0x00, 0x07, 0x0C, 0x00, 0xC0, 0x00, 0x30, 0x00, 0x20, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char (
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x40, 0x00, 0xC0, 0x00, 0x30, 0x00, 0x00, 0x07, 0x0E, 0x00, 0x00, 0xFC, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char )
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char *
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char +
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char -
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char .
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char /
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x01, 0x00, 0xC0, 0x00, 0x06, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0xC0, 0x00, 0x06, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 0
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 1
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x0C, 0x00, 0x40, 0x00, 0x0A, 0x00, 0x20, 0x00, 0x09, 0x00, 0x20, 0x80, 0x08, 0x00, 0x20, 0x40, 0x08, 0x00, 0x20, 0x20, 0x08, 0x00, 0x20, 0x10, 0x08, 0x00, 0x40, 0x08, 0x08, 0x00, 0x80, 0x07, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 2
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x40, 0x00, 0x04, 0x00, 0x40, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x08, 0x08, 0x00, 0x20, 0x08, 0x08, 0x00, 0x20, 0x08, 0x04, 0x00, 0x40, 0x14, 0x06, 0x00, 0x80, 0xE3, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 3
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x86, 0x00, 0x00, 0x00, 0x81, 0x08, 0x00, 0xC0, 0x80, 0x08, 0x00, 0x20, 0x80, 0x08, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 0x00, 0x80, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 4
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0xE0, 0x0F, 0x04, 0x00, 0x20, 0x08, 0x00, 0x00, 0x20, 0x04, 0x08, 0x00, 0x20, 0x04, 0x08, 0x00, 0x20, 0x04, 0x08, 0x00, 0x20, 0x04, 0x08, 0x00, 0x20, 0x08, 0x04, 0x00, 0x00, 0x18, 0x06, 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 5
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x63, 0x03, 0x00, 0x80, 0x10, 0x04, 0x00, 0x40, 0x08, 0x08, 0x00, 0x40, 0x08, 0x08, 0x00, 0x20, 0x08, 0x08, 0x00, 0x20, 0x08, 0x08, 0x00, 0x20, 0x10, 0x04, 0x00, 0x20, 0xE0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 6
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x0C, 0x00, 0x20, 0x80, 0x03, 0x00, 0x20, 0x78, 0x00, 0x00, 0x20, 0x07, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC7, 0x03, 0x00, 0x40, 0x28, 0x04, 0x00, 0x20, 0x10, 0x08, 0x00, 0x20, 0x10, 0x08, 0x00, 0x20, 0x10, 0x08, 0x00, 0x20, 0x10, 0x08, 0x00, 0x40, 0x28, 0x04, 0x00, 0x80, 0xC7, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 8
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x20, 0x20, 0x08, 0x00, 0x20, 0x20, 0x08, 0x00, 0x20, 0x20, 0x08, 0x00, 0x20, 0x20, 0x04, 0x00, 0x40, 0x10, 0x02, 0x00, 0x80, 0x09, 0x01, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char 9
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x0E, 0x00, 0x00, 0x0E, 0x0E, 0x00, 0x00, 0x0E, 0x0E, 0x00, 0x00, 0x0E, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char :
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x0E, 0x07, 0x00, 0x00, 0x0E, 0x01, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ;
  0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00, 0x01, 0x04, 0x00, 0x80, 0x00, 0x08, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char <
  0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char =
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x08, 0x00, 0x80, 0x00, 0x08, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,  // Code for char >
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x0C, 0x00, 0x40, 0xE0, 0x0C, 0x00, 0x40, 0x20, 0x0C, 0x00, 0x80, 0x11, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ?
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0x07, 0x00, 0x40, 0x00, 0x08, 0x00, 0x20, 0x00, 0x10, 0x00, 0x10, 0x78, 0x10, 0x00, 0x10, 0x84, 0x10, 0x00, 0x10, 0x82, 0x10, 0x00, 0x20, 0x82, 0x08, 0x00, 0xC0, 0xFF, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char @
  0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x40, 0x00, 0x0C, 0x00, 0x40, 0x80, 0x0B, 0x00, 0x40, 0x70, 0x08, 0x00, 0x40, 0x4E, 0x00, 0x00, 0xC0, 0x41, 0x00, 0x00, 0xC0, 0x41, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x78, 0x08, 0x00, 0x00, 0xC0, 0x09, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00,  // Code for char A
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x80, 0x28, 0x08, 0x00, 0x00, 0x27, 0x04, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char B
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x80, 0x00, 0x04, 0x00, 0xC0, 0x00, 0x04, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x80, 0x00, 0x08, 0x00, 0x80, 0x00, 0x04, 0x00, 0xC0, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char C
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x80, 0x00, 0x04, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char D
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x7C, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0x81, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char E
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x7C, 0x08, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char F
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x01, 0x00, 0x00, 0x03, 0x02, 0x00, 0x80, 0x00, 0x04, 0x00, 0x80, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x40, 0x08, 0x00, 0x40, 0x40, 0x08, 0x00, 0x80, 0x40, 0x08, 0x00, 0x80, 0x40, 0x08, 0x00, 0xC0, 0xC1, 0x07, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,  // Code for char G
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char H
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char I
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x04, 0x00, 0xC0, 0xFF, 0x03, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char J
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x20, 0x08, 0x00, 0x40, 0x30, 0x08, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0xC2, 0x00, 0x00, 0x40, 0x01, 0x03, 0x00, 0xC0, 0x00, 0x0C, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char K
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0xC0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char L
  0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0xC0, 0x00, 0x08, 0x00, 0x00, 0x07, 0x08, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x07, 0x08, 0x00, 0xC0, 0x01, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00,  // Code for char M
  0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0xC0, 0x00, 0x08, 0x00, 0x00, 0x03, 0x08, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x40, 0x00, 0x03, 0x00, 0x40, 0x00, 0x0C, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,  // Code for char N
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x80, 0x00, 0x04, 0x00, 0x80, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x80, 0x00, 0x04, 0x00, 0x80, 0x00, 0x04, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char O
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x20, 0x08, 0x00, 0x40, 0x20, 0x08, 0x00, 0x40, 0x20, 0x08, 0x00, 0x40, 0x20, 0x08, 0x00, 0x40, 0x20, 0x00, 0x00, 0x40, 0x20, 0x00, 0x00, 0x80, 0x10, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char P
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x80, 0x00, 0x44, 0x00, 0x80, 0x00, 0x64, 0x00, 0x40, 0x00, 0x38, 0x00, 0x40, 0x00, 0x28, 0x00, 0x40, 0x00, 0x28, 0x00, 0x40, 0x00, 0x28, 0x00, 0x80, 0x00, 0x44, 0x00, 0x80, 0x00, 0x44, 0x00, 0x00, 0x03, 0x43, 0x00, 0x00, 0xFC, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char Q
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x20, 0x08, 0x00, 0x40, 0x20, 0x08, 0x00, 0x40, 0x20, 0x00, 0x00, 0x40, 0x20, 0x00, 0x00, 0x40, 0x60, 0x00, 0x00, 0x80, 0x90, 0x01, 0x00, 0x00, 0x0F, 0x02, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char R
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x07, 0x04, 0x00, 0x80, 0x08, 0x04, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x80, 0x20, 0x04, 0x00, 0xC0, 0xC3, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char S
  0x00, 0x00, 0x00, 0x00, 0xC0, 0x07, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xC0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char T
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0x03, 0x00, 0x40, 0x00, 0x04, 0x00, 0x40, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x04, 0x00, 0xC0, 0xFF, 0x03, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,  // Code for char U
  0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x40, 0x0E, 0x00, 0x00, 0x40, 0x70, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x40, 0xC0, 0x01, 0x00, 0x40, 0x3C, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,  // Code for char V
  0x40, 0x00, 0x00, 0x00, 0xC0, 0x07, 0x00, 0x00, 0x40, 0xF8, 0x07, 0x00, 0x40, 0x00, 0x08, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0xF8, 0x07, 0x00, 0xC0, 0x07, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,  // Code for char W
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x0E, 0x00, 0xC0, 0x00, 0x09, 0x00, 0x40, 0xC3, 0x08, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x40, 0xC3, 0x00, 0x00, 0xC0, 0x00, 0x09, 0x00, 0x40, 0x00, 0x0E, 0x00, 0x40, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char X
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x40, 0x03, 0x00, 0x00, 0x40, 0x04, 0x08, 0x00, 0x00, 0x18, 0x08, 0x00, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0x18, 0x08, 0x00, 0x40, 0x04, 0x08, 0x00, 0x40, 0x03, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char Y
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0xC0, 0x07, 0x0A, 0x00, 0x40, 0x00, 0x09, 0x00, 0x40, 0xC0, 0x08, 0x00, 0x40, 0x20, 0x08, 0x00, 0x40, 0x10, 0x08, 0x00, 0x40, 0x0C, 0x08, 0x00, 0x40, 0x02, 0x08, 0x00, 0x40, 0x01, 0x08, 0x00, 0xC0, 0x80, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char Z
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x7F, 0x00, 0x20, 0x00, 0x40, 0x00, 0x20, 0x00, 0x40, 0x00, 0x20, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char [
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char BackSlash
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x40, 0x00, 0x20, 0x00, 0x40, 0x00, 0x20, 0x00, 0x40, 0x00, 0xE0, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ]
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ^
  0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04,  // Code for char _
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char `
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x84, 0x08, 0x00, 0x00, 0x44, 0x08, 0x00, 0x00, 0x42, 0x08, 0x00, 0x00, 0x42, 0x08, 0x00, 0x00, 0x42, 0x08, 0x00, 0x00, 0x42, 0x04, 0x00, 0x00, 0x44, 0x04, 0x00, 0x00, 0xF8, 0x0F, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char a
  0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char b
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char c
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x20, 0x04, 0x04, 0x00, 0x20, 0x08, 0x02, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00,  // Code for char d
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x48, 0x02, 0x00, 0x00, 0x44, 0x04, 0x00, 0x00, 0x42, 0x08, 0x00, 0x00, 0x42, 0x08, 0x00, 0x00, 0x42, 0x08, 0x00, 0x00, 0x42, 0x08, 0x00, 0x00, 0x44, 0x08, 0x00, 0x00, 0x48, 0x04, 0x00, 0x00, 0x70, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char e
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0xC0, 0xFF, 0x0F, 0x00, 0x60, 0x02, 0x08, 0x00, 0x20, 0x02, 0x08, 0x00, 0x20, 0x02, 0x08, 0x00, 0x20, 0x02, 0x08, 0x00, 0x20, 0x02, 0x08, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char f
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x02, 0x08, 0x01, 0x00, 0x02, 0x08, 0x01, 0x00, 0x02, 0x08, 0x01, 0x00, 0x02, 0x08, 0x01, 0x00, 0x04, 0x04, 0x01, 0x00, 0x08, 0x82, 0x00, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char g
  0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0xF8, 0x0F, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char h
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x38, 0xFE, 0x0F, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char i
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x38, 0x02, 0x80, 0x00, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char j
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x0A, 0x09, 0x00, 0x00, 0x06, 0x0A, 0x00, 0x00, 0x02, 0x0C, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char k
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char l
  0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0xFE, 0x0F, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xFC, 0x0F, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xFC, 0x0F, 0x00, 0x00, 0x00, 0x08, 0x00,  // Code for char m
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0xFE, 0x0F, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0xF8, 0x0F, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char n
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char o
  0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0xFE, 0xFF, 0x01, 0x00, 0x08, 0x02, 0x01, 0x00, 0x04, 0x04, 0x01, 0x00, 0x02, 0x08, 0x01, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char p
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x01, 0x00, 0x04, 0x04, 0x01, 0x00, 0x08, 0x02, 0x01, 0x00, 0xFE, 0xFF, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,  // Code for char q
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0xFE, 0x0F, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char r
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x1C, 0x04, 0x00, 0x00, 0x22, 0x08, 0x00, 0x00, 0x22, 0x08, 0x00, 0x00, 0x22, 0x08, 0x00, 0x00, 0x22, 0x08, 0x00, 0x00, 0x22, 0x08, 0x00, 0x00, 0x44, 0x04, 0x00, 0x00, 0x8E, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char s
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0xE0, 0xFF, 0x03, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char t
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xFE, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00, 0xFE, 0x0F, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char u
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x72, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x72, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char v
  0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0xC2, 0x07, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x02, 0x08, 0x00, 0x00, 0xC2, 0x07, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,  // Code for char w
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x02, 0x0C, 0x00, 0x00, 0x06, 0x0A, 0x00, 0x00, 0x0A, 0x09, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00, 0x0A, 0x09, 0x00, 0x00, 0x06, 0x0A, 0x00, 0x00, 0x02, 0x0C, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char x
  0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x0E, 0x00, 0x01, 0x00, 0x32, 0x00, 0x01, 0x00, 0xC0, 0x81, 0x01, 0x00, 0x00, 0x66, 0x01, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0xC2, 0x01, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char y
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x0C, 0x00, 0x00, 0x02, 0x0A, 0x00, 0x00, 0x02, 0x09, 0x00, 0x00, 0x82, 0x08, 0x00, 0x00, 0x42, 0x08, 0x00, 0x00, 0x22, 0x08, 0x00, 0x00, 0x12, 0x08, 0x00, 0x00, 0x0A, 0x08, 0x00, 0x00, 0x06, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char z
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x80, 0xBF, 0x3F, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char |
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x80, 0xBF, 0x3F, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char }
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ~
  0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0x20, 0x00, 0x08, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00   // Code for char 
};

const GRAPH_FontTypedef font14x27Info = {
  font14x27,  // font data
  14,         // 21 columns
  4,          // 5 bytes per columns
  32,         // first char is 32 (space)
  96          // 96 chars
};

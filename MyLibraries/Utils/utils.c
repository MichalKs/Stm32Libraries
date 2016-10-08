/**
 * @file    utils.c
 * @brief   Utility and help functions.
 * @date    08.10.2016
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

#include "utils.h"
#include "timers.h"
#include <stdio.h>

/**
 * @addtogroup UTILS
 * @{
 */

/**
 * @brief Function determines byte order of given architecture.
 * @retval TRUE Architecture is big endian.
 * @retval FALSE Architecture is little endian.
 */
Boolean UTILS_IsArchitectureBigEndian(void) {
  const int i = TRUE;
  return (*(char*)&i) == FALSE;
}
/**
 * @brief Converts big endian value to host endianness.
 * @param value Value to convert
 * @return Converted value
 */
unsigned int UTILS_ConvertUnsignedIntToHostEndianness(unsigned int value) {

  const int NUMBER_OF_BYTES_IN_INT = 4;
  const int LAST_BYTE_IN_ARRAY = 3;

  // if we're on big endian arch
  // then do nothing
  if (UTILS_IsArchitectureBigEndian()) {
    return value;
  }

  // else convert to little endian
  unsigned int returnValue = 0;
  uint8_t* destination = (uint8_t*)&returnValue;
  uint8_t* source = (uint8_t*)&value;

  for (int i = 0; i < NUMBER_OF_BYTES_IN_INT; i++) {
    destination[i] = source[LAST_BYTE_IN_ARRAY-i];
  }

  return returnValue;
}
/**
 * @brief Send data in hex format to terminal.
 * @param dataBuffer Data buffer.
 * @param length Number of bytes to send.
 * @warning Uses blocking delays so as not to overflow buffer.
 */
void UTILS_Hexdump(const uint8_t const *dataBuffer, int length) {

  const int MAXIMUM_CHARACTERS_IN_LINE = 16;
  const int DELAY_TIME_MILLIS = 50;
  const int DELAY_TIME = 100;
  int i = 0;

  while (length--) {

    printf("%02x ", dataBuffer[i]);
    i++;

    if ((i % MAXIMUM_CHARACTERS_IN_LINE) == 0) {
      printf(NEWLINE_SEQUENCE);
    }

    if ((i % DELAY_TIME_MILLIS) == 0) {
      TIMER_DelayMillis(DELAY_TIME); // Delay so as not to overflow buffer
    }
  }
  printf(NEWLINE_SEQUENCE);
}
/**
 * @brief Send data in hex and ASCII format to terminal.
 * @param dataBuffer Data buffer.
 * @param length Number of bytes to send.
 * @warning Uses blocking delays so as not to overflow buffer.
 */
void UTILS_HexdumpWithCharacters(const uint8_t const *dataBuffer, int length) {

  const int MAXIMUM_CHARACTERS_IN_LINE = 8;
  const int DELAY_TIME_MILLIS = 50;
  const int DELAY_TIME = 100;
  const char LOWEST_CHARACTER_TO_DISPLAY = ' ';
  const char HIGHEST_CHARACTER_TO_DISPLAY = '~';
  const char PLACEHOLDER_FOR_NONDISPLAYED_VALUES = '.';
  int i = 0;

  while (length--) {

    if ((dataBuffer[i] >= LOWEST_CHARACTER_TO_DISPLAY) &&
        (dataBuffer[i] <= HIGHEST_CHARACTER_TO_DISPLAY)) {
      printf("%02x %c ", dataBuffer[i], dataBuffer[i]);
    } else { // nonalphanumeric as dot
      printf("%02x %c ", dataBuffer[i], PLACEHOLDER_FOR_NONDISPLAYED_VALUES);
    }

    i++;

    if ((i % MAXIMUM_CHARACTERS_IN_LINE) == 0) {
      printf(NEWLINE_SEQUENCE);
    }

    if ((i % DELAY_TIME_MILLIS) == 0) {
      TIMER_DelayMillis(DELAY_TIME); // Delay so as not to overflow buffer
    }
  }
  printf(NEWLINE_SEQUENCE);
}
/**
 * @brief Send data in hex format to terminal.
 * @param dataBuffer Data buffer.
 * @param length Number of bytes to send.
 * @warning Uses blocking delays so as not to overflow buffer.
 */
void UTILS_Hexdump16(const uint16_t const *dataBuffer, int length) {

  const int MAXIMUM_CHARACTERS_IN_LINE = 8;
  const int DELAY_TIME_MILLIS = 50;
  const int DELAY_TIME = 100;
  int i = 0;

  while (length--) {

    printf("%04x", dataBuffer[i], dataBuffer[i]);
    i++;

    if ((i % MAXIMUM_CHARACTERS_IN_LINE) == 0) {
      printf(NEWLINE_SEQUENCE);
    }

    if ((i % DELAY_TIME_MILLIS) == 0) {
      TIMER_DelayMillis(DELAY_TIME); // Delay so as not to overflow buffer
    }
  }
  printf(NEWLINE_SEQUENCE);
  
}
/**
 * @}
 */

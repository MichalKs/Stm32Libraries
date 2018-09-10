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

#ifndef UTILS_H_
#define UTILS_H_

#include <inttypes.h>

/**
 * @defgroup  UTILS UTILS
 * @brief     Common utility functions and macros
 */

/**
 * @addtogroup UTILS
 * @{
 */

#define NEWLINE_SEQUENCE "\r\n"   ///< Sequence to send to get new line in terminal
#define NUMBER_OF_BITS_IN_BYTE 8
#define IS_EVEN(x) ((x) % 2 == 0)

/**
 * @brief Boolean type for flags
 */
typedef enum {
  FALSE = 0,  //!< FALSE
  TRUE  = 1   //!< TRUE
} Boolean;

typedef enum {
  RESULT_OK,
} ResultCode;

void Utils_hexdump(const uint8_t * dataBuffer, int length);
void Utils_hexdumpWithCharacters(const uint8_t * dataBuffer, int length);
void Utils_hexdump16(const uint16_t * dataBuffer, int length);
unsigned int Utils_convertUnsignedIntToHostEndianness(unsigned int value);
Boolean Utils_isArchitectureBigEndian(void);

/**
 * @}
 */

#endif /* UTILS_H_ */

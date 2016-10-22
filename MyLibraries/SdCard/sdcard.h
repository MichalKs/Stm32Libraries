/**
 * @file    sdcard.h
 * @brief   SD card control functions.
 * @date    22 kwi 2014
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

#ifndef SDCARD_H_
#define SDCARD_H_

#include <inttypes.h>

/**
 * @defgroup  SD_CARD SD CARD
 * @brief     SD Card library functions
 */

/**
 * @addtogroup SD_CARD
 * @{
 */

typedef enum {
  SD_NO_ERROR = 0,
  SD_RESPONSE_ERROR,
  SD_INIT_FAILED,
  SD_CMD_ERROR,
  SD_BLOCK_READ_ERROR,
  SD_BLOCK_WRITE_ERROR,
  SD_CARD_NOT_INITALIZED,
} SD_CardErrorsTypedef;

int SD_Initialize   (void);
int SD_ReadSectors  (uint8_t* buf, uint32_t sector, uint32_t count);
int SD_WriteSectors (uint8_t* buf, uint32_t sector, uint32_t count);
uint64_t SD_ReadCapacity(void);

/**
 * @}
 */

#endif /* SDCARD_H_ */

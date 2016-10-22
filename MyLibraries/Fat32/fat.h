/**
 * @file    fat.h
 * @brief   FAT file system implementation.
 * @date    23.10.2016
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

#ifndef FAT_H_
#define FAT_H_

#include <inttypes.h>

/**
 * @defgroup  FAT FAT
 * @brief     FAT filesystem functions.
 */

/**
 * @addtogroup FAT
 * @{
 */

typedef enum {
  FAT_NO_ERROR = 0,
  FAT_INVALID_MBR_ERROR = -100,
  FAT_INVALID_PARTITION_ERROR,
  FAT_HAL_ERROR,
  FAT_HAL_READ_ERROR,
  FAT_HAL_WRITE_ERROR,
  FAT_TOO_MANY_FILES,
  FAT_WRONG_PARTITION_SIZE,
  FAT_INCOMPATIBLE_SECTOR_LENGTH,
} FAT_ErrorTypedef;

int FAT_Init(int (*phyInit)(void),
    int (*phyReadSectors)(uint8_t* buf, uint32_t sector, uint32_t count),
    int (*phyWriteSectors)(uint8_t* buf, uint32_t sector, uint32_t count));
int FAT_OpenFile(const char* filename);
int FAT_ReadFile(int file, uint8_t* data, int count);
int FAT_MoveRdPtr(int file, int newWrPtr);
int FAT_MoveWrPtr(int file, int newWrPtr);
int FAT_WriteFile(int file, const uint8_t* data, int count);

/**
 * @}
 */

#endif /* FAT_H_ */

/**
 * @file:   ili9320_hal.c
 * @brief:  Hardware abstraction layer for ILI9320
 * @date:   08.10.2016
 * @author: Michal Ksiezopolski
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
#ifndef INC_ILI9320_HAL_H_
#define INC_ILI9320_HAL_H_

#include <inttypes.h>

uint16_t  ILI9320_HAL_ReadReg   (uint16_t address);
void      ILI9320_HAL_Initialize(void);
void      ILI9320_HAL_WriteReg  (uint16_t address, uint16_t data);
void      ILI9320_HAL_ResetOn   (void);
void      ILI9320_HAL_ResetOff  (void);

#endif /* INC_ILI9320_HAL_H_ */

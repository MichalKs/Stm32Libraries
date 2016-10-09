/**
 * @file    spi_hal.h
 * @brief	  
 * @date    09.10.2016
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
#ifndef INC_SPI3_H_
#define INC_SPI3_H_

#include <inttypes.h>

/**
 * @defgroup  SPI SPI
 * @brief     SPI control functions
 */

/**
 * @addtogroup SPI
 * @{
 */

typedef enum {
  SPI_HAL_SPI1,
  SPI_HAL_SPI2,
  SPI_HAL_SPI3,
} SPI_HAL_Typedef;

void    SPI_HAL_Init        (SPI_HAL_Typedef spi);
void    SPI_HAL_Select      (SPI_HAL_Typedef spi);
void    SPI_HAL_Deselect    (SPI_HAL_Typedef spi);
void SPI_HAL_ReadBuffer(SPI_HAL_Typedef spi, uint8_t* receiveBuffer, int length);
void SPI_HAL_SendBuffer(SPI_HAL_Typedef spi, uint8_t* transmitBuffer, int length);
void SPI_HAL_TransmitBuffer(SPI_HAL_Typedef spi, uint8_t* receiveBuffer,
    uint8_t* transmitBuffer, int length);

/**
 * @}
 */

#endif /* INC_SPI3_H_ */

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

#define ILI9320_ADDRESS   (*((volatile unsigned short *) 0x60000000)) ///< Address for writing register number
#define ILI9320_DATA      (*((volatile unsigned short *) 0x60020000)) ///< Address for writing data
/**
 * @brief Function for writing data to ILI9320 through FSMC.
 * @param address Register address.
 * @param data Data to write.
 */
static inline void ILI9320_HAL_WriteReg(uint16_t address, uint16_t data) {
  ILI9320_ADDRESS = address;
  ILI9320_DATA = data;
}
/**
 * @brief Function for reading a given register.
 * @param address Register address.
 * @return Contents of register.
 */
static inline uint16_t ILI9320_HAL_ReadReg(uint16_t address) {
  ILI9320_ADDRESS = address;
  return ILI9320_DATA;
}
/**
 * @brief Sets address to write to the display
 * @param address New address
 */
static inline void ILI9320_HAL_WriteAddress(uint16_t address) {
  ILI9320_ADDRESS = address;
}
/**
 * @brief Sends data to the display
 * @param data Data to send
 */
static inline void ILI9320_HAL_WriteData(uint16_t data) {
  ILI9320_DATA = data;
}
/**
 * @brief Sends data buffer to display
 * @param dataToWrite Data to write
 * @param length Length of data to write
 */
static inline void ILI9320_HAL_WriteDataBuffer(uint16_t* dataToWrite, int length) {
  for (int i = 0; i < length; i++) {
    ILI9320_DATA = dataToWrite[i];
  }
}
/**
 * @brief Reads data from the display
 * @param readData Read data buffer
 * @param length Length of data to read
 */
static inline void ILI9320_HAL_ReadDataBuffer(uint16_t* readData, int length) {
  for (int i = 0; i < length; i++) {
    readData[i] = ILI9320_DATA;
  }
}
void      ILI9320_HAL_Initialize(void);
void      ILI9320_HAL_ResetOn   (void);
void      ILI9320_HAL_ResetOff  (void);

#endif /* INC_ILI9320_HAL_H_ */

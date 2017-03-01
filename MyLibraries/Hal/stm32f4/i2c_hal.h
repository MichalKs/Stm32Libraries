/*
 * i2c.h
 *
 *  Created on: 01.03.2017
 *      Author: mik
 */

#ifndef MYLIBRARIES_HAL_STM32F4_I2C_HAL_H_
#define MYLIBRARIES_HAL_STM32F4_I2C_HAL_H_

#include "utils.h"

typedef enum {
  I2C_HAL_RESULT_OK,
  I2C_HAL_NO_RESPONSE,
  I2C_HAL_WRONG_BUS,
} I2cHalResultCode;

typedef enum {
  I2C_HAL_I2C1,
} I2cNumber;

void I2c_initialize(I2cNumber i2cNumber);
I2cHalResultCode I2c_sendBuffer(I2cNumber i2cNumber, int i2cAddress, uint8_t * dataToSend,
    int length);
I2cHalResultCode I2c_readBuffer(I2cNumber i2cNumber, int i2cAddress, uint8_t * readBuffer,
    int length);

#endif /* MYLIBRARIES_HAL_STM32F4_I2C_HAL_H_ */

/*
 * i2c.c
 *
 *  Created on: 01.03.2017
 *      Author: mik
 */

#include "i2c_hal.h"
#include "common_hal.h"
#include <stm32f4xx_hal.h>

#define I2C1_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define I2C1_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C1_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C1_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define I2C1_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()
#define I2C1_SCL_PIN                    GPIO_PIN_6
#define I2C1_SCL_GPIO_PORT              GPIOB
#define I2C1_SCL_AF                     GPIO_AF4_I2C1
#define I2C1_SDA_PIN                    GPIO_PIN_7
#define I2C1_SDA_GPIO_PORT              GPIOB
#define I2C1_SDA_AF                     GPIO_AF4_I2C1

static I2C_HandleTypeDef i2c1Handle; /// Handle for I2C

#define I2C_MAXIMUM_DELAY_MILLIS 100

/**
 * @brief Initialize I2C
 */
void I2c_initialize(I2cNumber i2cNumber) {

  I2C_HandleTypeDef * i2cHandle;
  switch(i2cNumber) {
  case I2C_HAL_I2C1:
    i2cHandle = &i2c1Handle;
    i2cHandle->Instance = I2C1;
    break;
  default:
    return;
  }

  i2cHandle->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  i2cHandle->Init.ClockSpeed      = 500;
  i2cHandle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  i2cHandle->Init.DutyCycle       = I2C_DUTYCYCLE_2;
  i2cHandle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  i2cHandle->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
  i2cHandle->Init.OwnAddress1     = 0;
  i2cHandle->Init.OwnAddress2     = 0;
  if(HAL_I2C_Init(i2cHandle) != HAL_OK) {
    CommonHal_errorHandler();
  }
}
/**
 * @brief Send data to I2C
 * @param i2cAddress Address of peripheral on I2C bus
 * @param dataToSend Data to send
 * @param length Length of data
 * @return Result code
 */
I2cHalResultCode I2c_sendBuffer(I2cNumber i2cNumber, int i2cAddress, uint8_t * dataToSend,
    int length) {

  I2C_HandleTypeDef * i2cHandle;
  switch(i2cNumber) {
  case I2C_HAL_I2C1:
    i2cHandle = &i2c1Handle;
    break;
  default:
    return I2C_HAL_WRONG_BUS;
  }

  if(HAL_I2C_Master_Transmit(i2cHandle, (uint16_t)i2cAddress,
       (uint8_t*)dataToSend, length, I2C_MAXIMUM_DELAY_MILLIS) != HAL_OK) {
    return I2C_HAL_NO_RESPONSE;
  }
  return I2C_HAL_RESULT_OK;
}

/**
 * @brief Read data from I2C
 * @param i2cAddress Address of peripheral on I2C bus
 * @param readBuffer Buffer to receive data
 * @param length Length of data to receive
 * @return Result code
 */
I2cHalResultCode I2c_readBuffer(I2cNumber i2cNumber, int i2cAddress, uint8_t * readBuffer,
    int length) {

  I2C_HandleTypeDef * i2cHandle;
  switch(i2cNumber) {
  case I2C_HAL_I2C1:
    i2cHandle = &i2c1Handle;
    break;
  default:
    return I2C_HAL_WRONG_BUS;
  }

  while(HAL_I2C_Master_Receive(i2cHandle, (uint16_t)i2cAddress,
       (uint8_t*)readBuffer, length, I2C_MAXIMUM_DELAY_MILLIS) != HAL_OK) {
    return I2C_HAL_NO_RESPONSE;
  }
  return I2C_HAL_RESULT_OK;
}
/**
  * @brief I2C MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param hi2c: I2C handle pointer
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef * i2cHandle) {
  I2C1_SCL_GPIO_CLK_ENABLE();
  I2C1_SDA_GPIO_CLK_ENABLE();
  I2C1_CLK_ENABLE();
  GPIO_InitTypeDef  gpioInitialization;
  gpioInitialization.Pin       = I2C1_SCL_PIN;
  gpioInitialization.Mode      = GPIO_MODE_AF_OD;
  gpioInitialization.Pull      = GPIO_PULLUP;
  gpioInitialization.Speed     = GPIO_SPEED_FAST;
  gpioInitialization.Alternate = I2C1_SCL_AF;
  HAL_GPIO_Init(I2C1_SCL_GPIO_PORT, &gpioInitialization);
  gpioInitialization.Pin = I2C1_SDA_PIN;
  gpioInitialization.Alternate = I2C1_SDA_AF;
  HAL_GPIO_Init(I2C1_SDA_GPIO_PORT, &gpioInitialization);
}
/**
  * @brief I2C MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hi2c: I2C handle pointer
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef * i2cHandle) {
  I2C1_FORCE_RESET();
  I2C1_RELEASE_RESET();
  HAL_GPIO_DeInit(I2C1_SCL_GPIO_PORT, I2C1_SCL_PIN);
  HAL_GPIO_DeInit(I2C1_SDA_GPIO_PORT, I2C1_SDA_PIN);
}

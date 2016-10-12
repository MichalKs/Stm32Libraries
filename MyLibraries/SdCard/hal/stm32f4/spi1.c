/**
 * @file    spi1.c
 * @brief   SPI control functions
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

#include <spi1.h>
#include <stm32f4xx.h>
#include "common_hal.h"

/**
 * @addtogroup SPI1
 * @{
 */
static SPI_HandleTypeDef spi1Handle;
#define SPI1_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPI1_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
#define SPI1_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()
#define SPI1_SCK_PIN                     GPIO_PIN_5
#define SPI1_SCK_GPIO_PORT               GPIOA
#define SPI1_SCK_AF                      GPIO_AF5_SPI1
#define SPI1_MISO_PIN                    GPIO_PIN_6
#define SPI1_MISO_GPIO_PORT              GPIOA
#define SPI1_MISO_AF                     GPIO_AF5_SPI1
#define SPI1_MOSI_PIN                    GPIO_PIN_7
#define SPI1_MOSI_GPIO_PORT              GPIOA
#define SPI1_MOSI_AF                     GPIO_AF5_SPI1
#define SPI1_CS_PIN                      GPIO_PIN_4
#define SPI1_CS_PORT                     GPIOA
/**
 * @brief Initialize SPI1 and SS pin.
 */
void SPI1_Init(void) {
  GPIO_InitTypeDef  gpioInitialization;
  SPI1_SCK_GPIO_CLK_ENABLE();
   SPI1_MISO_GPIO_CLK_ENABLE();
   SPI1_MOSI_GPIO_CLK_ENABLE();
   SPI1_CS_GPIO_CLK_ENABLE();
   SPI1_CLK_ENABLE();

   gpioInitialization.Pin       = SPI1_SCK_PIN;
   gpioInitialization.Mode      = GPIO_MODE_AF_PP;
   gpioInitialization.Pull      = GPIO_NOPULL;
   gpioInitialization.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
   gpioInitialization.Alternate = SPI1_SCK_AF;
   HAL_GPIO_Init(SPI1_SCK_GPIO_PORT, &gpioInitialization);

   gpioInitialization.Pin = SPI1_MISO_PIN;
   gpioInitialization.Alternate = SPI1_MISO_AF;
   HAL_GPIO_Init(SPI1_MISO_GPIO_PORT, &gpioInitialization);

   gpioInitialization.Pin = SPI1_MOSI_PIN;
   gpioInitialization.Alternate = SPI1_MOSI_AF;
   HAL_GPIO_Init(SPI1_MOSI_GPIO_PORT, &gpioInitialization);

   gpioInitialization.Pin    = SPI1_CS_PIN;
   gpioInitialization.Mode   = GPIO_MODE_OUTPUT_PP;
   gpioInitialization.Speed  = GPIO_SPEED_FREQ_VERY_HIGH;
   gpioInitialization.Pull   = GPIO_NOPULL;
   HAL_GPIO_Init(SPI1_CS_PORT, &gpioInitialization);
   HAL_GPIO_WritePin(SPI1_CS_PORT, SPI1_CS_PIN, GPIO_PIN_SET);
  SPI_HandleTypeDef *currentHandle;

  currentHandle = &spi1Handle;
  currentHandle->Instance = SPI1;
  currentHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;

  currentHandle->Init.Direction         = SPI_DIRECTION_2LINES;
  currentHandle->Init.CLKPhase          = SPI_PHASE_1EDGE;
  currentHandle->Init.CLKPolarity       = SPI_POLARITY_LOW;
  currentHandle->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  currentHandle->Init.CRCPolynomial     = 7;
  currentHandle->Init.DataSize          = SPI_DATASIZE_8BIT;
  currentHandle->Init.FirstBit          = SPI_FIRSTBIT_MSB;
  currentHandle->Init.NSS               = SPI_NSS_SOFT;
  currentHandle->Init.TIMode            = SPI_TIMODE_DISABLE;
  currentHandle->Init.Mode              = SPI_MODE_MASTER;

  if(HAL_SPI_Init(currentHandle) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }
}
/**
 * @brief Select chip.
 */
void SPI1_Select(void) {

  HAL_GPIO_WritePin(SPI1_CS_PORT, SPI1_CS_PIN, GPIO_PIN_RESET);

}
/**
 * @brief Deselect chip.
 */
void SPI1_Deselect(void) {

  HAL_GPIO_WritePin(SPI1_CS_PORT, SPI1_CS_PIN, GPIO_PIN_SET);
}
/**
 * @brief Transmit data on SPI1
 * @param data Sent data
 * @return Received data
 * @warning Blocking function!
 */
uint8_t SPI1_Transmit(uint8_t data) {

  uint8_t i;

  HAL_SPI_TransmitReceive(&spi1Handle, &data, &i, 1, 5000);

  // Loop while transmit register in not empty
//  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//
//  SPI_I2S_SendData(SPI1, data); // Send byte (start transmit)
//
//  // Wait for new data (transmit end)
//  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//
//  i = SPI_I2S_ReceiveData(SPI1); // Received data

  return i;
}
/**
 * @brief Send multiple data on SPI1.
 * @param buf Buffer to send.
 * @param len Number of bytes to send.
 * @warning Blocking function!
 */
void SPI1_SendBuffer(uint8_t* buf, uint32_t len) {

  while (len--) {
    SPI1_Transmit(*buf++);
  }
}
/**
 * @brief Read multiple data on SPI1.
 * @param buf Buffer to place read data.
 * @param len Number of bytes to read.
 * @warning Blocking function!
 */
void SPI1_ReadBuffer(uint8_t* buf, uint32_t len) {

  while (len--) {
    *buf++ = SPI1_Transmit(0xff);
  }
}
/**
 * @brief Write multiple data on SPI1.
 * @param buf Buffer with write data.
 * @param len Number of bytes to write.
 * @warning Blocking function!
 */
void SPI1_WriteBuffer(uint8_t* buf, uint32_t len) {

  while (len--) {
    SPI1_Transmit(*buf++);
  }
}
/**
 * @brief Transmit multiple data on SPI1.
 * @param rx_buf Receive buffer.
 * @param tx_buf Transmit buffer.
 * @param len Number of bytes to transmit.
 */
void SPI1_TransmitBuffer(uint8_t* rx_buf, uint8_t* tx_buf, uint32_t len) {

  while (len--) {
    *rx_buf = SPI1_Transmit(*tx_buf);
    tx_buf++;
    rx_buf++;
  }
}

/**
 * @}
 */

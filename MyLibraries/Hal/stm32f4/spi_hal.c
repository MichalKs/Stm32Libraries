/**
 * @file    spi_hal.c
 * @brief   HAL driver for SPI
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

#include "spi_hal.h"
#include "common_hal.h"
#include <stm32f4xx.h>

/**
 * @addtogroup SPI
 * @{
 */
#define SPI3_CLK_ENABLE()                __HAL_RCC_SPI3_CLK_ENABLE()
#define SPI3_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPI3_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPI3_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPI3_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI3_FORCE_RESET()               __HAL_RCC_SPI3_FORCE_RESET()
#define SPI3_RELEASE_RESET()             __HAL_RCC_SPI3_RELEASE_RESET()
#define SPI3_SCK_PIN                     GPIO_PIN_10
#define SPI3_SCK_GPIO_PORT               GPIOC
#define SPI3_SCK_AF                      GPIO_AF6_SPI3
#define SPI3_MISO_PIN                    GPIO_PIN_11
#define SPI3_MISO_GPIO_PORT              GPIOC
#define SPI3_MISO_AF                     GPIO_AF6_SPI3
#define SPI3_MOSI_PIN                    GPIO_PIN_12
#define SPI3_MOSI_GPIO_PORT              GPIOC
#define SPI3_MOSI_AF                     GPIO_AF6_SPI3
#define SPI3_CS_PIN                      GPIO_PIN_15
#define SPI3_CS_PORT                     GPIOA

static SPI_HandleTypeDef spi1Handle;
static SPI_HandleTypeDef spi3Handle;

#define SPI_MAX_DELAY_TIME 500 ///< Maximum delay for polling mode

/**
 * @brief Initialize SPI and SS pin.
 */
void SPI_HAL_Init(SPI_HAL_Typedef spi) {

  SPI_HandleTypeDef *currentHandle;

  switch (spi) {
  case SPI_HAL_SPI1:
    currentHandle = &spi1Handle;
    spi3Handle.Instance = SPI1;
    break;
  case SPI_HAL_SPI3:
    currentHandle = &spi3Handle;
    spi3Handle.Instance = SPI3;
    break;

  default:
    return;
  }

  spi3Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  spi3Handle.Init.Direction         = SPI_DIRECTION_2LINES;
  spi3Handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
  spi3Handle.Init.CLKPolarity       = SPI_POLARITY_LOW;
  spi3Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  spi3Handle.Init.CRCPolynomial     = 7;
  spi3Handle.Init.DataSize          = SPI_DATASIZE_8BIT;
  spi3Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  spi3Handle.Init.NSS               = SPI_NSS_SOFT;
  spi3Handle.Init.TIMode            = SPI_TIMODE_DISABLE;
  spi3Handle.Init.Mode              = SPI_MODE_MASTER;

  if(HAL_SPI_Init(currentHandle) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }
}
/**
 * @brief Select chip.
 */
void SPI_HAL_Select(SPI_HAL_Typedef spi) {
  switch(spi) {
  case SPI_HAL_SPI3:
    HAL_GPIO_WritePin(SPI3_CS_PORT, SPI3_CS_PIN, GPIO_PIN_RESET);
    break;

  default:
    return;
  }
}
/**
 * @brief Deselect chip.
 */
void SPI_HAL_Deselect(SPI_HAL_Typedef spi) {
  switch(spi) {
  case SPI_HAL_SPI3:
    HAL_GPIO_WritePin(SPI3_CS_PORT, SPI3_CS_PIN, GPIO_PIN_SET);
    break;

  default:
    return;
  }
}
/**
 * @brief Send multiple data on SPI.
 * @param transmitBuffer Buffer to send.
 * @param length Number of bytes to send.
 * @warning Blocking function!
 */
void SPI_HAL_SendBuffer(SPI_HAL_Typedef spi, uint8_t* transmitBuffer, int length) {

  if (HAL_SPI_Transmit(&spi3Handle, (uint8_t*)transmitBuffer,
       length, SPI_MAX_DELAY_TIME) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }
}
/**
 * @brief Read multiple data on SPI.
 * @param receiveBuffer Buffer to place read data.
 * @param length Number of bytes to read.
 * @warning Blocking function!
 */
void SPI_HAL_ReadBuffer(SPI_HAL_Typedef spi, uint8_t* receiveBuffer, int length) {

  if (HAL_SPI_Receive(&spi3Handle, (uint8_t*)receiveBuffer,
       length, SPI_MAX_DELAY_TIME) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }
}
/**
 * @brief Transmit multiple data on SPI3.
 * @param receiveBuffer Receive buffer.
 * @param transmitBuffer Transmit buffer.
 * @param length Number of bytes to transmit.
 * @warning Blocking function!
 */
void SPI_HAL_TransmitBuffer(SPI_HAL_Typedef spi, uint8_t* receiveBuffer,
    uint8_t* transmitBuffer, int length) {

  if (HAL_SPI_TransmitReceive(&spi3Handle, (uint8_t*)transmitBuffer,
      (uint8_t *)receiveBuffer, length, SPI_MAX_DELAY_TIME) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }
}
/**
 * @brief Initalize SPI HAL driver
 * @param spiHandle Handle of SPI
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle) {
  GPIO_InitTypeDef  gpioInitialization;

  if (spiHandle == &spi3Handle) {
    SPI3_SCK_GPIO_CLK_ENABLE();
    SPI3_MISO_GPIO_CLK_ENABLE();
    SPI3_MOSI_GPIO_CLK_ENABLE();
    SPI3_CS_GPIO_CLK_ENABLE();
    SPI3_CLK_ENABLE();

    gpioInitialization.Pin       = SPI3_SCK_PIN;
    gpioInitialization.Mode      = GPIO_MODE_AF_PP;
    gpioInitialization.Pull      = GPIO_PULLUP;
    gpioInitialization.Speed     = GPIO_SPEED_FAST;
    gpioInitialization.Alternate = SPI3_SCK_AF;
    HAL_GPIO_Init(SPI3_SCK_GPIO_PORT, &gpioInitialization);

    gpioInitialization.Pin = SPI3_MISO_PIN;
    gpioInitialization.Alternate = SPI3_MISO_AF;
    HAL_GPIO_Init(SPI3_MISO_GPIO_PORT, &gpioInitialization);

    gpioInitialization.Pin = SPI3_MOSI_PIN;
    gpioInitialization.Alternate = SPI3_MOSI_AF;
    HAL_GPIO_Init(SPI3_MOSI_GPIO_PORT, &gpioInitialization);

    gpioInitialization.Pin    = SPI3_CS_PIN;
    gpioInitialization.Mode   = GPIO_MODE_OUTPUT_PP;
    gpioInitialization.Speed  = GPIO_SPEED_FAST;
    gpioInitialization.Pull   = GPIO_NOPULL;
    HAL_GPIO_Init(SPI3_CS_PORT, &gpioInitialization);
    HAL_GPIO_WritePin(SPI3_CS_PORT, SPI3_CS_PIN, GPIO_PIN_SET);
  }

}
/**
 * @brief Deinitalize SPI HAL driver
 * @param spiHandle Handle of SPI
 */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle) {
  if (spiHandle == &spi3Handle) {
    SPI3_FORCE_RESET();
    SPI3_RELEASE_RESET();

    HAL_GPIO_DeInit(SPI3_SCK_GPIO_PORT, SPI3_SCK_PIN);
    HAL_GPIO_DeInit(SPI3_MISO_GPIO_PORT, SPI3_MISO_PIN);
    HAL_GPIO_DeInit(SPI3_MOSI_GPIO_PORT, SPI3_MOSI_PIN);
    HAL_GPIO_DeInit(SPI3_CS_PORT, SPI3_CS_PIN);
  }
}
/**
 * @}
 */

/**
 * @file    spi3.c
 * @brief	  
 * @date    17 gru 2014
 * @author  Michal Ksiezopolski
 *
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

#include "spi3.h"
#include "common_hal.h"
#include <stm32f4xx.h>

/**
 * @addtogroup SPI3
 * @{
 */
#define SPIx                             SPI3
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI3_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

#define SPIx_FORCE_RESET()               __HAL_RCC_SPI3_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI3_RELEASE_RESET()

/* Definition for SPIx Pins */
#define SPIx_SCK_PIN                     GPIO_PIN_10
#define SPIx_SCK_GPIO_PORT               GPIOC
#define SPIx_SCK_AF                      GPIO_AF6_SPI3
#define SPIx_MISO_PIN                    GPIO_PIN_11
#define SPIx_MISO_GPIO_PORT              GPIOC
#define SPIx_MISO_AF                     GPIO_AF6_SPI3
#define SPIx_MOSI_PIN                    GPIO_PIN_12
#define SPIx_MOSI_GPIO_PORT              GPIOC
#define SPIx_MOSI_AF                     GPIO_AF6_SPI3

SPI_HandleTypeDef SpiHandle;

/**
 * @brief Initialize SPI3 and SS pin.
 */
void SPI3_Init(void) {

  GPIO_InitTypeDef GPIO_InitStruct;

  // Software chip select pin, PA15 = SS
  GPIO_InitStruct.Pin    = GPIO_PIN_15;
  GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed  = GPIO_SPEED_FAST;
  GPIO_InitStruct.Pull   = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET); // Set SS line

  SpiHandle.Instance               = SPIx;
  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
  SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
  SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
  SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  SpiHandle.Init.CRCPolynomial     = 7;
  SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SpiHandle.Init.NSS               = SPI_NSS_SOFT;
  SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
  SpiHandle.Init.Mode              = SPI_MODE_MASTER;

  if(HAL_SPI_Init(&SpiHandle) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }
}
/**
 * @brief Select chip.
 */
void SPI3_Select(void) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET); // Reset SS line
}
/**
 * @brief Deselect chip.
 */
void SPI3_Deselect(void) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
}
/**
 * @brief Send multiple data on SPI3.
 * @param buf Buffer to send.
 * @param len Number of bytes to send.
 * @warning Blocking function!
 */
void SPI3_SendBuffer(uint8_t* buf, uint32_t len) {

  if (HAL_SPI_Transmit(&SpiHandle, (uint8_t*)buf,
       len, 5000) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }
}
/**
 * @brief Read multiple data on SPI3.
 * @param buf Buffer to place read data.
 * @param len Number of bytes to read.
 * @warning Blocking function!
 */
void SPI3_ReadBuffer(uint8_t* buf, uint32_t len) {

  if (HAL_SPI_Receive(&SpiHandle, (uint8_t*)buf,
       len, 5000) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }
}
/**
 * @brief Transmit multiple data on SPI3.
 * @param rx_buf Receive buffer.
 * @param tx_buf Transmit buffer.
 * @param len Number of bytes to transmit.
 */
void SPI3_TransmitBuffer(uint8_t* rx_buf, uint8_t* tx_buf, uint32_t len) {

  if (HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)tx_buf,
      (uint8_t *)rx_buf, len, 5000) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }

}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  SPIx_SCK_GPIO_CLK_ENABLE();
  SPIx_MISO_GPIO_CLK_ENABLE();
  SPIx_MOSI_GPIO_CLK_ENABLE();
  /* Enable SPI clock */
  SPIx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* SPI SCK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = SPIx_SCK_AF;

  HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

  /* SPI MISO GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPIx_MISO_PIN;
  GPIO_InitStruct.Alternate = SPIx_MISO_AF;

  HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

  /* SPI MOSI GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
  GPIO_InitStruct.Alternate = SPIx_MOSI_AF;

  HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief SPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to its default state
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  /*##-1- Reset peripherals ##################################################*/
  SPIx_FORCE_RESET();
  SPIx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure SPI SCK as alternate function  */
  HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
  /* Configure SPI MISO as alternate function  */
  HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
  /* Configure SPI MOSI as alternate function  */
  HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);
}

/**
 * @}
 */

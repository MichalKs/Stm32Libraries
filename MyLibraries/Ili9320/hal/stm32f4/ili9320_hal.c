/**
 * @file: 	ili9320_hal.c
 * @brief:	Hardware abstraction layer for ILI9320
 * @date: 	08.10.2016
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

#include <stm32f4xx.h>
#include "ili9320_hal.h"
#include "common_hal.h"

#define ILI9320_RST_PORT  GPIOB                 ///< GPIO for reset pin
#define ILI9320_RST_PIN   GPIO_PIN_4            ///< Reset pin
#define ILI9320_RST_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()  ///< Clock for reset pin

#define ILI9320_ADDRESS   (*((volatile unsigned short *) 0x60000000)) ///< Address for writing register number
#define ILI9320_DATA      (*((volatile unsigned short *) 0x60020000)) ///< Address for writing data

/**
 * @brief Initialize ILI9320 hardware layer.
 */
void ILI9320_HAL_Initialize(void) {

  ILI9320_RST_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*
   * **************************************************
   * PIN mapping:
   *
   * FSMC_D0  - PD14
   * FSMC_D1  - PD15
   * FSMC_D2  - PD0
   * FSMC_D3  - PD1
   * FSMC_D4  - PE7
   * FSMC_D5  - PE8
   * FSMC_D6  - PE9
   * FSMC_D7  - PE10
   * FSMC_D8  - PE11
   * FSMC_D9  - PE12
   * FSMC_D10 - PE13
   * FSMC_D11 - PE14
   * FSMC_D12 - PE15
   * FSMC_D13 - PD8
   * FSMC_D14 - PD9
   * FSMC_D15 - PD10
   * FSMC_NOE - PD4 - RD
   * FSMC_NWE - PD5 - WR
   * FSMC_NE1 - PD7 - CS
   * FSMC_A16 - PD11 - RS
   * RESET  - PB4
   * **************************************************
   */
  GPIO_InitTypeDef gpioInitalize;
  gpioInitalize.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 |
      GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
      GPIO_PIN_11 | GPIO_PIN_14 | GPIO_PIN_15;
  gpioInitalize.Mode   = GPIO_MODE_AF_PP;
  gpioInitalize.Speed  = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioInitalize.Pull   = GPIO_NOPULL;
  gpioInitalize.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOD, &gpioInitalize);

  gpioInitalize.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 |
  GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
  GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpioInitalize);

  gpioInitalize.Pin = ILI9320_RST_PIN;
  gpioInitalize.Mode = GPIO_MODE_OUTPUT_PP;
  gpioInitalize.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioInitalize.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ILI9320_RST_PORT, &gpioInitalize);

  __HAL_RCC_FSMC_CLK_ENABLE();

  SRAM_HandleTypeDef sramHandle = {0};
  FSMC_NORSRAM_TimingTypeDef timing = {0};

  sramHandle.Instance  = FSMC_NORSRAM_DEVICE;
  sramHandle.Extended  = FSMC_NORSRAM_EXTENDED_DEVICE;

  timing.AddressSetupTime       = 4;
  timing.AddressHoldTime        = 0;
  timing.DataSetupTime          = 0x14;
  timing.BusTurnAroundDuration  = 0;
  timing.CLKDivision            = 0;
  timing.DataLatency            = 0;
  timing.AccessMode             = FSMC_ACCESS_MODE_B;

  sramHandle.Init.NSBank             = FSMC_NORSRAM_BANK1;
  sramHandle.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
  sramHandle.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
  sramHandle.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  sramHandle.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
  sramHandle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  sramHandle.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
  sramHandle.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
  sramHandle.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
  sramHandle.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
  sramHandle.Init.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE;
  sramHandle.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  sramHandle.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;

  if(HAL_SRAM_Init(&sramHandle, &timing, &timing) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }
}
/**
 * @brief Function for writing data to ILI9320 through FSMC.
 * @param address Register address.
 * @param data Data to write.
 */
void ILI9320_HAL_WriteReg(uint16_t address, uint16_t data) {
  ILI9320_ADDRESS = address;
  ILI9320_DATA = data;
}
/**
 * @brief Function for reading a given register.
 * @param address Register address.
 * @return Contents of register.
 */
uint16_t ILI9320_HAL_ReadReg(uint16_t address) {
  ILI9320_ADDRESS = address;
  return ILI9320_DATA;
}
/**
 * @brief Turn reset on.
 */
void ILI9320_HAL_ResetOn(void) {
  HAL_GPIO_WritePin(ILI9320_RST_PORT, ILI9320_RST_PIN, GPIO_PIN_RESET);
}
/**
 * @brief Turn reset off.
 */
void ILI9320_HAL_ResetOff(void) {
  HAL_GPIO_WritePin(ILI9320_RST_PORT, ILI9320_RST_PIN, GPIO_PIN_SET);
}

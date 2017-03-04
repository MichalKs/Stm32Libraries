/**
 * @file    mfrc522.c
 * @brief
 * @date    3 lut 2015
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

#include "mfrc522.h"
#include "spi_hal.h"
#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf(""str"%s",##args,"")
#define println(str, args...) printf("RFID--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif


typedef enum {
  MFRC522_COMMAND_REG     = 0x01, ///< Starts and stops commands execution
  MFRC522_COMIEN_REG      = 0x02, ///< Enabling IRQs
  MFRC522_DIVIEN_REG      = 0x03,
  MFRC522_COMIRQ_REG      = 0x04,
  MFRC522_DIVIRQ_REG      = 0x05,
  MFRC522_ERROR_REG       = 0x06,
  MFRC522_STATUS1_REG     = 0x07,
  MFRC522_STATUS2_REG     = 0x08,
  MFRC522_FIFODATA_REG    = 0x09,
  MFRC522_FIFOLEVEL_REG   = 0x0a,
  MFRC522_WATERLEVEL_REG  = 0x0b,
  MFRC522_CONTROL_REG     = 0x0c,
  MFRC522_BITFRAMING_REG  = 0x0d,
  MFRC522_COLL_REG        = 0x0e,
  // Page 1 - communication
  MFRC522_MODE_REG        = 0x11,
  MFRC522_TXMODE_REG      = 0x12,
  MFRC522_RXMODE_REG      = 0x13,
  MFRC522_TXCONTROL_REG   = 0x14,
  MFRC522_TXASK_REG       = 0x15,
  MFRC522_TXSEL_REG       = 0x16,
  MFRC522_RXSEL_REG       = 0x17,
  MFRC522_RXTHRESHOLD_REG = 0x18,
  MFRC522_DEMOD_REG       = 0x19,
  MFRC522_MFTX_REG        = 0x1c,
  MFRC522_MFRX_REG        = 0x1d,
  MFRC522_SERIALSPEED_REG = 0x1f,

  // Page 2 - configuration
  MFRC522_CRCRESULT_REG_MSB = 0x21,
  MFRC522_CRCRESULT_REG_LSB = 0x22,
  MFRC522_MODWIDTH_REG    = 0x24,
  MFRC522_RFCFG_REG       = 0x26,
  MFRC522_GSN_REG         = 0x27,
  MFRC522_CWGSP_REG       = 0x28,
  MFRC522_MODGSP_REG      = 0x29,
  MFRC522_TMODE_REG       = 0x2a,
  MFRC522_TPRESCALER_REG  = 0x2b,
  MFRC522_RELOAD_REG_MSB    = 0x2c,
  MFRC522_RELOAD_REG_LSB    = 0x2d,
  MFRC522_TCNTRVAL_REG_MSB  = 0x2e,
  MFRC522_TCNTRVAL_REG_LSB  = 0x2f,

  // Page 3 - test
  MFRC522_TESTSEL1_REG    = 0x31,
  MFRC522_TESTSEL2_REG    = 0x32,
  MFRC522_TESTPINEN_REG   = 0x33,
  MFRC522_TESTPINVAL_REG  = 0x34,
  MFRC522_TESTBUS_REG     = 0x35,
  MFRC522_AUTOTEST_REG    = 0x36,
  MFRC522_VERSION_REG     = 0x37,
  MFRC522_ANALOGTEST_REG  = 0x38,
  MFRC522_TESTDAC1_REG    = 0x39,
  MFRC522_TESTDAC2_REG    = 0x3a,
  MFRC522_TESTADC_REG     = 0x3b,
} Mrfc522Registers;

/**
 *
 */
typedef enum {
  CMD_IDLE                = 0,//!< CMD_IDLE
  CMD_MEMORY              = 1,//!< CMD_MEM
  CMD_GENERATE_RANDOM_ID  = 2,//!< CMD_GENERATE_RANDOM_ID
  CMD_CALCULATE_CRC       = 3,//!< CMD_CALC_CRC
  CMD_TRANSMIT            = 4,//!< CMD_TRANSMIT
  CMD_NO_CMD_CHANGE       = 7,//!< CMD_NO_CMD_CHANGE
  CMD_RECEIVE             = 8,//!< CMD_RECEIVE
  CMD_TRANSCEIVE          = 12,//!< CMD_TRANSCEIVE
  CMD_MF_AUTHENT          = 14,//!< CMD_MF_AUTHENT
  CMD_SOFT_RESET          = 15,//!< CMD_SOFT_RESET
} Mfrc522Commands;

static uint8_t readRegister(uint8_t address);
static void writeRegister(uint8_t address, uint8_t data);
static void softReset(void);

/**
 * @brief Initialize communication with RFID reader
 */
void Mfrc522_initialize(void) {
  SpiHal_initialize(SPI_HAL_SPI1);
  for (int i = 0; i < 20; i++) {
    SpiHal_transmitByte(SPI_HAL_SPI1, 0x00);
  }
  softReset();
  uint8_t registerValue;
  registerValue = readRegister(MFRC522_COMMAND_REG);
  println("Command reg 0x%02x", registerValue);
  registerValue = readRegister(MFRC522_VERSION_REG);
  println("Version 0x%02x", registerValue);
  if (registerValue != 0x91 && registerValue != 0x92) {
    println("Wrong device");
  }
}
/**
 * @brief Write to a register
 * @param address
 * @param data
 */
void writeRegister(uint8_t address, uint8_t data) {
  SpiHal_select(SPI_HAL_SPI1);
  // MSB = 0 for write
  SpiHal_transmitByte(SPI_HAL_SPI1, (address << 1) & 0x7f);
  SpiHal_transmitByte(SPI_HAL_SPI1, data);
  SpiHal_deselect(SPI_HAL_SPI1);
}
/**
 * @brief Write buffer to a register
 * @param address
 * @param data
 * @param len
 */
void writeBuffer(uint8_t address, uint8_t* data, uint8_t length) {
  SpiHal_select(SPI_HAL_SPI1);
  // MSB = 0 for write
  // send register address
  SpiHal_transmitByte(SPI_HAL_SPI1,(address << 1) & 0x7f);
  for (int i = 0; i < length; i++) {
    SpiHal_transmitByte(SPI_HAL_SPI1, data[i]);
  }
  SpiHal_deselect(SPI_HAL_SPI1);
}
/**
 * @brief Read a register
 * @param address
 * @return
 */
uint8_t readRegister(uint8_t address) {
  SpiHal_select(SPI_HAL_SPI1);
  // MSB = 1 for read
  SpiHal_transmitByte(SPI_HAL_SPI1, ((address << 1) & 0x7f) | 0x80);
  uint8_t registerValue = SpiHal_transmitByte(SPI_HAL_SPI1, 0x00);
  SpiHal_deselect(SPI_HAL_SPI1);
  return registerValue;
}
/**
 * @brief Execute soft reset
 */
void softReset(void) {
  writeRegister(MFRC522_COMMAND_REG, CMD_SOFT_RESET);
}

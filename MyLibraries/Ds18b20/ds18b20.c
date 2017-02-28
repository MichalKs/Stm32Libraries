/**
 * @file: 	ds18b20.c
 * @brief:	DS18B20 digital thermometer library
 * @date: 	5 sie 2014
 * @author: Michal Ksiezopolski
 * 
 * @details The DS18B20 is assumed to be in non-parasite mode
 * (TODO Check if parasite mode also works with library).
 * The DS18B20 data line should be pulled up with a 4k7 resistor.
 *
 * TODO Add power supply read, recall EEPROM, use of multiple DS18B20.
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


#include "ds18b20.h"
#include "onewire.h"
#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf("DS18B20--> "str"%s",##args,"\r")
#define println(str, args...) printf("DS18B20--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

/**
 * @brief DS18B20 commands
 */
typedef enum {
  DS18B20_CMD_CONVERT_TEMPERATURE = 0x44, ///< Convert temperature command
  DS18B20_CMD_WRITE_SCRATCHPAD    = 0x4e, ///< Write scratchpad command
  DS18B20_CMD_READ_SCRATCHPAD     = 0xbe, ///< Read scratchpad command
  DS18B20_CMD_COPY_SCRATCHPAD     = 0x48, ///< Copy scratchpad to internal EEPROM command
  DS18B20_CMD_RECALL_EEPROM       = 0xb8, ///< Recall configuration data from internal EEPROM command
  DS18B20_CMD_READ_POWER          = 0xb4, ///< Read power status command
} Ds18b20Commands;
/**
 * @brief Memory structure of the DS18B20
 */
typedef struct {
  uint8_t temperatureLsb;         ///< Temperature value - lower byte
  uint8_t temperatureMsb;         ///< Temperature value - higher byte
  uint8_t temperatureAlarmHigher; ///< Temperature alarm - higher byte
  uint8_t temperatureAlarmLower;  ///< Temperature alarm - lower byte
  uint8_t configuration;          ///< Configuration byte
  uint8_t reserved0;              ///< Reserved
  uint8_t reserved1;              ///< Reserved
  uint8_t reserved2;              ///< Reserved
  uint8_t crc;                    ///< CRC calculated from previous fields
} __attribute((packed)) Ds18b20Memory;
/**
 * @brief DS18B20 measurement resolution
 */
typedef enum {
  DS18B20_RESOLUTION_9_BITS   = (0<<5), ///< 9  bit resolution
  DS18B20_RESOLUTION_10_BITS  = (1<<5), ///< 10 bit resolution
  DS18B20_RESOLUTION_11_BITS  = (2<<5), ///< 11 bit resolution
  DS18B20_RESOLUTION_12_BITS  = (3<<5), ///< 12 bit resolution (default)
} Ds18b20Resolution;
#define ROMCODE_SIZE               8    ///< Size of device ROMCODE
#define ROMCODE_DEVICE_ID          0x28 ///< Device ROMCODE ID for DS18B20 family
#define ROMCODE_DEVICE_ID_POSITION 0    ///< Position of device ID in ROMCODE
static char romCode[ROMCODE_SIZE];   ///< Device ROMCODE

/**
 * @brief Initialize DS18B20 digital thermometer.
 * @return Result code
 */
Ds18b20ResultCode Ds18b20_initialize(void) {
  Onewire_readRom(romCode);
  if (romCode[ROMCODE_DEVICE_ID_POSITION] != ROMCODE_DEVICE_ID) {
    println("Not DS18B20!");
    return DS18B20_NO_DEVICE_ON_BUS;
  }
  return DS18B20_RESULT_OK;
}
/**
 * @brief Send start temperature conversion command.
 */
void Ds18b20_conversionStart(void) {
  Onewire_matchRom(romCode);
  Onewire_writeByte(DS18B20_CMD_CONVERT_TEMPERATURE);
}
/**
 * @brief Write scratchpad commands
 * @param th High byte of temperature alarm value
 * @param tl Low byte of temperature alarm value
 * @param conf Configuration byte
 */
void Ds18b20_writeScratchPad(uint8_t th, uint8_t tl, uint8_t conf) {

  Onewire_matchRom(romCode);
  Onewire_writeByte(DS18B20_CMD_WRITE_SCRATCHPAD);

  Onewire_writeByte(th); // high alarm temperature
  Onewire_writeByte(tl); // low  alarm temperature

  conf |= 0x1f; // 5 LSB bits always 1
  conf &= ~(1<<7); // MSB always 0
  Onewire_writeByte(conf);
}
/**
 * @brief Copies scratchpad into DS18B20 EEPROM.
 * Configuration will be restored after powerdown.
 */
void Ds18b20_copyScratchPad(void) {
  Onewire_matchRom(romCode);
  Onewire_writeByte(DS18B20_CMD_COPY_SCRATCHPAD);
}
/**
 * @brief Reads DS18B20 scratchpad.
 * @param buf Buffer for scratchpad
 */
void Ds18b20_readScratchPad(char * memoryBuffer) {
  Onewire_matchRom(romCode);
  Onewire_writeByte(DS18B20_CMD_READ_SCRATCHPAD);
  for (int i = 0; i < (int)sizeof(Ds18b20Memory); i++) {
    memoryBuffer[i] = Onewire_readByte();
  }
}
/**
 * @brief Reads DS18B20 temperature.
 * @return Temperature value in degrees Celsius
 */
float Ds18b20_readTemperatureCelsius(void) {
  Ds18b20Memory ds18b20Memory;
  Ds18b20_readScratchPad((char*)&ds18b20Memory);
  uint8_t t1 = (ds18b20Memory.temperatureLsb >> 4) & 0x0f;
  t1 |= ((ds18b20Memory.temperatureMsb << 4) & 0x70);
  float t2 = 0;
  if (ds18b20Memory.temperatureLsb & 0x08) {
    t2 += 0.5f;
  }
  if (ds18b20Memory.temperatureLsb & 0x04) {
    t2 += 0.25f;
  }
  if (ds18b20Memory.temperatureLsb & 0x02) {
    t2 += 0.125f;
  }
  if (ds18b20Memory.temperatureLsb & 0x01) {
    t2 += 0.0625f;
  }
  float ret = (float)t1 + t2;
  return ret;
}

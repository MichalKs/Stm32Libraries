/**
 * @file: 	onewire.c
 * @brief:	   
 * @date: 	30 lip 2014
 * @author: Michal Ksiezopolski
 * 
 *
 * @details All the functions have been written based
 * on the Maxim Integrated DS18B20 datasheet.
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

#include "onewire.h"
#include "onewire_hal.h"
#include "timers.h"
#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf("1WIRE--> "str"%s",##args,"")
#define println(str, args...) printf("1WIRE--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

#define ONEWIRE_MAX_DEVICES 16 ///< Maximum number of devices on the bus
#define ROM_LENGTH_WITHOUT_CRC 8 ///< Length of ROM without CRC

//static uint64_t romCode[ONEWIRE_MAX_DEVICES]; ///< Romcodes of found devices.
//static uint16_t deviceCounter; ///< Number of found devices on the bus.
/**
 * @brief Onewire commands
 */
typedef enum {
  ONEWIRE_CMD_SEARCH_ROM    = 0xf0,//!< ONEWIRE_CMD_SEARCH_ROM
  ONEWIRE_CMD_READ_ROM      = 0x33,//!< ONEWIRE_CMD_READ_ROM
  ONEWIRE_CMD_MATCH_ROM     = 0x55,//!< ONEWIRE_CMD_MATCH_ROM
  ONEWIRE_CMD_SKIP_ROM      = 0xcc,//!< ONEWIRE_CMD_SKIP_ROM
  ONEWIRE_CMD_ALARM_SEARCH  = 0xec,//!< ONEWIRE_CMD_ALARM_SEARCH
} OnewireCommands;

/**
 * @brief Initialize ONEWIRE bus.
 */
void Onewire_initialize(void) {
  OnewireHal_initialize();
}
/**
 * @brief Reset the bus
 * @return Result code
 */
OnewireResultCode Onewire_resetBus(void) {
  const int BUS_LOW_TIME_MICROS = 480;
  OnewireHal_busLow();
  Timer_delayMicros(BUS_LOW_TIME_MICROS);
  const int BUS_RELEASE_TIME_MICROS = 60;
  OnewireHal_releaseBus();
  Timer_delayMicros(BUS_RELEASE_TIME_MICROS);
  // by now device should pull bus low - presence pulse
  Boolean busState = OnewireHal_readBus();
  const int BUS_WAIT_TIME_MICROS = 420;
  // minimum 480us (after release time) - 60us
  Timer_delayMicros(BUS_WAIT_TIME_MICROS);
  if (busState) {
    return ONEWIRE_NO_DEVICES_ON_BUS;
  } else {
    return ONEWIRE_RESULT_OK;
  }
}
/**
 * @brief Writes a bit
 * @param bit Bit
 */
void Onewire_writeBit(Boolean bit) {
  const int BUS_LOW_TIME_MICROS = 1;
  OnewireHal_busLow();
  Timer_delayMicros(BUS_LOW_TIME_MICROS);

  // release bus for high bit
  if (bit & 0x01) {
    OnewireHal_releaseBus();
  }
  const int BIT_HOLD_TIME_MICROS = 60;
  Timer_delayMicros(BIT_HOLD_TIME_MICROS);
  OnewireHal_releaseBus(); // this is necessary for 0 bit
  // this delay is crucial - doesn't work without it
  const int FINAL_DELAY_MICROS = 1;
  Timer_delayMicros(FINAL_DELAY_MICROS);
}
/**
 * @brief Writes a byte
 * @param data Byte
 */
void Onewire_writeByte(uint8_t data) {
  // data on ONEWIRE is sent LSB first
  for (int i = 0; i < NUMBER_OF_BITS_IN_BYTE; i++) {
    Onewire_writeBit(data & 0x01);
    data >>= 1; // shift data
  }
}
/**
 * @brief Reads a bit
 * @return Read bit
 */
Boolean Onewire_readBit(void) {
  const int BUS_LOW_TIME_MICROS = 1;
  OnewireHal_busLow();
  Timer_delayMicros(BUS_LOW_TIME_MICROS);
  OnewireHal_releaseBus();
  const int DEVICE_RESPONSE_DELAY_MICROS = 15;
  // delay for device to respond - must be under 15us from initial falling edge
  Timer_delayMicros(DEVICE_RESPONSE_DELAY_MICROS);
  Boolean bit = OnewireHal_readBus();
  // whole read slot should be 60us + 1us of gap
  const int FINAL_DELAY_MICROS = 45;
  Timer_delayMicros(FINAL_DELAY_MICROS);
  return bit;
}
/**
 * @brief Reads a byte
 * @return Read byte
 */
char Onewire_readByte(void) {
  char readByte = 0;
  for (int i = 0; i < NUMBER_OF_BITS_IN_BYTE; i++) {
    readByte |= (Onewire_readBit() << i);
  }
  return readByte;
}
/**
 * @brief Reads ROM code of device on the bus
 * @warning This command works only if there is only one device
 * on the bus. Data collision will occur if there are more than
 * one device.
 * @details First byte is device family code (0x28 for DS18B20).
 * Last byte is CRC
 * @param buf Buffer for storing ROM code
 * @return Result code
 * TODO Add CRC calculation
 */
OnewireResultCode Onewire_readRom(char * bufferForRomData) {
  OnewireResultCode result = Onewire_resetBus();
  if (result != ONEWIRE_RESULT_OK) {
    return ONEWIRE_NO_DEVICES_ON_BUS;
  }
  Onewire_writeByte(ONEWIRE_CMD_READ_ROM);
  for (int i = 0; i < ROM_LENGTH_WITHOUT_CRC; i++) {
    bufferForRomData[i] = Onewire_readByte();
    printf("0x%02x ", (uint8_t)bufferForRomData[i]);
  }
  printf("\r\n");
  return ONEWIRE_RESULT_OK;
}
/**
 * @brief Send match ROM command
 * @param rom ROM code
 */
void Onewire_matchRom(char * rom) {
  Onewire_resetBus();
  Onewire_writeByte(ONEWIRE_CMD_MATCH_ROM);
  for (int i = 0; i < ROM_LENGTH_WITHOUT_CRC; i++) {
    Onewire_writeByte(rom[i]);
  }
}


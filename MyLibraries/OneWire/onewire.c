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

//static uint64_t romCode[ONEWIRE_MAX_DEVICES]; ///< Romcodes of found devices.
//static uint16_t deviceCounter; ///< Number of found devices on the bus.

#define ONEWIRE_CMD_SEARCH_ROM    0xf0
#define ONEWIRE_CMD_READ_ROM      0x33
#define ONEWIRE_CMD_MATCH_ROM     0x55
#define ONEWIRE_CMD_SKIP_ROM      0xcc
#define ONEWIRE_CMD_ALARM_SEARCH  0xec

/**
 * @brief Initialize ONEWIRE bus.
 */
void Onewire_initialize(void) {
  // initialize hardware
  OnewireHal_initialize();
}
/**
 * @brief Reset the bus
 * @retval FALSE Devices present on bus
 * @retval TRUE No devices on bus
 */
OnewireResultCode Onewire_resetBus(void) {

  OnewireHal_busLow(); // pull bus low for 480us
  Timer_delayMicros(480);
  OnewireHal_releaseBus(); // release bus for 60us
  Timer_delayMicros(60);

  // by now device should pull bus low - presence pulse
  uint8_t ret = OnewireHal_readBus();

  Timer_delayMicros(420); // minimum 480us (after realase time) - 60us

  if (ret) {
//    println("No devices");
    return ONEWIRE_NO_DEVICES_ON_BUS;
  } else {
//    println("Devices present on bus");
    return ONEWIRE_RESULT_OK;
  }
}
/**
 * @brief Writes a bit
 * @param bit Bit
 */
void Onewire_writeBit(Boolean bit) {
  OnewireHal_busLow(); // pull bus low for 1us
  Timer_delayMicros(1);

  // release bus for high bit
  if (bit & 0x01) {
    OnewireHal_releaseBus();
  }
  Timer_delayMicros(60);
  OnewireHal_releaseBus(); // this is necessary for 0 bit
  Timer_delayMicros(1); // this delay is crucial - doesn't work without it
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
uint8_t Onewire_readBit(void) {

  OnewireHal_busLow(); // pull bus low for 1us
  Timer_delayMicros(1);

  OnewireHal_releaseBus();
  Timer_delayMicros(15); // delay for device to respond - must be under 15us from initial falling edge

  uint8_t ret = OnewireHal_readBus();

  Timer_delayMicros(45); // whole read slot should be 60us + 1us of gap

  return ret;
}
/**
 * @brief Reads a byte
 * @return Read byte
 */
uint8_t Onewire_readByte(void) {

  uint8_t ret = 0;

  for (int i = 0; i < NUMBER_OF_BITS_IN_BYTE; i++) {
    ret |= (Onewire_readBit() << i);
  }

  return ret;
}
/**
 * @brief Reads ROM code of device on the bus
 *
 * @warning This command works only if there is only one device
 * on the bus. Data collision will occur if there are more than
 * one device.
 *
 * @details First byte is device family code (0x28 for DS18B20).
 * Last byte is CRC
 *
 * @param buf Buffer for storing ROM code
 *
 * TODO Add CRC calculation
 */
uint8_t Onewire_readRom(uint8_t* buf) {

  uint8_t ret = Onewire_resetBus();

  if (ret) {
    return 1; // no devices on bus
  }

  Onewire_writeByte(ONEWIRE_CMD_READ_ROM); // read ROM

  for (int i = 0; i < 8; i++) {
    buf[i] = Onewire_readByte();
    printf("0x%02x ", buf[i]);
  }
  printf("\r\n");

  return 0;

}
/**
 * @brief Send match ROM command
 * @param rom ROM code
 */
void Onewire_matchRom(uint8_t* rom) {

  Onewire_resetBus();
  Onewire_writeByte(ONEWIRE_CMD_MATCH_ROM); // match ROM
  for (int i = 0; i < 8; i++) {
    Onewire_writeByte(rom[i]);
  }

}


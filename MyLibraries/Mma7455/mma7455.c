/*
 * Mma7455.c
 *
 *  Created on: 12.03.2017
 *      Author: mik
 */

#include "mma7455.h"
#include "i2c_hal.h"


typedef enum {
  XAxisLsb = 0x00,
  XAxisMsb = 0x01,
  YAxisLsb = 0x02,
  YAxisMsb = 0x03,
  ZAxisLsb = 0x04,
  ZAxisMsb = 0x05,

} Mma7455Registers;


void Mma7455_initialize(void) {

}

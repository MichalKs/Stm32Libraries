/**
 * @file: 	hmc5883l.c
 * @brief:	   
 * @date: 	11 maj 2014
 * @author: Michal Ksiezopolski
 * 
 * @details The compass should be held in an ideally
 * horizontal position to get the bearings right. If not,
 * the results will be useless garbage.
 *
 * TODO Add tilting compensation.
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


#include "hmc5883l.h"
#include "i2c_hal.h"
#include "timers.h"
#include <math.h>
#include <stdio.h>

#define DEBUG_HMC5883L

#ifdef DEBUG_HMC5883L
#define print(str, args...) printf("HMC5883L--> "str"%s",##args,"\r")
#define println(str, args...) printf("HMC5883L--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

#define HMC5883L_ADDRESS 0x3c ///< Address on I2C bus
/**
 * @brief Register addresses
 */
typedef enum {
  HMC5883L_CONFA     = 0x00, ///< Configuration register A (r/w)
  HMC5883L_CONFB     = 0x01, ///< Configuration register B (r/w)
  HMC5883L_MODE      = 0x02, ///< Mode register (r/w)
  HMC5883L_DATAX_MSB = 0x03, ///< Data output X MSB register (r)
  HMC5883L_DATAX_LSB = 0x04, ///< Data output X LSB register (r)
  HMC5883L_DATAZ_MSB = 0x05, ///< Data output Z MSB register (r)
  HMC5883L_DATAZ_LSB = 0x06, ///< Data output Z LSB register (r)
  HMC5883L_DATAY_MSB = 0x07, ///< Data output Y MSB register (r)
  HMC5883L_DATAY_LSB = 0x08, ///< Data output Y LSB register (r)
  HMC5883L_STATUS    = 0x09, ///< Status register (r)
  HMC5883L_IDA       = 0x0a, ///< Identification register A (r)
  HMC5883L_IDB       = 0x0b, ///< Identification register B (r)
  HMC5883L_IDC       = 0x0c, ///< Identification register C (r)
} Hmc5883lRegisters;
/**
 *
 */
typedef enum {
  HMC6883L_MODE_CONTINUOUS = 0x00,  //!< HMC6883L_MODE_CONT
  HMC6883L_MODE_SINGLE = 0x01,//!< HMC6883L_MODE_SINGLE
  HMC6883L_MODE_IDLE = 0x10,  //!< HMC6883L_MODE_IDLE
} Hmc5883lMode;
/**
 *
 */
typedef enum {
  HMC6883L_1SAMP = 0x00,//!< HMC6883L_1SAMP
  HMC6883L_2SAMP = 0x01,//!< HMC6883L_2SAMP
  HMC6883L_4SAMP = 0x10,//!< HMC6883L_4SAMP
  HMC6883L_8SAMP = 0x11,//!< HMC6883L_8SAMP
} Hmc5883lNumberOfSamplesToAverage;
/**
 *
 */
typedef enum {
  HMC6883L_0Hz75,//!< HMC6883L_0Hz75
  HMC6883L_1Hz5, //!< HMC6883L_1Hz5
  HMC6883L_3Hz,  //!< HMC6883L_3Hz
  HMC6883L_7Hz5, //!< HMC6883L_7Hz5
  HMC6883L_15,   //!< HMC6883L_15 15 Hz - default value
  HMC6883L_30,   //!< HMC6883L_30
  HMC6883L_75,   //!< HMC6883L_75
} Hmc5883lDataRate;

static void readXYZ(int16_t * xMeasurement, int16_t * yMeasurement, int16_t * zMeasurement);
static void changeMode(Hmc5883lMode mode);
static uint8_t readRegister(uint8_t address);
static void writeRegister(uint8_t address, uint8_t value);

uint8_t readRegister(uint8_t address) {
  const int READ_WRITE_LENGTH = 1;
  I2c_sendBuffer(I2C_HAL_I2C1, HMC5883L_ADDRESS, &address, READ_WRITE_LENGTH);
  Timer_delayMillis(10);
  uint8_t registerValue;
  I2c_readBuffer(I2C_HAL_I2C1, HMC5883L_ADDRESS, &registerValue, READ_WRITE_LENGTH);
  Timer_delayMillis(10);
  return registerValue;
}
void writeRegister(uint8_t address, uint8_t value) {
  const int WRITE_LENGTH = 2;
  uint8_t dataToWrite[WRITE_LENGTH];
  dataToWrite[0] = address;
  dataToWrite[1] = value;
  I2c_readBuffer(I2C_HAL_I2C1, HMC5883L_ADDRESS, dataToWrite, WRITE_LENGTH);
  Timer_delayMillis(10);
}
/**
 * @brief Initialize the digital compass
 */
void Hmc5883l_initialize(void) {
  I2c_initialize(I2C_HAL_I2C1);
  // Read id registers and print them out.
  uint8_t registerValue = readRegister(HMC5883L_STATUS);
  println("Status %02x", registerValue);
  registerValue = readRegister(HMC5883L_MODE);
  println("Mode %02x", registerValue);
  uint8_t identificationRegisterA = readRegister(HMC5883L_IDA);
  println("Id A %02x", identificationRegisterA);
  uint8_t identificationRegisterB = readRegister(HMC5883L_IDB);
  println("Id B %02x", identificationRegisterB);
  uint8_t identificationRegisterC = readRegister(HMC5883L_IDC);
  println("Id C %02x", identificationRegisterC);
  if (identificationRegisterA != 'H' || identificationRegisterB != '4' ||
      identificationRegisterC != '3') {
    println("Error. Wrong device");
  }
  registerValue = readRegister(HMC5883L_STATUS);
  println("Status %02x", registerValue);
  registerValue = readRegister(HMC5883L_MODE);
  println("Mode %02x", registerValue);

  // continuous measurement mode
  changeMode(HMC6883L_MODE_CONTINUOUS);
  registerValue = readRegister(HMC5883L_MODE);
  println("Mode %02x", registerValue);
}

/**
 * @brief Reads the current direction angle.
 * @return Direction angle (0 or 360 means north, 180 means south
 * 90 east and 270 west).
 */
double Hmc5883l_readAngle(void) {

  int16_t xMeasurement, yMeasurement, zMeasurement;

  // Read XYZ
  readXYZ(&xMeasurement, &yMeasurement, &zMeasurement);

  println("x=%d y=%d z=%d", xMeasurement, yMeasurement, zMeasurement);

  double direction; // the direction angle

  // These formulas are taken from AN-203 application note
  // for the Honeywell compass
  // If the compass is horizontal these formulas actually
  // work.
  if (yMeasurement > 0) {
    direction = 90.0 - atan((double)xMeasurement/(double)yMeasurement)*180.0/M_PI;
  } else if (yMeasurement < 0){
    direction = 270.0 - atan((double)xMeasurement/(double)yMeasurement)*180.0/M_PI;
  } else if (yMeasurement == 0 && xMeasurement < 0) {
    direction = 180.0;
  } else {
    direction = 0.0;
  }

  return direction;
}
/**
 * @brief Read XYZ readings from the compass.
 * @details Check AN-203 for the Honewell compass
 * to check out what these mean and how they should behave.
 * @param xMeasurement
 * @param yMeasurement
 * @param zMeasurement
 */
void readXYZ(int16_t * xMeasurement, int16_t * yMeasurement, int16_t * zMeasurement) {
  uint16_t x, y, z;
  // Read x
  x = (uint16_t)readRegister(HMC5883L_DATAX_MSB) << 8;
  x |= (uint16_t)readRegister(HMC5883L_DATAX_LSB);
  // Read y
  y = (uint16_t)readRegister(HMC5883L_DATAY_MSB) << 8;
  y |= (uint16_t)readRegister(HMC5883L_DATAY_LSB);
  // Read z
  z = (uint16_t)readRegister(HMC5883L_DATAZ_MSB) << 8;
  z |= (uint16_t)readRegister(HMC5883L_DATAZ_LSB);
  *xMeasurement = (int16_t)x;
  *yMeasurement = (int16_t)y;
  *zMeasurement = (int16_t)z;
}
/**
 * @brief Change the mode of the compass.
 * @param mode New mode
 */
void changeMode(Hmc5883lMode mode) {
  // Mode bits are the two LSB of MODE register
  writeRegister(HMC5883L_MODE, mode & 0x03);
}

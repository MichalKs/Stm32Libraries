/*
 * bmp085.c
 *
 *  Created on: 03.03.2017
 *      Author: mik
 */

#include "bmp085.h"
#include "i2c_hal.h"
#include "timers.h"
#include <stdio.h>
#include <math.h>

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf(""str"%s",##args,"")
#define println(str, args...) printf("MAIN--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

#define BMP085_ADDRESS 0xee ///< BMP085 address on I2C bus
/**
 * @brief BMP085 registers
 */
typedef enum {
  BMP085_EEPROM_START = 0xaa,
  BMP085_CONTROL_REGISTER_VALUE = 0xf4,  //!< BMP085_CONTROL_REGISTER_VALUE
  BMP085_MEASURMENT_REGISTER_MSB = 0xf6, //!< BMP085_MEASURMENT_REGISTER_MSB
  BMP085_MEASURMENT_REGISTER_LSB = 0xf7, //!< BMP085_MEASURMENT_REGISTER_LSB
  BMP085_MEASURMENT_REGISTER_XLSB = 0xf8,//!< BMP085_MEASURMENT_REGISTER_XLSB
} Bmp085Registers;
/**
 * @brief BMP085 measurements
 */
typedef enum {
  MEASURMENT_TEMPERATURE = 0x2e,            //!< MEASURMENT_TEMPERATURE
  MEASUREMENT_PRESSURE_OVERSAMPLING0 = 0x34,//!< MEASUREMENT_PRESSURE_OVERSAMPLING0
  MEASUREMENT_PRESSURE_OVERSAMPLING1 = 0x74,//!< MEASUREMENT_PRESSURE_OVERSAMPLING1
  MEASUREMENT_PRESSURE_OVERSAMPLING2 = 0xb4,//!< MEASUREMENT_PRESSURE_OVERSAMPLING2
  MEASUREMENT_PRESSURE_OVERSAMPLING3 = 0xf4,//!< MEASUREMENT_PRESSURE_OVERSAMPLING3
} Bmp085Measurements;
/**
 * @brief BMP085 conversion times
 */
typedef enum {
  TEMPERATURE_CONVERSION_TIME_MILLIS = 5,            //!< TEMPERATURE_CONVERSION_TIME_MILLIS
  PRESSURE_OVERSAMPLING0_CONVERSION_TIME_MILLIS = 5, //!< PRESSURE_OVERSAMPLING0_CONVERSION_TIME_MILLIS
  PRESSURE_OVERSAMPLING1_CONVERSION_TIME_MILLIS = 8, //!< PRESSURE_OVERSAMPLING1_CONVERSION_TIME_MILLIS
  PRESSURE_OVERSAMPLING2_CONVERSION_TIME_MILLIS = 14,//!< PRESSURE_OVERSAMPLING2_CONVERSION_TIME_MILLIS
  PRESSURE_OVERSAMPLING3_CONVERSION_TIME_MILLIS = 26,//!< PRESSURE_OVERSAMPLING3_CONVERSION_TIME_MILLIS
} Mbp085ConversionTimes;

typedef struct {
  int16_t ac1;
  int16_t ac2;
  int16_t ac3;
  uint16_t ac4;
  uint16_t ac5;
  uint16_t ac6;
  int16_t b1;
  int16_t b2;
  int16_t mb;
  int16_t mc;
  int16_t md;
} __attribute__((packed)) EepromCalibrationCoefficients;

static EepromCalibrationCoefficients eepromCalibrationCoefficients;

void readCalibrationCoefficients(void) {
  const int WRITE_LENGTH = 1;
  uint8_t dataToWrite[WRITE_LENGTH];
  dataToWrite[0] = BMP085_EEPROM_START;
  I2c_sendBuffer(I2C_HAL_I2C1, BMP085_ADDRESS, dataToWrite, WRITE_LENGTH);
  const int READ_LENGTH = sizeof(EepromCalibrationCoefficients);
  I2c_readBuffer(I2C_HAL_I2C1, BMP085_ADDRESS, (uint8_t*)&eepromCalibrationCoefficients,
      READ_LENGTH);

  uint8_t * dataPointer = (uint8_t*)&eepromCalibrationCoefficients;

  UTILS_Hexdump((uint8_t*)&eepromCalibrationCoefficients,
      sizeof(EepromCalibrationCoefficients));

  for (int i = 0; i < (int)sizeof(EepromCalibrationCoefficients); i++) {
    uint8_t temporaryStorage = *dataPointer;
    *dataPointer = *(dataPointer+1);
    *(dataPointer+1) = temporaryStorage;
    dataPointer += 2;
  }
  UTILS_Hexdump((uint8_t*)&eepromCalibrationCoefficients,
      sizeof(EepromCalibrationCoefficients));

  println("ac1=%d, ac2=%d, ac3=%d, ac4=%u, ac5=%u, ac6=%u \r\n"
      "b1=%d, b2=%d, mb=%d, mc=%d, md=%d", eepromCalibrationCoefficients.ac1,
      eepromCalibrationCoefficients.ac2, eepromCalibrationCoefficients.ac3,
      eepromCalibrationCoefficients.ac4, eepromCalibrationCoefficients.ac5,
      eepromCalibrationCoefficients.ac6, eepromCalibrationCoefficients.b1,
      eepromCalibrationCoefficients.b2, eepromCalibrationCoefficients.mb,
      eepromCalibrationCoefficients.mc, eepromCalibrationCoefficients.md);
}
/**
 * @brief Start a measurement
 * @param measurementType Measurement type
 */
void startMeasurement(Bmp085Measurements measurementType) {
  const int WRITE_LENGTH = 2;
  uint8_t dataToWrite[WRITE_LENGTH];
  dataToWrite[0] = BMP085_CONTROL_REGISTER_VALUE;
  dataToWrite[1] = measurementType;
  I2c_sendBuffer(I2C_HAL_I2C1, BMP085_ADDRESS, dataToWrite, WRITE_LENGTH);
}
/**
 * @brief Reads the last measurments
 */
uint16_t readMeasurement(void) {
  const int WRITE_LENGTH = 1;
  uint8_t dataToWrite[WRITE_LENGTH];
  dataToWrite[0] = BMP085_MEASURMENT_REGISTER_MSB;
  I2c_sendBuffer(I2C_HAL_I2C1, BMP085_ADDRESS, dataToWrite, WRITE_LENGTH);
  const int READ_LENGTH = 2;
  uint8_t dataToRead[READ_LENGTH];
  I2c_readBuffer(I2C_HAL_I2C1, BMP085_ADDRESS, dataToRead, READ_LENGTH);

  uint16_t measurement = (uint16_t)dataToRead[0]<<8 | dataToRead[1];
  return measurement;
}
/**
 * @brief Initialize BMP085 module
 */
void Bmp085_initialize(void) {
  I2c_initialize(I2C_HAL_I2C1);

  readCalibrationCoefficients();

}
float calculateTemperature(uint16_t temperatureMeasurement) {
  float x1 = ((float)temperatureMeasurement - (float)eepromCalibrationCoefficients.ac6) *
      (float)eepromCalibrationCoefficients.ac5 / powf(2, 15);
  float x2 = (float)eepromCalibrationCoefficients.mc * powf(2, 11) /
      (x1 + eepromCalibrationCoefficients.md);
  float b5 = x1 + x2;
  float temperatureCelsius = (b5 + 8) / powf(2, 4);
  return temperatureCelsius/10.0f;

}
/**
 * @brief Process measurements
 * @details This function should be run in main
 */
void Bmp085_readMeasurements(void) {
  uint16_t temperatureMeasurement;

  typedef enum {
    START_TEMPERATURE_MEASURMENT,
    TEMPERATURE_DELAY,
  } MeasurementState;

  static MeasurementState state = START_TEMPERATURE_MEASURMENT;
  static unsigned int conversionTimerMillis;

  switch(state) {
  case START_TEMPERATURE_MEASURMENT:
    startMeasurement(MEASURMENT_TEMPERATURE);
    conversionTimerMillis = Timer_getTimeMillis();
    state = TEMPERATURE_DELAY;
    break;
  case TEMPERATURE_DELAY:
    if (Timer_delayTimer(TEMPERATURE_CONVERSION_TIME_MILLIS, conversionTimerMillis)) {
      temperatureMeasurement = readMeasurement();
      float temperatureCelsius = calculateTemperature(temperatureMeasurement);
      println("Temperature = %.2f", temperatureCelsius);
      state = START_TEMPERATURE_MEASURMENT;
    }
    break;
  }
}












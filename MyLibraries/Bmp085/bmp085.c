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

#define DEBUG_BMP085

#ifdef DEBUG_BMP085
#define print(str, args...) printf(""str"%s",##args,"")
#define println(str, args...) printf("BMP085--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

#define BMP085_ADDRESS 0xee ///< BMP085 address on I2C bus
/**
 * @brief BMP085 registers
 */
typedef enum {
  BMP085_EEPROM_START = 0xaa,            //!< BMP085_EEPROM_START
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
/**
 * @brief Calibration coefficients - see datasheet of BMP085
 */
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
static EepromCalibrationCoefficients eepromCalibrationCoefficients; ///< Calibration coefficents
static float b5; ///< This factor is needed for both pressure and temperature

static void readCalibrationCoefficients(void);
static void startMeasurement(Bmp085Measurements measurementType);
static uint16_t readMeasurement(void);
static float calculateTemperatureDegreesCelsius(uint16_t temperatureMeasurement);
static float calculatePressureHectopascals(uint16_t pressureMeasurement);

/**
 * @brief Initialize BMP085 module
 */
void Bmp085_initialize(void) {
  I2c_initialize(I2C_HAL_I2C1);
  readCalibrationCoefficients();
}
/**
 * @brief Process measurements
 * @details This function should be run in main
 */
void Bmp085_readMeasurements(void) {
  uint16_t temperatureMeasurement, pressureMeasurement;

  typedef enum {
    START_TEMPERATURE_MEASURMENT,
    TEMPERATURE_DELAY,
    START_PRESSURE_MEASUREMENT,
    PRESSURE_DELAY,
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
      float temperatureCelsius = calculateTemperatureDegreesCelsius(temperatureMeasurement);
      println("Temperature = %.2f deg. Celsius", temperatureCelsius);
      state = START_PRESSURE_MEASUREMENT;
    }
    break;
  case START_PRESSURE_MEASUREMENT:
    startMeasurement(MEASUREMENT_PRESSURE_OVERSAMPLING0);
    conversionTimerMillis = Timer_getTimeMillis();
    state = PRESSURE_DELAY;
    break;
  case PRESSURE_DELAY:
    if (Timer_delayTimer(PRESSURE_OVERSAMPLING0_CONVERSION_TIME_MILLIS,
        conversionTimerMillis)) {
      pressureMeasurement = readMeasurement();
      float pressureHectopascals = calculatePressureHectopascals(pressureMeasurement);
      println("Pressure = %.2f hPa", pressureHectopascals);
      state = START_TEMPERATURE_MEASURMENT;
    }
    break;
  }
}
/**
 * @brief Reads the calibration coefficients of BMP085
 */
void readCalibrationCoefficients(void) {
  const int WRITE_LENGTH = 1;
  uint8_t dataToWrite[WRITE_LENGTH];
  dataToWrite[0] = BMP085_EEPROM_START;
  I2c_sendBuffer(I2C_HAL_I2C1, BMP085_ADDRESS, dataToWrite, WRITE_LENGTH);
  const int READ_LENGTH = sizeof(EepromCalibrationCoefficients);
  I2c_readBuffer(I2C_HAL_I2C1, BMP085_ADDRESS, (uint8_t*)&eepromCalibrationCoefficients,
      READ_LENGTH);
  // The coefficents are stored in big-endian order so we have to swap the bytes
  uint8_t * dataPointer = (uint8_t*)&eepromCalibrationCoefficients;
  for (int i = 0; i < (int)sizeof(EepromCalibrationCoefficients); i++) {
    uint8_t temporaryStorage = *dataPointer;
    *dataPointer = *(dataPointer+1);
    *(dataPointer+1) = temporaryStorage;
    dataPointer += 2;
  }
  println("ac1=%d, ac2=%d, ac3=%d, ac4=%u, ac5=%u,\r\n "
      "ac6=%u b1=%d, b2=%d, mb=%d, mc=%d, md=%d", eepromCalibrationCoefficients.ac1,
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
 * @brief Calculate temperature adjusting it based on calibration coefficients
 * @details This is taken from BMP085 datasheet
 * @param temperatureMeasurement The measurement read from the sensor
 * @return Temperature in degrees Celsius
 */
float calculateTemperatureDegreesCelsius(uint16_t temperatureMeasurement) {
  float x1 = ((float)temperatureMeasurement - (float)eepromCalibrationCoefficients.ac6) *
      (float)eepromCalibrationCoefficients.ac5 / powf(2, 15);
  float x2 = (float)eepromCalibrationCoefficients.mc * powf(2, 11) /
      (x1 + eepromCalibrationCoefficients.md);
  b5 = x1 + x2;
  float temperatureCelsius = (b5 + 8) / powf(2, 4);
  return temperatureCelsius/10.0f;
}
/**
 * @brief Calculate pressure adjusting it based on calibration coefficients
 * @details This is taken from BMP085 datasheet
 * @param pressureMeasurement The pressure read from the sensor
 * @return Pressure in hPa
 */
float calculatePressureHectopascals(uint16_t pressureMeasurement) {

  // these calculations are ridiculous but seem to work
  float b6 = b5-4000;
  float x1 = eepromCalibrationCoefficients.b2 * (b6 * b6 / powf(2, 12)) / powf(2, 11);
  float x2 = eepromCalibrationCoefficients.ac2 * b6 / powf(2, 11);
  float x3 = x1 + x2;
  float b3 = (eepromCalibrationCoefficients.ac1*4+x3+2)/4;
  x1 = eepromCalibrationCoefficients.ac3 * b6 / powf(2, 13);
  x2 = eepromCalibrationCoefficients.b1 * (b6 * b6 / powf(2, 12)) / powf(2, 16);
  x3 = (x1+x2+2)/4;
  float b4 = (float)eepromCalibrationCoefficients.ac4 * (unsigned long)(x3+32768) /
      powf(2, 15);
  float b7 = ((unsigned long)pressureMeasurement - b3) * 50000;
  float p;
  if (b7 < 0x80000000) {
    p = b7 * 2 / b4;
  } else {
    p = b7 / b4 * 2;
  }
  x1 = p / 256 * p / 256;
  x1 = x1 * 3038 / powf(2, 16);
  x2 = (-7357 * p) / powf(2, 16);
  p = p + (x1 + x2 + 3791) / 16;

  return p/100.0f;
}

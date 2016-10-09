/**
 * @file    tsc2046.c
 * @brief   TSC2046 touch screen library
 * @date    09.10.2016
 * @author  Michal Ksiezopolski
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
#include "tsc2046.h"
#include "spi_hal.h"
#include "tsc2046_hal.h"
#include "utils.h"
#include "timers.h"
#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf(""str"%s",##args,"")
#define println(str, args...) printf("TSC--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

/**
 * @addtogroup TSC2046
 * @{
 */
/*
 * Settings of the analog input pins
 */
#define MEASURE_TEMP0   0b000 ///< Measure temperature
#define MEASURE_Y       0b001 ///< Measure Y position
#define MEASURE_VBAT    0b010 ///< Measure battery voltage
#define MEASURE_Z1      0b011
#define MEASURE_Z2      0b100
#define MEASURE_X       0b101 ///< Measure X position
#define MEASURE_AUX     0b110
#define MEASURE_TEMP1   0b111 ///< Measure temperature

#define MODE_8BIT   1 ///< 8-bit data mode
#define MODE_12BIT  0 ///< 12-bit data mode

#define SINGLE_ENDED 1 ///< Single ended measurement
#define DIFFERENTIAL 0 ///< Differential measurement

#define PD_POWER_DOWN 0b00 ///< Power down mode
#define PD_VREF_OFF   0b01 ///< Turn off Vref
#define PD_ADC_OFF    0b10 ///< Turn off ADC
#define PD_ALWAYS_ON  0b11 ///< No power down

#define MAX_EVENTS    20  ///< Maximum number of registered events
#define TSC2046_SPI   SPI_HAL_SPI3 ///< SPI for controlling touchscreen

/**
 * @brief Control byte
 * @details MSB is the start bit (always 1). Then come 3
 * analog input bits, mode bit (12bit = 0, 8bit = 1).
 * Next comes the SER/DFR bit (single ended mode = 1,
 * differential mode = 0). Two LSB bits are the
 * power down mode select bits.
 */
typedef union {
  struct {
    uint8_t powerDown :2;
    uint8_t serDfr :1;
    uint8_t mode: 1;
    uint8_t channelSelect :3;
    uint8_t startBit :1;
  } bits;
  uint8_t byte;
} ControlByteTypedef;
/**
 * @brief Structure for defining an event triggered but touching
 * a specific region of the touchscreen.
 */
typedef struct {
  void (*eventCb)(int x, int y); ///< Callback function for event (gets exact coordinates of touch)
  uint16_t x;       ///< X coordinate of event region origin
  uint16_t y;       ///< Y coordinate of event region origin
  uint16_t width;   ///< Width of event region
  uint16_t height;  ///< Height of event region
} TSC2046_EventTypedef;

static TSC2046_EventTypedef registeredEvents[MAX_EVENTS]; ///< Registered events
static int numberOfRegisteredEvents;      ///< Number of registered events
static volatile Boolean wasTouchDetected; ///< Was touch detected in IRQ

static void touchInterruptCallback(void);
static void readTouchPosition(int *x, int *y);

/**
 * @brief Initialize the touch screen library.
 */
void TSC2046_Initialize(void) {

  const int TOUCHSCREEN_COMMAND_LENGTH = 3;
  const int POSITION_COMMAND = 0;
  const int POSITION_HIGH_BYTE = 1;
  const int POSITION_LOW_BYTE = 2;

  SPI_HAL_Init(SPI_HAL_SPI3);
  TSC2046_HAL_PenirqInit(touchInterruptCallback);

  // send first commands
  // with 2 LSB bits = 0 to enable PENIRQ
  ControlByteTypedef ctrl;
  ctrl.bits.startBit = 1;
  ctrl.bits.serDfr = DIFFERENTIAL;
  ctrl.bits.mode = MODE_12BIT;
  ctrl.bits.powerDown = PD_POWER_DOWN;
  ctrl.bits.channelSelect = MEASURE_X;

  uint8_t txBuffer[TOUCHSCREEN_COMMAND_LENGTH];
  txBuffer[POSITION_COMMAND] = ctrl.byte;
  txBuffer[POSITION_HIGH_BYTE] = 0;
  txBuffer[POSITION_LOW_BYTE] = 0;

  SPI_HAL_Select(SPI_HAL_SPI3);
  SPI_HAL_SendBuffer(SPI_HAL_SPI3, txBuffer, TOUCHSCREEN_COMMAND_LENGTH);
  SPI_HAL_Deselect(SPI_HAL_SPI3);
}
/**
 * @brief Registers a given region of the touch screen
 * to trigger an event.
 * @param x X position of starting point of region
 * @param y Y position of starting point of region
 * @param width Width of region
 * @param height Height of region
 * @param eventCb Callback function for event.
 * @return Current event index or -1 in case of error.
 */
int TSC2046_RegisterEvent(int x, int y, int width, int height,
    void (*eventCb)(int x, int y)) {

  // if too many events
  if (numberOfRegisteredEvents >= MAX_EVENTS) {
    return -1;
  }

  // complete event structure
  registeredEvents[numberOfRegisteredEvents].x = x;
  registeredEvents[numberOfRegisteredEvents].y = y;
  registeredEvents[numberOfRegisteredEvents].width = width;
  registeredEvents[numberOfRegisteredEvents].height = height;
  registeredEvents[numberOfRegisteredEvents].eventCb = eventCb;

  // update count
  numberOfRegisteredEvents++;
  return numberOfRegisteredEvents;
}
/**
 * @brief Handler for touch screen actions.
 * @details Call this function regularly in main to handle
 * touch screen events.
 */
void TSC2046_Update(void) {

  const int DEBOUNCE_TIME = 20;
  const int WAIT_TIME = 100;

  typedef enum {
    WAITING_FOR_IRQ,
    IRQ_RECEIVED,
    WAIT_FOR_DEBOUNCE,
    WAIT_FOR_NEXT_TOUCH,
  } TouchStateTypedef;

  static int debounce;
  static TouchStateTypedef touchState = WAITING_FOR_IRQ;

  switch (touchState) {

  case WAITING_FOR_IRQ:
    if (wasTouchDetected) {
      touchState = IRQ_RECEIVED;
    }
    break;

  case IRQ_RECEIVED:
    debounce = TIMER_GetTimeMillis();
    touchState = WAIT_FOR_DEBOUNCE;
    break;

  case WAIT_FOR_DEBOUNCE:
    if (TIMER_DelayTimer(DEBOUNCE_TIME, debounce)) {
      int x, y;
      touchState = WAIT_FOR_NEXT_TOUCH;
      debounce = TIMER_GetTimeMillis();
      // still down?
      if (!TSC2046_HAL_ReadPenirq()) {

        readTouchPosition(&x, &y);

        for (int i = 0; i < numberOfRegisteredEvents; i++) {
          if ((x > registeredEvents[i].x) && (x <= registeredEvents[i].width +
              registeredEvents[i].x) && (y > registeredEvents[i].y) &&
              (y <= registeredEvents[i].height + registeredEvents[i].y)) {
            if (registeredEvents[i].eventCb) {
              registeredEvents[i].eventCb(x, y);
            }
          }
        }
      }
    }
    break;

  case WAIT_FOR_NEXT_TOUCH:
    if (TIMER_DelayTimer(WAIT_TIME, debounce)) {
      touchState = WAITING_FOR_IRQ;
      wasTouchDetected = FALSE;
    }
    break;
  }

}
/**
 * @brief Read X and Y position on touch screen.
 * @param x Pointer to store X coordinate.
 * @param y Pointer to store Y coordinate.
 */
void readTouchPosition(int *x, int *y) {

  const int TOUCHSCREEN_COMMAND_LENGTH = 3;
  const int POSITION_COMMAND = 0;
  const int POSITION_HIGH_BYTE = 1;
  const int POSITION_LOW_BYTE = 2;
  const int TOUCH_BIT_SHIFT = 3;

  TSC2046_HAL_DisablePenirq(); // disable IRQ during read
  SPI_HAL_Select(SPI_HAL_SPI3);

  // control byte
  ControlByteTypedef ctrl;
  ctrl.bits.startBit      = TRUE;
  ctrl.bits.serDfr        = DIFFERENTIAL;
  ctrl.bits.mode          = MODE_12BIT;
  ctrl.bits.powerDown     = PD_POWER_DOWN;

  // read Y
  ctrl.bits.channelSelect = MEASURE_Y;
  uint8_t rxBuffer[TOUCHSCREEN_COMMAND_LENGTH];
  uint8_t txBuffer[TOUCHSCREEN_COMMAND_LENGTH];
  txBuffer[POSITION_COMMAND] = ctrl.byte;
  txBuffer[POSITION_HIGH_BYTE] = 0;
  txBuffer[POSITION_LOW_BYTE] = 0;
  SPI_HAL_TransmitBuffer(SPI_HAL_SPI3, rxBuffer, txBuffer,
      TOUCHSCREEN_COMMAND_LENGTH);

  int tmpY = ((int)rxBuffer[POSITION_HIGH_BYTE])<<8;
  tmpY |= rxBuffer[POSITION_LOW_BYTE];

  // read X
  ctrl.bits.channelSelect = MEASURE_X;
  txBuffer[POSITION_COMMAND] = ctrl.byte;
  SPI_HAL_TransmitBuffer(SPI_HAL_SPI3, rxBuffer, txBuffer,
      TOUCHSCREEN_COMMAND_LENGTH);

  int tmpX = ((int)rxBuffer[POSITION_HIGH_BYTE])<<8;
  tmpX |= rxBuffer[POSITION_LOW_BYTE];

  *y = tmpY >> TOUCH_BIT_SHIFT;
  *x = tmpX >> TOUCH_BIT_SHIFT;

  println("Touch position: x = %d y = %d", *x, *y);

  SPI_HAL_Deselect(SPI_HAL_SPI3);
  TSC2046_HAL_EnablePenirq();
}
/**
 * @brief Callback function called by lower layer whenever
 * PENIRQ signal is asserted. This signalizes that the
 * touch screen was pressed.
 */
void touchInterruptCallback(void) {
  wasTouchDetected = TRUE;
}
/**
 * @}
 */

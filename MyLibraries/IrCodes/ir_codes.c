/**
 * @file   ir.c
 * @brief  Infrared decoding functions
 * @date   12.03.2017
 * @author Michal Ksiezopolski
 * 
 * @details For now this function only supports RC5 decoding.
 * TODO Add other IR formats.
 *
 * @verbatim
 * Copyright (c) 2017 Michal Ksiezopolski.
 * All rights reserved. This program and the 
 * accompanying materials are made available 
 * under the terms of the GNU Public License 
 * v3.0 which accompanies this distribution, 
 * and is available at 
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#include "ir_codes.h"
#include "ir_codes_hal.h"
#include <stdio.h>

#define DEBUG_IR

#ifdef DEBUG_IR
  #define print(str, args...) printf(""str"%s",##args,"")
  #define println(str, args...) printf("IR--> "str"%s",##args,"\r\n")
#else
  #define print(str, args...) (void)0
  #define println(str, args...) (void)0
#endif

/**
 * @defgroup  IR IR
 * @brief     Infrared decoding functions
 */
/**
 * @addtogroup IR
 * @{
 */

static void resetFrameCb(void);
static void receiveDataCb(int pulseWidthMicros, IrPulseState pulse);

/**
 * @brief IR coding
 */
typedef enum {
  RC5   = 0, ///< RC5 coding
  SIRC  = 1, ///< SIRC coding
} IrCoding;

#define DEFAULT_IR_CODING               RC5  ///< Default coding for library
#define RC5_NUMBER_OF_BITS_IN_FRAME     13   ///< Number of bits in RC5 frame
#define RC5_FRAME_TIMEOUT_MICROS        3600 ///< Timeout value in us of RC5 frame
#define RC5_MAX_BIT_LENGTH_MICROS       1900 ///< Max bit length in us
#define RC5_MIN_HALFBIT_LENGTH_MICROS   700  ///< Min half bit value in us
#define RC5_MAX_HALFBIT_LENGTH_MICROS   1000 ///< Max half bit value in us

static uint16_t receivedFrame;      ///< The whole received frame
static uint8_t frameAddress;        ///< Remote address
static uint8_t frameCommand;        ///< Remote command
static Boolean frameToggleBit;      ///< Toggle bit
static int pulseCount;              ///< Counts the number of half bits
static int bitCount;                ///< Counts the number of bits received
static int numberOfReceivedFrames;  ///< Received frames counter
/**
 * @brief RC5 commands
 */
typedef enum {
  RC5_DIGIT0          = 0, //!< RC5_DIGIT0
  RC5_DIGIT1          = 1, //!< RC5_DIGIT1
  RC5_DIGIT2          = 2, //!< RC5_DIGIT2
  RC5_DIGIT3          = 3, //!< RC5_DIGIT3
  RC5_DIGIT4          = 4, //!< RC5_DIGIT4
  RC5_DIGIT5          = 5, //!< RC5_DIGIT5
  RC5_DIGIT6          = 6, //!< RC5_DIGIT6
  RC5_DIGIT7          = 7, //!< RC5_DIGIT7
  RC5_DIGIT8          = 8, //!< RC5_DIGIT8
  RC5_DIGIT9          = 9, //!< RC5_DIGIT9
  RC5_STANDBY         = 12,//!< RC5_STANDBY
  RC5_MUTE            = 13,//!< RC5_MUTE
  RC5_VOLUME_UP       = 16,//!< RC5_VOLUME_UP
  RC5_VOLUME_DOWN     = 17,//!< RC5_VOLUME_DOWN
  RC5_BRIGHTNESS_UP   = 18,//!< RC5_BRIGHTNESS_UP
  RC5_BRIGHTNESS_DOWN = 19,//!< RC5_BRIGHTNESS_DOWN
  RC5_CHANNEL_UP      = 32,//!< RC5_CHANNEL_UP
  RC5_CHANNEL_DOWN    = 33,//!< RC5_CHANNEL_DOWN
  RC5_SLEEP           = 38,//!< RC5_SLEEP
  RC5_TXT             = 60,//!< RC5_TXT
} RemoteCommandsRc5;

/**
 * @brief Initialize decoding IR signals.
 * @details Sets up callback functions for lower layers
 * and the frame timeout time.
 * After the timeout is over the frame is reset.
 * If an edge is detected on the line then information about
 * it is transported to the higher layer via the read data callback.
 */
void IrCodes_initialize(void) {
  IrCodesHal_initialize(receiveDataCb, resetFrameCb, RC5_FRAME_TIMEOUT_MICROS);
}
/**
 * @brief Decode RC5 data
 * @details This function is called by the lower layer every time
 * a transition on the IR data line occurs (rising or falling edge).
 * The period is the time between two falling edges (the IR data
 * line is pulled up in idle state).
 * @param pulseWidthMicros Width of the received pulse in us.
 * @param pulse IR_LOW_PULSE - low pulse (rising edge), IR_HIGH_PULSE - high pulse (falling edge)
 */
void receiveDataCb(int pulseWidthMicros, IrPulseState pulse) {

  // frame starts with a falling edge
  // since the data line is normally high (pullup)
  // frame visualization: -----|_
  if (pulseCount == 0 && pulse == IR_HIGH_PULSE) {
    pulseCount++;
    bitCount = RC5_NUMBER_OF_BITS_IN_FRAME;
    receivedFrame = 0; // new frame
    return;
  } else if (pulseCount == 0 && pulse == IR_LOW_PULSE) {
    // frame should start with falling edge
    resetFrameCb();
    return;
  }

  // Pulse width was irrelevant at first edge.
  // It is relevant for all following edges
  if ((pulseWidthMicros > RC5_MAX_BIT_LENGTH_MICROS) ||
      (pulseWidthMicros < RC5_MIN_HALFBIT_LENGTH_MICROS)) {
    println("Frame error - wrong pulse width");
    resetFrameCb();
    return;
  }

  // frame visualization: -----|_|-
  if (pulseCount == 1) {
    // pulse width has to be 800 us - first two bits are a one
    if (pulseWidthMicros > RC5_MAX_HALFBIT_LENGTH_MICROS) {
      println("Frame error - wrong start bits, probably not RC5");
      resetFrameCb();
      return;
    }
    receivedFrame |= (1<<bitCount--); // First bit (put as MSB) is a one
    pulseCount++;
  // frame visualization: -----|_|-|_
  } else if (pulseCount == 2) {
    // pulseWidth has to be 800 us - first two bits are a one
    if (pulseWidthMicros > RC5_MAX_HALFBIT_LENGTH_MICROS) {
      println("Frame error - wrong start bits, probably not RC5");
      resetFrameCb();
      return;
    }
    receivedFrame |= (1<<bitCount--); // Second bit is a one
    pulseCount++;
  } else if (bitCount < 12) { // for bits 11 to 0
    // if pulseWidth is about 1700us then two half bits were transmitted
    // so increment pulseCount one more time
    if (pulseWidthMicros > RC5_MAX_HALFBIT_LENGTH_MICROS) {
      pulseCount++;
    }
    // even pulseCounts mean middle of bit
    // rising edge in the middle of a bit means a zero
    // falling edge is a one
    if (pulseCount % 2 == 0) {
      if (pulse == IR_HIGH_PULSE) { // falling edge
        receivedFrame |= (1<<bitCount--);
      } else { // rising edge
        bitCount--;
      }
    }
    // when bit zero is written, bitCount is -1
    if (bitCount == -1) {
      resetFrameCb(); // reset frame
      numberOfReceivedFrames++; // add received frame
      frameToggleBit = (receivedFrame>>11) & (0x0001); // toggle bit is 3rd bit
      frameAddress = (receivedFrame>>6) & (0x001f); // address is 5 bits
      frameCommand = (receivedFrame>>0) & (0x003f); // command is 6 bits
      println("Frame received: %04x. Toggle = %d Command = %d Address = %d",
          receivedFrame, frameToggleBit, frameCommand, frameAddress);
      return;
    }
    pulseCount++; // increment pulse count value
  }

}
/**
 * @brief Resets frame after timeout.
 */
void resetFrameCb(void) {
  bitCount = RC5_NUMBER_OF_BITS_IN_FRAME;
  pulseCount = 0;
}

/**
 * @}
 */

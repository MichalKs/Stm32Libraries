/**
 * @file    serial_port.c
 * @brief   Communication with PC functions.
 * @date    08.10.2016
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

#include "serial_port.h"
#include "fifo.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

#ifndef COMM_DEBUG
  #define COMM_DEBUG
#endif

#ifdef COMM_DEBUG
  #define print(str, args...) printf("COMM--> "str"%s",##args,"\r")
  #define println(str, args...) printf("COMM--> "str"%s",##args,"\r\n")
#else
  #define print(str, args...) (void)0
  #define println(str, args...) (void)0
#endif

/**
 * @addtogroup SERIAL_PORT
 * @{
 */
#define TRANSMIT_BUFFER_LENGTH  512   ///< Transmit buffer length
#define RECEIVE_BUFFER_LENGTH   32    ///< Receive buffer length

static char receiveBuffer[RECEIVE_BUFFER_LENGTH];       ///< Buffer for received data.
static char transmitBuffer[TRANSMIT_BUFFER_LENGTH];     ///< Buffer for transmitted data.
static Fifo receiveFifo;                                ///< RX FIFO
static Fifo transmitFifo;                               ///< TX FIFO
static const char TERMINATOR_CHARACTER = '\r';          ///< Frame terminator character

static int frameCounter; ///< Nonzero signals a new frame (number of received frames)

static UsartTransmission getMoreDataToTransmit(void);
static void receiveNewDataFromHal(char receivedCharacter);

/**
 * @brief Initialize communication terminal interface.
 * @param baudRate Required baud rate
 */
void SerialPort_initialize(int baudRate) {
  UsartHalInitialization usartInitialization;
  usartInitialization.baudRate = baudRate;
  usartInitialization.getMoreDataToTransmit = getMoreDataToTransmit;
  usartInitialization.sendDataToUpperLayer = receiveNewDataFromHal;
  Usart_initialize(DEBUG_CONSOLE_USART, &usartInitialization);
  // Initialize RX FIFO for receiving data from PC
  Fifo_addNewFifo(&receiveFifo, receiveBuffer, RECEIVE_BUFFER_LENGTH);
  // Initialize TX FIFO for transferring data to PC
  Fifo_addNewFifo(&transmitFifo, transmitBuffer, TRANSMIT_BUFFER_LENGTH);
}
/**
 * @brief Send a char to PC.
 * @details This function can be called in _write for printf to work
 * @param characterToSend Character to send.
 */
void SerialPort_putCharacter(char characterToSend) {
  // disable IRQ so it doesn't screw up FIFO count - leads to errors in transmission
  Usart_disableIrq(DEBUG_CONSOLE_USART);
  Fifo_push(&transmitFifo, characterToSend);
  // enable transmitter if inactive
  if (!Usart_isSendingData(DEBUG_CONSOLE_USART)) {
    Usart_sendDataIrq(DEBUG_CONSOLE_USART);
  }
  // enable IRQ again
  Usart_enableIrq(DEBUG_CONSOLE_USART);
}
/**
 * @brief Send string to PC with newline
 * @param line Line to send
 */
void SerialPort_printLine(char * line) {
  for (unsigned int i = 0; i < strlen(line); i++) {
    SerialPort_putCharacter(line[i]);
  }
  SerialPort_putCharacter('\r');
  SerialPort_putCharacter('\n');
}
/**
 * @brief Get a char from PC
 * @return Received character
 * @warning Blocking function! Waits until char is received.
 */
char SerialPort_getCharacter(void) {
  char receivedCharacter;
  while (Fifo_isEmpty(&receiveFifo)); // wait until buffer is not empty
  Fifo_pop(&receiveFifo, &receivedCharacter); // Get data from RX buffer
  return (char)receivedCharacter;
}
/**
 * @brief Get a complete frame from PC (nonblocking)
 * @param buf Buffer for data (data will be null terminated for easier string manipulation)
 * @param len Length not including terminator character of frame
 * @retval COMM_GOT_FRAME Received frame
 * @retval COMM_NO_FRAME_READY No frame in buffer
 * @retval COMM_FRAME_ERROR Frame error
 */
SerialPortResultCode SerialPort_getFrame(char* frameBuffer, int* length, int maximumLength) {
  // FIXME Probably add some IRQ synchronization here
  *length = 0;
  if (frameCounter) {
    while (TRUE) {
      // no more data and terminator wasn't reached => error
      if (Fifo_isEmpty(&receiveFifo)) {
        frameCounter = 0;
        *length = 0;
        println("Invalid frame");
        Fifo_flush(&receiveFifo);
        return SERIAL_PORT_FRAME_ERROR;
      }

      char receivedByte;
      Fifo_pop(&receiveFifo, &receivedByte);
      frameBuffer[(*length)++] = receivedByte;

      if (*length >= maximumLength) {
        frameCounter = 0;
        *length = 0;
        println("Frame too long");
        Fifo_flush(&receiveFifo);
        return SERIAL_PORT_FRAME_TOO_LARGE;
      }

      // if end of frame
      if (receivedByte == TERMINATOR_CHARACTER) {
        (*length)--; // length without terminator character
        frameBuffer[*length] = 0; // terminator character converted to NULL terminator
        frameCounter--;
        return SERIAL_PORT_GOT_FRAME;
      }
    }
  } else {
    return SERIAL_PORT_NO_FRAME_READY;
  }
}
/**
 * @brief Callback for receiving data from PC.
 * @param receivedCharacter Data sent from lower layer software.
 */
void receiveNewDataFromHal(char receivedCharacter) {
  FifoResultCode result = Fifo_push(&receiveFifo, receivedCharacter);
  // Checking result to ensure no buffer overflow occurred
  if ((receivedCharacter == TERMINATOR_CHARACTER) && (result == 0)) {
    frameCounter++;
  }
}
/**
 * @brief Callback for transmitting data to lower layer
 * @param dataToTransmit Transmitted data
 * @return Number of bytes to be transmitted
 */
UsartTransmission getMoreDataToTransmit(void) {
  // Buffer for transmitted data for HAL module (double buffering)
  static char transmitBufferHal[TRANSMIT_BUFFER_LENGTH];
  UsartTransmission transmission = {0};

  if (Fifo_isEmpty(&transmitFifo)) {
    return transmission;
  }
  // get all the data at one go
  int i = 0;
  while (Fifo_pop(&transmitFifo, transmitBufferHal + i) == FIFO_OK) {
    i++;
  }
  transmission.transmitBuffer = transmitBufferHal;
  transmission.bufferLength = i;
  return transmission;
}
/**
 * @}
 */

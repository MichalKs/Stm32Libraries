/**
 * @file    comm.h
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
#include <string.h>
#include <stdio.h>
#include <usart.h>

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
 * @addtogroup COMM
 * @{
 */

#define TRANSMIT_BUFFER_LENGTH  UART_BUF_LEN_TX ///< Transmit buffer length
#define RECEIVE_BUFFER_LENGTH   32              ///< Receive buffer length
#define TERMINATOR_CHARACTER    '\r'            ///< Frame terminator character

static char receiveBuffer[RECEIVE_BUFFER_LENGTH];    ///< Buffer for received data.
static char transmitBuffer[TRANSMIT_BUFFER_LENGTH];  ///< Buffer for transmitted data.
static FIFO_Typedef receiveFifo;  ///< RX FIFO
static FIFO_Typedef transmitFifo; ///< TX FIFO
static int frameCounter; ///< Nonzero signals a new frame (number of received frames)

static int transmitCb(char* characterToSend);
static void receiveCb(char receivedCharacter);

/**
 * @brief Initialize communication terminal interface.
 * @param baudRate Required baud rate
 */
void SerialPort_initialize(int baudRate) {

  // pass baud rate
  // callback for received data and callback for transmitted data
  Usart_initialize(USART_HAL_USART2, baudRate, receiveCb, transmitCb);

  // Initialize RX FIFO for receiving data from PC
  FIFO_Add(&receiveFifo, receiveBuffer, RECEIVE_BUFFER_LENGTH);

  // Initialize TX FIFO for transferring data to PC
  FIFO_Add(&transmitFifo, transmitBuffer, TRANSMIT_BUFFER_LENGTH);

}
/**
 * @brief Send a char to PC.
 * @details This function can be called in _write for printf to work
 *
 * @param characterToSend Character to send.
 */
void SerialPort_putCharacter(char characterToSend) {

  // disable IRQ so it doesn't screw up FIFO count - leads to errors in transmission
  UART_DisableIrq();

  FIFO_Push(&transmitFifo, characterToSend); // Put data in TX buffer

  // enable transmitter if inactive
  if (!UART_IsSendingData()) {
    UART_SendDataIrq();
  }

  // enable IRQ again
  UART_EnableIrq();
}
/**
 * @brief Send string to PC with newline
 * @param line Line to send
 */
void SerialPort_printLine(char* line) {
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
  uint8_t receivedCharacter;
  while (FIFO_IsEmpty(&receiveFifo)); // wait until buffer is not empty
  FIFO_Pop(&receiveFifo, &receivedCharacter); // Get data from RX buffer
  return (char)receivedCharacter;
}
/**
 * @brief Get a complete frame from PC(nonblocking)
 * @param buf Buffer for data (data will be null terminated for easier string manipulation)
 * @param len Length not including terminator character
 * @retval COMM_GOT_FRAME Received frame
 * @retval COMM_NO_FRAME_READY No frame in buffer
 * @retval COMM_FRAME_ERROR Frame error
 */
SerialPortResultCode SerialPort_getFrame(char* frameBuffer, int* length, int maximumLength) {

  char receivedByte;
  *length = 0;

  if (frameCounter) {
    while (TRUE) {

      // no more data and terminator wasn't reached => error
      if (FIFO_IsEmpty(&receiveFifo)) {
        frameCounter = 0;
        *length = 0;
        println("Invalid frame");
        FIFO_Flush(&receiveFifo);
        return SERIAL_PORT_FRAME_ERROR;
      }

      FIFO_Pop(&receiveFifo, &receivedByte);
      frameBuffer[(*length)++] = receivedByte;

      if (*length >= maximumLength) {
        frameCounter = 0;
        *length = 0;
        println("Frame too long");
        FIFO_Flush(&receiveFifo);
        return SERIAL_PORT_FRAME_TOO_LARGE;
      }

      // if end of frame
      if (receivedByte == TERMINATOR_CHARACTER) {
        (*length)--; // length without terminator character
        frameBuffer[*length] = 0; // terminator character converted to NULL terminator
        break;
      }

    }
    frameCounter--;
    return SERIAL_PORT_GOT_FRAME;

  } else {
    return SERIAL_PORT_NO_FRAME_READY;
  }
}
/**
 * @brief Callback for receiving data from PC.
 * @param c Data sent from lower layer software.
 */
void receiveCb(char receivedCharacter) {

  // Put data in RX buffer
  FIFO_ErrorTypedef result = FIFO_Push(&receiveFifo, receivedCharacter);

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
int transmitCb(char* dataToTransmit) {

  char* localBuffer = dataToTransmit;

  if (FIFO_IsEmpty(&transmitFifo)) {
    return 0;
  }

  // get all the data at one go
  int i = 0;
  while (FIFO_Pop(&transmitFifo, localBuffer+i) == FIFO_OK) {
    i++;
  }

  return i;
}
/**
 * @}
 */

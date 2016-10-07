/**
 * @file    comm.c
 * @brief   Communication with PC functions.
 * @date    25 sie 2014
 * @author  Michal Ksiezopolski
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

#include <comm.h>
#include <fifo.h>
#include <stdio.h>
#include <uart.h>
#include <string.h>

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

static uint8_t receiveBuffer[RECEIVE_BUFFER_LENGTH];    ///< Buffer for received data.
static uint8_t transmitBuffer[TRANSMIT_BUFFER_LENGTH];  ///< Buffer for transmitted data.
static FIFO_Typedef receiveFifo;  ///< RX FIFO
static FIFO_Typedef transmitFifo; ///< TX FIFO
static int frameCounter; ///< Nonzero signals a new frame (number of received frames)

static int transmitCb(char* characterToSend);
static void receiveCb(char receivedCharacter);

/**
 * @brief Initialize communication terminal interface.
 * @param baudRate Required baud rate
 */
void COMM_Initialize(int baudRate) {

  // pass baud rate
  // callback for received data and callback for transmitted data
  UART_Initialize(baudRate, receiveCb, transmitCb);

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
void COMM_PutCharacter(char characterToSend) {

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
void COMM_PrintLine(char* line) {
  for (unsigned int i = 0; i < strlen(line); i++) {
    COMM_PutCharacter(line[i]);
  }
  COMM_PutCharacter('\r');
  COMM_PutCharacter('\n');
}
/**
 * @brief Get a char from PC
 * @return Received char.
 * @warning Blocking function! Waits until char is received.
 */
char COMM_GetCharacter(void) {

  uint8_t receivedCharacter;

  while (FIFO_IsEmpty(&receiveFifo)); // wait until buffer is not empty

  FIFO_Pop(&receiveFifo, &receivedCharacter); // Get data from RX buffer

  return (char)receivedCharacter;
}
/**
 * @brief Get a complete frame from PC(nonblocking)
 * @param buf Buffer for data (data will be null terminated for easier string manipulation)
 * @param len Length not including terminator character
 * @retval 0 Received frame
 * @retval 1 No frame in buffer
 * @retval 2 Frame error
 * TODO Add maximum length checking so as not to overflow
 */
int COMM_GetFrame(uint8_t* buf, int* len) {

  uint8_t c;
  *len = 0; // zero out length variable

  if (frameCounter) {
    while (1) {

      // no more data and terminator wasn't reached => error
      if (FIFO_IsEmpty(&receiveFifo)) {
        *len = 0;
        println("Invalid frame");
        return 2;
      }
      FIFO_Pop(&receiveFifo, &c);
      buf[(*len)++] = c;

      // if end of frame
      if (c == TERMINATOR_CHARACTER) {
        (*len)--; // length without terminator character
        buf[*len] = 0; // USART terminator character converted to NULL terminator
        break;
      }

    }
    frameCounter--;
    return 0;

  } else {

    return 1;
  }
}
/**
 * @brief Callback for receiving data from PC.
 * @param c Data sent from lower layer software.
 */
void receiveCb(char receivedCharacter) {

  uint8_t res = FIFO_Push(&receiveFifo, receivedCharacter); // Put data in RX buffer

  // Checking res to ensure no buffer overflow occurred
  if ((receivedCharacter == TERMINATOR_CHARACTER) && (res == 0)) {
    frameCounter++;
  }
}
/**
 * @brief Callback for transmitting data to lower layer
 * @param c Transmitted data
 * @retval 0 There is no more data in buffer (stop transmitting)
 * @retval 1 Valid data in c
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

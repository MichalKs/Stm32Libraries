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

#ifndef COMM_H_
#define COMM_H_

#include <inttypes.h>

/**
 * @defgroup  COMM COMM
 * @brief     Communication with PC functions.
 */

/**
 * @addtogroup COMM
 * @{
 */
/**
 * @brief Communication errors
 */
typedef enum {
  SERIAL_PORT_GOT_FRAME,      //!< COMM_GOT_FRAME
  SERIAL_PORT_NO_FRAME_READY, //!< SERIAL_PORT_NO_FRAME_READY
  SERIAL_PORT_FRAME_ERROR,    //!< SERIAL_PORT_FRAME_ERROR
  SERIAL_PORT_FRAME_TOO_LARGE,//!< SERIAL_PORT_FRAME_TOO_LARGE
} SerialPortResultCode;

void                 SerialPort_initialize   (int baudRate);
void                 SerialPort_putCharacter (char characterToSend);
char                 SerialPort_getCharacter (void);
SerialPortResultCode SerialPort_getFrame     (char* frameBuffer, int* length, int maximumLength);
void                 SerialPort_printLine    (char* line);
/**
 * @}
 */

#endif /* COMM_H_ */

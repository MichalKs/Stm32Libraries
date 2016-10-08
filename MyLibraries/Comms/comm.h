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
  COMM_GOT_FRAME,      //!< COMM_GOT_FRAME
  COMM_NO_FRAME_READY, //!< COMM_NO_FRAME_READY
  COMM_FRAME_ERROR,    //!< COMM_FRAME_ERROR
  COMM_FRAME_TOO_LARGE,//!< COMM_FRAME_TOO_LARGE
} COMM_ErrorTypedef;

void              COMM_Initialize   (int baudRate);
void              COMM_PutCharacter (char characterToSend);
char              COMM_GetCharacter (void);
COMM_ErrorTypedef COMM_GetFrame     (char* frameBuffer, int* length, int maximumLength);
void              COMM_PrintLine    (char* line);
/**
 * @}
 */

#endif /* COMM_H_ */

/**
 * @file    fifo.h
 * @brief   First in first out buffer implementation
 * @date    07.10.2016
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

#ifndef FIFO_H_
#define FIFO_H_

#include "utils.h"

/**
 * @defgroup  FIFO FIFO
 * @brief     First in first out buffer functions
 */

/**
 * @addtogroup FIFO
 * @{
 */

/**
 * @brief FIFO structure typedef.
 */
typedef struct {
  int   head;   ///< Head
  int   tail;   ///< Tail
  char* buffer; ///< Pointer to buffer
  int   length; ///< Maximum length of FIFO
  int   count;  ///< Current number of data elements
} FIFO_Typedef;

typedef enum {
  FIFO_OK,
  FIFO_ZERO_LENGTH,
  FIFO_NULL_BUFFER,
  FIFO_FULL,
  FIFO_EMPTY,

} FIFO_ErrorTypedef;


int     FIFO_Add    (FIFO_Typedef* fifo, char* dataBuffer, int length);
int     FIFO_Push   (FIFO_Typedef* fifo, char c);
int     FIFO_Pop    (FIFO_Typedef* fifo, char* c);
Boolean FIFO_IsEmpty(FIFO_Typedef* fifo);

/**
 * @}
 */

#endif /* FIFO_H_ */

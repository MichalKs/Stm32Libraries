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
  int   head;       ///< Head
  int   tail;       ///< Tail
  char* dataBuffer; ///< Pointer to buffer
  int   length;     ///< Maximum length of FIFO
  int   count;      ///< Current number of data elements
} Fifo;
/**
 * @brief Error codes
 */
typedef enum {
  FIFO_OK,         //!< FIFO_OK
  FIFO_ZERO_LENGTH,//!< FIFO_ZERO_LENGTH
  FIFO_NULL_BUFFER,//!< FIFO_NULL_BUFFER
  FIFO_FULL,       //!< FIFO_FULL
  FIFO_EMPTY,      //!< FIFO_EMPTY
} FifoResultCode;

FifoResultCode Fifo_addNewFifo (Fifo * fifo, char * dataBuffer, int length);
FifoResultCode Fifo_push       (Fifo * fifo, char newData);
FifoResultCode Fifo_pop        (Fifo * fifo, char * data);
Boolean        Fifo_isEmpty    (Fifo * fifo);
void           Fifo_flush      (Fifo * fifo);
/**
 * @}
 */

#endif /* FIFO_H_ */

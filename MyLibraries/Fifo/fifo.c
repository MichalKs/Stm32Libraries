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

#include <fifo.h>
#include <stdio.h>

#ifdef DEBUG_FIFO
  #define print(str, args...) printf("FIFO--> "str"%s",##args,"\r")
  #define println(str, args...) printf("FIFO--> "str"%s",##args,"\r\n")
#else
  #define print(str, args...) (void)0
  #define println(str, args...) (void)0
#endif

/**
 * @addtogroup FIFO
 * @{
 */

/**
 * @brief Add a FIFO.
 *
 * @details To add a FIFO, you need to define a FIFO_TypeDef
 * structure and initialize it with the proper length and
 * buffer pointer. The rest is handled automatically.
 *
 * @param fifo Pointer to FIFO structure
 * @retval FIFO_OK FIFO added successfully
 * @retval FIFO_ZERO_LENGTH FIFO length is 0
 * @retval FIFO_NULL_BUFFER Received buffer is null
 */
FIFO_ErrorTypedef FIFO_Add(FIFO_Typedef* fifo, char* dataBuffer, int length) {

  if (length == 0) {
    return FIFO_ZERO_LENGTH;
  }

  if (dataBuffer == NULL) {
    return FIFO_NULL_BUFFER;
  }

  fifo->dataBuffer = dataBuffer;
  fifo->length = length;
  fifo->tail  = 0;
  fifo->head  = 0;
  fifo->count = 0;

  return FIFO_OK;
}
/**
 * @brief Pushes data to FIFO.
 * @param fifo Pointer to FIFO structure
 * @param newData Data byte
 * @retval FIFO_OK Data added
 * @retval FIFO_FULL FIFO is full
 */
FIFO_ErrorTypedef FIFO_Push(FIFO_Typedef* fifo, char newData) {

  // Check for overflow
  if (fifo->count == fifo->length) {
    return FIFO_FULL;
  }

  fifo->dataBuffer[fifo->head++] = newData;
  fifo->count++;

  if (fifo->head == fifo->length) {
    fifo->head = 0; // start from beginning
  }

  return FIFO_OK;
}
/**
 * @brief Pops data from the FIFO.
 * @param fifo Pointer to FIFO structure
 * @param c data
 * @retval FIFO_OK Got valid data
 * @retval FIFO_EMPTY FIFO is empty
 */
FIFO_ErrorTypedef FIFO_Pop(FIFO_Typedef* fifo, char* c) {
  if (fifo->count == 0) {
    return FIFO_EMPTY;
  }

  *c = fifo->dataBuffer[fifo->tail++];
  fifo->count--;

  if (fifo->tail == fifo->length) {
    fifo->tail = 0; // start from beginning
  }

  return FIFO_OK;
}
/**
 * @brief Checks whether the FIFO is empty.
 * @param fifo Pointer to FIFO structure
 * @retval TRUE FIFO is empty
 * @retval FALSE FIFO is not empty
 */
Boolean FIFO_IsEmpty(FIFO_Typedef* fifo) {
  if (fifo->count == 0) {
    return TRUE;
  }
  return FALSE;
}
void FIFO_Flush(FIFO_Typedef* fifo) {
  fifo->tail  = 0;
  fifo->head  = 0;
  fifo->count = 0;
}
/**
 * @}
 */

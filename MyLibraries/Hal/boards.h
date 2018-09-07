/*
 * boards.h
 *
 *  Created on: 7 wrz 2018
 *      Author: michal
 */

#ifndef MYLIBRARIES_HAL_BOARDS_H_
#define MYLIBRARIES_HAL_BOARDS_H_

#ifdef BOARD_STM32F4_DISCOVERY
  #include <stm32f4xx_hal.h>
#endif
#ifdef BOARD_STM32F7_DISCOVERY
  #include <stm32f7xx_hal.h>
#endif

#endif /* MYLIBRARIES_HAL_BOARDS_H_ */

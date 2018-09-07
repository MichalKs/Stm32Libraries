/*
 * f7_discovery_defs.h
 *
 *  Created on: 02.06.2017
 *      Author: mik
 */

#ifndef MYLIBRARIES_LED_HAL_F7_DISCOVERY_DEFS_H_
#define MYLIBRARIES_LED_HAL_F7_DISCOVERY_DEFS_H_

#define LED1_PORT GPIOI
#define LED1_PIN  GPIO_PIN_1

/**
 * @brief Definition of LEDs in HAL
 */
static LedHalDefinition ledHalDefinitions[BOARD_MAXIMUM_AVAILABLE_LEDS] = {
    {LED1_PORT, LED1_PIN},
};

#endif /* MYLIBRARIES_LED_HAL_F7_DISCOVERY_DEFS_H_ */

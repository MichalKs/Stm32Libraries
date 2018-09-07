/*
 * f4_discovery_defs.h
 *
 *  Created on: 02.06.2017
 *      Author: mik
 */

#ifndef MYLIBRARIES_LED_HAL_F4_DISCOVERY_DEFS_H_
#define MYLIBRARIES_LED_HAL_F4_DISCOVERY_DEFS_H_

#define LED1_PORT GPIOD
#define LED1_PIN  GPIO_PIN_12
#define LED2_PORT GPIOD
#define LED2_PIN  GPIO_PIN_13
#define LED3_PORT GPIOD
#define LED3_PIN  GPIO_PIN_14
#define LED4_PORT GPIOD
#define LED4_PIN  GPIO_PIN_15

/**
 * @brief Definition of LEDs in HAL
 */
static LedHalDefinition ledHalDefinitions[BOARD_MAXIMUM_AVAILABLE_LEDS] = {
    {LED1_PORT, LED1_PIN},
    {LED2_PORT, LED2_PIN},
    {LED3_PORT, LED3_PIN},
    {LED4_PORT, LED4_PIN},
};

#endif /* MYLIBRARIES_LED_HAL_F4_DISCOVERY_DEFS_H_ */

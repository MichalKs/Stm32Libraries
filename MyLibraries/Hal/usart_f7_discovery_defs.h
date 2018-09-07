/*
 * usart_f7_discovery_defs.h
 *
 *  Created on: 02.06.2017
 *      Author: mik
 */

#ifndef MYLIBRARIES_HAL_USART_F7_DISCOVERY_DEFS_H_
#define MYLIBRARIES_HAL_USART_F7_DISCOVERY_DEFS_H_

#define USART6_CLK_ENABLE()              __HAL_RCC_USART6_CLK_ENABLE()
#define USART6_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define USART6_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define USART6_FORCE_RESET()             __HAL_RCC_USART6_FORCE_RESET()
#define USART6_RELEASE_RESET()           __HAL_RCC_USART6_RELEASE_RESET()
#define USART6_TX_PIN                    GPIO_PIN_6
#define USART6_TX_GPIO_PORT              GPIOC
#define USART6_TX_AF                     GPIO_AF8_USART6
#define USART6_RX_PIN                    GPIO_PIN_7
#define USART6_RX_GPIO_PORT              GPIOC
#define USART6_RX_AF                     GPIO_AF8_USART6
#define USART6_IRQ_NUMBER                USART6_IRQn
#define USART6_IRQ_PRIORITY              15

#endif /* MYLIBRARIES_HAL_USART_F7_DISCOVERY_DEFS_H_ */
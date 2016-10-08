/**
 * @file    uart.c
 * @brief   Controlling the UART
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

#include "uart.h"
#include "common_hal.h"
#include <stm32f4xx_hal.h>

/**
 * @addtogroup UART
 * @{
 */
// hardware configuration
#define USART                           USART2
#define USART_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
#define USART_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USART_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()
#define USART_TX_PIN                    GPIO_PIN_2
#define USART_TX_GPIO_PORT              GPIOA
#define USART_TX_AF                     GPIO_AF7_USART2
#define USART_RX_PIN                    GPIO_PIN_3
#define USART_RX_GPIO_PORT              GPIOA
#define USART_RX_AF                     GPIO_AF7_USART2
#define USART_IRQ_NUMBER                USART2_IRQn
#define USART_IRQ_HANDLER               USART2_IRQHandler
#define USART_IRQ_PRIORITY              15

#define RECEIVE_BUFFER_LENGTH 1             ///< Length of the low level receive buffer
static void (*rxCallback)(char);            ///< Callback function for receiving data
static int  (*txCallback)(char*);           ///< Callback function for transmitting data (fills up buffer with data to send)
static UART_HandleTypeDef uartHandle;       ///< Handle for UART peripheral
static char rxBuffer[RECEIVE_BUFFER_LENGTH];///< Reception buffer - we receive one character at a time
static volatile Boolean isSendingData;      ///< Flag saying if UART is currently sending any data

/**
 * @brief Enables UART IRQ
 */
void UART_EnableIrq(void) {
  HAL_NVIC_EnableIRQ(USART_IRQ_NUMBER);
}
/**
 * @brief Disables UART IRQ
 */
void UART_DisableIrq(void) {
  HAL_NVIC_DisableIRQ(USART_IRQ_NUMBER);
}
/**
 * @brief Checks if UART is currently sending any data
 * @details If so the IRQ will automatically get new data from the FIFO. If not we
 * have to explicitly call HAL_UART_SendData to enable the TX IRQ.
 * @retval TRUE UART is sending data
 * @retval FALSE UART is not sending data
 */
Boolean UART_IsSendingData(void) {
  return isSendingData;
}
/**
 * @brief Sends data using the UART IRQ
 * @details This function is called automatically when the TX IRQ is running.
 * However if the IRQ is not running this function has to be called manually to
 * enable the IRQ.
 */
void UART_SendDataIrq(void) {

  // has to be static to serve as a buffer for UART
  static char buf[UART_BUF_LEN_TX];

  // if no function set do nothing
  if (txCallback == NULL) {
    return;
  }

  // get the data
  int numberOfBytes = txCallback(buf);

  // if there is any data in the FIFO
  if (numberOfBytes > 0) {
    // send it to PC
    if (HAL_UART_Transmit_IT(&uartHandle, (uint8_t*)buf, numberOfBytes) != HAL_OK) {
      COMMON_HAL_ErrorHandler();
    }
    isSendingData = TRUE;
  } else {
    isSendingData = FALSE;
  }
}
/**
 * @brief Initialize UART
 * @param baud Baud rate
 * @param rxCb Receive callback
 * @param txCb Transmit callback
 */
void UART_Initialize(int baud, void(*rxCb)(char), int(*txCb)(char*) ) {

  txCallback = txCb;
  rxCallback = rxCb;

  uartHandle.Instance        = USART;
  uartHandle.Init.BaudRate   = baud;
  uartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  uartHandle.Init.StopBits   = UART_STOPBITS_1;
  uartHandle.Init.Parity     = UART_PARITY_NONE;
  uartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  uartHandle.Init.Mode       = UART_MODE_TX_RX;

  if (HAL_UART_Init(&uartHandle) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }

  if(HAL_UART_Receive_IT(&uartHandle, (uint8_t*)rxBuffer, RECEIVE_BUFFER_LENGTH) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }
}
// ********************** HAL UART callbacks and IRQs **********************
/**
  * @brief  Transfer completed callback
  * @param  uart UART handle
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * uart) {

  if (rxCallback == NULL) {
    return;
  }

  // send the received char to upper layer
  rxCallback(*rxBuffer);

  // start another reception
  if (HAL_UART_Receive_IT(uart, (uint8_t *)(rxBuffer), RECEIVE_BUFFER_LENGTH) != HAL_OK) {
    COMMON_HAL_ErrorHandler();
  }
}
/**
 * @brief Transfer completed callback (called whenever IRQ sends the whole buffer)
 * @param uart UART handle
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef * uart) {
  UART_SendDataIrq();
}
/**
 * @brief Initialize low level UART
 * @param uart UART handle pointer
 */
void HAL_UART_MspInit(UART_HandleTypeDef * uart) {

  GPIO_InitTypeDef  gpioInitalization;

  USART_TX_GPIO_CLK_ENABLE();
  USART_RX_GPIO_CLK_ENABLE();
  USART_CLK_ENABLE();

  gpioInitalization.Pin       = USART_TX_PIN;
  gpioInitalization.Mode      = GPIO_MODE_AF_PP;
  gpioInitalization.Pull      = GPIO_PULLUP;
  gpioInitalization.Speed     = GPIO_SPEED_FAST;
  gpioInitalization.Alternate = USART_TX_AF;
  HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpioInitalization);

  gpioInitalization.Pin       = USART_RX_PIN;
  gpioInitalization.Alternate = USART_RX_AF;
  HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpioInitalization);

  HAL_NVIC_SetPriority(USART_IRQ_NUMBER, USART_IRQ_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(USART_IRQ_NUMBER);
}
/**
  * @brief Deinitialize low level UART
  * @param uart UART handle pointer
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef * uart) {
  USART_FORCE_RESET();
  USART_RELEASE_RESET();

  HAL_GPIO_DeInit(USART_TX_GPIO_PORT, USART_TX_PIN);
  HAL_GPIO_DeInit(USART_RX_GPIO_PORT, USART_RX_PIN);

  HAL_NVIC_DisableIRQ(USART_IRQ_NUMBER);
}
/**
 * @brief  This function handles UART interrupt request.
 */
void USART_IRQ_HANDLER(void) {
  HAL_UART_IRQHandler(&uartHandle);
}
/**
 * @}
 */

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

#include "usart.h"
#include "common_hal.h"
#ifdef BOARD_STM32F4_DISCOVERY
  #include "usart_f4_discovery_defs.h"
#endif
#ifdef BOARD_STM32F7_DISCOVERY
  #include "usart_f7_discovery_defs.h"
#endif

/**
 * @addtogroup UART
 * @{
 */

/**
 * @brief Length of the low level receive buffer
 * @details This length assumes that the receive function will do its job before the next
 * byte comes. For baud rate 115200 and 10 bits per one byte (adding stop bit and start bit) this
 * means that the byte rate is 11520 bytes per second meaning that a byte may come every 87us -
 * more than enough time to pack up the previous byte in some receive buffer
 */
#define RECEIVE_BUFFER_LENGTH       1
#define NUMBER_OF_AVAILABLE_USARTS  2 ///< Number of available USARTs

/**
 * @brief Basic USART strucutre
 */
typedef struct {
  UsartNumber usartNumber;                              ///< USART number
  UART_HandleTypeDef * handle;                          ///< USART handle
  void (*sendDataToUpperLayer)(char receivedCharacter); ///< Function for sending received data to upper layer
  UsartTransmission (*getMoreDataToTransmit)(void);     ///< Function for getting more data to transmit (fills up buffer with data to send)
  char receiveBuffer[RECEIVE_BUFFER_LENGTH];            ///< Receive buffer
  Boolean isSendingData;                                ///< Flag saying if UART is currently sending any data
  Boolean isInitialized;
} UsartControl;

static UART_HandleTypeDef usart2Handle;     ///< Handle for UART peripheral
static UART_HandleTypeDef usart6Handle;     ///< Handle for UART peripheral

UsartControl usartControl[NUMBER_OF_AVAILABLE_USARTS]; ///< The USARTs

static UsartNumber getUsartNumberFromHandle(UART_HandleTypeDef * usartHandle);

/**
 * @brief Initialize UART
 * @param baud Baud rate
 * @param rxCb Receive callback
 * @param txCb Transmit callback
 */
void Usart_initialize(UsartNumber usart, UsartHalInitialization * usartInitialization) {
  if (usart >= NUMBER_OF_AVAILABLE_USARTS) {
    return;
  }
  switch(usart) {
  case USART_HAL_USART2:
    usart2Handle.Instance = USART2;
    usartControl[usart].handle = &usart2Handle;
    break;
  case USART_HAL_USART6:
    usart2Handle.Instance = USART6;
    usartControl[usart].handle = &usart6Handle;
    break;
  default:
    return;
  }
  usartControl[usart].usartNumber = usart;
  usartControl[usart].isInitialized = TRUE;
  usartControl[usart].sendDataToUpperLayer = usartInitialization->sendDataToUpperLayer;
  usartControl[usart].getMoreDataToTransmit = usartInitialization->getMoreDataToTransmit;
  usartControl[usart].isSendingData = FALSE;

  usartControl[usart].handle->Init.BaudRate   = usartInitialization->baudRate;
  usartControl[usart].handle->Init.WordLength = UART_WORDLENGTH_8B;
  usartControl[usart].handle->Init.StopBits   = UART_STOPBITS_1;
  usartControl[usart].handle->Init.Parity     = UART_PARITY_NONE;
  usartControl[usart].handle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  usartControl[usart].handle->Init.Mode       = UART_MODE_TX_RX;

  if (HAL_UART_Init(usartControl[usart].handle) != HAL_OK) {
    CommonHal_errorHandler();
  }

  if (HAL_UART_Receive_IT(usartControl[usart].handle,
      (uint8_t*)usartControl[usart].receiveBuffer, RECEIVE_BUFFER_LENGTH) != HAL_OK) {
    CommonHal_errorHandler();
  }
}
/**
 * @brief Enables UART IRQ
 */
void Usart_enableIrq(UsartNumber usart) {
  switch(usart) {
  case USART_HAL_USART2:
    HAL_NVIC_EnableIRQ(USART2_IRQ_NUMBER);
    break;
  case USART_HAL_USART6:
    HAL_NVIC_EnableIRQ(USART6_IRQ_NUMBER);
    break;
  }
}
/**
 * @brief Disables UART IRQ
 */
void Usart_disableIrq(UsartNumber usart) {
  switch(usart) {
  case USART_HAL_USART2:
    HAL_NVIC_DisableIRQ(USART2_IRQ_NUMBER);
    break;
  case USART_HAL_USART6:
    HAL_NVIC_DisableIRQ(USART6_IRQ_NUMBER);
    break;
  }
}
/**
 * @brief Checks if UART is currently sending any data
 * @details If so the IRQ will automatically get new data from the FIFO. If not we
 * have to explicitly call HAL_UART_SendData to enable the TX IRQ.
 * @retval TRUE UART is sending data
 * @retval FALSE UART is not sending data
 */
Boolean Usart_isSendingData(UsartNumber usart) {
  if (usart >= NUMBER_OF_AVAILABLE_USARTS) {
    return FALSE;
  }
  return usartControl[usart].isSendingData;
}
/**
 * @brief Sends data using the UART IRQ
 * @details This function is called automatically when the TX IRQ is running.
 * However if the IRQ is not running this function has to be called manually to
 * enable the IRQ.
 */
void Usart_sendDataIrq(UsartNumber usart) {
  if (usart >= NUMBER_OF_AVAILABLE_USARTS) {
    return;
  }
  if (usartControl[usart].getMoreDataToTransmit == NULL) {
    return;
  }

  UsartTransmission transmission = usartControl[usart].getMoreDataToTransmit();
  // if there is any data in the FIFO
  if (transmission.bufferLength > 0) {
    // send it to PC
    if (HAL_UART_Transmit_IT(usartControl[usart].handle,
        (uint8_t*)transmission.transmitBuffer, transmission.bufferLength) != HAL_OK) {
      CommonHal_errorHandler();
    }
    usartControl[usart].isSendingData = TRUE;
  } else {
    usartControl[usart].isSendingData = FALSE;
  }
}
/**
 * @brief Gets USART number based on the handle
 * @details Used for identifying USART in IRQ handler
 * @return USART number
 */
UsartNumber getUsartNumberFromHandle(UART_HandleTypeDef * usartHandle) {
  if (usartHandle == &usart2Handle) {
    return USART_HAL_USART2;
  }
  if (usartHandle == &usart6Handle) {
    return USART_HAL_USART6;
  }
  return USART_HAL_EMPTY;
}
// ********************** HAL UART callbacks and IRQs **********************
/**
  * @brief  Transfer completed callback
  * @param  usartHandle UART handle
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * usartHandle) {
  UsartNumber usart = getUsartNumberFromHandle(usartHandle);
  if (usart == USART_HAL_EMPTY) {
    return;
  }
  if (usartControl[usart].sendDataToUpperLayer == NULL) {
    return;
  }
  // send the received char to upper layer
  usartControl[usart].sendDataToUpperLayer(*usartControl[usart].receiveBuffer);
  // start another reception
  if (HAL_UART_Receive_IT(usartControl[usart].handle,
      (uint8_t*)usartControl[usart].receiveBuffer, RECEIVE_BUFFER_LENGTH) != HAL_OK) {
    CommonHal_errorHandler();
  }
}
/**
 * @brief Transfer completed callback (called whenever IRQ sends the whole buffer)
 * @param uart UART handle
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef * usartHandle) {
  UsartNumber usart = getUsartNumberFromHandle(usartHandle);
  if (usart == USART_HAL_EMPTY) {
    return;
  }
  Usart_sendDataIrq(usart);
}
/**
 * @brief Initialize low level UART
 * @param uart UART handle pointer
 */
void HAL_UART_MspInit(UART_HandleTypeDef * usartHandle) {
  if (usartHandle == &usart2Handle) {
    USART2_TX_GPIO_CLK_ENABLE();
    USART2_RX_GPIO_CLK_ENABLE();
    USART2_CLK_ENABLE();
    GPIO_InitTypeDef  gpioInitalization;
    gpioInitalization.Pin       = USART2_TX_PIN;
    gpioInitalization.Mode      = GPIO_MODE_AF_PP;
    gpioInitalization.Pull      = GPIO_PULLUP;
    gpioInitalization.Speed     = GPIO_SPEED_FAST;
    gpioInitalization.Alternate = USART2_TX_AF;
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &gpioInitalization);
    gpioInitalization.Pin       = USART2_RX_PIN;
    gpioInitalization.Alternate = USART2_RX_AF;
    HAL_GPIO_Init(USART2_RX_GPIO_PORT, &gpioInitalization);
    HAL_NVIC_SetPriority(USART2_IRQ_NUMBER, USART2_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQ_NUMBER);
  } else if (usartHandle == &usart6Handle) {
    USART6_TX_GPIO_CLK_ENABLE();
    USART6_RX_GPIO_CLK_ENABLE();
#ifdef BOARD_STM32F7_DISCOVERY
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART6;
    RCC_PeriphClkInit.Usart6ClockSelection = RCC_USART6CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
#endif
    USART6_CLK_ENABLE();
    GPIO_InitTypeDef  gpioInitalization;
    gpioInitalization.Pin       = USART6_TX_PIN;
    gpioInitalization.Mode      = GPIO_MODE_AF_PP;
    gpioInitalization.Pull      = GPIO_PULLUP;
    gpioInitalization.Speed     = GPIO_SPEED_FAST;
    gpioInitalization.Alternate = USART6_TX_AF;
    HAL_GPIO_Init(USART6_TX_GPIO_PORT, &gpioInitalization);
    gpioInitalization.Pin       = USART6_RX_PIN;
    gpioInitalization.Alternate = USART6_RX_AF;
    HAL_GPIO_Init(USART6_RX_GPIO_PORT, &gpioInitalization);
    HAL_NVIC_SetPriority(USART6_IRQ_NUMBER, USART6_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQ_NUMBER);
  }
}
/**
  * @brief Deinitialize low level UART
  * @param uart UART handle pointer
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef * usartHandle) {
  if (usartHandle == &usart2Handle) {
    USART2_FORCE_RESET();
    USART2_RELEASE_RESET();
    HAL_GPIO_DeInit(USART2_TX_GPIO_PORT, USART2_TX_PIN);
    HAL_GPIO_DeInit(USART2_RX_GPIO_PORT, USART2_RX_PIN);
    HAL_NVIC_DisableIRQ(USART2_IRQ_NUMBER);
  } else if (usartHandle == &usart6Handle) {
    USART6_FORCE_RESET();
    USART6_RELEASE_RESET();
    HAL_GPIO_DeInit(USART6_TX_GPIO_PORT, USART6_TX_PIN);
    HAL_GPIO_DeInit(USART6_RX_GPIO_PORT, USART6_RX_PIN);
    HAL_NVIC_DisableIRQ(USART6_IRQ_NUMBER);
  }
}
/**
 * @brief This function handles UART interrupt request.
 */
void USART2_IRQHandler(void) {
  HAL_UART_IRQHandler(&usart2Handle);
}
/**
 * @brief This function handles UART interrupt request.
 */
void USART6_IRQHandler(void) {
  HAL_UART_IRQHandler(&usart6Handle);
}
/**
 * @}
 */

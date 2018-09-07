/**
 * @file    common_hal.h
 * @brief   Common HAL files for STM32
 * @date    08.10.2016
 * @author  Michal Ksiezopolski
 *
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

#include "common_hal.h"
#include "led_hal.h"
#include "utils.h"

#define HAL_ERROR_LED_NUMBER 3 ///< Number of LED for HAL errors

#ifdef BOARD_STM32F4_DISCOVERY
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  */
static void systemClockConfig(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM        = 8;
  RCC_OscInitStruct.PLL.PLLN        = 336;
  RCC_OscInitStruct.PLL.PLLP        = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ        = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    CommonHal_errorHandler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType       = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider   = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider  = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider  = RCC_HCLK_DIV2;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    CommonHal_errorHandler();
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001) {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}
#endif

#ifdef BOARD_STM32F7_DISCOVERY
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  */
static void systemClockConfig(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    CommonHal_errorHandler();
  }

  /* activate the OverDrive to reach the 216 Mhz Frequency */
  if(HAL_PWREx_EnableOverDrive() != HAL_OK) {
    CommonHal_errorHandler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) {
    CommonHal_errorHandler();
  }
}
/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  */
static void configureMpu(void) {
  MPU_Region_InitTypeDef mpuInitialization;
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  mpuInitialization.Enable = MPU_REGION_ENABLE;
  mpuInitialization.BaseAddress = 0x20010000;
  mpuInitialization.Size = MPU_REGION_SIZE_256KB;
  mpuInitialization.AccessPermission = MPU_REGION_FULL_ACCESS;
  mpuInitialization.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  mpuInitialization.IsCacheable = MPU_ACCESS_CACHEABLE;
  mpuInitialization.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  mpuInitialization.Number = MPU_REGION_NUMBER0;
  mpuInitialization.TypeExtField = MPU_TEX_LEVEL0;
  mpuInitialization.SubRegionDisable = 0x00;
  mpuInitialization.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&mpuInitialization);

  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}
/**
  * @brief  CPU L1-Cache enable.
  */
static void enableCpuCache(void) {
  SCB_EnableICache();
  SCB_EnableDCache();
}
#endif
/**
 * @brief Initialize HAL layer
 */
void CommonHal_initialize(void) {

  // Initialize LED for signaling errors
  LedHal_initialize(HAL_ERROR_LED_NUMBER);
  LedHal_changeLedState(HAL_ERROR_LED_NUMBER, FALSE);

#ifdef BOARD_STM32F7_DISCOVERY
  /* Configure the MPU attributes as Write Through */
  configureMpu();

  /* Enable the CPU Cache */
  enableCpuCache();
#endif

  /* STM32 HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  systemClockConfig();
}

/**
 * @brief Error handler for HAL
 */
void CommonHal_errorHandler(void) {
  LedHal_changeLedState(HAL_ERROR_LED_NUMBER, TRUE);
  while(TRUE) {

  }
}
/**
  * @brief   This function handles NMI exception.
  */
void NMI_Handler(void) {

}
/**
  * @brief  This function handles Hard Fault exception.
  */
void HardFault_Handler(void) {
  LedHal_changeLedState(HAL_ERROR_LED_NUMBER, TRUE);
  while (TRUE) {

  }
}
/**
  * @brief  This function handles Memory Manage exception.
  */
void MemManage_Handler(void) {
  LedHal_changeLedState(HAL_ERROR_LED_NUMBER, TRUE);
  while (TRUE) {

  }
}
/**
  * @brief  This function handles Bus Fault exception.
  */
void BusFault_Handler(void) {
  LedHal_changeLedState(HAL_ERROR_LED_NUMBER, TRUE);
  while (TRUE) {

  }
}
/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void) {
  LedHal_changeLedState(HAL_ERROR_LED_NUMBER, TRUE);
  while (TRUE) {

  }
}
/**
  * @brief  This function handles SVCall exception.
  */
void SVC_Handler(void) {

}
/**
  * @brief  This function handles Debug Monitor exception.
  */
void DebugMon_Handler(void) {

}
/**
  * @brief  This function handles PendSVC exception.
  */
void PendSV_Handler(void) {

}

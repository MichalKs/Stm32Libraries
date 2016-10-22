/*
 * LCDConf.c
 *
 *  Created on: 21.10.2016
 *      Author: mik
 */

#include <GUIDRV_FlexColor.h>
#include "LCDConf.h"
#include "ili9320.h"
#include "ili9320_hal.h"

/**
 * @brief Callback for LCD initalization for STemWin.
 */
void LCD_X_Config(void) {

  GUI_DEVICE * guiDevice;
  CONFIG_FLEXCOLOR configuration = {0};
  GUI_PORT_API portApi = {0};

  guiDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_565, 0, 0);
  configuration.Orientation = GUI_SWAP_XY;
  GUIDRV_FlexColor_Config(guiDevice, &configuration);

  LCD_SetSizeEx (0, ILI9320_DISPLAY_SIZE_X, ILI9320_DISPLAY_SIZE_Y);
  LCD_SetVSizeEx(0, ILI9320_DISPLAY_SIZE_X, ILI9320_DISPLAY_SIZE_Y);

  portApi.pfWrite16_A0  = ILI9320_HAL_WriteAddress;
  portApi.pfWrite16_A1  = ILI9320_HAL_WriteData;
  portApi.pfWriteM16_A0 = ILI9320_HAL_WriteAddressMultiple;
  portApi.pfWriteM16_A1 = ILI9320_HAL_WriteDataBuffer;
  portApi.pfReadM16_A1  = ILI9320_HAL_ReadDataBuffer;
  GUIDRV_FlexColor_SetFunc(guiDevice, &portApi, GUIDRV_FLEXCOLOR_F66708,
      GUIDRV_FLEXCOLOR_M16C0B16);

}
/**
 * @brief Library callback for LCD configuration
 * @param layerIndex Index of layer
 * @param command Command
 * @param data Additional data
 * @retval -1 Error
 * @retval 0 OK
 */
int LCD_X_DisplayDriver(unsigned int layerIndex, unsigned int command, void * data) {

  switch (command) {

  case LCD_X_INITCONTROLLER:
    ILI9320_Initializtion();
    return 0;

  default:
    return -1;
  }

}

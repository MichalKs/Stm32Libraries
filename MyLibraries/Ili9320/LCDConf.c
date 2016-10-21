/*
 * LCDConf.c
 *
 *  Created on: 21.10.2016
 *      Author: mik
 */

#include "GUI_Private.h"
#include "GUIDRV_FlexColor.h"
#include "LCDConf.h"
#include "ili9320.h"
#include "ili9320_hal.h"

void LCD_X_Config(void) {

  GUI_DEVICE * guiDevice;
  CONFIG_FLEXCOLOR configuration = {0};
  GUI_PORT_API portApi = {0};

  guiDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_M565, 0, 0);
  configuration.Orientation = 1;
  GUIDRV_FlexColor_Config(guiDevice, &configuration);

  LCD_SetSizeEx (0, ILI9320_DISPLAY_SIZE_X, ILI9320_DISPLAY_SIZE_Y);
  LCD_SetVSizeEx(0, ILI9320_DISPLAY_SIZE_X, ILI9320_DISPLAY_SIZE_Y);

  portApi.pfWrite16_A0  = ILI9320_HAL_WriteAddress;
  portApi.pfWrite16_A1  = ILI9320_HAL_WriteData;
  portApi.pfWriteM16_A1 = ILI9320_HAL_WriteDataBuffer;
  portApi.pfReadM16_A1  = ILI9320_HAL_ReadDataBuffer;
  GUIDRV_FlexColor_SetFunc(guiDevice, &portApi, GUIDRV_FLEXCOLOR_F66708,
      GUIDRV_FLEXCOLOR_M16C0B16);

}

/**
  * @brief  This function is called by the display driver for several purposes.
  *         To support the according task the routine needs to be adapted to
  *         the display controller. Please note that the commands marked with
  *         'optional' are not cogently required and should only be adapted if
  *         the display controller supports these features
  * @param  LayerIndex: Index of layer to be configured
  * @param  Cmd       :Please refer to the details in the switch statement below
  * @param  pData     :Pointer to a LCD_X_DATA structure
  * @retval Status (-1 : Error,  0 : Ok)
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

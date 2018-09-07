/*
 * stemwin_gui.c
 *
 *  Created on: 22.10.2016
 *      Author: mik
 */

#include "stemwin_gui.h"
#include "utils.h"
#include "tsc2046.h"
#include <GUI.h>
#include <WM.h>
#include <FRAMEWIN.h>
#include <BUTTON.h>
#include <stdio.h>

#define DEBUG_STGUI

#ifdef DEBUG_STGUI
#define print(str, args...) printf(""str"%s",##args,"")
#define println(str, args...) printf("MAIN--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

#define EXAMPLE_FRAME_ID (GUI_ID_USER+1) ///< ID of frame

static void frameCallback(WM_MESSAGE * message);
static void okButtonCallback(WM_MESSAGE * message);
static void touchCallback(int x, int y);

/**
 * @brief Initalize STemWin based GUI
 */
void ST_GUI_Init(void) {
  // FIXME Put this in hal for GUI
//  __HAL_RCC_CRC_CLK_ENABLE();
  GUI_Init();

  TSC2046_Initialize();
  TSC2046_RegisterEvent(0, 0, 1500, 4000, touchCallback);

  WM_SetDesktopColor(GUI_RED);
  FRAMEWIN_Handle frame = FRAMEWIN_CreateEx(0, 0, 200, 100,
      0, WM_CF_SHOW, 0, EXAMPLE_FRAME_ID, "Example frame", &frameCallback);

  FRAMEWIN_AddMaxButton(frame, FRAMEWIN_BUTTON_RIGHT, FALSE);
  FRAMEWIN_AddMinButton(frame, FRAMEWIN_BUTTON_RIGHT, FALSE);
  FRAMEWIN_AddCloseButton(frame, FRAMEWIN_BUTTON_RIGHT, FALSE);

  BUTTON_Handle okButton = BUTTON_CreateEx(20, 20, 50, 30, frame, WM_CF_SHOW, 0,
      GUI_ID_BUTTON0);
  BUTTON_SetText(okButton, "OK");
  WM_SetCallback(okButton, okButtonCallback);

}
/**
 * @brief Run STemWin based GUI
 */
void ST_GUI_Run(void) {
  TSC2046_Update();
  GUI_Exec();
}
/**
 * @brief Callback for frame
 * @param message
 */
void frameCallback(WM_MESSAGE * message) {
  switch(message->MsgId) {
    default:
      FRAMEWIN_Callback(message);
      break;
  }
}
/**
 * @brief Callback for OK button
 * @param message
 */
void okButtonCallback(WM_MESSAGE * message) {

  if ((message->MsgId == WM_NOTIFY_PARENT_REFLECTION) &&
    (message->Data.v == WM_NOTIFICATION_RELEASED)) {
    println("OK button pressed");
  } else {
    BUTTON_Callback(message);
  }

}

void touchCallback(int x, int y) {
  println("GUI touch");
}

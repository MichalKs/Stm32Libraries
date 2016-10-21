/*
 * GUIConf.c
 *
 *  Created on: 21.10.2016
 *      Author: mik
 */

#include "GUI.h"

#define BYTES_PER_KILOBYTE    1024 ///< Number of bytes in kilobyte
#define GRAPHICS_MEMORY_SIZE_BYTES  100 * BYTES_PER_KILOBYTE ///< StemWin memory in bytes
#define WORD_SIZE 4 ///< Word size on MCU

U32 graphicsMemory[GRAPHICS_MEMORY_SIZE_BYTES / WORD_SIZE]; ///< Memory array for STemwin

/**
 * @brief This is an initalization function called by the GUI_Init() function
 * of STemWin
 */
void GUI_X_Config(void) {
  // Assign memory to graphics library
  GUI_ALLOC_AssignMemory(graphicsMemory, GRAPHICS_MEMORY_SIZE_BYTES);
}

/**
 * @file: 	onewire.c
 * @brief:	   
 * @date: 	30 lip 2014
 * @author: Michal Ksiezopolski
 * 
 * @verbatim
 * Copyright (c) 2014 Michal Ksiezopolski.
 * All rights reserved. This program and the 
 * accompanying materials are made available 
 * under the terms of the GNU Public License 
 * v3.0 which accompanies this distribution, 
 * and is available at 
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#ifndef ONEWIRE_C_
#define ONEWIRE_C_

#include "utils.h"

/**
 * @brief Result codes
 */
typedef enum {
  ONEWIRE_RESULT_OK,        //!< ONEWIRE_RESULT_OK
  ONEWIRE_NO_DEVICES_ON_BUS,//!< ONEWIRE_NO_DEVICES_ON_BUS
} OnewireResultCode;

OnewireResultCode Onewire_resetBus  (void);
char              Onewire_readByte  (void);
void              Onewire_writeByte (uint8_t data);
void              Onewire_initialize(void);
OnewireResultCode Onewire_readRom   (char * buf);
void              Onewire_matchRom  (char * rom);

#endif /* ONEWIRE_C_ */

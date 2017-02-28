/**
 * @file: 	onewire_hal.h
 * @brief:	ONEWIRE low level functions
 * @date: 	9 paź 2014
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

#ifndef ONEWIRE_HAL_H_
#define ONEWIRE_HAL_H_

#include "utils.h"

void    OnewireHal_releaseBus  (void);
void    OnewireHal_busLow      (void);
Boolean OnewireHal_readBus     (void);
void    OnewireHal_initialize  (void);

#endif /* ONEWIRE_HAL_H_ */

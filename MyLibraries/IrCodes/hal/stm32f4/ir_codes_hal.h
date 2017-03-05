/**
 * @file: 	ir_hal.h
 * @brief:	
 * @date: 	3 gru 2014
 * @author: Michal Ksiezopolski
 *
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
#ifndef INC_IR_HAL_H_
#define INC_IR_HAL_H_

#include "utils.h"

void IR_HAL_Init(
    void (*readDataCb)(uint16_t pulseWidth, uint8_t edge),
    void (*resetFrameCb)(void),
    uint32_t timeout);


#endif /* INC_IR_HAL_H_ */

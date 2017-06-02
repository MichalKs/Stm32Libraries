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

/**
 * @brief Pulse level
 */
typedef enum {
  IR_LOW_PULSE, //!< IR_LOW_PULSE
  IR_HIGH_PULSE,//!< IR_HIGH_PULSE
} IrPulseState;

void IrCodesHal_initialize(
    void (*readDataCb)(int pulseWidth, IrPulseState pulse),
    void (*resetFrameCb)(void),
    int timeout);

#endif /* INC_IR_HAL_H_ */

/**
 * @file    tsc2046.h
 * @brief   TSC2046 touch screen library
 * @date    09.10.2016
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
#ifndef INC_TSC2046_H_
#define INC_TSC2046_H_

#include <inttypes.h>

/**
 * @defgroup  TSC2046 TSC2046
 * @brief     TSC2046 touch screen library
 */

/**
 * @addtogroup TSC2046
 * @{
 */
void  TSC2046_Initialize    (void);
void  TSC2046_Update        (void);
int   TSC2046_RegisterEvent (int x, int y, int width, int height,
        void (*eventCb)(int x, int y));
/**
 * @}
 */

#endif /* INC_TSC2046_H_ */

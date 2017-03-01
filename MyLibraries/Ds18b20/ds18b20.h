/**
 * @file 	  ds18b20.h
 * @brief	  DS18B20 thermometer library
 * @date 	  01.03.2017
 * @author  Michal Ksiezopolski
 * 
 * @verbatim
 * Copyright (c) 2017 Michal Ksiezopolski.
 * All rights reserved. This program and the 
 * accompanying materials are made available 
 * under the terms of the GNU Public License 
 * v3.0 which accompanies this distribution, 
 * and is available at 
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "utils.h"

/**
 * @brief Result codes
 */
typedef enum {
  DS18B20_RESULT_OK,       //!< DS18B20_RESULT_OK
  DS18B20_NO_DEVICE_ON_BUS,//!< DS18B20_NO_DEVICE_ON_BUS
} Ds18b20ResultCode;

float             Ds18b20_readTemperatureCelsius  (void);
Ds18b20ResultCode Ds18b20_initialize              (void);
void              Ds18b20_conversionStart         (void);

#endif /* DS18B20_H_ */

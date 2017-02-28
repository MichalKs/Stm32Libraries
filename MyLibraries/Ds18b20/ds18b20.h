/**
 * @file: 	ds18b20.h
 * @brief:	   
 * @date: 	5 sie 2014
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

#ifndef DS18B20_H_
#define DS18B20_H_

#include <inttypes.h>

double  Ds18b20_readTemperatureCelsius        (void);
uint8_t Ds18b20_initialize            (void);
void    Ds18b20_conversionStart (void);

#endif /* DS18B20_H_ */

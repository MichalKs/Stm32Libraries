/**
 * @file: 	hmc5883l.h
 * @brief:	   
 * @date: 	11 maj 2014
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

#ifndef HMC5883L_H_
#define HMC5883L_H_

#include "utils.h"

void Hmc5883l_initialize(void);
float Hmc5883l_readAngle(void);

#endif /* HMC5883L_H_ */

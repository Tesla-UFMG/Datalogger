/*
 * Initializers.h
 *
 *  Created on: Aug 26, 2021
 *      Author: Lucas
 */

#ifndef INC_INITIALIZERS_H_
#define INC_INITIALIZERS_H_

#include "stm32h7xx.h"


void init_CAN();
void debug_temp(uint16_t debug_periodo_ms); //não tinha ela no .h, apesar de estar no .c

#endif /* INC_INITIALIZERS_H_ */

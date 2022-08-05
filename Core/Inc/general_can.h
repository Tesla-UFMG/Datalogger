/*
 * general_can.h
 *
 *  Created on: Aug 26, 2021
 *      Author: Lucas
 */

#ifndef INC_GENERAL_CAN_H_
#define INC_GENERAL_CAN_H_

#include "stm32h7xx.h"

void initialize_general_CAN(FDCAN_HandleTypeDef* can_ref);
HAL_StatusTypeDef CAN_poll();
void general_can_transmit(uint32_t id, uint16_t* data);

#endif /* INC_GENERAL_CAN_H_ */

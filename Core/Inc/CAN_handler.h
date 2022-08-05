/*
 * CAN_handler.h
 *
 *  Created on: Aug 26, 2021
 *      Author: Lucas
 */

#ifndef INC_CAN_HANDLER_H_
#define INC_CAN_HANDLER_H_

#include "stm32h7xx.h"

void initialize_CAN(FDCAN_HandleTypeDef* hfdcan, FDCAN_TxHeaderTypeDef* TxHeader);

void can_transmit(FDCAN_HandleTypeDef* hfdcan, FDCAN_TxHeaderTypeDef* TxHeader,
                  uint32_t id, uint16_t* data);

#endif /* INC_CAN_HANDLER_H_ */

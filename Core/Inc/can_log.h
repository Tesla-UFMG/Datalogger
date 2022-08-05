/*
 * can_log.h
 *
 *  Created on: Oct 6, 2021
 *      Author: Lucas
 */

#ifndef CAN_LOG_H_
#define CAN_LOG_H_

#include "stm32h7xx.h"

#define CAN_IDS_NUMBER 400

/* CAN message struct */
typedef struct{
  uint16_t word_0;
  uint16_t word_1;
  uint16_t word_2;
  uint16_t word_3;
}CanIdData_t;

void Clean_CAN_Struct(void);

void canMessageReceived(uint16_t id, uint16_t *data);

#endif /* CAN_LOG_H_ */

/*
 * can_log.c
 *
 *  Created on: Oct 6, 2021
 *      Author: Lucas
 */
#include "can_log.h"

// ONDE EU CRIO ESSA STRUCT? ELA TEM Q SER EXTERN NÉ?

CanIdData_t can_vector[CAN_IDS_NUMBER];



void Clean_CAN_Struct(void) {
	for (uint16_t i = 0; i < CAN_IDS_NUMBER; i++) {
		can_vector[i].word_0 = 0;
		can_vector[i].word_1 = 0;
		can_vector[i].word_2 = 0;
		can_vector[i].word_3 = 0;
	}
}

void canMessageReceived(uint16_t id, uint8_t *data) {
	if (id > CAN_IDS_NUMBER - 1)
		return;
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_6);
	uint16_t *data_word = (uint16_t*) data;
	can_vector[id].word_0 = data_word[0];
	can_vector[id].word_1 = data_word[1];
	can_vector[id].word_2 = data_word[2];
	can_vector[id].word_3 = data_word[3];
}

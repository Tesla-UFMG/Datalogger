#include "general_can.h"
#include "CAN_handler.h"
#include "SD.h"
#include "can_log.h"

static FDCAN_HandleTypeDef* can_ptr;;

static FDCAN_TxHeaderTypeDef TxHeader;

uint8_t RxData[8];
FDCAN_RxHeaderTypeDef RxHeader;
uint16_t datageneral[4];
uint32_t idgeneral;


//função que inicializa a can geral, chamada em initializer.c
void initialize_general_CAN(FDCAN_HandleTypeDef* can_ref) {
	can_ptr = can_ref;
	void CAN_general_receive_callback(FDCAN_HandleTypeDef*, uint32_t);
	initialize_CAN(can_ptr, CAN_general_receive_callback, &TxHeader);
}



//função usada para transmitir alguma mensagem
void general_can_transmit(uint32_t id, uint16_t* data) {
	can_transmit(can_ptr, &TxHeader, id, data);
}



//função de callback, chamada quando chega qualquer mensagem, de qualquer ID
void CAN_general_receive_callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs)  {
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
			/* Reception Error */
			Error_Handler();
		}

		idgeneral = RxHeader.Identifier;
		for(int i = 0; i < 8; i += 2){
			datageneral[i/2] = (uint16_t)((RxData[i+1] << 8) | RxData[i]);
		}
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_6); //A LED DA CAN (PIN 3, TÁ QUEIMADA)

		canMessageReceived(idgeneral, datageneral);

		if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
			/* Notification Error */
			Error_Handler();
		}
	}
}



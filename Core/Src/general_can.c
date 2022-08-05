#include "general_can.h"

#include "CAN_handler.h"
#include "SD.h"
#include "can_log.h"

static FDCAN_HandleTypeDef* can_ptr;
;

static FDCAN_TxHeaderTypeDef TxHeader;

uint8_t RxData[8];
FDCAN_RxHeaderTypeDef RxHeader;
uint16_t datageneral[4];
uint32_t idgeneral;

// função que inicializa a can geral, chamada em initializer.c
void initialize_general_CAN(FDCAN_HandleTypeDef* can_ref) {
    can_ptr = can_ref;
    initialize_CAN(can_ptr, &TxHeader);
}

// função usada para transmitir alguma mensagem
void general_can_transmit(uint32_t id, uint16_t* data) {
    can_transmit(can_ptr, &TxHeader, id, data);
}

// função de callback, chamada quando chega qualquer mensagem, de qualquer ID
HAL_StatusTypeDef CAN_poll() {
    if (HAL_FDCAN_GetRxFifoFillLevel(can_ptr, FDCAN_RX_FIFO0) < 1) {
        // if the queue is empty returns OK and dont't get any message
        return HAL_OK;
    }

    //
    if (HAL_FDCAN_GetRxMessage(can_ptr, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
        // if the message failed returns HAL_ERROR and dont't save message
        return HAL_ERROR;
    }

    idgeneral = RxHeader.Identifier;
    for (int i = 0; i < 8; i += 2) {
        datageneral[i / 2] = (uint16_t)((RxData[i + 1] << 8) | RxData[i]);
    }
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_6); // A LED DA CAN (PIN 3, TÁ QUEIMADA)

    canMessageReceived(idgeneral, datageneral);

    // after saving message there still might be messages left to be read, so returns BUSY
    return HAL_BUSY;
}

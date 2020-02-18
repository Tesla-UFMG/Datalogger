#ifndef CAN_LOG_H_
#define CAN_LOG_H_

//Includes:
#include "main.h"
#include "stm32f1xx_hal.h"
//#include "can.h"
//#include "spi.h"
//#include "gpio.h"
#include "fatfs.h"
//Declaring functions:
FRESULT SD_Create_File();
void canMessageReceived(uint16_t id,uint8_t* data);
void writeSD(void);
void cabecalho(void);
#endif //END CAN_LOG_H

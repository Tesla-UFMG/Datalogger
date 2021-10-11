/*
 * SD.h
 *
 *  Created on: Sep 4, 2021
 *      Author: Lucas
 */

#ifndef INC_SD_H_
#define INC_SD_H_

#include "main.h"
#include "stm32h7xx_hal.h"
#include "fatfs.h"
#include "can_log.h"

#define CAN_IDS_NUMBER 400

FRESULT SD_Create_File();
void writeSD(void);


#endif /* INC_SD_H_ */

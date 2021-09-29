///*
// * SD.c
// *
// *  Created on: Sep 4, 2021
// *      Author: Lucas
// */
//
//#include "SD.h"
//#include <stdio.h>
//#include <string.h>
//#include <stdarg.h>
//
//static FATFS g_sFatFs;
//FIL file;
//char bufferFile[20];//buffer with the name of the file
//int count = 0;
//uint8_t _datalog_flag = 0;
//
//
//FRESULT SD_Create_File(void)
//{
//	FRESULT fresult;
//	uint8_t contFile = 0;	/*enumerate the file*/
//	fresult = f_mount(&g_sFatFs, "0:", 0);	/*mount SD card*/
//	do
//	{
//		sprintf(bufferFile, "ARQ%02d.txt", contFile);
//		fresult = f_stat(bufferFile, &file);
//		contFile++;
//	} while(fresult != FR_NO_FILE);
//	contFile = 0;
//	fresult = f_open(&file, bufferFile, FA_CREATE_ALWAYS ); /*Create file on SD card*/
//	fresult = f_close(&file);
//	if(fresult == FR_OK){
//		//HAL_GPIO_TogglePin(DATALOG_LED_GPIO_Port, DATALOG_LED_Pin); //Blinks the led to indicate that there was no erros
//		_datalog_flag  = 1;
//	}
//	else _datalog_flag = 0;
//	return fresult;
//}
//void writeSD(void)
//{
//	UINT bytes_written;
//	FRESULT fresult;
//	char buffer_log[256];
//
//	int len = sprintf(buffer_log, "\r%u\n",HAL_GetTick());
//	fresult = f_open(&file, bufferFile, FA_OPEN_ALWAYS | FA_WRITE); //open file on SD card to write
//	fresult = f_lseek(&file, file.obj.objsize);//goes to the end of the file
//	fresult = f_write(&file, buffer_log, len, &bytes_written);     //write data to the file
//	len = sprintf(buffer_log, "\r%u\n",HAL_GetTick());
//	fresult = f_write(&file, buffer_log, len, &bytes_written);     //write data to the file
//	fresult = f_close(&file);//closes the file
//	if(fresult == FR_OK){
//		//HAL_GPIO_TogglePin(DATALOG_LED_GPIO_Port, DATALOG_LED_Pin); //Blinks the led to indicate that there was no erros
//		_datalog_flag  = 1;
//	}
//	else _datalog_flag = 0;
//}

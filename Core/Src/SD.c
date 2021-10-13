/*
 * SD.c
 *
 *  Created on: Sep 4, 2021
 *      Author: Lucas
 */

#include "SD.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


extern CanIdData_t can_vector[CAN_IDS_NUMBER];
//temporário
int16_t gyroX, gyroY, gyroZ, accelX, accelY, accelZ, temp;


static FATFS g_sFatFs;
FIL file;
char bufferFile[20];//buffer with the name of the file
int count = 0;
uint8_t _datalog_flag = 0;


FRESULT SD_Create_File(void)
{
	FRESULT fresult;
	uint8_t contFile = 0;	/*enumerate the file*/
	fresult = f_mount(&g_sFatFs, "0:", 0);	/*mount SD card*/
	do
	{
		sprintf(bufferFile, "ARQ%02d.txt", contFile);
		fresult = f_stat(bufferFile, &file);
		contFile++;
	} while(fresult != FR_NO_FILE);
	contFile = 0;
	fresult = f_open(&file, bufferFile, FA_CREATE_ALWAYS ); /*Create file on SD card*/
	fresult = f_close(&file);
	if(fresult == FR_OK){
		//HAL_GPIO_TogglePin(DATALOG_LED_GPIO_Port, DATALOG_LED_Pin); //Blinks the led to indicate that there was no erros
		_datalog_flag  = 1;
	}
	else _datalog_flag = 0;
	return fresult;
}
void writeSD(void)
{
	UINT bytes_written; //(ver live expression)
	FRESULT fresult;
	char buffer_log[256]; //(ver live expression)
	uint8_t block[512];
	uint32_t time = HAL_GetTick();

	//printar tudo do BMS e da ECU

	//como o buffer tem 256 espaços char (1 byte cada), posso imprimir 16 unint 16
	//Não precisa de mais buffers, escrever no cartão SD no meio dos dois HAL_GetTick pra ver qnt tempo leva no salvamento
	//é só fazer o len = sprintf(buffer_log) e escrever as viáveis q tu quiser

	//int len = sprintf(buffer_log, "\r %u %u %u %u %u %u %u %u %u %u\n",time,can_vector[1].word_1,can_vector[101].word_0,(int8_t)can_vector[1].word_0, can_vector[101].word_1, can_vector[101].word_1,can_vector[101].word_2, can_vector[101].word_3);
	int len = snprintf((char*) block, sizeof(block),
			//1   2   3   4   5   6   7   8
			"%lu\t%u\t%u\t%d\t%u\t%u\t%u\t%u\t"	//1//Timer\ECU_Flag\ECU_Timer\ECU_Mode\Volante\Pedal\Hodometro_P\Hodometro_T
			"%u\t%u\t%d\t%u\t%u\t%u\t%u\t%u\t"	//2//Torque_RM\Torque_LM\TRef_R\Tref_L\V_Motor_R\V_Motor_L\Frenagem\Speed_LR
			"%u\t%u\t%u\t%u\t%u\t%u\t%d\t%d\t"	//3//Speed_RR\tref_R\tCurrent_RM\tCurrent_LM\T_Inversor_R\T_Inversor_L\AccelX\AccelY
			"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t"	//4//AccelZ\GiroX\GiroY\GiroZ\Temp_Ambiente\MLX_0\MLX_1\MLX_2
			"%d\t%u\t%u\t%u\t%X\t%X\t%u\t%u\t"	//5//MLX_3\Sensor_Pressao_1\ADC_1\T_Disco_1\Extensometro_1\Extensometro_2\Sensor_Pressao_2\ADC_2
			"%u\t%X\t%X\t%X\t%u\t%u\t%u\t%u\t"	//6//T_Disco_2\Extensometro_3\Extensometro_4\Extensometro_5\ADC_3\Tensao_GLV\Tensao_Total\Temp_Media_Banco
			"%u\t%u\t%d\t%u\t%u\t%u\t%u\t%u\t"	//7//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%d\t%u\t%u\t%u\t%u\n",	//8//Temp_Pack_12\Temp_Pack_21\Temp_Pack_22\Temp_Pack_31\Temp_Pack_32\Temp_Pack_41\Temp_Pack_42\Temp_Pack_51\Temp_Pack_52\test
	/*1	*/	time,can_vector[1].word_1, can_vector[101].word_3, (int8_t)can_vector[1].word_0, can_vector[101].word_1, can_vector[101].word_2,can_vector[102].word_2,can_vector[102].word_3,
	/*2	*/	can_vector[103].word_0,can_vector[103].word_1,can_vector[103].word_2,can_vector[103].word_2,can_vector[104].word_0,can_vector[104].word_1,can_vector[104].word_2,can_vector[105].word_0,
	/*3	*/	can_vector[105].word_1,can_vector[106].word_0,can_vector[106].word_2,can_vector[106].word_3,can_vector[107].word_0,can_vector[107].word_2,accelX,accelY,
	/*4	*/	accelZ,gyroX,gyroY,gyroZ,temp,(int16_t)can_vector[156].word_0,(int16_t)can_vector[156].word_1,(int16_t)can_vector[156].word_2,
	/*5	*/	(int16_t)can_vector[156].word_3,can_vector[170].word_0,can_vector[170].word_1,can_vector[170].word_2,*(uint32_t*)(&(can_vector[171].word_0)),(uint32_t)(can_vector[171].word_1),can_vector[172].word_0,can_vector[172].word_1,
	/*6	*/	can_vector[172].word_2,(uint32_t)(can_vector[173].word_0),(uint32_t)(can_vector[178].word_0),(uint32_t)(can_vector[178].word_2),can_vector[179].word_0,can_vector[52].word_0,can_vector[53].word_1,can_vector[53].word_2,
	/*7	*/	can_vector[53].word_3,can_vector[51].word_0,can_vector[51].word_1,can_vector[51].word_2,can_vector[51].word_3,can_vector[263].word_1,can_vector[263].word_2,can_vector[268].word_1,
	/*8	*/	can_vector[268].word_2,can_vector[273].word_1,can_vector[273].word_2,can_vector[278].word_1,can_vector[278].word_2,can_vector[283].word_1,can_vector[288].word_2);
	fresult = f_open(&file, bufferFile, FA_OPEN_ALWAYS | FA_WRITE); //open file on SD card to write
	fresult = f_lseek(&file, file.obj.objsize);//goes to the end of the file
	fresult = f_write(&file, block/*buffer_log*/, len, &bytes_written);     //write data to the file

	//len = sprintf(buffer_log, "\r%u\n",HAL_GetTick()); //isso é pra imprimir o tempo dnv pra ver qnt tempo demorou pra escrever
	//fresult = f_write(&file,block /*buffer_log*/, len, &bytes_written);     //write data to the file
	fresult = f_close(&file);//closes the file
	if(fresult == FR_OK){
		//HAL_GPIO_TogglePin(DATALOG_LED_GPIO_Port, DATALOG_LED_Pin); //Blinks the led to indicate that there was no erros
		_datalog_flag  = 1;
	}
	else _datalog_flag = 0;
}



/*
 * SD.c
 *
 *  Created on: Sep 4, 2021
 *      Author: Lucas
 */

#include "SD.h"
#include "general_can.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

extern CanIdData_t can_vector[CAN_IDS_NUMBER];

static FATFS g_sFatFs;
FIL file;
FIL file1;
char bufferFile[20];//buffer with the name of the file
int count = 0;
uint8_t _datalog_flag = 0;

int aux;

extern IWDG_HandleTypeDef hiwdg1;

char block[700];

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
		HAL_IWDG_Refresh(&hiwdg1);
	} while(fresult != FR_NO_FILE);
	contFile = 0;
	fresult = f_open(&file, bufferFile, FA_CREATE_ALWAYS ); /*Create file on SD card*/
	fresult = f_close(&file);
	HAL_IWDG_Refresh(&hiwdg1);
	if(fresult == FR_OK){
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_5); //Blinks the led to indicate that there was no erros
		_datalog_flag  = 1;
	}
	else _datalog_flag = 0;

	return fresult;
}

void readSD(void)
{
	//fazer um f_lseek, dps um fgets ou um f_read, colocar num buffer e escolher a posição do hodometro

	//testar o f_lseek normal e o debaixo aí com -1

	if(aux > 0){
		UINT bytes_read;
		FRESULT fresult;
		uint16_t ultima_linha[25];

		sprintf(bufferFile, "ARQ%02d.txt", aux);
		fresult = f_stat(bufferFile, &file1);

		fresult = f_open(&file1, bufferFile, FA_OPEN_ALWAYS | FA_READ); //open file on SD card to write*/
		fresult = f_lseek(&file1, file.obj.objsize - 1);
		fresult = f_read(&file1, ultima_linha, 20, &bytes_read);
		fresult = f_close(&file1);//closes the file

		uint16_t controle[4] = {0, 0, 0, ultima_linha[9]};

		general_can_transmit(102, controle);
	}
}

void Cabecalho(void)
{
	UINT bytes_written;
	FRESULT fresult;

	char cabecalho[1000];
	strcpy(cabecalho,   "TIMER\tVOL\tACELERADOR\tFREIO\tMODO\tHODOMETRO_P\tHODOMETRO_"
	           "T\tTORQUE_R\tTORQUE_L\tTORQUE_REF_R_\tTORQUE_REF_L\tSPEED_R_MOTOR\tSPEED_L_"
	           "MOTOR\tECU_FLAG_1\tECU_FLAG_2\tSPEED_FRONT_L_TIE\tSPEED_FRONT_"
	           "R_TIE\tSPEED_BACK_R_TIE\tSPEED_BACK_L_TIE\tTORQUE_GAIN\tSENTIDO_VOL\tCORR_"
	           "TORQUE_R\tCORR_TORQUE_L\tSPEED_L\tSPEED_R\tPOWER_L\tPOWER_R\t"
	           "ACCEL_X\tACCEL_Y\tACCEL_Z\tGYRO_X\tGYRO_Y\tGYRO_Z\tV_MAX\tV_"
	           "MIN\tDELTA_V\tTEMP_MAX\tCONTATORES_STATUS\tV_TS\tI_"
	           "SENSOR_1_ALTA\tI_SENSOR_2_BAIXA\tI_SENSOR_2_ALTA\tI_SENSOR_3_BAIXA\tI_SENSOR_"
	           "3_ALTA\tI_SENSOR_4_BAIXA\tI_SENSOR_4_"
	           "ALTA\tTemp101\tTemp102\tTemp103\tTemp104\tTem"
	           "p105\tV_TOT_1\tV_REF_1\tTemp201\tTemp202\tTemp203\tTemp204\tTemp205\tV_TOT_"
	           "2\tV_REF_2\tTemp301\tTemp302\tTemp303\tTemp304\tTemp305\tV_TOT_"
	           "3\tV_REF_3\tTemp401\tTemp402\tTemp403\tTemp404\tTemp405"
	           "\tV_TOT_4\tV_REF_4\n");
	int tamanho = strlen(cabecalho);


	fresult = f_open(&file, bufferFile, FA_OPEN_ALWAYS | FA_WRITE); //open file on SD card to write
	fresult = f_write(&file, cabecalho, tamanho, &bytes_written); //write data to the file
	fresult = f_close(&file);
}

void writeSD(void)
{
	UINT bytes_written;
	FRESULT fresult;
	//uint16_t block[310];
	int len;
	uint32_t time = HAL_GetTick();

	len = snprintf((char*) block, sizeof(block),
			//1   2   3   4
			"%lu\t%u\t%u\t%u\t"     //Id 101
			"%u\t%u\t%u\t"	    //Id 102
			"%u\t%u\t%u\t%u\t"      //Id 103
			"%u\t%u\t%u\t%u\t"      //Id 104

			"%u\t%u\t%u\t%u\t"      //Id 105
			"%u\t%u\t%u\t%u\t"      //Id 106
			//"%u\t%u\t%u\t%u\t"		Id 107
			"%u\t%u\t%u\t%u\t"		//Id 109

			"%u\t%u\t%u\t"		//Id 291
			"%u\t%u\t%u\t"      //Id 292

			"%u\t%u\t%u\t%u\t"		//Id 50
			"%u\t%u\t"		//Id 51
			"%u\t%u\t%u\t"		    //Id 54
			"%u\t%u\t%u\t%u\t"		//Id 55

//			"%u\t%u\t%u\t%u\t"		//Id 256
//			"%u\t%u\t%u\t%u\t"		//Id 257
//			"%u\t%u\t%u\t%u\t"		//Id 258
			"%u\t%u\t%u\t%u\t"		//Id 259
			"%u\t%u\t%u\t"		//Id 260

//			"%u\t%u\t%u\t%u\t"		//Id 261
//			"%u\t%u\t%u\t%u\t"		//Id 262
//			"%u\t%u\t%u\t%u\t"		//Id 263
			"%u\t%u\t%u\t%u\t"		//Id 264
			"%u\t%u\t%u\t"		//Id 265

//			"%u\t%u\t%u\t%u\t"		//Id 266
//			"%u\t%u\t%u\t%u\t"		//Id 267
//			"%u\t%u\t%u\t%u\t"		//Id 268
			"%u\t%u\t%u\t%u\t"		//Id 269
			"%u\t%u\t%u\t"		//Id 270

//			"%u\t%u\t%u\t%u\t"		//Id 271
//			"%u\t%u\t%u\t%u\t"		//Id 272
//			"%u\t%u\t%u\t%u\t"		//Id 273
			"%u\t%u\t%u\t%u\t"		//Id 274
			"%u\t%u\t%u\n",		//Id 275

/*Id 101 */	time, VOLANTE, ACELERADOR, FREIO,
/*Id 102 */ MODO, HODOMETRO_PARCIAL, HODOMETRO_TOTAL,
/*Id 103 */ TORQUE_R, TORQUE_L, REF_TORQUE_R, REF_TORQUE_L,
/*Id 104 */	SPEED_R_MOTOR, SPEED_L_MOTOR, ECU_CONTROL_ID1, ECU_CONTROL_ID2,

/*Id 105 */	SPEED_FRONT_L_TIE, SPEED_FRONT_R_TIE,SPEED_BACK_R_TIE, SPEED_BACK_L_TIE,
/*Id 106 */	GANHO_TORQUE, SENTIDO_VOLANTE, CORRENTE_TORQUE_R, CORRENTE_TORQUE_L,
/*Id 109 */	SPEED_L,SPEED_R, POWER_L, POWER_R,

/*Id 291 */	ACCEL_X, ACCEL_Y, ACCEL_Z,
/*Id 292 */	GYRO_X, GYRO_Y, GYRO_Z,

/*Id 50 */	V_MAX, V_MIN,DELTA_V, TEMP_MAX,
/*Id 51 */	CONTATORES_STATUS, V_TS,
/*Id 54 */	/*CURRENT_SENSOR_1_BAIXA*/ CURRENT_SENSOR_1_ALTA, CURRENT_SENSOR_2_BAIXA, CURRENT_SENSOR_2_ALTA,
/*Id 55 */	CURRENT_SENSOR_3_BAIXA, CURRENT_SENSOR_3_ALTA, CURRENT_SENSOR_4_BAIXA, CURRENT_SENSOR_4_ALTA,

///*Id 260 */  V_CELL_PACK1_1, V_CELL_PACK1_2,V_CELL_PACK1_3,V_CELL_PACK1_4,
///*Id 261 */	V_CELL_PACK1_5, V_CELL_PACK1_6,V_CELL_PACK1_7,V_CELL_PACK1_8,
///*Id 262 */	V_CELL_PACK1_9, V_CELL_PACK1_10,V_CELL_PACK1_11,V_CELL_PACK1_12,
/*Id 263 */	TEMP_PACK1_1, TEMP_PACK1_2,TEMP_PACK1_3,TEMP_PACK1_4,
/*Id 264 */	TEMP_PACK1_5, V_TOT_PACK1, V_REF_PACK1,

///*Id 265 */  V_CELL_PACK2_1, V_CELL_PACK2_2,V_CELL_PACK2_3,V_CELL_PACK2_4,
///*Id 266 */	V_CELL_PACK2_5, V_CELL_PACK2_6,V_CELL_PACK2_7,V_CELL_PACK2_8,
///*Id 267 */	V_CELL_PACK2_9, V_CELL_PACK2_10,V_CELL_PACK2_11,V_CELL_PACK2_12,
/*Id 268 */	TEMP_PACK2_2, TEMP_PACK2_2,TEMP_PACK2_3,TEMP_PACK2_4,
/*Id 269 */	TEMP_PACK2_5, V_TOT_PACK2, V_REF_PACK2,

///*Id 270 */  V_CELL_PACK3_1, V_CELL_PACK3_2,V_CELL_PACK3_3,V_CELL_PACK3_4,
///*Id 271 */	V_CELL_PACK3_5, V_CELL_PACK3_6,V_CELL_PACK3_7,V_CELL_PACK3_8,
///*Id 272 */	V_CELL_PACK3_9, V_CELL_PACK3_10,V_CELL_PACK3_11,V_CELL_PACK3_12,
/*Id 273 */	TEMP_PACK3_1, TEMP_PACK3_2,TEMP_PACK3_3,TEMP_PACK3_4,
/*Id 274 */	TEMP_PACK3_5, V_TOT_PACK3, V_REF_PACK3,

///*Id 275 */  V_CELL_PACK4_1, V_CELL_PACK4_2,V_CELL_PACK4_3,V_CELL_PACK4_4,
///*Id 276 */	V_CELL_PACK4_5, V_CELL_PACK4_6,V_CELL_PACK4_7,V_CELL_PACK4_8,
///*Id 277 */	V_CELL_PACK4_9, V_CELL_PACK4_10,V_CELL_PACK4_11,V_CELL_PACK4_12,
/*Id 278 */	TEMP_PACK4_1, TEMP_PACK4_2,TEMP_PACK4_3,TEMP_PACK4_4,
/*Id 279 */	TEMP_PACK4_5, V_TOT_PACK4, V_REF_PACK4);

	fresult = f_open(&file, bufferFile, FA_OPEN_ALWAYS | FA_WRITE); //open file on SD card to write*/
	fresult = f_lseek(&file, file.obj.objsize);//goes to the end of the file
	fresult = f_write(&file, block, len, &bytes_written);//write data to the file
	fresult = f_close(&file);//closes the file

	if(fresult == FR_OK){
		_datalog_flag  = 1;
	}
	else _datalog_flag = 0;

	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_5);
}


//void Condicoes_Teste(void)
//{
//	if(PRESS_PTE != 0){
//		UINT bytes_written;
//		FRESULT fresult;
//		uint16_t condicoes[126];
//		int len;
//
//		len = snprintf((char*) condicoes, sizeof(condicoes),
//				//1   2  3   4   5
//				"%u\t%u\t%u\t%u\t"      //Id 180
//				"%u\t%u\t%u\t%u\t"	    //Id 181
//				"%u\t%u\t%u\t%u\t"      //Id 182
//				"%u\t%u\t%u\t"          //Id 183
//
//				"%u\t%u\t%u\t%u\t"      //Id 184
//				"%u\t%u\t%u\t%u\t"      //Id 185
//				"%u\t%u\t%u\t%u\t"      //Id 186
//				"%u\t%u\t%u\t%u\t"      //Id 187
//				"%u\t%u\t%u\t%u\n",     //Id 188
//
//
//	    /*Id 180 */	DIA, MES, ANO, HORA_1,
//		/*Id 181 */ MINUTO_1, HORA_2, MINUTO_2, TEMP,
//		/*Id 182 */ TEMP_PISTA, TORQUE_ECU, ENDURO, AUTOCROSS,
//		/*Id 183 */	SKIDPAD, ACELERACAO, BRAKE,
//
//		/*Id 184 */	VT, CA ,NENHUM, PNEU_CHUVA,
//		/*Id 185 */	PNEU_HOOSIER_NOVO, PNEU_HOOSIER_ANTIGO, PNEU_RBC, SECA,
//		/*Id 186 */ MOLHADA, UMIDA, VACORUJA, ASA,
//		/*Id 187 */	BICO, ENTRE_EIXOS, PESO_CARRO, PESO_PILOTO,
//		/*Id 188 */	PRESS_PDD, PRESS_PDE, PRESS_PTD, PRESS_PTE);
//
//		fresult = f_open(&file, bufferFile, FA_OPEN_ALWAYS | FA_WRITE); //open file on SD card to write*/
//		fresult = f_lseek(&file, 0); //goes to beginning of the file (n sei se precisa)
//		fresult = f_write(&file, condicoes, len, &bytes_written);//write data to the file
//		fresult = f_close(&file);//closes the file
//
//		if(fresult == FR_OK){
//			_datalog_flag  = 1;
//		}
//
//		PRESS_PTE = 0; //condição de saída
//	}
//
//}






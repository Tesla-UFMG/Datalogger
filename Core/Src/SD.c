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

uint64_t num_resets = 0;

extern IWDG_HandleTypeDef hiwdg1;

char block[2000];

FRESULT SD_Create_File(void)
{
	FRESULT fresult;
	uint8_t contFile = 0;	/*enumerate the file*/

	fresult = f_mount(&g_sFatFs, "0:", 0);	/*mount SD card*/
	do
	{
		sprintf(bufferFile, "ARQ%02d.csv", contFile);
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

		sprintf(bufferFile, "ARQ%02d.csv", aux);
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

	char cabecalho[2000];
	/*
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
	           "\tV_TOT_4\tV_REF_4\n");*/

	strcpy(cabecalho,   "sep = ,\ntime "

			"Velocidade_Media, Volante, Acelerador,"

			"Freio, Modo, Ganho_Torque, Hodometro_P, Hodometro_T,"

			"Flag_Erro_ECU, Flag_Status, Referencia_MD, Referencia_ME,"

			"Frenagem_Regen,"

			"Torq_MD, Torq_ME,"

			"Tensao_Max, Tensao_Min, Var_Tensao, Temp_Max,"

			"Modo_BMS, Flag_Erro_BMS, Contatores, Tensao_Trat,"

			"Corr_1_Alta, Corr_2_Alta,"

			"Ten_P1_C1, Ten_P1_C2, Ten_P1_C3, Ten_P1_C4,"
			" Ten_P1_C5, Ten_P1_C6, Ten_P1_C7, Ten_P1_C8,"
			" Ten_P1_C9, Ten_P1_C10, Ten_P1_C11, Ten_P1_C12,"
			" Temp_P1_T1, Temp_P1_T2, Temp_P1_T3, Temp_P1_T4,"
			" Temp_P1_T5, Ten_Total_P1,Ten_Ref_P1, Balanc_P1,"

			" Ten_P2_C1, Ten_P2_C2, Ten_P2_C3, Ten_P2_C4,"
			" Ten_P2_C5, Ten_P2_C6, Ten_P2_C7, Ten_P2_C8,"
			" Ten_P2_C9, Ten_P2_C10, Ten_P2_C11, Ten_P2_C12,"
			" Temp_P2_T1, Temp_P2_T2, Temp_P2_T3, Temp_P2_T4,"
			"Temp_P2_T5, Ten_Total_P2,Ten_Ref_P2, Balanc_P2,"

			"Ten_P3_C1, Ten_P3_C2, Ten_P3_C3, Ten_P3_C4,"
			" Ten_P3_C5, Ten_P3_C6, Ten_P3_C7, Ten_P3_C8,"
			" Ten_P3_C9, Ten_P3_C10, Ten_P3_C11, Ten_P3_C12,"
			" Temp_P3_T1, Temp_P3_T2, Temp_P3_T3, Temp_P3_T4,"
			"Temp_P3_T5, Ten_Total_P3,Ten_Ref_P3, Balanc_P3,"

			"Ten_P4_C1, Ten_P4_C2, Ten_P4_C3, Ten_P4_C4,"
			" Ten_P4_C5, Ten_P4_C6, Ten_P4_C7, Ten_P4_C8,"
			" Ten_P4_C9, Ten_P4_C10, Ten_P4_C11, Ten_P4_C12,"
			" Temp_P4_T1, Temp_P4_T2, Temp_P4_T3, Temp_P4_T4,"
			" Temp_P4_T5, Ten_Total_P4,Ten_Ref_P4, Balanc_P4\n");
	int tamanho = strlen(cabecalho);


	fresult = f_open(&file, bufferFile, FA_OPEN_ALWAYS | FA_WRITE); //open file on SD card to write
	fresult = f_write(&file, cabecalho, tamanho, &bytes_written); //write data to the file
	fresult = f_close(&file);
}

void SD_Placa_Inicializada(void)
{
	FRESULT fresult;
	//fresult = f_mount(&g_sFatFs, "0:", 0);	/*mount SD card*/
    FILINFO fno;
	fresult = f_mount(&g_sFatFs, "0:", 0);	/*mount SD card*/

	fresult = f_stat(bufferFile, &fno);

	if (fresult ==  FR_NO_FILE)
	{
		// Criar arquivo
		SD_Create_File();
		Cabecalho();
	}
	else
	{
		num_resets += 1;
		readSD();
	}


}

void writeSD(void)
{
	UINT bytes_written;
	FRESULT fresult;
	//uint16_t block[310];
	int len;
	uint32_t time = HAL_GetTick();

	len = snprintf((char*) block, sizeof(block),
			"%lu,"

			"%u,%u,%u,%u,"

			"%u,%u,%u,%u,"

			"%u,%u,%u,%u,"

			"%u,"

			"%u,%u,"

			"%u,%u,%u,%u,"

			"%u,%u,%u,%u,"

			"%u,%u,"

			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"

			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"

			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"

			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,"
			"%u,%u,%u,%u,\n",
			time,
			Velocidade_Media, Volante, Acelerador, Freio,
			Modo, Ganho_Torque, Hodometro_P, Hodometro_T,
			Flag_Erro_ECU, Flag_Status, Referencia_MD, Referencia_ME,

			Frenagem_Regen,

			Torq_MD, Torq_ME,

			Tensao_Max, Tensao_Min,Var_Tensao, Temp_Max,
			Modo_BMS, Flag_Erro_BMS, Contatores, Tensao_Trat,
			Corr_1_Alta, Corr_2_Alta,

			Ten_P1_C1, Ten_P1_C2, Ten_P1_C3, Ten_P1_C4,
			Ten_P1_C5, Ten_P1_C6, Ten_P1_C7, Ten_P1_C8,
			Ten_P1_C9, Ten_P1_C10, Ten_P1_C11, Ten_P1_C12,
			Temp_P1_T1, Temp_P1_T2, Temp_P1_T3, Temp_P1_T4,
			Temp_P1_T5, Ten_Total_P1,Ten_Ref_P1, Balanc_P1,
			Ten_P2_C1, Ten_P2_C2, Ten_P2_C3, Ten_P2_C4,
			Ten_P2_C5, Ten_P2_C6, Ten_P2_C7, Ten_P2_C8,
			Ten_P2_C9, Ten_P2_C10, Ten_P2_C11, Ten_P2_C12,
			Temp_P2_T1, Temp_P2_T2, Temp_P2_T3, Temp_P2_T4,
			Temp_P2_T5, Ten_Total_P2,Ten_Ref_P2, Balanc_P2,
			Ten_P3_C1, Ten_P3_C2, Ten_P3_C3, Ten_P3_C4,
			Ten_P3_C5, Ten_P3_C6, Ten_P3_C7, Ten_P3_C8,
			Ten_P3_C9, Ten_P3_C10, Ten_P3_C11, Ten_P3_C12,
			Temp_P3_T1, Temp_P3_T2, Temp_P3_T3, Temp_P3_T4,
			Temp_P3_T5, Ten_Total_P3,Ten_Ref_P3, Balanc_P3,
			Ten_P4_C1, Ten_P4_C2, Ten_P4_C3, Ten_P4_C4,
			Ten_P4_C5, Ten_P4_C6, Ten_P4_C7, Ten_P4_C8,
			Ten_P4_C9, Ten_P4_C10, Ten_P4_C11, Ten_P4_C12,
			Temp_P4_T1, Temp_P4_T2, Temp_P4_T3, Temp_P4_T4,
			Temp_P4_T5, Ten_Total_P4,Ten_Ref_P4, Balanc_P4);

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





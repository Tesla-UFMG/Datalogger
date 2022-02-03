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
int aux = 0;

static FATFS g_sFatFs;
FIL file;
char bufferFile[20];//buffer with the name of the file
int count = 0;
uint8_t _datalog_flag = 0;

uint8_t buffer_log[10][400];


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
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_5); //Blinks the led to indicate that there was no erros
		_datalog_flag  = 1;
	}
	else _datalog_flag = 0;

	return fresult;
}

void Cabecalho(void){
	UINT bytes_written;
	FRESULT fresult;

	char cabecalho[1301];
	strcpy(cabecalho, "Timer\tVol\tAccel\tFreio\tModo\tFrenagemReg\tHodometroP\tHodometroT\tTorqueRM\tTorqueLM\tTorqueRefR\tTorqueRefL\tVelRM\tVelLM\tEcuEventId\tEcuEventId\tVelFrontLTie\tVelFrontRTie\tVelBackRTie\tVelBackLTie\tTorqueGain\tTCurrentRM\tTCurrentLM\tTempInversorR1\tTempInversorR2\tTempInversorL1\tTempInversor2L\tAccelX\tAccelY\tAccelZ\tErro\tGyroX\tGyroY\tGyroZ\tErro\tISensor2\tISensor1High\tISensor1Low\tISensor3\tGlvVoltage\tCarga(%)\tAir\tTensaoTot\tTMaxBancoBat\tTensaoMin\tTensaoMax\tVCel400\tVCel401\tVCel402\tVCel403\tVCel404\tVCel405\tVCel406\tVCel407\tVCel408\tVCel409\tVCel410\tVCel411\tVCel412\tTemp400\tTemp401\tTemp302\tTemp403\tTemp404\tVCel500\tVCel501\tVCel502\tVCel53\tVCel504\tVCel505\tVCel506\tVCel507\tVCel508\tVCel509\tVCel510\tVCel511\tVCel512\tTemp500\tTemp501\tTemp502\tTemp503\tTemp504\tVCel300\tVCel301\tVCel302\tVCel303\tVCel304\tVCel305\tVCel306\tVCel307\tVCel308\tVCel309\tVCel310\tVCel311\tVCel312\tTemp300\tTemp301\tTemp302\tTemp303\tTemp304\tVCel200\tVCel201\tVCel202\tVCel203\tVCel204\tVCel205\tVCel206\tVCel207\tVCel208\tVCel209\tVCel210\tVCel211\tVCel212\tTemp200\tTemp201\tTemp202\tTemp203\tTemp204\tVCel100\tVCel101\tVCel102\tVCel103\tVCel104\tVCel105\tVCel106\tVCel107\tVCel108\tVCel109\tVCel110\tVCel111\tVCel112\tTemp100\tTemp101\tTemp102\tTemp103\tTemp104\tVCel000\tVCel001\tVCel002\tVCel003\tVCel004\tVCel005\tVCel006\tVCel007\tVCel008\tVCel009\tVCel010\tVCel011\tVCel012\tTemp000\tTemp001\tTemp002\tTemp003\tTemp004\n");
	int tamanho = strlen(cabecalho);

	fresult = f_open(&file, bufferFile, FA_OPEN_ALWAYS | FA_WRITE); //open file on SD card to write*/
	fresult = f_write(&file, cabecalho, tamanho, &bytes_written); //write data to the file
	fresult = f_close(&file);
}

void writeSD(void)
{
	UINT bytes_written;
	FRESULT fresult;
	//implementar dos dois jeitos, pra ficar fácil de testar e trocar nos testes caso algum falhe
	uint8_t block[512];
	int len;
	uint32_t time = HAL_GetTick();

	len = snprintf((char*) block, sizeof(block),
			//1   2   3   4   5   6   7   8
			"%lu\t%u\t%u\t%u\t%d\t%u\t%u\t%u\t"	//1//Timer\Volante\Acelerador\Freio\Modo\Frenagem_Reg\Hodometro_P\Hodometro_T
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//2//Torque_RM\Torque_LM\TRef_R\Tref_L\V_Motor_R\V_Motor_L\Control_Event_id\Control_Event_id
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//3//V_Front_L_Tie\V_Front_R_Tie\V_Back_R_Tie\V_Back_L_Tie\Torque_Gain\tCurrent_RM\tCurrent_LM\T_Inversor1_D
			"%u\t%u\t%u\t%d\t%d\t%d\t%u\t"	//4//T_Inversor2_D\T_Inversor1_E\T_Inversor2_E\AccelX\AccelY\AccelZ\Erro\GyroX
			"%d\t%d\t%u\t%u\t%d\t%d\t%d\t%d\t"	//5//GyroY\GyroZ\Erro\T_Disco_1\Extensometro_1\Extensometro_2\Sensor_Pressao_2\ADC_2
			"%u*\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//6//T_Disco_2\Extensometro_3\Extensometro_4\Extensometro_5\ADC_3\Tensao_GLV\Tensao_Total\Temp_Media_Banco
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//7//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//8//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//9//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//10//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%d\t%u\t%u\t%u\t%u\t%u\t"	//11//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//12//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//13//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//14//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//15//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//16//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//17//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//18//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//19//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\n",					//20//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11

	/*1	*/	time,can_vector[101].word_1, can_vector[101].word_2, can_vector[101].word_3, (int8_t)can_vector[102].word_0, can_vector[102].word_1,can_vector[102].word_2,can_vector[102].word_3,
	/*2	*/	can_vector[103].word_0,can_vector[103].word_1,can_vector[103].word_2,can_vector[103].word_3,can_vector[104].word_0,can_vector[104].word_1,can_vector[104].word_2,can_vector[104].word_3,
	/*3	*/	can_vector[105].word_0,can_vector[105].word_1,can_vector[105].word_2,can_vector[105].word_3,can_vector[106].word_0,can_vector[106].word_2,can_vector[106].word_3,can_vector[107].word_0,
	/*4	*/	can_vector[107].word_1,can_vector[107].word_2,can_vector[107].word_3,can_vector[108].word_0,can_vector[108].word_1,can_vector[108].word_2,can_vector[108].word_3,/*can_vector[109].word_0*/
	/*5	*/	can_vector[109].word_0,can_vector[109].word_1,can_vector[109].word_2,can_vector[109].word_3,can_vector[50].word_0,can_vector[50].word_1,can_vector[50].word_2,can_vector[50].word_3,
	/*6	*/	can_vector[51].word_0,can_vector[51].word_1,can_vector[51].word_3,can_vector[52].word_1,can_vector[52].word_3,can_vector[53].word_0,can_vector[53].word_1,can_vector[256].word_0,
	/*7	*/	/*can_vector[256].word_1*/can_vector[256].word_1,can_vector[256].word_2,can_vector[256].word_3,can_vector[257].word_0,can_vector[257].word_1,can_vector[257].word_2,can_vector[257].word_3,
	/*8	*/	can_vector[258].word_0,can_vector[258].word_1,can_vector[258].word_2,can_vector[258].word_3,can_vector[259].word_0,can_vector[259].word_1,can_vector[259].word_2,can_vector[259].word_3,
	/*9	*/	can_vector[260].word_0,can_vector[260].word_1,can_vector[261].word_0,can_vector[261].word_1,can_vector[261].word_2,can_vector[261].word_3,can_vector[262].word_0,can_vector[262].word_1,
	/*10*/	can_vector[262].word_2,can_vector[262].word_3,can_vector[263].word_0,can_vector[263].word_1,can_vector[263].word_2,can_vector[263].word_3,can_vector[264].word_0,can_vector[264].word_1,
	/*11*/	can_vector[264].word_2,can_vector[264].word_3,can_vector[265].word_0,can_vector[265].word_1,can_vector[266].word_0,can_vector[266].word_1,can_vector[266].word_2,can_vector[266].word_3,
	/*12*/	can_vector[267].word_0,can_vector[267].word_1,can_vector[267].word_2,can_vector[267].word_3,can_vector[268].word_0,can_vector[268].word_1,can_vector[268].word_2,can_vector[268].word_3,
	/*13*/	can_vector[269].word_0,can_vector[269].word_1,can_vector[269].word_2,can_vector[269].word_3,can_vector[270].word_0,can_vector[270].word_1,can_vector[271].word_0,can_vector[271].word_1,
	/*14*/	can_vector[271].word_2,can_vector[271].word_3,can_vector[272].word_0,can_vector[272].word_1,can_vector[272].word_2,can_vector[272].word_3,can_vector[273].word_0,can_vector[273].word_1,
	/*15*/	can_vector[273].word_2,can_vector[273].word_3,can_vector[274].word_0,can_vector[274].word_1,can_vector[274].word_2,can_vector[274].word_3,can_vector[275].word_0,can_vector[275].word_1,
	/*16*/	can_vector[276].word_0,can_vector[276].word_1,can_vector[276].word_2,can_vector[276].word_3,can_vector[277].word_0,can_vector[277].word_1,can_vector[277].word_2,can_vector[277].word_3,
	/*17*/	can_vector[278].word_0,can_vector[278].word_1,can_vector[278].word_2,can_vector[278].word_3,can_vector[279].word_0,can_vector[279].word_1,can_vector[279].word_2,can_vector[279].word_3,
	/*18*/	can_vector[280].word_0,can_vector[280].word_1,can_vector[281].word_0,can_vector[281].word_1,can_vector[281].word_2,can_vector[281].word_3,can_vector[282].word_0,can_vector[282].word_1,
	/*19*/	can_vector[282].word_2,can_vector[282].word_3,can_vector[283].word_0,can_vector[283].word_1,can_vector[283].word_2,can_vector[283].word_3,can_vector[284].word_0,can_vector[284].word_1,
	/*20*/	can_vector[284].word_2,can_vector[284].word_3,can_vector[285].word_0,can_vector[285].word_1);

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

void writeSDBuffer(void)
{
	UINT bytes_written;
	FRESULT fresult;
	int len;
	uint32_t time = HAL_GetTick();

//Arrumar os comentários dos IDs da CAN
	 len = snprintf((char*) buffer_log[aux], sizeof(buffer_log[aux]),
			//1   2   3   4   5   6   7   8
			"%lu\t%u\t%u\t%u\t%d\t%u\t%u\t%u\t"	//1//Timer\Volante\Acelerador\Freio\Modo\Frenagem_Reg\Hodometro_P\Hodometro_T
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//2//Torque_RM\Torque_LM\TRef_R\Tref_L\V_Motor_R\V_Motor_L\Control_Event_id\Control_Event_id
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//3//V_Front_L_Tie\V_Front_R_Tie\V_Back_R_Tie\V_Back_L_Tie\Torque_Gain\tCurrent_RM\tCurrent_LM\T_Inversor1_D
			"%u\t%u\t%u\t%d\t%d\t%d\t%u\t"	//4//T_Inversor2_D\T_Inversor1_E\T_Inversor2_E\AccelX\AccelY\AccelZ\Erro\GyroX
			"%d\t%d\t%u\t%u\t%d\t%i\t%d\t%d\t"	//5//GyroY\GyroZ\Erro\T_Disco_1\Extensometro_1\Extensometro_2\Sensor_Pressao_2\ADC_2
			"%u*\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//6//T_Disco_2\Extensometro_3\Extensometro_4\Extensometro_5\ADC_3\Tensao_GLV\Tensao_Total\Temp_Media_Banco
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//7//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//8//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//9//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//10//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%d\t%u\t%u\t%u\t%u\t%u\t"	//11//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//12//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//13//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//14//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//15//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//16//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//17//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//18//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//19//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
			"%u\t%u\t%u\t%u\n",					//20//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11

	/*1	*/	time,can_vector[101].word_1, can_vector[101].word_2, can_vector[101].word_3, (int8_t)can_vector[102].word_0, can_vector[102].word_1,can_vector[102].word_2,can_vector[102].word_3,
	/*2	*/	can_vector[103].word_0,can_vector[103].word_1,can_vector[103].word_2,can_vector[103].word_3,can_vector[104].word_0,can_vector[104].word_1,can_vector[104].word_2,can_vector[104].word_3,
	/*3	*/	can_vector[105].word_0,can_vector[105].word_1,can_vector[105].word_2,can_vector[105].word_3,can_vector[106].word_0,can_vector[106].word_2,can_vector[106].word_3,can_vector[107].word_0,
	/*4	*/	can_vector[107].word_1,can_vector[107].word_2,can_vector[107].word_3,can_vector[108].word_0,can_vector[108].word_1,can_vector[108].word_2,can_vector[108].word_3,/*can_vector[109].word_0*/
	/*5	*/	can_vector[109].word_0,can_vector[109].word_1,can_vector[109].word_2,can_vector[109].word_3,can_vector[50].word_0,can_vector[50].word_1,can_vector[50].word_2,can_vector[50].word_3,
	/*6	*/	can_vector[51].word_0,can_vector[51].word_1,can_vector[51].word_3,can_vector[52].word_1,can_vector[52].word_3,can_vector[53].word_0,can_vector[53].word_1,can_vector[256].word_0,
	/*7	*/	/*can_vector[256].word_1*/can_vector[256].word_1,can_vector[256].word_2,can_vector[256].word_3,can_vector[257].word_0,can_vector[257].word_1,can_vector[257].word_2,can_vector[257].word_3,
	/*8	*/	can_vector[258].word_0,can_vector[258].word_1,can_vector[258].word_2,can_vector[258].word_3,can_vector[259].word_0,can_vector[259].word_1,can_vector[259].word_2,can_vector[259].word_3,
	/*9	*/	can_vector[260].word_0,can_vector[260].word_1,can_vector[261].word_0,can_vector[261].word_1,can_vector[261].word_2,can_vector[261].word_3,can_vector[262].word_0,can_vector[262].word_1,
	/*10*/	can_vector[262].word_2,can_vector[262].word_3,can_vector[263].word_0,can_vector[263].word_1,can_vector[263].word_2,can_vector[263].word_3,can_vector[264].word_0,can_vector[264].word_1,
	/*11*/	can_vector[264].word_2,can_vector[264].word_3,can_vector[265].word_0,can_vector[265].word_1,can_vector[266].word_0,can_vector[266].word_1,can_vector[266].word_2,can_vector[266].word_3,
	/*12*/	can_vector[267].word_0,can_vector[267].word_1,can_vector[267].word_2,can_vector[267].word_3,can_vector[268].word_0,can_vector[268].word_1,can_vector[268].word_2,can_vector[268].word_3,
	/*13*/	can_vector[269].word_0,can_vector[269].word_1,can_vector[269].word_2,can_vector[269].word_3,can_vector[270].word_0,can_vector[270].word_1,can_vector[271].word_0,can_vector[271].word_1,
	/*14*/	can_vector[271].word_2,can_vector[271].word_3,can_vector[272].word_0,can_vector[272].word_1,can_vector[272].word_2,can_vector[272].word_3,can_vector[273].word_0,can_vector[273].word_1,
	/*15*/	can_vector[273].word_2,can_vector[273].word_3,can_vector[274].word_0,can_vector[274].word_1,can_vector[274].word_2,can_vector[274].word_3,can_vector[275].word_0,can_vector[275].word_1,
	/*16*/	can_vector[276].word_0,can_vector[276].word_1,can_vector[276].word_2,can_vector[276].word_3,can_vector[277].word_0,can_vector[277].word_1,can_vector[277].word_2,can_vector[277].word_3,
	/*17*/	can_vector[278].word_0,can_vector[278].word_1,can_vector[278].word_2,can_vector[278].word_3,can_vector[279].word_0,can_vector[279].word_1,can_vector[279].word_2,can_vector[279].word_3,
	/*18*/	can_vector[280].word_0,can_vector[280].word_1,can_vector[281].word_0,can_vector[281].word_1,can_vector[281].word_2,can_vector[281].word_3,can_vector[282].word_0,can_vector[282].word_1,
	/*19*/	can_vector[282].word_2,can_vector[282].word_3,can_vector[283].word_0,can_vector[283].word_1,can_vector[283].word_2,can_vector[283].word_3,can_vector[284].word_0,can_vector[284].word_1,
	/*20*/	can_vector[284].word_2,can_vector[284].word_3,can_vector[285].word_0,can_vector[285].word_1);

		if(aux == 9){
			fresult = f_open(&file, bufferFile, FA_OPEN_ALWAYS | FA_WRITE); //open file on SD card to write*/
			fresult = f_lseek(&file, file.obj.objsize);//goes to the end of the file
			for(int i = 0; i < 10; i++){
				fresult = f_write(&file, buffer_log[i], len, &bytes_written);     //write data to the file
			}
			fresult = f_close(&file);//closes the file
			//memset usar essa função pra zerar o buffer dps de escever, ver se melhora o tempo
			//ver se o /t influencia no tempo de escrita
			//alguns dados só
			//a frequência ta mto alta pq ele tá escrevendo dados pequenos ainda, 20, 30, imagina qnd for uma parada
			//de 16 bits, por exemplo
			//Pegar as flags (exemplo) q ocupam 1 bit, pra ocupar o espaço de 4 bytes (exemplo) de um dado que
			//tem 12 bytes(tipo ADC) e colocar as flags lá. Esse dado ficaria um número gigante no txt, tipo 98723912392
			//daí, na janela de análise, vc teria q filtrar esse dado pra mostrar os bits dessas dos 4 bytes da flag
			//isso dá de fazer fazendo shift qnd recebe o dado


			//TENTAR MUDAR O CLOCK DO SDMMC1 PARA 50MHz e 125MHz e ver oq muda********
		}
		if(fresult == FR_OK){
			_datalog_flag  = 1;
		}
		else _datalog_flag = 0;

		if(aux != 9)
			aux++;
		else
			aux = 0;

		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_5);
}



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
	//fresult = f_write(&file, arrayComOsNomes, sizeoff(array), &bytes_written);     //write data to the file
	fresult = f_close(&file);
	if(fresult == FR_OK){
		//HAL_GPIO_TogglePin(DATALOG_LED_GPIO_Port, DATALOG_LED_Pin); //Blinks the led to indicate that there was no erros
		_datalog_flag  = 1;
	}
	else _datalog_flag = 0;
	return fresult;
}

void cabecalho(void)
{
	UINT bytes_written;
	FRESULT fresult;
	char buffer_log[1400];
	strcpy(buffer_log, "Timer\tVol\tAccel\tFreio\tModo\tFrenagemReg\tHodometroP\tHodometroT\tTorqueRM\tTorqueLM\tTorqueRefR\tTorqueRefL\tVelRM\tVelLM\tEcuEventId\tEcuEventId\tVelFrontLTie\tVelFrontRTie\tVelBackRTie\tVelBackLTie\tTorqueGain\tTCurrentRM\tTCurrentLM\tT\tTempInversorR1\tTempInversorR2\tTempInversorL1\tTempInversor2L\tAccelX\tAccelY\tAccelZ\tErro\tGyroX\tGyroY\tGyroZ\tErro\tISensor2\tISensor1High\tISensor1Low\tISensor3\tGlvVoltage\tCarga(%)\tAir\tTensaoTot\tTMaxBancoBat\tTensaoMin\tTensaoMax\tVCell400\tVCell401\tVCell402\tVCell403\tVCell404\tVCell405\tVCell406\tVCell407\tVCell408\tVCell409\tVCell410\tVCell411\tVCell412\tTemp400\tTemp401\tTemp302\tTemp403\tTemp404\tVCell500\tVCell501\tVCell502\tVCell53\tVCell504\tVCell505\tVCell506\tVCell507\tVCell508\tVCell509\tVCell510\tVCell511\tVCell512\tTemp500\tTemp501\tTemp502\tTemp503\tTemp504\tVCell300\tVCell301\tVCell302\tVCell303\tVCell304\tVCell305\tVCell306\tVCell307\tVCell308\tVCell309\tVCell310\tVCell311\tVCell312\tTemp300\tTemp301\tTemp302\tTemp303\tTemp304\tVCell200\tVCell201\tVCell202\tVCell203\tVCell204\tVCell205\tVCell206\tVCell207\tVCell208\tVCell209\tVCell210\tVCell211\tVCell212\tTemp200\tTemp201\tTemp202\tTemp203\tTemp204\tVCell100\tVCell101\tVCell102\tVCell103\tVCell104\tVCell105\tVCell106\tVCell107\tVCell108\tVCell109\tVCell110\tVCell111\tVCell112\tTemp100\tTemp101\tTemp102\tTemp103\tTemp104\tVCell000\tVCell001\tVCell002\tVCell003\tVCell004\tVCell005\tVCell006\tVCell007\tVCell008\tVCell009\tVCell010\tVCell011\tVCell012\tTemp000\tTemp001\tTemp002\tTemp003\tTemp004");
	int len = strlen(buffer_log);
	fresult = f_open(&file, bufferFile, FA_OPEN_ALWAYS | FA_WRITE); //open file on SD card to write
	fresult = f_lseek(&file, file.obj.objsize);//goes to the end of the file
	fresult = f_write(&file, buffer_log, len+1, &bytes_written);//write data to the file
	fresult = f_close(&file);//closes the file
}

void writeSD(void)
{
	UINT bytes_written;
	FRESULT fresult;
	char buffer_log[256];
	uint8_t block[512];
	uint32_t time = HAL_GetTick();

//Arrumar os comentários dos IDs da CAN

		int len = snprintf((char*) block, sizeof(block),
				//1   2   3   4   5   6   7   8
				"%lu\t%u\t%u\t%u\t%d\t%u\t%u\t%u\t"	//1//Timer\Volante\Acelerador\Freio\Modo\Frenagem_Reg\Hodometro_P\Hodometro_T
				"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//2//Torque_RM\Torque_LM\TRef_R\Tref_L\V_Motor_R\V_Motor_L\Control_Event_id\Control_Event_id
				"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//3//V_Front_L_Tie\V_Front_R_Tie\V_Back_R_Tie\V_Back_L_Tie\Torque_Gain\tCurrent_RM\tCurrent_LM\T_Inversor1_D
				"%u\t%u\t%u\t%d\t%d\t%d\t%u\t%d\t"	//4//T_Inversor2_D\T_Inversor1_E\T_Inversor2_E\AccelX\AccelY\AccelZ\Erro\GyroX
				"%d\t%d\t%u\t%u\t%f\t%f\t%f\t%f\t"	//5//GyroY\GyroZ\Erro\T_Disco_1\Extensometro_1\Extensometro_2\Sensor_Pressao_2\ADC_2
				"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//6//T_Disco_2\Extensometro_3\Extensometro_4\Extensometro_5\ADC_3\Tensao_GLV\Tensao_Total\Temp_Media_Banco
				"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//7//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
				"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//8//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
				"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"	//9//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11
				"%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t",	//10//Temp_Max_Banco\I_Sensor_1_H\I_Sensor_2_L\I_Sensor_2\I_Sensor_3\Temp_Pack_01\Temp_Pack_02\Temp_Pack_11

		/*1	*/	time,can_vector[101].word_1, can_vector[101].word_2, can_vector[101].word_3, (int8_t)can_vector[102].word_0, can_vector[102].word_1,can_vector[102].word_2,can_vector[102].word_3,
		/*2	*/	can_vector[103].word_0,can_vector[103].word_1,can_vector[103].word_2,can_vector[103].word_3,can_vector[104].word_0,can_vector[104].word_1,can_vector[104].word_2,can_vector[104].word_3,
		/*3	*/	can_vector[105].word_0,can_vector[105].word_1,can_vector[105].word_2,can_vector[105].word_3,can_vector[106].word_0,can_vector[106].word_2,can_vector[106].word_3,can_vector[107].word_0,
		/*4	*/	can_vector[107].word_1,can_vector[107].word_2,can_vector[107].word_3,can_vector[108].word_0,can_vector[108].word_1,can_vector[108].word_2,can_vector[108].word_3,can_vector[109].word_0,
		/*5	*/	can_vector[109].word_0,can_vector[109].word_1,can_vector[109].word_2,can_vector[109].word_3,can_vector[50].word_0,can_vector[50].word_1,can_vector[50].word_2,can_vector[50].word_3,
		/*6	*/	can_vector[51].word_0,can_vector[51].word_1,can_vector[51].word_3,can_vector[52].word_1,can_vector[52].word_3,can_vector[53].word_0,can_vector[53].word_1,can_vector[256].word_0,
		/*7	*/	can_vector[256].word_1,can_vector[256].word_1,can_vector[256].word_2,can_vector[256].word_3,can_vector[257].word_0,can_vector[257].word_1,can_vector[257].word_2,can_vector[257].word_3,
		/*8	*/	can_vector[258].word_0,can_vector[258].word_1,can_vector[258].word_2,can_vector[258].word_3,can_vector[259].word_0,can_vector[259].word_1,can_vector[259].word_2,can_vector[259].word_3,
		/*9	*/	can_vector[260].word_0,can_vector[260].word_1,can_vector[261].word_0,can_vector[261].word_1,can_vector[261].word_2,can_vector[261].word_3,can_vector[262].word_0,can_vector[262].word_1,
		/*10*/	can_vector[262].word_2,can_vector[262].word_3,can_vector[263].word_0,can_vector[263].word_1,can_vector[263].word_2,can_vector[263].word_3,can_vector[264].word_0,can_vector[264].word_1);

		fresult = f_open(&file, bufferFile, FA_OPEN_ALWAYS | FA_WRITE); //open file on SD card to write
		fresult = f_lseek(&file, file.obj.objsize);//goes to the end of the file
		fresult = f_write(&file, block/*buffer_log*/, len, &bytes_written);     //write data to the file

		len = snprintf((char*) block, sizeof(block),
				//1   2   3   4   5   6   7   8
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

		fresult = f_write(&file, block/*buffer_log*/, len, &bytes_written);     //write data to the file
		fresult = f_close(&file);//closes the file
		if(fresult == FR_OK){
			_datalog_flag  = 1;
		}
		else _datalog_flag = 0;

}



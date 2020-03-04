//Includes:
#include "can_log.h"
#include "fatfs_sd.h"
#include "master_da_prototipo.h"
#include "strings.h"
//Variables;

	uint8_t ECU_Mode = -1;
uint16_t sensorpressao1, Pedal;
uint16_t leitura_PotInt;
uint16_t sensorpressao2;
uint16_t leitura_PotInt2, PotTD;
uint16_t TensaoGLV;
int16_t current_sensor1_baixa, current_sensor1_alta, current_sensor2, current_sensor3;
uint16_t tempmediabb, tempmaxbb, tensaototal, temp_pack0_1, temp_pack0_2, temp_pack1_1, temp_pack1_2, temp_pack2_1, temp_pack2_2, temp_pack3_1, temp_pack3_2, temp_pack4_1, temp_pack4_2, temp_pack5_1, temp_pack5_2;
int16_t IRCan[16];
uint16_t tempInt;
uint16_t tempInt2;
uint16_t ECU_Timer = 0;
uint16_t Speed_LR = 0, V_motor_D = 0, V_motor_E = 0, Intensidade_Frenagem = 0, tempInv_D1 = 0, tempInv_D2, tempInv_E1, tempInv_E2;
uint16_t Speed_RR = 0;
int16_t Torque_LM = 0;
int16_t Torque_RM = 0;
uint16_t Torque_ref_R = 0, Torque_ref_L = 0;
int16_t Current_LM = 0;
int16_t Current_RM = 0;
int16_t Current_BAT = 0;
uint8_t GANHO_TORQUE = 0;
uint16_t Volt_BAT = 0;
uint16_t Tensao_GLV = 0;
uint16_t Volante, Hodometro_P, Hodometro_T;
extern long int timer;
uint8_t _datalog_flag = 0;
uint8_t _ecu_flag;
uint32_t ext1;
uint32_t ext2;
uint32_t ext22;
uint32_t ext13, ext23;
static FATFS g_sFatFs;
FIL file;
char bufferFile[20];//buffer with the name of the file
extern char buffer_log[512];
extern char buffer_aux[40];


 //DATALOG VARIABLES

/*This function creates and open the file*/
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
	return fresult;
}


void canMessageReceived(uint16_t id, uint8_t* data)
{

	uint16_t* data_word = (uint16_t*) data;
	switch(id)
	{
		case 1:
			ECU_Mode = data_word[0];
			_ecu_flag = data_word[1];
			sprintf(buffer_aux, "%d\t%d\t", data_word[0],data_word[1]);
			balde_caixa();
			break;
		case 51:
			current_sensor1_baixa = data_word[0];
			current_sensor1_alta = data_word[1];
			current_sensor2 = data_word[2];
			current_sensor3 = data_word[3];
			sprintf(buffer_aux, "%d\t%d\t%d\t%d\t", data_word[0],data_word[1],data_word[2],data_word[3]);
			balde_caixa();
			break;
		case 52:
			TensaoGLV =  data_word[0];
			sprintf(buffer_aux, "%d\t", data_word[0]);
			balde_caixa();
			break;
		case 53:

			tensaototal = data_word[1];
			tempmediabb = data_word[2];
			tempmaxbb = data_word[3];
			sprintf(buffer_aux, "%d\t%d\t%d\t", data_word[3],data_word[1],data_word[2]);
			balde_caixa();
			break;
		case 101:
			ECU_Timer = 	data_word[0];
			Pedal =			data_word[2];
			Volante = 		data_word[1];
			sprintf(buffer_aux, "%d\t%d\t%d\t", data_word[0],data_word[1],data_word[2]);
			balde_caixa();
			break;
		case 102:
			Hodometro_P = data_word[2];
			Hodometro_T = data_word[3];
			sprintf(buffer_aux, "%d\t%d\t",data_word[2],data_word[3]);
			balde_caixa();
			break;
		case 103:
			Torque_ref_R =  data_word[2];
			Torque_ref_L =  data_word[2];
			Torque_RM = 	data_word[0];
			Torque_LM = 	data_word[1];
			sprintf(buffer_aux, "%d\t%d\t%d\t%d\t", data_word[0],data_word[1],data_word[2],data_word[2]);
			balde_caixa();
			break;
		case 104:
			V_motor_D = data_word[0];
			V_motor_E = data_word[1];
			Intensidade_Frenagem = data_word[2];
			sprintf(buffer_aux, "%d\t%d\t%d\t", data_word[0],data_word[1],data_word[2]);
			balde_caixa();
			break;
		case 105:
			Speed_LR = 		data_word[0];
			Speed_RR = 		data_word[1];
			sprintf(buffer_aux, "%d\t%d\t", data_word[0],data_word[1]);
			balde_caixa();
			break;
		case 106:
			GANHO_TORQUE = 	data_word[0];
			Current_RM = 	data_word[2];
			Current_LM = 	data_word[3];
			sprintf(buffer_aux, "%d\t%d\t", data_word[0],data_word[2]);
			balde_caixa();
			break;
		case 107:
			tempInv_D1 = data_word[0];
			//tempInv_D2 = data_word[1];
			tempInv_E1 = data_word[2];
			//tempInv_E2 = data_word[3];
			sprintf(buffer_aux, "%d\t%d\t", data_word[0],data_word[2]);
			balde_caixa();
			break;
		case 156:
			IRCan[0] = data_word[0];
			IRCan[1] = data_word[1];
			IRCan[2] = data_word[2];
			IRCan[3] = data_word[3];
			sprintf(buffer_aux, "%d\t%d\t%d\t%d\t", data_word[0],data_word[1],data_word[2],data_word[3]);
			balde_caixa();
			break;
		case 170:
			sensorpressao1=data_word[0];
			leitura_PotInt =  data_word[1];
			tempInt =         data_word[2];
			sprintf(buffer_aux, "%d\t%d\t%d\t", data_word[0],data_word[1],data_word[2]);
			balde_caixa();
			break;

		case 171:
			ext1 = *(uint32_t*)(&(data_word[0]));
			ext2 = *(uint32_t*)(&(data_word[2]));
			sprintf(buffer_aux, "%d\t%d\t", data_word[0],data_word[2]);
			balde_caixa();
			break;
		case 172:
			sensorpressao2 = data_word[0];
			leitura_PotInt2 =  data_word[1];
			tempInt2 =         data_word[2];
			sprintf(buffer_aux, "%d\t%d\t%d\t", data_word[0],data_word[1],data_word[2]);
			balde_caixa();
			break;
		case 173:
			ext22 = *(uint32_t*)(&(data_word[0]));
			sprintf(buffer_aux, "%d\t", data_word[0]);
			balde_caixa();
			break;
		case 178:
			ext13 = *(uint32_t*)(&(data_word[0]));
			ext23 = *(uint32_t*)(&(data_word[2]));
			sprintf(buffer_aux, "%d\t%d\t", data_word[0],data_word[2]);
			balde_caixa();
			break;
		case 179:
			PotTD = data_word[0];
			sprintf(buffer_aux, "%d\t", data_word[0]);
			balde_caixa();
			break;
		case 263:
			temp_pack0_1 = data_word[1];
			temp_pack0_2 = data_word[2];
			sprintf(buffer_aux, "%d\t%d\t",data_word[1],data_word[2]);
			balde_caixa();
			break;
		case 268:
			temp_pack1_1 = data_word[1];
			temp_pack1_2 = data_word[2];
			sprintf(buffer_aux, "%d\t%d\t", data_word[1],data_word[2]);
			balde_caixa();
			break;
		case 273:
			temp_pack2_1 = data_word[1];
			temp_pack2_2 = data_word[2];
			sprintf(buffer_aux, "%d\t%d\t", data_word[1],data_word[2]);
						balde_caixa();
			break;
		case 278:
			temp_pack3_1 = data_word[1];
			temp_pack3_2 = data_word[2];
			sprintf(buffer_aux, "%d\t%d\t", data_word[1],data_word[2]);
						balde_caixa();
			break;
		case 283:
			temp_pack4_1 = data_word[1];
			temp_pack4_2 = data_word[2];
			sprintf(buffer_aux, "%d\t%d\t", data_word[1],data_word[2]);
						balde_caixa();
			break;
		case 288:
			temp_pack5_1 = data_word[1];
			temp_pack5_2 = data_word[2];
			sprintf(buffer_aux, "%d\t%d\t", data_word[1],data_word[2]);
						balde_caixa();
			break;

	}

}

void balde_caixa(void)
{
	strcat(buffer_log, buffer_aux);
	if(strlen(buffer_log)>= 512)
	{
		writeSD();
		buffer_log[0]= '\0';
	}

}

void writeSD(void)
{
	UINT bytes_written;
	FRESULT fresult;
	timer = HAL_GetTick();
	UINT len = strlen(buffer_log);
	fresult = f_open(&file, bufferFile, FA_OPEN_ALWAYS | FA_WRITE); //open file on SD card to write
	fresult = f_lseek(&file, file.fsize);//goes to the end of the file
	fresult = f_write(&file, buffer_log, len, &bytes_written);     //write data to the file
	fresult = f_close(&file);//closes the file
    if(fresult == FR_OK){
    	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin); //Blinks the led to indicate that there was no erros
    	_datalog_flag  = 1;
    }
    else _datalog_flag = 0;
}

void cabecalho(void)
{
	UINT bytes_written;
	FRESULT fresult;
	char buffer_log[256];
	strcpy("ECU_Timer\tSpeed_LR\tSpeed_RR\tTorque_LM\tTorque_RM\tref_R\tCurrent_LM\tCurrent_RM\tCurrent_BAT\tVolt_BAT\taccelX\taccelY\taccelZ\tgyroX\tgyroY\tgyroZ\ttemp\tlei_Bet\tlei_Pot\ttermopar\text1\text2\tTensaoGLV\tIRcan0\tIRcan1\tIRcan2\tIRcan3\tIRcan4\tIRcan5\tIRcan6\tIRcan7\tIRcan8\tIRcan9\tIRcan10\tIRcan11\tIRcan12\tIRcan13\tIRcan14\tIRcan15",buffer_log);
	fresult = f_open(&file, bufferFile, FA_OPEN_ALWAYS | FA_WRITE); //open file on SD card to write
	fresult = f_lseek(&file, file.fsize);//goes to the end of the file
	fresult = f_write(&file, buffer_log, 256, &bytes_written);//write data to the file
	fresult = f_close(&file);//closes the file
	    if(fresult == FR_OK){
	    	for(int i = 0; i<4; i++){
	    		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin); //Blinks the led to indicate that there was no erros
	    		HAL_Delay(20);
	    	}

	    }
}


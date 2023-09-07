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
#include "general_can.h"


#define CAN_IDS_NUMBER 400

FRESULT SD_Create_File();
void writeSD(void);
void readSD(void);

void Cabecalho(void);
void Condicoes_Teste(void);

/*CONTROLE*/

//Defines Novos

#define Velocidade_Media can_vector[76].word_0
#define Volante can_vector[76].word_1
#define Acelerador can_vector[76].word_2
#define Freio can_vector[76].word_3

#define Modo can_vector[77].word_0
#define Ganho_Torque can_vector[77].word_1
#define Hodometro_P can_vector[77].word_2
#define Hodometro_T can_vector[77].word_3

#define Flag_Erro_ECU can_vector[78].word_0
#define Flag_Status can_vector[78].word_1
#define Referencia_MD can_vector[78].word_2
#define Referencia_ME can_vector[78].word_3

#define Velocidade_DE can_vector[79].word_0
#define Velocidade_DD can_vector[79].word_1
#define Velocidade_TE can_vector[79].word_2
#define Velocidade_TD can_vector[79].word_3

#define Red_Toque_RE can_vector[80].word_0
#define Red_Torque_RD can_vector[80].word_1
#define Controle_D1 can_vector[80].word_2
#define Controle_D2 can_vector[80].word_3

#define Frenagem_Regen can_vector[81].word_0
//#define
#define IMU_BSE can_vector[81].word_2
#define IMU_Vel can_vector[81].word_3

#define Vel_ME can_vector[85].word_0
#define Torq_ME can_vector[85].word_1
#define Pot_ME can_vector[85].word_2
#define Corre_ME can_vector[85].word_3

#define Energia_ME can_vector[86].word_0
#define SobreCarga_ME can_vector[86].word_1
#define Temp1_ME can_vector[86].word_2
#define Temp2_ME can_vector[86].word_3

#define MsgPerdida_ME can_vector[87].word_0
#define Busoff_ME can_vector[87].word_1
#define Est_CAN_ME can_vector[87].word_2
//#define

#define Est_Inv_ME can_vector[88].word_0
#define Falha_ME can_vector[88].word_1
#define Alarme_ME can_vector[88].word_2
//#define

#define Vel_MD can_vector[95].word_0
#define Torq_MD can_vector[95].word_1
#define Pot_MD can_vector[95].word_2
#define Corre_MD can_vector[95].word_3

#define Energia_MD can_vector[96].word_0
#define SobreCarga_MD can_vector[96].word_1
#define Temp1_MD can_vector[96].word_2
#define Temp2_MD can_vector[96].word_3

#define MsgPerdida_MD can_vector[97].word_0
#define Busoff_MD can_vector[97].word_1
#define Est_CAN_MD can_vector[97].word_2
//#define

#define Est_Inv_MD can_vector[98].word_0
#define Falha_MD can_vector[98].word_1
#define Alarme_MD can_vector[98].word_2
//#define

#define Acel_X can_vector[291].word_0
#define Acel_Y can_vector[291].word_1
#define Acel_Z can_vector[291].word_2
#define IMU_Temp can_vector[291].word_3

#define Giros_X can_vector[292].word_0
#define Giros_Y can_vector[292].word_1
#define Giros_Z can_vector[292].word_2
#define Erro_IMU can_vector[292].word_3

#define Beacon can_vector[151].word_0
//#define
//#define
//#define

#define Pressao_D can_vector[152].word_0
#define Pressao_T can_vector[152].word_1
#define Brake_Bias can_vector[152].word_2
#define Hodo_P_Aq can_vector[152].word_3

#define Data_Status can_vector[153].word_0
//#define
//#define
//#define

#define Tensao_Max can_vector[226].word_0
#define Tensao_Min can_vector[226].word_1
#define Var_Tensao can_vector[226].word_2
#define Temp_Max can_vector[226].word_3

#define Modo_BMS can_vector[227].word_0
#define Flag_Erro_BMS can_vector[227].word_1
#define Contatores can_vector[227].word_2
#define Tensao_Trat can_vector[227].word_3

//#define
#define Corr_1_Alta can_vector[230].word_1
//#define
#define Corr_2_Alta can_vector[230].word_3

#define Carga_Atual can_vector[231].word_0
#define Soc_Inicial can_vector[231].word_1
#define Carga_Int can_vector[231].word_2
#define Temp_Med can_vector[231].word_3

#define Val_SoC can_vector[232].word_1
//#define
//#define
//#define

#define Ten_P1_C1 can_vector[301].word_0
#define Ten_P1_C2 can_vector[301].word_1
#define Ten_P1_C3 can_vector[301].word_2
#define Ten_P1_C4 can_vector[301].word_3

#define Ten_P1_C5 can_vector[302].word_0
#define Ten_P1_C6 can_vector[302].word_1
#define Ten_P1_C7 can_vector[302].word_2
#define Ten_P1_C8 can_vector[302].word_3

#define Ten_P1_C9 can_vector[303].word_0
#define Ten_P1_C10 can_vector[303].word_1
#define Ten_P1_C11 can_vector[303].word_2
#define Ten_P1_C12 can_vector[303].word_3

#define Temp_P1_T1 can_vector[304].word_0
#define Temp_P1_T2 can_vector[304].word_1
#define Temp_P1_T3 can_vector[304].word_2
#define Temp_P1_T4 can_vector[304].word_3

#define Temp_P1_T5 can_vector[305].word_0
#define Ten_Total_P1 can_vector[305].word_1
#define Ten_Ref_P1 can_vector[305].word_2
#define Balanc_P1 can_vector[305].word_3

//----------------------------------//

#define Ten_P2_C1 can_vector[306].word_0
#define Ten_P2_C2 can_vector[306].word_1
#define Ten_P2_C3 can_vector[306].word_2
#define Ten_P2_C4 can_vector[306].word_3

#define Ten_P2_C5 can_vector[307].word_0
#define Ten_P2_C6 can_vector[307].word_1
#define Ten_P2_C7 can_vector[307].word_2
#define Ten_P2_C8 can_vector[307].word_3

#define Ten_P2_C9 can_vector[308].word_0
#define Ten_P2_C10 can_vector[308].word_1
#define Ten_P2_C11 can_vector[308].word_2
#define Ten_P2_C12 can_vector[308].word_3

#define Temp_P2_T1 can_vector[309].word_0
#define Temp_P2_T2 can_vector[309].word_1
#define Temp_P2_T3 can_vector[309].word_2
#define Temp_P2_T4 can_vector[309].word_3

#define Temp_P2_T5 can_vector[310].word_0
#define Ten_Total_P2 can_vector[310].word_1
#define Ten_Ref_P2 can_vector[310].word_2
#define Balanc_P2 can_vector[310].word_3

//----------------------------------//


#define Ten_P3_C1 can_vector[311].word_0
#define Ten_P3_C2 can_vector[311].word_1
#define Ten_P3_C3 can_vector[311].word_2
#define Ten_P3_C4 can_vector[311].word_3

#define Ten_P3_C5 can_vector[312].word_0
#define Ten_P3_C6 can_vector[312].word_1
#define Ten_P3_C7 can_vector[312].word_2
#define Ten_P3_C8 can_vector[312].word_3

#define Ten_P3_C9 can_vector[313].word_0
#define Ten_P3_C10 can_vector[313].word_1
#define Ten_P3_C11 can_vector[313].word_2
#define Ten_P3_C12 can_vector[313].word_3

#define Temp_P3_T1 can_vector[314].word_0
#define Temp_P3_T2 can_vector[314].word_1
#define Temp_P3_T3 can_vector[314].word_2
#define Temp_P3_T4 can_vector[314].word_3

#define Temp_P3_T5 can_vector[315].word_0
#define Ten_Total_P3 can_vector[315].word_1
#define Ten_Ref_P3 can_vector[315].word_2
#define Balanc_P3 can_vector[315].word_3

//----------------------------------//

#define Ten_P4_C1 can_vector[316].word_0
#define Ten_P4_C2 can_vector[316].word_1
#define Ten_P4_C3 can_vector[316].word_2
#define Ten_P4_C4 can_vector[316].word_3

#define Ten_P4_C5 can_vector[317].word_0
#define Ten_P4_C6 can_vector[317].word_1
#define Ten_P4_C7 can_vector[317].word_2
#define Ten_P4_C8 can_vector[317].word_3

#define Ten_P4_C9 can_vector[318].word_0
#define Ten_P4_C10 can_vector[318].word_1
#define Ten_P4_C11 can_vector[318].word_2
#define Ten_P4_C12 can_vector[318].word_3

#define Temp_P4_T1 can_vector[319].word_0
#define Temp_P4_T2 can_vector[319].word_1
#define Temp_P4_T3 can_vector[319].word_2
#define Temp_P4_T4 can_vector[319].word_3

#define Temp_P4_T5 can_vector[320].word_0
#define Ten_Total_P4 can_vector[320].word_1
#define Ten_Ref_P4 can_vector[320].word_2
#define Balanc_P4 can_vector[320].word_3


/* Defines Antigos
#define VOLANTE can_vector[101].word_1
#define ACELERADOR can_vector[101].word_2
#define FREIO can_vector[101].word_3

#define MODO can_vector[102].word_0
#define FRENAGEM_REG can_vector[102].word_1
#define HODOMETRO_PARCIAL can_vector[102].word_2
#define HODOMETRO_TOTAL can_vector[102].word_3

#define TORQUE_R can_vector[103].word_0
#define TORQUE_L can_vector[103].word_1
#define REF_TORQUE_R can_vector[103].word_2
#define REF_TORQUE_L can_vector[103].word_3

#define SPEED_R_MOTOR can_vector[104].word_0
#define SPEED_L_MOTOR can_vector[104].word_1
#define ECU_CONTROL_ID1 can_vector[104].word_2
#define ECU_CONTROL_ID2 can_vector[104].word_3

#define SPEED_FRONT_L_TIE can_vector[105].word_0
#define SPEED_FRONT_R_TIE can_vector[105].word_1
#define SPEED_BACK_R_TIE can_vector[105].word_2
#define SPEED_BACK_L_TIE can_vector[105].word_3

#define GANHO_TORQUE can_vector[106].word_0
#define SENTIDO_VOLANTE can_vector[106].word_1
#define CORRENTE_TORQUE_R can_vector[106].word_2
#define CORRENTE_TORQUE_L can_vector[106].word_3

#define TEMP_INV_1_R can_vector[107].word_0
#define TEMP_INV_2_R can_vector[107].word_1
#define TEMP_INV_1_L can_vector[107].word_2
#define TEMP_INV_2_L can_vector[107].word_3

#define SPEED_L can_vector[109].word_0
#define SPEED_R can_vector[109].word_1
#define POWER_L can_vector[109].word_2
#define POWER_R can_vector[109].word_3

#define ENERGY_L can_vector[110].word_0
#define ENERGY_R can_vector[110].word_1
#define OVERLOAD_L can_vector[110].word_2
#define OVERLOAD_R can_vector[110].word_3

#define LOST_MSG_L can_vector[111].word_0
#define LOST_MSG_R can_vector[111].word_1
#define BUS_OFF_L can_vector[111].word_2
#define BUS_OFF_R can_vector[111].word_3

#define CAN_STATE_L can_vector[112].word_0
#define CAN_STATE_R can_vector[112].word_1
#define INV_STATE_L can_vector[112].word_2
#define INV_STATE_R can_vector[112].word_3

#define FAILURE_L can_vector[113].word_0
#define FALILURE_R can_vector[113].word_1
#define ALARM_L can_vector[113].word_2
#define ALARM_R can_vector[113].word_3

#define ACCEL_X can_vector[291].word_0
#define ACCEL_Y can_vector[291].word_1
#define ACCEL_Z can_vector[291].word_2
#define ERRO can_vector[291].word_3

#define GYRO_X can_vector[292].word_0
#define GYRO_Y can_vector[292].word_1
#define GYRO_Z can_vector[292].word_2
#define ERROR can_vector[292].word_3

/*Não definido
#define REDUC_TORQUE_L_TIE can_vector[105].word_0
#define REDUC_TORQUE_R_TIE can_vector[105].word_1
#define  can_vector[105].word_2
#define  can_vector[105].word_3
*/

//INVERSOR ESQUERDO
//#define SPEED_L can_vector[110].word_0
//#define TORQUE_L can_vector[110].word_1
//#define POTENCIA_L can_vector[110].word_2
//#define CORR_TORQUE_L can_vector[110].word_3
//
//#define CONSUMED_ENERGY_L can_vector[111].word_0
//#define OVERLOAD_L can_vector[111].word_1
//#define TEMP1_MOSF_L can_vector[111].word_2
//#define TEMP2_MOSF_L can_vector[111].word_3
//
//#define LOST_MSG_L can_vector[112].word_0
//#define CONT_BUSOFF_L can_vector[112].word_1
//#define ESTADO_CAN_L can_vector[112].word_2
//
//
//#define STATE_INV_L can_vector[113].word_0
//#define FALHA_ATUAL_L can_vector[113].word_1
//#define ALARME_ATUAL_L can_vector[113].word_2
//INVERSOR ESQUERDO

//INVERSOR DIREITO
//#define SPEED_R can_vector[120].word_0
//#define TORQUE_R can_vector[120].word_1
//#define POTENCIA_R can_vector[120].word_2
//#define CORR_TORQUE_R can_vector[120].word_3
//
//#define CONSUMED_ENERGY_R can_vector[121].word_0
//#define OVERLOAD_R can_vector[121].word_1
//#define TEMP1_MOSF_R can_vector[121].word_2
//#define TEMP2_MOSF_R can_vector[121].word_3
//
//#define LOST_MSG_R can_vector[122].word_0
//#define CONT_BUSOFF_R can_vector[122].word_1
//#define ESTADO_CAN_R can_vector[122].word_2
//
//
//#define STATE_INV_R can_vector[123].word_0
//#define FALHA_ATUAL_R can_vector[123].word_1
//#define ALARME_ATUAL_R can_vector[123].word_2
//INVERSOR DIREITO

//#define ACEL_X can_vector[130].word_0
//#define ACEL_Y can_vector[130].word_1
//#define ACEL_Z can_vector[130].word_2
//#define ERROR_IMU can_vector[130].word_3
//
//#define GYRO_X can_vector[131].word_0
//#define GYRO_Y can_vector[131].word_1
//#define GYRO_Z can_vector[131].word_2
//#define ERRO_IMU can_vector[131].word_3

/*CONTROLE*/


/*AQUISIÇÃO*/

//TESTE
//#define DIA can_vector[180].word_0
//#define MES can_vector[180].word_1
//#define ANO can_vector[180].word_2
//#define HORA_1 can_vector[180].word_3
//
//#define MINUTO_1 can_vector[181].word_0
//#define HORA_2 can_vector[181].word_1
//#define MINUTO_2 can_vector[181].word_2
//#define TEMP can_vector[181].word_3
//
//#define TEMP_PISTA can_vector[182].word_0
//#define TORQUE_ECU can_vector[182].word_1
//#define ENDURO can_vector[182].word_2
//#define AUTOCROSS can_vector[182].word_3
//
//#define SKIDPAD can_vector[183].word_0
//#define ACELERACAO can_vector[183].word_1
//#define BRAKE can_vector[183].word_2
//
//#define VT can_vector[184].word_0
//#define CA can_vector[184].word_1
//#define NENHUM can_vector[184].word_2
//#define PNEU_CHUVA can_vector[184].word_3
//
//#define PNEU_HOOSIER_NOVO can_vector[185].word_0
//#define PNEU_HOOSIER_ANTIGO can_vector[185].word_1
//#define PNEU_RBC can_vector[185].word_2
//#define SECA can_vector[185].word_3
//
//#define MOLHADA can_vector[186].word_0
//#define UMIDA can_vector[186].word_1
//#define VACORUJA can_vector[186].word_2
//#define ASA can_vector[186].word_3
//
//#define BICO can_vector[187].word_0
//#define ENTRE_EIXOS can_vector[187].word_1
//#define PESO_CARRO can_vector[187].word_2
//#define PESO_PILOTO can_vector[187].word_3
//
//#define PRESS_PDD can_vector[188].word_0
//#define PRESS_PDE can_vector[188].word_1
//#define PRESS_PTD can_vector[188].word_2
//#define PRESS_PTE can_vector[188].word_3

//TESTE

/*AQUISIÇÃO*/


/*BATERIAS*/


#define V_MAX can_vector[50].word_0
#define V_MIN can_vector[50].word_1
#define DELTA_V can_vector[50].word_2
#define TEMP_MAX can_vector[50].word_3

#define MODO_BMS can_vector[51].word_0
#define BMS_ERROR_FLAG can_vector[51].word_1
#define CONTATORES_STATUS can_vector[51].word_2
#define V_TS can_vector[51].word_3

//#define CURRENT_SENSOR_1_BAIXA can_vector[54].word_0
#define CURRENT_SENSOR_1_ALTA can_vector[54].word_1
#define CURRENT_SENSOR_2_BAIXA can_vector[54].word_2
#define CURRENT_SENSOR_2_ALTA can_vector[54].word_3

#define CURRENT_SENSOR_3_BAIXA can_vector[55].word_0
#define CURRENT_SENSOR_3_ALTA can_vector[55].word_1
#define CURRENT_SENSOR_4_BAIXA can_vector[55].word_2
#define CURRENT_SENSOR_4_ALTA can_vector[55].word_3

//PACK1
#define V_CELL_PACK1_1 can_vector[260].word_0
#define V_CELL_PACK1_2 can_vector[260].word_1
#define V_CELL_PACK1_3 can_vector[260].word_2
#define V_CELL_PACK1_4 can_vector[260].word_3

#define V_CELL_PACK1_5 can_vector[261].word_0
#define V_CELL_PACK1_6 can_vector[261].word_1
#define V_CELL_PACK1_7 can_vector[261].word_2
#define V_CELL_PACK1_8 can_vector[261].word_3

#define V_CELL_PACK1_9 can_vector[262].word_0
#define V_CELL_PACK1_10 can_vector[262].word_1
#define V_CELL_PACK1_11 can_vector[262].word_2
#define V_CELL_PACK1_12 can_vector[262].word_3

#define TEMP_PACK1_1 can_vector[263].word_0
#define TEMP_PACK1_2 can_vector[263].word_1
#define TEMP_PACK1_3 can_vector[263].word_2
#define TEMP_PACK1_4 can_vector[263].word_3

#define TEMP_PACK1_5 can_vector[264].word_0
#define V_TOT_PACK1 can_vector[264].word_1
#define V_REF_PACK1 can_vector[264].word_2
#define FLAG_BAL_PACK1 can_vector[264].word_3
//PACK1

//PACK2
#define V_CELL_PACK2_1 can_vector[265].word_0
#define V_CELL_PACK2_2 can_vector[265].word_1
#define V_CELL_PACK2_3 can_vector[265].word_2
#define V_CELL_PACK2_4 can_vector[265].word_3

#define V_CELL_PACK2_5 can_vector[266].word_0
#define V_CELL_PACK2_6 can_vector[266].word_1
#define V_CELL_PACK2_7 can_vector[266].word_2
#define V_CELL_PACK2_8 can_vector[266].word_3

#define V_CELL_PACK2_9 can_vector[267].word_0
#define V_CELL_PACK2_10 can_vector[267].word_1
#define V_CELL_PACK2_11 can_vector[267].word_2
#define V_CELL_PACK2_12 can_vector[267].word_3

#define TEMP_PACK2_1 can_vector[268].word_0
#define TEMP_PACK2_2 can_vector[268].word_1
#define TEMP_PACK2_3 can_vector[268].word_2
#define TEMP_PACK2_4 can_vector[268].word_3

#define TEMP_PACK2_5 can_vector[269].word_0
#define V_TOT_PACK2 can_vector[269].word_1
#define V_REF_PACK2 can_vector[269].word_2
#define FLAG_BAL_PACK2 can_vector[269].word_3
//PACK2

//PACK3
#define V_CELL_PACK3_1 can_vector[270].word_0
#define V_CELL_PACK3_2 can_vector[270].word_1
#define V_CELL_PACK3_3 can_vector[270].word_2
#define V_CELL_PACK3_4 can_vector[270].word_3

#define V_CELL_PACK3_5 can_vector[271].word_0
#define V_CELL_PACK3_6 can_vector[271].word_1
#define V_CELL_PACK3_7 can_vector[271].word_2
#define V_CELL_PACK3_8 can_vector[271].word_3

#define V_CELL_PACK3_9 can_vector[272].word_0
#define V_CELL_PACK3_10 can_vector[272].word_1
#define V_CELL_PACK3_11 can_vector[272].word_2
#define V_CELL_PACK3_12 can_vector[272].word_3

#define TEMP_PACK3_1 can_vector[273].word_0
#define TEMP_PACK3_2 can_vector[273].word_1
#define TEMP_PACK3_3 can_vector[273].word_2
#define TEMP_PACK3_4 can_vector[273].word_3

#define TEMP_PACK3_5 can_vector[274].word_0
#define V_TOT_PACK3 can_vector[274].word_1
#define V_REF_PACK3 can_vector[274].word_2
#define FLAG_BAL_PACK3 can_vector[274].word_3
//PACK3

//PACK4
#define V_CELL_PACK4_1 can_vector[275].word_0
#define V_CELL_PACK4_2 can_vector[275].word_1
#define V_CELL_PACK4_3 can_vector[275].word_2
#define V_CELL_PACK4_4 can_vector[275].word_3

#define V_CELL_PACK4_5 can_vector[276].word_0
#define V_CELL_PACK4_6 can_vector[276].word_1
#define V_CELL_PACK4_7 can_vector[276].word_2
#define V_CELL_PACK4_8 can_vector[276].word_3

#define V_CELL_PACK4_9 can_vector[277].word_0
#define V_CELL_PACK4_10 can_vector[277].word_1
#define V_CELL_PACK4_11 can_vector[277].word_2
#define V_CELL_PACK4_12 can_vector[277].word_3

#define TEMP_PACK4_1 can_vector[278].word_0
#define TEMP_PACK4_2 can_vector[278].word_1
#define TEMP_PACK4_3 can_vector[278].word_2
#define TEMP_PACK4_4 can_vector[278].word_3

#define TEMP_PACK4_5 can_vector[279].word_0
#define V_TOT_PACK4 can_vector[279].word_1
#define V_REF_PACK4 can_vector[279].word_2
#define FLAG_BAL_PACK4 can_vector[279].word_3
//PACK4



/*BATERIAS*/


#endif /* INC_SD_H_ */

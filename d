[1mdiff --git a/.settings/language.settings.xml b/.settings/language.settings.xml[m
[1mindex 140bdbc..7113e01 100644[m
[1m--- a/.settings/language.settings.xml[m
[1m+++ b/.settings/language.settings.xml[m
[36m@@ -5,7 +5,7 @@[m
 			<provider copy-of="extension" id="org.eclipse.cdt.ui.UserLanguageSettingsProvider"/>[m
 			<provider-reference id="org.eclipse.cdt.core.ReferencedProjectsLanguageSettingsProvider" ref="shared-provider"/>[m
 			<provider-reference id="org.eclipse.cdt.managedbuilder.core.MBSLanguageSettingsProvider" ref="shared-provider"/>[m
[31m-			<provider class="com.st.stm32cube.ide.mcu.toolchain.armnone.setup.CrossBuiltinSpecsDetector" console="false" env-hash="-4526304643690041" id="com.st.stm32cube.ide.mcu.toolchain.armnone.setup.CrossBuiltinSpecsDetector" keep-relative-paths="false" name="MCU ARM GCC Built-in Compiler Settings" parameter="${COMMAND} ${FLAGS} -E -P -v -dD &quot;${INPUTS}&quot;" prefer-non-shared="true">[m
[32m+[m			[32m<provider class="com.st.stm32cube.ide.mcu.toolchain.armnone.setup.CrossBuiltinSpecsDetector" console="false" env-hash="-210766486793083581" id="com.st.stm32cube.ide.mcu.toolchain.armnone.setup.CrossBuiltinSpecsDetector" keep-relative-paths="false" name="MCU ARM GCC Built-in Compiler Settings" parameter="${COMMAND} ${FLAGS} -E -P -v -dD &quot;${INPUTS}&quot;" prefer-non-shared="true">[m
 				<language-scope id="org.eclipse.cdt.core.gcc"/>[m
 				<language-scope id="org.eclipse.cdt.core.g++"/>[m
 			</provider>[m
[36m@@ -17,7 +17,7 @@[m
 			<provider-reference id="org.eclipse.cdt.core.ReferencedProjectsLanguageSettingsProvider" ref="shared-provider"/>[m
 			<provider-reference id="org.eclipse.cdt.managedbuilder.core.MBSLanguageSettingsProvider" ref="shared-provider"/>[m
 			<provider copy-of="extension" id="org.eclipse.cdt.managedbuilder.core.GCCBuildCommandParser"/>[m
[31m-			<provider class="com.st.stm32cube.ide.mcu.toolchain.armnone.setup.CrossBuiltinSpecsDetector" console="false" env-hash="-4526304643690041" id="com.st.stm32cube.ide.mcu.toolchain.armnone.setup.CrossBuiltinSpecsDetector" keep-relative-paths="false" name="MCU ARM GCC Built-in Compiler Settings" parameter="${COMMAND} ${FLAGS} -E -P -v -dD &quot;${INPUTS}&quot;" prefer-non-shared="true">[m
[32m+[m			[32m<provider class="com.st.stm32cube.ide.mcu.toolchain.armnone.setup.CrossBuiltinSpecsDetector" console="false" env-hash="-210766486793083581" id="com.st.stm32cube.ide.mcu.toolchain.armnone.setup.CrossBuiltinSpecsDetector" keep-relative-paths="false" name="MCU ARM GCC Built-in Compiler Settings" parameter="${COMMAND} ${FLAGS} -E -P -v -dD &quot;${INPUTS}&quot;" prefer-non-shared="true">[m
 				<language-scope id="org.eclipse.cdt.core.gcc"/>[m
 				<language-scope id="org.eclipse.cdt.core.g++"/>[m
 			</provider>[m
[1mdiff --git a/Core/Src/SD.c b/Core/Src/SD.c[m
[1mindex 454b3fd..28dc6a5 100644[m
[1m--- a/Core/Src/SD.c[m
[1m+++ b/Core/Src/SD.c[m
[36m@@ -16,7 +16,8 @@[m [mextern CanIdData_t can_vector[CAN_IDS_NUMBER];[m
 static FATFS g_sFatFs;[m
 FIL file;[m
 FIL file1;[m
[31m-char bufferFile[20];//buffer with the name of the file[m
[32m+[m[32m// Forcando tudo ser salvo em apenas  um arquivo[m
[32m+[m[32mchar bufferFile[20] = "ARQ00.csv";//buffer with the name of the file[m
 int count = 0;[m
 uint8_t _datalog_flag = 0;[m
 [m
[36m@@ -65,7 +66,7 @@[m [mvoid readSD(void)[m
 		FRESULT fresult;[m
 		uint16_t ultima_linha[25];[m
 [m
[31m-		sprintf(bufferFile, "ARQ%02d.csv", aux);[m
[32m+[m		[32m//sprintf(bufferFile, "ARQ%02d.csv", aux);[m
 		fresult = f_stat(bufferFile, &file1);[m
 [m
 		fresult = f_open(&file1, bufferFile, FA_OPEN_ALWAYS | FA_READ); //open file on SD card to write*/[m
[36m@@ -101,7 +102,7 @@[m [mvoid Cabecalho(void)[m
 	           "3\tV_REF_3\tTemp401\tTemp402\tTemp403\tTemp404\tTemp405"[m
 	           "\tV_TOT_4\tV_REF_4\n");*/[m
 [m
[31m-	strcpy(cabecalho,   "sep = ,\ntime "[m
[32m+[m	[32mstrcpy(cabecalho,   "sep = ,\ntime, Numero_Resets "[m
 [m
 			"Velocidade_Media, Volante, Acelerador,"[m
 [m
[36m@@ -109,7 +110,37 @@[m [mvoid Cabecalho(void)[m
 [m
 			"Flag_Erro_ECU, Flag_Status, Referencia_MD, Referencia_ME,"[m
 [m
[31m-			"Frenagem_Regen,"[m
[32m+[m			[32m"Velocidade_DE, Velocidade_DD, Velocidade_TE, Velocidade_TD,"[m
[32m+[m
[32m+[m			[32m"Red_Toque_RE, Red_Torque_RD,Controle_D1\,Controle_D2,"[m
[32m+[m
[32m+[m			[32m"Frenagem_Regen, IMU_BSE, IMU_Vel,"[m
[32m+[m
[32m+[m			[32m"Vel_ME,Torq_ME, Pot_ME, Corre_ME,"[m
[32m+[m
[32m+[m			[32m"Energia_ME, SobreCarga_ME, Temp1_ME, Temp2_ME,"[m
[32m+[m
[32m+[m			[32m"MsgPerdida_ME, Busoff_ME, Est_CAN_ME,"[m
[32m+[m
[32m+[m			[32m"Est_Inv_ME, Falha_ME, Alarme_ME,"[m
[32m+[m
[32m+[m			[32m"Vel_MD, Torq_MD,Pot_MD, Corre_MD,"[m
[32m+[m
[32m+[m			[32m"Energia_MD, SobreCarga_MD, Temp1_MD,Temp2_MD,"[m
[32m+[m
[32m+[m			[32m"MsgPerdida_MD, Busoff_MD, Est_CAN_MD,"[m
[32m+[m
[32m+[m			[32m"Est_Inv_MD, Falha_MD, Alarme_MD,"[m
[32m+[m
[32m+[m			[32m"Acel_X, Acel_Y, Acel_Z, IMU_Temp,"[m
[32m+[m
[32m+[m			[32m"Giros_X, Giros_Y, Giros_Z, Erro_IMU,"[m
[32m+[m
[32m+[m			[32m"Beacon,"[m
[32m+[m
[32m+[m			[32m"Pressao_D, Pressao_T, Brake_Bias, Hodo_P_Aq,"[m
[32m+[m
[32m+[m			[32m"Data_Status,"[m
 [m
 			"Tensao_Max, Tensao_Min, Var_Tensao, Temp_Max,"[m
 [m
[36m@@ -117,6 +148,9 @@[m [mvoid Cabecalho(void)[m
 [m
 			"Corr_1_Alta, Corr_2_Alta,"[m
 [m
[32m+[m			[32m"Carga_Atual, Soc_Inicial, Carga_Int, Temp_Med,"[m
[32m+[m
[32m+[m			[32m"Val_SoC,"[m
 			"Ten_P1_C1, Ten_P1_C2, Ten_P1_C3, Ten_P1_C4,"[m
 			" Ten_P1_C5, Ten_P1_C6, Ten_P1_C7, Ten_P1_C8,"[m
 			" Ten_P1_C9, Ten_P1_C10, Ten_P1_C11, Ten_P1_C12,"[m
[36m@@ -181,7 +215,11 @@[m [mvoid writeSD(void)[m
 	uint32_t time = HAL_GetTick();[m
 [m
 	len = snprintf((char*) block, sizeof(block),[m
[31m-			"%lu,"[m
[32m+[m			[32m"%lu, %u"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,%u,"[m
 [m
 			"%u,%u,%u,%u,"[m
 [m
[36m@@ -191,12 +229,44 @@[m [mvoid writeSD(void)[m
 [m
 			"%u,"[m
 [m
[32m+[m			[32m"%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,"[m
[32m+[m
[32m+[m			[32m"%u,%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,"[m
[32m+[m
 			"%u,%u,%u,%u,"[m
 [m
 			"%u,%u,%u,%u,"[m
 [m
 			"%u,%u,"[m
 [m
[32m+[m			[32m"%u,%u,%u,%u,"[m
[32m+[m
[32m+[m			[32m"%u,"[m
[32m+[m
 			"%u,%u,%u,%u,"[m
 			"%u,%u,%u,%u,"[m
 			"%u,%u,%u,%u,"[m
[36m@@ -220,14 +290,32 @@[m [mvoid writeSD(void)[m
 			"%u,%u,%u,%u,"[m
 			"%u,%u,%u,%u,"[m
 			"%u,%u,%u,%u,\n",[m
[31m-			time,[m
[32m+[m			[32mtime, num_resets,[m
 			Velocidade_Media, Volante, Acelerador, Freio,[m
 			Modo, Ganho_Torque, Hodometro_P, Hodometro_T,[m
 			Flag_Erro_ECU, Flag_Status, Referencia_MD, Referencia_ME,[m
[32m+[m			[32mVelocidade_DE,Velocidade_DD, Velocidade_TE, Velocidade_TD,[m
[32m+[m			[32mRed_Toque_RE, Red_Torque_RD,Controle_D1, Controle_D2,[m
 			Frenagem_Regen,[m
[32m+[m			[32mIMU_BSE, IMU_Vel,[m
[32m+[m			[32mVel_ME,Torq_ME, Pot_ME, Corre_ME,[m
[32m+[m			[32mEnergia_ME, SobreCarga_ME, Temp1_ME, Temp2_ME,[m
[32m+[m			[32mMsgPerdida_ME, Busoff_ME, Est_CAN_ME,[m
[32m+[m			[32mEst_Inv_ME, Falha_ME, Alarme_ME,[m
[32m+[m			[32mVel_MD, Torq_MD,Pot_MD, Corre_MD,[m
[32m+[m			[32mEnergia_MD, SobreCarga_MD, Temp1_MD,Temp2_MD,[m
[32m+[m			[32mMsgPerdida_MD, Busoff_MD,Est_CAN_MD,[m
[32m+[m			[32mEst_Inv_MD, Falha_MD, Alarme_MD,[m
[32m+[m			[32mAcel_X, Acel_Y, Acel_Z, IMU_Temp,[m
[32m+[m			[32mGiros_X, Giros_Y, Giros_Z, Erro_IMU,[m
[32m+[m			[32mBeacon,[m
[32m+[m			[32mPressao_D, Pressao_T, Brake_Bias,Hodo_P_Aq,[m
[32m+[m			[32mData_Status,[m
 			Tensao_Max, Tensao_Min,Var_Tensao, Temp_Max,[m
 			Modo_BMS, Flag_Erro_BMS, Contatores, Tensao_Trat,[m
 			Corr_1_Alta, Corr_2_Alta,[m
[32m+[m			[32mCarga_Atual,Soc_Inicial, Carga_Int, Temp_Med,[m
[32m+[m			[32mVal_SoC,[m
 			Ten_P1_C1, Ten_P1_C2, Ten_P1_C3, Ten_P1_C4,[m
 			Ten_P1_C5, Ten_P1_C6, Ten_P1_C7, Ten_P1_C8,[m
 			Ten_P1_C9, Ten_P1_C10, Ten_P1_C11, Ten_P1_C12,[m
[36m@@ -313,3 +401,4 @@[m [mvoid writeSD(void)[m
 [m
 [m
 [m
[41m+[m

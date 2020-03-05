#include "main.h"
#include "stm32f1xx_hal.h"
#include "strings.h"
//#include "can.h"
#include "fatfs.h"
//#include "i2c.h"
//#include "spi.h"
//#include "gpio.h"
//#include "sd_hal_mpu6050.h"


void SetupACEL(void);//Funcao de inicializa�ao do MPU
void recordAccelRegisters(void);//Fun�ao de leitura do Acelerometro
void recordGyroRegisters(void);//funcao de leitura do Giroscopio
void temperatura(void);//desnecessario
void sdDemoStart(void);
void writeSd(void);

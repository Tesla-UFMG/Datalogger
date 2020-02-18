#include <master_da_prototipo.h>
CAN_HandleTypeDef hcan;
CAN_FilterTypeDef sFilterConfig;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8];
uint8_t RxData[8];
uint32_t TxMailbox;
I2C_HandleTypeDef hi2c1; //handle do i2c;
uint16_t addressACEL = 0x68;//endereco do MPU
uint8_t buffer[6] = {0};
int16_t gyroX, gyroY, gyroZ, accelX, accelY, accelZ;
int16_t temp;
uint8_t i;
uint8_t _accel_ok = 1;
long int timer = 0, timer1;

void SetupACEL(void)
{
	uint16_t MemAdd = 0x6B;
	buffer[0] = 0;
	timer = (long int)HAL_GetTick();
	while (HAL_I2C_Mem_Write(&hi2c1,(uint16_t)addressACEL<<1, MemAdd, 1, buffer, 1, 100) != HAL_OK )//Desabilita o sleep mode do MPU
	{
		timer1 = (long int)HAL_GetTick();
		if(timer1-timer >= 50)break;
	}
	MemAdd = (uint16_t)0x1B;
	buffer[0] = 0;
	timer = (long int)HAL_GetTick();
	while (HAL_I2C_Mem_Write(&hi2c1,(uint16_t)addressACEL<<1, MemAdd, 1, buffer, 1, 100) != HAL_OK)//Configura o giroscopio com a sensibilidade de 250
	{
		timer1 = (long int)HAL_GetTick();
		if(timer1-timer >= 50)break;
	}
	MemAdd = (uint16_t)0x1C;
	buffer[0] = 0;
	timer = (long int)HAL_GetTick();
	while (HAL_I2C_Mem_Write(&hi2c1,(uint16_t)addressACEL<<1, MemAdd, 1, buffer, 1, 100) != HAL_OK)//Configura o acelerometro com a sensibilidade de 4G
	{
		timer1 = (long int)HAL_GetTick();
		if(timer1-timer >= 50)break;
	}
	MemAdd = (uint16_t)0x1A;
	buffer[0] = 6;
	timer = (long int)HAL_GetTick();
	while (HAL_I2C_Mem_Write(&hi2c1,(uint16_t)addressACEL<<1, MemAdd, 1, buffer, 1, 100) != HAL_OK)//Configura o low pass filter interno do MPU como 6Hz
	{
		timer1 = (long int)HAL_GetTick();
		if(timer1-timer >= 50){
			_accel_ok = 0;
			break;
		}
	}
}

void recordAccelRegisters()
{
	buffer[0] = 0x3B;
	timer = (long int)HAL_GetTick();
	while (HAL_I2C_Master_Transmit(&hi2c1, (addressACEL<<1), buffer, 1, 100) != HAL_OK)//Inicializa a comunica�ao com o registrador do acelerometro
	{
		timer1 = (long int)HAL_GetTick();
				if(timer1-timer >= 50){
					_accel_ok = 0;
					break;
				}
	}
	timer = (long int)HAL_GetTick();
	while (HAL_I2C_Master_Receive(&hi2c1, (addressACEL<<1), buffer, 6, 100) != HAL_OK)//Recebe os dados do acelerometro
	{
		timer1 = (long int)HAL_GetTick();
				if(timer1-timer >= 50){
					_accel_ok = 0;
					break;
				}
	}
	accelX = (int16_t)(buffer[0] << 8 | buffer[1]);//buffer[0] == HSB **** buffer[1] == LSB de uma variavel de 16 bits
	accelY = (int16_t)(buffer[2] << 8 | buffer[3]);//buffer[2] == HSB **** buffer[3] == LSB de uma variavel de 16 bits
	accelZ = (int16_t)(buffer[4] << 8 | buffer[5]);//buffer[4] == HSB **** buffer[5] == LSB de uma variavel de 16 bits
	accelX = (accelX / 16384.0)*1000 - 37;
	accelY = (accelY / 16384.0)*1000 - 21;
	accelZ = (accelZ / 16384.0)*1000 + 117;
}

void recordGyroRegisters(void)
{
	buffer[0] = 0x43;
	timer = (long int)HAL_GetTick();
	while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)addressACEL<<1, buffer, 1, 100)!= HAL_OK)
	{
		timer1 = (long int)HAL_GetTick();
				if(timer1-timer >= 50){
					_accel_ok = 0;
					break;
				}
	}
	timer = (long int)HAL_GetTick();
	while(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)addressACEL<<1, buffer, 6, 100)!= HAL_OK)
	{
		timer1 = (long int)HAL_GetTick();
				if(timer1-timer >= 50){
					_accel_ok = 0;
					break;
				}
	}
	gyroX = (int16_t)(buffer[0] << 8 | buffer[1]);
	gyroY= (int16_t)(buffer[2] << 8 | buffer[3]);
	gyroZ = (int16_t)(buffer[4] << 8 | buffer[5]);
	gyroX = (gyroX / 131) + 6;
	gyroY = (gyroY / 131);
	gyroZ = (gyroZ / 131);
}
void temperatura(void)
{
	buffer[0] = 0x41;
	timer = (long int)HAL_GetTick();
	while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)addressACEL<<1, buffer, 1, 100)!= HAL_OK)
	{
		timer1 = (long int)HAL_GetTick();
				if(timer1-timer >= 50){
					_accel_ok = 0;
					break;
				}
	}
	timer = (long int)HAL_GetTick();
	while(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)addressACEL<<1, buffer, 2, 100)!= HAL_OK)
	{
		timer1 = (long int)HAL_GetTick();
				if(timer1-timer >= 50){
					_accel_ok = 0;
					break;
				}
	}
	temp = (int16_t)(buffer[0] << 8 | buffer[1]);
	temp = temp/340 + (int16_t)36.53;
}

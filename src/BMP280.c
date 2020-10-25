#include "BMP280.h"

int32_t t_fine;

uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;

uint16_t dig_P1;
int16_t dig_P2;
int16_t dig_P3;
int16_t dig_P4;
int16_t dig_P5;
int16_t dig_P6;
int16_t dig_P7;
int16_t dig_P8;
int16_t dig_P9;

void BMP280_Init(I2C_TypeDef* I2Cx)
{
	
}

int32_t BMP280_ReadTemp()
{	
	int32_t var1, var2, T, adc_T;
	
	adc_T = BMP280_Read_24(BMP280_REG_TEMPDATA);
	
	var1 = ((((adc_T / 8) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) / 2048;
	var2 = (((((adc_T / 16) - ((int32_t)dig_T1)) * ((adc_T / 16) - ((int32_t)dig_T1))) / 4096) * ((int32_t)dig_T3)) / 16384;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) / 256;
	
	return T;
}

int32_t BMP280_ReadPress()
{
	volatile float var1, var2, P, adc_P;
	adc_P = BMP280_Read_24(BMP280_REG_PRESSUREDATA);

	var1 = (t_fine / 2.0) - 64000.0;
	var2 = var1 * var1 * dig_P6 / 32768.0;
	var2 = var2 + var1 * dig_P5 * 2.0;
	var2 = (var2 / 4.0) + (dig_P4 * 65536.0);
	var1 = (dig_P3 * var1 * var1 / 524288.0 + dig_P2 * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0) * dig_P1;

	P = 1048576.0 - adc_P;
	P = (P - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = dig_P9 * P * P / 2147483648.0;
	var2 = P * dig_P8 / 32768.0;
	P = P + (var1 + var2 + dig_P7) / 16.0;
	
	return P;
}

void BMP280_WriteReg(uint8_t reg, uint8_t value)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
		
	I2C_GenerateSTART(I2C1, ENABLE);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, BMP280_ADDRESS << 1, I2C_Direction_Transmitter);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, reg);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C1, value);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t BMP280_Read_8(uint8_t reg)
{
	volatile uint8_t data = 0;
	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE); //first start
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, BMP280_ADDRESS << 1, I2C_Direction_Transmitter);
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, reg);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTART(I2C1, ENABLE); //second start
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_AckPositionConfig(I2C1, I2C_AckPosition_Next);
	
	I2C_Send7bitAddress(I2C1, BMP280_ADDRESS << 1, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	data = I2C_ReceiveData(I2C1);
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	return data;
}

uint16_t BMP280_Read_16(uint8_t reg) //LSB:MSB
{
	volatile uint16_t data = 0;
	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE); //first start
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, BMP280_ADDRESS << 1, I2C_Direction_Transmitter);
	
	I2C_AckPositionConfig(I2C1, I2C_AckPosition_Next);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, reg);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTART(I2C1, ENABLE); //second start
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, BMP280_ADDRESS << 1, I2C_Direction_Receiver);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	I2C_AckPositionConfig(I2C1, I2C_AckPosition_Next);
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	data |= I2C_ReceiveData(I2C1); //LSB
	
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	data |= (I2C_ReceiveData(I2C1) << 8); //MSB
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	return data;
}

uint32_t BMP280_Read_24(uint8_t reg) //MSB:LSB:XLSB
{	
	volatile uint32_t data = 0;
	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE); //first start
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, BMP280_ADDRESS << 1, I2C_Direction_Transmitter);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, reg);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTART(I2C1, ENABLE); //second start
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, BMP280_ADDRESS << 1, I2C_Direction_Receiver);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	data |= I2C_ReceiveData(I2C1);
	data <<= 8; //MSB
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	data |= I2C_ReceiveData(I2C1);
	data <<= 8; //LSB
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	data |= I2C_ReceiveData(I2C1); //XLSB
	data >>= 4;
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	return data;
}

void BMP280_ReadCalibration()
{
	dig_T1 = (uint16_t)BMP280_Read_16(BMP280_REG_DIG_T1);
  dig_T2 = (int16_t)BMP280_Read_16(BMP280_REG_DIG_T2);
  dig_T3 = (int16_t)BMP280_Read_16(BMP280_REG_DIG_T3);
	
  dig_P1 = (uint16_t)BMP280_Read_16(BMP280_REG_DIG_P1);
  dig_P2 = (int16_t)BMP280_Read_16(BMP280_REG_DIG_P2);
  dig_P3 = (int16_t)BMP280_Read_16(BMP280_REG_DIG_P3);
  dig_P4 = (int16_t)BMP280_Read_16(BMP280_REG_DIG_P4);
  dig_P5 = (int16_t)BMP280_Read_16(BMP280_REG_DIG_P5);
  dig_P6 = (int16_t)BMP280_Read_16(BMP280_REG_DIG_P6);
  dig_P7 = (int16_t)BMP280_Read_16(BMP280_REG_DIG_P7);
  dig_P8 = (int16_t)BMP280_Read_16(BMP280_REG_DIG_P8);
  dig_P9 = (int16_t)BMP280_Read_16(BMP280_REG_DIG_P9);
}
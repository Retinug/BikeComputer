#ifndef BMP280_H
#define BMP280_H

#include "stm8l15x.h"
#include "stm8l15x_i2c.h"

#define BMP280_ADDRESS 0x76
#define BMP280_ID 0x58

typedef enum {
	BMP280_REG_DIG_T1 = 0x88,
	BMP280_REG_DIG_T2 = 0x8A,
	BMP280_REG_DIG_T3 = 0x8C,
	BMP280_REG_DIG_P1 = 0x8E,
	BMP280_REG_DIG_P2 = 0x90,
	BMP280_REG_DIG_P3 = 0x92,
	BMP280_REG_DIG_P4 = 0x94,
	BMP280_REG_DIG_P5 = 0x96,
	BMP280_REG_DIG_P6 = 0x98,
	BMP280_REG_DIG_P7 = 0x9A,
  BMP280_REG_DIG_P8 = 0x9C,
  BMP280_REG_DIG_P9 = 0x9E,
  BMP280_REG_CHIPID = 0xD0,
  BMP280_REG_VERSION = 0xD1,
  BMP280_REG_SOFTRESET = 0xE0,
  BMP280_REG_CAL26 = 0xE1,
  BMP280_REG_STATUS = 0xF3,
  BMP280_REG_CONTROL = 0xF4,
  BMP280_REG_CONFIG = 0xF5,
  BMP280_REG_PRESSUREDATA = 0xF7,
  BMP280_REG_TEMPDATA = 0xFA
	}BMP280_REG_TypeDef;

typedef struct {
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
	} BMP280_CALIB_Data;

typedef enum
{
	BMP280_Sampling_none = (uint8_t)0x00,
	BMP280_Sampling_x1 = (uint8_t)0x01,
	BMP280_Sampling_x2 = (uint8_t)0x02,
	BMP280_Sampling_x4 = (uint8_t)0x03,
	BMP280_Sampling_x8 = (uint8_t)0x04,
	BMP280_Sampling_x16 = (uint8_t)0x05
	
} BMP280_Sampling;

typedef enum
{
	BMP280_Filter_none = (uint8_t)0x00,
	BMP280_Filter_x2 = (uint8_t)0x01,
	BMP280_Filter_x4 = (uint8_t)0x02,
	BMP280_Filter_x8 = (uint8_t)0x03,
	BMP280_Filter_x16 = (uint8_t)0x04
	
} BMP280_Filter;

typedef enum
{
	BMP280_Mode_Sleep = (uint8_t)0x00,
	BMP280_Mode_Forced = (uint8_t)0x01,
	BMP280_Mode_Normal = (uint8_t)0x03,
	BMP280_Mode_RESET = (uint8_t)0xB6

} BMP280_Mode;

typedef enum
{
	BMP280_Standby_MS1 = (uint8_t)0x00,
	BMP280_Standby_MS63 = (uint8_t)0x01,
	BMP280_Standby_MS125 = (uint8_t)0x02,
	BMP280_Standby_MS250 = (uint8_t)0x03,
	BMP280_Standby_MS500 = (uint8_t)0x04,
	BMP280_Standby_MS1000 = (uint8_t)0x05,
	BMP280_Standby_MS2000 = (uint8_t)0x06,
	BMP280_Standby_MS4000 = (uint8_t)0x07
} BMP280_Standby;

void BMP280_Init(I2C_TypeDef* I2Cx);
int32_t BMP280_ReadTemp(void);
int32_t BMP280_ReadPress(void);
void BMP280_ReadAlt();

void BMP280_WriteReg(uint8_t reg, uint8_t value);
uint8_t BMP280_Read_8(uint8_t reg);
int8_t BMP280_Read_8_sign(uint8_t reg);
uint16_t BMP280_Read_16(uint8_t reg);
uint32_t BMP280_Read_24(uint8_t reg);
void BMP280_ReadCalibration(void);

#endif
#ifndef BMP280_H
#define BMP280_H

#include "stm8l15x.h"

#define BMP280_ADDRESS 0x76
#define BMP280_ID 0x58

enum {
  BMP280_REGISTER_DIG_T1 = 0x88,
  BMP280_REGISTER_DIG_T2 = 0x8A,
  BMP280_REGISTER_DIG_T3 = 0x8C,
  BMP280_REGISTER_DIG_P1 = 0x8E,
  BMP280_REGISTER_DIG_P2 = 0x90,
  BMP280_REGISTER_DIG_P3 = 0x92,
  BMP280_REGISTER_DIG_P4 = 0x94,
  BMP280_REGISTER_DIG_P5 = 0x96,
  BMP280_REGISTER_DIG_P6 = 0x98,
  BMP280_REGISTER_DIG_P7 = 0x9A,
  BMP280_REGISTER_DIG_P8 = 0x9C,
  BMP280_REGISTER_DIG_P9 = 0x9E,
  BMP280_REGISTER_CHIPID = 0xD0,
  BMP280_REGISTER_VERSION = 0xD1,
  BMP280_REGISTER_SOFTRESET = 0xE0,
  BMP280_REGISTER_CAL26 = 0xE1,
  BMP280_REGISTER_STATUS = 0xF3,
  BMP280_REGISTER_CONTROL = 0xF4,
  BMP280_REGISTER_CONFIG = 0xF5,
  BMP280_REGISTER_PRESSUREDATA = 0xF7,
  BMP280_REGISTER_TEMPDATA = 0xFA,
};

typedef enum
{
	BMP280_Sampling_none = (uint8_t)0x00,
	BMP280_Sampling_x1 = (uint8_t)0x01,
	BMP280_Sampling_x2 = (uint8_t)0x02,
	BMP280_Sampling_x4 = (uint8_t)0x03,
	BMP280_Sampling_x8 = (uint8_t)0x04,
	BMP280_Sampling_x16 = (uint8_t)0x05,
	
} BMP280_Sampling;

typedef enum
{
	BMP280_Filter_none = (uint8_t)0x00,
	BMP280_Filter_x2 = (uint8_t)0x01,
	BMP280_Filter_x4 = (uint8_t)0x02,
	BMP280_Filter_x8 = (uint8_t)0x03,
	BMP280_Filter_x16 = (uint8_t)0x04,
	
} BMP280_Filter;

typedef enum
{
	BMP280_Mode_Sleep = (uint8_t)0x00,
	BMP280_Mode_Forced = (uint8_t)0x01,
	BMP280_Mode_Normal = (uint8_t)0x03,
	BMP280_Mode_RESET_CODE = (uint8_t)0xB6,

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
	BMP280_Standby_MS4000 = (uint8_t)0x07,

} BMP280_Standby;
 
void readTemp();
void readPress();
void readAlt();

#endif
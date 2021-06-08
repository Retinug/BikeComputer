#ifndef CALC_H
#define CALC_H

#include "stm8l15x.h"

#define CALC_PI 31415;
//#define CALC_INCH_CM 254;

uint32_t CALC_SpeedINCH(uint16_t millis, uint8_t diameter);
uint32_t CALC_DiameterLenMM(uint8_t diameter);
#endif
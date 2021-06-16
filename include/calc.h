#ifndef CALC_H
#define CALC_H

#include "stm8l15x.h"

#define CALC_PI 3.1415
#define CALC_INCH_CM 2.54

uint32_t CALC_Speed(uint16_t millis, uint8_t diameter);
uint8_t CALC_Cadence(uint16_t millis);

float CALC_Dist(uint32_t speed);
#endif
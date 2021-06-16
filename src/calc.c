#include "calc.h"

uint32_t CALC_Speed(uint16_t millis, uint8_t diameter)
{
	return (uint32_t)(1.0 / millis * (CALC_INCH_CM * diameter * CALC_PI) * 1000) * 3.6;
}

uint8_t CALC_Cadence(uint16_t millis)
{
	return 60 * 1000 / millis;

}

float CALC_Dist(uint32_t speed)
{
	return speed / 3.6 / 100.0;
}
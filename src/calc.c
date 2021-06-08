#include "calc.h"

uint32_t CALC_SpeedINCH(uint16_t millis, uint8_t diameter)
{
	uint32_t a;
	float res = 1.0/millis/1000.0*(2.54 * diameter * 3.1415)*1000.0;
	//uint32_t res = 254 * diameter * 31415 / 10000;
	//return 1000.0 / millis * res / 100;// * CALC_INCH_CM * freq * CALC_PI;
	a = (uint32_t)(res*1000.0);
	return a;
}
/*
uint32_t CALC_DiameterLenMM(uint8_t diameter)
{ 
	return CALC_INCH_CM * diameter;
}
*/
#ifndef BSTRING_H
#define BSTRING_H

#include "stm8l15x.h"
#include <string.h>
#include <math.h>

void itoa(int32_t num, char* arr, uint8_t prec);
void reverse(char* s);

#endif
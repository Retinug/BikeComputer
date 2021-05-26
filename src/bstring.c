#include "bstring.h"

void itoa(int32_t num, char* arr, uint8_t prec)
{
	//uint8_t ch[FONTBUFFER_SIZE] = {' '};
	uint8_t i, sign;
 
	if ((sign = num) < 0)
		num = -num;
	i = 0;
	do {
		arr[i++] = num % 10 + '0';
		if(prec > 0)
		{
			prec--;
			if(prec == 0)
				arr[i++] = '.';
		}
	} while ((num /= 10) > 0);
	if (sign < 0)
		arr[i++] = '-';
	arr[i] = '\0';	
}

void ftoa(float num, char* arr, uint8_t prec)
{
	uint8_t i, sign;
	num = floor(num);
	num /= 100;
	if ((sign = num) < 0)
		num = -num;
	i = 0;
	do {
		arr[i++] = num / 10 + '0';
		if(num < 1.0 && prec >= 0)
		{
			num * 10;
			if(prec > 0)
			{
				prec--;
			}
			else if(prec == 0)
			{
				arr[i++] = '.';
			}
		}
		
	} while ((num /= 10) > 0.0);
	if (sign < 0)
		arr[i++] = '-';
	arr[i] = '\0';	
}

void reverse(char* s)
{
	int i, j;
	char c;
	
	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
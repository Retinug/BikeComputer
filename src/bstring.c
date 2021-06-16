#include "bstring.h"

void itoa(int32_t num, char* arr, uint8_t prec)
{
	uint8_t i, sign;
 
	if (num < 0)
	{
		sign = 1;
		num = -num;
	}
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
	if (sign > 0)
		arr[i++] = '-';
	arr[i] = '\0';	
}

void reverse(char* s)
{
	uint8_t i, j;
	uint8_t c;
	
	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
#include "SH1106.h"


uint8_t buffer[128 * 64 / 8];

void SH1106_Init()
{
	int a = 0;
}

void SH1106_setPixel(int x, int y)
{
	if (x >= 0 && x < 128 && y >= 0 && y < 64) 
	{
		buffer[x + (y/8)*128] |=  (1 << (y&7));
	}
}


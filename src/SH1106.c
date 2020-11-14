#include "SH1106.h"

uint8_t buffer[128 * 64 / 8];

void SH1106_Init()
{
	SH1106_WriteCommand(SH1106_DISPLAYOFF); 				// 0xAE
	SH1106_WriteCommand(SH1106_CLKDIV);             // 0xD5
	SH1106_WriteCommand(0x80);
	SH1106_WriteCommand(SH1106_SETMULTRAT);         // 0xA8
	SH1106_WriteCommand(0x3F);
	SH1106_WriteCommand(SH1106_SETOFFSET);          // 0xD3
	SH1106_WriteCommand(0x00);
	
	SH1106_WriteCommand(SH1106_SETCHARGEPUMP);      // 0x30
	
	SH1106_WriteCommand(SH1106_SETSTARTLINE);				// 0x40
	SH1106_WriteCommand(SH1106_SETREMAPLEFT);				// 0xA1
	
	SH1106_WriteCommand(SH1106_COMDIRDEC);					// 0xC8
	SH1106_WriteCommand(SH1106_SETPAD); 						// 0xDA
	SH1106_WriteCommand(0x12);
	SH1106_WriteCommand(SH1106_SETCONTRAST);				// 0x81
	SH1106_WriteCommand(0xFF);
	SH1106_WriteCommand(SH1106_PRECHARGE);					// 0xd9
	SH1106_WriteCommand(0x22);
	SH1106_WriteCommand(SH1106_SETVCOM);						// 0xDB
	SH1106_WriteCommand(0x40);
	SH1106_WriteCommand(SH1106_SETENTIREOFF);				// 0xA4
	SH1106_WriteCommand(SH1106_SETNORMDISPLAY);			// 0xA6
	SH1106_WriteCommand(SH1106_DISPLAYON); 					// 0xAF
}

void SH1106_Update()
{
	int i, j;
	for(i = 0; i < 8; i++)
	{
		SH1106_WriteCommand(SH1106_SETPAGEADDR + i);
		SH1106_WriteCommand(SH1106_SETLOWCOLUMN & 0x0F);
		SH1106_WriteCommand(SH1106_SETHIGHCOLUMN >> 4);
		for(j = 0; j < 132; j++)
		{
			if(j < 128)
			{
				SH1106_WriteData(buffer[i * 128 + j]);
			}
			else
			{
				SH1106_WriteData(0);
			}
		}
	}
}

void SH1106_UpdatePage(uint8_t num)
{
	uint8_t i;
	SH1106_WriteCommand(SH1106_SETPAGEADDR + num);
	SH1106_WriteCommand(SH1106_SETLOWCOLUMN & 0x0F);
	SH1106_WriteCommand(SH1106_SETHIGHCOLUMN >> 4);
	
	for(i = 0; i < 132; i++)
	{
		if(i < 128)
		{
			SH1106_WriteData(buffer[num * 128 + i]);
		}
		else
		{
			SH1106_WriteData(0);
		}
	}
}

void SH1106_SetPixel(uint8_t x, uint8_t y)
{
	buffer[x + (y / 8) * 128] |= (1 << (y & 7));
}

void SH1106_WriteCommand(uint8_t command)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
		
	I2C_GenerateSTART(I2C1, ENABLE);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, SH1106_ADDRESS << 1, I2C_Direction_Transmitter);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, 0x00);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C1, command);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void SH1106_WriteData(uint8_t data)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
		
	I2C_GenerateSTART(I2C1, ENABLE);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, SH1106_ADDRESS << 1, I2C_Direction_Transmitter);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, 0x40);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C1, data);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}
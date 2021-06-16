#ifndef SH1106_H
#define SH1106_H

#include "stm8l15x.h"
#include "stm8l15x_i2c.h"

#include "macros.h"
#include <string.h>
#include "bstring.h"

#define SH1106_WIDTH 128
#define SH1106_HEIGHT 64

#define SH1106_ADDRESS 0x3C
#define SH1106_SETLOWCOLUMN 0x00
#define SH1106_SETHIGHCOLUMN 0x10

#define SH1106_SETCHARGEPUMP 0x30

#define SH1106_SETSTARTLINE 0x40
#define SH1106_SETCONTRAST 0x81
#define SH1106_SETREMAPRIGHT 0xA0
#define SH1106_SETREMAPLEFT 0xA1

#define SH1106_SETENTIREOFF 0xA4
#define SH1106_SETENTIREON 0xA5

#define SH1106_SETNORMDISPLAY 0xA6
#define SH1106_SETREVDISPLAY 0xA7
#define SH1106_SETMULTRAT 0xA8

#define SH1106_SETCONVERT 0xAD
#define SH1106_SETCONVERTON 0x8A
#define SH1106_SETCONVERTOFF 0x8B

#define SH1106_DISPLAYOFF 0xAE
#define SH1106_DISPLAYON 0xAF

#define SH1106_SETPAGEADDR 0xB0

#define SH1106_COMDIRINC 0xC0
#define SH1106_COMDIRDEC 0xC8

#define SH1106_SETOFFSET 0xD3

#define SH1106_CLKDIV 0xD5
#define SH1106_PRECHARGE 0xD9

#define SH1106_SETPAD 0xDA

#define SH1106_SETVCOM 0xDB

#define FONT_WIDTH 8
#define FONT_HEIGHT 16

#define FONTBUFFER_SIZE 10

typedef enum
{
	SH1106_ALIGN_LEFT = (uint8_t)0x00,
	SH1106_ALIGN_CENTER = (uint8_t)0x01,
	SH1106_ALIGN_RIGHT = (uint8_t)0x02

} SH1106_ALIGN;


extern uint8_t buffer[SH1106_WIDTH * SH1106_HEIGHT / 8];
extern const uint8_t font[];

void SH1106_Init(void);

void SH1106_Update(void);
void SH1106_UpdatePage(uint8_t num);

void SH1106_SetPixel(uint8_t x, uint8_t y);
void SH1106_DrawLine_Vert(uint8_t x, uint8_t y, uint8_t len);
void SH1106_DrawLine_Horiz(uint8_t x, uint8_t y, uint8_t len);
void SH1106_DrawNum(uint8_t x, uint8_t y, int32_t num, uint8_t len, uint8_t precision, SH1106_ALIGN align);
void SH1106_DrawChar(uint8_t x, uint8_t y, char ch);
void SH1106_DrawString(uint8_t x, uint8_t y, char* str);
void SH1106_DrawStringAlign(uint8_t x, uint8_t y, char* str, uint8_t len, SH1106_ALIGN align);
void SH1106_DrawStringAlignBox(uint8_t x, uint8_t y, char* str, uint8_t len, SH1106_ALIGN align);
void SH1106_DrawTime(uint8_t x, uint8_t y, uint8_t hour, uint8_t min);
void SH1106_Clear(void);

void SH1106_WriteCommand(uint8_t command);

#endif
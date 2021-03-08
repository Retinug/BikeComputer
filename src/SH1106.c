#include "SH1106.h"

uint8_t buffer[SH1106_WIDTH * SH1106_HEIGHT / 8];

const uint8_t font[] = {
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char  
	0x06, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0xFC, 0x0D, 0xFC, 0x0D, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char !
	0x07, 0x00, 0x00, 0x06, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x1E, 0x00, 0x00, 0x00,  // Code for char "
	0x07, 0x40, 0x02, 0xF0, 0x0F, 0xF0, 0x0F, 0x40, 0x02, 0xF0, 0x0F, 0xF0, 0x0F, 0x40, 0x02, 0x00, 0x00,  // Code for char #
	0x07, 0x30, 0x06, 0x78, 0x0E, 0xC8, 0x08, 0xFE, 0x3F, 0x88, 0x09, 0x38, 0x0F, 0x30, 0x06, 0x00, 0x00,  // Code for char $
	0x07, 0x18, 0x06, 0x24, 0x03, 0xA4, 0x01, 0xD8, 0x06, 0x60, 0x09, 0x30, 0x09, 0x18, 0x06, 0x00, 0x00,  // Code for char %
	0x07, 0x00, 0x07, 0x98, 0x0F, 0xFC, 0x08, 0xE4, 0x09, 0x3C, 0x07, 0x98, 0x0F, 0x80, 0x08, 0x00, 0x00,  // Code for char &
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char '
	0x06, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x03, 0xF8, 0x07, 0x0C, 0x0C, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00,  // Code for char (
	0x06, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x0C, 0x0C, 0xF8, 0x07, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00,  // Code for char )
	0x08, 0x80, 0x00, 0xA0, 0x02, 0xE0, 0x03, 0xC0, 0x01, 0xC0, 0x01, 0xE0, 0x03, 0xA0, 0x02, 0x80, 0x00,  // Code for char *
	0x07, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0xE0, 0x03, 0xE0, 0x03, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00,  // Code for char +
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ,
	0x07, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00,  // Code for char -
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char .
	0x07, 0x00, 0x06, 0x00, 0x03, 0x80, 0x01, 0xC0, 0x00, 0x60, 0x00, 0x30, 0x00, 0x18, 0x00, 0x00, 0x00,  // Code for char /
	0x07, 0xF8, 0x07, 0xFC, 0x0F, 0x84, 0x09, 0xC4, 0x08, 0x64, 0x08, 0xFC, 0x0F, 0xF8, 0x07, 0x00, 0x00,  // Code for char 0
	0x07, 0x00, 0x00, 0x10, 0x08, 0x18, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00,  // Code for char 1
	0x07, 0x08, 0x0E, 0x0C, 0x0F, 0x84, 0x09, 0xC4, 0x08, 0x64, 0x08, 0x3C, 0x0C, 0x18, 0x0C, 0x00, 0x00,  // Code for char 2
	0x07, 0x08, 0x04, 0x0C, 0x0C, 0x44, 0x08, 0x44, 0x08, 0x44, 0x08, 0xFC, 0x0F, 0xB8, 0x07, 0x00, 0x00,  // Code for char 3
	0x07, 0xC0, 0x01, 0xE0, 0x01, 0x30, 0x01, 0x18, 0x09, 0xFC, 0x0F, 0xFC, 0x0F, 0x00, 0x09, 0x00, 0x00,  // Code for char 4
	0x07, 0x7C, 0x04, 0x7C, 0x0C, 0x44, 0x08, 0x44, 0x08, 0x44, 0x08, 0xC4, 0x0F, 0x84, 0x07, 0x00, 0x00,  // Code for char 5
	0x07, 0xF8, 0x07, 0xFC, 0x0F, 0x44, 0x08, 0x44, 0x08, 0x44, 0x08, 0xCC, 0x0F, 0x88, 0x07, 0x00, 0x00,  // Code for char 6
	0x07, 0x0C, 0x00, 0x0C, 0x00, 0x04, 0x0F, 0x84, 0x0F, 0xC4, 0x00, 0x7C, 0x00, 0x3C, 0x00, 0x00, 0x00,  // Code for char 7
	0x07, 0xB8, 0x07, 0xFC, 0x0F, 0x44, 0x08, 0x44, 0x08, 0x44, 0x08, 0xFC, 0x0F, 0xB8, 0x07, 0x00, 0x00,  // Code for char 8
	0x07, 0x78, 0x04, 0xFC, 0x0C, 0x84, 0x08, 0x84, 0x08, 0x84, 0x08, 0xFC, 0x0F, 0xF8, 0x07, 0x00, 0x00,  // Code for char 9
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x06, 0x30, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char :
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x16, 0x30, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ;
	0x07, 0x00, 0x00, 0x80, 0x00, 0xC0, 0x01, 0x60, 0x03, 0x30, 0x06, 0x18, 0x0C, 0x08, 0x08, 0x00, 0x00,  // Code for char <
	0x07, 0x00, 0x00, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 0x00, 0x00,  // Code for char =
	0x07, 0x00, 0x00, 0x08, 0x08, 0x18, 0x0C, 0x30, 0x06, 0x60, 0x03, 0xC0, 0x01, 0x80, 0x00, 0x00, 0x00,  // Code for char >
	0x07, 0x18, 0x00, 0x1C, 0x00, 0x04, 0x00, 0x84, 0x0D, 0xC4, 0x0D, 0x7C, 0x00, 0x38, 0x00, 0x00, 0x00,  // Code for char ?
	0x08, 0xE0, 0x03, 0x10, 0x04, 0xC8, 0x0B, 0x28, 0x0A, 0x28, 0x09, 0xE8, 0x0B, 0x10, 0x02, 0xE0, 0x01,  // Code for char @
	0x07, 0xF0, 0x0F, 0xF8, 0x0F, 0x8C, 0x00, 0x84, 0x00, 0x8C, 0x00, 0xF8, 0x0F, 0xF0, 0x0F, 0x00, 0x00,  // Code for char A
	0x07, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0x44, 0x08, 0x44, 0x08, 0xFC, 0x0F, 0xB8, 0x07, 0x00, 0x00,  // Code for char B
	0x07, 0xF8, 0x07, 0xFC, 0x0F, 0x04, 0x08, 0x04, 0x08, 0x04, 0x08, 0x1C, 0x0E, 0x18, 0x06, 0x00, 0x00,  // Code for char C
	0x07, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0x04, 0x08, 0x04, 0x08, 0xFC, 0x0F, 0xF8, 0x07, 0x00, 0x00,  // Code for char D
	0x07, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0xC4, 0x08, 0xE4, 0x09, 0x0C, 0x0C, 0x1C, 0x0E, 0x00, 0x00,  // Code for char E
	0x07, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0xC4, 0x08, 0xE4, 0x01, 0x0C, 0x00, 0x1C, 0x00, 0x00, 0x00,  // Code for char F
	0x07, 0xF8, 0x07, 0xFC, 0x0F, 0x04, 0x08, 0x04, 0x08, 0x84, 0x08, 0x9C, 0x0F, 0x98, 0x0F, 0x00, 0x00,  // Code for char G
	0x07, 0xFC, 0x0F, 0xFC, 0x0F, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0xFC, 0x0F, 0xFC, 0x0F, 0x00, 0x00,  // Code for char H
	0x06, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00,  // Code for char I
	0x07, 0x00, 0x06, 0x00, 0x0E, 0x00, 0x08, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x07, 0x04, 0x00, 0x00, 0x00,  // Code for char J
	0x07, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0xC0, 0x00, 0xE0, 0x01, 0x3C, 0x0F, 0x1C, 0x0E, 0x00, 0x00,  // Code for char K
	0x07, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0x04, 0x08, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x0E, 0x00, 0x00,  // Code for char L
	0x07, 0xFC, 0x0F, 0xF8, 0x0F, 0x70, 0x00, 0xE0, 0x00, 0x70, 0x00, 0xF8, 0x0F, 0xFC, 0x0F, 0x00, 0x00,  // Code for char M
	0x07, 0xFC, 0x0F, 0xF8, 0x0F, 0x70, 0x00, 0xE0, 0x00, 0xC0, 0x01, 0xFC, 0x0F, 0xFC, 0x0F, 0x00, 0x00,  // Code for char N
	0x07, 0xF8, 0x07, 0xFC, 0x0F, 0x04, 0x08, 0x04, 0x08, 0x04, 0x08, 0xFC, 0x0F, 0xF8, 0x07, 0x00, 0x00,  // Code for char O
	0x07, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0x84, 0x08, 0x84, 0x00, 0xFC, 0x00, 0x78, 0x00, 0x00, 0x00,  // Code for char P
	0x07, 0xF8, 0x07, 0xFC, 0x0F, 0x04, 0x08, 0x04, 0x0E, 0x04, 0x0C, 0xFC, 0x1F, 0xF8, 0x17, 0x00, 0x00,  // Code for char Q
	0x07, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0x84, 0x00, 0x84, 0x01, 0xFC, 0x0F, 0x78, 0x0E, 0x00, 0x00,  // Code for char R
	0x07, 0x18, 0x06, 0x3C, 0x0E, 0x64, 0x08, 0x44, 0x08, 0xC4, 0x08, 0x9C, 0x0F, 0x18, 0x07, 0x00, 0x00,  // Code for char S
	0x07, 0x00, 0x00, 0x1C, 0x00, 0x0C, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0x0C, 0x08, 0x1C, 0x00, 0x00, 0x00,  // Code for char T
	0x07, 0xFC, 0x07, 0xFC, 0x0F, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0xFC, 0x0F, 0xFC, 0x07, 0x00, 0x00,  // Code for char U
	0x07, 0xFC, 0x01, 0xFC, 0x03, 0x00, 0x06, 0x00, 0x0C, 0x00, 0x06, 0xFC, 0x03, 0xFC, 0x01, 0x00, 0x00,  // Code for char V
	0x07, 0xFC, 0x0F, 0xFC, 0x07, 0x00, 0x03, 0x80, 0x01, 0x00, 0x03, 0xFC, 0x07, 0xFC, 0x0F, 0x00, 0x00,  // Code for char W
	0x07, 0x0C, 0x0C, 0x3C, 0x0F, 0xF0, 0x03, 0xE0, 0x01, 0xF0, 0x03, 0x3C, 0x0F, 0x0C, 0x0C, 0x00, 0x00,  // Code for char X
	0x07, 0x00, 0x00, 0x3C, 0x00, 0x7C, 0x08, 0xC0, 0x0F, 0xC0, 0x0F, 0x7C, 0x08, 0x3C, 0x00, 0x00, 0x00,  // Code for char Y
	0x07, 0x1C, 0x0E, 0x0C, 0x0F, 0x84, 0x09, 0xC4, 0x08, 0x64, 0x08, 0x3C, 0x0C, 0x1C, 0x0E, 0x00, 0x00,  // Code for char Z
	0x06, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x0F, 0xFC, 0x0F, 0x04, 0x08, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00,  // Code for char [
	0x07, 0x18, 0x00, 0x30, 0x00, 0x60, 0x00, 0xC0, 0x00, 0x80, 0x01, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00,  // Code for char BackSlash
	0x06, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0x00, 0x00, 0x00, 0x00,  // Code for char ]
	0x07, 0x20, 0x00, 0x30, 0x00, 0x18, 0x00, 0x0C, 0x00, 0x18, 0x00, 0x30, 0x00, 0x20, 0x00, 0x00, 0x00,  // Code for char ^
	0x08, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20,  // Code for char _
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char `
	0x07, 0x00, 0x07, 0xA0, 0x0F, 0xA0, 0x08, 0xA0, 0x08, 0xE0, 0x07, 0xC0, 0x0F, 0x00, 0x08, 0x00, 0x00,  // Code for char a
	0x07, 0x04, 0x00, 0xFC, 0x0F, 0xFC, 0x0F, 0x20, 0x08, 0x20, 0x08, 0xE0, 0x0F, 0xC0, 0x07, 0x00, 0x00,  // Code for char b
	0x07, 0xC0, 0x07, 0xE0, 0x0F, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0x60, 0x0C, 0x40, 0x04, 0x00, 0x00,  // Code for char c
	0x07, 0xC0, 0x07, 0xE0, 0x0F, 0x20, 0x08, 0x24, 0x08, 0xFC, 0x07, 0xFC, 0x0F, 0x00, 0x08, 0x00, 0x00,  // Code for char d
	0x07, 0xC0, 0x07, 0xE0, 0x0F, 0x20, 0x09, 0x20, 0x09, 0x20, 0x09, 0xE0, 0x0D, 0xC0, 0x05, 0x00, 0x00,  // Code for char e
	0x07, 0x00, 0x00, 0x20, 0x08, 0xF8, 0x0F, 0xFC, 0x0F, 0x24, 0x08, 0x2C, 0x00, 0x08, 0x00, 0x00, 0x00,  // Code for char f
	0x07, 0xC0, 0x27, 0xE0, 0x6F, 0x20, 0x48, 0x20, 0x48, 0xC0, 0x7F, 0xE0, 0x3F, 0x20, 0x00, 0x00, 0x00,  // Code for char g
	0x07, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0x40, 0x00, 0x20, 0x00, 0xE0, 0x0F, 0xC0, 0x0F, 0x00, 0x00,  // Code for char h
	0x06, 0x00, 0x00, 0x00, 0x00, 0x20, 0x08, 0xEC, 0x0F, 0xEC, 0x0F, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,  // Code for char i
	0x06, 0x00, 0x30, 0x00, 0x70, 0x00, 0x40, 0x20, 0x40, 0xEC, 0x7F, 0xEC, 0x3F, 0x00, 0x00, 0x00, 0x00,  // Code for char j
	0x07, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0x80, 0x01, 0xC0, 0x03, 0x60, 0x0E, 0x20, 0x0C, 0x00, 0x00,  // Code for char k
	0x06, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0xFC, 0x0F, 0xFC, 0x0F, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,  // Code for char l
	0x07, 0xE0, 0x0F, 0xE0, 0x0F, 0x60, 0x00, 0xC0, 0x07, 0x60, 0x00, 0xE0, 0x0F, 0xC0, 0x0F, 0x00, 0x00,  // Code for char m
	0x07, 0x20, 0x00, 0xE0, 0x0F, 0xC0, 0x0F, 0x20, 0x00, 0x20, 0x00, 0xE0, 0x0F, 0xC0, 0x0F, 0x00, 0x00,  // Code for char n
	0x07, 0xC0, 0x07, 0xE0, 0x0F, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0xE0, 0x0F, 0xC0, 0x07, 0x00, 0x00,  // Code for char o
	0x07, 0x20, 0x40, 0xE0, 0x7F, 0xC0, 0x7F, 0x20, 0x48, 0x20, 0x08, 0xE0, 0x0F, 0xC0, 0x07, 0x00, 0x00,  // Code for char p
	0x07, 0xC0, 0x07, 0xE0, 0x0F, 0x20, 0x08, 0x20, 0x48, 0xE0, 0x7F, 0xE0, 0x7F, 0x00, 0x40, 0x00, 0x00,  // Code for char q
	0x07, 0x20, 0x08, 0xE0, 0x0F, 0xC0, 0x0F, 0x60, 0x08, 0x20, 0x00, 0x60, 0x00, 0x60, 0x00, 0x00, 0x00,  // Code for char r
	0x07, 0x40, 0x04, 0xE0, 0x0C, 0xA0, 0x09, 0x20, 0x09, 0x20, 0x0B, 0x60, 0x0E, 0x40, 0x04, 0x00, 0x00,  // Code for char s
	0x06, 0x20, 0x00, 0x20, 0x00, 0xF8, 0x07, 0xFC, 0x0F, 0x20, 0x08, 0x20, 0x04, 0x00, 0x00, 0x00, 0x00,  // Code for char t
	0x07, 0xE0, 0x07, 0xE0, 0x0F, 0x00, 0x08, 0x00, 0x08, 0xE0, 0x07, 0xE0, 0x0F, 0x00, 0x08, 0x00, 0x00,  // Code for char u
	0x07, 0xE0, 0x01, 0xE0, 0x03, 0x00, 0x06, 0x00, 0x0C, 0x00, 0x06, 0xE0, 0x03, 0xE0, 0x01, 0x00, 0x00,  // Code for char v
	0x07, 0xE0, 0x07, 0xE0, 0x0F, 0x00, 0x0C, 0xC0, 0x07, 0x00, 0x0C, 0xE0, 0x0F, 0xE0, 0x07, 0x00, 0x00,  // Code for char w
	0x07, 0x60, 0x0C, 0xE0, 0x0E, 0x80, 0x03, 0x00, 0x01, 0x80, 0x03, 0xE0, 0x0E, 0x60, 0x0C, 0x00, 0x00,  // Code for char x
	0x07, 0xE0, 0x47, 0xE0, 0x4F, 0x00, 0x48, 0x00, 0x48, 0x00, 0x68, 0xE0, 0x3F, 0xE0, 0x1F, 0x00, 0x00,  // Code for char y
	0x07, 0x60, 0x0C, 0x20, 0x0E, 0x20, 0x0B, 0xA0, 0x09, 0xE0, 0x08, 0x60, 0x08, 0x20, 0x0C, 0x00, 0x00,  // Code for char z
	0x07, 0x00, 0x00, 0x40, 0x00, 0x40, 0x00, 0xF8, 0x07, 0xBC, 0x0F, 0x04, 0x08, 0x04, 0x08, 0x00, 0x00,  // Code for char {
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x0F, 0x3C, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char |
	0x07, 0x00, 0x00, 0x04, 0x08, 0x04, 0x08, 0xBC, 0x0F, 0xF8, 0x07, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00,  // Code for char }
	0x07, 0x08, 0x00, 0x0C, 0x00, 0x04, 0x00, 0x0C, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x04, 0x00, 0x00, 0x00,  // Code for char ~
	0x03, 0xF0, 0x1F, 0x10, 0x10, 0xF0, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // Code for char 
};

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
		SH1106_UpdatePage(i);
	}
}

void SH1106_UpdatePage(uint8_t num)
{
	uint8_t i;
	SH1106_WriteCommand(SH1106_SETPAGEADDR + num);
	SH1106_WriteCommand(SH1106_SETLOWCOLUMN);
	SH1106_WriteCommand(SH1106_SETHIGHCOLUMN);
	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, SH1106_ADDRESS << 1, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, 0x40);
	
	for(i = 0; i < 132; i++)
	{
		if(i < SH1106_WIDTH)
		{
			while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
			I2C_SendData(I2C1, buffer[num * SH1106_WIDTH + i]);
			
			while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		}
		else
		{
			while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
			I2C_SendData(I2C1, 0);
			
			while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		}
	}
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void SH1106_SetPixel(uint8_t x, uint8_t y)
{
	buffer[x + (y / 8) * SH1106_WIDTH] |= (1 << (y & 7));
}

void SH1106_DrawLine_Vert(uint8_t x, uint8_t y, uint8_t len)
{
	uint8_t i;
	for(i = 0; i < len; i++)
	{
		SH1106_SetPixel(x, y + i);
	}
}

void SH1106_DrawLine_Horiz(uint8_t x, uint8_t y, uint8_t len)
{
	uint8_t i;
	for(i = 0; i < len; i++)
	{
		SH1106_SetPixel(x + i, y);
	}
}

void SH1106_DrawNum(uint8_t x, uint8_t y, int16_t num, uint8_t precision)
{
	uint8_t ch[10] = {' '};
	int16_t numCount = num;
	uint8_t i = 0;
	bool isInt = TRUE;
	
	uint8_t len = 0;
	
	do
	{
		len++;
		numCount /= 10;
	}while(numCount);
	
	if(precision > 0)
	{
		len++;
		isInt = FALSE;
	}
	
	if(num < 0)
	{
		ch[0] = '-';
		num = -num;
		if(precision > 0) precision++;
		len++;
	}
	
	len--;
	
	do
	{
		if(!isInt && len - i == precision)
		{
			ch[len - i] = '.';
			i++;
		}
		else
		{
			ch[len - i] = num % 10 + 48;
			num /= 10;
			i++;
		}
	}while(num);
	
	SH1106_DrawString(x, y, ch);
}

void SH1106_DrawChar(uint8_t x, uint8_t y, char ch)
{
	uint16_t currInd, currBit;
	uint8_t currX = 0;
	
	uint16_t fontIndex = (ch - 32) + 16 * (ch - 32);
	
	for(currInd = fontIndex + 1; currInd < fontIndex + font[fontIndex] * 2; currInd+=2)
	{
		for(currBit = 0; currBit < 16; currBit++)
		{
			if(currBit < 8)
			{
				if(isbit(font[currInd],currBit))
				{
					SH1106_SetPixel(x + currX, y + currBit);
				}
			}
			else
			{
				if(isbit(font[currInd + 1],currBit - 8))
				{
					SH1106_SetPixel(x + currX, y + currBit);
				}
			}
		}
		
		currX++;
	}
}

void SH1106_DrawString(uint8_t x, uint8_t y, char* str)
{
	uint8_t ind, lenX = 0;
	
	while(str[++lenX] != '\0');
	
	for(ind = 0; ind < lenX; ind++)
	{
		SH1106_DrawChar(x, y, str[ind]);
		x+=8;
	}
}

void SH1106_DrawStringAlign(uint8_t x, uint8_t y, char* str, uint8_t len, SH1106_ALIGN align)
{
	uint8_t size, offset;
	if(align == SH1106_ALIGN_LEFT)
	{
		SH1106_DrawString(x, y, str);
	}
	else
	{
		while(str[++size] != '\0');
		size = size * FONT_WIDTH;
		//size = (sizeof(str) - 1) * FONT_WIDTH;
		
		if(align == SH1106_ALIGN_CENTER)
		{
			offset = len / 2 - size / 2;
			SH1106_DrawString(offset, y, str);
		}
		else
		{
			offset = len - size;
			SH1106_DrawString(offset, y, str);
		}
	}
}

void SH1106_Clear()
{
	uint16_t i;
	for(i = 0; i < (SH1106_WIDTH * SH1106_HEIGHT) / 8; i++)
	{
		buffer[i] = 0;
	}
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
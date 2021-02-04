#ifndef MACROS_H
#define MACROS_H

#define sbit(byte, bit) ((byte) |=  (1<<(bit))) //set
#define cbit(byte, bit) ((byte) &= ~(1<<(bit))) //clear
#define tbit(byte, bit) ((byte) ^=  (1<<(bit)))	//toggle
#define isbit(byte, bit)((byte) &   (1<<(bit))) //is bit set

#endif
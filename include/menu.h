#ifndef MENU_H
#define MENU_H

#include "SH1106.h"
#include "global.h"

typedef enum
{
	MENU_STATUS_OFF = 0,
	MENU_STATUS_MAIN,
	MENU_STATUS_ADDITIONAL1,
	MENU_STATUS_ADDITIONAL2,
	MENU_STATUS_SETTINGS_MAIN,
	MENU_STATUS_SETTINGS_DIAMETER,
	MENU_STATUS_SETTINGS_ETRTO,
	MENU_STATUS_SETTINGS_CADENCE,
	MENU_STATUS_SETTINGS_TIME,
	MENU_STATUS_SETTINGS_RESET
} MENU_STATUS;

typedef enum
{
	SETTING_DIAMETER = 0,
	SETTING_ETRTO,
	SETTING_CADENCE,
	SETTING_TIME,
	SETTING_RESET,
	SETTING_EXIT
} SETTING_STATUS;

typedef struct
{
	uint8_t len;
	char* label;
	char* name[8];
} SETTING_STRUCT;


extern MENU_STATUS screenStatus;
extern MENU_STATUS screenStatusLast;
extern SETTING_STATUS settingSelected;
extern int8_t selectedSub;

void MENU_Change(MENU_STATUS newMenu, BUTTON_STATUS status, SETTING_STATUS newSetting, DATA_USER* user);
void MENU_DrawScreenLines(void);
void MENU_DrawScreenText(uint8_t count, char* str[]);
void MENU_DrawScreenData(uint16_t data1, uint8_t p1, uint16_t data2, uint8_t p2, uint16_t data3, uint8_t p3, uint16_t data4, uint8_t p4);
void MENU_DrawScreenLabel(char* label);
void MENU_DrawScreenSettings(int8_t num);
void MENU_DrawSubMenuOne(uint8_t param, char* label, char* paramText);
void MENU_DrawScreenDiameter(DATA_USER* user);
void MENU_DrawScreenEtrto();
void MENU_DrawScreenCadence(DATA_USER* user);
void MENU_DrawScreenTime();
void MENU_DrawScreenReset(DATA_USER* user);
void MENU_DrawBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
#endif
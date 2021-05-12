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
	SETTING_CADENCE,
	SETTING_TIME,
	SETTING_RESET,
	SETTING_EXIT
} SETTING_STATUS;

extern MENU_STATUS screenStatus;
extern SETTING_STATUS settingSelected;

void MENU_Change(MENU_STATUS newMenu, SETTING_STATUS newSetting, DATA_USER* user);
void MENU_DrawScreenLines(void);
void MENU_DrawScreenText(uint8_t count, char* str[]);
void MENU_DrawScreenData(uint16_t data1, uint8_t p1, uint16_t data2, uint8_t p2, uint16_t data3, uint8_t p3, uint16_t data4, uint8_t p4);
void MENU_DrawScreenSettings(int8_t num);
#endif
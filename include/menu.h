#ifndef MENU_H
#define MENU_H

#include "SH1106.h"
#include "BMP280.h"
#include "global.h"
#include "main.h"

#define MENU_TEXT_POS1 0, 0
#define MENU_TEXT_POS2 64, 0
#define MENU_TEXT_POS3 0, 32
#define MENU_TEXT_POS4 64, 32

#define MENU_DATA_POS1 0, 16
#define MENU_DATA_POS2 64, 16
#define MENU_DATA_POS3 0, 48
#define MENU_DATA_POS4 64, 48

typedef enum
{
	MENU_STATUS_OFF = 0,
	MENU_STATUS_MAIN,
	MENU_STATUS_ADDITIONAL1,
	MENU_STATUS_ADDITIONAL2,
	MENU_STATUS_SETTINGS_MAIN,
	MENU_STATUS_SETTINGS_DIAMETER,
	MENU_STATUS_SETTINGS_CADENCE,
	MENU_STATUS_SETTINGS_RESETRIDE,
	MENU_STATUS_SETTINGS_RESET
} MENU_STATUS;

typedef enum
{
	SETTING_DIAMETER = 0,
	SETTING_CADENCE,
	SETTING_RESETRIDE,
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

void MENU_Change(MENU_STATUS newMenu, BUTTON_STATUS status, SETTING_STATUS newSetting);
void MENU_DrawScreenVertLine(void);
void MENU_DrawScreenHorisLine(void);
void MENU_DrawScreenLines(void);
void MENU_DrawScreenText(uint8_t count, char* str[]);
void MENU_DrawScreenData(uint32_t data1, uint8_t p1, uint32_t data2, uint8_t p2, uint32_t data3, uint8_t p3, uint32_t data4, uint8_t p4);
void MENU_DrawScreenParam(uint8_t count, char* str[]);
void MENU_DrawScreenLabel(char* label);
void MENU_DrawScreenSettings(int8_t num);
void MENU_DrawSubMenuOne(uint8_t param, char* label, char* paramText);
void MENU_DrawScreenDiameter(void);
void MENU_DrawScreenCadence(void);
void MENU_DrawScreenReset(void);
#endif
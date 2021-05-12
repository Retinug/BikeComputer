#include "menu.h"

const char *settingMenu[] = {"Diameter", "Cadence", "Time", "Reset", "Exit"};

MENU_STATUS screenStatus = MENU_STATUS_OFF;
SETTING_STATUS settingSelected = SETTING_DIAMETER;
uint8_t offset = 0;

void MENU_Change(MENU_STATUS newMenu, SETTING_STATUS newSetting, DATA_USER* user)
{
	switch(screenStatus)
	{
		case MENU_STATUS_OFF:
			break;
			
		case MENU_STATUS_MAIN:
			MENU_DrawScreenText("CAD", "SPD", "DIST", "R TIME");
			MENU_DrawScreenData(user->wheel, 0, user->pedal, 0, user->wheel, 0, user->wheel, 0);
			break;
			
		case MENU_STATUS_ADDITIONAL1:
			MENU_DrawScreenText("ODO", "ATIME", "ALT", "TEMP");
			MENU_DrawScreenData(1, 0, 2, 0, 3, 0, user->temp, 2);
			//SH1106_DrawStringAlign(0, 31, "Add1", 128, SH1106_ALIGN_CENTER);
			//SH1106_DrawNum(20, 48, alt, 0);
			//SH1106_DrawNum(20, 16, temp, 2);
			break;
			
		case MENU_STATUS_ADDITIONAL2:
			MENU_DrawScreenText("MAX", "TIME", "AVG", " ");
			//SH1106_DrawStringAlign(0, 31, "Add2", 128, SH1106_ALIGN_CENTER);
			break;
			
		case MENU_STATUS_SETTINGS_MAIN:
			MENU_DrawScreenSettings(newSetting);
			break;
			
		case MENU_STATUS_SETTINGS_DIAMETER:
			break;
			
		case MENU_STATUS_SETTINGS_ETRTO:
			break;
			
		case MENU_STATUS_SETTINGS_CADENCE:
			break;
			
		case MENU_STATUS_SETTINGS_TIME:
			break;
			
		case MENU_STATUS_SETTINGS_RESET:
			break;
	}
}

void MENU_DrawScreenLines()
{
	SH1106_DrawLine_Horiz(0, SH1106_HEIGHT / 2 - 1, SH1106_WIDTH);
	SH1106_DrawLine_Horiz(0, SH1106_HEIGHT / 2, SH1106_WIDTH);
	SH1106_DrawLine_Vert(SH1106_HEIGHT - 1, 0, SH1106_WIDTH);
	SH1106_DrawLine_Vert(SH1106_HEIGHT, 0, SH1106_WIDTH);
}

void MENU_DrawScreenText(char* str1, char* str2, char* str3, char* str4)
{
	SH1106_DrawStringAlign(0, 0, str1, SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
	SH1106_DrawStringAlign(SH1106_WIDTH / 2, 0, str2, SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
	SH1106_DrawStringAlign(0, SH1106_HEIGHT / 2, str3, SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
	SH1106_DrawStringAlign(SH1106_WIDTH / 2, SH1106_HEIGHT / 2, str4, SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
}

void MENU_DrawScreenData(uint16_t data1, uint8_t p1, uint16_t data2, uint8_t p2, uint16_t data3, uint8_t p3, uint16_t data4, uint8_t p4)
{
	SH1106_DrawNum(0, 16, data1, SH1106_WIDTH / 2, p1, SH1106_ALIGN_CENTER);
  SH1106_DrawNum(0, 48, data2, SH1106_WIDTH / 2, p2, SH1106_ALIGN_CENTER);
	SH1106_DrawNum(64, 16, data3, SH1106_WIDTH / 2, p3, SH1106_ALIGN_CENTER);
	SH1106_DrawNum(64, 48, data4, SH1106_WIDTH / 2, p4, SH1106_ALIGN_CENTER);
	//SH1106_DrawNum(0, 48, data3, p3);
	//SH1106_DrawNum(64, 48, data4, p4);
	//SH1106_DrawNum(0, 16, data1, 0);
	//SH1106_DrawStringAlign(0, 16, str1, SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
	//SH1106_DrawStringAlign(SH1106_WIDTH / 2, 48, str2, SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
	//SH1106_DrawStringAlign(0, SH1106_HEIGHT / 2 + 16, str3, SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
	//SH1106_DrawStringAlign(SH1106_WIDTH / 2, SH1106_HEIGHT / 2 + 48, str4, SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
}

void MENU_DrawScreenSettings(int8_t num)
{
	uint8_t i, selectY;
	volatile uint8_t a = num;
	if(offset == 0 && num > 3) offset++;
	if(offset != 0 && num < 1) offset--;
	selectY = (num - offset) * FONT_HEIGHT;
	SH1106_DrawChar(2, selectY , '>');
	for(i = 0; i < 4; i++)
	{
		SH1106_DrawString(10, i*16, settingMenu[i+offset]);
	}
}
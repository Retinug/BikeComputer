#include "menu.h"

const char *settingMenu[] = {"Diameter", "Cadence", "Time", "Reset", "Exit"};

const char *textMain[] = {"CAD", "SPD", "DIST", "R TIME"};
const char *textAdd1[] = {"ODO", "ATIME", "ALT", "TEMP"};
const char *textAdd2[] = {"MAX", "TIME", "AVG"};

MENU_STATUS screenStatus = MENU_STATUS_OFF;
//MENU_STATUS_SETTINGS screenStatus = MENU_STATUS_OFF;
SETTING_STATUS settingSelected = SETTING_DIAMETER;
uint8_t offset = 0;

void MENU_Change(MENU_STATUS newMenu, SETTING_STATUS newSetting, DATA_USER* user)
{
	switch(screenStatus)
	{
		case MENU_STATUS_OFF:
			break;
			
		case MENU_STATUS_MAIN:
			MENU_DrawScreenLines();
			MENU_DrawScreenText(4, textMain);
			MENU_DrawScreenData(user->wheel, 0, user->pedal, 0, user->wheel, 0, user->wheel, 0);
			break;
			
		case MENU_STATUS_ADDITIONAL1:
			MENU_DrawScreenLines();
			MENU_DrawScreenText(4, textAdd1);
			MENU_DrawScreenData(1, 0, 2, 0, 3, 0, user->temp, 2);
			//SH1106_DrawStringAlign(0, 31, "Add1", 128, SH1106_ALIGN_CENTER);
			//SH1106_DrawNum(20, 48, alt, 0);
			//SH1106_DrawNum(20, 16, temp, 2);
			break;
			
		case MENU_STATUS_ADDITIONAL2:
			MENU_DrawScreenLines();
			MENU_DrawScreenText(3, textAdd2);
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

void MENU_DrawScreenText(uint8_t count, char* str[])
{
	if(count > 0)
		SH1106_DrawStringAlign(0, 0, str[0], SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
	if(count > 1)
		SH1106_DrawStringAlign(SH1106_WIDTH / 2, 0, str[1], SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
	if(count > 2)
		SH1106_DrawStringAlign(0, SH1106_HEIGHT / 2, str[2], SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
	if(count > 3)
		SH1106_DrawStringAlign(SH1106_WIDTH / 2, SH1106_HEIGHT / 2, str[3], SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
}

void MENU_DrawScreenData(uint16_t data1, uint8_t p1, uint16_t data2, uint8_t p2, uint16_t data3, uint8_t p3, uint16_t data4, uint8_t p4)
{
	SH1106_DrawNum(0, 16, data1, SH1106_WIDTH / 2, p1, SH1106_ALIGN_CENTER);
  SH1106_DrawNum(0, 48, data2, SH1106_WIDTH / 2, p2, SH1106_ALIGN_CENTER);
	SH1106_DrawNum(64, 16, data3, SH1106_WIDTH / 2, p3, SH1106_ALIGN_CENTER);
	SH1106_DrawNum(64, 48, data4, SH1106_WIDTH / 2, p4, SH1106_ALIGN_CENTER);
}

void MENU_DrawScreenSettings(int8_t num)
{
	uint8_t i, selected;
	volatile uint8_t a = num;
	if(offset == 0 && num > 3) offset++;
	if(offset != 0 && num < 1) offset--;
	selected = (num - offset);// * FONT_HEIGHT;
	SH1106_DrawChar(2, selected * FONT_HEIGHT , '>');
	for(i = 0; i < 4; i++)
	{
		SH1106_DrawString(10, i*16, settingMenu[i+offset]);
	}
	//screenStatus = selected;
}
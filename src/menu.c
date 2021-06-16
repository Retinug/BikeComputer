#include "menu.h"

const SETTING_STRUCT settingMenu = {5, "Settings", {"Diameter", "Cadence", "Reset ride", "Reset", "Exit"}};
const char* textMain[] = {"CAD", "SPD", "DIST", "R TIME"};
const char* textAdd1[] = {"ODO", "A TIME", "ALT", "TEMP"};
const char* textAdd2[] = {"MAX", "AVG"};
const char* textReset[] = {"No", "Yes"};
MENU_STATUS screenStatus = MENU_STATUS_OFF;
MENU_STATUS screenStatusLast = MENU_STATUS_OFF;
SETTING_STATUS settingSelected = SETTING_DIAMETER;
int8_t selectedSub = -1;
uint8_t offset = 0;
int8_t prevNum = -1;

void MENU_Change(MENU_STATUS newMenu, BUTTON_STATUS status, SETTING_STATUS newSetting)
{
	switch(screenStatus)
	{
		case MENU_STATUS_OFF:
			break;
			
		case MENU_STATUS_MAIN:
			MENU_DrawScreenLines();
			MENU_DrawScreenText(4, textMain);
			if(user.cadenceLimit - 30 > user.cadence)
				SH1106_DrawChar(10, 0, '^');
			SH1106_DrawNum(MENU_DATA_POS1, user.cadence, SH1106_WIDTH / 2, 0, SH1106_ALIGN_CENTER);
			SH1106_DrawNum(MENU_DATA_POS2, user.speed, SH1106_WIDTH / 2, 2, SH1106_ALIGN_CENTER);
			if(user.distance < 1000)
			{
				
				SH1106_DrawNum(MENU_DATA_POS3, (int32_t)user.distance, SH1106_WIDTH / 2, 0, SH1106_ALIGN_CENTER);
			}
			else
				SH1106_DrawNum(MENU_DATA_POS3, (int32_t)user.distance / 10, SH1106_WIDTH / 2, 2, SH1106_ALIGN_CENTER);
			SH1106_DrawTime(MENU_DATA_POS4, user.timeR.hour, user.timeR.min);
			
			break;
			
		case MENU_STATUS_ADDITIONAL1:
			MENU_DrawScreenLines();
			MENU_DrawScreenText(4, textAdd1);
			if(user.odometer < 1000)
			{
				
				SH1106_DrawNum(MENU_DATA_POS1, (int32_t)user.odometer, SH1106_WIDTH / 2, 0, SH1106_ALIGN_CENTER);
			}
			else
				SH1106_DrawNum(MENU_DATA_POS1, (int32_t)user.odometer / 10, SH1106_WIDTH / 2, 2, SH1106_ALIGN_CENTER);
				
			SH1106_DrawTime(MENU_DATA_POS2, user.timeA.hour, user.timeA.min);
			SH1106_DrawNum(MENU_DATA_POS3, alt, SH1106_WIDTH / 2, 0, SH1106_ALIGN_CENTER);
			SH1106_DrawNum(MENU_DATA_POS4, temp, SH1106_WIDTH / 2, 2, SH1106_ALIGN_CENTER);
			break;
			
		case MENU_STATUS_ADDITIONAL2:
			MENU_DrawScreenVertLine();
			MENU_DrawScreenText(2, textAdd2);
			SH1106_DrawNum(MENU_DATA_POS1, user.speedMax, SH1106_WIDTH / 2, 2, SH1106_ALIGN_CENTER);
			SH1106_DrawNum(MENU_DATA_POS2, user.speedAvg, SH1106_WIDTH / 2, 2, SH1106_ALIGN_CENTER);
			break;
		
		case MENU_STATUS_SETTINGS_MAIN:
			MENU_DrawScreenSettings(newSetting);
			break;
			
		case MENU_STATUS_SETTINGS_DIAMETER:
			MENU_DrawScreenDiameter();
			break;
			
		case MENU_STATUS_SETTINGS_CADENCE:
			MENU_DrawScreenCadence();
			break;
			
		case MENU_STATUS_SETTINGS_RESETRIDE:
			MENU_DrawScreenReset();
			break;
			
		case MENU_STATUS_SETTINGS_RESET:
			MENU_DrawScreenReset();
			break;
	}
}

void MENU_DrawScreenVertLine()
{
	SH1106_DrawLine_Vert(SH1106_HEIGHT - 1, 0, SH1106_WIDTH);
	SH1106_DrawLine_Vert(SH1106_HEIGHT, 0, SH1106_WIDTH);
}
void MENU_DrawScreenHorisLine()
{
	SH1106_DrawLine_Horiz(0, SH1106_HEIGHT / 2 - 1, SH1106_WIDTH);
	SH1106_DrawLine_Horiz(0, SH1106_HEIGHT / 2, SH1106_WIDTH);
}

void MENU_DrawScreenLines()
{
	MENU_DrawScreenVertLine();
	MENU_DrawScreenHorisLine();
}

void MENU_DrawScreenText(uint8_t count, char* str[])
{
	if(count > 0)
		SH1106_DrawStringAlign(MENU_TEXT_POS1, str[0], SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
	if(count > 1)
		SH1106_DrawStringAlign(MENU_TEXT_POS2, str[1], SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
	if(count > 2)
		SH1106_DrawStringAlign(MENU_TEXT_POS3, str[2], SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
	if(count > 3)
		SH1106_DrawStringAlign(MENU_TEXT_POS4, str[3], SH1106_WIDTH / 2, SH1106_ALIGN_CENTER);
}

void MENU_DrawScreenLabel(char* label)
{
	SH1106_DrawStringAlign(0, 0, label, SH1106_WIDTH, SH1106_ALIGN_CENTER);
	SH1106_DrawLine_Horiz(0, 16, SH1106_WIDTH);
}

void MENU_DrawScreenSettings(int8_t num)
{
	uint8_t i;
	int8_t select;
	if(prevNum == -1)
		prevNum = 0;
		
	if(prevNum == 0 && num == settingMenu.len - 1) //start - end
		offset = settingMenu.len - 3;
	else if(prevNum == settingMenu.len - 1 && num == 0) //end - start
		offset = 0;
	else if(prevNum < num && num - offset > 2) //to end
		offset++;
	else if(prevNum > num && num - offset < 0) //to start
		offset--;
	select = num - offset;
	
	MENU_DrawScreenLabel(settingMenu.label);
	SH1106_DrawChar(2, select * FONT_HEIGHT + FONT_HEIGHT , '>');
	for(i = 0; i < 3; i++)
	{
		SH1106_DrawString(10, i * FONT_HEIGHT + FONT_HEIGHT, settingMenu.name[i+offset]);
	}
	prevNum = num;
}

void MENU_DrawSubMenuOne(uint8_t param, char* label, char* paramText)
{
	switch(selectedSub)
	{
		case 0:
			SH1106_DrawChar(2, 16, '>');
		break;
		case 1:
			SH1106_DrawChar(2, 32, '>');
		break;
		case 2:
			SH1106_DrawChar(84, 16, '>');
		break;
	}
	
	MENU_DrawScreenLabel(label);
	SH1106_DrawString(16, 16, paramText);
	SH1106_DrawNum(100, 16, (uint16_t)param, FONT_WIDTH * 3, 0, SH1106_ALIGN_LEFT);
	SH1106_DrawString(16, 32, "Exit");
}

void MENU_DrawScreenDiameter()
{
	MENU_DrawSubMenuOne(user.diameter, "Diameter", "Inch");
}

void MENU_DrawScreenCadence()
{
	MENU_DrawSubMenuOne(user.cadenceLimit, "Cadence limit", "Freq");
}

void MENU_DrawScreenReset()
{
	switch(selectedSub)
	{
		case 0:
			SH1106_DrawChar(0, 16, '>');
		break;
		case 1:
			SH1106_DrawChar(0, 32, '>');
		break;
	}
	
	MENU_DrawScreenLabel("Are you sure?");
	SH1106_DrawString(16, 16, "No");
	SH1106_DrawString(16, 32, "Yes");
}

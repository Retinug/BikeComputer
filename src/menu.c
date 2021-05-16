#include "menu.h"

//const char *settingMenu[] = {"Diameter", "Cadence", "Time", "Reset", "Exit"};

const SETTING_STRUCT settingMenu = {7, "Settings", {"Diameter", "Tire", "ETRTO", "Cadence", "Time", "Reset", "Exit"}};

//const SETTING_STRUCT diameterMenu = {6, "Diameter", {"Diameter", "ETRTO", "Cadence", "Time", "Reset", "Exit"}};
//const SETTING_STRUCT diameterMenu = {3, "Diameter", {"Standart", "ERTO", "Exit"}};

const char *textMain[] = {"CAD", "SPD", "DIST", "R TIME"};
const char *textAdd1[] = {"ODO", "ATIME", "ALT", "TEMP"};
const char *textAdd2[] = {"MAX", "TIME", "AVG"};

MENU_STATUS screenStatus = MENU_STATUS_OFF;
MENU_STATUS screenStatusLast = MENU_STATUS_OFF;
SETTING_STATUS settingSelected = SETTING_DIAMETER;
int8_t diameterStatus = -1, etrtoStatus = -1;
int8_t selectedSub = -1;
uint8_t offset = 0;
int8_t diameterNum = 0;
int8_t prevNum = -1;
bool direct;
bool last;
bool first;

void MENU_Change(MENU_STATUS newMenu, BUTTON_STATUS status, SETTING_STATUS newSetting, DATA_USER* user)
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
			MENU_DrawScreenDiameter(user, status);
			//MENU_DrawScreenSettings(&diameterMenu, newSetting);
			//SH1106_DrawStringAlignBox(0, 20, "Diameter", 128, SH1106_ALIGN_CENTER);
			//SH1106_DrawStringAlignBox(0, 40, "ETRTO", 128, SH1106_ALIGN_CENTER);
			//SH1106_DrawString(32, 20, "Diameter");
			//MENU_DrawBox(30, 18, 80, 38);
			//MENU_DrawBox(30, 39, 80, 58);
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
	
	SH1106_DrawStringAlign(0, 0, settingMenu.label, SH1106_WIDTH, SH1106_ALIGN_CENTER);
	SH1106_DrawChar(2, select * FONT_HEIGHT + FONT_HEIGHT , '>');
	for(i = 0; i < 3; i++)
	{
		SH1106_DrawString(10, i * FONT_HEIGHT + FONT_HEIGHT, settingMenu.name[i+offset]);
	}
	prevNum = num;
}

void MENU_DrawScreenDiameter(DATA_USER* user, BUTTON_STATUS status)
{
	
	
	switch(selectedSub)
	{
		case 0:
			SH1106_DrawChar(0, 16 , '>');
		break;
		case 1:
			SH1106_DrawChar(0, 32 , '>');
		break;
		case 2:
			SH1106_DrawChar(96, 16 , '>');
		break;
	}
	
	SH1106_DrawString(16, 16, "Diameter:");
	//
	SH1106_DrawNum(108, 16, user->diameter, FONT_WIDTH * 3, 0, SH1106_ALIGN_LEFT);
	SH1106_DrawString(16, 32, "Exit:");
	//if(diameterStatus == -1)
	//	diameterStatus = 0;
	//SH1106_DrawStringAlign(16, 16, "Diameter:", 80, SH1106_ALIGN_LEFT);
	//SH1106_DrawNum(80, 16, user->diameter, 0, SH1106_ALIGN_LEFT);
	
	/*switch(status)
	{
		case BUTTON_NEXT:
			if(diameterNum < 2)
				diameterNum++;
		break;
		
		case BUTTON_CENTER:
			switch(diameterNum)
			{
				case 0:
					diameterNum = 1;
				break;
				
				case 1: 
					diameterNum = 0;
					screenStatus = MENU_STATUS_SETTINGS_MAIN;
				break;
		}
		break;
		
		case BUTTON_PREV:
		break;
	}
	
	switch(diameterNum)
	{
		case 0:
			SH1106_DrawChar(0, 16 , '>');
		break;
		case 1:
			SH1106_DrawChar(0, 32 , '>');
		break;
		case 3:
			SH1106_DrawChar(0, 48 , '>');
		break;
	}
	
*/
}


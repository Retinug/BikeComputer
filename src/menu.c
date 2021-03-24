#include "menu.h"

MENU_STATUS screenStatus = MENU_STATUS_OFF;

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
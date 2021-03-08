#include "menu.h"

void MENU_DrawScreenLines()
{
	SH1106_DrawLine_Horiz(0, SH1106_HEIGHT / 2 - 1, SH1106_WIDTH);
	SH1106_DrawLine_Horiz(0, SH1106_HEIGHT / 2, SH1106_WIDTH);
	
	SH1106_DrawLine_Vert(SH1106_HEIGHT - 1, 0, SH1106_WIDTH);
	SH1106_DrawLine_Vert(SH1106_HEIGHT, 0, SH1106_WIDTH);
}
#include "main.h"

uint8_t count = 0;
bool needUpdate = FALSE;
uint16_t wheel = 0, pedal = 0;
volatile int16_t alt = 0;
volatile int16_t temp = 0;
uint32_t zeroPres = 0;

@far @interrupt void IRQ_Handler_EXTI_Button_1()
{
	if(screenStatus == MENU_STATUS_OFF) screenStatus = MENU_STATUS_MAIN;
	screenStatus--;
	if(screenStatus < 1) screenStatus = MENU_STATUS_ADDITIONAL2;
	needUpdate = TRUE;
	EXTI_ClearITPendingBit(EXTI_IT_Pin1);
}

@far @interrupt void IRQ_Handler_EXTI_Button_2()
{
	EXTI_ClearITPendingBit(EXTI_IT_Pin2);
}

@far @interrupt void IRQ_Handler_EXTI_Button_3()
{
	if(screenStatus == MENU_STATUS_OFF) screenStatus = MENU_STATUS_MAIN;
	screenStatus++;
	if(screenStatus > 3) screenStatus = MENU_STATUS_MAIN;
	needUpdate = TRUE;
	EXTI_ClearITPendingBit(EXTI_IT_Pin3);
}

@far @interrupt void IRQ_Handler_EXTI_Reed_Wheel()
{
	wheel++;
	
	EXTI_ClearITPendingBit(EXTI_IT_Pin4);
}

@far @interrupt void IRQ_Handler_EXTI_Reed_Pedal()
{
	pedal++;
	
	EXTI_ClearITPendingBit(EXTI_IT_Pin7);
}

@far @interrupt void IRQ_Handler_EXTI_Tim4_Ovf()
{
	if(count == 1)
	{
		TIM4_Cmd(DISABLE);
	}
	else
	{
		count++;
	}
	TIM4_ClearITPendingBit(TIM4_IT_Update);
}

void Init()
{
	CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
	
	GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Mode_In_FL_IT);
	GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Mode_In_FL_IT);
	GPIO_Init(GPIOD, GPIO_Pin_3, GPIO_Mode_In_FL_IT);
	
	GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_In_FL_IT);
	GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_In_FL_IT);
	
	EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling);
	EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Falling);
	EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Falling);
	EXTI_SetPinSensitivity(EXTI_Pin_4, EXTI_Trigger_Falling);
	EXTI_SetPinSensitivity(EXTI_Pin_7, EXTI_Trigger_Falling);
	
	CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);
	I2C_Init(I2C1, I2C_MAX_FAST_FREQ, 0x05, I2C_Mode_I2C, I2C_DutyCycle_2, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);
	I2C_Cmd(I2C1, ENABLE);

	I2C_AcknowledgeConfig(I2C1, ENABLE);

	CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
	TIM4_TimeBaseInit(TIM4_Prescaler_32768, 244);
	TIM4_ClearFlag(TIM4_FLAG_Update);
	TIM4_ITConfig(TIM4_IT_Update, ENABLE);
	TIM4_Cmd(ENABLE);

	SH1106_Init();
	
	BMP280_Config(BMP280_Standby_MS1, BMP280_Filter_x2, BMP280_Sampling_x2, BMP280_Sampling_x16, BMP280_Mode_Forced);
	BMP280_ReadTemp();
	zeroPres = BMP280_ReadPress();
}

void Update()
{
	switch(screenStatus)
	{
		case MENU_STATUS_OFF:
			break;
			
		case MENU_STATUS_MAIN:
			MENU_DrawScreenText("CAD", "SPD", "DIST", "R TIME");
			MENU_DrawScreenData(wheel, pedal, &wheel, &wheel);
			break;
			
		case MENU_STATUS_ADDITIONAL1:
			MENU_DrawScreenText("ODO", "ATIME", "ALT", "TEMP");
			//SH1106_DrawStringAlign(0, 31, "Add1", 128, SH1106_ALIGN_CENTER);
			//SH1106_DrawNum(20, 48, alt, 0);
			//SH1106_DrawNum(20, 16, temp, 2);
			break;
			
		case MENU_STATUS_ADDITIONAL2:
			MENU_DrawScreenText("MAX", "TIME", "AVG", " ");
			//SH1106_DrawStringAlign(0, 31, "Add2", 128, SH1106_ALIGN_CENTER);
			break;
			
		case MENU_STATUS_SETTINGS_MAIN:
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
	SH1106_Update();
	SH1106_Clear();
}

int main()
{
	Init();
	
	SH1106_WriteCommand(SH1106_DISPLAYOFF);
	
	MENU_DrawScreenLines();
	SH1106_Update();
	SH1106_WriteCommand(SH1106_DISPLAYON);
	
	enableInterrupts();

	screenStatus = MENU_STATUS_MAIN;
	
	while (1)
	{
		if(needUpdate == TRUE)
		{
			Update();
			needUpdate = FALSE;
		}
		if(count == 1)
			BMP280_Config(BMP280_Standby_MS1, BMP280_Filter_x2, BMP280_Sampling_x2, BMP280_Sampling_x16, BMP280_Mode_Forced);
		{
			count = 0;
			temp = (int16_t)BMP280_ReadTemp();
			alt = (int16_t)BMP280_ReadAlt(zeroPres);
			
			Update();
			
			//temp = BMP280_ReadTemp();
			//MENU_DrawScreenLines();
			//SH1106_DrawNum(10, 16, temp, 2);
			//SH1106_DrawString(0, 48, "String#!&");
			//SH1106_DrawString(10, 32, "Wheel");
			//SH1106_DrawStringAlign(0, 0, "Wheel", 63, SH1106_ALIGN_CENTER);
			//SH1106_DrawNum(10, 48, wheel, 0);
			//SH1106_DrawString(70, 32, "Pedal");
			//SH1106_DrawNum(70, 48, pedal, 0);
			
			TIM4_Cmd(ENABLE);
		}

	}
}
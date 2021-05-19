#include "main.h"

BUTTON_STATUS buttonPress = BUTTON_NONE;
BUTTON_STATUS buttonPressSubMenu = BUTTON_NONE;
uint8_t countTimer = 0;
bool needUpdate = FALSE;
uint32_t zeroPres = 0;
DATA_USER user;

@far @interrupt void IRQ_Handler_EXTI_Button_1()
{
	buttonPress = BUTTON_PREV;
	EXTI_ClearITPendingBit(EXTI_IT_Pin1);
}

@far @interrupt void IRQ_Handler_EXTI_Button_2()
{
	buttonPress = BUTTON_CENTER;
	EXTI_ClearITPendingBit(EXTI_IT_Pin2);
}

@far @interrupt void IRQ_Handler_EXTI_Button_3()
{
	buttonPress = BUTTON_NEXT;
	EXTI_ClearITPendingBit(EXTI_IT_Pin3);
}

@far @interrupt void IRQ_Handler_EXTI_Reed_Wheel()
{
	needUpdate = TRUE;
	user.wheel++;
	
	EXTI_ClearITPendingBit(EXTI_IT_Pin4);
}

@far @interrupt void IRQ_Handler_EXTI_Reed_Pedal()
{
	needUpdate = TRUE;
	user.pedal++;
	
	EXTI_ClearITPendingBit(EXTI_IT_Pin7);
}

@far @interrupt void IRQ_Handler_EXTI_Tim4_Ovf()
{
	if(countTimer++ > COUNT_OVF - 1)
	{
		TIM4_Cmd(DISABLE);
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
	needUpdate = TRUE;
}

void Update()
{
	MENU_Change(screenStatus, buttonPressSubMenu, settingSelected, &user);
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

	if(user.diameter < DIAMETERINCH_MIN)
		user.diameter = DIAMETERINCH_MIN;
	if(user.cadence < CADENCE_MIN)
		user.cadence = CADENCE_MIN;
	enableInterrupts();


	screenStatus = MENU_STATUS_MAIN;
	countTimer = COUNT_OVF;
	Update();
	while (1)
	{
		if(buttonPress)
		{
			buttonPressSubMenu = buttonPress;
			switch(screenStatus)
			{
				case MENU_STATUS_OFF:
					screenStatus = MENU_STATUS_MAIN;
					break;
			
				case MENU_STATUS_MAIN:
					screenStatusLast = screenStatus;
					switch(buttonPress)
					{
						case BUTTON_PREV:
							screenStatus = MENU_STATUS_ADDITIONAL2;
						break;
						
						case BUTTON_NEXT:
							screenStatus = MENU_STATUS_ADDITIONAL1;
						break;
						
						case BUTTON_CENTER:
							screenStatus = MENU_STATUS_SETTINGS_MAIN;
						break;
					}
					
					break;
					
				case MENU_STATUS_ADDITIONAL1:
					screenStatusLast = screenStatus;
					switch(buttonPress)
					{
						case BUTTON_PREV:
							screenStatus = MENU_STATUS_MAIN;
						break;
						
						case BUTTON_NEXT:
							screenStatus = MENU_STATUS_ADDITIONAL2;
						break;
						
						case BUTTON_CENTER:
							screenStatus = MENU_STATUS_SETTINGS_MAIN;
						break;
					}
					
					break;
					
				case MENU_STATUS_ADDITIONAL2:
					screenStatusLast = screenStatus;
					switch(buttonPress)
					{
						case BUTTON_PREV:
							screenStatus = MENU_STATUS_ADDITIONAL1;
						break;
						
						case BUTTON_NEXT:
							screenStatus = MENU_STATUS_MAIN;
						break;
						
						case BUTTON_CENTER:
							screenStatus = MENU_STATUS_SETTINGS_MAIN;
						break;
					}
					break;
					
				case MENU_STATUS_SETTINGS_MAIN:
					//if(settingSelected == SETTING_NONE) settingSelected = SETTING_DIAMETER;
					selectedSub = -1;
					TIM4_Cmd(DISABLE);
					switch(buttonPress)
					{
						case BUTTON_PREV:
							if(settingSelected == SETTING_DIAMETER) settingSelected = SETTING_EXIT;
							else settingSelected--;
						break;
						
						case BUTTON_NEXT:
							if(settingSelected == SETTING_EXIT) settingSelected = SETTING_DIAMETER;
							else settingSelected++;
						break;
						
						case BUTTON_CENTER:
							if(selectedSub == -1)
										selectedSub = 0;
							switch(settingSelected)
							{
								case SETTING_DIAMETER:
									screenStatus = MENU_STATUS_SETTINGS_DIAMETER;
								break;
								
								case SETTING_ETRTO:
									screenStatus = MENU_STATUS_SETTINGS_ETRTO;
								break;
								
								case SETTING_CADENCE:
									screenStatus = MENU_STATUS_SETTINGS_CADENCE;
								break;
								
								case SETTING_TIME:
									screenStatus = MENU_STATUS_SETTINGS_TIME;
								break;
								
								case SETTING_RESET:
									screenStatus = MENU_STATUS_SETTINGS_RESET;
								break;
								case SETTING_EXIT:
									screenStatus = MENU_STATUS_MAIN;
									buttonPressSubMenu = BUTTON_NONE;
									settingSelected = 0;
									screenStatus = screenStatusLast;
									TIM4_Cmd(ENABLE);
								break;

							}
							
					}
					
				break;
					
				case MENU_STATUS_SETTINGS_DIAMETER:
					switch(buttonPress)
						{
							case BUTTON_PREV:
							if(selectedSub != 2)
							{
								
								if(selectedSub == 0)
									selectedSub = 1;
								else
									selectedSub--;
							}
							else
							{
								user.diameter--;
								if(user.diameter < DIAMETERINCH_MIN)
									user.diameter = DIAMETERINCH_MAX;
							}
							break;
							
							case BUTTON_NEXT:
							if(selectedSub != 2)
							{
								if(selectedSub == 1)
									selectedSub = 0;
								else
									selectedSub++;
							}
							else
							{
								user.diameter++;
								if(user.diameter > DIAMETERINCH_MAX)
									user.diameter = DIAMETERINCH_MIN;
							}
							break;
							
							case BUTTON_CENTER:
								switch(selectedSub)
								{
									case 0:
										selectedSub = 2;
									break;
									
									case 1:
										screenStatus = MENU_STATUS_SETTINGS_MAIN;
									break;
									
									case 2:
										selectedSub = 0;
									break;
								}
								
						}
					break;
					
				case MENU_STATUS_SETTINGS_ETRTO:
					break;
					
				case MENU_STATUS_SETTINGS_CADENCE:
					switch(buttonPress)
						{
							case BUTTON_PREV:
							if(selectedSub != 2)
							{
								
								if(selectedSub == 0)
									selectedSub = 1;
								else
									selectedSub--;
							}
							else
							{
								user.cadence--;
								if(user.cadence < CADENCE_MIN)
									user.cadence = CADENCE_MAX;
							}
							break;
							
							case BUTTON_NEXT:
							if(selectedSub != 2)
							{
								if(selectedSub == 1)
									selectedSub = 0;
								else
									selectedSub++;
							}
							else
							{
								user.cadence++;
								if(user.cadence > CADENCE_MAX)
									user.cadence = CADENCE_MIN;
							}
							break;
							
							case BUTTON_CENTER:
								switch(selectedSub)
								{
									case 0:
										selectedSub = 2;
									break;
									
									case 1:
										screenStatus = MENU_STATUS_SETTINGS_MAIN;
									break;
									
									case 2:
										selectedSub = 0;
									break;
								}
								
						}
					break;
					
				case MENU_STATUS_SETTINGS_TIME:
					break;
					
				case MENU_STATUS_SETTINGS_RESET:
				switch(buttonPress)
						{
							case BUTTON_PREV:
								if(selectedSub == 0)
									selectedSub = 1;
								else
									selectedSub--;
							break;
							
							case BUTTON_NEXT:
								if(selectedSub == 1)
									selectedSub = 0;
								else
									selectedSub++;
							break;
							
							case BUTTON_CENTER:
								switch(selectedSub)
								{
									case 0:
										screenStatus = MENU_STATUS_SETTINGS_MAIN;
									break;
									
									case 1:
										user.diameter = DIAMETERINCH_MIN;
										user.cadence = CADENCE_MIN;
										screenStatus = MENU_STATUS_SETTINGS_MAIN;
									break;
								}
								
						}
					break;
			}
			buttonPress = BUTTON_NONE;
			
			needUpdate = TRUE;
			//Update();
			//needUpdate = FALSE;
		}
		
		if(countTimer > COUNT_OVF - 1) 
		{
			BMP280_Config(BMP280_Standby_MS1, BMP280_Filter_x2, BMP280_Sampling_x2, BMP280_Sampling_x16, BMP280_Mode_Forced);
			countTimer = 0;
			user.temp = (int16_t)BMP280_ReadTemp();
			user.alt = (int16_t)BMP280_ReadAlt(zeroPres);
			
			needUpdate = TRUE;
			//Update();
			
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
		
		if(needUpdate == TRUE)
		{
			needUpdate = FALSE;
			Update();
		}
			

	}
}
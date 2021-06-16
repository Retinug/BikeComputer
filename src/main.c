#include "main.h"

BUTTON_STATUS buttonPress = BUTTON_NONE;
BUTTON_STATUS buttonPressSubMenu = BUTTON_NONE;
DATA_USER user;
uint8_t countTimer = 0;
uint8_t updateStatus;
float zeroPres;
int32_t alt;
int16_t temp;
uint8_t timeOut;
uint16_t millis, millisWheel, millisCadence;


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
	if(millisWheel == 0)
	{
		millisWheel = millis;
		sbit(updateStatus, UPDATE_WHEEL);
	}
	else if(millis - millisWheel > 75 && !isbit(updateStatus, UPDATED_WHEEL))
	{
		millisWheel = millis - millisWheel;
		sbit(updateStatus, UPDATED_WHEEL);
	}
	
	EXTI_ClearITPendingBit(EXTI_IT_Pin4);
}

@far @interrupt void IRQ_Handler_EXTI_Reed_Pedal()
{
	if(millisCadence == 0)
	{
		millisCadence = millis;
		sbit(updateStatus, UPDATE_CADENCE);
	}
	else if(millis - millisCadence > 75 && !isbit(updateStatus, UPDATED_CADENCE))
	{
		millisCadence = millis - millisCadence;
		sbit(updateStatus, UPDATED_CADENCE);
	}
	
	EXTI_ClearITPendingBit(EXTI_IT_Pin7);
}

@far @interrupt void IRQ_Handler_EXTI_Tim3_Ovf()
{
	millis++;
	TIM3_ClearITPendingBit(TIM3_IT_Update);
}

@far @interrupt void IRQ_Handler_EXTI_Tim4_Ovf()
{
	if(countTimer++ > COUNT_OVF - 1)
	{
		TIM4_Cmd(DISABLE);
	}
	TIM4_ClearITPendingBit(TIM4_IT_Update);
}

void Sleep(void)
{
	CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_128);
	TIM3_Cmd(DISABLE);
	TIM4_Cmd(DISABLE);
	sbit(updateStatus, UPDATE_SLEEP);
	millis = 0;
	zeroPres = 0;
	SH1106_WriteCommand(SH1106_DISPLAYOFF);
	
}

void WakeUp(void)
{
	CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
	TIM3_Cmd(ENABLE);
	TIM4_Cmd(ENABLE);
	Update();
	SH1106_WriteCommand(SH1106_DISPLAYON);
	cbit(updateStatus, UPDATE_SLEEP);
}

void Init()
{
	disableInterrupts();
	CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
	
	CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);
	CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
	
	millis = 0; 
	millisWheel = 0;
	millisCadence = 0;
	updateStatus = 0;
	
	GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Mode_In_FL_IT);
	GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Mode_In_FL_IT);
	GPIO_Init(GPIOD, GPIO_Pin_3, GPIO_Mode_In_FL_IT);
	
	GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_In_FL_No_IT);
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

	CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
	TIM3_TimeBaseInit(TIM3_Prescaler_128, TIM3_CounterMode_Up, 125);
	TIM3_ClearFlag(TIM3_FLAG_Update);
	TIM3_ITConfig(TIM3_IT_Update, ENABLE);
	

	CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
	TIM4_TimeBaseInit(TIM4_Prescaler_32768, 244);
	TIM4_ClearFlag(TIM4_FLAG_Update);
	TIM4_ITConfig(TIM4_IT_Update, ENABLE);

	SH1106_Init();
	
	BMP280_Config(BMP280_Standby_MS1, BMP280_Filter_x2, BMP280_Sampling_x2, BMP280_Sampling_x16, BMP280_Mode_Forced);
	temp = BMP280_ReadTemp();
	
	sbit(updateStatus, UPDATE_SCREEN);
	
	user.speed = 0;
	user.speedMax = 0;
	user.speedAvg = 0;
	user.distance = 0;
	user.odometer = 0;
	user.cadence = 0;
	user.cadenceLimit = 0;
	user.diameter = 0;
	
	TIM3_Cmd(ENABLE);
	TIM4_Cmd(ENABLE);
}

void UpdateCalc()
{
	float mileage;
	TIM3_Cmd(DISABLE);
	disableInterrupts();
	if(isbit(updateStatus, UPDATED_CADENCE))
	{
		user.cadence = CALC_Cadence(millisCadence);
		millisCadence = 0;
		millis = 0;
	}
	
	if(isbit(updateStatus, UPDATED_WHEEL))
	{
		user.speed = CALC_Speed(millisWheel, user.diameter);
		
		if(user.speed > user.speedMax)
			user.speedMax = user.speed;
		if(user.speedAvg == 0)
			user.speedAvg = user.speed;
			
		mileage = CALC_Dist(user.speed);
		user.odometer += mileage;
		user.distance += mileage;
		
		millisWheel = 0;
		millis = 0;
		
		TimeInc(&user.timeR);
		TimeInc(&user.timeA);
	}
	
	enableInterrupts();
}

void TimeInc(TIME_CLOCK* time)
{
	if(time->hour > 99 && time->min > 59)
	{
		time->hour = 0;
		time->min = 0;
	}
	time->sec++;
	if(time->sec > 59)
	{
		user.speedAvg = (user.speedAvg + user.speed) / 2;
		time->sec = 0;
		time->min++;
		if(time->min > 59)
		{
			time->min = 0;
			time->hour++;
			if(time->hour > 99)
				time->hour = 0;
		}
	}
}

void Update()
{
	disableInterrupts();
	MENU_Change(screenStatus, buttonPressSubMenu, settingSelected);
	SH1106_Update();
	
	enableInterrupts();
	SH1106_Clear();	

	cbit(updateStatus, UPDATE_WHEEL);
	cbit(updateStatus, UPDATED_WHEEL);
	cbit(updateStatus, UPDATE_CADENCE);
	cbit(updateStatus, UPDATED_CADENCE);
	
	if(millis - millisWheel  > 2000)
		user.speed = 0;
	if(millis - millisCadence > 2000)
		user.cadence = 0;
	
	TIM3_Cmd(ENABLE);
}

void ResetRide()
{
	user.distance = 0;
	user.timeR.hour = 0;
	user.timeR.min = 0;
	user.timeR.sec = 0;
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
	if(user.cadenceLimit < CADENCE_MIN)
		user.cadenceLimit = CADENCE_MIN;
		
	enableInterrupts();
	screenStatus = MENU_STATUS_MAIN;
	countTimer = COUNT_OVF;
	Update();
	while (1)
	{
		if(millis > TIMEOUT_LIMIT)
			Sleep();
		
		if(isbit(updateStatus, UPDATE_SLEEP) && (isbit(updateStatus, UPDATE_WHEEL) || isbit(updateStatus, UPDATE_CADENCE)|| buttonPress))
		{
			WakeUp();
			buttonPress = BUTTON_NONE;
			millis = 0;
		}
		
		if(!isbit(updateStatus, UPDATE_SLEEP))
		{
			if(buttonPress)
			{
				buttonPressSubMenu = buttonPress;
				switch(screenStatus)
				{
					case MENU_STATUS_OFF:
						screenStatus = MENU_STATUS_MAIN;
						break;
				
					case MENU_STATUS_MAIN: //SETTINGS MAIN
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
						
					case MENU_STATUS_ADDITIONAL1: //SETTINGS ADD2
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
						
					case MENU_STATUS_ADDITIONAL2: //SETTINGS ADD2
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
						
					case MENU_STATUS_SETTINGS_MAIN: //SETTINGS MAIN
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
									
									case SETTING_CADENCE:
										screenStatus = MENU_STATUS_SETTINGS_CADENCE;
									break;
									
									case SETTING_RESETRIDE:
										screenStatus = MENU_STATUS_SETTINGS_RESETRIDE;
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
						
					case MENU_STATUS_SETTINGS_DIAMETER: //DIAMETER
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
						
					case MENU_STATUS_SETTINGS_CADENCE: //CADENCE
						switch(buttonPress)
						{
							case BUTTON_PREV:
								if(selectedSub == 0)
									selectedSub = 1;
								else if(selectedSub == 2)
								{
									if(user.cadenceLimit <= CADENCE_MIN)
									{
										user.cadenceLimit = CADENCE_MAX;
									}
									else
										user.cadenceLimit--;
								}
								else
									selectedSub--;
							break;
							
							case BUTTON_NEXT:
								if(selectedSub == 1)
									selectedSub = 0;
								else if(selectedSub == 2)
								{
									if(user.cadenceLimit >= CADENCE_MAX)
									{
										user.cadenceLimit = CADENCE_MIN;
									}
									else
										user.cadenceLimit++;
								}
								else
									selectedSub++;
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
						
					
					case MENU_STATUS_SETTINGS_RESETRIDE: //RESET RIDE
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
										ResetRide();
										screenStatus = MENU_STATUS_SETTINGS_MAIN;
									break;
								}
								
							}
							break;
						
					case MENU_STATUS_SETTINGS_RESET: // RESET
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
										ResetRide();
										user.odometer = 0;
										user.timeA.hour = 0;
										user.timeA.min = 0;
										user.timeA.sec = 0;
										user.speedAvg = 0;
										user.speedMax = 0;
										screenStatus = MENU_STATUS_SETTINGS_MAIN;
									break;
								}
								
						}
						break;
				}
				buttonPress = BUTTON_NONE;
				
				sbit(updateStatus, UPDATE_SCREEN);
			}
			
			if(isbit(updateStatus, UPDATE_SCREEN))
			{
				cbit(updateStatus, UPDATE_SCREEN);
				Update();
			}
			
			if(countTimer > COUNT_OVF - 1) 
			{
				disableInterrupts();
				BMP280_Config(BMP280_Standby_MS1, BMP280_Filter_x2, BMP280_Sampling_x2, BMP280_Sampling_x16, BMP280_Mode_Forced);
				
				countTimer = 0;
				temp = (int16_t)BMP280_ReadTemp();
				if(zeroPres == 0)
					zeroPres = BMP280_ReadPress();
				alt = BMP280_ReadAlt(zeroPres);
				enableInterrupts();
				
				sbit(updateStatus, UPDATE_SCREEN);
				UpdateCalc();
				
				TIM4_Cmd(ENABLE);
			}
			
			
		} 
	}
}
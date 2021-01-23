#include "main.h"

uint8_t x = 0;

void delay(uint32_t time)
{
	volatile uint32_t counter = 0;
	while(counter != time)
	{
		counter++;
	}
}

@far @interrupt void IRQ_Handler_EXTI_Button_1()
{
	SH1106_SetPixel(x++,20);
	EXTI_ClearITPendingBit(EXTI_IT_Pin1);
	return;
}

@far @interrupt void IRQ_Handler_EXTI_Button_2()
{
	SH1106_SetPixel(x++,30);
	EXTI_ClearITPendingBit(EXTI_IT_Pin2);
	return;
}

@far @interrupt void IRQ_Handler_EXTI_Button_3()
{
	SH1106_SetPixel(x++,40);
	EXTI_ClearITPendingBit(EXTI_IT_Pin3);
	return;
}

/*
void INTERRUPT_HANDLER(IRQ_Handler_EXTI_PORT_D, 7)
{
	SH1106_SetPixel(0,1);
	SH1106_Update();
}
*/

void Init()
{
	CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
	
	GPIO_Init(BUTTON_PORT, GPIO_Pin_1, GPIO_Mode_In_FL_IT);
	GPIO_Init(BUTTON_PORT, GPIO_Pin_2, GPIO_Mode_In_FL_IT);
	GPIO_Init(BUTTON_PORT, GPIO_Pin_3, GPIO_Mode_In_FL_IT);
	
	EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling);
	EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Falling);
	EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Falling);
	
	CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);
	I2C_Init(I2C1, I2C_MAX_FAST_FREQ, 0x05, I2C_Mode_I2C, I2C_DutyCycle_2, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);
	I2C_Cmd(I2C1, ENABLE);

	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	SH1106_Init();
	
	BMP280_Config(BMP280_Standby_MS1, BMP280_Filter_x2, BMP280_Sampling_x16, BMP280_Sampling_x16, BMP280_Mode_Forced);
}

int main()
{	
	int i = 0, j = 0;
	delay(100000);
	
	Init();
	
	SH1106_WriteCommand(SH1106_DISPLAYOFF);
	
	SH1106_DrawScreenLines();
	SH1106_Update();
	SH1106_WriteCommand(SH1106_DISPLAYON);
	
	
	enableInterrupts();
	
	BMP280_ReadTemp();
	BMP280_ReadPress();
	while (1)
	{
		/*
		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))
		{
			j = 20;
		}
		else if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2))
		{
			j = 40;
		}
		
		else if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3))
		{
			j = 50;
		}
		*/
		
		SH1106_SetPixel(i,j);
		i++;

		//SH1106_WriteCommand(SH1106_DISPLAYOFF);	
		if(i > 127)
		{
			j++;
			i = 0;
		}
		
		SH1106_Update();
		delay(500);
		
		
		
	}
}
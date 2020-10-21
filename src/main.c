#include "stm8l15x.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_i2c.h"
#include "stm8l15x_exti.h"

#include "BMP280.h"

//#define DEBUG

uint8_t TEMPdata1;
uint8_t TEMPdata2;
uint8_t TEMPdata3;
uint8_t TEMPdata4;
uint8_t TEMPdata5;

uint8_t address1;
uint8_t address2;

ITStatus status;
I2C_Event_TypeDef event;
I2C_FLAG_TypeDef flag;



void delay(uint32_t time)
{
	volatile uint32_t counter = 0;
	while(counter != 100000)
	{
		counter++;
	}
	/*
	for(counter = 0; counter < time; counter++)
	{
		#asm
		nop
		#endasm
	}
	*/
}

main()
{
	uint8_t config = 0;
	uint8_t meas = 0;
	int i;
	volatile uint8_t d, b = 0;
	volatile temp;
	I2C_Event_TypeDef event;
	FlagStatus status;
	
	delay(100000);
	
	CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
	
	CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);
	I2C_Init(I2C1, I2C_MAX_STANDARD_FREQ, 0x05, I2C_Mode_I2C, I2C_DutyCycle_2, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);
	//I2C_AcknowledgeConfig(I2C1, ENABLE);
	//I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);
	I2C_Cmd(I2C1, ENABLE);

	//I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	config = (BMP280_Standby_MS1 << 5) | (BMP280_Filter_x2 << 2) | 0;
	meas = (BMP280_Sampling_x16 << 5) | (BMP280_Sampling_x16 << 2) | BMP280_Mode_Normal;
	
	while (1)
	{
		delay(500);
		
		//meas = (BMP280_Sampling_x16 << 5);// | (BMP280_Sampling_x16 << 2) | 11;
		BMP280_WriteReg(BMP280_REG_CONTROL, meas);
		BMP280_WriteReg(BMP280_REG_CONFIG, config);
		BMP280_ReadCalibration();
		BMP280_ReadTemp();
		BMP280_Read_24(BMP280_REG_TEMPDATA);
		
		d = BMP280_Read_8(BMP280_REG_CONFIG);
		b = BMP280_Read_8(BMP280_REG_CONTROL);
		
		
		/*
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
		I2C_GenerateSTART(I2C1, ENABLE);
		
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
		I2C_Send7bitAddress(I2C1, BMP280_ADDRESS << 1, I2C_Direction_Transmitter);
		
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
		I2C_SendData(I2C1, BMP280_REG_PRESSUREDATA);
		
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		I2C_GenerateSTART(I2C1, ENABLE); ////////////////////////////////////////////////////////////
		
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
		I2C_Send7bitAddress(I2C1, BMP280_ADDRESS << 1, I2C_Direction_Receiver);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
		
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
		I2C_AcknowledgeConfig(I2C1, DISABLE);
		TEMPdata1 = I2C_ReceiveData(I2C1);
		
		I2C_GenerateSTOP(I2C1, ENABLE);
		TEMPdata2 = I2C_ReceiveData(I2C1);
		
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
		TEMPdata3 = I2C_ReceiveData(I2C1);
		I2C_AcknowledgeConfig(I2C1, ENABLE);
		//////////////
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
		TEMPdata1 = I2C_ReceiveData(I2C1);
		
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
		TEMPdata2 = I2C_ReceiveData(I2C1);
		I2C_AckPositionConfig(I2C1, I2C_AckPosition_Next);
		
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
		TEMPdata3 = I2C_ReceiveData(I2C1);
		
		I2C_GenerateSTOP(I2C1, ENABLE);
		
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
		TEMPdata4 = I2C_ReceiveData(I2C1);

		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
		TEMPdata5 = I2C_ReceiveData(I2C1);
		*/
		//delay(500);
	}
}
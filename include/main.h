#ifndef MAIN_H
#define MAIN_H

#include "stm8l15x.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_i2c.h"
#include "stm8l15x_exti.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_tim4.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_adc.h"

#include "BMP280.h"
#include "SH1106.h"

#include "calc.h"
#include "global.h"
#include "menu.h"
#include "macros.h"

#define BUTTON_PIN1 1
#define BUTTON_PIN2 2
#define BUTTON_PIN3 3

#define REED_PEDAL_PORT GPIOB
#define REED_PEDAL_PIN 7
#define REED_WHEEL_PORT GPIOD
#define REED_WHEEL_PIN 4

#define COUNT_OVF 2

#define UPDATE_SCREEN   0
#define UPDATE_WHEEL    1
#define UPDATED_CADENCE 2
#define UPDATED_WHEEL   3
#define UPDATE_CADENCE  4
#define UPDATE_SLEEP    5

#define TIMEOUT_LIMIT 60000

extern BUTTON_STATUS buttonPress;
extern BUTTON_STATUS buttonPressSubMenu;
extern int32_t alt;
extern int16_t temp;
extern uint8_t adc;

void Init(void);

@far @interrupt void IRQ_Handler_EXTI_Button_1(void);
@far @interrupt void IRQ_Handler_EXTI_Button_2(void);
@far @interrupt void IRQ_Handler_EXTI_Button_3(void);

@far @interrupt void IRQ_Handler_EXTI_Reed_Wheel(void);
@far @interrupt void IRQ_Handler_EXTI_Reed_Pedal(void);

@far @interrupt void IRQ_Handler_EXTI_Tim3_Ovf(void);
@far @interrupt void IRQ_Handler_EXTI_Tim4_Ovf(void);

void TimeInc(TIME_CLOCK* time);
void Update(void);
void UpdateCalc(void);
void Sleep(void);
void WakeUp(void);
void ResetRide(void);
#endif
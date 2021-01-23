#ifndef MAIN_H
#define MAIN_H

#include "stm8l15x.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_i2c.h"
#include "stm8l15x_exti.h"
#include "stm8l15x_gpio.h"

#include "BMP280.h"
#include "SH1106.h"

#define BUTTON_PORT GPIOD
#define BUTTON_PIN1 1
#define BUTTON_PIN2 2
#define BUTTON_PIN3 3

#define REED_PEDAL_PORT GPIOB
#define REED_PEDAL_PIN 7
#define REED_WHEEL_PORT GPIOD
#define REED_WHEEL_PIN 4

void Init(void);

@far @interrupt void IRQ_Handler_EXTI_Button_1(void);
@far @interrupt void IRQ_Handler_EXTI_Button_2(void);
@far @interrupt void IRQ_Handler_EXTI_Button_3(void);

#endif
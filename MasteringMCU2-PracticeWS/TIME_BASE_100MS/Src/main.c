/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - Mastering Microcontroller  TIMERS, PWM, CAN, RTC,LOW POWER
 *  GOAL: using timer6 basic timer to generate interrupt at 100ms
 *  	blinking LED using TIM6 IRQ
 *  	verify using logic analyzer
 *
 *		Chap7Vid5 -> Chap7Vid11
 */
#include "stm32l4xx_hal_conf.h"
#include "stm32l4r5xx.h"
#include "main.h"
#include <string.h>

void SystemClockConfig(void);
void Error_handler(void);
void Timer6_Init (void);
void GPIO_Init (void);
void UART3_Init(void);

TIM_HandleTypeDef htimer6;
UART_HandleTypeDef huart3;
int main(void)
{
	HAL_Init(); // init HAL
	SystemClockConfig(); // config system clock if needed
	Timer6_Init();
	GPIO_Init();

	// start the TIMER6
	HAL_TIM_Base_Start(&htimer6);
	while(1)
	{
		while(! (TIM6->SR & TIM_SR_UIF)); // while no flag -> stay here
		TIM6->SR = 0;
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);
	}

	while(1);
	return 0;
}

void SystemClockConfig(void)
{
	RCC_OscInitTypeDef Osc_Init_Param = {0};
	RCC_ClkInitTypeDef Clk_Init_Param = {0};

	Osc_Init_Param.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	Osc_Init_Param.HSIState = RCC_HSI_ON;
	Osc_Init_Param.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

	if (HAL_RCC_OscConfig(&Osc_Init_Param) != HAL_OK)
	{
		Error_handler();
	}

	Clk_Init_Param.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	Clk_Init_Param.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
								RCC_CLOCKTYPE_PCLK1	| RCC_CLOCKTYPE_PCLK2;
	Clk_Init_Param.AHBCLKDivider = RCC_SYSCLK_DIV1;
	Clk_Init_Param.APB1CLKDivider = RCC_HCLK_DIV2;
	Clk_Init_Param.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&Clk_Init_Param, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_handler();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}
void Error_handler(void)
{
	while(1);
}

void Timer6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 24;
	htimer6.Init.Period = 64000-1; // 63999 tick + 1 tick for generate update event
	if (HAL_TIM_Base_Init(&htimer6) != HAL_OK)
	{
		Error_handler();
	}
}
void GPIO_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_14;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB,&ledgpio);

}
void UART3_Init (void)
{
	huart3.Instance = USART3; 					// Peripheral high level initialization chap5vid6
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.Mode =  UART_MODE_TX_RX;		// Peripheral high level initialization chap5vid6
	if ( HAL_UART_Init(&huart3)!=HAL_OK)
	{
		Error_handler();
	}
}

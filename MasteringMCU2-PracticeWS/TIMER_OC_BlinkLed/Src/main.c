/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - Mastering Microcontroller  TIMERS, PWM, CAN, RTC,LOW POWER
 *  GOAL: using timer2 general timer and Input Compare function to measure frequency
 *  	from 32Khz LSE - which is config as osc output MCO1 - pin PA8
 *  	Timer2 using channel 1 as input capture - PA0
 *  	verify using logic analyzer
 *		Chap8vid1 -> Chap8vid11
 */
#include "stm32l4xx_hal_conf.h"
#include "stm32l4r5xx.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

void System_Clk_Config(uint8_t clock_freq);
void Error_handler(void);
void Timer15_Init(void);

TIM_HandleTypeDef htimer15;
UART_HandleTypeDef huart3;

uint32_t pulse1_value = 25000; //0.5hz output/ 1s pulse -- f count timer = 25Khz
uint32_t ccr_value =0;
int main(void)
{
	HAL_Init(); // init HAL
	System_Clk_Config(SYS_CLOCK_FREQ_50MHZ); // config sysclk and tim_clk = 50Mhz
	Timer15_Init();
	if (HAL_TIM_OC_Start_IT(&htimer15,TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}
	while(1);
	return 0;
}

void System_Clk_Config(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;	//chap6vid4
	RCC_ClkInitTypeDef clk_init;	//chap6vid4
	uint8_t FlashLatency = 0;

	memset(&osc_init,0,sizeof(osc_init));
	memset(&clk_init,0,sizeof(clk_init));

	/* Create param structure for OSCONFIG & CLKCONFIG */
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
	osc_init.LSEState = RCC_LSE_ON;;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource =RCC_PLLSOURCE_HSI;

	switch(clock_freq)
	{
		case SYS_CLOCK_FREQ_50MHZ :
		{
			/*Init param struct to config PLL */
			osc_init.PLL.PLLM = 4;
			osc_init.PLL.PLLN = 50;
			osc_init.PLL.PLLR = RCC_PLLR_DIV4;
			osc_init.PLL.PLLQ = RCC_PLLR_DIV2; //default
			osc_init.PLL.PLLP = RCC_PLLR_DIV2; //default

			/*Init param struct to config bus clock*/
			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
								RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			/*Adjust flash latency for desired frequency*/
			FlashLatency =  FLASH_ACR_LATENCY_2WS;
			break;
		}
		case SYS_CLOCK_FREQ_84MHZ :
		{
			/*Init param struct to config PLL*/
			osc_init.PLL.PLLM = 4;
			osc_init.PLL.PLLN = 42;
			osc_init.PLL.PLLR = RCC_PLLR_DIV2;
			osc_init.PLL.PLLQ = RCC_PLLR_DIV2; //default
			osc_init.PLL.PLLP = RCC_PLLR_DIV2; //default
			/*Init param struct to config bus clock*/
			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
								RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			/*Adjust flash latency for desired frequency*/
			FlashLatency = FLASH_ACR_LATENCY_4WS;
			break;
		}
		case SYS_CLOCK_FREQ_120MHZ :
		{
			/*Init param struct to config PLL */
			osc_init.PLL.PLLM = 4;
			osc_init.PLL.PLLN = 60;
			osc_init.PLL.PLLR = RCC_PLLR_DIV2;
			osc_init.PLL.PLLQ = RCC_PLLR_DIV2; //default
			osc_init.PLL.PLLP = RCC_PLLR_DIV2; //default

			/*Init param struct to config bus clock*/
			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
								RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			/*Adjust flash latency for desired frequency*/
			FlashLatency = FLASH_ACR_LATENCY_5WS;
			break;
		}
		default :
			break;
	}

	/*Parsing param into config functions*/
	if (HAL_RCC_OscConfig(&osc_init) != HAL_OK)	//chap6vid4
	{
		Error_handler();
	}

	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_LSE,RCC_MCODIV_1 ); // Output selected clk to GPIO pin PA8
	// pin configuration is took care by this function also - Chap8vid13

	if (HAL_RCC_ClockConfig(&clk_init,FlashLatency) != HAL_OK)
	{
		Error_handler(); // FLASH LATENCY IS IMPORTANT - later half chap6vid5
	}

	/* AFTER THIS POINT, SYSTEM USE HSI 16Mhz AS SYSCLK */
	/* As HCLK drive SYSTEM TIMER & we want to config interrupt after every 1ms = 1/1000 s
	 *
	 * 1 tick		   = 1 / HCLK	(sec)
	 * ? tick		   = 1/1000 	(sec)
	 * => number of tick = HCLK/1000
	 *
	 * */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000); // param is number of tick to archive 1ms between 2 systick interrupt

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); // set prescale value for System timer clock
														// HCLK or HCLK_DIV8


}
void Error_handler(void)
{
	while(1);
}

void Timer15_Init(void)
{
	htimer15.Instance = TIM15;
	htimer15.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer15.Init.Prescaler = 1999; // 50Mhz / 2000 = 25Khz timer count frequency
	htimer15.Init.Period = 0xFFFF; // TIM15 16bits timer
	if (HAL_TIM_OC_Init(&htimer15) != HAL_OK)	// Enable timer input capture time based
	{
		Error_handler();
	}

	TIM_BreakDeadTimeConfigTypeDef brk_Config;
	brk_Config.OffStateRunMode = TIM_OSSR_DISABLE;
	HAL_TIMEx_ConfigBreakDeadTime(&htimer15,&brk_Config);

	TIM_OC_InitTypeDef timer15OC_Config;
	timer15OC_Config.OCMode = TIM_OCMODE_TOGGLE;
	timer15OC_Config.Pulse = pulse1_value; // 1s trigger with 1Khz timer count frequency
	timer15OC_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
	if (HAL_TIM_OC_ConfigChannel(&htimer15,&timer15OC_Config,TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}
}
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
 	ccr_value = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1, ccr_value+pulse1_value);
}

/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - Mastering Microcontroller  TIMERS, PWM, CAN, RTC,LOW POWER
 *  GOAL: using timer6 basic timer to generate interrupt at 10 micro second
 *  	blinking LED using TIM6 IRQ
 *  	verify using logic analyzer
 *		Chap7Vid13
 */
#include "stm32l4xx_hal_conf.h"
#include "stm32l4r5xx.h"
#include "main.h"
#include <string.h>

void System_Clk_Config(uint8_t clock_freq);
void Error_handler(void);
void Timer6_Init (void);
void GPIO_Init (void);

TIM_HandleTypeDef htimer6;
UART_HandleTypeDef huart3;
int main(void)
{
	HAL_Init(); // init HAL
	System_Clk_Config(SYS_CLOCK_FREQ_50MHZ); // config sysclk and tim_clk = 50Mhz
	Timer6_Init();
	GPIO_Init();

	// start the TIMER6
	HAL_TIM_Base_Start_IT(&htimer6);

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
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
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
			clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

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

	if (HAL_RCC_ClockConfig(&clk_init,FlashLatency) != HAL_OK)
	{
		Error_handler(); // FLASH LATENCY IS IMPORTANT - later half chap6vid5
	}

	/* AFTER THIS POINT, SYSTEM USE HSI 16Mhz AS SYSCLK */

	//__HAL_RCC_HSI_DISABLE();	//disable HSI to save power - chap6vid5

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

void Timer6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 0; // prescale = param +1 => prescaler here =1
	htimer6.Init.Period = 500-1;
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

/*As the basic timer does not have a lot functions
 * so we only choose the update event handler
 * which call back "HAL_TIM_PeriodElapsedCallback"
 * inside the TIM_IRQhandler
 *
 * */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
}

/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - Mastering Microcontroller  TIMERS, PWM, CAN, RTC,LOW POWER
 *  GOAL: using TIM2 OC PWM function to generate PWM signal for 4 channel - 25%,45%,75%,90%
 *  	verify using logic analyzer
 *		Chap10vid3 -> Chap10vid8
 */
#include "stm32l4xx_hal_conf.h"
#include "stm32l4r5xx.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

void System_Clk_Config(uint8_t clock_freq);
void Error_handler(void);
void Timer2_Init(void);
void UART3_Init (void);

TIM_HandleTypeDef htimer2;
UART_HandleTypeDef huart3;

uint32_t ccr_content = 0;
uint32_t pulse1_value = 25000; //500hz output -- f count timer = 25Mhz
uint32_t pulse2_value = 12500; //1Khz output -- f count timer = 25Mhz
uint32_t pulse3_value = 6250; //2Khz output -- f count timer = 25Mhz
uint32_t pulse4_value = 3125; //4Khz output -- f count timer = 25Mhz

char mesg[100];
int main(void)
{
	HAL_Init(); // init HAL
	System_Clk_Config(SYS_CLOCK_FREQ_50MHZ); // config sysclk and tim_clk = 50Mhz
	Timer2_Init();
	UART3_Init();

	memset(mesg, 0, sizeof(mesg)); // reset char array;
	if (HAL_TIM_PWM_Start(&htimer2 , TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}
	if (HAL_TIM_PWM_Start(&htimer2 , TIM_CHANNEL_2) != HAL_OK)
	{
		Error_handler();
	}
	if (HAL_TIM_PWM_Start(&htimer2 , TIM_CHANNEL_3) != HAL_OK)
	{
		Error_handler();
	}
	if (HAL_TIM_PWM_Start(&htimer2 , TIM_CHANNEL_4 ) != HAL_OK)
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

void Timer2_Init(void)
{
	memset(&htimer2, 0, sizeof(htimer2));
	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Prescaler = 4999; // TIM clk = 50Mhz/5000 = 10Khz
	htimer2.Init.Period = 0x270F; // count to 9999+1 to make period 1s
	if (HAL_TIM_PWM_Init(&htimer2) != HAL_OK)	// Enable timer input capture time based
	{
		Error_handler();
	}

	TIM_OC_InitTypeDef timer2OC_PWM_Config;
	memset(&timer2OC_PWM_Config, 0, sizeof(timer2OC_PWM_Config));
	timer2OC_PWM_Config.OCMode = TIM_OCMODE_PWM1;
	timer2OC_PWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
	timer2OC_PWM_Config.Pulse = htimer2.Init.Period*25/100;
	timer2OC_PWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH; // Chap9vid3 - 10m00s
	if (HAL_TIM_PWM_ConfigChannel(&htimer2, &timer2OC_PWM_Config, TIM_CHANNEL_1) !=HAL_OK)
	{
		Error_handler();
	}
	timer2OC_PWM_Config.Pulse = htimer2.Init.Period*45/100;
	if (HAL_TIM_PWM_ConfigChannel(&htimer2, &timer2OC_PWM_Config, TIM_CHANNEL_2) !=HAL_OK)
	{
		Error_handler();
	}
	timer2OC_PWM_Config.Pulse = htimer2.Init.Period*75/100;
	if (HAL_TIM_PWM_ConfigChannel(&htimer2, &timer2OC_PWM_Config, TIM_CHANNEL_3) !=HAL_OK)
	{
		Error_handler();
	}
	timer2OC_PWM_Config.Pulse = htimer2.Init.Period*90 /100;
	if (HAL_TIM_PWM_ConfigChannel(&htimer2, &timer2OC_PWM_Config, TIM_CHANNEL_4) !=HAL_OK)
	{
		Error_handler();
	}

}

void UART3_Init(void)
{
	huart3.Instance = USART3; 					// Peripheral high level initialization chap5vid6
	huart3.Init.BaudRate = 9600;
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


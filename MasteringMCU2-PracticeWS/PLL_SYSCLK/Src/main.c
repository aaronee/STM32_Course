/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - Mastering Microcontroller  TIMERS, PWM, CAN, RTC,LOW POWER
 *		Chap6Vid010 -> Chap6Vid11
 *		Flash corrupt issue after reset
 *		https://community.st.com/s/question/0D50X00009XkZTeSAN/powering-cycling-stm32f4-board-corrupts-flash-programming
 *		https://stackoverflow.com/questions/22595175/flash-is-corrupting-after-a-couple-of-immediate-resets
 *
 */
#include "stm32l4xx_hal_conf.h"
#include "stm32l4r5xx.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
/*----------------------------------------------------------------*/

#define TRUE 1
#define FALSE 0
/*----------------------------------------------------------------*/
void System_Clk_Config(uint8_t clock_freq);
void UART3_Init(void);
void Error_Handler(void);
/*----------------------------------------------------------------*/

UART_HandleTypeDef huart3;
/*----------------------------------------------------------------*/

int main(void)
{
	char msg[17];
	char msg1[50] = {'t','h','i','s',' ','i','s',' ','t','e','s','t'};
	uint8_t clock_freq = 0 ;
	HAL_Init();	//Initialize HAL abstract layer
	System_Clk_Config(SYS_CLOCK_FREQ_50MHZ);
//	System_Clk_Config(SYS_CLOCK_FREQ_84MHZ);
//	System_Clk_Config(SYS_CLOCK_FREQ_120MHZ);

	UART3_Init(); // init USART after init clock to ensure the baud rate

	/*Debug using printf and USART3*/
	memset(msg,0,sizeof(msg));
	sprintf(msg,"SYSCLK: %lu \r\n",HAL_RCC_GetSysClockFreq());
	//SPRINTF print the message line to string "msg", before useit to be transmitted
	HAL_UART_Transmit(&huart3, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"HCLK: %lu \r\n",HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart3, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK1: %lu \r\n",HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart3, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK2: %lu \r\n",HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart3, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

	while(1)
	{
//		HAL_Delay(500);
		HAL_UART_Transmit(&huart3, (uint8_t *) msg1, strlen(msg1), HAL_MAX_DELAY);
	}

		return 0;
}
/*----------------------------------------------------------------*/

void UART3_Init(void)
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
		Error_Handler();
	}
}
/*----------------------------------------------------------------*/
/* Chap6vid10 -> Chap6vid11*/

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
		Error_Handler();
	}

	if (HAL_RCC_ClockConfig(&clk_init,FlashLatency) != HAL_OK)
	{
		Error_Handler(); // FLASH LATENCY IS IMPORTANT - later half chap6vid5
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
/*----------------------------------------------------------------*/
void Error_Handler(void)
{
 while(1);
}



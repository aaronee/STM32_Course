/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - Mastering Microcontroller  TIMERS, PWM, CAN, RTC,LOW POWER
 *		Chap6Vid03 -> Chap6Vid9
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

void UART3_Init(void);
void Error_Handler(void);
/*----------------------------------------------------------------*/

UART_HandleTypeDef huart3;
/*----------------------------------------------------------------*/

int main(void)
{
	RCC_OscInitTypeDef osc_init;	//chap6vid4
	RCC_ClkInitTypeDef clk_init;	//chap6vid4
	char msg[17];

	HAL_Init();	//Initialize HAL abstract layer

	/* Create argument structure for OSCCONFIG */
	memset(&osc_init,0,sizeof(osc_init)); // set value of a block of memory

//	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;	//chap6vid4
//	osc_init.HSEState = RCC_HSE_BYPASS;	//chap6vid4
//	osc_init.HSEState = RCC_HSE_BYPASS;	//chap6vid4

	/* using HSI instead HSE because board MB1312A
	 * does not have MCO connect to STM32L4
	 */
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	osc_init.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&osc_init) != HAL_OK)	//chap6vid4
	{
		Error_Handler();
	}


	/* Create argument structure for CLOCKCONFIG */
	memset(&clk_init,0,sizeof(clk_init));	//chap6vid5
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;	//chap6vid5
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;	//chap6vid5
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV4;	//chap6vid5
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;	//chap6vid5
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;	//chap6vid5
	if (HAL_RCC_ClockConfig(&clk_init,FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		Error_Handler(); // FLASH LATENCY IS IMPORTANT - later half chap6vid5
	}

	/* AFTER THIS POINT, SYSTEM USE HSI 16Mhz AS SYSCLK */

	//__HAL_RCC_HSI_DISABLE();	//disable HSI to save power - chap6vid5
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000); // param is number of tick to archive 1ms between 2 systick interrupt
													// as HCLK is 4Mhz => numbers of tick to be 1ms = 4Mhz/1000
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); // set prescale value for System timer clock
														// HCLK or HCLK_DIV8

	UART3_Init(); // init USART after init clock to ensure the baudrate

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
		HAL_Delay(500);
		HAL_UART_Transmit(&huart3, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);
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
void Error_Handler(void)
{
 while(1);
}



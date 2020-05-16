/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - Mastering Microcontroller  TIMERS, PWM, CAN, RTC,LOW POWER
 *		Chap4Vid7 -> Chap5Vid14
 */
#include "stm32l4xx_hal_conf.h"
#include "stm32l4r5xx.h"
#include "main.h"
#include <string.h>

void SystemClockConfig(void);
void UART3_Init(void);
void Error_handler(void);

char *my_mes = "this is USART3 PD8 onboard \n"; //chap5-vid12

UART_HandleTypeDef huart3;
int main(void)
{
	HAL_Init(); // init HAL abstract layer
	SystemClockConfig(); // config system clock if needed
	UART3_Init();

	uint16_t len_of_data = strlen(my_mes); //chap5-vid12
	while(1)
	{
		HAL_UART_Transmit(&huart3,(uint8_t*) my_mes, len_of_data, HAL_MAX_DELAY);
//	if (HAL_UART_Transmit(&huart3,(uint8_t*) my_mes, len_of_data, HAL_MAX_DELAY) != HAL_OK)
//	{
//		Error_handler();
//	}//chap5-vid12
	}
	return 0;
}

void SystemClockConfig(void)
{

}
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
		Error_handler();
	}
}
void Error_handler(void)
{

}

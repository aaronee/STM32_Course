/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - Mastering Microcontroller  TIMERS, PWM, CAN, RTC,LOW POWER
 *		Chap5Vid14 -> Chap5Vid15
 */
#include "stm32l4xx_hal_conf.h"
#include "stm32l4r5xx.h"
#include "main.h"
#include <string.h>
/*----------------------------------------------------------------*/

#define TRUE 1
#define FALSE 0
/*----------------------------------------------------------------*/

void SystemClockConfig(void);
void UART3_Init(void);
void Error_handler(void);
uint8_t CapitalConvert(uint8_t);
/*----------------------------------------------------------------*/

uint8_t mes_complete = FALSE;
uint8_t rev_buffer[100];
char *my_mes = "the software is running \n"; //chap5-vid12
uint8_t rev_data;
UART_HandleTypeDef huart3;
uint8_t count =0;
/*----------------------------------------------------------------*/

int main(void)
{
	HAL_Init(); //init HAL abstract layer
	SystemClockConfig(); // config system clock if needed
	UART3_Init();
	uint16_t len_of_mymess = strlen(my_mes);
	HAL_UART_Transmit(&huart3,(uint8_t *)my_mes,len_of_mymess, HAL_MAX_DELAY);
//	while(mes_complete != TRUE)
//	{
		HAL_UART_Receive_IT(&huart3,&rev_data,1);//chap5vid16
		/* Configure USART3 to receive data in IT mode for the 1st byte
		 * after 1st data has been received, the decision on continue
		 * or stop receiving next data will be handle in
		 * Callback function below - HAL_UART_RxCpltCallback
		 * This function is apart of USART3 ISR started in "it.c"
		 *
		 */
//	}
	while(1);

		return 0;
}
/*----------------------------------------------------------------*/
void SystemClockConfig(void)
{

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
		Error_handler();
	}
}
/*----------------------------------------------------------------*/
void Error_handler(void)
{

}
/*----------------------------------------------------------------*/
uint8_t CapitalConvert(uint8_t data) //chap5vid15
{
	if (data >= 'a' && data <= 'z')
	{
		data -= 'a'-'A';
	}
	return data;
}
/*----------------------------------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) //chap5vid18
{
	rev_buffer[count]=rev_data;
	if (rev_data == '\r')
	{
		mes_complete =TRUE;
		rev_buffer[count++]='\r';
		HAL_UART_Transmit(&huart3,rev_buffer,count,HAL_MAX_DELAY);
	}
	else
	{
		count++;
	}
	if(mes_complete !=TRUE)
	{
		HAL_UART_Receive_IT(&huart3,&rev_data,1);
		/* If the message have not been fully received, continue to
		 * set USART to receive data in IT mode, else it will not
		 * receive data anymore !!!
		 * THIS IS MODIFY CODE from the code in LECTURE VIDEO
		*/
	}
}


/*
 * it.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 */
#include "main.h"
extern UART_HandleTypeDef huart3; //chap5vid13

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

}
void USART3_IRQHandler(void) // chap5vid17
{
	HAL_UART_IRQHandler(&huart3);
}


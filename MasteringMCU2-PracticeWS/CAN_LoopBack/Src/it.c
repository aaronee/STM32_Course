/*
 * it.c
 *
 *  Created on: Mar. 5, 2020
 *      Author: khoahuynh
 */
#include "main.h"
extern UART_InitTypeDef huart3;
extern CAN_HandleTypeDef hcan1;

/*Set Up Systick timer interrupt*/
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void USART3_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart3);
}

void CAN1_TX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan1);
}

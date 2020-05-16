/*
 * msp.c
 *
 *  Created on: Mar. 5, 2020
 *      Author: khoahuynh
 */

#include "main.h"
/*System MSP init */
void HAL_MspInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	SCB->SHCSR |= 0x07 << 16; // Enable usagefault-memoryfault-busfault system exceptions- chap5vid5

	HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
}

void HAL_UART_MspInit (UART_HandleTypeDef *huart)
{
	//Enable CLK in RCC
	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	//Init GPIO
	GPIO_InitTypeDef UART3IO_Init;
	UART3IO_Init.Pin = GPIO_PIN_8;
	UART3IO_Init.Mode = GPIO_MODE_AF_PP;
	UART3IO_Init.Pull = GPIO_NOPULL;
	UART3IO_Init.Speed = GPIO_SPEED_FREQ_LOW;
	UART3IO_Init.Alternate = GPIO_AF7_USART3; // USART3 TX - PD8
	HAL_GPIO_Init(GPIOD, &UART3IO_Init);

	UART3IO_Init.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOD, &UART3IO_Init);

	//Init UART3 interrupt
	HAL_NVIC_EnableIRQ(USART3_IRQn);
	HAL_NVIC_SetPriority(USART3_IRQn,15,0);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	// Init clk for CAN and CANIO
	__HAL_RCC_CAN1_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	// Init CANIO
	GPIO_InitTypeDef CAN1IO;
	CAN1IO.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	CAN1IO.Mode = GPIO_MODE_AF_PP;
	CAN1IO.Alternate = GPIO_AF9_CAN1;
	CAN1IO.Pull = GPIO_NOPULL;
	CAN1IO.Speed = GPIO_SPEED_FREQ_HIGH; // for 25Mhz -> 50Mhz
	HAL_GPIO_Init(GPIOD,&CAN1IO);
	//Init CAN interrupt NVIC
	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	HAL_NVIC_SetPriority(CAN1_TX_IRQn,15,0);

}

/*
 * msp.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 */
#include "stm32l4xx_hal.h"
#include "main.h"

extern DMA_HandleTypeDef hDMA1_M2P_stream ;

void HAL_MspInit(void)
{
	// HERE WE DO LOW LEVEL PROCESSOR SPECIFIC INIT - chap5vid3
	//1. Set up the priority grouping of the ARM cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); // set interrupt priority grouping - chap5vid4
	//2. Enable the required system exceptions of the ARM cortex mx processor
	SCB->SHCSR |= 0x07 << 16; // Enable usagefault-memoryfault-busfault system exceptions- chap5vid5
	//3. Set up the priority if the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);// - chap5vid5
	HAL_NVIC_SetPriority(BusFault_IRQn,0,0);// - chap5vid5
	HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);// - chap5vid5

}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if(huart->Instance == LPUART1)
	{
		// enable power management peripheral
		__HAL_RCC_PWR_CLK_ENABLE();
		// enable VDDIO2 using for lpuart1
		__HAL_PWR_VDDIO2_ENABLE();
		// enable lpuart1 and its io
		__HAL_RCC_LPUART1_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();

		GPIO_InitTypeDef lpuart1_io = {0};
		lpuart1_io.Pin = GPIO_PIN_7 | GPIO_PIN_8;
		lpuart1_io.Mode = GPIO_MODE_AF_PP;
		lpuart1_io.Pull = GPIO_NOPULL;
		lpuart1_io.Speed = GPIO_SPEED_LOW;
		lpuart1_io.Alternate = GPIO_AF8_LPUART1;

		HAL_GPIO_Init(GPIOG,&lpuart1_io);

		// link DMA
	    __HAL_LINKDMA(huart,hdmatx,hDMA1_M2P_stream);

		// enable lpuart1 nvic
		HAL_NVIC_EnableIRQ(LPUART1_IRQn);
		HAL_NVIC_SetPriority(LPUART1_IRQn,15,0);
	}
}

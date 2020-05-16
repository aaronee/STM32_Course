/*
 * msp.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 */
#include "main.h"

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
	if (huart->Instance == LPUART1)
	{
		__HAL_RCC_GPIOG_CLK_ENABLE();
		__HAL_RCC_PWR_CLK_ENABLE();
	    HAL_PWREx_EnableVddIO2();
		GPIO_InitTypeDef gpio_usart;

		__HAL_RCC_LPUART1_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE(); // chap5vid13
		//2. Set up pin muxing - chap5vid8
		gpio_usart.Pin = GPIO_PIN_7 | GPIO_PIN_8;
		gpio_usart.Mode = GPIO_MODE_AF_PP;
		gpio_usart.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		gpio_usart.Alternate = GPIO_AF8_LPUART1;
		HAL_GPIO_Init(GPIOG, &gpio_usart);

		HAL_GPIO_Init(GPIOG, &gpio_usart);
		//3. Enable IRQ and set the priority of it (NVIC setting)
		HAL_NVIC_EnableIRQ(LPUART1_IRQn); //Enable IRQ in NVIC->ISER
		HAL_NVIC_SetPriority(LPUART1_IRQn,15,0); // Set priority in NVIC->IPR
	}
}

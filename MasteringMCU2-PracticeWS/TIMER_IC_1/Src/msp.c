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
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	//Enable clock gate for TIMER2 and respective IO
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//Config GPIO to behave as TIMER2 input channel 1
	GPIO_InitTypeDef timer2IO;
	timer2IO.Pin = GPIO_PIN_0;
	timer2IO.Mode = GPIO_MODE_AF_PP;
	timer2IO.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA,&timer2IO);
	//Config Interrupt
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn,15,0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_usart;
	//HERE WE DO LOW LEVEL USART3 PERIPHERAL INIT - chap5vid7
	//1. Enable the clock for USART3 - chap5vid9
	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE(); // chap5vid13
	//2. Set up pin muxing - chap5vid8
	gpio_usart.Pin = GPIO_PIN_8;
	gpio_usart.Mode = GPIO_MODE_AF_PP;
	gpio_usart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_usart.Alternate = GPIO_AF7_USART3; // USART3 TX - PD8
	HAL_GPIO_Init(GPIOD, &gpio_usart);

	gpio_usart.Pin = GPIO_PIN_9; // USART3 RX - PD9
	HAL_GPIO_Init(GPIOD, &gpio_usart);
	//3. Enable IRQ and set the priority of it (NVIC setting)
	HAL_NVIC_EnableIRQ(USART3_IRQn); //Enable IRQ in NVIC->ISER
	HAL_NVIC_SetPriority(USART3_IRQn,15,0); // Set priority in NVIC->IPR
}


/*
 * msp.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 */
#include "stm32l4xx_hal.h"
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

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	// Enable clock for TIMER 6
	__HAL_RCC_TIM6_CLK_ENABLE();
	// NVIC setting for TIMER6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	// SetUp Priority for TIM6 & DAC priority, select the lowest pre-emp priority
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn,15,0);

}
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	/*Enable USART3 CLK*/
	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_SLEEP_DISABLE(); // turn off USART3 when in SLeep Mode
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Enable USART3 IO*/
	GPIO_InitTypeDef uart3IO;
	uart3IO.Pin = GPIO_PIN_8;
	uart3IO.Mode = GPIO_MODE_AF_PP;
	uart3IO.Alternate = GPIO_AF7_USART3;
	uart3IO.Pull = GPIO_NOPULL;
	uart3IO.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD,&uart3IO);

	/*Config NVIC*/
	HAL_NVIC_EnableIRQ(USART3_IRQn); //Enable IRQ in NVIC->ISER
	HAL_NVIC_SetPriority(USART3_IRQn,15,0); // Set priority in NVIC->IPR
}
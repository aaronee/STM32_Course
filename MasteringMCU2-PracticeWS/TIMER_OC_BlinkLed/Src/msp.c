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
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	//Enable clock gate for TIMER2 and respective IO
	__HAL_RCC_TIM15_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	//Config GPIO to behave as TIMER2 input channel 1
	GPIO_InitTypeDef timer15IO;
	timer15IO.Pin = GPIO_PIN_14;
	timer15IO.Mode = GPIO_MODE_AF_PP;
	timer15IO.Alternate = GPIO_AF14_TIM15;
	HAL_GPIO_Init(GPIOB,&timer15IO);
	//Config Interrupt
	HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
	HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn,15,0);
}

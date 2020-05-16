/*
 * it.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 */
#include "main.h"
extern TIM_HandleTypeDef htimer15;

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

}
void TIM1_BRK_TIM15_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer15);
}



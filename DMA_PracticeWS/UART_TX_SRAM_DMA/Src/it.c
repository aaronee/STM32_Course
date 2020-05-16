/*
 * it.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 */
#include "main.h"
extern UART_HandleTypeDef hlpuart1;
extern DMA_HandleTypeDef hDMA1_M2P_stream;

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

}

void DMA1_Channel1_IRQHandler (void)
{
	HAL_DMA_IRQHandler(&hDMA1_M2P_stream);
}
void LPUART1_IRQHandler (void)
{
	HAL_UART_IRQHandler(&hlpuart1);
}

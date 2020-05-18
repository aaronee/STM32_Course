/*
 * it.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 */
#include "main.h"
void clear_exti_pending_bit(void);

void EXTI15_10_IRQHandler(void)
{
	clear_exti_pending_bit();
}

void clear_exti_pending_bit(void)
{
	if(EXTI->PR1 & EXTI_PR1_PIF13)
	{
		// clear pending bit if IRQ is triggered
		EXTI->PR1 |= EXTI_PR1_PIF13;
	}
}
void DMA1_Channel1_IRQHandler(void)
{
	if((DMA1->ISR>>1) & 1UL)
	{
		DMA1->IFCR |= (1UL<<1UL); // clear TCIF1 DMA1 chan1
	}
	//while(1); // hang here - project completed - transfer 10byte to lpuart1 tx by DMA1 channel1
}

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

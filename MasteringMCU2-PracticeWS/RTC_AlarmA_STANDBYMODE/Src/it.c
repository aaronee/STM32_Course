/*
 * it.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 */
#include "main.h"

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

}
void EXTI15_10_IRQHandler(void)
{
	/**
	 * As the Processor enter the STANDBY mode, and only wake up
	 * if the B1 button is press => this interrupt will never be called
	 * This line is written to test RTC by pressing B1 as a button trigger
	 * not as a WKUP source
	 */
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
void RTC_Alarm_IRQHandler(void)
{
	HAL_RTC_AlarmIRQHandler(&hrtc);
}

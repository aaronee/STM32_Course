/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "it.h"
#include "FreeRTOS.h"
#include "task.h"

extern TIM_HandleTypeDef htim2;


void HardFault_Handler(void)
{
  while (1);
}

void MemManage_Handler(void)
{
  while (1);
}
void BusFault_Handler(void)
{
  while (1);
}
void UsageFault_Handler(void)
{
  while (1);
}


/**
  * @brief This function handles TIM2 global interrupt.
  * 	TIM2 is used as HAL time base
  */
void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim2);
}

void EXTI15_10_IRQHandler(void)
{
	traceISR_ENTER();
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
	traceISR_ENTER();
}

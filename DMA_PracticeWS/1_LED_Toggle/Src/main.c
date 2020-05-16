/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - DMA Programming
 *
 *      Goal: Toggling the LED
 *      Using DMA to writing data from SRAM to LED output (toggling LED)
 *      Implement Manual trigger DMA or Automatically trigger DMA
 *      WE use DMA polling mode only in this example
 */
#include "stm32l4r5xx.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

void SystemClockConfig(uint8_t clock_freq);
void Error_Handler(void);
void GPIO_Init(void);
void DMA_Init(void);

DMA_HandleTypeDef hDMA1_M2M_stream = {0};
uint8_t LED_data[2] = {0x00,0xff};

int main(void)
{
	HAL_Init(); // init HAL
	SystemClockConfig(SYS_CLOCK_FREQ_16MHZ); // config system clock if needed
	GPIO_Init();
	DMA_Init();
	while(1)
	{
		if(HAL_DMA_Start(&hDMA1_M2M_stream,(uint32_t)&LED_data[0],(uint32_t)&GPIOB->ODR,1) != HAL_OK)
		{
			Error_Handler();
		}
		HAL_DMA_PollForTransfer(&hDMA1_M2M_stream,HAL_DMA_FULL_TRANSFER,HAL_MAX_DELAY);
		HAL_Delay(1000);
		if(HAL_DMA_Start(&hDMA1_M2M_stream,(uint32_t)&LED_data[1],(uint32_t)&GPIOB->ODR,1) != HAL_OK)
		{
			Error_Handler();
		}
		HAL_DMA_PollForTransfer(&hDMA1_M2M_stream,HAL_DMA_FULL_TRANSFER,HAL_MAX_DELAY);
		HAL_Delay(1000);
	}

}

void SystemClockConfig(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;	//chap6vid4
	RCC_ClkInitTypeDef clk_init;	//chap6vid4
	uint8_t FlashLatency = 0;

	memset(&osc_init,0,sizeof(osc_init));
	memset(&clk_init,0,sizeof(clk_init));

	/* Create param structure for OSCONFIG & CLKCONFIG */
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource =RCC_PLLSOURCE_HSI;

	switch(clock_freq)
	{
	case SYS_CLOCK_FREQ_16MHZ :
			{
				/*Init param struct to config PLL */
				osc_init.PLL.PLLState = RCC_PLL_OFF;
				/*Init param struct to config bus clock*/
				clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
									RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
				clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
				clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
				clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
				clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

				/*Adjust flash latency for desired frequency*/
				FlashLatency =  FLASH_ACR_LATENCY_0WS;
				break;
			}
		case SYS_CLOCK_FREQ_50MHZ :
		{
			/*Init param struct to config PLL */
			osc_init.PLL.PLLM = 4;
			osc_init.PLL.PLLN = 50;
			osc_init.PLL.PLLR = RCC_PLLR_DIV4;
			osc_init.PLL.PLLQ = RCC_PLLR_DIV2; //default
			osc_init.PLL.PLLP = RCC_PLLR_DIV2; //default

			/*Init param struct to config bus clock*/
			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
								RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			/*Adjust flash latency for desired frequency*/
			FlashLatency =  FLASH_ACR_LATENCY_2WS;
			break;
		}
		case SYS_CLOCK_FREQ_84MHZ :
		{
			/*Init param struct to config PLL*/
			osc_init.PLL.PLLM = 4;
			osc_init.PLL.PLLN = 42;
			osc_init.PLL.PLLR = RCC_PLLR_DIV2;
			osc_init.PLL.PLLQ = RCC_PLLR_DIV2; //default
			osc_init.PLL.PLLP = RCC_PLLR_DIV2; //default
			/*Init param struct to config bus clock*/
			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
								RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			/*Adjust flash latency for desired frequency*/
			FlashLatency = FLASH_ACR_LATENCY_4WS;
			break;
		}
		case SYS_CLOCK_FREQ_120MHZ :
		{
			/*Init param struct to config PLL */
			osc_init.PLL.PLLM = 4;
			osc_init.PLL.PLLN = 60;
			osc_init.PLL.PLLR = RCC_PLLR_DIV2;
			osc_init.PLL.PLLQ = RCC_PLLR_DIV2; //default
			osc_init.PLL.PLLP = RCC_PLLR_DIV2; //default

			/*Init param struct to config bus clock*/
			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
								RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			/*Adjust flash latency for desired frequency*/
			FlashLatency = FLASH_ACR_LATENCY_5WS;
			break;
		}
		default :
			break;
	}

	/*Parsing param into config functions*/
	if (HAL_RCC_OscConfig(&osc_init) != HAL_OK)	//chap6vid4
	{
		Error_Handler();
	}

	if (HAL_RCC_ClockConfig(&clk_init,FlashLatency) != HAL_OK)
	{
		Error_Handler(); // FLASH LATENCY IS IMPORTANT - later half chap6vid5
	}
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000); // param is number of tick to archive 1ms between 2 systick interrupt

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); // set prescale value for System timer clock
														// HCLK or HCLK_DIV8
}

void Error_Handler(void)
{
	while(1);
}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// can't put port C in sleep, cause Button1 - PC13 is the source to wake up
	/*Enable button B1 - PC13*/
	GPIO_InitTypeDef button1IO;
	button1IO.Pin = GPIO_PIN_13;
	button1IO.Mode = GPIO_MODE_EVT_FALLING; // Put GPIO in event generating mode
	button1IO.Pull = GPIO_NOPULL;
	button1IO.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC,&button1IO);

	/*Enable GPIOB7 - LED blue*/
	button1IO.Pin = GPIO_PIN_7;
	button1IO.Mode = GPIO_MODE_OUTPUT_PP; // Put GPIO in event generating mode
	button1IO.Pull = GPIO_NOPULL;
	button1IO.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB,&button1IO);
}

void DMA_Init(void)
{
	__HAL_RCC_DMAMUX1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	hDMA1_M2M_stream.Instance = DMA1_Channel1;
	hDMA1_M2M_stream.Init.Direction = DMA_MEMORY_TO_MEMORY;
	hDMA1_M2M_stream.Init.PeriphInc = DMA_PINC_DISABLE;
	hDMA1_M2M_stream.Init.MemInc = DMA_MINC_DISABLE;
	hDMA1_M2M_stream.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hDMA1_M2M_stream.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hDMA1_M2M_stream.Init.Mode = DMA_NORMAL;
	hDMA1_M2M_stream.Init.Priority = DMA_PRIORITY_LOW;
	hDMA1_M2M_stream.Init.Request = DMA_REQUEST_MEM2MEM;

	if(HAL_DMA_Init(&hDMA1_M2M_stream) != HAL_OK)
	{
		Error_Handler();
	}

}
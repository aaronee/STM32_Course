/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - DMA Programming
 *
 *      Goal: DMA with UART TX
 *      DMA transmit data from memory to tx while receive request of UART TX
 */
#include "stm32l4xx_hal_conf.h"
#include "stm32l4r5xx.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

#define OFFSET 0x800
#define DATALOC (uint8_t *)SRAM1_BASE + OFFSET

void SystemClockConfig(uint8_t clock_freq);
void Error_Handler(void);
void GPIO_Init(void);
void DMA_Init(void);
void UART_Init(void);
void my_DMA_callback_func(DMA_HandleTypeDef *hdma);
UART_HandleTypeDef hlpuart1 = {0};
DMA_HandleTypeDef hDMA1_M2P_stream = {0};
uint8_t Buffer[20] = "hello world! \r\n";
int main(void)
{
	HAL_Init(); // init HAL
	SystemClockConfig(SYS_CLOCK_FREQ_16MHZ); // configure system clock if needed
	GPIO_Init();
	UART_Init();
	DMA_Init();

	HAL_UART_Transmit_DMA(&hlpuart1,Buffer,20); //strlen((char*)Buffer)

	while(1)
	{
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

void UART_Init(void)
{
	hlpuart1.Instance = LPUART1;
	hlpuart1.Init.BaudRate = 9600;
	hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	hlpuart1.Init.Mode = UART_MODE_TX_RX;
	hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
	hlpuart1.Init.StopBits = UART_STOPBITS_1;
	hlpuart1.Init.Parity = UART_PARITY_NONE;
	hlpuart1.Init.OverSampling = UART_OVERSAMPLING_16;
	hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;

	if (HAL_UART_Init(&hlpuart1)!= HAL_OK)
	{
		Error_Handler();
	}
}

void DMA_Init(void)
{
	__HAL_RCC_DMAMUX1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();


	// init DMA for lpuart1
	hDMA1_M2P_stream.Instance = DMA1_Channel1;
	hDMA1_M2P_stream.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hDMA1_M2P_stream.Init.PeriphInc = DMA_PINC_DISABLE;
	hDMA1_M2P_stream.Init.MemInc = DMA_MINC_ENABLE;
	hDMA1_M2P_stream.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hDMA1_M2P_stream.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hDMA1_M2P_stream.Init.Mode = DMA_NORMAL;
	hDMA1_M2P_stream.Init.Priority = DMA_PRIORITY_LOW;
	hDMA1_M2P_stream.Init.Request = DMA_REQUEST_LPUART1_TX;

	if(HAL_DMA_Init(&hDMA1_M2P_stream) != HAL_OK)
	{
		Error_Handler();
	}

	// DMA link to Uart struct is implemented in uart msp call

	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn,15,0);
}

void my_DMA_callback_func(DMA_HandleTypeDef *hdma)
{
	if(hdma->Instance == DMA1_Channel1)
	{
		//DATA transfers complete call back
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == LPUART1)
  {
	  HAL_UART_Transmit(&hlpuart1,(uint8_t*)"TX completed \r\n",strlen("TX completed \r\n"),HAL_MAX_DELAY);
  }
}
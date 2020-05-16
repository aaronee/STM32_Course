/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - Mastering Microcontroller  TIMERS, PWM, CAN, RTC,LOW POWER
 *      GOAL: Using Button1 - PC13 interrupt to trigger USART3 to send message
 *      	Using WFI instruction to put processor into sleep when needed
 *      	Chap20vid1 to Chap20vid3
 */
#include "stm32l4xx_hal_conf.h"
#include "stm32l4r5xx.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

void SystemClockConfig(uint8_t clock_freq);
void Error_Handler(void);
void UART_Init(void);
void GPIO_Init(void);
void GPIO_AnalogConfig(void);

extern uint8_t some_data[];
UART_HandleTypeDef huart3;
int main(void)
{
	HAL_Init(); // init HAL
	SystemClockConfig(SYS_CLOCK_FREQ_16MHZ); // config system clock if needed
	UART_Init();
	GPIO_Init();
	GPIO_AnalogConfig();

	while(1)
	{
		__WFI(); // go to sleep mode when processor come back to while loop
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

	/* AFTER THIS POINT, SYSTEM USE HSI 16Mhz AS SYSCLK */

	//__HAL_RCC_HSI_DISABLE();	//disable HSI to save power - chap6vid5

	/* As HCLK drive SYSTEM TIMER & we want to config interrupt after every 1ms = 1/1000 s
	 *
	 * 1 tick		   = 1 / HCLK	(sec)
	 * ? tick		   = 1/1000 	(sec)
	 * => number of tick = HCLK/1000
	 *
	 * */
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
	// can't put port C in sleep, cause Button1 - PC13 is the source to wake up
	/*Enable button B1 - PC13*/
	GPIO_InitTypeDef button1IO;
	button1IO.Pin = GPIO_PIN_13;
	button1IO.Mode = GPIO_MODE_IT_FALLING;
	button1IO.Pull = GPIO_NOPULL;
	button1IO.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC,&button1IO);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,15,0);


}
void GPIO_AnalogConfig(void)
{
	/*Set variables*/
	uint32_t pin_unused;
	GPIO_InitTypeDef gpiod_unused, gpioc_unused;

	/*Set pin to analog for port D,C*/
	/*Turn unused GPIO to Analog mode portC*/
	pin_unused = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |
								GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
								GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 |
								GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
								GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	gpiod_unused.Pin = pin_unused;
	gpiod_unused.Mode = GPIO_MODE_ANALOG; // or Pull down (connect to GND)
	HAL_GPIO_Init(GPIOD,&gpiod_unused);

	/*Turn unused GPIO to Analog mode portC*/
	pin_unused = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |
					GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
					GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 |
					GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
					GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;
	gpioc_unused.Pin = pin_unused;
	gpioc_unused.Mode = GPIO_MODE_ANALOG; // or Pull down (connect to GND)
	HAL_GPIO_Init(GPIOC,&gpioc_unused);
}
void UART_Init(void)
{
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 230400;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	if (HAL_UART_Init(&huart3) != HAL_OK)
	{
		Error_Handler();
	}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_UART_Transmit(&huart3,some_data,strlen((char*)some_data),HAL_MAX_DELAY);
}

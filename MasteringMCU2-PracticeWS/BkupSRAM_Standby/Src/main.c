/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - Mastering Micro-controller  TIMERS, PWM, CAN, RTC,LOW POWER
 *		Chap26Vid4/5
 *		Enter standby mode, using SRAM to store data before enter Standby mode
 *		Using WakeUp pin to exit from STANDBY mode
 */
#include "stm32l4xx_hal_conf.h"
#include "stm32l4r5xx.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
/*----------------------------------------------------------------*/

#define TRUE 1
#define FALSE 0

UART_HandleTypeDef hlpuart1;
uint32_t *pSRAM2_base_addr;
char data_to_stored[5] = {"Hello"};
char mesg[100] = {0};
/*----------------------------------------------------------------*/
void System_Clk_Config(uint8_t clock_freq);
void UART_Init(void);
void GPIO_Init(void);
void Error_handler(void);
/*----------------------------------------------------------------*/

int main(void)
{
	HAL_Init();	//Initialize HAL abstract layer
	System_Clk_Config(SYS_CLOCK_FREQ_50MHZ);
	GPIO_Init();
	UART_Init();

	/*Write the data to SRAM*/
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_LOW); // WKUP pin 2 GPIO PC13 active LOW


	HAL_PWREx_EnableSRAM2ContentRetention(); //Enable SRAM2 content retention in Standby mode
	pSRAM2_base_addr = (uint32_t *)0x10000000UL;

	if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

		sprintf(mesg,"Out of Standby Mode\r\n");
		HAL_UART_Transmit(&hlpuart1,(uint8_t*)mesg,strlen(mesg),HAL_MAX_DELAY);
		uint8_t* data = (uint8_t *)pSRAM2_base_addr;
		if (*data != 'H')
		{
			sprintf(mesg,"Data in SRAM in-correct\r\n");
			HAL_UART_Transmit(&hlpuart1,(uint8_t*)mesg,strlen(mesg),HAL_MAX_DELAY);
		}
	}
	else
	{
		for (uint8_t cnt = 0; cnt < (strlen(data_to_stored)+1); cnt++)
		{
			*(pSRAM2_base_addr + cnt) = data_to_stored[cnt];
		}
	}

	HAL_Delay(500);
	while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) != GPIO_PIN_SET); // button press -> process to STANDBY
	HAL_Delay(500);

	sprintf(mesg,"Go to Standby Mode\r\n");
	HAL_UART_Transmit(&hlpuart1,(uint8_t*)mesg,strlen(mesg),HAL_MAX_DELAY);

	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
	HAL_PWR_EnterSTANDBYMode();

	while(1);
	return 0;
}
/*----------------------------------------------------------------*/
void UART_Init(void)
{
	 hlpuart1.Instance = LPUART1;
	 hlpuart1.Init.BaudRate = 209700;
	 hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
	 hlpuart1.Init.StopBits = UART_STOPBITS_1;
	 hlpuart1.Init.Parity = UART_PARITY_NONE;
	 hlpuart1.Init.Mode = UART_MODE_TX_RX;
	 hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	 hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	 hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	 hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	 hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;	// Peripheral high level initialization chap5vid6
	if ( HAL_UART_Init(&hlpuart1)!=HAL_OK)
	{
		Error_handler();
	}
}
void System_Clk_Config(uint8_t clock_freq)
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
		Error_handler();
	}

	if (HAL_RCC_ClockConfig(&clk_init,FlashLatency) != HAL_OK)
	{
		Error_handler();
	}
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);


}
/*----------------------------------------------------------------*/
void GPIO_Init(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef B1_param; // active high
	B1_param.Pin = GPIO_PIN_13;
	B1_param.Mode = GPIO_MODE_INPUT;
	B1_param.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOC,&B1_param);
}
void Error_handler(void)
{
 while(1);
}



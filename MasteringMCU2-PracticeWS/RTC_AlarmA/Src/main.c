/**
 *
 * Author: Aaron Huynh - Created on: Apr. 13, 2020
 *
 * File name: main.c
 * Project name: RTC_AlarmA
 *
 * Description: configure Alarm A (by pressing B1) to blink the LD2 after 10 second
 *
 */
#include "stm32l4xx_hal_conf.h"
#include "stm32l4r5xx.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
/*----------------------------------------------------------------*/

#define TRUE 1
#define FALSE 0

UART_HandleTypeDef hlpuart1 = {0};
RTC_HandleTypeDef hrtc = {0};

char mesg[100] = {0};
/*----------------------------------------------------------------*/
void System_Clk_Config(uint8_t clock_freq);
void RTC_Init(void);
void RTC_CalendarConfig(void);
void RTC_AlarmConfig(void);
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
	RTC_Init();
//	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_LOW); // WKUP pin 2 GPIO PC13 active LOW
//
//	if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
//	{
//		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
//		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
//
//		sprintf(mesg,"Exit Standby mode by hitting RST button \r\n");
//		if (HAL_UART_Transmit(&hlpuart1,(uint8_t *)mesg,strlen(mesg),HAL_MAX_DELAY) != HAL_OK)
//		{
//			Error_handler();
//		}
//
//		HAL_GPIO_EXTI_Callback(GPIO_PIN_13);
//	}
//
//	sprintf(mesg,"Enter Standby mode \r\n");
//	if (HAL_UART_Transmit(&hlpuart1,(uint8_t *)mesg,strlen(mesg),HAL_MAX_DELAY) != HAL_OK)
//	{
//		Error_handler();
//	}
//
//	HAL_Delay(500);
//	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
//	HAL_PWR_EnterSTANDBYMode();
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
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.LSEState = RCC_LSE_ON;
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

	/* Configure Systick timer after init clock*/
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	/* Set Systick IRQ to maximum */
	HAL_NVIC_SetPriority(SysTick_IRQn,0,0);
}
void RTC_Init(void)
{
	if((hrtc.Instance->ISR & RTC_ISR_INITS) != RTC_ISR_INITS) // check if RTC is already running and init
	{
		// using LSE => set prescaler = default value
		hrtc.Instance = RTC;
		hrtc.Init.AsynchPrediv = 127;
		hrtc.Init.SynchPrediv = 255;
		hrtc.Init.HourFormat = RTC_HOURFORMAT_12 ;
		hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
		hrtc.Init.OutPutPolarity =  RTC_OUTPUT_POLARITY_LOW;
		hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
		hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
		if (HAL_RTC_Init(&hrtc)!= HAL_OK)
		{
			Error_handler();
		}
	}
}
void RTC_CalendarConfig(void)
{
	if((hrtc.Instance->ISR & RTC_ISR_INITS) != RTC_ISR_INITS)
	{
		// This configure RTC calendar as 12:11:10 PM Date 12 June 2018 Friday
		RTC_TimeTypeDef RTCtime_param = {0};
		RTC_DateTypeDef RTCdate_param = {0};
		RTCtime_param.Hours = 12; // this is BIN format
		RTCtime_param.Minutes = 45;
		RTCtime_param.Seconds = 00;
		RTCtime_param.TimeFormat = RTC_HOURFORMAT12_PM;
		if (HAL_RTC_SetTime(&hrtc,&RTCtime_param,RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_handler();
		}
		RTCdate_param.Date = 0x12; // this is BCD format
		RTCdate_param.Month = RTC_MONTH_JUNE;
		RTCdate_param.WeekDay = RTC_WEEKDAY_TUESDAY;
		RTCdate_param.Year = 0x18;
		if (HAL_RTC_SetDate(&hrtc,&RTCdate_param,RTC_FORMAT_BCD) != HAL_OK)
		{
			Error_handler();
		}
	}
}
void GPIO_Init(void)
{
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE(); // PC13
	__HAL_RCC_GPIOB_CLK_ENABLE(); // lD2 - blue

	/*<B1 button PC13>*/
	GPIO_InitTypeDef B1_param; // active LOW
	B1_param.Pin = GPIO_PIN_13;
	B1_param.Mode = GPIO_MODE_IT_FALLING;
	B1_param.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOC,&B1_param);

	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,15,0);


	/*<LD2 led blue PB7>*/
	GPIO_InitTypeDef LD2_param;
	LD2_param.Pin = GPIO_PIN_7;
	LD2_param.Mode = GPIO_MODE_OUTPUT_PP;
	LD2_param.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOB,&LD2_param);

}
void Error_handler(void)
{
 while(1);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	if(GPIO_Pin == GPIO_PIN_13)
	{
		RTC_TimeTypeDef readtime_struct = {0};
		RTC_DateTypeDef readdate_struct = {0};

		RTC_CalendarConfig();


		if( HAL_RTC_GetTime(&hrtc,&readtime_struct,RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_handler();
		}
		if( HAL_RTC_GetDate(&hrtc,&readdate_struct,RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_handler();
		}
		sprintf(mesg,"TIME: %02d-%02d-%02d \r\n",readtime_struct.Hours,readtime_struct.Minutes,readtime_struct.Seconds);
		if (HAL_UART_Transmit(&hlpuart1,(uint8_t *)mesg,strlen(mesg),HAL_MAX_DELAY) != HAL_OK)
		{
			Error_handler();
		}
		sprintf(mesg,"DATE: %02d-%02d-%02d \r\n",readdate_struct.Date,readdate_struct.Month,readdate_struct.Year);
		if (HAL_UART_Transmit(&hlpuart1,(uint8_t *)mesg,strlen(mesg),HAL_MAX_DELAY) != HAL_OK)
		{
			Error_handler();
		}

		RTC_AlarmConfig();
	}
}
/**
 * Function name: RTC_AlarmConfig
 * Description: configure Alarm A at xx:45:0
 *
 * @param: void
 * @return: void
 */
void RTC_AlarmConfig(void)
{
	// xx:45:09

	RTC_AlarmTypeDef RTCAlarm_param = {0};
	/* Deactivate current running Alarm before set it again*/
	HAL_RTC_DeactivateAlarm(&hrtc,RTC_ALARM_A);

	RTCAlarm_param.Alarm = RTC_CR_ALRAE;
	RTCAlarm_param.AlarmTime.Minutes = 45;
	RTCAlarm_param.AlarmTime.Seconds = 9;
	RTCAlarm_param.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY  | RTC_ALARMMASK_HOURS;
	RTCAlarm_param.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;

	if(HAL_RTC_SetAlarm_IT(&hrtc,&RTCAlarm_param,RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

	sprintf(mesg,"Alarm A is set \r\n");
	if (HAL_UART_Transmit(&hlpuart1,(uint8_t *)mesg,strlen(mesg),HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}
}
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	if(hrtc->Instance == RTC)
	{
		sprintf(mesg,"Turn on LED for 2 seconds \r\n");
		if (HAL_UART_Transmit(&hlpuart1,(uint8_t *)mesg,strlen(mesg),HAL_MAX_DELAY) != HAL_OK)
		{
			Error_handler();
		}

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_Delay(2000);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	}

}

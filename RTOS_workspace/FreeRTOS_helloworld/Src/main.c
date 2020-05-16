/**
 *
 * Author: Aaron Huynh - Created on: Apr. 9, 2020
 *
 * File name: main.c
 * Project name: STM32_helloworld
 *
 * Description: Create 2 task 1 & 2
 * 				Task 1 print "helloworld from task 1"
 * 				Task 2 print "helloworld from task 2"
 * 				case 1: Use Arm semi-host to print log on console
 * 				case 2: Use Uart to print log
 *
 */
#include "stm32l4r5xx.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/*----------------------------------------------------------------*/
//#define USE_SEMIHOSTING
#define USE_UART
#define TRUE 1
#define FALSE 0

UART_HandleTypeDef hlpuart1 = {0};

/*----------------------------------------------------------------*/
void UART_Init(void);
void Error_handler(void);
static void prvSetupHardware(void);
void vTask1_handler(void *params);
void vTask2_handler(void *params);
char mesg[100] = {0};

#ifdef USE_SEMIHOSTING
// Call semihosting
extern void initialise_monitor_handles(void);
#endif

TaskHandle_t xTaskHandle1 = NULL;
TaskHandle_t xTaskHandle2 = NULL;

/*----------------------------------------------------------------*/

int main(void)
{
	prvSetupHardware();

	#ifdef USE_SEMIHOSTING
	initialise_monitor_handles();
	printf("semihosting runs \n");
	#endif

	#ifdef USE_UART
	sprintf(mesg,"LPUART1 is runing \r\n");
	HAL_UART_Transmit(&hlpuart1,(uint8_t*)mesg, strlen(mesg), HAL_MAX_DELAY);
	#endif

	/* Create task 1 and task 2*/
	xTaskCreate(vTask1_handler, "Task-1", configMINIMAL_STACK_SIZE, NULL,2, &xTaskHandle1);
	xTaskCreate(vTask2_handler, "Task-2", configMINIMAL_STACK_SIZE, NULL,2, &xTaskHandle2);

	vTaskStartScheduler();

	while(1);
}

/*----------------------------------------------------------------*/
void vTask1_handler(void *params)
{
	while(1)
	{
		sprintf(mesg,"This is FREERTOS task 1 using LPUART1 \r\n");
		HAL_UART_Transmit(&hlpuart1,(uint8_t*)mesg, strlen(mesg), HAL_MAX_DELAY);
		taskYIELD(); // leave CPU as soon as print complete
	}
}
void vTask2_handler(void *params)
{
	while(1)
	{
		sprintf(mesg,"This is FREERTOS task 2 using LPUART1 \r\n");
		HAL_UART_Transmit(&hlpuart1,(uint8_t*)mesg, strlen(mesg), HAL_MAX_DELAY);
	}
}

void Error_handler(void)
{
	while(1);
}

static void prvSetupHardware(void)
{
	HAL_Init();
	UART_Init();
}

/**
 *	Callback for HAL clock based
 *
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    HAL_IncTick();
  }
}
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

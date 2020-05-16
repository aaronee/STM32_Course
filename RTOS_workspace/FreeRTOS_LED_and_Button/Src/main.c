/**
 *
 * Author: Aaron Huynh - Created on: Apr. 9, 2020
 *
 * File name: main.c
 * Project name: FreeRTOS_led_and_button
 *
 * Description: Create 2 tasks: led_task & button_task
 * 				Button_task: polling the status of B1 button, and set the flag when B1 is pushed
 * 				Led_task: Turn on the LD2 if flag 's set, otw, turn of the LD2
 *
 */
#include "stm32l4r5xx.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/*----------------------------------------------------------------*/
//#define USE_SEMIHOSTING
//#define USE_UART
#define TRUE 1
#define FALSE 0

UART_HandleTypeDef hlpuart1 = {0};

/*----------------------------------------------------------------*/
void UART_Init(void);
void GPIO_Init(void);
void Error_handler(void);
static void prvSetupHardware(void);
void vLD2task_handler(void *params);
void vB1task_handler(void *params);
char mesg[100] = {0};
uint8_t bflag = 0;

#ifdef USE_SEMIHOSTING
// Call semihosting
extern void initialise_monitor_handles(void);
#endif

TaskHandle_t xLD2taskHandle = NULL;
TaskHandle_t xB1taskHandle = NULL;

/*----------------------------------------------------------------*/

int main(void)
{
	DWT->CTRL |= (1UL << 0UL); //Enable the CYCCNT counter using for SYSVIEW to use as time stamp

	//SYSVIEW starts recording
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

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
	xTaskCreate(vLD2task_handler,"LD2-task",configMINIMAL_STACK_SIZE,NULL,2,&xLD2taskHandle);
	xTaskCreate(vB1task_handler,"B1-task",configMINIMAL_STACK_SIZE,NULL,2,&xB1taskHandle);

	vTaskStartScheduler();

	for(;;);

}

/*----------------------------------------------------------------*/
void vLD2task_handler(void *params)
{
	while(1)
	{
		if (bflag == TRUE)
		{
			SEGGER_SYSVIEW_Print("TURN ON LD2");
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
		}
		else
		{
			SEGGER_SYSVIEW_Print("TURN OFF LD2");
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
		}
		taskYIELD();
	}
}
void vB1task_handler(void *params)
{
	while(1)
	{
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) != GPIO_PIN_RESET)
		{
			bflag = TRUE;
		}
		else
		{
			bflag = FALSE;
		}
	}
}

void Error_handler(void)
{
	while(1);
}

static void prvSetupHardware(void)
{
	HAL_Init();
	GPIO_Init();
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
void GPIO_Init(void)
{
	__HAL_RCC_GPIOG_CLK_ENABLE(); // lpuart1 tx-rx
	__HAL_RCC_GPIOC_CLK_ENABLE(); // PC13
	__HAL_RCC_GPIOB_CLK_ENABLE(); // lD2 - blue

	/*<B1 button PC13>*/
	GPIO_InitTypeDef B1_param; // active LOW
	B1_param.Pin = GPIO_PIN_13;
	B1_param.Mode = GPIO_MODE_INPUT;
	B1_param.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOC,&B1_param);

	/*<LD2 led blue PB7>*/
	GPIO_InitTypeDef LD2_param;
	LD2_param.Pin = GPIO_PIN_7;
	LD2_param.Mode = GPIO_MODE_OUTPUT_PP;
	LD2_param.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOB,&LD2_param);
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

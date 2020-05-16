/**
 *
 * Author: Aaron Huynh - Created on: Apr. 9, 2020
 *
 * File name: main.c
 * Project name: FreeRTOS_led_and_button
 *
 * Description: Create 2 tasks: Task-1 (priority - 2) & Task-2 (priority - 3)
 * 				Task-1 will blink the LED for every 100ms
 * 				Task-2 will blink the LED for every 1s
 * 				When button is pressed, reverse the priority of 2 tasks
 *
 * 				CHAPTER 21
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

void vTask1_handler(void *params);
void vTask2_handler(void *param);

TaskHandle_t xTask1 = NULL;
TaskHandle_t xTask2 = NULL;

char mesg[100] = {0};
uint8_t bflag = 0;

#ifdef USE_SEMIHOSTING
// Call semihosting
extern void initialise_monitor_handles(void);
#endif


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
	xTaskCreate(vTask1_handler,"Task-1",configMINIMAL_STACK_SIZE,NULL,2,&xTask1);
	xTaskCreate(vTask2_handler,"Task-2",configMINIMAL_STACK_SIZE,NULL,3,&xTask2);

	vTaskStartScheduler();

	for(;;);

}

/*----------------------------------------------------------------*/
void vTask1_handler(void *params)
{
	while(1)
	{
		if(bflag)
		{
			bflag = FALSE;
			UBaseType_t p1 = uxTaskPriorityGet(xTask1);
			UBaseType_t p2 = uxTaskPriorityGet(xTask2);

			vTaskPrioritySet(xTask1,p2);
			vTaskPrioritySet(xTask2,p1);
		}
		else
		{
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
			HAL_Delay(100);
		}
	}
}
void vTask2_handler(void *param)
{
	while(1)
	{
		if(bflag)
		{
			bflag = FALSE;
			UBaseType_t p1 = uxTaskPriorityGet(xTask1);
			UBaseType_t p2 = uxTaskPriorityGet(xTask2);

			vTaskPrioritySet(xTask1,p2);
			/*!<Up to here 2 tasks will have the same priority and follow FreeRTOS rule
			 * task2 will be switch into immediately, than it will switch back to here when it has done>*/
			vTaskPrioritySet(xTask2,p1);
			/*!<Up to here task2 has the higher priority, the processor will be switch to task2 immediately
			 * and task2 will keep the processor until b1 is pressed again>*/
		}
		else
		{
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
			HAL_Delay(1000);
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
/**
 * Peripheral initialize
 *
 */
void GPIO_Init(void)
{
	__HAL_RCC_GPIOG_CLK_ENABLE(); // lpuart1 tx-rx
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
/**
 * Callback for EXTI line 13
 *
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_13)
	{
		bflag = TRUE;
	}
}

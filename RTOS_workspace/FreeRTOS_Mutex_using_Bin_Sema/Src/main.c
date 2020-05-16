/**
 *
 * Author: Aaron Huynh - Created on: Apr. 9, 2020
 *
 * File name: main.c
 * Project name: FreeRTOS_led_and_button
 *
 * Description: This example is a copy of FreeRTOS QUEUE processing example aiming to
 * 				demonstrating the use of semaphore to avoid collision when multiple task using same resource
 * Problem: app_rtc_read() when trying to use the same variable [taskmesg] to store
 * 			2 messages and send to uart_write_queue back to back
 * 			vTask2() trying to print messages whenever the queue is filled, doesn't care the
 * 			last message is transmitted completed or not
 * Solution: Give and take a binary semaphore to ensure that 1 message is transmitted success to another
 *
 * 			Line #75 we create a semaphore handler
 * 			Line #137 we create a semaphore
 * 			Line #468 we take the semaphore in app_rtc_read()
 * 				and give back when uart is finished at #188 in vTask2_handler()
 *			Line #473 we take the semaphore the 2nd time and give back when uart is finished at #188 again
 *
 *<-this is line 22
 */
#include "stm32l4r5xx.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/*----------------------------------------------------------------*/
//#define USE_SEMIHOSTING
//#define USE_UART
#define TRUE 1
#define FALSE 0

#define LED_ON_CMD 			1
#define LED_OFF_CMD 		2
#define LED_TOGGLE_CMD 		3
#define LED_TOGGLE_STOP_CMD	4
#define LED_READ_STATUS		5
#define RTC_READ_DATETIME 	6
#define EXIT				0

UART_HandleTypeDef hlpuart1 = {0};
RTC_HandleTypeDef hrtc = {0};

/*----------------------------------------------------------------*/
void UART_Init(void);
void GPIO_Init(void);
void Error_handler(void);
void OSC_Init(void);
void RTC_Init(void);
static void prvSetupHardware(void);
uint8_t getCMD(uint8_t *buff);
void getARGS(uint8_t *buff); // not implement in this example
void led_toggle(TimerHandle_t xTimer);

void app_led_on(void);
void app_led_off(void);
void app_led_toggle(uint32_t toggle_duration);
void app_led_stop(void);
void app_led_read(char *taskmesg);
void app_rtc_read(char *taskmesg);
void app_error_print(char *taskmesg);
void app_exit(void);

void vTask1_handler(void *params);
void vTask2_handler(void *param);
void vTask3_handler(void *param);
void vTask4_handler(void *param);


xSemaphoreHandle xBinarySemaphore;


TaskHandle_t xTask1 = NULL;
TaskHandle_t xTask2 = NULL;
TaskHandle_t xTask3 = NULL;
TaskHandle_t xTask4 = NULL;

QueueHandle_t command_queue = NULL;
QueueHandle_t uart_write_queue = NULL;

TimerHandle_t led_timer_handle = NULL;

uint8_t rxbuffer[10] = {0};
uint8_t rev_data = 0;
uint8_t count = 0; //using to navigate in RX buffer

char menu[] = {"\r\n\
Turn the led on     : 1\r\n\
Turn the led off    : 2\r\n\
Toggle the led      : 3\r\n\
Stop toggle the led : 4\r\n\
Read LED state      : 5\r\n\
Date & time 	    : 6\r\n\
Exit App            : 0\r\n\
Enter your option   :"};

typedef struct APP_CMD {
	uint8_t CMD_NUMBER;	// 1 bytes
	uint8_t CMD_ARGS[10]; // 10 bytes
}APP_CMD_t;

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
	HAL_UART_Receive_IT(&hlpuart1,&rev_data,1);

	#ifdef USE_SEMIHOSTING
	initialise_monitor_handles();
	printf("semihosting runs \n");
	#endif

	#ifdef USE_UART
	sprintf(mesg,"LPUART1 is runing \r\n");
	HAL_UART_Transmit(&hlpuart1,(uint8_t*)mesg, strlen(mesg), HAL_MAX_DELAY);
	#endif


	vSemaphoreCreateBinary(xBinarySemaphore);


	/* Creat 2 queue */
//	xQueueCreate(10,sizeof(APP_CMD_t)); // 10 * 11 bytes = 110bytes will be allocated on HEAP
	uart_write_queue = xQueueCreate(10, sizeof(char*));
	command_queue = xQueueCreate(10,sizeof(APP_CMD_t*)); // 10 * 4bytes = 40bytes will be allocated on HEAP

	if ((command_queue != NULL) && (uart_write_queue != NULL) && xBinarySemaphore != NULL)
	{
		xTaskCreate(vTask1_handler,"menu_print_task",500,NULL,1,&xTask1);
		xTaskCreate(vTask2_handler,"uart_write_task",500,NULL,2,&xTask2);
		xTaskCreate(vTask3_handler,"cmd_handle_task",500,NULL,2,&xTask3);
		xTaskCreate(vTask4_handler,"cmd_process_task",500,NULL,2,&xTask4);

		vTaskStartScheduler();

	}
	else
	{
		char mesg[50] = {0};
		sprintf(mesg,"Queue creation fail \r\n");
		HAL_UART_Transmit(&hlpuart1,(uint8_t*)mesg,strlen(mesg),HAL_MAX_DELAY);
	}
	for(;;);

}

/*----------------------------------------------------------------*/
void vTask1_handler(void *params)
{
	/*!<menu_print_task>*/
	char *pDada = menu;
	while(1)
	{
		xQueueSend(uart_write_queue,&pDada,portMAX_DELAY);

		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
	}
}
void vTask2_handler(void *param)
{
	/*!<uart_write_task>*/

	char *pData = NULL;
	while(1)
	{
		xQueueReceive(uart_write_queue, &pData, portMAX_DELAY);

		// Take the semaphore before printing
		HAL_UART_Transmit(&hlpuart1,(uint8_t *)pData,strlen(pData),HAL_MAX_DELAY);
		xSemaphoreGive(xBinarySemaphore);
		// Give back semaphore when finished
	}
}
void vTask3_handler(void *param)
{
	/*!<cmd_handle_task>*/
	uint8_t command_code = 0;
	APP_CMD_t *new_cmd = NULL;
	while(1)
	{
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		// send command to queue
		new_cmd = (APP_CMD_t*) pvPortMalloc(sizeof(APP_CMD_t));

		taskENTER_CRITICAL();
		command_code = getCMD(rxbuffer);
		new_cmd->CMD_NUMBER = command_code;
		getARGS(new_cmd->CMD_ARGS);
		taskEXIT_CRITICAL();
		// send to queue
		xQueueSend(command_queue,&new_cmd,portMAX_DELAY);
	}
}
void vTask4_handler(void *param)
{
	/*!<cmd_process_task>*/
	APP_CMD_t *new_cmd = NULL;
	char mesg[100] = {0};
	TickType_t toggle_durration = pdMS_TO_TICKS(500);
	while(1)
	{
		xQueueReceive(command_queue,&new_cmd,portMAX_DELAY);
		if (new_cmd->CMD_NUMBER == LED_ON_CMD)
		{
//			SEGGER_SYSVIEW_Print("LED_ON");
			app_led_on();
		}
		else if (new_cmd->CMD_NUMBER == LED_OFF_CMD)
		{
//			SEGGER_SYSVIEW_Print("LED_OFF");
			app_led_off();
		}
		else if (new_cmd->CMD_NUMBER == LED_TOGGLE_CMD)
		{
//			SEGGER_SYSVIEW_Print("LED_TOG");
			app_led_toggle(toggle_durration);
		}
		else if (new_cmd->CMD_NUMBER == LED_TOGGLE_STOP_CMD)
		{
//			SEGGER_SYSVIEW_Print("LED_NOT_TOG");
			app_led_stop();
		}
		else if (new_cmd->CMD_NUMBER == LED_READ_STATUS)
		{
//			SEGGER_SYSVIEW_Print("LED_READ");
			app_led_read(mesg);
		}
		else if (new_cmd->CMD_NUMBER == RTC_READ_DATETIME)
		{
//			SEGGER_SYSVIEW_Print("RTC_READ");
			app_rtc_read(mesg);
		}
		else if (new_cmd->CMD_NUMBER == EXIT)
		{
			app_exit();
		}
		else
		{
			app_error_print(mesg);		}
		}

		// free the allocated memory
		vPortFree(new_cmd);
}

void Error_handler(void)
{
	while(1);
}

/**
 *	Callback for HAL clock based
 *
 */
/**
 * Callback for UART
 *
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/*!<The xTaskNonifyFromISR will set this value to TRUE if it unblocks an higher priority Task>*/
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if(huart->Instance == LPUART1)
	{
		rxbuffer[count] = rev_data;
		if(rev_data == '\r')
		{
			count = 0;
			// Notice command handle task
			xTaskNotifyFromISR(xTask3,0,eNoAction,&xHigherPriorityTaskWoken);
			xTaskNotifyFromISR(xTask1,0,eNoAction,&xHigherPriorityTaskWoken);

			HAL_UART_Receive_IT(&hlpuart1,&rev_data,1);
		}
		else
		{
			count++;
			HAL_UART_Receive_IT(&hlpuart1,&rev_data,1);
		}

		/*!<if the any higher priority task got unlock, yield them*/
		if (xHigherPriorityTaskWoken)
		{
			taskYIELD();
		}
	}
}
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
static void prvSetupHardware(void)
{
	HAL_Init();
	OSC_Init();
	RTC_Init();
	GPIO_Init();
	UART_Init();
}
 /* Function supply for prvSetupHardware*/
void GPIO_Init(void)
{
	__HAL_RCC_GPIOG_CLK_ENABLE(); // lpuart1 tx-rx
	__HAL_RCC_GPIOB_CLK_ENABLE(); // lD2 - blue


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
	 hlpuart1.Init.BaudRate = 9600;
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
void OSC_Init(void)
{
	RCC_OscInitTypeDef OSC_param = {0};
	OSC_param.LSEState = RCC_LSE_ON;
	OSC_param.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	OSC_param.PLL.PLLState = RCC_PLL_OFF;

	if (HAL_RCC_OscConfig(&OSC_param) != HAL_OK)
	{
		Error_handler();
	}
}
void RTC_Init(void)
{

	RTC_TimeTypeDef RTCtime_param = {0};
	RTC_DateTypeDef RTCdate_param = {0};
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

	RTCdate_param.Date = 0x12; // this is BCD format
	RTCdate_param.Month = RTC_MONTH_JUNE;
	RTCdate_param.WeekDay = RTC_WEEKDAY_TUESDAY;
	RTCdate_param.Year = 0x18;
	if (HAL_RTC_SetDate(&hrtc,&RTCdate_param,RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_handler();
	}

	RTCtime_param.Hours = 12; // this is BIN format
	RTCtime_param.Minutes = 11;
	RTCtime_param.Seconds = 10;
	RTCtime_param.TimeFormat = RTC_HOURFORMAT12_PM;
	if (HAL_RTC_SetTime(&hrtc,&RTCtime_param,RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}
}


/* Functions supply for command handle task - xTask3 */
uint8_t getCMD(uint8_t *buff)
{
	return buff[0] - 48;
}
void getARGS (uint8_t *buff)
{
	/*!< This is an unused function for further development> */
}


/* Functions supply for command process task - xTask4 */
void app_led_on(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
}
void app_led_off(void)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
}
void app_led_toggle(uint32_t duration)
{
	if (led_timer_handle == NULL)
	{
		//1. create a software timer
		led_timer_handle = xTimerCreate("LED-TIMER",duration,pdTRUE,NULL,&led_toggle);
		xTimerStart(led_timer_handle,portMAX_DELAY);
	}
	else
	{
		//2. start a software timer
		xTimerStart(led_timer_handle,portMAX_DELAY);
	}
}
void led_toggle(TimerHandle_t xTimer)
{
	if(xTimer == led_timer_handle)
	{
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
	}
}
void app_led_stop(void)
{
	xTimerStop(led_timer_handle,portMAX_DELAY);
}
void app_led_read(char *taskmesg)
{
	sprintf(taskmesg,"\r\nLED state : %d \r\n",HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7));
	xQueueSend(uart_write_queue,&taskmesg,portMAX_DELAY);
}
void app_rtc_read(char *taskmesg)
{
	RTC_TimeTypeDef rev_time = {0};
	RTC_DateTypeDef rev_date = {0};
	HAL_RTC_GetTime(&hrtc,&rev_time,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&rev_date,RTC_FORMAT_BIN);
	/*!<Must read RTC_DR or the RTC_TR will be locked - Read up on the BYPSHAD control bit in RTC_CR*/
	xSemaphoreTake(xBinarySemaphore,portMAX_DELAY);
	sprintf(taskmesg, "\r\n Current Time is: %d Hour: %d Minute: %d Second \r\n", rev_time.Hours, rev_time.Minutes, rev_time.Seconds);
	xQueueSend(uart_write_queue,&taskmesg,portMAX_DELAY);

	//vTaskDelay(pdMS_TO_TICKS(50));
	xSemaphoreTake(xBinarySemaphore,portMAX_DELAY);
	sprintf(taskmesg, "\r\n Current Date is: %d: %d: %d \r\n", rev_date.Date, rev_date.Month, rev_date.Year);
	xQueueSend(uart_write_queue,&taskmesg,portMAX_DELAY);

}
void app_error_print(char *taskmesg)
{
	sprintf(taskmesg,"\r\nInvalid command received \r\n");
	xQueueSend(uart_write_queue,&taskmesg,portMAX_DELAY);
}
void app_exit(void)
{

	vTaskDelete(xTask1);
	vTaskDelete(xTask2);
	vTaskDelete(xTask3);
	vTaskDelete(xTask4);

	HAL_NVIC_DisableIRQ(LPUART1_IRQn);

}


void vApplicationIdleHook( void )
{
	__WFI();
}
//<--- this is line 500
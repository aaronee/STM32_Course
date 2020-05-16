/**
 *
 * Author: Aaron Huynh - Created on: Apr. 9, 2020
 *
 * File name: main.c
 * Project name: FreeRTOS_led_and_button
 *
 * Description: Here 2 tasks are created with different priorities.
 * 				Both Tasks will print some characters on to the STDOUT/UART when they get a chance to run
 * 				Here Task 2's priority > Task1's Priority.
 * 				So Task 2 runs first and prints some data to the STDOUT
 * 				then it blocks for "random" number of ticks.
 * 				When Task2 blocks, Task1 takes over the CPU and start running and tries to
 * 				output its string of data on to the stdout,but here is catch,
 * 				since Task2 is blocked for random number of ticks, we don't know when it will wake up.
 * 				If it wakes up, then it will definitely preempt the Task1 which
 * 				may be in the middle of printing string. so you will see a distorted output,
 * 				if you don't use mutex to protect the stdout.
 *
 */
#include "stm32l4r5xx.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/*----------------------------------------------------------------*/
#define TRUE 1
#define FALSE 0

//MUTEX switch : un comment the below line to use mutex
//#define USE_MUTEX

UART_HandleTypeDef hlpuart1 = {0};


char usr_msg[250]={0};
TaskHandle_t xTaskHandle1=NULL;
TaskHandle_t xTaskHandle2=NULL;

/*----------------------------------------------------------------*/
void UART_Init(void);
void GPIO_Init(void);
void Error_handler(void);
static void prvSetupHardware(void);


//function prototypes

/* Dimensions the buffer into which messages destined for stdout are placed. */
#define mainMAX_MSG_LEN	( 80 )

/* The task to be created.  Two instances of this task are created. */
static void prvPrintTask( void *pvParameters );

/* The function that uses a mutex to control access to standard out. */
static void prvNewPrintString( const portCHAR *pcString );

/*-----------------------------------------------------------*/

#ifdef USE_MUTEX
/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
mutex type semaphore that is used to ensure mutual exclusive access to UART. */
xSemaphoreHandle xMutex;
#endif


/*----------------------------------------------------------------*/

int main(void)
{
	DWT->CTRL |= (1UL << 0UL); //Enable the CYCCNT counter using for SYSVIEW to use as time stamp

	//SYSVIEW starts recording
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	prvSetupHardware();

	sprintf(usr_msg,"Demo of mutual exclusion using Mutex APIs\r\n");
	HAL_UART_Transmit(&hlpuart1,(uint8_t *)usr_msg,strlen(usr_msg),HAL_MAX_DELAY);

#ifdef USE_MUTEX
    /* Before a semaphore is used it must be explicitly created.  In this example
	a mutex type semaphore is created. */
    xMutex = xSemaphoreCreateMutex();
#endif

	/* The tasks are going to use a pseudo random delay, seed the random number
	generator. */
	srand( 567 );

#ifdef USE_MUTEX
	/* Only create the tasks if the semaphore was created successfully. */
	if( xMutex != NULL )
	{
#endif
		/* Create two instances of the tasks that attempt to write stdout.  The
		string they attempt to write is passed in as the task parameter.  The tasks
		are created at different priorities so some pre-emption will occur. */
		xTaskCreate( prvPrintTask, "Print1", 240, "Task 1 ******************************************\r\n", 1, NULL );
		xTaskCreate( prvPrintTask, "Print2", 240, "Task 2 ------------------------------------------\r\n", 2, NULL );

		/* Start the scheduler so the created tasks start executing. */
		vTaskStartScheduler();
#ifdef USE_MUTEX
	}
#endif

	for(;;);

}

/*----------------------------------------------------------------*/
static void prvPrintTask( void *pvParameters )
{
char *pcStringToPrint;

	/* Two instances of this task are created so the string the task will send
	to prvNewPrintString() is passed in the task parameter.  Cast this to the
	required type. */
	pcStringToPrint = ( char * ) pvParameters;

	for( ;; )
	{
		/* Print out the string using the newly defined function. */
		prvNewPrintString( pcStringToPrint );

		/* Wait a pseudo random time.  Note that rand() is not necessarily
		re-entrant, but in this case it does not really matter as the code does
		not care what value is returned.  In a more secure application a version
		of rand() that is known to be re-entrant should be used - or calls to
		rand() should be protected using a critical section. */
		vTaskDelay( rand() & 0XF );
	}
}
static void prvNewPrintString( const portCHAR *pcString )
{
static char cBuffer[ mainMAX_MSG_LEN ];

#ifdef USE_MUTEX
	/* The semaphore is created before the scheduler is started so already
	exists by the time this task executes.
	Attempt to take the semaphore, blocking indefinitely if the mutex is not
	available immediately.  The call to xSemaphoreTake() will only return when
	the semaphore has been successfully obtained so there is no need to check the
	return value.  If any other delay period was used then the code must check
	that xSemaphoreTake() returns pdTRUE before accessing the resource (in this
	case standard out. */
	xSemaphoreTake( xMutex, portMAX_DELAY );
	{
#endif
		/* The following line will only execute once the semaphore has been
		successfully obtained - so standard out can be accessed freely. */
		sprintf( cBuffer, "%s", pcString );
		HAL_UART_Transmit(&hlpuart1,(uint8_t *)cBuffer,strlen(cBuffer),HAL_MAX_DELAY);
		#ifdef USE_MUTEX
	}
	xSemaphoreGive( xMutex );
#endif

}

void Error_handler(void)
{
	while(1);
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

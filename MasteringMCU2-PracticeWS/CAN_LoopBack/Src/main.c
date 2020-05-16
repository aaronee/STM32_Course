/**
 *
 * Author: Aaron Huynh - Created on: Apr. 9, 2020
 *
 * File name: main.c
 * Project name: CAN_LoopBack
 *
 * Description:
 *      Chap14vid4 to Chap14vid12
 *      Implement bcCAN in loop mode to test the CAN Tx function
 *      Setting CAN controller
 *      observe data on oscilloscope
 */
#include "main.h"
#include <string.h>
#include <stdio.h>

void Error_handler(void);
void Sysclk_Init(void);
void GPIO_Init(void);
void UART_Init(void);
void CAN1_Init(void);
void CAN1_Tx (uint8_t data[], uint32_t mailbox);

RCC_OscInitTypeDef OscInit_Param;
RCC_ClkInitTypeDef ClkInit_Param;
UART_HandleTypeDef huart3;
CAN_HandleTypeDef hcan1;

uint8_t mesg[20] = {'H','e','l','l','o'};
uint32_t CANTX_mailbox; /* tpTxMailbox pointer to a variable where the function will return
the TxMailbox used to store the Tx message.*/


int main(void)
{
	HAL_Init();
	Sysclk_Init();
	GPIO_Init();
	UART_Init();
	CAN1_Init();
	HAL_CAN_Start(&hcan1); // move CAN bus to NORMAL mode from INITIALIZE mode
	CAN1_Tx(mesg, CANTX_mailbox);
	memset(mesg,0,sizeof(mesg));

	while(1)
	{
//		memset(mesg,0,sizeof(mesg));
//		sprintf((char *)mesg,"SYSCLK: %lu \r\n",HAL_RCC_GetSysClockFreq());
//		HAL_UART_Transmit(&huart3, (uint8_t *) mesg, strlen((char *)mesg), HAL_MAX_DELAY);
//
//		memset(mesg,0,sizeof(mesg));
//		sprintf((char *)mesg,"HCLK: %lu \r\n",HAL_RCC_GetHCLKFreq());
//		HAL_UART_Transmit(&huart3, (uint8_t *) mesg, strlen((char *)mesg), HAL_MAX_DELAY);
//
//		memset(mesg,0,sizeof(mesg));
//		sprintf((char *)mesg,"PCLK1: %lu \r\n",HAL_RCC_GetPCLK1Freq());
//		HAL_UART_Transmit(&huart3, (uint8_t *) mesg, strlen((char *)mesg), HAL_MAX_DELAY);
//
//		memset(mesg,0,sizeof(mesg));
//		sprintf((char *)mesg,"PCLK2: %lu \r\n",HAL_RCC_GetPCLK2Freq());
//		HAL_UART_Transmit(&huart3, (uint8_t *) mesg, strlen((char *)mesg), HAL_MAX_DELAY);
//		HAL_Delay(500);
	}
}

void Error_handler(void)
{
	while(1);
}
void Sysclk_Init(void)
{
	//HSI-PLL-50Hz
	memset(&OscInit_Param,0,sizeof(OscInit_Param));
	OscInit_Param.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	OscInit_Param.HSIState= RCC_HSI_ON;
	OscInit_Param.HSICalibrationValue= RCC_HSICALIBRATION_DEFAULT;

	OscInit_Param.PLL.PLLState= RCC_PLL_ON;
	OscInit_Param.PLL.PLLSource= RCC_PLLSOURCE_HSI;
	OscInit_Param.PLL.PLLM= 4;
	OscInit_Param.PLL.PLLN= 50;
	OscInit_Param.PLL.PLLR= RCC_PLLR_DIV4;
	OscInit_Param.PLL.PLLQ= RCC_PLLR_DIV2;
	OscInit_Param.PLL.PLLP= RCC_PLLR_DIV2;

	if(HAL_RCC_OscConfig(&OscInit_Param) != HAL_OK)
	{
		Error_handler();
	}

	memset(&ClkInit_Param,0,sizeof(ClkInit_Param));
	ClkInit_Param.ClockType= RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
							| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	ClkInit_Param.SYSCLKSource= RCC_SYSCLKSOURCE_PLLCLK;
	ClkInit_Param.AHBCLKDivider= RCC_SYSCLK_DIV1;
	ClkInit_Param.APB1CLKDivider= RCC_HCLK_DIV2;
	ClkInit_Param.APB2CLKDivider= RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&ClkInit_Param,FLASH_LATENCY_2) != HAL_OK)
	{
		Error_handler();
	}

	//reconfig the duration of Systick timer 1 micro-second for SYSTICK interrupt
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}
void GPIO_Init(void)
{

}
void UART_Init(void)
{
	memset(&huart3,0,sizeof(huart3));
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	if(HAL_UART_Init(&huart3) != HAL_OK)
	{
		Error_handler();
	}
}
void CAN1_Init(void)
{
	/*Chap14-Vid5,Vid6*/
	hcan1.Instance = CAN1;
	hcan1.Init.AutoBusOff = DISABLE;
	hcan1.Init.AutoRetransmission = ENABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;
	// Setting CAN bit timming http://www.bittiming.can-wiki.info
	hcan1.Init.Prescaler = 5;
	hcan1.Init.Mode = CAN_MODE_LOOPBACK;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;

	if (HAL_CAN_Init(&hcan1) != HAL_OK)
	{
		Error_handler();
	}
}
void CAN1_Tx(uint8_t mesg[], uint32_t mailbox)
{
	CAN_TxHeaderTypeDef TxHeader;
	TxHeader.StdId = 0x65D;
	TxHeader.IDE = CAN_ID_STD; // select standard or extend dataframe
	TxHeader.RTR = CAN_RTR_DATA; // select request or data to transmit
	TxHeader.DLC = 5; // transmit 5bytes

	if (HAL_CAN_AddTxMessage(&hcan1,&TxHeader,mesg, &mailbox) != HAL_OK)
	{
		Error_handler();
	}
	while(HAL_CAN_IsTxMessagePending(&hcan1, mailbox)); // waiting for transmit completion
	if (HAL_UART_Transmit(&huart3,mesg,strlen((char *)mesg),HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}
}



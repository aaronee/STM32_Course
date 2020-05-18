/*
 * main.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 *      Udemy - DMA Programming
 *
 *      Goal: lpuart1, dma, tx from scratch
 */
#include "stm32l4r5xx.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

#define OFFSET 0x800
#define DATALOCAD (SRAM1_BASE + OFFSET)
#define DATALOC (uint8_t *)(SRAM1_BASE + OFFSET)

void Error_Handler(void);
void GPIO_Init(void);
void DMA_Init(void);
void UART_Init(void);

void send_data(uint8_t *buffdata);
uint8_t data[]="helloworld";

int main(void)
{
	GPIO_Init();
	UART_Init();
	DMA_Init();
	memcpy(DATALOC,data,10);

	while(!((1UL<<7UL) & (LPUART1->ISR)));
	LPUART1->ICR |= (1UL<<6UL); //clear TC flag
	DMA1_Channel1->CCR |= (1UL<<0UL);	// DMA1 channel 1 enable
	while((LPUART1->ISR>>6UL) & 1);

	while(1);
	return 0;
}

void GPIO_Init(void)
{


	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // enable clk for syscfg register

	/*Enable button B1 - PC13*/
	//configure B1 characteristics
	GPIOC->MODER &= ~(GPIO_MODER_MODE13_0|GPIO_MODER_MODE13_1); // set moder 13 to input mode
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD13_0|GPIO_PUPDR_PUPD13_1); // no pull

	// config pin 13 port C to EXTI line 13
	SYSCFG->EXTICR[3] &= ~(0xF << 4UL); // clear the bits
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;  // configure in SYSCFG register

	//configure edge detection
	EXTI->FTSR1 |= EXTI_FTSR1_FT13; // detect falling edge from exti line 13

	//enable interrupt of gpio pin
	EXTI->IMR1 |= EXTI_IMR1_IM13; // un-mask interrupt from exti line 13

	//enable nvic for gpio pin
	__NVIC_EnableIRQ(EXTI15_10_IRQn); // enable exti line 15to10 in NVIC
	__NVIC_SetPriority(EXTI15_10_IRQn,15);

}

void DMA_Init(void)
{
//	__HAL_RCC_DMAMUX1_CLK_ENABLE();
//	__HAL_RCC_DMA1_CLK_ENABLE();
//
//	hDMA1_M2M_stream.Instance = DMA1_Channel1;
//	hDMA1_M2M_stream.Init.Direction = DMA_MEMORY_TO_MEMORY;
//	hDMA1_M2M_stream.Init.PeriphInc = DMA_PINC_DISABLE;
//	hDMA1_M2M_stream.Init.MemInc = DMA_MINC_DISABLE;
//	hDMA1_M2M_stream.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//	hDMA1_M2M_stream.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//	hDMA1_M2M_stream.Init.Mode = DMA_NORMAL;
//	hDMA1_M2M_stream.Init.Priority = DMA_PRIORITY_LOW;
//	hDMA1_M2M_stream.Init.Request = DMA_REQUEST_MEM2MEM;
//
//	if(HAL_DMA_Init(&hDMA1_M2M_stream) != HAL_OK)
//	{
//		Error_Handler();
//	}
	RCC->AHB1ENR |= RCC_AHB1ENR_DMAMUX1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	DMAMUX1_Channel0->CCR |= 35; // config dmamux1 channel 0 ( DMA1_channel1 ) to lpuart1_tx request

	DMA1_Channel1->CCR &= ~(1UL<<14UL); // mem2mem disable
	DMA1_Channel1->CCR &= ~(0x3<<10UL); // 8bit memory size
	DMA1_Channel1->CCR &= ~(0x3<<8UL);	// 8bit peripheral size
	DMA1_Channel1->CCR 		|= (1UL<<7UL);	// mem increase
	DMA1_Channel1->CCR 		|= (1UL<<4UL);	// read from mem to peri
	DMA1_Channel1->CCR 		|= (1UL<<1UL);	// transfercomplete interrupt enable
	DMA1_Channel1->CPAR		|= 0x40008028;	// address of LPUART1 Tx
	DMA1_Channel1->CNDTR	|= 10;
	DMA1_Channel1->CMAR		|= SRAM1_BASE + OFFSET;


	//enable NVIC
	__NVIC_EnableIRQ(DMA1_Channel1_IRQn);


}
void UART_Init(void)
{
	/*!<enable clk for vddio2 gpio for lpuart1 and its io */
	RCC->APB1ENR1 	|= RCC_APB1ENR1_PWREN;//enable power reg
	PWR->CR2 		|= PWR_CR2_IOSV; //VDDIO2 is valid
	RCC->APB1ENR2 	|= RCC_APB1ENR2_LPUART1EN;//enable lpuart1 clk

	/*!<config the gpio for lpuart tx*/
	//enable GPIOG
	RCC->AHB2ENR 	|= RCC_AHB2ENR_GPIOGEN;
	//configure PG7 as TX
	GPIOG->MODER 	|= GPIO_MODER_MODE7_1;
	GPIOG->MODER	&= ~GPIO_MODER_MODE7_0; //alternative function
	GPIOG->PUPDR 	&= ~GPIO_PUPDR_PUPD7_1;
	GPIOG->PUPDR 	|= GPIO_PUPDR_PUPD7_0;// pull-up 01
	GPIOG->OTYPER 	&= ~(GPIO_OTYPER_IDR_7); // push-pull
	GPIOG->OSPEEDR 	&= ~(GPIO_OSPEEDER_OSPEEDR7_1|GPIO_OSPEEDER_OSPEEDR7_0); // slow
	GPIOG->AFR[0] 	|= GPIO_AFRL_AFSEL7_3;
	GPIOG->AFR[0]	&= ~(GPIO_AFRL_AFSEL7_2|GPIO_AFRL_AFSEL7_1|GPIO_AFRL_AFSEL7_0); // AF8 in AFRLOW

	/*!<config lpuart1 characteristic>*/
	LPUART1->BRR |= 0x1A0AA; // 9600baud with 16bit oversampling + 4Mhz uart clk
	LPUART1->CR3 |= (1UL<<7UL); // enable DMA transmition

	//keep the rest default

	/*!<enable tx engine & enable uart>*/
	LPUART1->CR1 |= (1UL<<3UL); // transmit enable
	LPUART1->CR1 |= (1UL<<0UL); // Uart enable
}
void send_data(uint8_t *buffdata)
{
	// check if transmit data status register is empty; TXE is 1
	// put a byte to DR if ok
	while(!((1UL<<7UL) & (LPUART1->ISR)));

	for(uint8_t i=0; i< strlen((char*)buffdata);i++)
	{
		LPUART1->TDR = data[i];
		while(!((1UL<<7UL) & (LPUART1->ISR)));
	}
}

void Error_Handler(void)
{
	while(1);
}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

//
//
//
//
//
//
//

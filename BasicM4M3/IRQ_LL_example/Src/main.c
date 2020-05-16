/******************************************************************************************/
/*                                Name: USART                                             */
/*                                Author: Khoa Huynh                                      */
/*                                Date: 30-July-2019                                      */
/*                                Desciption:                                             */
/*-Perform the communicate via UART on Nucleo STM32L4R5ZITx                               */
/*                                                                                        */
/******************************************************************************************/
/*--Header--------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32l4r5xx.h"
/*--Define--------------------------------------------------------------------------------*/
/*--Global-Variable---------------------------------------------------------------------- */
/*--Function Prototypes-------------------------------------------------------------------*/
void delay (void);
void initUSART3(void);
void initRCC(void);
void blinkBLUE(void);
void initPORT(void);
int putchar(int);
/*--Main----------------------------------------------------------------------------------*/
int main()
{
  initRCC();
  initPORT();
  initUSART3();
  //char my_mes[] = "this is Khoa ";
  char my_mes[] = "This is USART3 \n";
  while(1)
  {
	  uint8_t data_len = strlen(my_mes);
	  for (int i = 0; i < data_len; i++)
	  {
		  putchar(my_mes[i]);
	  }

  }
} // EO Main

/*--Function------------------------------------------------------------------------------*/
/************************************************/
/*             Name: initUSART3                 */
/*-Initiating USART3 for STM32L4R5xx            */
/*-Oversampling 8 (OVER8=default/0)             */
/*-Baudrate: 9600-USARTDIV=4Mhz/9600=0x1A0(416) */
/*                                              */
/************************************************/
void initUSART3(void)
{
  USART3->BRR |= 0x1A0U;  //select baudrate for USART3=9600
  USART3->CR1 |= 0x9U;  // enable USART3 - Transmit Enable
}
/************************************************/
/*             Name: initRCC                    */
/*-Initiating RCC for STM32L4R5xx               */
/*-MCU run on MSI clock 4Mhz(reset value)       */
/*-Open Clock Gate for USART3                   */
/*-Open AHB2 clockgate                          */
/*                                              */
/************************************************/
void initRCC(void)
{
  RCC->APB1ENR1 |= (1UL<<18); //USART3 clock gate enable
  RCC->AHB2ENR |= (1UL<<1); 	// Enable PB
  RCC->AHB2ENR |= (1UL<<3); //Enable PD
}
/************************************************/
/*             Name: Blink LED                  */
/*-Blink LED on port B 7 with delay()           */
/*                                              */
/************************************************/
void blinkBLUE(void)
{
	delay();
	GPIOB->ODR ^= (1UL<<7); // PB7 output = 1
	delay();
	GPIOB->ODR |= (1UL<<7); // PB7 output = 1
}
/************************************************/
/*             Name: initPORTB                  */
/*-initial PORTB                                */
/*                                              */
/************************************************/
void initPORT(void)
{
  GPIOB->MODER ^= (1UL<<15); // Config PB7 general output
  GPIOB->ODR |= (1UL<<7);    // PB7 output = 1
  GPIOD->MODER ^= (1UL<<16); // Config PD8 alternate output-USART3 TX
  GPIOD->MODER |= (1UL<<17); // Config PD8 alternate output-USART3 TX
  GPIOD->OTYPER = 0x00000000U; // All pin push-pull
  GPIOD->AFR[1] = 0x07; // USART3_TX alternate function to pin 8 AFRH
}
/************************************************/
/*             Name: putchar                    */
/*-printf() from stdlib use putchar() for output*/
/* putchar() is device specific, so we need to  */
/* supply it                                    */
/*                                              */
/************************************************/
int putchar(int c)
{
	while(!((USART3->ISR) & (1<<7)));
	USART3->TDR = c;
	return(c);
}
/************************************************/
/*             Name: delay	                    */
/*-delay for a amount of time                   */
/*                                              */
/************************************************/
void delay (void)
{
	for (int i=0; i<50000; i++);
}

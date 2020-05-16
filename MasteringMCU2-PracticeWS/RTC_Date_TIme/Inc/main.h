/*
 * main.h
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_


#include "stm32l4xx_hal.h" //chap5vid17

#define SYS_CLOCK_FREQ_50MHZ 50
#define SYS_CLOCK_FREQ_84MHZ 84
#define SYS_CLOCK_FREQ_120MHZ 120

extern void Error_handler(void);

#endif /* INC_MAIN_H_ */

/*
 -----------------------------------------------------------
 * Module: HMI_ECU Application
 *
 * File: HMI_ECU.h
 *
 * Description: Application Header File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */
#ifndef HMI_ECU_H_
#define HMI_ECU_H_

#include "uart.h"
#include "timer.h"

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Definitions					     |
 -------------------------------------------------------------
 */

#define TIMER_INIT_VALUE	(0u)
#define TIMER_COMPARE_VALUE (7812u)

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Types Definition					 |
 -------------------------------------------------------------
 */

Timer_ConfigType TIMER_configuartions =
{
TIMER_INIT_VALUE,
TIMER_COMPARE_VALUE,
TIMER1_ID,
F_CPU_1024,
COMPARE_MODE
};

UART_ConfigType UART_configurations =
{
UART_8BIT_DATA,
NO_PARITY,
UART_1_STOP_BIT,
BAUD_9600
};

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Functions Prototypes				 |
 -------------------------------------------------------------
 */

/*----------------------------------------------------------*/

/* Timer call back function
 *
 * Description: Call back function
 *
 * Return: void
 *
 */
void HMI_timerCallBack(void);

/*----------------------------------------------------------*/

#endif /* HMI_ECU_H_ */

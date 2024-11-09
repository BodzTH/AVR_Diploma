/*
 -----------------------------------------------------------
 * Module: Control_ECU Application
 *
 * File: Control_ECU.h
 *
 * Description: Application Header File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_

#include "uart.h"
#include "twi.h"
#include "timer.h"

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Definitions					     |
 -------------------------------------------------------------
 */

#define MY_TWI_ADDRESS		(0b00000010u)
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

TWI_ConfigType TWI_conigurations =
{
MY_TWI_ADDRESS,
FULL_SPEED_MODE_400KBITS /* Full speed mode for EEPROM */
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
void CTRL_timerCallBack(void);

/*----------------------------------------------------------*/

#endif /* CONTROL_ECU_H_ */

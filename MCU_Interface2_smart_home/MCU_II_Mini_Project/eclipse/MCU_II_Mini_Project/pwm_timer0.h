/*
 -----------------------------------------------------------
 * Module: PWM TIMER0
 *
 * File: pwm_timer0.h
 *
 * Description: PWM TIMER0 Driver Header File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#ifndef PWM_TIMER0_H_
#define PWM_TIMER0_H_

#include "std_types.h"

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Definitions					       	 |
 -------------------------------------------------------------
 */

/*Fast PWM Modes*/
#define PWM_MODE_NON_INVERTING 0
#define PWM_MODE_INVERTING     1

#define PWM_TIMER0_PRESCALER_NO   1
#define PWM_TIMER0_PRESCALER_8    2
#define PWM_TIMER0_PRESCALER_64   3
#define PWM_TIMER0_PRESCALER_256  4
#define PWM_TIMER0_PRESCALER_1024 5

#define PWM_TIMER0_PORT 	PORTB_ID
#define PWM_TIMER0_PIN  	PIN3_ID

#define PWM_TIMER0_MODE      PWM_MODE_NON_INVERTING
#define PWM_TIMER0_PRESCALER PWM_TIMER0_PRESCALER_1024

#define TIMER0_MAX_COUNT 255
#define MIN_DUTY_CYCLE 	 0
#define MAX_DUTY_CYCLE 	 100

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Types Declaration					 |
 -------------------------------------------------------------
 */

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Functions Prototypes				 |
 -------------------------------------------------------------
 */

/*----------------------------------------------------------*/

/* Initialize Timer0 in PWM mode
 *
 * Description: Initializes Timer0 in PWM mode and sets the required duty cycle
 * Prescaler: F_CPU/1024
 * Non-inverting mode
 * The function configures OC0 as the output pin.
 *
 * Pram: Adjusting PWM duty cycle
 *
 * Return: void
 *
 */
void PWM_Timer0_Start(uint8 duty_cycle);

/*----------------------------------------------------------*/

#endif /* PWM_TIMER0_H_ */

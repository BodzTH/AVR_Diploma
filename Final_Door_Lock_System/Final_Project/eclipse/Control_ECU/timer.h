/*
 -----------------------------------------------------------
 * Module: Timer
 *
 * File: timer.h
 *
 * Description: Timer Driver Header File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Types Declaration					 |
 -------------------------------------------------------------
 */
typedef enum
{
	TIMER0_ID, TIMER1_ID, TIMER2_ID
} Timer_ID_Type;

typedef enum
{
	/*Timer0 and Timer1 Clock Select*/
	NO_CLOCK,
	F_CPU_CLOCK,
	F_CPU_8,
	F_CPU_64,
	F_CPU_256,
	F_CPU_1024,
	EXTERNAL_CLOCK_FALLING,
	EXTERNAL_CLOCK_RISING,

	/*Timer2 Clock Select*/
	TIMER2_NO_CLOCK = 0x00,
	TIMER2_F_CPU_CLOCK,
	TIMER2_F_CPU_8,
	TIMER2_F_CPU_32,
	TIMER2_F_CPU_64,
	TIMER2_F_CPU_128,
	TIMER2_F_CPU_256,
	TIMER2_F_CPU_1024,
} Timer_ClockType;

typedef enum
{
	NORMAL_MODE, /* Overflow mode */
	COMPARE_MODE, /* Clear Timer on Compare Match mode TOP=OCR0/OCR1/OCR2 */

} Timer_ModeType;

typedef struct
{
	uint16 timer_InitialValue;
	uint16 timer_compare_MatchValue; /*it will be used in compare mode only*/
	Timer_ID_Type timer_ID;
	Timer_ClockType timer_clock;
	Timer_ModeType timer_mode;
} Timer_ConfigType;

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Functions Prototypes				 |
 -------------------------------------------------------------
 */

/*----------------------------------------------------------*/

/* Initialize Timer Module
 *
 * Description: Function to initialize the Timer driver
 *
 * Pram: pointer to the configuration structure with type Timer_ConfigType.
 *
 * Return: void
 *
 */
void Timer_init(const Timer_ConfigType *Config_Ptr);

/*----------------------------------------------------------*/

/* Disable Specific Timer Module
 *
 * Description: Function to disable the Timer via Timer_ID.
 *
 * Pram: Timer_ID
 *
 * Return: void
 *
 */
void Timer_deInit(Timer_ID_Type timer_type);

/*----------------------------------------------------------*/

/* Timer Callback Function
 *
 * Description: Function to set the Call Back function address to the required Timer.
 *
 * Pram: pointer to Call Back function and Timer Id you want to set The Callback to it.
 *
 * Return: void
 *
 */
void Timer_setCallBack(void (*a_ptr)(void), Timer_ID_Type a_timer_ID);

/*----------------------------------------------------------*/

#endif /* TIMER_H_ */

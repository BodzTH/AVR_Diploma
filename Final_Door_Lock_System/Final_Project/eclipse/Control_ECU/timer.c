/*
 -----------------------------------------------------------
 * Module: Timer
 *
 * File: timer.c
 *
 * Description: Timer Driver Source File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */
#include "timer.h"         /* To use timer type declarations and function prototypes */
#include <avr/io.h>        /* To use TIMERs Registers */
#include <avr/interrupt.h> /* For TIMERs ISR */

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Global Variables				     |
 -------------------------------------------------------------
 */
/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr_TIMER0)(void) = NULL_PTR; /* A global pointer to function, to point at call back function address for Timer0 */
static volatile void (*g_callBackPtr_TIMER1)(void) = NULL_PTR; /* A global pointer to function, to point at call back function address for Timer1 */
static volatile void (*g_callBackPtr_TIMER2)(void) = NULL_PTR; /* A global pointer to function, to point at call back function address for Timer2 */

/*
 -------------------------------------------------------------
 | 	 	 	 	    Interrupt Service Routines               |
 -------------------------------------------------------------
 */

ISR(TIMER0_OVF_vect) /* Timer0 Overflow interrupt request */
{
    if (g_callBackPtr_TIMER0 != NULL_PTR) /* If pointer doesn't point at NULL */
    {
        (*g_callBackPtr_TIMER0)(); /*  As interrupt request, call back the pointer to function */
    }
}
ISR(TIMER0_COMP_vect) /* Timer0 Compare value interrupt request */
{
    if (g_callBackPtr_TIMER0 != NULL_PTR) /* If pointer doesn't point at NULL */
    {
        (*g_callBackPtr_TIMER0)(); /*  As interrupt request, call back the pointer to function */
    }
}
ISR(TIMER1_OVF_vect) /* Timer1 Overflow interrupt request */

{
    if (g_callBackPtr_TIMER1 != NULL_PTR) /* If pointer doesn't point at NULL */
    {
        (*g_callBackPtr_TIMER1)(); /*  As interrupt request, call back the pointer to function */
    }
}
ISR(TIMER1_COMPA_vect) /* Timer1 Compare value interrupt request */
{
    if (g_callBackPtr_TIMER1 != NULL_PTR) /* If pointer doesn't point at NULL */
    {
        (*g_callBackPtr_TIMER1)(); /*  As interrupt request, call back the pointer to function */
    }
}
ISR(TIMER2_OVF_vect) /* Timer2 Overflow interrupt request */
{
    if (g_callBackPtr_TIMER2 != NULL_PTR) /* If pointer doesn't point at NULL */
    {
        (*g_callBackPtr_TIMER2)(); /*  As interrupt request, call back the pointer to function */
    }
}
ISR(TIMER2_COMP_vect) /* Timer2 Compare value interrupt request */
{
    if (g_callBackPtr_TIMER2 != NULL_PTR) /* If pointer doesn't point at NULL */
    {
        (*g_callBackPtr_TIMER2)(); /*  As interrupt request, call back the pointer to function */
    }
}

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Functions Definitions				 |
 -------------------------------------------------------------
 */

/*----------------------------------------------------------*/

/* Initialize Timer Module
 *
 * Description: Initialize specific Timer module via timer_ID
 *
 * Pram: Address of configuration structure with type Timer_ConfigType.
 *
 * Return: void
 *
 */
void Timer_init(const Timer_ConfigType *Config_Ptr)
{
    switch (Config_Ptr->timer_ID) /* Check for timer ID */
    {
    case TIMER0_ID:
        TCNT0 = Config_Ptr->timer_InitialValue;     /* Assign Timer0 initial value */
        TCCR0 = (1 << FOC0);                        /* As this is a non-PWM mode set Timer0 Force Output bit */
        TIMSK |= (1 << TOIE0);                      /* Enable Timer0 overflow interrupt by default */
        if (Config_Ptr->timer_mode == COMPARE_MODE) /* If Compare value mode */
        {
            TCCR0 |= (1 << WGM01);                       /* Select compare mode bits WGM00= 0, WGM01= 1 */
            OCR0 = Config_Ptr->timer_compare_MatchValue; /* Assign Timer0 Compare value */
            TIMSK &= ~(1 << TOIE0);                      /* Disable Timer0 Overflow interrupt */
            TIMSK |= (1 << OCIE0);                       /* Enable Timer0 Compare value interrupt */
        }
        TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_clock & 0x07); /* Select Timer0 Clock prescale to start start counting */
        break;
    case TIMER1_ID:
        TCNT1 = Config_Ptr->timer_InitialValue;     /* Assign Timer1 initial value */
        TCCR1A = (1 << FOC1A) | (1 << FOC1B);       /* As this is a non-PWM mode set Timer1 Force Output Compare A:B bits */
        TIMSK |= (1 << TOIE1);                      /* Enable Timer1 overflow interrupt by default */
        if (Config_Ptr->timer_mode == COMPARE_MODE) /* If Compare value mode */
        {
            TCCR1B = (1 << WGM12);                        /* Select compare mode bits WGM10= 0, WGM11= 0, WGM12= 1, WGM13= 0 */
            OCR1A = Config_Ptr->timer_compare_MatchValue; /* Assign Timer1 Compare value */
            TIMSK &= ~(1 << TOIE1);                       /* Disable Timer1 Overflow interrupt */
            TIMSK |= (1 << OCIE1A);                       /* Enable Timer1 Compare value interrupt */
        }
        TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->timer_clock & 0x07); /* Select Timer1 Clock prescale to start start counting */
        break;

    case TIMER2_ID:
        TCNT2 = Config_Ptr->timer_InitialValue;     /* Assign Timer2 initial value */
        TCCR2 = (1 << FOC2);                        /* As this is a non-PWM mode set Timer2 Force Output Compare bit */
        TIMSK |= (1 << TOIE2);                      /* Enable Timer2 overflow interrupt by default */
        if (Config_Ptr->timer_mode == COMPARE_MODE) /* If Compare value mode */
        {
            TCCR2 |= (1 << WGM21);                       /* Select compare mode bits WGM20= 0, WGM21= 1 */
            OCR2 = Config_Ptr->timer_compare_MatchValue; /* Assign Timer2 Compare value */
            TIMSK &= ~(1 << TOIE2);                      /* Disable Timer2 Overflow interrupt */
            TIMSK |= (1 << OCIE2);                       /* Enable Timer2 Compare value interrupt */
        }
        TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->timer_clock & 0x07); /* Select Timer2 Clock prescale to start start counting */
        break;
    default:
        /* Do Nothing */
        break;
    }
}

/*----------------------------------------------------------*/

/* Disable Timer Module
 *
 * Description: Disable specific Timer module via Timer_ID.
 *
 * Pram: Timer ID
 *
 * Return: void
 *
 */
void Timer_deInit(Timer_ID_Type timer_type)
{
    switch (timer_type) /* Check for timer ID */
    {
    case TIMER0_ID:
        TIMSK &= ~(1 << TOIE0); /* Disable Timer0 Overflow interrupt */
        TIMSK &= ~(1 << OCIE0); /* Disable Timer0 Compare value interrupt */
        /* Clear All Timer0 Registers */
        TCCR0 = 0;
        TCNT0 = 0;
        g_callBackPtr_TIMER0 = NULL_PTR; /* Assign Timer0 pointer to point to NULL*/
        break;
    case TIMER1_ID:
        TIMSK &= ~(1 << TOIE1);  /* Disable Timer1 Overflow interrupt */
        TIMSK &= ~(1 << OCIE1A); /* Disable Timer1 Compare value interrupt */
        /* Clear All Timer1 Registers */
        TCCR1A = 0;
        TCCR1B = 0;
        TCNT1 = 0;
        g_callBackPtr_TIMER1 = NULL_PTR; /* Assign Timer1 pointer to point to NULL*/
        break;
    case TIMER2_ID:
        TIMSK &= ~(1 << TOIE2); /* Disable Timer2 Overflow interrupt */
        TIMSK &= ~(1 << OCIE2); /* Disable Timer2 Compare value interrupt */
        /* Clear All Timer2 Registers */
        TCCR2 = 0;
        TCNT2 = 0;
        g_callBackPtr_TIMER2 = NULL_PTR; /* Assign Timer1 pointer to point to NULL*/
        break;
    default:
        /* Do Nothing */
        break;
    }
}

/*----------------------------------------------------------*/

/* Timer Callback Function
 *
 * Description: Assign pointer to point at passed call back function address argument via a_timer_id
 *
 * Pram: Timer ID and address of call back function
 *
 * Return: void
 *
 */
void Timer_setCallBack(void (*a_ptr)(void), Timer_ID_Type a_timer_ID)
{
    /* Save the address of the Call back function in TimerX global variable */
    switch (a_timer_ID)
    {
    case TIMER0_ID:
        /* Save the address of the Call back function in g_callBackPtr_TIMER0 global variable */
        g_callBackPtr_TIMER0 = a_ptr;
        break;
    case TIMER1_ID:
        /* Save the address of the Call back function in g_callBackPtr_TIMER1 global variable */
        g_callBackPtr_TIMER1 = a_ptr;
        break;
    case TIMER2_ID:
        /* Save the address of the Call back function in g_callBackPtr_TIMER2 global variable */
        g_callBackPtr_TIMER2 = a_ptr;
        break;
    default:
        /* Do Nothing */
        break;
    }
}

/*----------------------------------------------------------*/

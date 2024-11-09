/*
 -----------------------------------------------------------
 * Module: 
 *
 * File: memory_mapped_reg.h
 *
 * Description: 
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#ifndef ATMEGA32_MEMORY_MAPPED_REG_H_
#define ATMEGA32_MEMORY_MAPPED_REG_H_

#include "std_types.h"

/*
 These registers are memory mapped as 0x20 offset to the address of direct access peripheral registers
 and the registers address can be accessed with assembly (IN/OUT) instructions
 */

/*Status register*/
#define SREG	(*(volatile uint8 *)(0x5F))

/*Stack pointer registers*/
#define SPH	(*(volatile uint8 *)(0x5E))
#define SPL (*(volatile uint8 *)(0x5D))

/*Interrupt registers*/
#define GICR	(*(volatile uint8 *)(0x5B))
#define GIFR	(*(volatile uint8 *)(0x5A))
#define TIMSK	(*(volatile uint8 *)(0x59))
#define TIFR	(*(volatile uint8 *)(0x58))
#define MCUCR	(*(volatile uint8 *)(0x5B))

/*Timer0 registers*/

/*Timer1 registers*/

/*Timer2 registers*/

/* Port A */
#define PINA	(*(volatile uint8 *)(0x39))
#define DDRA	(*(volatile uint8 *)(0x3A))
#define PORTA	(*(volatile uint8 *)(0x39))

/* Port B */
#define PINB    (*(volatile uint8 *)(0x36))
#define DDRB    (*(volatile uint8 *)(0x37))
#define PORTB   (*(volatile uint8 *)(0x38))

/* Port C */
#define PINC    (*(volatile uint8 *)(0x33))
#define DDRC    (*(volatile uint8 *)(0x34))
#define PORTC   (*(volatile uint8 *)(0x35))

/* Port D */
#define PIND    (*(volatile uint8 *)(0x30))
#define DDRD    (*(volatile uint8 *)(0x31))
#define PORTD   (*(volatile uint8 *)(0x32))

#define ADMUX	(*(volatile uint8 *)(0x27))
#define REFS1	7
#define REFS0	6
#define ADLR	5

#define ADCSRA	(*(volatile uint8 *)(0x26))
#define ADEN	7
#define ADSC	6
#define ADATE	5
#define ADIF	4
#define ADIE	3
#define ADPS2	2
#define ADPS1	1
#define ADPS0	0

#define SFIOR	(*(volatile uint8 *)(0x50))

#define ADC		(*(volatile uint8 *)(0x24))

#endif /* ATMEGA32_MEMORY_MAPPED_REG_H_ */

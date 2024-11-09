/*
 -----------------------------------------------------------
 * Module: PIR
 *
 * File: pir.h
 *
 * Description: PIR Driver Header File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#ifndef PIR_H_
#define PIR_H_

#include "std_types.h"
/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Definitions					     |
 -------------------------------------------------------------
 */
#define PIR_PORT PORTC_ID
#define PIR_PIN  PIN2_ID

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Functions Prototypes				 |
 -------------------------------------------------------------
 */

/*----------------------------------------------------------*/

/* Initialize PIR
 *
 * Description: Function to initialize the PIR driver.
 *
 * Return: void
 *
 */
void PIR_init(void);

/*----------------------------------------------------------*/

/* To get state of PIR
 *
 * Description: Function to return PIR State
 *
 * Return: uint8
 *
 */
uint8 PIR_getState(void);

/*----------------------------------------------------------*/

#endif /* PIR_H_ */

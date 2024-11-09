/*
 -----------------------------------------------------------
 * Module: PIR
 *
 * File: pir.c
 *
 * Description: PIR Driver Source File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#include "pir.h"
#include "gpio.h"

/*----------------------------------------------------------*/

/* Initialize PIR
 *
 * Description: Function to initialize the PIR driver.
 *
 * Return: void
 *
 */
void PIR_init(void)
{
	/* Setup PIR sensor PIN as input */
	GPIO_setupPinDirection(PIR_PORT, PIR_PIN, PIN_INPUT);
}

/*----------------------------------------------------------*/

/* To get state of PIR
 *
 * Description: Function to return PIR State
 *
 * Return: uint8
 *
 */
uint8 PIR_getState(void)
{
	/* Return current pin value */
	return GPIO_readPin(PIR_PORT, PIR_PIN);
}
/*----------------------------------------------------------*/

/*
 -----------------------------------------------------------
 * Module: Buzzer
 *
 * File: buzzer.c
 *
 * Description: Buzzer Driver Source File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#include "buzzer.h"
#include "gpio.h"

/*----------------------------------------------------------*/

/* Initialize Buzzer
 *
 * Description: Initializes the buzzer pin direction and turn off the buzzer.
 *
 * Return: void
 *
 */
void Buzzer_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}

/*----------------------------------------------------------*/

/* Turn on Buzzer
 *
 * Description: Activates the buzzer.
 *
 * Return: void
 *
 */
void Buzzer_on(void)
{
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}

/*----------------------------------------------------------*/

/* Turns off Buzzer
 *
 * Description: Deactivates the buzzer.
 *
 * Return: void
 *
 */
void Buzzer_off(void)
{
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}

/*----------------------------------------------------------*/

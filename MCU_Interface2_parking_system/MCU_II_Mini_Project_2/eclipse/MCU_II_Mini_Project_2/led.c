/*
 -----------------------------------------------------------
 * Module: LED
 *
 * File: led.c
 *
 * Description: LED Driver Source File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#include "led.h"
#include "gpio.h"

/*----------------------------------------------------------*/

/* Initialize LEDs
 *
 * Description: Initializes all LEDs (red, green, blue) pins direction.
 * Turn off all the LEDs
 *
 * Return: void
 *
 */
void LEDS_init(void)
{
	GPIO_setupPinDirection(LED_PORT_SELECT, LED_RED, PIN_OUTPUT);
	GPIO_setupPinDirection(LED_PORT_SELECT, LED_GREEN, PIN_OUTPUT);
	GPIO_setupPinDirection(LED_PORT_SELECT, LED_BLUE, PIN_OUTPUT);
#if LOGIC_CONNECTION
	GPIO_writePin(LED_PORT_SELECT, LED_RED, LOGIC_LOW);
	GPIO_writePin(LED_PORT_SELECT, LED_GREEN, LOGIC_LOW);
	GPIO_writePin(LED_PORT_SELECT, LED_BLUE, LOGIC_LOW);
#else
	GPIO_writePin(LED_PORT_SELECT, LED_RED, LOGIC_HIGH);
	GPIO_writePin(LED_PORT_SELECT, LED_GREEN, LOGIC_HIGH);
	GPIO_writePin(LED_PORT_SELECT, LED_BLUE, LOGIC_HIGH);
#endif
}

/*----------------------------------------------------------*/

/* Turns on LED
 *
 * Description: Turns on the specified LED.
 *
 * Pram: ID of the LED PIN
 *
 * Return: void
 *
 */
void LED_on(LED_ID id)
{
#if LOGIC_CONNECTION
	GPIO_writePin(LED_PORT_SELECT, id, LOGIC_HIGH);
#else
	GPIO_writePin(LED_PORT_SELECT, id, LOGIC_LOW);
#endif
}

/*----------------------------------------------------------*/

/* Turns on LED
 *
 * Description: Turns off the specified LED.
 *
 * Pram: ID of the LED PIN
 *
 * Return: void
 *
 */
void LED_off(LED_ID id)
{
#if LOGIC_CONNECTION
	GPIO_writePin(LED_PORT_SELECT, id, LOGIC_LOW);
#else
	GPIO_writePin(LED_PORT_SELECT, id, LOGIC_HIGH);
#endif
}

/*----------------------------------------------------------*/

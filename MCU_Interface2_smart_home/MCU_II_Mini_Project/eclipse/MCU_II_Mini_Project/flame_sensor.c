/*
 -----------------------------------------------------------
 * Module: Flame Sensor
 *
 * File: flame_sensor.c
 *
 * Description: Flame Sensor Driver Source File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#include "flame_sensor.h"
#include "gpio.h"

/*----------------------------------------------------------*/

/* Initialize Flame Sensor
 *
 * Description: Initializes the flame sensor pin direction
 *
 * Return: void
 *
 */
void FlameSensor_init(void)
{
	GPIO_setupPinDirection(FLAME_SENSOR_PORT, FLAME_SENSOR_OUT, PIN_INPUT);
}

/*----------------------------------------------------------*/

/* Rotates DC Motor
 *
 * Description: Reads the value from the flame sensor and returns it.
 *
 * Return: void
 *
 */
uint8 FlameSensor_getValue(void)
{
	if (GPIO_readPin(FLAME_SENSOR_PORT, FLAME_SENSOR_OUT))
	{
		return LOGIC_HIGH;
	}
	else
	{
		return LOGIC_LOW;
	}
}

/*----------------------------------------------------------*/

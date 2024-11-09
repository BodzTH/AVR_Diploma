/*
 -----------------------------------------------------------
 * Module: Flame Sensor
 *
 * File: flame_sensor.h
 *
 * Description: Flame Sensor Driver Header File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#ifndef FLAME_SENSOR_H_
#define FLAME_SENSOR_H_

#include "std_types.h"

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Definitions					       	 |
 -------------------------------------------------------------
 */

#define FLAME_SENSOR_PORT PORTD_ID
#define FLAME_SENSOR_OUT  PIN2_ID

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

/* Initialize Flame Sensor
 *
 * Description: Initializes the flame sensor pin direction
 *
 * Return: void
 *
 */
void FlameSensor_init(void);

/*----------------------------------------------------------*/

/* Rotates DC Motor
 *
 * Description: Reads the value from the flame sensor and returns it.
 *
 * Return: void
 *
 */
uint8 FlameSensor_getValue(void);

/*----------------------------------------------------------*/

#endif /* FLAME_SENSOR_H_ */

/*
 -----------------------------------------------------------
 * Module: LDR
 *
 * File: ldr_sensor.h
 *
 * Description: LDR Driver Header File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#ifndef LDR_SENSOR_H_
#define LDR_SENSOR_H_

#include "std_types.h"

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Definitions					       	 |
 -------------------------------------------------------------
 */
#define LDR_SENSOR_CHANNEL_ID 0
#define LDR_SENSOR_MAX_VOLT_VALUE 2.56
#define LDR_SENSOR_MAX_LIGHT_INTENSITY 100

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

/* Gets the current light intensity
 *
 * Description: Reads the LDR sensor value and returns the light intensity
 *
 * Return: unsigned 16-bit integer of light intensity value
 *
 */
uint16 LDR_getLightIntensity(void);

/*----------------------------------------------------------*/
#endif /* LDR_SENSOR_H_ */

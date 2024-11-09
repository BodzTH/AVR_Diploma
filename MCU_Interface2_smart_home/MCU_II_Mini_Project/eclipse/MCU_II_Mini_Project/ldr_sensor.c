/*
 -----------------------------------------------------------
 * Module: LDR
 *
 * File: ldr_sensor.c
 *
 * Description: LDR Driver Source File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#include "ldr_sensor.h"
#include "adc.h"

/* Gets the current light intensity
 *
 * Description: Reads the LDR sensor value and returns the light intensity
 *
 * Return: unsigned 16-bit integer of light intensity value
 *
 */
uint16 LDR_getLightIntensity(void)
{
	ADC_init();
	uint16 ldr_value = 0;
	uint16 adc_value = 0;

	/* Read ADC channel where the LDR sensor is connected */
	adc_value = ADC_readChannel(LDR_SENSOR_CHANNEL_ID);
	/* Calculate the LDR from the ADC value*/
	ldr_value = (uint16) (((uint64) adc_value * LDR_SENSOR_MAX_LIGHT_INTENSITY * ADC_REF_VOLT_VALUE)
			/ (ADC_MAXIMUM_VALUE * LDR_SENSOR_MAX_VOLT_VALUE));

	return ldr_value;
}

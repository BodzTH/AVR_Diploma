/*
 -----------------------------------------------------------
 * Module: Application
 *
 * File: car_parking_system.c
 *
 * Description: Application Source File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#include "led.h"
#include "lcd.h"
#include "buzzer.h"
#include "hc_sr_04.h"
#include <util/delay.h>

int main(void)
{
	LEDS_init();
	LCD_init();
	Buzzer_init();
	Ultrasonic_init();
	uint16 ultrasonic_distance;
	while (1)
	{
		ultrasonic_distance = Ultrasonic_readDistance();
		LCD_displayStringRowColumn(0, 1, "Distance=");
		LCD_intgerToString(ultrasonic_distance);
		LCD_displayString(" cm  ");
		if (ultrasonic_distance > 20)
		{
			LED_off(LED_RED);
			LED_off(LED_GREEN);
			LED_off(LED_BLUE);
		}
		else if (ultrasonic_distance > 15)
		{
			LED_on(LED_RED);
			LED_off(LED_GREEN);
			LED_off(LED_BLUE);
		}
		else if (ultrasonic_distance > 10)
		{
			LED_on(LED_RED);
			LED_on(LED_GREEN);
			LED_off(LED_BLUE);
		}
		else if (ultrasonic_distance > 5)
		{
			LED_on(LED_RED);
			LED_on(LED_GREEN);
			LED_on(LED_BLUE);

		}
		else
		{
			Buzzer_on();
			LCD_displayStringRowColumn(1, 6, "STOP");
			while (ultrasonic_distance <= 5)
			{
				LCD_displayStringRowColumn(0, 9, "=");
				LCD_intgerToString(ultrasonic_distance);
				LED_on(LED_RED);
				LED_on(LED_GREEN);
				LED_on(LED_BLUE);
				_delay_ms(300);
				LED_off(LED_RED);
				LED_off(LED_GREEN);
				LED_off(LED_BLUE);
				_delay_ms(300);

				ultrasonic_distance = Ultrasonic_readDistance();
			}
			LCD_displayStringRowColumn(1, 6, "    ");
			Buzzer_off();
		}
	}
}

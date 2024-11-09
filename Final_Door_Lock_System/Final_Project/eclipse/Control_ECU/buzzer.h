/*
 -----------------------------------------------------------
 * Module: Buzzer
 *
 * File: buzzer.h
 *
 * Description: Buzzer Driver Header File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#ifndef BUZZER_H_
#define BUZZER_H_

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Definitions					       	 |
 -------------------------------------------------------------
 */
#define BUZZER_PORT PORTC_ID
#define BUZZER_PIN  PIN7_ID

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Functions Prototypes				 |
 -------------------------------------------------------------
 */

/*----------------------------------------------------------*/

/* Initialize Buzzer
 *
 * Description: Initializes the buzzer pin direction and turn off the buzzer.
 *
 * Return: void
 *
 */
void Buzzer_init(void);

/*----------------------------------------------------------*/

/* Turn on Buzzer
 *
 * Description: Activates the buzzer.
 *
 * Return: void
 *
 */
void Buzzer_on(void);

/*----------------------------------------------------------*/

/* Turns off Buzzer
 *
 * Description: Deactivates the buzzer.
 *
 * Return: void
 *
 */
void Buzzer_off(void);

/*----------------------------------------------------------*/

#endif /* BUZZER_H_ */

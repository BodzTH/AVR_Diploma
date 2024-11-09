/*
 -----------------------------------------------------------
 * Module: DC Motor
 *
 * File: dc_motor.h
 *
 * Description: DC Motor Driver Header File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Definitions					       	 |
 -------------------------------------------------------------
 */

#define DC_MOTOR_PORT_IN PORTD_ID
#define DC_MOTOR_IN1  PIN6_ID
#define DC_MOTOR_IN2  PIN7_ID

#define DC_MOTOR_PORT_EN PORTB_ID
#define DC_MOTOR_EN1  PIN3_ID


/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Types Declaration					 |
 -------------------------------------------------------------
 */
typedef enum
{
	OFF,CW,ACW
}DcMotor_State;

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Functions Prototypes				 |
 -------------------------------------------------------------
 */

/*----------------------------------------------------------*/

/* Initialize DC Motor
 *
 * Description: Initializes the DC motor by setting the direction for the motor pins and stopping the
 * motor at the beginning.
 *
 * Return: void
 *
 */
void DcMotor_Init(void);

/*----------------------------------------------------------*/

/* Rotates DC Motor
 *
 * Description: Controls the motor's state (Clockwise/Anti-Clockwise/Stop) and adjusts the speed based
 * on the input duty cycle.
 *
 * Pram1: state: Enum or uint8 that indicates motor direction (CW, A-CW, Stop).
 * Pram2: speed: Motor speed in percentage (0 to 100%).
 *
 * Return: void
 *
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

/*----------------------------------------------------------*/


#endif /* DC_MOTOR_H_ */

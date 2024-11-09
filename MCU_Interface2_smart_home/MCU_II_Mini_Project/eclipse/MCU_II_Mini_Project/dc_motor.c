/*
 -----------------------------------------------------------
 * Module: DC Motor
 *
 * File: dc_motor.c
 *
 * Description: DC Motor Driver Source File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#include "dc_motor.h"
#include "gpio.h"
#include "pwm_timer0.h"

/*----------------------------------------------------------*/

/* Initialize DC Motor
 *
 * Description: Initializes the DC motor by setting the direction for the motor pins and stopping the
 * motor at the beginning.
 *
 * Return: void
 *
 */
void DcMotor_Init(void)
{
	GPIO_setupPinDirection(DC_MOTOR_PORT, DC_MOTOR_IN1, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT, DC_MOTOR_IN2, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT, DC_MOTOR_EN1, PIN_OUTPUT);

#if LOGIC_CONNECTION
	GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN1, LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN2, LOGIC_LOW);

#else
	GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN1, LOGIC_HIGH);
	GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN2, LOGIC_HIGH);
#endif
}

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
void DcMotor_Rotate(DcMotor_State state, uint8 speed)
{
	PWM_Timer0_Start(speed);
	switch (state)
	{
	case OFF:
		/* Stop the motor */
#if LOGIC_CONNECTION
		GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN1, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN2, LOGIC_LOW);
#else
	GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN1, LOGIC_HIGH);
	GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN2, LOGIC_HIGH);
#endif
		break;
	case CW:
		/* Rotates the motor CW */
#if LOGIC_CONNECTION
		GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN1, LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN2, LOGIC_LOW);
#else
	GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN1, LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN2, LOGIC_HIGH);
#endif
		break;
	case ACW:
		/* Rotates the motor A-CW */
#if LOGIC_CONNECTION
		GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN1, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN2, LOGIC_HIGH);
#else
		GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN1, LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT, DC_MOTOR_IN2, LOGIC_LOW);
#endif
		break;
	default:
		/* Do Nothing */
		break;
	}
}

/*----------------------------------------------------------*/


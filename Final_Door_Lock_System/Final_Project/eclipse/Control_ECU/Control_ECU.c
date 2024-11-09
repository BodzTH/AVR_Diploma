#include "Control_ECU.h"
#include "dc_motor.h"
#include "pir.h"
#include "external_eeprom.h"
#include "buzzer.h"
#include <util/delay.h>
#include <avr/io.h>

/* System settings */
#define PASSWORD_LENGTH 				(5u)
#define MAX_PASSWORD_MISMATCHED 		(3u)
#define EEPROM_START_MEM_LOC_ADDRESS 	(0x0311u)

/* Message codes between ECUs */
#define CTRL_READY_TO_RECEIVE 			(0x20u)
#define PASSWORD_MATCHED 				(0x21u)
#define PASSWORD_MISMATCHED 			(0x22u)
#define LOCK_SYSTEM 					(0x23u)
#define HMI_DOOR_UNLOCKED 				(0x24u)
#define HMI_DOOR_LOCKING 				(0x25u)
#define CHANGE_PASSWORD 				(0x26u)
#define OPEN_DOOR 						(0x27u)

/* PIR sensor motion states */
#define PIR_MOTION_DETECTED				TRUE
#define PIR_MOTION_NOT_DETECTED			FALSE

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Global Variables   				 |
 -------------------------------------------------------------
 */
volatile uint16 g_ticks = 0; /* global variable to hold number of ticks, each tick is 1 second */

/* System states */
volatile enum {
	STANDBY,		/* System is waiting at the main loop, ready for HMI UART message */
	DOOR_UNLOCKING, /* System unlocking the door for 15 seconds */
	DOOR_UNLOCKED,	/* The door unlocked and waiting for people to pass */
	DOOR_LOCKING,	/* System locking the door for 15 seconds */
	SYSTEM_LOCKED	/* The system is in a locked state for security */
} g_sys_state;

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 Functions Definition				 |
 -------------------------------------------------------------
 */

/*----------------------------------------------------------*/

/* Initialize CTRL_ECU App modules
 *
 * Description: Initializes All needed CTRL_ECU modules
 *
 * Return: void
 *
 */
void CTRL_Init(void)
{
	UART_init(&UART_configurations);				  /* Initialiaze UART module */
	TWI_init(&TWI_conigurations);					  /* Initialiaze I2C module */
	Timer_setCallBack(CTRL_timerCallBack, TIMER1_ID); /* Passing the address of the call back funciton to TIMER1 */
	Timer_init(&TIMER_configuartions);				  /* Start TIMER1 module */
	PIR_init();										  /* Initialiaze PIR sensor */
	DcMotor_Init();									  /* Initialiaze DC motor */
	Buzzer_init();									  /* Initialiaze Buzzer */
	/* Enable global interrupts */
	SREG |= (1 << 7);
}

/*----------------------------------------------------------*/

/* Control ECU Save Password
 *
 * Description: Save password in the EEPROM
 *
 * Return: void
 *
 */
void CTRL_savePassword(void)
{
	uint8 uart_recieve_buffer = 0;				/* To store the byte in the recieve buffer in a varaible */
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) /* Loop for number of PASSWORD_LENGTH characters */
	{
		uart_recieve_buffer = UART_recieveByte();								 /* Store in uart_recieve_buffer the uart recieve buffer byte sent from HMI */
		EEPROM_writeByte(EEPROM_START_MEM_LOC_ADDRESS + i, uart_recieve_buffer); /* Save the uart_recieve_buffer byte in EEPROM_START_MEM_LOC_ADDRESS + i(next byte address) */
		_delay_ms(10);															 /* EEPROM read/write time */
		UART_sendByte(CTRL_READY_TO_RECEIVE);									 /* Send Control ECU is ready to receive */
	}
}

/*----------------------------------------------------------*/

/* Control ECU Change Password
 *
 * Description: Check for old password and if password matched
 * change password and save the new one in the EEPROM
 *
 * Return: void
 *
 */
void CTRL_changePassword(void)
{
	static uint8 mismatched_counter = 0;		/* To statically hold number of mismatched passwords */
	uint8 eeprom_value;							/* To store current password character in EEPROM */
	uint8 uart_recieve_buffer;					/* To store the byte in the recieve buffer in a varaible */
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) /* Loop for number of PASSWORD_LENGTH characters */
	{
		EEPROM_readByte(EEPROM_START_MEM_LOC_ADDRESS + i, &eeprom_value); /* Store current address value in password1 at i index */
		_delay_ms(10);													  /* EEPROM read/write time*/
		uart_recieve_buffer = UART_recieveByte();						  /* Store in password2 at i index the uart recieve buffer byte sent from HMI */
		if (uart_recieve_buffer != eeprom_value)						  /* If a single character mismatch detected */
		{
			mismatched_counter++;							   /* Count up number of mismatches */
			if (mismatched_counter == MAX_PASSWORD_MISMATCHED) /* If max password mismatches reached */
			{
				mismatched_counter = 0;		 /* Clear mismatch counter for next password entries */
				g_ticks = 0;				 /* Clear Timer ticks to start counting from 0 seconds */
				g_sys_state = SYSTEM_LOCKED; /* Switch to SYSTEM_LOCKED state */
				UART_sendByte(LOCK_SYSTEM);	 /* Send LOCK_SYSTEM to HMI */
				Buzzer_on();				 /* Turn on buzzer as system is in locked state */
			}
			else /* Else max password mismatches not reached send PASSWORD_MISMATCHED to HMI */
			{
				UART_sendByte(PASSWORD_MISMATCHED);
			}
			return;
		}
		UART_sendByte(CTRL_READY_TO_RECEIVE); /* Send Control ECU is ready to receive */
	}
	/* As for loop executed without return, execute the code below */
	mismatched_counter = 0;		 /* Clear mismatch counter for next password entries */
	CTRL_savePassword();
}

/*----------------------------------------------------------*/

/* Control ECU Unlock Door
 *
 * Description: Check for old password and if password matched
 * Open the Door
 *
 * Return: void
 *
 */
void CTRL_unlockDoor(void)
{
	static uint8 mismatched_counter = 0;		/* To statically hold number of mismatched passwords */
	uint8 eeprom_value;							/* To store current password character in EEPROM */
	uint8 uart_recieve_buffer;					/* To store the byte in the recieve buffer in a varaible */
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) /* Loop for number of PASSWORD_LENGTH characters */
	{
		EEPROM_readByte(EEPROM_START_MEM_LOC_ADDRESS + i, &eeprom_value); /* Store current address value in password1 at i index */
		_delay_ms(10);													  /* EEPROM read/write time*/
		uart_recieve_buffer = UART_recieveByte();						  /* Store in uart_recieve_buffer the uart recieve buffer byte sent from HMI */
		if (uart_recieve_buffer != eeprom_value)						  /* If a single character mismatch detected */
		{
			mismatched_counter++;							   /* Count up number of mismatches */
			if (mismatched_counter == MAX_PASSWORD_MISMATCHED) /* If max password mismatches reached */
			{
				mismatched_counter = 0;		 /* Clear mismatch counter for next password entries */
				g_ticks = 0;				 /* Clear Timer ticks to start counting from 0 seconds */
				g_sys_state = SYSTEM_LOCKED; /* Switch to SYSTEM_LOCKED state */
				UART_sendByte(LOCK_SYSTEM);	 /* Send LOCK_SYSTEM to HMI */
				Buzzer_on();				 /* Turn on buzzer as system is in locked state */
			}
			else /* Else max password mismatches not reached send PASSWORD_MISMATCHED to HMI */
			{
				UART_sendByte(PASSWORD_MISMATCHED);
			}
			return;
		}
		UART_sendByte(CTRL_READY_TO_RECEIVE); /* Send Control ECU is ready to receive */
	}
	/* As for loop executed without return, execute the code below */
	mismatched_counter = 0;		 /* Clear mismatch counter for next password entries */
	g_ticks = 0;				  /* Clear Timer ticks to start counting from 0 seconds */
	g_sys_state = DOOR_UNLOCKING; /* Switch to DOOR_UNLOCKING state */
	DcMotor_Rotate(CW, 100);	  /* Rotate DC motor clockwise to open the door */
}

/*----------------------------------------------------------*/

/*  Control ECU Timer call back function
 *
 * Description: Call back function
 *
 * Return: void
 *
 */
void CTRL_timerCallBack(void)
{
	g_ticks++;			 /* Increment timer ticks as timer interrupts each 1 second */
	switch (g_sys_state) /* Check for current state*/
	{
	case DOOR_UNLOCKING:  /* Door Unlocking state, DC motor rotates CW */
		if (g_ticks == 15)/* If 15 second passed */
		{
			g_ticks = 0;				 /* Clear Timer ticks to start counting from 0 seconds */
			g_sys_state = DOOR_UNLOCKED; /* Switch to DOOR_UNLOCKING state */
			DcMotor_Rotate(OFF, 0);		 /* Stop DC motor to wait for people to pass */
		}
		break;

	case DOOR_UNLOCKED:								   /* Door Unlocked state, DC motor is off for people to pass */
		if (PIR_getState() == PIR_MOTION_NOT_DETECTED) /* If PIR sensor not detecting a motion close the door */
		{
			g_ticks = 0;					 /* Clear Timer ticks to start counting from 0 seconds */
			g_sys_state = DOOR_LOCKING;		 /* Switch to DOOR_LOCKING state */
			UART_sendByte(HMI_DOOR_LOCKING); /* Send HMI_DOOR_LOCKING to HMI */
			DcMotor_Rotate(ACW, 100);		 /* Rotate DC Motor clockwise to close the door */
		}

		break;

	case DOOR_LOCKING: /* Door Locking state, DC motor rotates ACW */
		if (g_ticks == 15) /* If 15 second passed */
		{
			g_sys_state = STANDBY;	/* Switch to STANDBY state */
			DcMotor_Rotate(OFF, 0); /* Stop DC Motor as door is closed */
		}
		break;

	case SYSTEM_LOCKED: /* System Locked state, System is locked and does not read UART message */
		if (g_ticks == 60) /* If 1 minute passed */
		{
			g_sys_state = STANDBY; /* Switch to STANDBY state */
			Buzzer_off();		   /* Turn off buzzer as system returned to Standby state */
		}
		break;
	default:
			/* Do Nothing as it's stand by state */
		break;
	}
}

/*----------------------------------------------------------*/

/*
 -------------------------------------------------------------
 | 	 	 	 	 	 	 MAIN FUNCTION      				 |
 -------------------------------------------------------------
 */

int main(void)
{
	CTRL_Init();				   /* Initialize modules needed for Control ECU*/
	CTRL_savePassword();		   /* As start of the program call CTRL save password function */
	uint8 uart_recieve_buffer = 0; /* To store the byte in the recieve buffer in a varaible */
	while (1)					   /* Main Loop*/
	{
		if (g_sys_state != SYSTEM_LOCKED) /* If system state is not locked */
		{
			uart_recieve_buffer = UART_recieveByte(); /* Store in uart_recieve_buffer the uart recieve buffer byte sent from HMI */
			if (uart_recieve_buffer == OPEN_DOOR)	  /* If the uart_recieve_buffer byte message code is OPEN_DOOR */
			{
				CTRL_unlockDoor(); /* Call CTRL unlcok door function */
			}
			else if (uart_recieve_buffer == CHANGE_PASSWORD) /* If the uart_recieve_buffer byte message code is CHANGE_PASSWORD */
			{
				CTRL_changePassword(); /* Call CTRL change password function */
			}
		}
	}
}

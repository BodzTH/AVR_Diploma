/*
 -----------------------------------------------------------
 * Module: HMI_ECU Application
 *
 * File: HMI_ECU.c
 *
 * Description: Application Source File
 *
 * Author: Bodz
 -----------------------------------------------------------
 */

#include "HMI_ECU.h"
#include "keypad.h"
#include "lcd.h"
#include <util/delay.h>
#include <avr/io.h>

/* System settings */
#define PASSWORD_LENGTH             (5u)
#define ENTER_KEY                   ('=')
#define OPEN_DOOR_KEY               ('+')
#define CHANGE_PASSWORD_KEY         ('-')

/* Message codes between ECUs */
#define CTRL_READY_TO_RECEIVE       (0x20u)
#define PASSWORD_MATCHED            (0x21u)
#define PASSWORD_MISMATCHED         (0x22u)
#define LOCK_SYSTEM                 (0x23u)
#define HMI_DOOR_UNLOCKED	        (0x24u)
#define HMI_DOOR_LOCKING            (0x25u)
#define CHANGE_PASSWORD             (0x26u)
#define OPEN_DOOR                   (0x27u)

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

/* Initialize HMI_ECU App modules
 *
 * Description: Initializes All needed HMI_ECU modules
 *
 * Return: void
 *
 */
void HMI_Init(void)
{
	UART_init(&UART_configurations);				 /* Initialiaze UART module */
	LCD_init();										 /* Initialiaze LCD module */
	Timer_setCallBack(HMI_timerCallBack, TIMER1_ID); /* Passing the address of the call back funciton to TIMER1 */
	Timer_init(&TIMER_configuartions);				 /* Start TIMER1 module */
	/* Enable global interrupts */
	SREG |= (1 << 7);
}

/*----------------------------------------------------------*/

/* HMI ECU Save Password
 *
 * Description: Save password in the EEPROM
 *
 * Return: void
 *
 */
void HMI_savePassword(void)
{
	uint8 key;							/* Stores keypad pressed key value */
	uint8 password_character_index = 0; /* Holds the index of character in the password array */
	/* Reverse initialized to prevent password less than 5 characters */
	uint8 password1[PASSWORD_LENGTH];		/* To store first password characters */
	uint8 password2[PASSWORD_LENGTH];		/* To store second password characters */
	boolean first_password_entry = TRUE;	/* To check for first password entry */
	boolean matched_input_password = FALSE; /* To check for matched re-entered password */
	/* Display to user first password entery request */
	LCD_clearScreen();					  /* Clear screen */
	LCD_displayString("Plz enter pass:"); /* Display message in the first row */
	LCD_moveCursor(1, 0);				  /* Move cursor to second row to display input characters */
	while (!matched_input_password)		  /* Stay in loop until matched input password flag is true */
	{
		key = KEYPAD_getPressedKey();	  /* Read pressed key and store it in key variable */
		if (first_password_entry == TRUE) /* If first password entry */
		{
			if (key >= 0 && key <= 9 && password_character_index < PASSWORD_LENGTH) /* Read 0 to 9 values only while password length within range */
			{
				LCD_displayCharacter('*');				   /* Display asterisk as user input a character */
				password1[password_character_index] = key; /* Assign password index with key value */
				password_character_index++;				   /* Increment password_character_index for next character input */
			}
			else if (key == ENTER_KEY && password_character_index == PASSWORD_LENGTH) /* If key value is the Enter key and same password length entered */
			{
				/* Display to user second password entery request */
				LCD_clearScreen();					   /* Clear screen */
				LCD_displayString("Plz re-enter the"); /* Display message in the first row */
				LCD_moveCursor(1, 0);				   /* Move cursor to second row to display second message */
				LCD_displayString("same pass: ");	   /* Display message in the second row */
				first_password_entry = FALSE;		   /* Flag first password entry as false for second password entry */
				password_character_index = 0;		   /* Clear password character index to begin from first character */
			}
		}
		else /* Else second password entry */
		{
			if (key >= 0 && key <= 9 && password_character_index < PASSWORD_LENGTH) /* Read 0 to 9 values only while password length within range */
			{
				LCD_displayCharacter('*');				   /* Display asterisk as user input a character */
				password2[password_character_index] = key; /* Assign password index with key value */
				password_character_index++;				   /* Increment password_character_index for next character input */
			}
			else if (key == ENTER_KEY && password_character_index == PASSWORD_LENGTH) /* If key value is the Enter key and same password length entered */
			{
				matched_input_password = TRUE;				/* Flag matched input password as true to quit the while loop */
				password_character_index = 0;				/* Clear password character index to begin from first character */
				for (uint8 i = 0; i < PASSWORD_LENGTH; i++) /* Loop for number of PASSWORD_LENGTH characters */
				{
					if (password1[i] != password2[i]) /* If a single character mismatch detected */
					{
						first_password_entry = TRUE;	/* Flag first password entry as true to begin first password entry again */
						matched_input_password = FALSE; /* Flag matched input password as false to keep reading password entries again */
						/* Display to user that password doesn't match */
						LCD_clearScreen();					   /* Clear screen */
						LCD_displayString("PASSWORD DOESN'T"); /* Display message in the first row */
						LCD_moveCursor(1, 0);				   /* Move cursor to second row to display second message */
						LCD_displayString("	    MATCH!"); /* Display message in the second row */
						_delay_ms(500);						   /* Display for 0.5 second */
						/* Display the password entry message again */
						LCD_clearScreen();					  /* Clear screen */
						LCD_displayString("Plz enter pass:"); /* Display message in the first row */
						LCD_moveCursor(1, 0);				  /* Move cursor to second row to display input characters */
						break;								  /* Break the for loop */
					}
				}
			}
		}
		_delay_ms(500); /* Press time */
	}
	/* As while loop executed, execute the code below */
	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) /* Save new password */
	{
		UART_sendByte(password1[i]); /* Send indexed character byte to CTRL */
		while (UART_recieveByte() != CTRL_READY_TO_RECEIVE); /* Wait until CTRL sends ready to receive code */
	}
	/* Display the main options */
	LCD_clearScreen();									 /* Clear screen */
	LCD_displayStringRowColumn(0, 0, "+ : Open Door");	 /* Display open door option in first row */
	LCD_displayStringRowColumn(1, 0, "- : Change Pass"); /* Display change password option in second row */
}

/*----------------------------------------------------------*/

/* HMI ECU Change Password
 *
 * Description: Check for old password and if password matched
 * change password and save the new one in the EEPROM
 *
 * Return: void
 *
 */
void HMI_changePassword(void)
{
	uint8 key;										   /* Stores keypad pressed key value */
	uint8 uart_recieve_buffer = 0;					   /*To store the byte in the recieve buffer in a varaible */
	uint8 password_character_index = 0;				   /* Holds the index of character in the password array */
	uint8 password[PASSWORD_LENGTH];				   /* To store password characters */
	boolean password_entered = FALSE;				   /* To check for user password confirmation */
	/* Display user first password entery request */
	LCD_clearScreen();					/* Clear screen */
	LCD_displayString("Plz enter old"); /* Display message in the first row */
	LCD_moveCursor(1, 0);				/* Move cursor to second row to display second message */
	LCD_displayString("pass: ");		/* Display message in the second row */
	while (!password_entered)			/* Stay in loop until password entred flag is true */
	{
		key = KEYPAD_getPressedKey();											/* Read pressed key and store it in key variable */
		if (key >= 0 && key <= 9 && password_character_index < PASSWORD_LENGTH) /* Read 0 to 9 values only while password length within range */
		{

			LCD_displayCharacter('*');				   /* Display asterisk as user input a character */
			password[password_character_index] = key;  /* Assign password index with key value */
			password_character_index++;				   /* Increment password_character_index for next character input */
		}
		else if (key == ENTER_KEY && password_character_index == PASSWORD_LENGTH) /* If key value is the Enter key and same password length entered */
		{
			password_entered = TRUE; /* Flag password entered as true to quit the while loop */
		}
		_delay_ms(500); /* Press time */
	}
	UART_sendByte(CHANGE_PASSWORD); /* Send to CTRL change password code to call change password function */

	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) /* Loop for number of PASSWORD_LENGTH characters */
	{
		UART_sendByte(password[i]);						/* Send indexed character byte to CTRL */
		uart_recieve_buffer = UART_recieveByte();		/* store in uart_recieve_buffer the uart recieve buffer byte sent from CTRL */
		if (uart_recieve_buffer == PASSWORD_MISMATCHED) /* If the uart_recieve_buffer byte message code is PASSWORD_MISMATCHED */
		{
			/* Display to user that password is wrong */
			LCD_clearScreen();					 /* Clear screen */
			LCD_displayString("WRONG PASSWORD"); /* Display message in the first row */
			_delay_ms(500);						 /* Display for 0.5 second */
			HMI_changePassword();				 /* Call change password function to begin password entry again */
			return;								 /* Return to quit the function */
		}
		else if (uart_recieve_buffer == LOCK_SYSTEM) /* If the uart_recieve_buffer byte message code is LOCK_SYSTEM */
		{
			g_ticks = 0;				 /* Clear Timer ticks to start counting from 0 seconds */
			g_sys_state = SYSTEM_LOCKED; /* Switch to SYSTEM_LOCKED state */
			/* Display to user that the system is locked */
			LCD_clearScreen(); /* Clear screen */
			LCD_displayString(" SYSTEM LOCKED");  /* Display message in the first row */
			LCD_moveCursor(1, 0);				  /* Move cursor to second row to display second message */
			LCD_displayString(" Wait for 1 min"); /* Display message in the second row */
			return;								  /* Return to quit the function */
		}
		else if (uart_recieve_buffer == CTRL_READY_TO_RECEIVE) /* If the uart_recieve_buffer byte message code is CTRL_READY_TO_RECEIVE */
		{
			/* Do nothing */
		}
		else
		{
			while (UART_recieveByte() != CTRL_READY_TO_RECEIVE); /* Wait until CTRL sends ready to receive code */
		}
	}
	HMI_savePassword(); /* As for loop executed without return, call the save password function */
}
/*----------------------------------------------------------*/

/* HMI ECU Unlock door
 *
 * Description: Check for old password and if password matched
 * Open the Door
 *
 * Return: void
 *
 */
void HMI_unlockDoor(void)
{
	uint8 key;							/* Stores keypad pressed key value */
	uint8 uart_recieve_buffer = 0;		/*To store the byte in the recieve buffer in a varaible */
	uint8 password_character_index = 0; /* Holds the index of character in the password array */
	uint8 password[PASSWORD_LENGTH];	/* To store password characters */
	boolean password_entered = FALSE;	/* To check for user password confirmation */
	/* Display user first password entery request */
	LCD_clearScreen();					/* Clear screen */
	LCD_displayString("Plz enter old"); /* Display message in the first row */
	LCD_moveCursor(1, 0);				/* Move cursor to second row to display second message */
	LCD_displayString("pass: ");		/* Display message in the second row */
	while (!password_entered)			/* Stay in loop until password entred flag is true */
	{
		key = KEYPAD_getPressedKey();											/* Read pressed key and store it in key variable */
		if (key >= 0 && key <= 9 && password_character_index < PASSWORD_LENGTH) /* Read 0 to 9 values only while password length within range */
		{

			LCD_displayCharacter('*');				   /* Display asterisk as user input a character */
			password[password_character_index] = key;  /* Assign password index with key value */
			password_character_index++;				   /* Increment password_character_index for next character input */
		}
		else if (key == ENTER_KEY && password_character_index == PASSWORD_LENGTH) /* If key value is the Enter key and same password length entered */
		{
			password_entered = TRUE; /* Flag password entered as true to quit the while loop */
		}
		_delay_ms(500); /* Press time */
	}
	UART_sendByte(OPEN_DOOR); /* Send to CTRL open door code to call change password function */

	for (uint8 i = 0; i < PASSWORD_LENGTH; i++) /* Loop for number of PASSWORD_LENGTH characters */
	{
		UART_sendByte(password[i]);						/* Send indexed character byte to CTRL */
		uart_recieve_buffer = UART_recieveByte();		/* store in uart_recieve_buffer the uart recieve buffer byte sent from CTRL */
		if (uart_recieve_buffer == PASSWORD_MISMATCHED) /* If the uart_recieve_buffer byte message code is PASSWORD_MISMATCHED */
		{
			/* Display to user that password is wrong */
			LCD_clearScreen();					 /* Clear screen */
			LCD_displayString("WRONG PASSWORD"); /* Display message in the first row */
			_delay_ms(500);						 /* Display for 0.5 second */
			HMI_unlockDoor();					 /* Call unlock door function to begin password entry again */
			return;								 /* Return to quit the function */
		}
		else if (uart_recieve_buffer == LOCK_SYSTEM) /* If the uart_recieve_buffer byte message code is LOCK_SYSTEM */
		{
			g_ticks = 0;				 /* Clear Timer ticks to start counting from 0 seconds */
			g_sys_state = SYSTEM_LOCKED; /* Switch to SYSTEM_LOCKED state */
			/* Display to user that the system is locked */
			LCD_clearScreen(); /* Clear screen */
			LCD_displayString(" SYSTEM LOCKED");  /* Display message in the first row */
			LCD_moveCursor(1, 0);				  /* Move cursor to second row to display second message */
			LCD_displayString(" Wait for 1 min"); /* Display message in the second row */
			return;								  /* Return to quit the function */
		}
		else if (uart_recieve_buffer == CTRL_READY_TO_RECEIVE) /* If the uart_recieve_buffer byte message code is CTRL_READY_TO_RECEIVE */
		{
			/* Do nothing */
		}
		else
		{
			while (UART_recieveByte() != CTRL_READY_TO_RECEIVE); /* Wait until CTRL sends ready to receive code */
		}
	}
	/* As for loop executed without return, execute the code below */
	g_ticks = 0; /* Clear Timer ticks to start counting from 0 seconds */
	g_sys_state = DOOR_UNLOCKING; /* Switch to DOOR_UNLOCKING state */
	/* Display to user that Door unlocking */
	LCD_clearScreen(); /* Clear screen */
	LCD_displayString(" Door Unlocking"); /* Display message in the first row */
	LCD_moveCursor(1, 0);  /* Move cursor to second row to display second message */
	LCD_displayString(" Please Wait"); /* Display message in the second row */
}

/*----------------------------------------------------------*/

/* HMI ECU Timer call back function
 *
 * Description: Call back function
 *
 * Return: void
 *
 */
void HMI_timerCallBack(void)
{
	g_ticks++;			 /* Increment timer ticks as timer interrupts each 1 second */
	switch (g_sys_state) /* Check for current state*/
	{
	case DOOR_UNLOCKING:  /* Door Unlocking state, display to user door unlocking message */
		if (g_ticks == 15)/* If 15 second passed */
		{
			g_ticks = 0;				 /* Clear Timer ticks to start counting from 0 seconds */
			g_sys_state = DOOR_UNLOCKED; /* Switch to DOOR_UNLOCKED state */
			/* Display to user wait for people to enter message */
			LCD_clearScreen();					   /* Clear screen */
			LCD_displayString(" Wait for people"); /* Display message in the first row */
			LCD_moveCursor(1, 0);				   /* Move cursor to second row to display second message */
			LCD_displayString("  To Enter");	   /* Display message in the second row */
		}
		break;

	case DOOR_UNLOCKED:
		while (UART_recieveByte() != HMI_DOOR_LOCKING); /* Wait until CTRL sends door locking code to HMI */
		g_ticks = 0;
		g_ticks = 0;				/* Clear Timer ticks to start counting from 0 seconds */
		g_sys_state = DOOR_LOCKING; /* Switch to DOOR_LOCKING state */
		/* Display to user that door is locking */
		LCD_clearScreen();					 /* Clear screen */
		LCD_displayString("  Door Locking"); /* Display message in the first row */

		break;

	case DOOR_LOCKING:
		if (g_ticks == 15) /* If 15 second passed */
		{
			g_ticks = 0;		   /* Clear Timer ticks to start counting from 0 seconds */
			g_sys_state = STANDBY; /* Switch to STANDBY state */
			/* Display the main options */
			LCD_clearScreen();									 /* Clear screen */
			LCD_displayStringRowColumn(0, 0, "+ : Open Door");	 /* Display open door option in first row */
			LCD_displayStringRowColumn(1, 0, "- : Change Pass"); /* Display change password option in second row */
		}
		break;

	case SYSTEM_LOCKED: /* System Locked state, System is locked and does not read keypad press */

		if (g_ticks == 60) /* If 1 minute passed */
		{
			g_sys_state = STANDBY; /* Switch to STANDBY state */
			/* Display the main options */
			LCD_clearScreen();									 /* Clear screen */
			LCD_displayStringRowColumn(0, 0, "+ : Open Door");	 /* Display open door option in first row */
			LCD_displayStringRowColumn(1, 0, "- : Change Pass"); /* Display change password option in second row */
		}
		break;
	default:
		/* Do Nothing as it's stand by state */
		break;
	}
}
/*----------------------------------------------------------*/

int main(void)
{
	uint8 key;	/* Stores keypad pressed key value */
	HMI_Init(); /* Initialize modules needed for HMI ECU*/
	/* Display Door Lock System welcome message */
	LCD_displayString("Door Lock System"); /* Display message in the first row */
	_delay_ms(500);						   /* Display for 0.5 second */
	HMI_savePassword();					   /* As start of the program call HMI save password function */
	while (1)							   /* Main Loop*/
	{

		if (g_sys_state != SYSTEM_LOCKED) /* If system state is not locked */
		{
			key = KEYPAD_getPressedKey(); /* Read pressed key and store it in key variable */

			if (key == OPEN_DOOR_KEY) /* If key value is the open door key */
			{
				HMI_unlockDoor(); /* Call HMI unlcok door function */
			}
			else if (key == CHANGE_PASSWORD_KEY) /* If key value is the change password key */
			{
				HMI_changePassword(); /* Call HMI change password function */
			}
			_delay_ms(500); /* Press time */
		}
	}
}

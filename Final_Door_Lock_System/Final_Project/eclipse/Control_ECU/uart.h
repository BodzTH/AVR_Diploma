/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                        Types Declarations                                   *
 *******************************************************************************/
typedef enum
{
	UART_5BIT_DATA,
	UART_6BIT_DATA,
	UART_7BIT_DATA,
	UART_8BIT_DATA,
} UART_BitDataType;

typedef enum
{
	NO_PARITY, EVEN_PARITY, ODD_PARITY
} UART_ParityType;

typedef enum
{
	UART_1_STOP_BIT, UART_2_STOP_BITS
} UART_StopBitType;

typedef enum
{
	BAUD_10 = 10,
	BAUD_300 = 300,
	BAUD_600 = 600,
	BAUD_1200 = 1200,
	BAUD_2400 = 2400,
	BAUD_4800 = 4800,
	BAUD_9600 = 9600,
	BAUD_14400 = 14400,
	BAUD_19200 = 19200,
	BAUD_38400 = 38400,
	BAUD_57600 = 57600,
	BAUD_115200 = 115200,
	BAUD_128000 = 128000,
	BAUD_256000 = 256000
} UART_BaudRateType;

typedef struct
{
	UART_BitDataType bit_data;
	UART_ParityType parity;
	UART_StopBitType stop_bit;
	UART_BaudRateType baud_rate;
} UART_ConfigType;

/*******************************************************************************
 *                     Functions Prototypes                                    *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */

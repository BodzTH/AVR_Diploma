#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define zero PORTA = (PORTA & 0x01) | 0x7E;
#define one PORTA = (PORTA & 0x01) | 0x0C;
#define two PORTA = (PORTA & 0x01) | 0xB6;
#define three PORTA = (PORTA & 0x01) | 0x9E;
#define four PORTA = (PORTA & 0x01) | 0xCC;
#define five PORTA = (PORTA & 0x01) | 0xDA;
#define six PORTA = (PORTA & 0x01) | 0xFA;
#define seven PORTA = (PORTA & 0x01) | 0x0E;
#define eight PORTA = (PORTA & 0x01) | 0xFE;
#define nine PORTA = (PORTA & 0x01) | 0xCE;

//Define Variables to Hold each 7-segment value
//Starts from 6 to 1 as Proteus leftmost 7-Segment is 1
unsigned char seconds1stDigit = 0;
unsigned char seconds2ndDigit = 0;
unsigned char mode = 0; /*To Hold Decrementing/Incrementing Mode*/
unsigned char buttonFlag1 = 1; /*A Flag to prevent level signal execution*/
unsigned char buttonFlag2 = 1; /*A Flag to prevent level signal execution*/
unsigned char buttonFlag3 = 1; /*A Flag to prevent level signal execution*/
unsigned char resetFlag = 1; /*A Flag to turn off/on time out Buzzer/Led*/
/*

 void zero()
 {
 PORTA = (PORTA & 0x01) | 0x7E;
 }
 void one()
 {
 PORTA = (PORTA & 0x01) | 0x0C;
 }
 void two()
 {
 PORTA = (PORTA & 0x01) | 0xB6;
 }
 void three()
 {
 PORTA = (PORTA & 0x01) | 0x9E;
 }
 void four()
 {
 PORTA = (PORTA & 0x01) | 0xCC;
 }
 void five()
 {
 PORTA = (PORTA & 0x01) | 0xDA;
 }
 void six()
 {
 PORTA = (PORTA & 0x01) | 0xFA;
 }
 void seven()
 {
 PORTA = (PORTA & 0x01) | 0x0E;
 }
 void eight()
 {
 PORTA = (PORTA & 0x01) | 0xFE;
 }
 void nine()
 {
 PORTA = (PORTA & 0x01) | 0xCE;
 }
 */

void GPIO_Init(void) {
	DDRA = (DDRA & 0x01) | 0xFE; /*Set PA1-PA7 as output pins*/
	DDRB = (DDRB & 0x8F) | 0x70; /*Set PB4-PB6 as output pins*/
	DDRC = (DDRC & 0x38) | 0xC7; /*Set PC0-PC2,PC6,PC7 as output pins*/
	DDRD &= 0xC3; /*Set PD2-PD5 as output pins*/

	PORTA = (PORTA & 0x01) | 0x7E; /*Initialize two 7-segment with 0*/
	PORTC = (PORTC & 0x38) | 0xC1; /*Turn on two 7-segment and write one to count up LED*/
	PORTB |= 0x70; /*Disable any push button input*/
}
void INT01_Init(void) {
	//Raise Interrupt 0,1 Flag with falling edge
	MCUCR = (MCUCR & 0xF0) | 0x0A;
	//Enable INT0,INT1 External Interrupt Request
	GICR |= 0xC0;
}
void TIMER1_Init(void) {
	TCCR1A = (1 << FOC1A); /*non-PWM mode*/
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); /*Mode 4 Top=OCR1A N=1024*/
	TCNT1 = 0; /*Start counting from 0*/
	OCR1A = 15625; /*16M/1024 = 15625Hz, 1 Second after 15625 Clock Tick*/
	TIMSK |= (1 << OCIE1A); /*Enable Timer1A_CTC Mode Interrupt Request*/
}
//Pause Button PD2
ISR( INT0_vect) {
	TCCR1B &= ~(1 << CS12) & ~(1 << CS10); /*Disable Clock source*/
	GIFR |= (1 << INTF0); /*write logic one to clear the INTF0 Flag bit*/
}
//Resume Button PB2
ISR( INT1_vect) {
	TCCR1B |= (1 << CS12) | (1 << CS10); /*Enable Clock source N=1024*/
	GIFR |= (1 << INTF1); /*write logic one to clear the INTF1 Flag bit*/
}
//TIMER1 Interrupt for every 1 second count
ISR( TIMER1_COMPA_vect) {
	SREG |= (1 << 7); /*To enable nested interrupts to pause/reset/resume stop watch at any time*/
	if (!mode) //if count up mode
	{
		resetFlag = 1; /*Enable the reset flag as count mode changed*/
		//increment only if less than 9
		if (seconds1stDigit < 9) {
			seconds1stDigit++;
		}
		//else if to count until 59 seconds before reseting the stopwatch
		else if (seconds2ndDigit == 5 && seconds1stDigit == 9) {
			seconds2ndDigit = seconds1stDigit = 0;
		}
		//else if to count to 9 before clearing seconds1stDigit
		else if (seconds1stDigit == 9) {
			seconds1stDigit = 0;
			seconds2ndDigit++;
		}
		// if statements to count and reset based on max eligible value
		if (seconds2ndDigit == 6) {
			seconds2ndDigit = 0;
		}
	} else {
		//Only decrement if one of the six 7-segment not equal 0
		if (seconds1stDigit != 0 || seconds2ndDigit != 0) {
			seconds1stDigit--;
		}
		//decrement if seconds1stDigit overflows and seconds2ndDigit not equal 0
		if (seconds2ndDigit != 0 && seconds1stDigit == 255) {
			seconds2ndDigit--;
			seconds1stDigit = 9;
		}
		//if all count variables equal 0 and the resetflag is 1,
		//then turn on time out Buzzer/LED
		if (seconds2ndDigit == 0 && seconds1stDigit == 0 && resetFlag) {
			PORTC |= (1 << PC5);
		}
	}
	TIFR |= (1 << OCF1A); /*Write logic one to clear the OCF1A Flag bit*/
}

int main(void) {
	//Enable Global Interrupt Request
	SREG |= (1 << 7);
	GPIO_Init();
	INT01_Init();
	TIMER1_Init();

	while (1) {
		PORTB = (PORTB | 0x70) & ~(1 << PB4);
		if (!(PIND & (1 << PIND4))) /*Reset Button*/
		{
			_delay_ms(10); /*To prevent signal bouncing*/
			if (!(PIND & (1 << PIND4))) {
				if (buttonFlag1) /*To execute rising/falling edge only once*/
				{
					seconds2ndDigit = seconds1stDigit = 0;
					resetFlag = 0; /*As reset button pushed clear the flag as count down mode check for it*/
					PORTC &= ~(1 << PC5); /*As reset button pushed turnoff  time out Buzzer/LED*/
					buttonFlag1 = 0; /*Clear the variable as the button is pushed*/
				}

			}
		} else if (!(PIND & (1 << PIND5))) /*Change count mode button*/
		{
			_delay_ms(10); /*To prevent signal bouncing*/
			if (!(PIND & (1 << PIND5))) {
				if (buttonFlag1) /*To execute rising/falling edge only once*/
				{
					mode ^= 1; /*Toggle count mode*/
					if (!mode) {
						//Turn off count down LED and time out Buzzer/LED
						//and turn on count up LED
						PORTC &= ~(1 << PC1);
						PORTC |= (1 << PC0);
						PORTC &= ~(1 << PC5);
					} else {
						//Turn off count up LED and turn on count down LED
						PORTC &= ~(1 << PC0);
						PORTC |= (1 << PC1);
					}
					buttonFlag1 = 0; /*Clear the variable as the button is pushed*/
				}

			}
		} else {
			buttonFlag1 = 1;
		}
		//SECONDS 1st Digit
		PORTB = (PORTB | 0x70) & ~(1 << PB5);
		if (!(PIND & (1 << PIND4))) /*Decrement button*/
		{
			_delay_ms(10);
			if (!(PIND & (1 << PIND4))) {
				if (buttonFlag2) {
					if (seconds1stDigit != 0) {
						seconds1stDigit--;
					} else if (seconds2ndDigit != 0 && seconds1stDigit == 0) {
						seconds2ndDigit--;
						seconds1stDigit = 9;
					}
					buttonFlag2 = 0;
				}

			}
		} else if (!(PIND & (1 << PIND5))) /*Increment button*/
		{
			_delay_ms(10);
			if (!(PIND & (1 << PIND5))) {
				if (buttonFlag2) {
					if (seconds1stDigit < 9 && seconds2ndDigit <= 5) {
						seconds1stDigit++;
					} else if (seconds2ndDigit < 5 && seconds1stDigit == 9) {
						seconds1stDigit = 0;
						seconds2ndDigit++;
					}
					buttonFlag2 = 0;
					resetFlag = 1;
				}

			}
		} else {
			buttonFlag2 = 1;
		}
		//SECONDS 2nd Digit
		PORTB = (PORTB | 0x70) & ~(1 << PB6);
		if (!(PIND & (1 << PIND4))) /*Decrement button*/
		{
			_delay_ms(10);
			if (!(PIND & (1 << PIND4))) {
				if (buttonFlag3) {
					if (seconds2ndDigit != 0) {
						seconds2ndDigit--;
					}
					buttonFlag3 = 0;
				}

			}
		} else if (!(PIND & (1 << PIND5))) /*Increment button*/
		{
			_delay_ms(10);
			if (!(PIND & (1 << PIND5))) {
				if (buttonFlag3) {
					if (seconds2ndDigit < 5) {
						seconds2ndDigit++;
					}
					buttonFlag3 = 0;
					resetFlag = 1;
				}

			}
		} else {
			buttonFlag3 = 1;
		}

		//Turn off one 7-segment and turn on the targeted 7-segment to write a number into it
		PORTC = (PORTC & 0x3F) | (1 << PC6);
		switch (seconds1stDigit) {
		case 0:
			zero
			break;
		case 1:
			one
			break;
		case 2:
			two
			break;
		case 3:
			three
			break;
		case 4:
			four
			break;
		case 5:
			five
			break;
		case 6:
			six
			break;
		case 7:
			seven
			break;
		case 8:
			eight
			break;
		case 9:
			nine
			break;
		}
		_delay_ms(1); /*Delay 1 millisecond to see the two 7-segment all turned on at the same time for the naked eye*/

		PORTC = (PORTC & 0x3F) | (1 << PC7);
		switch (seconds2ndDigit) {
		case 0:
			zero
			break;
		case 1:
			one
			break;
		case 2:
			two
			break;
		case 3:
			three
			break;
		case 4:
			four
			break;
		case 5:
			five
			break;
		case 6:
			six
			break;
		case 7:
			seven
			break;
		case 8:
			eight
			break;
		case 9:
			nine
			break;
		}
		_delay_ms(1);
	}
}

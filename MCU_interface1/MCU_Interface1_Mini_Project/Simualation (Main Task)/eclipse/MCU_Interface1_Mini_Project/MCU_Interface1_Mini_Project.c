#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//Define Variables to Hold each 7-segment value
//Starts from 6 to 1 as Proteus leftmost 7-Segment is 1
unsigned char seconds6 = 0;
unsigned char seconds5 = 0;
unsigned char minutes4 = 0;
unsigned char minutes3 = 0;
unsigned char hours2 = 0;
unsigned char hours1 = 0;
unsigned char mode = 0; /*To Hold Decrementing/Incrementing Mode*/
unsigned char buttonFlag = 1; /*A Flag to prevent level signal execution*/
unsigned char resetFlag = 1; /*A Flag to turn off/on time out Buzzer/Led*/

void GPIO_Init(void)
{

	DDRA = 0x3F; /*Set PA0-PA5 as output pins and PA6,PA7 as input pins*/
	DDRB = 0x00; /*Set PB0-PB7 as input pins*/
	DDRC = (DDRC & 0x3F) | 0x0F; /*Set PC0-PC3 as output pins and PC6,PC7 as input pins*/
	DDRD = (DDRD & 0x03) | 0x01; /*Set PD0,PD4,PD5 as output pins and PD2,PD3,PD6,PD7 as input pins*/

	PORTA = 0xFF; /*Enable internal pullup resistor for PA6,PA7 and enable all six 7-Segment*/
	PORTC = (PORTC & 0xF0) | 0xC0; /*Enable internal pullup resistor for PC6,PC7 and display 0 on 7-segment*/
	PORTB = 0xFF; /*Enable internal pullup resistor for PB0-PB7*/
	PORTD = (PORTD & 0x02) | 0xD4; /*Enable internal pullup resistor for PD2,PD6,PD7 and turn on PD4*/

}
void INT012_Init(void)
{
	//Raise Interrupt 1 Flag with rising edge
	MCUCR |= (1 << ISC01) | (1 << ISC10) | (1 << ISC11); /*(1 << ISC01) for INT0*/
	//Raise Interrupt 0 Flag with falling edge
	MCUCR &= ~(1 << ISC00);
	//Raise Interrupt 2 Flag with falling edge
	MCUCSR &= ~(1 << ISC2);
	//Enable INT012 External Interrupt Request
	GICR |= 0xE0;
}
void TIMER1_Init(void)
{
	TCCR1A = (1 << FOC1A); /*non-PWM mode*/
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); /*Mode 4 Top=OCR1A N=1024*/
	TCNT1 = 0; /*Start counting from 0*/
	OCR1A = 15625; /*16M/1024 = 15625Hz, 1 Second after 15625 Clock Tick*/
	TIMSK |= (1 << OCIE1A); /*Enable Timer1A_CTC Mode Interrupt Request*/
}
//Reset Button PD2
ISR( INT0_vect)
{
	//Assign all Timer variables to start from 0
	hours1 = hours2 = minutes3 = minutes4 = seconds5 = seconds6 = 0;
	resetFlag = 0; /*As reset button pushed clear the flag as countdown mode check for it*/
	PORTD &= ~(1 << PD0); /*As reset button pushed turnoff  time out Buzzer/LED*/
	GIFR |= (1 << INTF0); /*write logic one to clear the INTF0 Flag bit*/

}
//Pause Button PD3
ISR( INT1_vect)
{
	TCCR1B &= ~(1 << CS12) & ~(1 << CS10); /*Disable Clock source*/
	GIFR |= (1 << INTF1); /*write logic one to clear the INTF1 Flag bit*/
}
//Resume Button PB2
ISR( INT2_vect)
{
	TCCR1B |= (1 << CS12) | (1 << CS10); /*Enable Clock source N=1024*/
	GIFR |= (1 << INTF2); /*write logic one to clear the INTF2 Flag bit*/
}
//TIMER1 Interrupt for every 1 second count
ISR( TIMER1_COMPA_vect)
{
	SREG |= (1 << 7); /*To enable nested interrupts to pause/reset/resume stop watch at any time*/
	if (!mode) //if count up mode
	{
		resetFlag = 1; /*Enable the reset flag as count mode changed*/
		//increment only if less than 9
		if (seconds6 < 9)
		{
			seconds6++;
		}
		//else if to count until 59 seconds before reseting the stopwatch
		else if (hours1 == 9 && hours2 == 9 && minutes3 == 5 && minutes4 == 9
				&& seconds5 == 5 && seconds6 == 9)
		{
			hours1 = hours2 = minutes3 = minutes4 = seconds5 = seconds6 = 0;
		}
		//else if to count to 9 before clearing seconds6
		else if (seconds6 == 9)
		{
			seconds6 = 0;
			seconds5++;
		}
		// if statements to count and reset based on max eligible value
		if (seconds5 == 6)
		{
			seconds5 = 0;
			minutes4++;
		}
		if (minutes4 == 10)
		{
			minutes4 = 0;
			minutes3++;
		}
		if (minutes3 == 6)
		{
			minutes3 = 0;
			hours2++;
		}
		if (hours2 == 10)
		{
			hours2 = 0;
			hours1++;
		}
	}
	else
	{
		//Only decrement if one of the six 7-segment not equal 0
		if (seconds6 != 0 || seconds5 != 0 || minutes4 != 0 || minutes3 != 0
				|| hours2 != 0 || hours1 != 0)
		{
			seconds6--;
		}
		//decrement if seconds6 overflows and seconds5 not equal 0
		if (seconds5 != 0 && seconds6 == 255)
		{
			seconds5--;
			seconds6 = 9;
		}
		//decrement if seconds6 overflows and seconds5 equal 0 and minutes4 not equal 0
		if (minutes4 != 0 && seconds5 == 0 && seconds6 == 255)
		{
			minutes4--;
			seconds5 = 5;
			seconds6 = 9;
		}
		//same logic as previous statement
		if (minutes3 != 0 && minutes4 == 0 && seconds5 == 0 && seconds6 == 255)
		{
			minutes3--;
			minutes4 = 9;
			seconds5 = 5;
			seconds6 = 9;
		}
		//same logic as previous statement
		if (hours2 != 0 && minutes3 == 0 && minutes4 == 0 && seconds5 == 0
				&& seconds6 == 255)
		{
			hours2--;
			minutes3 = 5;
			minutes4 = 9;
			seconds5 = 5;
			seconds6 = 9;
		}
		//same logic as previous statement
		if (hours1 != 0 && hours2 == 0 && minutes3 == 0 && minutes4 == 0
				&& seconds5 == 0 && seconds6 == 255)
		{
			hours1--;
			hours2 = 9;
			minutes3 = 5;
			minutes4 = 9;
			seconds5 = 5;
			seconds6 = 9;
		}
		//if all count variables equal 0 and the resetflag is 1,
		//then turn on time out Buzzer/LED
		if (hours1 == 0 && hours2 == 0 && minutes3 == 0 && minutes4 == 0
				&& seconds5 == 0 && seconds6 == 0 && resetFlag)
		{
			PORTD |= (1 << PD0);
		}
	}
	TIFR |= (1 << OCF1A); /*Write logic one to clear the OCF1A Flag bit*/
}

int main(void)
{
	//Enable Global Interrupt Request
	SREG |= (1 << 7);
	GPIO_Init();
	INT012_Init();
	TIMER1_Init();

	while (1)
	{
		if (!(PINB & (1 << PINB7))) /*check if change mode button pushed*/
		{
			_delay_ms(30); /*To prevent signal bouncing*/
			if (!(PINB & (1 << PINB7)))
			{
				if (buttonFlag) /*To execute rising/falling edge only once*/
				{
					mode ^= 1; /*Toggle count mode*/
					if (!mode)
					{
						//Turn off count down LED and time out Buzzer/LED
						//and turn on count up LED
						PORTD &= ~(1 << PD5);
						PORTD |= (1 << PD4);
						PORTD &= ~(1 << PD0);
					}
					else
					{
						//Turn off count up LED and turn on count down LED
						PORTD &= ~(1 << PD4);
						PORTD |= (1 << PD5);
					}
					buttonFlag = 0; /*Clear the variable as the button is pushed*/
				}
			}
		}
		//HOURS 1st Digit
		else if (!(PINB & (1 << PINB0))) /*Decrement button*/
		{
			_delay_ms(30);
			if (!(PINB & (1 << PINB0)))
			{
				if (buttonFlag)
				{
					if (hours2 != 0)
					{
						hours2--;
					}
					else if (hours1 != 0 && hours2 == 0)
					{
						hours1--;
						hours2 = 9;
					}
					buttonFlag = 0;
				}
			}
		}
		else if (!(PINB & (1 << PINB1))) /*Increment button*/
		{
			_delay_ms(30);
			if (!(PINB & (1 << PINB1)))
			{
				if (buttonFlag)
				{
					if (hours2 < 9)
					{
						hours2++;
					}
					else if (hours1 < 9 && hours2 == 9)
					{
						hours2 = 0;
						hours1++;
					}
					buttonFlag = 0;
					resetFlag = 1;
				}
			}
		}
		//HOURS 2nd Digit
		else if (!(PIND & (1 << PIND6))) /*Decrement button*/
		{
			_delay_ms(30);
			if (!(PIND & (1 << PIND6)))
			{
				if (buttonFlag)
				{

					if (hours1 != 0)
					{
						hours1--;
					}

					buttonFlag = 0;
				}
			}
		}
		else if (!(PIND & (1 << PIND7))) /*Increment button*/
		{
			_delay_ms(30);
			if (!(PIND & (1 << PIND7)))
			{
				if (buttonFlag)
				{
					if (hours1 < 9)
					{
						hours1++;
					}

					buttonFlag = 0;
					resetFlag = 1;
				}
			}
		}
		//MINUTES 1st Digit
		else if (!(PINB & (1 << PINB3))) /*Decrement button*/
		{
			_delay_ms(30);
			if (!(PINB & (1 << PINB3)))
			{
				if (buttonFlag)
				{
					if (minutes4 != 0)
					{
						minutes4--;
					}
					else if (minutes3 != 0 && minutes4 == 0)
					{
						minutes3--;
						minutes4 = 9;
					}
					buttonFlag = 0;
				}
			}
		}
		else if (!(PINB & (1 << PINB4))) /*Increment button*/
		{
			_delay_ms(30);
			if (!(PINB & (1 << PINB4)))
			{
				if (buttonFlag)
				{
					if (minutes4 < 9 && minutes3 <= 5)
					{
						minutes4++;
					}
					else if (minutes3 < 5 && minutes4 == 9)
					{
						minutes4 = 0;
						minutes3++;
					}
					buttonFlag = 0;
					resetFlag = 1;
				}
			}
		}
		//MINUTES 2nd Digit
		else if (!(PINA & (1 << PINA6))) /*Decrement button*/
		{
			_delay_ms(30);
			if (!(PINA & (1 << PINA6)))
			{
				if (buttonFlag)
				{
					if (minutes3 != 0)
					{
						minutes3--;
					}
					buttonFlag = 0;
				}
			}
		}
		else if (!(PINA & (1 << PINA7))) /*Increment button*/
		{
			_delay_ms(30);
			if (!(PINA & (1 << PINA7)))
			{
				if (buttonFlag)
				{
					if (minutes3 < 5)
					{
						minutes3++;
					}
					buttonFlag = 0;
					resetFlag = 1;
				}
			}
		}
		//SECONDS 1st Digit
		else if (!(PINB & (1 << PINB5))) /*Decrement button*/
		{
			_delay_ms(30);
			if (!(PINB & (1 << PINB5)))
			{
				if (buttonFlag)
				{
					if (seconds6 != 0)
					{
						seconds6--;
					}
					else if (seconds5 != 0 && seconds6 == 0)
					{
						seconds5--;
						seconds6 = 9;
					}
					buttonFlag = 0;
				}
			}
		}
		else if (!(PINB & (1 << PINB6))) /*Increment button*/
		{
			_delay_ms(30);
			if (!(PINB & (1 << PINB6)))
			{
				if (buttonFlag)
				{
					if (seconds6 < 9 && seconds5 <= 5)
					{
						seconds6++;
					}
					else if (seconds5 < 5 && seconds6 == 9)
					{
						seconds6 = 0;
						seconds5++;
					}
					buttonFlag = 0;
					resetFlag = 1;
				}
			}
		}
		//SECONDS 2nd Digit
		else if (!(PINC & (1 << PINC6))) /*Decrement button*/
		{
			_delay_ms(30);
			if (!(PINC & (1 << PINC6)))
			{
				if (buttonFlag)
				{
					if (seconds5 != 0)
					{
						seconds5--;
					}
					buttonFlag = 0;
				}
			}
		}
		else if (!(PINC & (1 << PINC7))) /*Increment button*/
		{
			_delay_ms(30);
			if (!(PINC & (1 << PINC7)))
			{
				if (buttonFlag)
				{
					if (seconds5 < 5)
					{
						seconds5++;
					}
					buttonFlag = 0;
					resetFlag = 1;
				}
			}
		}
		else
		{
			buttonFlag = 1; /*if noting pushed set the button flag*/
		}

		//Turn off five 7-segment and turn on the targeted 7-segment to write a number into it
		PORTA = (PORTA & 0xC0) | (1 << PA5);
		PORTC = (PORTC & 0xF0) | (seconds6 & 0x0F);
		_delay_ms(1); /*Delay 1 millisecond to see the six 7-segment all turned on at the same time for the naked eye*/

		PORTA = (PORTA & 0xC0) | (1 << PA4);
		PORTC = (PORTC & 0xF0) | (seconds5 & 0x0F);
		_delay_ms(1);

		PORTA = (PORTA & 0xC0) | (1 << PA3);
		PORTC = (PORTC & 0xF0) | (minutes4 & 0x0F);
		_delay_ms(1);

		PORTA = (PORTA & 0xC0) | (1 << PA2);
		PORTC = (PORTC & 0xF0) | (minutes3 & 0x0F);
		_delay_ms(1);

		PORTA = (PORTA & 0xC0) | (1 << PA1);
		PORTC = (PORTC & 0xF0) | (hours2 & 0x0F);
		_delay_ms(1);

		PORTA = (PORTA & 0xC0) | (1 << PA0);
		PORTC = (PORTC & 0xF0) | (hours1 & 0x0F);
		_delay_ms(1);
	}
}

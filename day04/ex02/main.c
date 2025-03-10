#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t	sw1 = 0;
volatile uint8_t	sw2 = 0;

void	setup_buttons( void )
{
	DDRD &= ~(1 << DDD2) & (1 << DDD4);
	//	Falling edge triggers interrupt
	EICRA |= (1 << ISC01);
	//	Enable external interrupt INT0
	EIMSK |= (1 << INT0);
	
	//	Enable interrupt for SW2
	PCICR |= (1 << PCIE2);
	
	PCIFR |= (1 << PCIF2);
	//	Masking to focus on PD4
	PCMSK2 |= (1 << PCINT20);
}

void	setup_leds( void )
{
	DDRB |= (1 << DDB0)  | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
	PORTB &= ~(1 << PB0) & ~(1 << PB1) & ~(1 << PB2) & ~(1 << PB4);
}

void	update_leds( uint8_t counter )
{
	PORTB = (counter << PORTB0) | ((counter >> 1) << PORTB1) | ((counter >> 2) << PORTB2) | ((counter >> 3) << PORTB4);
}

ISR(INT0_vect)
{
	sw1 = 1;
}

ISR(PCINT2_vect)
{
	if (!(PIND & (1 << PD4)))
		sw2 = 1;
}

int		main ( void ) 
{
	uint8_t	counter = 0;
	
	setup_buttons();
	setup_leds();
	
	SREG |= (1 << 7);
	
	while (1)
	{
		if (sw1 == 1)
		{
			counter = (counter + 1) % 16;
			update_leds(counter);
			while (!(PIND & (1 << PD2)))
				_delay_ms(20);
			sw1 = 0;
		}
		else if (sw2 == 1)
		{
			if (counter > 0)
				counter--;
			else
				counter = 15;
			update_leds(counter);
			while (!(PIND & (1 << PD4)))
				_delay_ms(20);
			sw2 = 0;
		}
	}
}
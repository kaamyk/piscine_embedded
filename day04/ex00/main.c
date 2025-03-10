#include "include.h"

volatile uint8_t	state = 0;

ISR(INT0_vect)
{
	state = 1;
}

int		main ( void )
{
	//	Setup button
	DDRD &= ~(1 << DDD2);
	//	Setup led
	DDRB |= (1 << DDB0);
	
	//	Falling edge triggers interrupt
	EICRA |= (1 << ISC01);
	//	Enable external interrupt INT0
	EIMSK |= (1 << INT0);
	sei();
	while (1)
	{
		if (state == 1)
		{
			PORTB ^= (1 << PB0);
			_delay_ms(150);
			state = 0;
		}
	}
}
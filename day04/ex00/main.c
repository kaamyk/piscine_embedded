#include "include.h"

ISR(INT0_vect)
{
	EIMSK &= ~(1 << INT0);
	_delay_ms(50);
	
	PORTB ^= (1 << PB0);
	
	EIMSK |= (1 << INT0);
}

int		main ( void )
{
	//	Setup button
	DDRD &= ~(1 << DDD2);
	//	Setup led
	DDRB |= (1 << DDB0);
	
	//	Any logical change trigger interrupt
	EICRA |= (1 << ISC01);
	//	Enable external interrupt INT0
	EIMSK |= (1 << INT0);
	sei();
	while (1);
}
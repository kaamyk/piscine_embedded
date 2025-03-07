#include "include.h"

uint8_t		up = 0;	// Toggle avec un timer 1Hz

void	init_timers( void )
{
	//	Prescaler
		//	64
	CLKPR = (1 << CLKPCE);
	CLKPR = (1 << CLKPS2) | (1 << CLKPS1);
	
	//	Timer0
		//	divider 1024
	TCCR0A |= (1 << COM0A1) | (1 << COM0A0)
		
	//	Timer1
		//	clear bot, set comp | mode 14 (Fast pwm, top ICR1) | divider 8
	TCCR1A |= (1 << COM1A1) | (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);
}

ISR()	// Iterrupt toggle up/down
{
	up ^= 1;
}

int		main ( void )
{
	
	
	while (1);
}
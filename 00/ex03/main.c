#include <avr/io.h>
#include <util/delay.h>

int		main ( void )
{
	// Set PB0 as output
	DDRB |= (1 << DDB0);
	// Set PD2 as input
	DDRD &= ~(1 << DDD2);

	//	Stores PD2 state (high or low)
	unsigned short state = 0;
	while (1)
	{
		// if pin 2 of port D == 0 => change state
		if (~PIND & (1 << 2))
		{
			// if PD2 is high
			if (state)
				PORTB &= ~(1 << PORTB0);
			else
				PORTB |= (1 << PORTB0);
			state = !state;
			// Disable blinking when hold the button
			while (~PIND & (1 << 2));
		}
		// Prevent bounce effect
		_delay_ms(50);
	}
}
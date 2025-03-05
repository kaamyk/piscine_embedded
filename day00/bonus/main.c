#include <avr/io.h>
#include <util/delay.h>

int	main ( void )
{
	//	Set up 4 LEDS || Set up PB0/1/2/4 as output
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
	//	Set 2 buttons || Set up PD2/4 as input
	DDRD &= ~(1 << DDD2) | ~(1 << DDD4);

	// test
	// PORTB |= (1 << PORTB0) | (1 << PORTB1) |(1 << PORTB2);
	unsigned char counter = 0;
	while (1)
	{
		//	If sw1 is pushed
		if (~PIND & (1 << 2))
		{
			if (counter == 15)
				counter ^= counter;
			else
				counter++;
				//	Get each bit from counter and assgin them to PORTB => Set up the LEDS
				PORTB = (counter << PORTB0) | ((counter >> 1) << PORTB1) | ((counter >> 2) << PORTB2) | ((counter >> 3) << PORTB4);
				//	Prevent bounce effect
				while (~PIND & (1 << 2));
			}
			//	if sw2 is pushed
			else if (~PIND & (1 << 4))
			{
				if (counter == 0)
				counter = 15;
				else
				counter--;
				PORTB = (counter << PORTB0) | ((counter >> 1) << PORTB1) | ((counter >> 2) << PORTB2) | ((counter >> 3) << PORTB4);
				//	Prevent bounce effect
				while (~PIND & (1 << 4));
			}
		//	Prevent bounce effect
		_delay_ms(100);
	}
}
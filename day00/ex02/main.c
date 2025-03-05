#include <avr/io.h>
#include <util/delay.h>

int		main ( void )
{
	//	Set PD2 as input
	DDRD &= ~(1 << DDD2);
	//	Set PB0 as output
	DDRB |= (1 << DDB0);
	
	while (1)
	{	
		// if pin 2 of port D == 1 => set output as low
		if (PIND & (1 << 2))
			PORTB &= ~(1 << PORTB0);
		// if pin 2 of port D == 0 => set utput as high
		else
			PORTB |= (1 << PORTB0);
	}
}
#include <avr/io.h>

int		main ( void )
{
	//	Define PB1 as output
	DDRB |= (1 << DDB1);
	uint32_t	count = 0;

	//	Define PB1 as high (parce que pourquoi pas)
	PORTB |= (1 << PORTB1);

	while (1)
	{
		//	Count = 16M / 2 / 5 => cpu_freq / (high then low) / nb_tic_to_loop
		count = 1600000;
		//	Decrement cout until = 0
		while (count--);
		//	PORTB = PORTB xor PORTB1
		//	11001001 ^ 00000100 = 11001101 ||| 11001100 ^ 00000100 = 11001001
		PORTB ^= (1 << PORTB1);
	}

	
}
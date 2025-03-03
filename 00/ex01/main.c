#include <avr/io.h>

int	main( void )
{
	// DDxn = pin direction (in || out)
	//		Line sets the direction to out
	DDRB |= (1 << DDB0);

	// PORTxn = if pin as output sets the analogic output (1 || 0)
	// 		Line sets the pin as high 
	PORTB |= (1 << PORTB0);	

	while (1)
	{
	}
}
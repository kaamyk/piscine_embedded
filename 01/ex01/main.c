#include <avr/io.h>

int		main ( void )
{
	//	Set LED as output
	DDRB |= (1 << DDB1);
	//	Toggle OC1A on compare match 
	TCCR1A |= (1 << COM1A0);
	//	Set up timer1 divider to 1024 and turn on mode 4 (CTC)
	TCCR1B |= (1 << CS10) | (1 << CS12) | (1 << WGM12);
	TCNT1 = 0;
	//	16M / 1024 = 15625
	//	To make a 1Hz frequency, we need to divide by 2
	OCR1A = 7812;
	while(1);
}
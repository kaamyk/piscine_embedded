#include <avr/io.h>

int		main( void )
{
	//	PB1 set as output
	DDRB = (1 << DDB1);
	/*

		Timer1 set on mode 14 for fast PWM using ICR1 as TOP
		Timer1 clear OC1A on match and set it at BOTTOM
		We set the timer's divider to 1024
			The counter goes up to 15625
	*/
	TCCR1A |= (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1<< CS12);
	//	ICR1 is set to 16M / 1024
	ICR1 = 15625;
	//	OCR1A is set to ICR1 / 10
	OCR1A = 1562;
	while (1);
}
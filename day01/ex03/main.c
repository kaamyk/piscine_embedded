#include <avr/io.h>
#include <util/delay.h>

int		main( void )
{
	//	PB1 set as output
	DDRB |= (1 << DDB1);
	//	PD2 and PD4 set as input
	DDRD &= ~(1 << DDB2) | ~(1 << DDB4);
	
	/*
		Timer1 setup with divider of 1024
		Timer1 set to mode 14 for fast PWM and ICR1 as TOP
	*/
	TCCR1A |= (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS12);
	//	Set ICR1 to 16M / 1024
	ICR1 = 15625;
	//	Initialize OCR1 to ICR1 / 10
	OCR1A = 1562;
	
	volatile float counter = 0.100f;
	uint8_t state[2] = {0, 0};

	while (1)
	{
		// --- handle button hold ---
		if (PIND & (1 << PD2))
			state[0] = 0;
		if (PIND & (1 << PD4))
			state[1] = 0;


		// --- SW1 or SW2 is pushed ---
			//	if SW1 is pushed
		if (~PIND & (1 << PD2) && counter < 1.0f && state[0] == 0)
		{
			state[0] = 1;
			counter += 0.100f;
		}
			//	else if SW2 is pushed
		else if (~PIND & (1 << PD4) && counter > 0.115f && state[1] == 0)
		{
			state[1] = 1;
			counter -= 0.100f;
		}

		// --- Write the match value in OCR1A ---
		OCR1A = ICR1 * counter;
	}
}
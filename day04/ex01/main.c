#include "include.h"

uint8_t		up = 0;	// Toggle avec un timer 1Hz
uint8_t		t0_ovf_counter = 0;

void	init_timers( void )
{
	//	Timer0
		//	PWM, phase correct, TOP=255
	TCCR0A |= (1 << WGM00);
		
	//	Timer1
		//	set bot, clear comp | mode 14 (Fast pwm, top ICR1) 
	TCCR1A |= (1 << COM1A1) | (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS12);
	ICR1 = 255;
}

ISR(TIMER0_OVF_vect)
{
	t0_ovf_counter++;
	PORTB |= (1 << PB0);
	if (t0_ovf_counter >= 122)
	{
		up ^= 1;
		t0_ovf_counter ^= t0_ovf_counter;
		up & 1 ? OCR1A++ : OCR1A--;
	}
}

int		main ( void )
{
	DDRB |= (1 << DDB0);
	init_timers();
	sei();
	while (1);
}
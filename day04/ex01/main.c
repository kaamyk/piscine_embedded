#include "include.h"

volatile uint8_t	ovf = 0;
volatile uint8_t	div_2 = 0;

#define COMP_VALUE	((F_CPU / 64) / 1000) - 1

void	init_timers( void )
{
	//	Timer0
		//	set BOT / clear COMP | Fast PWM, TOP = OCRA
	TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
		//	divider 64 -> f_timer = F_CPU / divider -> OCRXA = (f_timer/f_desired) - 1
	TCCR0B |= (1 << WGM02) | (1 << CS00) | (1 << CS01);
	OCR0A = COMP_VALUE; 
	
	TIMSK0 |= (1 << OCIE0A);
		
	//	Timer1
		//	set bot, clear comp | mode 14 (Fast pwm, top ICR1) | divider 8
	TCCR1A |= (1 << COM1A1) | (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);
	ICR1 = 255;
	OCR1A = 0;
}

ISR(TIMER0_COMPA_vect)
{
	div_2 = (div_2 + 1) % 2;
	if (div_2 == 1)
		ovf = 1;
}

void	setup_led( void )
{
	DDRB |= (1 << DDB1);
}

int		main ( void )
{
	uint8_t	up = 1;
	uint8_t	counter = 0;
	
	setup_led();
	init_timers();
	
	SREG |= (1 << 7);
	
	while (1)
	{
		if (ovf == 1 && div_2 == 1)
		{
			up == 1 ? counter ++ : counter--;
			OCR1A = counter;
			if ((counter >= 255 && up == 1) || (counter == 0 && up == 0))
				up ^= 1;
			ovf = 0;
		}
	}
}
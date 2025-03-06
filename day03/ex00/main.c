#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t		t_count[3] = [0, 1, 2];

void	setup_led( void )
{
	DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);
}

void	define_timer_1Hz( void )
{
	// Timer1 => mode 4 (CTC), / 256, compare match 
	TCCR1A |= (1 << COM1A0) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
	TCNT1 = 0;
	OCR1A = 33249;
	TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
	for (uint8_t i = 0; i < 3; i++)
	{
		t_count[i]++;
		if (t_count[i] == 3)
		{
			t_count[i] = 0;
			PORTD = (1 <<);
		}
	}
}


int		main( void )
{
	setup_led();
	setup_timer();	
	PORTD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
}
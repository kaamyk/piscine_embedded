#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

uint8_t	g_seq_counter = 0;

void	setup_led( void )
{
	DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);
}

void	define_timer_1Hz( void )
{
	// Timer1 => mode 4 (CTC), / 256, compare match 
	TCCR1A |= (1 << COM1A0) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	TCNT1 = 0;
	OCR1A = 62500;
	TIMSK1 |= (1 << OCIE1A);
}

void	print_sequence( void )
{
	const uint8_t		pin_count[3] = {PD3, PD6, PD5};
	const uint8_t		print_seq[7] = {4, 2, 1, 6, 3, 5, 7};
	
	for (uint8_t j = 0; j < 3; j++)
	{
		if (print_seq[g_seq_counter] & (1 << j))
			PORTD |= (1 << pin_count[j]);
	}
}

ISR(TIMER1_COMPA_vect)
{
	PORTD &= ~(1 << PD3) & ~(1 << PD5) & ~(1 << PD6);
	print_sequence();
	++g_seq_counter;
	if (g_seq_counter >= 7)
		g_seq_counter ^= g_seq_counter;
}

int		main( void )
{
	setup_led();
	define_timer_1Hz();	
	sei();
	g_seq_counter = 0;
	while(1);
}
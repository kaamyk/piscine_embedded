#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

uint8_t		pin_count[3] = {PD3, PD5, PD6};
uint8_t		g_count = 0;

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

ISR(TIMER1_COMPA_vect)
{
	g_count++;
	if (g_count >= 3)
		g_count = 0;
	PORTD &= ~(1 << PD3) & ~(1 << PD5) & ~(1 << PD6);
	PORTD |= (1 << pin_count[g_count]);
}

int		main( void )
{
	setup_led();
	define_timer_1Hz();	
	sei();
	while(1);
}
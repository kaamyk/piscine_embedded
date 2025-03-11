#include <avr/io.h>

void	setup_pv1( void )
{
	//	Select the ADC0
	// ADMUX &= 0b0000;
	//	Select the voltage | Left adjust result 
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	
	//	Enable ADC | divider = 128
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void	init_rgb( void )
{
	//	Blue -> OC2B
	//	Fast PWM TOP=0xFF | OC2B Set BOT, clear comp
	TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << COM2B1);
	TCCR2B |= (1 << CS20);
	OCR2B = 0;

	//	Fast PWM, TOP 0xFF | clear BOT, set COMP
	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
	//	Red -> OC0B
	//	Green -> OC0A
	TCCR0B |= (1 << CS00);
}

void	set_led_output( void )
{
	//	RGB
	DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);
	//	Gauge
	DDRB  |= (1 << DDB4) | (1 << DDB2) | (1 << DDB1) | (1 << DDB0);
}

void	set_rgb( uint16_t r, uint16_t g, uint16_t b)
{
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void	wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85)
	{
		set_rgb(255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170)
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}
	else
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

void	write_gauge( uint8_t val )
{
	//	clear leds
	PORTB &= ~(1 << DDB4) & ~(1 << DDB2) & ~(1 << DDB1) & ~(1 << DDB0);
	//	set leds
	if (val >= 63)
		PORTB |= (1 << DDB0);
	if (val >= 127)
		PORTB |= (1 << DDB1);
	if (val >= 191)
		PORTB |= (1 << DDB2);
	if (val == 255)
		PORTB |= (1 << DDB4);
}

int		main ( void )
{
	uint8_t	value = 0;
	// PV1 ==> ADC_POT / ADC0
	set_led_output();
	init_rgb();
	setup_pv1();
	while (1)
	{
		//	After each conversion the flag is set to 0
		//	Set the ADSC bit in ADCSRA
		ADCSRA |= (1 << ADSC);
		//	Wait for the conversion to be done
		while (ADCSRA & (1 << ADSC));
		value = ADCH;
		wheel(value);
		write_gauge(value);
	}
}
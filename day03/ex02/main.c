#include <avr/io.h>
#include <util/delay.h>


uint8_t		pos = 0;
uint8_t		up = 1;

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

void	init_rgb( void )
{
	//	Blue -> OC2B
	//	Fast PWM TOP=0xFF | OC2B Set BOT, clear comp
	TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << COM2B1);
	TCCR2B |= (1 << CS20);
	OCR2B = 0;

	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
	//	Red -> OC0B
	//	Green -> OC0A
	TCCR0B |= (1 << CS00);
}

void	set_led_output( void )
{
	DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);
}

int		main ( void )
{
	init_timer();
	set_led_output();
	while (1)
	{
		_delay_ms(20);
		wheel(pos);
		up == 1 ? pos++ : pos--;
		if (pos >= 250)
			up = 0;
		if (pos <= 10)
			up = 1;
	}
}

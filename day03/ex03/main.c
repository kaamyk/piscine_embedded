#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#define	BASE "0123456789ABCDEF"

uint8_t pos = 0;
uint8_t up = 1;
char input[7] = {0};
uint8_t input_counter = 0;
uint8_t	input_done = 0;

void set_rgb(uint16_t r, uint16_t g, uint16_t b)
{
	PORTB |= (1 << PB2);
	_delay_ms(100);
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
	PORTB &= ~(1 << PB2);
}

void init_rgb(void)
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

void	init_uart( void )
{
	/*
		Enable transmitter and recepter
		Character size to 8 bit
		Enable interuption
	*/
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (3 << UCSZ00) | (1 << RXCIE0);
	/*
		Disable parity
		Sets stop bit to 1
	*/
	UCSR0C &= ~(3 << UPM00) & ~(1 << USBS0);
	const uint16_t	ubrr = F_CPU / (16 * UART_BAUDRATE);
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
}

void	uart_tx( char c )
{
	//	while data register si not empty
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void	uart_printstr( const char * str )
{
	while (*str)
	{
		uart_tx(*str);
		str++;
	}
}

void set_led_output(void)
{
	DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);
}

uint8_t		parse_input(void)
{
	if (input[0] != '#')
		return (1);
	for (uint8_t i = 1; i < 7; i++) 
	{
		if ((input[i] < '0' || input[i] > '9') && (input[i] < 'A' || input[i] > 'F'))
			return (1);
	}
	return (0);
}

uint8_t		atoi_base( const char *s )
{
	uint8_t		first_digit = strchr(BASE, s[0]) - BASE;
	uint8_t		second_digit = strchr(BASE, s[1]) - BASE;
	return (first_digit * second_digit);
}

void	handle_input( void )
{
	while(input_done == 0);
	input_done = 0;
	if (parse_input() == 0)
	{
		set_rgb(atoi_base(input + 1), atoi_base(input + 3), atoi_base(input + 5));
	}
}

ISR(USART_RX_vect)
{
	char c = UDR0;
	if (c == '\n' || c == '\r')
	{
		input_done = 1;
		uart_tx('\r');
		uart_tx('\n');
	}
	else if (c == '\b' || c == 127)
	{
		if (input_counter > 0)
		{
			uart_printstr("\b \b");
			input_counter--;
		}
	}
	else if (c >= ' ' && c < 127)
	{
		if (input_counter < 7)
		{
			uart_tx(c);
			input[input_counter++] = c;
		}
	}
}

int main(void)
{
	init_rgb();
	init_uart();
	set_led_output();
	//debug
	DDRB |= (1 << DDB2);
	sei();
	while (1)
	{
		handle_input();
	}
}
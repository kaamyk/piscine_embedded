#include <avr/io.h>
#include <avr/interrupt.h>

void	uart_init( void )
{
	// Check there is on other communication going on
	//	Turn on the Transmitter and Receiver bits	
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
	//	Set the character size to 8 bits
	UCSR0C |= (3 << UCSZ00);
	//	(not mandatory) Set a UNIQUE stop bit
	UCSR0C &= ~(1 << USBS0);
	//	(not mandatory) Disable the parity
	UCSR0B &= ~(3 << UPM00);

	//	Calculate the Brr
	const uint16_t ubrr = (F_CPU / (16 * UART_BAUDRATE));
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
}

void	uart_tx( char c )
{
	//	while data register si not empty
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void	define_timer_halfHz( void )
{
	// Timer1 => mode 4 (CTC), / 256, compare match 
	TCCR1A |= (1 << COM1A0) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
	TCNT1 = 0;
	OCR1A = 33249;
	TIMSK1 |= (1 << OCIE1A);
}

void	uart_printstr( const char * str )
{
	while (*str)
	{
		uart_tx(*str);
		str++;
	}
}

ISR(TIMER1_COMPA_vect)
{
	const char	*str = "Hello World!";
	uart_printstr(str);
	uart_tx('\n');
	uart_tx('\r');
}

int		main ( void )
{
	define_timer_1Hz();
	//	Enable interuptions
	sei();
	uart_init();
	
	while(1);
}
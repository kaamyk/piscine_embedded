#include <avr/io.h>
#include <avr/interrupt.h>

void	init_uart( void )
{
	/*
		Enable transmitter and recepter
		Character size to 8 bit
		Enable interruption
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
	//	while Data Register si not empty
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

ISR(USART_RX_vect) // p50
{
	char	c = UDR0;
	// uart_tx(c);
	if (c >= 'a' && c <= 'z')
		c -= 'a' - 'A';
	else if (c >= 'A' && c <= 'Z')
		c += 'a' - 'A';
	uart_tx(c);
}

int		main ( void )
{
	init_uart();
	sei();
	while (1);
}
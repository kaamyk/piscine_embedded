#include <avr/io.h>
#include <avr/interrupt.h>

void	init_uart( void )
{
	/*
		Enable transmitter and recepter
		Character size to 8 bit
	*/
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (3 << UCSZ00);
	/*
		Disable parity
		Sets stop bit to 1
	*/
	UCSR0C &= ~(3 << UPM00) & ~(1 << USBS0);
	const uint16_t	ubrr = F_CPU / (16 * UART_BAUDRATE);
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
}

char uart_rx( void )
{
	//	while Data Register is empty
	while (!(UCSR0A & (1 << RXC0)));
	return (UDR0);
}

void	uart_tx( char c )
{
	//	while Data Register si not empty
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

int		main ( void )
{
	init_uart();
	while (1)
	{
		uart_tx(uart_rx());
	}
}

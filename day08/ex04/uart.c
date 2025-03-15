#include "uart.h"

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

void	init_uart_int( void )
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

void	uart_tx( const uint8_t c )
{
	//	while data register si not empty
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

char	uart_rx( void )
{
	//	while Data Register is empty
	while (!(UCSR0A & (1 << RXC0)));
	return (UDR0);
}

void	uart_printstr( const char * str )
{
	while (*str)
	{
		uart_tx(*str);
		str++;
	}
}

void	print_hex_value_8( const uint8_t c )
{
	uart_tx(BASE[(c >> 4) & 0x0F]);
	uart_tx(BASE[c & 0x0F]);
}

void	print_hex_value_16( const uint16_t c )
{
	uart_tx(BASE[(c >> 12) & 0x0F]);
	uart_tx(BASE[(c >> 8) & 0x0F]);
	uart_tx(BASE[(c >> 4) & 0x0F]);
	uart_tx(BASE[c & 0x0F]);
}

void	uart_nl( void )
{
	uart_tx('\n');
	uart_tx('\r');
}
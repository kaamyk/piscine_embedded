#include <avr/io.h>
#include <util/delay.h>

#define	BASE "0123456789abcdef"

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
	//	while Data Register si not empty
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

void	setup_pv1( void )
{
	//	Select the ADC0
	// ADMUX &= 0b0000;
	//	Select the voltage | Left adjust result
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	
	//	Enable ADC | divider = 128
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void	select_channel( uint8_t channel )
{
	ADMUX &= ~(3 << MUX0);
	ADMUX |= (channel << MUX0);
}

int		main ( void )
{
	uint8_t	value = 0;
	uint8_t	channel = 0;
	// PV1 ==> ADC_POT / ADC0
	init_uart();
	setup_pv1();
	while (1)
	{
		select_channel(channel);
		//	After each conversion the flag is set to 0
		//	Set the ADSC bit in ADCSRA
		ADCSRA |= (1 << ADSC);
		//	Wait for the conversion to be done
		while (ADCSRA & (1 << ADSC));
		value = ADCH;
		uart_tx(BASE[(value >> 4) % 16]);
		uart_tx(BASE[value % 16]);
		channel == 2 ? uart_printstr("\r\n") : uart_printstr(", ");
		if (channel == 2)
			_delay_ms(20);
		channel = (channel + 1) % 3;
	}
}
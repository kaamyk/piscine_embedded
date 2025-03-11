#include <avr/io.h>
#include <util/delay.h>

#define	BASE "0123456789"

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
	//	Select the voltage 
	ADMUX |= (1 << REFS0) | (1 << REFS1);
	
	//	Enable ADC | divider = 128
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void	select_channel( void )
{
	ADMUX &= ~(3 << MUX0);
	ADMUX |= (1 << MUX3);
}
void	ft_itoa( uint16_t val )
{
	if (val >= 10)
		ft_itoa(val / 10);
	uart_tx((val % 10) + '0');
}

uint16_t	convert( uint16_t value )
{
	// Use of linear function
	// 0x160 = 352 | 0x010d = 269
	//	slope = 0.783
	float	slope = (25 - (-40)) / (0x0160 - 0x010d);
	//	intercept = -250.616
	float	intercept = 25 - (slope * 0x0160);
	return ((slope * value) + intercept);
}

int		main ( void )
{
	uint16_t	value = 0;
	// PV1 ==> ADC_POT / ADC0
	init_uart();
	setup_pv1();
	select_channel();
	while (1)
	{
		//	After each conversion the flag is set to 0
		//	Set the ADSC bit in ADCSRA
		ADCSRA |= (1 << ADSC);
		//	Wait for the conversion to be done
		while (ADCSRA & (1 << ADSC));
		value = ADC;
		//
		ft_itoa(convert(value));
		_delay_ms(20);
		uart_printstr("\r\n");
	}
}
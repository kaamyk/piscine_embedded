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
	ADMUX |= (1 << REFS0);
	
	//	Enable ADC | divider = 128
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void	select_channel( uint8_t channel )
{
	ADMUX &= ~(3 << MUX0);
	//	channel == 0b000 || 0b001 || 0b010
	ADMUX |= (channel << MUX0);
}

void	ft_itoa( uint16_t val )
{
	if (val >= 10)
		ft_itoa(val / 10);
	uart_tx((val % 10) + '0');
}

int		main ( void )
{
	uint16_t	value = 0;
	uint8_t		channel = 0;
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
		//	uin16_t can contain 10bits
		value = ADC;
		ft_itoa(value);
		channel == 2 ? uart_printstr("\r\n") : uart_printstr(", ");
		if (channel == 2)
			_delay_ms(20);
		channel = (channel + 1) % 3;
	}
}

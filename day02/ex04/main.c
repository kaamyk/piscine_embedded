#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

char		g_user_input[2][256] = {{0}, {0}};
uint8_t	g_buffer_position = 0;
uint8_t	g_buf_selector = 0;
uint8_t	g_counter = 0;
uint8_t	g_input_done = 0;

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

uint8_t		check_inputs( void )
{
	// uint8_t i = 0;
	const char	*username = "anvincen";
	const char	*password = "12345";

	uart_printstr(g_user_input[0]);
	uart_tx('\r');
	uart_tx('\n');
	uart_printstr(g_user_input[1]);
	uart_tx('\r');
	uart_tx('\n');
	if (strcmp(g_user_input[0], username) != 0
	|| strcmp(g_user_input[1], password) != 0)
		return (1);
	// while (i < 255 && g_user_input[0][i] && username[i])
	// {
	// 	uart_tx(g_user_input[0][i]);
	// 	uart_tx(username[i]);
	// 	uart_tx('\r');
	// 	uart_tx('\n');
	// 	if (g_user_input[0][i] != username[i]);
	// 		return (1);
	// 	i++;
	// }
	// i = 0;
	// while (i < 255 && g_user_input[1][i] && password[i])
	// {
	// 	uart_tx(i + '0');
	// 	if (g_user_input[1][i] != password[i]);
	// 		return (1);
	// 	i++;
	// }
	return (0);
}

void	print_prompt_user( void )
{
	const char	*username = "Type your username: ";
	uart_printstr(username);
}

void	print_prompt_pass( void )
{
	const char	*password = "Type your password: ";
	uart_printstr(password);
}

void	led_routine(void)
{
	PORTB &= ~(1 << PB0) & (1 << PB1) & (1 << PB2) & ~(1 << PB4);
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
	_delay_ms(500);
	PORTB |= (1 << PB0);		
	_delay_ms(1000);
	PORTB |= (1 << PB1);		
	_delay_ms(1000);
	PORTB |= (1 << PB2);		
	_delay_ms(1000);
	PORTB |= (1 << PB4);		
	_delay_ms(1000);
	if (check_inputs() == 0)
	{
		while (1)
		{
			PORTB &= ~(1 << PB0) | ~(1 << PB1) | ~(1 << PB2) | ~(1 << PB4); 
			_delay_ms(200);
			PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
			_delay_ms(200);
		}
	}
	else
	{
		PORTB &= ~(1 << PB1) & ~(1 << PB4);
	}
}

ISR(USART_RX_vect) // p50
{
	char	c = UDR0;
	if (c == '\n' || c == '\r')
	{
		g_user_input[g_buf_selector][g_counter] = '\0';	
		g_buf_selector = 1;
		g_input_done = 1;
		uart_tx('\n');
		uart_tx('\r');
	}
	else if ((c >= ' ' && c <= 127) || c == '\b')
	{
		if ((c == '\b' || c == 127))
		{
			if (g_counter > 0)
			{
				uart_printstr("\b \b");
				g_counter--;
			}
		}
		else 
		{
			if (g_buf_selector == 0)
			{
				uart_tx(c);
			}
			else if (g_buf_selector == 1)
			uart_tx('*');
			g_user_input[g_buf_selector][g_counter++] = c;
		}
	}
}

void	password( void )
{
	g_buf_selector = 0;
	g_counter = 0;
	g_input_done = 0;
	print_prompt_user();
	uart_tx('O');
	while (g_input_done == 0);

	g_counter = 0;
	g_input_done = 0;
	print_prompt_pass();
	while (g_input_done == 0);

	// uart_printstr(g_user_input[0]);
	// uart_tx('\n');
	// uart_tx('\r');
	// uart_printstr(g_user_input[1]);
	// uart_tx('\n');
	// uart_tx('\r');

	led_routine();
}

void	clear_buf( void )
{
	uint8_t	i = 0;

	while (i < 255)
	{
		g_user_input[0][i] = '\0';
		g_user_input[1][i] = '\0';
		i++;
	}
}

int		main ( void )
{
	init_uart();
	sei();

	while (1)
	{
		password();
		clear_buf();
	}
}
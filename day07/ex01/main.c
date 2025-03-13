#include "main.h"

char	input[256] = {0};
uint8_t	input_counter = 0;
uint8_t	input_done = 0;

uint8_t	eeprom_read( const uint16_t addr )
{
	while (EECR & (1 << EEPE));
	EEAR = addr & 0x1ff;
	EECR |= (1 << EERE);
	return (EEDR);
}

void	eeprom_write( const uint16_t addr, const uint8_t byte )
{
	while (EECR & (1 << EEPE));
	while (SPMCSR & (1 << SELFPRGEN));
	EEAR = addr & 0x1ff;
	EEDR = byte;
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);
}

void	print_addr( const uint16_t addr )
{
	uart_printstr("0000");
	print_hex_value_16(addr);
	uart_tx(' ');
}

void	eeprom_read_range( const uint16_t start, const uint16_t range )
{
	uint8_t	data = 0;
	
	for (uint16_t i = start; i < 0x03ff && i < range; i++)
	{
		if ((start + i) % 16 == 0)
		{
			uart_nl();
			print_addr(start + i);
		}
		else if (((start + i) % 2) == 0) 
			uart_tx(' ');
		data = eeprom_read(start + i);
		print_hex_value_8(data);
	}
	uart_nl();
}

uint16_t		atos_base( const char *s )
{
	uint16_t		first_digit = strchr(BASE, s[0]) - BASE;
	uint16_t		second_digit = strchr(BASE, s[1]) - BASE;
	uint16_t		third_digit = strchr(BASE, s[2]) - BASE;
	uint16_t		quad_digit = strchr(BASE, s[4]) - BASE;
	
	return (((first_digit & 0x0F) << 12) | ((second_digit & 0x0F) << 8) | \
			((third_digit & 0x0F) << 8) | (quad_digit & 0x0F));
}

uint8_t		handle_input( uint16_t *w_addr, uint8_t *w_byte )
{
	uint8_t i = 0;
	char	s_tmp[4] = {0};
	for (; i < 4 && input[i] != 0 && input[i] != ' '; i++)
	{
		if ((input[i] < '0' || input[i] > '9') && (input [i] < 'a' || input[i] > 'f'))
		{
			uart_printstr("Address: wrong digit\n\r");
			return (1);
		}
		s_tmp[i] = input[i];
	}
	if (input[i] != ' ')
	{
		uart_printstr("Address: too big\r\n");
		return (1);
	}
	*w_addr = atos_base(s_tmp);
	i++;
	if (input[i] == 0 || input[i + 1] == 0)
	{
		uart_printstr("Byte: wrong format or empty\r\n");
		return (1);
	}
	*w_byte = atos_base(input + i) & 0x00ff;
	return (0);
}

void get_input( void )
{
	char c = 0;
	while (input_done == 0)
	{
		while (c == 0)
			c = uart_rx();
		if (c == '\n' || c == '\r')
		{
			uart_tx('\r');
			uart_tx('\n');
			input_done = 1;
		}
		else if (c == '\b' || c == 127)
		{
			if (input_counter > 0)
			{
				uart_printstr("\b \b");
				input_counter--;
			}
		}
		else if (c == ' ' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))
		{
			if (input_counter <= 7)
			{
				uart_tx(c);
				input[input_counter++] = c;
			}
		}
		c = 0;
	}
}

int		main( void )
{
	//debug
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
	
	uint16_t	w_addr = 0;
	uint8_t		w_byte = 0;
	
	init_uart();
	while (1)
	{
		get_input();
		if (input_done == 1)
		{
			handle_input(&w_addr, &w_byte);
			print_hex_value_16(w_addr);
			print_hex_value_8(w_byte);
			eeprom_read_range(0x00, 0x03ff);
			input_counter = 0;
			input_done = 0;
		}
	}
}

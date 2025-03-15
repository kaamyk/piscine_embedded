#include "main.h"

#define RED "\e[31m"
#define RESET "\e[0m"

uint8_t	 input_done = 0;
uint16_t w_addr		= 0;

uint8_t eeprom_read(const uint16_t addr)
{
	while (EECR & (1 << EEPE));
	EEAR = addr & 0x3ff;
	EECR |= (1 << EERE);
	return (EEDR);
}

void eeprom_write(const uint16_t addr, const uint8_t byte)
{
	while (EECR & (1 << EEPE));
	while (SPMCSR & (1 << SELFPRGEN));
	EEAR = addr & 0x3ff;
	EEDR = byte;
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);
}

void print_addr(const uint16_t addr)
{
	uart_printstr("0000");
	print_hex_value_16(addr);
	uart_tx(' ');
}


void	eeprom_read_range( const uint16_t start, const uint16_t range )
{
	uint8_t	data = 0;
	
	for (uint16_t i = start; i < 0x03f0 && i < range; i++)
	{
		if (i % 16 == 0)
		{
			uart_nl();
			print_addr(start + i);
		}
		else if (i % 2 == 0) 
			uart_tx(' ');
		data = eeprom_read(i);
		print_hex_value_8(data);
	}
	uart_nl();
}


uint8_t is_hexa(const char c)
{
	return ((c >= 'a' && c <= 'f') || (c >= '0' && c <= '9'));
}

uint16_t atos_base(const uint8_t *s)
{
	uint16_t digits[4] = {0};
	uint8_t	 i		   = 0;

	while (i < 4)
	{
		digits[i] = (strchr(BASE, *s) - BASE);
		s++;
		i++;
	}

	return (((digits[0] & 0x0F) << 12) | ((digits[1] & 0x0F) << 8) |
			((digits[2] & 0x0F) << 4) | (digits[3] & 0x0F));
}

uint8_t atob_base(const char *s)
{
	uint16_t first_digit  = strchr(BASE, s[0]) - BASE;
	uint16_t second_digit = strchr(BASE, s[1]) - BASE;

	return (((first_digit & 0x0F) << 4) | (second_digit & 0x0F));
}

uint16_t get_addr(const char input[256])
{
	uint8_t s_tmp[4] = {0};
	uint8_t i		 = 0;

	while (i < 4 && input[i] != '\0')
	{
		s_tmp[i] = input[i];
		i++;
	}
	if (input[i] != ' ')
	{
		uart_printstr("Address: Wrong format\r\n");
		return (0xffff);
	}

	return (atos_base(s_tmp));
}

uint8_t handle_input(const char input[256], uint16_t *w_addr, uint8_t *w_byte)
{
	uint8_t i = 0;

	*w_addr = get_addr(input);
	uart_nl();
	if (*w_addr == 0xffff)
		return (1);
	if (*w_addr > 0x03ff)
	{
		uart_printstr("Address: too high\r\n");
		return (1);
	}
	while (input[i++] != ' ');
	if (input[i] == 0 || is_hexa(input[i]) == 0 || input[i + 1] == 0 ||
		is_hexa(input[i + 1]) == 0)
	{
		uart_printstr("Byte: wrong format or empty\r\n");
		return (1);
	}
	*w_byte = atob_base(input + i);
	return (0);
}

void get_input(char input[256])
{
	uint8_t input_counter = 0;
	char	c			  = 0;

	while (input_done == 0)
	{
		while (c == 0) c = uart_rx();
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
		else if (c == ' ' || is_hexa(c) == 1)
		{
			if (input_counter < 7)
			{
				uart_tx(c);
				input[input_counter++] = c;
			}
		}
		c = 0;
	}
}

uint8_t check_key_exists(const char w_addr, const uint8_t w_byte)
{
	const uint8_t c = eeprom_read(w_addr);
	return (c == w_byte);
}

int main(void)
{
	char	input[256] = {0};
	// uint8_t key_exists = 0;
	uint8_t w_byte	   = 0;

	init_uart();
	while (1)
	{
		get_input(input);
		if (input_done == 1)
		{
			input_done = 0;
			w_addr	   = 0;
			w_byte	   = 0;
			if (handle_input(input, &w_addr, &w_byte) == 0)
			{
				if (check_key_exists(w_addr, w_byte) == 0)
				{
					eeprom_write(w_addr, w_byte);
					eeprom_read_range(0x00, 0x03ff);
				}
				else
				{
					uart_printstr("Value already exists\r\n");
				}
			}
		}
	}
}

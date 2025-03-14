#include "main.h"

#define START_BYTE 0x80
#define MID_BYTE 0x81
#define STOP_BYTE 0x82

uint8_t input_done = 0;

uint8_t define_cmd(const uint8_t cmd[7])
{
	if (ft_strcmp(cmd, (uint8_t *)"READ") == 0)
		return (READ);
	else if (ft_strcmp(cmd, (uint8_t *)"WRITE") == 0)
		return (WRITE);
	else if (ft_strcmp(cmd, (uint8_t *)"FORGET") == 0)
		return (FORGET);
	else if (ft_strcmp(cmd, (uint8_t *)"PRINT") == 0)
		return (PRINT);
	else
		return (FALSE);
}

void print_value(uint16_t key_addr)
{
	uint8_t val_str[17] = {0};
	uint8_t c			= 0;

	while (c != MID_BYTE)
	{
		c = eeprom_read(key_addr++);
	}
	for (uint8_t i = 0; i < 17 && val_str[i] != STOP_BYTE; i++)
	{
		val_str[i] = eeprom_read(key_addr++);
		if (val_str[i] == STOP_BYTE)
			val_str[i] = 0;
	}
	
}

void read_value(const uint8_t *key)
{
	uint16_t r_addr		  = 0;
	uint16_t addr_counter = 0;
	uint8_t	 c			  = 0;
	uint8_t	 i			  = 0;

	while (addr_counter <= 0x03ff)
	{
		if (eeprom_read(addr_counter++) == START_BYTE)
		{
			r_addr = addr_counter++;
			c	   = 0;
			while (addr_counter <= 0x03ff && c != STOP_BYTE)
			{
				c = eeprom_read(addr_counter++);
				if (c != key[i++])
					break;
			}
			if (c == MID_BYTE)
		}
	}
}

uint8_t find_free_space(const uint8_t key_l, const uint8_t val_l)
{
	uint8_t	 free_l		  = 0;
	uint16_t total_l	  = key_l + val_l + 2;
	uint16_t addr_counter = 0;
	uint16_t w_addr		  = 0;

	while (addr_counter <= 0x03ff)
	{
		if (free_l == total_l)
		{
			break;
		}
		else if (eeprom_read(addr_counter) == START_BYTE)
		{
			free_l = 0;
			while (addr_counter < 0x03ff &&
				   eeprom_read(addr_counter++) != STOP_BYTE);
			w_addr = addr_counter;
		}
		else
		{
			++addr_counter;
			++free_l;
		}
	}
	return (free_l >= total_l ? w_addr : 0);
}

int8_t check_value_exists(const uint8_t key[17])
{
	uint8_t	 c			  = 0;
	uint8_t	 tmp_str[35]  = {0};
	uint16_t addr_counter = 0;

	while (addr_counter < 0x03ff)
	{
		c = eeprom_read(addr_counter++);
		if (c == 0x80)
		{
			for (uint8_t i = 0; i < 34 && tmp_str[i] != '\0'; i++)
			{
				tmp_str[i] = eeprom_read(addr_counter++);
				if (tmp_str[i] == MID_BYTE)
				{
					tmp_str[i] = '\0';
					break;
				}
			}
			if (ft_strcmp(tmp_str, key) == 0)
			{
				uart_printstr("Key: already exists\n\r");
				return (1);
			}
		}
	}
	return (0);
}

void write_key_value(const uint8_t key[17], const uint8_t val[17])
{
	uart_printstr("write << \r\n");
	uint8_t	 key_l	= ft_strlen(key);
	uint8_t	 val_l	= ft_strlen(val);
	uint16_t w_addr = find_free_space(key_l, val_l);

	if (w_addr == 0)
	{
		uart_printstr("No space found ...\r\n");
		return;
	}
	print_hex_value_16(w_addr);
	uart_nl();
	eeprom_write(w_addr++, START_BYTE);
	for (uint8_t i = 0; i < 17 && key[i] != '\0'; i++)
	{
		eeprom_write(w_addr++, key[i]);
	}
	eeprom_write(w_addr++, MID_BYTE);
	for (uint8_t i = 0; i < 17 && val[i] != '\0'; i++)
	{
		eeprom_write(w_addr++, val[i]);
	}
	eeprom_write(w_addr, STOP_BYTE);
}

void forget_key_value(const char *input) {}

uint8_t get_cmd(const char **input, uint8_t cmd[7])
{
	uint8_t i = 0;
	for (; i < 6 && **input != ' '; i++)
	{
		cmd[i] = **input;
		(*input)++;
	}
	return (**input != ' ');
}

uint8_t get_key(const char **input, uint8_t key[17])
{
	uint8_t i = 0;

	(*input)++;
	for (; i < 16 && **input != ' '; i++)
	{
		key[i] = **input;
		(*input)++;
	}
	uart_printstr((char *)key);
	uart_nl();
	return (**input != ' ');
}

uint8_t get_val(const char **input, uint8_t val[17])
{
	uint8_t i = 0;

	(*input)++;
	for (; i < 16 && **input != ' '; i++)
	{
		val[i] = **input;
		(*input)++;
	}
	uart_printstr((char *)val);
	uart_nl();
	return (**input != '\0');
}

uint8_t split_input(const char input[256], uint8_t cmd[7], uint8_t key[17],
					uint8_t val[17])
{
	(void)get_cmd(&input, cmd);
	(void)get_key(&input, key) == 1;
	(void)get_val(&input, val) == 1;
	return (0);
}

uint8_t handle_input(const char input[256], uint16_t *w_addr, uint8_t *w_byte)
{
	uint8_t cmd[7]	= {0};
	uint8_t key[17] = {0};
	uint8_t val[17] = {0};

	//	get cmd
	if (split_input(input, cmd, key, val) == 1)
		return (1);
	switch (define_cmd(cmd))
	{
	case READ:
		if (key[0] == '\0')
		{
			uart_printstr("Key: Wrong format\r\n");
			return (1);
		}
		read_value(key);
		break;
	case WRITE:
		if (key[0] == '\0')
		{
			uart_printstr("Key: Wrong format\r\n");
			return (1);
		}
		if (val[0] == '\0')
		{
			uart_printstr("Value: Wrong format\r\n");
			return (1);
		}

		if (check_value_exists(key) == 0)
			write_key_value(key, val);
		break;
	case FORGET:
		forget_key_value(input);
		break;
	case PRINT:
		print_value(key);
		break;
	default:
		uart_printstr("Command: invalid command\n\r");
		return (1);
	}
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
			uart_nl();
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
		else if (c == ' ' || is_printable(c) == 1)
		{
			if (input_counter < 255)
			{
				uart_tx(c);
				input[input_counter++] = c;
			}
		}
		c = 0;
	}
}

int main(void)
{
	char	 input[256] = {0};
	uint16_t w_addr		= 0;
	uint8_t	 w_byte		= 0;

	init_uart();
	while (1)
	{
		get_input(input);
		if (input_done == 1)
		{
			input_done = 0;
			w_addr	   = 0;
			w_byte	   = 0;
			handle_input(input, &w_addr, &w_byte);
			eeprom_read_range(0x00, 0x03ff);
		}
	}
}

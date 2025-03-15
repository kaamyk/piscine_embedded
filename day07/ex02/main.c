#include "main.h"

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

void read_value(const uint8_t *key)
{
	uint16_t addr_counter = 0;

	addr_counter = find_key(key);
	if (addr_counter == 0xffff)
		uart_printstr("empty");
	addr_counter = find_next_value(addr_counter);
	print_value(addr_counter);
}

int8_t check_value_exists(const uint8_t *key)
{
	uint16_t addr_counter = 0;

	addr_counter = find_key(key);
	if (addr_counter == 0xffff)
		return (1);
	return (0);
}

void write_key_value(const uint8_t key[17], const uint8_t val[17])
{
	uint8_t	 key_l	= ft_strlen(key);
	uint8_t	 val_l	= ft_strlen(val);
	uint16_t w_addr = find_free_space(key_l, val_l);
	if (w_addr == 0xffff)
	{
		uart_printstr("No space found ...\r\n");
		return;
	}
	uart_printstr("done\r\n");
	eeprom_write(w_addr++, START_BYTE);
	for (uint8_t i = 0; i < key_l; i++)
	{
		eeprom_write(w_addr++, key[i]);
	}
	eeprom_write(w_addr++, MID_BYTE);
	for (uint8_t i = 0; i < val_l; i++)
	{
		eeprom_write(w_addr++, val[i]);
	}
	eeprom_write(w_addr, STOP_BYTE);
}

void forget_key_value(const uint8_t *key)
{
	uint16_t addr_counter = find_key(key);
	if (addr_counter == 0xffff)
	{
		uart_printstr("Key does not exists\r\n");
		return;
	}
	addr_counter--;
	eeprom_write(addr_counter, 0x00);
	uart_printstr("done\r\n");
}

uint8_t get_cmd(const char **input, uint8_t cmd[7])
{
	uint8_t i = 0;
	for (; i < 6 && **input != '\0' && **input != ' '; i++)
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
	for (; i < 16 && **input != '\0' && **input != ' '; i++)
	{
		key[i] = **input;
		(*input)++;
	}
	return (**input != ' ');
}

uint8_t get_val(const char **input, uint8_t val[17])
{
	uint8_t i = 0;

	while (**input != ' ') (*input)++;
	(*input)++;
	for (; i < 16 && **input != '\0' && **input != ' '; i++)
	{
		val[i] = **input;
		(*input)++;
	}
	return (**input != '\0');
}

uint8_t split_input(const char input[256], uint8_t cmd[7], uint8_t key[17],
					uint8_t val[17])
{
	(void)get_cmd(&input, cmd);
	(void)get_key(&input, key);
	(void)get_val(&input, val);
	return (0);
}

uint8_t handle_input(const char input[256], uint16_t *w_addr, uint8_t *w_byte)
{
	uint8_t cmd[7]	= {0};
	uint8_t key[17] = {0};
	uint8_t val[17] = {0};

	ft_bzero(cmd);
	ft_bzero(key);
	ft_bzero(val);
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
			uart_printstr("Key: empty\r\n");
			return (1);
		}
		if (val[0] == '\0')
		{
			uart_printstr("Value: empty\r\n");
			return (1);
		}
		if (check_value_exists(key) == 1)
			write_key_value(key, val);
		else
			uart_printstr("Key already exists\r\n");
		break;
	case FORGET:
		forget_key_value(key);
		break;
	case PRINT:
		eeprom_read_range(0x0000, 0x03ff);
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

	uart_printstr("super_eeprom> ");
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
	input[input_counter] = 0;
}

int main(void)
{
	char	 input[256] = {0};
	uint16_t w_addr		= 0;
	uint8_t	 w_byte		= 0;

	init_uart();
	while (1)
	{
		ft_bzero((uint8_t *)input);
		get_input(input);
		if (input_done == 1)
		{
			input_done = 0;
			w_addr	   = 0;
			w_byte	   = 0;
			handle_input(input, &w_addr, &w_byte);
			// eeprom_read_range(0x00, 0x03ff);
		}
	}
}

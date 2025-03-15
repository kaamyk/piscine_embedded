#include "eeprom.h"

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
	EEAR = addr & 0x1ff;
	EEDR = byte;
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);
}

void eeprom_writestr(const uint16_t addr, const uint8_t *str)
{
	for (uint8_t i = 0; str[i] != 0; i++)
	{
		eeprom_write(addr + i, str[i]);
	}
}

void print_addr(const uint16_t addr)
{
	uart_printstr("0000");
	print_hex_value_16(addr);
	uart_tx(' ');
}

void print_ascii(const char *c_data)
{
	uart_printstr(" |");
	for (uint8_t j = 0; j < 16; j++)
		is_printable(c_data[j]) == 1 ? uart_tx(c_data[j]) : uart_tx(' ');
	uart_printstr("|\r\n");
}

void eeprom_read_range(const uint16_t start, const uint16_t range)
{
	uint8_t data	   = 0;
	char	c_data[17] = {0};
	int16_t real_start = start - (start % 16);

	for (uint16_t i = 0; real_start + i <= 0x03ff && real_start + i <= range;
		 i++)
	{
		if (i % 16 == 0)
			print_addr(real_start + i);
		else if ((i % 2) == 0)
			uart_tx(' ');
		data = eeprom_read(real_start + i);
		print_hex_value_8(data);
		c_data[i % 16] = data;
		if (i % 16 == 15)
			print_ascii(c_data);
	}
	uart_nl();
}

uint16_t find_free_space(const uint8_t key_l, const uint8_t val_l)
{
	uint8_t	 free_l		  = 0;
	uint16_t total_l	  = key_l + val_l + 2;
	uint16_t addr_counter = 0;
	uint16_t w_addr		  = 0;

	while (addr_counter <= 0x03ff && free_l <= total_l)
	{
		if (free_l == total_l)
		{
			return (w_addr);
		}
		else if (eeprom_read(addr_counter) == START_BYTE)
		{
			free_l = 0;
			addr_counter = find_next_stop(addr_counter);
			w_addr = ++addr_counter;
		}
		else
		{
			++addr_counter;
			++free_l;
		}
	}
	uart_printstr("Total_l == ");
	print_hex_value_16(total_l);
	uart_nl();
	uart_printstr("w_addr == ");
	print_hex_value_16(w_addr);
	uart_nl();
	return (0xffff);
}

void print_key(uint16_t addr)
{
	uint8_t c = 0;

	while (addr <= 0x3ff && c != MID_BYTE)
	{
		if (c != 0) uart_tx(c);
		c = eeprom_read(addr++);
	}
	uart_nl();
}

void print_value(uint16_t addr)
{
	uint8_t c = 0;
	
	while (addr <= 0x3ff && c != STOP_BYTE)
	{
		if (c != 0) uart_tx(c);
		c = eeprom_read(addr++);
	}
	uart_nl();
}

uint16_t find_next_key(uint16_t addr)
{
	while (addr <= 0x03fc && eeprom_read(addr++) != START_BYTE);
	return (addr);
}

uint16_t find_next_value(uint16_t addr)
{
	while (addr <= 0x03fd && eeprom_read(addr++) != MID_BYTE);
	return (addr);
}

uint16_t find_next_stop(uint16_t addr)
{
	while (addr <= 0x03fd && eeprom_read(addr) != STOP_BYTE) ++addr;
	return (addr);
}

uint16_t find_key(const uint8_t *key)
{
	uint16_t key_addr			  = 0;
	uint16_t addr_counter		  = 0;
	uint8_t c = 0;

	do
	{
		addr_counter = find_next_key(addr_counter);
		key_addr = addr_counter;
		for (uint8_t i = 0; addr_counter <= 0x03fd && i < MAX_KEY_LEN; i++)
		{
			c = eeprom_read(addr_counter++);
			if (key[i] != c && c != MID_BYTE)
				break ;
			else if (c == MID_BYTE)
				return (key_addr);
		}
		key_addr = 0xffff;
	} while (addr_counter <= 0x03fc);
	return (key_addr);
}

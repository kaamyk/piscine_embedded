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

void	eeprom_writestr( const uint16_t addr, const uint8_t *str )
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

	for (uint16_t i = 0; real_start + i <= 0x03ff && real_start + i <= range; i++)
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

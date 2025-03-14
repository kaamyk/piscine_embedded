#include "main.h"

uint8_t	eeprom_read( const uint16_t addr )
{
	while (EECR & (1 << EEPE));
	EEAR = addr & 0x3ff;
	EECR |= (1 << EERE);
	return (EEDR);
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
	
	for (uint16_t i = start; i < 0x03f0 && i < range; i++)
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


int		main( void )
{
	init_uart();
	eeprom_read_range(0x00, 0x03ff);
	while (1);
}

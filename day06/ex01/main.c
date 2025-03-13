#include "uart.h"
#include "i2c.h"

void	AHT20_init(void)
{
	i2c_start();
	i2c_write(SLA << 1);
	i2c_write(0xBE);
	i2c_stop();
	_delay_ms(10);
}

void	AHT20_request_data(void)
{
	i2c_start();
	i2c_write(SLA << 1);
	i2c_write(0xAC);
	i2c_write(0x33);
	i2c_write(0x00);
	i2c_stop();
	_delay_ms(80);
}

void	AHT20_read_data( void )
{
	uint8_t	status = 0;
	
	i2c_start();
	//	SLA + R
	i2c_write((SLA << 1) | 1);
	do {
		i2c_read();
		uart_tx(' ');
		status = TWDR;
		if (status & 0x80)
		{
			i2c_stop();
			_delay_ms(10);
			i2c_start();
			i2c_write((SLA << 1) | 1);
		}
	}while (status & 0x80);
	for (uint8_t i = 0; i < 6; i++)
	{
		i2c_read();
		uart_tx(' ');
	}
	uart_tx('\b');
	uart_nl();
	i2c_stop();
}

int main(void)
{
	init_uart();
	i2c_init();
	AHT20_init();
	while (1)
	{
		AHT20_request_data();
		AHT20_read_data();
		_delay_ms(500);
	}
}

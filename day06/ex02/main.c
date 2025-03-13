#include <stdio.h>
#include <string.h>
#include <stdio.h>

#include "uart.h"
#include "i2c.h"

void	itoa( int32_t n )
{
	if (n >= 10)
		itoa(n/10);
	uart_tx((n % 10) + '0');
}

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

void	convert( const uint8_t data[6] )
{
	int32_t humidity_raw = ((uint32_t)data[1] << 16) | ((uint32_t)data[1] << 8) | (data[3]);
	humidity_raw >>= 4;
	float	humidity = ((float)humidity_raw / (1 << 12)) * 100;
	int32_t temperature_raw = ((uint32_t)data[1]) | ((uint32_t)data[2]) | ((uint32_t)data[3]);
	float	temperature = (((float)temperature_raw / (1 << 12)) * 200) - 50;
	char	buf[256] = {0};
	uart_printstr("Temperature: ");
	temperature_raw = temperature;
	itoa(temperature_raw);
	uart_printstr(".C, Humidity: ");
	humidity_raw = humidity;
	itoa(humidity_raw);
	uart_printstr("%\n\r");
}

void	AHT20_read_data( void )
{
	uint8_t	raw_data[6] = {0};
	uint8_t	status = 0;
	
	i2c_start();
	//	SLA + R
	i2c_write((SLA << 1) | 1);
	do {
		i2c_read_ack();
		// uart_tx(' ');
		status = TWDR;
		if (status & 0x80)
		{
			i2c_stop();
			_delay_ms(10);
			i2c_start();
			i2c_write((SLA << 1) | 1);
		}
	}while (status & 0x80);
	for (uint8_t i = 0; i < 5; i++)
	{
		i2c_read_ack();
		raw_data[i] = TWDR;
		// uart_tx(' ');
	}
	i2c_read_nack();
	raw_data[5] = TWDR;
	i2c_stop();
	convert(raw_data);
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
		_delay_ms(200);
	}
}

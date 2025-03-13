#include <stdlib.h>

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

void	convert( const uint8_t data[3][7] )
{
	int32_t humidity_raw[3] = {0};
	int32_t temperature_raw[3] = {0};
	float	humidity = 0;
	float	temperature = 0;
	for (uint8_t i = 0; i < 3; i++)
	{
		humidity_raw[i] = ((uint32_t)data[i][1] << 12) | ((uint32_t)data[i][2] << 4) | (data[i][3] >> 4);
		temperature_raw[i] = (((uint32_t)data[i][3] & 0x0F) << 16) | ((uint32_t)data[i][4] << 8) | ((uint32_t)data[i][5]);
		humidity += ((float)humidity_raw[i] * 100) / 1048576.0;
        temperature += (((float)temperature_raw[i] * 200) / 1048576.0) - 50;	
	}
	humidity /= 3;
	temperature /= 3;
	char	buf[256] = {0};
	uart_printstr("Temperature: ");
	dtostrf(temperature, 6, 2, buf);
	uart_printstr(buf);
	uart_printstr(".C, Humidity: ");
	dtostrf(humidity, 6, 3, buf);
	uart_printstr(buf);
	uart_printstr("%\n\r");
}

void	AHT20_read_data( uint8_t raw_data[7] )
{
	uint8_t	status = 0;
	
	i2c_start();
	//	SLA + R
	i2c_write((SLA << 1) | 1);
	_delay_ms(80);
	do {
		i2c_start();
		i2c_write((SLA << 1) | 0);
		i2c_write(0x71);
		i2c_start();
		i2c_write((SLA << 1) | 1);
		i2c_read();
		status = TWDR;
		if (status ^ (1 << 3))
		{
			i2c_write((SLA << 1) | 0);
			i2c_write(0xbe);
			i2c_write(0x08);
			i2c_write(0x00);
			_delay_ms(10);
		}
		i2c_stop();
	} while (status & 0x80);
	
	i2c_start();
	i2c_write((SLA << 1) | 1);
	for (uint8_t i = 0; i < 6; i++)
	{
		i2c_read();
		raw_data[i] = TWDR;
	}
	i2c_read();
	raw_data[6] = TWDR;
	i2c_stop();
}

int main(void)
{
	uint8_t raw_data[3][7] = {0};
	init_uart();
	i2c_init();
	AHT20_init();
	while (1)
	{
		AHT20_request_data();
		for (uint8_t i = 0; i < 3; i++)
			AHT20_read_data(raw_data[i]);
		convert(raw_data);
		_delay_ms(1000);
	}
}

#include "main.h"

volatile int8_t g_interrupt = 0;

void pca9555_write(const uint8_t reg, const uint8_t value)
{
	i2c_start();
	i2c_write(PCA_ADDR << 1);
	i2c_write(reg);
	i2c_write(value);
	i2c_stop();
}

uint8_t pca9555_read(const uint8_t reg)
{
	i2c_start();
	i2c_write(PCA_ADDR << 1);
	i2c_write(reg);
	i2c_start();
	i2c_write(PCA_ADDR << 1 | 1);
	i2c_read();
	i2c_stop();
	return (TWDR);
}

void debounce(uint8_t button)
{
	if (pca9555_read(button) & 1)
	{
		_delay_ms(50);
		while (pca9555_read(IP0) & 1) _delay_ms(20);
	}
}

int main(void)
{
	uint8_t value = 0b11111111;
	uint8_t counter = 0;

	i2c_init();
	pca9555_write(CP0, 0b11110001);
	pca9555_write(OP0, 0xff);

	SREG |= (1 << SREG_I);

	while (1)
	{
		if (pca9555_read(IP0) & 1)
		{
			debounce(IP0);
			counter = (counter + 1) % 8;
			if (counter & 1)
				value &= D9;
			if (counter & (1 << 1))
				value &= D10;
			if (counter & (1 << 2))
				value &= D11;
			pca9555_write(OP0, value);
			g_interrupt = 0;
			value = 0xff;
		}
	}
}

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

int main(void)
{
	i2c_init();
	pca9555_write(CP0, CA4);
	pca9555_write(OP0, 0xff);
	pca9555_write(OP0, CA4);
	pca9555_write(CP1, 0b00000000);
	pca9555_write(OP1, 0x00);
	pca9555_write(OP1, TWO);

	while (1)
	{
	}
}

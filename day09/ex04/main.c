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

void ui8todisplay(const uint8_t digit_selector, const uint8_t n)
{
	uint8_t tab[10]	  = {ZERO, ONE, TWO,   THREE,  FOUR,
						 FIVE, SIX, SEVEN, HEIGHT, NINE};
	uint8_t digits[4] = {CA4, CA3, CA2, CA1};

	pca9555_write(CP0, digits[digit_selector]);
	pca9555_write(OP0, digits[digit_selector]);
	pca9555_write(CP1, 0b00000000);
	if (digit_selector == 0)
		pca9555_write(OP1, tab[n % 10]);
	else
		pca9555_write(OP1, tab[n]);
	_delay_ms(5);
	pca9555_write(OP1, 0x00);
}

int main(void)
{
	i2c_init();

	while (1)
	{
		ui8todisplay(0, 2);
		ui8todisplay(1, 4);
	}
}

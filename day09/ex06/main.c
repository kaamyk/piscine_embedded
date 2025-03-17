#include "main.h"

#define DELAY 5

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
	_delay_ms(4);
	pca9555_write(OP1, 0x00);
}

void setup_pv1(void)
{
	//	Select the ADC0
	// ADMUX &= 0b0000;
	//	Select the voltage
	ADMUX |= (1 << REFS0);

	//	Enable ADC | divider = 128
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t read_rv1(void)
{
	ADCSRA |= (1 << ADSC);
	//	Wait for the conversion to be done
	while (ADCSRA & (1 << ADSC));
	//	uin16_t can contain 10bits
	return (ADC);
}

uint8_t check_latest(const uint16_t *latest)
{
	return (latest[0] != latest[1] || latest[1] != latest[2] || latest[2] != latest[3]);
}

int main(void)
{
	uint16_t latest[4] = {0};
	uint16_t counter   = 0;

	i2c_init();
	setup_pv1();

	while (1)
	{
		ui8todisplay(0, counter % 10);
		ui8todisplay(1, (counter / 10) % 10);
		ui8todisplay(2, (counter / 100) % 10);
		ui8todisplay(3, (counter / 1000) % 10);
		latest[0] = latest[1];
		latest[1] = latest[2];
		latest[2] = latest[3];
		latest[3] = read_rv1();
		if (check_latest(latest) == 1)
			counter = latest[3];
	}
}

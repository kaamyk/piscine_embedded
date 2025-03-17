#include "main.h"

volatile int8_t g_interrupt = 0;

void define_timer_1Hz(void)
{
	// Timer1 => mode 4 (CTC), / 256, compare match
	TCCR1A |= (1 << COM1A0) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	TCNT1 = 0;
	OCR1A = 62500;
	TIMSK1 |= (1 << OCIE1A);
}

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

void ui8todisplay(const uint8_t n)
{
	uint8_t tab[10] = {ZERO, ONE, TWO,	 THREE,	 FOUR,
					   FIVE, SIX, SEVEN, HEIGHT, NINE};

	pca9555_write(CP0, CA4);
	pca9555_write(OP0, 0xff);
	pca9555_write(OP0, CA4);
	pca9555_write(CP1, 0b00000000);
	pca9555_write(OP1, 0x00);
	pca9555_write(OP1, tab[n]);
}

ISR(TIMER1_COMPA_vect) { g_interrupt = 1; }

int main(void)
{
	uint8_t counter = 0;

	i2c_init();
	define_timer_1Hz();

	SREG |= (1 << SREG_I);

	while (1)
	{
		if (g_interrupt == 1)
		{
			ui8todisplay(counter);
			counter = (counter + 1) % 10;
			g_interrupt = 0;
		}
	}
}

#include "main.h"

volatile int8_t g_interrupt = 0;

void define_timer_1Hz(void)
{
	// Timer1 => mode 4 (CTC), / 256, compare match
	TCCR1A |= (1 << COM1A0) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	TCNT1 = 0;
	OCR1A = 31250;
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

ISR(TIMER1_COMPA_vect) { g_interrupt = 1; }

int main(void)
{
	uint8_t value = 0b11111111;

	i2c_init();
	pca9555_write(CP0, 0b11110111);
	define_timer_1Hz();

	SREG |= (1 << SREG_I);

	while (1)
	{
		if (g_interrupt == 1)
		{
			value ^= (1 << 3);
			pca9555_write(OP0, value);
			g_interrupt = 0;
		}
	}
}

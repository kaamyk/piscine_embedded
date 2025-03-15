#include "rgb.h"

volatile uint8_t g_interrupt = 0;

void define_timer_4Hz(void)
{
	// Timer1 => mode 4 (CTC), / 256, compare match
	TCCR1A |= (1 << COM1A0) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	TCNT1 = 0;
	OCR1A = 18940;
	TIMSK1 |= (1 << OCIE1A);
}

void init_spi_master(void)
{
	//	Set PB3(MISO) as output
	DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
	//	Enable spi | as master | prescaler 16
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_master_transmit(char data)
{
	SPDR = data;
	//	Wait for SPIF (SPI Interrup Flag) to be set (transfer complete)
	while (!(SPSR & (1 << SPIF)));
}

void APA102_start(void)
{
	spi_master_transmit(0);
	spi_master_transmit(0);
	spi_master_transmit(0);
	spi_master_transmit(0);
}

void APA102_transmit_led(char global, char r, char g, char b)
{
	spi_master_transmit(0xe0 | (global & 0x1f));
	spi_master_transmit(b);
	spi_master_transmit(g);
	spi_master_transmit(r);
	// spi_master_transmit(0);
}

void APA102_stop(void)
{
	const uint8_t n = (3 / 2) + 1;
	for (uint8_t i = 0; i < n; i++)
	{
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
	}
}

void led_line_toggle(const uint8_t seq_counter)
{
	APA102_start();
	switch (seq_counter)
	{
	case 0:
		APA102_transmit_led(0x07, 0xff, 0xff, 0xff);
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		break;
	case 1:
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		APA102_transmit_led(0x07, 0xff, 0xff, 0xff);
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		break;
	case 2:
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		APA102_transmit_led(0x07, 0xff, 0xff, 0xff);
		break;
	case 3:
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		break;
	}
	APA102_stop();
	_delay_ms(250);
}

ISR(TIMER1_COMPA_vect) { g_interrupt = 1; }

int main(void)
{

	uint8_t seq_counter = 0;

	define_timer_4Hz();
	init_spi_master();

	SREG |= (1 << SREG_I);

	while (1)
	{
		if (g_interrupt == 1)
		{
			led_line_toggle(seq_counter);
			seq_counter = (seq_counter + 1) % 4;
			g_interrupt = 0;
		}
	}
}

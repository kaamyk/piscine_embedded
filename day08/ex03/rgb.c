#include "rgb.h"

void setup_pv1(void)
{
	//      Select the ADC0
	// ADMUX &= 0b0000;
	//      Select the voltage | Left adjust result
	ADMUX |= (1 << REFS0) | (1 << ADLAR);

	//      Enable ADC | divider = 128
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
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
	APA102_stop();
	APA102_start();
	switch (seq_counter)
	{
	case 0:
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		break;
	case 1:
		APA102_transmit_led(0x07, 0xff, 0xff, 0xff);
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		break;
	case 2:
		APA102_transmit_led(0x07, 0xff, 0xff, 0xff);
		APA102_transmit_led(0x07, 0xff, 0xff, 0xff);
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
		break;
	case 3:
		APA102_transmit_led(0x07, 0xff, 0xff, 0xff);
		APA102_transmit_led(0x07, 0xff, 0xff, 0xff);
		APA102_transmit_led(0x07, 0xff, 0xff, 0xff);
		break;
	}
}

int main(void)
{

	uint8_t value = 0;

	init_spi_master();
	setup_pv1();

	SREG |= (1 << SREG_I);
	
	while (1)
	{
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1 << ADSC));
		value = ADCH;
		led_line_toggle(value / 85);
	}
}

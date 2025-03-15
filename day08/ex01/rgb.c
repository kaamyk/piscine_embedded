#include "main.h"

void	init_spi_master(void)
{
	//	Set PB3(MISO) as output
	DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
	//	Enable spi | as master | prescaler 16
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void	spi_master_transmit(char data)
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
	spi_master_transmit(0);
}

void APA102_transmit_reset(void)
{
	spi_master_transmit(0xe0);
	spi_master_transmit(0x00);
	spi_master_transmit(0x00);
	spi_master_transmit(0x00);
	spi_master_transmit(0x00);
}

int main(void)
{
	init_spi_master();

	APA102_start();

	while (1)
	{
		APA102_transmit_led(0x0f, 0xff, 0x00, 0x00);
		APA102_transmit_reset();
		_delay_ms(1000);
		APA102_transmit_led(0x0f, 0x00, 0xff, 0x00);
		APA102_transmit_reset();
		_delay_ms(1000);
		APA102_transmit_led(0x0f, 0x00, 0x00, 0xff);
		APA102_transmit_reset();
		_delay_ms(1000);
		APA102_transmit_led(0x0f, 0xff, 0xff, 0x00);
		APA102_transmit_reset();
		_delay_ms(1000);
		APA102_transmit_led(0x0f, 0x00, 0xff, 0xff);
		APA102_transmit_reset();
		_delay_ms(1000);
		APA102_transmit_led(0x0f, 0xff, 0x00, 0xff);
		APA102_transmit_reset();
		_delay_ms(1000);
		APA102_transmit_led(0x0f, 0xff, 0xff, 0xff);
		APA102_transmit_reset();
		_delay_ms(1000);
	}
}

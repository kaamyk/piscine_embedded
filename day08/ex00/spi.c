#include "spi.h"

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

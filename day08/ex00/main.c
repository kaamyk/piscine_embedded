#include "main.h"

void APA102_start(void)
{
	spi_master_transmit(0);
	spi_master_transmit(0);
	spi_master_transmit(0);
	spi_master_transmit(0);
}

void APA102_transmit_led(char global, char b, char g, char r)
{
	spi_master_transmit(0xe0 | (global & 0x1f));
	spi_master_transmit(b);
	spi_master_transmit(g);
	spi_master_transmit(r);
	spi_master_transmit(0);
}

int		main (void)
{
	init_spi_master();
	
	APA102_start();
	APA102_transmit_led(11, 0x00, 0x00, 0xff);
	
	while (1);
}
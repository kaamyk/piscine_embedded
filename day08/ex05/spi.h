#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void	init_spi_master(void);
void	spi_master_transmit(uint8_t data);

#endif
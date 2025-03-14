#ifndef EEPROM_H
#define EEPROM_H

#include <avr/io.h>

#include "utils.h"
#include "uart.h"

uint8_t eeprom_read(const uint16_t addr);
void eeprom_write(const uint16_t addr, const uint8_t byte);
void	eeprom_writestr( const uint16_t addr, const uint8_t *str );
void print_addr(const uint16_t addr);
void eeprom_read_range(const uint16_t start, const uint16_t range);

#endif
#ifndef EEPROM_H
#define EEPROM_H

#include <avr/io.h>

#include "utils.h"
#include "uart.h"

#define START_BYTE 0x80
#define MID_BYTE 0x81
#define STOP_BYTE 0x82

#define MAX_KEY_LEN 0x16
#define MAX_VAL_LEN 0x16

uint8_t	 eeprom_read(const uint16_t addr);
void	 eeprom_write(const uint16_t addr, const uint8_t byte);
void	 eeprom_writestr(const uint16_t addr, const uint8_t *str);
void	 print_addr(const uint16_t addr);
void	 eeprom_read_range(const uint16_t start, const uint16_t range);
void	 print_key(uint16_t addr);
void	 print_value(uint16_t addr);
uint8_t find_free_space(const uint8_t key_l, const uint8_t val_l);
uint16_t find_next_value(uint16_t addr);
uint16_t find_next_key(uint16_t addr);
uint16_t find_key(const uint8_t *key);
#endif

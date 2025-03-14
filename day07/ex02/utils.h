#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <string.h>

#define BASE "0123456789abcdef"

uint8_t	 is_hexa(const char c);
uint8_t	 is_printable(const char c);
uint16_t atos_base(const uint8_t *s);
uint8_t	 atob_base(const char *s);
int	ft_strlen(const uint8_t *s);
int ft_strcmp(const uint8_t *s, const uint8_t *s1);

#endif

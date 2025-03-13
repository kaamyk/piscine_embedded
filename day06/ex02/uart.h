#ifndef	INCLUDE_H
#define	INCLUDE_H

#include <avr/io.h>
#include <stdint.h>

#define BASE "0123456789ABCDEF"

void	init_uart( void );
void	init_rgb( void );
void	uart_tx( const uint8_t c );
void	uart_printstr( const char * str );
void	print_hex_value( char c );
void	uart_nl( void );

#endif
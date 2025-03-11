#ifndef	INCLUDE_H
#define	INCLUDE_H

#include <avr/io.h>

void	init_uart( void );
void	init_rgb( void );
void	uart_tx( const uint8_t c );
void	uart_printstr( const char * str );
void	uart_putnbr_hex( const uint8_t n );
void	uart_nl( void );

#endif
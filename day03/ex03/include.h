#ifndef	INCLUDE_H
#define	INCLUDE_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

void	init_uart( void );
void	init_rgb(void);
void	uart_tx( char c );
void	uart_printstr( const char * str );

#endif
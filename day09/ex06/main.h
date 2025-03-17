#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "i2c.h"

#define PCA_ADDR 0x20
#define D9 0b1111101
#define D10 0b1111011
#define D11 0b1110111

#define CA1 0b11101111
#define CA2 0b11011111
#define CA3 0b10111111
#define CA4 0b01111111

#define ZERO 	0b00111111
#define ONE		0b00000110
#define TWO		0b01011011
#define THREE	0b01001111
#define FOUR	0b01100110
#define FIVE	0b01101101
#define SIX		0b01111101
#define SEVEN	0b00000111
#define HEIGHT	0b01111111
#define NINE	0b01101111

enum
{
	IP0,
	IP1,
	OP0,
	OP1,
	PI0,
	PI1,
	CP0,
	CP1
};

#endif

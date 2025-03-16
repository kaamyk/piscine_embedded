#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "i2c.h"

#define PCA_ADDR 0x20
#define D9	0b1111101
#define D10	0b1111011
#define D11	0b1110111

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

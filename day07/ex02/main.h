#ifndef MAIN_H
#define MAIN_H

#include "uart.h"
#include "eeprom.h"
#include "utils.h"

#include <avr/io.h>
#include <avr/eeprom.h>
#include <string.h>

enum
{
	READ,
	WRITE,
	FORGET,
	PRINT,
	FALSE
};

#endif
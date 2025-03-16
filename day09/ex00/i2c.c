#include "i2c.h"

void i2c_init(void)
{
	//	SCL = 100k
	//	SCL = CPUf / (16+2(TWBR)*(PrescValue))
	//	<=> PWBR = ((CPUf/SCL)-16)/2
	//	<=> PWBR = 72 (0x48)
	TWBR = PWBR;
	//	TWI enable
	TWCR |= (1 << TWEN);
	TWSR = 0x00;
}

void i2c_start( void )
{
	TWCR |= (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);	//	interrupt flag | Start (master) | TWI enable
	while (!(TWCR & (1 << TWINT)));						// Wait for start to be transmitted
}

void i2c_stop(void)
{
	//	interupt flag | Stop | TWI enable
	TWCR |= (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2c_read(void)
{
	TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
}

void i2c_write(const uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}
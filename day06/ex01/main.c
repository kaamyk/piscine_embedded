#include "i2c.h"
#include "uart.h"

#define SLA 0x38
#define PWBR 0x48

void i2c_init( void )
{
	//	SCL = 100k
	//	SCL = CPUf / (16+2(TWBR)*(PrescValue))
	//	<=> PWBR = ((CPUf/SCL)-16)/2
	//	<=> PWBR = 72 (0x48)
	TWBR = PWBR;
	//	TWI enable
	TWCR |= (1 << TWEN);
}

void i2c_start( void )
{
	uart_tx('\b');
	uart_tx('\r');

	TWCR |= (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);	//	interrupt flag | Start (master) | TWI enable
	while (!(TWCR & (1 << TWINT)));						// Wait for start to be transmitted
	uart_putnbr_hex(TWSR);
	uart_printstr(", ");

	i2c_write(SLA << 1);
	while (!(TWCR & (1 << TWINT)));						//	while (ack not received)
	uart_putnbr_hex(TWSR);
	uart_nl();
}

void i2c_stop( void )
{
	//	interupt flag | Stop | TWI enable
	TWCR |= (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while (TWCR & (1 << TWSTO));
}

void	i2c_read( void )
{
	uart_putnbr_hex(TWDR);
	TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
}

void	i2c_write( unsigned char data )
{
	TWDR = data;
	uart_putnbr_hex(TWDR);
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));		//	Wait for SLA+W sent and ACK received
}

void	print_hex_value( char c )
{
	for (uint8_t i = 0; i < 7; i++)
	{
		i2c_read();
		uart_tx(' ');
	}
	uart_nl();
}

int main(void)
{
	//debug
	DDRB |= (1 << DDB0) | (1 << DDB1);
	i2c_init();
	init_uart();
	while (1)
	{
		i2c_start();
		i2c_stop();
	}
}

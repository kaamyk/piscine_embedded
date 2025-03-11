#include <avr/io.h>
#include <util/twi.h>

#define SLA 0x38
#define PWBR 0x48
#define BASE "0123456789ABCDEF"

void init_uart(void)
{
	/*
	        Enable transmitter and recepter
	        Character size to 8 bit
	        Enable interuption
	*/
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (3 << UCSZ00) | (1 << RXCIE0);
	/*
	        Disable parity
	        Sets stop bit to 1
	*/
	UCSR0C &= ~(3 << UPM00) & ~(1 << USBS0);
	const uint16_t ubrr = F_CPU / (16 * UART_BAUDRATE);
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
}

void uart_tx(char c)
{
	//	while Data Register si not empty
	while (!(UCSR0A & (1 << UDRE0)))
	;
	UDR0 = c;
}

void uart_printstr(const char *str)
{
	while (*str)
	{
		uart_tx(*str);
		str++;
	}
}

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

void	i2c_write( uint8_t byte )
{
	TWDR = byte;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));		//	Wait for SLA+W sent and ACK received
}

void i2c_start( void )
{
	uart_tx('\b');
	uart_tx('\r');
	
	TWCR |= (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);	//	interrupt flag | Start (master) | TWI enable
	while (!(TWCR & (1 << TWINT)));						// Wait for start to be transmitted
	uart_tx(BASE[((TWSR & (0xF8)) >> 4) % 16]);
	uart_tx(BASE[(TWSR & (0xF8)) % 16]);
	
	uart_tx(',');
	uart_tx(' ');
	
	i2c_write(SLA << 1);
	while (!(TWCR & (1 << TWINT)));						//	while (ack not received)
	uart_tx(BASE[((TWSR & (0xF8)) >> 4) % 16]);
	uart_tx(BASE[(TWSR & (0xF8)) % 16]);
	
	uart_tx('\n');
	uart_tx('\r');
}

void i2c_stop(void)
{
	//	interupt flag | Stop | TWI enable
	TWCR |= (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while (TWCR & (1 << TWSTO));
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

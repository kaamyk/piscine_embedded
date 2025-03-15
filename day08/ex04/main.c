#include "main.h"

char	input[10]	  = {0};
uint8_t input_counter = 0;
uint8_t input_done	  = 1;

uint8_t leds[3][3] = {0};

uint8_t atob_base(const char *s)
{
	uint16_t first_digit  = strchr(BASE, s[0]) - BASE;
	uint16_t second_digit = strchr(BASE, s[1]) - BASE;

	return (((first_digit & 0x0F) << 4) | (second_digit & 0x0F));
}

void APA102_start(void)
{
	spi_master_transmit(0);
	spi_master_transmit(0);
	spi_master_transmit(0);
	spi_master_transmit(0);
}

void APA102_transmit_led(char global, char b, char g, char r)
{
	spi_master_transmit(0xe0 | (global & 0x1f));
	spi_master_transmit(b);
	spi_master_transmit(g);
	spi_master_transmit(r);
	spi_master_transmit(0);
}

void APA102_stop(void)
{
	const uint8_t n = (3 / 2) + 1;
	for (uint8_t i = 0; i < n; i++)
	{
		APA102_transmit_led(0x00, 0x00, 0x00, 0x00);
	}
}

void led_line_toggle(const uint8_t r, const uint8_t g, const uint8_t b,
					 const uint8_t led)
{
	// PORTB ^= (1 << PB0);
	print_hex_value_8(led);
	uart_nl();
	APA102_stop();
	leds[led][0] = r;
	leds[led][1] = g;
	leds[led][2] = b;
	APA102_start();
	APA102_transmit_led(0x07, leds[0][0], leds[0][1], leds[0][2]);
	APA102_transmit_led(0x07, leds[1][0], leds[1][1], leds[1][2]);
	APA102_transmit_led(0x07, leds[2][0], leds[2][1], leds[2][2]);
}

void handle_input(void)
{
	if (input[0] != '#')
	{
		uart_printstr("Missing \'#\'\r\n");
		return;
	}
	if (input[7] != 'D')
	{
		uart_printstr("Input format invalid\r\n");
		return;
	}
	led_line_toggle(atob_base(input + 1), atob_base(input + 3),
					atob_base(input + 5), input[8] - '6');
}

ISR(USART_RX_vect)
{
	char c = UDR0;
	if (c == '\n' || c == '\r')
	{
		uart_nl();
		input_done = 1;
		handle_input();
		memset(input, 0, 10);
		input_counter = 0;
	}
	else if (c == '\b' || c == 127)
	{
		if (input_counter > 0)
		{
			uart_printstr("\b \b");
			input_counter--;
		}
	}
	else if ((input_counter == 0 && c == '#') || (c >= 'A' && c <= 'F') ||
			 (c >= '0' || c <= '9'))
	{
		if (input_counter < 9)
		{
			uart_tx(c);
			input[input_counter++] = c;
		}
	}
}

int main(void)
{
	// debug
	DDRB |= (1 << DDB0);

	init_spi_master();
	init_uart();
	APA102_start();

	SREG |= (1 << SREG_I);

	while (1);
}
